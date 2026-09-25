"""Python testcase for Measurement_Monitor."""
import pytest
import time
import traceback


@pytest.mark.WI("WI-377551")
@pytest.mark.description("Qualification Test for loopback configuration")
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
    print("-----------------------------------WI-377551 :START-----------------------------------")
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

    print("")
    print("## PART-1 ##")
    temp_index0 = ["MMIC_Config.profile_cfg[0].miscFeatureEn",
                   "MMIC_Config.profile_cfg[0].txCalibEnCfg",
                   "MMIC_Config.profile_cfg[0].hpfCornerFreq2",
                   "MMIC_Config.profile_cfg[0].hpfCornerFreq1",
                   "MMIC_Config.profile_cfg[0].digOutSampleRate",
                   "MMIC_Config.profile_cfg[0].numAdcSamples",
                   "MMIC_Config.profile_cfg[0].txStartTime",
                   "MMIC_Config.profile_cfg[0].freqSlopeConst",
                   "MMIC_Config.profile_cfg[0].txPhaseShifter",
                   "MMIC_Config.profile_cfg[0].rampEndTime",
                   "MMIC_Config.profile_cfg[0].adcStartTimeConst",
                   "MMIC_Config.profile_cfg[0].idleTimeConst",
                   "MMIC_Config.profile_cfg[0].startFreqConst",
                   "MMIC_Config.profile_cfg[0].pfCalLutUpdate",
                   "MMIC_Config.profile_cfg[0].pfVcoSelect",
                   "MMIC_Config.profile_cfg[0].profileId"]

    # Add watches and read profile_cfg[0]
    for var in temp_index0:
        T32_R5A.add_var_watch(var)
        time.sleep(0.2)  # Add delay between watch additions

    time.sleep(2)  # Increased delay for stability
    temp_index0_values = []
    for var in temp_index0:
        retry_count = 0
        max_retries = 3
        while retry_count < max_retries:
            try:
                val = T32_R5A.read_var(var)
                temp_index0_values.append(val)
                print(f"{var} = {val}")
                break
            except Exception as e:
                retry_count += 1
                if retry_count >= max_retries:
                    print(f"Error reading {var}: {e}")
                    temp_index0_values.append(None)
                else:
                    time.sleep(1)

    print("")
    temp_index6 = ["MMIC_Config.profile_cfg[6].miscFeatureEn",
                   "MMIC_Config.profile_cfg[6].txCalibEnCfg",
                   "MMIC_Config.profile_cfg[6].hpfCornerFreq2",
                   "MMIC_Config.profile_cfg[6].hpfCornerFreq1",
                   "MMIC_Config.profile_cfg[6].digOutSampleRate",
                   "MMIC_Config.profile_cfg[6].numAdcSamples",
                   "MMIC_Config.profile_cfg[6].txStartTime",
                   "MMIC_Config.profile_cfg[6].freqSlopeConst",
                   "MMIC_Config.profile_cfg[6].txPhaseShifter",
                   "MMIC_Config.profile_cfg[6].rampEndTime",
                   "MMIC_Config.profile_cfg[6].adcStartTimeConst",
                   "MMIC_Config.profile_cfg[6].idleTimeConst",
                   "MMIC_Config.profile_cfg[6].startFreqConst",
                   "MMIC_Config.profile_cfg[6].pfCalLutUpdate",
                   "MMIC_Config.profile_cfg[6].pfVcoSelect",
                   "MMIC_Config.profile_cfg[6].profileId"]

    # Add watches and read profile_cfg[6]
    for var in temp_index6:
        T32_R5A.add_var_watch(var)
        time.sleep(0.2)  # Add delay between watch additions

    time.sleep(2)  # Increased delay for stability
    temp_index6_values = []
    for var in temp_index6:
        retry_count = 0
        max_retries = 3
        while retry_count < max_retries:
            try:
                val = T32_R5A.read_var(var)
                temp_index6_values.append(val)
                print(f"{var} = {val}")
                break
            except Exception as e:
                retry_count += 1
                if retry_count >= max_retries:
                    print(f"Error reading {var}: {e}")
                    temp_index6_values.append(None)
                else:
                    time.sleep(1)

    # Compare all parameters except the last one (profileId)
    part1_condition = 1
    for i in range(len(temp_index0) - 1):
        if temp_index0_values[i] is None or temp_index6_values[i] is None:
            print(f"Could not read some variables, skipping comparison at index {i}")
            part1_condition = 0
            print("Part-1 execution is failed")
            break
        if temp_index6_values[i] != temp_index0_values[i]:
            print("The parameters in MMIC_Config.profile_cfg[6] and MMIC_Config.profile_cfg[0] are not same")
            part1_condition = 0
            print("Part-1 execution is failed")
            break

    if part1_condition == 1:
        print("The parameters in MMIC_Config.profile_cfg[6] and MMIC_Config.profile_cfg[0] are same")
        print("Part-1 execution is successfull")

    print("")
    print("## PART-2 ##")
    print("")

    # Reload symbols and wait for system to stabilize
    print("Reloading symbols...")
    try:
        T32_R5A.cmd("Data.LOAD.Elf * /NoCODE")
        time.sleep(5)
        T32_R5A.cmd("sYmbol.AutoLoad.CHECKLINUX")
        time.sleep(3)
    except Exception as e:
        print(f"Warning during symbol reload: {e}")
        traceback.print_exc()

    temp_part2 = ["MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.tx3_noise_power[0]",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.tx2_noise_power[0]",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.tx1_noise_power[0]",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.tx0_noise_power[0]",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.rx_noise_power2",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.rx_noise_power1",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.tx_gain_value[0]",
                  "MMIC_Stream.MMIC_Data.mmic_tx_rx_T0_gain_phase_val.rx_gain_value[0]"]

    # Add watches with longer delays
    for var in temp_part2:
        try:
            T32_R5A.add_var_watch(var.strip())
            time.sleep(0.5)
        except Exception as e:
            print(f"Warning adding watch for {var.strip()}: {e}")

    print("During First frame :")
    time.sleep(10)  # Increased wait time for data to be available
    initial_values = []
    for var in temp_part2:
        retry_count = 0
        val = None
        while retry_count < 5:  # Increased retries
            try:
                val = T32_R5A.read_var(var.strip())
                initial_values.append(val)
                print(f"{var.strip()} = {val}")
                break
            except Exception as e:
                retry_count += 1
                if retry_count < 5:
                    time.sleep(2)  # Increased delay between retries
                else:
                    print(f"Error reading {var.strip()}: {e}")
                    traceback.print_exc()
                    initial_values.append(None)

    print("")
    time.sleep(10)
    print("After 10sec :")
    current_values = []
    for var in temp_part2:
        retry_count = 0
        val = None
        while retry_count < 5:  # Increased retries
            try:
                val = T32_R5A.read_var(var.strip())
                current_values.append(val)
                print(f"{var.strip()} = {val}")
                break
            except Exception as e:
                retry_count += 1
                if retry_count < 5:
                    time.sleep(2)  # Increased delay between retries
                else:
                    print(f"Error reading {var.strip()}: {e}")
                    traceback.print_exc()
                    current_values.append(None)

    # Check if all values remained the same
    part2_condition = 1
    for i in range(len(temp_part2)):
        if initial_values[i] is None or current_values[i] is None:
            print("Could not read some variables in Part-2")
            part2_condition = 0
            break
        if current_values[i] != initial_values[i]:
            print("Part-2 execution is failed")
            part2_condition = 0
            break

    if part2_condition == 1:
        print("Data coming from RSS to MSS as part of sync report populated in the first frame is used ")
        print("Part-2 execution is successfull")

    print("")
    print("## PART-3 ##")
    print("")

    # Ensure symbols are loaded for PART-3
    print("Ensuring symbols are loaded for PART-3...")
    time.sleep(5)

    temp_part3 = ["MMIC_Error.Discard_Error_Count",
                  "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Loop_Back_Fault.rx_if_phase_gain_mm_err_cnt",
                  "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Loop_Back_Fault.tx_phase_gain_mm_err_cnt",
                  "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Loop_Back_Fault.rx_phase_gain_mm_err_cnt",
                  "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Loop_Back_Fault.rx_gain_ph_mismatch_errcode",
                  "MMIC_Stream.MMIC_Data.MMIC_Fault_Status.MMIC_Loop_Back_Fault.loop_back_test_integrity_error"]

    # Add watches and read values with retries
    for var in temp_part3:
        try:
            T32_R5A.add_var_watch(var.strip())
            time.sleep(0.5)
        except Exception as e:
            print(f"Warning adding watch for {var.strip()}: {e}")

    time.sleep(5)  # Increased wait time
    part3_values = []
    for var in temp_part3:
        retry_count = 0
        val = None
        while retry_count < 5:  # Increased retries
            try:
                val = T32_R5A.read_var(var.strip())
                part3_values.append(val)
                print(f"{var.strip()} = {val}")
                break
            except Exception as e:
                retry_count += 1
                if retry_count < 5:
                    time.sleep(2)  # Increased delay between retries
                else:
                    print(f"Error reading {var.strip()}: {e}")
                    traceback.print_exc()
                    part3_values.append(None)

    if (None not in part3_values
            and part3_values[0] == 0 and part3_values[1] == 0 and part3_values[2] == 0
            and part3_values[3] == 0 and part3_values[4] == 0 and part3_values[5] == 85):
        print("Part-3 execution is successfull")
        part3_condition = 1
    else:
        print("Part-3 execution is failed")
        part3_condition = 0

    if part1_condition == 1 and part2_condition == 1 and part3_condition == 1:
        final_condition = True
    else:
        final_condition = False
    print("")
    print("-----------------------------------WI-377551 :END-------------------------------------")

    final_condition = final_condition and flag
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
