# -*- coding: utf-8 -*-
"""
Created on Tuesday Nov-26 12:29:06 2024

@author: mandeep.singh1@aptiv.com
"""

import sys, os, re
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

    # Task-specific error file to avoid race conditions in distributed mode
    task_id = sys.argv[5] if len(sys.argv) > 5 else "0"
    pipeLineError = f'{output_path}/pipeLineError_{task_id}.txt'

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
                if 'DGPS_OUTPUT' in folder or 'ORCAS' in folder:
                    f_orcas = 1
            if f_orcas == 0:
                error, flags_pipeline[0] = updateFileError(pipeLineFile,"[Error] : DGPS/ORCAS folder not present\n")
            print('inside-dc :',flags_pipeline)
        else:
            for folder in folderOp:
                if "ORCAS" == folder:
                    f_orcas = 1
                elif "CANoe" == folder:
                    f_can = 1
                elif "rR" in folder:
                    f_rFLR = 1

            if f_can == 0 and f_rFLR == 0:
                for root, dirs, files in os.walk(output_path):
                    if "rR" in root and any(file.endswith('.MF4') or file.endswith('.mf4') for file in files):
                        f_rFLR = 1
                        break
            if f_orcas == 0:
                error, flags_pipeline[0] = updateFileError(pipeLineFile,"[Error] : ORCAS folder not present\n")
            if f_can == 0 and f_rFLR == 0:
                error, flags_pipeline[1] = updateFileError(pipeLineFile,"[Error] : CANoe or rR output folder not present\n")
                
    if error == False:
        # Robust parallel deletion (avoid TOCTOU race condition)
        try:
            os.remove(pipeLineError)
        except OSError:
            pass
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
#    helper to extract 4-digit log number from MF4 filename
#
###################################################
def _get_log_num(filepath):
    """
    Extract the trailing 4-digit log number from an MF4 filename.
    Handles both patterns:
    - Original: CCA_9010_3100099_DEBUG_20250828_163703_0000.MF4 → 0000
    - With rR suffix: CCA_9010_3100099_BUS_20250828_164648_0039_rR00070107.MF4 → 0039
    """
    basename = os.path.basename(filepath)
    # Try to match: _NNNN_rR (rTag output files) or _NNNN.MF4 (standard files)
    m = re.search(r'_(\d{4})(?:_rR|\.(?:MF4|mf4))', basename)
    return m.group(1) if m else None


