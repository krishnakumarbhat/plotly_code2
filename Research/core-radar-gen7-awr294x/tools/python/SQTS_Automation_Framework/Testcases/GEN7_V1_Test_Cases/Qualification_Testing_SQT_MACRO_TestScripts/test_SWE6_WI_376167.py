"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time


@pytest.mark.WI("WI_376167")
@pytest.mark.description("Write of MSS_RETENTION ram memory by Asil application")
def test_MCU_Configuration_And_Supervision(Power, T32_R5A, T32_C66, Report):
    """WinCLEAR."""
    T32_R5A.Clean_and_Reset()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Down")
    time.sleep(2)
    try:
        T32_R5A.cmd("SYStem.Up")
    except BaseException:
        print("An exception occurred")
    time.sleep(5)

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
    except BaseException:
        print("An exception occurred")
    time.sleep(5)
    print("-----------------------------------WI_376167 :START-----------------------------------")
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
        print(t, " ", T_1[T])
        condition = 1
    else:
        print("Execution is stopped, Radar Status is ", T_1[T])
        condition = 0

    if condition == 1:
        temp = "MpuTestCase"
        T32_R5A.add_var_watch(temp)
        Temp = T32_R5A.read_var(temp)
        print(temp, "=", Temp)
        time.sleep(0.5)
        T32_R5A.cmd("Var.set %e MpuTestCase = 16")
        time.sleep(0.5)
        print("MpuTestCase to MPU_MSS_RETENTION_WRITE_ACCESS_ASIL")
        if T == 3:
            print(t, " ", T_1[T])
            condition_1 = 1
            print("The software is running")
        else :
            condition_1 = 0
        print("-------------------------END-----------------------------")
        if condition == 1 and condition_1 == 1 :
            print("All the Test steps are successful")
            main_condition = 1
        else:
            main_condition = 0

        assert main_condition
    else:
        print("Execution is stopped")
        assert condition
