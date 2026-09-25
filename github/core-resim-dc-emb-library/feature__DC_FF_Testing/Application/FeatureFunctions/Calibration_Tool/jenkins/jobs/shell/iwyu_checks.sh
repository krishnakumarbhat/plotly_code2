#!/bin/bash -e

# runs with alsw-generic docker image if the following env variables are available:
# WORKSPACE
# REPO_PATH

export CONFIGURATION="${1}"
export INSTALL_PATH="/opt/apps/workspace/install"
export IWYU_PATH="/opt/tools/iwyu/bin/include-what-you-use"
export REPORT_PATH="${WORKSPACE}/iwyu_check_log_${CONFIGURATION}"

# create folder for feature logs (if it does not exist already)
mkdir -p ${REPORT_PATH}

cd /opt/apps/workspace/

# create build folder (make sure that no old data is present)
rm -rf build && mkdir -p build && cd build

. ${REPO_PATH}/jenkins/jobs/shell/all_configurations.sh

# create build and install folder (and make sure that no old data is present)
rm -rf ${WORKSPACE}/iwyu_checks_build_${CONFIGURATION} && rm -rf ${WORKSPACE}/iwyu_checks_install_${CONFIGURATION}
mkdir -p ${WORKSPACE}/iwyu_checks_build_${CONFIGURATION} && cd ${WORKSPACE}/iwyu_checks_build_${CONFIGURATION}

# configure cmake with clang and iwyu
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_FLAGS="${CMAKE_DEFINES}" -DCMAKE_C_INCLUDE_WHAT_YOU_USE=${IWYU_PATH} -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=${IWYU_PATH} -DCMAKE_INSTALL_PREFIX=${INSTALL_FOLDER} ${REPO_PATH}/testing/c_testing

# build and save log to file in order to evaluate compiler warnings
make 2>&1 | tee -a ${REPORT_PATH}/iwyu_checks_log_config.txt

# generate iwyu report from iwyu log file
python3 -u ${REPO_PATH}/jenkins/jobs/python/create_iwyu_report.py ${REPORT_PATH}/iwyu_checks_log_config.txt

mv ${WORKSPACE}/iwyu_checks_build_${CONFIGURATION}/iwyu_report.txt ${REPORT_PATH}/.
