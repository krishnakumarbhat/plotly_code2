#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

###########################################################
import os
import re
from resim_staticdata import *
import resim_staticdata
###########################################################

# Flag to track if GPO-V2 cache files have been initialized for this run
_gpo_v2_cache_initialized = False

#**********************************************************
#          helper to exclude the 0th log segment
#**********************************************************
def _is_zero_segment_file(filename):
    """Return True if `filename` is the 0th log segment (e.g. '..._0000_b05.MF4',
    '..._0000_b04.MF4', '..._0000_BUS_...MF4'). When the 'rm_zero' pipeline argument
    is passed, these files (and by extension their paired bus/deb/ref logs, since
    they share the same '_0000_' segment token) must be excluded entirely from the
    resim JSONs."""
    return rm_zero and '_0000_' in filename

#**********************************************************
#   helper to build the pre-cached g02 (and v01 fallback) lists used by VIDEO
#**********************************************************
# Cache filenames that have already been cleared once in this process, so stale
# entries from a prior invocation aren't carried over into a fresh run.
_g02_cache_cleared = set()

def _cache_tagged_files(path, jobout_dir, cache_filename, tag):
    """Scan `path` for *{tag}*.MF4 files and append any new ones (de-duplicated,
    idempotent) to `cache_filename` under jobout_dir. This is the pre-cached list
    that the VIDEO step in resim_child.py reads SRR_REFERENCE files from for
    GPO-V2 / MCIP / DGPS docker types (mirrors the existing dgps_g02_cache.txt pattern)."""
    cache_path = os.path.join(jobout_dir, cache_filename)
    if cache_filename not in _g02_cache_cleared:
        if os.path.isfile(cache_path):
            os.remove(cache_path)
        _g02_cache_cleared.add(cache_filename)

    found = []
    for _root, _dirs, _files in os.walk(path, followlinks=True):
        for _fn in _files:
            if _fn.lower().endswith('.mf4') and tag in _fn.lower() and not _is_zero_segment_file(_fn):
                found.append(os.path.join(_root, _fn).replace('\\', '/'))
    found = sorted(set(found))

    file_exists = os.path.isfile(cache_path)
    existing = set()
    if file_exists:
        with open(cache_path) as rf:
            existing = {l.strip() for l in rf if l.strip()}
    new_entries = [f for f in found if f not in existing]
    if new_entries:
        with open(cache_path, 'a' if file_exists else 'w') as f:
            f.write('\n'.join(new_entries) + '\n')
    print(f"[VIDEO_CACHE] : found {len(found)} {tag} file(s) under {path}, added {len(new_entries)} new entry(ies) to {cache_filename}")
    return len(found)

def _cache_g02_files(path, jobout_dir, cache_filename):
    return _cache_tagged_files(path, jobout_dir, cache_filename, '_g02')

