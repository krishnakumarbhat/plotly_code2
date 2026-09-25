#!/bin/bash -e

# runs with alsw-generic docker if the following env variables are available:
# WORKSPACE
# REPO_PATH

# copy example_files folder to second location
cp -r ${REPO_PATH}/testing/example_files /opt/apps/workspace/compare_example_files

# remove auto-generated files from copy
find /opt/apps/workspace/compare_example_files -name \*.c -type f -delete
find /opt/apps/workspace/compare_example_files -name \*.h -type f -delete

# run calibration tool (to make sure that auto-generated files are up-to-date)
chmod +x ${REPO_PATH}/dist/ct_main
${REPO_PATH}/dist/ct_main /opt/apps/workspace/compare_example_files/Core/cool_feature_cal.xml

# create folder for feature logs (if it does not exist already)
mkdir -p ${WORKSPACE}/executable_check_logs

# check for changes in calibration files (ignore exit code with || true)
diff -r ${REPO_PATH}/testing/example_files /opt/apps/workspace/compare_example_files >> ${WORKSPACE}/executable_check_logs/cal_file_diff.txt || true
