#!/bin/bash
# run_hpcc_burst.sh — local multi-worker replay burst template (HPCC plan).
# Requires: cluster grant, JFROG_ACCESS_TOKEN, built APT_SRR_RESIM.exe,
# input_path.xml per log. Splits N logs x W workers with 1-log halo;
# workers inject converged alignment headers (Solution A) or fast-lock DA
# (Solution B); Kalman replay slices use the associative prefix-scan
# engine (resim_research/parallel_kf.py, float64) for O(log N) span.
set -euo pipefail
N_LOGS="${1:-300}"
WORKERS="${2:-20}"
HALO="${HALO:-1}"
EXE="${SIL_EXE:-./APT_SRR_RESIM.exe}"
MANIFEST="${MANIFEST:-mf4_data/mf4_MANIFEST.csv}"
OUTDIR="${OUTDIR:-resim_out}"
mkdir -p "$OUTDIR"
echo "[burst] logs=$N_LOGS workers=$WORKERS halo=$HALO exe=$EXE"
echo "[burst] DRY RUN (no cluster): slice plan only."
python3 - "$N_LOGS" "$WORKERS" "$HALO" <<'EOF'
import sys
n, w, h = map(int, sys.argv[1:4])
base = n // w
for i in range(w):
    s = max(0, i * base - h)
    e = min(n, (i + 1) * base + h)
    print(f"worker {i:02d}: logs [{s},{e}) core [{i*base},{min(n,(i+1)*base)})")
EOF
echo "[burst] submit with: sbatch slurm_resim.sbatch  (edit paths first)"
