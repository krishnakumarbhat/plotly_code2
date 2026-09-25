#!/bin/bash
SIMG_PATH=$1
OUTPUT_PATH=$2
INPUT_LIST="$2/jobout/SIL_Input_all.txt"
SCRIPT="$OUTPUT_PATH/jobout/.resim.sh"
SCRIPT1="$OUTPUT_PATH/jobout/.resim_mining.sh"
SCRIPT2="$OUTPUT_PATH/jobout/.stats_mining.sh"
SCRIPT3="$OUTPUT_PATH/jobout/.honda_validator.sh"


if [[ $SIMG_PATH == *'scale1'* ]]; then
    YIELD_CUST="stla_scale1"

elif [[ $SIMG_PATH == *'scale3'* ]]; then
    YIELD_CUST="stla_scale3"

elif [[ $SIMG_PATH == *'traton'* ]]; then
    YIELD_CUST="traton"

elif [[ $SIMG_PATH == *'honda'* ]]; then
    YIELD_CUST="honda"

elif [[ $SIMG_PATH == *'rnasdv'* ]]; then
    YIELD_CUST="rnasdv"

elif [[ $SIMG_PATH == *'dc'* ]]; then
    YIELD_CUST="dc" 

fi

if [[ $SIMG_PATH == *'projects'* ]]; then
    EXECUTION_PROJECT='STLA-THUNDER'
else
    EXECUTION_PROJECT='RNA-SDV-SRR7'
fi

echo "INPUT_LIST= $INPUT_LIST" >> $OUTPUT_PATH/RESIM_details.txt
ARRAY=0

ARRAY=$(wc <"$INPUT_LIST" -l)
MAX_ARRAY=$(( ${ARRAY} + 1 ))
echo "Number of data to process ""$ARRAY" >> $OUTPUT_PATH/RESIM_details.txt
JOBS=$((${ARRAY} / ${MAX_ARRAY}))
echo "total no of jobs is" $JOBS >> $OUTPUT_PATH/RESIM_details.txt
for (( i = 0; i <= JOBS; i++ )); do
    ARRAY_START=$((${i} * ${MAX_ARRAY}))
    echo array start is $ARRAY_START >> $OUTPUT_PATH/RESIM_details.txt
    ARRAY_STOP=$(( ${ARRAY_START} + ${MAX_ARRAY}))
    echo array stop is $ARRAY_STOP >> $OUTPUT_PATH/RESIM_details.txt
    ARRAY_LENGTH=$(( ${ARRAY_STOP} - ${ARRAY_START}))
    echo array length is $ARRAY_LENGTH >> $OUTPUT_PATH/RESIM_details.txt
    if [ $ARRAY_STOP -gt ${ARRAY} ];then
      ARRAY_LENGTH=$(( ${ARRAY} % ${MAX_ARRAY}))
      echo new array length is $ARRAY_LENGTH >> $OUTPUT_PATH/RESIM_details.txt
      ARRAY_STOP=$(( ${ARRAY_START} + ${ARRAY_LENGTH}))
      echo new array stop is $ARRAY_STOP >> $OUTPUT_PATH/RESIM_details.txt
    fi
    echo "JOBS "$(( ${ARRAY_START} + 1)) - ${ARRAY_STOP} >> $OUTPUT_PATH/RESIM_details.txt
    echo "output path is $OUTPUT_PATH" >> $OUTPUT_PATH/RESIM_details.txt

    JOB_ID=$(sbatch -A $EXECUTION_PROJECT -a "1-$ARRAY_LENGTH%$ARRAY_LENGTH" $SCRIPT $ARRAY_START $SIMG_PATH $OUTPUT_PATH | awk '{print $NF}')

    echo "Job $JOB_ID is submitted" >> $OUTPUT_PATH/RESIM_details.txt

done
echo ""
JOB_ID1=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p highPrio $SCRIPT1 $OUTPUT_PATH/output/.mining.txt $YIELD_CUST | awk '{print $NF}')
JOB_ID2=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p highPrio $SCRIPT2 $OUTPUT_PATH/output/.SIL_Statistics.txt | awk '{print $NF}')

if [[ $SIMG_PATH == *'honda'* ]]; then
    JOB_ID3=$(sbatch -A $EXECUTION_PROJECT --dependency=afterany:$JOB_ID -p highPrio $SCRIPT3 $OUTPUT_PATH/output | awk '{print $NF}')
fi

echo "Job $JOB_ID1 is submitted" >> $OUTPUT_PATH/RESIM_details.txt
JOBID_STATUS="PENDING     "
JOBID1_STATUS="PENDING     "
JOBID2_STATUS="PENDING     "

if [[ $SIMG_PATH == *'honda'* ]]; then
    echo "[INFO] : PipeLine Triggered  ReSIm_docker: $JOB_ID | ReSim_Mining: $JOB_ID1 | Stats_Mining: $JOB_ID2 | blf_validator: $JOB_ID3"
else
    echo "[INFO] : PipeLine Triggered  ReSIm_docker: $JOB_ID | ReSim_Mining: $JOB_ID1 | Stats_Mining: $JOB_ID2"
fi
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
rm "$OUTPUT_PATH/jobout/.resim.sh"
rm "$OUTPUT_PATH/jobout/.resim_mining.sh"
rm "$OUTPUT_PATH/jobout/.stats_mining.sh"

if [[ $SIMG_PATH == *'honda'* ]]; then
    while : ; do
        job_status=$( squeue -j "${JOB_ID3}" )
        if [[ $job_status = *${JOB_ID3}* ]]; then
            sleep 5
        else
            break
        fi
    done
fi
rm "$OUTPUT_PATH/jobout/.honda_validator.sh"

echo "         | Done        |  $JOBID_STATUS |  $JOBID1_STATUS |  $JOBID2_STATUS |     COMPLETED     |"
echo "         |_____________|_______________|_______________|_______________|___________________|"
echo ""
