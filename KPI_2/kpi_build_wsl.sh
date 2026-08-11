#!/usr/bin/env bash
set -euo pipefail

# Build script for WSL/dev only.
# Runtime execution should always use ./main.sh with prebuilt .simg files.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if [[ "$(uname -s)" != "Linux" ]]; then
  echo "ERROR: build_wsl.sh must run on Linux/WSL." >&2
  exit 1
fi

if command -v apptainer >/dev/null 2>&1; then
  CTR_BUILD="apptainer"
elif command -v singularity >/dev/null 2>&1; then
  CTR_BUILD="singularity"
else
  echo "ERROR: Neither apptainer nor singularity is available for build." >&2
  exit 1
fi

echo "Using container builder: $CTR_BUILD"

echo "[1/3] Building interactiveplot.simg"
"$CTR_BUILD" build --force interactiveplot.simg intplot_kpi/singularity_interactiveplot.def

echo "[2/3] Building kpi.simg"
"$CTR_BUILD" build --force kpi.simg UDP_KPI/Singularity_KPI.def

echo "[3/3] Building can_kpi.simg"
"$CTR_BUILD" build --force can_kpi.simg can_kpi/can_singularity_KPI.def

echo "Build complete. Artifacts:"
ls -lh interactiveplot.simg kpi.simg can_kpi.simg
