"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-362025")
@pytest.mark.description("Test to check Alignment Faults")
def test_FAULT_MANAGER(Power, T32_R5A, T32_C66, Report):
    """WinCLEAR."""
    T32_R5A.Clean_and_Reset()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Down")
    time.sleep(5)
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
    print("-----------------------------------WI-362025 :START-----------------------------------")
    print("")
    # Checking Radar Status
    T_1 = []
    T_1.append("RADAR_CTL_INIT_FAIL")
    T_1.append("RADAR_CTL_INIT_NOT_STARTED")
    T_1.append("RADAR_CTL_INIT_STARTED")
    T_1.append("RADAR_CTL_INIT_SUCCESS")
    t = "Radar_Ctl_Data.init_status"
    T = T32_R5A.read_var(t)
    print("")

    if T == 3 :
        flag = True
        print(t, "=", T_1[T])
    else :
        print("Execution is stopped, Radar Status is ", T_1[T])
        flag = False

    T32_R5A.cmd("Break")
    time.sleep(0.5)
    command = r"RE_PLT_Diag_Alignment_Diagnostics_50ms+0x0C"
    T32_R5A.set_breakpoint(command)
    time.sleep(3)
    T32_R5A.cmd("Go")
    time.sleep(2)
    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit inside the function Diag_Feature_Function_Calibration_Fault_Test_50ms")
    else :
        print("Breakpoint is not hit inside the function Diag_Feature_Function_Calibration_Fault_Test_50ms")
        flag = False
    time.sleep(1)
    temp = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    TEMP = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    temp[0] = "Alignment_Errors.Autoalign_outof_range_error"
    temp[1] = "Alignment_Errors.ShortTrack_Calibration_error"
    temp[2] = "Alignment_Errors.Service_Calibration_error"
    temp[3] = "Alignment_Errors.Static_Calibration_error"
    temp[4] = "Alignment_Errors.No_Initial_Calibration_error"
    temp[5] = "Platform_Active_Fault_Table.platform_bits.alignment_no_initial_calibration_error_fault"
    temp[6] = "Platform_Active_Fault_Table.platform_bits.alignment_static_calibration_error_fault"
    temp[7] = "Platform_Active_Fault_Table.platform_bits.alignemt_service_calibration_error_fault"
    temp[8] = "Platform_Active_Fault_Table.platform_bits.alignemt_shorttrack_calibration_error_fault"
    temp[9] = "Platform_Active_Fault_Table.platform_bits.alignemt_autoalignment_out_of_range_fault"
    for i in range(0, len(temp)) :
        T32_R5A.add_var_watch(temp[i])
        time.sleep(0.5)
        TEMP[i] = T32_R5A.read_var(temp[i])
        print(temp[i], "=", TEMP[i])
        if TEMP[i] == 170 or TEMP[i] == 1 :
            condition_flag = 1
        else :
            condition_flag = 0
    if condition_flag == 0 :
        flag = True
        print("No Alignment Errors are set in free run")
    else :
        print("Alignment Errors are set in free run")
        flag = False
    print("")
    print("After Overwritting Alignment_Errors to CODED_TRUE")
    T32_R5A.cmd("Var.set %e Alignment_Errors.Autoalign_outof_range_error = 170")
    time.sleep(0.5)
    T32_R5A.cmd("Var.set %e Alignment_Errors.ShortTrack_Calibration_error = 170")
    time.sleep(0.5)
    T32_R5A.cmd("Var.set %e Alignment_Errors.Service_Calibration_error = 170")
    time.sleep(0.5)
    T32_R5A.cmd("Var.set %e Alignment_Errors.Static_Calibration_error = 170")
    time.sleep(1)
    T32_R5A.cmd("Var.set %e Alignment_Errors.No_Initial_Calibration_error = 170")
    time.sleep(1)
    T32_R5A.cmd("Go")
    time.sleep(2)
    for i in range(5, 10) :
        TEMP[i] = T32_R5A.read_var(temp[i])
        time.sleep(0.5)
        print(temp[i], "=", TEMP[i])

    print("")
    print("-----------------------------------WI-362025 :END-------------------------------------")
    time.sleep(5)
    condition = 0
    if TEMP[5] == 1 and TEMP[6] == 1 and TEMP[7] == 1 and TEMP[8] == 1 and TEMP[9] == 1 and flag:
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    assert condition