###################################################
#
#    function to read and collect input files from json
#
###################################################
def collect_input_files(json_file, toolAsked, source_dir=""):
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

        # Fix temp /dev/shm BUS file paths: replace with original network paths from source_dir
        # These temp paths are deleted after MUDP input, so bordnet can't access them.
        # source_dir = original directory where debug files came from (passed in highPrio mode)
        print(f"[BUS_PATH_FIX] : source_dir={repr(source_dir)}, exists={os.path.isdir(source_dir) if source_dir else False}")
        if source_dir and os.path.isdir(source_dir):
            for idx in [0, 1]:
                if files[idx]:
                    fixed = []
                    replaced = 0
                    for f in files[idx]:
                        if '/dev/shm/' in f or '/tmp/tmp.' in f:
                            basename = os.path.basename(f)
                            
                            # Try multiple search locations:
                            # 1. Direct in source_dir (same as debug files)
                            # 2. Parent directory (sibling folders like debug/, bus/, canoe/, etc.)
                            # 3. Recursive search in parent directory
                            search_paths = [
                                os.path.join(source_dir, basename),                    # source_dir/BUS_FILE
                                os.path.join(os.path.dirname(source_dir), basename),  # parent/BUS_FILE
                            ]
                            
                            # Add wildcard search: find any file matching the basename
                            parent_dir = os.path.dirname(source_dir)
                            if os.path.isdir(parent_dir):
                                for root, dirs, found_files in os.walk(parent_dir):
                                    for found_file in found_files:
                                        if found_file == basename:
                                            search_paths.insert(0, os.path.join(root, found_file))
                                            break
                            
                            # Try each search path
                            orig_found = None
                            for search_path in search_paths:
                                if os.path.isfile(search_path):
                                    orig_found = search_path
                                    break
                            
                            if orig_found:
                                fixed.append(orig_found)
                                replaced += 1
                                print(f"[BUS_PATH_FIX] : Found: {basename} at {orig_found}")
                            else:
                                fixed.append(f)  # fallback: keep temp path
                                print(f"[BUS_PATH_FIX] : NOT FOUND: {basename}, keeping temp path")
                        else:
                            fixed.append(f)
                    files[idx] = fixed
                    print(f"[BUS_PATH_FIX] : stream[{idx}] fixed {replaced}/{len(fixed)} temp paths")
        elif source_dir:
            print(f"[BUS_PATH_FIX] : WARNING - source_dir not found: {source_dir}")
        else:
            print(f"[BUS_PATH_FIX] : WARNING - source_dir is empty/None, cannot fix bus paths")

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
            # Collect ALL MF4 files from rR* folders; will filter by log-number matching in write_bordJson()
            files_op=[[], [], [], []]
            rr_files = []
            print(f"[BORD_COLLECT_DEBUG] : upuFlag=True - Starting walk of out_path={out_path}")
            for root, dir, files in os.walk(out_path):
                print(f"[BORD_COLLECT_DEBUG] : Walking {root}, files count: {len(files)}")
                for file in files:
                    if "rR" in root and (file.endswith('.MF4') or file.endswith('.mf4')):
                        full_path = f"{root}/{file}"
                        rr_files.append(full_path)
                        print(f"[BORD_COLLECT_DEBUG] : Found rR file: {full_path}")
            rr_files.sort()
            files_op[2] = rr_files
            print(f"[BORD_COLLECT] : upuFlag=True - collected {len(files_op[2])} total MF4 files from rR* folder (will filter by log-number intersection)")
            return files_op

    else:
        if toolAsked == 'html':
            op_files = []
            if is_dgps:
                folder = 'DGPS_OUTPUT/ResimulationOutput'
                # Fallback to ORCAS if DGPS_OUTPUT doesn't exist
                test_path = f'{out_path}/{folder}' if out_path[-1] != '/' else f'{out_path}{folder}'
                if not os.path.isdir(test_path):
                    folder = 'ORCAS'
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
            rr_files = []
            print(f"[BORD_COLLECT_DEBUG] : Starting walk of out_path={out_path}, upuFlag={upuFlag}")
            for root, dirs, files in os.walk(out_path):
                print(f"[BORD_COLLECT_DEBUG] : Walking {root}, files count: {len(files)}")
                if len(files) != 0:
                    # Check for specific output folders (these are NOT mutually exclusive with rR check)
                    if 'Resim_Radars_CAN_Corner_bus' in root or "rFLR" in root:
                        files_op[0] = pickFiles(root)
                        print(f"[BORD_COLLECT_DEBUG] : Found CAN_Corner_bus/rFLR: {len(files_op[0])} files")
                    if 'Aptiv_FLR_Obj_Perc_for_mPAD' in root:
                        files_op[1] = pickFiles(root)
                        print(f"[BORD_COLLECT_DEBUG] : Found Aptiv_FLR: {len(files_op[1])} files")
                    if 'CANoe' in root:
                        files_op[2] = pickFiles(root)
                        print(f"[BORD_COLLECT_DEBUG] : Found CANoe: {len(files_op[2])} files")
                    if 'Resim_Radars_CAN_Side_bus' in root:
                        files_op[3] = pickFiles(root)
                        print(f"[BORD_COLLECT_DEBUG] : Found CAN_Side_bus: {len(files_op[3])} files")
                    
                    # ALWAYS check for rR* folders (rTag output directories) - use if, not elif!
                    if 'rR' in root:
                        print(f"[BORD_COLLECT_DEBUG] : Found rR directory: {root}")
                        for file in files:
                            if file.endswith('.MF4') or file.endswith('.mf4'):
                                full_path = f"{root}/{file}"
                                rr_files.append(full_path)
                                log_num = _get_log_num(file)
                                print(f"[BORD_COLLECT_DEBUG] : Found rR file: {full_path} (log {log_num})")
            
            if len(rr_files) != 0:
                rr_files.sort()
                files_op[2] = rr_files
            print(f"[BORD_COLLECT] : upuFlag=False - collected {len(files_op[2])} total MF4 files from rR* folder (will filter by log-number intersection)")
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
    # Return paired files for MUDP to ensure matching counts
    return files_il, files_ol


