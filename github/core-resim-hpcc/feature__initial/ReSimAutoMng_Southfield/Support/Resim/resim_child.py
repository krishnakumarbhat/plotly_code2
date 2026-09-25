#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

###########################################################
from resim_splitter import *
###########################################################


def resim_child_script():
    return '''#!/bin/bash
#SBATCH --job-name=ReSimJob
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=6  
#SBATCH --mem=64G
#SBATCH --time=08:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/work/RESIM/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/work/RESIM/playground/jobout/slurmout_%A_%a.log

PIPE_START=$SECONDS
echo "[Pipeline_Execution] : starts at $PIPE_START"


TASK_START="$1"
SIMG="$2"
OUT="$3"
UPU="$4"
CONFIG="$5"

JOBOUT=$OUT/jobout
OUTPUT=$OUT/output
mkdir -p $JOBOUT
mkdir -p $OUTPUT

INPUT_SUPPORT='''+f"{inputfile}"+'''
HTML_SIMG=`grep html_simg $INPUT_SUPPORT | cut -d ":" -f2`
BORD_SIMG=`grep bord_simg $INPUT_SUPPORT | cut -d ":" -f2`
JSON_CREATOR=`grep json_creator $INPUT_SUPPORT | cut -d ":" -f2`
HTML_CONFIG=`grep html_config_gen7 $INPUT_SUPPORT | cut -d ":" -f2`
MUDP_SIMG_PATH=`grep mudp_simg $INPUT_SUPPORT | cut -d ":" -f2`
MUDP_CONFIG=`grep mudp_config_gen7 $INPUT_SUPPORT | cut -d ":" -f2`
MUDP_CONFIG_DGPS=`grep mudp_config_dgps $INPUT_SUPPORT | cut -d ":" -f2`
ALIGN=`grep align_kpi $INPUT_SUPPORT | cut -d ":" -f2`
DET=`grep det_kpi $INPUT_SUPPORT | cut -d ":" -f2`
DWNSEL=`grep downsel_kpi $INPUT_SUPPORT | cut -d ":" -f2`
IDMAT=`grep idmat_kpi $INPUT_SUPPORT | cut -d ":" -f2`
RCAP=`grep rcap_kpi $INPUT_SUPPORT | cut -d ":" -f2`
META_KPI=`grep meta_kpi $INPUT_SUPPORT | cut -d ":" -f2`
CAN_KPI=`grep can_kpi $INPUT_SUPPORT | cut -d ":" -f2`
CONVERTER_SIMG=`grep "^converter_simg:" $INPUT_SUPPORT | cut -d ":" -f2`
CONVERTER_CONFIG=`grep -P "^converter_config:" $INPUT_SUPPORT | grep -v "_dgps" | cut -d ":" -f2`
CONVERTER_CONFIG_DGPS=`grep "^converter_config_dgps:" $INPUT_SUPPORT | cut -d ":" -f2`
SIMG_NAME=$(basename "$SIMG" | tr '[:upper:]' '[:lower:]')
if [[ $SIMG_NAME == *'_dgps'* || $SIMG_NAME == *'dgps'* ]]; then
    CONVERTER_CONFIG=$CONVERTER_CONFIG_DGPS
fi
echo "[VIDEO_Config] : CONVERTER_SIMG=$CONVERTER_SIMG"
echo "[VIDEO_Config] : CONVERTER_CONFIG=$CONVERTER_CONFIG"


if [[ $SIMG == *'stla_small'* ]]; then
    BORD_CONFIG=`grep bord_config_small $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="STLA-SMALL"

elif [[ $SIMG_NAME == *'_dgps'* || $SIMG_NAME == *'dgps'* ]]; then
    BORD_CONFIG=`grep bord_config_v2 $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="DGPS"

elif [[ $SIMG == *'resim_v2'* && $SIMG == *'platform'* ]]; then
    BORD_CONFIG=`grep bord_config_v2 $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="GPO-V2"

elif [[ $SIMG == *'mcip'* ]]; then
    BORD_CONFIG=`grep bord_config_v2 $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="MCIP"

else
    YIELD_CUST="DEFAULT"
fi

echo "[M-INFO] : YIELD_CUST=$YIELD_CUST"

# Override MUDP config for DGPS if available
if [[ $YIELD_CUST == "DGPS" && -n $MUDP_CONFIG_DGPS ]]; then
    MUDP_CONFIG=$MUDP_CONFIG_DGPS
    echo "[M-INFO] : Using DGPS MUDP config: $MUDP_CONFIG"
fi

# Start application
echo "______________ Start of the application ______________"
TASK_LINE=$(( $SLURM_ARRAY_TASK_ID + $TASK_START ))
echo "[M-INFO] : TASK_NUMBER= $TASK_LINE"
INPUTFILE=$(sed -n ${TASK_LINE}p "${JOBOUT}/SIL_Input_all.txt")
echo "[M-INFO] : ${INPUTFILE}"
LOG_NAME=$(sed -n ${TASK_LINE}p "${JOBOUT}/SIL_input_session.txt")
echo "[Splitter_Execution] : execution requested for - $LOG_NAME"

IFS="," read -r -a flog <<< "$LOG_NAME"
DIR=$(dirname "${flog[0]}")
SESSION_BASE="${flog[2]// /}"
FILE_BASENAME=$(basename "${flog[0]}")
IFS="_" read -r -a newarr <<< "$FILE_BASENAME"


if [[ -n "$SESSION_BASE" ]]; then
    SESSION_STRIP=$(dirname "$(dirname "$SESSION_BASE")")
    child="${DIR#${SESSION_STRIP}/}"
else
    child=$(basename "$DIR")
fi
echo "[M-INFO] : output child folder: $child"


#if [[ $DIR == *'STLA-SMALL'* ]];then
#    child=${DIR##*"2-Sim/"}
#elif [[ $DIR == *'RNA-SDV-SRR7'* ]];then
#    child=${DIR##*"9-Upload/"}
#elif [[ $DIR == *'GPO-IFV7XX'* ]];then
#    IFS="/" read -r -a path <<< "${flog[0]}"
#    INPUT_DIR=${flog[0]%%"/${path[-2]}"*}
#    child=${INPUT_DIR##*"1-Raw/"}
#else
#    child="OUT_RESULT/out/${SLURM_ARRAY_TASK_ID}"
#fi
#echo " $UPU - output path3 : $child"

mkdir -p $OUTPUT/$child

TEMPDIR=$(mktemp -d --tmpdir=/dev/shm/ --suffix=".${SLURM_JOB_ID}_${SLURM_ARRAY_TASK_ID}" )
trap "rm -rf $TEMPDIR; echo \\"Removing ${TEMPDIR} \\"; exit "  SIGINT SIGTERM
if [ $? -ne 0 ]
then
        echo [M-INFO] : Failed to create TEMPDIR: $TEMPDIR, with errno 0
        exit 1
else
        echo "[M-INFO] : TEMPDIR is: $TEMPDIR"
        echo "[M-INFO] : Running ls -d on TEMPDIR"
        ls -d "$TEMPDIR"
        RET=$?
        if [ $RET -ne 0 ]
        then
                echo "[M-INFO] : Trying to create directory with direct mkdir - 1st time failed mktemp failed?"
                mkdir -p "$TEMPDIR"
                ls -l "$TEMPDIR"
        else
                echo "[M-INFO] : ls returned: $RET"
        fi
fi

module load singularity/3.8.0
SIMG_START=$SECONDS
echo "[Resim_Execution] : starts at $SIMG_START"
echo "$JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out" >> $OUTPUT/.mining.txt

singularity exec $SIMG''' + f''' {resimScript}''' + ''' $INPUTFILE $TEMPDIR $CONFIG

SIMG_END=$SECONDS
echo "[Resim_Execution] : ends at $SIMG_END"
echo "[Resim_Execution] : total resim execution time is $(( $SIMG_END - $SIMG_START ))"
echo ""

echo "[M-INFO] : moving from temp to output location"
cp -r $TEMPDIR/* $OUTPUT/"$child"
echo "[M-INFO] : moved from temp to output location"
echo ""

echo "[M-INFO] : removing tempdir"
rm -rf ${TEMPDIR}
echo "[M-INFO] : removed tempdir"
echo ""


###********************* pipeline Script section ****************

PY_START=$SECONDS
echo "[PIPELINE_CONFIG] : starts at  $SECONDS"

if [[ $YIELD_CUST == "DGPS" ]]; then
    echo "[PIPELINE_CONFIG] : DGPS mode - skipping pipeline_jsonCreator (HTML/BORD disabled)"
    EXIT_CODE=4
    # Generate MUDP list files for DGPS
    mkdir -p $OUTPUT/$child/MUDP_REPORT
    python3 -c "
import json, os, glob
jf = '$INPUTFILE'
outdir = '$OUTPUT/$child'
with open(jf) as f:
    data = json.load(f)
# Input list: b05 files from SRR_DEBUG
inp_files = []
for s in data.get('reprocessingInputFileStreams', []):
    if s.get('key') == 'SRR_DEBUG':
        inp_files = s.get('files', [])
        break
ilist = f'{outdir}/MUDP_REPORT/input.txt'
with open(ilist, 'w') as f:
    for fp in inp_files:
        print(fp, file=f)
print(f'[MUDP_DGPS_PREP] : Wrote {len(inp_files)} input files to {ilist}')
# Output list: resim output MF4 files from ORCAS folder
dgps_out = f'{outdir}/ORCAS'
out_files = []
if os.path.isdir(dgps_out):
    out_files = sorted(glob.glob(f'{dgps_out}/*.MF4') + glob.glob(f'{dgps_out}/*.mf4'))
olist = f'{outdir}/MUDP_REPORT/output.txt'
with open(olist, 'w') as f:
    for fp in out_files:
        print(fp, file=f)
print(f'[MUDP_DGPS_PREP] : Wrote {len(out_files)} output files to {olist}')
"
else
    python $JSON_CREATOR $INPUTFILE $OUTPUT/$child $SIMG $UPU  ${SLURM_ARRAY_TASK_ID}
    EXIT_CODE=$?
fi

echo "[M-INFO] : exit code is - $EXIT_CODE"
echo "[PIPELINE_CONFIG] : ends at  $SECONDS"
echo "[PIPELINE_CONFIG] : time took $(( $SECONDS - $PY_START )) to complete"


HTML(){
HTML_SIMG_START=$SECONDS
echo "[HTML_Execution] : starts at  $HTML_SIMG_START" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt 2>&1
singularity exec $HTML_SIMG /RUN_HTML.sh $HTML_CONFIG $OUTPUT/$child/HTML_REPORT/HTMLInputs_${SLURM_ARRAY_TASK_ID}.json $OUTPUT/$child/HTML_REPORT/ >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt 2>&1
HTML_SIMG_END=$SECONDS
echo "[HTML_Execution] : ends at  $HTML_SIMG_END" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt 2>&1 
echo "[HTML_Execution] : total html execution time is $(( $HTML_SIMG_END - $HTML_SIMG_START )) " >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt 2>&1
echo " " >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt 2>&1

}


BORD(){
echo "$1 : $2" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
echo "singularity exec $BORD_SIMG /RUN_BORDNET.sh $BORD_CONFIG $1 $OUTPUT/$child/BORDNET_REPORT/" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
BORD_SIMG_START=$SECONDS
echo "[BORDNET_Execution] : starts at  $BORD_SIMG_START" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
singularity exec $BORD_SIMG /RUN_BORDNET.sh $BORD_CONFIG $1 $OUTPUT/$child/BORDNET_REPORT/ >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
BORD_SIMG_END=$SECONDS
echo "[BORDNET_Execution] : ends at  $BORD_SIMG_END" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
echo "[BORDNET_Execution] : total bordnet execution time is $(( $BORD_SIMG_END - $BORD_SIMG_START ))" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
echo " " >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_$2.txt 2>&1
}


MUDP(){
echo "$1 : $2" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
echo "singularity exec $MUDP_SIMG_PATH /RUN_MUDP.sh $MUDP_CONFIG $1 $OUTPUT/$child/MUDP_REPORT/">> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
MUDP_SIMG_START=$SECONDS
echo "[MUDP_Execution] : starts at  $MUDP_SIMG_START" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
singularity exec $MUDP_SIMG_PATH /RUN_MUDP.sh $MUDP_CONFIG $1 $OUTPUT/$child/MUDP_REPORT/ >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
MUDP_SIMG_END=$SECONDS
echo "[MUDP_Execution] : ends at  $MUDP_SIMG_END" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
echo "[MUDP_Execution] : total MUDP execution time is $(( $MUDP_SIMG_END - $MUDP_SIMG_START ))" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
echo " " >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_$2.txt 2>&1
}


VIDEO(){
VIDEO_START=$SECONDS
VIDEO_LOG=$JOBOUT/${SLURM_ARRAY_TASK_ID}/VIDEO.txt
VIDEO_OUT=$OUTPUT/$child/VIDEO
mkdir -p $VIDEO_OUT
echo "[VIDEO_Execution] : starts at  $VIDEO_START" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : CONVERTER_SIMG=$CONVERTER_SIMG" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : CONVERTER_CONFIG=$CONVERTER_CONFIG" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : INPUTFILE=$INPUTFILE" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : OUTPUT_DIR=$VIDEO_OUT" >> $VIDEO_LOG 2>&1
if [[ ! -f $CONVERTER_SIMG ]]; then
    echo "[VIDEO_Execution] : ERROR - converter simg not found: $CONVERTER_SIMG" >> $VIDEO_LOG 2>&1
elif [[ ! -f $CONVERTER_CONFIG ]]; then
    echo "[VIDEO_Execution] : ERROR - converter config not found: $CONVERTER_CONFIG" >> $VIDEO_LOG 2>&1
else
    echo "[VIDEO_PREP] : Preparing converter JSON" >> $VIDEO_LOG 2>&1
    SIMG_LOWER_V=$(echo "$SIMG" | tr '[:upper:]' '[:lower:]')
    if [[ $SIMG_LOWER_V == *'dgps'* || $SIMG_LOWER_V == *'_dgps'* ]]; then
        # DGPS: create separate clean JSONs per g02 file from pre-cached list
        python3 -c "
import json, os, re, sys
jobout = '$JOBOUT'
video_out = '$VIDEO_OUT'
json_file = '$INPUTFILE'

# Read g02 files from pre-cached list
g02_cache = os.path.join(jobout, 'dgps_g02_cache.txt')
if os.path.isfile(g02_cache):
    with open(g02_cache) as f:
        g02_files = sorted([l.strip() for l in f if l.strip()])
    print('[VIDEO_PREP] : DGPS - loaded ' + str(len(g02_files)) + ' g02 files from cache')
else:
    print('[VIDEO_PREP] : DGPS - ERROR: g02 cache not found at ' + g02_cache)
    g02_files = []

if not g02_files:
    print('[VIDEO_PREP] : DGPS - no g02 files, skipping video')
    sys.exit(0)

# Group g02 files by recording directory (parent of parent = session timestamp)
# Each recording dir gets its own converter JSON for init-file prepending
from collections import OrderedDict
dir_groups = OrderedDict()
for fp in g02_files:
    parent = os.path.dirname(fp)
    dir_groups.setdefault(parent, []).append(fp)

# Find init file (first/smallest) per recording directory
init_for_dir = {}
for parent, files in dir_groups.items():
    rec_parent = os.path.dirname(parent)
    if rec_parent not in init_for_dir or files[0] < init_for_dir[rec_parent]:
        init_for_dir[rec_parent] = files[0]

total_jsons = 0
json_list_file = os.path.join(video_out, 'converter_json_list.txt')
with open(json_list_file, 'w') as jlist:
    for idx, g02_path in enumerate(g02_files):
        parent = os.path.dirname(g02_path)
        rec_parent = os.path.dirname(parent)
        init_file = init_for_dir.get(rec_parent)
        # Build file list: prepend init file for camera SPS/PPS data
        file_list = []
        if init_file and g02_path != init_file:
            file_list = [init_file, g02_path]
        else:
            file_list = [g02_path]
        converter_data = {
            'reprocessingInputFileStreams': [
                {'key': 'BN_CALIFR', 'files': []},
                {'key': 'BN_FASETH', 'files': []},
                {'key': 'SRR_DEBUG', 'files': []},
                {'key': 'SRR_REFERENCE', 'files': file_list}
            ]
        }
        out_json = os.path.join(video_out, f'converter_input_{idx}.json')
        with open(out_json, 'w') as f:
            json.dump(converter_data, f, indent=2)
        print(out_json, file=jlist)
        total_jsons += 1

print(f'[VIDEO_PREP] : DGPS - created {total_jsons} separate converter JSONs in {video_out}')
" >> $VIDEO_LOG 2>&1
        # Run converter for each JSON
        TOTAL_VIDEOS=0
        if [[ -f $VIDEO_OUT/converter_json_list.txt ]]; then
            while IFS= read -r CONV_JSON; do
                [[ -z "$CONV_JSON" ]] && continue
                echo "[VIDEO_Execution] : running converter with $CONV_JSON" >> $VIDEO_LOG 2>&1
                singularity exec $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG "$CONV_JSON" $VIDEO_OUT >> $VIDEO_LOG 2>&1
                rm -f "$CONV_JSON" >> $VIDEO_LOG 2>&1
            done < $VIDEO_OUT/converter_json_list.txt
            rm -f $VIDEO_OUT/converter_json_list.txt
        fi
    else
        # Non-DGPS: use existing resim JSON directly
        CONVERTER_JSON=$VIDEO_OUT/converter_input.json
        cp "$INPUTFILE" "$CONVERTER_JSON"
        echo "singularity exec $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG $CONVERTER_JSON $VIDEO_OUT" >> $VIDEO_LOG 2>&1
        singularity exec $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG $CONVERTER_JSON $VIDEO_OUT >> $VIDEO_LOG 2>&1
        rm -f $CONVERTER_JSON >> $VIDEO_LOG 2>&1
    fi
    VIDEO_EXIT=$?
    echo "[VIDEO_Execution] : converter exit code=$VIDEO_EXIT" >> $VIDEO_LOG 2>&1
    VIDEO_FILE_COUNT=$(find "$VIDEO_OUT" -type f \( -iname '*.mp4' -o -iname '*.mkv' -o -iname '*.avi' -o -iname '*.mov' -o -iname '*.wmv' -o -iname '*.flv' -o -iname '*.webm' -o -iname '*.mpeg' -o -iname '*.mpg' -o -iname '*.m4v' \) 2>/dev/null | wc -l)
    if [[ $VIDEO_FILE_COUNT -eq 0 ]]; then
        echo "[VIDEO_Execution] : WARNING - converter produced no video files" >> $VIDEO_LOG 2>&1
    else
        echo "[VIDEO_Execution] : converter produced $VIDEO_FILE_COUNT video file(s)" >> $VIDEO_LOG 2>&1
    fi
    echo "[VIDEO_Cleanup] : removing non-video files" >> $VIDEO_LOG 2>&1
    find "$VIDEO_OUT" -type f ! \( -iname '*.mp4' -o -iname '*.mkv' -o -iname '*.avi' -o -iname '*.mov' -o -iname '*.wmv' -o -iname '*.flv' -o -iname '*.webm' -o -iname '*.mpeg' -o -iname '*.mpg' -o -iname '*.m4v' \) -delete >> $VIDEO_LOG 2>&1
    echo "[VIDEO_Cleanup] : cleanup complete" >> $VIDEO_LOG 2>&1
fi
VIDEO_END=$SECONDS
echo "[VIDEO_Execution] : ends at  $VIDEO_END" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : total video execution time is $(( $VIDEO_END - $VIDEO_START ))" >> $VIDEO_LOG 2>&1
echo " " >> $VIDEO_LOG 2>&1
}


if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    HTML & 
    pid1=$!
    MUDP "$OUTPUT/$child/MUDP_REPORT/input.txt" "input" &
    pid2=$!
    MUDP "$OUTPUT/$child/MUDP_REPORT/output.txt" "output" &
    pid3=$!
else
    echo "[HTML_Execution] : HTML not requested"
    if [[ $EXIT_CODE -ne 4 ]]; then
        echo "[MUDP_Execution] : MUDP not requested"
    fi
fi

if [[ $EXIT_CODE -eq 4 ]]; then
    echo "[HTML_Execution] : HTML not requested (DGPS)"
    MUDP "$OUTPUT/$child/MUDP_REPORT/input.txt" "input" &
    pid2=$!
    MUDP "$OUTPUT/$child/MUDP_REPORT/output.txt" "output" &
    pid3=$!
    echo "[BORDNET_Execution] : BORDNET not requested (DGPS)"
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    BORD "$OUTPUT/$child/BORDNET_REPORT/BORDNET_iList_${SLURM_ARRAY_TASK_ID}.json" "input" & 
    pid4=$!
    BORD "$OUTPUT/$child/BORDNET_REPORT/BORDNET_oList_${SLURM_ARRAY_TASK_ID}.json" "output" & 
    pid5=$!

else
    if [[ $EXIT_CODE -ne 4 ]]; then
        echo "[BORDNET_Execution] : BORDNET not requested"
    fi
fi

if [[ $EXIT_CODE -eq 3 ]]; then
    echo "[HTML_Execution] : HTML not requested"
    echo "[MUDP_Execution] : MUDP not requested"
    echo "[BORDNET_Execution] : BORDNET not requested"
fi

if [[ -n $CONVERTER_SIMG && -n $CONVERTER_CONFIG ]]; then
    VIDEO &
    pid6=$!
    echo "[VIDEO_Execution] : Video converter triggered (pid=$pid6)"
else
    echo "[VIDEO_Execution] : VIDEO not configured (converter_simg='$CONVERTER_SIMG' converter_config='$CONVERTER_CONFIG')"
fi

wait $pid1 2>/dev/null
wait $pid2 2>/dev/null
wait $pid3 2>/dev/null
wait $pid4 2>/dev/null
wait $pid5 2>/dev/null
wait $pid6 2>/dev/null

echo ""

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_input.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_output.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/HTML.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_input.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_output.txt

    KPI_FLIST="$OUTPUT/$child/MUDP_REPORT/kpi_flist.txt"
    echo "$OUTPUT/$child/MUDP_REPORT" >> $KPI_FLIST
fi

if [[ $EXIT_CODE -eq 4 ]]; then
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_input.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out 2>/dev/null
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_output.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out 2>/dev/null
    rm -f $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_input.txt
    rm -f $JOBOUT/${SLURM_ARRAY_TASK_ID}/MUDP_output.txt

    KPI_FLIST="$OUTPUT/$child/MUDP_REPORT/kpi_flist.txt"
    echo "$OUTPUT/$child/MUDP_REPORT" >> $KPI_FLIST
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_input.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_output.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_input.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/BORD_output.txt
fi

if [[ -n $CONVERTER_SIMG && -n $CONVERTER_CONFIG ]]; then
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/VIDEO.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm -f $JOBOUT/${SLURM_ARRAY_TASK_ID}/VIDEO.txt
fi

echo "$OUTPUT/${child}"/*.xml >> $OUTPUT/.SIL_Statistics.txt

KPI_UPU(){
    KPI_START=$SECONDS
    echo "[KPI_UPU_Execution] : start at $KPI_START : $2" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_$2.txt 2>&1
    echo "python $1 $KPI_FLIST $META_KPI $OUTPUT/$child/MUDP_REPORT" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_$2.txt 2>&1
    python $1 $KPI_FLIST $META_KPI $OUTPUT/$child/MUDP_REPORT >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_$2.txt 2>&1
    KPI_END=$SECONDS
    echo "[KPI_UPU_Execution] : ends at  $KPI_END" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_$2.txt 2>&1
    echo "[KPI_UPU_Execution] : total KPI execution time is $(( $KPI_END - $KPI_START ))" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_$2.txt 2>&1
}

KPI_CAN(){
    KPI_START=$SECONDS
    echo "[KPI_CAN_Execution] : start at $KPI_START : $2" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt 2>&1
    echo "python $1  $OUTPUT/$child/BORDNET_REPORT/MCIP_TRACES/PCAN/INPUT  $OUTPUT/$child/BORDNET_REPORT/MCIP_TRACES/PCAN/OUTPUT $OUTPUT/$child/BORDNET_REPORT/" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt 2>&1
    python $1 $OUTPUT/$child/BORDNET_REPORT/MCIP_TRACES/PCAN/INPUT  $OUTPUT/$child/BORDNET_REPORT/MCIP_TRACES/PCAN/OUTPUT $OUTPUT/$child/BORDNET_REPORT/ >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt 2>&1
    KPI_END=$SECONDS
    echo "[KPI_CAN_Execution] : ends at  $KPI_END" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt 2>&1
    echo "[KPI_CAN_Execution] : total KPI execution time is $(( $KPI_END - $KPI_START ))" >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt 2>&1
}


if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    KPI_UPU "$ALIGN" "align" & 
    pid1=$!
    KPI_UPU "$DET" "det" &
    pid2=$!
    KPI_UPU "$DWNSEL" "dwnsel" &
    pid3=$!
    KPI_UPU "$IDMAT" "idmat" &
    pid4=$!
    KPI_UPU "$RCAP" "rcap" &
    pid5=$!
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    KPI_CAN "$CAN_KPI" & 
    pid6=$!
fi

wait $pid1
wait $pid2
wait $pid3
wait $pid4
wait $pid5
wait $pid6

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_align.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_det.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_dwnsel.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_idmat.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_rcap.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_align.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_det.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_dwnsel.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_idmat.txt
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_rcap.txt
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    cat $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $JOBOUT/${SLURM_ARRAY_TASK_ID}/KPI_CAN.txt
fi


echo "[Pipeline_Execution] : ends at $SECONDS "
echo "[Pipeline_Execution] : total pipeline execution time is $(( $SECONDS - $PIPE_START ))"
'''


