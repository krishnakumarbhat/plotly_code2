# -*- coding: utf-8 -*-
"""
Created on Mon Aug 26 12:11:06 2024

@author: qj743z
"""

from glob import glob
import json 
import os
import sys


def html_collect_files(files, requestFile):
    file_found = []
    for file in files:
        if requestFile in file:
            file_found.append(file)
    file_found.sort()
    return file_found

def html_write_json(file, data, value):
    no_of_files = len(data)
    file_written_count = 1
    file.write(f'\t"{value}" :\n\t[\n')
    for log in data:
        log = log.replace('\\', '/')
        file.write(f'\t\t"{log}"')
        if file_written_count < no_of_files:
            file.write(",\n")
        else:
            file.write("\n")
        file_written_count += 1
    file.write('\t]')
        
    if value != 'OUTPUT_MF4':
        file.write(',')
    file.write('\n\n')
    
def html_write_bordnet(file, data, value):
    no_of_files = len(data)
    file_written_count = 1
    file.write('\t\t{\n')
    file.write(f'\t\t\t"key": "{value}",\n\t\t\t"files": [\n')
   
    for log in data:
        log = log.replace('\\', '/')
        file.write(f'\t\t\t\t"{log}"')
        if file_written_count < no_of_files:
            file.write(",\n")
        else:
            file.write("\n")
        file_written_count += 1
    file.write('\t\t\t]\n')
    file.write('\t\t}')
        
    if value != 'OUTPUT_SRR_DEBUG':
        file.write(',')
    file.write('\n')

def read_input_files(input_json,types,sing_path):
    
    if ('stla_scale1' in sing_path) or ('stla_scale3' in sing_path) or ('stla_scale4' in sing_path):
        with open(input_json) as file:
            data = json.load(file)
            
        if 'stla_scale1' in sing_path:
            for i in range(len(data['reprocessingInputFileStreams'])):
                if data['reprocessingInputFileStreams'][i]['key'] == 'SRR_DEBUG':
                    deb_key_index = i
                if data['reprocessingInputFileStreams'][i]['key'] == 'BN_FASETH':
                    bus_key_index = i
          
        elif ('stla_scale3' in sing_path) or ('stla_scale4' in sing_path):
            for i in range(len(data['reprocessingInputFileStreams'])):
                if types == 'HTML':
                    if data['reprocessingInputFileStreams'][i]['key'] == 'Resim_Radars_deb':
                        deb_key_index = i
                elif types == 'BORDNET':
                    if data['reprocessingInputFileStreams'][i]['key'] == 'Resim_Radars_CAN_Corner_bus':
                        deb_key_index = i
                if data['reprocessingInputFileStreams'][i]['key'] == 'Aptiv_FLR_Obj_Perc_for_mPAD':
                    mpad_key_index = i
                    
        
        if types == 'HTML':
            input_files = data['reprocessingInputFileStreams'][deb_key_index]['files']
            mpad_files = []
        
        elif types == 'BORDNET':   
            if 'stla_scale1' in sing_path:
                input_files = data['reprocessingInputFileStreams'][bus_key_index]['files']
                mpad_files = []
            else:
                input_files = data['reprocessingInputFileStreams'][deb_key_index]['files']
                mpad_files = data['reprocessingInputFileStreams'][mpad_key_index]['files']
    
    
    else:
        with open(input_json) as file:
            data = file.read().splitlines()
        input_files = data
        mpad_files = []
            
            
    return input_files,mpad_files
    
