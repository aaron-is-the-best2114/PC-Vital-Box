#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <thread>
#include <chrono>
#include "serial/serial.h"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void strip_newline(std::string& str) {
    // Find and remove any newline character from the string
    size_t pos = str.find('\n');
    if (pos != std::string::npos) {
        str.erase(pos);
    }
}

// Struct to hold data from PowerShell commands
struct SystemData {
    std::string GPU_temp;
    std::string CPU_temp;
    std::string cpu_util;
    std::string cpu_freq;
    std::string memory;
    std::string gpu_util;
    std::string gpu_power;
};

// Function to gather all data concurrently
void gather_data(SystemData &data) {
    std::thread t1([&data]() { data.GPU_temp = exec("PowerShell.exe -Command \"nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits\""); });
    std::thread t2([&data]() { data.CPU_temp = exec("PowerShell.exe -Command \"Get-CimInstance -Namespace root/cimv2 -ClassName Win32_PerfFormattedData_Counters_ThermalZoneInformation | Select-Object -ExpandProperty Temperature\""); });
    std::thread t3([&data]() { data.cpu_util = exec("PowerShell.exe -Command \"$cpuUsage = Get-Counter '\\Processor Information(_Total)\\% Processor Utility' | Select-Object -ExpandProperty CounterSamples | Select-Object -ExpandProperty CookedValue; [math]::Round($cpuUsage)\""); });
    std::thread t4([&data]() { data.cpu_freq = exec("PowerShell.exe -Command \"$MaxClockSpeed = (Get-CimInstance CIM_Processor).MaxClockSpeed; $ProcessorPerformance = (Get-Counter -Counter '\\Processor Information(_Total)\\% Processor Performance').CounterSamples.CookedValue; $CurrentClockSpeed = $MaxClockSpeed * ($ProcessorPerformance / 100); $FormattedClockSpeed = '{0:N2}' -f ($CurrentClockSpeed / 1000); Write-Host $FormattedClockSpeed\""); });
    std::thread t5([&data]() { data.memory = exec("PowerShell.exe -Command \"$os = Get-WmiObject -Class Win32_OperatingSystem; ' {0:N2}GB/{1:N2}GB' -f (($os.TotalVisibleMemorySize - $os.FreePhysicalMemory) / 1MB), ($os.TotalVisibleMemorySize / 1MB)\""); });
    std::thread t6([&data]() { data.gpu_util = exec("PowerShell.exe -Command \"nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits\""); });
    std::thread t7([&data]() { data.gpu_power = exec("PowerShell.exe -Command \"nvidia-smi --query-gpu=power.draw --format=csv,noheader,nounits | ForEach-Object { [math]::Round([double]$_) }\""); });

    // Wait for all threads to finish
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
}

int main(int argc, char* argv[]) {
    auto ports = serial::list_ports(); // Fetch the list of available ports
    std::cout << "Available Ports: " << std::endl;

    for (auto port : ports) {
        std::cout << port.port << std::endl; // Output the port name
    }

    std::cout << std::endl;

    serial::Serial my_serial("COM13", 115200, serial::Timeout::simpleTimeout(250));

    while (true) {  // Continuous loop to send data
        SystemData data;

        // Gather data concurrently
        gather_data(data);

        // Convert the string output to an integer (Kelvin)
        int CPU_temp_kelvin = std::stoi(data.CPU_temp);

        // Convert from Kelvin to Celsius
        int CPU_temp_celsius = CPU_temp_kelvin - 273;

        strip_newline(data.GPU_temp);
        strip_newline(data.cpu_util);
        strip_newline(data.gpu_util);

        std::string temp = "TEMP:" + data.GPU_temp + ":" + std::to_string(CPU_temp_celsius);  // Message to send
        my_serial.write(temp);  // Send the message
        std::cout << temp << std::endl;

        std::string cpu = "\nUTIL:" + data.cpu_util + ":" + data.cpu_freq;  // Message to send
        my_serial.write(cpu);  // Send the message
        std::cout << cpu << std::endl;

        std::string mem = "\nMEM:" + data.memory;  // Message to send
        my_serial.write(mem);  // Send the message
        std::cout << mem << std::endl;

        std::string gpu = "\nGPU:" + data.gpu_util + ":" + data.gpu_power;  // Message to send
        my_serial.write(gpu);  // Send the message
        std::cout << gpu << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second before sending again
    }

    my_serial.close();

    return 0;
}