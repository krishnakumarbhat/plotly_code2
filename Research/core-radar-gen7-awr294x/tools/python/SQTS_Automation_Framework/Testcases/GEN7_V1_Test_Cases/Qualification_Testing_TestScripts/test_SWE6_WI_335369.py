"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-335369")
@pytest.mark.description("Qualification test validation for RF Health Monitor Weightage Calculation - Positive Test Case")
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
    print("-----------------------------------WI-335369 :START-----------------------------------")
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

    # Create array of MMIC status variables to monitor
    mmic_status_vars = [
        "MMIC_RadarAPIs_Status.synth_freq_err",
        "MMIC_RadarAPIs_Status.tx0_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx1_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx2_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx3_ball_brk_err",
        "MMIC_RadarAPIs_Status.temperature_max_err",
        "MMIC_RadarAPIs_Status.tx0_pow_err",
        "MMIC_RadarAPIs_Status.tx1_pow_err",
        "MMIC_RadarAPIs_Status.tx2_pow_err",
        "MMIC_RadarAPIs_Status.tx3_pow_err",
        "MMIC_RadarAPIs_Status.supply_pmclklo_err",
        "MMIC_RadarAPIs_Status.rx_gain_mismatch_err",
        "MMIC_RadarAPIs_Status.rx_ph_mismatch_err",
        "MMIC_RadarAPIs_Status.apll_vctrl_err"
    ]

    # Create array of RF Health status variables to monitor
    rfhealth_status_vars = [
        "Wdt_Stop_Start_Test",
        "((*(SMC_Cal_Data_Ptr)).Radar_Health_Cal).k_rfhealth_algo_enable_str",
        "((*(SMC_Cal_Data_Ptr)).Radar_Health_Cal).k_rfhealth_monitor_enable_str",
        "RFHealth_EnableStatusFlag",
        "RFHealthPercentage"
    ]

    # Read initial values
    print()
    print("# Reading initial variable values #")
    print(f"Total MMIC status variables: {len(mmic_status_vars)}")
    print(f"Total RF Health status variables: {len(rfhealth_status_vars)}")

    # Overwrite Wdt_Stop_Start_Test to 1
    print()
    print("Overwriting Wdt_Stop_Start_Test to 1")
    T32_R5A.cmd("Var.set %e Wdt_Stop_Start_Test = 1")
    time.sleep(2)

    # Set breakpoint at start of Populate_MMIC_Stream
    print()
    command = "Populate_MMIC_Stream"
    T32_R5A.set_breakpoint(command)
    T32_R5A.cmd("Go")
    time.sleep(2)
    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit at the start of the function Populate_MMIC_Stream")
    else:
        print("Breakpoint is not hit at the start of the function Populate_MMIC_Stream")

    # Set breakpoint at end of Populate_MMIC_Stream
    print()
    command = "Populate_MMIC_Stream+0xB90"
    T32_R5A.set_breakpoint(command)
    T32_R5A.cmd("Go")
    time.sleep(2)
    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit at the end of the function Populate_MMIC_Stream")
    else:
        print("Breakpoint is not hit at the end of the function Populate_MMIC_Stream")

    # Check all MMIC status variables should be 0
    print()
    print("Checking MMIC status variables:")
    error_count = 0
    for var in mmic_status_vars:
        T32_R5A.add_var_watch(var)
        val = T32_R5A.read_var(var)
        if val != 0:
            print(f"{var} = {val} (Expected 0)")
            error_count += 1

    if error_count == 0:
        print(f"All {len(mmic_status_vars)} MMIC status variables = 0")
        mmic_check = True
    else:
        print(f"ERROR: {error_count} variables not equal to 0")
        mmic_check = False

    # Check RF Health status variables
    print()
    print("Checking RF Health status variables:")
    rfhealth_values = []
    for var in rfhealth_status_vars:
        T32_R5A.add_var_watch(var)
        val = T32_R5A.read_var(var)
        rfhealth_values.append(val)
        print(f"{var} = {val}")

    # Validate RF Health values
    # Expected: Wdt_Stop_Start_Test=1, k_rfhealth_algo_enable_str=1, k_rfhealth_monitor_enable_str=8191, RFHealth_EnableStatusFlag=TRUE(1)
    if (rfhealth_values[0] == 1
            and rfhealth_values[1] == 1
            and rfhealth_values[2] == 8191 and rfhealth_values[3] == 1):
        print("RF Health status variables have expected values")
        rfhealth_check = True
    else:
        print("ERROR: RF Health status variables do not match expected values")
        print("Expected: Wdt_Stop_Start_Test=1, k_rfhealth_algo_enable_str=1, k_rfhealth_monitor_enable_str=8191, RFHealth_EnableStatusFlag=1")
        rfhealth_check = False

    # Delete breakpoints and continue execution
    T32_R5A.delete_all_breakpoints()
    T32_R5A.cmd("Go")
    time.sleep(5)

    # Check additional RF Health variables after execution
    print()
    print("Checking additional RF Health variables:")
    T32_R5A.add_var_watch("SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_monitor_enable_str")
    val1 = T32_R5A.read_var("SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_monitor_enable_str")
    print(f"SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_monitor_enable_str = {val1}")

    T32_R5A.add_var_watch("SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_algo_enable_str")
    val2 = T32_R5A.read_var("SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_algo_enable_str")
    print(f"SMC_Cal_Data.Radar_Health_Cal.k_rfhealth_algo_enable_str = {val2}")

    T32_R5A.add_var_watch("MMIC_Stream.MMIC_Data.RFHealth_Status.RFHealthPercentage")
    val3 = T32_R5A.read_var("MMIC_Stream.MMIC_Data.RFHealth_Status.RFHealthPercentage")
    print(f"MMIC_Stream.MMIC_Data.RFHealth_Status.RFHealthPercentage = {val3}")

    if val1 == 8191 and val2 == 1:
        status_2 = 1
    else:
        status_2 = 0

    if val3 > 80:
        status_3 = 1
    else:
        status_3 = 0
    print("")
    print("-----------------------------------WI-335369 :END-------------------------------------")
    time.sleep(2)
    condition = 0
    if mmic_check and rfhealth_check and status_2 == 1 and status_3 == 1 and flag:
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
