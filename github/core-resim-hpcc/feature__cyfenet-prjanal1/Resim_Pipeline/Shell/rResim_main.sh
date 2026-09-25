#!/bin/bash

IFS="," read -r -a my_values <<< "$@"
SIMG=${my_values[2]}
CUST=${my_values[3]}
EXECUTION_PROJECT=${my_values[4]}
EXECUTION_MODE=${my_values[5]}
CONFIG=${my_values[6]}
OUT=${my_values[7]}
highPrio=${my_values[8]}
UPU=${my_values[9]}
BUS_TAG=${my_values[10]:-b04}
RM_ZERO=${my_values[11]:-False}

if [[ $highPrio == "True" ]];then
    PARTITION="-p highPrio"
else 
    PARTITION=""
fi

# Helios shares storage with Krakow but requires its own SLURM account/partition.
# The 8k3p89 account must be paired with the 8k3 partition.
MINING_PARTITION="highPrio"
if [[ $EXECUTION_PROJECT == "8k3p89" ]]; then
    PARTITION="-p 8k3"
    MINING_PARTITION="8k3"
fi

echo "partition requested : $PARTITION"

JOBOUT="$OUT/jobout"
OUTPUT="$OUT/output"

INPUT_FILE="$JOBOUT/SIL_Input_all.txt"
CHILD="$JOBOUT/.resim.sh"
RMIN="$JOBOUT/.rming.sh"
SMIN="$JOBOUT/.sming.sh"


echo "SIMG	  : $SIMG" >> $OUT/RESIM_details.txt
echo "CUST      : $CUST" >> $OUT/RESIM_details.txt
echo "PROJECT	  : $EXECUTION_PROJECT" >> $OUT/RESIM_details.txt
echo "MODE	  : $EXECUTION_MODE" >> $OUT/RESIM_details.txt
echo "CONFIG	  : $CONFIG" >> $OUT/RESIM_details.txt
echo "OUTPUT	  : $OUTPUT" >> $OUT/RESIM_details.txt
echo "INPUT	  : $INPUT_FILE" >> $OUT/RESIM_details.txt
echo "CHILD	  : $CHILD" >> $OUT/RESIM_details.txt
echo "RMining	  : $RMIN" >> $OUT/RESIM_details.txt
echo "SMining	  : $SMIN" >> $OUT/RESIM_details.txt
echo "highPrio  : $highPrio" >> $OUT/RESIM_details.txt
echo "UPU FS    : $UPU" >> $OUT/RESIM_details.txt
echo "BUS_TAG   : $BUS_TAG" >> $OUT/RESIM_details.txt
echo "RM_ZERO   : $RM_ZERO" >> $OUT/RESIM_details.txt
echo "" >> $OUT/RESIM_details.txt
echo "" >> $OUT/RESIM_details.txt


############################################################################################################################
#********************* main section to configure jobs


echo "*********** Job Related Details ***********" >> $OUT/RESIM_details.txt

TASKS=0

TASKS=$(wc <"$INPUT_FILE" -l)

if [ -z "$TASKS" ] || [ "$TASKS" -eq 0 ] 2>/dev/null; then
    echo "[ERROR] : No tasks found in $INPUT_FILE (0 lines) - no jobs submitted." | tee -a $OUT/RESIM_details.txt
    echo "[ERROR] : Check that each session path in the input list resolves to files (no stray CR/whitespace, valid path)." | tee -a $OUT/RESIM_details.txt
    exit 1
fi

echo "Job(s)  requested	: 1" >> $OUT/RESIM_details.txt
echo "Task(s) requested	: $TASKS" >> $OUT/RESIM_details.txt
echo "TASKS ARRAY		: 1 - ${TASKS}" >> $OUT/RESIM_details.txt

# All tasks submitted as a single array job (TASK_START offset of 0 for resim_child.py's
# SLURM_ARRAY_TASK_ID + TASK_START line-number lookup); no chunking/splitting needed.
JOB_ID=$(sbatch -A $EXECUTION_PROJECT $PARTITION -a "1-$TASKS%$TASKS" $CHILD 0 $SIMG $OUT $UPU $CONFIG | awk '{print $NF}')

JOB_ID1=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p $MINING_PARTITION $RMIN $OUTPUT/.mining.txt $CUST $CUST | awk '{print $NF}')
JOB_ID2=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p $MINING_PARTITION $SMIN $OUTPUT/.SIL_Statistics.txt | awk '{print $NF}')

echo "" >> $OUT/RESIM_details.txt
echo "*********** JobId Related Details ***********" >> $OUT/RESIM_details.txt
echo "ReSim Job Id		: $JOB_ID" >> $OUT/RESIM_details.txt
echo "RMining Job Id		: $JOB_ID1" >> $OUT/RESIM_details.txt
echo "SMining Job Id		: $JOB_ID2" >> $OUT/RESIM_details.txt


############################################################################################################################
#********************* section for jobs status

JOBID_STATUS="PENDING     "
JOBID1_STATUS="PENDING     "
JOBID2_STATUS="PENDING     "

echo ""
echo "[INFO] : PipeLine Triggered  ReSIm_docker: $JOB_ID | ReSim_Mining: $JOB_ID1 | Stats_Mining: $JOB_ID2"
echo     "          _________________________________________________________________________________ "
echo     "         | Job Status  |  ReSim_Docker |  ReSim_Mining |  Stats_Mining |  Post_Processing  |"
echo     "         |-------------|---------------|---------------|---------------|-------------------|"
echo -ne "         | Submitted   |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r

while : ; do

	job_status=$( squeue -j "${JOB_ID}" --noheader 2>/dev/null )

	if [[ -n "$job_status" ]]; then
		if [[ $job_status = *'  R'* ]]; then
			JOBID_STATUS="RUNNING     "
                        echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
		fi
	else
		JOBID_STATUS="COMPLETED   "
		echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
		break
	fi
	sleep 5
done
while : ; do

	job_status=$( squeue -j "${JOB_ID1}" )

	if [[ $job_status = *${JOB_ID1}* ]]; then
		if [[ $job_status = *'  R'* ]]; then
			JOBID1_STATUS="RUNNING     "
                        echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
		fi
	else
		JOBID1_STATUS="COMPLETED   "
                echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
		break
	fi
	sleep 5
done
while : ; do

	job_status=$( squeue -j "${JOB_ID2}" )

	if [[ $job_status = *${JOB_ID2}* ]]; then
		if [[ $job_status = *'  R'* ]]; then
			JOBID2_STATUS="RUNNING     "
                        echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
		fi
	else
		JOBID2_STATUS="COMPLETED   "
                echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
		break
	fi
	sleep 5
done

echo -ne "         | Done        |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     PROCESSING    |" \\r

rm "$CHILD"
rm "$RMIN"
rm "$SMIN"

echo "         | Done        |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     COMPLETED     |"
echo "         |_____________|_______________|_______________|_______________|___________________|"
echo ""