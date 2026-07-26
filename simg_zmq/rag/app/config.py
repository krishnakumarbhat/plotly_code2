from __future__ import annotations

import os
from pathlib import Path


def _load_env_file(env_path: Path) -> None:
    if not env_path.exists():
        return
    try:
        lines = env_path.read_text(encoding='utf-8').splitlines()
    except OSError:
        return

    for line in lines:
        stripped = line.strip()
        if not stripped or stripped.startswith('#') or '=' not in stripped:
            continue
        key, value = stripped.split('=', 1)
        key = key.strip()
        value = value.strip().strip('"').strip("'")
        if key and key not in os.environ:
            os.environ[key] = value


class AppConfig:
    def __init__(self) -> None:
        self.project_root = Path(__file__).resolve().parents[1]
        _load_env_file(self.project_root / '.env')
        self.workspace_root = self.project_root.parent
        default_data_dir = self.workspace_root / '.store' / 'rag'
        self.data_dir = Path(self._sanitize_path(os.getenv('RAG_DATA_DIR', str(default_data_dir))))
        self.sqlite_path = Path(
            self._sanitize_path(os.getenv('RAG_SQLITE_PATH', str(self.data_dir / 'rag_logs.db')))
        )
        self.vector_store_dir = Path(
            self._sanitize_path(
                os.getenv('RAG_VECTOR_STORE_DIR', str(self.data_dir / 'chroma.db'))
            )
        )

        raw_html_root = os.getenv(
            "HTML_ROOT_PATH",
            str(self.workspace_root),
        )
        self.html_root_path = Path(self._sanitize_path(raw_html_root))
        self.file_keyword = os.getenv("HTML_FILE_KEYWORD", "")
        self.exclude_file_keyword = os.getenv("HTML_EXCLUDE_FILE_KEYWORD", "dummy")
        self.allow_all_html_fallback = os.getenv("ALLOW_ALL_HTML_FALLBACK", "true").lower() in {
            "1",
            "true",
            "yes",
            "y",
        }
        self.max_files_per_ingest = int(os.getenv("MAX_FILES_PER_INGEST", "0"))
        self.max_file_bytes = int(os.getenv("MAX_FILE_BYTES", str(2 * 1024 * 1024)))
        self.parse_english_only = os.getenv("PARSE_ENGLISH_ONLY", "true").lower() in {
            "1",
            "true",
            "yes",
            "y",
        }
        self.supported_extensions = [
            extension.strip().lower()
            for extension in os.getenv(
                "RAG_SUPPORTED_EXTENSIONS",
                ".html,.htm,.md,.txt,.json,.xml,.yaml,.yml,.py",
            ).split(",")
            if extension.strip()
        ]
        self.exclude_dirs = {
            directory.strip().lower()
            for directory in os.getenv(
                "RAG_EXCLUDE_DIRS",
                ".git,__pycache__,.pytest_cache,.venv,venv,node_modules,model,llm_model,.store,simg_sh_hpcc,generate_upload",
            ).split(",")
            if directory.strip()
        }
        self.chunk_size = int(os.getenv("CHUNK_SIZE", "1800"))
        self.chunk_overlap = int(os.getenv("CHUNK_OVERLAP", "160"))
        self.embedding_dimension = int(os.getenv("EMBEDDING_DIMENSION", "384"))
        self.vector_backend = (os.getenv("VECTOR_BACKEND", "chroma") or "chroma").strip().lower()
        self.max_session_messages = max(2, int(os.getenv("RAG_SESSION_MESSAGES", "6")))
        default_vector_store_path = self.vector_store_dir / "vector_store.json"
        self.vector_store_json_path = Path(
            self._sanitize_path(
                os.getenv("VECTOR_STORE_JSON_PATH", str(default_vector_store_path))
            )
        )
        self.collection_name = os.getenv("CHROMA_COLLECTION", "runtime_validation_logs")
        self.llm_similarity_top_k = int(os.getenv("LLM_SIMILARITY_TOP_K", "5"))
        self.auto_ingest_on_start = os.getenv("RAG_AUTO_INGEST_ON_START", "true").lower() in {
            "1",
            "true",
            "yes",
            "y",
        }
        self.qwen_gguf_path = self._resolve_gguf_path(os.getenv('QWEN_GGUF_PATH', ''))
        self.qwen_fallback_gguf_path = self._resolve_gguf_path(os.getenv('QWEN_FALLBACK_GGUF_PATH', ''))
        self.qwen_model_id = os.getenv('QWEN_MODEL_ID', '').strip()
        self.llm_backend = (os.getenv('LLM_BACKEND', 'llama_server') or 'llama_server').strip().lower()
        self.llama_server_path = self._resolve_llama_server_path(os.getenv('LLAMA_SERVER_PATH', '').strip())
        self.llama_server_host = (os.getenv('LLAMA_SERVER_HOST', '127.0.0.1') or '127.0.0.1').strip()
        self.llama_server_port = int(os.getenv('LLAMA_SERVER_PORT', '8081'))
        self.llama_server_base_url = (
            os.getenv('LLAMA_SERVER_BASE_URL', '').strip()
            or f'http://{self.llama_server_host}:{self.llama_server_port}'
        )
        self.llama_server_autostart = os.getenv('LLAMA_SERVER_AUTOSTART', 'true').lower() in {
            '1',
            'true',
            'yes',
            'y',
        }
        self.llm_context_window = int(os.getenv('LLM_CONTEXT_WINDOW', '4096'))
        self.llm_max_new_tokens = int(os.getenv('LLM_MAX_NEW_TOKENS', '384'))
        self.llm_n_threads = int(os.getenv('LLM_N_THREADS', str(os.cpu_count() or 8)))
        self.llm_n_batch = int(os.getenv('LLM_N_BATCH', '512'))
        self.llm_n_gpu_layers = int(os.getenv('LLM_N_GPU_LAYERS', '0'))
        self.host = os.getenv("FLASK_HOST", "127.0.0.1")
        self.port = int(os.getenv("FLASK_PORT", "5000"))

        self.data_dir.mkdir(parents=True, exist_ok=True)
        self.sqlite_path.parent.mkdir(parents=True, exist_ok=True)
        self.vector_store_dir.mkdir(parents=True, exist_ok=True)

    @staticmethod
    def _running_in_container() -> bool:
        return (
            Path('/.singularity.d').exists()
            or Path('/.dockerenv').exists()
            or bool(os.getenv('SINGULARITY_CONTAINER'))
            or bool(os.getenv('DOCKER_CONTAINER'))
        )

    @staticmethod
    def _sanitize_path(raw_path: str) -> str:
        cleaned = raw_path.strip().strip('"').strip("'")
        if cleaned.endswith("'"):
            cleaned = cleaned[:-1]
        return cleaned

    def _default_cache_root(self) -> Path:
        explicit = self._sanitize_path(os.getenv('RAG_CACHE_ROOT', ''))
        if explicit:
            return Path(explicit)

        explicit_local_root = self._sanitize_path(os.getenv('HPCC_RUNTIME_LOCAL_ROOT', ''))
        if explicit_local_root:
            return Path(explicit_local_root) / '.cache_html'

        bundle_root = self._sanitize_path(os.getenv('HPCC_BUNDLE_ROOT', ''))
        if bundle_root:
            return Path(bundle_root) / 'runtime_state' / 'main_html' / '.cache_html'

        if self._running_in_container():
            for candidate in (Path('/local/hpc_tools/.cache_html'), Path('/var/tmp/hpc_tools/.cache_html'), Path('/tmp/hpc_tools/.cache_html')):
                try:
                    candidate.mkdir(parents=True, exist_ok=True)
                except OSError:
                    continue
                return candidate

        return self.workspace_root / 'simg' / '.cache_html'

    def _resolve_project_path(self, raw_path: str, default_relative_path: str) -> Path:
        cleaned = self._sanitize_path(raw_path or default_relative_path)
        candidate = Path(cleaned)
        if candidate.is_absolute() or (len(cleaned) >= 2 and cleaned[1] == ':'):
            return candidate
        return self.project_root / candidate

    def _resolve_gguf_path(self, raw_path: str) -> Path:
        cleaned = self._sanitize_path(raw_path)
        if cleaned:
            candidate = self._resolve_project_path(cleaned, cleaned)
            if candidate.exists():
                return candidate
        model_dir = self.project_root / 'model'
        if model_dir.is_dir():
            ggufs = sorted(model_dir.glob('*.gguf'))
            if ggufs:
                return ggufs[0]
        return self.project_root / 'model' / 'model.gguf'

    def _resolve_llama_server_path(self, raw_path: str) -> Path:
        candidates = []
        cleaned = self._sanitize_path(raw_path)
        if cleaned:
            candidates.append(self._resolve_project_path(cleaned, cleaned))
        candidates.extend(
            [
                self.project_root / 'tools' / 'llama.cpp' / 'llama-server',
                self.project_root / 'tools' / 'llama.cpp' / 'llama-server.exe',
            ]
        )
        for candidate in candidates:
            if candidate.exists():
                return candidate
        return candidates[0]
