#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

usage() {
  echo "Usage: $0 <sanitizer> <variant> [flags]"
  echo "  where 'sanitizer' can be: asan, ubsan, or valgrind"
  echo "  and 'variant' can be: srr7p, flr7, flr7v3, or all"
  echo "  and 'flags' are any other command-line parameters to be passed into the aptiv_warnings_analyzer"
  exit 1
}

SANITIZER=${1:-}
VARIANT=${2:-}
if [ "$SANITIZER" == "" ]; then
  usage
fi
if [ "$VARIANT" == "" ]; then
  usage
fi

run_sanitizer() {
  SANITIZER=$1
  VARIANT=$2
  echo "Running $SANITIZER for variant $VARIANT"
  if [ "$SANITIZER" == "valgrind" ]; then # If we're running Valgrind, we need to run a different command and then gather the results
    ./bazelisk test //:valgrind_all_unit_tests --variant=$VARIANT --valgrind >$SANITIZER.log || true
    find -H bazel-out -type f -name '*_valgrind.log' -exec cat {} + >>$SANITIZER.log
  else
    ./bazelisk test //:all_unit_tests --config=$SANITIZER --variant=$VARIANT 2>&1 >>$SANITIZER.log || true
  fi
}

rm $SANITIZER.log || true

if [ "$VARIANT" == "all" ]; then
  run_sanitizer $SANITIZER flr7
  run_sanitizer $SANITIZER flr7v3
  run_sanitizer $SANITIZER srr7p
else
  run_sanitizer $SANITIZER $VARIANT
fi

echo "Running Aptiv Warnings Analyzer"
# These shifts remove the first two parameters from the arguments, so that we can pass the rest through.
shift
shift
./awa.sh $SANITIZER $@
