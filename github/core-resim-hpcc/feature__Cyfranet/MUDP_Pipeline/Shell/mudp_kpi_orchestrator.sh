#!/bin/bash
# ============================================================
#  MUDP + UDP_KPI Pipeline  -  SLURM Orchestrator
#  Called by mudp_kpi_main.py via os.system()
#  Receives all input_parameter values as a single CSV string
# ============================================================

IFS="," read -r -a my_values <<< "$@"
# my_values[0] = this script path (orchestrator)
# my_values[1] = input txt path
# my_values[2] = SLURM project
# my_values[3] = execution output path (RESULTS-KPI/date/time)
# my_values[4] = total task count

EXECUTION_PROJECT=${my_values[2]}
OUT=${my_values[3]}
TOTAL_TASKS=${my_values[4]}

JOBOUT="$OUT/jobout"
OUTPUT="$OUT/output"
CHILD="$JOBOUT/.mudp_kpi_child.sh"

# Helios shares storage with Krakow but requires its own SLURM account/partition.
# Detected via the "8k3" identifier present in the HPCC username or the account
# name resolved by the python pipeline (8k3p89).
PARTITION="-p highPrio"
if [[ $USER == *'8k3'* || $USER == *'8K3'* || $EXECUTION_PROJECT == "8k3p89" ]]; then
    PARTITION="-p 8k3"
fi
echo "partition requested : $PARTITION"

echo "PROJECT   : $EXECUTION_PROJECT"  >> $OUT/MUDP_KPI_details.txt
echo "OUTPUT    : $OUTPUT"             >> $OUT/MUDP_KPI_details.txt
echo "JOBOUT    : $JOBOUT"             >> $OUT/MUDP_KPI_details.txt
echo "TASKS     : $TOTAL_TASKS"        >> $OUT/MUDP_KPI_details.txt
echo ""                                >> $OUT/MUDP_KPI_details.txt


############################################################
#  Submit SLURM array job
############################################################
echo "*********** Job Related Details ***********" >> $OUT/MUDP_KPI_details.txt
echo "Task(s) requested : $TOTAL_TASKS"            >> $OUT/MUDP_KPI_details.txt

# Submit one array job covering all tasks (1-based indexing)
# TASK_START=0 means task N reads file N from jobout
TASK_START=0
JOB_ID=$(sbatch -A $EXECUTION_PROJECT \
               $PARTITION \
               -a "1-${TOTAL_TASKS}%${TOTAL_TASKS}" \
               $CHILD $TASK_START $OUT \
        | awk '{print $NF}')

echo "MUDP_KPI Job Id : $JOB_ID" >> $OUT/MUDP_KPI_details.txt
echo ""                          >> $OUT/MUDP_KPI_details.txt


############################################################
#  Monitor progress
############################################################
JOBID_STATUS="PENDING     "

echo ""
echo "[INFO] : Pipeline Triggered  MUDP_KPI_Job: $JOB_ID"
echo     "          ___________________________________________________ "
echo     "         | Job Status  |  MUDP_Docker  |  UDP_KPI  |        |"
echo     "         |-------------|---------------|-----------|--------|"
echo -ne "         | Submitted   |  $JOBID_STATUS |  WAITING  |        |" \\r

while : ; do
    job_status=$(squeue -j "${JOB_ID}" 2>/dev/null)

    if [[ $job_status = *${JOB_ID}* ]]; then
        if [[ $job_status = *'  R'* ]]; then
            JOBID_STATUS="RUNNING     "
            echo -ne "         | In Progress |  $JOBID_STATUS |  RUNNING  |        |" \\r
        fi
    else
        JOBID_STATUS="COMPLETED   "
        echo -ne "         | In Progress |  $JOBID_STATUS |  DONE     |        |" \\r
        break
    fi
    sleep 5
done

echo -ne "         | Done        |  $JOBID_STATUS |  DONE     |  POST  |" \\r
rm -f "$CHILD"
echo ""
echo "         | Done        |  $JOBID_STATUS |  DONE     |  COMPLETED  |"
echo "         |_____________|_______________|___________|_____________|"
echo ""
echo "[INFO] : Results available at: $OUTPUT"
