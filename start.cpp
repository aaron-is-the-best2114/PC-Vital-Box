#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <fstream>
#include <memory>
#include <array>

// Function to convert a wide string (wchar_t) to a narrow string (char)
std::string convertWideToNarrow(const std::wstring& wideStr) {
    int narrowStrSize = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string narrowStr(narrowStrSize, 0);
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &narrowStr[0], narrowStrSize, nullptr, nullptr);
    return narrowStr;
}

// Function to check if the Arduino is plugged into COM13
bool isArduinoPluggedIn() {
    std::wstring portName = L"\\\\.\\COM13";
    HANDLE hSerial = CreateFileW(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial); // If the handle is valid, close it
        return true; // Arduino is plugged in
    } else {
        return false; // Arduino is not plugged in
    }
}

// Function to start the main watchdog program (main.exe)
void startWatchdog() {
    std::wstring command = L"main.exe"; // Use the name of your main watchdog program
    STARTUPINFOA si = { sizeof(si) };  // Change to STARTUPINFOA for narrow characters
    PROCESS_INFORMATION pi;

    std::string narrowCmd = convertWideToNarrow(command); // Convert the wide string to narrow string

    if (!CreateProcessA(nullptr, &narrowCmd[0], nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to start main.exe. Error: " << GetLastError() << std::endl;
        return;
    }

    // Wait until the process has finished
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    bool arduinoWasPluggedIn = false;
    while (true) {
        bool arduinoPluggedIn = isArduinoPluggedIn();
        
        if (arduinoPluggedIn && !arduinoWasPluggedIn) {
            std::cout << "Arduino plugged in, starting the watchdog program..." << std::endl;
            std::thread watchdogThread(startWatchdog); // Start the watchdog in a new thread
            watchdogThread.detach(); // Detach to allow it to run independently
            arduinoWasPluggedIn = true;
        } else if (!arduinoPluggedIn && arduinoWasPluggedIn) {
            std::cout << "Arduino unplugged, stopping the watchdog program..." << std::endl;
            // Add code to stop the watchdog if needed
            arduinoWasPluggedIn = false;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2)); // Check every 2 seconds
    }

    return 0;
}