#!/usr/bin/env bash
set -euo pipefail

DEF_FILE="${1:-ResimHTMLReport.def}"
OUT_IMG="${2:-ResimHTMLReport.simg}"

if [[ ! -f "$DEF_FILE" ]]; then
  echo "Definition file not found: $DEF_FILE"
  echo "Usage: ./build_simg.sh [DEF_FILE] [OUTPUT_SIMG]"
  exit 1
fi

if command -v apptainer >/dev/null 2>&1; then
  SINGULARITY_CMD="apptainer"
elif command -v singularity >/dev/null 2>&1; then
  SINGULARITY_CMD="singularity"
else
  echo "Neither apptainer nor singularity is installed."
  echo "Install one of them, then re-run this script."
  exit 1
fi

echo "Using: $SINGULARITY_CMD"
echo "Building image: $OUT_IMG from $DEF_FILE"

if [[ "$(id -u)" -eq 0 ]]; then
  "$SINGULARITY_CMD" build "$OUT_IMG" "$DEF_FILE"
else
  echo "Root privileges are usually required for build. Trying with sudo..."
  sudo "$SINGULARITY_CMD" build "$OUT_IMG" "$DEF_FILE"
fi

echo "Build complete: $OUT_IMG"