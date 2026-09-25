#!/usr/bin/env bash
###############################################################################
# Run the Log Viewer Singularity image directly (without Slurm).
# For Slurm, use: sbatch slurm_log_viewer.sh
###############################################################################
set -euo pipefail

SIMG="${1:-$(dirname "$0")/log_viewer.simg}"
PORT="${PORT:-5000}"
HOST="${HOST:-0.0.0.0}"
CACHE_DIR="${CACHE_DIR:-$HOME/.cache_html}"

mkdir -p "$CACHE_DIR"

export LOGVIEW_HOST="$HOST"
export LOGVIEW_PORT="$PORT"
export LOGVIEW_NO_BROWSER=1

NODE_IP=$(hostname -I 2>/dev/null | awk '{print $1}' || echo "localhost")

echo "============================================================"
echo "Starting Log Viewer (non-Slurm)"
echo "  Image : $SIMG"
echo "  Bind  : $HOST:$PORT"
echo "  URL   : http://$NODE_IP:$PORT/"
echo "  Cache : $CACHE_DIR"
echo "============================================================"

exec singularity run \
    --bind "$CACHE_DIR:$HOME/.cache_html" \
    "$SIMG"