def _cache_v01_files(path, jobout_dir, cache_filename):
    """Fallback source for VIDEO when a session has no g02 files at all."""
    return _cache_tagged_files(path, jobout_dir, cache_filename, '_v01')

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

    # Enforce equal number of logs for paired streams across all docker types.
    # This ensures that SRR_DEBUG and SRR_REFERENCE counts match, which is critical
    # for proper pairing in retry scenarios and all customer environments.
    deb_len = len(input_data[0])
    ref_len = len(input_data[4])
    if deb_len != ref_len:
        min_len = min(deb_len, ref_len)
        simg_name = os.path.basename(input_parameter[2]).lower()
        print(f"[WARN] : Unequal logs in JSON {cnt} (DEBUG:{deb_len}, REF:{ref_len}). Truncating all streams to {min_len} ({simg_name})")
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
    upuFlag = False
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
                    if _is_zero_segment_file(file): continue
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
            # Try new b04/b05 tag filtering first
            deb = getfiles(path,'_b05',1)
            cbus = getfiles(path,'_b04',1)
            # Fall back to old debrad/datarad folder approach if no b04/b05 files found
            if len(deb) == 0 or len(cbus) == 0:
                deb = getfiles(path+'/RADAR_DEBUG','debrad')
                cbus = getfiles(path+'/RADAR_DATA','datarad')
            mpad = cbus
            ref = getfiles(path, '_REF_', 1)
            upuFlag = True
            createjson([deb, cbus, mpad, sbus, ref, path])
        elif 'platform' in simg_name and 'v2' in simg_name and 'dgps' not in simg_name:
            print(path)
            upuFlag = True
            # Pre-cache g02 files for this docker type, read by the VIDEO step (converter SRR_REFERENCE)
            _cache_g02_files(path, f'{input_parameter[7]}/jobout', 'gpo_v2_g02_cache.txt')
            _cache_v01_files(path, f'{input_parameter[7]}/jobout', 'gpo_v2_v01_cache.txt')
            files = getfiles(path, '_DEBUG_')
            #print(files)
            for file in files:
                if "_b05" in file : deb = getfiles(path, '_b05',1); cbus = getfiles(path, f'_{bus_tag}',1); ref = getfiles(path, '_REF_',1); break
                elif "SDVRADAR" in file : deb = getfiles(path, 'SDVRADAR',1); ref = getfiles(path, '_REF_',1); break
                elif "SDV_RADAR" in file : deb = getfiles(path, 'SDV_RADAR',1); ref = getfiles(path, '_REF_',1); break
                elif "BlackMKZ" in file : deb = getfiles(path, '_b05',1); ref = getfiles(path, '_REF_',1); break
                else:
                    filename=file.split('/')[-1]
                    #print(filename)
                    if '_DEBUG_' in filename and 'ORCAS' not in filename and 'CANoe' not in filename: deb.append(file)
                    elif '_BUS_' in filename: cbus.append(file)
                    elif '_REF_' in filename and 'CANoe' not in filename: ref.append(file)

            if len(cbus) == 0: cbus = getfiles(path, f'_{bus_tag}',1)
            # For GPO-V2, BN_FASETH should follow the bus stream (b04 by default, or b02 when requested).
            mpad = cbus
            #createjson([deb, cbus, mpad, sbus, ref, path])
        elif 'ifv600_' in simg_name:
            # ADCAM docker (ifv600_*.simg): only p01 MF4 files feed SRR_DEBUG;
            # BN_CALIFR/BN_FASETH/SRR_REFERENCE stay empty for this docker type.
            upuFlag = True
            all_files = getfiles(path, '')
            for file in all_files:
                filename = file.split('/')[-1]
                if '_p01' in filename.lower():
                    deb.append(file)
            cbus = []
            mpad = []
            ref = []
        elif 'mcip' in simg_name :
            # Pre-cache g02 files for this docker type, read by the VIDEO step (converter SRR_REFERENCE)
            _cache_g02_files(path, f'{input_parameter[7]}/jobout', 'mcip_g02_cache.txt')
            _cache_v01_files(path, f'{input_parameter[7]}/jobout', 'mcip_v01_cache.txt')
            # Pre-cache b05/bus files too: a single batch can span multiple nested
            # session folders, so splitter_highPrio.sh must read the full recursive
            # set from a cache instead of scanning only the first file's own folder.
            _cache_tagged_files(path, f'{input_parameter[7]}/jobout', 'mcip_b05_cache.txt', '_b05')
            _cache_tagged_files(path, f'{input_parameter[7]}/jobout', 'mcip_bus_cache.txt', f'_{bus_tag}')
            deb = getfiles(path,'_b05',1 )
            cbus = getfiles(path,f'_{bus_tag}',1 )
            # BN_CALIFR and BN_FASETH should both follow the BUS stream (BUS BUS DEBUG)
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
            # Fallback source for VIDEO when a session has no g02 files at all
            _cache_v01_files(path, _jobout, 'dgps_v01_cache.txt')

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
                # No separate BUS-tagged files found for this session; fall back to DEBUG
                mpad = cbus = deb
            else:
                # BN_CALIFR and BN_FASETH should both follow the BUS stream (BUS BUS DEBUG)
                mpad = cbus
        createjson([deb, cbus, mpad, sbus, ref, path])
    input_parameter[9] = upuFlag

    


