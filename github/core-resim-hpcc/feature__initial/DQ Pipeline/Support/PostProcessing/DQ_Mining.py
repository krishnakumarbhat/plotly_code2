# -*- coding: utf-8 -*-
"""
Created on Thu Apr 18 16:38:57 2024

@author: qj743z
"""

import xml.etree.ElementTree as ET
import pandas as pd
import os
import numpy as np
import sys
from pandas import ExcelWriter
import sys
from datetime import datetime
from openpyxl import load_workbook
from openpyxl.styles import PatternFill
from openpyxl.styles.differential import DifferentialStyle
from openpyxl.formatting.rule import Rule
from openpyxl.workbook.workbook import Workbook
from texttable import Texttable
import matplotlib.pyplot as plt
import openpyxl
import platform


class DataQualityToolExtraction():
    def __init__(self):
        self._func_name = os.path.basename(__file__).replace(".py", "")
        self._headers = dict()
        self._version = '0.1'

    def plot_generation(self, bk, sheet_name, excel_file, pos, pos2, pos3, pos4, pos5, output_path1):
        book = bk
        sheet = book[sheet_name]

        if sheet_name == 'RESIM_Detailed_Info':
            df1 = pd.read_excel(excel_file, sheet_name)  # sheet 1 contains the data see above
            df1['index'] = df1.index

            # Create a figure instance
            fig1 = plt.figure(1, figsize=(5, 5))
            fig2 = plt.figure(2, figsize=(5, 5))
            fig3 = plt.figure(3, figsize=(5, 5))
            fig4 = plt.figure(4, figsize=(5, 5))
            fig5 = plt.figure(5, figsize=(5, 5))

            # Create an axes instance
            ax1 = fig1.add_subplot(111)
            ax1.set_title('Time Sync Issue')
            ax1.set_xlabel('Index Value')
            ax1.set_ylabel('Scan diff')

            ax2 = fig2.add_subplot(111)
            ax2.set_title('CDC Partially Drop')
            ax2.set_xlabel('Index Value')
            ax2.set_ylabel('CDC Drop')

            ax3 = fig3.add_subplot(111)
            ax3.set_title('CDC Fully Drop')
            ax3.set_xlabel('Index Value')
            ax3.set_ylabel('CDC Drop')

            ax4 = fig4.add_subplot(111)
            ax4.set_title('UDP Partially drop')
            ax4.set_xlabel('Index Value')
            ax4.set_ylabel('UDP Drop')

            ax5 = fig5.add_subplot(111)
            ax5.set_title('UDP Fully Drop')
            ax5.set_xlabel('Index Value')
            ax5.set_ylabel('UDP Drop')

            # Create the line chart
            #print('df1 columns are: ', df1.columns)

            ax1.plot(df1['index'], df1['TimeSync_issue'], linestyle='solid', linewidth=1.5, color='blue')
            ax2.plot(df1['index'], df1['CDC_PARTIALLY_DROP'], linestyle='solid', linewidth=1.5, color='blue')
            ax3.plot(df1['index'], df1['CDC_FULLY_DROP'], linestyle='solid', linewidth=1.5, color='blue')
            ax4.plot(df1['index'], df1['UDP_PARTIALLY_DROP'], linestyle='solid', linewidth=1.5, color='blue')
            ax5.plot(df1['index'], df1['UDP_FULLY_DROP'], linestyle='solid', linewidth=1.5, color='blue')
            # Save the figure
            fig1.savefig(output_path1 + '/' + 'TimeSync_issue.png')
            fig2.savefig(output_path1 + '/' + 'CDC_Partially_drop.png')
            fig3.savefig(output_path1 + '/' + 'CDC_Fully_drop.png')
            fig4.savefig(output_path1 + '/' + 'UDP_Partially_drop.png')
            fig5.savefig(output_path1 + '/' + 'UDP_Fully_drop.png')

            # load the created line image and insert in sheet2
            img1 = openpyxl.drawing.image.Image(output_path1 + '/' + 'TimeSync_issue.png')
            img2 = openpyxl.drawing.image.Image(output_path1 + '/' + 'CDC_Partially_drop.png')
            img3 = openpyxl.drawing.image.Image(output_path1 + '/' + 'CDC_Fully_drop.png')
            img4 = openpyxl.drawing.image.Image(output_path1 + '/' + 'UDP_Partially_drop.png')
            img5 = openpyxl.drawing.image.Image(output_path1 + '/' + 'UDP_Fully_drop.png')

            sheet.add_image(img1, pos)
            sheet.add_image(img2, pos2)
            sheet.add_image(img3, pos3)
            sheet.add_image(img4, pos4)
            sheet.add_image(img5, pos5)

    def kpi_sheet_generation(self, output_excel_sheet):
        def df_style(val):
            return "font-weight: bold"

        try:
            df_input = pd.read_excel(output_excel_sheet, sheet_name='Overall_Summary_Count', engine='openpyxl', na_filter=False)
            logs_count = sum(df_input['Total_Logs'])
            resim_fail = sum(df_input['Resim_Fail_count'])
            resim_pass = sum(df_input['Resim_Pass_count'])
            resim_fail_DQ = sum(df_input['DQ_Fail_count'])
            resim_pass_DQ = sum(df_input['DQ_Pass_Count'])
            RL_fail_count = sum(df_input['RESIM_RL_Fail_Count'])
            RR_fail_count = sum(df_input['RESIM_RR_Fail_Count'])
            FL_fail_count = sum(df_input['RESIM_FL_Fail_Count'])
            FR_fail_count = sum(df_input['RESIM_FR_Fail_Count'])
            FC_fail_count = sum(df_input['RESIM_FC_Fail_Count'])

            RL_fail_count_DQ = sum(df_input['DQ_RL_Fail_Count'])
            RR_fail_count_DQ = sum(df_input['DQ_RR_Fail_Count'])
            FL_fail_count_DQ = sum(df_input['DQ_FL_Fail_Count'])
            FR_fail_count_DQ = sum(df_input['DQ_FR_Fail_Count'])
            FC_fail_count_DQ = sum(df_input['DQ_FC_Fail_Count'])

            date_wise_total_logs = df_input.groupby(['Date'], as_index=False)['Total_Logs'].sum()
            date_wise_resim_pass = df_input.groupby(['Date'], as_index=False)['Resim_Pass_count'].sum()
            date_wise_resim_fail = df_input.groupby(['Date'], as_index=False)['Resim_Fail_count'].sum()
            date_wise_dq_pass = df_input.groupby(['Date'], as_index=False)['DQ_Pass_Count'].sum()
            date_wise_dq_fail = df_input.groupby(['Date'], as_index=False)['DQ_Fail_count'].sum()

            month_wise_total_logs = df_input.groupby(['Month'], as_index=False)['Total_Logs'].sum()
            month_wise_resim_pass = df_input.groupby(['Month'], as_index=False)['Resim_Pass_count'].sum()
            month_wise_resim_fail = df_input.groupby(['Month'], as_index=False)['Resim_Fail_count'].sum()
            month_wise_dq_pass = df_input.groupby(['Month'], as_index=False)['DQ_Pass_Count'].sum()
            month_wise_dq_fail = df_input.groupby(['Month'], as_index=False)['DQ_Fail_count'].sum()

            df_input1 = pd.read_excel(output_excel_sheet, sheet_name='RESIM_Detailed_Info', engine='openpyxl')
            df_input2 = pd.read_excel(output_excel_sheet, sheet_name='DQ_Detailed_Info', engine='openpyxl')

            missed_scan_drop_dq = sum(df_input2['Missed_Scan_Indexes'])

            final_dict = {}

            for k in range(len(df_input1['Observation'])):
                if df_input1['RESIM_STATUS'][k] == 'FAIL':
                    error_captured = False
                    if 'UDP Packetloss found in sensor' in df_input1['Observation'][k]:
                        key = "UDP Packetloss found in sensor : "
                        key_created = False
                        if 'sensor[RL]' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if 'sensor[RR]' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if 'sensor[FL]' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if 'sensor[FR]' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if 'sensor[FC]' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1

                        else:
                            key = 'UDP Packetloss found in sensor'
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        error_captured = True

                    if 'CDC completely dropped' in df_input1['Observation'][k]:
                        key = "CDC completely dropped :"
                        key_created = False
                        if 'Sensor[RL]' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if 'Sensor[RR]' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if 'Sensor[FL]' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if 'Sensor[FR]' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if 'Sensor[FC]' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1

                        else:
                            key = 'CDC completely dropped at some ScanIndexes'
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        error_captured = True

                    if 'CDC Chunk Loss Occured' in df_input1['Observation'][k]:
                        key = "CDC partially dropped for continous cycles :"
                        key_created = False
                        if 'Cycles [RL]' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if 'Cycles [RR]' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if 'Cycles [FL]' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if 'Cycles [FR]' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if 'Cycles [FC]' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        else:
                            key = 'CDC Chunk Loss Occured'
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        error_captured = True

                    if 'Sensor Radar cycles diff is more than three cycles' in df_input1['Observation'][k]:
                        key = "Time Syncronization Error : "
                        key_created = False
                        if 'Pos [RL]' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if 'Pos [RR]' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if 'Pos [FL]' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if 'Pos [FR]' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if 'Pos [FC]' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1

                        else:
                            key = 'Sensor Radar cycles diff is more than three cycles'
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        error_captured = True

                    if 'No reason mentioned in xml file' in df_input1['Observation'][k]:
                        key = "No reason mentioned in xml file "
                        try:
                            final_dict[key] = final_dict[key] + 1
                        except:
                            final_dict[key] = 1
                        error_captured = True

                    if 'Sensor reset found' in df_input1['Observation'][k]:
                        key = "Sensor reset : "
                        key_created = False
                        if '[RL] Sensor reset' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if '[RR] Sensor reset' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if '[FL] Sensor reset' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if '[FR] Sensor reset' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if '[FC] Sensor reset' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        else:
                            try:
                                final_dict[df_input1['Observation'][k]] = final_dict[df_input1['Observation'][k]] + 1
                            except:
                                final_dict[df_input1['Observation'][k]] = 1
                        error_captured = True

                    if 'No UDP DATA Found in Sensor' in df_input1['Observation'][k]:
                        key = "No UDP DATA : "
                        key_created = False
                        if 'Sensor [RL]' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if 'Sensor [RR]' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if 'Sensor [FL]' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if 'Sensor [FR]' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if 'Sensor [FC]' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        else:
                            try:
                                final_dict[df_input1['Observation'][k]] = final_dict[df_input1['Observation'][k]] + 1
                            except:
                                final_dict[df_input1['Observation'][k]] = 1
                        error_captured = True

                    if '[MISMATCH_SCAN] in Sensor' in df_input1['Observation'][k]:
                        key = "Continous 3 cycle Scan Drop : "
                        key_created = False
                        if '[MISMATCH_SCAN] in Sensor [RL]' in df_input1['Observation'][k]:
                            key = key + ' RL'
                            key_created = True
                        if '[MISMATCH_SCAN] in Sensor [RR]' in df_input1['Observation'][k]:
                            key = key + ' RR'
                            key_created = True
                        if '[MISMATCH_SCAN] in Sensor [FL]' in df_input1['Observation'][k]:
                            key = key + ' FL'
                            key_created = True
                        if '[MISMATCH_SCAN] in Sensor [FR]' in df_input1['Observation'][k]:
                            key = key + ' FR'
                            key_created = True
                        if '[MISMATCH_SCAN] in Sensor [FC]' in df_input1['Observation'][k]:
                            key = key + ' FC'
                            key_created = True

                        if key_created:
                            try:
                                final_dict[key] = final_dict[key] + 1
                            except:
                                final_dict[key] = 1
                        else:
                            try:
                                final_dict[df_input1['Observation'][k]] = final_dict[df_input1['Observation'][k]] + 1
                            except:
                                final_dict[df_input1['Observation'][k]] = 1
                        error_captured = True

                    if not error_captured:
                        try:
                            final_dict[df_input1['Observation'][k]] = final_dict[df_input1['Observation'][k]] + 1
                        except:
                            final_dict[df_input1['Observation'][k]] = 1

        except Exception as e:
            print("sheet or column not found in output excel")
            print(e)

        df_summary = pd.DataFrame(columns=["Expression", "Data_Quality Yield(%)", "Counts"])

        overall_summary_datewise = pd.DataFrame()
        overall_summary_monthwise = pd.DataFrame()

        '''To extract overall KPI '''
        df_summary.loc[0] = ['Total Logs present in all xml files', ' ', logs_count]

        df_summary.loc[1] = ['DATA QUALITY STATUS :-->', ' ', ' ']
        df_summary.loc[2] = ['         Total No of passed logs  with no UDP/CDC drop ',
                             (resim_pass_DQ / logs_count) * 100, resim_pass_DQ]
        df_summary.loc[3] = ['         Total No of failed logs with partial(UDP/CDC) drop',
                             (resim_fail_DQ / logs_count) * 100, resim_fail_DQ]
        df_summary.loc[4] = [' ', ' ', ' ']

        df_summary.loc[5] = ['DATA QUALITY SENSOR FAILURE  :-->', ' ', ' ']
        df_summary.loc[6] = ['         RL Failure ', (RL_fail_count_DQ / logs_count) * 100, RL_fail_count_DQ]
        df_summary.loc[7] = ['         RR Failure ', (RR_fail_count_DQ / logs_count) * 100, RR_fail_count_DQ]
        df_summary.loc[8] = ['         FL Failure ', (FL_fail_count_DQ / logs_count) * 100, FL_fail_count_DQ]
        df_summary.loc[9] = ['         FR Failure ', (FR_fail_count_DQ / logs_count) * 100, FR_fail_count_DQ]
        df_summary.loc[10] = ['         FC Failure ', (FC_fail_count_DQ / logs_count) * 100, FC_fail_count_DQ]
        df_summary.loc[11] = [' ', ' ', ' ']

        df_summary.loc[12] = ['Total Missed Scan Indexes', ' ', missed_scan_drop_dq]
        df_summary.loc[13] = [' ', ' ', ' ']

        df_summary.loc[14] = ['RESIM STATUS :-->', ' ', ' ']
        df_summary.loc[15] = ['         Total No of passed logs ', (resim_pass / logs_count) * 100, resim_pass]
        df_summary.loc[16] = ['         Total No of failed logs', (resim_fail / logs_count) * 100, resim_fail]
        df_summary.loc[17] = [' ', ' ', ' ']

        df_summary.loc[18] = ['RESIM RADAR SENSOR FAILURE  :-->', ' ', ' ']
        df_summary.loc[19] = ['         RL Failure ', (RL_fail_count / logs_count) * 100, RL_fail_count]
        df_summary.loc[20] = ['         RR Failure ', (RR_fail_count / logs_count) * 100, RR_fail_count]
        df_summary.loc[21] = ['         FL Failure ', (FL_fail_count / logs_count) * 100, FL_fail_count]
        df_summary.loc[22] = ['         FR Failure ', (FR_fail_count / logs_count) * 100, FR_fail_count]
        df_summary.loc[23] = ['         FC Failure ', (FC_fail_count / logs_count) * 100, FC_fail_count]
        df_summary.loc[24] = [' ', ' ', ' ']

        df_summary.loc[26] = ['RESIM FAILURE REASONS :-->', ' ', ' ']

        dict_keys_final = list(final_dict.keys())
        dict_values_final = list(final_dict.values())

        for m in range(len(dict_keys_final)):
            df_summary.loc[m + 27] = [dict_keys_final[m], (dict_values_final[m] / logs_count) * 100,
                                      dict_values_final[m]]

        with pd.ExcelWriter(output_excel_sheet, engine='openpyxl', mode='a') as writer:
            df_summary.to_excel(writer, sheet_name='Overall_KPI', index=False)

        '''To extract datewise KPI '''
        overall_summary_datewise['Date'] = date_wise_total_logs['Date']
        overall_summary_datewise['Total_logs'] = date_wise_total_logs['Total_Logs']
        overall_summary_datewise['Resim_Pass_count'] = date_wise_resim_pass['Resim_Pass_count']
        overall_summary_datewise['Resim_Fail_count'] = date_wise_resim_fail['Resim_Fail_count']
        overall_summary_datewise['DQ_Pass_Count'] = date_wise_dq_pass['DQ_Pass_Count']
        overall_summary_datewise['DQ_Fail_count'] = date_wise_dq_fail['DQ_Fail_count']
        overall_summary_datewise['Resim_Status'] = np.where(overall_summary_datewise['Resim_Fail_count'] == 0, 'PASS',
                                                            'FAIL')
        overall_summary_datewise['DQ_Status'] = np.where(overall_summary_datewise['DQ_Fail_count'] == 0, 'PASS', 'FAIL')
        overall_summary_datewise['RESIM_Yield(%)'] = (overall_summary_datewise['Resim_Pass_count'] /
                                                      overall_summary_datewise['Total_logs']) * 100
        overall_summary_datewise['DQ_Yield(%)'] = (overall_summary_datewise['DQ_Pass_Count'] / overall_summary_datewise[
            'Total_logs']) * 100
        overall_summary_datewise = overall_summary_datewise.loc[:,
                                   ['Date', 'Resim_Status', 'DQ_Status', 'Total_logs', 'Resim_Pass_count',
                                    'Resim_Fail_count', 'DQ_Pass_Count', 'DQ_Fail_count', 'RESIM_Yield(%)',
                                    'DQ_Yield(%)']]

        with pd.ExcelWriter(output_excel_sheet, engine='openpyxl', mode='a') as writer:
            overall_summary_datewise.to_excel(writer, sheet_name='Datewise_KPI', index=False)

        '''To extract monthwise KPI '''
        overall_summary_monthwise['Month'] = month_wise_total_logs['Month']
        overall_summary_monthwise['Total_logs'] = month_wise_total_logs['Total_Logs']
        overall_summary_monthwise['Resim_Pass_count'] = month_wise_resim_pass['Resim_Pass_count']
        overall_summary_monthwise['Resim_Fail_count'] = month_wise_resim_fail['Resim_Fail_count']
        overall_summary_monthwise['DQ_Pass_Count'] = month_wise_dq_pass['DQ_Pass_Count']
        overall_summary_monthwise['DQ_Fail_count'] = month_wise_dq_fail['DQ_Fail_count']
        overall_summary_monthwise['Resim_Status'] = np.where(overall_summary_monthwise['Resim_Fail_count'] == 0, 'PASS',
                                                             'FAIL')
        overall_summary_monthwise['DQ_Status'] = np.where(overall_summary_monthwise['DQ_Fail_count'] == 0, 'PASS',
                                                          'FAIL')
        overall_summary_monthwise['RESIM_Yield(%)'] = (overall_summary_monthwise['Resim_Pass_count'] /
                                                       overall_summary_monthwise['Total_logs']) * 100
        overall_summary_monthwise['DQ_Yield(%)'] = (overall_summary_monthwise['DQ_Pass_Count'] /
                                                    overall_summary_monthwise['Total_logs']) * 100
        overall_summary_monthwise = overall_summary_monthwise.loc[:,
                                    ['Month', 'Resim_Status', 'DQ_Status', 'Total_logs', 'Resim_Pass_count',
                                     'Resim_Fail_count', 'DQ_Pass_Count', 'DQ_Fail_count', 'RESIM_Yield(%)',
                                     'DQ_Yield(%)']]

        with pd.ExcelWriter(output_excel_sheet, engine='openpyxl', mode='a') as writer:
            overall_summary_monthwise.to_excel(writer, sheet_name='Monthwise_KPI', index=False)

    def get_scan_diff(self, scan_diff):
        len_col = len(scan_diff.axes[1])
        diff_val = []

        for m in range(len(scan_diff)):
            if len_col == 5:
                value1, value2, value3, value4, value5 = scan_diff[scan_diff.columns.values[0]][m], \
                    scan_diff[scan_diff.columns.values[1]][m], scan_diff[scan_diff.columns.values[2]][m], \
                    scan_diff[scan_diff.columns.values[3]][m], scan_diff[scan_diff.columns.values[4]][m]
                max_scan_val, min_scan_val = max(int(value1), int(value2), int(value3), int(value4), int(value5)), min(
                    int(value1), int(value2), int(value3), int(value4), int(value5))
                max_diff_scan_value = int(max_scan_val) - int(min_scan_val)
                diff_val.append(max_diff_scan_value)

            elif len_col == 4:
                value1, value2, value3, value4 = scan_diff[scan_diff.columns.values[0]][m], \
                    scan_diff[scan_diff.columns.values[1]][m], scan_diff[scan_diff.columns.values[2]][m], \
                    scan_diff[scan_diff.columns.values[3]][m]
                max_scan_val, min_scan_val = max(int(value1), int(value2), int(value3), int(value4)), min(int(value1),
                                                                                                          int(value2),
                                                                                                          int(value3),
                                                                                                          int(value4))
                max_diff_scan_value = int(max_scan_val) - int(min_scan_val)
                diff_val.append(max_diff_scan_value)

            elif len_col == 3:
                value1, value2, value3 = scan_diff[scan_diff.columns.values[0]][m], \
                    scan_diff[scan_diff.columns.values[1]][m], scan_diff[scan_diff.columns.values[2]][m]
                max_scan_val, min_scan_val = max(int(value1), int(value2), int(value3)), min(int(value1), int(value2),
                                                                                             int(value3))
                max_diff_scan_value = int(max_scan_val) - int(min_scan_val)
                diff_val.append(max_diff_scan_value)

            elif len_col == 2:
                value1, value2 = scan_diff[scan_diff.columns.values[0]][m], scan_diff[scan_diff.columns.values[1]][m]
                max_scan_val, min_scan_val = max(int(value1), int(value2)), min(int(value1), int(value2))
                max_diff_scan_value = int(max_scan_val) - int(min_scan_val)
                diff_val.append(max_diff_scan_value)

            elif len_col == 1:
                value1 = scan_diff[scan_diff.columns.values[0]][m]
                max_scan_val, min_scan_val = int(value1), int(value1)
                max_diff_scan_value = int(max_scan_val) - int(min_scan_val)
                diff_val.append(max_diff_scan_value)

        return diff_val

    def get_details(self, file_name):
        scan_diff = pd.DataFrame()
        df_error_msgs_list = []
        overall_summary = pd.DataFrame()
        overall_summary_count = pd.DataFrame()
        dq_summary = pd.DataFrame()
        path_details = pd.DataFrame()
        merged_data = pd.DataFrame()
        merged_data_dq = pd.DataFrame()
        sensor_count = 0
        log_replay_mode = None

        t = Texttable()

        '''Parse the data'''
        try:
            self.modify_xml(file_name)
            xml_data = ET.parse(file_name)
            parent_tag = xml_data.getroot()
            child_tags = list(parent_tag)
            sub_child_tags = list(child_tags)
            tool_version = child_tags[0].text
            '''To get the particular index '''

            stripped_child_tags = str(child_tags).strip(' "\'\t\r\n').replace('\t', " ").split(',')

            for a in range(len(stripped_child_tags)):
                if 'RESIM_Log_Quality_Check_Summary' in stripped_child_tags[a]:
                    resim_index = a
                    break

            for b in range(len(stripped_child_tags)):
                if 'Overall_Log_Quality_Check_Summary' in stripped_child_tags[b]:
                    DQ_index = b
                    break

            for c in range(len(stripped_child_tags)):
                if 'Overall_Radar_Report_Summary' in stripped_child_tags[c]:
                    Radar_summary_index = c
                    break

            for e in range(len(stripped_child_tags)):
                if '.mf4' in stripped_child_tags[e].lower() or '.pcap' in stripped_child_tags[e].lower():
                    Log_index = e
                    break

            for h in range(len(stripped_child_tags)):
                if 'Overall_RESIM_Radar_Report_Summary' in stripped_child_tags[h]:
                    RESIM_Radar_summary_index = h
                    break
            
            for g in range(len(stripped_child_tags)):
                if 'CDC_Quality_Check_Summary' in stripped_child_tags[g]:
                    cdc_quality_check_summary_index = g
                    break
            
            for child in parent_tag:
                #print(child.tag, child.attrib)
                if child.tag == 'Sensors_Enabled':
                    #print(child.text)
                    sensor_list = (child.text).strip()
                    sensor_count = len(sensor_list.split())

            for child in parent_tag:
                #print(child.tag, child.attrib)
                if child.tag == 'LOG_REPLAY_MODE':
                    #print(child.text)
                    log_replay_mode = (child.text).strip()
                    


            '''To extract first level pass/fail details'''
            quality_check_summary_initial_temp = sub_child_tags[resim_index].text.strip(' "\'\t\r\n').replace('\t', " ")
            quality_check_summary_initial_v2 = quality_check_summary_initial_temp.split('\n')[0]
            if '-------+-' in quality_check_summary_initial_v2:
                quality_check_summary_initial = quality_check_summary_initial_temp.split(
                    quality_check_summary_initial_v2)
            initial_summary_col = (quality_check_summary_initial[1].strip(' "\'\t\r\n')).split('|')
            initial_summary_col_list = [var2.strip(' "\'\t\r\n') for var2 in initial_summary_col if var2]

            initial_summary = pd.DataFrame(columns=initial_summary_col_list)

            for i in range(2, len(quality_check_summary_initial) - 1):
                reason = ''
                stripped_data = quality_check_summary_initial[i].strip(' "\'\t\r\n').replace('\t', " ").split()
                if 'FAIL' in stripped_data[2]:
                    for j in range(3, len(stripped_data)):
                        reason += stripped_data[j] + ' '
                    if bool(reason):
                        reason = reason
                    else:
                        reason = 'No reason mentioned in xml file'

                else:
                    reason = 'No Failure observed'

                initial_summary.loc[len(initial_summary)] = [stripped_data[0], stripped_data[1], stripped_data[2], reason]

            #initial_summary.to_excel('initial_summary.xlsx')
            logs_count = len(initial_summary[initial_summary.columns[1]])
            resim_fail = sum(initial_summary[initial_summary.columns[2]] == 'FAIL')
            resim_pass = sum(initial_summary[initial_summary.columns[2]] == 'PASS')
            if resim_fail == 0:
                resim_status = 'PASS'
            else:
                resim_status = 'FAIL'

            '''To extract log details'''
            try:
                log_details_list = list(sub_child_tags[Log_index])
                log_details_tags = str(log_details_list).strip(' "\'\t\r\n').replace('\t', " ").split(',')
                for g in range(len(log_details_tags)):
                    if 'LOGPATH' in log_details_tags[g]:
                        logpath_index = g
                        break
                log = log_details_list[logpath_index].text.strip(' "\'\t\r\n').replace('\t', " ").split('\\')
                log_path = os.path.dirname(log[0])

            except Exception as e:
                exc_type, exc_obj, exc_tb = sys.exc_info()
                error_msg = "ERROR " + str(e) + " FOUND IN LINE: " + str(exc_tb.tb_lineno)
                print(error_msg)
                log_path = 'Input Location Unknown'

            logname = initial_summary['Log name'][0]

            if 'Resim_Radars_deb' not in logname:
                try:set_no_value = (logname.split('_'))[-2]
                except:set_no_value = None
                #print('Set No is: ', set_no_value)
                try:
                    var_temp = (logname.split('_'))
                    date = None
                    month = None
                    date_flag = False
                    for var1 in var_temp:
                        if len(var1) == 8:
                            try:
                                date_object = datetime.strptime(str(var1), "%Y%m%d")
                                month = datetime.strftime(date_object, "%B")
                                date = var1
                                date_flag = True
                            except:
                                date = (logname.split('_'))[-2]
                                month = (logname.split('_'))[-2]
                            break
                    if not date_flag:
                        date = (logname.split('_'))[-2]
                        month = (logname.split('_'))[-2]

                except:
                    date = (logname.split('_'))[-2]
                    month = (logname.split('_'))[-2]

                try:vehicle = (logname.split('_'))[1]
                except:vehicle = None
            else:
                try:set_no_value = ((logname.split('_'))[1])[9:]
                except:set_no_value = None

                try:date = ((logname.split('_'))[1])[:8]
                except:date = None

                try:month = date[:-2]
                except:month = None

                try:vehicle = (logname.split('_'))[0]
                except:vehicle = None

            '''To extract CDC level details'''
            cdc_quality_check_summary_initial_temp = sub_child_tags[cdc_quality_check_summary_index].text.strip(' "\'\t\r\n').replace('\t', " ")
            cdc_quality_check_summary_initial_v2 = cdc_quality_check_summary_initial_temp.split('\n')[0]
            if '-------+-' in cdc_quality_check_summary_initial_v2:
                cdc_quality_check_summary_initial = cdc_quality_check_summary_initial_temp.split(
                cdc_quality_check_summary_initial_v2)
            cdc_initial_summary_col = (cdc_quality_check_summary_initial[1].strip(' "\'\t\r\n')).split('|')
            cdc_initial_summary_col_list = [var2.strip(' "\'\t\r\n') for var2 in cdc_initial_summary_col if var2]
            cdc_initial_summary = pd.DataFrame(columns=cdc_initial_summary_col_list)

            for i in range(2, len(cdc_quality_check_summary_initial) - 1):
                temp_var = cdc_quality_check_summary_initial[i].replace('NOT SATURATED', 'NOT_SATURATED')
                stripped_data = temp_var.strip(' "\'\t\r\n').replace('\t', " ").split()
                #print('The stipped data is: ', stripped_data)
                #print('length of stripped data is: ', len(stripped_data))
    
                cdc_initial_summary.loc[len(cdc_initial_summary)] = [stripped_data[x] for x in range(len(stripped_data))]

            #print('CDC_initial_summary is: \n', cdc_initial_summary)


            '''To extract scans details for all cases'''
            # quality_check_summary_details= sub_child_tags[DQ_index].text.strip(' "\'\t\r\n').replace('\t'," ").split('-------+------------------------------------------------------------------------------------------------------+-----------------+----------------------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+--------------------+---------------+---------------------+---------------------+---------------------+---------------------+---------------------+--------------------------+--------------------------+--------------------------+--------------------------+-')
            # quality_check_summary_details= sub_child_tags[DQ_index].text.strip(' "\'\t\r\n').replace('\t'," ").split('-------+------------------------------------------------------------------------------------------------------+-----------------+----------------------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+--------------------+---------------+---------------------+-----------------+------------+---------------------+-----------------+------------+---------------------+---------------------+---------------------+--------------------------+--------------------------+--------------------------+--------------------------+-')

            quality_check_summary_details2 = sub_child_tags[DQ_index].text.strip(' "\'\t\r\n').replace('\t', " ")
            quality_check_summary_details3 = quality_check_summary_details2.split('\n')[0]
            if '-------+-' in quality_check_summary_details3:
                quality_check_summary_details4 = quality_check_summary_details2.split(quality_check_summary_details3)

            stripped_data_details = quality_check_summary_details4[2].strip(' "\'\t\r\n').replace('\t', " ").split()
            # stripped_data_details = quality_check_summary_details[2].strip(' "\'\t\r\n').replace('\t'," ").split()

            stripped_col_temp = quality_check_summary_details4[1].strip(' "\'\t\r\n').split('|')
            stripped_col_details = [var2.strip(' "\'\t\r\n') for var2 in stripped_col_temp if var2]

            failure_analysis = pd.DataFrame(columns=stripped_col_details)

            stripped_data_details_list = []
            start = 0  # Initialize starting index for slicing
            split_value = len(stripped_col_details)
            # Iterate through list to find
            # indices of the split value

            for i, value in enumerate(stripped_data_details):
                if i % split_value == 0:
                    # Add the sublist from start to the current index
                    stripped_data_details_list.append(stripped_data_details[start:(start + split_value)])

                    # Update start to next index after the split value
                    start = i + split_value

            # Add the last sublist if there are remaining elements
            if start < len(stripped_data_details):
                stripped_data_details_list.append(stripped_data_details[start:])

            for var_list in stripped_data_details_list:
                failure_analysis.loc[(len(failure_analysis))] = var_list

            '''To get the sensors values if any particular sensor is disabled'''

            #failure_analysis.to_excel('failure_analysis.xlsx')
            scan_diff['RL'] = failure_analysis['RL_SCANS'].replace('NA', np.nan)
            scan_diff['RR'] = failure_analysis['RR_SCANS'].replace('NA', np.nan)
            scan_diff['FR'] = failure_analysis['FR_SCANS'].replace('NA', np.nan)
            scan_diff['FL'] = failure_analysis['FL_SCANS'].replace('NA', np.nan)
            scan_diff['FC'] = failure_analysis['FC_SCANS'].replace('NA', np.nan)
            scan_diff = scan_diff.dropna(axis=1)

            diff_val = self.get_scan_diff(scan_diff)
            failure_analysis['TimeSync_issue'] = diff_val

            #print('The merged data is: \n', merged_data)
            merged_data = pd.merge(initial_summary,failure_analysis, how='inner')
            merged_data.insert(0, 'Set_No', set_no_value)
            #print('The updated merged data is: \n', merged_data)

            '''To extract indexes details '''
            # dq_failure_results = sub_child_tags[DQ_index].text.strip(' "\'\t\r\n').replace('\t'," ").split('-------+------------------------------------------------------------------------------------------------------+------------------------------+---------------------+-----------------------------------+---------------------------+---------------+---------------+---------------+---------------+---------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------------+-')
            quality_check_summary_details5 = sub_child_tags[DQ_index].text.strip(' "\'\t\r\n').replace('\t', " ")
            dq_failure_results2 = sub_child_tags[DQ_index].text.strip(' "\'\t\r\n').replace('\t', " ")
            quality_check_summary_initial_temp2 = quality_check_summary_details5.split('\n')
            dq_failure_results3 = dq_failure_results2.split('\n')
            count = 0
            for x in dq_failure_results3:
                if '-------+-' in x:
                    count = count + 1
                    if count == 4:
                        index = dq_failure_results3.index(x)

            dq_failure_results4 = dq_failure_results3[index]

            if '-------+-' in dq_failure_results4:
                dq_failure_results5 = dq_failure_results2.split(dq_failure_results4)

            stripped_dq_details = dq_failure_results5[2].strip(' "\'\t\r\n').replace('\t', " ").split()
            # stripped_dq_details = dq_failure_results[2].strip(' "\'\t\r\n').replace('\t', " ").split()

            stripped_col_temp_dq_2 = dq_failure_results5[1].strip(' "\'\t\r\n').split('|')
            stripped_col_dq_2 = [var2.strip(' "\'\t\r\n') for var2 in stripped_col_temp_dq_2 if var2]
            failure_analysis_dq = pd.DataFrame(columns=stripped_col_dq_2)

            stripped_dq_2_details_list = []
            start_dq = 0  # Initialize starting index for slicing
            split_value = len(stripped_col_dq_2)
            # Iterate through list to find
            # indices of the split value
            for i, value in enumerate(stripped_dq_details):
                if i % split_value == 0:
                    stripped_dq_2_details_list.append(stripped_dq_details[start_dq:(start_dq + split_value)])
                    # Update start to next index after the split value
                    start_dq = i + split_value

            # Add the last sublist if there are remaining elements
            if start_dq < len(stripped_dq_details):
                stripped_dq_2_details_list.append(stripped_dq_details[start_dq:])

            #print('stripped_dq_2_details_list is: ', stripped_dq_2_details_list)
            for var_list in stripped_dq_2_details_list:
                #print('var list length is: ', len(var_list))
                failure_analysis_dq.loc[(len(failure_analysis_dq))] = var_list

            dq_summary['DQ_Status'] = failure_analysis['STATUS of packet loss tool']
            dq_summary['Total_Scan_Indexes'] = failure_analysis_dq['Total Number Of Scan Indexes']
            dq_summary['Missed_Scan_Indexes'] = failure_analysis_dq['Missed Scan Indexes']
            if 'Scan Indexes drop per minute' in stripped_col_dq_2:
                dq_summary['Scan Indexes drop per minute per sensor'] = failure_analysis_dq['Scan Indexes drop per minute'].astype(float)
            if 'Percentage Of Missed Scan Indexes' in stripped_col_dq_2:
                dq_summary['Percentage Of Missed Scan Indexes'] = failure_analysis_dq['Percentage Of Missed Scan Indexes'].astype(float)
                avg_scan_index_drop_percentage_temp = (dq_summary['Percentage Of Missed Scan Indexes'].sum())/len(dq_summary)
                avg_scan_index_drop_percentage = round(avg_scan_index_drop_percentage_temp, 2)
            if 'CDC SATURATION STATUS' in cdc_initial_summary_col_list:
                dq_summary['CDC SATURATION STATUS'] = cdc_initial_summary['CDC SATURATION STATUS']
            merged_data_dq = pd.concat([initial_summary['Log name'], dq_summary], axis=1)
            merged_data_dq.insert(0, 'Set_No', str(set_no_value))

            resim_fail_DQ = sum(dq_summary['DQ_Status'] == 'FAIL')
            resim_pass_DQ = sum(dq_summary['DQ_Status'] == 'PASS')
            #print('length of dq logs are: ', len(dq_summary))
            #print('length of sensor count is: ', sensor_count)
            #print('the sum of scan indexes drop per minute per second is: ', dq_summary['Scan Indexes drop per minute per sensor'].sum())
            #print('The dq summary is: ', dq_summary)
            if sensor_count != 0:
                avg_scan_index_drop_temp = ((dq_summary['Scan Indexes drop per minute per sensor'].sum())/len(dq_summary))*sensor_count
                avg_scan_index_drop = round(avg_scan_index_drop_temp, 2)
            if resim_fail_DQ == 0:
                DQ_final_status = 'PASS'
            else:
                DQ_final_status = 'FAIL'

            '''To extract RESIM Radar report summary details'''
            RL_loss = 0
            RR_loss = 0
            FL_loss = 0
            FR_loss = 0
            FC_loss = 0
            radar_summary = sub_child_tags[RESIM_Radar_summary_index].text.strip(' "\'\t\r\n').replace('\t', " ").split(
                'Num of Logs Failed for RESIM for Radar Pos :')
            for f in range(1, len(radar_summary)):
                stripped_radar_data = radar_summary[f].strip(' "\'\t\r\n').replace('\t', " ").split(':')
                if 'RL' in stripped_radar_data[0] and len(stripped_radar_data[0][:-1]) == 2:
                    RL_loss = stripped_radar_data[1][1:]
                if 'RR' in stripped_radar_data[0] and len(stripped_radar_data[0][:-1]) == 2:
                    RR_loss = stripped_radar_data[1][1:]
                if 'FL' in stripped_radar_data[0] and len(stripped_radar_data[0][:-1]) == 2:
                    FL_loss = stripped_radar_data[1][1:]
                if 'FR' in stripped_radar_data[0] and len(stripped_radar_data[0][:-1]) == 2:
                    FR_loss = stripped_radar_data[1][1:]
                if 'FC' in stripped_radar_data[0] and len(stripped_radar_data[0][:-1]) == 2:
                    FC_loss = stripped_radar_data[1][1:]

            '''To extract Radar report summary details'''
            RL_loss_DQ = 0
            RR_loss_DQ = 0
            FL_loss_DQ = 0
            FR_loss_DQ = 0
            FC_loss_DQ = 0
            radar_summary_DQ = sub_child_tags[Radar_summary_index].text.strip(' "\'\t\r\n').replace('\t', " ").split(
                'Num of Logs Failed for Radar Pos :')
            for w in range(1, len(radar_summary_DQ)):
                stripped_radar_data_dq = radar_summary_DQ[w].strip(' "\'\t\r\n').replace('\t', " ").split(':')
                if 'RL' in stripped_radar_data_dq[0] and len(stripped_radar_data_dq[0][:-1]) == 2:
                    RL_loss_DQ = stripped_radar_data_dq[1][1:]
                if 'RR' in stripped_radar_data_dq[0] and len(stripped_radar_data_dq[0][:-1]) == 2:
                    RR_loss_DQ = stripped_radar_data_dq[1][1:]
                if 'FL' in stripped_radar_data_dq[0] and len(stripped_radar_data_dq[0][:-1]) == 2:
                    FL_loss_DQ = stripped_radar_data_dq[1][1:]
                if 'FR' in stripped_radar_data_dq[0] and len(stripped_radar_data_dq[0][:-1]) == 2:
                    FR_loss_DQ = stripped_radar_data_dq[1][1:]
                if 'FC' in stripped_radar_data_dq[0] and len(stripped_radar_data_dq[0][:-1]) == 2:
                    FC_loss_DQ = stripped_radar_data_dq[1][1:]

            overall_summary['Log_Set_No'] = [str(set_no_value)]
            overall_summary['Log_set_path'] = [log_path]
            overall_summary['RESIM_Status'] = [resim_status]
            overall_summary['Data_Quality_Status'] = [DQ_final_status]
            overall_summary['Total_Logs'] = [logs_count]
            overall_summary['Resim_Pass(%)'] = [(resim_pass / logs_count) * 100]
            overall_summary['DQ_Pass(%)'] = [(resim_pass_DQ / logs_count) * 100]
            overall_summary['RESIM_RL_Failure(%)'] = [(int(RL_loss) / logs_count) * 100]
            overall_summary['RESIM_RR_Failure(%)'] = [(int(RR_loss) / logs_count) * 100]
            overall_summary['RESIM_FL_Failure(%)'] = [(int(FL_loss) / logs_count) * 100]
            overall_summary['RESIM_FR_Failure(%)'] = [(int(FR_loss) / logs_count) * 100]
            overall_summary['RESIM_FC_Failure(%)'] = [(int(FC_loss) / logs_count) * 100]
            overall_summary['DQ_RL_Failure(%)'] = [(int(RL_loss_DQ) / logs_count) * 100]
            overall_summary['DQ_RR_Failure(%)'] = [(int(RR_loss_DQ) / logs_count) * 100]
            overall_summary['DQ_FL_Failure(%)'] = [(int(FL_loss_DQ) / logs_count) * 100]
            overall_summary['DQ_FR_Failure(%)'] = [(int(FR_loss_DQ) / logs_count) * 100]
            overall_summary['DQ_FC_Failure(%)'] = [(int(FC_loss_DQ) / logs_count) * 100]
            overall_summary['Resim_Fail(%)'] = [(resim_fail / logs_count) * 100]
            overall_summary['DQ_Fail(%)'] = [(resim_fail_DQ / logs_count) * 100]
            if sensor_count !=0:
                overall_summary['Scan Indexes drop per minute'] = [avg_scan_index_drop]
            
            overall_summary['Percentage Of Missed Scan Indexes'] = [avg_scan_index_drop_percentage]
            overall_summary['LOG_REPLAY_MODE'] = [log_replay_mode]
            
            overall_summary_count['Log_Set_No'] = [str(set_no_value)]
            overall_summary_count['Log_set_path'] = [log_path]
            overall_summary_count['RESIM_Status'] = [resim_status]
            overall_summary_count['Data_Quality_Status'] = [DQ_final_status]
            overall_summary_count['Total_Logs'] = [logs_count]
            overall_summary_count['Resim_Pass_count'] = [resim_pass]
            overall_summary_count['Resim_Fail_count'] = [resim_fail]
            overall_summary_count['DQ_Pass_Count'] = [resim_pass_DQ]
            overall_summary_count['DQ_Fail_count'] = [resim_fail_DQ]
            overall_summary_count['RESIM_RL_Fail_Count'] = [int(RL_loss)]
            overall_summary_count['RESIM_RR_Fail_Count'] = [int(RR_loss)]
            overall_summary_count['RESIM_FL_Fail_Count'] = [int(FL_loss)]
            overall_summary_count['RESIM_FR_Fail_Count'] = [int(FR_loss)]
            overall_summary_count['RESIM_FC_Fail_Count'] = [int(FC_loss)]
            overall_summary_count['DQ_RL_Fail_Count'] = [int(RL_loss_DQ)]
            overall_summary_count['DQ_RR_Fail_Count'] = [int(RR_loss_DQ)]
            overall_summary_count['DQ_FL_Fail_Count'] = [int(FL_loss_DQ)]
            overall_summary_count['DQ_FR_Fail_Count'] = [int(FR_loss_DQ)]
            overall_summary_count['DQ_FC_Fail_Count'] = [int(FC_loss_DQ)]
            overall_summary_count['Tool_version'] = [tool_version]
            overall_summary_count['Date'] = [date]
            overall_summary_count['Month'] = [month]
            overall_summary_count['Vehicle'] = [vehicle]

            path_details['Set_No'] = [set_no_value]
            path_details['File_path'] = [file_name]

        except Exception as error:
            exc_type, exc_obj, exc_tb = sys.exc_info()
            error_msg = "ERROR: " + str(error) + " FOUND IN LINE: " + str(exc_tb.tb_lineno)
            df_error_msgs_list.append([file_name, error_msg])
            print('\n      ## XML FILE PARSING ERROR :--> ')
            t.add_rows([['File_name', 'Error'], [file_name, error_msg]])
            print(t.draw())

        df_error_msgs = pd.DataFrame(data=df_error_msgs_list,columns=['file_path', 'error_message'])
        return merged_data, merged_data_dq, overall_summary, overall_summary_count, path_details, df_error_msgs

    def check_validity(self, file_path):
        '''To check whether path provided is correct or not'''
        is_valid = False
        if os.path.isdir(file_path):
            is_valid = True

        return is_valid

    def modify_xml(self, file_path):
        with open(file_path, 'r') as f:
            data = f.readlines()
        #print(len(data))
        with open(file_path, 'w') as f:
            for var in data:
                # for line in var.split('\n'):
                if ('<Log_' in var or '</Log_' in var) and '.mf4>' in var:
                    # old_line.append(line)
                    # new_line.append(line.replace('SRR6','SRR6P'))
                    var = var.replace('SRR6+', 'SRR6P')
                    # print(var)
                    # index = var.index(line)
                    # var[index] = newline
                f.write(var)

    def run(self, file_name, **kwargs):
        try:
            merged_data, merged_data_dq, overall_summary, overall_summary_count, path_details, df_error_msgs = self.get_details(
                file_name)

            out = dict()
            out['RESIM_Detailed_Info'] = merged_data
            out['DQ_Detailed_Info'] = merged_data_dq
            out['Overall_Summary'] = overall_summary
            out['Overall_Summary_Count'] = overall_summary_count
            out['DQ_xml_path_details'] = path_details
            out['error_msgs'] = df_error_msgs
            return out

        except Exception as error:
            exc_type, exc_obj, exc_tb = sys.exc_info()
            error_msg1 = "ERROR " + str(error) + " FOUND IN LINE: " + str(exc_tb.tb_lineno)

