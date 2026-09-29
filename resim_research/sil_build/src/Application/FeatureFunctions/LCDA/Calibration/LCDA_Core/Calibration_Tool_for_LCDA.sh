#!/bin/bash -e

# Determine repo main folder
CORE_CAL_PATH="$( cd "$( dirname "$0" )" && pwd )"
BASE_PATH="${CORE_CAL_PATH}/../../.."

# Run Calibration Tool
echo "####### Running calibration tool..."
${BASE_PATH}/Calibration_Tool/dist/ct_main ${CORE_CAL_PATH}/lcda_cal.xml
