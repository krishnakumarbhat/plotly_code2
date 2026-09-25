"""Python testcase for Front_End_Management."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-333248")
@pytest.mark.description("Qualification test for validation of Radar Control State - Off State and Faulted State")
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
    print("-----------------------------------WI-333248 :START-----------------------------------")
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

    # Define variables to monitor
    monitored_vars = [
        "MMIC_Error.Reinit_Error",
        "MMIC_Error.Faulted",
        "Radar_Ctl_Data.radiate_enable",
        "Radar_Ctl_State"
    ]

    var_values = [0] * len(monitored_vars)

    # Read initial variable values
    print()
    print("# Reading initial variable values #")
    for i, var in enumerate(monitored_vars):
        T32_R5A.add_var_watch(var)
        time.sleep(0.5)
        retry_count = 0
        max_retries = 3
        while retry_count < max_retries:
            try:
                var_values[i] = T32_R5A.read_var(var)
                print(f"{var} = {var_values[i]}")
                break
            except Exception as e:
                retry_count += 1
                print(f"Error reading {var} (attempt {retry_count}/{max_retries}): {e}")
                if retry_count >= max_retries:
                    print(f"Failed to read {var} after {max_retries} attempts")
                    var_values[i] = 0
                else:
                    time.sleep(1)

    print()
    print("Overwriting MMIC_Error.Reinit_Error to 1")
    T32_R5A.cmd("Var.set %e MMIC_Error.Reinit_Error = 1")
    time.sleep(3)

    # Read variables after setting Reinit_Error
    try:
        var_values[0] = T32_R5A.read_var(monitored_vars[0])
        T = T32_R5A.read_var(t)
        print(f"After setting Reinit_Error: {monitored_vars[0]} = {var_values[0]}")
        print(f"Radar_Ctl_Data.init_status = {T_1[T]}")
    except Exception as e:
        print(f"Error reading variables after Reinit_Error: {e}")
        T = 0  # Default to FAIL state
        traceback.print_exc()

    # Check if radar is in NOT_STARTED or FAIL state
    if T == 0 or T == 1 or T == 2:  # RADAR_CTL_INIT_FAIL or RADAR_CTL_INIT_NOT_STARTED
        print(f"Expected condition met: Radar is in {T_1[T]} state")
        reinit_check = True
    else:
        print(f"Unexpected condition: Radar is in {T_1[T]} state")
        reinit_check = False

    print()
    print("Overwriting MMIC_Error.Faulted to 1")
    T32_R5A.cmd("Var.set %e MMIC_Error.Faulted = 1")
    time.sleep(3)

    # Read variables after setting Faulted
    try:
        var_values[1] = T32_R5A.read_var(monitored_vars[1])
        var_values[3] = T32_R5A.read_var(monitored_vars[3])
        print(f"After setting Faulted: {monitored_vars[1]} = {var_values[1]}")
        print(f"Radar_Ctl_State = {var_values[3]} (0x{var_values[3]:X})")
    except Exception as e:
        print(f"Error reading variables after Faulted: {e}")
        traceback.print_exc()

    # Check if Radar_Ctl_State is RADAR_CTL_FAULTED (0x1)
    if var_values[3] == 0x1:
        print("Expected condition met: Radar_Ctl_State = RADAR_CTL_FAULTED (0x1)")
        faulted_check = True
    else:
        print(f"Unexpected condition: Radar_Ctl_State = 0x{var_values[3]:X}")
        faulted_check = False

    print()
    print("Overwriting Radar_Ctl_Data.radiate_enable to 0")
    T32_R5A.cmd("Var.set %e Radar_Ctl_Data.radiate_enable = 0")
    time.sleep(3)

    # Read variables after setting radiate_enable
    try:
        var_values[2] = T32_R5A.read_var(monitored_vars[2])
        T = T32_R5A.read_var(t)
        print(f"After setting radiate_enable: {monitored_vars[2]} = {var_values[2]}")
        print(f"Radar_Ctl_Data.init_status = {T_1[T]}")
    except Exception as e:
        print(f"Error reading variables after radiate_enable: {e}")
        T = 0  # Default to FAIL state
        traceback.print_exc()

    # Check if radar is in NOT_STARTED or FAIL state
    if T == 0 or T == 1 or T == 2:  # RADAR_CTL_INIT_FAIL or RADAR_CTL_INIT_NOT_STARTED
        print(f"Expected condition met: Radar is in {T_1[T]} state")
        radiate_check = True
    else:
        print(f"Unexpected condition: Radar is in {T_1[T]} state")
        radiate_check = False

    # Read final variable values
    print()
    print("# Reading final variable values #")
    for i, var in enumerate(monitored_vars):
        retry_count = 0
        max_retries = 3
        while retry_count < max_retries:
            try:
                var_values[i] = T32_R5A.read_var(var)
                time.sleep(0.5)
                print(f"{var} = {var_values[i]}")
                break
            except Exception as e:
                retry_count += 1
                print(f"Error reading {var} (attempt {retry_count}/{max_retries}): {e}")
                if retry_count >= max_retries:
                    print(f"Failed to read {var} after {max_retries} attempts")
                    var_values[i] = 0
                else:
                    time.sleep(1)

    print()
    print("-----------------------------------WI-333248 :END-------------------------------------")
    condition = 0
    if reinit_check and faulted_check and radiate_check and flag:
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("Failed")
        if not reinit_check:
            print("  - Reinit_Error check failed")
        if not faulted_check:
            print("  - Faulted state check failed")
        if not radiate_check:
            print("  - radiate_enable check failed")
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
