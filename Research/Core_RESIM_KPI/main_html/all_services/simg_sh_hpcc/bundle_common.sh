#!/usr/bin/env bash
set -euo pipefail

bundle_timestamp() {
    date +%Y%m%d_%H%M%S 2>/dev/null || python3 - <<'PY'
from datetime import datetime
print(datetime.now().strftime('%Y%m%d_%H%M%S'))
PY
}

bundle_python() {
    if command -v python3 >/dev/null 2>&1; then
        printf '%s\n' python3
        return 0
    fi
    if command -v python >/dev/null 2>&1; then
        printf '%s\n' python
        return 0
    fi
    echo 'python3 or python is required.' >&2
    return 1
}

bundle_load_runtime_module() {
    if command -v apptainer >/dev/null 2>&1 || command -v singularity >/dev/null 2>&1; then
        return 0
    fi

    if [[ -f /etc/profile.d/modules.sh ]]; then
        # shellcheck disable=SC1091
        source /etc/profile.d/modules.sh >/dev/null 2>&1 || true
    fi

    if command -v module >/dev/null 2>&1; then
        module load singularity >/dev/null 2>&1 || module load apptainer >/dev/null 2>&1 || true
        return 0
    fi

    if command -v modulecmd >/dev/null 2>&1; then
        eval "$(modulecmd bash load singularity 2>/dev/null)" || eval "$(modulecmd bash load apptainer 2>/dev/null)" || true
    fi
}

bundle_runtime_bin() {
    bundle_load_runtime_module
    if command -v apptainer >/dev/null 2>&1; then
        command -v apptainer
        return 0
    fi
    if command -v singularity >/dev/null 2>&1; then
        command -v singularity
        return 0
    fi
    echo 'Apptainer/Singularity is required.' >&2
    return 1
}

bundle_abs_path() {
    local input_path="$1"
    local py
    py="$(bundle_python)"
    "$py" - "$input_path" <<'PY'
import os
import sys

print(os.path.abspath(os.path.expanduser(sys.argv[1])))
PY
}

bundle_ensure_file() {
    local file_path="$1"
    if [[ ! -f "$file_path" ]]; then
        echo "Missing file: $file_path" >&2
        return 1
    fi
}

bundle_default_output_dir() {
    local bundle_root="$1"
    local label="$2"
    local output_dir="$bundle_root/out/${label}_$(bundle_timestamp)"
    mkdir -p "$output_dir"
    printf '%s\n' "$output_dir"
}

bundle_default_config_xml() {
    local bundle_root="$1"
    local source_target="${2:-udp_kpi}"
    local file_name='ConfigInteractivePlots.xml'
    if [[ "$source_target" == 'can_kpi' ]]; then
        file_name='ConfigInteractivePlots_bordnet.xml'
    fi

    local candidate
    for candidate in \
        "$bundle_root/$file_name" \
        "$bundle_root/bundle_src/KPI/intplot_kpi/$file_name"
    do
        if [[ -f "$candidate" ]]; then
            printf '%s\n' "$candidate"
            return 0
        fi
    done

    printf '%s\n' "$bundle_root/$file_name"
}

bundle_set_container_env() {
    local key="$1"
    local value="$2"
    export "$key=$value"
    export "APPTAINERENV_${key}=$value"
    export "SINGULARITYENV_${key}=$value"
}

bundle_run_image() {
    local image_path="$1"
    shift

    local runtime_bin
    runtime_bin="$(bundle_runtime_bin)"

    local -a command
    command=("$runtime_bin" run)

    if [[ -n "${BUNDLE_ROOT:-}" && -d "${BUNDLE_ROOT:-}" ]]; then
        command+=(--bind "$BUNDLE_ROOT:$BUNDLE_ROOT")
    fi
    if [[ -n "${HPCC_PROJECT_ROOT:-}" && -d "${HPCC_PROJECT_ROOT:-}" ]]; then
        command+=(--bind "$HPCC_PROJECT_ROOT:$HPCC_PROJECT_ROOT")
    fi

    local host_path
    for host_path in /net /scratch /mnt; do
        if [[ -d "$host_path" ]]; then
            command+=(--bind "$host_path:$host_path")
        fi
    done

    command+=("$image_path" "$@")
    "${command[@]}"
}

bundle_wait_for_port() {
    local host="$1"
    local port="$2"
    local retries="${3:-60}"
    local delay="${4:-1}"
    local py
    py="$(bundle_python)"

    local attempt
    for attempt in $(seq 1 "$retries"); do
        if "$py" - "$host" "$port" <<'PY'
import socket
import sys

host = sys.argv[1]
port = int(sys.argv[2])
try:
    with socket.create_connection((host, port), timeout=1.0):
        pass
except OSError:
    raise SystemExit(1)
raise SystemExit(0)
PY
        then
            return 0
        fi
        sleep "$delay"
    done

    return 1
}

bundle_make_pair_json() {
    local input_hdf="$1"
    local output_hdf="$2"
    local py
    py="$(bundle_python)"
    local tmp_json="${TMPDIR:-/tmp}/hpcc_pair_$(bundle_timestamp)_$$.json"

    "$py" - "$input_hdf" "$output_hdf" "$tmp_json" <<'PY'
import json
import sys

input_hdf, output_hdf, target = sys.argv[1:4]
with open(target, 'w', encoding='utf-8') as handle:
    json.dump({'INPUT_HDF': [input_hdf], 'OUTPUT_HDF': [output_hdf]}, handle, indent=2)
PY

    printf '%s\n' "$tmp_json"
}

bundle_require_tmux() {
    if ! command -v tmux >/dev/null 2>&1; then
        echo 'tmux is required for detached KPI launches.' >&2
        return 1
    fi
}

bundle_user_run_dir() {
    local label="$1"
    local user_name
    user_name="$(id -un 2>/dev/null || printf '%s' user)"
    local run_dir="${BUNDLE_ROOT:-$(pwd)}/runs/$user_name/${label}_$(bundle_timestamp)"
    mkdir -p "$run_dir"
    printf '%s\n' "$run_dir"
}