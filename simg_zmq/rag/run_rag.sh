#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUNDLE_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$BUNDLE_ROOT/bundle_src}"

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

IMAGE_PATH="$SCRIPT_DIR/rag.simg"
if [[ ! -f "$IMAGE_PATH" && -f "$BUNDLE_ROOT/rag.simg" ]]; then
    IMAGE_PATH="$BUNDLE_ROOT/rag.simg"
    echo "Using $IMAGE_PATH"
elif [[ ! -f "$IMAGE_PATH" && -f "$BUNDLE_ROOT/generate_upload/rag/rag.simg" ]]; then
    IMAGE_PATH="$BUNDLE_ROOT/generate_upload/rag/rag.simg"
    echo "Using $IMAGE_PATH"
elif [[ ! -f "$IMAGE_PATH" ]]; then
    echo "Missing rag.simg next to run_rag.sh or under generate_upload/rag/." >&2
    exit 1
fi

if ! RUNTIME_BIN="$(runtime_bin)"; then
    echo 'Apptainer/Singularity is required to run rag.simg.' >&2
    exit 1
fi

resolve_model_path() {
    if [[ -n "${QWEN_GGUF_PATH:-}" && -f "${QWEN_GGUF_PATH:-}" ]]; then
        printf '%s' "$QWEN_GGUF_PATH"
        return 0
    fi
    local candidate
    for candidate in \
        "${QWEN_MODEL_DIR:-}" \
        "$PROJECT_ROOT/rag/model" \
        "$BUNDLE_ROOT/rag/model" \
        "$PROJECT_ROOT/rag/qwn_kk_fine_model" \
        "$BUNDLE_ROOT/rag/qwn_kk_fine_model" \
        "$PROJECT_ROOT/qwn_kk_fine_model" \
        "$BUNDLE_ROOT/qwn_kk_fine_model"; do
        [[ -n "$candidate" && -d "$candidate" ]] || continue
        candidate="$(find "$candidate" -maxdepth 2 -type f -name '*.gguf' | head -n 1)"
        if [[ -n "$candidate" && -f "$candidate" ]]; then
            printf '%s' "$candidate"
            return 0
        fi
    done
    return 1
}

if [[ -z "${RAG_CACHE_ROOT:-}" ]]; then
    if [[ -n "${HPCC_RUNTIME_LOCAL_ROOT:-}" ]]; then
        export RAG_CACHE_ROOT="${HPCC_RUNTIME_LOCAL_ROOT}/cache_html"
    else
        export RAG_CACHE_ROOT="/tmp/hpc_tools/.cache_html"
    fi
fi
mkdir -p "$RAG_CACHE_ROOT/rag"

if [[ -z "${RAG_DATA_DIR:-}" ]]; then
    export RAG_DATA_DIR="$RAG_CACHE_ROOT/rag"
fi

bind_args=()
for host_path in "$BUNDLE_ROOT" "$PROJECT_ROOT" "$RAG_CACHE_ROOT" /net /scratch /mnt /local /tmp; do
    if [[ -n "$host_path" && -d "$host_path" ]]; then
        bind_args+=(--bind "$host_path:$host_path")
    fi
done

if model_path="$(resolve_model_path 2>/dev/null)"; then
    export QWEN_GGUF_PATH="$model_path"
    model_dir="$(dirname "$model_path")"
    if [[ -d "$model_dir" ]]; then
        bind_args+=(--bind "$model_dir:$model_dir")
    fi
fi

if [[ $# -eq 0 ]]; then
    set -- --talk
fi

cmd=(
    "$RUNTIME_BIN" run
    "${bind_args[@]}"
    --env "HPCC_BUNDLE_ROOT=$BUNDLE_ROOT"
    --env "HPCC_PROJECT_ROOT=$PROJECT_ROOT"
    --env "FLASK_HOST=${FLASK_HOST:-0.0.0.0}"
    --env "FLASK_PORT=${FLASK_PORT:-5100}"
    --env "RAG_CACHE_ROOT=$RAG_CACHE_ROOT"
    --env "RAG_DATA_DIR=$RAG_DATA_DIR"
    --env "VECTOR_BACKEND=${VECTOR_BACKEND:-chroma}"
)
if [[ -n "${QWEN_GGUF_PATH:-}" ]]; then
    cmd+=(--env "QWEN_GGUF_PATH=$QWEN_GGUF_PATH")
fi
cmd+=("$IMAGE_PATH" "$@")

exec "${cmd[@]}"