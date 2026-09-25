#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="$ROOT_DIR/simg_sh_hpcc"

if ! command -v apptainer >/dev/null 2>&1 && ! command -v singularity >/dev/null 2>&1; then
    echo "Apptainer/Singularity is required in WSL to build the HPCC bundle." >&2
    exit 1
fi

BUILDER="apptainer"
if ! command -v "$BUILDER" >/dev/null 2>&1; then
    BUILDER="singularity"
fi

mkdir -p "$OUT_DIR"

prepare_bundle_layout() {
    mkdir -p "$OUT_DIR" "$OUT_DIR/kpi/can" "$OUT_DIR/kpi/udp" "$OUT_DIR/kpi/int_plot" "$OUT_DIR/rag"
    rm -f \
        "$OUT_DIR/can_kpi.simg" \
        "$OUT_DIR/udp_kpi.simg" \
        "$OUT_DIR/interactiveplot.simg" \
        "$OUT_DIR/rag.simg" \
        "$OUT_DIR/kpi.simg"
}

build_broker_zipapp() {
    local zipapp_dir
    zipapp_dir="$(mktemp -d)"

    cp "$ROOT_DIR/hpcc_main.py" "$zipapp_dir/hpcc_main.py"
    cp "$ROOT_DIR/hpcc_runtime_store.py" "$zipapp_dir/hpcc_runtime_store.py"
    cat > "$zipapp_dir/__main__.py" <<'PY'
from hpcc_main import main

if __name__ == '__main__':
    main()
PY

    python3 -m zipapp "$zipapp_dir" -o "$OUT_DIR/hpcc_main.pyz" -p '/usr/bin/env python3'
    chmod +x "$OUT_DIR/hpcc_main.pyz"
    rm -rf "$zipapp_dir"
}

