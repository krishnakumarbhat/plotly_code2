from glob import glob
import os
input_file_list = []

#**********************************************************
#          function to collect debug files
#**********************************************************
def get_deb_files(path):
    deb_file = []
    mcip_deb_file =[]
    '''log_data = glob(f"{path}/*.MF4")
    if len(log_data) == 0:
        log_data = glob(f"{path}/*.mf4")'''
    
    for root, dir, files in os.walk(path):
        for file in files:
            if '.mf4' in file or '.MF4' in file:
                deb_file.append(f"{root}/{file}")

    for file in deb_file:
        if "_b05" in file or "SDV_RADAR_" in file : mcip_deb_file.append(file)
    if len(mcip_deb_file) != 0: return mcip_deb_file;
    return deb_file


#**********************************************************
#          function to collect brr files
#**********************************************************
def get_brr_files(path, job_path=None):
    brr_file = []
    for root, dir, files in os.walk(path):
        for file in files:
            if '.brr' in file or '.BRR' in file:
                brr_file.append(f"{root}/{file}")

    if brr_file and job_path:
        from dq_brrpicker import create_input_json
        output_json_path = f"{job_path}/jobout/input.json"
        create_input_json(brr_file, output_json_path)

    return brr_file


#**********************************************************
#          function to create .txt file to run Quality Tool
#**********************************************************
def create_fList(job_path, file_list, cnt, total):
    print(f'[INFO] : creating .txt/.json for {cnt}/{total}',end='\r')
    path = f'{job_path}/jobout/DQ_fList_{cnt}.txt'
    file = open(path,'w')
    file_list.sort()
    for split_file in file_list:
        file.write(split_file)
        file.write('\n')
    file.close()
    input_file_list.append(path)
    return input_file_list


#**********************************************************
#          function to collect list of all session .txt file path
#          on which Quality tool needs to be run
#**********************************************************
def create_input_fList(job_path):
    print(f'[INFO] : Creating InputList for Quality Checker',end='\r')
    path =  f'{job_path}/jobout/DQ_fList_all.txt'
    file = open(path,'w')
    for split_file in input_file_list:
        file.write(split_file)
        file.write('\n')
    file.close()
    print(f'[INFO] : Created  InputList for Quality Checker')
    return path


"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
22/08/2024          Mandeep Singh       FHW-268     splitter for STLA_SCALE1 DQ(created 1st version of file )

######################################################################################################
"""
