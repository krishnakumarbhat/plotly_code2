"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-335808")
@pytest.mark.description("Test to check SafetyCritcalFaultActive UpdateFlag")
def test_FAULT_MANAGER(Power, T32_R5A, T32_C66, Report):
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

    print("-----------------------------------WI-335808 :START-----------------------------------")
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

    # Define variables for monitoring
    var_names = [
        "MMIC_Errors_Stub",
        "RadarDiagRequest.SafetyCriticalFaultActive"
    ]

    # Read variables in free run
    print("In free run monitoring:")
    T32_R5A.add_var_watch(var_names[0])
    T32_R5A.add_var_watch(var_names[1])
    mmic_error_val = T32_R5A.read_var(var_names[0])
    safety_fault_val = T32_R5A.read_var(var_names[1])
    print(f"  {var_names[0]} = {mmic_error_val}")
    print(f"  {var_names[1]} = {safety_fault_val}")

    # Check values in free run (should be 0 before modification)
    if mmic_error_val == 0 and safety_fault_val == 85:
        print("Free run values are within expected range")
    else:
        print("Initial values are not zero in free run")

    time.sleep(1)
    print("")
    print("Setting MMIC_Errors_Stub to 1 to simulate error condition")
    time.sleep(0.5)
    T32_R5A.cmd("Var.set %e MMIC_Errors_Stub = 1")
    time.sleep(2)

    # Re-read variables after modification
    mmic_error_val = T32_R5A.read_var(var_names[0])
    safety_fault_val = T32_R5A.read_var(var_names[1])
    print("After modification:")
    print(f"  {var_names[0]} = {mmic_error_val}")
    print(f"  {var_names[1]} = {safety_fault_val}")

    print("")
    print("-----------------------------------WI-335808 :END-------------------------------------")
    time.sleep(3)

    # Assertion logic
    if mmic_error_val == 1 and safety_fault_val == 170:
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
# 09/12/2025   SHAIK OSMANE GANI  EAH-7463              Script correction
