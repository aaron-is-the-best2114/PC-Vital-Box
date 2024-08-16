Set WshShell = CreateObject("WScript.Shell")
WshShell.Run chr(34) & "C:\Program Files\ArduinoSysInfoBox\run_script.bat" & Chr(34), 0
Set WshShell = Nothing