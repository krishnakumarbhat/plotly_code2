"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-355526")
@pytest.mark.description("To test the IPC faults handling MSS to DSS and DSS to MSS.")
def test_Fault_Manager(Power, T32_R5A, T32_C66, Report):
    """WinCLEAR."""
    T32_R5A.Clean_and_Reset()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Down")
    time.sleep(5)
    try:
        T32_R5A.cmd("SYStem.Up")
    except Exception:
        print("An exception occurred R5A")
        traceback.print_exc()
    time.sleep(6)
    attachAttempts = 0
    while T32_R5A.get_run_state() != 3:  # Running
        T32_R5A.cmd("SYStem.Attach")
        attachAttempts += 1
        time.sleep(10)
        T32_R5A.print(f'-------- State: {T32_R5A.get_run_state()} --------')
        if (attachAttempts > 5) :
            break

    T32_C66.cmd("SYStem.Mode.NoDebug")
    time.sleep(2)
    try:
        T32_C66.cmd("SYStem.Mode.Attach")
    except Exception:
        print("An exception occurred C66")
        traceback.print_exc()
    time.sleep(5)
    print("-----------------------------------WI-355526:START-----------------------------------")
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

    t1 = "Platform_Active_Fault_Table.platform_bits.dss_to_mss_ipc_fault"
    T32_R5A.add_var_watch(t1)
    temp_2 = T32_R5A.read_var(t1)
    print(t1, "=", temp_2)
    t2 = "Platform_Active_Fault_Table.platform_bits.mss_to_dss_ipc_fault"
    T32_R5A.add_var_watch(t2)
    temp_3 = T32_R5A.read_var(t2)
    print(t2, "=", temp_3)
    t3 = "Ipc_mss_dss_test_stub"
    T32_R5A.add_var_watch(t3)
    temp_4 = T32_R5A.read_var(t3)
    print(t3, "=", temp_4)
    time.sleep(1)
    print("Overwriting Ipc_mss_dss_test_stub to 1 ")
    T32_R5A.cmd("Var.set %e Ipc_mss_dss_test_stub = 1")
    time.sleep(1)
    temp_2 = T32_R5A.read_var(t1)
    print(t1, "=", temp_2)
    T32_R5A.cmd("Var.set %e Ipc_mss_dss_test_stub = 0")
    time.sleep(3)
    print("Overwriting Ipc_mss_dss_test_stub to 2 ")
    T32_R5A.cmd("Var.set %e Ipc_mss_dss_test_stub = 2")
    time.sleep(1)
    temp_3 = T32_R5A.read_var(t2)
    print(t2, "=", temp_3)
    time.sleep(1)
    if temp_2 == 1 and temp_3 == 1:
        print("Passed")
    else :
        print("failed")
    print("-----------------------------------WI-355526 :END-------------------------------------")
    time.sleep(5)
    condition = 0
    if temp_2 == 1 and temp_3 == 1 and flag :
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    assert condition
