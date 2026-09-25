#!/bin/bash -e

# runs with alsw-generic docker if configuration number (as defined in all_configurations.sh)
# is passed as command line argument and the following env variables are available:
# WORKSPACE
# REPO_PATH

export CONFIGURATION="${1}"
export BUILD_FOLDER="/opt/apps/workspace/c_coverity_build_${CONFIGURATION}"
export INSTALL_FOLDER="/opt/apps/workspace/c_coverity_install_${CONFIGURATION}"
export LOGGING_FOLDER="${WORKSPACE}/c_coverity_logs_${CONFIGURATION}"

export PATH=$PATH:/opt/coverity/cov-analysis-linux64-2022.3.0/bin
export COV_EXCLUDE_FILTER="(.*Mock_Files.*)"
export COV_CODING_STANDARD="--coding-standard-config ${REPO_PATH}/jenkins/configs/misrac2012-all-HIS_SRF.config"
export COV_INCLUDE_FILTER="(.*)"

# Disable usage of ccache for coverity build
export CCACHE_DISABLE=true

# configure coverity to exclude some folders (speeds up calculation)
cov-configure --gcc --config /opt/apps/workspace/cov.xml --xml-option=skip_file:"${REPO_PATH}/testing/Mock_Files/.*"

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

# call make guarded by cov-build
cov-build --config /opt/apps/workspace/cov.xml --dir /opt/apps/workspace/coverity_results --return-emit-failures --emit-complementary-info make

# analyze build with cov-analyze
cov-analyze --dir /opt/apps/workspace/coverity_results  --security --concurrency --enable-fnptr --enable-constraint-fpp --enable-virtual --checker-option DEADCODE:no_dead_default:true --checker-option RESOURCE_LEAK:allow_main:true ${COV_CODING_STANDARD} --strip-path ${REPO_PATH}

# create html report and save it in coverity_logs folder
cov-format-errors --filesort --exclude-files ${COV_EXCLUDE_FILTER} --include-files ${COV_INCLUDE_FILTER} --dir /opt/apps/workspace/coverity_results --html-output ${LOGGING_FOLDER}/report

# move summary to own folder to reduce memory used by Jenkins (publishHTML will store all files from folder for each report)
mkdir -p ${LOGGING_FOLDER}/summary
mv ${LOGGING_FOLDER}/report/summary.html ${LOGGING_FOLDER}/summary/summary.html
