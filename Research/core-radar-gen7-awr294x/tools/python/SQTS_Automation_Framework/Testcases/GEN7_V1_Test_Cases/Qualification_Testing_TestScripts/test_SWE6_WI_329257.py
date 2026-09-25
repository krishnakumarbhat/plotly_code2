"""Python testcase for Measurement_Monitor."""
import pytest
import time


@pytest.mark.WI("WI_329257")
@pytest.mark.description("Test for Voltage State Machine update")
def test_Front_End_Management(Power, T32_R5A, T32_C66, Report):
    """WinCLEAR."""
    T32_R5A.Clean_and_Reset()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Down")
    time.sleep(5)
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
    print("-----------------------------------WI-329257 :START-----------------------------------")
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

    # t1 = "Radar_Ctl_State"
    # temp1 = T32_R5A.add_var_watch(t1)
    # temp1 = T32_R5A.read_var(temp1)
    # print(t1, "=", temp1)
    li = ["MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.XCP_Over_Temperature_Api_Counter", "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.Over_Temp_DegC",
          "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.RC_In_Degraded_Mode_Count", "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.RC_In_Idle_Mode_Count",
          "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.RC_Over_Temp_Fault_Count", "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.Temperature_Error_Clear_Counter",
          "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.Temperature_Error_Counter", "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.Temperature_Error_In_Degraded",
          "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.OT_Error_Status_Prev_Cycle", "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.RC_Idle_Timeout_Status"]
    count = 0
    ref_val = [0, 0, 0, 0, 0, 0, 0, 85, 0, 85]
    for i in li:
        T32_R5A.add_var_watch(i)
        temp_2 = T32_R5A.read_var(i)
        print(i, "=", temp_2)
        print(temp_2, "=", ref_val[count])
        time.sleep(1)
        if temp_2 == ref_val[count]:
            print("Passed")
        else :
            print("failed")
        if temp_2 == ref_val[count] and flag :
            print("Passed")
            condition = 1
        else:
            condition = 0
            print("failed")
        count = count + 1
    time.sleep(1)
    print(count)
    print("")
    print("-----------------------------------WI-329257 :END-------------------------------------")
    assert condition
###############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
# 09/12/2025   SHAIK OSMANE GANI  EAH-7463              Script correction
