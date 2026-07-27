#!/usr/bin/env bash
set -uo pipefail
source /etc/profile.d/modules.sh >/dev/null 2>&1 || true
module load apptainer >/dev/null 2>&1 || module load singularity >/dev/null 2>&1 || true
RUNTIME_BIN="$(command -v apptainer || command -v singularity)"
MODEL_DIR="/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_5/rag/model"
MODEL="$(find "$MODEL_DIR" -maxdepth 1 -name '*.gguf' -print -quit)"
echo "resolved model: $MODEL"
timeout 40 "$RUNTIME_BIN" exec --bind "$MODEL_DIR:$MODEL_DIR" rag.simg \
    /app/rag/tools/llama.cpp/llama-server -m "$MODEL" --host 127.0.0.1 --port 8099 -c 4096 -t 4 -b 512 -n 64 2>&1 | head -n 80
