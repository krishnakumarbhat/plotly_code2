"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-355535")
@pytest.mark.description("Test to check Radar Interference Fault Detection")
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

    temp = "Platform_Active_Fault_Table.platform_bits.interference_detection_fault"
    T32_R5A.add_var_watch(temp)
    TEMP = T32_R5A.read_var(temp)
    if TEMP == 1 :
        print("interference_detection_fault is set Platform_Active_Fault_Table in free run")
        flag = False
    else :
        print("In free run Platform_Active_Fault_Table.platform_bits.interference_detection_fault = ", TEMP)
    T32_R5A.cmd("Break")
    time.sleep(0.5)
    command = r"RE_PLT_Diag_Radar_Blockage_Detection_Test_50ms+0x78"
    T32_R5A.set_breakpoint(command)
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
    print("After Overwritting Radar_Interference_value to TRUE")
    T32_R5A.cmd("Var.set %e ipc_d2m_sp_post_proc_buffer.Radar_Interference_value = 170")
    T32_R5A.cmd("Go")
    time.sleep(2)
    TEMP = T32_R5A.read_var(temp)
    print(temp, "=", TEMP)
    print("")
    print("-----------------------------------WI-355535 :END-------------------------------------")
    time.sleep(5)
    condition = 0
    if TEMP == 1 and flag :
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    assert condition
