# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

###########################################################
from resim_child import *
###########################################################

#**********************************************************
#          function to get user inputs
#**********************************************************
def userInteraction():
    def display_menu():
        print(menu_open)
        cnt=1
        for option in menu_options:
            print(f"*   {cnt}".ljust(9,' ')+f"| {option}".ljust(42,' ')+"*");cnt+=1
        print(menu_close)

    message=''
    config=''
    global input_parameter
    mode='Default'
    action = input("[INFO] : Proceed with Default Docker Configuration (Y/N): ")
    if action.lower() != 'y': 
        display_menu(); selection = int(input("Enter Choice : ")); print(f"[INFO] : Selected: {mode}")
        try    : mode = menu_options[selection-1]
        except : print('[ERROR] : Invalid selection, exiting');sys.exit()

        if selection in range(1,len(menu_options)):
            config = input(f"Provide FW Config File for {mode} mode : ")
            if not '.xml' in config:message=f"[ERROR] : {config} is not in .xml format. "
            elif not os.path.isfile(config):message=f"[ERROR] : {config} is not a valid file."

        if len(message)>1: print(message, "Exiting Application"); sys.exit()
    input_parameter.append(mode)
    input_parameter.append(config)
    print(aptiv)


#**********************************************************
#          function to validate inputs provided
#**********************************************************
def validate_argv():
    message=''
    global input_parameter, highPrio
    print('[INFO]  : Inputs - Validating ', end='\r')
    if len(sys.argv) < 3:


        message="""[ERROR] : Inputs Validation :- short of needed argument
        : Expected INPUT format : rResim.sh <input_list : mandatory> <Resim singularity : mandatory>
[INFO]  : Above parameters should be passed with full path."""
    else:
        if not '.txt' in sys.argv[1]:message=f"[ERROR] : {sys.argv[1]} is not in .txt format. "
        elif not os.path.isfile(sys.argv[1]):message=f"[ERROR] : {sys.argv[1]} does not exist."

        if not '.simg' in sys.argv[2]:message=f"[ERROR] : {sys.argv[2]} is not singularity image."
        elif not os.path.isfile(sys.argv[2]):message=f"[ERROR] : {sys.argv[2]} does not exist."
          
    print('[INFO] : Inputs - Validated  ')
    if len(message)>1: print(message, "Exiting Application"); sys.exit()
    
    if 'small' in sys.argv[2] : cust = 'stla_small'; PROJECT='STLA-SMALL'
    elif 'mcip' in sys.argv[2] : cust = 'stla_mcip'; PROJECT='STLA-THUNDER'
    elif 'ceer' in sys.argv[2] : cust = 'ceer'; PROJECT='CEER-PROGRAM'
    else : 
        if cluster == 'Southfield': cust = 'Platform'; PROJECT='GPO-IFV7XX'
        else : cust = 'Platform'; PROJECT='CEER-PROGRAM'

    # Helios uses a dedicated SLURM account (8k3p89) regardless of the
    # singularity image based routing above. Detected by username since
    # 'cluster' is now shared ("Helios/Krakow") after the codebase merge.
    if cluster == 'Helios/Krakow' and getpass.getuser().lower().startswith('8k3'):
        PROJECT = '8k3p89'

    input_parameter.append(sys.argv[1])
    input_parameter.append(sys.argv[2])
    input_parameter.append(cust)
    input_parameter.append(PROJECT)
    userInteraction()


