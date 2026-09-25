"""Python testcase for Measurement_Monitor."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-335367")
@pytest.mark.description("Qualification test validation for RF Health Monitor Weightage Calculation - Negative test case")
def test_Front_End_Management(Power, T32_R5A, T32_C66, Report):
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
    print("-----------------------------------WI-335367 :START-----------------------------------")
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

    # Define MMIC_RadarAPIs_Status variables to monitor
    mmic_status_vars = [
        "MMIC_RadarAPIs_Status.temperature_max_err",
        "MMIC_RadarAPIs_Status.synth_freq_err",
        "MMIC_RadarAPIs_Status.tx0_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx1_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx2_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx3_ball_brk_err",
        "MMIC_RadarAPIs_Status.tx0_pow_err",
        "MMIC_RadarAPIs_Status.tx1_pow_err",
        "MMIC_RadarAPIs_Status.tx2_pow_err",
        "MMIC_RadarAPIs_Status.tx3_pow_err",
        "MMIC_RadarAPIs_Status.supply_pmclklo_err",
        "MMIC_RadarAPIs_Status.dcbias_pmclklo_err",
        "MMIC_RadarAPIs_Status.lvds_pmclklo_err",
        "MMIC_RadarAPIs_Status.supply_tx0_err",
        "MMIC_RadarAPIs_Status.supply_tx1_err",
        "MMIC_RadarAPIs_Status.supply_tx2_err",
        "MMIC_RadarAPIs_Status.supply_tx3_err",
        "MMIC_RadarAPIs_Status.dcbias_tx0_err",
        "MMIC_RadarAPIs_Status.dcbias_tx1_err",
        "MMIC_RadarAPIs_Status.dcbias_tx2_err",
        "MMIC_RadarAPIs_Status.dcbias_tx3_err",
        "MMIC_RadarAPIs_Status.supply_rx0_err",
        "MMIC_RadarAPIs_Status.supply_rx1_err",
        "MMIC_RadarAPIs_Status.supply_rx2_err",
        "MMIC_RadarAPIs_Status.supply_rx3_err",
        "MMIC_RadarAPIs_Status.dcbias_rx0_err",
        "MMIC_RadarAPIs_Status.dcbias_rx1_err",
        "MMIC_RadarAPIs_Status.dcbias_rx2_err",
        "MMIC_RadarAPIs_Status.dcbias_rx3_err",
        "MMIC_RadarAPIs_Status.apll_vctrl_err",
        "MMIC_RadarAPIs_Status.synth_vco1_vctrl_max_freq_err",
        "MMIC_RadarAPIs_Status.synth_vco1_vctrl_min_freq_err",
        "MMIC_RadarAPIs_Status.rx_gain_mismatch_err",
        "MMIC_RadarAPIs_Status.rx_ph_mismatch_err",
        "MMIC_RadarAPIs_Status.rx_hpf_err",
        "MMIC_RadarAPIs_Status.rx_lpf_err",
        "MMIC_RadarAPIs_Status.rx_ifa_gain_err",
        "MMIC_RadarAPIs_Status.tx_gain_mismatch_err",
        "MMIC_RadarAPIs_Status.tx_ph_mismatch_err",
        "MMIC_RadarAPIs_Status.tx0_phshift_phase_err",
        "MMIC_RadarAPIs_Status.tx1_phshift_phase_err",
        "MMIC_RadarAPIs_Status.tx2_phshift_phase_err",
        "MMIC_RadarAPIs_Status.tx3_phshift_phase_err",
        "MMIC_RadarAPIs_Status.tx0_phshift_amp_err",
        "MMIC_RadarAPIs_Status.tx1_phshift_amp_err",
        "MMIC_RadarAPIs_Status.tx2_phshift_amp_err",
        "MMIC_RadarAPIs_Status.tx3_phshift_amp_err",
        "MMIC_RadarAPIs_Status.tx0_dac_phshift_err",
        "MMIC_RadarAPIs_Status.tx1_dac_phshift_err",
        "MMIC_RadarAPIs_Status.tx2_dac_phshift_err",
        "MMIC_RadarAPIs_Status.tx3_dac_phshift_err",
        "MMIC_RadarAPIs_Status.periodic_config_reg_read_err",
        "MMIC_RadarAPIs_Status.clk_pair0_err",
        "MMIC_RadarAPIs_Status.clk_pair1_err",
        "MMIC_RadarAPIs_Status.clk_pair2_err",
        "MMIC_RadarAPIs_Status.clk_pair3_err",
        "MMIC_RadarAPIs_Status.clk_pair4_err",
        "MMIC_RadarAPIs_Status.temperature_sensor_consistency_check_err",
        "MMIC_RadarAPIs_Status.supply_gpadc_ref1_err",
        "MMIC_RadarAPIs_Status.supply_gpadc_ref2_err",
        "MMIC_RadarAPIs_Status.supply_gpadc_ref3_err",
        "MMIC_RadarAPIs_Status.rx_abs_gain_err"
    ]

    # Define RF Health status variables to monitor
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
    # Set breakpoint and run
    print("Set breakpoint and run")
    command = "Populate_MMIC_Stream+0xB90"
    T32_R5A.set_breakpoint(command)
    time.sleep(2)
    T32_R5A.cmd("Go")
    time.sleep(2)

    if T32_R5A.get_run_state() == 2:
        print("Breakpoint hit at Populate_MMIC_Stream+0xB90")

        # Check MMIC status values at breakpoint
        print()
        print("# Checking MMIC_RadarAPIs_Status values at breakpoint #")
        error_count = 0
        for var in mmic_status_vars:
            T32_R5A.add_var_watch(var)
            val = T32_R5A.read_var(var)
            print(f"{var} = {val}")
            if val != 0:
                error_count += 1

        if error_count == 0:
            print(f"All {len(mmic_status_vars)} MMIC status variables = 0")
            mmic_check = True
        else:
            print(f"ERROR: {error_count} variables not equal to 0")
            mmic_check = False

        # Continue execution
        T32_R5A.delete_all_breakpoints()
        T32_C66.delete_all_breakpoints()
        time.sleep(2)
        T32_R5A.cmd("Go")
        time.sleep(2)

        # Check RF Health status values after Go
        print()
        print("# Checking RF Health status values after Go #")
        rfhealth_vals = []
        for var in rfhealth_status_vars:
            T32_R5A.add_var_watch(var)
            val = T32_R5A.read_var(var)
            rfhealth_vals.append(val)
            print(f"{var} = {val}")

        # Validate RF Health values
        if (rfhealth_vals[0] == 0 or 1
                and rfhealth_vals[1] == 1
                and rfhealth_vals[2] == 8191
                and rfhealth_vals[3] == 1):
            print("RF Health status values are as expected")
            rfhealth_check = True
        else:
            print("ERROR: RF Health status values do not match")
            rfhealth_check = False

        condition = mmic_check and rfhealth_check and flag
    else:
        print("ERROR: Breakpoint was not hit")
        condition = False

    print()
    print("-----------------------------------WI-335367 :END-------------------------------------")
    if condition:
        print("Passed")
    else:
        print("Failed")
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
