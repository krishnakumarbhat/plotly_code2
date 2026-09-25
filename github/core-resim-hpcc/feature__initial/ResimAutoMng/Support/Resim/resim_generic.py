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
    elif 'ceer' in sys.argv[2] : cust = 'ceer'; PROJECT='GPO-IFV7XX'
    else : 
        if cluster == 'Southfield': cust = 'Platform'; PROJECT='GPO-IFV7XX'
        else : cust = 'Platform'; PROJECT='RNA-SDV-SRR7'

    input_parameter.append(sys.argv[1])
    input_parameter.append(sys.argv[2])
    input_parameter.append(cust)
    input_parameter.append(PROJECT)
    userInteraction()


#**********************************************************
#          function to create slurm script to get resource and run job
#**********************************************************
def childCreation():
    def createchildfile(file, lines, msg, err):
        with open(file, 'w') as file:
            for line in lines:
                if base_out in line:
                    file.write(f"{msg}\n")
                elif base_err in line:
                    file.write(f"{err}\n")
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
    
    createchildfile(f'{jobout_path}/.resim.sh', resim_child, new_out_resim, new_err_resim)
    createchildfile(f'{jobout_path}/.rming.sh', resim_mining, new_out_rMin, new_err_rMin)
    createchildfile(f'{jobout_path}/.sming.sh', stats_mining, new_out_sMin, new_err_sMin)

    print('[INFO] : configured Resim Scripts')
    input_parameter.append(executionPath)
    if highPrio : input_parameter.append("True")
    else : input_parameter.append("False")



#**********************************************************
#          function to collect session from .txt file
#**********************************************************
def collectsessions():
    global sessions
    with open(input_parameter[1],'r') as file:
        sets = file.readlines()
    for path in sets:
        path = path.split('\n')[0]
        if path[-1] == '/' : path = path[:-1]
        sessions.append(path)
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




"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
15/04/2025          Mandeep Singh       FHW-223     splitter for STLA_SCALE1 Resim(created 1st version of file )

######################################################################################################
"""