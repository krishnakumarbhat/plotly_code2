"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-363103")
@pytest.mark.description("Test to check the Detection Processing Loop Overrun fault")
def test_FAULT_MANAGER(Power, T32_R5A, T32_C66, Report):
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

    print("-----------------------------------WI-363103 :START-----------------------------------")
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
        print("Passed")
        condition = 1
    else :
        condition = 0
        print("failed")
    assert condition
    flag = True

    # Add variable watches
    temp_1 = "IPC_D2M_Buffer[0].payload.diag_data.det_proc_loop_overrun_cnt"
    T32_R5A.add_var_watch(temp_1)
    temp_2 = "IPC_D2M_Buffer[1].payload.diag_data.det_proc_loop_overrun_cnt"
    T32_R5A.add_var_watch(temp_2)
    temp_3 = "Platform_Active_Fault_Table.platform_bits.detection_processing_loop_overrun_fault"
    T32_R5A.add_var_watch(temp_3)

    # Read initial values
    TEMP_1 = T32_R5A.read_var(temp_1)
    TEMP_2 = T32_R5A.read_var(temp_2)
    TEMP_3 = T32_R5A.read_var(temp_3)
    print("In free run ", temp_1, "=", TEMP_1)
    print("In free run ", temp_2, "=", TEMP_2)
    print("In free run ", temp_3, "=", TEMP_3)

    if TEMP_1 < 6 and TEMP_2 < 6 and TEMP_3 == 0:
        print("Initial values are as expected")
    else:
        print("det_proc_loop_overrun_cnt is greater than maximum value in free run or fault already set")
        flag = False

    time.sleep(3)
    print("")
    print("Increasing det_proc_loop_overrun_cnt to create detection_processing_loop_overrun_fault")
    time.sleep(1)
    T32_R5A.cmd("Var.set %e IPC_D2M_Buffer[0].payload.diag_data.det_proc_loop_overrun_cnt = 6")
    time.sleep(1)
    T32_R5A.cmd("Var.set %e IPC_D2M_Buffer[1].payload.diag_data.det_proc_loop_overrun_cnt = 6")
    time.sleep(1)

    # Read values after increasing
    TEMP_1 = T32_R5A.read_var(temp_1)
    TEMP_2 = T32_R5A.read_var(temp_2)
    TEMP_3 = T32_R5A.read_var(temp_3)
    print(temp_1, "=", TEMP_1)
    print(temp_2, "=", TEMP_2)
    print(temp_3, "=", TEMP_3)

    if TEMP_3 == 1:
        print("detection_processing_loop_overrun_fault is set to 1 as expected")
    else:
        print("detection_processing_loop_overrun_fault is not set to 1")
        flag = False

    print("")
    print("Decreasing det_proc_loop_overrun_cnt to clear detection_processing_loop_overrun_fault")
    time.sleep(1)
    T32_R5A.cmd("Var.set %e IPC_D2M_Buffer[0].payload.diag_data.det_proc_loop_overrun_cnt = 5")
    time.sleep(1)
    T32_R5A.cmd("Var.set %e IPC_D2M_Buffer[1].payload.diag_data.det_proc_loop_overrun_cnt = 5")
    time.sleep(1)

    # Read values after decreasing
    TEMP_1 = T32_R5A.read_var(temp_1)
    TEMP_2 = T32_R5A.read_var(temp_2)
    TEMP_3 = T32_R5A.read_var(temp_3)
    print(temp_1, "=", TEMP_1)
    print(temp_2, "=", TEMP_2)
    print(temp_3, "=", TEMP_3)

    if TEMP_3 == 0:
        print("detection_processing_loop_overrun_fault is cleared to 0 as expected")
    else:
        print("detection_processing_loop_overrun_fault is not cleared to 0")
        flag = False

    print("")
    print("-----------------------------------WI-363103 :END-------------------------------------")
    time.sleep(3)
    condition = 0
    if flag:
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
# 09/12/2025   AZEEJ SHAIK        EAH-7440             Script correction
