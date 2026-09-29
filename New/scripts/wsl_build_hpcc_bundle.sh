#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
OUT_DIR="${1:-$ROOT_DIR/generate_upload}"
TMP_DIR="$(mktemp -d)"

cleanup() {
    rm -rf "$TMP_DIR"
}
trap cleanup EXIT

runtime_bin() {
    if command -v apptainer >/dev/null 2>&1; then
        printf '%s' apptainer
        return 0
    fi
    if command -v singularity >/dev/null 2>&1; then
        printf '%s' singularity
        return 0
    fi
    return 1
}

if ! RUNTIME_BIN="$(runtime_bin)"; then
    echo 'Apptainer/Singularity is required to build the HPCC bundle.' >&2
    exit 1
fi

mkdir -p \
    "$OUT_DIR" \
    "$OUT_DIR/kpi/can" \
    "$OUT_DIR/kpi/udp" \
    "$OUT_DIR/kpi/int_plot" \
    "$OUT_DIR/rag" \
    "$OUT_DIR/bundle_src/KPI"

python3 "$ROOT_DIR/build_static.py"

cp "$ROOT_DIR/main_html/temp_dir/hpcc_main.py" "$TMP_DIR/hpcc_main.py"
cp "$ROOT_DIR/main_html/temp_dir/hpcc_runtime_store.py" "$TMP_DIR/hpcc_runtime_store.py"
cat > "$TMP_DIR/__main__.py" <<'PY'
from hpcc_main import main

if __name__ == '__main__':
    main()
PY
python3 -m zipapp "$TMP_DIR" -o "$OUT_DIR/hpcc_main.pyz" -p '/usr/bin/env python3'
chmod +x "$OUT_DIR/hpcc_main.pyz"

cp "$ROOT_DIR/bundle_common.sh" "$OUT_DIR/"
cp "$ROOT_DIR/main_hpcc.sh" "$OUT_DIR/"
cp "$ROOT_DIR/run_hpcc.sh" "$OUT_DIR/"
cp "$ROOT_DIR/run_hpcc_stack.sh" "$OUT_DIR/"
cp "$ROOT_DIR/kpi_runtime_launcher.sh" "$OUT_DIR/"
cp "$ROOT_DIR/cleanup_memory.sh" "$OUT_DIR/"
cp "$ROOT_DIR/rag/run_rag.sh" "$OUT_DIR/rag/"
chmod +x \
    "$OUT_DIR/main_hpcc.sh" \
    "$OUT_DIR/run_hpcc.sh" \
    "$OUT_DIR/run_hpcc_stack.sh" \
    "$OUT_DIR/kpi_runtime_launcher.sh" \
    "$OUT_DIR/cleanup_memory.sh" \
    "$OUT_DIR/rag/run_rag.sh"

rm -rf "$OUT_DIR/bundle_src/main_html" "$OUT_DIR/bundle_src/Hyperlink_tool" "$OUT_DIR/bundle_src/KPI/intplot_kpi"
cp -a "$ROOT_DIR/main_html" "$OUT_DIR/bundle_src/main_html"
cp -a "$ROOT_DIR/Hyperlink_tool" "$OUT_DIR/bundle_src/Hyperlink_tool"
cp -a "$ROOT_DIR/KPI/intplot_kpi" "$OUT_DIR/bundle_src/KPI/intplot_kpi"

"$RUNTIME_BIN" build --force --fakeroot "$OUT_DIR/main_html.simg" "$ROOT_DIR/Singularity.def"
"$RUNTIME_BIN" build --force --fakeroot "$OUT_DIR/kpi/can/can_kpi.simg" "$ROOT_DIR/KPI/can_kpi/can_singularity_KPI.def"
"$RUNTIME_BIN" build --force --fakeroot "$OUT_DIR/kpi/udp/udp_kpi.simg" "$ROOT_DIR/KPI/UDP_KPI/Singularity_KPI.def"
"$RUNTIME_BIN" build --force --fakeroot "$OUT_DIR/kpi/int_plot/intplot_kpi.simg" "$ROOT_DIR/KPI/intplot_kpi/singularity_interactiveplot.def"
"$RUNTIME_BIN" build --force --fakeroot "$OUT_DIR/rag/rag.simg" "$ROOT_DIR/rag/Singularity_RAG.def"

echo "Built HPCC bundle at $OUT_DIR"