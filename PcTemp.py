# MIT License
# 
# Copyright (c) 2024 Aaron
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so.
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

import subprocess
import serial
import time
import sys
import os
from concurrent.futures import ThreadPoolExecutor

def initialize_serial():
    try:
        ser = serial.Serial("COM3", 115200, timeout=1)
        ser.setDTR(False)
        time.sleep(0.5)
        ser.flushInput()
        ser.setDTR(True)
        time.sleep(1)
        return ser
    except serial.SerialException as e:
        print(f"Failed to initialize serial: {e}")
        sys.exit(1)  # Exit with error code

def send_data(ser, data):
    try:
        ser.write(data.encode())
    except Exception as e:
        print(f"Failed to send data: {e}")
        sys.exit(1)  # Exit with error code

def run_powershell_command(command):
    try:
        process = subprocess.Popen(['powershell', '-command', command], stdout=subprocess.PIPE)
        output = process.communicate()[0].decode().strip()
        return output
    except Exception as e:
        print(f"Failed to run command '{command}': {e}")
        sys.exit(1)  # Exit with error code

def get_gpu_type():
    command = 'wmic path win32_VideoController get name'
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    return result.stdout.strip()

def main():
    ser = initialize_serial()
    
    gpu_type = get_gpu_type()
    
    if 'NVIDIA' in gpu_type:
        print("Detected NVIDIA GPU")
        commands = {
            "GPU_temp": 'nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits',
            "GPU_Usage": "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits",
            "GPU_Power": "nvidia-smi --query-gpu=power.draw --format=csv,noheader,nounits | ForEach-Object { [math]::Round([double]$_) }"
        }
    elif 'AMD' in gpu_type or 'Radeon' in gpu_type:
        print("Detected AMD GPU")
        commands = {
            "GPU_temp": 'New-HWCustomObject -CustomObjectName "HARDWAREMONITOR"; $HARDWAREMONITOR.EnableGPU = $true; $HARDWAREMONITOR.GetMeasurementValues().Value',
            "GPU_Usage": 'Write-Output "$([math]::Round((((Get-Counter "\\GPU Engine(*engtype_3D)\\Utilization Percentage").CounterSamples | where CookedValue).CookedValue | measure -sum).sum,2))"',
        }
    else:
        print("No dedicated GPU detected or GPU is not from NVIDIA or AMD")

    uptime = 0
    
    while True:
        try:
            commands.update({
                "CPU_temp": 'Get-CimInstance -Namespace root/cimv2 -ClassName Win32_PerfFormattedData_Counters_ThermalZoneInformation | Select-Object -ExpandProperty Temperature',
                "CPU_Usage": "$cpuUsage = Get-Counter '\\Processor Information(_Total)\\% Processor Utility' | Select-Object -ExpandProperty CounterSamples | Select-Object -ExpandProperty CookedValue; [math]::Round($cpuUsage)",
                "CPU_Freq": "$MaxClockSpeed = (Get-CimInstance CIM_Processor).MaxClockSpeed; $ProcessorPerformance = (Get-Counter -Counter '\\Processor Information(_Total)\\% Processor Performance').CounterSamples.CookedValue; $CurrentClockSpeed = $MaxClockSpeed * ($ProcessorPerformance / 100); $FormattedClockSpeed = '{0:N2}' -f ($CurrentClockSpeed / 1000); Write-Host $FormattedClockSpeed",
                "Mem_Usage": "$os = Get-WmiObject -Class Win32_OperatingSystem; ' {0:N2}GB/{1:N2}GB' -f (($os.TotalVisibleMemorySize - $os.FreePhysicalMemory) / 1MB), ($os.TotalVisibleMemorySize / 1MB)"
            })

            with ThreadPoolExecutor() as executor:
                futures = {executor.submit(run_powershell_command, cmd): name for name, cmd in commands.items()}
                results = {name: future.result() for future, name in futures.items()}

            if 'NVIDIA' in gpu_type:
                GPU = results["GPU_temp"]
                GPU_Util = results["GPU_Usage"]
                GPU_Power = results["GPU_Power"]
            elif 'AMD' in gpu_type or 'Radeon' in gpu_type:
                gpu_temp_output = results["GPU_temp"]
                filtered_output = '\n'.join(line for line in gpu_temp_output.split('\n') if "Set the Measurement Components" not in line and "Default are CPU:Enabled, GPU:Disabled" not in line)
                temp_values = filtered_output.split()
                first_temp_value = temp_values[0].strip()
                GPU = first_temp_value
                GPU_Util = results["GPU_Usage"]
                GPU_Power = "N/A"
            elif 'AMD' not in gpu_type or 'Radeon' not in gpu_type or 'NVIDIA' not in gpu_type:
                GPU = results["CPU_temp"]
                GPU_Util = results["CPU_Usage"]
                GPU_Power = "N/A"

            try:
                CPU = int(float(results["CPU_temp"]) - 273.15)  # Convert from Kelvin to Celsius
            except ValueError as e:
                print(f"Error converting CPU temperature: {e}")
                cpu_temp_output = results["CPU_temp"]
                temp_values = cpu_temp_output.split()
                first_temp_value = temp_values[0].strip()
                CPU = int(float(first_temp_value) - 273.15)  # Convert from Kelvin to Celsius

            CPU_Util = results["CPU_Usage"]
            CPU_Freq = results["CPU_Freq"]
            Mem_Usage = results["Mem_Usage"]

            send_data(ser, f"TEMP:{GPU}:{CPU}\n")
            send_data(ser, f"UTIL:{CPU_Util}:{CPU_Freq}\n")
            send_data(ser, f"MEM:{Mem_Usage}\n")
            send_data(ser, f"GPU:{GPU_Util}:{GPU_Power}\n")
            
            uptime = uptime + 1
            print(uptime)

        except Exception as e:
            print(f"An error occurred: {e}")
            sys.exit(1)  # Exit with error code to trigger the watchdog

if __name__ == "__main__":
    main()