# highPriority
def createFileList(path, curr_cnt, sessions):
    try: input_parameter[9]
    except: input_parameter.append(False)

    global input_file_list, upuFlag
    upuFlag = False  # Initialize with default value
    
    def getfiles(path, requestfor, upu=0):
        global upuFlag
        request_file = []
        #print("UPU :",upu)
        if os.path.exists(path): print("path exist")
        else: print("issue with path")
        for root, dir, files in os.walk(path, followlinks=True):
            for file in files:
                if file.endswith('.MF4') or file.endswith('.mf4'):
                    if _is_zero_segment_file(file): continue
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
    singleSession=15
    simg_name = os.path.basename(input_parameter[2]).lower()
    if input_parameter[5] == 'dSpace':
        files = getfiles(path,'sensordata_UDP')
    else:
        if 'resim_stla_small' in simg_name:
            # Try new LABCAR3 flat naming first
            files = getfiles(path,'_b05',1)
            # Fall back to old debrad/RADAR_DEBUG folder structure only if no _b05 files found
            if len(files) == 0:
                files = getfiles(path+'/RADAR_DEBUG','debrad')
        elif 'platform' in simg_name and 'v2' in simg_name and 'dgps' not in simg_name:
            # Pre-cache all b05 and b04 files for GPO-V2 to handle nested folder structures
            global _gpo_v2_cache_initialized
            upuFlag = True
            _jobout = f'{input_parameter[7]}/jobout'
            _b05_cache = []; _bus_cache = []
            _bus_tag_lower = bus_tag.lower()
            
            # Clear old cache files on first GPO-V2 path of this run
            if not _gpo_v2_cache_initialized:
                _b05_cache_path = f'{_jobout}/gpo_v2_b05_cache.txt'
                _bus_cache_path = f'{_jobout}/gpo_v2_bus_cache.txt'
                try:
                    if os.path.isfile(_b05_cache_path):
                        os.remove(_b05_cache_path)
                        print(f'[GPO-V2] : Cleared old b05 cache file')
                    if os.path.isfile(_bus_cache_path):
                        os.remove(_bus_cache_path)
                        print(f'[GPO-V2] : Cleared old {_bus_tag_lower} (bus) cache file')
                except Exception as e:
                    print(f'[GPO-V2] : WARNING - Could not clear old cache files: {e}')
                _gpo_v2_cache_initialized = True
            
            # Search root: the given path itself (irrespective of how deeply nested its
            # sub-sessions/subfolders are). If a file was passed instead of a directory,
            # fall back to its immediate parent directory.
            if os.path.isdir(path):
                _search_root = path
            else:
                _search_root = os.path.dirname(path)
            
            print(f'[GPO-V2] : Input path: {path}')
            print(f'[GPO-V2] : Search root: {_search_root}')
            
            # Diagnostics: list immediate subdirectories under search root so we can see
            # whether ALL sibling test-session folders are visible/reachable from here.
            try:
                _top_level_dirs = sorted([d for d in os.listdir(_search_root) if os.path.isdir(os.path.join(_search_root, d))])
                print(f'[GPO-V2] : Found {len(_top_level_dirs)} top-level folder(s) under search root: {_top_level_dirs}')
            except Exception as e:
                print(f'[GPO-V2] : ERROR listing search root {_search_root}: {e}')
                _top_level_dirs = []
            
            _walk_errors = []
            def _gpo_v2_walk_onerror(err):
                _walk_errors.append(str(err))
            
            _mf4_count_by_topdir = {d: 0 for d in _top_level_dirs}
            
            # Walk from search root to find ALL b05/DEBUG and bus (b04/b02/BUS) files at ANY depth.
            # Some GPO-V2/Platform sessions use "_b05"/"_{bus_tag}" (b04/b02) file naming,
            # others use literal "_DEBUG_"/"_BUS_" naming (e.g. separate BUS/DEBUG/REF
            # subfolders) - support both so sessions using the DEBUG/BUS convention aren't
            # counted as 0 files and skipped.
            for _root, _dirs, _files in os.walk(_search_root, followlinks=True, onerror=_gpo_v2_walk_onerror):
                _rel = os.path.relpath(_root, _search_root)
                _top = _rel.split(os.sep)[0] if _rel != '.' else None
                for _fn in _files:
                    if _fn.endswith('.MF4') or _fn.endswith('.mf4'):
                        if _is_zero_segment_file(_fn): continue
                        if _top in _mf4_count_by_topdir:
                            _mf4_count_by_topdir[_top] += 1
                        _fn_lower = _fn.lower()
                        _fp = f'{_root}/{_fn}'
                        if '_b05' in _fn_lower and 'ORCAS' not in _fn:
                            _b05_cache.append(_fp)
                        elif f'_{_bus_tag_lower}' in _fn_lower:
                            _bus_cache.append(_fp)
                        elif '_debug_' in _fn_lower and 'orcas' not in _fn_lower and 'canoe' not in _fn_lower:
                            _b05_cache.append(_fp)
                        elif '_bus_' in _fn_lower:
                            _bus_cache.append(_fp)
            
            if _walk_errors:
                print(f'[GPO-V2] : WARNING - {len(_walk_errors)} error(s) occurred while walking {_search_root}:')
                for _e in _walk_errors[:10]:
                    print(f'  - {_e}')
            
            print(f'[GPO-V2] : .mf4 file count per top-level folder (any naming):')
            for _d, _c in _mf4_count_by_topdir.items():
                print(f'  - {_d}: {_c} .mf4 file(s)')
            
            # De-duplicate before anything else. A symlinked/looping folder structure
            # (common on irods-mounted paths, since os.walk(followlinks=True)) or this
            # same session/path being processed more than once in a single run can
            # otherwise make the same physical file show up twice for this one call,
            # which then propagates into duplicated SRR_DEBUG/BN_CALIFR entries in the JSON.
            _b05_cache = sorted(set(_b05_cache)); _bus_cache = sorted(set(_bus_cache))
            
            # Check if cache files already exist (multiple paths case)
            _b05_cache_path = f'{_jobout}/gpo_v2_b05_cache.txt'
            _bus_cache_path = f'{_jobout}/gpo_v2_bus_cache.txt'
            _b05_exists = os.path.isfile(_b05_cache_path)
            _bus_exists = os.path.isfile(_bus_cache_path)
            
            # Only append entries that aren't already present in the on-disk cache.
            # This makes the cache build idempotent: if this path/session ever gets
            # processed more than once (e.g. re-entrant call, retry), the same file
            # path won't be written into the shared cache twice.
            def _append_new_entries(cache_path, exists, entries):
                existing = set()
                if exists:
                    with open(cache_path, 'r') as _rf:
                        existing = {line.strip() for line in _rf if line.strip()}
                new_entries = [e for e in entries if e not in existing]
                if new_entries:
                    with open(cache_path, 'a' if exists else 'w') as _f:
                        _f.write('\n'.join(new_entries) + '\n')
                return new_entries

            _append_new_entries(_b05_cache_path, _b05_exists, _b05_cache)
            _append_new_entries(_bus_cache_path, _bus_exists, _bus_cache)

            # Pre-cache g02 files too, read by the VIDEO step (converter SRR_REFERENCE)
            _cache_g02_files(_search_root, _jobout, 'gpo_v2_g02_cache.txt')
            _cache_v01_files(_search_root, _jobout, 'gpo_v2_v01_cache.txt')
            
            print(f'[GPO-V2] : Pre-cached {len(_b05_cache)} b05 files and {len(_bus_cache)} {_bus_tag_lower} (bus) files from {os.path.basename(path)}')
            if _b05_cache:
                # Show first file from each test session for verification
                _sessions = set()
                for _fp in _b05_cache:
                    _session = '/'.join(_fp.split('/')[-4:-1])
                    _sessions.add(_session)
                print(f'[GPO-V2] : Test sessions found: {len(_sessions)}')
                for _s in sorted(_sessions)[:5]:
                    print(f'  - {_s}')
            
            # Populate file lists for JSON creation from local collected cache
            # (Already contains only files from current path due to walk starting at path)
            deb = _b05_cache
            cbus = _bus_cache
            mpad = _b05_cache
            ref = []
            
            files = _b05_cache
        elif 'ifv600_' in simg_name:
            # ADCAM docker (ifv600_*.simg): only p01 MF4 files count as input logs.
            upuFlag = True
            all_files = getfiles(path, '', 1)
            files = [f for f in all_files if '_p01' in os.path.basename(f).lower()]
        elif 'mcip' in simg_name :
            # Pre-cache g02 files for MCIP, read by the VIDEO step (converter SRR_REFERENCE)
            _cache_g02_files(path, f'{input_parameter[7]}/jobout', 'mcip_g02_cache.txt')
            _cache_v01_files(path, f'{input_parameter[7]}/jobout', 'mcip_v01_cache.txt')
            # Pre-cache b05/bus files too: a batch can span multiple nested session
            # folders, so splitter_highPrio.sh must read the full recursive set from
            # a cache instead of scanning only the first file's own folder.
            _cache_tagged_files(path, f'{input_parameter[7]}/jobout', 'mcip_b05_cache.txt', '_b05')
            _cache_tagged_files(path, f'{input_parameter[7]}/jobout', 'mcip_bus_cache.txt', f'_{bus_tag}')
            # Collect both DEBUG (_b05) and BUS files (respecting bus_tag parameter for b02/b04)
            deb = getfiles(path, '_b05', 1)
            cbus = getfiles(path, f'_{bus_tag}', 1)
            # For MCIP paired logging: use only b05 files as primary, shell script will find corresponding bus files
            files = deb
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
            # Fallback source for VIDEO when a session has no g02 files at all
            _cache_v01_files(_search_root, _jobout, 'dgps_v01_cache.txt')
            
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
    print(f"[DEBUG] : Total files found for session: {cnt}")
    #print(cnt, files)

    if cnt == 0:
        print(f"[WARN] : No input logs found for session {path}; skipping this session")
        return

    # Group files by test session (parent directory hierarchy) to avoid mixing sessions in one batch
    files_by_session = {}
    for file_path in files:
        # Extract test session identifier: go up 2 levels from file (file -> 0000 -> CEER_PT037...)
        test_session_dir = os.path.dirname(os.path.dirname(file_path))
        if test_session_dir not in files_by_session:
            files_by_session[test_session_dir] = []
        files_by_session[test_session_dir].append(file_path)
    
    #print(f"[DEBUG] : Files grouped into {len(files_by_session)} test sessions")
    #for session_dir, session_files in files_by_session.items():
    #    print(f"[DEBUG] : {os.path.basename(session_dir)}: {len(session_files)} files")
    
    # Batch within each session: singleSession files per batch regardless of subfolder
    # (Folder structure is preserved via individual file paths in the JSON)
    for session_dir in sorted(files_by_session.keys()):
        session_files = files_by_session[session_dir]
        session_cnt = len(session_files)

        if session_cnt % singleSession == 0:
            subset = (session_cnt // singleSession)
        else:
            subset = (session_cnt // singleSession) + 1

        print(f"[DEBUG] : Session {os.path.basename(session_dir)}: Creating {subset} batch entries ({session_cnt} files, {singleSession} per batch)")

        for i in range(0, subset):
            if i == 0:
                if subset == 1:
                    newpath = f"{session_files[i]},{session_cnt},{session_dir}"
                else:
                    newpath = f"{session_files[i]},{singleSession},{session_dir}"
            elif i == subset - 1:
                newpath = f"{session_files[singleSession*i]},{session_cnt-singleSession*i},{session_dir}"
            else:
                newpath = f"{session_files[singleSession*i]},{singleSession},{session_dir}"

            input_file_list.append(newpath)
            batch_file_index = singleSession*i if i > 0 else 0
            batch_file_count = singleSession if i < subset-1 else (session_cnt - batch_file_index)
            print(f"[DEBUG] : Batch {i} of {os.path.basename(session_dir)}: starting from files[{batch_file_index}] ({os.path.basename(session_files[batch_file_index])}), processing {batch_file_count} files")

        print(f"[INFO] : collecting files information : {curr_cnt}/{sessions}      Sub-Sessions : {len(input_file_list)}", end='\r')
    
    input_parameter[9] = upuFlag