#**********************************************************
#          function to create slurm script to get resource and run job
#**********************************************************
def childCreation():
    def createchildfile(file, lines, msg, err, mem_line=None):
        with open(file, 'w') as file:
            for line in lines:
                if base_out in line:
                    file.write(f"{msg}\n")
                elif base_err in line:
                    file.write(f"{err}\n")
                elif mem_line and base_mem in line:
                    file.write(f"{mem_line}\n")
                else:
                    file.write(f'{line}\n')

    global input_parameter
    pathprtn=os.getcwd().split(server)
    basepath=pathprtn[0] + f'{server}' + pathprtn[1].split('/')[0]
    crruser=getpass.getuser()
    date = (str(datetime.datetime.now())).split(' ')[0]
    time = (str(datetime.datetime.now())).split(' ')[1].replace(':','-').replace('.','-')
    executionPath = f"{basepath}/2-Sim/USER_DATA/{crruser}/RESULTS-RESIM/{date}/{time}"

    jobout_path = f"{executionPath}/jobout"
    output_path = f"{executionPath}/output"
    new_out_resim = f"#SBATCH -o {jobout_path}/%a/%A_%a.out"
    new_err_resim = f"#SBATCH -e {jobout_path}/%a/%A_%a.out"
    new_out_rMin = f"#SBATCH -o {jobout_path}/Resim_Mining.out"
    new_err_rMin = f"#SBATCH -e {jobout_path}/Resim_Mining.out"
    new_out_sMin = f"#SBATCH -o {jobout_path}/Stats_Mining.out"
    new_err_sMin = f"#SBATCH -e {jobout_path}/Stats_Mining.out"

    os.makedirs(jobout_path)
    os.makedirs(output_path)

    if highPrio:resim_child = resim_child_script_highPrio().split('\n')
    else: resim_child = resim_child_script().split('\n')
    resim_mining = resim_mining_child.split('\n')
    stats_mining = stats_mining_child.split('\n')

    # Gen8 logs need more memory per task than the templated default.
    simg_name = os.path.basename(input_parameter[2]).lower()
    gen8_mem = "#SBATCH --mem=64G" if 'resim_gen8' in simg_name else None

    createchildfile(f'{jobout_path}/.resim.sh', resim_child, new_out_resim, new_err_resim, gen8_mem)
    createchildfile(f'{jobout_path}/.rming.sh', resim_mining, new_out_rMin, new_err_rMin)
    createchildfile(f'{jobout_path}/.sming.sh', stats_mining, new_out_sMin, new_err_sMin)

    print('[INFO] : configured Resim Scripts')
    input_parameter.append(executionPath)
    if highPrio : input_parameter.append("True")
    else : input_parameter.append("False")



#**********************************************************
#          function to detect if path contains MF4 files (direct level only)
#**********************************************************
def contains_mf4_files_direct(path):
    """Check if a path directly contains MF4 data files (non-recursive)"""
    if not os.path.exists(path):
        return False
    try:
        for item in os.listdir(path):
            if item.endswith('.MF4') or item.endswith('.mf4'):
                return True
    except (OSError, PermissionError):
        pass
    return False


#**********************************************************
#          function to detect if path contains MF4 files (any level)
#**********************************************************
def contains_mf4_files(path):
    """Check if a path or its subdirectories contain MF4 data files (recursive)"""
    if not os.path.exists(path):
        return False
    try:
        for root, dirs, files in os.walk(path, followlinks=True):
            for file in files:
                if file.endswith('.MF4') or file.endswith('.mf4'):
                    return True
    except (OSError, PermissionError):
        pass
    return False


#**********************************************************
#          function to detect nested session folders
#**********************************************************
def detect_nested_sessions(parent_path):
    """
    Auto-detect nested session folders inside a given path.
    Enabled for Southfield, Krakow and Helios clusters.
    Distinguishes between:
    - Data segment folders (0000-0034, etc.) → part of single session, return []
    - True nested sessions (different identifiers) → return list of nested paths
    Returns list of nested session paths if found, empty list otherwise.
    """
    if cluster not in ['Southfield', 'Krakow', 'Helios']:
        return []
    
    if not os.path.isdir(parent_path):
        return []
    
    nested_sessions = []
    
    # Check if parent path itself directly contains MF4 files (not in subdirs)
    if contains_mf4_files_direct(parent_path):
        return []  # Parent is a session itself, no nested detection needed
    
    # Look for subdirectories that contain MF4 files (one level down)
    try:
        candidates = []
        for item in os.listdir(parent_path):
            item_path = os.path.join(parent_path, item)
            if os.path.isdir(item_path) and contains_mf4_files(item_path):
                candidates.append(item)
        
        if not candidates:
            return []
        
        # Check if candidates are data segment folders (numbered sequentially like 0000-0034)
        # If ALL are 4-digit zero-padded numbers, they're data segments, not nested sessions
        all_numeric_segments = True
        for candidate in candidates:
            if not (len(candidate) == 4 and candidate.isdigit()):
                all_numeric_segments = False
                break
        
        if all_numeric_segments:
            # These are data segment folders (0000, 0001, ..., 0034), not nested sessions
            # Return empty list to treat parent as a single session
            print(f"[INFO] : {os.path.basename(parent_path)}: Detected {len(candidates)} data segment folders (0000-style), treating as single session")
            return []
        
        # Not data segments - these are true nested sessions
        for item in candidates:
            nested_sessions.append(os.path.join(parent_path, item))
            
    except (OSError, PermissionError) as e:
        print(f"[WARNING] : Unable to scan {parent_path}: {e}")
        return []
    
    return sorted(nested_sessions)


