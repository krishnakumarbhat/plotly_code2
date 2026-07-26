#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PYTHONPATH="$SCRIPT_DIR:$SCRIPT_DIR/app${PYTHONPATH:+:$PYTHONPATH}"

FLASK_HOST="${FLASK_HOST:-127.0.0.1}"
FLASK_PORT="${FLASK_PORT:-5100}"
LLAMA_SERVER_PORT="${LLAMA_SERVER_PORT:-8081}"
QWEN_GGUF="${QWEN_GGUF:-$(find "$SCRIPT_DIR" -name '*.gguf' -print -quit 2>/dev/null || echo '')}"

export FLASK_HOST FLASK_PORT LLAMA_SERVER_PORT
export LLM_BACKEND="${LLM_BACKEND:-llama_server}"
export LLAMA_SERVER_AUTOSTART="${LLAMA_SERVER_AUTOSTART:-true}"
export VECTOR_BACKEND="${VECTOR_BACKEND:-chroma}"
export RAG_AUTO_INGEST_ON_START="${RAG_AUTO_INGEST_ON_START:-true}"

[ -n "$QWEN_GGUF" ] && export QWEN_GGUF_PATH="$QWEN_GGUF"

exec python3 "$SCRIPT_DIR/run.py" --talk "$@"
