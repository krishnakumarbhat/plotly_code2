"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-347621")
@pytest.mark.description("Test to check Blockage and Algo Error Faults")
def test_FAULT_MANAGER(Power, T32_R5A, T32_C66, Report):
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
    print("-----------------------------------WI-347621 :START-----------------------------------")
    print("")
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

    # Group fault variable names into a list
    fault_vars = [
        "Platform_Active_Fault_Table.platform_bits.damping_algo_error_fault",
        "Platform_Active_Fault_Table.platform_bits.blockage_algo_error_fault"
    ]

    # Add watches and read initial values for fault variables
    fault_values = []
    for var in fault_vars:
        T32_R5A.add_var_watch(var)
        value = T32_R5A.read_var(var)
        fault_values.append(value)
        if value == 1:
            print(f"{var.split('.')[-1]} is set in free run")
            flag = False
        else:
            print(f"In free run {var} = {value}")

    TEMP, TEMP_1 = fault_values
    # T32_R5A.cmd("Break")
    time.sleep(0.5)
    T32_R5A.set_breakpoint("RE_PLT_Diag_Radar_Blockage_Detection_Test_50ms+0x14")
    time.sleep(3)
    T32_R5A.cmd("Go")
    time.sleep(2)
    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit inside the function Diag_Radar_Interference_Detection_Test_50ms")
    else :
        print("Breakpoint is not hit inside the function Diag_Radar_Interference_Detection_Test_50ms")
        flag = False
    time.sleep(1)
    print("")
    print("After Overwritting Blockage_Algo_Error to TRUE")
    T32_R5A.cmd("Var.set %e ipc_d2m_sp_post_proc_buffer.Blockage_Algo_Error = 170")
    time.sleep(0.5)
    print("After Overwritting Damping_Algo_Error to TRUE")
    T32_R5A.cmd("Var.set %e ipc_d2m_sp_post_proc_buffer.Damping_Algo_Error = 170")
    time.sleep(0.5)
    T32_R5A.cmd("Go")
    time.sleep(2)

    # Read updated fault values
    updated_fault_values = []
    for var in fault_vars:
        value = T32_R5A.read_var(var)
        updated_fault_values.append(value)
        print(f"{var} = {value}")
    TEMP, TEMP_1 = updated_fault_values

    print("")
    print("-----------------------------------WI-347621 :END-------------------------------------")
    time.sleep(5)

    # Check test condition
    if TEMP == 1 and TEMP_1 == 1 and flag:
        print("Passed")
        assert True
    else:
        print("failed")
        raise AssertionError()
##############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
