@echo off
setlocal

:: Set the target directories
set "target_dir_arduino=C:\Program Files (x86)\VitalBox"
set "target_dir_startup=%AppData%\Microsoft\Windows\Start Menu\Programs\Startup"

:: Delete files from ArduinoSysInfoBox
del "%source_dir%\main.cpp" "%target_dir_arduino%"
del "%source_dir%\watchdog.cpp" "%target_dir_arduino%"
del "%source_dir%\start.cpp" "%target_dir_arduino%"
del "%source_dir%\main.exe" "%target_dir_arduino%"
del "%source_dir%\watchdog.exe" "%target_dir_arduino%"
del "%source_dir%\start.exe" "%target_dir_arduino%"
del "%source_dir%\serial" "%target_dir_arduino%"


:: Remove the ArduinoSysInfoBox directory if it's empty
rd "%target_dir_arduino%" 2>nul

:: Delete run_script.vbs from the Startup directory
del "%target_dir_startup%\PC-Vital-Box.lnk"

echo Files and folders have been deleted successfully!
pause