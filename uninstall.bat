@echo off
setlocal

:: Set the target directories
set "target_dir_arduino=C:\Program Files\ArduinoSysInfoBox"
set "target_dir_startup=%AppData%\Microsoft\Windows\Start Menu\Programs\Startup"

:: Delete files from ArduinoSysInfoBox
del "%target_dir_arduino%\PcTemp.py"
del "%target_dir_arduino%\watchdog.py"
del "%target_dir_arduino%\run_script.bat"

:: Remove the ArduinoSysInfoBox directory if it's empty
rd "%target_dir_arduino%" 2>nul

:: Delete run_script.vbs from the Startup directory
del "%target_dir_startup%\run_script.vbs"

echo Files and folders have been deleted successfully!
pause