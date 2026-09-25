"""Python testcase for MCU_Configuration_And_Supervision."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-363831")
@pytest.mark.description("Qualification test validation for CPU Errors - negative test case")
def test_MCU_Configuration_And_Supervision(Power, T32_R5A, T32_C66, Report):
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
    print("-----------------------------------WI-363831 :START-----------------------------------")
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

    time.sleep(4)
    T32_R5A.print("Entering into the Test Case.")

    print()
    temp = "Wdt_Stop_Start_Test"
    T32_R5A.add_var_watch(temp)
    T32_R5A.cmd("Var.set %e Wdt_Stop_Start_Test = 1")
    time.sleep(3)
    try:
        T32_R5A.cmd("Break")
    except Exception:
        pass
    time.sleep(3)

    print()
    print("CASE-1: ")
    Addr = "0x2140020"
    Expected_Value = "0x00000222"
    Rec_Value = T32_R5A.Peripherals_Read_Via_Address(Addr)
    print("Checking PMICCLKOUT_CLK_SRC_SEL Register Data is as ", Rec_Value)
    assert (Rec_Value == Expected_Value), "PMICCLKOUT_CLK_SRC_SEL is not set to 0x00000222 as exected. WI-363831 Test Case got Failed."
    print("PMICCLKOUT_CLK_SRC_SEL is set to 0x00000222 as exected. CASE-1 got PASSED.")
    time.sleep(3)

    print()
    print("CASE-2: ")
    Addr_1 = "0x214004C"
    Expected_Value_1 = "0x00000111"
    Rec_Value = T32_R5A.Peripherals_Read_Via_Address(Addr_1)
    print("Checking PMICCLKOUT_DIV_VAL Register Data is as ", Rec_Value)
    assert (Rec_Value == Expected_Value_1), "PMICCLKOUT_DIV_VAL is not set to 0x00000111 as exected. WI-363831 Test Case got Failed."
    print("PMICCLKOUT_DIV_VAL is set to 0x00000111 as exected. CASE-2 got PASSED.")
    time.sleep(3)

    print()
    print("CASE-3: ")
    Addr_2 = "0x214021C"
    Expected_Value_2 = "0x008E8203"
    Rec_Value = T32_R5A.Peripherals_Read_Via_Address(Addr_2)
    print("Checking PMICCLKOUT_DCDC_CTRL Register Data is as ", Rec_Value)
    assert (Rec_Value == Expected_Value_2), "PMICCLKOUT_DCDC_CTRL is not set to 0x008E8203 as exected. WI-363831 Test Case got Failed."
    print("PMICCLKOUT_DCDC_CTRL is set to 0x008E8203 as exected. CASE-3 got PASSED.")
    time.sleep(3)

    print()
    print("CASE-4: ")
    Addr_3 = "0x2140220"
    Expected_Value_3 = "0x00005B06"
    Rec_Value = T32_R5A.Peripherals_Read_Via_Address(Addr_3)
    print("Checking PMICCLKOUT_DCDC_SLOPE Register Data is as ", Rec_Value)
    assert (Rec_Value == Expected_Value_3), "PMICCLKOUT_DCDC_SLOPE is not set to 0x008E8203 as exected. WI-363831 Test Case got Failed."
    print("PMICCLKOUT_DCDC_SLOPE is set to 0x008E8203 as exected. CASE-4 got PASSED.")
    time.sleep(3)

    print()
    print("CASE-5: ")
    Addr_4 = "0x214008C"
    Expected_Value_4 = "0x00000000"
    Rec_Value = T32_R5A.Peripherals_Read_Via_Address(Addr_4)
    print("Checking PMICCLKOUT_CLK_GATE Register Data is as ", Rec_Value)
    assert (Rec_Value == Expected_Value_4), "PMICCLKOUT_CLK_GATE is not set to 0x008E8203 as exected. WI-363831 Test Case got Failed."
    print("PMICCLKOUT_CLK_GATE is set to 0x008E8203 as exected")
    time.sleep(3)

    print()
    print("All Five Cases were PASS.\nWI-363831 Test Case got Passed.")
    print("-----------------------------------WI-363831 :END-------------------------------------")

    final_condition = flag
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
