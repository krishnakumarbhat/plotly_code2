"""Python testcase for Measurement_Monitor."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-375163")
@pytest.mark.description("Qualification test for Negative validation of Degraded mode(Over Temperature Handling)")
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
    print("-----------------------------------WI-375163 :START-----------------------------------")
    # Checking Radar Status
    T_1 = []
    T_1.append("RADAR_CTL_INIT_FAIL")
    T_1.append("RADAR_CTL_INIT_NOT_STARTED")
    T_1.append("RADAR_CTL_INIT_STARTED")
    T_1.append("RADAR_CTL_INIT_SUCCESS")
    T_2 = []
    T_2.append("RADAR_CTL_OFF")
    T_2.append("RADAR_CTL_FAULTED")
    T_2.append("RADAR_CTL_INIT")
    T_2.append("RADAR_CTL_READY")
    T_2.append("RADAR_CTL_CONFIGURED")
    T_2.append("RADAR_CTL_TRANSMITTING")
    T_2.append("RADAR_CTL_A2D_INJECTION")
    T_2.append("RADAR_CTL_PURE_CW")
    T_2.append("RADAR_CTL_DEGRADED")
    T_2.append("RADAR_CTL_IDLE")
    T_2.append("RADAR_CTL_NUM_STATES")
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
        final_condition = 1
    else:
        final_condition = 0
    if final_condition == 1:
        print("-----------------------------------WI-375163 :START-----------------------------------")
        temp = ["XCP_MMIC_FIT_Enable", "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.Over_Temp_DegC",
                "MMIC_Stream.MMIC_Data.MMIC_Over_Temperature_Handle.XCP_Over_Temperature_Api_Counter",
                "Radar_Ctl_State"]

        # Add variable watches
        for var in temp:
            T32_R5A.add_var_watch(var)

        print("Overwritting XCP_MMIC_FIT_Enable to XCP_MMIC_OT_Degraded_FIT")
        T32_R5A.cmd("Var.set %e XCP_MMIC_FIT_Enable = 5")
        time.sleep(1)

        # Read and display initial values
        xcp_fit_enable = T32_R5A.read_var(temp[0])
        over_temp_degc = T32_R5A.read_var(temp[1])
        api_counter = T32_R5A.read_var(temp[2])
        radar_state = T32_R5A.read_var(temp[3])

        print(f"{temp[0]} = {xcp_fit_enable}")
        print(f"{temp[1]} = {over_temp_degc}")
        print(f"{temp[2]} = {api_counter}")
        print(f"{temp[3]} = {T_2[radar_state]}")

        if xcp_fit_enable == 5 and over_temp_degc >= 139 and api_counter > 0 and radar_state == 8:
            print("After changing XCP_MMIC_FIT_Enable to XCP_MMIC_OT_Degraded_FIT, XCP_Over_Temperature_Api_Counter is incrementing")
            degraded_condition = 1
        else:
            print("After changing XCP_MMIC_FIT_Enable to XCP_MMIC_OT_Degraded_FIT, XCP_Over_Temperature_Api_Counter is not incrementing")
            degraded_condition = 0

        # Wait for counter to reach zero (threshold limit)
        time.sleep(5)
        attempts = 0
        while api_counter != 0 and attempts < 8:
            time.sleep(5)
            attempts += 1
            api_counter = T32_R5A.read_var(temp[2])

        print("")
        print("##After XCP_Over_Temperature_Api_Counter reached the threshold limit 300##")

        # Read and display final values
        xcp_fit_enable = T32_R5A.read_var(temp[0])
        over_temp_degc = T32_R5A.read_var(temp[1])
        api_counter = T32_R5A.read_var(temp[2])
        radar_state = T32_R5A.read_var(temp[3])

        print(f"{temp[0]} = {xcp_fit_enable}")
        print(f"{temp[1]} = {over_temp_degc}")
        print(f"{temp[2]} = {api_counter}")
        print(f"{temp[3]} = {T_2[radar_state]}")

        if radar_state == 4 or radar_state == 5:
            normal_condition = 1
        else:
            normal_condition = 0

        final_condition = degraded_condition and normal_condition
        print("")
        print("-----------------------------------WI-375163 :END-------------------------------------")
    else:
        print("Execution is stopped, Radar Status is ", T_1[T])
    assert final_condition
##############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
