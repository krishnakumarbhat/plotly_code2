#!/usr/bin/env python3

import os
import sys
import re
import subprocess
import shutil
import platform
from pathlib import Path
import concurrent.futures

IS_WINDOWS = platform.system() == 'Windows'
PYTHON_CMD = "python" if IS_WINDOWS else "python3"

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

def setup_build_environment():
    if os.path.exists('build'):
        shutil.rmtree('build')
    os.makedirs('build')
    os.chdir('build')

def main():
    print(f"Detected operating system: {platform.system()}")
    
    # Hardcoded values for pipeline
    customer_name = "CEER"
    input_output = 5
    build_type_choice = "1"  
    
    map_path = os.path.normpath('Code/LogicModel2.h')
    customer_map = read_customer_map(map_path)
    
    if not customer_map:
        print("Error: No customers found in LogicModel2.h")
        sys.exit(1)
    
    setup_build_environment()
    
    if customer_name not in customer_map:
        print(f"Error: Customer '{customer_name}' not found in LogicModel2.h map")
        sys.exit(1)
        
    customer_id = customer_map[customer_name]
    
    with concurrent.futures.ThreadPoolExecutor() as executor:
        python_future = executor.submit(check_python)
        
        if not python_future.result():
            print("Python is required but not found.")
            sys.exit(1)
    
    try:
        original_dir = os.getcwd()
        
        run_command(f"{PYTHON_CMD} ../UpdateModelDescription.py {input_output}", shell=True)
        
        if os.getcwd() != original_dir:
            os.chdir(original_dir)
            
    except Exception as e:
        print(f"Failed to run UpdateModelDescription.py: {e}")
        sys.exit(1)
    
    build_type = "Release"  
    
    build_type_dir = os.path.join(os.getcwd(), build_type)
    if os.path.exists(build_type_dir):
        shutil.rmtree(build_type_dir)
    os.makedirs(build_type_dir)
    os.chdir(build_type_dir)
    
    cmake_args = [
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-DCUSTOMER_ID={customer_id}",
        f"-DCUSTOMER={customer_name}",
        f"-DNUM_OUTPUTS={input_output}",
        f"-DNUM_INPUTS={input_output}",
        "-DCUSTOMER_FW=0"  # For Release build
    ]
    
    if IS_WINDOWS and len(sys.argv) > 1:
        cmake_args.insert(0, f"-G {sys.argv[1]}")
    
    cmake_cmd = f"cmake {' '.join(cmake_args)} ../.."
    
    if IS_WINDOWS:
        log_file = "CMake_Build_Log.txt"
        run_command(f"{cmake_cmd} >{log_file} && type {log_file}", shell=True)
    else:
        run_command(cmake_cmd, capture_output=False, shell=True)
    
    print("Running CMake build...")
    build_cmd = f"cmake --build ./ --config {build_type}"
    run_command(build_cmd, capture_output=False, shell=True)
    
    print("[Note]: Deliveries folder should be created in parent directory")

if __name__ == "__main__":
    main()