def resim_child_script_highPrio():
    return '''#!/bin/bash
#SBATCH --job-name=ReSimJob
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=6  
#SBATCH --mem=48G
#SBATCH --time=04:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/work/RESIM/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/work/RESIM/playground/jobout/slurmout_%A_%a.log

PIPE_START=$SECONDS
echo "[Pipeline_Execution] : starts at $PIPE_START"


TASK_START="$1"
SIMG="$2"
OUT="$3"
UPU="$4"
CONFIG="$5"
echo "upu:$UPU"

JOBOUT=$OUT/jobout
OUTPUT=$OUT/output
mkdir -p $JOBOUT
mkdir -p $JOBOUT/${SLURM_ARRAY_TASK_ID}
mkdir -p $OUTPUT

INPUT_SUPPORT='''+f"{inputfile}"+'''
HTML_SIMG=`grep html_simg $INPUT_SUPPORT | cut -d ":" -f2`
BORD_SIMG=`grep bord_simg $INPUT_SUPPORT | cut -d ":" -f2`
JSON_CREATOR=`grep json_creator $INPUT_SUPPORT | cut -d ":" -f2`
HTML_CONFIG=`grep html_config_gen7 $INPUT_SUPPORT | cut -d ":" -f2`
MUDP_SIMG_PATH=`grep mudp_simg $INPUT_SUPPORT | cut -d ":" -f2`
MUDP_CONFIG=`grep mudp_config_gen7 $INPUT_SUPPORT | cut -d ":" -f2`
MUDP_CONFIG_DGPS=`grep mudp_config_dgps $INPUT_SUPPORT | cut -d ":" -f2`
SPLITTER='''+f"{splitter_path}"+'''
ALIGN=`grep align_kpi $INPUT_SUPPORT | cut -d ":" -f2`
DET=`grep det_kpi $INPUT_SUPPORT | cut -d ":" -f2`
DWNSEL=`grep downsel_kpi $INPUT_SUPPORT | cut -d ":" -f2`
IDMAT=`grep idmat_kpi $INPUT_SUPPORT | cut -d ":" -f2`
RCAP=`grep rcap_kpi $INPUT_SUPPORT | cut -d ":" -f2`
META_KPI=`grep meta_kpi $INPUT_SUPPORT | cut -d ":" -f2`
CAN_KPI=`grep can_kpi $INPUT_SUPPORT | cut -d ":" -f2`
CONVERTER_SIMG=`grep "^converter_simg:" $INPUT_SUPPORT | cut -d ":" -f2`
CONVERTER_CONFIG=`grep -P "^converter_config:" $INPUT_SUPPORT | grep -v "_dgps" | cut -d ":" -f2`
CONVERTER_CONFIG_DGPS=`grep "^converter_config_dgps:" $INPUT_SUPPORT | cut -d ":" -f2`
SIMG_LOWER=$(echo "$SIMG" | tr '[:upper:]' '[:lower:]')
if [[ $SIMG_LOWER == *'_dgps'* || $SIMG_LOWER == *'dgps'* ]]; then
    CONVERTER_CONFIG=$CONVERTER_CONFIG_DGPS
fi
echo "[VIDEO_Config] : CONVERTER_SIMG=$CONVERTER_SIMG"
echo "[VIDEO_Config] : CONVERTER_CONFIG=$CONVERTER_CONFIG"
UDP_KPI_SIMG=`grep "^udp_kpi_simg:" $INPUT_SUPPORT | cut -d ":" -f2`
echo "[UDP_KPI_Config] : UDP_KPI_SIMG=$UDP_KPI_SIMG"

if [[ $SIMG == *'stla_small'* ]]; then
    BORD_CONFIG=`grep bord_config_small $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="STLA-SMALL"

elif [[ $SIMG_LOWER == *'_dgps'* || $SIMG_LOWER == *'dgps'* ]]; then
    BORD_CONFIG=`grep bord_config_v2 $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="DGPS"

elif [[ $SIMG == *'resim_v2'* && $SIMG == *'platform'* ]]; then
    BORD_CONFIG=`grep bord_config_v2 $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="GPO-V2"

elif [[ $SIMG == *'mcip'* ]]; then
    BORD_CONFIG=`grep bord_config_v2 $INPUT_SUPPORT | cut -d ":" -f2`
    YIELD_CUST="MCIP"

else
    YIELD_CUST="DEFAULT"
fi

echo "[M-INFO] : YIELD_CUST=$YIELD_CUST"

# Override MUDP config for DGPS if available
if [[ $YIELD_CUST == "DGPS" && -n $MUDP_CONFIG_DGPS ]]; then
    MUDP_CONFIG=$MUDP_CONFIG_DGPS
    echo "[M-INFO] : Using DGPS MUDP config: $MUDP_CONFIG"
fi

# Start application
echo "______________ Start of the application ______________"
TASK_LINE=$(( $SLURM_ARRAY_TASK_ID + $TASK_START ))
echo "[M-INFO] : TASK_NUMBER= $TASK_LINE"
INPUTFILE=$(sed -n ${TASK_LINE}p "${JOBOUT}/SIL_Input_all.txt")
echo "[M-INFO] : ${INPUTFILE}"
LOG_NAME=$(sed -n ${TASK_LINE}p "${JOBOUT}/SIL_input_session.txt")
echo "[Splitter_Execution] : execution requested for - $LOG_NAME"

IFS="," read -r -a flog <<< "$LOG_NAME"
DIR=$(dirname "${flog[0]}")
SESSION_BASE="${flog[2]// /}"
FILE_BASENAME=$(basename "${flog[0]}")

if [[ -n "$SESSION_BASE" ]]; then
    SESSION_STRIP=$(dirname "$(dirname "$SESSION_BASE")")
    child="${DIR#${SESSION_STRIP}/}"
else
    child=$(basename "$DIR")
fi
echo "[M-INFO] : output child folder: $child"

mkdir -p $OUTPUT/"$child"

TEMPDIR=$(mktemp -d --tmpdir=/dev/shm/ --suffix=".${SLURM_JOB_ID}_${SLURM_ARRAY_TASK_ID}" )
trap "rm -rf $TEMPDIR; echo \\"Removing ${TEMPDIR} \\"; exit "  SIGINT SIGTERM
if [ $? -ne 0 ]
then
        echo [M-INFO] : Failed to create TEMPDIR: $TEMPDIR, with errno 0
        exit 1
else
        echo "[M-INFO] : TEMPDIR is: $TEMPDIR"
        echo "[M-INFO] : Running ls -d on TEMPDIR"
        ls -d "$TEMPDIR"
        RET=$?
        if [ $RET -ne 0 ]
        then
                echo "[M-INFO] : Trying to create directory with direct mkdir - 1st time failed mktemp failed?"
                mkdir -p "$TEMPDIR"
                ls -l "$TEMPDIR"
        else
                echo "[M-INFO] : ls returned: $RET"
        fi
fi

TMPOUT=$TEMPDIR/RESIM-RESULTS/"$child"
mkdir -p $TMPOUT 
JSON_FILE="$JOBOUT/SIL_input_${SLURM_ARRAY_TASK_ID}.json"
$SPLITTER "${flog[0]}" "${flog[1]}" $TEMPDIR $JSON_FILE $YIELD_CUST "$SESSION_BASE"

# Patch SRR_REFERENCE in JSON with corresponding _REF_ files (VEH mode only)
python -c "
import json, os, re
jf = '${JSON_FILE}'
td = '${TEMPDIR}'
debug_log = '${flog[0]}'
yield_cust = '$YIELD_CUST'
if os.path.isfile(jf):
    with open(jf) as f:
        data = json.load(f)

    # For DGPS: verify SRR_REFERENCE has a g03 for every b05; patch missing entries from cache
    if yield_cust == 'DGPS':
        deb_files_dgps = []
        ref_stream = None
        for stream in data.get('reprocessingInputFileStreams', []):
            if stream.get('key') == 'SRR_DEBUG':
                deb_files_dgps = stream.get('files', [])
            elif stream.get('key') == 'SRR_REFERENCE':
                ref_stream = stream
        existing = ref_stream.get('files', []) if ref_stream else []
        print(f'[REF_PATCH] : DGPS mode - SRR_REFERENCE has {len(existing)}/{len(deb_files_dgps)} g03 files from splitter')
        if len(existing) < len(deb_files_dgps):
            jobout_dir = os.path.dirname(jf)
            g03_cache_path = os.path.join(jobout_dir, 'dgps_g03_cache.txt')
            if os.path.isfile(g03_cache_path):
                import shutil
                with open(g03_cache_path) as gc:
                    all_g03 = [l.strip() for l in gc if l.strip()]
                g03_by_stem = {}
                for gp in all_g03:
                    gname = os.path.basename(gp)
                    gstem = re.sub(r'_g03\.MF4$', '', gname, flags=re.IGNORECASE)
                    g03_by_stem[gstem.lower()] = gp
                existing_basenames = {os.path.basename(p) for p in existing}
                new_refs = []
                for dp in deb_files_dgps:
                    dname = os.path.basename(dp)
                    dstem = re.sub(r'_b05\.MF4$', '', dname, flags=re.IGNORECASE)
                    g03_src = g03_by_stem.get(dstem.lower())
                    if g03_src:
                        g03_bn = os.path.basename(g03_src)
                        if g03_bn not in existing_basenames:
                            dest = os.path.join(td, 'LOGS', g03_bn)
                            if not os.path.isfile(dest):
                                shutil.copy2(g03_src, dest)
                            new_refs.append(dest)
                            existing_basenames.add(g03_bn)
                if new_refs and ref_stream is not None:
                    ref_stream['files'] = existing + new_refs
                    print(f'[REF_PATCH] : DGPS - added {len(new_refs)} missing g03 files, total={len(existing)+len(new_refs)}')
                else:
                    print(f'[REF_PATCH] : DGPS - WARNING: could not find missing g03 files in cache')
            else:
                print(f'[REF_PATCH] : DGPS - WARNING: g03 cache not found at {g03_cache_path}')
    else:
        # VEH mode: collect DEBUG files from SRR_DEBUG to find corresponding REF files
        deb_stems = set()
        for stream in data.get('reprocessingInputFileStreams', []):
            if stream.get('key') == 'SRR_DEBUG':
                for fp in stream.get('files', []):
                    name = os.path.splitext(os.path.basename(fp))[0]
                    # Extract stem: replace _DEBUG_ with empty to get common part
                    stem = name.replace('_DEBUG_', '_').replace('_debug_', '_')
                    # Also handle b05 style
                    stem = re.sub(r'_b05$', '', stem, flags=re.IGNORECASE)
                    deb_stems.add(stem.lower())
                break

        ref_files = []
        # First try: find _REF_ .MF4 files in TEMPDIR that match debug stems
        for root, dirs, files in os.walk(td):
            for fn in files:
                if '_REF_' in fn and (fn.endswith('.MF4') or fn.endswith('.mf4')):
                    ref_stem = os.path.splitext(fn)[0].replace('_REF_', '_').replace('_ref_', '_').lower()
                    if not deb_stems or ref_stem in deb_stems:
                        fp = os.path.join(root, fn)
                        if fp not in ref_files:
                            ref_files.append(fp)

        # Fallback: derive REF source directory from DEBUG log path
        if not ref_files:
            debug_dir = os.path.dirname(debug_log)
            ref_dir = debug_dir.replace('_DEBUG_', '_REF_')
            if os.path.isdir(ref_dir):
                for fn in sorted(os.listdir(ref_dir)):
                    if '_REF_' in fn and (fn.endswith('.MF4') or fn.endswith('.mf4')):
                        ref_stem = os.path.splitext(fn)[0].replace('_REF_', '_').replace('_ref_', '_').lower()
                        if not deb_stems or ref_stem in deb_stems:
                            fp = os.path.join(ref_dir, fn)
                            if fp not in ref_files:
                                ref_files.append(fp)

        ref_files.sort()
        for stream in data.get('reprocessingInputFileStreams', []):
            if stream.get('key') == 'SRR_REFERENCE':
                existing = stream.get('files', [])
                existing_set = set(existing)
                new_refs = [f for f in ref_files if f not in existing_set]
                if new_refs:
                    stream['files'] = existing + new_refs
                    print(f'[REF_PATCH] : Added {len(new_refs)} corresponding _REF_ files to SRR_REFERENCE')
                elif not existing:
                    print(f'[REF_PATCH] : WARNING - no corresponding _REF_ files found')
                else:
                    print(f'[REF_PATCH] : SRR_REFERENCE already has {len(existing)} files, no new matches')
                break

    with open(jf, 'w') as f:
        json.dump(data, f, indent=2)
"

module load singularity/3.8.0
SIMG_START=$SECONDS
echo "[Resim_Execution] : starts at $SIMG_START"
echo "$JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out" >> $OUTPUT/.mining.txt

RESIM_ATTEMPT=1
RESIM_JSON=$JSON_FILE
RESIM_EXIT=0

# Set max retries to the number of logs in this session (not a hardcoded limit)
MAX_RETRIES=$(python3 -c "
import json
count = 20
try:
    with open('$JSON_FILE') as f:
        data = json.load(f)
    for s in data.get('reprocessingInputFileStreams', []):
        if s.get('key') == 'SRR_DEBUG':
            count = len(s.get('files', []))
            break
except Exception:
    pass
print(count)
")
echo "[CRASH_RECOVERY] : Max retries set to $MAX_RETRIES (based on session log count)"

while true; do
    echo "[Resim_Execution] : attempt $RESIM_ATTEMPT with JSON=$RESIM_JSON"
    singularity exec $SIMG''' + f''' {resimScript}''' + ''' $RESIM_JSON $TMPOUT $CONFIG >> $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>&1
    RESIM_EXIT=$?

    # Check if docker crashed: either via exit code (128+signal) or by detecting
    # crash messages in the output (container wrapper may swallow the exit code)
    CRASH_DETECTED=0
    if [[ $RESIM_EXIT -ge 132 && $RESIM_EXIT -le 139 ]]; then
        CRASH_DETECTED=1
    elif grep -q "Segmentation fault" $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>/dev/null; then
        CRASH_DETECTED=1
        RESIM_EXIT=139
    elif grep -qP "Exit Code - 13[2-9]" $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>/dev/null; then
        CRASH_DETECTED=1
        RESIM_EXIT=$(grep -oP "Exit Code - \K13[2-9]" $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt | tail -1)
    fi

    if [[ $CRASH_DETECTED -eq 1 ]]; then
        echo "[DOCKER] : Exit Code - $RESIM_EXIT -> ReSim Application Crashed (attempt $RESIM_ATTEMPT)"

        # Parse output to find which logs were attempted (Running Resim for Log <path>)
        PROCESSED_LOGS=$(grep -oP '(?<=Running Resim for Log <)[^>]+' $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt | sort -u)
        PROCESSED_COUNT=$(echo "$PROCESSED_LOGS" | grep -c '.' 2>/dev/null || echo 0)
        echo "[CRASH_RECOVERY] : $PROCESSED_COUNT logs were attempted before crash"

        # Rebuild JSON excluding processed logs
        NEXT_JSON="${RESIM_JSON%.json}_retry${RESIM_ATTEMPT}.json"
        echo "$PROCESSED_LOGS" > $TMPOUT/_crash_processed.txt
        python3 -c "
import json, sys, os, re
pf = '$TMPOUT/_crash_processed.txt'
processed = set()
if os.path.isfile(pf):
    with open(pf) as fp:
        processed = {l.strip() for l in fp if l.strip()}
with open('$RESIM_JSON') as f:
    data = json.load(f)
# If no processed logs detected (crash before first log started, or DGPS simg uses
# different log format), skip the first SRR_DEBUG file to break the infinite retry cycle
if not processed:
    for stream in data.get('reprocessingInputFileStreams', []):
        if stream.get('key') == 'SRR_DEBUG':
            if stream.get('files'):
                first_log = stream['files'][0]
                processed.add(first_log)
                print(f'[CRASH_RECOVERY] : No processed logs detected - assuming crash on first log, skipping: {os.path.basename(first_log)}')
            break
remaining = 0
remaining_stems = set()
# First pass: filter b05 streams and collect remaining stems
for stream in data.get('reprocessingInputFileStreams', []):
    if stream.get('key') == 'SRR_REFERENCE':
        continue
    files = stream.get('files', [])
    if not files:
        continue
    new_files = [f for f in files if f not in processed]
    removed = len(files) - len(new_files)
    if stream.get('key') == 'SRR_DEBUG':
        remaining = len(new_files)
        # Collect stems from remaining b05 files for g03 matching
        for fp in new_files:
            stem = re.sub(r'_b05\.MF4$', '', os.path.basename(fp), flags=re.IGNORECASE)
            remaining_stems.add(stem.lower())
        print(f'[CRASH_RECOVERY] : SRR_DEBUG: {len(files)} -> {remaining} files (removed {removed} processed/crashed)')
    stream['files'] = new_files
# Second pass: filter SRR_REFERENCE g03 files to match remaining b05 stems
if remaining_stems:
    for stream in data.get('reprocessingInputFileStreams', []):
        if stream.get('key') == 'SRR_REFERENCE':
            ref_files = stream.get('files', [])
            new_refs = []
            for fp in ref_files:
                ref_stem = re.sub(r'_g03\.MF4$', '', os.path.basename(fp), flags=re.IGNORECASE)
                if ref_stem.lower() in remaining_stems:
                    new_refs.append(fp)
            removed_refs = len(ref_files) - len(new_refs)
            print(f'[CRASH_RECOVERY] : SRR_REFERENCE: {len(ref_files)} -> {len(new_refs)} files (removed {removed_refs} non-matching g03)')
            stream['files'] = new_refs
            break
if remaining == 0:
    print('[CRASH_RECOVERY] : No remaining logs to process')
    sys.exit(1)
with open('$NEXT_JSON', 'w') as f:
    json.dump(data, f, indent=2)
print(f'[CRASH_RECOVERY] : Retry JSON written to $NEXT_JSON')
" >> $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>&1
        REBUILD_EXIT=$?

        if [[ $REBUILD_EXIT -ne 0 ]]; then
            echo "[CRASH_RECOVERY] : No more logs to retry, stopping"
            break
        fi

        RESIM_JSON=$NEXT_JSON
        RESIM_ATTEMPT=$((RESIM_ATTEMPT + 1))

        # Archive previous attempt output and start fresh for next attempt
        mv $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim_attempt$((RESIM_ATTEMPT - 1)).txt 2>/dev/null

        # Safety limit: max retries = number of logs in the session
        if [[ $RESIM_ATTEMPT -gt $MAX_RETRIES ]]; then
            echo "[CRASH_RECOVERY] : Max retry attempts ($MAX_RETRIES) reached, stopping"
            break
        fi
    else
        # Normal exit (success or non-crash error), no retry needed
        if [[ $RESIM_EXIT -ne 0 ]]; then
            echo "[DOCKER] : Exit Code - $RESIM_EXIT -> ReSim Application Executed With Error"
        fi
        break
    fi
done
echo "[Resim_Execution] : completed after $RESIM_ATTEMPT attempt(s), last exit code=$RESIM_EXIT"

# Concatenate all attempt outputs to the job output
cat $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim_attempt*.txt $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out 2>/dev/null
rm -f $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim_attempt*.txt $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt

SIMG_END=$SECONDS
echo "[Resim_Execution] : ends at $SIMG_END"
echo "[Resim_Execution] : total resim execution time is $(( $SIMG_END - $SIMG_START ))"
echo ""


#: <<'END_COMMENT'
###********************* pipeline Script section ****************

PY_START=$SECONDS
echo "[PIPELINE_CONFIG] : starts at  $SECONDS"

echo "$JSON_CREATOR $JSON_FILE $TMPOUT $SIMG $UPU '${SLURM_ARRAY_TASK_ID}'"
if [[ $YIELD_CUST == "DGPS" ]]; then
    echo "[PIPELINE_CONFIG] : DGPS mode - skipping pipeline_jsonCreator (HTML/BORD disabled)"
    EXIT_CODE=4
    # Generate MUDP list files for DGPS
    mkdir -p $TMPOUT/MUDP_REPORT
    python3 -c "
import json, os, glob, sys
jf = '$JSON_FILE'
tmpout = '$TMPOUT'
task_id = '${SLURM_ARRAY_TASK_ID}'
if not os.path.isfile(jf):
    print(f'[MUDP_DGPS_PREP] : ERROR - JSON not found: {jf}', file=sys.stderr)
    sys.exit(1)
with open(jf) as f:
    data = json.load(f)
# Input list: b05 files from SRR_DEBUG
inp_files = []
for s in data.get('reprocessingInputFileStreams', []):
    if s.get('key') == 'SRR_DEBUG':
        inp_files = s.get('files', [])
        break
ilist = f'{tmpout}/MUDP_REPORT/MUDP_iList_{task_id}.txt'
with open(ilist, 'w') as f:
    for fp in inp_files:
        print(fp, file=f)
print(f'[MUDP_DGPS_PREP] : Wrote {len(inp_files)} input files to {ilist}')
# Output list: resim output MF4 files from ORCAS folder
dgps_out = f'{tmpout}/ORCAS'
out_files = []
if os.path.isdir(dgps_out):
    out_files = sorted(glob.glob(f'{dgps_out}/*.MF4') + glob.glob(f'{dgps_out}/*.mf4'))
olist = f'{tmpout}/MUDP_REPORT/MUDP_oList_{task_id}.txt'
with open(olist, 'w') as f:
    for fp in out_files:
        print(fp, file=f)
print(f'[MUDP_DGPS_PREP] : Wrote {len(out_files)} output files to {olist}')
"
else
    python $JSON_CREATOR $JSON_FILE $TMPOUT $SIMG $UPU "${SLURM_ARRAY_TASK_ID}"
    EXIT_CODE=$?
fi

echo "[M-INFO] : exit code is - $EXIT_CODE"
echo "[PIPELINE_CONFIG] : ends at  $SECONDS"
echo "[PIPELINE_CONFIG] : time took $(( $SECONDS - $PY_START )) to complete"


HTML(){
HTML_SIMG_START=$SECONDS
echo "[HTML_Execution] : starts at  $HTML_SIMG_START" >>  $TMPOUT/HTML_REPORT/${SLURM_ARRAY_TASK_ID}_HTML.txt 2>&1
singularity exec $HTML_SIMG /RUN_HTML.sh $HTML_CONFIG $TMPOUT/HTML_REPORT/HTMLInputs_${SLURM_ARRAY_TASK_ID}.json $TMPOUT/HTML_REPORT/ >> $TMPOUT/HTML_REPORT/${SLURM_ARRAY_TASK_ID}_HTML.txt 2>&1
HTML_SIMG_END=$SECONDS
echo "[HTML_Execution] : ends at  $HTML_SIMG_END" >>  $TMPOUT/HTML_REPORT/${SLURM_ARRAY_TASK_ID}_HTML.txt 2>&1 
echo "[HTML_Execution] : total html execution time is $(( $HTML_SIMG_END - $HTML_SIMG_START )) " >>  $TMPOUT/HTML_REPORT/${SLURM_ARRAY_TASK_ID}_HTML.txt 2>&1
echo " " >>  $TMPOUT/HTML_REPORT/${SLURM_ARRAY_TASK_ID}_HTML.txt 2>&1
}


BORD(){
echo "$1 : $2" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
echo "singularity exec $BORD_SIMG /RUN_BORDNET.sh $BORD_CONFIG $1 $TMPOUT/BORDNET_REPORT/" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
BORD_SIMG_START=$SECONDS
echo "[BORDNET_Execution] : starts at  $BORD_SIMG_START" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
singularity exec $BORD_SIMG /RUN_BORDNET.sh $BORD_CONFIG $1 $TMPOUT/BORDNET_REPORT/ >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
BORD_SIMG_END=$SECONDS
echo "[BORDNET_Execution] : ends at  $BORD_SIMG_END" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
echo "[BORDNET_Execution] : total bordnet execution time is $(( $BORD_SIMG_END - $BORD_SIMG_START ))" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
echo " " >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_$2.txt 2>&1
}


MUDP(){
MUDP_OUTDIR=$TMPOUT/MUDP_REPORT/MUDP_${2^}
mkdir -p $MUDP_OUTDIR
echo "$1 : $2" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
echo "singularity exec $MUDP_SIMG_PATH /RUN_MUDP.sh $MUDP_CONFIG $1 $MUDP_OUTDIR/">> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
MUDP_SIMG_START=$SECONDS
echo "[MUDP_Execution] : starts at  $MUDP_SIMG_START" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
singularity exec $MUDP_SIMG_PATH /RUN_MUDP.sh $MUDP_CONFIG $1 $MUDP_OUTDIR/ >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
MUDP_SIMG_END=$SECONDS
echo "[MUDP_Execution] : ends at  $MUDP_SIMG_END" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
echo "[MUDP_Execution] : total MUDP execution time is $(( $MUDP_SIMG_END - $MUDP_SIMG_START ))" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
echo " " >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_$2.txt 2>&1
}


VIDEO(){
VIDEO_START=$SECONDS
# Keep VIDEO_LOG outside VIDEO_OUT so the cleanup find-delete doesn't erase it
VIDEO_LOG=$TMPOUT/${SLURM_ARRAY_TASK_ID}_VIDEO.txt
VIDEO_OUT=$TMPOUT/VIDEO
mkdir -p $VIDEO_OUT
echo "[VIDEO_Execution] : starts at  $VIDEO_START" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : CONVERTER_SIMG=$CONVERTER_SIMG" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : CONVERTER_CONFIG=$CONVERTER_CONFIG" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : JSON_FILE=$JSON_FILE" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : OUTPUT_DIR=$VIDEO_OUT" >> $VIDEO_LOG 2>&1
if [[ ! -f $CONVERTER_SIMG ]]; then
    echo "[VIDEO_Execution] : ERROR - converter simg not found: $CONVERTER_SIMG" >> $VIDEO_LOG 2>&1
elif [[ ! -f $CONVERTER_CONFIG ]]; then
    echo "[VIDEO_Execution] : ERROR - converter config not found: $CONVERTER_CONFIG" >> $VIDEO_LOG 2>&1
else
    echo "[VIDEO_PREP] : Preparing converter JSON" >> $VIDEO_LOG 2>&1
    SIMG_LOWER_V=$(echo "$SIMG" | tr '[:upper:]' '[:lower:]')
    if [[ $SIMG_LOWER_V == *'dgps'* || $SIMG_LOWER_V == *'_dgps'* ]]; then
        # DGPS: create separate clean JSONs per g02 file from pre-cached list
        python3 -c "
import json, os, re, sys
jobout = '$JOBOUT'
video_out = '$VIDEO_OUT'
json_file = '$JSON_FILE'

# Get b05 stems from the resim JSON to filter only this task's g02 files
b05_stems = set()
try:
    with open(json_file) as f:
        data = json.load(f)
    for s in data.get('reprocessingInputFileStreams', []):
        if s.get('key') == 'SRR_DEBUG':
            for fp in s.get('files', []):
                name = os.path.splitext(os.path.basename(fp))[0]
                stem = re.sub(r'_b05$', '', name, flags=re.IGNORECASE).lower()
                b05_stems.add(stem)
            break
except:
    pass
print(f'[VIDEO_PREP] : DGPS - b05 stems for this task: {len(b05_stems)}')

# Read g02 files from pre-cached list
g02_cache = os.path.join(jobout, 'dgps_g02_cache.txt')
if os.path.isfile(g02_cache):
    with open(g02_cache) as f:
        all_g02 = sorted([l.strip() for l in f if l.strip()])
    print('[VIDEO_PREP] : DGPS - loaded ' + str(len(all_g02)) + ' g02 files from cache')
else:
    print('[VIDEO_PREP] : DGPS - ERROR: g02 cache not found at ' + g02_cache)
    all_g02 = []

# Filter g02 to only this task's matching stems
if b05_stems:
    g02_files = []
    for fp in all_g02:
        fn_stem = re.sub(r'_g02\.mf4$', '', os.path.basename(fp), flags=re.IGNORECASE).lower()
        if fn_stem in b05_stems:
            g02_files.append(fp)
    g02_files.sort()
    print(f'[VIDEO_PREP] : DGPS - matched {len(g02_files)} g02 files for this task')
else:
    g02_files = all_g02

if not g02_files:
    print('[VIDEO_PREP] : DGPS - no g02 files, skipping video')
    sys.exit(0)

# Group g02 files by recording directory for init-file prepending
from collections import OrderedDict
dir_groups = OrderedDict()
for fp in g02_files:
    parent = os.path.dirname(fp)
    dir_groups.setdefault(parent, []).append(fp)

# Find init file (first/smallest) per recording directory
init_for_dir = {}
for parent, files in dir_groups.items():
    rec_parent = os.path.dirname(parent)
    if rec_parent not in init_for_dir or files[0] < init_for_dir[rec_parent]:
        init_for_dir[rec_parent] = files[0]

total_jsons = 0
json_list_file = os.path.join(video_out, 'converter_json_list.txt')
with open(json_list_file, 'w') as jlist:
    for idx, g02_path in enumerate(g02_files):
        parent = os.path.dirname(g02_path)
        rec_parent = os.path.dirname(parent)
        init_file = init_for_dir.get(rec_parent)
        # Build file list: prepend init file for camera SPS/PPS data
        file_list = []
        if init_file and g02_path != init_file:
            file_list = [init_file, g02_path]
        else:
            file_list = [g02_path]
        converter_data = {
            'reprocessingInputFileStreams': [
                {'key': 'BN_CALIFR', 'files': []},
                {'key': 'BN_FASETH', 'files': []},
                {'key': 'SRR_DEBUG', 'files': []},
                {'key': 'SRR_REFERENCE', 'files': file_list}
            ]
        }
        out_json = os.path.join(video_out, f'converter_input_{idx}.json')
        with open(out_json, 'w') as f:
            json.dump(converter_data, f, indent=2)
        print(out_json, file=jlist)
        total_jsons += 1

print(f'[VIDEO_PREP] : DGPS - created {total_jsons} separate converter JSONs in {video_out}')
" >> $VIDEO_LOG 2>&1
        # Run converter for each JSON
        TOTAL_VIDEOS=0
        if [[ -f $VIDEO_OUT/converter_json_list.txt ]]; then
            while IFS= read -r CONV_JSON; do
                [[ -z "$CONV_JSON" ]] && continue
                echo "[VIDEO_Execution] : running converter with $CONV_JSON" >> $VIDEO_LOG 2>&1
                singularity exec $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG "$CONV_JSON" $VIDEO_OUT >> $VIDEO_LOG 2>&1
                rm -f "$CONV_JSON" >> $VIDEO_LOG 2>&1
            done < $VIDEO_OUT/converter_json_list.txt
            rm -f $VIDEO_OUT/converter_json_list.txt
        fi
    else
        # Non-DGPS: use existing resim JSON directly
        CONVERTER_JSON=$VIDEO_OUT/converter_input.json
        cp "$JSON_FILE" "$CONVERTER_JSON"
        echo "singularity exec $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG $CONVERTER_JSON $VIDEO_OUT" >> $VIDEO_LOG 2>&1
        singularity exec $CONVERTER_SIMG /RUN_CONVERTER.sh $CONVERTER_CONFIG $CONVERTER_JSON $VIDEO_OUT >> $VIDEO_LOG 2>&1
        rm -f $CONVERTER_JSON >> $VIDEO_LOG 2>&1
    fi
    VIDEO_EXIT=$?
    echo "[VIDEO_Execution] : converter exit code=$VIDEO_EXIT" >> $VIDEO_LOG 2>&1
    VIDEO_FILE_COUNT=$(find "$VIDEO_OUT" -type f \( -iname '*.mp4' -o -iname '*.mkv' -o -iname '*.avi' -o -iname '*.mov' -o -iname '*.wmv' -o -iname '*.flv' -o -iname '*.webm' -o -iname '*.mpeg' -o -iname '*.mpg' -o -iname '*.m4v' \) 2>/dev/null | wc -l)
    if [[ $VIDEO_FILE_COUNT -eq 0 ]]; then
        echo "[VIDEO_Execution] : WARNING - converter produced no video files" >> $VIDEO_LOG 2>&1
    else
        echo "[VIDEO_Execution] : converter produced $VIDEO_FILE_COUNT video file(s)" >> $VIDEO_LOG 2>&1
    fi
    echo "[VIDEO_Cleanup] : removing non-video files" >> $VIDEO_LOG 2>&1
    find "$VIDEO_OUT" -type f ! \( -iname '*.mp4' -o -iname '*.mkv' -o -iname '*.avi' -o -iname '*.mov' -o -iname '*.wmv' -o -iname '*.flv' -o -iname '*.webm' -o -iname '*.mpeg' -o -iname '*.mpg' -o -iname '*.m4v' \) -delete >> $VIDEO_LOG 2>&1
    echo "[VIDEO_Cleanup] : cleanup complete" >> $VIDEO_LOG 2>&1
fi
VIDEO_END=$SECONDS
echo "[VIDEO_Execution] : ends at  $VIDEO_END" >> $VIDEO_LOG 2>&1
echo "[VIDEO_Execution] : total video execution time is $(( $VIDEO_END - $VIDEO_START ))" >> $VIDEO_LOG 2>&1
echo " " >> $VIDEO_LOG 2>&1
}


if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    echo "[HTML_Execution] : HTML disabled in highPrio mode"
    MUDP "$TMPOUT/MUDP_REPORT/MUDP_iList_${SLURM_ARRAY_TASK_ID}.txt" "input"
    echo "[CLEANUP] : Removing TEMPDIR/LOGS to free /dev/shm for MUDP output"
    rm -rf $TEMPDIR/LOGS
    MUDP "$TMPOUT/MUDP_REPORT/MUDP_oList_${SLURM_ARRAY_TASK_ID}.txt" "output"
else
    echo "[HTML_Execution] : HTML not requested"
    if [[ $EXIT_CODE -ne 4 ]]; then
        echo "[MUDP_Execution] : MUDP not requested"
    fi
fi

if [[ $EXIT_CODE -eq 4 ]]; then
    echo "[HTML_Execution] : HTML not requested (DGPS)"
    MUDP "$TMPOUT/MUDP_REPORT/MUDP_iList_${SLURM_ARRAY_TASK_ID}.txt" "input"
    echo "[CLEANUP] : Removing TEMPDIR/LOGS to free /dev/shm for MUDP output"
    rm -rf $TEMPDIR/LOGS
    MUDP "$TMPOUT/MUDP_REPORT/MUDP_oList_${SLURM_ARRAY_TASK_ID}.txt" "output"
    echo "[BORDNET_Execution] : BORDNET not requested (DGPS)"
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    BORD $TMPOUT/BORDNET_REPORT/BORDNET_iList_${SLURM_ARRAY_TASK_ID}.json "input" &  
    pid4=$!
    BORD $TMPOUT/BORDNET_REPORT/BORDNET_oList_${SLURM_ARRAY_TASK_ID}.json "output" &  
    pid5=$!
else
    if [[ $EXIT_CODE -ne 4 ]]; then
        echo "[BORDNET_Execution] : BORDNET not requested"
    fi
fi

if [[ $EXIT_CODE -eq 3 ]]; then
    echo "[HTML_Execution] : HTML not requested"
    echo "[MUDP_Execution] : MUDP not requested"
    echo "[BORDNET_Execution] : BORDNET not requested"
fi

if [[ -n $CONVERTER_SIMG && -n $CONVERTER_CONFIG ]]; then
    VIDEO &
    pid_video=$!
    echo "[VIDEO_Execution] : Video converter triggered (pid=$pid_video)"
else
    echo "[VIDEO_Execution] : VIDEO not configured (converter_simg='$CONVERTER_SIMG' converter_config='$CONVERTER_CONFIG')"
fi

# MUDP ran sequentially (no wait needed); collect logs if they exist
if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 || $EXIT_CODE -eq 4 ]]; then
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_input.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out 2>/dev/null
    rm -f $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_input.txt
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_output.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out 2>/dev/null
    rm -f $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_MUDP_output.txt
fi

wait $pid4
cat $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_input.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
rm $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_input.txt

wait $pid5
cat $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_output.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
rm $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_BORD_output.txt

if [[ -n $CONVERTER_SIMG && -n $CONVERTER_CONFIG ]]; then
    wait $pid_video
    cat $TMPOUT/VIDEO/${SLURM_ARRAY_TASK_ID}_VIDEO.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $TMPOUT/VIDEO/${SLURM_ARRAY_TASK_ID}_VIDEO.txt
fi

echo ""

UDP_KPI(){
UDP_KPI_START=$SECONDS
UDP_KPI_LOG=$TMPOUT/${SLURM_ARRAY_TASK_ID}_UDP_KPI.txt
UDP_KPI_OUT=$TMPOUT/UDP_KPI
mkdir -p $UDP_KPI_OUT
echo "[UDP_KPI_Execution] : starts at  $UDP_KPI_START" >> $UDP_KPI_LOG 2>&1
echo "[UDP_KPI_Execution] : UDP_KPI_SIMG=$UDP_KPI_SIMG" >> $UDP_KPI_LOG 2>&1
if [[ ! -f $UDP_KPI_SIMG ]]; then
    echo "[UDP_KPI_Execution] : ERROR - UDP KPI simg not found: $UDP_KPI_SIMG" >> $UDP_KPI_LOG 2>&1
else
    # Generate UDP KPI JSON from MUDP Input and MUDP Output HDF files (paired by log stem)
    python3 -c "
import json, os, glob, re

mudp_input_dir = '$TMPOUT/MUDP_REPORT/MUDP_Input'
mudp_output_dir = '$TMPOUT/MUDP_REPORT/MUDP_Output'
udp_kpi_out = '$UDP_KPI_OUT'
task_id = '${SLURM_ARRAY_TASK_ID}'

all_input = sorted(glob.glob(os.path.join(mudp_input_dir, '*.h5')))
all_output = sorted(glob.glob(os.path.join(mudp_output_dir, '*.h5')))

# Build output lookup: strip trailing _rXXXXXXXX version tag to get base stem
out_by_stem = {}
for fp in all_output:
    stem = re.sub(r'_r[0-9]+$', '', os.path.splitext(os.path.basename(fp))[0])
    out_by_stem[stem] = fp

# Keep only pairs where both input and output exist
paired_input, paired_output = [], []
for fp in all_input:
    stem = os.path.splitext(os.path.basename(fp))[0]
    if stem in out_by_stem:
        paired_input.append(fp)
        paired_output.append(out_by_stem[stem])

print(f'[UDP_KPI_PREP] : {len(all_input)} input / {len(all_output)} output HDF found, {len(paired_input)} pairs matched')
if len(all_input) != len(all_output):
    unmatched = [os.path.basename(f) for f in all_input if os.path.splitext(os.path.basename(f))[0] not in out_by_stem]
    print(f'[UDP_KPI_PREP] : WARNING - unmatched input HDF (no output): {unmatched}')

if not paired_input:
    print('[UDP_KPI_PREP] : ERROR - no paired HDF files found, skipping JSON creation')
else:
    udp_kpi_json = {
        'INPUT_HDF': paired_input,
        'OUTPUT_HDF': paired_output
    }
    json_path = os.path.join(udp_kpi_out, f'UDP_KPI_input_{task_id}.json')
    with open(json_path, 'w') as jf:
        json.dump(udp_kpi_json, jf, indent=4)
    print(f'[UDP_KPI_PREP] : Created JSON with {len(paired_input)} paired INPUT/OUTPUT HDF files')
    print(f'[UDP_KPI_PREP] : JSON path: {json_path}')
" >> $UDP_KPI_LOG 2>&1

    UDP_KPI_JSON=$UDP_KPI_OUT/UDP_KPI_input_${SLURM_ARRAY_TASK_ID}.json
    if [[ -f $UDP_KPI_JSON ]]; then
        echo "[UDP_KPI_Execution] : Running singularity run $UDP_KPI_SIMG json $UDP_KPI_JSON $UDP_KPI_OUT" >> $UDP_KPI_LOG 2>&1
        singularity run $UDP_KPI_SIMG json $UDP_KPI_JSON $UDP_KPI_OUT >> $UDP_KPI_LOG 2>&1
        echo "[UDP_KPI_Execution] : exit code=$?" >> $UDP_KPI_LOG 2>&1
    else
        echo "[UDP_KPI_Execution] : ERROR - UDP KPI JSON not created, skipping execution" >> $UDP_KPI_LOG 2>&1
    fi
fi
UDP_KPI_END=$SECONDS
echo "[UDP_KPI_Execution] : ends at  $UDP_KPI_END" >> $UDP_KPI_LOG 2>&1
echo "[UDP_KPI_Execution] : total UDP KPI execution time is $(( $UDP_KPI_END - $UDP_KPI_START ))" >> $UDP_KPI_LOG 2>&1
echo " " >> $UDP_KPI_LOG 2>&1
}

if [[ -n $UDP_KPI_SIMG ]]; then
    UDP_KPI
    cat $TMPOUT/${SLURM_ARRAY_TASK_ID}_UDP_KPI.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out 2>/dev/null
    rm -f $TMPOUT/${SLURM_ARRAY_TASK_ID}_UDP_KPI.txt
else
    echo "[UDP_KPI_Execution] : UDP KPI not configured (udp_kpi_simg not set)"
fi

echo "moving from temp to output location"
cp -r $TEMPDIR/RESIM-RESULTS/* $OUTPUT
echo "moved from temp to output location"
rm -rf $TMPOUT/HTML_REPORT
rm -rf $TMPOUT/ORCAS
rm -rf $TMPOUT/CANoe

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 || $EXIT_CODE -eq 4 ]]; then  
    KPI_FLIST="$TMPOUT/MUDP_REPORT/kpi_flist.txt"
    echo "KPI file_list is at : $KPI_FLIST"
    echo "$TMPOUT/MUDP_REPORT" >> $KPI_FLIST
fi

echo "$OUTPUT/${child}"/*.xml >> $OUTPUT/.SIL_Statistics.txt

KPI_UPU(){
    KPI_START=$SECONDS
    echo "[KPI_UPU_Execution] : start at $KPI_START : $2" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_$2.txt 2>&1
    echo "python $1 $KPI_FLIST $META_KPI $OUTPUT/$child/MUDP_REPORT" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_$2.txt 2>&1
    python $1 $KPI_FLIST $META_KPI $OUTPUT/$child/MUDP_REPORT >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_$2.txt 2>&1
    KPI_END=$SECONDS
    echo "[KPI_UPU_Execution] : ends at  $KPI_END" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_$2.txt 2>&1
    echo "[KPI_UPU_Execution] : total KPI execution time is $(( $KPI_END - $KPI_START ))" >> $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_$2.txt 2>&1
}

KPI_CAN(){
    KPI_START=$SECONDS
    echo "[KPI_CAN_Execution] : start at $KPI_START : $2" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt 2>&1
    echo "python $1 $TMPOUT/BORDNET_REPORT/MCIP_TRACES/PCAN/INPUT $TMPOUT/BORDNET_REPORT/MCIP_TRACES/PCAN/OUTPUT $OUTPUT/$child/BORDNET_REPORT/" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt 2>&1
    python $1 $TMPOUT/BORDNET_REPORT/MCIP_TRACES/PCAN/INPUT $TMPOUT/BORDNET_REPORT/MCIP_TRACES/PCAN/OUTPUT $OUTPUT/$child/BORDNET_REPORT/ >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt 2>&1
    KPI_END=$SECONDS
    echo "[KPI_CAN_Execution] : ends at  $KPI_END" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt 2>&1
    echo "[KPI_CAN_Execution] : total KPI execution time is $(( $KPI_END - $KPI_START ))" >> $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt 2>&1
}


if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    KPI_UPU "$ALIGN" "align" & 
    pid1=$!
    KPI_UPU "$DET" "det" &
    pid2=$!
    KPI_UPU "$DWNSEL" "dwnsel" &
    pid3=$!
    KPI_UPU "$IDMAT" "idmat" &
    pid4=$!
    KPI_UPU "$RCAP" "rcap" &
    pid5=$!
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    KPI_CAN "$CAN_KPI" & 
    pid4=$!
fi

wait $pid1
wait $pid2
wait $pid3
wait $pid4
wait $pid5
wait $pid6

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 1 ]]; then
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_align.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_det.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_dwnsel.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_idmat.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    cat $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_rcap.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_align.txt
    rm $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_det.txt
    rm $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_dwnsel.txt
    rm $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_idmat.txt
    rm $TMPOUT/MUDP_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_rcap.txt
fi

if [[ $EXIT_CODE -eq 0 || $EXIT_CODE -eq 2 ]]; then
    cat $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm $TMPOUT/BORDNET_REPORT/${SLURM_ARRAY_TASK_ID}_KPI_CAN.txt
fi

#END_COMMENT

cp -r $TEMPDIR/RESIM-RESULTS/* $OUTPUT
rm -rf ${TEMPDIR}
echo "removed tempdir"

echo "[Pipeline_Execution] : ends at $SECONDS "
echo "[Pipeline_Execution] : total pipeline execution time is $(( $SECONDS - $PIPE_START ))"
'''

