"""Python testcase for MCU Safety."""
import pytest
import os
import time
from typing import Literal
from dataclasses import dataclass
from pathlib import Path
import pandas as pd
from tabulate import tabulate

original = os.getcwd()
print(original)
xls = pd.ExcelFile(r'.\Messages\SWE5_RDD_Second_Pass.xlsx')
df = pd.read_excel(xls, 'SRR7')
os.chdir(original)
columns = df.columns
Excel = {}
out = []
for column in columns:
    Excel[column] = df[column].tolist()
row = len(Excel["Work_Items"])
print(row)


@pytest.fixture(scope='session')
def T32_R5A(Trace32_Session_Minimal, sqts_config):
    """T32 Instance for R5A."""
    t32 = Trace32_Session_Minimal
    files = sqts_config['Trace32']['files']
    flags = sqts_config['Trace32']['files']['flags']
    autoflash_cmm = Path(files['auto_flash_cmm']).absolute()
    pbl_hex = Path(files['pbl_hex']).absolute()
    hsm_hex = Path(files['hsm_hex']).absolute()
    pbl_elf = Path(files['pbl_elf']).absolute()
    app_hex = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['app_hex'])).absolute()
    mss_elf = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['mss_elf'])).absolute()
    dss_elf = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['dss_elf'])).absolute()
    smc_ptp = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['smc_ptp'])).absolute()
    usc_ptp = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['usc_ptp'])).absolute()
    time.sleep(10)
    t32.run_cmm(
        f'"{autoflash_cmm}"',
        sqts_config['Trace32']['pre_erase'],
        pbl_hex,
        hsm_hex,
        pbl_elf,
        app_hex,
        mss_elf,
        dss_elf,
        smc_ptp,
        usc_ptp,
        timeout_s=60
    )
    t32.delete_all_breakpoints()
    yield t32


@pytest.fixture(scope='session')
def T32_C66(Trace32_Session_Minimal2):
    """T32 Instance for C66."""
    t32 = Trace32_Session_Minimal2
    t32.delete_all_breakpoints()
    yield t32


@pytest.fixture(scope='session', autouse=True)
def global_setup(request, Power_Session):
    """T32 power session."""
    Power_Session.switch_on()
    request.getfixturevalue('Trace32_Session_Minimal')
    request.getfixturevalue('Trace32_Session_Minimal2')
    time.sleep(2)
    yield
    Power_Session.switch_off()


@dataclass
class InterfaceData:
    """Class to hold test case variable and functions."""

    provider_interface_variable: str
    provider_core: Literal['C66', 'R5A']
    receiver_interface_variable: str
    receiver_core: Literal['R5A', 'C66']


