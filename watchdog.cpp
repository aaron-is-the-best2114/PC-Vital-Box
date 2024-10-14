#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> // For system()
#include <csignal> // For kill()
#include <sys/types.h>
#include <windows.h> // For Windows-specific functions
#include <stdexcept>

HANDLE start_script() {
    // Start the main.exe process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create the process
    if (!CreateProcess(NULL,   // No module name (use command line)
        (LPSTR)"main.exe",     // Command line to run main.exe
        NULL,                  // Process handle not inheritable
        NULL,                  // Thread handle not inheritable
        FALSE,                 // Set handle inheritance to FALSE
        0,                     // No creation flags
        NULL,                  // Use parent's environment block
        NULL,                  // Use parent's starting directory 
        &si,                   // Pointer to STARTUPINFO structure
        &pi)) {                // Pointer to PROCESS_INFORMATION structure
        throw std::runtime_error("Failed to start main.exe");
    }

    std::cout << "Started main.exe with PID " << pi.dwProcessId << std::endl;
    return pi.hProcess;  // Return the process handle
}

void monitor_script() {
    while (true) {
        HANDLE process = start_script();

        // Monitor the process
        while (true) {
            // Wait for the process to finish or timeout after 5 minutes (300 seconds)
            DWORD wait_result = WaitForSingleObject(process, 300000); // 300,000 milliseconds = 5 minutes

            if (wait_result == WAIT_TIMEOUT) {
                // If the process took too long, terminate and restart it
                std::cout << "Restarting main.exe due to timeout..." << std::endl;
                TerminateProcess(process, 0); // Terminate the process
                CloseHandle(process);         // Close the handle
                break;                        // Restart the process
            } 
            else if (wait_result == WAIT_OBJECT_0) {
                // Process has exited, check for exit code
                DWORD exit_code;
                GetExitCodeProcess(process, &exit_code);
                if (exit_code != 0) {
                    std::cout << "main.exe exited with error code " << exit_code << ". Restarting..." << std::endl;
                } else {
                    std::cout << "main.exe exited successfully." << std::endl;
                }
                CloseHandle(process); // Close the process handle
                break;                // Restart the process regardless of the exit code
            }
        }

        // Wait for a short period before restarting
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    try {
        monitor_script();  // Start monitoring the script
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}