resim_mining_child = '''#!/usr/bin/env bash
#SBATCH --job-name=R_Mining
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1 
#SBATCH --mem=4G
#SBATCH --time=01:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log

'''+F"{resimMining}"+''' $1 $2 $2
echo " "
'''

stats_mining_child = '''#!/usr/bin/env bash
#SBATCH --job-name=S_Mining
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1 
#SBATCH --mem=4G
#SBATCH --time=01:00:00
#SBATCH -o /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log
#SBATCH -e /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/d1cse7/playground/jobout/slurmout_%A_%a.log

'''+F"{sourceEnv}"+'''
'''+F"{statsMining }"+''' $1
echo " "
'''


simg = ""

#**********************************************************
#          function to validate inputs provided
#**********************************************************
def validate_inputs():
    global simg, config, reqDoc
    validation_error = 0
    in_list = ".txt"
#*** section to read input parameters 
    print('[INFO] : Inputs - Validating ', end='\r')
    try : in_list = sys.argv[1]
    except : validation_error = 1
        
    if validation_error != 1:
        try : simg = sys.argv[2]
        except: validation_error = 2

    print('[INFO] : Inputs - Validated  ')

#*** section to validate parameters 
    if validation_error == 1:
        print('[ERROR] : Inputs - Validation Error - Input fList not provided.    ', end='')
    elif validation_error == 2:
        print('[ERROR] : Inputs - Validation Error - Singularity image not provided.    ', end='')
    if validation_error == 1 or validation_error == 2:
        print('[ Expected INPUT format : rResim.sh <input_list : mandatory> <Resim singularity : mandatory> ]\n[INFO] : Above parameters should be passed with full path\n \n')
        exit()

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
    
    if existance_error == 1:
        exit()
    if "projects" in simg :
        server = "projects/"
    elif "PROJECTS" in simg :
        server = "PROJECTS/"
    else:
        server="NOT defined"
    project = simg.split(server)[1].split('/')[0]
    if "projects" in os.getcwd() :
        script = "/mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/ReSimAutoMng/Shell/rResim_main.sh"
    elif "PROJECTS" in os.getcwd() :
        script = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/ReSimAutoMng/Shell/rResim_main.sh"

    if 'bmwsp25' in simg:
        reqDoc = getChoice(1)
    elif 'stla_' in simg:
        reqDoc = getChoice(2)
    
    return script, simg, in_list


