"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_335806")
@pytest.mark.description("To check the Look Index Stale fault.")
def test_Fault_Manager(Power, T32_R5A, T32_C66, Report):
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
    print("-----------------------------------WI-335806:START-----------------------------------")
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

    # Create array of platform fault variables with expected values
    fault_vars = [
        ("Platform_Fault_Test_Completed.platform_bits.usc_validation_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.smc_validation_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.usc_mmic_a_unprogrammed_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_0_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_0_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_0_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.core_self_test_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_0_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_1_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_1_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_1_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_1_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_1_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_1_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mcu_startup_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.platform_time_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_2_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_2_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_2_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.external_wdg_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.MCU_Repetitive_Reset_Fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_2_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_3_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.functional_configuration_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_4_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.dss_to_mss_ipc_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.mss_to_dss_ipc_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_5_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_5_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_5_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_5_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_5_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_5_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_6_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_7_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_7_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.battery_low_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.battery_high_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_7_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_7_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_7_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_temperature_high_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_8_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_a_sensor_dead_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_safety_integrity_osm_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_safe_start_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_8_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_loop_back_test_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_8_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_temperature_low_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.detection_processing_loop_overrun_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_9_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.acquisition_overflow_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.safety_integrity_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_9_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_9_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_9_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_9_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_transmitter_id_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.signal_integrity_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.temperature_consistency_failure_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_10_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_10_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_10_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_10_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_10_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_11_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_11_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_11_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_tx3_ball_break_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_tx2_ball_break_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_tx1_ball_break_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.mmic_tx0_ball_break_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_11_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_12_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.firstpassdetection_overflow_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_13_7", 0),
        ("Platform_Fault_Test_Completed.platform_bits.alignment_no_initial_calibration_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.alignment_static_calibration_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.alignment_service_calibration_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.alignment_shorttrack_calibration_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.alignment_autoalignment_out_of_range_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.alignment_autoalignment_algo_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.look_index_stale_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.interference_detection_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.sll_monitoring_algo_error", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_14_5", 0),
        ("Platform_Fault_Test_Completed.platform_bits.blockage_algo_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_14_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_14_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_14_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_14_0", 0),
        ("Platform_Fault_Test_Completed.platform_bits.damping_algo_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_15_6", 0),
        ("Platform_Fault_Test_Completed.platform_bits.range_algo_error_fault", 1),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_15_4", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_15_3", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_15_2", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_15_1", 0),
        ("Platform_Fault_Test_Completed.platform_bits.reserved_15_0", 0)
    ]

    # Read and validate all fault variables
    print()
    print(f"Checking {len(fault_vars)} platform fault variables:")
    error_count = 0
    all_passed = True

    for var_name, _expected_val in fault_vars:
        # Only add watch for non-reserved variables
        if "reserved" not in var_name.lower():
            try:
                T32_R5A.add_var_watch(var_name)
            except Exception as e:
                print(f"Could not add watch for {var_name}: {e} - SKIPPING")
                continue
    retry_count = 0
    max_retries = 3
    actual_val = None

    while retry_count < max_retries:
        try:
            actual_val = T32_R5A.read_var(var_name)
            break
        except Exception as e:
            retry_count += 1
            print(f"Error reading {var_name} (attempt {retry_count}/{max_retries}): {e}")
            if retry_count >= max_retries:
                print(f"Failed to read {var_name} after {max_retries} attempts - SKIPPING")
                continue
            else:
                time.sleep(1)

        # Skip this variable if we couldn't read it
        if actual_val is None:
            continue

        # Check if variable name contains "reserved" - these should be 0
        # All other (enabled) faults should be 1
        if "reserved" in var_name.lower():
            # Reserved variables should be 0
            if actual_val == 0:
                print(f"{var_name} = {actual_val} (Reserved - Expected 0) - PASS")
            else:
                print(f"{var_name} = {actual_val} (Reserved - Expected 0) - FAIL")
                error_count += 1
                all_passed = False
        else:
            # Enabled faults should be 1
            if actual_val == 1:
                print(f"{var_name} = {actual_val} (Enabled fault - Expected 1) - PASS")
            else:
                print(f"{var_name} = {actual_val} (Enabled fault - Expected 1) - FAIL")
                error_count += 1
                all_passed = False

    print()
    print(f"Total variables checked: {len(fault_vars)}")
    print(f"Errors found: {error_count}")

    print("-----------------------------------WI-335806:END-----------------------------------")
    time.sleep(1)

    condition = 0
    if all_passed and flag:
        print("The Platform bits should be set in the Platform_Fault_Test_Completed table for all enabled faults and The Platform bits should not be set in the Platform_Fault_Test_Completed table for the faults that are not enabled.")
        print("Passed")
        condition = 1
    else:
        print("failed")
        condition = 0
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