def sort_dqFlist(file_list):
    print(file_list)
    modifieData=[]
    with open(file_list,'r') as file:
       rawdata=file.readlines()

    splitter = f"/{rawdata[0].split('/')[-2]}/"
    path=rawdata[0].split(splitter)[0]

    for i in range(0,len(rawdata)):
        modifieData.append(f"{path}/{i+1}/Data_Logging_Quality.xml")

    with open(file_list,'w') as file:
        for line in modifieData:
            file.write(f"{line}\n")


if __name__ == '__main__':
    print("[INFO] : DQ Mining Starting")
    q = 'n'
    if q == 'n':
        kwargs = dict()
        Test = DataQualityToolExtraction()
        print("\nInitializing Setup..........................")
        filelist = []
        dfa = pd.DataFrame()
        dfc = pd.DataFrame()
        dfb = pd.DataFrame()
        dfh = pd.DataFrame()
        dfg = pd.DataFrame()
        dfd = pd.DataFrame()
        dfe = pd.DataFrame()
        count = 0
        err_count = 0
        dir_err_count = 0
        blank_count = 0

        script_path = sys.argv[0]  # path of script
        file_list = sys.argv[1]  # pass name of filelist as an argument
        #sort_dqFlist(file_list)
        # file_list = r"C:\Users\qj743z\Documents\Simulation_Team\DC\CDC\DQ_fList.txt"
        

        if len(sys.argv) == 3:  # If output path is provided
            output_path1 = sys.argv[2]
        else:  # take default path of script path
            new_path = script_path[:-36]  # remove script name
            new_dir = "DQ_output_results"
            output_path1 = os.path.join(new_path, new_dir)

        if not (os.path.isdir(output_path1)):
            os.mkdir(output_path1)
        else:
            pass

        print("\n*************************** START OF SCRIPT *************************************")

        if file_list.endswith('.txt'):
            print("Checking xml files in xml filelist (DQ_fList.txt) :-->")
            with open(file_list) as file:
                filelist = file.read().splitlines()
                #print(filelist)
            print("Done!!!!")

            for j in range(len(filelist)):
                if not bool(filelist[j]):
                    blank_count += 1

            print("\nPROCESSING DETAILS :--> \n")
            print("   ** Extraction will start for " + str(len(filelist) - blank_count) + " sets")
            print("\n   ** Processing started!!!!!!")
            for i in range(len(filelist)):
                #print(filelist)
                try:
                    
                    file_path, file_name = os.path.split(filelist[i])
                    if len(file_path) != 0:
                        validity_check = Test.check_validity(file_path)
                        if validity_check:
                            cell = Test.run(filelist[i], **kwargs)

                            if len(cell['RESIM_Detailed_Info']) != 0 or len(cell['Overall_Summary']) != 0 or len(
                                    cell['DQ_Detailed_Info']) != 0 or \
                                    len(cell['DQ_xml_path_details']) != 0 or len(cell['Overall_Summary_Count']) != 0:
                                df4 = pd.DataFrame(cell['Overall_Summary'])
                                dfb = pd.concat([dfb, df4], axis=0)
                                  
                                df3 = pd.DataFrame(cell['RESIM_Detailed_Info'])
                                dfc = pd.concat([dfc, df3], axis=0)
                                    
                                df5 = pd.DataFrame(cell['DQ_Detailed_Info'])
                                dfd = pd.concat([dfd, df5], axis=0)
                                    
                                df6 = pd.DataFrame(cell['DQ_xml_path_details'])
                                dfe = pd.concat([dfe,df6],axis=0)
                                    
                                df7 = pd.DataFrame(cell['Overall_Summary_Count'])
                                dfa = pd.concat([dfa, df7], axis=0)
                                count += 1

                            if len(pd.DataFrame(cell['error_msgs'])) != 0:
                                df9 = pd.DataFrame(cell['error_msgs'])
                                dfh = pd.concat([dfh, df9], axis=0)
                                err_count += 1

                            sys.stdout.write('\r')
                            sys.stdout.flush()
                        else:
                            print("\n      ## Issue in directory path for : " + filelist[i])
                            dir_err_count += 1
                    else:
                        pass

                except Exception as e:
                    exc_type, exc_obj, exc_tb = sys.exc_info()
                    error_msg = "ERROR " + \
                                str(e) + " FOUND IN LINE: " + str(exc_tb.tb_lineno)

            if dir_err_count != 0:
                print("\n   ** Directory Error observed for " + str(dir_err_count) + " sets")
            if count != 0:
                print("\n   ** Extraction completed successfully for " + str(count) + " sets")
            if err_count != 0:
                print("\n   ** Parsing Error observed for " + str(err_count) + " sets\n")

            opsys = platform.system()
            if opsys == 'Windows':
                folder_name = (file_list.split('\\')[-2]).split('OUTPUT_DIR_')[-1]   #local running
                output_path = output_path1 + '\\' + 'DQ_extraction_report1' + '_' + folder_name + '.xlsx'         #for local running
                output_txt = output_path1 + '\\'
            elif opsys == 'Linux':
                folder_name = (file_list.split('/')[-2]).split('OUTPUT_DIR_')[-1]  # for server running
                output_path = output_path1 + '/' + 'DQ_extraction_report' + '_' + folder_name + '.xlsx'  # for server running
                output_txt = output_path1 + '/'

            print("\nEXTRACTION DETAILS :-->\n")
            with pd.ExcelWriter(output_path) as writer:
                try:
                    dfb.to_excel(writer, sheet_name='Overall_Summary', index=False)
                    print('Overall Summary details extracted in sheet for ' + str(len(dfb)) + ' sets')
                except:
                    print('Overall Summary not extracted in sheet')

                try:
                    if 'Log_No' in dfc.columns:
                        dfc.drop('Log_No', axis=1, inplace=True)
                    if 'STATUS of RESIM' in dfc.columns:
                        dfc.drop('STATUS of RESIM', axis=1, inplace=True)
                    if 'STATUS of packet loss tool' in dfc.columns:
                        dfc.drop('STATUS of packet loss tool', axis=1, inplace=True)
                    dfc.to_excel(writer, sheet_name='RESIM_Detailed_Info', index=False)
                    print('Resim Log details extracted in sheet for ' + str(len(dfb)) + ' sets')
                except:
                    print('Resim Log details not extracted in sheet')
                try:
                    dfd.to_excel(writer, sheet_name='DQ_Detailed_Info', index=False)
                    print('DQ Log details extracted in sheet for ' + str(len(dfb)) + ' sets')
                except:
                    print('DQ Log details not extracted in sheet')
                try:
                    dfe.to_excel(writer, sheet_name='DQ_xml_path_details', index=False)
                    print('DQ path details extracted in sheet for ' + str(len(dfe)) + ' sets')
                except:
                    print('DQ path details not extracted in sheet')
                try:
                    dfa.to_excel(writer, sheet_name='Overall_Summary_Count', index=False)
                    print('Overall Summary count details extracted in sheet for ' + str(len(dfb)) + ' sets')
                except:
                    print('Overall Summary count details not extracted in sheet')
                try:
                    dfh.to_excel(
                        writer, sheet_name='error_msgs', index=False)
                    print('Error found in ' + str(len(dfh)) + ' xml files')
                except:
                    print('No errors found in xml files')

            wb = load_workbook(output_path)
            sheet_to_color = wb["Overall_Summary"]

            red_fill = PatternFill(bgColor="FF4433")
            green_fill = PatternFill(bgColor="90EE90")
            yellow_fill = PatternFill(bgColor="FFBF00")
            dxf = DifferentialStyle(fill=red_fill)
            dxf1 = DifferentialStyle(fill=green_fill)
            dxf2 = DifferentialStyle(fill=yellow_fill)

            '''To get colour codes as red and green'''
            # Formulae for RESIM_STATUS
            #r = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            #r1 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            #r2 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            #r.formula = ['$C2="FAIL"']
            #r1.formula = ['$C2="PASS"']
            #r2.formula = ['$C2="FAIL"' and '$F2>0']
            #range_to_color = "C2:C" + str(len(dfb) + 1)
            #sheet_to_color.conditional_formatting.add(range_to_color, r)
            #sheet_to_color.conditional_formatting.add(range_to_color, r1)
            #sheet_to_color.conditional_formatting.add(range_to_color, r2)

            # Formulae for DQ_STATUS
            #r3 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            #r4 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            #r3.formula = ['$D2="FAIL"']
            #r4.formula = ['$D2="PASS"']
            #range_to_color1 = "D2:D" + str(len(dfb) + 1)
            #sheet_to_color.conditional_formatting.add(range_to_color1, r3)
            #sheet_to_color.conditional_formatting.add(range_to_color1, r4)

            #To get colour codes as red , orange and green
                
            # Formulae for RESIM_STATUS
            r = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r1 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r2 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r.formula = ['$C2="FAIL"' and '$F2=0']
            r1.formula = ['$C2="PASS"']
            r2.formula = ['$C2="FAIL"' and '$F2>0']
            range_to_color = "C2:C" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color, r)
            sheet_to_color.conditional_formatting.add(range_to_color, r1)
            sheet_to_color.conditional_formatting.add(range_to_color, r2)

            # Formulae for DQ_STATUS
            r3 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r4 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r3.formula = ['$D2="FAIL"']
            r4.formula = ['$D2="PASS"']
            range_to_color1 = "D2:D" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color1, r3)
            sheet_to_color.conditional_formatting.add(range_to_color1, r4)

            sheet_to_color = wb["RESIM_Detailed_Info"]

            r3 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r4 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r3.formula = ['$C2="FAIL"']
            r4.formula = ['$C2="PASS"']
            range_to_color1 = "C2:C" + str(len(dfc) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color1, r3)
            sheet_to_color.conditional_formatting.add(range_to_color1, r4)
            # Formulae for DQ_STATUS
            #r3 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            #r4 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            #r5 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            #r3.formula = ['$D2="FAIL"' and '$G2=0']
            #r4.formula = ['$D2="PASS"']
            #r5.formula = ['$D2="FAIL"' and '$G2>0']
            #range_to_color1 = "D2:D" + str(len(dfb) + 1)
            #sheet_to_color.conditional_formatting.add(range_to_color1, r3)
            #sheet_to_color.conditional_formatting.add(range_to_color1, r4)
            #sheet_to_color.conditional_formatting.add(range_to_color1, r5)
            

            # Plotting
            try:
                Test.plot_generation(wb, 'RESIM_Detailed_Info', output_path, "AG2", "AP2", "AG31", "AP31", "AG65",
                                     output_path1)

            except:
                print("Some issue in adding charts")
                pass

            wb.save(output_path)
            Test.kpi_sheet_generation(output_path)
            '''change KPI Summary to 4th position'''
            wb = load_workbook(output_path)
            sheetnames = wb.sheetnames
            kpi = wb["Overall_KPI"]
            date_wise = wb["Datewise_KPI"]
            month_wise = wb["Monthwise_KPI"]

            if len(sheetnames) == 8:
                wb.move_sheet(kpi, -2)
                wb.move_sheet(date_wise, -2)
                wb.move_sheet(month_wise, -2)
            else:
                wb.move_sheet(kpi, -3)
                wb.move_sheet(date_wise, -3)
                wb.move_sheet(month_wise, -3)

            wb.save(output_path)
            dfb['resim_status_temp'] = ''
            dfb.loc[dfb['Resim_Pass(%)'] == 100, 'resim_status_temp'] = 'Pass'
            dfb.loc[dfb['Resim_Pass(%)'] == 0, 'resim_status_temp'] = 'Fail'
            dfb.loc[((dfb['Resim_Pass(%)'] > 0) & (dfb['Resim_Pass(%)'] < 100)), 'resim_status_temp'] = 'Partial'

            var_resim_trigger = []
            if (dfb['resim_status_temp'] == 'Pass').any():
                var_pass = dfb[(dfb['resim_status_temp'] == 'Pass')]['Log_set_path'].to_list()
                var_resim_trigger = var_resim_trigger + var_pass
                with open(output_txt+'Session_Pass.txt', 'w') as f:
                    for line in var_pass:
                        f.write(f"{line}\n")

            if (dfb['resim_status_temp'] == 'Partial').any():
                var_partial = dfb[(dfb['resim_status_temp'] == 'Partial')]['Log_set_path'].to_list()
                var_resim_trigger = var_resim_trigger + var_partial
                with open(output_txt+'Session_Partial.txt', 'w') as f:
                    for line in var_partial:
                        f.write(f"{line}\n")

            if (dfb['resim_status_temp'] == 'Fail').any():
                var = dfb[(dfb['resim_status_temp'] == 'Fail')]['Log_set_path'].to_list()
                with open(output_txt+'Session_Fail.txt', 'w') as f:
                    for line in var:
                        f.write(f"{line}\n")

            if var_resim_trigger:
                with open(output_txt + 'Resim_trigger_session.txt', 'w') as f:
                    for line in var_resim_trigger:
                        f.write(f"{line}\n")

            print('\nDetailed summary (errors & results) is available at generated report path')
            print('Output Excel is Saved at : ', output_path1)

        else:
            print("\nKindly pass correct input xml filelist ie (DQ_fList.txt) \n")

        print("\n****************************** END OF SCRIPT *************************************\n")

