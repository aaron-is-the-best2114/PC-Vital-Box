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
import time
import sys

def start_script():
    """Starts the main script and returns the process object."""
    return subprocess.Popen([sys.executable, 'PcTemp.py'])

def monitor_script():
    """Monitors the main script, restarts it periodically, and handles crashes."""
    while True:
        process = start_script()
        print(f"Started script with PID {process.pid}")

        # Monitor the script and restart it every 5 minutes or if it exits with an error
        while True:
            try:
                # Wait for the script to finish or timeout after 5 minutes
                process.wait(timeout=300)  # 300 seconds = 5 minutes
                if process.returncode != 0:
                    print(f"Script exited with error code {process.returncode}. Restarting...")
                    break  # Restart script
            except subprocess.TimeoutExpired:
                # Kill the process if it's running too long and restart
                print("Restarting script due to timeout...")
                process.terminate()  # Terminate the script
                process.wait()  # Wait for the termination
                break  # Restart script

if __name__ == "__main__":
    monitor_script()