#**********************************************************
#          function to create input file with nested sessions
#**********************************************************
def create_nested_sessions_input(parent_path):
    """
    Create a new input.txt file with auto-detected nested session paths.
    Returns the path to the new input file.
    """
    nested_sessions = detect_nested_sessions(parent_path)
    
    if not nested_sessions:
        return None
    
    # Create new input file in a temporary location
    input_dir = os.path.dirname(input_parameter[1])
    new_input_file = os.path.join(input_dir, f"nested_sessions_input_{int(datetime.datetime.now().timestamp())}.txt")
    
    try:
        with open(new_input_file, 'w') as f:
            for session_path in nested_sessions:
                f.write(f"{session_path}\n")
        
        print(f"[INFO] : Auto-detected {len(nested_sessions)} nested session(s)")
        print(f"[INFO] : Created new input file: {new_input_file}")
        
        return new_input_file
    except (IOError, OSError) as e:
        print(f"[ERROR] : Failed to create nested sessions input file: {e}")
        return None


#**********************************************************
#          function to collect session from .txt file
#**********************************************************
def collectsessions():
    global sessions, input_parameter
    
    # Read the original input file
    with open(input_parameter[1],'r') as file:
        sets = file.readlines()
    
    # Process each path from input file
    # Use strip() (not just split('\n')[0]) so trailing '\r' from Windows/CRLF-edited
    # input.txt files (common when multiple session paths are pasted line-by-line)
    # is removed too. A leftover '\r' makes the path not exist on disk, which
    # silently drops every session (0 files found) and leads to an empty
    # SIL_Input_all.txt -> TASKS=0 -> "Invalid job array specification" on sbatch.
    original_sessions = []
    for path in sets:
        path = path.strip()
        if path and path[-1] == '/' : 
            path = path[:-1]
        if path:
            original_sessions.append(path)
    
    # For Krakow and Southfield: check each path for nested sessions
    expanded_sessions = []
    for parent_path in original_sessions:
        nested_sessions = detect_nested_sessions(parent_path)
        
        if nested_sessions:
            # Path contains nested sessions - DO NOT expand the list.
            # Instead, pass the parent path as-is to createFileList().
            # createFileList() will walk the entire directory tree, group files by
            # session and batch each session separately (guaranteed isolation).
            print(f"[INFO] : Auto-detected {len(nested_sessions)} nested session(s) in {os.path.basename(parent_path)}")
            expanded_sessions.append(parent_path)  # Keep parent path, not individual nested sessions
        else:
            # No nested sessions found - use parent path as is
            expanded_sessions.append(parent_path)
    
    # Add all sessions (original or expanded) to the global sessions list
    sessions.extend(expanded_sessions)
    
    """if 'resim_stla_small' in input_parameter[2]:
        sub_session = []
        for session in sessions:
            for root, dir, files in os.walk(session):
                for file in files:
                    print(file)
                    if ('.MF4' in file or '.mf4' in file) and 'debrad' in file:
                        sesnum=file.split('_')[-3]
                        if not any(sesnum in item for item in sub_session): sub_session.append(f'{root}/{file}')
                
        sessions.clear()
        sessions=sub_session"""
            
#**********************************************************
#          function to collect input logs for Cx
#**********************************************************
def collectInputLogs():
    session_cnt = 1
    for path in sessions:
        #if session_cnt == 6: session_cnt=1
        if highPrio : createFileList(path, session_cnt, len(sessions))
        else :  createJson(path, session_cnt, len(sessions))
        session_cnt += 1
    
    createInputFlist()
    #input_parameter.append(upuFlag)
    print(f"\n\nOutput : {input_parameter[7]}")
