#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

###########################################################
from resim_staticdata import *
###########################################################


#**********************************************************
#          function to collect log from each json for output folder structure
#**********************************************************
def get_outputstructurelist():
    global input_parameter
    txt_path =  f'{input_parameter[7]}/jobout/SIL_Input_all.txt'
    json_path = f'{input_parameter[7]}/jobout/SIL_input_session.txt'

    txt_file = open(txt_path, 'r')
    txt_file_input=txt_file.readlines()
    txt_file.close()

    session_file = open(json_path, 'w')
    for json_file in txt_file_input:
        json_file=json_file.replace("\n","")
        if input_parameter[5] != 'dSpace':
            json_file=open(json_file)
            data=json.load(json_file)
            session=data['reprocessingInputFileStreams'][1]['files']
            session_base = data.get('sessionBasePath', '')
            for log_path in session:
                if "/" in log_path:
                    log = f"{log_path}, {len(session)}, {session_base}"
                    session_file.write(log)
                    session_file.write("\n")
                    break
        else: 
            file = open(json_file,'r')
            logs = file.readlines() 
            file.close()
            for log in logs:
                log=log.replace("\n","")
                if "/" in log:
                   logi = log.split("/")[-1]
                   logi = f"{log}, {len(logs)}"
                   session_file.write(logi)
                   session_file.write("\n")
                   break

    session_file.close() 


#**********************************************************
#          function to write all json in txt file
#**********************************************************
def createInputFlist():
    global input_parameter
    print(f'\n[INFO] : Creating InputList for Resim Run',end='\r')
    path =  f'{input_parameter[7]}/jobout/SIL_Input_all.txt'
    file = open(path,'w')
    if highPrio:  path = f'{input_parameter[7]}/jobout/SIL_input_session.txt'; file1=open(path,'w')
    for split_file in input_file_list:
        file.write(split_file)
        file.write('\n')
        if highPrio : file1.write(split_file); file1.write('\n')
    file.close()
    if highPrio : file1.close()
    print(f'[INFO] : Created  InputList for Resim Run')
    #input_parameter.append(path)
    if not highPrio: get_outputstructurelist()



#**********************************************************
#          function to write key and values in json
#**********************************************************
def write_json(file, data, value):
    no_of_files = len(data)
    file_written_count = 1
    file.write('''\t\t{\n\t\t\t"key": "''' + value + '''",\n\t\t\t\t"files": [\n''')
    for log in data:
        log = log.replace('\\', '/')
        file.write('''\t\t\t\t\t\t"''' + log + '''"''')
        if file_written_count < no_of_files:
            file.write(",\n")
        else:
            file.write("\n")
        file_written_count += 1
    if value != "SRR_REFERENCE":
        file.write('''\t\t\t\t\t ]\n\t\t},\n''')
    else:
        file.write('''\t\t\t\t\t ]\n\t\t}\n''')


#**********************************************************
#          function to sort and create json files
#**********************************************************
def createjson(input_data):
    global input_file_list
    cnt = sessions.index(input_data[5])+1
    json_path = f'{input_parameter[7]}/jobout/SIL_input_{cnt}.json' 
    jsonLists.append(json_path)
    
    input_data[0].sort()
    input_data[1].sort()
    input_data[2].sort()
    input_data[3].sort()
    input_data[4].sort()

    # Enforce equal number of logs for paired streams (SRR_DEBUG and SRR_REFERENCE)
    # especially for DGPS mode where pairing is critical.
    simg_name = os.path.basename(input_parameter[2]).lower()
    if ('dgps' in simg_name):
        deb_len = len(input_data[0])
        ref_len = len(input_data[4])
        if deb_len != ref_len:
            min_len = min(deb_len, ref_len)
            print(f"[WARN] : Unequal logs in JSON {cnt} (DEBUG:{deb_len}, REF:{ref_len}). Truncating all streams to {min_len}")
            input_data[0] = input_data[0][:min_len] # SRR_DEBUG
            input_data[1] = input_data[1][:min_len] # BN_CALIFR
            input_data[2] = input_data[2][:min_len] # BN_FASETH
            input_data[4] = input_data[4][:min_len] # SRR_REFERENCE
            # Special case for mpad if it was passed as input_data[3] (sbus usually empty here but just in case)
            if len(input_data) > 3 and isinstance(input_data[3], list):
                if len(input_data[3]) > min_len: input_data[3] = input_data[3][:min_len]

    json_data = {
        "sessionBasePath": input_data[5],
        "reprocessingInputFileStreams": [
            {"key": "BN_CALIFR", "files": input_data[1]},
            {"key": "BN_FASETH", "files": input_data[2]},
            {"key": "SRR_DEBUG", "files": input_data[0]},
            {"key": "SRR_REFERENCE", "files": input_data[4]}
        ]
    }

    with open(json_path, 'w') as json_file:
        json.dump(json_data, json_file, indent='\t')

    input_file_list.append(json_path)
    print(f'[INFO] : created .json for {cnt}/{len(sessions)}',end='\r')


