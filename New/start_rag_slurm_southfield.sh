#!/usr/bin/env bash
# Launches the RAG (Gemma GGUF) service on a dedicated Slurm compute
# allocation (1 node, 64GB RAM) instead of the shared login node, and keeps
# it alive inside a detached tmux session so it survives SSH disconnects.
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

source /etc/profile.d/modules.sh >/dev/null 2>&1 || true
module load slurm >/dev/null 2>&1 || true

: "${RAG_SLURM_PARTITION:=defq}"
: "${RAG_SLURM_ACCOUNT:=radarcore}"
: "${RAG_SLURM_QOS:=normal}"
: "${RAG_SLURM_TIME:=12:00:00}"
: "${FLASK_PORT:=5100}"

mkdir -p logs
LOG_PATH="logs/rag_srun.log"
echo "[$(date -Iseconds 2>/dev/null || date)] Starting RAG via srun (partition=$RAG_SLURM_PARTITION account=$RAG_SLURM_ACCOUNT mem=64G nodes=1)" >> "$LOG_PATH"

exec srun \
    --partition="$RAG_SLURM_PARTITION" \
    --account="$RAG_SLURM_ACCOUNT" \
    --qos="$RAG_SLURM_QOS" \
    --nodes=1 --ntasks=1 --cpus-per-task=32 --mem=64G \
    --time="$RAG_SLURM_TIME" \
    --job-name=hpcc_rag \
    bash -c '
        source /etc/profile.d/modules.sh >/dev/null 2>&1 || true
        if ! command -v apptainer >/dev/null 2>&1 && ! command -v singularity >/dev/null 2>&1; then
            module load singularity/3.11.4 >/dev/null 2>&1 || module load singularity >/dev/null 2>&1 || true
        fi
        if ! command -v apptainer >/dev/null 2>&1 && ! command -v singularity >/dev/null 2>&1; then
            echo "Apptainer/Singularity runtime is unavailable on the allocated compute node." >&2
            exit 127
        fi
        echo "RAG running on host: $(hostname)"
        cd "'"$SCRIPT_DIR"'"
        FLASK_PORT="'"$FLASK_PORT"'" ./rag/run_rag.sh --talk
    ' >> "$LOG_PATH" 2>&1
