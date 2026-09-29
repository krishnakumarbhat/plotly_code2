#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
WORKSPACE_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)

IMAGE=${IMAGE:-can-interactive-plot:e2e}
INPUT_HDF=${1:-/mnt/c/Users/ouymc2/Desktop/IFV7XX_WBATR95060NC89209_20250919_083635_0000_b04_HDF.h5}
OUTPUT_HDF=${2:-/mnt/c/Users/ouymc2/Desktop/IFV7XX_WBATR95060NC89209_20250919_083635_0000_b04_rR00100012_HDF.h5}
RESULT_DIR=${3:-/mnt/c/Users/ouymc2/Desktop/can_interactive_plot_e2e}

command -v docker >/dev/null 2>&1 || {
    echo "docker is required in WSL. Start Docker Desktop or install a Docker engine first." >&2
    exit 127
}

[ -f "$INPUT_HDF" ] || { echo "Input HDF not found: $INPUT_HDF" >&2; exit 1; }
[ -f "$OUTPUT_HDF" ] || { echo "Output HDF not found: $OUTPUT_HDF" >&2; exit 1; }

mkdir -p "$RESULT_DIR"
DATA_DIR=$(CDPATH= cd -- "$(dirname -- "$INPUT_HDF")" && pwd)
INPUT_NAME=$(basename -- "$INPUT_HDF")
OUTPUT_NAME=$(basename -- "$OUTPUT_HDF")
INPUT_JSON="$RESULT_DIR/inputs.json"

python3 - "$INPUT_NAME" "$OUTPUT_NAME" > "$INPUT_JSON" <<'PY'
import json
import sys

input_name, output_name = sys.argv[1:]
print(json.dumps({
    "INPUT_HDF": [f"/data/{input_name}"],
    "OUTPUT_HDF": [f"/data/{output_name}"],
}, indent=2))
PY

export DOCKER_BUILDKIT=${DOCKER_BUILDKIT:-1}
echo "Building $IMAGE"
docker build -f "$SCRIPT_DIR/Dockerfile" -t "$IMAGE" "$WORKSPACE_ROOT"

echo "Running combined CAN KPI server and Interactive Plot pipeline"
docker run --rm --init \
    --user "$(id -u):$(id -g)" \
    -e CAN_KPI_SERVER_LOG=/results/can_kpi_server.log \
    -v "$DATA_DIR:/data:ro" \
    -v "$RESULT_DIR:/results" \
    "$IMAGE" both \
    /app/can_interactive_plot/ConfigInteractivePlots_bordnet.xml \
    /results/inputs.json \
    /results/interactive

echo "Running standalone CAN KPI validation"
docker run --rm --init \
    --user "$(id -u):$(id -g)" \
    -v "$DATA_DIR:/data:ro" \
    -v "$RESULT_DIR:/results" \
    "$IMAGE" kpi \
    /results/inputs.json \
    /results/can_kpi

HTML_COUNT=$(find "$RESULT_DIR/interactive" -type f -name '*.html' | wc -l)
KPI_COUNT=$(find "$RESULT_DIR/can_kpi" -type f -name '*.html' | wc -l)
[ "$HTML_COUNT" -gt 0 ] || { echo "No interactive HTML files were generated" >&2; exit 1; }
[ "$KPI_COUNT" -gt 0 ] || { echo "No standalone CAN KPI HTML files were generated" >&2; exit 1; }

for label in Timestamp ScanIndex Range Elevation Azimuth; do
    if ! grep -Ril --include='*.html' -- "$label" "$RESULT_DIR/interactive" | head -n 1 | grep -q .; then
        echo "Requested plot label not found in interactive HTML: $label" >&2
        exit 1
    fi
done

if ! grep -Ril --include='*.html' -E -- 'RR|Rear Right|CEER_RR' "$RESULT_DIR/interactive" | head -n 1 | grep -q .; then
    echo "RR series label not found in interactive HTML" >&2
    exit 1
fi

printf 'E2E PASS\ninteractive_html=%s\ncan_kpi_html=%s\nresults=%s\n' \
    "$HTML_COUNT" "$KPI_COUNT" "$RESULT_DIR"
find "$RESULT_DIR" -type f -name '*.html' -print | sort
