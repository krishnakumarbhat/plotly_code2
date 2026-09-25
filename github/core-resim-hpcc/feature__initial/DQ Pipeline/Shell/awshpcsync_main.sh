#!/bin/bash

Cx=$1
File=$2
joboutPath=$4
SCRIPT=$joboutPath/awshpccchild.sh
EXECUTION_PROJECT='RNA-SDV-SRR7'

echo "INPUT_LIST= $File" >> $joboutPath/copyjobDetails.txt
ARRAY=0

echo "$File" >> $joboutPath/copyjobDetails.txt

ARRAY=$(wc <"$File" -l)
MAX_ARRAY=$(( ${ARRAY} + 1 ))
echo "Number of data to process ""$ARRAY" >> $joboutPath/copyjobDetails.txt
JOBS=$((${ARRAY} / ${MAX_ARRAY}))
echo "total no of jobs is" $JOBS >> $joboutPath/copyjobDetails.txt
for (( i = 0; i <= JOBS; i++ )); do
    ARRAY_START=$((${i} * ${MAX_ARRAY}))
    echo array start is $ARRAY_START >> $joboutPath/copyjobDetails.txt
    ARRAY_STOP=$(( ${ARRAY_START} + ${MAX_ARRAY}))
    echo array stop is $ARRAY_STOP >> $joboutPath/copyjobDetails.txt
    ARRAY_LENGTH=$(( ${ARRAY_STOP} - ${ARRAY_START}))
    echo array length is $ARRAY_LENGTH >> $joboutPath/copyjobDetails.txt
    if [ $ARRAY_STOP -gt ${ARRAY} ];then
      ARRAY_LENGTH=$(( ${ARRAY} % ${MAX_ARRAY}))
      echo new array length is $ARRAY_LENGTH >> $joboutPath/copyjobDetails.txt
      ARRAY_STOP=$(( ${ARRAY_START} + ${ARRAY_LENGTH}))
      echo new array stop is $ARRAY_STOP >> $joboutPath/copyjobDetails.txt
    fi
    echo "JOBS "$(( ${ARRAY_START} + 1)) - ${ARRAY_STOP} >> $joboutPath/copyjobDetails.txt
    echo "Reports path is $joboutPath" >> $joboutPath/copyjobDetails.txt

    echo "$SCRIPT $ARRAY_START $@" >>$joboutPath/copyjobDetails.txt

    JOB_ID=$(sbatch -A $EXECUTION_PROJECT -p highPrio -a "1-$ARRAY_LENGTH%200" $SCRIPT $ARRAY_START $@ | awk '{print $NF}')

    echo "Job $JOB_ID is submitted" >> $joboutPath/copyjobDetails.txt
done

JOBID_STATUS="PENDING      "

echo "[INFO] : PipeLine Triggered  AWS->HPCC downloading: $JOB_ID"
echo     "          ________________________________"
echo     "         | Job Status  |  Download Status |"
echo     "         |-------------|------------------|"
echo -ne "         | Submitted   |  $JOBID_STATUS   |" \\r

                   

while : ; do

	job_status=$( squeue -j "${JOB_ID}" )

	if [[ $job_status = *${JOB_ID}* ]]; then
		if [[ $job_status = *'  R'* ]]; then
			JOBID_STATUS="RUNNING        "
                        echo -ne "         | In Progress |  $JOBID_STATUS |" \\r
		fi
	else
		JOBID_STATUS="COMPLETED    "
		echo "         | Done        |  $JOBID_STATUS   |"
                echo "         |_____________|__________________|"
		break
	fi
	sleep 1
done

#rm $SCRIPT