#**********************************************************
#          function to config dc docker
#**********************************************************
def docConfig(choice, value):
    global resimScript
    if choice == 1:
        if value == 1:
            option = 'ipnext_srr'
            resimScript = '/RUN_RESIM_SRR.sh'
        elif value == 2:
            option = 'ipnext_mrr'
            resimScript = '/RUN_RESIM_MRR.sh'
        elif value == 3:
            option = 'ipnext_srr_dgps'
            resimScript = '/RUN_RESIM_SRR_DGPS.sh'
        elif value == 4:
            option = 'ipnext_mrr_dgps'
            resimScript = '/RUN_RESIM_MRR_DGPS.sh'
    elif choice == 2:
        if value == 1:
            option = 'stla_veh'
            resimScript = '/RUN_RESIM.sh'
        elif value == 2:
            option = 'stla_dgps'
            resimScript = '/RUN_RESIM_DGPS.sh'
    return option


#**********************************************************
#          function to create slurm script to get resource and run job
#**********************************************************
def getChoice(choice):
    global dc_value
    if choice == 1:
        minVal=1
        maxVal=4
        menu='''1 > SRR_Vehicle\n2 > MRR_Vehicle\n3 > SRR_DGPS\n4 > MRR_DGPS\n'''
    elif choice == 2:
        minVal=1
        maxVal=2
        menu='''1 > Vehicle\n2 > DGPS\n'''
    print(menu, end='')
    dc_value=int(input('Enter Choice : '))
    if dc_value < minVal or dc_value > maxVal:
        print(f'[WARNING] : Invalid choice - {dc_value}, Please select again')
        getChoice(choice)
    return docConfig(choice, dc_value)
        


