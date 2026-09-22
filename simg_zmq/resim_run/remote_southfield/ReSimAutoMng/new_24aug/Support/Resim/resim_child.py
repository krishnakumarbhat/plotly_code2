#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

###########################################################
from resim_splitter import *
from resim_staticdata import load_simg, simg_exec
###########################################################


def resim_child_script():
    return '''#!/bin/bash
#SBATCH --job-name=ReSimJob
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=6  
#SBATCH --mem=80G
#SBATCH --time=12:00:00
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
    YIELD_CUST="STLA-SMALL"

elif [[ $SIMG_NAME == *'_dgps'* || $SIMG_NAME == *'dgps'* ]]; then
    YIELD_CUST="DGPS"

elif [[ $SIMG_NAME == *'v2'* && $SIMG_NAME == *'platform'* && $SIMG_NAME != *'dgps'* ]]; then
    YIELD_CUST="GPO-V2"

elif [[ $SIMG_NAME == *'mcip'* ]]; then
    YIELD_CUST="MCIP"

elif [[ $SIMG_NAME == *'ifv600_'* ]]; then
    YIELD_CUST="ADCAM"

else
    YIELD_CUST="DEFAULT"
fi

echo "[M-INFO] : YIELD_CUST=$YIELD_CUST"

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
    # Use SESSION_BASE (common to all files in batch) as output root
    # This preserves folder structure for batches with multiple subfolders
    # Extract only the top-level session folder (e.g., PT037 from .../VSYS4/PT037)
    child=$(basename "$SESSION_BASE")
    echo "[M-INFO] : output child folder (session-level): $child"
    echo "[M-INFO] : full session path: $SESSION_BASE"
    echo "[M-INFO] : batch will preserve all subfolders: 0000, 0001, etc."
else
    child=$(basename "$DIR")
    echo "[M-INFO] : output child folder: $child"
    echo "[M-WARNING] : SESSION_BASE not provided; using directory basename"
fi


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

'''+f"{load_simg}"+'''
SIMG_START=$SECONDS
echo "[Resim_Execution] : starts at $SIMG_START"
echo "$JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out" >> $OUTPUT/.mining.txt

if [[ $YIELD_CUST == "ADCAM" ]]; then
    # ADCAM docker (ifv600_*.simg): singularity run (not exec) - takes the JSON
    # and an output path directly, no RUN_RESIM.sh wrapper script or CONFIG arg.
    echo "[Resim_Execution] : ADCAM mode - singularity run $SIMG $INPUTFILE $TEMPDIR"
    singularity run $SIMG $INPUTFILE $TEMPDIR
else
    '''+f"{simg_exec}"+''' $SIMG''' + f''' {resimScript}''' + ''' $INPUTFILE $TEMPDIR $CONFIG
fi

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
# HTML/BORDNET/MUDP/KPI tools removed - pipeline runs Resim + Video only

echo "$OUTPUT/${child}"/*.xml >> $OUTPUT/.SIL_Statistics.txt

VIDEO(){
VIDEO_START=$SECONDS
VIDEO_LOG=$JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
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
    if [[ "$YIELD_CUST" == "DGPS" || "$YIELD_CUST" == "GPO-V2" || "$YIELD_CUST" == "MCIP" ]]; then
        # DGPS/GPO-V2/MCIP: create separate clean JSONs per g02 file from pre-cached list
        python3 -c "
import json, os, re, sys
jobout = '$JOBOUT'
video_out = '$VIDEO_OUT'
json_file = '$INPUTFILE'
yield_cust = '$YIELD_CUST'

# Read g02 files from the pre-cached list for this docker type
g02_cache_names = {'DGPS': 'dgps_g02_cache.txt', 'GPO-V2': 'gpo_v2_g02_cache.txt', 'MCIP': 'mcip_g02_cache.txt'}
v01_cache_names = {'DGPS': 'dgps_v01_cache.txt', 'GPO-V2': 'gpo_v2_v01_cache.txt', 'MCIP': 'mcip_v01_cache.txt'}
g02_cache = os.path.join(jobout, g02_cache_names.get(yield_cust, 'dgps_g02_cache.txt'))
if os.path.isfile(g02_cache):
    with open(g02_cache) as f:
        g02_files = sorted([l.strip() for l in f if l.strip()])
    print(f'[VIDEO_PREP] : {yield_cust} - loaded ' + str(len(g02_files)) + ' g02 files from cache')
else:
    print(f'[VIDEO_PREP] : {yield_cust} - g02 cache not found at ' + g02_cache)
    g02_files = []

if not g02_files:
    # Fallback: no g02 files available for this session, use v01 files instead
    v01_cache = os.path.join(jobout, v01_cache_names.get(yield_cust, 'dgps_v01_cache.txt'))
    if os.path.isfile(v01_cache):
        with open(v01_cache) as f:
            g02_files = sorted([l.strip() for l in f if l.strip()])
        print(f'[VIDEO_PREP] : {yield_cust} - no g02 files, falling back to ' + str(len(g02_files)) + ' v01 files from cache')
    else:
        print(f'[VIDEO_PREP] : {yield_cust} - no g02 files; v01 cache not found at ' + v01_cache)

if not g02_files:
    print(f'[VIDEO_PREP] : {yield_cust} - no g02 or v01 files, skipping video')
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

print(f'[VIDEO_PREP] : {yield_cust} - created {total_jsons} separate converter JSONs in {video_out}')
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
        # Non-DGPS/GPO-V2/MCIP: use existing resim JSON directly
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


if [[ -n $CONVERTER_SIMG && -n $CONVERTER_CONFIG ]]; then
    VIDEO
else
    echo "[VIDEO_Execution] : VIDEO not configured (converter_simg='$CONVERTER_SIMG' converter_config='$CONVERTER_CONFIG')"
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
#SBATCH --mem=32G
#SBATCH --time=03:00:00
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
SPLITTER='''+f"{splitter_path}"+'''
BUS_TAG='''+f"{bus_tag}"+'''
RM_ZERO='''+f"{str(rm_zero).lower()}"+'''
echo "[M-INFO] : BUS_TAG=$BUS_TAG"
echo "[M-INFO] : RM_ZERO=$RM_ZERO"
CONVERTER_SIMG=`grep "^converter_simg:" $INPUT_SUPPORT | cut -d ":" -f2`
CONVERTER_CONFIG=`grep -P "^converter_config:" $INPUT_SUPPORT | grep -v "_dgps" | cut -d ":" -f2`
CONVERTER_CONFIG_DGPS=`grep "^converter_config_dgps:" $INPUT_SUPPORT | cut -d ":" -f2`
SIMG_LOWER=$(echo "$SIMG" | tr '[:upper:]' '[:lower:]')
if [[ $SIMG_LOWER == *'_dgps'* || $SIMG_LOWER == *'dgps'* ]]; then
    CONVERTER_CONFIG=$CONVERTER_CONFIG_DGPS
fi
echo "[VIDEO_Config] : CONVERTER_SIMG=$CONVERTER_SIMG"
echo "[VIDEO_Config] : CONVERTER_CONFIG=$CONVERTER_CONFIG"

if [[ $SIMG == *'stla_small'* ]]; then
    YIELD_CUST="STLA-SMALL"

elif [[ $SIMG_LOWER == *'_dgps'* || $SIMG_LOWER == *'dgps'* ]]; then
    YIELD_CUST="DGPS"

elif [[ $SIMG_LOWER == *'v2'* && $SIMG_LOWER == *'platform'* && $SIMG_LOWER != *'dgps'* ]]; then
    YIELD_CUST="GPO-V2"

elif [[ $SIMG_LOWER == *'mcip'* ]]; then
    YIELD_CUST="MCIP"

elif [[ $SIMG_LOWER == *'ifv600_'* ]]; then
    YIELD_CUST="ADCAM"

else
    YIELD_CUST="DEFAULT"
fi

echo "[M-INFO] : YIELD_CUST=$YIELD_CUST"

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
    # Use SESSION_BASE (common to all files in batch) as output root
    # This preserves folder structure for batches with multiple subfolders,
    # and keeps ALL batches of the same session writing to the SAME session-level
    # output folder instead of a folder named after only the first log's subfolder.
    SESSION_STRIP=$(dirname "$(dirname "$SESSION_BASE")")
    child="${SESSION_BASE#${SESSION_STRIP}/}"
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

TMPOUT=$TEMPDIR/RESIM-RESULTS/
mkdir -p $TMPOUT 
JSON_FILE="$JOBOUT/SIL_input_${SLURM_ARRAY_TASK_ID}.json"
$SPLITTER "${flog[0]}" "${flog[1]}" $TEMPDIR $JSON_FILE $YIELD_CUST "$SESSION_BASE" "$BUS_TAG" "$RM_ZERO"

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

'''+f"{load_simg}"+'''
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
    if [[ $YIELD_CUST == "ADCAM" ]]; then
        # ADCAM docker (ifv600_*.simg): singularity run (not exec) - takes the
        # JSON and an output path directly, no RUN_RESIM.sh wrapper or CONFIG arg.
        echo "[Resim_Execution] : ADCAM mode - singularity run $SIMG $RESIM_JSON $TMPOUT" >> $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>&1
        singularity run $SIMG $RESIM_JSON $TMPOUT >> $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>&1
    else
        '''+f"{simg_exec}"+''' $SIMG''' + f''' {resimScript}''' + ''' $RESIM_JSON $TMPOUT $CONFIG >> $TMPOUT/${SLURM_ARRAY_TASK_ID}_resim.txt 2>&1
    fi
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
# First pass: filter the SRR_DEBUG stream by exact path (this is the only stream
# whose paths actually appear in 'processed') and collect the surviving stems.
for stream in data.get('reprocessingInputFileStreams', []):
    if stream.get('key') != 'SRR_DEBUG':
        continue
    files = stream.get('files', [])
    new_files = [f for f in files if f not in processed]
    removed = len(files) - len(new_files)
    remaining = len(new_files)
    for fp in new_files:
        stem = re.sub(r'_[A-Za-z0-9]+\.MF4$', '', os.path.basename(fp), flags=re.IGNORECASE)
        remaining_stems.add(stem.lower())
    print(f'[CRASH_RECOVERY] : SRR_DEBUG: {len(files)} -> {remaining} files (removed {removed} processed/crashed)')
    stream['files'] = new_files
    break
# Second pass: filter BN_CALIFR / BN_FASETH (bus) and SRR_REFERENCE (g03) streams
# by matching each file's stem against the remaining SRR_DEBUG stems, instead of
# comparing full paths against 'processed' (which never contains bus/reference
# paths). Without this, bus/reference files for the crashed/processed log were
# never dropped, leaving those streams out of sync (extra file) with SRR_DEBUG.
for stream in data.get('reprocessingInputFileStreams', []):
    key = stream.get('key')
    if key == 'SRR_DEBUG':
        continue
    files = stream.get('files', [])
    if not files:
        continue
    if key in ('BN_CALIFR', 'BN_FASETH', 'SRR_REFERENCE'):
        new_files = []
        for fp in files:
            stem = re.sub(r'_[A-Za-z0-9]+\.MF4$', '', os.path.basename(fp), flags=re.IGNORECASE)
            if stem.lower() in remaining_stems:
                new_files.append(fp)
    else:
        new_files = [f for f in files if f not in processed]
    removed = len(files) - len(new_files)
    print(f'[CRASH_RECOVERY] : {key}: {len(files)} -> {len(new_files)} files (removed {removed})')
    stream['files'] = new_files
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


###********************* pipeline Script section ****************
# HTML/BORDNET/MUDP/UDP_KPI tools removed - pipeline runs Resim + Video only

# Copy resim results to final output location
echo "[M-INFO] : copying resim results to output location"
cp -r $TEMPDIR/RESIM-RESULTS/* $OUTPUT/$child
echo "[M-INFO] : resim results copied to $OUTPUT/$child"

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
    if [[ "$YIELD_CUST" == "DGPS" || "$YIELD_CUST" == "GPO-V2" || "$YIELD_CUST" == "MCIP" ]]; then
        # DGPS/GPO-V2/MCIP: create separate clean JSONs per g02 file from pre-cached list
        python3 -c "
import json, os, re, sys
jobout = '$JOBOUT'
video_out = '$VIDEO_OUT'
json_file = '$JSON_FILE'
yield_cust = '$YIELD_CUST'

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
print(f'[VIDEO_PREP] : {yield_cust} - b05 stems for this task: {len(b05_stems)}')

# Read g02 files from the pre-cached list for this docker type
g02_cache_names = {'DGPS': 'dgps_g02_cache.txt', 'GPO-V2': 'gpo_v2_g02_cache.txt', 'MCIP': 'mcip_g02_cache.txt'}
v01_cache_names = {'DGPS': 'dgps_v01_cache.txt', 'GPO-V2': 'gpo_v2_v01_cache.txt', 'MCIP': 'mcip_v01_cache.txt'}

def _load_cache(path):
    if os.path.isfile(path):
        with open(path) as f:
            return sorted([l.strip() for l in f if l.strip()])
    return None

def _filter_by_stems(files, suffix_regex):
    if not b05_stems:
        return list(files)
    matched = []
    for fp in files:
        fn_stem = re.sub(suffix_regex, '', os.path.basename(fp), flags=re.IGNORECASE).lower()
        if fn_stem in b05_stems:
            matched.append(fp)
    matched.sort()
    return matched

g02_cache = os.path.join(jobout, g02_cache_names.get(yield_cust, 'dgps_g02_cache.txt'))
all_g02 = _load_cache(g02_cache)
if all_g02 is None:
    print(f'[VIDEO_PREP] : {yield_cust} - g02 cache not found at ' + g02_cache)
    all_g02 = []
else:
    print(f'[VIDEO_PREP] : {yield_cust} - loaded ' + str(len(all_g02)) + ' g02 files from cache')

g02_files = _filter_by_stems(all_g02, r'_g02\.mf4$')
if all_g02:
    print(f'[VIDEO_PREP] : {yield_cust} - matched {len(g02_files)} g02 files for this task')

if not g02_files:
    # Fallback: no g02 files for this task, use v01 files instead
    v01_cache = os.path.join(jobout, v01_cache_names.get(yield_cust, 'dgps_v01_cache.txt'))
    all_v01 = _load_cache(v01_cache)
    if all_v01 is None:
        print(f'[VIDEO_PREP] : {yield_cust} - no g02 files; v01 cache not found at ' + v01_cache)
        all_v01 = []
    else:
        print(f'[VIDEO_PREP] : {yield_cust} - no g02 files; loaded ' + str(len(all_v01)) + ' v01 files from cache')
    g02_files = _filter_by_stems(all_v01, r'_v01\.mf4$')
    if all_v01:
        print(f'[VIDEO_PREP] : {yield_cust} - matched {len(g02_files)} v01 files for this task (fallback)')

if not g02_files:
    print(f'[VIDEO_PREP] : {yield_cust} - no g02 or v01 files, skipping video')
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

print(f'[VIDEO_PREP] : {yield_cust} - created {total_jsons} separate converter JSONs in {video_out}')
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
        # Non-DGPS/GPO-V2/MCIP: use existing resim JSON directly
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


if [[ -n $CONVERTER_SIMG && -n $CONVERTER_CONFIG ]]; then
    VIDEO
    cat $TMPOUT/${SLURM_ARRAY_TASK_ID}_VIDEO.txt >> $JOBOUT/${SLURM_ARRAY_TASK_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out
    rm -f $TMPOUT/${SLURM_ARRAY_TASK_ID}_VIDEO.txt
else
    echo "[VIDEO_Execution] : VIDEO not configured (converter_simg='$CONVERTER_SIMG' converter_config='$CONVERTER_CONFIG')"
fi

echo "$OUTPUT/$child"/*.xml >> $OUTPUT/.SIL_Statistics.txt

cp -r $TEMPDIR/RESIM-RESULTS/* $OUTPUT/$child
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
        script = "/mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/ReSim_Pipeline/Shell/rResim_main.sh"
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
