"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_351613")
@pytest.mark.description("Qualification test for validation of ADC Peripheral Initialization Failure - Negative Test case")
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
    print("-----------------------------------WI-351613 :START-----------------------------------")
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

    variables = [
        "Radar_Ctl_Data.radiate_enable",
        "XCP_MMIC_FIT_Enable",
        "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Startup_Error.mmic_adc_buf_init_fail_flag",
        "Platform_Active_Fault_Table.platform_bits.mmic_safe_start_fault"
    ]
    values = []

    for var in variables:
        T32_R5A.add_var_watch(var)
        value = T32_R5A.read_var(var)
        values.append(value)
        print(var, "=", value)

    temp_2, temp_4, temp_5 = values[0], values[2], values[3]

    T32_R5A.cmd("Var.set %e XCP_MMIC_FIT_Enable = XCP_MMIC_ADCBuf_Init_FIT")
    print("")
    print("Overwriting XCP_MMIC_FIT_Enable to XCP_MMIC_ADCBuf_Init_FIT")
    print("")
    time.sleep(5)

    # Re-read variables after command execution
    temp_4 = T32_R5A.read_var(variables[2])
    T = T32_R5A.read_var(t)
    temp_2 = T32_R5A.read_var(variables[0])
    temp_5 = T32_R5A.read_var(variables[3])

    print(variables[2], "=", temp_4)
    print(t, "=", T_1[T])
    print(variables[0], "=", temp_2)
    print(variables[3], "=", temp_5)
    time.sleep(3)
    T32_R5A.delete_all_breakpoints()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Down")
    time.sleep(2)
    try:
        T32_R5A.cmd("SYStem.Up")
    except Exception:
        print("An exception occurred R5A")
        traceback.print_exc()
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
    except Exception:
        print("An exception occurred C66")
        traceback.print_exc()
    time.sleep(5)
    print("")
    print("-----------------------------------WI-351613 :END-------------------------------------")
    if temp_4 == 170 and (T == 0 or T == 1) and flag:
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
# 09/12/2025   AZEEJ SHAIK        EAH-7440             Script correction
