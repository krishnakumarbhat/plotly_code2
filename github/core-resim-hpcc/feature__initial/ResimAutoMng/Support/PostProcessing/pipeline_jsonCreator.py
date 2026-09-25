# -*- coding: utf-8 -*-
"""
Created on Tuesday Nov-26 12:29:06 2024

@author: mandeep.singh1@aptiv.com
"""

import sys, os
import json, glob


###################################################
#
#    function to capture error
#
###################################################
def updateFileError (pipeLineFile, message):
    pipeLineFile.write(message)
    return True, 0


###################################################
#
#    function to validate output files
#
###################################################
def validate_outputFiles (output_path, simg, upuFlag):
    error = False
    f_orcas = f_rFLR = f_crnBus = f_mpad = f_sideBus = f_can = 0
    flags_pipeline = [1,1]
    pipeLineError = f'{output_path}/pipeLineError.txt'
    pipeLineFile = open(pipeLineError, 'w')
    pipeLineFile.write(f"Customer : {simg}\n")
    folderOp = os.listdir(output_path)

    print(f"{output_path} :{upuFlag}")

    if upuFlag: pass                
    else:    
        if "resim_stla_scale3" in simg:
            for folder in folderOp:
                if folder == "ORCAS":
                    f_orcas = 1
                elif folder == "Resim_Radars_CAN_Corner_bus":
                    f_crnBus = 1
                elif folder == "Aptiv_FLR_Obj_Perc_for_mPAD":
                    f_mpad = 1
            if f_orcas == 0:
                error, flags_pipeline[0] = updateFileError(pipeLineFile,"[Error] : ORCAS folder not present\n")
            if f_crnBus == 0:
                error, flags_pipeline[1] = updateFileError(pipeLineFile,"[Error] : Resim_Radars_CAN_Corner_bus folder not present\n")
            if f_mpad == 0:
                error, flags_pipeline[1] = updateFileError(pipeLineFile,"[Error] : Aptiv_FLR_Obj_Perc_for_mPAD folder not present\n")
    
            if f_crnBus ==0 and f_mpad == 0:
                flags_pipeline[1] = 0
            else:
                flags_pipeline[1] = 1
    
        elif "resim_stla_scale1" in simg:
            for folder in folderOp:
                if folder == "ORCAS":
                    f_orcas = 1
                elif "rFLR" in folder:
                    f_crnBus = 1
            if f_orcas == 0:
                error, flags_pipeline[0] = updateFileError(pipeLineFile,"[Error] : ORCAS folder not present\n")
            if f_crnBus == 0:
                error, flags_pipeline[1] = updateFileError(pipeLineFile,"[Error] : rFLR folder not present\n")
        elif ("dc" in simg or "dgps" in simg.lower()) and "platform" not in simg:
            flags_pipeline[1] = 0
            for folder  in folderOp:
                if 'DGPS_OUTPUT' in folder:
                    f_orcas = 1
            if f_orcas == 0:
                error, flags_pipeline[0] = updateFileError(pipeLineFile,"[Error] : DGPS folder not present\n")
            print('inside-dc :',flags_pipeline)
        else:
            for folder in folderOp:
                if "ORCAS" == folder:
                    f_orcas = 1
                elif "CANoe" == folder:
                    f_can = 1
            if f_orcas == 0:
                error, flags_pipeline[0] = updateFileError(pipeLineFile,"[Error] : ORCAS folder not present\n")
            if f_can == 0:
                error, flags_pipeline[1] = updateFileError(pipeLineFile,"[Error] : CANoe folder not present\n")
                
    if error == False:
        os.remove(pipeLineError)
    pipeLineFile.close()
    return flags_pipeline


###################################################
#
#    function to collect input files
#
###################################################
def get_files(files, action):
    
    valid_files = []
    logname = files[0].split('/')[-1]
    logpath = files[0].split(f'/{logname}')[0]
    for file in files:
        valid_file = file.split('/')[-1]
        valid_files.append(valid_file)

    '''if action == 0:
        return logpath, valid_files
    elif action == 1:
        return files'''
    return logpath, valid_files


