"""Python testcase for Measurement_Monitor."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-361460")
@pytest.mark.description("Qualification test validation for CPU Errors - negative test case")
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
    print("-----------------------------------WI-361460 :START-----------------------------------")
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

    # Define CPU fault variables to monitor
    cpu_fault_vars = [
        "Wdt_Stop_Start_Test",
        "RF_Mon_Report.cpu_fault_report.faultType",
        "Async_Fault_Flag.cpu_fault_flag"
    ]

    # Read initial values
    print()
    print("# Reading initial variable values #")
    for var in cpu_fault_vars:
        T32_R5A.add_var_watch(var)

    # Overwrite variables to create CPU error
    print()
    print("Overwriting Wdt_Stop_Start_Test to 1")
    T32_R5A.cmd("Var.set %e Wdt_Stop_Start_Test = 1")

    print("Overwriting RF_Mon_Report.cpu_fault_report.faultType to 1 to create cpu_err")
    T32_R5A.cmd("Var.set %e RF_Mon_Report.cpu_fault_report.faultType = 1")

    print("Overwriting Async_Fault_Flag.cpu_fault_flag to 170")
    T32_R5A.cmd("Var.set %e Async_Fault_Flag.cpu_fault_flag = 170")

    # Set breakpoint and run
    print()
    command = "MMIC_Diagnostics\\99"
    T32_R5A.set_breakpoint(command)
    T32_R5A.cmd("Go")
    time.sleep(2)

    if T32_R5A.get_run_state() == 2:
        print("Breakpoint is hit at end of the function MMIC_Diagnostics()")

        # Read values at breakpoint
        print()
        print("# Reading values at breakpoint #")
        final_values = []
        for var in cpu_fault_vars:
            retry_count = 0
            max_retries = 3
            while retry_count < max_retries:
                try:
                    val = T32_R5A.read_var(var)
                    final_values.append(val)
                    print(f"{var} = {val}")
                    break
                except Exception as e:
                    retry_count += 1
                    print(f"Error reading {var} (attempt {retry_count}/{max_retries}): {e}")
                    if retry_count >= max_retries:
                        print(f"Failed to read {var} after {max_retries} attempts")
                        final_values.append(0)
                    else:
                        time.sleep(1)

        # Add and read MMIC_Errors.cpu_err (only available after breakpoint)
        print()
        print("# Reading MMIC_Errors.cpu_err (available after breakpoint) #")
        T32_R5A.add_var_watch("MMIC_Errors.cpu_err")
        retry_count = 0
        max_retries = 3
        cpu_err_val = 0
        while retry_count < max_retries:
            try:
                cpu_err_val = T32_R5A.read_var("MMIC_Errors.cpu_err")
                print(f"MMIC_Errors.cpu_err = {cpu_err_val}")
                break
            except Exception as e:
                retry_count += 1
                print(f"Error reading MMIC_Errors.cpu_err (attempt {retry_count}/{max_retries}): {e}")
                if retry_count >= max_retries:
                    print(f"Failed to read MMIC_Errors.cpu_err after {max_retries} attempts")
                    cpu_err_val = 0
                else:
                    time.sleep(1)

        # Validate MMIC_Errors.cpu_err = 170
        print()
        if cpu_err_val == 170:
            print(f"MMIC_Errors.cpu_err = {cpu_err_val} (Expected 170) - PASS")
            condition = True
        else:
            print(f"MMIC_Errors.cpu_err = {cpu_err_val} (Expected 170) - FAIL")
            condition = False
    else:
        print("Breakpoint is not hit at end of the function MMIC_Diagnostics()")
        condition = False

    print("")
    print("-----------------------------------WI-361460 :END-------------------------------------")
    time.sleep(1)

    final_condition = condition and flag
    if final_condition:
        print("Passed")
        assert True
    else:
        print("Failed")
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
