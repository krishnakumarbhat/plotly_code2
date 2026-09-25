import argparse
import sys
import os
import time
import subprocess
import toml
import msvcrt
from pathlib import Path

SQTS_DIR = Path(__file__).parent.parent
os.chdir(SQTS_DIR)
sys.path.insert(1, str(SQTS_DIR))

from src import VectorExe, Trace32
from src.print_trace_lib import Trace


def read_config_toml():
    global CONFIG
    CONFIG = toml.load(TOML_FILE_PATH)


# Set up for before all test cases here
def OpenT32():
    global CONFIG
    read_config_toml()

    sqts_config = CONFIG

    config = sqts_config['Trace32']['connection'][0]

    # Code to run before the first test case that requests this fixture
    executable = config['executable']
    t32_api = Trace32(executable)

    # First close any Trace32 windows
    try:
        subprocess.check_call([f"taskkill /im {Path(executable).name}"], stdout=subprocess.DEVNULL)
        time.sleep(3)
    except Exception:
        pass

    try:
        t32_api.start_exe(t32_config=config['t32_config'])
        t32_api.connect(
            connection_port=config['port'],
            connection_protocol=config['protocol']
        )

    except Exception as exc:

        t32_api.exit_exe()
        raise ConnectionError('Unable to connect to Trace32') from exc

    t32_config = sqts_config['Trace32']

    t32_api.set_processor_type(t32_config['processor_type'])

    t32_setup = t32_config['setup']

    # If a cmm path was specified in the toml file the script will use that
    for cmm in t32_setup['cmm']:
        specified_path = cmm['path']
        if not specified_path:
            continue

        cmm_path = Path(specified_path).absolute()
        if cmm.get('change_dir', True):
            cmm_dir = cmm_path.parent
            t32_api.cmd(f'CD "{cmm_dir}"')

        t32_api.run_cmm(cmm_path, timeout_s=60)

    # If the hsm verify is set as true then the script will do it, otherwise it won't
    if t32_setup['hsm_verify']:
        tc397x_config = t32_config['TC397x']

        t32_api.set_hsm(tc397x_dir=tc397x_config['HSM_dir'])

        flashing_files_paths = tc397x_config['flashing_files']

        flashing_files = [
            {'C': True, 'V': True, 'file': flashing_files_paths['application']},
            {'S': True, 'file': flashing_files_paths['symbols']}
        ]

        t32_api.tc397x.configure(
            flashing_files=flashing_files,
            source_code_dirs=tc397x_config['source_code_dirs'],
            options=tc397x_config['options']
        )

        t32_api.tc397x.verify()

        time.sleep(2)

    mem_accs_cmd = 'B:: SYStem.Option.DUALPORT.ON'
    t32_api.cmd(mem_accs_cmd)

    time.sleep(2)

    t32_api.power_on()

    return t32_api


def start_canoe():
    read_config_toml()
    CONFIG_PATH = CONFIG["CANoe"]["config_path"]
    canoe = VectorExe("CANoe", enable_verbose=False)
    canoe.start_exe()
    canoe.load_config(CONFIG_PATH)
    canoe.start_measurement()
    time.sleep(10)

    return canoe


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=""""Launch CANoe, T32 or both with the configuration paths given in an XML file.
    Pass no arguments to load both from the default XML file 'config.xml' in the current directory""")

    parser.add_argument(
        "-toml", "--conf-paths-toml",
        dest="toml_path",
        type=str,
        nargs=1,
        help="the path to the toml file containing the configuration paths"
    )
    parser.add_argument(
        "-l", "--Launch",
        dest="launch",
        type=str,
        nargs=1,
        help="'CANoe' or 'T32' or 'ALL'",
        choices=['CANoe', 'T32', 'ALL']
    )

    # get the options from the cmd arguments
    parsed_args = parser.parse_args()
    global TOML_FILE_PATH
    TOML_FILE_PATH = parsed_args.toml_path[0] if parsed_args.toml_path else 'pyproject.toml'
    launch_canoe = True if not parsed_args.launch or parsed_args.launch[0].lower() in ['canoe', 'all'] else False
    launcht32 = True if not parsed_args.launch or parsed_args.launch[0].lower() in ['t32', 'all'] else False

    # print out some info
    Trace.print_header("Testing Automation framework: LOADING CANoe &/OR T32 ")
    Trace.print_info(f"toml file path is:  {TOML_FILE_PATH}")
    Trace.print_info(
        f"will launch: {'CANoe' if launch_canoe else ''} {'and' if launch_canoe and launcht32 else ''} {'TRACE32' if launcht32 else ''}")

    canoe_obj = None
    t32_obj = None

    if launch_canoe:
        canoe_obj = start_canoe()

    if launcht32:
        t32_obj = OpenT32()

    quit = None

    while quit != b'q':
        print("Press 'q' to quit")
        quit = msvcrt.getch()

    if canoe_obj is not None:
        canoe_obj.exit_exe()

    if t32_obj is not None:
        t32_obj.exit_exe()
