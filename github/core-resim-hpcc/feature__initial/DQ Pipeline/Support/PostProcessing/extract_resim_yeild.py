# -*- coding: utf-8 -*-
"""
Created on Mon Sep 30 15:39:39 2024

@author: qj743z
"""




import sys
import pandas as pd
import os
import numpy as np
from openpyxl import load_workbook
from openpyxl.styles.differential import DifferentialStyle
from openpyxl.styles import PatternFill
from openpyxl.formatting.rule import Rule

class ExtractResimYield():
    
    def __init__(self):
        self._func_name = os.path.basename(__file__).replace(".py", "")
        self._headers = dict()
        self._version = '0.1'
        
        self._headers['Yield_details_SCALE3'] = ['Set_No','Total_logs','Debug_Yield(%)','SRR6P_CAN_Yield(%)','FLR4P_mPad_Yield(%)','RESIM_Overall_Remarks',\
                                          'Debug_Pass_Count','Debug_Fail_Count','mPAD_Pass_Count','mPAD_Fail_Count','Log_remarks','Input_path','Output_path']
            
        self._headers['Log_Details_SCALE3'] = ['Set_No','Log_name','Debug_File_Present','Bus_File_Present','mPAD_File_Present','Debug_File_Size(MB)','Bus_File_Size(MB)','mPAD_File_Size(MB)','File_Status']
       
        self._headers['Yield_details_SCALE1'] = ['Set_No','Total_logs','Debug_Yield(%)','Bus_Yield(%)','RESIM_Overall_Remarks','Debug_Pass_count','Debug_Fail_count','Bus_Pass_Count','Bus_Fail_Count','Input_path','Output_path']
        
        self._headers['Log_Details_SCALE1'] = ['Set_No','Log_name','Debug_File_Present','Bus_File_Present','Debug_File_Size(MB)','Bus_File_Size(MB)','File_Status']
        
    def get_headers(self):
          """
          :return: returns headers
          """ 
          return self._headers  
    
    def kpi_sheet_generation(self, customer_name,output_excel_sheet):
        
        if customer_name == "stla_scale1":
            try:
                
                df_input_scale1 = pd.read_excel(output_excel_sheet,sheet_name='Yield_details_SCALE1', engine='openpyxl')
                logs_count = sum(df_input_scale1['Total_logs'])
                total_debug_pass_count = sum(df_input_scale1['Debug_Pass_count'])
                total_debug_fail_count = sum(df_input_scale1['Debug_Fail_count'])
                total_bus_pass_count = sum(df_input_scale1['Bus_Pass_Count'])
                total_bus_fail_count = sum(df_input_scale1['Bus_Fail_Count'])
                
            except Exception as e:
                    print("sheet or column not found in output excel")
                    print(e)
                
            df_summary = pd.DataFrame(columns=["Expression", "Counts"])
            
            df_summary.loc[0]=['Total Logs present in all files' ,logs_count]
            df_summary.loc[1]=['DEBUG FILES STATUS :-->',' ']
            df_summary.loc[2]=['         Total No of passed files ',total_debug_pass_count]
            df_summary.loc[3]=['         Total No of failed files',total_debug_fail_count]
            df_summary.loc[4]=['         Debug Yield %',(total_debug_pass_count/logs_count)*100]
            df_summary.loc[5]=[' ',' ']
            df_summary.loc[6]=['BUS FILES STATUS :-->',' ']
            df_summary.loc[7]=['         Total No of passed logs ',total_bus_pass_count]
            df_summary.loc[8]=['         Total No of failed logs',total_bus_fail_count]
            df_summary.loc[9]=['         Bus Yield %',(total_bus_pass_count/logs_count)*100]
          
            
        elif customer_name == "stla_scale3":
            try:
                
                df_input_scale3 = pd.read_excel(output_excel_sheet,sheet_name='Yield_details_SCALE3', engine='openpyxl')
                logs_count = sum(df_input_scale3['Total_logs'])
                total_debug_pass_count = sum(df_input_scale3['Debug_Pass_Count'])
                total_debug_fail_count = sum(df_input_scale3['Debug_Fail_Count'])
                total_mpad_pass_count = sum(df_input_scale3['mPAD_Pass_Count'])
                total_mpad_fail_count = sum(df_input_scale3['mPAD_Fail_Count'])
                
            except Exception as e:
                    print("sheet or column not found in output excel")
                    print(e)
                
            df_summary = pd.DataFrame(columns=["Expression", "Counts"])
            
            df_summary.loc[0]=['Total Logs present in all files' ,logs_count]
            df_summary.loc[1]=['DEBUG FILES STATUS :-->',' ']
            df_summary.loc[2]=['         Total No of passed files ',total_debug_pass_count]
            df_summary.loc[3]=['         Total No of failed files',total_debug_fail_count]
            df_summary.loc[4]=['         Debug Yield %',(total_debug_pass_count/logs_count)*100]
            df_summary.loc[5]=[' ',' ']
            df_summary.loc[6]=['mPAD FILES STATUS :-->',' ']
            df_summary.loc[7]=['         Total No of passed logs ',total_mpad_pass_count]
            df_summary.loc[8]=['         Total No of failed logs',total_mpad_fail_count]
            df_summary.loc[9]=['         mPAD Yield %',(total_mpad_pass_count/logs_count)*100]
        
        with pd.ExcelWriter(output_excel_sheet,engine='openpyxl',mode='a') as writer:
            df_summary.to_excel(writer, sheet_name='KPI_summary', index=False)
            
    def check_validity_input_SCALE3(self,customer_name,input_dir_path):
        debug_input_list = []
        mPad_input_list = []
        corner_CAN_list = []
        side_CAN_list = []
        is_valid_input = False
        print(f'input_dir_path : {input_dir_path}')
        
        if os.path.isdir(input_dir_path):
            is_valid_input = True    
            for root, dirs, files in os.walk(input_dir_path): 
                #folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('\\')
                folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('/')
                print(f' folder_name_input : { folder_name_input}')
                for file in files:
                    if 'Aptiv_FLR_Obj_Perc_for_mPAD' in file and '.MF4' in file:
                        mPad_input_list.append(str(os.path.join(root, file)))
                    elif 'Resim_Radars_deb' in file and '.MF4' in file:
                        debug_input_list.append(str(os.path.join(root, file)))
                    elif 'Resim_Radars_CAN_Corner_bus' in file and '.MF4' in file:
                        corner_CAN_list.append(str(os.path.join(root,file)))
                    elif 'Resim_Radars_CAN_Side_bus' in file and '.MF4' in file:
                        side_CAN_list.append(str(os.path.join(root,file)))
        print(f'returning : {is_valid_input}\n{debug_input_list}\n{corner_CAN_list}\n{side_CAN_list}\n{mPad_input_list}')
        return is_valid_input,debug_input_list,corner_CAN_list,side_CAN_list,mPad_input_list

    def check_validity_output_SCALE3(self,output_dir_path):
        log_list = []
        mPad_output_list = []
        debug_output_list = []
        
        txt_list = []
        
        details = []
        
        
        '''To check whether path provided is correct or not'''
        is_valid_output = False
       
        if os.path.isdir(output_dir_path):
            is_valid_output = True    
            for root, dirs, files in os.walk(output_dir_path): 
                #folder_name_output = root.strip(' "\'\t\r\n').replace('\t'," ").split('\\')
                folder_name_output = root.strip(' "\'\t\r\n').replace('\t'," ").split('/')
                for file in files:
                    if 'Resim_Radars_deb' in folder_name_output[-1] and '_CAN.mf4' not in file and len(folder_name_output[-1])==16:
                        debug_output_list.append(str(os.path.join(root, file)))
                    elif 'Resim_Radars_CAN_Corner_bus' in folder_name_output[-1]:
                        log_list.append(str(os.path.join(root,file)))
                    elif 'Aptiv_FLR_Obj_Perc_for_mPAD' in folder_name_output[-1]:
                        mPad_output_list.append(str(os.path.join(root, file)))
                   
                    else:
                        txt_list.append(str(os.path.join(root, file)))
        
            details.append([debug_output_list,log_list,mPad_output_list])
            
        return is_valid_output,details
   
    
    def get_log_details(self,debug_input_list,set_no_value,details):
        debug_file_details = pd.DataFrame()
        bus_file_present = [0] * len(debug_input_list)
        mpad_file_present = [0] * len(debug_input_list)
        debug_file_present = [0] * len(debug_input_list)
        log_names = []
        set_no = []
        file_size_mb = [0] * len(debug_input_list)
        file_size_mb_bus = [0] * len(debug_input_list)
        file_size_mb_mpad = [0] * len(debug_input_list)
        file_status = [0] * len(debug_input_list)
        
        for i in range(len(debug_input_list)):
            log_path , log_name = os.path.split(debug_input_list[i])
            log_names.append(log_name)
            set_no.append(set_no_value)
            
            if len(details[0]) == 0:
               debug_file_present[i] = False
               file_size_mb[i] = 0 
            
            elif len(details[0]) == 1:
                log_path_debug , log_name_debug = os.path.split(details[0][0])
                if log_name == log_name_debug:
                    debug_file_present[i] = True
                    file_info = os.stat(details[0][0])
                    file_size_bytes = file_info.st_size
                    
                    file_size_kb = file_size_bytes / 1024
                    file_size_mb[i] = file_size_kb / 1024
                    
                else:
                    debug_file_present[i] = False
                    file_size_mb[i] = 0
                  
            else:
                for j in range(len(details[0])):  #debug_files 
                    log_path_debug , log_name_debug = os.path.split(details[0][j])
                    if log_name == log_name_debug:
                        debug_file_present[i] = True
                        file_info = os.stat(details[0][j])
                        file_size_bytes = file_info.st_size
                        
                        file_size_kb = file_size_bytes / 1024
                        file_size_mb[i] = file_size_kb / 1024
                        break
                    else:
                        debug_file_present[i] = False
                        file_size_mb[i] = 0
                        
            
            if len(details[1]) == 0 :
                bus_file_present[i] = 'Not generated'
                file_size_mb_bus[i] = 0
            
            elif len(details[1]) == 1 :
                log_path_bus , log_name_bus = os.path.split(details[1][0])
                splitted_bus_file = log_name_bus.split('_')
                rtag = splitted_bus_file[-2] + '_' + splitted_bus_file[-1]
                log_name_bus = log_name_bus.replace('_'+rtag,'.MF4')
                if log_name == log_name_bus:
                    bus_file_present[i] = True
                    file_info = os.stat(details[1][0])
                    file_size_bytes = file_info.st_size
                    
                    file_size_kb_bus = file_size_bytes / 1024
                    file_size_mb_bus[i] = file_size_kb_bus / 1024
                
                else:
                    bus_file_present[i] = False
                    file_size_mb_bus[i] = 0
                    
            
            else:
                for k in range(len(details[1])): #bus_files
                    log_path_bus , log_name_bus = os.path.split(details[1][k])
                    splitted_bus_file = log_name_bus.split('_')
                    rtag = splitted_bus_file[-2] + '_' + splitted_bus_file[-1]
                    log_name_bus = log_name_bus.replace('_'+rtag,'.MF4')
                    if log_name == log_name_bus:
                        bus_file_present[i] = True
                        file_info = os.stat(details[1][k])
                        file_size_bytes = file_info.st_size
                        
                        file_size_kb_bus = file_size_bytes / 1024
                        file_size_mb_bus[i] = file_size_kb_bus / 1024
                        break
                    else:
                        bus_file_present[i] = False
                        file_size_mb_bus[i] = 0
                       
            
            if len(details[2]) == 0 :
               mpad_file_present[i] = False
               file_size_mb_mpad[i] = 0
               
            elif len(details[2]) == 1 :
               log_path_mpad , log_name_mpad = os.path.split(details[2][0])
               splitted_mpad_file = log_name_mpad.split('_')
               rtag1 = '_'
               for m in range(3,len(splitted_mpad_file)-1):
                   rtag1 += splitted_mpad_file[m] + '_'
               
               log_name_mpad = log_name_mpad.replace(rtag1,'__Resim_Radars_deb_')
               if log_name == log_name_mpad:
                   mpad_file_present[i] = True
                   file_info = os.stat(details[2][0])
                   file_size_bytes = file_info.st_size
                   
                   file_size_kb_mpad = file_size_bytes / 1024
                   file_size_mb_mpad[i] = file_size_kb_mpad / 1024
                   
               else:
                   mpad_file_present[i] = False
                   file_size_mb_mpad[i] = 0
                
            else:
                for p in range(len(details[2])): #mpad_files
                    log_path_mpad , log_name_mpad = os.path.split(details[2][p])
                    splitted_mpad_file = log_name_mpad.split('_')
                    rtag1 = '_'
                    for m in range(3,len(splitted_mpad_file)-1):
                        rtag1 += splitted_mpad_file[m] + '_'
                    
                    log_name_mpad = log_name_mpad.replace(rtag1,'__Resim_Radars_deb_')
                    if log_name == log_name_mpad:
                        mpad_file_present[i] = True
                        file_info = os.stat(details[2][p])
                        file_size_bytes = file_info.st_size
                        
                        file_size_kb_mpad = file_size_bytes / 1024
                        file_size_mb_mpad[i] = file_size_kb_mpad / 1024
                        break
                    else:
                        mpad_file_present[i] = False
                        file_size_mb_mpad[i] = 0
                        
                
            if debug_file_present[i] == True and mpad_file_present[i] == True:
                file_status[i] = 'PASS'
            else:
                file_status[i] = 'BAD-LOG'
        
        debug_file_details['Set_No'] = set_no
        debug_file_details['Log_name'] = log_names
        debug_file_details['Debug_File_Present'] = debug_file_present
        debug_file_details['Bus_File_Present'] = bus_file_present
        debug_file_details['mPAD_File_Present'] = mpad_file_present
        debug_file_details['Debug_File_Size(MB)'] = file_size_mb
        debug_file_details['Bus_File_Size(MB)'] = file_size_mb_bus
        debug_file_details['mPAD_File_Size(MB)'] = file_size_mb_mpad
        debug_file_details['File_Status'] = file_status
        
        
        return debug_file_details
    
    def get_yeild_SCALE3_SCALE4(self, customer_name, input_dir_path, output_dir_path,debug_input_list,details):
        print(f'[yield] : {customer_name}\n\n{input_dir_path}\n\n{output_dir_path}\n\n{debug_input_list}\n\n{details}\n\n')
        merged_data =pd.DataFrame()
        debug_file_details = pd.DataFrame()
        
        debug_yeild =[]
        SRR6P_yeild = []
        FLR4P_yeild = []
        remark = []
        set_no = []
        logs = []
        input_dpath = []
        output_dpath = []
        log_remarks = []
        debug_pass_count = []
        debug_fail_count = []
        mpad_pass_count = []
        mpad_fail_count = []
        
        input_debug_files = len(debug_input_list)
        log_path , log_name = os.path.split(debug_input_list[0])
        set_no_value = ((log_name.split('_'))[1])[9:]
        
        if customer_name == 'stla_scale3':
            if input_debug_files!=0: 
                set_no.append(set_no_value)
                logs.append(input_debug_files)
                input_dpath.append(input_dir_path)
                output_dpath.append(output_dir_path)
                
                output_debug_files = len(details[0][0])
                output_bus_file = len(details[0][1])
                output_mPad_file = len(details[0][2])
                        
                debug_file_details = self.get_log_details(debug_input_list,set_no_value,details[0])
               
                yield_debug = (output_debug_files/input_debug_files)*100
                yield_srr6p = (output_bus_file/input_debug_files)*100
                yield_flr4p = (output_mPad_file/input_debug_files)*100
                debug_yeild.append(yield_debug)
                SRR6P_yeild.append(yield_srr6p)
                FLR4P_yeild.append(yield_flr4p)
                debug_pass_count.append(output_debug_files)
                mpad_pass_count.append(output_mPad_file)
                debug_fail_count.append(input_debug_files-output_debug_files)
                mpad_fail_count.append(input_debug_files-output_mPad_file)
                
                if yield_debug == 100 and yield_flr4p == 100:
                    remark.append('PASS')
                else:
                    remark.append('FAIL')
                
                if output_debug_files == 0 and output_mPad_file == 0:
                    log_remarks.append('No files generated')
                elif yield_debug == yield_flr4p == 100 :
                    log_remarks.append('Full Files generated')
                else:
                    log_remarks.append('Partial Files generated')
                
                merged_data['Set_No'] = set_no
                merged_data['Total_logs'] = logs
                merged_data['Debug_Yield(%)'] = debug_yeild
                merged_data['SRR6P_CAN_Yield(%)'] = SRR6P_yeild
                merged_data['FLR4P_mPad_Yield(%)'] = FLR4P_yeild
                merged_data['RESIM_Overall_Remarks'] = remark
                merged_data['Debug_Pass_Count'] = debug_pass_count
                merged_data['Debug_Fail_Count'] = debug_fail_count
                merged_data['mPAD_Pass_Count'] = mpad_pass_count
                merged_data['mPAD_Fail_Count'] = mpad_fail_count
                merged_data['Log_remarks'] = log_remarks
                merged_data['Input_path'] = input_dpath
                merged_data['Output_path'] = output_dpath
        
        return merged_data,debug_file_details
    
    def check_validity_input_SCALE1(self,input_dir_path):
        log_list = []
        is_valid = False
        
        if os.path.isdir(input_dir_path):
            is_valid = True    
            for root, dirs, files in os.walk(input_dir_path): 
                for file in files:
                    if '_deb' in file and '.MF4' in file:
                        log_list.append(str(os.path.join(root,file)))
     
        return is_valid,log_list
    
    def check_validity_output_SCALE1(self,dir_path):
        debug_list = []
        bus_list = []
        log_list_output = []
        is_valid = False
        
        if os.path.isdir(dir_path):
            is_valid = True    
            for root, dirs, files in os.walk(dir_path): 
                #folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('\\')
                folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('/')
                for file in files:
                    if 'SRR_RAW_DEBUG_SCALE1' in folder_name_input[-1] and '.MF4' in file:
                        debug_list.append(str(os.path.join(root, file)))
                    elif '_CAN.mf4' in file:
                        log_list_output.append(str(os.path.join(root,file)))
                    elif '_r4SRR' in folder_name_input[-1] and ('rM02' not in folder_name_input[-1]) and '.MF4' in file:
                        bus_list.append(str(os.path.join(root,file)))
     
        return is_valid,debug_list,log_list_output,bus_list
    
    def get_yield_SCALE1(self,input_dir_path,output_dir_path,debug_list,log_list,bus_list):
        merged_data_scale1 = pd.DataFrame()
        debug_file_details = []
        
        debug_yield =[]
        bus_yield = []
        set_no = []
        logs = []
        input_dpath = []
        output_dpath = []
        remark = []
        debug_pass = []
        bus_pass = []
        debug_fail = []
        bus_fail = []
        
        
        if(len(log_list)!=0):
            debug_files = len(debug_list)
            log_files = len(log_list)
            bus_files = len(bus_list)
            
            log_path , log_name = os.path.split(log_list[0])
            set_no_value = (log_name.split('_'))[3]
            
            yield_debug = (debug_files/log_files)*100
            yield_bus = (bus_files/log_files)*100
            failed_debug_files = (log_files-debug_files)
            failed_bus_files = (log_files-bus_files)
            debug_yield.append(yield_debug)
            bus_yield.append(yield_bus)
            set_no.append(set_no_value)
            logs.append(log_files)
            input_dpath.append(input_dir_path)
            output_dpath.append(output_dir_path)
            if yield_debug == 100 and yield_bus == 100:
                remark.append('PASS')
            else:
                remark.append('FAIL')
            debug_pass.append(debug_files)
            bus_pass.append(bus_files)
            debug_fail.append(failed_debug_files)   
            bus_fail.append(failed_bus_files)
            
            merged_data_scale1['Set_No'] = set_no
            merged_data_scale1['Total_logs'] = logs
            merged_data_scale1['Debug_Yield(%)'] = debug_yield
            merged_data_scale1['Bus_Yield(%)'] = bus_yield
            merged_data_scale1['RESIM_Overall_Remarks'] = remark
            merged_data_scale1['Debug_Pass_count'] = debug_pass
            merged_data_scale1['Debug_Fail_count'] = debug_fail
            merged_data_scale1['Bus_Pass_Count'] = bus_pass
            merged_data_scale1['Bus_Fail_Count'] = bus_fail
            merged_data_scale1['Input_path'] = input_dpath
            merged_data_scale1['Output_path'] = output_dpath
         
            
            for i in range(len(log_list)):
                   log_path , log_name = os.path.split(log_list[i])
                   for j in range(len(debug_list)):  
                       log_path_debug , log_name_debug = os.path.split(debug_list[j])
                       
                       if log_name == log_name_debug:
                           debug_file_present = True
                           file_info = os.stat(debug_list[j])
                           file_size_bytes = file_info.st_size
                           
                           file_size_kb = file_size_bytes / 1024
                           file_size_mb = file_size_kb / 1024
                           break
                       else:
                           debug_file_present = False 
                           file_size_mb = 0
                   
                   for k in range(len(bus_list)):
                       log_path_bus , log_name_bus = os.path.split(bus_list[k])
                       #rtag = log_path_bus.split('\\')[-1]
                       rtag = log_path_bus.split('/')[-1]
                       log_name_bus = log_name_bus.replace(rtag+'_bus','deb')
                   
                       if log_name == log_name_bus:
                           bus_file_present = True
                           file_info_bus = os.stat(bus_list[k])
                           file_size_bytes_bus = file_info_bus.st_size
                           
                           file_size_kb_bus = file_size_bytes_bus / 1024
                           file_size_mb_bus = file_size_kb_bus / 1024
                           break
                       else:
                           bus_file_present = False
                           file_size_mb_bus = 0
                     
                   
                   if debug_file_present == True and bus_file_present == True:
                       file_status = 'PASS'
                   else:
                       file_status = 'BAD-LOG' 
                   debug_file_details.append([set_no_value,log_name,debug_file_present,bus_file_present,file_size_mb,file_size_mb_bus,file_status])
        
        return merged_data_scale1,debug_file_details
    
    
    def run(self, customer_name,merged_data ,debug_file_details,**kwargs):
        try:   
         
            out = dict() 
            events = np.array(merged_data, dtype=object)
            events1 = np.array(debug_file_details, dtype=object)
            
            if customer_name == "stla_scale3":
                out['Yield_details_SCALE3'] = events
                
                out['Log_Details_SCALE3'] = events1
                
            else:
                out['Yield_details_SCALE1'] = events
                
                out['Log_Details_SCALE1'] = events1
            
            return out
          
        except Exception as error:
               exc_type, exc_obj, exc_tb = sys.exc_info()
               error_msg = "ERROR " + \
                   str(error) + " FOUND IN LINE: " + str(exc_tb.tb_lineno)

