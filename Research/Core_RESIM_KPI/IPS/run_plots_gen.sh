#!/bin/bash
# ==============================================================================
# run_plots_gen.sh - sbatch wrapper for plots_gen.py
#
# Usage (submit to Slurm - runs in background, survives terminal close):
#   sbatch run_plots_gen.sh <mode>
#
#   mode 1 = GroundTruth vs TrackerOutput  (reads input.xml)
#   mode 2 = TrackerOutput vs TrackerOutput (reads input_tracker_vs_tracker.xml)
#
# Check status:   squeue -u $USER
# View output:    tail -f plots_gen_<jobid>.out
#
# Recommended: use launch.sh to be prompted for the mode interactively.
# ==============================================================================

#SBATCH --job-name=plots_gen
#SBATCH --account=RNA-SDV-SRR7
#SBATCH --mem=32G
#SBATCH --cpus-per-task=1
#SBATCH --time=04:00:00
#SBATCH --output=%x_%j.out
#SBATCH --error=%x_%j.err

set -eo pipefail

MODE="${1:-}"
if [[ -z "${MODE}" ]]; then
    echo "ERROR: mode argument required.  Usage: sbatch run_plots_gen.sh <1|2>" >&2
    exit 1
fi
if [[ "${MODE}" != "1" && "${MODE}" != "2" ]]; then
    echo "ERROR: mode must be 1 or 2 (got '${MODE}')" >&2
    exit 1
fi

# SLURM_SUBMIT_DIR is set by sbatch to the directory sbatch was called from.
# Fallback to BASH_SOURCE when running interactively.
SCRIPT_DIR="${SLURM_SUBMIT_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)}"

if [[ "${MODE}" == "1" ]]; then
    XML_PATH="${SCRIPT_DIR}/input_gt_vs_tracker.xml"
else
    XML_PATH="${SCRIPT_DIR}/input_tracker_vs_tracker.xml"
fi

echo "=== run_plots_gen.sh starting ==="
echo "Mode:       ${MODE}"
echo "SCRIPT_DIR: ${SCRIPT_DIR}"
echo "XML_PATH:   ${XML_PATH}"

# ---------------------------------------------------------------------------- #
# 1.  Load required modules
# ---------------------------------------------------------------------------- #
echo "Loading modules..."
module load slurm             2>/dev/null || true
module load singularity/3.8.0 2>/dev/null || module load singularity 2>/dev/null || true
echo "Modules loaded."

# ---------------------------------------------------------------------------- #
# 2.  Parse Output_Path from the relevant xml
# ---------------------------------------------------------------------------- #
if [[ ! -f "${XML_PATH}" ]]; then
    echo "ERROR: ${XML_PATH} not found" >&2
    exit 1
fi

xml_get() {
    sed -n "s|.*<${1}>\(.*\)</${1}>.*|\1|p" "${XML_PATH}" | tr -d '[:space:]'
}

OUTPUT_PATH="$(xml_get Output_Path)"
if [[ -z "${OUTPUT_PATH}" ]]; then
    echo "ERROR: <Output_Path> not found in ${XML_PATH}" >&2
    exit 1
fi

echo "OUTPUT_PATH: ${OUTPUT_PATH}"

# ---------------------------------------------------------------------------- #
# 3.  Apply umask 022 and ensure Output_Path exists with correct permissions
# ---------------------------------------------------------------------------- #
umask 022

mkdir -p "${OUTPUT_PATH}" 2>/dev/null || true
chmod 755 "${OUTPUT_PATH}" 2>/dev/null && echo "chmod 755: ${OUTPUT_PATH}" \
    || echo "WARN: chmod failed for ${OUTPUT_PATH} (continuing)"

# ---------------------------------------------------------------------------- #
# 4.  Run plots_gen.py passing the chosen mode
# ---------------------------------------------------------------------------- #
echo "=== Launching plots_gen.py --mode ${MODE} ==="
python3 "${SCRIPT_DIR}/plots_gen.py" --mode "${MODE}"
echo "=== Done ==="