###################################################
#
#    function to read and collect input files from json
#
###################################################
def collect_input_files(json_file,toolAsked):
    file = open(json_file, 'r')
    content = json.load(file)
    file.close()

    if toolAsked == 'html':
        return get_files(content['reprocessingInputFileStreams'][2]['files'], 0)
    elif toolAsked == 'bordnet':
        files=[None, None, None, None]
        files[0] = content['reprocessingInputFileStreams'][0]['files']
        files[1] = content['reprocessingInputFileStreams'][1]['files']
        files[2]= content['reprocessingInputFileStreams'][2]['files']
        if 'stla_scale3' in simg or 'stla_scale4' in simg: 
            files[3] = content['reprocessingInputFileStreams'][3]['files']
        return files
        


###################################################
#
#    function to read and collect output files from json
#
###################################################
def collect_output_files(out_path,toolAsked, simg, upuFlag):
    def pickFiles(pathToCheck):
        allFiles = glob.glob(f'{pathToCheck}/*.MF4')
        if len(allFiles) == 0:
            allFiles = glob.glob(f'{pathToCheck}/*.mf4')
        if len(allFiles) == 0:
            allFiles = glob.glob(f'{pathToCheck}/*.blf')
        allFiles.sort()
        return allFiles
        
    is_dgps = 'dgps' in simg.lower() or ('dc' in simg and 'platform' not in simg)
    if upuFlag and not is_dgps:
        if toolAsked == 'html':
            files_op=[]
            path=""
            for root, dir, files in os.walk(out_path):
                if "rR" in root: rtag=root.split('/')[-2]
                for file in files:    
                    if "rR" in root and ".MF4" in file and '_b05' in file :
                        path=root.split(rtag)[0]+rtag
                        file=root.split(f"{rtag}/")[-1]+"/"+file
                        files_op.append(file)
            files_op.sort()
            return path, files_op

        elif toolAsked == 'bordnet':
            files_op=[[], [], [], []]
            for root, dir, files in os.walk(out_path):
                for file in files:
                    if "rR" in root and ".MF4" in file and '_b04' in file :
                        files_op[2].append(f"{root}/{file}")
            files_op[2].sort()
            return files_op

    else:
        if toolAsked == 'html':
            op_files = []
            if is_dgps:
                folder = 'DGPS_OUTPUT/ResimulationOutput'
            else:
                folder = 'ORCAS'
            if 'resim_stla_scale3' in simg or 'resim_stla_scale4' in simg:
                folder = "Resim_Radars_deb"
    
            if out_path[-1] == '/':
                path = f'{out_path}{folder}'
            else:
                path = f'{out_path}/{folder}'
    
            files = pickFiles(path)
            for file in files:
                op_files.append(file.split('/')[-1].split('\n')[0])
            op_files.sort()
            #print ("output paths : ",path)
            #print ("output files : ",files_op)
            return path, op_files
                
        elif toolAsked == 'bordnet':
            files_op=[[], [], [], []]
            for root, dirs, files in os.walk(out_path):
                #print("\n\nroot  : ", root)
                #print("dirs  : ", dirs)
                #print("files : ", files)
                if len(files) != 0:
                    if 'Resim_Radars_CAN_Corner_bus' in root or "rFLR" in root:
                        files_op[0] = pickFiles(root)
                    elif 'Aptiv_FLR_Obj_Perc_for_mPAD' in root:
                        files_op[1] = pickFiles(root)
                    elif 'CANoe' in root:
                        files_op[2] = pickFiles(root)
                    elif 'Resim_Radars_CAN_Side_bus' in root:
                        files_op[3] = pickFiles(root)
            #print ("output files : ",files_op)
        return files_op


'''##########         html and bordnet json writer function         ##########'''

