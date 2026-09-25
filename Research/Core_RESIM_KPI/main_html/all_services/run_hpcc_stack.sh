#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_PATH="$SCRIPT_DIR/run_hpcc_stack.log"

echo "[$(date -Iseconds 2>/dev/null || date)] run_hpcc_stack.sh -> main_hpcc.sh" | tee -a "$LOG_PATH"
exec bash "$SCRIPT_DIR/main_hpcc.sh" "$@" 2>&1 | tee -a "$LOG_PATH"