#!/usr/bin/env python3

import os
import sys
import platform
import concurrent.futures

from Build import (
    Colors,
    IS_WINDOWS,
    PYTHON_CMD,
    CUSTOMER_PARAMS,
    enable_windows_ansi_colors,
    read_customer_map,
    run_command,
    check_python,
    remove_directory_safe,
)


def build_customer(fmu_root, customer_name, input_output, customer_id, generator_arg):
    build_root = os.path.join(fmu_root, 'build')
    remove_directory_safe(build_root)
    os.makedirs(build_root, exist_ok=True)
    os.chdir(build_root)

    print(f"{Colors.BLUE}[INFO] Updating model description for {customer_name} (IO={input_output}){Colors.RESET}")
    run_command(f"{PYTHON_CMD} ../UpdateModelDescription.py {input_output}", capture_output=False, shell=True)

    build_type = "Release"
    build_type_dir = os.path.join(build_root, build_type)
    remove_directory_safe(build_type_dir)
    os.makedirs(build_type_dir, exist_ok=True)
    os.chdir(build_type_dir)

    cmake_args = [
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-DCUSTOMER_ID={customer_id}",
        f"-DCUSTOMER={customer_name}",
        f"-DNUM_OUTPUTS={input_output}",
        f"-DNUM_INPUTS={input_output}",
        "-DCUSTOMER_FW=0",
        "-DCMAKE_POLICY_VERSION_MINIMUM=3.5",
        "-Wno-deprecated",
    ]

    if IS_WINDOWS and generator_arg:
        cmake_args.insert(0, f"-G {generator_arg}")

    cmake_cmd = f"cmake {' '.join(cmake_args)} ../.."

    print(f"{Colors.BLUE}[INFO] Configuring CMake for {customer_name}{Colors.RESET}")
    if IS_WINDOWS:
        log_file = "CMake_Build_Log.txt"
        run_command(f"{cmake_cmd} >{log_file} && type {log_file}", capture_output=False, shell=True)
    else:
        run_command(cmake_cmd, capture_output=False, shell=True)

    print(f"{Colors.GREEN}[INFO] Building FMU for {customer_name}...{Colors.RESET}")
    run_command(f"cmake --build ./ --config {build_type}", capture_output=False, shell=True)

    print(f"{Colors.GREEN}[INFO] {customer_name} FMU built and copied to Deliverables{Colors.RESET}")

    os.chdir(fmu_root)
    remove_directory_safe(build_root)


def main():
    enable_windows_ansi_colors()
    print(f"Detected operating system: {platform.system()}")

    fmu_root = os.getcwd()
    customer_map = read_customer_map(os.path.normpath('Code/LogicModel2.h'))

    if not customer_map:
        print(f"{Colors.RED}Error: No customers found in LogicModel2.h{Colors.RESET}")
        sys.exit(1)

    with concurrent.futures.ThreadPoolExecutor() as executor:
        python_future = executor.submit(check_python)
        if not python_future.result():
            print(f"{Colors.RED}Python is required but not found.{Colors.RESET}")
            sys.exit(1)

    # Optional CMake generator, forwarded the same way Build.py accepts it
    generator_arg = sys.argv[1] if len(sys.argv) > 1 else None

    for customer_name, input_output in CUSTOMER_PARAMS.items():
        print(f"{Colors.BLUE}[INFO] ==================== Building {customer_name} ===================={Colors.RESET}")

        if customer_name not in customer_map:
            print(f"{Colors.RED}Error: Customer '{customer_name}' not found in LogicModel2.h map{Colors.RESET}")
            sys.exit(1)

        customer_id = customer_map[customer_name]
        build_customer(fmu_root, customer_name, input_output, customer_id, generator_arg)

    print(f"{Colors.GREEN}[SUCCESS] All customer FMUs built and copied to Deliverables.{Colors.RESET}")


if __name__ == "__main__":
    main()