###################################################
#
#    function to update key and value for json
#
###################################################
def updateJson(file, path, files_o, key, action=1):
    fileUpdated = 1
    if action == 0:
        file.write(f'\t"{key}" :\n\t[\n')
    else:
        file.write('''\t\t{\n\t\t\t"key": "''' + key + '''",\n\t\t\t\t"files": [\n''')
    for log in files_o:
        if action == 0:
            file.write(f'\t\t"{path}/{log}"')
        else:
            file.write('''\t\t\t\t\t\t"''' + log + '''"''')
        if fileUpdated < len(files_o):
            file.write(",\n")
        else:
            file.write("\n")
        fileUpdated += 1
    if action == 0:
        file.write('\t]')
    else:
        file.write('''\t\t\t\t\t ]\n\t\t}''')

    if key != 'SRR_DUMMY':
        file.write(',')
    file.write('\n')


###################################################
#
#    function to write txt for MUDP tool
#
###################################################
def write_mudp(path,files, path_out, requestFor):
    mudpPath = f'{path_out}/MUDP_REPORT'
    try:os.mkdir(mudpPath)
    except: pass
    if requestFor =="i": txtfile=f"{mudpPath}/MUDP_iList_{sys.argv[5]}.txt"
    else: txtfile=f"{mudpPath}/MUDP_oList_{sys.argv[5]}.txt"

    with open(txtfile,'w') as file:
        for log in files:
            file.write(f"{path}/{log}\n")

    
###################################################
#
#    function to write json for html tool
#
###################################################
def write_htmlJson(path_i, path_o, files_i, files_o, path_out):
    jsnoPath = f'{path_out}/HTML_REPORT'
    try:os.mkdir(jsnoPath)
    except: pass
    files_il = []
    files_ol = []
    for i in files_o:
        for j in files_i:
            if j.split('.')[0] in i:
                files_il.append(j)
                files_ol.append(i)
    files_il.sort()
    files_ol.sort()
    file = f'{jsnoPath}/HTMLInputs_{sys.argv[5]}.json'
    jsonFile = open(file, 'w')
    jsonFile.write('{\n')
    updateJson(jsonFile, path_i, files_il, "INPUT_MF4", 0)
    updateJson(jsonFile, path_o, files_ol, "OUTPUT_MF4", 0)
    updateJson(jsonFile, None, [], "SRR_DUMMY", 0)
    jsonFile.write('}')
    jsonFile.close()


###################################################
#
#    function to write json for bordnet tool
#
###################################################
def write_bordJson(files_i, files_o, path_out, upuFlag):
    jsnoPath = f'{path_out}/BORDNET_REPORT'
    try:os.mkdir(jsnoPath)
    except: pass
    file = f'{jsnoPath}/BORDNET_iList_{sys.argv[5]}.json'
    jsonIFile = open(file, 'w')
    file = f'{jsnoPath}/BORDNET_oList_{sys.argv[5]}.json'
    jsonOFile = open(file, 'w')

    jsonIFile.write('{\n    "reprocessingInputFileStreams": [\n')
    jsonOFile.write('{\n    "reprocessingInputFileStreams": [\n')
    updateJson(jsonIFile, None, [], "INPUT_BN_CALIFR")
    updateJson(jsonOFile, None, [], "INPUT_BN_CALIFR")
    if 'stla_mcip' in simg:
        updateJson(jsonIFile, None, [], "INPUT_BN_FASETH")
        updateJson(jsonIFile, None, files_i[1], "INPUT_SRR_DEBUG")
        updateJson(jsonOFile, None, [], "INPUT_BN_FASETH")
        updateJson(jsonOFile, None, [], "INPUT_SRR_DEBUG")
    else:
        updateJson(jsonIFile, None, [], "INPUT_BN_FASETH")
        updateJson(jsonIFile, None, files_i[2], "INPUT_SRR_DEBUG")
        updateJson(jsonOFile, None, [], "INPUT_BN_FASETH")
        updateJson(jsonOFile, None, [], "INPUT_SRR_DEBUG")
    
    updateJson(jsonIFile, None, [], "OUTPUT_BN_CALIFR")
    updateJson(jsonOFile, None, [], "OUTPUT_BN_CALIFR")
    if 'stla_mcip' in simg:
        updateJson(jsonIFile, None, [], "INPUT_BN_FASETH")
        updateJson(jsonIFile, None, [], "INPUT_SRR_DEBUG")
        updateJson(jsonOFile, None, [], "OUTPUT_BN_FASETH")
        updateJson(jsonOFile, None, files_o[2], "OUTPUT_SRR_DEBUG")
    else:
        updateJson(jsonIFile, None, [], "INPUT_BN_FASETH")
        updateJson(jsonIFile, None, [], "INPUT_SRR_DEBUG")
        updateJson(jsonOFile, None, [], "OUTPUT_BN_FASETH")
        updateJson(jsonOFile, None, files_o[2], "OUTPUT_SRR_DEBUG")
    
    updateJson(jsonIFile, None, [], "SRR_DUMMY")
    updateJson(jsonOFile, None, [], "SRR_DUMMY")
    jsonIFile.write('\t\t]\n}')
    jsonOFile.write('\t\t]\n}')
    jsonIFile.close()
    jsonOFile.close()