#**********************************************************
#          function to create slurm script to get resource and run job
#**********************************************************
def collect_input_files(path, sessions):
    resimScript = '/RUN_RESIM.sh'
    deb = []
    cbus = []
    mpad = []
    sbus = []
    ref = []
    json_paths = []
    cnt = sessions.index(path)
    path=path.split('\n')[0]
    if "resim_stla_scale1" in simg:
        deb = get_files(path, 'deb')
        cbus = get_files(path, 'bus')
        mpad = cbus

    elif "resim_stla_scale3" in simg or "resim_stla_scale4" in simg:
        deb = get_files(f'{path}/Resim_Radars_deb', 'deb')
        cbus = get_files(f'{path}/Resim_Radars_CAN_Corner_bus', 'bus')
        mpad = get_files(f'{path}/Aptiv_FLR_Obj_Perc_for_mPAD', 'mPAD')
        if "stla_scale3" in simg:
            sbus = cbus
        else:
            sbus = get_files(f'{path}/Resim_Radars_CAN_Side_bus', 'bus')

    elif 'dc' in simg:
        if 'bmwsp25' in simg:
            if 'ipnext_srr' in reqDoc:
                mpad = get_files(f'{path}/BN_RADETH', 'KPI_SRR_DS')
                ref = mpad
                if 'dgps' in reqDoc:
                    ref = get_files(f'{path}/MT_RE', 'KPI_SRR_DS')
            elif 'ipnext_mrr' in reqDoc:
                mpad = get_files(f'{path}/BN_RADETH', 'KPI_MRR_DS')
                ref = mpad
                if 'dgps' in reqDoc:
                    ref = get_files(f'{path}/MT_RE', 'KPI_MRR_DS')

        elif 'stla_' in simg:
            #print(' stla dc')
            deb = get_files(f'{path}', 'deb')
            sbus = ref = cbus = mpad = deb
            if 'dgps' in reqDoc:
                ref = get_files(f'{path}', 'ref')

    else:
        if "honda" in simg:
            key = 'HONDA_SRR6'
        if "traton" in simg:
            key = 'TRATON_SRR6'
        if "rnasdv" in simg or 'gpo-gen7' in simg:
            key = '_Aptiv.'
        deb = get_files(path, key)
        cbus = mpad = deb

    return deb, cbus, mpad, sbus, ref, cnt+1


"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
15/04/2025          Mandeep Singh       FHW-223     splitter for STLA_SCALE1 Resim(created 1st version of file )

######################################################################################################
"""