if __name__ == '__main__':
    q = 'n'
    if q == 'n':
        kwargs = dict()
        Test = ExtractResimYield()
        dfb = pd.DataFrame()
        dfc = pd.DataFrame()
        
        script_path = sys.argv[0]   #path of script
        customer_name = sys.argv[1]
        print('[yeild] : ',sys.argv[0],' : ',sys.argv[1] )
        #customer_name = "SCALE3"
        #dir_path= r"C:\Users\qj743z\Documents\Simulation_Team\DC\SCALE_3_Logs\input.txt"
        dir_path = sys.argv[2]
        
        if(len(sys.argv)==4):  #If output path is provided
            output_path1 = sys.argv[3]
        else:                           #take default path of script path
            new_path = script_path[:-22]     #remove script name 
            new_dir = "Yeild_results"  
            output_path1 = os.path.join(new_path,new_dir)
       
        if not(os.path.isdir(output_path1)):
            os.mkdir(output_path1)
        else: 
            pass
        
        blank_count = 0
        print("\n*************************** START OF SCRIPT *************************************")
        if (dir_path.endswith('.txt')):
            print("Reading paths in filelist (.txt) :-->")
            with open(dir_path) as file:
                filelist = file.read().splitlines() 
            print("Done!!!!")
            
            for j in range(len(filelist)):
                if not bool(filelist[j]):
                    blank_count+=1
            
            print("\nPROCESSING DETAILS :--> \n")
            print("   ** Extraction will start for " + str(len(filelist)-blank_count) + " sets")
            print("\n   ** Processing started!!!!!!")
            
            print("\n     ** Checking for valid directory path")
            set_count = 0
        
            if customer_name == "stla_scale3": 
                for i in range(len(filelist)):
                        x = str(filelist[i]).strip(' "\'\t\r\n').replace('\t'," ").split(' ')
                        if len(x) == 2:
                            input_dir_path = x[0]
                            output_dir_path = x[1]
                            path_check_input,debug_input_list,corner_CAN_list,side_CAN_list,mPad_input_list = Test.check_validity_input_SCALE3(customer_name,input_dir_path)
                            path_check_output,details = Test.check_validity_output_SCALE3(output_dir_path)
                          
                            #parent_folder = (input_dir_path.split('\\'))[-2]
                       
                            parent_folder = (input_dir_path.split('/'))[-2]
                            
                            if(path_check_input == True and path_check_output == True): 
                                set_count+=1
                                print("\n       ## Proper Directory path found for Input & Output for set " + str(set_count))
                               
                                merged_data,debug_file_details = Test.get_yeild_SCALE3_SCALE4(customer_name, input_dir_path, output_dir_path, sorted(debug_input_list),details)
                                
                                cell = Test.run(customer_name,merged_data,debug_file_details,**kwargs)
                                
                                if len(cell['Yield_details_SCALE3'])!=0 or len(cell['Log_Details_SCALE3'])!=0:
                                    df4 = pd.DataFrame(cell['Yield_details_SCALE3'])
                                    dfb = pd.concat([dfb, df4], axis=0)  
                                    
                                    df5 = pd.DataFrame(cell['Log_Details_SCALE3'])
                                    dfc = pd.concat([dfc, df5], axis=0)  
                                    
                            else:
                                print("\n       ## Directory path is not proper!!")
                        
                        else:
                            print("Filelist structure is not proper!!!")
                
                print("\n   ** Processing done!!!!!!")
                #output_path = output_path1 + '\\' + 'Yield_report1_' + parent_folder + '.xlsx'       #for local running
                output_path = output_path1 + '/' + 'Yield_report_' + parent_folder + '.xlsx'        #for server running
                
                print("\nEXTRACTION DETAILS :-->\n")
                with pd.ExcelWriter(output_path) as writer:
                    try:
                        dfb.columns = Test._headers['Yield_details_SCALE3']
                        dfb.to_excel(writer, sheet_name='Yield_details_SCALE3', index=False)
                        print('Yield details extracted in sheet')
                    except:
                        print('Yield details not extracted in sheet') 
                        
                    try:
                        dfc.columns = Test._headers['Log_Details_SCALE3']
                        dfc.to_excel(writer, sheet_name='Log_Details_SCALE3', index=False)
                        print('Log details extracted in sheet')
                    except:
                        print('Log details not extracted in sheet') 
                        
            elif customer_name == "stla_scale1": 
            
                for i in range(len(filelist)):
                        #split_file_path = filelist[i].split("\\")  # for local running
                        split_file_path = filelist[i].split("/")  #for server running
                        #set_no_value = split_file_path[-1]
                        parent_folder = split_file_path[-2]
                        x = str(filelist[i]).strip(' "\'\t\r\n').replace('\t'," ").split(' ')
                        
                        if len(x) == 2:
                            input_dir_path = x[0]
                            output_dir_path = x[1]
                            path_check_input,log_list = Test.check_validity_input_SCALE1(input_dir_path)
                            path_check_output,debug_input_list,log_list_output,bus_list = Test.check_validity_output_SCALE1(output_dir_path)
                            
                            if(path_check_input == True and path_check_output == True): 
                                set_count+=1
                                print("\n       ## Proper Directory path found for Input & Output for set " + str(set_count))
                               
                                merged_data_scale1,debug_file_details = Test.get_yield_SCALE1(input_dir_path,output_dir_path,sorted(debug_input_list),sorted(log_list),sorted(bus_list))
                                cell = Test.run(customer_name,merged_data_scale1,debug_file_details,**kwargs)
                                
                                if len(cell['Yield_details_SCALE1'])!=0 or len(cell['Log_Details_SCALE1'])!=0:
                                    df4 = pd.DataFrame(cell['Yield_details_SCALE1'])
                                    dfb = pd.concat([dfb, df4], axis=0)  
                                    
                                    df5 = pd.DataFrame(cell['Log_Details_SCALE1'])
                                    dfc = pd.concat([dfc, df5], axis=0)  
                                    
                            else:
                                print("\n       ## Directory path is not proper!!")
                        
                        else:
                            print("Filelist structure is not proper!!!")
            
            
                print("\n   ** Processing done!!!!!!")
                #output_path = output_path1 + '\\' + 'Yield_report2_' + parent_folder + '.xlsx'       #for local running
                output_path = output_path1 + '/' + 'Yield_report_' + parent_folder + '.xlsx'        #for server running
                
                print("\nEXTRACTION DETAILS :-->\n")
                with pd.ExcelWriter(output_path) as writer:
                    try:
                        dfb.columns = Test._headers['Yield_details_SCALE1']
                        dfb.to_excel(writer, sheet_name='Yield_details_SCALE1', index=False)
                        print('Yield details extracted in sheet')
                    except:
                        print('Yield details not extracted in sheet') 
                        
                    try:
                        dfc.columns = Test._headers['Log_Details_SCALE1']
                        dfc.to_excel(writer, sheet_name='Log_Details_SCALE1', index=False)
                        print('Log details extracted in sheet')
                    except:
                        print('Log details not extracted in sheet') 
            
            
        
        wb = load_workbook(output_path)
        red_fill = PatternFill(bgColor="FF4433")  
        yellow_fill = PatternFill(bgColor="FFBF00") 
        green_fill = PatternFill(bgColor="90EE90")  
        dark_green_fill = PatternFill(bgColor="00FF7F")   
        white_fill = PatternFill(bgColor="FFFFFF")
        
        dxf = DifferentialStyle(fill=red_fill)
        dxf1 = DifferentialStyle(fill=yellow_fill)
        dxf2 = DifferentialStyle(fill=green_fill)
        dxf3 = DifferentialStyle(fill=dark_green_fill)
        dxf4 = DifferentialStyle(fill=white_fill)
        
        if customer_name == "stla_scale3":
            sheet_to_color = wb["Yield_details_SCALE3"]      
            sheet_to_color1 = wb["Log_Details_SCALE3"]  
            # Formulae for DEBUG_YIELD
            r = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r1 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r2 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r3 = Rule(type="expression", dxf=dxf3, stopIfTrue=True)
            
            r.formula = ['$D2>0' and '$D2<=20']
            r1.formula = ['$D2>20' and '$D2<90']
            r2.formula = ['$D2>=90' and '$D2<99']
            r3.formula = ['$D2=100']
            
            range_to_color = "D2:D" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color, r)
            sheet_to_color.conditional_formatting.add(range_to_color, r1)
            sheet_to_color.conditional_formatting.add(range_to_color, r2)
            sheet_to_color.conditional_formatting.add(range_to_color, r3)
            
            # Formulae for SRR6P_YIELD
            r4 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r5 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r6 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r7 = Rule(type="expression", dxf=dxf3, stopIfTrue=True)
            
            r4.formula = ['$E2>0' and '$E2<=20']
            r5.formula = ['$E2>20' and '$E2<90']
            r6.formula = ['$E2>=90' and '$E2<99']
            r7.formula = ['$E2=100']
            range_to_color1 = "E2:E" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color1, r4)
            sheet_to_color.conditional_formatting.add(range_to_color1, r5)
            sheet_to_color.conditional_formatting.add(range_to_color1, r6)
            sheet_to_color.conditional_formatting.add(range_to_color1, r7)
            
            # Formulae for FLR4P_YIELD
            r8 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r9 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r10 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r11 = Rule(type="expression", dxf=dxf3, stopIfTrue=True)
            r8.formula = ['$F2>0' and '$F2<=20']
            r9.formula = ['$F2>20' and '$F2<90']
            r10.formula = ['$F2>=90' and '$F2<99']
            r11.formula = ['$F2=100']
            range_to_color2 = "F2:F" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color2, r8)
            sheet_to_color.conditional_formatting.add(range_to_color2, r9)
            sheet_to_color.conditional_formatting.add(range_to_color2, r10)
            sheet_to_color.conditional_formatting.add(range_to_color2, r11)
            
            #File_status
            r12 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r13 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r14 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r16 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            
            r12.formula = ['$F2=0']  #debug size = 0
            r14.formula = ['$H2=0']  #mpad size = 0
            r13.formula = ['$I2="PASS"']
            r16.formula = ['$I2="BAD-LOG"']
            range_to_color2 = "I2:I" + str(len(dfc) + 1)
            range_to_color3 = "C2:C" + str(len(dfc) + 1) #debug file present/not
            range_to_color4 = "E2:E" + str(len(dfc) + 1) #mpad file present/not
           
            sheet_to_color1.conditional_formatting.add(range_to_color2, r16)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r13)
            
            sheet_to_color1.conditional_formatting.add(range_to_color3, r12)
            sheet_to_color1.conditional_formatting.add(range_to_color4, r14)
            
        elif customer_name == "stla_scale1":
            sheet_to_color = wb["Yield_details_SCALE1"]      
            sheet_to_color1 = wb["Log_Details_SCALE1"]  
            
            # Formulae for DEBUG_YIELD
            r = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r1 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r2 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r3 = Rule(type="expression", dxf=dxf3, stopIfTrue=True)
            
            r.formula = ['$C2>0' and '$C2<=20']
            r1.formula = ['$C2>20' and '$C2<90']
            r2.formula = ['$C2>=90' and '$C2<100']
            r3.formula = ['$C2=100']
            
            range_to_color = "C2:C" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color, r)
            sheet_to_color.conditional_formatting.add(range_to_color, r1)
            sheet_to_color.conditional_formatting.add(range_to_color, r2)
            sheet_to_color.conditional_formatting.add(range_to_color, r3)
            
            # Formulae for BUS_YIELD
            r4 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r5 = Rule(type="expression", dxf=dxf1, stopIfTrue=True)
            r6 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r7 = Rule(type="expression", dxf=dxf3, stopIfTrue=True)
            
            r4.formula = ['$D2>0' and '$D2<=20']
            r5.formula = ['$D2>20' and '$D2<90']
            r6.formula = ['$D2>=90' and '$D2<100']
            r7.formula = ['$D2=100']
            range_to_color1 = "D2:D" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color1, r4)
            sheet_to_color.conditional_formatting.add(range_to_color1, r5)
            sheet_to_color.conditional_formatting.add(range_to_color1, r6)
            sheet_to_color.conditional_formatting.add(range_to_color1, r7)
            
            #RESIM_Overall_Remarks
            r13 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r14 = Rule(type="expression", dxf=dxf3, stopIfTrue=True)
            r13.formula = ['$E2="FAIL"']
            r14.formula = ['$E2="PASS"']
            range_to_color1 = "E2:E" + str(len(dfb) + 1)
            sheet_to_color.conditional_formatting.add(range_to_color1, r13)
            sheet_to_color.conditional_formatting.add(range_to_color1, r14)
            
            # Formulae for FILE_STATUS
            r8 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r9 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r10 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r8.formula = ['$F2=0']
            r9.formula = ['$E2>0' and '$F2>0']
            r10.formula = ['$E2=0']
            range_to_color2 = "G2:G" + str(len(dfc) + 1)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r8)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r9)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r10)
            
            # Formulae for FILE_PRESENT_BUS
            r11 = Rule(type="expression", dxf=dxf, stopIfTrue=True)
            r12 = Rule(type="expression", dxf=dxf2, stopIfTrue=True)
            r8.formula = ['$F2=0']
            r9.formula = ['$E2>0' and '$F2>0']
            r10.formula = ['$E2=0']
            range_to_color3 = "D2:D" + str(len(dfc) + 1)
            range_to_color4 = "C2:C" + str(len(dfc) + 1)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r8)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r9)
            sheet_to_color1.conditional_formatting.add(range_to_color2, r10)
            
            sheet_to_color1.conditional_formatting.add(range_to_color3, r8)
            sheet_to_color1.conditional_formatting.add(range_to_color4, r10)
            
            
        wb.save(output_path)
        
        
        Test.kpi_sheet_generation(customer_name, output_path)
        
        
        print("\nEXTRACTION COMPLETED !!!")
        print('\nDetailed summary (errors & results) is available at generated report path')
        
        print('Output Excel is Saved at : ', output_path1)
        
        print("\n****************************** END OF SCRIPT *************************************\n")        
       
    
    
"""
######################################################################################################
DATE(DD/MM/YY)            NAME                         JIRA NUMBER DESCRIPTION
10/06/2024             Sonal Rajurkar           Script to provide yeild for RESIM output logs
11/06/2024             Sonal Rajurkar           Added logic to :
                                                -- extract different yield 
                                                -- colour coding for yield percentages
                                                -- made compatible for directory as well as filelist
12/06/2024             Sonal Rajurkar           Made it compatible to extract SCALE1 logs
17/06/2024             Sonal Rajurkar           Added log wise details for SCALE 1 
19/06/2024             Sonal Rajurkar           Added log wise details for SCALE 3
20/06/2024             Sonal Rajurkar           Removed mode column from log wise details
                                                -- added bus file not generated tag and made it compatible for 1 mode
                                                -- Added KPI for SCALE 3 as well
######################################################################################################
"""    
    
    