sync_bundle_support_files() {
    local bundle_src_dir="$OUT_DIR/bundle_src"

    rm -rf "$bundle_src_dir"
    mkdir -p "$bundle_src_dir" "$OUT_DIR/kpi/can" "$OUT_DIR/kpi/udp" "$OUT_DIR/kpi/int_plot" "$OUT_DIR/rag"
    rm -f "$OUT_DIR/hpcc_main.py" "$OUT_DIR/hpcc_runtime_store.py"

    build_broker_zipapp

    rm -rf "$bundle_src_dir/main_html"
    cp -a "$ROOT_DIR/main_html" "$bundle_src_dir/main_html"
    rm -rf "$bundle_src_dir/main_html/__pycache__" "$bundle_src_dir/main_html/simg/.cache_html"
    find "$bundle_src_dir/main_html" -type d -name '__pycache__' -prune -exec rm -rf {} +
    : > "$bundle_src_dir/main_html/__init__.py"

    rm -rf "$bundle_src_dir/Hyperlink_tool"
    cp -a "$ROOT_DIR/Hyperlink_tool" "$bundle_src_dir/Hyperlink_tool"
    rm -rf "$bundle_src_dir/Hyperlink_tool/code/.vscode"
    find "$bundle_src_dir/Hyperlink_tool" -type d -name '__pycache__' -prune -exec rm -rf {} +

    mkdir -p "$bundle_src_dir/KPI/intplot_kpi"
    cp "$ROOT_DIR/KPI/intplot_kpi/ConfigInteractivePlots.xml" "$bundle_src_dir/KPI/intplot_kpi/ConfigInteractivePlots.xml"
    cp "$ROOT_DIR/KPI/intplot_kpi/ConfigInteractivePlots_bordnet.xml" "$bundle_src_dir/KPI/intplot_kpi/ConfigInteractivePlots_bordnet.xml"
    cp "$ROOT_DIR/KPI/intplot_kpi/config.json" "$bundle_src_dir/KPI/intplot_kpi/config.json"
    cp "$ROOT_DIR/KPI/intplot_kpi/Inputperstreamplot.json" "$bundle_src_dir/KPI/intplot_kpi/Inputperstreamplot.json"
    cp "$ROOT_DIR/KPI/intplot_kpi/InputsInteractivePlot.json" "$bundle_src_dir/KPI/intplot_kpi/InputsInteractivePlot.json"
    cp "$ROOT_DIR/KPI/intplot_kpi/InputsPerSensorInteractivePlot.json" "$bundle_src_dir/KPI/intplot_kpi/InputsPerSensorInteractivePlot.json"

    cp "$ROOT_DIR/KPI/intplot_kpi/ConfigInteractivePlots.xml" "$OUT_DIR/ConfigInteractivePlots.xml"
    cp "$ROOT_DIR/KPI/intplot_kpi/ConfigInteractivePlots_bordnet.xml" "$OUT_DIR/ConfigInteractivePlots_bordnet.xml"

    cp "$ROOT_DIR/bundle_common.sh" "$OUT_DIR/bundle_common.sh"
    cp "$ROOT_DIR/main_hpcc.sh" "$OUT_DIR/main_hpcc.sh"
    cp "$ROOT_DIR/run_hpcc_stack.sh" "$OUT_DIR/run_hpcc_stack.sh"
    cp "$ROOT_DIR/kpi_runtime_launcher.sh" "$OUT_DIR/kpi_runtime_launcher.sh"
    cp "$ROOT_DIR/scripts/cleanup_memory.sh" "$OUT_DIR/cleanup_memory.sh"
    cp "$ROOT_DIR/Readme/README_kpi.md" "$OUT_DIR/README_kpi.md"
    cp "$ROOT_DIR/Readme/README_hpcc_integration.md" "$OUT_DIR/README_hpcc_integration.md"

    cp -a "$ROOT_DIR/scripts/hpcc_bundle_templates/kpi/." "$OUT_DIR/kpi/"
    cp -a "$ROOT_DIR/scripts/hpcc_bundle_templates/rag/." "$OUT_DIR/rag/"

    cat > "$OUT_DIR/BUNDLE_INFO.txt" <<EOF
HPCC standalone bundle
generated_at=$(date -Iseconds 2>/dev/null || date)
source_root=$ROOT_DIR
bundle_root=$OUT_DIR
contains=hpcc_main.pyz,bundle_src/,bundle_common.sh,cleanup_memory.sh,main_hpcc.sh,run_hpcc_stack.sh,main_html.simg,kpi/can/can_kpi.simg,kpi/udp/udp_kpi.simg,kpi/int_plot/intplot_kpi.simg,rag/rag.simg
launch=PORT=5005 HPCC_BROKER_PORT=9105 ./main_hpcc.sh
direct_url=http://10.214.45.45:<shifted_port>/
public_host_override=HPCC_PUBLIC_HOST=10.214.45.45
tunnel=ssh -N -L <local_port>:127.0.0.1:<remote_port> <user>@<login-node>
EOF

    chmod +x "$OUT_DIR/bundle_common.sh" "$OUT_DIR/cleanup_memory.sh" "$OUT_DIR/main_hpcc.sh" "$OUT_DIR/kpi_runtime_launcher.sh" "$OUT_DIR/run_hpcc_stack.sh"
    find "$OUT_DIR/kpi" -type f -name '*.sh' -exec chmod +x {} +
    find "$OUT_DIR/rag" -type f -name '*.sh' -exec chmod +x {} +
}

build_image() {
    local target_name="$1"
    local definition_file="$2"

    echo "[build] $target_name <- $definition_file"
    mkdir -p "$(dirname "$OUT_DIR/$target_name")"
    "$BUILDER" build --force --fakeroot "$OUT_DIR/$target_name" "$definition_file"
}

prepare_bundle_layout
build_image "main_html.simg" "$ROOT_DIR/Singularity.def"
build_image "kpi/can/can_kpi.simg" "$ROOT_DIR/KPI/can_kpi/can_singularity_KPI.def"
build_image "kpi/udp/udp_kpi.simg" "$ROOT_DIR/KPI/UDP_KPI/Singularity_KPI.def"
build_image "kpi/int_plot/intplot_kpi.simg" "$ROOT_DIR/KPI/intplot_kpi/singularity_interactiveplot.def"
build_image "rag/rag.simg" "$ROOT_DIR/rag/Singularity_RAG.def"
sync_bundle_support_files

echo "[done] HPCC bundle written to $OUT_DIR"