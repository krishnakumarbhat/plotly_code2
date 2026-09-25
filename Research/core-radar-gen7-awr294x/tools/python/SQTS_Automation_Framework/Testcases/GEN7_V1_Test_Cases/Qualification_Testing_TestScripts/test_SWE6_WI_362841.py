"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_362841")
@pytest.mark.description("Qualification Test for MMIC function configuration fault - Negative Test Case")
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

    print("-----------------------------------WI-362841 :START-----------------------------------")
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
    try:
        T = T32_R5A.read_var(t)
        print("")
        print(t, "=", T_1[T])
    except Exception as e:
        print(f"Error reading radar status: {e}")
        traceback.print_exc()
        T = 0
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
            try:
                T = T32_R5A.read_var(t)
                print("After power cycle:")
                print(t, "=", T_1[T])
                print()
            except Exception as e:
                print(f"Error reading radar status after power cycle: {e}")
                traceback.print_exc()
                T = 0
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
        print("Passed")
        condition = 1
    else :
        condition = 0
        print("failed")
    assert condition

    # Define variables to monitor
    variables = [
        "Radar_Ctl_Data.radiate_enable",
        "XCP_MMIC_FIT_Enable",
        "MMIC_Reconfig",
        "MMIC_Error.Faulted",
        "MMIC_Error.Reinit_Error",
        "MMIC_Error.Look_Stale_Fault",
        "MMIC_Error.Function_Config_Fault",
        "MMIC_Error.Reinit_Error_Count",
        "MMIC_Error.Reinit_Clear_Count",
        "MMIC_Error.Reinit_Error_Total",
        "MMIC_Error.Discard_Error",
        "MMIC_Error.Discard_Error_Count",
        "MMIC_Error.Discard_Error_Total",
        "MMIC_Error.MMIC_Config_Error",
        "MMIC_Error.MMIC_Config_Error_Count",
        "MMIC_Error.MMIC_Config_Error_Total"
    ]

    # Read initial values
    print("")
    print("Initial values:")
    initial_values = []
    for var in variables:
        val = T32_R5A.read_var(var)
        initial_values.append(val)
        print(f"{var} = {val}")

    print("")
    T32_R5A.cmd("Var.set %e XCP_MMIC_FIT_Enable = XCP_MMIC_Config_FIT")
    print("Writing the variable value as XCP_MMIC_Config_FIT for XCP_MMIC_FIT_Enable")
    T32_R5A.cmd("Var.set %e MMIC_Reconfig = 1")
    print("Writing the variable value as 1 for MMIC_Reconfig")
    time.sleep(2)

    # Read values after setting
    print("")
    print("Values after setting XCP_MMIC_Config_FIT and MMIC_Reconfig:")
    final_values = []
    for var in variables:
        val = T32_R5A.read_var(var)
        final_values.append(val)
        print(f"{var} = {val}")

    # Validate conditions
    # final_values indices: 0=radiate_enable, 1=XCP_MMIC_FIT_Enable, 2=MMIC_Reconfig,
    # 3=Faulted, 4=Reinit_Error, 5=Look_Stale_Fault, 6=Function_Config_Fault,
    # 7=Reinit_Error_Count, 8=Reinit_Clear_Count, 9=Reinit_Error_Total,
    # 10=Discard_Error, 11=Discard_Error_Count, 12=Discard_Error_Total,
    # 13=MMIC_Config_Error, 14=MMIC_Config_Error_Count, 15=MMIC_Config_Error_Total

    final_condition = 0
    if (final_values[6] == 170  # Function_Config_Fault = 170
            and final_values[11] > 6     # Discard_Error_Count > 6
            and final_values[12] > 6     # Discard_Error_Total > 6
            and final_values[14] > 6     # MMIC_Config_Error_Count > 6
            and final_values[15] > 6):   # MMIC_Config_Error_Total > 6
        print("Passed")
        final_condition = 1
    else:
        final_condition = 0
        print("failed")
    print("-----------------------------------WI-362841 :END-------------------------------------")
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
