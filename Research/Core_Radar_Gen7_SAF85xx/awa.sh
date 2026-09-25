#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

echo "awa script start"

SANITIZER=${1:-}
if [ "$SANITIZER" == "" ]; then
  echo "Usage: $0 <asan | ubsan | valgrind | compiler> [flags]"
  echo "where 'flags' are any other command-line parameters to be passed into the aptiv_warnings_analyzer"
  exit 1
fi
# This shift removes the first parameter from the arguments, so that we can pass the rest through.
shift
if [ "$SANITIZER" == "compiler" ]; then
  echo "aptiv_warnings_analyzer compiler --jira-project=\"DND,DDR,CUW,DDS,EIJ,GOG,GNZ,GHW,CUX,GOC,EOM,FHS,DNP,EPB,EEG,HYX\" --jira-label=${SANITIZER} --windriver --clang --buildlog ${SANITIZER}.log --html-report ${SANITIZER}.html --csv-report ${SANITIZER}.csv --baseline-toml ${SANITIZER}_baseline.toml $@"
  aptiv_warnings_analyzer compiler --jira-project="DND,DDR,CUW,DDS,EIJ,GOG,GNZ,GHW,CUX,GOC,EOM,FHS,DNP,EPB,EEG,HYX" --jira-label=${SANITIZER} --windriver --clang --buildlog ${SANITIZER}.log --html-report ${SANITIZER}.html --csv-report ${SANITIZER}.csv --baseline-toml ${SANITIZER}_baseline.toml $@
else
  echo "aptiv_warnings_analyzer code_checkers --jira-project=\"DND,DDR,CUW,DDS,EIJ,GOG,GNZ,GHW,CUX,GOC,EOM,FHS,DNP,EPB,EEG,HYX\" --jira-label=${SANITIZER} $SANITIZER --input-file ${SANITIZER}.log --html-report ${SANITIZER}.html --csv-report ${SANITIZER}.csv --baseline ${SANITIZER}_baseline.toml $@"
  aptiv_warnings_analyzer code_checkers --jira-project="DND,DDR,CUW,DDS,EIJ,GOG,GNZ,GHW,CUX,GOC,EOM,FHS,DNP,EPB,EEG,HYX" --jira-label=${SANITIZER} $SANITIZER --input-file ${SANITIZER}.log --html-report ${SANITIZER}.html --csv-report ${SANITIZER}.csv --baseline ${SANITIZER}_baseline.toml $@
fi
