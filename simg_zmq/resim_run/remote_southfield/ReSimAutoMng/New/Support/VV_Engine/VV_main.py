import os, sys, datetime, getpass

base_out = "#SBATCH -o"
base_err = "#SBATCH -e"
version = "1"

def resim_vv_mining_child(script):
    return '''#!/bin/bash
#SBATCH --job-name=VV_Min
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1 
#SBATCH --mem=32G
#SBATCH --time=01:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log

'''+f"{script}"+''' $1
echo " "
rm "$2/.resimvv.sh"
rm "$2/.rvvming.sh"
'''

aptiv = f'''
\t************************************************************************
\t*           ___       ______  _________ _________ __          __       *
\t*          / _ \     |  __  | \__   __/ \__   __/ \ \        / /       *
\t*         / / \ \    | |__| |    | |       | |     \ \      / /        *
\t*        / /___\ \   |  ____|    | |       | |      \ \    / /         *
\t*       / ______\ \  | |         | |     __| |__     \ \__/ /          *
\t*      /_/       \_\ |_|         |_|    /_______\     \____/           *
\t*                                                                      *
\t************************************************************************
\t*                                                                      *
\t*          ReSET:Resim Singularity Execution Pipeline Tool             *
\t*                                                                      *
\t************************************************************************
\t************************************************************************
\t* version: {version}                   Help : mandeep.singh1@aptiv.com       *
\t************************************************************************
****************************************************************************************'''


def resim_vv_child_script():
    return  '''#!/bin/bash
#SBATCH --job-name=rVV
#SBATCH --nodes=1
#SBATCH --ntasks=1               
#SBATCH --cpus-per-task=6  
#SBATCH --mem=32G
#SBATCH --time=01:30:00
#SBATCH -o /net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA/z5daa9/work/VV_new/playground/jobout/VV/%A_%a.out
#SBATCH -e /net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA/z5daa9/work/VV_new/playground/jobout/VV/%A_%a.out

PIPE_START=$SECONDS
echo "[Pipeline_Execution] : starts at $PIPE_START"

ARRAY_START="$1"
SIMG_PATH="$2"
RESULT_OUT="$3/output"
RESULT_JOB="$3/jobout"

# Start application
echo "______________ Start of the application ______________"
LINE=$(( $SLURM_ARRAY_TASK_ID + $ARRAY_START ))
echo "LINE_NUMBER= $LINE"
INPUT_TXT=$(sed -n ${LINE}p "${RESULT_JOB}/SIL_Input_All.txt")
echo ${INPUT_TXT}

LOG=$(head -n 1 "$INPUT_TXT")
echo $LOG
#Log_count=$(wc <"$INPUT_PATH" -l)
#echo "$First_log, $Log_count" >> "${RESULT_JOB}"/SIL_input_session.txt

DIR=$(dirname "${LOG}")
FOLDER_STRUCT=${DIR##*"gzvcb3/"}
mkdir -p $RESULT_OUT/$FOLDER_STRUCT

LOG_NAME=$(sed -n ${LINE}p "${RESULT_JOB}/SIL_input_session.txt")
echo "[Splitter_Execution] : execution requested for - $LOG_NAME"

#mkdir -p $RESULT_OUT/VV/"${SLURM_ARRAY_TASK_ID}"

TEMPDIR=$(mktemp -d --tmpdir=/dev/shm/ --suffix=".${SLURM_JOB_ID}_${SLURM_ARRAY_TASK_ID}" )
trap "rm -rf $TEMPDIR; echo \\"Removing ${TEMPDIR} \\"; exit "  SIGINT SIGTERM
if [ $? -ne 0 ]
then
        echo Failed to create TEMPDIR: $TEMPDIR, with errno 0
        exit 1
else
        echo "TEMPDIR is: $TEMPDIR"
        echo "Running ls -d on TEMPDIR"
        ls -d "$TEMPDIR"
        RET=$?
        if [ $RET -ne 0 ]
        then
                echo "Trying to create directory with direct mkdir - 1st time failed mktemp failed?"
                mkdir -p "$TEMPDIR"
                ls -l "$TEMPDIR"
        else
                echo "ls returned: $RET"
        fi
fi

module load singularity/3.8.0
SIMG_START=$SECONDS
echo "[Resim_Execution] : starts at $SIMG_START" 
echo $SIMG_PATH $INPUT_TXT $TEMPDIR $CONFIG_PATH
echo "$RESULT_JOB/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out" >> $RESULT_OUT/.mining.txt
singularity exec $SIMG_PATH /RUN_VV.sh $INPUT_TXT $TEMPDIR
SIMG_END=$SECONDS
echo "[Resim_Execution] : ends at $SIMG_END" 
echo "[Resim_Execution] : total resim execution time is $(( $SIMG_END - $SIMG_START ))"

echo "moving from temp to output location"
cp -r $TEMPDIR/* $RESULT_OUT/$FOLDER_STRUCT
sleep 5 
echo "moved from temp to output location"
rm -rf ${TEMPDIR}
echo "removed tempdir"

rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_track.txt
echo "[Pipeline_Execution] : total pipeline execution time is $(( $SECONDS - $PIPE_START ))"
'''

