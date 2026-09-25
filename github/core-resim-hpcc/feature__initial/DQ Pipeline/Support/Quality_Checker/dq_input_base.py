from dq_splitter import *
from dq_input_static import *


version = '2.0'
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
\t*               ReSET:Resim Singularity Execution Tool                 *
\t*                                                                      *
\t************************************************************************
\t************************************************************************
\t* version: {version}                    Help : mandeep.singh@aptiv.com       *
\t************************************************************************
****************************************************************************************'''

base_out = "#SBATCH -o"
base_err = "#SBATCH -e"
base_child = '''#!/bin/bash
#SBATCH --job-name=RR_QualityChecker           
#SBATCH --nodes=1
#SBATCH --ntasks=1               
#SBATCH --cpus-per-task=4
#SBATCH --mem=64G
#SBATCH --time=16:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/z5daa9/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/z5daa9/playground/jobout/slurmout_%A_%a.log


ARRAY_START="$1"
SIMG_PATH="$2"
CONFIG_PATH="$3"
INPUT_LIST="$4"
RESULT_OUT="$5"

echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : ===== Task ${SLURM_ARRAY_TASK_ID} started ====="
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : SIMG_PATH   = $SIMG_PATH"
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : CONFIG_PATH = $CONFIG_PATH"
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : INPUT_LIST  = $INPUT_LIST"
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : RESULT_OUT  = $RESULT_OUT"

# Start application
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : ______________ Start of the application ______________"
LINE=$(( $SLURM_ARRAY_TASK_ID + $ARRAY_START ))
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : LINE_NUMBER = $LINE"
INPUTFILE=$(sed -n ${LINE}p "${INPUT_LIST}")
echo "[$(date '+%Y-%m-%d %H:%M:%S')] [CHILD] : INPUTFILE   = $INPUTFILE"

NEW_FOLDER=$(head -n 1 "$INPUTFILE")
echo $NEW_FOLDER
DIR="$(dirname $NEW_FOLDER)"
#echo ${OUTPUT_PATH}

