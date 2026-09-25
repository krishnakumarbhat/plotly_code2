#!/usr/bin/env python3

import os
import sys
import re
import subprocess
import shutil
import platform
from pathlib import Path
import concurrent.futures

class Colors:
    BLUE = '\033[34m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    RED = '\033[31m'
    RESET = '\033[0m'

IS_WINDOWS = platform.system() == 'Windows'
PYTHON_CMD = "python" if IS_WINDOWS else "python3"

# Shared with BuildForAllCustomer.py
CUSTOMER_PARAMS = {
    "CEER": 5,   # Note: For CEER P600 variant only io should be 3 else 5
    "GPO_Gen8": 5,
    "AL": 4,
}

def enable_windows_ansi_colors():
    if IS_WINDOWS:
        try:
            import ctypes
            kernel32 = ctypes.windll.kernel32
            kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7)
        except:
            pass



def read_customer_map(map_path):
    customer_map = {}
    
    try:
        with open(map_path, 'r') as f:
            content = f.read()
            
        map_pattern = re.compile(r'const\s+unordered_map\s*<\s*int\s*,\s*string\s*>\s*CustomerName\s*=\s*\{(.*?)\};', re.DOTALL)
        map_match = map_pattern.search(content)
        
        if not map_match:
            raise ValueError("Could not find CustomerName map in LogicModel2.h")
            
        map_content = map_match.group(1)
        
        # Parse each entry in the map
        entry_pattern = re.compile(r'\{\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*"([^"]+)"\s*\}')
        entries = entry_pattern.finditer(map_content)
        
        for entry in entries:
            # Convert hex to integer
            id_str = entry.group(1)
            if id_str.startswith('0x'):
                customer_id = int(id_str, 16)
            else:
                customer_id = int(id_str)
                
            customer_name = entry.group(2)
            customer_map[customer_name] = customer_id
        
    except Exception as e:
        print(f"Error reading customer map from {map_path}: {e}")
        
    return customer_map

def run_command(command, check=True, capture_output=True, shell=True):
    try:
        if capture_output:
            result = subprocess.run(
                command, 
                shell=shell, 
                check=check, 
                text=True,
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE
            )
            return result.stdout
        else:
            subprocess.run(command, shell=shell, check=check)
            return ""
    except subprocess.CalledProcessError as e:
        print(f"Command failed: {e}")
        if hasattr(e, 'stderr') and e.stderr:
            print(f"Error output: {e.stderr}")
        if check:
            sys.exit(1)
        return None

def check_python():
    try:
        python_cmd = "python --version" if IS_WINDOWS else "python --version 2>&1 || python --version 2>&1"
        version = run_command(python_cmd).strip()
        return True
    except Exception:
        print("Python is not installed.")
        return False

def remove_directory_safe(directory):
    """Safely remove directory using appropriate method for OS"""
    if not os.path.exists(directory):
        return True
    
    max_retries = 3
    for attempt in range(max_retries):
        try:
            if IS_WINDOWS:
                # Windows: use shutil.rmtree
                shutil.rmtree(directory, ignore_errors=False)
            else:
                # Linux/Mac: use rm -rf which is more reliable for NFS and open file handles
                result = subprocess.run(f'rm -rf "{directory}"', shell=True, check=True, capture_output=True, text=True)
            return True
        except Exception as e:
            if attempt < max_retries - 1:
                print(f"Attempt {attempt + 1}: Failed to remove {directory}, retrying...")
                import time
                time.sleep(1)
            else:
                print(f"Warning: Could not remove {directory}: {e}")
                return False
    return False

def setup_build_environment():
    """Setup clean build environment"""
    build_dir = 'build'
    remove_directory_safe(build_dir)
    os.makedirs(build_dir, exist_ok=True)
    os.chdir(build_dir)

def handle_customer_selection():
    customers = list(CUSTOMER_PARAMS.keys())

    print("Select Customer:")
    for idx, name in enumerate(customers, start=1):
        print(f"{idx}. {name}")

    customer_choice = input(": ")

    try:
        index = int(customer_choice) - 1
        if index < 0 or index >= len(customers):
            raise ValueError
    except ValueError:
        print("Invalid Customer")
        sys.exit(1)

    customer_name = customers[index]
    return customer_name, CUSTOMER_PARAMS[customer_name]

def main():
    enable_windows_ansi_colors()
    
    print(f"Detected operating system: {platform.system()}")
    
    map_path = os.path.normpath('Code/LogicModel2.h')
    customer_map = read_customer_map(map_path)
    
    if not customer_map:
        print("Error: No customers found in LogicModel2.h")
        sys.exit(1)
    
    setup_build_environment()
    customer_name, input_output = handle_customer_selection()
    
    # Look up the customer ID from the map
    if customer_name not in customer_map:
        print(f"Error: Customer '{customer_name}' not found in LogicModel2.h map")
        sys.exit(1)
        
    customer_id = customer_map[customer_name]
    
    with concurrent.futures.ThreadPoolExecutor() as executor:
        python_future = executor.submit(check_python)
        
        print("Select build type:")
        print("1. Release")
        print("2. Debug")
        build_type_choice = input(": ")
        
        if build_type_choice not in ["1", "2"]:
            print("Invalid build type")
            sys.exit(1)
        
        if not python_future.result():
            print("Python is required but not found.")
            sys.exit(1)
    
    try:
        run_command(f"{PYTHON_CMD} ../UpdateModelDescription.py {input_output}", shell=True)
    except Exception as e:
        print(f"Failed to run UpdateModelDescription.py: {e}")
        sys.exit(1)
    
    build_type = "Release" if build_type_choice == "1" else "Debug"
    
    build_type_dir = os.path.join(os.getcwd(), build_type)
    remove_directory_safe(build_type_dir)
    os.makedirs(build_type_dir, exist_ok=True)
    os.chdir(build_type_dir)
    
    cmake_args = [
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-DCUSTOMER_ID={customer_id}",
        f"-DCUSTOMER={customer_name}",
        f"-DNUM_OUTPUTS={input_output}",
        f"-DNUM_INPUTS={input_output}",
        "-DCMAKE_POLICY_VERSION_MINIMUM=3.5",
        "-Wno-deprecated"
    ]
    
    if build_type_choice == "1":
        cmake_args.append("-DCUSTOMER_FW=0")
    
    if IS_WINDOWS and len(sys.argv) > 1:
        cmake_args.insert(0, f"-G {sys.argv[1]}")
        if build_type_choice == "1":
            cmake_args.append("-Ddefaultcolor=[34m")
    
    cmake_cmd = f"cmake {' '.join(cmake_args)} ../.."
    
    if IS_WINDOWS:
        log_file = "CMake_Build_Log.txt"
        run_command(f"{cmake_cmd} >{log_file} && type {log_file}", shell=True)
    else:
        run_command(cmake_cmd, capture_output=False, shell=True)
    
    print(f"{Colors.GREEN}Running CMake build...{Colors.RESET}")
    build_cmd = f"cmake --build ./ --config {build_type}"
    run_command(build_cmd, capture_output=False, shell=True)
    
    print(f"{Colors.YELLOW}[Note]: Deliveries folder should be created in parent directory{Colors.RESET}")

if __name__ == "__main__":
    main()
