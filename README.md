# PC Vital Box

## Overview
PC Vital Box is a software application designed to monitor and display system information such as GPU temperature, CPU temperature, CPU utilization, CPU frequency, Memory Usage, GPU utilization, and GPU power on an Arduino display. The software consists of a C++ Script (`main.cpp`) that collects system information and sends it to an Arduino, and a watchdog script (`watchdog.cpp`) that ensures the main script runs continuously by restarting it periodically and handling crashes.

## Components Used
The PC Vital Box physical product includes the following components:
- **Arduino**
- **2 I2C LCD1602 Displays**
- **Two buttons**:
  - **Reset button**: Resets the Arduino.
  - **Menu button**: Changes the type of information shown on the LCD.
- **A switch**: Disconnects power to the Arduino.

## Installation
To install PC Vital Box:

### Download and Run the Installer (**ENSURE YOU READ ANY AND ALL INSTRUCTIONS**)
1. Go to https://www.msys2.org/ and download and run the installer.
2. Before you finish with the installer, make sure the `Run MSYS2 Now.` box is checked. It should open the UCRT64 environment.
3. Inside the enviornment run this line of code: `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
4. When that is complete, close the environment and open either Windows Terminal, Windows PowerShell, or Command Prompt and type `g++ --version` to ensure it was installed correctly.
5. Then Download the `PCVitalBox Installer.exe` file from the releases page on this repository.
6. Run the installer. It will move all necessary files to the appropriate directories and set up the application to start automatically with Windows.

## Uninstalling the Program
To uninstall PC Vital Box:

1. **Navigate to the Installation Directory**
   - Open File Explorer and go to `C:\Program Files (x86)\VitalBox`.

2. **Run the Uninstaller**
   - In the installation directory, locate and run the `uninstall.bat` file.
   - This script will remove all files associated with PC Vital Box and clean up the installation directory.

If you have trouble uninstalling the program or need further assistance, please create an issue on this GitHub repository: [PC-Vital-Box](https://github.com/aaron-is-the-best2114/PC-Vital-Box).

## How It Works

### `Comming Soon`

## Running the Application
After installation, the application will automatically start with Windows. You can check the Arduino display to see the system information being updated in real-time.

## Troubleshooting
If you encounter any issues or need assistance, please create an issue on our GitHub repository: [PC-Vital-Box](https://github.com/aaron-is-the-best2114/PC-Vital-Box). Provide details about the problem you're facing, and we will help you resolve it as quickly as possible.

## License
This software is open-source. Please refer to the included license file for more details.

---
