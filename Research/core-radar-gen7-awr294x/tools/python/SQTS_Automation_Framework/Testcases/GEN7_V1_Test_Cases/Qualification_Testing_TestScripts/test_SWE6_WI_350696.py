"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_350696")
@pytest.mark.description("Qualification test validation for MMIC Initialization by configuring different MMIC Attributes")
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
    print("-----------------------------------WI-350696 :START-----------------------------------")
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

    temp = [
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Device_Power_On",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Get_Version",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Channel_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Adc_Output_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Low_Power_Mode_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Async_Event_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Tx_Freq_Pwr_Limit_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Init_Calib_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Init",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Continuous_Mode_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Misc_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Idletimevar_Lut_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Bpm_Lut_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Advance_Chirp_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Dyn_Pwr_Save_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Test_Source_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Test_Source_En",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Advance_Frame_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Sensor_Start",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Continuous_Mode_En",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Calib_Mon_Time_Unit_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Runtime_Calib_Config",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Monitoring_Cfg",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Subframe_Start",
        "MMIC_Stream.MMIC_Data.MMIC_Config_Status.Rf_Bootup_Status"
    ]

    TEMP = []
    counter = 0
    time.sleep(1)
    for i in range(len(temp)):
        T32_R5A.add_var_watch(temp[i])
        time.sleep(1)
        value = T32_R5A.read_var(temp[i])
        TEMP.append(value)
        if i not in [9, 15, 16, 19]:
            counter += value
            print(temp[i], "=", value)
    print("")
    print("-----------------------------------WI-350696 :END-------------------------------------")
    if counter == 21 and flag:
        print("Passed")
        assert True
    else :
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
