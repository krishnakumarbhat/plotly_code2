# -*- coding: utf-8 -*-
"""
Created on Created on Wed Dec 10 08:38:57 2025

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""
def script_master():
    return '''
#!/bin/bash

ARRAY_LENGTH=$1
SIMG=$2
INPUT_TXT=$3
EXECUTION_PROJ=$4
OUTPUT=$5

JOBOUT_DIR="${OUTPUT}/jobout"
OUTPUT_DIR="${OUTPUT}/output"

SCRIPT="${JOBOUT_DIR}/.smoketest.sh"
SCRIPT1="${JOBOUT_DIR}/.rming.sh"

YIELD_CUST='V2'

JOBS=1

echo "Execution details :" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "*********************************************************************************" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "Sil-Engine config   : $ARRAY_LENGTH"   >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "SIMG                : $SIMG"           >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "INPUT_TXT           : $INPUT_TXT"      >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "EXECUTION_PROJ      : $EXECUTION_PROJ" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "OUTPUT              : $OUTPUT"         >> "${OUTPUT}/.smoketest_execution_log.txt"
#echo "SCRIPT              : $SCRIPT"         >> "${OUTPUT}/.smoketest_execution_log.txt" 
#echo "SCRIPT1             : $SCRIPT1"        >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "*********************************************************************************" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "Submitted JOBS :" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "*********************************************************************************" >> "${OUTPUT}/.smoketest_execution_log.txt"


#: << "END_COMMENT"
for (( i=0; i<=JOBS-1; i++  )); do
   JOB_ID_SMOKE=$(sbatch -A $EXECUTION_PROJ -p highPrio -a "1-$ARRAY_LENGTH%$ARRAY_LENGTH" $SCRIPT $SIMG $INPUT_TXT $OUTPUT | awk '{print $NF}')
done
echo""
JOB_ID_MINING=$(sbatch -A $EXECUTION_PROJ --dependency=afterany:$JOB_ID_SMOKE -p highPrio $SCRIPT1 $OUTPUT_DIR/.mining.txt $YIELD_CUST $YIELD_CUST | awk '{print $NF}')

echo "Submitted JOBS : $OUTPUT"
echo "*****************************"
echo " Smoketest Job : $JOB_ID_SMOKE"
echo " Mining    Job : $JOB_ID_MINING"
echo "*****************************"
echo "[M-INFO] : Use 'squeue -j <JOB_ID>' to monitor the individual job status (Example - squeue -j $JOB_ID_SMOKE)."
echo "[M-INFO] : Use 'squeue -u <USER_ID> to monitor all jobs status."
echo""
echo "Smoketest : $JOB_ID_SMOKE"  >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "Mining    : $JOB_ID_MINING" >> "${OUTPUT}/.smoketest_execution_log.txt"
echo "*********************************************************************************" >> "${OUTPUT}/.smoketest_execution_log.txt"

rm $SCRIPT
rm $SCRIPT1
rm $0
#END_COMMENT

'''