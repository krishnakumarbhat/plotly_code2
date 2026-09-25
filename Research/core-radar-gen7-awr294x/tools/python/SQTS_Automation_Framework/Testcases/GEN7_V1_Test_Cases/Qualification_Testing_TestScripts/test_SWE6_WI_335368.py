"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_335368")
@pytest.mark.description("Qualification test validation for RF Health Monitor Enable - Negative Test Case")
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
    print("-----------------------------------WI-335368 :START-----------------------------------")
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

    # Create array of RF Health variables to monitor
    rfhealth_vars = [
        "Wdt_Stop_Start_Test",
        "SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_algo_enable_str",
        "MMIC_Stream.MMIC_Data.RFHealth_Status.RFHealth_EnableStatusFlag",
        "RFHealth_EnableStatusFlag"
    ]

    # Read initial values
    print("Reading initial values of RF Health variables:")
    initial_values = []
    for var in rfhealth_vars:
        T32_R5A.add_var_watch(var)
        value = T32_R5A.read_var(var)
        initial_values.append(value)
        print(f"{var} = {value}")
    print("")

    print("##########################################################################")
    print("Overwriting Wdt_Stop_Start_Test to 1")
    T32_R5A.cmd("Var.set %e Wdt_Stop_Start_Test = 1")
    time.sleep(2)

    T32_R5A.set_breakpoint("Populate_MMIC_Stream")
    time.sleep(5)
    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit at the start of the function Populate_MMIC_Stream")
    else:
        print("Breakpoint is not hit at the start of the function Populate_MMIC_Stream")
    time.sleep(5)
    print("")
    T32_R5A.delete_all_breakpoints()

    print("Overwriting the value of SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_algo_enable_str to 0")
    T32_R5A.write_var('SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_algo_enable_str', 0)
    time.sleep(5)
    T32_R5A.cmd("Go")

    command = r'Populate_MMIC_Stream+0xB90'
    T32_R5A.set_breakpoint(command)
    time.sleep(3)
    T32_R5A.cmd("Go")
    time.sleep(3)
    T32_R5A.cmd("Go")
    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit at the end of the function Populate_MMIC_Stream")
    else:
        print("Breakpoint is not hit at the end of the function Populate_MMIC_Stream")

    # Read values after breakpoint and verify they are 0
    print("")
    print("Reading values after breakpoint:")
    final_values = []
    for var in rfhealth_vars:
        value = T32_R5A.read_var(var)
        final_values.append(value)
        print(f"{var} = {value}")
    print("")
    print("-----------------------------------WI-335368 :END-------------------------------------")
    time.sleep(2)

    # Verify: All values should be 0 after the second Go command
    condition = 0
    if final_values[0] == 0 and final_values[1] == 0 and final_values[2] == 0 and final_values[3] == 0 and flag == 1:
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    assert condition
##############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440             Script correction