'''##########         html and bordnet initial function         ##########'''

###################################################
#
#    function to create json for html tool
#
###################################################
def create_dc_json_html (json_file,output_path):
    #inp_path, inp_files = collect_input_files(json_file,'html')
    oup_path, oup_files = collect_output_files(output_path,'html', simg)
    write_htmlJson(oup_path, oup_path, oup_files,oup_files, output_path)


###################################################
#
#    function to create json for html tool
#
###################################################
def create_json_html (json_file,output_path, upuFlag):
    inp_path, inp_files = collect_input_files(json_file,'html')
    oup_path, oup_files = collect_output_files(output_path,'html', simg, upuFlag)
    write_htmlJson(inp_path, oup_path, inp_files,oup_files, output_path)
    write_mudp(inp_path, inp_files,output_path,"i")
    write_mudp(oup_path, oup_files,output_path,"o")


###################################################
#
#    function to create json for bordnet tool
#
###################################################
def create_json_bordnet (json_file,output_path, simg, upuFlag):
    files_i = collect_input_files(json_file,'bordnet')
    files_o = collect_output_files(output_path,'bordnet', simg, upuFlag)
    #print(files_o)
    write_bordJson(files_i, files_o, output_path, upuFlag)


###################################################
#
#    function to set error code based on truth table
#                                                               #########################
###################################################             # HTML | BORD | ErrCode #
def getErrorCode(flag):                                         #------|------|---------#
    if flag[0] == 1 and flag[1] == 1:                           #  1   |  1   |    0    # -> HTML and BORDNET will run
        err = 0                                                 #  1   |  0   |    1    # -> Only HTML will run
    elif flag[0] == 1 and flag[1] == 0:                         #  0   |  1   |    2    # -> Only Bordnet Will run
        err = 1                                                 #  0   |  0   |    3    # -> None will run
    elif flag[0] == 0 and flag[1] == 1:                         #########################
        err = 2                                                 # 1: success | 0 : fail #
    else:                                                       #########################
        err = 3
    return err


###################################################
#
#    main function
#
###################################################
if __name__ == '__main__':
    json_file = sys.argv[1]
    output_path = sys.argv[2]
    simg = sys.argv[3]
    upuFlag = sys.argv[4].strip("'") == 'True'
    flag = validate_outputFiles(output_path, simg, upuFlag)
    print('[DOCKER] :',flag)

    if flag[0] == 1:
        create_json_html(json_file,output_path, upuFlag)
    if flag[1] == 1:
        create_json_bordnet(json_file,output_path, simg, upuFlag)
    errCode = getErrorCode(flag)
    exit(errCode)
