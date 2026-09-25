#!/usr/bin/env python3

import os
import sys
import re
import argparse
import subprocess
import shutil
import platform
from pathlib import Path
import concurrent.futures

class Colors:
    BLUE = '\033[34m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    RESET = '\033[0m'

IS_WINDOWS = platform.system() == 'Windows'
PYTHON_CMD = sys.executable

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
            
        map_pattern = re.compile(r'const\s+(?:std::)?unordered_map\s*<\s*int\s*,\s*(?:std::)?string\s*>\s*CustomerName\s*=\s*\{(.*?)\};', re.DOTALL)
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
        version = run_command(f"{PYTHON_CMD} --version").strip()
        return True
    except Exception:
        print("Python is not installed.")
        return False

def setup_build_environment():
    if os.path.exists('build'):
        shutil.rmtree('build')
    os.makedirs('build')
    os.chdir('build')

def handle_customer_selection():
    print("Select Customer:")
    print("1. CEER")
    print("2. GPO_Gen8")
    print("3. STLA_SMALL_IFV600")
    
    customer_choice = input(": ")
    
    # Set Input_Output 
    customer_params = {
        "1": {"name": "CEER", "io": 5},
        "2": {"name": "GPO_Gen8", "io": 5},
        "3": {"name": "STLA_SMALL_IFV600", "io": 5}
    }
    
    if customer_choice not in customer_params:
        print("Invalid Customer")
        sys.exit(1)
    
    return customer_params[customer_choice]["name"], customer_params[customer_choice]["io"]

def main():
    enable_windows_ansi_colors()

    parser = argparse.ArgumentParser(description="Build an LM2 FMU")
    parser.add_argument("--customer", choices=["CEER", "GPO_Gen8", "STLA_SMALL_IFV600"])
    parser.add_argument("--build-type", choices=["Release", "Debug"])
    parser.add_argument("--sanitize", action="store_true",
                        help="Enable AddressSanitizer and LeakSanitizer flags (Linux only)")
    parser.add_argument("--generator", help="CMake generator to use on Windows")
    parser.add_argument("--toolset", default="v142",
                         help="CMake -T toolset spec for the VS generator (Windows only). "
                              "'version=14.29.30133,host=x64' is silently ignored by the VS 18 2026 "
                              "generator (falls back to the newest toolset); 'v142' correctly pins MSVC 14.29.")
    args = parser.parse_args()

    if args.sanitize and platform.system() != "Linux":
        parser.error("--sanitize is supported on Linux only")
    
    print(f"Detected operating system: {platform.system()}")
    
    map_path = os.path.normpath('Code/LogicModel2.h')
    customer_map = read_customer_map(map_path)
    
    if not customer_map:
        print("Error: No customers found in LogicModel2.h")
        sys.exit(1)
    
    setup_build_environment()
    if args.customer:
        customer_name = args.customer
        input_output = 5
    else:
        customer_name, input_output = handle_customer_selection()
    
    # Look up the customer ID from the map
    if customer_name not in customer_map:
        print(f"Error: Customer '{customer_name}' not found in LogicModel2.h map")
        sys.exit(1)
        
    customer_id = customer_map[customer_name]
    
    with concurrent.futures.ThreadPoolExecutor() as executor:
        python_future = executor.submit(check_python)
        
        if args.build_type:
            build_type_choice = "1" if args.build_type == "Release" else "2"
        else:
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
        "-DCMAKE_POLICY_VERSION_MINIMUM=3.5",
        "-DCMAKE_CXX_STANDARD=14",
        # "-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS",
        "-Wno-deprecated"
    ]
    sil_lib_dir = os.environ.get("SIL_LIB_DIR", "").strip()
    if sil_lib_dir:
        cmake_args.append(f"-DSIL_LIB_DIR={sil_lib_dir}")
    emblib_artifact_dir = os.environ.get("EMBLIB_ARTIFACT_DIR", "").strip()
    if emblib_artifact_dir:
        cmake_args.append(f"-DEMBLIB_ARTIFACT_DIR={emblib_artifact_dir}")
    
    if build_type_choice == "1":
        cmake_args.append("-DCUSTOMER_FW=0")

    if args.sanitize:
        cmake_args.append("-DENABLE_SANITIZERS=ON")
    
    if IS_WINDOWS and args.generator:
        cmake_args.insert(0, f'-G "{args.generator}"')
        if build_type_choice == "1":
            cmake_args.append("-Ddefaultcolor=[34m")

    # Pin the MSVC toolset used by the VS generator/MSBuild. Without this, MSBuild
    # resolves PlatformToolset independently of vcvarsall/PATH and silently picks the
    # newest installed MSVC toolset (e.g. 14.51) instead of the one activated via
    # vcvarsall.bat (14.29), even though `where cl` reports 14.29.
    if IS_WINDOWS and args.toolset and args.generator and "Visual Studio" in args.generator:
        cmake_args.insert(0, f'-T {args.toolset}')

    cmake_cmd = f"cmake {' '.join(cmake_args)} ../.."
    print(f"{Colors.BLUE}Final CMake command: {cmake_cmd}{Colors.RESET}")
    
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
