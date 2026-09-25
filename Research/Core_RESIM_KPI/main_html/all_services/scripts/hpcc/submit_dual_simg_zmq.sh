#!/usr/bin/env bash
#SBATCH --job-name=intplot_kpi_dual
#SBATCH --output=%x_%j.out
#SBATCH --error=%x_%j.err
#SBATCH --partition=plcyf-com
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=64G
#SBATCH --time=04:00:00

set -euo pipefail

# Slurm submission wrapper.
# You can override variables via sbatch --export=...
# Example:
# sbatch --export=CONFIG_XML=/shared/ConfigInteractivePlots.xml,INPUT_JSON=/shared/InputsInteractivePlot.json,OUT_DIR=/shared/out scripts/hpcc/submit_dual_simg_zmq.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORK_ROOT_DEFAULT="$(cd "$SCRIPT_DIR/../.." && pwd)"

WORK_ROOT="${WORK_ROOT:-$WORK_ROOT_DEFAULT}"
INTERACTIVE_SIMG="${INTERACTIVE_SIMG:-$WORK_ROOT/intplot_kpi/interactiveplot.simg}"
KPI_SIMG="${KPI_SIMG:-$WORK_ROOT/UDP_KPI/kpi.simg}"
CONFIG_XML="${CONFIG_XML:-$WORK_ROOT/intplot_kpi/ConfigInteractivePlots.xml}"
INPUT_JSON="${INPUT_JSON:-$WORK_ROOT/intplot_kpi/InputsInteractivePlot.json}"
OUT_DIR="${OUT_DIR:-$WORK_ROOT/intplot_kpi/out_hpcc}"
LOG_DIR="${LOG_DIR:-$OUT_DIR/simg_logs}"
PORT="${PORT:-5560}"

mkdir -p "$OUT_DIR" "$LOG_DIR"
umask 022

bash "$WORK_ROOT/scripts/hpcc/run_dual_simg_zmq.sh" \
  --interactive-simg "$INTERACTIVE_SIMG" \
  --kpi-simg "$KPI_SIMG" \
  --config-xml "$CONFIG_XML" \
  --input-json "$INPUT_JSON" \
  --int-output "$OUT_DIR" \
  --port "$PORT" \
  --log-dir "$LOG_DIR" \
  --bind "$WORK_ROOT:$WORK_ROOT"