#**********************************************************
#          function to sort and create json files
#**********************************************************
def createtxt(input_data):
    global input_file_list
    cnt = sessions.index(input_data[1])+1
    json_path = f'{input_parameter[7]}/jobout/SIL_input_{cnt}.txt' 
    jsonLists.append(json_path)
    
    input_data[0].sort()

    with open(json_path, 'w') as txt_file:
        for log in input_data[0]:
            txt_file.write(f"{log}\n")

    input_file_list.append(json_path)
    print(f'[INFO] : created .txt for {cnt}/{len(sessions)}',end='\r')


#**********************************************************
#          function to collect files to be written in json
#**********************************************************

#default priority
def createJson(path, curr_cnt, sessions):
    try: input_parameter[9]
    except: input_parameter.append(False)

    global upuFlag
    print(f"[INFO] : collecting files information : {curr_cnt}/{sessions}", end='\r')
    def getfiles(path, requestfor, upu=0):
        global upuFlag
        request_file = []
        if os.path.exists(path): print("path exist")
        else: print("issue with path")
        for root, dir, files in os.walk(path, followlinks=True):
            #print(files)
            for file in files:
                #print(file)
                if file.endswith('.MF4') or file.endswith('.mf4'):
                    full_path = f"{root}/{file}"
                    if upu == 1 :
                        upuFlag = True
                        #print(files)
                        if requestfor in full_path : request_file.append(full_path)
                    else:
                        request_file.append(full_path)
        #print(request_file)
        return request_file

    
    deb=[]
    cbus=[]
    sbus=[]
    mpad=[]
    ref=[]

    if input_parameter[5] == 'dSpace':
        deb = getfiles(path, 'sensordata_UDP')
        createtxt([deb, path])
    else:
        simg_name = os.path.basename(input_parameter[2]).lower()
        print(input_parameter[2])
        if 'resim_stla_small' in simg_name:
            deb = getfiles(path+'/RADAR_DEBUG','debrad')
            cbus = getfiles(path+'/RADAR_DATA','datarad')
            mpad = cbus
            ref = getfiles(path, '_REF_', 1)
            #createjson([deb, cbus, mpad, sbus, ref, path])
        elif 'platform' in simg_name and 'v2' in simg_name and 'dgps' not in simg_name:
            print(path)
            files = getfiles(path, '_DEBUG_')
            #print(files)
            for file in files:
                if "_b05" in file : deb = getfiles(path, '_b05',1); cbus = getfiles(path, '_b04',1); ref = getfiles(path, '_REF_',1); break
                elif "SDVRADAR" in file : deb = getfiles(path, 'SDVRADAR',1); ref = getfiles(path, '_REF_',1); break
                elif "SDV_RADAR" in file : deb = getfiles(path, 'SDV_RADAR',1); ref = getfiles(path, '_REF_',1); break
                elif "BlackMKZ" in file : deb = getfiles(path, '_b05',1); ref = getfiles(path, '_REF_',1); break
                else:
                    filename=file.split('/')[-1]
                    #print(filename)
                    if '_DEBUG_' in filename and 'ORCAS' not in filename and 'CANoe' not in filename: deb.append(file)
                    elif '_BUS_' in filename: cbus.append(file)
                    elif '_REF_' in filename and 'CANoe' not in filename: ref.append(file)

            if len(cbus) == 0: mpad = cbus = deb
            else: mpad = cbus
            #createjson([deb, cbus, mpad, sbus, ref, path])
        elif 'mcip' in simg_name :
            deb = getfiles(path,'_b05',1 )
            cbus = getfiles(path,'_b04',1 )
            mpad = cbus
            ref = getfiles(path, '_REF_', 1)
        elif '_dgps' in simg_name or 'dgps' in simg_name:
            # First, fetch all g03 and g02 files from the root search path once to act as "precached" list
            root_g03_map = {}
            root_g02_map = {}
            _jobout = f'{input_parameter[7]}/jobout'
            
            print(f"[DGPS] : Pre-scanning root for g03/g02 files in {path}")
            for _root, _dirs, _files in os.walk(path, followlinks=True):
                for _fn in _files:
                    if _fn.lower().endswith('.mf4'):
                        _fn_lower = _fn.lower()
                        _fp = os.path.join(_root, _fn).replace('\\', '/')
                        if '_g03' in _fn_lower:
                            _stem = re.sub(r'_g03\.mf4$', '', _fn_lower)
                            root_g03_map[_stem] = _fp
                        elif '_g02' in _fn_lower:
                            _stem = re.sub(r'_g02\.mf4$', '', _fn_lower)
                            root_g02_map[_stem] = _fp

            # Write cache files for debug visibility
            with open(f'{_jobout}/dgps_g03_cache.txt', 'w') as _f:
                _f.write('\n'.join(sorted(root_g03_map.values())))
            with open(f'{_jobout}/dgps_g02_cache.txt', 'w') as _f:
                _f.write('\n'.join(sorted(root_g02_map.values())))

            # Now collect all b05 files in the current session path
            all_files = getfiles(path, '')
            print(f"[DGPS] : total MF4 files found in session path : {len(all_files)}")
            
            g02_files = []
            for file in all_files:
                fname_lower = os.path.basename(file).lower()
                if '_b05' in fname_lower:
                    stem = re.sub(r'_b05\.mf4$', '', fname_lower)
                    # Fetch from "precached" maps
                    g03_match = root_g03_map.get(stem)
                    g02_match = root_g02_map.get(stem)
                    
                    if g03_match:
                        deb.append(file)
                        ref.append(g03_match)
                        if g02_match:
                            g02_files.append(g02_match)
                    else:
                        print(f"[DGPS] : Skipping {file} - no matching g03 found in cache")

            print(f"[DGPS] : b05 (DEBUG) paired files    : {len(deb)}")
            print(f"[DGPS] : g03 (REF resim) files: {len(ref)}")
            print(f"[DGPS] : g02 (REF video) files: {len(g02_files)}")
            cbus = list(deb)
            mpad = list(deb)
            if deb or ref:
                upuFlag = True
            print(f'[DGPS] : Written g03/g02 cache to {_jobout}/')
        elif 'dc' in input_parameter[2] : pass
        elif 'vv' in input_parameter[2] : pass
        else:
            # Default / Gen7 handler: categorize by filename tags
            all_files = getfiles(path, '')
            for file in all_files:
                filename = file.split('/')[-1]
                if '_DEBUG_' in filename and 'ORCAS' not in filename:
                    deb.append(file)
                elif '_BUS_' in filename:
                    cbus.append(file)
                elif '_REF_' in filename:
                    ref.append(file)
            if len(cbus) == 0:
                mpad = cbus = deb
            else:
                mpad = cbus
        createjson([deb, cbus, mpad, sbus, ref, path])
    input_parameter[9] = upuFlag

    