def get_input():
    error = False

    try:
        inputxt = sys.argv[1]
        if ".txt" not in inputxt: print("[ERROR] : Passed file is not a txt file"); error=True
    except: print("[ERROR] : txt file not passed"); error=True
    
    try:
        simg = sys.argv[2]
        if ".simg" not in simg: print("[ERROR] : Passed file is not a singularity file"); error=True
    except : print("[ERROR] : Singularity image not passed"); error=True
    

    if error : sys.exit()

    return [simg, inputxt]
    

def childCreation(input_parameter):
    def createchildfile(file, lines, msg, err):
        with open(file, 'w') as file:
            for line in lines:
                if base_out in line:
                    file.write(f"{msg}\n")
                elif base_err in line:
                    file.write(f"{err}\n")
                else:
                    file.write(f'{line}\n')
                    
    hpcc = os.getcwd()
    if 'projects' in hpcc:
        cluster="Southfield"
        server="projects/"
        script = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Shell/rResimVV_main.sh"
        mining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/PostProcessing/mining_vv/VV_mining.py"
    else:
        cluster = "Krakow"
        server = "PROJECTS/"
        script = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Shell/rResimVV_main.sh"
        mining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/PostProcessing/mining_vv/VV_mining.py"

    input_parameter.append(script)
    input_parameter.append(mining)
    pathprtn=hpcc.split(server)
    basepath=pathprtn[0] + f'{server}' + pathprtn[1].split('/')[0]
    crruser=getpass.getuser()
    date = (str(datetime.datetime.now())).split(' ')[0]
    time = (str(datetime.datetime.now())).split(' ')[1].replace(':','-').replace('.','-')
    executionPath = f"{basepath}/2-Sim/USER_DATA/{crruser}/RESULTS-RESIM_VV/{date}/{time}"

    jobout_path = f"{executionPath}/jobout"
    output_path = f"{executionPath}/output"
    new_out_resim = f"#SBATCH -o {jobout_path}/%A_%a.out"
    new_err_resim = f"#SBATCH -e {jobout_path}/%A_%a.out"
    new_out_rMin = f"#SBATCH -o {jobout_path}/Resim_Mining.out"
    new_err_rMin = f"#SBATCH -e {jobout_path}/Resim_Mining.out"

    os.makedirs(jobout_path)
    os.makedirs(output_path)

    resim_vv_child = resim_vv_child_script().split('\n')
    resim_vv_mining = resim_vv_mining_child(input_parameter[3]).split('\n')

    createchildfile(f'{jobout_path}/.resimvv.sh', resim_vv_child, new_out_resim, new_err_resim)
    createchildfile(f'{jobout_path}/.rvvming.sh', resim_vv_mining, new_out_rMin, new_err_rMin)

    print('[INFO] : configured Resim Scripts')
    input_parameter.append(executionPath)



    

"""********************************************************************************* main section *********************************************************************************"""
##############################################################
#
# main function
# flow :
#       create child .sh
#
##############################################################

if __name__=='__main__':
    input_parameter=get_input()
    
    childCreation(input_parameter)
    #print(input_parameter)
    os.system(f"{input_parameter[2]} {input_parameter}")
    
    


