#!/bin/bash -e

# runs with alsw-generic docker if configuration number (as defined in all_configurations.sh)
# is passed as command line argument and the following env variables are available:
# WORKSPACE
# REPO_PATH

export CONFIGURATION="${1}"
export BUILD_FOLDER="/opt/apps/workspace/c_build_${CONFIGURATION}"
export INSTALL_FOLDER="/opt/apps/workspace/c_install_${CONFIGURATION}"
export LOGGING_FOLDER="${WORKSPACE}/c_testing_logs"
export LOGGING_FILE="${LOGGING_FOLDER}/build_log_config_${CONFIGURATION}.txt"

# create logging folder (if it does not exist already)
mkdir -p ${LOGGING_FOLDER}

# this will define a variable CMAKE_DEFINES with the specific configuration
. ${REPO_PATH}/jenkins/jobs/shell/all_configurations.sh

# create build and install folder (and make sure that no old data is present)
rm -rf ${BUILD_FOLDER} && rm -rf ${INSTALL_FOLDER}
mkdir -p ${BUILD_FOLDER} && cd ${BUILD_FOLDER}

# CMake configuration of calibration tool
# Note that the generation of the .c and .h files required here is done in the previous Jenkins state "Run Calibration Tool"
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_FLAGS="${CMAKE_DEFINES}" -DCMAKE_INSTALL_PREFIX=${INSTALL_FOLDER} ${REPO_PATH}/testing/c_testing

# set pipefail to get error code for all commands (otherwise tee will shadow errors)
set -o pipefail

# build and save log to file in order to evaluate compiler warnings
echo "Configuration: ${CMAKE_DEFINES}" >> ${LOGGING_FILE}
make 2>&1 | tee -a ${LOGGING_FILE}

