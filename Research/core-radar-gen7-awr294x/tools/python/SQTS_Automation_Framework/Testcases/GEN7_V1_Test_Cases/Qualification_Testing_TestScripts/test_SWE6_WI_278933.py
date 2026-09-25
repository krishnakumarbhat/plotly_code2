"""Python testcase for Mode_Manager_Core."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_278933")
@pytest.mark.description("Test for Voltage State Machine update")
def test_Mode_Manager_Core(Power, T32_R5A, T32_C66, Report):
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

    print("-----------------------------------WI-278933 :START-----------------------------------")
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

    # Define variables to monitor
    monitored_vars = [
        "VBattState",
        "Platform_Active_Fault_Table.platform_bits.battery_low_fault",
        "Platform_Active_Fault_Table.platform_bits.battery_high_fault"
    ]
    var_values = [0] * len(monitored_vars)

    def read_and_print_vars():
        for i, var in enumerate(monitored_vars):
            T32_R5A.add_var_watch(var)
            time.sleep(1)
            var_values[i] = T32_R5A.read_var(var)
            print(f"{var} = {var_values[i]}")
            time.sleep(1)

    print("# Initial State: Reading monitored variables #\n")
    read_and_print_vars()
    assert var_values[0] == 1, f"VBattState not in Normal Condition: {var_values[0]}"
    assert var_values[1] == 0, f"battery_low_fault incorrectly set: {var_values[1]}"
    assert var_values[2] == 0, f"battery_high_fault incorrectly set: {var_values[2]}"

    # Transition to High Voltage
    print("\n# Transition: Setting voltage to 16.7V (High Voltage) #")
    Power.set_output_voltage(16.7)
    time.sleep(3)
    current_voltage = Power.get_output_voltage()
    print(f"Current voltage is {current_voltage} V.")
    read_and_print_vars()
    assert var_values[0] == 3, f"VBattState not in High Voltage Condition: {var_values[0]}"
    assert var_values[1] == 0, f"battery_low_fault incorrectly set: {var_values[1]}"
    assert var_values[2] == 1, f"battery_high_fault not set: {var_values[2]}"

    # Transition to Low Voltage
    print("\n# Transition: Setting voltage to 7.5V (Low Voltage) #")
    Power.set_output_voltage(7.5)
    time.sleep(3)
    current_voltage = Power.get_output_voltage()
    print(f"Current voltage is {current_voltage} V.")
    read_and_print_vars()
    assert var_values[0] == 2, f"VBattState not in Low Voltage Condition: {var_values[0]}"
    assert var_values[1] == 1, f"battery_low_fault not set: {var_values[1]}"
    assert var_values[2] == 0, f"battery_high_fault incorrectly set: {var_values[2]}"

    # Transition to Normal Voltage
    print("\n# Transition: Setting voltage to 12.0V (Normal Voltage) #")
    Power.set_output_voltage(12.0)
    time.sleep(3)
    current_voltage = Power.get_output_voltage()
    print(f"Current voltage is {current_voltage} V.")
    read_and_print_vars()
    assert var_values[0] == 1, f"VBattState not returned to Normal Condition: {var_values[0]}"
    assert var_values[1] == 0, f"battery_low_fault still set: {var_values[1]}"
    assert var_values[2] == 0, f"battery_high_fault still set: {var_values[2]}"

    print("")
    print("-----------------------------------WI-278933 :END-------------------------------------")
    print("All voltage state transitions verified successfully")
    time.sleep(3)
###############################################################################
#                               Revision History                              #
###############################################################################
#
# MM/DD/YYYY  Name/
#               Initials           JIRA         Explanation of changes done here.
#    Date         By              ###-####               Description
# ----------    ---------         --------      ----------------------------------
# 09/12/2025   AZEEJ SHAIK        EAH-7440              Script correction
