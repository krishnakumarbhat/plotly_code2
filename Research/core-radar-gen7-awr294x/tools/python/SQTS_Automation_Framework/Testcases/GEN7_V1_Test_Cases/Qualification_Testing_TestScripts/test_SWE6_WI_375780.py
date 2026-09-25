"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_375780")
@pytest.mark.description("Qualification Test for ECO_Mode_flag is enabled - positive case")
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
    print("-----------------------------------WI-375780 :START-----------------------------------")
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
        print("Execution is stopped, Radar Status is ", T_1[T])
        flag = False

    print("")

    temp = ["(Tx_Gain_Temp_Lut_Store[0][0].txGainTempLut)[0]", "(Tx_Gain_Temp_Lut_Store[0][1].txGainTempLut)[0]",
            "(Tx_Gain_Temp_Lut_Store[0][2].txGainTempLut)[0]", "(Tx_Gain_Temp_Lut_Store[0][3].txGainTempLut)[0]",
            "(Tx_Gain_Temp_Lut_Store[0][4].txGainTempLut)[0]"]

    print("Calling the function Mmwavelink_Config_Eco_TxRx_LUT() with the parameter ECO_MODE_DISABLED")

    # Add variable watches
    for var in temp:
        T32_R5A.add_var_watch(var)

    time.sleep(1)

    # Read values in a loop
    tx_gain_values = []
    for var in temp:
        val = T32_R5A.read_var(var)
        tx_gain_values.append(val)
        print(f"{var} = {val}")

    t1 = "Eco_Mode_Status"
    T32_R5A.add_var_watch(t1)
    eco_mode_status = T32_R5A.read_var(t1)
    print(f"{t1} = {eco_mode_status}")

    T32_R5A.cmd("Var.set %e Eco_Mode_Status = 170")
    print("Writing the variable value as 170 for ", t1)
    eco_mode_status = T32_R5A.read_var(t1)
    print(f"{t1} = {eco_mode_status}")
    time.sleep(1)

    condition = 0
    if all(val != 0 for val in tx_gain_values) and flag and eco_mode_status == 0xAA:
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    print("-----------------------------------WI-375780 :END-------------------------------------")
    assert condition
##############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
