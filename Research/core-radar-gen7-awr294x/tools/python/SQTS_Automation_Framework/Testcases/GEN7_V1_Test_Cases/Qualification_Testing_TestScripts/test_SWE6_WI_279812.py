"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-279812")
@pytest.mark.description("Test for Battery High Fault")
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

    print("-----------------------------------WI-279812 :START-----------------------------------")
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

    temp_2 = "Platform_Active_Fault_Table.platform_bits.battery_high_fault"
    T32_R5A.add_var_watch(temp_2)
    temp_3 = "RadarDiagRequest.RadarDiagShutdown"
    T32_R5A.add_var_watch(temp_3)
    TEMP_4 = T32_R5A.read_var(temp_3)
    print("Value of RadarDiagRequest.RadarDiagShutdown in free run is ", TEMP_4)
    Power.set_output_voltage(17)
    current_voltage = Power.get_output_voltage()
    print(f'Current voltage is {current_voltage} V.')
    time.sleep(5)
    T = T32_R5A.read_var(t)
    print("Radar Status after changing the voltage to 17V is ", T_1[T])
    TEMP_2 = T32_R5A.read_var(temp_2)
    if TEMP_2 == 1:
        print("battery high fault is set when voltage is changed to 17V")
    else :
        print("battery high fault is not set when voltage is changed to 17v")
    TEMP_4 = T32_R5A.read_var(temp_3)
    print("Value of RadarDiagRequest.RadarDiagShutdown when battery_high_fault is set ", TEMP_4)

    Power.set_output_voltage(12)
    current_voltage = Power.get_output_voltage()
    print(f'Current voltage is {current_voltage} V.')
    time.sleep(5)

    TEMP_3 = T32_R5A.read_var(temp_2)
    if TEMP_3 == 0:
        print("battery high fault is cleared after changing the voltage to 12")
    else :
        print("battery high fault is not cleared after changing the voltage to 12")
        flag = False

    print("")
    print("-----------------------------------WI-279812 :END-------------------------------------")
    time.sleep(3)
    condition = 0
    if TEMP_2 == 1 and TEMP_4 == 170 and flag:
        print("Passed")
        condition = 1
    else :
        condition = 0
        print("failed")
    assert condition
###############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
