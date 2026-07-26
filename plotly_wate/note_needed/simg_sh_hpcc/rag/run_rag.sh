#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export BUNDLE_ROOT="${HPCC_BUNDLE_ROOT:-$(cd "$SCRIPT_DIR/.." && pwd)}"
export HPCC_PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$BUNDLE_ROOT/bundle_src}"
# shellcheck disable=SC1091
source "$BUNDLE_ROOT/bundle_common.sh"

IMAGE_PATH="$SCRIPT_DIR/rag.simg"
bundle_ensure_file "$IMAGE_PATH"

MODE='talk'
HTML_ROOT=''
if [[ "${1:-}" == '--scrap' ]]; then
    MODE='scrap'
    HTML_ROOT="$(bundle_abs_path "$2")"
    bundle_ensure_file "$HTML_ROOT"
elif [[ "${1:-}" == '-h' || "${1:-}" == '--help' ]]; then
    echo 'Usage: run_rag.sh [--talk] | [--scrap <html_root>]' >&2
    exit 0
fi

export FLASK_PORT="${FLASK_PORT:-5100}"
if [[ "$MODE" == 'scrap' ]]; then
    export HTML_ROOT_PATH="$HTML_ROOT"
    bundle_run_image "$IMAGE_PATH" --scrap "$HTML_ROOT"
    exit $?
fi

bundle_run_image "$IMAGE_PATH" --talk