@pytest.mark.Req("")
@pytest.mark.WI("")
@pytest.mark.description("SWE5_RDD_Second_Pass")
def test_SWE5_RDD_Second_Pass(T32_R5A, T32_C66, Report):
    """Test the min value."""
    T32_R5A.delete_all_breakpoints()
    T32_C66.delete_all_breakpoints()
    T32_R5A.cmd("SYStem.Mode.Down")
    time.sleep(2)
    try:
        T32_R5A.cmd("SYStem.Mode.Up")
    except BaseException:
        print("An exception occurred R5A")
    time.sleep(5)
    attachAttempts = 0
    while T32_R5A.get_run_state() != 3:
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
    except BaseException:
        print("An exception occurred C66")
    time.sleep(5)
    t = 0
    Result1 = []
    f = []
    WI = 0
    time.sleep(5)
    while t != (row):
        cur_index = t
        var_count = 0
        T32_R5A.delete_all_breakpoints()
        T32_C66.delete_all_breakpoints()
        attachAttempts = 0
        T32_R5A.power_on()
        while (T32_R5A.get_run_state() != 3) or (T32_C66.get_run_state() != 3):
            print(f'-------- T32_R5A State: {T32_R5A.get_run_state()} --------')
            if T32_R5A.get_run_state() != 3:
                T32_C66.cmd("SYStem.Mode.Down")
                time.sleep(2)
                T32_R5A.cmd("SYStem.RESetTarget")
                time.sleep(2)
                T32_R5A.cmd("SYStem.Mode.Down")
                time.sleep(2)
                if (T32_R5A.get_run_state() == 0):
                    T32_R5A.cmd("SYStem.Mode.Prepare")
                    time.sleep(2)
                    T32_R5A.cmd("SYStem.Mode.Down")
                    time.sleep(2)
                    T32_R5A.cmd("SYStem.Mode.Attach")
            elif (T32_C66.get_run_state() != 3) and (T32_R5A.get_run_state() == 3):
                time.sleep(2)
                print(f'-------- T32_C66 State: {T32_C66.get_run_state()} --------')
                T32_C66.cmd("SYStem.Mode.NoDebug")
                time.sleep(2)
                try:
                    T32_C66.cmd("SYStem.Mode.Attach")
                except BaseException:
                    print("An exception occurred C66")
                    time.sleep(2)
                print(f'-------- 66 State: {T32_C66.get_run_state()} --------')
            else:
                print(f'-------- 11 State: {T32_R5A.get_run_state()} --------')
                print(f'-------- 11 State: {T32_C66.get_run_state()} --------')
                break
            # attachAttempts += 1
            # if attachAttempts > 5:
                # break
        time.sleep(5)
        for j in range(t, row):
            if str(Excel["Provider_interface_var"][j]) != 'nan' and str(Excel["Work_Items"][j]) == str(Excel["Work_Items"][cur_index]) or str(Excel["Work_Items"][j]) == 'nan':
                var_count += 1
                t += 1
            elif str(Excel["Provider_interface_var"][j]) != 'nan' and str(Excel["Work_Items"][cur_index]) == 'nan':
                var_count += 1
                t += 1
            else:
                break
        start_index = cur_index  # /// Excel sheet cell start index for WorkItems
        print(start_index)
        stop_index = t  # /// Excel sheet cell start ndex for WorkItems
        print(stop_index)
        count = 0
        for _i in range(start_index, stop_index):
            count += 1
        temp1 = str('RE_Radar_Ctl_Look_Trigger') + ' /Program /Onchip'
        T32_R5A.set_breakpoint(temp1)
        time.sleep(2)
        print(T32_R5A.get_run_state())
        # print(T32_C66.get_run_state())
        # Read Var from Trace 32 and Add the Var in Tarce32 window Provider_core_window
        for i in range(start_index, stop_index):
            if (str(Excel["Provider_core_window"][i]) != 'nan') and (str(Excel["Provider_interface_var"][i]) != 'nan'):
                if str(Excel["Provider_core_window"][i]) == 'R5A':
                    T32_R5A.add_var_watch(str(Excel["Provider_interface_var"][i]))
                    # print(T32_Core0.read_var(str(Excel["Provider_interface_var"][i])))
                elif str(Excel["Provider_core_window"][i]) == 'C66':
                    T32_C66.add_var_watch(str(Excel["Provider_interface_var"][i]))
                    # print(T32_Core0.read_var(str(Excel["Provider_interface_var"][i])))
            else:
                print('Var_name or Provider_core_window is missing')
                Excel["Provider_interface_var"][i] = Excel["Provider_interface_var"][i] + 'Provider_interface_var is missing'
        # Break point set in Trace 32 window Provider_core_window
        if (str(Excel["Provider_core_window"][start_index]) != 'nan') and (str(Excel["Provider_Function"][start_index]) != 'nan') and (str(Excel["Pro_Break_Point"][start_index]) != 'nan'):
            if str(Excel["Provider_core_window"][start_index]) == 'R5A':
                if str(Excel["Pro_Break_Point"][start_index]) == 'Start' or str(Excel["Pro_Break_Point"][start_index]) == 'Default':
                    temp1 = str(Excel["Provider_Function"][start_index]) + ' /Program /Onchip'
                    T32_R5A.set_breakpoint(temp1)
                elif str(Excel["Pro_Break_Point"][start_index]) == 'End':
                    temp1 = str(Excel["Provider_Function"][start_index])
                    T32_R5A.set_breakpoint_end(temp1)
                else:
                    print("Pro_Break_Point field is missing for ", str(Excel["Provider_Function"][start_index]))
            elif str(Excel["Provider_core_window"][start_index]) == 'C66':
                if str(Excel["Pro_Break_Point"][start_index]) == 'Start' or str(Excel["Pro_Break_Point"][start_index]) == 'Default':
                    temp1 = str(Excel["Provider_Function"][start_index]) + ' /Program /Onchip'
                    T32_C66.set_breakpoint(temp1)
                elif str(Excel["Pro_Break_Point"][start_index]) == 'End':
                    temp1 = str(Excel["Provider_Function"][start_index])
                    T32_C66.set_breakpoint_end(temp1)
                else:
                    print("Pro_Break_Point field is missing for ", str(Excel["Provider_Function"][start_index]))
        else:
            print('Provider_Function is missing')
            Excel["Provider_Function"][start_index] = Excel["Provider_Function"][start_index] + 'Provider_Function is missing'
        time.sleep(1)
        temp_1 = []
        c = 0
        T32_R5A.power_on()
        time.sleep(2)
        # print(start_index,stop_index)
        for i in range(start_index, stop_index):
            # print('i-',i)
            # print('c-',c)
            if (str(Excel["Provider_core_window"][i]) != 'nan') and (str(Excel["Provider_interface_var"][i]) != 'nan'):
                if str(Excel["Provider_core_window"][i]) == 'R5A':
                    temp_1.append(T32_R5A.read_var(str(Excel["Provider_interface_var"][i])))
                    print("pro[", c, ']:-', temp_1[c])
                elif str(Excel["Provider_core_window"][i]) == 'C66':
                    temp_1.append(T32_C66.read_var(str(Excel["Provider_interface_var"][i])))
                    print("pro[", c, ']:-', temp_1[c])
                if c < stop_index:
                    c += 1
                    # print('c-',c)
            else:
                print('Var_name or Provider_core_window is missing')
                Excel["Provider_interface_var"][i] = Excel["Provider_interface_var"][i] + 'Provider_interface_var is missing'
        # Delete all Break point in Trace 32 windows Provider_core_window
        if str(Excel["Provider_core_window"][start_index]) == 'R5A':
            T32_R5A.delete_all_breakpoints()
        elif str(Excel["Provider_core_window"][start_index]) == 'C66':
            T32_C66.delete_all_breakpoints()
        time.sleep(1)
        # Break point set in Trace 32 window  Receiver_core_window
        if (str(Excel["Receiver_core_window"][start_index]) != 'nan') and (str(Excel["Receiver_Function"][start_index]) != 'nan') and (str(Excel["Rec_Break_Point"][start_index]) != 'nan'):
            if str(Excel["Receiver_core_window"][start_index]) == 'R5A':
                if str(Excel["Rec_Break_Point"][start_index]) == 'Start' or str(Excel["Rec_Break_Point"][start_index]) == 'Default':
                    temp1 = str(Excel["Receiver_Function"][start_index]) + ' /Program /Onchip'
                    T32_R5A.set_breakpoint(temp1)
                elif str(Excel["Rec_Break_Point"][start_index]) == 'End':
                    temp1 = str(Excel["Receiver_Function"][start_index])
                    T32_R5A.set_breakpoint_end(temp1)
                else:
                    print("Rec_Break_Point field is missing for ", str(Excel["Receiver_Function"][start_index]))
            elif str(Excel["Receiver_core_window"][start_index]) == 'C66':
                if str(Excel["Rec_Break_Point"][start_index]) == 'Start' or str(Excel["Rec_Break_Point"][start_index]) == 'Default':
                    temp1 = str(Excel["Receiver_Function"][start_index]) + ' /Program /Onchip'
                    T32_C66.set_breakpoint(temp1)
                elif str(Excel["Rec_Break_Point"][start_index]) == 'End':
                    temp1 = str(Excel["Receiver_Function"][start_index])
                    T32_C66.set_breakpoint_end(temp1)
                else:
                    print("Rec_Break_Point field is missing for ", str(Excel["Receiver_Function"][start_index]))
        else:
            print('Receiver_Function is missing')
            Excel["Receiver_Function"][start_index] = Excel["Receiver_Function"][start_index] + 'Receiver_Function is missing'
        time.sleep(1)
        # Give Go or run the SW in Trace 32 window Provider_core_window
        if str(Excel["Provider_core_window"][start_index]) == 'R5A':
            T32_R5A.power_on()
        elif str(Excel["Provider_core_window"][start_index]) == 'C66':
            T32_C66.power_on()
        time.sleep(1)
        # Read Var from Trace 32 window Receiver_core_window
        temp_2 = []
        c = 0
        for i in range(start_index, stop_index):
            # print('i--',i)
            # print('c--',c)
            if (str(Excel["Receiver_core_window"][i]) != 'nan') and (str(Excel["Receiver_interface_var"][i]) != 'nan'):
                if str(Excel["Receiver_core_window"][i]) == 'R5A':
                    T32_R5A.add_var_watch(str(Excel["Receiver_interface_var"][i]))
                    temp_2.append(T32_R5A.read_var(str(Excel["Receiver_interface_var"][i])))
                    print("rec[", c, ']:-', temp_2[c])
                elif str(Excel["Receiver_core_window"][i]) == 'C66':
                    T32_C66.add_var_watch(str(Excel["Receiver_interface_var"][i]))
                    temp_2.append(T32_C66.read_var(str(Excel["Receiver_interface_var"][i])))
                    print("rec[", c, ']:-', temp_2[c])
                if c < stop_index:
                    c += 1
                    # print('c--',c)
            else:
                print('Var_name or Receiver_core_window is missing')
                Excel["Receiver_interface_var"][i] = Excel["Receiver_interface_var"][i] + 'Receiver_interface_var is missing'
        # Delete all Break point in Trace 32 windows Receiver_core_window
        if str(Excel["Receiver_core_window"][start_index]) == 'R5A':
            T32_R5A.delete_all_breakpoints()
        elif str(Excel["Receiver_core_window"][start_index]) == 'C66':
            T32_C66.delete_all_breakpoints()
        time.sleep(1)
        # Give Go or run the SW in Trace 32 window Receiver_core_window
        if str(Excel["Receiver_core_window"][start_index]) == 'R5A':
            T32_R5A.power_on()
        elif str(Excel["Receiver_core_window"][start_index]) == 'C66':
            T32_C66.power_on()
        n = []
        ll = []
        flag = 0
        c = 0
        for i in range(start_index, stop_index):
            if (temp_1[c] == temp_2[c]) and (str(Excel["Provider_interface_var"][i]) != 'nan'):
                n.append(i + 1)
                n.append(Excel["Work_Items"][start_index])
                n.append(Excel["Interface_name"][start_index])
                n.append(Excel["Provider_Function"][start_index])
                n.append(Excel["Provider_interface_var"][i])
                n.append(Excel["Receiver_interface_var"][i])
                Excel["Status"][i] = 'Passed'
                n.append("Passed")
                ll = ll + [n]
                n = []
            else:
                n.append(i + 1)
                n.append(Excel["Work_Items"][start_index])
                n.append(Excel["Interface_name"][start_index])
                n.append(Excel["Provider_Function"][start_index])
                n.append(Excel["Provider_interface_var"][i])
                n.append(Excel["Receiver_interface_var"][i])
                Excel["Status"][i] = 'Failed'
                n.append("Failed")
                ll = ll + [n]
                n = []
                count -= 1
                flag = 1
            c += 1
        p = []
        if 1 == flag:
            WI += 1
            p.append(WI)
            p.append(Excel["Work_Items"][start_index])
            p.append(Excel["Interface_name"][start_index])
            p.append('Failed')
            Result1.append(False)
        else:
            WI += 1
            p.append(WI)
            p.append(Excel["Work_Items"][start_index])
            p.append(Excel["Interface_name"][start_index])
            p.append('Passed')
            Result1.append(True)
        print(p)
        print('-----------------||-------------------')
        table = tabulate(ll, headers=["S.No", "Work_Items", "Interface_name", "Provider_Function", "Provider_interface_var", "Receiver_interface_var", "Status"], tablefmt='orgtbl')
        print(table)
        f += [p]
    print('--------------------Final-----------------------')
    table_1 = tabulate(f, headers=["S.No", "Work_Items", "Interface_name", "Results"], tablefmt='orgtbl')
    print(table_1)
    flag1 = 0
    for i in range(0, WI):
        # print(i)
        if Result1[i]:
            continue
        else:
            flag1 = 1
    if 0 == flag1:
        Result = True
    else:
        Result = False
    assert Result