"""
######################################################################################################
DATE(DD/MM/YY)            NAME                         JIRA NUMBER DESCRIPTION
17/04/2024             Sonal Rajurkar           Initial script to extract details from data quality xml files 
18/04/2024             Sonal Rajurkar           Added the logic to extract:
                                                -- details for all the logs set wise 
                                                -- Combine xml files set wise and providing overall summary irrespective of set
                                                -- KPI Summary sheet will provide the statistics for overall summary
19/04/2024             Sonal Rajurkar           Added Set No in detailed Info page 
22/04/2024             Sonal Rajurkar           Modified output folder path to be same as script path
24/04/2024             Sonal Rajurkar           Added scan indexes details and segregated RESIM & DQ tabs separately
25/04/2024             Sonal Rajurkar           Updated KPI Summary tab, added colour codes for pass/ fail
29/04/2024             Sonal Rajurkar           Added the logic to extract log path details from log.txt file and added new sheet for xml path details
13/05/2024             Sonal Rajurkar           printing modifications, added function to check validity
14/06/2024             Sonal Rajurkar           Added logic to add scan diff for resim details & box plot to show it in plot
20/06/2024             Sonal Rajurkar           Added full & partial CDC drop values 
22/06/2024             Sonal Rajurkar           Added one sheet to extract date wise details
24/06/2024             Sonal Rajurkar           Added one sheet to extract month wise details,radar failure details
25/06/2024             Sonal Rajurkar           Added Failure reasons for RESIM in KPI sheet
04/07/2024             Sonal Rajurkar           Added yield% in KPI sheet
05/07/2024             Sonal Rajurkar           Modified logic to take log path details . Now those details will come from xml file itself. Added RESIM Radar failure counts  
12/07/2024             Sonal Rajurkar           Added few more failure reasons sensor vise  
17/07/2024             Sonal Rajurkar           If any sensors are disabled then added logic for the same
20/08/2024             Sonal Rajurkar           Added scatter plots for scan diff & cdc drop , minor changes in KPI sheet
10/03/2025             Nagaraj Bannadabavi      Updated code to read table columns automatically, minor changes handle dataframe columns effectively
######################################################################################################
"""