#!/bin/bash

# =============================================================================
# run_singularity.sh – Host-side launcher for the ifv600-resim Singularity image
#
# Usage:
#   ./run_singularity.sh <flist.txt> <output_path>
#
#   flist.txt    Absolute path to the file listing MF4 input log files
#                (one absolute Linux path per line).
#   output_path  Absolute (or relative) path to the directory where RESIM
#                output files should be written.
#
# What this script does:
#   1. Resolves flist.txt and output_path on the host.
#   2. Finds the common ancestor directory of all MF4 files in flist.txt.
#   3. Binds the flist location, MF4 root, and output directory into the
#      container at the same absolute paths.
#   4. Runs the prebuilt Singularity image from adcam_Docker/build/.
# =============================================================================

set -eu

SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
IMAGE_PATH="${SCRIPT_DIR}/build/ifv600-resim.simg"
CONFIG_HOST="/home/kiran22ns1b/wkspUbuntu24/wslResimConf/SIL_XML/SIL_Engine_Config_Adcam.xml"

usage() {
    echo "Usage: $0 <flist.txt> <output_path>"
    echo ""
    echo "  flist.txt    Path to the file listing MF4 input log files (one per line)."
    echo "               All paths must be absolute Linux paths readable on this host."
    echo "  output_path  Host directory where RESIM output will be written."
    exit 1
}

[ $# -lt 2 ] && usage
[ -f "$IMAGE_PATH" ] || { echo "ERROR: Singularity image not found: $IMAGE_PATH" >&2; exit 1; }
command -v singularity >/dev/null 2>&1 || { echo "ERROR: singularity is not on PATH." >&2; exit 1; }
[ -f "$CONFIG_HOST" ] || { echo "ERROR: Config file not found: $CONFIG_HOST" >&2; exit 1; }

FLIST_HOST="$(realpath "$1")"
OUTPUT_HOST="$(realpath -m "$2")"

[ -f "$FLIST_HOST" ] || { echo "ERROR: flist.txt not found: $FLIST_HOST" >&2; exit 1; }

MF4_ROOT=""

while IFS= read -r raw_line || [ -n "$raw_line" ]; do
    line="${raw_line//$'\r'/}"
    line="${line//\\//}"
    [ -z "$line" ] && continue
    [ "${line#\#}" != "$line" ] && continue

    d="$(dirname "$line")"

    if [ -z "$MF4_ROOT" ]; then
        MF4_ROOT="$d"
    else
        while [ "$d" != "$MF4_ROOT" ] && [ "${d#${MF4_ROOT}/}" = "$d" ]; do
            MF4_ROOT="$(dirname "$MF4_ROOT")"
            [ "$MF4_ROOT" = "/" ] && break
        done
    fi
done < "$FLIST_HOST"

if [ -z "$MF4_ROOT" ]; then
    echo "ERROR: flist.txt contains no valid file paths." >&2
    exit 1
fi

if [ ! -d "$MF4_ROOT" ]; then
    echo "ERROR: Resolved MF4 root directory does not exist: $MF4_ROOT" >&2
    exit 1
fi

mkdir -p "$OUTPUT_HOST"

FLIST_DIR="$(dirname "$FLIST_HOST")"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

REWRITTEN_FLIST="${TMP_DIR}/flist.txt"

while IFS= read -r raw_line || [ -n "$raw_line" ]; do
    line="${raw_line//$'\r'/}"
    line="${line//\\//}"
    [ -z "$line" ] && continue
    [ "${line#\#}" != "$line" ] && continue

    rel="${line#$MF4_ROOT}"
    rel="${rel#/}"
    printf '%s\n' "/data/mf4/${rel}"
done < "$FLIST_HOST" > "$REWRITTEN_FLIST"

echo "=== APT_SRR_RESIM Singularity Runner ==="
echo "  Image      : ${IMAGE_PATH}"
echo "  FLIST      : ${FLIST_HOST}"
echo "  MF4 root   : ${MF4_ROOT}  ->  /data/mf4  (read-only)"
echo "  Output     : ${OUTPUT_HOST}  ->  ${OUTPUT_HOST}"
echo ""

exec singularity exec \
    --bind "${MF4_ROOT}:/data/mf4:ro" \
    --bind "${TMP_DIR}:/data/flist:ro" \
    --bind "${OUTPUT_HOST}:${OUTPUT_HOST}:rw" \
    "$IMAGE_PATH" \
    /bin/sh -lc 'BIN_DIR=/app/Release; [ -x /app/APT_SRR_RESIM ] && BIN_DIR=/app || true; cd "$BIN_DIR"; export LD_LIBRARY_PATH="$BIN_DIR:${LD_LIBRARY_PATH:-}"; BIN="${BIN_DIR}/APT_SRR_RESIM"; exec "$BIN" "'"$CONFIG_HOST"'" /data/flist/flist.txt "'"$OUTPUT_HOST"'"'