#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="$ROOT_DIR/simg_sh_hpcc"

zipapp_dir="$(mktemp -d)"
trap 'rm -rf "$zipapp_dir"' EXIT

cp "$ROOT_DIR/hpcc_main.py" "$zipapp_dir/hpcc_main.py"
cp "$ROOT_DIR/hpcc_runtime_store.py" "$zipapp_dir/hpcc_runtime_store.py"
cat > "$zipapp_dir/__main__.py" <<'PY'
from hpcc_main import main

if __name__ == '__main__':
    main()
PY

python3 -m zipapp "$zipapp_dir" -o "$OUT_DIR/hpcc_main.pyz" -p '/usr/bin/env python3'
chmod +x "$OUT_DIR/hpcc_main.pyz"

cp "$ROOT_DIR/main_hpcc.sh" "$OUT_DIR/main_hpcc.sh"
chmod +x "$OUT_DIR/main_hpcc.sh"

cp "$ROOT_DIR/bundle_common.sh" "$OUT_DIR/bundle_common.sh"
chmod +x "$OUT_DIR/bundle_common.sh"

mkdir -p "$OUT_DIR/bundle_src/main_html"
rm -rf "$OUT_DIR/bundle_src/main_html"
cp -a "$ROOT_DIR/main_html" "$OUT_DIR/bundle_src/main_html"
rm -rf "$OUT_DIR/bundle_src/main_html/__pycache__" "$OUT_DIR/bundle_src/main_html/simg/.cache_html"
find "$OUT_DIR/bundle_src/main_html" -type d -name '__pycache__' -prune -exec rm -rf {} +
: > "$OUT_DIR/bundle_src/main_html/__init__.py"

if [[ -d "$ROOT_DIR/Hyperlink_tool" ]]; then
    rm -rf "$OUT_DIR/bundle_src/Hyperlink_tool"
    cp -a "$ROOT_DIR/Hyperlink_tool" "$OUT_DIR/bundle_src/Hyperlink_tool"
    rm -rf "$OUT_DIR/bundle_src/Hyperlink_tool/code/.vscode"
    find "$OUT_DIR/bundle_src/Hyperlink_tool" -type d -name '__pycache__' -prune -exec rm -rf {} +
fi