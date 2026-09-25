"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback
import re


def Platform_Active_Fault_Table_List(File_Name: str = r".\..\..\..\..\software\app\mss\autosar\Aptiv_SWC\COMMON\Diagnostic_Library\Include\Fault_Lib.h") -> list:
    """Parse fault table from header file and return list of active fault variables."""
    import os

    # Check if file exists
    if not os.path.exists(File_Name):
        print(f"Warning: Header file not found at {File_Name}")
        print("Using default fault list instead.")
        # Return empty list to allow script to continue without header file
        return []

    j = 0
    variable_names = []
    try:
        with open(File_Name, "r") as f:
            header_contents = f.read()
    except Exception as e:
        print(f"Error reading file: {e}")
        return []

    start_idx = header_contents.find(r"/* Packed_Platform_Fault_bits_T */")
    end_idx = header_contents.find("}", start_idx)
    if ((start_idx != -1) and (end_idx != -1)):
        struct_content = header_contents[start_idx:end_idx + 1]
    else:
        return []

    for line in struct_content.splitlines():
        matches = re.findall(r'([a-zA-Z_][a-zA-Z_0-9]*)', line)
        variable_names.extend(matches)

    a = []
    for i in variable_names:
        if i == 'bitfield8_t':
            a.append(variable_names[j + 1])
        j += 1

    Variables_Members = []
    for i in a:
        if not i.startswith('reserved_'):
            Variables_Members.append(i)

    Add_Var = r"Platform_Active_Fault_Table.platform_bits."
    Variables_List = [Add_Var + s for s in Variables_Members]
    return Variables_List


@pytest.mark.WI("WI-362079")
@pytest.mark.description("To check the Look Index Stale fault.")
def test_Fault_Manager(Power, T32_R5A, T32_C66, Report):
    """WinCLEAR."""
    T32_R5A.Clean_and_Reset()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.mode.Down")
    time.sleep(2)
    try:
        T32_R5A.cmd("SYStem.mode.Up")
    except Exception:
        print("An exception occurred R5A")
        traceback.print_exc()
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
        traceback.print_exc()
    time.sleep(5)
    print("-----------------------------------WI-362079:START-----------------------------------")
    # Checking Radar Status
    T_1 = []
    T_1.append("RADAR_CTL_INIT_FAIL")
    T_1.append("RADAR_CTL_INIT_NOT_STARTED")
    T_1.append("RADAR_CTL_INIT_STARTED")
    T_1.append("RADAR_CTL_INIT_SUCCESS")
    t = "Radar_Ctl_Data.init_status"
    # Wait for radar initialization with retry and power cycle on NOT_STARTED/FAIL
    retry_count = 0
    max_retries = 5
    power_cycle_attempts = 0
    max_power_cycles = 3
    T = T32_R5A.read_var(t)
    print("")
    print(t, "=", T_1[T])
    while T != 3 and retry_count < max_retries:
        retry_count += 1
        print(f"Waiting for radar initialization... (attempt {retry_count}/{max_retries})")
        # If radar is in NOT_STARTED state (1) or FAIL state (0), perform power cycle
        if (T == 1 or T == 0) and power_cycle_attempts < max_power_cycles:
            print(f"\nRadar in {T_1[T]} state. Performing power cycle {power_cycle_attempts + 1}/{max_power_cycles}...")
            power_cycle_attempts += 1
            Power.switch_off()
            time.sleep(3)
            Power.switch_on()
            time.sleep(3)
            # Perform Lauterbach reset to code start
            print("Performing Lauterbach reset to code start...")
            T32_R5A.cmd("SYStem.Down")
            time.sleep(2)
            T32_R5A.cmd("SYStem.Attach")
            time.sleep(5)
            # Check radar status again after power cycle
            T = T32_R5A.read_var(t)
            print("After power cycle:")
            print(t, "=", T_1[T])
            print()
        else:
            # Regular retry without power cycle
            time.sleep(5)
            try:
                T = T32_R5A.read_var(t)
                print(t, "=", T_1[T])
            except Exception as e:
                print(f"Error reading radar status: {e}")
                traceback.print_exc()
    if T == 3:
        flag = True
    else:
        flag = False

    # Get fault list from header file
    try:
        fault_name = Platform_Active_Fault_Table_List(r".\..\..\..\..\software\app\mss\autosar\Aptiv_SWC\COMMON\Diagnostic_Library\Include\Fault_Lib.h")
    except Exception as e:
        print(f"Error loading fault list: {e}")
        traceback.print_exc()
        fault_name = []

    if len(fault_name) > 0:
        print(f"Platform_Active_Fault_Table = {fault_name}")
        print()
        print(f"Number of Platform Active Faults which we have in software are {len(fault_name)}.")
        print()
    else:
        print("Skipping detailed fault list - header file not available")
        print()

    # Define specific faults to monitor
    monitored_faults = [
        "Platform_Active_Fault_Table.platform_bits.look_index_stale_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_temperature_high_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_a_sensor_dead_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_safe_start_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_a_safety_integtrity_check_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_loop_back_test_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_temperature_low_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_transmitter_id_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_tx3_ball_break_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_tx2_ball_break_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_tx1_ball_break_fault",
        "Platform_Active_Fault_Table.platform_bits.mmic_tx0_ball_break_fault",
        "Platform_Active_Fault_Table.platform_bits.acquisition_overflow_fault",
        "MMIC_Errors_Stub"
    ]

    fault_values_before = [0] * len(monitored_faults)
    fault_values_after = [0] * len(monitored_faults)

    # Read initial fault values
    print("# Reading initial fault values #")
    for i, fault in enumerate(monitored_faults):
        # T32_R5A.add_var_watch(fault)
        time.sleep(0.5)
        fault_values_before[i] = T32_R5A.read_var(fault)
        print(f"{fault} = {fault_values_before[i]}")

    # Overwrite MMIC_Errors_Stub to 1
    print()
    print("Overwriting MMIC_Errors_Stub to 1")
    T32_R5A.cmd("Var.set %e MMIC_Errors_Stub = 1")
    time.sleep(2)

    # Read fault values after setting MMIC_Errors_Stub
    print()
    print("# Reading fault values after setting MMIC_Errors_Stub #")
    for i, fault in enumerate(monitored_faults):
        fault_values_after[i] = T32_R5A.read_var(fault)
        time.sleep(0.5)
        print(f"{fault} = {fault_values_after[i]}")

    print()
    print("-----------------------------------WI-362079 :END-------------------------------------")
    time.sleep(3)

    condition = 0
    if (fault_values_after[1] == 1
            and fault_values_after[2] == 1
            and fault_values_after[3] == 1
            and fault_values_after[4] == 0
            and fault_values_after[5] == 1
            and fault_values_after[6] == 1
            and fault_values_after[7] == 1
            and fault_values_after[8] == 1
            and fault_values_after[9] == 1
            and fault_values_after[10] == 1
            and fault_values_after[11] == 1
            and fault_values_after[12] == 0
            and fault_values_after[13] == 1
            and flag):
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    assert condition
##############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
