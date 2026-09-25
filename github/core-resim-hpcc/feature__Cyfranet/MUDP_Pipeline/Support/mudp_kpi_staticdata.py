# -*- coding: utf-8 -*-
"""
MUDP + UDP_KPI Pipeline  -  Static Data & Configuration
Contains: cluster detection, path config, SLURM child script template
"""

import os
import sys
import getpass
import datetime
import re
from glob import glob

version = '1.0'

# ----------------------------------------------------------------
#  Runtime state  (populated by mudp_kpi_main.py functions)
# ----------------------------------------------------------------
input_parameter = []    # [orchestrator_sh, input_txt, project, exec_path, task_count]
all_b05_files   = []    # all b05 files collected from cache files
all_orcas_files = []    # all ORCAS .mf4 files found in output dirs
paired_b05      = []    # b05 files with a matching ORCAS file
paired_orcas    = []    # corresponding ORCAS files

# ----------------------------------------------------------------
#  Cluster / environment detection
# ----------------------------------------------------------------
_script_dir = os.path.dirname(os.path.abspath(__file__))
repo_root   = os.path.dirname(_script_dir)   # MUDP_Pipeline/

# Local config file for MUDP + UDP_KPI tools (user should edit these paths)
config_file = os.path.join(repo_root, 'Config', 'mudp_kpi_config.txt')

hpcc = os.getcwd()

if 'projects' in hpcc:
    cluster = 'Southfield'
    server  = 'projects/'
else:
    cluster = 'Krakow'
    server  = 'PROJECTS/'

# SLURM script placeholder tokens (replaced during childCreation)
_PLACEHOLDER_OUT = '#SBATCH -o PLACEHOLDER_OUT'
_PLACEHOLDER_ERR = '#SBATCH -e PLACEHOLDER_ERR'