def create_json_html(input_json,out_path,sing_path):
    html_validity = True
    ip_data,mpad_data = read_input_files(input_json,'HTML',sing_path)
    
    deb_op_data = []

    if os.path.isdir(out_path):
        for root, dirs, files in os.walk(out_path): 
            #folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('\\')  # for local testing
            folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('/')
            for file in files:
                if 'stla_scale1' in sing_path:
                    if 'SRR_RAW_DEBUG_SCALE1' in folder_name_input[-1] and '.MF4' in file:
                        deb_op_data.append(str(os.path.join(root,file)))
                elif ('stla_scale3' in sing_path) or ('stla_scale4' in sing_path):
                    if 'Resim_Radars_deb' in folder_name_input[-1] and '.MF4' in file:
                        deb_op_data.append(str(os.path.join(root,file)))
                elif 'honda' in sing_path:
                    if 'HONDA' in file:
                        deb_op_data.append(str(os.path.join(root,file)))
                elif 'rnasdv' in sing_path:
                    if '_Aptiv.MF4' in file:
                        deb_op_data.append(str(os.path.join(root,file)))
                elif 'traton' in sing_path:
                    if 'TRATON' in file:
                        deb_op_data.append(str(os.path.join(root,file)))
    
    if ('stla_scale1' in sing_path) or ('stla_scale3' in sing_path) or ('stla_scale4' in sing_path):
        ip_deb_files = html_collect_files(ip_data, 'deb')
        op_deb_files = html_collect_files(deb_op_data, 'deb')
    elif 'rnasdv' in sing_path:
        ip_deb_files = html_collect_files(ip_data, '_Aptiv.MF4')
        op_deb_files = html_collect_files(deb_op_data, '_Aptiv.MF4')
    elif 'honda' in sing_path:
        ip_deb_files = html_collect_files(ip_data, 'HONDA')
        op_deb_files = html_collect_files(deb_op_data, 'HONDA')
    elif 'traton' in sing_path:
        ip_deb_files = html_collect_files(ip_data, 'TRATON')
        op_deb_files = html_collect_files(deb_op_data, 'TRATON')
        
    output_files = []
    input_files = []
    
    for i in range(len(ip_deb_files)):
        log_path_ip, log_name_ip = os.path.split(ip_deb_files[i])
        for j in range(len(op_deb_files)):
            log_path_op, log_name_op = os.path.split(op_deb_files[j])
            if log_name_op in log_name_ip:
                output_files.append(op_deb_files[j])
                input_files.append(ip_deb_files[i])
                break
            
    new_path = out_path   
    new_dir = "HTML_REPORT"  
    output_path1 = os.path.join(new_path,new_dir)

    if not(os.path.isdir(output_path1)):
        os.mkdir(output_path1)
    else: 
        pass
    
    output_dir = output_path1

    if len(output_files)!=0 and len(input_files)!=0:
        #json_path = f'{output_dir}\\HTMLInputs.json'   # for local testing
        json_path = f'{output_dir}/HTMLInputs.json'
        json_file = open(json_path, 'w')
        json_file.write('{\n')
        html_write_json(json_file,input_files,'INPUT_MF4')
        html_write_json(json_file,output_files,'OUTPUT_MF4')
        json_file.write('}')
        json_file.close()
        
    else:
        html_validity = False
        error_path = f'{output_dir}/error_list.txt' 
        error_file = open(error_path,'w')
        error_file.write("Error while creating the HTML json , Output file not present")
        error_file.close()
        print("Error while creating the HTML json , Output file not present\n")
        
    return  html_validity 
    
def create_yield_filelist(input_json,out_path,sing_path):
    input_files,mpad_files = read_input_files(input_json,'HTML',sing_path)
    logpath = []
    for i in range(len(input_files)): 
        log_path,log_name = os.path.split(input_files[i])
        logpath.append(log_path)
    
    unique_log_paths = set(logpath)
        
    out_path = out_path.replace('\\', '/')
    
    new_path1 = out_path   
    new_dir1 = "YIELD_REPORT"  
    output_path2 = os.path.join(new_path1,new_dir1)

    if not(os.path.isdir(output_path2)):
        os.mkdir(output_path2)
    else: 
        pass
    
    output_dir = output_path2
    filelist_path = f'{output_dir}/yield_filelist.txt'
    yield_input_file = open(filelist_path, 'w')
    
    if(len(unique_log_paths))==1:
        unique_log_path = logpath[0]
        yield_input_file.write(f'{unique_log_path} {out_path}')
    else:
        for j in range(len(unique_log_paths)):
           unique_log_path =  list(unique_log_paths)[j]
           yield_input_file.write(f'{unique_log_path} {out_path}')
           yield_input_file.write("\n")
    
    yield_input_file.close()

