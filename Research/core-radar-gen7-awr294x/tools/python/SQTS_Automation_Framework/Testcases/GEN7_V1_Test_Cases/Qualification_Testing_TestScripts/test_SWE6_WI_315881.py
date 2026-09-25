"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_315881")
@pytest.mark.description("Qualification test validation for Look configuration API status check - Positive Test Case")
def test_FRONT_END_MANAGEMENT(Power, T32_R5A, T32_C66, Report):
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
    print("-----------------------------------WI-315881 :START-----------------------------------")
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
    temp = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    TEMP = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    counter = 0
    temp[0] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Device_Power_On"
    temp[1] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Get_Version"
    temp[2] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Channel_Config"
    temp[3] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Adc_Output_Config"
    temp[4] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Low_Power_Mode_Cfg"
    temp[5] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Async_Event_Config"
    temp[6] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Tx_Freq_Pwr_Limit_Cfg"
    temp[7] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Init_Calib_Config"
    temp[8] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Init"
    temp[9] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Continuous_Mode_Cfg"
    temp[10] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Misc_Config"
    temp[11] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Idletimevar_Lut_Cfg"
    temp[12] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Bpm_Lut_Config"
    temp[13] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Advance_Chirp_Config"
    temp[14] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Dyn_Pwr_Save_Cfg"
    temp[15] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Test_Source_Cfg"
    temp[16] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Test_Source_En"
    temp[17] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Advance_Frame_Config"
    temp[18] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Sensor_Start"
    temp[19] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Continuous_Mode_En"
    temp[20] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Calib_Mon_Time_Unit_Cfg"
    temp[21] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Runtime_Calib_Config"
    temp[22] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Monitoring_Cfg"
    temp[23] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Subframe_Start"
    temp[24] = "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Bootup_Status"
    time.sleep(1)
    for i in range(0, len(temp)) :
        T32_R5A.add_var_watch(temp[i])
        time.sleep(1)
        TEMP[i] = T32_R5A.read_var(temp[i])
        # Skip variables that should be 0: indices 4, 9, 15, 16, 19
        if i not in [4, 9, 15, 16, 19]:
            counter = counter + TEMP[i]
        print(temp[i], "=", TEMP[i])
    print("")
    print("-----------------------------------WI-315881 :END-------------------------------------")
    condition = 0
    if counter == 20 and flag == 1:
        print("Passed")
        condition = 1
    else :
        condition = 0
        print("failed")
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
