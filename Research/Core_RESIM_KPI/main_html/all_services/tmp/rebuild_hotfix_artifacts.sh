#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="/mnt/c/Users/ouymc2/Desktop/simg_zmq"
OUT_DIR="$ROOT_DIR/simg_sh_hpcc"

mkdir -p "$OUT_DIR"

BUILDER="apptainer"
if ! command -v "$BUILDER" >/dev/null 2>&1; then
  BUILDER="singularity"
fi

echo "[build] main_html.simg"
"$BUILDER" build --force --fakeroot "$OUT_DIR/main_html.simg" "$ROOT_DIR/Singularity.def"

echo "[build] hpcc_main.pyz"
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

echo "[done] updated $OUT_DIR/main_html.simg and $OUT_DIR/hpcc_main.pyz"