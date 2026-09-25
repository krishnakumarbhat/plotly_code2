"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-370774")
@pytest.mark.description("1st Pass Detection Fault(Gen7 V1)")
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

    print("-----------------------------------WI-370774 :START-----------------------------------")
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
        temp = "firstpass_fault_stub"
        T32_R5A.add_var_watch(temp)
        Temp = T32_R5A.read_var(temp)
        print(temp, "=", Temp)
        temp_1 = "Platform_Active_Fault_Table.platform_bits.firstpassdetection_overflow_fault"
        T32_R5A.add_var_watch(temp_1)

        time.sleep(0.5)
        T32_R5A.cmd("Var.set %e firstpass_fault_stub = 1")
        time.sleep(2)
        Temp_1 = T32_R5A.read_var(temp_1)
        if Temp_1 == 1:
            condition_1 = 1
            print(print(temp_1, "=", Temp_1))
        else:
            condition_1 = 0
            print("Platform_Active_Fault_Table.platform_bits.firstpassdetection_overflow_fault is Not Set")

        time.sleep(1.5)
        T32_R5A.cmd("Var.set %e firstpass_fault_stub = 0")
        time.sleep(2)
        Temp_1 = T32_R5A.read_var(temp_1)
        if Temp_1 == 0:
            condition_2 = 1
            print(print(temp_1, "=", Temp_1))
        else:
            condition_2 = 0
            print("Platform_Active_Fault_Table.platform_bits.firstpassdetection_overflow_fault is Not Cleared")
        print("-----------------------------------WI-370774 :END-------------------------------------")

        if condition == 1 and condition_1 == 1 and condition_2 == 1 :
            print("All the Test steps are successful")
            main_condition = 1
        else:
            main_condition = 0

        assert main_condition
    else:
        print("Execution is stopped")
        assert condition
