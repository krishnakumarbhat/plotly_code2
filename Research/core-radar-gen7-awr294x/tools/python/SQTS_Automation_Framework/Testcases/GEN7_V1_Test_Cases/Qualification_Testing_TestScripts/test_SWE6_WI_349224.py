"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_349224")
@pytest.mark.description("Qualification test validation for MMIC Error Mapping from Asynchronous events and diagnosis performed during boot sequence")
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
    print("-----------------------------------WI-349224 :START-----------------------------------")
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

    # Define variable lists
    fault_api_vars = [
        "MMIC_Fault_API_Status.pui_abort_err",
        "MMIC_Fault_API_Status.pip_abort_err",
        "MMIC_Fault_API_Status.pda_abort_err",
        "MMIC_Fault_API_Status.pff_abort_err",
        "MMIC_Fault_API_Status.boot_rom_crc_err",
        "MMIC_Fault_API_Status.boot_cr4_vim_lockstep_err",
        "MMIC_Fault_API_Status.boot_vim_test_err",
        "MMIC_Fault_API_Status.boot_stc_diag_err",
        "MMIC_Fault_API_Status.boot_cr4_stc_err",
        "MMIC_Fault_API_Status.boot_crc_test_err",
        "MMIC_Fault_API_Status.boot_rampgen_mem_ecc_err",
        "MMIC_Fault_API_Status.boot_dfe_mem_ecc_err",
        "MMIC_Fault_API_Status.boot_rampgen_lockstep_err",
        "MMIC_Fault_API_Status.boot_frc_lockstep_err",
        "MMIC_Fault_API_Status.boot_dfe_mem_pbist_err",
        "MMIC_Fault_API_Status.boot_rampgen_mem_pbist_err",
        "MMIC_Fault_API_Status.boot_pbist_test_err",
        "MMIC_Fault_API_Status.boot_wdt_test_err",
        "MMIC_Fault_API_Status.boot_esm_test_err",
        "MMIC_Fault_API_Status.boot_dfe_stc_err",
        "MMIC_Fault_API_Status.boot_frc_test_err",
        "MMIC_Fault_API_Status.boot_atcm_btcm_ecc_err",
        "MMIC_Fault_API_Status.boot_atcm_btcm_parity_err",
        "MMIC_Fault_API_Status.boot_dcc_err",
        "MMIC_Fault_API_Status.boot_socc_test_err",
        "MMIC_Fault_API_Status.boot_gpadc_test_err",
        "MMIC_Fault_API_Status.boot_fft_test_err",
        "MMIC_Fault_API_Status.boot_rti_test_err",
        "MMIC_Fault_API_Status.boot_pcr_test_err",
        "MMIC_Fault_API_Status.boot_bus_safety_err",
        "MMIC_Fault_API_Status.boot_ecc_aggregator_err",
        "MMIC_Fault_API_Status.boot_mpu_test_err"
    ]

    # Read and sum fault API status values
    counter_1 = 0
    print("Reading MMIC_Fault_API_Status variables:")
    for var in fault_api_vars:
        value = T32_R5A.read_var(var)
        counter_1 += value
        print(f"{var} = {value}")

    radar_api_vars = [
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

    # Read and sum radar API status values
    counter_2 = 0
    print("\nReading MMIC_RadarAPIs_Status variables:")
    for var in radar_api_vars:
        value = T32_R5A.read_var(var)
        counter_2 += value
        print(f"{var} = {value}")

    print("")
    print("-----------------------------------WI-349224 :END-------------------------------------")
    if counter_1 == 0 and counter_2 == 3 and flag:
        print("Passed")
        assert True
    else:
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
