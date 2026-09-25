#!/bin/bash
# ==============================================================================
# launch.sh - Interactive launcher
# ==============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

module load slurm 2>/dev/null || true
if ! command -v sbatch &>/dev/null; then
    echo "ERROR: sbatch not found. Load the slurm module manually and retry:" >&2
    echo "  module load slurm && bash launch.sh" >&2
    exit 1
fi

echo ""
echo "Select comparison mode:"
echo "  1) GroundTruth vs TrackerOutput"
echo "     (HDF_Input vs HDF_Output from the same resim run)"
echo "     Config: input.xml"
echo ""
echo "  2) TrackerOutput vs TrackerOutput"
echo "     (HDF_Output from resim run 1 vs HDF_Output from resim run 2)"
echo "     Config: input_tracker_vs_tracker.xml"
echo ""
read -rp "Enter mode [1/2]: " MODE

if [[ "${MODE}" != "1" && "${MODE}" != "2" ]]; then
    echo "ERROR: Invalid mode '${MODE}'. Must be 1 or 2." >&2
    exit 1
fi

echo ""
echo "Submitting sbatch job with mode ${MODE}..."
sbatch "${SCRIPT_DIR}/run_plots_gen.sh" "${MODE}"
