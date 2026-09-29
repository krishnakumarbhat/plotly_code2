#!/bin/bash

IFS="," read -r -a my_values <<< "$@"
RESIM_SIMG=${my_values[0]:1}
INPUT_LIST=${my_values[1]:1}
#VV_MINING=${my_values[3]:1}
OUT_PATH=${my_values[4]:1:-1}
JOBOUT=$OUT_PATH/jobout
SCRIPT_VV="$JOBOUT/.resimvv.sh"
VV_MINING="$JOBOUT/.rvvming.sh"

echo "RESIM_SIMG : $RESIM_SIMG" >> $OUT_PATH/RESIM_details.txt
echo "INPUT_LIST : $INPUT_LIST" >> $OUT_PATH/RESIM_details.txt
#echo "VV_MINING  : $VV_MINING" >> $OUT_PATH/RESIM_details.txt
echo "OUT_PATH   : $OUT_PATH" >> $OUT_PATH/RESIM_details.txt
#echo "JOBOUT     : $JOBOUT" >> $OUT_PATH/RESIM_details.txt



ALL_LOGS=$JOBOUT/SIL_Input_All.txt

count=0
path=$(cat $INPUT_LIST)
for line in $path; do
#    flag=0
    for file in "$line"/*; do
        count=$((count+1))
        echo "$file" >> $JOBOUT/inputlog_$count.txt
        echo "$JOBOUT/inputlog_$count.txt" >> $ALL_LOGS
#        if [ $flag == 0 ]; then
            echo "$file, 1" >> $JOBOUT/SIL_input_session.txt
#            flag=1
#        fi
    done
done

echo "" >> $OUT_PATH/RESIM_details.txt
echo "" >> $OUT_PATH/RESIM_details.txt
echo "*********** Job Related Details ***********" >> $OUT_PATH/RESIM_details.txt

# Helios shares storage with Krakow but requires its own SLURM account/partition,
# detected via the "8k3" identifier present in the HPCC username.
EXECUTION_PROJECT="RNA-SDV-SRR7"
PARTITION=""
if [[ $USER == *'8k3'* || $USER == *'8K3'* ]]; then
    EXECUTION_PROJECT="8k3p89"
    PARTITION="-p 8k3"
fi
echo "partition requested : $PARTITION"

TASKS=0

TASKS=$(wc <"$ALL_LOGS" -l)
MAX_TASKS=$(( ${TASKS} + 1 ))
JOBS=$((${TASKS} / ${MAX_TASKS}))

echo "Job(s)  requested	: $(( $JOBS+1 ))" >> $OUT_PATH/RESIM_details.txt
echo "Task(s) requested	: $TASKS" >> $OUT_PATH/RESIM_details.txt

for (( i = 0; i <= JOBS; i++ )); do
    TASKS_START=$((${i} * ${MAX_TASKS}))
    TASKS_STOP=$(( ${TASKS_START} + ${MAX_TASKS}))
    TASKS_LENGTH=$(( ${TASKS_STOP} - ${TASKS_START}))
    if [ $TASKS_STOP -gt ${TASKS} ];then
      TASKS_LENGTH=$(( ${TASKS} % ${MAX_TASKS}))
      TASKS_STOP=$(( ${TASKS_START} + ${TASKS_LENGTH}))
    fi
    echo "TASKS ARRAY		: $(( ${TASKS_START} + 1)) - ${TASKS_STOP}" >> $OUT_PATH/RESIM_details.txt
    
    JOB_ID=$(sbatch -A $EXECUTION_PROJECT $PARTITION -a "1-$TASKS_LENGTH%$TASKS_LENGTH" $SCRIPT_VV $TASKS_START $RESIM_SIMG $OUT_PATH | awk '{print $NF}')
done

MIN_JOB_ID=$(sbatch -A $EXECUTION_PROJECT $PARTITION --dependency=afterany:$JOB_ID $VV_MINING $OUT_PATH/output/.mining.txt $JOBOUT | awk '{print $NF}')

echo "" >> $OUT_PATH/RESIM_details.txt
echo "" >> $OUT_PATH/RESIM_details.txt
echo "*********** JobId Related Details ***********" >> $OUT_PATH/RESIM_details.txt
echo "VV ReSIm Job Id		: $JOB_ID " >> $OUT_PATH/RESIM_details.txt
echo "VV Mining Job Id	: $MIN_JOB_ID" >> $OUT_PATH/RESIM_details.txt

echo     "          _____________________________________________________________________________"
echo     "         | Job     |  ReSim_VV_Docker |  ReSim_VV_Mining |  OutPut Path                "
echo     "         |---------|------------------|------------------|-----------------------------"
echo     "         | Job Id  |  $JOB_ID        |  $MIN_JOB_ID        |  $OUT_PATH                "
echo     "          ============================================================================="




