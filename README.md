# PC Vital Box

## Overview
PC Vital Box is a software application designed to monitor and display system information such as GPU temperature, CPU temperature, CPU utilization, CPU frequency, Memory Usage, GPU utilization, and GPU power on an Arduino display. The software consists of a C++ Script (`main.cpp`) that collects system information and sends it to an Arduino, and a watchdog script (`watchdog.cpp`) that ensures the main script runs continuously by restarting it periodically and handling crashes.

## Components Used
The PC Vital Box physical product includes the following components:
- **Arduino**
- **I2C LCD1602 Display**
- **Two buttons**:
  - **Top button**: Resets the Arduino.
  - **Front button**: Changes the type of information shown on the LCD.
- **A switch**: Disconnects power to the LCD. After flipping the switch and turning it back on, you need to press the top reset button for the display to work again.

## Installation
To install PC Vital Box:

### Download and Run the Installer
1. Download the `PCVitalBox Installer.exe` file.
2. Run the installer. It will move all necessary files to the appropriate directories and set up the application to start automatically with Windows.

## Uninstalling the Program
To uninstall PC Vital Box:

1. **Navigate to the Installation Directory**
   - Open File Explorer and go to `C:\Program Files (x86)\VitalBox`.

2. **Run the Uninstaller**
   - In the installation directory, locate and run the `uninstall.bat` file.
   - This script will remove all files associated with PC Vital Box and clean up the installation directory.

If you have trouble uninstalling the program or need further assistance, please create an issue on our GitHub repository: [PC-Vital-Box](https://github.com/aaron-is-the-best2114/PC-Vital-Box).

## How It Works

### `PcTemp.py`
This script monitors various system metrics and communicates with the Arduino.

- **Initialization**:
  - The script attempts to initialize a serial connection with the Arduino on COM3 at a baud rate of 115200. If initialization fails, the script exits with an error code.

- **Data Collection**:
  - The script runs PowerShell commands to collect data about GPU temperature, CPU temperature, CPU usage, CPU frequency, memory usage, GPU utilization, and GPU power.
  - These commands are executed concurrently using `ThreadPoolExecutor` to improve performance.

- **Data Transmission**:
  - The collected data is formatted and sent to the Arduino via serial communication.

- **Error Handling**:
  - If any error occurs during data collection or transmission, the script exits with an error code, triggering the watchdog to restart it.

### `watchdog.py`
This script ensures that `PcTemp.py` is always running and handles any crashes.

- **Script Monitoring**:
  - The watchdog script starts `PcTemp.py` and monitors it.
  - If the script crashes or exits with an error code, or if it runs for longer than 5 minutes, the watchdog restarts it.

### `run_script.bat`
A batch file used to run `watchdog.py` from the command line.

### `run_script.vbs`
A VBScript file that runs `run_script.bat` silently on startup. This ensures that the watchdog script starts automatically when Windows starts.

### `install.bat`
A batch file used for installing the application. It moves the necessary files to the installation directory and sets up the application to run on startup.

### `delete_install.bat`
A batch file that deletes itself after running.

### `uninstall.bat`
A batch file used for uninstalling the application. It removes all files associated with PC Vital Box and cleans up the installation directory.

## Running the Application
After installation, the application will automatically start with Windows. You can check the Arduino display to see the system information being updated in real-time.

## Troubleshooting
If you encounter any issues or need assistance, please create an issue on our GitHub repository: [PC-Vital-Box](https://github.com/aaron-is-the-best2114/PC-Vital-Box). Provide details about the problem you're facing, and we will help you resolve it as quickly as possible.

## License
This software is open-source. Please refer to the included license file for more details.

---