file_name=${NEW_FOLDER##*/}
echo $dir_name
IFS="_" read -r -a logarr <<< "$file_name"

if [[ ${logarr[0]} == *'GT'* ]]; then
    veh="${logarr[0]}"
    IFS="T" read -r -a dt_tm <<<  "${logarr[1]}"
    dt="${dt_tm[0]}"
    tm="${dt_tm[1]}"
    child="${veh}/${dt}/${tm}"

elif [[ ${logarr[0]} == "TNDR1" || ${logarr[0]} == "TNRD1" ]]; then
    child="${logarr[1]}/${logarr[2]}/${logarr[3]}"

elif [[ $DIR == *'Honda'* ]]; then
    if [[ $DIR == *'Honda-Input'* ]]; then
        child=${DIR##*"Honda-Input/"}
    else
        if [[ ${#logarr[@]} == 9 ]]; then   
            child="HONDA_SRR6P"/"${logarr[3]}"/"${logarr[7]}"
        elif [[ ${#logarr[@]} == 7 ]]; then
            child="HONDA_SRR6P"/"${logarr[3]}"/"${SLURM_ARRAY_TASK_ID}"
        fi
    fi

elif [[ ${logarr[0]} == "TRATON" ]]; then
    child="${logarr[0]}/${logarr[3]}/${logarr[6]}"

elif [[ $FILE_BASENAME == *'KPI_MRR_DS'* || $FILE_BASENAME == *'KPI_SRR_DS'* ]]; then
    dt="VEH"
    tm="${logarr[-2]}"
    echo "$tm"
    if [[ $FILE_BASENAME == *'SRR_DS'* ]]; then
        veh="SRR"
    elif [[ $FILE_BASENAME == *'MRR_DS'* ]]; then
        veh="MRR"
    fi
    if [[ $SIMG_PATH == *'dgps'* ]]; then
        dt="DGPS"      
    fi
    child="${veh}/${dt}/${tm}"

else
    child="OUT_RESULT/out/${SLURM_ARRAY_TASK_ID}"
fi

mkdir -p $RESULT_OUT/"$child"

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

# ============ Use pre-converted MF4 from dedicated converter job ============
CONV_FLIST="$RESULT_OUT/jobout/conv_fList_${SLURM_ARRAY_TASK_ID}.txt"
if [ -f "$CONV_FLIST" ]; then
    MF4_FOLDER=$(head -n 1 "$CONV_FLIST")
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [DQ] : BRR session - MF4 folder = $MF4_FOLDER"
    # Build a proper file-list from the folder (mirrors dq_splitter.get_deb_files)
    MF4_FLIST="$RESULT_OUT/jobout/mf4_fList_${SLURM_ARRAY_TASK_ID}.txt"
    find "$MF4_FOLDER" -type f \( -name "*.mf4" -o -name "*.MF4" \) | sort > "$MF4_FLIST"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [DQ] : $(wc -l < $MF4_FLIST) .mf4 file(s) listed in $MF4_FLIST"
    INPUTFILE=$MF4_FLIST
fi
# ============ end converter check ============

#singularity exec $SIMG_PATH /RUN_MUDP.sh $CONFIG_PATH $INPUTFILE  $RESULT_OUT/"${newarr[1]}"/"${newarr[2]}"/"${newarr[3]}" #>> $OUTPUT_PATH/"jobout_$LINE.txt"
singularity exec $SIMG_PATH /RUN_MUDP.sh $CONFIG_PATH $INPUTFILE  $TEMPDIR #>> $OUTPUT_PATH/jobout_$LINE.txt

echo $RESULT_OUT/"$child"/Data_Logging_Quality.xml >> $RESULT_OUT/DQ_xml_fList.txt 


echo "moving from temp to output location"
mv -v $TEMPDIR/* $RESULT_OUT/"$child"


echo "moved from temp to output location"
rm -rf ${TEMPDIR}
echo "removed tempdir"
'''


base_converter = '''#!/bin/bash
#SBATCH --job-name=RR_Converter
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=64G
#SBATCH --time=03:00:00
#SBATCH -o /placeholder/jobout/slurmout_%A_%a.log
#SBATCH -e /placeholder/jobout/slurmout_%A_%a.log

ARRAY_START="$1"
CONVERTER_SIMG="$2"
CONVERTER_CONFIG="$3"
INPUT_LIST="$4"
RESULT_OUT="$5"

echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : ===== Task ${SLURM_ARRAY_TASK_ID} started ====="
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : CONVERTER_SIMG   = $CONVERTER_SIMG"
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : CONVERTER_CONFIG = $CONVERTER_CONFIG"
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : INPUT_LIST       = $INPUT_LIST"
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : RESULT_OUT       = $RESULT_OUT"

LINE=$(( $SLURM_ARRAY_TASK_ID + $ARRAY_START ))
INPUTFILE=$(sed -n ${LINE}p "${INPUT_LIST}")
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : INPUTFILE        = $INPUTFILE"

module load singularity/3.8.0

mkdir -p "$RESULT_OUT/jobout"

# Create per-task converter JSON
CONV_JSON="$RESULT_OUT/jobout/conv_input_${SLURM_ARRAY_TASK_ID}.json"
python3 -c "
import json
with open(\'$INPUTFILE\') as fh:
    files = [l.strip() for l in fh if l.strip()]
data = {\'reprocessingInputFileStreams\': [
    {\'key\': \'BN_CALIFR\', \'files\': []},
    {\'key\': \'BN_FASETH\', \'files\': []},
    {\'key\': \'SRR_DEBUG\', \'files\': files},
    {\'key\': \'SRR_REFERENCE\', \'files\': []}
]}
json.dump(data, open(\'$CONV_JSON\', \'w\'), indent=2)
print(\'[CONVERTER] : created $CONV_JSON with\', len(files), \'files\')
"

# Use shared filesystem path so singularity container can write to it directly
CONV_MF4_DIR="$RESULT_OUT/jobout/converted_mf4_${SLURM_ARRAY_TASK_ID}"
mkdir -p $CONV_MF4_DIR
echo "[CONVERTER] : output dir = $CONV_MF4_DIR"

if [ ! -f "$CONVERTER_SIMG" ]; then
    echo "[CONVERTER] : ERROR - simg not found: $CONVERTER_SIMG"
    exit 1
fi
if [ ! -f "$CONVERTER_CONFIG" ]; then
    echo "[CONVERTER] : ERROR - config not found: $CONVERTER_CONFIG"
    exit 1
fi
if [ ! -f "$CONV_JSON" ]; then
    echo "[CONVERTER] : ERROR - conv json not found: $CONV_JSON"
    exit 1
fi

echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : starts"
singularity exec --bind $CONV_MF4_DIR $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG $CONV_JSON $CONV_MF4_DIR
CONV_EXIT=$?
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : exit code $CONV_EXIT"

if [ $CONV_EXIT -ne 0 ]; then
    echo "[CONVERTER] : ERROR - converter failed, aborting"
    exit $CONV_EXIT
fi

# Converter creates ORCAS/ as a sibling of CONV_MF4_DIR inside jobout/
# so search the parent directory ($RESULT_OUT/jobout) for MF4 files
MF4_SUBDIR=$(find $RESULT_OUT/jobout -type f \( -name "*.mf4" -o -name "*.MF4" \) | head -n 1 | xargs -r dirname)

if [ -z "$MF4_SUBDIR" ]; then
    echo "[CONVERTER] : ERROR - no .mf4 files found after conversion, aborting"
    exit 1
fi

MF4_COUNT=$(find $MF4_SUBDIR -type f \( -name "*.mf4" -o -name "*.MF4" \) | wc -l)
echo "[CONVERTER] : $MF4_COUNT .mf4 file(s) found in $MF4_SUBDIR"

# Write only the folder path — dq_splitter.get_deb_files() walks it to collect MF4 files
CONV_FLIST="$RESULT_OUT/jobout/conv_fList_${SLURM_ARRAY_TASK_ID}.txt"
echo "$MF4_SUBDIR" > $CONV_FLIST
echo "[CONVERTER] : mf4 folder path written to $CONV_FLIST"
echo "[$(date \'+%Y-%m-%d %H:%M:%S\')] [CONVERTER] : ===== Task ${SLURM_ARRAY_TASK_ID} completed ====="
'''


base_exectractor = f'''#!/usr/bin/env bash
#SBATCH --job-name=rR_DQMining           
#SBATCH --nodes=1
#SBATCH --ntasks=1               
#SBATCH --cpus-per-task=1 
#SBATCH --mem=4G
#SBATCH --time=01:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log

echo "$1 $2"\n{dqMining} $1 $2
echo " "
'''


script = f"{dqScript }"
simg = f"{dqSimg}"
config = f"{dqCong }"

#**********************************************************
#          function to validate inputs provided
#**********************************************************
def validate_inputs():
    global simg, config
    validation_error = 0
    in_list = ".txt"
#*** section to read input parameters 
    print('[INFO] : Inputs - Validating ', end='\r')
    try : in_list = sys.argv[1]
    except : validation_error = 1
        
    if validation_error != 1:
        try : simg = sys.argv[2]
        except: validation_error = 2
        
    if validation_error != 1 and validation_error != 2:
        try : config = sys.argv[3] 
        except : validation_error = 3
    print('[INFO] : Inputs - Validated  ')

#*** section to validate parameters 
    if validation_error == 1:
        print('[ERROR] : Inputs - Validation Error - Input fList not provided, re-run by providing minimum inputList\nINPUT Expected : Quality_Checker.sh <input_list : mandatory> <Quality checker singularity : optional> <Quality checker config file : optional>\n[INFO] : Above parameter shuld be passed with full path ')
        exit()
    if validation_error == 2:
        print('[WARNING] : Validation Update - Default DQ Simg and Config file will be used ')
    elif validation_error == 3:
        print('[WARNING] : Inputs - Validation Update - Default Config file will be used          ')

#*** section to validate parametes existance
    existance_error = 0
    if ".txt" in in_list :
        if not os.path.isfile(in_list):
            print(f"[ERROR] : {in_list} does not exist")
            existance_error = 1
    else:
        print(f"[ERROR] : {in_list} is not in .txt format ")
        existance_error = 1
    
    if ".simg" in simg :
        if not os.path.isfile(simg):
            print(f"[ERROR] : {simg} does not exist")
            existance_error = 1
    else:
        print(f"[ERROR] : {simg} is not a singularity image")
        existance_error = 1
    
    if ".xml" in config :
        if not os.path.isfile(config):
            print(f"[ERROR] : {config} does not exist")
            existance_error = 1
    else:
        print(f"[ERROR] : {config} is not a congiguration file")
        existance_error = 1
    
    if existance_error == 1:
        exit()

    if validation_error == 2 or validation_error == 3:
        value = input('[USER_INPUT] : press Y/N to confirm : ')
        if value.lower() != 'y':
            print("Exiting Application")
            exit()
     
    return script , simg, config, in_list






"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
10/09/2024          Mandeep Singh       FHW-295     splitter for STLA_SCALE3 and STLA_SCALE4 added
22/08/2024          Mandeep Singh       FHW-268     splitter for STLA_SCALE1 DQ(created 1st version of file )

######################################################################################################
"""
