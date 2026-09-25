#!/usr/bin/env bash
# Build the Singularity .simg image on a Linux host (or inside WSL).
# Run from repo root:  bash main_html/code/singularity/build_image.sh

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

DEF_FILE="$REPO_ROOT/main_html/code/singularity/log_viewer.def"
OUT_FILE="$REPO_ROOT/main_html/code/singularity/log_viewer.simg"

echo "============================================================"
echo "Building Singularity image"
echo "Definition: $DEF_FILE"
echo "Output:     $OUT_FILE"
echo "============================================================"

# Build (requires root or fakeroot capability)
if command -v sudo &>/dev/null; then
    sudo singularity build --force "$OUT_FILE" "$DEF_FILE"
else
    singularity build --force "$OUT_FILE" "$DEF_FILE"
fi

echo ""
echo "Build complete: $OUT_FILE"
echo "Transfer to cluster and submit with: sbatch slurm_log_viewer.sh"