def create_json_bordnet(input_json,out_path,sing_path):
    bordnet_validity = True
    ip_data,mpad_data = read_input_files(input_json,'BORDNET',sing_path)
    
    bus_op_data = []
    deb_op_data = []
    mpad_op_data = []
    
    if os.path.isdir(out_path):
        for root, dirs, files in os.walk(out_path): 
            #folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('\\')  # for local testing
            folder_name_input = root.strip(' "\'\t\r\n').replace('\t'," ").split('/')
            for file in files:
                if 'stla_scale1' in sing_path:
                    ip_deb_files = html_collect_files(ip_data, 'bus')
                    ip_mpad_files = html_collect_files(ip_data, 'bus')
                    if '_r4SRR' in folder_name_input[-1] and ('rM02' not in folder_name_input[-1]) and '.MF4' in file:
                        bus_op_data.append(str(os.path.join(root,file)))
                elif ('stla_scale3' in sing_path) or ('stla_scale4' in sing_path):
                    ip_deb_files = html_collect_files(ip_data, 'bus')
                    ip_mpad_files = html_collect_files(mpad_data, 'mPAD') 
                    if 'Resim_Radars_CAN_Corner_bus' in folder_name_input[-1] and '.MF4' in file:
                        bus_op_data.append(str(os.path.join(root,file)))
                    elif 'Aptiv_FLR_Obj_Perc_for_mPAD' in folder_name_input[-1] and '.MF4' in file:
                        mpad_op_data.append(str(os.path.join(root,file)))
                    
                        
    if 'stla_scale1' in sing_path:
        op_fasth_files = html_collect_files(bus_op_data, 'bus')
        op_debug_files = html_collect_files(bus_op_data, 'bus')
        
    elif ('stla_scale3' in sing_path) or ('stla_scale4' in sing_path):
        op_fasth_files = html_collect_files(mpad_op_data, 'mPAD')
        op_debug_files = html_collect_files(bus_op_data, 'bus')
    
    else:
        ip_deb_files = []
        ip_mpad_files = []
        op_fasth_files = []
        op_debug_files = []
        
    new_path2 = out_path   
    new_dir2 = "BORDNET_REPORT"  
    output_path3 = os.path.join(new_path2,new_dir2)

    if not(os.path.isdir(output_path3)):
        os.mkdir(output_path3)
    else: 
        pass
    
    output_dir = output_path3

    if len(ip_mpad_files)!=0 and len(ip_deb_files)!=0 and len(op_fasth_files)!=0 and len(op_debug_files)!=0:
        #json_path = f'{output_dir}\\BORDNET_fList.json'   # for local testing
        json_path = f'{output_dir}/BORDNET_fList.json'
        json_file = open(json_path, 'w')
        json_file.write('{\n')
        json_file.write(f'\t"reprocessingInputFileStreams": [\n')
        html_write_bordnet(json_file,'','INPUT_BN_CALIFR')
        html_write_bordnet(json_file,ip_mpad_files,'INPUT_BN_FASETH')
        html_write_bordnet(json_file,ip_deb_files,'INPUT_SRR_DEBUG')
        html_write_bordnet(json_file,'','OUTPUT_BN_CALIFR')
        html_write_bordnet(json_file,op_fasth_files,'OUTPUT_BN_FASETH')
        html_write_bordnet(json_file,op_debug_files,'OUTPUT_SRR_DEBUG')
        json_file.write('\t]\n')
        json_file.write('}')
        json_file.close()
    else:
        bordnet_validity = False
        error_path = f'{output_dir}/error_list.txt' 
        error_file = open(error_path,'w')
        error_file.write("Error while creating the Bordnet json , Output file not present")
        error_file.close()
        print("Error while creating the BORDNET json , Output file not present\n")
        
    return bordnet_validity
    
if __name__ == '__main__':
    '''To run locallly'''
    #input_json = r"C:\Users\qj743z\Documents\Simulation_Team\DC\FHW-306\SIL_Input_s1.json"
    #out_path = r"C:\Users\qj743z\Documents\Simulation_Team\DC\FHW-306\HADD"
    #sing_path = r"C:\Users\qj743z\Documents\Simulation_Team\DC\FHW-306\resim_stla_scale1_20240830104009.simg"
    
    #input_json = r"C:\Users\qj743z\Documents\Simulation_Team\DC\SCALE_3_Logs\SIL_Input.json"
    #out_path = r"C:\Users\qj743z\Documents\Simulation_Team\DC\SCALE_3_Logs\CDC\151905"
    #sing_path = r"resim_stla_scale3_20240830104009.simg"
    
    #input_json = r"C:\Users\qj743z\Documents\Simulation_Team\DC\GEN7_logs\Scripts\RNA-GEN7_Json\SIL_Input.txt"
    #out_path = r"C:\Users\qj743z\Documents\Simulation_Team\DC\GEN7_logs\GEN7"
    #sing_path = r"resim_rnasdv_20240830104009.simg"
    
    '''To run on server'''
    input_json = sys.argv[1]
    out_path = sys.argv[2]
    sing_path = sys.argv[3]
    
    sing_path = (sing_path.split('/'))[-1]
    
  
    html_validity = create_json_html(input_json,out_path,sing_path)
    create_yield_filelist(input_json,out_path,sing_path)
    bordnet_validity = create_json_bordnet(input_json,out_path,sing_path)
     
    if html_validity == False and bordnet_validity == False:
        sys.exit(9)
    
    
"""
######################################################################################################
DATE(DD/MM/YY)            NAME                         JIRA NUMBER DESCRIPTION
26/08/2024             Sonal Rajurkar           Initial script to create html json which takes input json & output path asarguments 
27/08/2024             Sonal Rajurkar           Added logic to create filelist that will be input for yield script
28/08/2024             Sonal Rajurkar           Created logic to create BORDNET json 
10/09/2024             Sonal Rajurkar           Added the logic to extract matching no of deb files in HTML Json  
17/09/2024             Sonal Rajurkar           Added logic for SCALE3 & SCALE4 
01/10/2024             Sonal Rajurkar           Added logic for GEN7, HONDA, TRATON and changes in HTML json creator format

######################################################################################################
"""
