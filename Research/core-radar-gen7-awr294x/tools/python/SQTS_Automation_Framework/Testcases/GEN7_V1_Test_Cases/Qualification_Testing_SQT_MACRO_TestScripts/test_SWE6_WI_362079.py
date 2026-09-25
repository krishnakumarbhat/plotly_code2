"""Python testcase for Fault_Manager."""
import pytest
import time
import traceback


@pytest.mark.WI("WI_362079")
@pytest.mark.description("To check the Look Index Stale fault.")
def test_Fault_Manager(Power, T32_R5A, T32_C66, Report):
    """WinCLEAR."""
    T32_R5A.Clean_and_Reset()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Down")
    time.sleep(5)
    try:
        T32_R5A.cmd("SYStem.Up")
    except Exception:
        print("An exception occurred R5A")
        traceback.print_exc()
    time.sleep(6)
    attachAttempts = 0
    while T32_R5A.get_run_state() != 3:  # Running
        T32_R5A.cmd("SYStem.Attach")
        attachAttempts += 1
        time.sleep(10)
        T32_R5A.print(f'-------- State: {T32_R5A.get_run_state()} --------')
        if (attachAttempts > 5) :
            break

    T32_C66.cmd("SYStem.Mode.NoDebug")
    time.sleep(2)
    try:
        T32_C66.cmd("SYStem.Mode.Attach")
    except Exception:
        print("An exception occurred C66")
        traceback.print_exc()
    time.sleep(5)
    print("-----------------------------------WI-362079:START-----------------------------------")
    # Checking Radar Status
    T_1 = []
    T_1.append("RADAR_CTL_INIT_FAIL")
    T_1.append("RADAR_CTL_INIT_NOT_STARTED")
    T_1.append("RADAR_CTL_INIT_STARTED")
    T_1.append("RADAR_CTL_INIT_SUCCESS")
    t = "Radar_Ctl_Data.init_status"
    T = T32_R5A.read_var(t)
    print("")
    if T == 3 :
        flag = True
        print(t, "=", T_1[T])
    else :
        print("Execution is stopped, Radar Status is ", T_1[T])
        flag = False

    t1 = "MMIC_Errors_Stub"
    T32_R5A.add_var_watch(t1)
    temp_2 = T32_R5A.read_var(t1)
    print(t1, "=", temp_2)
    time.sleep(1)
    t2 = "Platform_Active_Fault_Table.platform_bits.look_index_stale_fault"
    T32_R5A.add_var_watch(t2)
    temp_3 = T32_R5A.read_var(t2)
    print(t2, "=", temp_3)
    time.sleep(1)
    t3 = "Platform_Active_Fault_Table.platform_bits.mmic_temperature_high_fault"
    T32_R5A.add_var_watch(t3)
    temp_4 = T32_R5A.read_var(t3)
    print(t3, "=", temp_4)
    time.sleep(1)
    t4 = "Platform_Active_Fault_Table.platform_bits.mmic_a_sensor_dead_fault"
    T32_R5A.add_var_watch(t4)
    temp_5 = T32_R5A.read_var(t4)
    print(t4, "=", temp_5)
    time.sleep(1)
    t5 = "Platform_Active_Fault_Table.platform_bits.mmic_safe_start_fault"
    T32_R5A.add_var_watch(t5)
    temp_6 = T32_R5A.read_var(t5)
    print(t5, "=", temp_6)
    time.sleep(1)
    t6 = "Platform_Active_Fault_Table.platform_bits.mmic_a_safety_integtrity_check_fault"
    T32_R5A.add_var_watch(t6)
    temp_7 = T32_R5A.read_var(t6)
    print(t6, "=", temp_7)
    time.sleep(1)
    t7 = "Platform_Active_Fault_Table.platform_bits.mmic_loop_back_test_fault"
    T32_R5A.add_var_watch(t7)
    temp_8 = T32_R5A.read_var(t7)
    print(t7, "=", temp_8)
    time.sleep(1)
    t8 = "Platform_Active_Fault_Table.platform_bits.mmic_temperature_low_fault"
    T32_R5A.add_var_watch(t8)
    temp_9 = T32_R5A.read_var(t8)
    print(t8, "=", temp_9)
    time.sleep(1)
    t9 = "Platform_Active_Fault_Table.platform_bits.mmic_transmitter_id_fault"
    T32_R5A.add_var_watch(t9)
    temp_10 = T32_R5A.read_var(t9)
    print(t9, "=", temp_10)
    time.sleep(1)
    t10 = "Platform_Active_Fault_Table.platform_bits.mmic_tx3_ball_break_fault"
    T32_R5A.add_var_watch(t10)
    temp_11 = T32_R5A.read_var(t10)
    print(t10, "=", temp_11)
    time.sleep(1)
    t11 = "Platform_Active_Fault_Table.platform_bits.mmic_tx2_ball_break_fault"
    T32_R5A.add_var_watch(t11)
    temp_12 = T32_R5A.read_var(t11)
    print(t11, "=", temp_12)
    time.sleep(1)
    t12 = "Platform_Active_Fault_Table.platform_bits.mmic_tx1_ball_break_fault"
    T32_R5A.add_var_watch(t12)
    temp_13 = T32_R5A.read_var(t12)
    print(t12, "=", temp_13)
    time.sleep(1)
    t13 = "Platform_Active_Fault_Table.platform_bits.mmic_tx0_ball_break_fault"
    T32_R5A.add_var_watch(t13)
    temp_14 = T32_R5A.read_var(t13)
    print(t13, "=", temp_14)
    time.sleep(1)
    t14 = "Platform_Active_Fault_Table.platform_bits.mmic_rf_ball_break_fault"
    T32_R5A.add_var_watch(t14)
    temp_15 = T32_R5A.read_var(t14)
    print(t14, "=", temp_15)
    time.sleep(1)
    t15 = "Platform_Active_Fault_Table.platform_bits.acquisition_overflow_fault"
    T32_R5A.add_var_watch(t15)
    temp_16 = T32_R5A.read_var(t15)
    print(t15, "=", temp_16)
    time.sleep(2)
    print("Overwriting MMIC_Errors_Stub to 1 ")
    T32_R5A.cmd("Var.set %e MMIC_Errors_Stub = 1")
    time.sleep(3)
    temp_2 = T32_R5A.read_var(t1)
    print(t1, "=", temp_2)
    time.sleep(1)
    temp_3 = T32_R5A.read_var(t2)
    print(t2, "=", temp_3)
    time.sleep(1)
    temp_4 = T32_R5A.read_var(t3)
    print(t3, "=", temp_4)
    time.sleep(1)
    temp_5 = T32_R5A.read_var(t4)
    print(t4, "=", temp_5)
    time.sleep(1)
    temp_6 = T32_R5A.read_var(t5)
    print(t5, "=", temp_6)
    time.sleep(1)
    temp_7 = T32_R5A.read_var(t6)
    print(t6, "=", temp_7)
    time.sleep(1)
    temp_8 = T32_R5A.read_var(t7)
    print(t7, "=", temp_8)
    time.sleep(1)
    temp_9 = T32_R5A.read_var(t8)
    print(t8, "=", temp_9)
    time.sleep(1)
    temp_10 = T32_R5A.read_var(t9)
    print(t9, "=", temp_10)
    time.sleep(1)
    temp_11 = T32_R5A.read_var(t10)
    print(t10, "=", temp_11)
    time.sleep(1)
    temp_12 = T32_R5A.read_var(t11)
    print(t11, "=", temp_12)
    time.sleep(1)
    temp_13 = T32_R5A.read_var(t12)
    print(t12, "=", temp_13)
    time.sleep(1)
    temp_14 = T32_R5A.read_var(t13)
    print(t13, "=", temp_14)
    time.sleep(1)
    temp_15 = T32_R5A.read_var(t14)
    print(t14, "=", temp_15)
    time.sleep(1)
    temp_16 = T32_R5A.read_var(t15)
    print(t15, "=", temp_16)
    time.sleep(2)
    print("-----------------------------------WI-362079 :END-------------------------------------")
    time.sleep(5)
    condition = 0
    if temp_2 and temp_4 == 1 and temp_5 == 1 and temp_6 == 1 and temp_7 == 0 and temp_8 == 1 and temp_9 == 1 and temp_10 == 1 and temp_11 == 1 and temp_12 == 1 and temp_13 == 1 and temp_14 == 1 and temp_15 == 0 and temp_16 == 1 and flag :
        print("Passed")
        condition = 1
    else:
        condition = 0
        print("failed")
    assert condition
