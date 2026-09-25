from dq_input_base import *
from dq_input_static import converterSimg, converterConfig


#**********************************************************
#          function to create slurm script to get resource and run job
#**********************************************************
def create_QualityChecker_script():
    print('[INFO] : configuring QualityChecker Script', end='\r')
    if "projects" in os.getcwd() :
        server = "projects/"
    elif "PROJECTS" in os.getcwd() :
        server = "PROJECTS/"
    else:
        server="NOT defined"
    path_to_pick = os.getcwd().split(server)
    pre_path= path_to_pick[0]+server
    project= path_to_pick[1].split('/')[0]
    user = getpass.getuser()
    datentime = str(datetime.datetime.now())
    date = datentime.split(' ')[0]
    time = datentime.split(' ')[1].replace(':','-').replace('.','-')
    path = f"{pre_path}{project}/2-Sim/USER_DATA/{user}/RESULTS-TOOL/{date}/{time}"
    jobout_path = f"{path}/jobout"
    output_path = f"{path}/output"
    new_out = f"#SBATCH -o {jobout_path}/%A_%a.out"
    new_err = f"#SBATCH -e {jobout_path}/%A_%a.out"
    new_out_dq = f"#SBATCH -o {jobout_path}/dq_mining.out"
    new_err_dq = f"#SBATCH -e {jobout_path}/dq_mining.out"

    os.makedirs(jobout_path)
    os.makedirs(output_path)
    
    base_child_data = base_child.split('\n')
    file = open(f"{jobout_path}/.QualityChecker.sh",'w')
    for line in base_child_data:
        if base_out in line:
            file.write(f"{new_out}\n")
        elif base_err in line:
            file.write(f"{new_err}\n")
        else:
            file.write(f'{line}\n')
    file.close()

    file=open(sys.argv[1], 'r')
    sessions=file.readlines()
    file.close()
    
    '''for i in range(1,len(sessions)+1):
        os.makedirs(f'{jobout_path}/{i}')'''

    
    file = open(f"{jobout_path}/.Log_QualityChecker_Mining.sh",'w')
    base_extractor_data = base_exectractor.split('\n')
    for line in base_extractor_data:
        if base_out in line:
            file.write(f"{new_out_dq}\n")
        elif base_err in line:
            file.write(f"{new_err_dq}\n")
        else:
            file.write(f'{line}\n')
    file.close()

    # Write dedicated converter SLURM script
    new_out_conv = f"#SBATCH -o {jobout_path}/%A_%a.out"
    new_err_conv = f"#SBATCH -e {jobout_path}/%A_%a.out"
    file = open(f"{jobout_path}/.Converter.sh", 'w')
    for line in base_converter.split('\n'):
        if base_out in line:
            file.write(f"{new_out_conv}\n")
        elif base_err in line:
            file.write(f"{new_err_conv}\n")
        else:
            file.write(f'{line}\n')
    file.close()

    print('[INFO] : configured  QualityChecker Script')
    return path


#**********************************************************
#          main function to trigger Quality Tool
#**********************************************************
def log(msg, log_file):
    print(msg)
    log_file.write(msg + '\n')
    log_file.flush()


if __name__ == '__main__':
    inputs_parameters = validate_inputs()
    print(aptiv)
    job_path = create_QualityChecker_script()

    log_path = f"{job_path}/jobout/dq_setup.out"
    try:
        lf = open(log_path, 'w')
    except Exception as e:
        # fallback: write next to the input txt file in cwd
        log_path = os.path.join(os.getcwd(), 'dq_setup.out')
        print(f'[WARNING] : could not write to {job_path}/jobout/, logging to {log_path} : {e}')
        lf = open(log_path, 'w')

    log(f'[SETUP] : ===== DQ Pipeline Setup Started =====', lf)
    log(f'[SETUP] : job_path       = {job_path}', lf)
    log(f'[SETUP] : input_list     = {inputs_parameters[3]}', lf)
    log(f'[SETUP] : simg           = {inputs_parameters[1]}', lf)
    log(f'[SETUP] : config         = {inputs_parameters[2]}', lf)

    file = open(inputs_parameters[3], 'r')
    sessions = file.readlines()
    file.close()
    tmp_cnt=0
    for path in sessions:
        path=path.split('\n')[0]
        if path[-1] == '/':
            path = path[:-1]
        sessions[tmp_cnt]=path
        tmp_cnt+=1

    log(f'[SETUP] : total sessions = {len(sessions)}', lf)

    for path in sessions:
        deb = []
        json_paths = []
        cnt = sessions.index(path)
        path=path.split('\n')[0]
        log(f'[SETUP] : ----- processing session {cnt+1}/{len(sessions)} : {path}', lf)
        deb = get_deb_files(path)
        if deb:
            log(f'[SETUP] : [MF4]  found {len(deb)} .mf4 file(s)', lf)
        if not deb:
            log(f'[SETUP] : [MF4]  no .mf4 files found, checking for .brr files', lf)
            deb = get_brr_files(path, job_path)
            if deb:
                log(f'[SETUP] : [BRR]  found {len(deb)} .brr file(s), converter will run on compute node', lf)
            else:
                log(f'[WARNING] : no .mf4 or .brr files found in {path}, skipping', lf)
                continue
        json_paths = create_fList(job_path, deb, cnt+1, len(sessions))
        log(f'[SETUP] : fList written → {json_paths[-1]}', lf)

    log(f'[SETUP] : created .txt/.json for {cnt+1}/{len(sessions)}', lf)
    input_file = create_input_fList(job_path)
    log(f'[SETUP] : master fList  → {input_file}', lf)
    log(f'[SETUP] : OUTPUT_PATH   = {job_path}', lf)
    log(f'[SETUP] : submitting SLURM job...', lf)
    os.system(f"{inputs_parameters[0]} {inputs_parameters[1]} {inputs_parameters[2]} {input_file} {job_path} {converterSimg} {converterConfig}")
    log(f'[SETUP] : ===== DQ Pipeline Setup Completed =====', lf)
    lf.close()




"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
22/08/2024          Mandeep Singh       FHW-268     splitter for STLA_SCALE1 DQ(created 1st version of file )

######################################################################################################
"""