# highPriority
def createFileList(path, curr_cnt, sessions):
    try: input_parameter[9]
    except: input_parameter.append(False)

    global input_file_list
    def getfiles(path, requestfor, upu=0):
        global upuFlag
        request_file = []
        #print("UPU :",upu)
        if os.path.exists(path): print("path exist")
        else: print("issue with path")
        for root, dir, files in os.walk(path, followlinks=True):
            for file in files:
                if file.endswith('.MF4') or file.endswith('.mf4'):
                    full_path = f"{root}/{file}"
                    if upu == 1:
                        upuFlag = True
                        if requestfor in file: request_file.append(full_path)
                    else:
                        if 'ORCAS' not in file and ('DEBUG' in file or '_b05' in file or 'SDVRADAR' in file or 'SDV_RADAR' in file) :
                            request_file.append(full_path)
        #print(request_file)
        return request_file


    files=[]
    singleSession=10
    simg_name = os.path.basename(input_parameter[2]).lower()
    if input_parameter[5] == 'dSpace':
        files = getfiles(path,'sensordata_UDP')
    else:
        if 'resim_stla_small' in simg_name:
            files = getfiles(path+'/RADAR_DEBUG','debrad')
        elif 'platform' in simg_name and 'v2' in simg_name and 'dgps' not in simg_name:
            files = getfiles(path, 'Gen7')
            #print(files)
            for file in files:
                if "_b05" in file : files = getfiles(path, '_b05',1);break
                elif "SDVRADAR" in file : files = getfiles(path, 'SDVRADAR',1);break
                elif "SDV_RADAR" in file : files = getfiles(path, 'SDV_RADAR',1);break
                elif "BlackMKZ" in file : files = getfiles(path, '_b05',1);break
        elif 'mcip' in simg_name :
            files = getfiles(path, '_b05', 1)
        elif '_dgps' in simg_name or 'dgps' in simg_name:
            # Pre-calculate g03/g02 file lists to filter b05 logs and for splitter_highPrio.sh cache
            _jobout = f'{input_parameter[7]}/jobout'
            _g03_cache = []; _g02_cache = []
            _g03_stems = set()
            _search_root = path
            
            # Walk entire tree once to find g03 and g02 files
            for _root, _dirs, _files in os.walk(_search_root, followlinks=True):
                for _fn in _files:
                    if _fn.endswith('.MF4') or _fn.endswith('.mf4'):
                        _fn_lower = _fn.lower()
                        _fp = f'{_root}/{_fn}'
                        if '_g03' in _fn_lower:
                            _g03_cache.append(_fp)
                            _stem = re.sub(r'_g03\.mf4$', '', _fn_lower)
                            _g03_stems.add(_stem)
                        elif '_g02' in _fn_lower:
                            _g02_cache.append(_fp)
            
            _g03_cache.sort(); _g02_cache.sort()
            with open(f'{_jobout}/dgps_g03_cache.txt', 'w') as _f:
                _f.write('\n'.join(_g03_cache))
            with open(f'{_jobout}/dgps_g02_cache.txt', 'w') as _f:
                _f.write('\n'.join(_g02_cache))
            
            # Now fetch b05 files and filter by g03 existence
            all_b05 = getfiles(path, '_b05', 1)
            files = []
            for b05 in all_b05:
                _b05_stem = re.sub(r'_b05\.mf4$', '', os.path.basename(b05).lower())
                if _b05_stem in _g03_stems:
                    files.append(b05)
                else:
                    print(f"[DGPS] : Filtering out {os.path.basename(b05)} - no matching g03 found")

            print(f'[DGPS] : Filtered {len(files)} paired b05 logs out of {len(all_b05)} total')
            print(f'[DGPS] : Pre-cached {len(_g03_cache)} g03 and {len(_g02_cache)} g02 file paths to jobout/')
        elif 'dc' in input_parameter[2] :
            files = getfiles(path, 'Gen7')
            for file in files:
                if "_b05" in file : files = getfiles(path, '_b05',1);break
                elif "SDVRADAR" in file : files = getfiles(path, 'SDVRADAR',1);break
                elif "SDV_RADAR" in file : files = getfiles(path, 'SDV_RADAR',1);break
                elif "BlackMKZ" in file : files = getfiles(path, '_b05',1);break
        elif 'vv' in input_parameter[2] : pass
        else:
            # Default / Gen7 handler
            files = getfiles(path, '')
            files = [f for f in files if '_DEBUG_' in f.split('/')[-1] and 'ORCAS' not in f.split('/')[-1] and 'CANoe' not in f.split('/')[-1]]

    files.sort()
    #print(files)
    cnt = len(files)
    #print(cnt, files)

    if cnt == 0:
        print(f"[WARN] : No input logs found for session {path}; skipping this session")
        return

    if cnt <=25: subset=1
    else: 
        if cnt%singleSession == 0 :subset=(cnt//singleSession)
        else : subset=(cnt//singleSession)+1
    for i in range(0,subset):
        if i==0:
            if subset == 1 : newpath=f"{files[i]},{cnt},{path}"
            else : newpath=f"{files[i]},{singleSession},{path}"
        elif i==subset-1 : newpath=f"{files[singleSession*i]},{cnt-singleSession*i},{path}"
        else : newpath=f"{files[singleSession*i]},{singleSession},{path}"
        input_file_list.append(newpath)
        print(f"[INFO] : collecting files information : {curr_cnt}/{sessions}      Sub-Sessions : {len(input_file_list)}", end='\r')
    input_parameter[9] = upuFlag
