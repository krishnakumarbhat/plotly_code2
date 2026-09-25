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
        for root, dir, files in os.walk(path):
            #print(files)
            for file in files:
                #print(file)
                if file.endswith('.MF4') or file.endswith('.mf4'):
                    if upu == 1 :
                        upuFlag = True
                        #print(files)
                        if requestfor in file : request_file.append(f"{root}/{file}")
                    else:
                        request_file.append(f"{root}/{file}")
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
        print(input_parameter[2])
        if 'resim_stla_small' in input_parameter[2]:
            deb = getfiles(path+'/RADAR_DEBUG','debrad')
            cbus = getfiles(path+'/RADAR_DATA','datarad')
            mpad = cbus
            ref = getfiles(path, '_REF_', 1)
            #createjson([deb, cbus, mpad, sbus, ref, path])
        elif 'platform' in input_parameter[2] and 'v2' in input_parameter[2] and 'dgps' not in input_parameter[2].lower():
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
        elif 'mcip' in input_parameter[2] :
            deb = getfiles(path,'_b05',1 )
            cbus = getfiles(path,'_b04',1 )
            mpad = cbus
            ref = getfiles(path, '_REF_', 1)
        elif '_dgps' in input_parameter[2].lower() or 'dgps' in input_parameter[2].lower():
            all_files = getfiles(path, '')
            print(f"[DGPS] : total MF4 files found in path : {len(all_files)}")

            # Build lookup maps keyed by common stem (everything before _b05/_g02/_g03)
            import re
            def dgps_stem(filepath):
                name = os.path.splitext(os.path.basename(filepath))[0]
                return re.sub(r'_(b05|g02|g03)$', '', name, flags=re.IGNORECASE)

            b05_map = {}  # stem -> filepath
            g03_map = {}
            g02_map = {}
            for file in all_files:
                fname_lower = os.path.basename(file).lower()
                stem = dgps_stem(file)
                if '_b05' in fname_lower:
                    b05_map.setdefault(stem, file)
                elif '_g03' in fname_lower:
                    g03_map.setdefault(stem, file)
                elif '_g02' in fname_lower:
                    g02_map.setdefault(stem, file)

            # If no files found locally, search sibling directories
            if not b05_map:
                parent = path.rstrip('/').rsplit('/', 1)[0] if '/' in path else ''
                if parent and os.path.isdir(parent):
                    for entry in sorted(os.listdir(parent)):
                        full = os.path.join(parent, entry)
                        if os.path.isdir(full) and os.path.normpath(full) != os.path.normpath(path):
                            for root, dirs, files_in in os.walk(full):
                                for fn in files_in:
                                    if (fn.endswith('.MF4') or fn.endswith('.mf4')):
                                        fn_lower = fn.lower()
                                        fp = os.path.join(root, fn)
                                        stem = dgps_stem(fp)
                                        if '_b05' in fn_lower:
                                            b05_map.setdefault(stem, fp)
                                        elif '_g03' in fn_lower:
                                            g03_map.setdefault(stem, fp)
                                        elif '_g02' in fn_lower:
                                            g02_map.setdefault(stem, fp)

            # Pair corresponding files based on b05 stems
            g02_files = []
            seen_deb = set()
            seen_ref = set()
            seen_g02 = set()
            for stem in sorted(b05_map.keys()):
                fp = b05_map[stem]
                if fp not in seen_deb:
                    deb.append(fp)
                    seen_deb.add(fp)
                if stem in g03_map and g03_map[stem] not in seen_ref:
                    ref.append(g03_map[stem])
                    seen_ref.add(g03_map[stem])
                if stem in g02_map and g02_map[stem] not in seen_g02:
                    g02_files.append(g02_map[stem])
                    seen_g02.add(g02_map[stem])

            # Fall back to g02 for SRR_REFERENCE if no g03 found
            if not ref and g02_files:
                ref = list(g02_files)
                print(f"[DGPS] : no g03 found, falling back to g02 for SRR_REFERENCE")

            # Log unmatched files for visibility
            unmatched_g03 = set(g03_map.keys()) - set(b05_map.keys())
            unmatched_g02 = set(g02_map.keys()) - set(b05_map.keys())
            if unmatched_g03:
                print(f"[DGPS] : WARNING - {len(unmatched_g03)} g03 files have no matching b05")
            if unmatched_g02:
                print(f"[DGPS] : WARNING - {len(unmatched_g02)} g02 files have no matching b05")

            print(f"[DGPS] : b05 (DEBUG) files    : {len(deb)}")
            print(f"[DGPS] : g03 (REF resim) files: {len(ref)}")
            print(f"[DGPS] : g02 (REF video) files: {len(g02_files)}")
            cbus = list(deb)
            mpad = list(deb)
            if deb or ref:
                upuFlag = True
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
        for root, dir, files in os.walk(path):
            for file in files:
                if file.endswith('.MF4') or file.endswith('.mf4'):
                    if upu == 1:
                        upuFlag = True
                        if requestfor in file: request_file.append(f"{root}/{file}")
                    else:
                        if 'ORCAS' not in file and ('DEBUG' in file or '_b05' in file or 'SDVRADAR' in file or 'SDV_RADAR' in file) :
                            request_file.append(f"{root}/{file}")
        #print(request_file)
        return request_file


    files=[]
    singleSession=20
    if input_parameter[5] == 'dSpace':
        files = getfiles(path,'sensordata_UDP')
    else:
        if 'resim_stla_small' in input_parameter[2]:
            files = getfiles(path+'/RADAR_DEBUG','debrad')
        elif 'platform' in input_parameter[2] and 'v2' in input_parameter[2] and 'dgps' not in input_parameter[2].lower():
            files = getfiles(path, 'Gen7')
            #print(files)
            for file in files:
                if "_b05" in file : files = getfiles(path, '_b05',1);break
                elif "SDVRADAR" in file : files = getfiles(path, 'SDVRADAR',1);break
                elif "SDV_RADAR" in file : files = getfiles(path, 'SDV_RADAR',1);break
                elif "BlackMKZ" in file : files = getfiles(path, '_b05',1);break
        elif 'mcip' in input_parameter[2] :
            files = getfiles(path, '_b05', 1)
        elif '_dgps' in input_parameter[2].lower() or 'dgps' in input_parameter[2].lower():
            files = getfiles(path, '_b05', 1)
            # Pre-cache g03/g02 file lists so splitter_highPrio.sh doesn't need to run find per task
            import re as _re
            _jobout = f'{input_parameter[7]}/jobout'
            _g03_cache = []; _g02_cache = []
            _search_root = path
            # Walk entire tree once to find g03 and g02 files
            for _root, _dirs, _files in os.walk(_search_root):
                for _fn in _files:
                    if _fn.endswith('.MF4') or _fn.endswith('.mf4'):
                        _fn_lower = _fn.lower()
                        _fp = f'{_root}/{_fn}'
                        if '_g03' in _fn_lower:
                            _g03_cache.append(_fp)
                        elif '_g02' in _fn_lower:
                            _g02_cache.append(_fp)
            _g03_cache.sort(); _g02_cache.sort()
            with open(f'{_jobout}/dgps_g03_cache.txt', 'w') as _f:
                _f.write('\n'.join(_g03_cache))
            with open(f'{_jobout}/dgps_g02_cache.txt', 'w') as _f:
                _f.write('\n'.join(_g02_cache))
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


"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
15/04/2025          Mandeep Singh       FHW-223     splitter for STLA_SCALE1 Resim(created 1st version of file )

######################################################################################################
"""
