#!/bin/bash
SIMG_PATH=$1
CONFIG_PATH=$2
INPUT_LIST=$3
OUTPUT_PATH=$4
CONVERTER_SIMG=$5
CONVERTER_CONFIG=$6

if [[ $SIMG_PATH == *'projects'* ]]; then
    EXECUTION_PROJECT='STLA-THUNDER'
else
    EXECUTION_PROJECT='RNA-SDV-SRR7'
fi

echo "INPUT_LIST= $INPUT_LIST" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
ARRAY=0

ARRAY=$(wc <"$INPUT_LIST" -l)
MAX_ARRAY=$(( ${ARRAY} + 1 ))
echo "Number of data to process ""$ARRAY" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
JOBS=$((${ARRAY} / ${MAX_ARRAY}))
echo "total no of jobs is" $JOBS >> $OUTPUT_PATH/QualityChecker_JobDetails.txt

# Detect whether sessions contain BRR files (need dedicated converter job)
IS_BRR=0
while IFS= read -r flist_path; do
    [ -z "$flist_path" ] && continue
    FIRST=$(head -n 1 "$flist_path" 2>/dev/null)
    if [[ "$FIRST" == *.brr || "$FIRST" == *.BRR ]]; then
        IS_BRR=1
        break
    fi
done < "$INPUT_LIST"
echo "IS_BRR=$IS_BRR" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt

for (( i = 0; i <= JOBS; i++ )); do
    ARRAY_START=$((${i} * ${MAX_ARRAY}))
    echo array start is $ARRAY_START >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    ARRAY_STOP=$(( ${ARRAY_START} + ${MAX_ARRAY}))
    echo array stop is $ARRAY_STOP >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    ARRAY_LENGTH=$(( ${ARRAY_STOP} - ${ARRAY_START}))
    echo array length is $ARRAY_LENGTH >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    if [ $ARRAY_STOP -gt ${ARRAY} ];then
      ARRAY_LENGTH=$(( ${ARRAY} % ${MAX_ARRAY}))
      echo new array length is $ARRAY_LENGTH >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
      ARRAY_STOP=$(( ${ARRAY_START} + ${ARRAY_LENGTH}))
      echo new array stop is $ARRAY_STOP >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    fi
    echo "JOBS "$(( ${ARRAY_START} + 1)) - ${ARRAY_STOP} >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    echo "output path is $OUTPUT_PATH" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    mkdir -p $OUTPUT_PATH/jobout
    SCRIPT="$OUTPUT_PATH/jobout/.QualityChecker.sh"
    CONV_SCRIPT="$OUTPUT_PATH/jobout/.Converter.sh"
    mkdir -p $OUTPUT_PATH/output
    RESULT_OUT=$OUTPUT_PATH/output
    echo $SCRIPT $ARRAY_START $SIMG_PATH $CONFIG_PATH $INPUT_LIST $RESULT_OUT >> $OUTPUT_PATH/QualityChecker_JobDetails.txt

    mkdir -p $RESULT_OUT/jobout

    if [ $IS_BRR -eq 1 ]; then
        # Submit dedicated converter job first (64G, 8 CPUs)
        # simg/config passed as $2/$3 into .Converter.sh — sourced from dq_input_static.py via dq_main.py
        CONV_JOB_ID=$(sbatch -A $EXECUTION_PROJECT -a "1-$ARRAY_LENGTH%$ARRAY_LENGTH" $CONV_SCRIPT $ARRAY_START $CONVERTER_SIMG $CONVERTER_CONFIG $INPUT_LIST $RESULT_OUT | awk '{print $NF}')
        echo "Converter Job $CONV_JOB_ID submitted" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
        echo "[INFO] : Converter job $CONV_JOB_ID submitted (64G, 8 CPUs)"
        # Submit DQ job only after converter completes successfully
        JOB_ID=$(sbatch -A $EXECUTION_PROJECT --dependency=afterok:$CONV_JOB_ID -a "1-$ARRAY_LENGTH%$ARRAY_LENGTH" $SCRIPT $ARRAY_START $SIMG_PATH $CONFIG_PATH $INPUT_LIST $RESULT_OUT | awk '{print $NF}')
    else
        JOB_ID=$(sbatch -A $EXECUTION_PROJECT -a "1-$ARRAY_LENGTH%$ARRAY_LENGTH" $SCRIPT $ARRAY_START $SIMG_PATH $CONFIG_PATH $INPUT_LIST $RESULT_OUT | awk '{print $NF}')
    fi

    
echo "Job $JOB_ID is submitted" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
    
done
echo ""
SCRIPT1="$OUTPUT_PATH/jobout/.Log_QualityChecker_Mining.sh"
JOB_ID1=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p highPrio $SCRIPT1 $RESULT_OUT/DQ_xml_fList.txt $RESULT_OUT | awk '{print $NF}')
echo "Job $JOB_ID1 is submitted" >> $OUTPUT_PATH/QualityChecker_JobDetails.txt
JOBID_STATUS="PENDING  "
JOBID1_STATUS="PENDING  "
echo "[INFO] : PipeLine Triggered  Quality_checker_docker: $JOB_ID and Quality_Checker_Mining: $JOB_ID1"
echo     "          ___________________________________________________________ "
echo     "         | Job Status  |  DQ_Docker |  DQ_Mining |  Post_Processing  |"
echo     "         |-------------|------------|------------|-------------------|"
echo -ne "         | Submitted   |  $JOBID_STATUS |  $JOBID1_STATUS |     WAITING       |" \\r
while : ; do

	job_status=$( squeue -j "${JOB_ID}" )

	if [[ $job_status = *${JOB_ID}* ]]; then
		if [[ $job_status = *'  R'* ]]; then
                        JOBID_STATUS="RUNNING  "
			echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |     WAITING       |" \\r
		fi
	else
		JOBID_STATUS="COMPLETED"
                echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |     WAITING       |" \\r
		break
	fi
        sleep 5
done
while : ; do

	job_status=$( squeue -j "${JOB_ID1}" )

	if [[ $job_status = *${JOB_ID1}* ]]; then
		if [[ $job_status = *'  R'* ]]; then
			JOBID1_STATUS="RUNNING  "
                        echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |     WAITING       |" \\r
		fi
	else
		JOBID1_STATUS="COMPLETED"
                echo -ne "         | In Progress |  $JOBID_STATUS |  $JOBID1_STATUS |     WAITING       |" \\r
		break
	fi 
        sleep 5
done
echo -ne "         | Done        |  $JOBID_STATUS |  $JOBID1_STATUS |     PROCESSING    |" \\r
rm "$OUTPUT_PATH/jobout/.QualityChecker.sh"
rm "$OUTPUT_PATH/jobout/.Log_QualityChecker_Mining.sh"
echo "         | Done        |  $JOBID_STATUS |  $JOBID1_STATUS |     COMPLETED     |"
echo "         |_____________|____________|____________|___________________|"
echo ""___