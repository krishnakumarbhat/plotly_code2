"""Python Script for Flashing and checking Radar_Ctl_Data."""
import pytest
import time
import traceback

from typing import Literal
from dataclasses import dataclass
from pathlib import Path


@pytest.fixture(scope='session')
def T32_R5A(Trace32_Session_Minimal, sqts_config, Power_Session):
    """T32 Instance for R5A."""
    t32 = Trace32_Session_Minimal
    files = sqts_config['Trace32']['files']
    flags = sqts_config['Trace32']['files']['flags']
    autoflash_cmm = Path(files['auto_flash_cmm']).absolute()
    pbl_hex = Path(files['pbl_hex']).absolute()
    hsm_hex = Path(files['hsm_hex']).absolute()
    pbl_elf = Path(files['pbl_elf']).absolute()
    app_hex = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['app_hex'])).absolute()
    mss_elf = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['mss_elf'])).absolute()
    dss_elf = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['dss_elf'])).absolute()
    smc_ptp = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['smc_ptp'])).absolute()
    usc_ptp = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['usc_ptp'])).absolute()
    time.sleep(10)
    t32.print("before reset")
    t32.cmd("SYStem.mode.Down")
    try:
        t32.cmd("SYStem.mode.Up")
    except Exception:
        print("An exception occurred R5A")
    t32.cmd("SYStem.mode.Attach")
    t32.cmd("Break")
    t32.print("after reset")
    t32.run_cmm(
        f'"{autoflash_cmm}"',
        sqts_config['Trace32']['pre_erase'],
        pbl_hex,
        hsm_hex,
        pbl_elf,
        app_hex,
        mss_elf,
        dss_elf,
        smc_ptp,
        usc_ptp,
        timeout_s=60
    )
    t32.delete_all_breakpoints()
    yield t32


@pytest.fixture(scope='session')
def T32_C66(Trace32_Session_Minimal2):
    """T32 Instance for C66."""
    t32 = Trace32_Session_Minimal2
    t32.delete_all_breakpoints()
    yield t32


@pytest.fixture(scope='session', autouse=True)
def global_setup(request, Power_Session):
    """T32 power session."""
    Power_Session.switch_on()
    request.getfixturevalue('Trace32_Session_Minimal')
    request.getfixturevalue('Trace32_Session_Minimal2')
    time.sleep(2)
    yield
    Power_Session.switch_off()


@dataclass
class InterfaceData:
    """Class to hold test case variable and functions."""

    provider_interface_variable: str
    provider_core: Literal['C66', 'R5A']
    receiver_interface_variable: str
    receiver_core: Literal['R5A', 'C66']


def test_SW_Radar_Status(T32_R5A, T32_C66, Report, Power_Session):
    """WinCLEAR."""
    T32_R5A.cmd("WinCLEAR")
    T32_R5A.cmd("Do .\\..\\..\\..\\instrumentation\\Lauterbach\\default_win_r5f.cmm")
    T32_R5A.delete_all_breakpoints()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.mode.Down")
    time.sleep(2)
    try:
        T32_R5A.cmd("SYStem.mode.Up")
    except Exception:
        print("An exception occurred R5A")
    time.sleep(5)
    attachAttempts = 0
    while T32_R5A.get_run_state() != 3:
        T32_R5A.cmd("SYStem.Attach")
        attachAttempts += 1
        time.sleep(10)
        T32_R5A.print(f'-------- State: {T32_R5A.get_run_state()} --------')
        if (attachAttempts > 5) :
            break
    T32_C66.cmd("SYStem.mode.NoDebug")
    time.sleep(2)
    try:
        T32_C66.cmd("SYStem.mode.Attach")
    except Exception:
        print("An exception occurred C66")
    time.sleep(5)
    # Checking Radar Status
    T_1 = []
    T_1.append("RADAR_CTL_INIT_FAIL")
    T_1.append("RADAR_CTL_INIT_NOT_STARTED")
    T_1.append("RADAR_CTL_INIT_STARTED")
    T_1.append("RADAR_CTL_INIT_SUCCESS")
    t = "Radar_Ctl_Data.init_status"
    T32_R5A.add_var_watch(t)

    # Wait for radar initialization with retry
    retry_count = 0
    max_retries = 10
    T = T32_R5A.read_var(t)
    print("")
    print(t, "=", T_1[T])

    while T != 3 and retry_count < max_retries:  # Wait for RADAR_CTL_INIT_SUCCESS
        retry_count += 1
        print(f"Waiting for radar initialization... (attempt {retry_count}/{max_retries})")
        time.sleep(5)
        try:
            T = T32_R5A.read_var(t)
            print(t, "=", T_1[T])
        except Exception as e:
            print(f"Error reading radar status: {e}")
            traceback.print_exc()

    # If radar is in fail state, perform power cycle and retry
    if T == 0:  # RADAR_CTL_INIT_FAIL
        print("Radar initialization failed. Performing power cycle...")
        Power_Session.power_cycle()
        time.sleep(5)
        T32_R5A.Clean_and_Reset()
        T32_C66.delete_all_breakpoints()
        try:
            T32_C66.cmd("SYStem.mode.Attach")
        except Exception:
            print("An exception occurred C66")
            traceback.print_exc()
        time.sleep(5)
        T = T32_R5A.read_var(t)
        print(t, "=", T_1[T])

    condition = 0
    if T == 3:
        print("Passed")
        condition = 1
    else :
        condition = 0
        print("failed")
    assert condition
