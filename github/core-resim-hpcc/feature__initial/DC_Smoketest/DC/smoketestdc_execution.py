# -*- coding: utf-8 -*-
"""
Created on Created on Wed Dec 10 08:38:57 2025

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""
from smoketestdc_dynamicimport import *



#***** function to validate inputs *****
def validateargv():
    global input_parameters, loginfo, tags
    inputs=[]
    def readxml(xmlpath):
        try:
            tagtree = ET.parse(xmlpath)
            root = tagtree.getroot()
            for child in root:
                subtag={}
                for sub in child:
                    subtag[sub.tag] = sub.text
                if child.tag not in tags:
                    tags[child.tag] = []
                tags[child.tag].append(subtag)
            for key in tags: tags[key]=tags[key][0]
            return tags['Execution_Request'].values()
        except Exception as e:
            print(f"[M-ERROR] : Error reading XML file: {e}")
            sys.exit(1)

    if len(sys.argv) == 2:
        if sys.argv[1].endswith('.xml'): 
            if os.path.exists(sys.argv[1]): inputs = readxml(sys.argv[1])
            else:print(f"[M-ERROR] : The provided path '{sys.argv[1]}' does not exist. Please check the path and try again."); sys.exit(1)
        else: print(f"[M-ERROR] : Argument {sys.argv[1]} must have '.xml' extension."); sys.exit(1)
    
    if len(sys.argv) == 5: 
        for i in sys.argv[1:5]:inputs.append(i)

    if len(sys.argv) != 2 and len(sys.argv) != 5:
        print(f"[M-ERROR] : Invalid number of arguments. Please provide exactly 1 or 4 argument.")
        print("[M-INFO] : with argument 1 - smoketest_dc.sh <dcSmoketestTest.xml>.")
        print("[M-INFO] : with argument 4 - smoketest_dc.sh <dc_simg.simg> <sil_engine_config.xml> <input.txt> <python_script.py>.")
        print("         : Please provide absolute path for each argument.")
        print("         : Exiting Application, re-trigger with above mentioned parameters.")
        sys.exit(1)
    else:
        expected_extensions = ['.simg', '.xml', '.txt', '.py']           
        
        for index, mpath in enumerate(inputs):
            #print(mpath)
            if not os.path.exists(mpath):
                print(f"[M-ERROR] : The provided path '{mpath}' does not exist. Please check the path and try again.")
                sys.exit(1)
            if not mpath.endswith(expected_extensions[index]):
                print(f"[M-ERROR] : Argument {index + 1} must have '{expected_extensions[index]}' extension. Received: '{mpath}'")
                sys.exit(1)
            if index == 2:
                with open(mpath, 'r') as f:
                    lines = f.readlines()
                    logname=lines[0].split('\n')[0]
                    loginfo = f'''{logname},{len(lines)}'''                   
            input_parameters.append(mpath)
        #print(f"[M-INFO] : Input Parameters are : {input_parameters}")

        

#***** function to get current date and time *****
def get_date_time():
    datentime = str(datetime.datetime.now())
    date = datentime.split(' ')[0]
    time = datentime.split(' ')[1].replace(':','-').replace('.','-')
    return date,time


#***** function to get path for jobout and output *****
def get_path():
    global input_parameters
    current_dir = os.getcwd()
    path_to_pick = current_dir.split(server)
    pre_path= path_to_pick[0]+server
    #print(server,project)

    user = getpass.getuser()
    date, time = get_date_time()

    path = f"{pre_path}{project}/2-Sim/USER_DATA/{user}/RESULTS-RESIM/{date}/{time}"
    jobout = f"{path}/jobout"
    output = f"{path}/output"

    os.makedirs(jobout)
    os.makedirs(output)
    input_parameters.append(project)
    input_parameters.append(path)
    return jobout

#***** function to create sbatch header for mining jobs *****
def get_content(requirements,script_type):
    content = f'''#!/bin/bash
#SBATCH --job-name={requirements[0]}           
#SBATCH --nodes={requirements[1]} 
#SBATCH --ntasks={requirements[2]}               
#SBATCH --cpus-per-task={requirements[3]}  
#SBATCH --mem={requirements[4]}G
#SBATCH --time={requirements[5]} 
#SBATCH -o 
#SBATCH -e

'''
    if script_type == '1':
        content += '''
SIMG="$1"
INPUT_TXT="$2"
OUT="$3"
PROJ="$4"

OUTPUT="$OUT/output"
JOBOUT="$OUT/jobout"
mkdir -p $JOBOUT
mkdir -p "$OUTPUT/${SLURM_ARRAY_TASK_ID}"

FW_FILE="$JOBOUT/SIL_Engine_Config_${SLURM_ARRAY_TASK_ID}.xml"

#********** section to start simg execution
echo "[Splitter_Execution] : execution requested for - '''+f'''{loginfo}"'''+'''

SIMG_START=$SECONDS
echo "[Pipeline_Execution] : starts at $PIPE_START seconds"
echo "[SMOKETEST_Execution] : starts at $SIMG_START seconds"

#********** section to add resim simg execution
module load singularity/3.8.0
echo "$JOBOUT/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out" >> $OUTPUT/.mining.txt
echo "singularity exec $SIMG /RUN_RESIM.sh $INPUT_TXT $OUTPUT/${SLURM_ARRAY_TASK_ID} $FW_FILE"
singularity exec $SIMG /RUN_RESIM.sh $INPUT_TXT $OUTPUT/${SLURM_ARRAY_TASK_ID} $FW_FILE

SIMG_END=$SECONDS
echo "[SMOKETEST_Execution] : ends at $SIMG_END seconds"
echo "[SMOKETEST_Execution] : total execution time is $(( $SIMG_END - $SIMG_START ))"
echo "[Pipeline_Execution] : total pipeline execution time is $(( $SIMG_END - $SIMG_START ))"

#********** section to end simg execution
'''

    elif script_type == '2':
        content += f'''
{resimMining} $@
'''
    return content


#***** function to create sbatch mining script *****
def sbatch_mining(scriptfile,script,requirement=[]):
    with open(scriptfile,'w') as f:
        for line in script.split('\n'):
            if "#SBATCH -o" in line:
                f.write(f"{requirement[0]}\n")
            elif "#SBATCH -e" in line:
                f.write(f"{requirement[1]}\n")
            else:
                f.write(f'{line}\n')
    os.chmod(scriptfile, 0o755)


#***** function to create execution scripts *****
def create_execution_scripts():
    jobout = get_path()
    r_out  = f"#SBATCH -o {jobout}/%A_%a.out"
    r_err  = f"#SBATCH -e {jobout}/%A_%a.out"
    rm_out = f"#SBATCH -o {jobout}/Resim_Mining.out"
    rm_err = f"#SBATCH -e {jobout}/Resim_Mining.out"

    script=get_content(['dc_smoke',1,1,6,64,"03:00:00"],'1')
    sbatch_mining(f'{jobout}/.smoketest.sh',script,[r_out,r_err])
    script=get_content(['dc_min',1,1,1,4,"03:00:00"],'2')
    sbatch_mining(f"{jobout}/.rming.sh",script,[rm_out,rm_err])
    script=script_master()
    sbatch_mining(f"{jobout}/.rmain.sh",script,)

    return jobout