# ----------------------------------------------------------------
#  SLURM child script template
#  {support_file}   -> path to support_files.txt (filled at creation time)
# ----------------------------------------------------------------
def mudp_kpi_child_script(mudp_config_path: str) -> str:
    return '''#!/bin/bash
#SBATCH --job-name=MUDP_KPI_Job
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=6
#SBATCH --mem=32G
#SBATCH --time=02:00:00
#SBATCH -o PLACEHOLDER_OUT
#SBATCH -e PLACEHOLDER_ERR

PIPE_START=$SECONDS
echo "[Pipeline_Execution] : starts at $PIPE_START"

# Normalize umask so file/dir permissions match interactive-shell runs
umask 0022

TASK_START="$1"
OUT="$2"

JOBOUT=$OUT/jobout
OUTPUT=$OUT/output
# Per-task log directory (created here so tool logs land in a clean subdir)
mkdir -p $JOBOUT/$SLURM_ARRAY_TASK_ID
mkdir -p $OUTPUT

# Main job output file (matches the #SBATCH -o path)
MAIN_LOG=${JOBOUT}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out

INPUT_CONFIG=''' + f"'{mudp_config_path}'" + '''
# tr also strips \\r (carriage return) - the config file is user-edited and may
# pick up Windows CRLF line endings, which would otherwise silently append an
# invisible \\r to the path (e.g. ".../resim_tool_mudp.simg\\r"), making every
# subsequent stat/singularity lookup fail with "no such file or directory"
# regardless of retries.
MUDP_SIMG=$(grep "^mudp_simg:" $INPUT_CONFIG | cut -d ":" -f2 | tr -d " \\r")
MUDP_CONFIG=$(grep "^mudp_config:" $INPUT_CONFIG | cut -d ":" -f2 | tr -d " \\r")
UDP_KPI_SIMG=$(grep "^udp_kpi_simg:" $INPUT_CONFIG | cut -d ":" -f2 | tr -d " \\r")

echo "[MUDP_KPI] : MUDP_SIMG=$MUDP_SIMG"
echo "[MUDP_KPI] : MUDP_CONFIG=$MUDP_CONFIG"
echo "[MUDP_KPI] : UDP_KPI_SIMG=$UDP_KPI_SIMG"

# ============================================================
#  wait_for_path - work around lazy NFS/autofs automount races.
#  This is typically the FIRST touch of these network export
#  paths in the job (unlike estd's pipeline, which only reaches
#  MUDP/UDP_KPI tens of seconds into the job after other steps
#  have already warmed the same mounts). A single "bash -f" test
#  can pass while singularity's own internal lstat (evaluated
#  when it snapshots its mount namespace) still returns ENOENT
#  ('could not open image ... no such file or directory').
#  Repeatedly stat'ing the path forces autofs to finish mounting
#  before we hand the path to singularity.
# ============================================================
wait_for_path(){
    local path=$1
    local retries=${2:-8}
    local delay=${3:-4}
    local i
    for (( i=1; i<=retries; i++ )); do
        if stat "$path" >/dev/null 2>&1; then
            return 0
        fi
        # Also nudge the parent directory - triggers autofs resolution
        ls -la "$(dirname "$path")" >/dev/null 2>&1
        sleep "$delay"
    done
    stat "$path" >/dev/null 2>&1
}

if ! wait_for_path "$MUDP_SIMG"; then
    echo "[ERROR] : MUDP simg not found or not reachable from this compute node: $MUDP_SIMG"
    exit 1
fi
if ! wait_for_path "$MUDP_CONFIG"; then
    echo "[ERROR] : MUDP config not found or not reachable from this compute node: $MUDP_CONFIG"
    exit 1
fi
# Warm the UDP_KPI mount early too, so it's already resolved by the time UDP_KPI() runs
wait_for_path "$UDP_KPI_SIMG" 3 2 >/dev/null 2>&1 || true

# Determine which task-specific list files to process
# TASK_LINE maps SLURM array task ID (1-based) + TASK_START to the file index
TASK_LINE=$(( $SLURM_ARRAY_TASK_ID + $TASK_START ))
ILIST_FILE="${JOBOUT}/${TASK_LINE}_iList.txt"
OLIST_FILE="${JOBOUT}/${TASK_LINE}_oList.txt"

echo "[MUDP_KPI] : TASK_LINE=$TASK_LINE"
echo "[MUDP_KPI] : ILIST_FILE=$ILIST_FILE"
echo "[MUDP_KPI] : OLIST_FILE=$OLIST_FILE"

if [[ ! -f "$ILIST_FILE" ]]; then
    echo "[ERROR] : iList file not found: $ILIST_FILE"
    exit 1
fi
if [[ ! -f "$OLIST_FILE" ]]; then
    echo "[ERROR] : oList file not found: $OLIST_FILE"
    exit 1
fi

echo "[MUDP_KPI] : Input files in iList  : $(wc -l < "$ILIST_FILE")"
echo "[MUDP_KPI] : Output files in oList : $(wc -l < "$OLIST_FILE")"

# Per-task output directories
TASK_OUTDIR=$OUTPUT/$SLURM_ARRAY_TASK_ID
MUDP_INPUT_DIR=$TASK_OUTDIR/MUDP_Input
MUDP_OUTPUT_DIR=$TASK_OUTDIR/MUDP_Output
UDP_KPI_DIR=$TASK_OUTDIR/UDP_KPI

mkdir -p $MUDP_INPUT_DIR
mkdir -p $MUDP_OUTPUT_DIR
mkdir -p $UDP_KPI_DIR

module load singularity/3.8.0

# ============================================================
#  run_singularity - retry wrapper for singularity exec/run.
#  Retries a few times if the image path was momentarily
#  unreachable (autofs race), instead of failing the whole task
#  on the first transient ENOENT.
#  $1 = log file, remaining args = full singularity command
# ============================================================
run_singularity(){
    local log=$1; shift
    local attempt exit_code
    for (( attempt=1; attempt<=3; attempt++ )); do
        echo "[SINGULARITY] : attempt $attempt/3 : $*" >> $log 2>&1
        "$@" >> $log 2>&1
        exit_code=$?
        if [[ $exit_code -eq 0 ]]; then
            return 0
        fi
        if grep -q "could not open image" "$log" 2>/dev/null; then
            echo "[SINGULARITY] : attempt $attempt failed (could not open image) - retrying after warm-up" >> $log 2>&1
            sleep 3
            continue
        fi
        break
    done
    return $exit_code
}


# ============================================================
#  MUDP function
#  $1 = list file (iList or oList)
#  $2 = output directory for MUDP results
#  $3 = label ("input" or "output")
# ============================================================
MUDP(){
    local list_file=$1
    local out_dir=$2
    local label=$3
    local log=$JOBOUT/$SLURM_ARRAY_TASK_ID/MUDP_${label}.txt

    echo "[MUDP_Execution] : $label - list file : $list_file"            >> $log 2>&1
    echo "[MUDP_Execution] : $label - output dir: $out_dir"             >> $log 2>&1
    echo "[MUDP_Execution] : running as uid=$(id -u) umask=$(umask)"    >> $log 2>&1

    if ! wait_for_path "$MUDP_SIMG"; then
        echo "[MUDP_Execution] : ERROR - MUDP simg still unreachable after retries: $MUDP_SIMG" >> $log 2>&1
    fi
    stat -c '[MUDP_Execution] : simg perms=%a owner=%U:%G size=%s' "$MUDP_SIMG" >> $log 2>&1

    echo "singularity exec $MUDP_SIMG /RUN_MUDP.sh $MUDP_CONFIG $list_file $out_dir/" >> $log 2>&1

    MUDP_START=$SECONDS
    echo "[MUDP_Execution] : starts at $MUDP_START" >> $log 2>&1

    run_singularity "$log" singularity exec "$MUDP_SIMG" /RUN_MUDP.sh "$MUDP_CONFIG" "$list_file" "$out_dir/"
    local exit_code=$?

    MUDP_END=$SECONDS
    echo "[MUDP_Execution] : exit code=$exit_code"                                      >> $log 2>&1
    echo "[MUDP_Execution] : ends at $MUDP_END"                                        >> $log 2>&1
    echo "[MUDP_Execution] : total time is $(( $MUDP_END - $MUDP_START )) seconds"     >> $log 2>&1
    echo " "                                                                            >> $log 2>&1
    return $exit_code
}

# Run MUDP on INPUT (b05 files) first; only proceed to OUTPUT if it succeeds
MUDP "$ILIST_FILE" "$MUDP_INPUT_DIR" "input"
MUDP_INPUT_STATUS=$?

MUDP_OUTPUT_STATUS=1
if [[ $MUDP_INPUT_STATUS -eq 0 ]]; then
    echo "[MUDP_KPI] : MUDP input done - freeing temp space before output run"
    MUDP "$OLIST_FILE" "$MUDP_OUTPUT_DIR" "output"
    MUDP_OUTPUT_STATUS=$?
else
    echo "[ERROR] : MUDP input run failed (exit=$MUDP_INPUT_STATUS) - skipping output run"
fi

# Merge MUDP logs into the main SLURM stdout file ($MAIN_LOG)
cat $JOBOUT/$SLURM_ARRAY_TASK_ID/MUDP_input.txt  >> $MAIN_LOG 2>/dev/null
cat $JOBOUT/$SLURM_ARRAY_TASK_ID/MUDP_output.txt >> $MAIN_LOG 2>/dev/null
rm -f $JOBOUT/$SLURM_ARRAY_TASK_ID/MUDP_input.txt
rm -f $JOBOUT/$SLURM_ARRAY_TASK_ID/MUDP_output.txt

if [[ $MUDP_INPUT_STATUS -ne 0 || $MUDP_OUTPUT_STATUS -ne 0 ]]; then
    echo "[ERROR] : One or more MUDP runs failed - skipping UDP_KPI"
    echo "[Pipeline_Execution] : ends at $SECONDS"
    echo "[Pipeline_Execution] : total pipeline execution time is $(( $SECONDS - $PIPE_START )) seconds"
    exit 1
fi


# ============================================================
#  UDP_KPI function
#  Pairs HDF5 files produced by MUDP_Input and MUDP_Output,
#  builds a JSON manifest, then runs the UDP_KPI singularity.
# ============================================================
UDP_KPI(){
    local udp_kpi_log=$JOBOUT/$SLURM_ARRAY_TASK_ID/UDP_KPI.txt

    echo "[UDP_KPI_Execution] : starts at $SECONDS" >> $udp_kpi_log 2>&1
    echo "[UDP_KPI_Execution] : UDP_KPI_SIMG=$UDP_KPI_SIMG" >> $udp_kpi_log 2>&1

    if ! wait_for_path "$UDP_KPI_SIMG"; then
        echo "[UDP_KPI_Execution] : ERROR - UDP KPI simg not found: $UDP_KPI_SIMG" >> $udp_kpi_log 2>&1
        return 1
    fi

    # Pair .h5 files from MUDP_Input and MUDP_Output by stem (strip trailing _rXXXXXXXX version tag)
    python3 -c "
import json, os, glob, re, sys

input_dir  = '$MUDP_INPUT_DIR'
output_dir = '$MUDP_OUTPUT_DIR'
udp_out    = '$UDP_KPI_DIR'
task_id    = '$SLURM_ARRAY_TASK_ID'

# Collect all HDF5 files recursively
all_input  = sorted(glob.glob(os.path.join(input_dir,  '**', '*.h5'), recursive=True))
all_output = sorted(glob.glob(os.path.join(output_dir, '**', '*.h5'), recursive=True))

# Build output lookup keyed by base stem (strip trailing version tag _rXXXXXXXX)
out_by_stem = {}
for fp in all_output:
    stem = re.sub(r'_r[A-Za-z]*[0-9]+$', '', os.path.splitext(os.path.basename(fp))[0])
    out_by_stem[stem] = fp

paired_input, paired_output = [], []
unmatched = []
for fp in all_input:
    stem = os.path.splitext(os.path.basename(fp))[0]
    if stem in out_by_stem:
        paired_input.append(fp)
        paired_output.append(out_by_stem[stem])
    else:
        unmatched.append(os.path.basename(fp))

print(f'[UDP_KPI_PREP] : {len(all_input)} input HDF / {len(all_output)} output HDF found')
print(f'[UDP_KPI_PREP] : {len(paired_input)} pairs matched')
if unmatched:
    print(f'[UDP_KPI_PREP] : WARNING - {len(unmatched)} unmatched input HDF: {unmatched[:5]}')

if not paired_input:
    print('[UDP_KPI_PREP] : ERROR - no paired HDF files found, skipping UDP_KPI')
    sys.exit(1)

udp_kpi_json = {'INPUT_HDF': paired_input, 'OUTPUT_HDF': paired_output}
json_path = os.path.join(udp_out, f'UDP_KPI_input_{task_id}.json')
with open(json_path, 'w') as jf:
    json.dump(udp_kpi_json, jf, indent=4)
print(f'[UDP_KPI_PREP] : JSON written to {json_path} ({len(paired_input)} pairs)')
" >> $udp_kpi_log 2>&1

    UDP_KPI_JSON=$UDP_KPI_DIR/UDP_KPI_input_${SLURM_ARRAY_TASK_ID}.json
    if [[ -f "$UDP_KPI_JSON" ]]; then
        echo "[UDP_KPI_Execution] : Running: singularity run $UDP_KPI_SIMG json $UDP_KPI_JSON $UDP_KPI_DIR" >> $udp_kpi_log 2>&1
        UDP_KPI_EXEC_START=$SECONDS
        run_singularity "$udp_kpi_log" singularity run "$UDP_KPI_SIMG" json "$UDP_KPI_JSON" "$UDP_KPI_DIR"
        echo "[UDP_KPI_Execution] : exit code=$?" >> $udp_kpi_log 2>&1
        echo "[UDP_KPI_Execution] : execution time = $(( $SECONDS - $UDP_KPI_EXEC_START )) seconds" >> $udp_kpi_log 2>&1
    else
        echo "[UDP_KPI_Execution] : ERROR - UDP_KPI JSON not created, skipping execution" >> $udp_kpi_log 2>&1
    fi

    UDP_KPI_END=$SECONDS
    echo "[UDP_KPI_Execution] : ends at $UDP_KPI_END"                                         >> $udp_kpi_log 2>&1
    echo "[UDP_KPI_Execution] : total UDP_KPI time is $(( $UDP_KPI_END - $SECONDS )) seconds" >> $udp_kpi_log 2>&1
    echo " "                                                                                   >> $udp_kpi_log 2>&1
}

if [[ -n "$UDP_KPI_SIMG" ]]; then
    UDP_KPI
    cat $JOBOUT/$SLURM_ARRAY_TASK_ID/UDP_KPI.txt >> $MAIN_LOG 2>/dev/null
    rm -f $JOBOUT/$SLURM_ARRAY_TASK_ID/UDP_KPI.txt
else
    echo "[UDP_KPI_Execution] : UDP_KPI not configured (udp_kpi_simg not set in support_files.txt)"
fi

echo "[Pipeline_Execution] : ends at $SECONDS"
echo "[Pipeline_Execution] : total pipeline execution time is $(( $SECONDS - $PIPE_START )) seconds"
'''


# ----------------------------------------------------------------
#  Convenience: read a key from support_files.txt
# ----------------------------------------------------------------
def read_support_key(key: str, filepath: str) -> str:
    if not os.path.isfile(filepath):
        return ''
    with open(filepath, 'r') as fh:
        for line in fh:
            line = line.strip()
            if line.startswith(f'{key}:'):
                return line.split(':', 1)[1].strip()
    return ''


# ----------------------------------------------------------------
#  ASCII banner
# ----------------------------------------------------------------
banner = f'''
\t**********************************************************************
\t*           ___       ______  _________ _________ __          __    *
\t*          / _ \\     |  __  | \\__   __/ \\__   __/ \\ \\        / /    *
\t*         / / \\ \\    | |__| |    | |       | |     \\ \\      / /     *
\t*        / /___\\ \\   |  ____|    | |       | |      \\ \\    / /      *
\t*       / ______\\ \\  | |         | |     __| |__     \\ \\__/ /       *
\t*      /_/       \\_\\ |_|         |_|    /_______\\     \\____/        *
\t**********************************************************************
\t*                                                                    *
\t*     MUDP + UDP_KPI Standalone Pipeline   (ver {version})               *
\t*                                                                    *
\t**********************************************************************
'''
