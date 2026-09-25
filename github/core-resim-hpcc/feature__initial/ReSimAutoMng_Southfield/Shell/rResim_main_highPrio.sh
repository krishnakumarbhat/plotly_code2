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


if [[ $highPrio == "True" ]];then
    PARTITION="-p highPrio"
else 
    PARTITION=""
fi

echo "partition requested : $PARTITION"

JOBOUT="$OUT/jobout"
OUTPUT="$OUT/output"

INPUT_FILE="$JOBOUT/SIL_Input_all.txt"
CHILD="$JOBOUT/.resim.sh"
RMIN="$JOBOUT/.rming.sh"
SMIN="$JOBOUT/.sming.sh"


echo "SIMG	  : $SIMG" >> $OUT/RESIM_details.txt
echo "CUST        : $CUST" >> $OUT/RESIM_details.txt
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

echo "" >> $OUT/RESIM_details.txt
echo "" >> $OUT/RESIM_details.txt


############################################################################################################################
#********************* main section to configure jobs


echo "*********** Job Related Details ***********" >> $OUT/RESIM_details.txt

TASKS=0

TASKS=$(wc <"$INPUT_FILE" -l)
MAX_TASKS=$(( ${TASKS} + 1 ))
JOBS=$((${TASKS} / ${MAX_TASKS}))

echo "Job(s)  requested	: $(( $JOBS+1 ))" >> $OUT/RESIM_details.txt
echo "Task(s) requested	: $TASKS" >> $OUT/RESIM_details.txt

for (( i = 0; i <= JOBS; i++ )); do
    TASK_START=$((${i} * ${MAX_TASKS}))
    TASK_STOP=$(( ${TASK_START} + ${MAX_TASKS}))
    TASK_LENGTH=$(( ${TASK_STOP} - ${TASK_START}))
    if [ $TASK_STOP -gt ${TASKS} ];then
      TASK_LENGTH=$(( ${TASKS} % ${MAX_TASKS}))
      #echo "TASK Length	:$TASK_LENGTH" >> $OUT/RESIM_details.txt
      TASK_STOP=$(( ${TASK_START} + ${TASK_LENGTH}))
      #echo new array stop is $TASK_STOP >> $OUT/RESIM_details.txt
    fi
    #echo "Start Task Id	: $(( $TASK_START+1 ))" >> $OUT/RESIM_details.txt
    #echo "Stop Task Id		: $TASK_STOP" >> $OUT/RESIM_details.txt
    echo "TASKS ARRAY		: $(( ${TASK_START} + 1)) - ${TASK_STOP}" >> $OUT/RESIM_details.txt

    #echo "sbatch -A $EXECUTION_PROJECT -a 1-$TASK_LENGTH%$TASK_LENGTH $CHILD $TASK_START $SIMG $OUT $UPU $CONFIG"
    JOB_ID=$(sbatch -A $EXECUTION_PROJECT $PARTITION -a "1-$TASK_LENGTH%$TASK_LENGTH" $CHILD $TASK_START $SIMG $OUT $UPU $CONFIG | awk '{print $NF}')
done

JOB_ID1=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p highPrio $RMIN $OUTPUT/.mining.txt $CUST $CUST | awk '{print $NF}')
JOB_ID2=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p highPrio $SMIN $OUTPUT/.SIL_Statistics.txt | awk '{print $NF}')

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

	job_status=$( squeue -j "${JOB_ID}" )

	if [[ $job_status = *${JOB_ID}* ]]; then
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

	job_status1=$( squeue -j "${JOB_ID1}" )
        job_status2=$( squeue -j "${JOB_ID2}" )

	if [[ $job_status1 = *${JOB_ID1}* || $job_status2 = *${JOB_ID2}* ]]; then
		if [[ $job_status1 = *'  R'* ]]; then
			JOBID1_STATUS="RUNNING     "
                elif [[ $job_status1 = *' PD'* ]]; then
			JOBID1_STATUS="PENDING     "
                else
		        JOBID1_STATUS="COMPLETED   "
                fi

                if [[ $job_status2 = *'  R'* ]]; then
			JOBID2_STATUS="RUNNING     "
                elif [[ $job_status2 = *' PD'* ]]; then
			JOBID2_STATUS="PENDING     "
                else
		        JOBID2_STATUS="COMPLETED   "
		fi
                echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     WAITING       |" \\r
	else
		JOBID1_STATUS="COMPLETED   "
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
