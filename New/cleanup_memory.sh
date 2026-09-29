#!/usr/bin/env bash
set -euo pipefail

RUNTIME_ROOT="${HPCC_RUNTIME_LOCAL_ROOT:-/local/hpc_tools}"
AGE_MINUTES="${HPCC_CLEANUP_AGE_MINUTES:-120}"

if [[ ! -d "$RUNTIME_ROOT" ]]; then
    echo "Runtime root not found: $RUNTIME_ROOT"
    exit 0
fi

echo "Cleaning transient runtime data under $RUNTIME_ROOT older than $AGE_MINUTES minutes"
find "$RUNTIME_ROOT" -mindepth 1 -maxdepth 1 -type d \( -name cache_html -o -name db -o -name hpcc_runtime -o -name sqlite \) -print0 \
  | while IFS= read -r -d '' path; do
        find "$path" -mindepth 1 -mmin +"$AGE_MINUTES" -exec rm -rf {} +
    done

echo "Cleanup complete"