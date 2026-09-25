"""Python testcase for FRONT_END_MANAGEMENT."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_342339")
@pytest.mark.description("Qualification test for validation of Safety Integrity Fault - Negative Test Case")
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
    print("-----------------------------------WI-342339 :START-----------------------------------")
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

    # Group variable names into a list
    var_names = [
        "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Safety_Integrity_Fault.tx_int_err_cnt",
        "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Safety_Integrity_Fault.saft_int_flt_sts",
        "MMIC_Mon_Config.txn_phshift_mon_conf.txPhaseErrorThresh",
        "MMIC_Reconfig",
        "XCP_MMIC_FIT_Enable",
        "MMIC_Mon_Config.txn_phshift_mon_conf.txAmplErrorThresh"
    ]

    # Read and print initial values
    print("Initial values:")
    initial_values = []
    for var in var_names:
        value = T32_R5A.read_var(var)
        initial_values.append(value)
        print(f"{var} = {value}")

    print("Overwriting XCP_MMIC_FIT_Enable to XCP_MMIC_SensorInt_FIT for ")
    T32_R5A.cmd("Var.set %e XCP_MMIC_FIT_Enable = XCP_MMIC_SensorInt_FIT")
    time.sleep(2)
    print("Overwriting  MMIC_Reconfig to True")
    T32_R5A.cmd("Var.set %e MMIC_Reconfig = 1")
    time.sleep(2)

    # Read and print updated values for all variables
    print("Updated values:")
    var_values = []
    for var in var_names:
        value = T32_R5A.read_var(var)
        var_values.append(value)
        print(f"{var} = {value}")
    temp_2, temp_3, temp_6, temp_4, temp_5, temp_7 = var_values
    T = T32_R5A.read_var(t)
    print(t, "=", T_1[T])
    print("")
    print("-----------------------------------WI-342339 :END-------------------------------------")
    time.sleep(5)
    condition = 0
    if temp_2 > 0 and temp_3 == 1 and temp_6 == 0 and temp_7 == 0 and flag :
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
