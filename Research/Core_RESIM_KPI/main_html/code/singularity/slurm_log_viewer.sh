#!/usr/bin/env bash
#SBATCH --job-name=log-viewer
#SBATCH --output=log_viewer_%j.out
#SBATCH --error=log_viewer_%j.err
#SBATCH --time=365-00:00:00          # 356 days max (adjust as needed)
#SBATCH --partition=interactive    # change to your partition
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=4G

###############################################################################
# Slurm job script to run the Log Viewer Flask service inside Singularity.
# The service binds to 0.0.0.0:<PORT> using host networking, so users on the
# company LAN (10.x.x.x) can reach it at http://<node-ip>:<PORT>/
#
# Usage:
#   sbatch slurm_log_viewer.sh              # uses defaults
#   PORT=8080 sbatch slurm_log_viewer.sh    # custom port
###############################################################################

set -euo pipefail

# ---------- Configuration (override via env before sbatch) ------------------
PORT="${PORT:-5000}"
HOST="${HOST:-0.0.0.0}"
SIMG="${SIMG:-$(dirname "$0")/log_viewer.simg}"
CACHE_DIR="${CACHE_DIR:-$HOME/.cache_html}"

# ---------- Setup ------------------------------------------------------------
mkdir -p "$CACHE_DIR"

export LOGVIEW_HOST="$HOST"
export LOGVIEW_PORT="$PORT"
export LOGVIEW_NO_BROWSER=1

NODE_IP=$(hostname -I | awk '{print $1}')

echo "============================================================"
echo "Starting Log Viewer"
echo "  Image : $SIMG"
echo "  Host  : $HOST:$PORT"
echo "  Node  : $NODE_IP"
echo "  URL   : http://$NODE_IP:$PORT/"
echo "  URL   : http://$NODE_IP:$PORT/html  (redirects to /)"
echo "  Cache : $CACHE_DIR"
echo "============================================================"

# ---------- Run --------------------------------------------------------------
# Bind-mount cache so downloaded data persists across container restarts.
singularity run \
    --bind "$CACHE_DIR:$HOME/.cache_html" \
    "$SIMG"
