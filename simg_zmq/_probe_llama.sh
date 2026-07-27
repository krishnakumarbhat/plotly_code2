#!/usr/bin/env bash
set -uo pipefail
source /etc/profile.d/modules.sh >/dev/null 2>&1 || true
module load apptainer >/dev/null 2>&1 || module load singularity >/dev/null 2>&1 || true
RUNTIME_BIN="$(command -v apptainer || command -v singularity)"
"$RUNTIME_BIN" exec rag.simg bash -c '
ls -la /app/rag/tools/llama.cpp/ 2>&1
echo ---env---
env | grep -i llama
echo ---model---
ls -la /app/rag/model 2>&1
'
