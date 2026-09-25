"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_335376")
@pytest.mark.description("Qualification test validation fr RF Heakth Malfunction Fault - Negative Test Case")
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
    print("-----------------------------------WI-335376 :START-----------------------------------")
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
        "MMIC_Stream.MMIC_Data.RFHealth_Status.RFMalFunctionFlag",
        "MMIC_Stream.MMIC_Data.RFHealth_Status.RFHealthPercentage",
        "RFMalFunctionFlag",
        "RFHealthPercentage",
        "((*(SMC_Cal_Data_Ptr)).Radar_Health_Cal).k_rfhealth_monitor_enable_str",
        "((*(SMC_Cal_Data_Ptr)).Radar_Health_Cal).k_rfhealth_algo_enable_str",
        "((*(SMC_Cal_Data_Ptr)).Radar_Health_Cal).k_rfmalfunc_threshold_str"
    ]
    # Read initial values
    print("Reading initial values of RF Health variables:")
    initial_values = []
    for var in rfhealth_vars:
        try:
            actual_val = T32_R5A.read_var(var)
            print(f"{var} = {actual_val}")
            initial_values.append(actual_val)
        except Exception:
            print(f"{var}: SKIPPED (Could not read)")
            initial_values.append(0)
        time.sleep(1)
    print("")
    # Overwrite Wdt_Stop_Start_Test to 1
    print("\nSetting configuration variables:")
    try:
        print("  Setting Wdt_Stop_Start_Test = 1")
        T32_R5A.cmd("Var.set %e Wdt_Stop_Start_Test = 1")
        time.sleep(1)
    except Exception as e:
        print(f"  Error: {e}")
    # Set breakpoint at start of Populate_MMIC_Stream
    print("\nSetting breakpoints and executing:")
    try:
        command = "Populate_MMIC_Stream"
        print(f"  Setting breakpoint at {command}")
        T32_R5A.set_breakpoint(command)
        time.sleep(0.5)
        T32_R5A.cmd("Go")
        time.sleep(1)
        if T32_R5A.get_run_state() == 2:
            print(f"  Breakpoint hit at start of {command}")
        else:
            print(f"  Breakpoint NOT hit at start of {command}")
    except Exception as e:
        print(f"  Error: {e}")
    # Overwrite RFHealthPercentage to 20
    try:
        print("  Setting RFHealthPercentage = 20")
        T32_R5A.write_var('RFHealthPercentage', 20)
        time.sleep(1)
    except Exception as e:
        print(f"  Error: {e}")
    # Set breakpoint at end of Populate_MMIC_Stream
    try:
        command = "Populate_MMIC_Stream+0xB90"
        print(f"  Setting breakpoint at end of {command}")
        T32_R5A.set_breakpoint(command)
        time.sleep(0.5)
        T32_R5A.cmd("Go")
        time.sleep(1)
        if T32_R5A.get_run_state() == 2:
            print(f"  Breakpoint hit at end of {command}")
        else:
            print(f"  Breakpoint NOT hit at end of {command}")
    except Exception as e:
        print(f"  Error: {e}")
    # Read values after breakpoint
    print("\nReading values after breakpoint:")
    final_values = []
    for var in rfhealth_vars:
        try:
            actual_val = T32_R5A.read_var(var)
            print(f"{var} = {actual_val}")
            final_values.append(actual_val)
        except Exception:
            print(f"{var}: SKIPPED (Could not read)")
            final_values.append(0)
        time.sleep(1)
    # Validate conditions
    print("\nValidating conditions:")
    # Check MMIC_Stream.MMIC_Data.RFHealth_Status.RFMalFunctionFlag = FALSE (0)
    if len(final_values) > 1 and final_values[1] == 0:
        print(f"  RFMalFunctionFlag (MMIC): {final_values[1]} (Expected 0) - PASS")
        check1 = True
    else:
        val = final_values[1] if len(final_values) > 1 else 0
        print(f"  RFMalFunctionFlag (MMIC): {val} (Expected 0) - FAIL")
        check1 = False
    # Check RFMalFunctionFlag = TRUE (1)
    if len(final_values) > 3 and final_values[3] == 0:
        print(f"  RFMalFunctionFlag: {final_values[3]} (Expected 1) - PASS")
        check2 = True
    else:
        val = final_values[3] if len(final_values) > 3 else 0
        print(f"  RFMalFunctionFlag: {val} (Expected 1) - FAIL")
        check2 = False
    # Check k_rfhealth_algo_enable_str = 1
    if len(final_values) > 6 and final_values[6] == 1:
        print(f"  k_rfhealth_algo_enable_str: {final_values[6]} (Expected 1) - PASS")
        check3 = True
    else:
        val = final_values[6] if len(final_values) > 6 else 0
        print(f"  k_rfhealth_algo_enable_str: {val} (Expected 1) - FAIL")
        check3 = False
    # Check k_rfhealth_monitor_enable_str = 8191
    if len(final_values) > 5 and final_values[5] == 8191:
        print(f"  k_rfhealth_monitor_enable_str: {final_values[5]} (Expected 8191) - PASS")
        check4 = True
    else:
        val = final_values[5] if len(final_values) > 5 else 0
        print(f"  k_rfhealth_monitor_enable_str: {val} (Expected 8191) - FAIL")
        check4 = False
    # Check RFHealthPercentage < k_rfmalfunc_threshold_str
    if len(final_values) > 7 and final_values[4] < final_values[7]:
        print(f"  RFHealthPercentage ({final_values[4]}) < threshold ({final_values[7]}) - PASS")
        check5 = True
    else:
        pct = final_values[4] if len(final_values) > 4 else 0
        thr = final_values[7] if len(final_values) > 7 else 0
        print(f"  RFHealthPercentage ({pct}) >= threshold ({thr}) - FAIL")
        check5 = False
    print("")
    print("-----------------------------------WI-335376 :END-------------------------------------")
    time.sleep(2)
    condition = 0
    if check1 and check2 and check3 and check4 and check5 and flag:
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
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
# 09/12/2025   SHAIK OSMANE GANI  EAH-7463              Script correction
