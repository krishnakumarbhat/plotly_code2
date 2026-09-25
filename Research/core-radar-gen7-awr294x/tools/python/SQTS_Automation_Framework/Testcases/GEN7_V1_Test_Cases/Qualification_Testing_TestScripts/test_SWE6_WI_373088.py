"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-373088")
@pytest.mark.description("Qualification Test Validation For MMIC Data Acquisition Overflow fault - Positive Test Case")
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
    print("-----------------------------------WI-373088 :START-----------------------------------")
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

    # Define MMIC DAQ overflow variables to monitor
    mmic_daq_vars = [
        "MMIC_Daq_Total_Err_Cnt",
        "MMIC_Daq_Over_Flow_Error_Cnt[0]",
        "MMIC_Daq_Over_Flow_Error_Cnt[1]",
        "MMIC_Daq_Over_Flow_Error_Cnt[2]",
        "MMIC_Daq_Over_Flow_Error_Cnt[3]",
        "MMIC_Daq_Over_Flow_Error_Cnt[4]"
    ]

    # Read and validate all variables
    print()
    print("# Reading MMIC DAQ overflow variables #")
    all_values = []
    all_passed = True

    for var in mmic_daq_vars:
        retry_count = 0
        max_retries = 3
        val = None

        while retry_count < max_retries:
            try:
                T32_R5A.add_var_watch(var)
                time.sleep(0.5)
                val = T32_R5A.read_var(var)
                all_values.append(val)
                print(f"{var} = {val}")
                break
            except Exception as e:
                retry_count += 1
                if retry_count >= max_retries:
                    print(f"Error reading {var} (attempt {retry_count}/{max_retries}): {e}")
                    print(f"Failed to read {var} after {max_retries} attempts")
                    all_values.append(-1)  # Mark as failed
                    all_passed = False
                else:
                    time.sleep(1)

    # Validate all values should be 0
    print()
    print("# Validating all variables should be 0 #")
    condition = True
    for i, var in enumerate(mmic_daq_vars):
        if i < len(all_values):
            if all_values[i] == 0:
                print(f"{var} = {all_values[i]} - PASS")
            else:
                print(f"{var} = {all_values[i]} (Expected 0) - FAIL")
                condition = False

    print("")
    print("-----------------------------------WI-373088 :END-------------------------------------")

    final_condition = condition and all_passed and flag
    if final_condition:
        print("Passed")
    else:
        print("Failed")

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
