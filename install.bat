@echo off
setlocal

:: Set the source directory to the directory where this batch file is located
set "source_dir=%~dp0"

:: Set the target directories
set "target_dir_arduino=C:\Program Files\ArduinoSysInfoBox"
set "target_dir_startup=%AppData%\Microsoft\Windows\Start Menu\Programs\Startup"

:: Create the target directory if it doesn't exist
if not exist "%target_dir_arduino%" mkdir "%target_dir_arduino%"

:: Move PcTemp.py, watchdog.py, and run_script.bat to ArduinoSysInfoBox
move "%source_dir%\PcTemp.py" "%target_dir_arduino%"
move "%source_dir%\watchdog.py" "%target_dir_arduino%"
move "%source_dir%\run_script.bat" "%target_dir_arduino%"

:: Move run_script.vbs to the Startup directory
move "%source_dir%\run_script.vbs" "%target_dir_startup%"

echo Files have been moved successfully!
pause