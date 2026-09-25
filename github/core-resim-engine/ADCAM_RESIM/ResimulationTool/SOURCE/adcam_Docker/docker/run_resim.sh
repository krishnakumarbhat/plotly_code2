#!/bin/bash
# =============================================================================
# run_docker.sh  –  Host-side launcher for the apt-srr-resim Docker container
#
# Usage:
#   ./run_docker.sh <flist.txt> <output_path>
#
#   flist.txt    Absolute path to the file listing MF4 input log files
#                (one absolute Linux path per line).
#   output_path  Absolute (or relative) path to the directory where RESIM
#                output files should be written.
#
# What this script does:
#   1. Reads flist.txt to determine the common ancestor directory of all
#      listed MF4 files (the "MF4 root") and mounts it read-only inside
#      the container at /data/mf4.
#   2. Rewrites flist.txt entries to container-internal paths and places
#      the result in a temporary directory mounted at /data/flist.
#   3. Mounts the output directory at /data/output (created if absent).
#   4. Runs the apt-srr-resim container with those mounts.
#
# Example:
#   ./run_docker.sh /work/logs/my_drive/flist.txt /work/results/resim_out
# =============================================================================
set -euo pipefail

IMAGE_NAME="ifv600-resim:latest"

# ---------------------------------------------------------------------------
# Usage / argument check
# ---------------------------------------------------------------------------
usage() {
    echo "Usage: $0 <flist.txt> <output_path>"
    echo ""
    echo "  flist.txt    Path to the file listing MF4 input log files (one per line)."
    echo "               All paths must be absolute Linux paths readable on this host."
    echo "  output_path  Host directory where RESIM output will be written."
    exit 1
}

[ $# -lt 2 ] && usage

FLIST_HOST="$(realpath "$1")"
OUTPUT_HOST="$(realpath -m "$2")"   # -m: do not require the dir to exist yet

[ -f "$FLIST_HOST" ] || { echo "ERROR: flist.txt not found: $FLIST_HOST" >&2; exit 1; }

# ---------------------------------------------------------------------------
# Resolve the common ancestor (MF4 root) of all paths in flist.txt
# ---------------------------------------------------------------------------
MF4_ROOT=""

while IFS= read -r raw_line || [ -n "$raw_line" ]; do
    # Strip Windows-style carriage return and normalise backslashes to forward
    line="${raw_line//$'\r'/}"
    line="${line//\\//}"
    # Skip blank lines and comment lines
    [[ -z "$line" || "$line" == \#* ]] && continue

    d="$(dirname "$line")"

    if [ -z "$MF4_ROOT" ]; then
        MF4_ROOT="$d"
    else
        # Walk upward until MF4_ROOT is a prefix of d
        while [[ "$d" != "$MF4_ROOT" && "$d" != "$MF4_ROOT/"* ]]; do
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
    echo "       Ensure flist.txt contains valid absolute Linux paths." >&2
    exit 1
fi

# ---------------------------------------------------------------------------
# Rewrite flist.txt with container-internal paths
# ---------------------------------------------------------------------------
CONTAINER_MF4_MOUNT="/data/mf4"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "${TMP_DIR}"' EXIT

REWRITTEN_FLIST="${TMP_DIR}/flist.txt"

while IFS= read -r raw_line || [ -n "$raw_line" ]; do
    line="${raw_line//$'\r'/}"
    line="${line//\\//}"
    [[ -z "$line" || "$line" == \#* ]] && continue
    # Strip the host MF4 root prefix and replace with container mount path
    rel="${line#$MF4_ROOT}"
    # Ensure exactly one leading slash between mount and relative path
    rel="${rel#/}"
    echo "${CONTAINER_MF4_MOUNT}/${rel}"
done < "$FLIST_HOST" > "$REWRITTEN_FLIST"

# ---------------------------------------------------------------------------
# Prepare output directory
# ---------------------------------------------------------------------------
mkdir -p "$OUTPUT_HOST"

# ---------------------------------------------------------------------------
# Run the container
# ---------------------------------------------------------------------------
echo "=== APT_SRR_RESIM Docker Runner ==="
echo "  Image      : ${IMAGE_NAME}"
echo "  MF4 root   : ${MF4_ROOT}  ->  ${CONTAINER_MF4_MOUNT}  (read-only)"
echo "  flist.txt  : ${FLIST_HOST}  ->  /data/flist/flist.txt"
echo "  Output     : ${OUTPUT_HOST}  ->  /data/output"
echo ""

cd /app
export LD_LIBRARY_PATH=/app/:$LD_LIBRARY_PATH
exec ./APT_SRR_RESIM \
    /app/SIL_Engine_Config.xml \
    "$FLIST_HOST" \
    "$OUTPUT_HOST"
