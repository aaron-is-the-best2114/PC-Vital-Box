@echo off
setlocal

:: Set the source directory to the directory where this batch file is located
set "source_dir=%~dp0"

:: Set the target directories
set "target_dir_arduino=C:\Program Files (x86)\VitalBox"
set "target_dir_startup=%AppData%\Microsoft\Windows\Start Menu\Programs\Startup"

:: Create the target directory if it doesn't exist
if not exist "%target_dir_arduino%" mkdir "%target_dir_arduino%"

move "%source_dir%\main.cpp" "%target_dir_arduino%"
move "%source_dir%\watchdog.cpp" "%target_dir_arduino%"
move "%source_dir%\start.cpp" "%target_dir_arduino%"
move "%source_dir%\main.exe" "%target_dir_arduino%"
move "%source_dir%\watchdog.exe" "%target_dir_arduino%"
move "%source_dir%\start.exe" "%target_dir_arduino%"
move "%source_dir%\serial" "%target_dir_arduino%"

:: Move PC-Vital-Box.lnk to the Startup directory
move "%source_dir%\PC-Vital-Box.lnk" "%target_dir_startup%"

echo Files have been moved successfully!
pause