###################################################
#
#    Helper function to extract log number range from input files
#
###################################################
def _get_log_range_from_inputs(input_files):
    """
    Extract the range of log numbers from input files.
    Returns a set of log numbers present in the inputs.
    Example: ['...0000.MF4', '...0001.MF4', '...0015.MF4'] → {'0000', '0001', '0015'}
    """
    log_nums = set()
    if input_files:
        for f in input_files:
            log_num = _get_log_num(f)
            if log_num:
                log_nums.add(log_num)
    return log_nums


"""
# BORDNET DISABLED - ENTIRE write_bordJson() FUNCTION COMMENTED OUT
def write_bordJson(files_i, files_o, path_out, upuFlag):
    print("[BORDNET_DISABLED] : write_bordJson() is disabled - BORDNET execution skipped")
    pass


# ORIGINAL write_bordJson CODE (DISABLED):
# jsnoPath = f'{path_out}/BORDNET_REPORT'
# try:os.mkdir(jsnoPath)
# except: pass
# ... (entire function code commented out for brevity)
"""


'''##########         html and bordnet initial function         ##########'''

###################################################
#
#    function to create json for html tool
#
###################################################
def create_dc_json_html (json_file,output_path):
    #inp_path, inp_files = collect_input_files(json_file,'html')
    oup_path, oup_files = collect_output_files(output_path,'html', simg)
    # write_htmlJson now returns paired files, discard since DC mode doesn't use MUDP
    _ = write_htmlJson(oup_path, oup_path, oup_files,oup_files, output_path)


###################################################
#
#    function to create json for html tool
#
###################################################
def create_json_html (json_file,output_path, upuFlag):
    inp_path, inp_files = collect_input_files(json_file,'html')
    oup_path, oup_files = collect_output_files(output_path,'html', simg, upuFlag)
    # write_htmlJson now returns paired files (files_il, files_ol)
    paired_inp_files, paired_oup_files = write_htmlJson(inp_path, oup_path, inp_files,oup_files, output_path)
    # Use paired files for MUDP to ensure input and output lists match
    write_mudp(inp_path, paired_inp_files,output_path,"i")
    write_mudp(oup_path, paired_oup_files,output_path,"o")


"""
# BORDNET DISABLED - ENTIRE create_json_bordnet() FUNCTION COMMENTED OUT
def create_json_bordnet (json_file, output_path, simg, upuFlag, source_dir=""):
    print("[BORDNET_DISABLED] : create_json_bordnet() is disabled - BORDNET JSON creation skipped")
    pass


# ORIGINAL create_json_bordnet CODE (DISABLED):
# print(f"[CREATE_JSON_BORDNET] : input json_file={json_file}")
# print(f"[CREATE_JSON_BORDNET] : output_path={output_path}")
# ... (entire function code commented out for brevity)
"""


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
    # sys.argv[6] = original source directory of debug files (passed in highPrio mode)
    # used to resolve temp /dev/shm bus file paths back to original network paths
    source_dir = sys.argv[6].strip() if len(sys.argv) > 6 else ""
    print(f'[INIT] : sys.argv count = {len(sys.argv)}')
    print(f'[INIT] : source_dir = {repr(source_dir)}')
    print(f'[INIT] : source_dir exists = {os.path.isdir(source_dir)}')
    flag = validate_outputFiles(output_path, simg, upuFlag)
    print('[DOCKER] :',flag)

    if flag[0] == 1:
        create_json_html(json_file, output_path, upuFlag)
    #if flag[1] == 1:
    #    create_json_bordnet(json_file, output_path, simg, upuFlag, source_dir)
    # BORDNET DISABLED - create_json_bordnet() call commented out
    errCode = getErrorCode(flag)
    exit(errCode)
