import argparse
import json
import os
import shlex
import shutil
import signal
import socket
import socketserver
import subprocess
import sys
import threading
import time
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

def _invocation_dir() -> Path:
    argv0 = Path(sys.argv[0]).resolve() if sys.argv and sys.argv[0] else Path(__file__).resolve()
    if argv0.is_file():
        return argv0.parent
    return argv0 if argv0.is_dir() else argv0.parent


_SCRIPT_DIR = _invocation_dir()
_BUNDLE_SRC_DIR = _SCRIPT_DIR / 'bundle_src'
for candidate in (str(_BUNDLE_SRC_DIR), str(_SCRIPT_DIR)):
    if candidate and candidate not in sys.path:
        sys.path.insert(0, candidate)

try:
    from main_html.runtime_store import RuntimeStore
except ModuleNotFoundError:
    from hpcc_runtime_store import RuntimeStore


def _to_wsl_path(path_value: Path) -> str:
    resolved = str(path_value.resolve())
    normalized = resolved.replace('\\', '/')
    if len(normalized) >= 2 and normalized[1] == ':':
        drive = normalized[0].lower()
        suffix = normalized[2:]
        if suffix.startswith('/'):
            suffix = suffix[1:]
        return f'/mnt/{drive}/{suffix}'
    return normalized


def _runtime_binary_name() -> str:
    if shutil.which('apptainer'):
        return 'apptainer'
    if shutil.which('singularity'):
        return 'singularity'
    return 'apptainer'


_SRUN_BINARY_CACHE: Optional[str] = None
_SRUN_BINARY_PROBED = False


def _srun_binary() -> Optional[str]:
    global _SRUN_BINARY_CACHE, _SRUN_BINARY_PROBED
    if _SRUN_BINARY_PROBED:
        return _SRUN_BINARY_CACHE

    _SRUN_BINARY_PROBED = True
    direct = shutil.which('srun')
    if direct:
        _SRUN_BINARY_CACHE = direct
        return _SRUN_BINARY_CACHE

    if os.name == 'nt':
        return None

    probe_script = """
if command -v srun >/dev/null 2>&1; then
    command -v srun
    exit 0
fi
if [[ -f /etc/profile.d/modules.sh ]]; then
    source /etc/profile.d/modules.sh >/dev/null 2>&1 || true
fi
if command -v module >/dev/null 2>&1; then
    module load slurm >/dev/null 2>&1 || true
elif command -v modulecmd >/dev/null 2>&1; then
    eval "$(modulecmd bash load slurm 2>/dev/null)" || true
fi
if command -v srun >/dev/null 2>&1; then
    command -v srun
fi
"""
    try:
        result = subprocess.run(
            ['bash', '-lc', probe_script],
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True,
        )
    except Exception:
        return None

    lines = [line.strip() for line in result.stdout.splitlines() if line.strip()]
    if not lines:
        return None

    candidate = lines[-1]
    if not candidate:
        return None

    _SRUN_BINARY_CACHE = candidate
    candidate_dir = str(Path(candidate).parent)
    if candidate_dir:
        existing_path = os.environ.get('PATH') or ''
        path_entries = existing_path.split(os.pathsep) if existing_path else []
        if candidate_dir not in path_entries:
            os.environ['PATH'] = candidate_dir + (os.pathsep + existing_path if existing_path else '')
    return _SRUN_BINARY_CACHE


def _cluster_slurm_defaults() -> Dict[str, str]:
    if os.path.isdir('/mnt/usmidet'):
        defaults = {
            'partition': 'defq',
            'account': 'radarcore',
            'qos': '',
        }
    elif os.path.isdir('/net/8k3'):
        defaults = {
            'partition': 'plcyf-com',
            'account': 'RNA-SDV-SRR7',
            'qos': '',
        }
    else:
        defaults = {
            'partition': 'compute',
            'account': '',
            'qos': '',
        }

    return {
        'partition': (os.environ.get('SLURM_PARTITION') or defaults['partition']).strip(),
        'account': (os.environ.get('SLURM_ACCOUNT') or defaults['account']).strip(),
        'qos': (os.environ.get('SLURM_QOS') or defaults['qos']).strip(),
    }


def _wsl_available() -> bool:
    return os.name == 'nt' and shutil.which('wsl') is not None


def _local_ipv4_address() -> Optional[str]:
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as connection:
            connection.connect(('8.8.8.8', 80))
            candidate = connection.getsockname()[0]
    except OSError:
        return None
    if not candidate or candidate.startswith('127.'):
        return None
    return candidate


def _wsl_windows_host_ip() -> Optional[str]:
    local_ip = _local_ipv4_address()
    if local_ip:
        return local_ip
    if not _wsl_available():
        return None
    try:
        result = subprocess.run(
            ['wsl', 'bash', '-lc', "awk '/nameserver/ {print $2; exit}' /etc/resolv.conf"],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True,
        )
    except Exception:
        return None
    candidate = result.stdout.strip()
    if not candidate:
        return None
    try:
        socket.inet_aton(candidate)
    except OSError:
        return None
    return candidate


def _wsl_guest_ip() -> Optional[str]:
    if not _wsl_available():
        return None
    try:
        result = subprocess.run(
            ['wsl', 'bash', '-lc', "hostname -I | awk '{print $1}'"],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True,
        )
    except Exception:
        return None
    candidate = result.stdout.strip().split()[0] if result.stdout.strip() else ''
    if not candidate:
        return None
    try:
        socket.inet_aton(candidate)
    except OSError:
        return None
    return candidate


def _from_wsl_path(path_value: str) -> Path:
    normalized = path_value.replace('\\', '/').strip()
    if normalized.startswith('/mnt/') and len(normalized) > 6:
        drive = normalized[5].upper()
        remainder = normalized[7:]
        return Path(f'{drive}:/{remainder}')
    return Path(normalized)


def _normalize_container_path(path_value: str) -> str:
    if not path_value:
        return path_value

    normalized = os.path.expandvars(os.path.expanduser(path_value.strip())).replace('\\', '/')
    if normalized.startswith('/'):
        return normalized
    if _wsl_available() and len(normalized) >= 2 and normalized[1] == ':':
        drive = normalized[0].lower()
        remainder = normalized[2:]
        if remainder.startswith('/'):
            remainder = remainder[1:]
        return f'/mnt/{drive}/{remainder}'
    return normalized


def _sanitize_runtime_path(path_value: str) -> str:
    normalized = _normalize_container_path(path_value)
    if not normalized:
        return normalized

    for marker in ('/net/', '/mnt/'):
        first = normalized.find(marker)
        last = normalized.rfind(marker)
        if first != -1 and last != -1 and last > first:
            return normalized[last:]
    return normalized


def _is_truthy(value: Any) -> bool:
    return str(value).strip().lower() in {'1', 'true', 'yes', 'y', 'on'}


def _strict_slurm_required(tool_key: str) -> bool:
    if tool_key not in {'can_kpi', 'udp_kpi', 'interactive_plot'}:
        return False

    explicit = (os.environ.get('HPCC_REQUIRE_SLURM_FOR_KPI') or '').strip()
    if explicit:
        return _is_truthy(explicit)

    auth_mode = (os.environ.get('HPC_TOOLS_AUTH_MODE') or '').strip().lower()
    return os.name != 'nt' and auth_mode == 'cluster'


def _slurm_immediate_seconds(tool_key: str, resources: Dict[str, Any]) -> str:
    explicit = str(resources.get('immediate') or '').strip()
    if explicit:
        return explicit
    return ''


def _project_root(script_anchor: Optional[Path] = None) -> Path:
    override = (os.environ.get('HPCC_PROJECT_ROOT') or '').strip()
    if override:
        return Path(override).resolve()

    anchor = (script_anchor or Path(__file__).resolve()).parent.resolve()
    if (anchor / 'main_html').is_dir() or (anchor / 'KPI').is_dir() or (anchor / 'rag').is_dir():
        return anchor

    bundle_source = anchor / 'bundle_src'
    if bundle_source.is_dir():
        return bundle_source.resolve()

    parent = anchor.parent
    if (parent / 'main_html').is_dir() or (parent / 'KPI').is_dir() or (parent / 'rag').is_dir():
        return parent.resolve()
    return anchor


def _runtime_root(project_root: Path, script_anchor: Optional[Path] = None) -> Path:
    override = (os.environ.get('HPCC_BUNDLE_ROOT') or '').strip()
    if override:
        return Path(override).resolve()

    anchor = (script_anchor or Path(__file__).resolve()).parent.resolve()
    if (anchor / 'main_html.simg').exists() or (anchor / 'main_hpcc.sh').exists():
        return anchor

    runtime_dir = project_root / 'generate_upload'
    if runtime_dir.is_dir():
        return runtime_dir.resolve()
    return project_root.resolve()

def _preferred_local_runtime_root() -> Path:
    candidates = []
    explicit = (os.environ.get('HPCC_RUNTIME_LOCAL_ROOT') or '').strip()
    if explicit:
        candidates.append(Path(explicit))
    candidates.extend([Path('/local/hpc_tools'), Path('/var/tmp/hpc_tools'), Path('/tmp/hpc_tools')])

    for candidate in candidates:
        try:
            candidate.mkdir(parents=True, exist_ok=True)
        except OSError:
            continue
        return candidate.resolve()

    fallback = Path('/tmp/hpc_tools')
    fallback.mkdir(parents=True, exist_ok=True)
    return fallback.resolve()


def _runtime_work_root(project_root: Path) -> Path:
    override = (os.environ.get('HPCC_RUNTIME_WORK_ROOT') or '').strip()
    if override:
        return Path(override).resolve()

    if os.name != 'nt':
        return (_preferred_local_runtime_root() / 'hpcc_runtime').resolve()

    return (_runtime_root(project_root) / 'runs').resolve()


def _output_runtime_log_dir(output_path: str, job_dir_name: str) -> Optional[Path]:
    candidate = _normalize_container_path(str(output_path or '').strip())
    if not candidate:
        return None
    return Path(candidate) / '.hpcc_runtime' / job_dir_name


def _default_interactive_config_path(project_root: Path, source_target: str) -> Path:
    runtime_root = _runtime_root(project_root)
    file_name = 'ConfigInteractivePlots_bordnet.xml' if source_target == 'can_kpi' else 'ConfigInteractivePlots.xml'
    candidates = [
        project_root / 'KPI' / 'intplot_kpi' / file_name,
        runtime_root / file_name,
        runtime_root / 'bundle_src' / 'KPI' / 'intplot_kpi' / file_name,
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate.resolve()
    return candidates[0]


def _shell_join(arguments: List[str]) -> str:
    return ' '.join(shlex.quote(argument) for argument in arguments)


def _write_askpass_script(password: str, path: Path) -> None:
    """Write a chmod 0o500 script that echoes the password for SSH_ASKPASS."""
    path.write_text(f'#!/bin/sh\nprintf \'%s\' {shlex.quote(password)}\n', encoding='utf-8')
    path.chmod(0o500)

def _tool_image_definition(workspace_root: Path, tool_key: str) -> Optional[Tuple[str, Path]]:
    definitions = {
        'main_html': ('main_html.simg', workspace_root / 'Singularity.def'),
        'can_kpi': ('kpi/can/can_kpi.simg', workspace_root / 'KPI' / 'can_kpi' / 'can_singularity_KPI.def'),
        'udp_kpi': ('kpi/udp/udp_kpi.simg', workspace_root / 'KPI' / 'UDP_KPI' / 'Singularity_KPI.def'),
        'interactive_plot': ('kpi/int_plot/intplot_kpi.simg', workspace_root / 'KPI' / 'intplot_kpi' / 'singularity_interactiveplot.def'),
        'rag': ('rag/rag.simg', workspace_root / 'rag' / 'Singularity_RAG.def'),
    }
    return definitions.get(tool_key)


def _rag_host_command(workspace_root: Path) -> List[str]:
    return [sys.executable, str(workspace_root / 'rag' / 'main.py'), '--talk']


def _main_html_host_command(workspace_root: Path) -> List[str]:
    return [sys.executable, str(workspace_root / 'main_html' / 'app.py')]


def _should_run_main_html_on_host(workspace_root: Path) -> bool:
    mode_override = (os.environ.get('HPCC_MAIN_HTML_RUN_MODE') or '').strip().lower()
    if mode_override == 'container':
        return False
    if mode_override == 'host':
        return True
    if os.name != 'nt':
        return False
    return (workspace_root / 'main_html' / 'app.py').exists()


def _should_run_rag_on_host(workspace_root: Path) -> bool:
    mode_override = (os.environ.get('HPCC_RAG_RUN_MODE') or '').strip().lower()
    if mode_override == 'container':
        return False
    if mode_override == 'host':
        return True
    if os.name != 'nt':
        return False

    model_path = workspace_root / 'rag' / 'model' / 'Qwen_Qwen3.5-2B-Q5_K_S.gguf'
    vulkan_server = workspace_root / 'rag' / 'tools' / 'llama.cpp-vulkan' / 'llama-server.exe'
    cpu_server = workspace_root / 'rag' / 'tools' / 'llama.cpp' / 'llama-server.exe'
    return model_path.exists() and (vulkan_server.exists() or cpu_server.exists())


def _rag_service_url(workspace_root: Path, for_wsl_client: bool = False) -> str:
    explicit = (os.environ.get('RAG_SERVICE_URL') or '').strip()
    if explicit:
        return explicit

    port = str(os.environ.get('FLASK_PORT', '5100'))
    if for_wsl_client and _should_run_rag_on_host(workspace_root):
        return f"http://{_wsl_windows_host_ip() or '127.0.0.1'}:{port}"

    advertised_host = (os.environ.get('HPCC_BROKER_ADVERTISE_HOST') or '').strip()
    if advertised_host and advertised_host != '127.0.0.1':
        return f'http://{advertised_host}:{port}'
    return f'http://127.0.0.1:{port}'


def _resolve_image_path(workspace_root: Path, tool_key: str, configured_image_path: str) -> Optional[Path]:
    configured = (configured_image_path or '').strip()
    candidate: Optional[Path] = None
    if configured:
        if os.name == 'nt' and configured.startswith('/mnt/'):
            candidate = _from_wsl_path(configured)
        else:
            raw_candidate = Path(configured)
            candidate = raw_candidate if raw_candidate.is_absolute() else workspace_root / raw_candidate
        if candidate.exists():
            return candidate.resolve()

    image_definition = _tool_image_definition(workspace_root, tool_key)
    if not image_definition:
        return None

    image_name, definition_path = image_definition
    return ensure_image(workspace_root, image_name, definition_path)


def _runtime_bundle_cache_root(workspace_root: Path) -> Path:
    cache_root = _runtime_work_root(workspace_root) / '.bundle_cache'
    cache_root.mkdir(parents=True, exist_ok=True)
    if os.name != 'nt':
        try:
            cache_root.chmod(0o755)
        except OSError:
            pass
    return cache_root


def _stage_bundle_file_for_user_job(workspace_root: Path, source_path: Path) -> Path:
    runtime_root = _runtime_root(workspace_root)
    try:
        relative_path = source_path.resolve().relative_to(runtime_root.resolve())
    except (OSError, ValueError):
        return source_path

    try:
        source_stat = source_path.stat()
    except OSError:
        return source_path

    cache_root = _runtime_bundle_cache_root(workspace_root)
    target_path = cache_root / relative_path
    target_path.parent.mkdir(parents=True, exist_ok=True)

    try:
        target_stat = target_path.stat()
        if target_stat.st_size == source_stat.st_size and int(target_stat.st_mtime) >= int(source_stat.st_mtime):
            return target_path
    except OSError:
        pass

    temp_path = target_path.with_name(f'{target_path.name}.tmp.{os.getpid()}')
    shutil.copy2(str(source_path), str(temp_path))
    if os.name != 'nt':
        try:
            temp_path.chmod(0o755 if (source_stat.st_mode & 0o111) else 0o644)
        except OSError:
            pass
    os.replace(str(temp_path), str(target_path))
    return target_path


def _stage_bundle_path_for_user_job(workspace_root: Path, path_value: str) -> str:
    normalized = _normalize_container_path(str(path_value or '').strip())
    if not normalized:
        return normalized

    candidate = Path(normalized)
    try:
        if not candidate.is_file():
            return normalized
    except OSError:
        return normalized

    return str(_stage_bundle_file_for_user_job(workspace_root, candidate))


def _stage_kpi_bundle_support(workspace_root: Path, target: str, interactive_mode: str = 'disabled') -> Path:
    runtime_root = _runtime_root(workspace_root)
    required_files: List[Path] = [
        runtime_root / 'bundle_common.sh',
        runtime_root / 'kpi_runtime_launcher.sh',
    ]

    if target == 'udp_kpi':
        required_files.extend([
            runtime_root / 'kpi' / 'udp' / 'run_udp.sh',
            runtime_root / 'kpi' / 'udp' / 'udp_kpi.simg',
        ])
        if interactive_mode == 'enabled':
            required_files.append(runtime_root / 'kpi' / 'inplot_udp.sh')
    elif target == 'can_kpi':
        required_files.extend([
            runtime_root / 'kpi' / 'can' / 'run_can.sh',
            runtime_root / 'kpi' / 'can' / 'can_kpi.simg',
        ])
        if interactive_mode == 'enabled':
            required_files.append(runtime_root / 'kpi' / 'inplot_can.sh')

    if interactive_mode != 'disabled' or target == 'interactive_plot':
        required_files.extend([
            runtime_root / 'kpi' / 'int_plot' / 'run_intplot.sh',
            runtime_root / 'kpi' / 'int_plot' / 'intplot_kpi.simg',
            runtime_root / 'ConfigInteractivePlots.xml',
            runtime_root / 'ConfigInteractivePlots_bordnet.xml',
        ])

    for path in required_files:
        if path.exists():
            _stage_bundle_file_for_user_job(workspace_root, path)

    return _runtime_bundle_cache_root(workspace_root)


def _container_run_command(
    image_path: Path,
    arguments: List[str],
    env_overrides: Optional[Dict[str, str]] = None,
    bind_paths: Optional[List[str]] = None,
) -> List[str]:
    env_overrides = {key: str(value) for key, value in (env_overrides or {}).items() if value not in (None, '')}
    bind_paths = [str(path) for path in (bind_paths or []) if str(path).strip()]
    if _wsl_available():
        command_parts: List[str] = []
        if env_overrides:
            command_parts.append('env')
            for key, value in env_overrides.items():
                command_parts.append(f'APPTAINERENV_{key}={shlex.quote(value)}')
        command_parts.extend([
            'apptainer',
            'run',
            '--bind',
            '/mnt/c:/mnt/c',
        ])
        for bind_path in bind_paths:
            command_parts.extend(['--bind', shlex.quote(bind_path)])
        command_parts.append(shlex.quote(_to_wsl_path(image_path)))
        command_parts.extend(shlex.quote(argument) for argument in arguments)
        return ['wsl', 'bash', '-lc', ' '.join(command_parts)]

    command = [_runtime_binary_name(), 'run']
    if Path('/mnt/c').exists():
        command.extend(['--bind', '/mnt/c:/mnt/c'])
    for bind_path in bind_paths:
        command.extend(['--bind', bind_path])
    for key, value in env_overrides.items():
        command.extend(['--env', f'{key}={value}'])
    command.append(str(image_path))
    command.extend(arguments)
    return command


def _script_command(script_path: Path, arguments: List[str]) -> List[str]:
    if _wsl_available():
        command_parts = ['bash', _to_wsl_path(script_path)]
        command_parts.extend(arguments)
        return ['wsl', 'bash', '-lc', ' '.join(shlex.quote(argument) for argument in command_parts)]

    command = ['bash', str(script_path)]
    command.extend(arguments)
    return command


def _service_environment(workspace_root: Path, tool_key: str) -> Dict[str, str]:
    runtime_root = _runtime_root(workspace_root)
    runtime_state_root = runtime_root / 'runtime_state'
    normalized_project_root = _normalize_container_path(str(workspace_root))
    normalized_runtime_root = _normalize_container_path(str(runtime_root))
    if tool_key == 'main_html':
        cache_dir = runtime_state_root / 'main_html' / 'cache_html'
        vlm_cache_dir = cache_dir / 'vlm_cache'
        local_model_dir = Path(
            os.environ.get('HPCC_HYPERLINK_VLM_MODEL_DIR', '').strip()
            or workspace_root / 'rag' / 'qwn_kk_fine_model'
        )
        cache_dir.mkdir(parents=True, exist_ok=True)
        vlm_cache_dir.mkdir(parents=True, exist_ok=True)
        runtime_db = cache_dir / 'hpc_tools_dev.db'
        normalized_cache_dir = _normalize_container_path(str(cache_dir))
        normalized_vlm_cache_dir = _normalize_container_path(str(vlm_cache_dir))
        normalized_runtime_db = _normalize_container_path(str(runtime_db))
        normalized_local_model_dir = _normalize_container_path(str(local_model_dir))
        return {
            'HPCC_PROJECT_ROOT': normalized_project_root,
            'HPCC_BUNDLE_ROOT': normalized_runtime_root,
            'HPCC_BROKER_HOST': os.environ.get('HPCC_BROKER_HOST', '127.0.0.1'),
            'HPCC_BROKER_PORT': os.environ.get('HPCC_BROKER_PORT', '9100'),
            'RAG_SERVICE_URL': os.environ.get('RAG_SERVICE_URL', _rag_service_url(workspace_root)),
            'HOST': os.environ.get('HOST', '0.0.0.0'),
            'PORT': os.environ.get('PORT', '5002'),
            'CACHE_HTML_DIR': normalized_cache_dir,
            'HYPERLINK_VLM_CACHE_DIR': normalized_vlm_cache_dir,
            'HYPERLINK_VLM_MODEL_DIR': normalized_local_model_dir,
            'HPCC_HYPERLINK_VLM_MODEL_DIR': normalized_local_model_dir,
            'HF_HUB_OFFLINE': os.environ.get('HF_HUB_OFFLINE', '1'),
            'TRANSFORMERS_OFFLINE': os.environ.get('TRANSFORMERS_OFFLINE', '1'),
            'HF_HUB_DISABLE_XET': os.environ.get('HF_HUB_DISABLE_XET', '1'),
            'HPCC_RUNTIME_DB': normalized_runtime_db,
            'DATABASE_URL': f'sqlite:///{normalized_runtime_db}',
        }
    if tool_key == 'rag':
        rag_state_dir = runtime_state_root / 'rag'
        vector_store_dir = rag_state_dir / 'vector_store'
        default_model_dir = workspace_root / 'rag' / 'qwn_kk_fine_model'
        default_gguf = default_model_dir / 'Qwen_Qwen3.5-2B-Q5_K_S.gguf'
        rag_state_dir.mkdir(parents=True, exist_ok=True)
        vector_store_dir.mkdir(parents=True, exist_ok=True)
        html_root = os.environ.get('HPCC_RAG_HTML_ROOT', '').strip() or _normalize_container_path(str(workspace_root))
        return {
            'HPCC_PROJECT_ROOT': normalized_project_root,
            'HPCC_BUNDLE_ROOT': normalized_runtime_root,
            'FLASK_HOST': os.environ.get('FLASK_HOST', '0.0.0.0'),
            'FLASK_PORT': os.environ.get('FLASK_PORT', '5100'),
            'HTML_ROOT_PATH': html_root,
            'ALLOW_ALL_HTML_FALLBACK': os.environ.get('ALLOW_ALL_HTML_FALLBACK', 'true'),
            'RAG_AUTO_INGEST_ON_START': os.environ.get('RAG_AUTO_INGEST_ON_START', 'true'),
            'RAG_DATA_DIR': _normalize_container_path(str(rag_state_dir)),
            'RAG_SQLITE_PATH': _normalize_container_path(str(rag_state_dir / 'rag_logs.db')),
            'RAG_VECTOR_STORE_DIR': _normalize_container_path(str(vector_store_dir)),
            'VECTOR_STORE_JSON_PATH': _normalize_container_path(str(vector_store_dir / 'vector_store.json')),
            'QWEN_GGUF_PATH': _normalize_container_path(str(default_gguf)),
            'QWEN_FALLBACK_GGUF_PATH': _normalize_container_path(str(default_gguf)),
            'LLM_N_GPU_LAYERS': os.environ.get('LLM_N_GPU_LAYERS', '35'),
            'HF_HUB_OFFLINE': os.environ.get('HF_HUB_OFFLINE', '1'),
            'TRANSFORMERS_OFFLINE': os.environ.get('TRANSFORMERS_OFFLINE', '1'),
        }
    if tool_key in {'can_kpi', 'udp_kpi', 'interactive_plot'}:
        rag_url = os.environ.get('RAG_SERVICE_URL', _rag_service_url(workspace_root))
        base = rag_url.rstrip('/')
        if base.startswith('http://'):
            host_part = base.split('://', 1)[1].rsplit(':', 1)[0]
            llama_url = f'http://{host_part}:8081'
        else:
            llama_url = os.environ.get('LLAMA_SERVER_BASE_URL', os.environ.get('RAG_LLAMA_URL', 'http://127.0.0.1:8081'))
        return {
            'RAG_SERVICE_URL': rag_url,
            'LLAMA_SERVER_BASE_URL': llama_url,
        }
    return {}


def ensure_image(workspace_root: Path, image_name: str, definition_path: Path) -> Optional[Path]:
    image_path = _runtime_root(workspace_root) / image_name
    if image_path.exists():
        return image_path

    print(f'[{image_name}] image not found. Attempting build from {definition_path.name}...')
    try:
        image_path.parent.mkdir(parents=True, exist_ok=True)
        if os.name == 'nt' and shutil.which('wsl'):
            subprocess.run(
                [
                    'wsl',
                    'bash',
                    '-lc',
                    (
                        f"cd {shlex.quote(_to_wsl_path(workspace_root))} && "
                        f"apptainer build --fakeroot {shlex.quote(_to_wsl_path(image_path))} {shlex.quote(_to_wsl_path(definition_path))}"
                    ),
                ],
                check=True,
            )
        elif shutil.which('apptainer') or shutil.which('singularity'):
            subprocess.run(
                [_runtime_binary_name(), 'build', '--fakeroot', str(image_path), str(definition_path)],
                check=True,
                cwd=str(workspace_root),
            )
        else:
            return None
    except Exception as exc:
        print(f'[{image_name}] build failed: {exc}')
        return None

    return image_path if image_path.exists() else None


def default_service_command(workspace_root: Path, tool_key: str) -> Optional[List[str]]:
    if tool_key == 'main_html':
        if _should_run_main_html_on_host(workspace_root):
            return _main_html_host_command(workspace_root)
        image_path = _resolve_image_path(workspace_root, tool_key, '')
        if image_path:
            return _container_run_command(image_path, [], _service_environment(workspace_root, 'main_html'))
        return _main_html_host_command(workspace_root)

    if tool_key == 'rag':
        if _should_run_rag_on_host(workspace_root):
            return _rag_host_command(workspace_root)
        image_path = _resolve_image_path(workspace_root, tool_key, '')
        if image_path:
            return _container_run_command(image_path, ['--talk'], _service_environment(workspace_root, 'rag'))
        return _rag_host_command(workspace_root)
    return None


def _start_wsl_broker(workspace_root: Path, port: int) -> subprocess.Popen:
    runtime_root = _runtime_root(workspace_root)
    broker_path = runtime_root / 'hpcc_main.py'
    if not broker_path.exists():
        broker_path = workspace_root / 'hpcc_main.py'
    command = (
        f"cd {shlex.quote(_to_wsl_path(runtime_root))} && "
        f"HPCC_BUNDLE_ROOT={shlex.quote(_to_wsl_path(runtime_root))} "
        f"HPCC_PROJECT_ROOT={shlex.quote(_to_wsl_path(workspace_root))} "
        f"python3 {shlex.quote(_to_wsl_path(broker_path))} --broker-only --host 0.0.0.0 --port {port}"
    )
    return subprocess.Popen(
        ['wsl', 'bash', '-lc', command],
        cwd=str(runtime_root),
    )


class PortForwardHandler(socketserver.BaseRequestHandler):
    def handle(self) -> None:
        try:
            upstream = socket.create_connection(self.server.target_address, timeout=5)
        except OSError:
            return

        stop_event = threading.Event()

        def _pipe(source: socket.socket, destination: socket.socket) -> None:
            try:
                while not stop_event.is_set():
                    chunk = source.recv(64 * 1024)
                    if not chunk:
                        break
                    destination.sendall(chunk)
            except OSError:
                pass
            finally:
                stop_event.set()
                try:
                    destination.shutdown(socket.SHUT_WR)
                except OSError:
                    pass

        forward = threading.Thread(target=_pipe, args=(self.request, upstream), daemon=True)
        backward = threading.Thread(target=_pipe, args=(upstream, self.request), daemon=True)
        forward.start()
        backward.start()
        forward.join()
        backward.join()
        try:
            upstream.close()
        except OSError:
            pass


class PortForwardServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True
    daemon_threads = True

    def __init__(self, address: Tuple[str, int], target_address: Tuple[str, int]):
        super().__init__(address, PortForwardHandler)
        self.target_address = target_address


def _start_windows_forwarders(remote_host: str, ports: List[int], bind_host: str = '127.0.0.1') -> List[PortForwardServer]:
    forwarders: List[PortForwardServer] = []
    started_threads: List[threading.Thread] = []
    try:
        for port in sorted(set(ports)):
            server = PortForwardServer((bind_host, port), (remote_host, port))
            thread = threading.Thread(target=server.serve_forever, daemon=True)
            thread.start()
            forwarders.append(server)
            started_threads.append(thread)
    except Exception:
        for server in forwarders:
            server.shutdown()
            server.server_close()
        raise
    return forwarders


def _stop_windows_forwarders(forwarders: List[PortForwardServer]) -> None:
    for server in forwarders:
        try:
            server.shutdown()
            server.server_close()
        except OSError:
            pass


class RuntimeBroker:
    def __init__(self, workspace_root: Path, store: RuntimeStore):
        self.workspace_root = workspace_root
        self.store = store
        self.processes: Dict[int, subprocess.Popen] = {}
        self.lock = threading.Lock()

    def handle(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        action = (payload.get('action') or '').strip().lower()
        if action == 'ping':
            return {
                'ok': True,
                'status': 'alive',
                'timestamp': self._utcnow(),
                'tools': self.store.list_tools(),
            }
        if action == 'status':
            runtime_job_id = int(payload['runtime_job_id'])
            job = self.store.get_job(runtime_job_id)
            return {'ok': True, 'job': job}
        if action == 'cancel':
            return self._cancel(int(payload['runtime_job_id']))
        if action == 'submit':
            return self._submit(payload)
        return {'ok': False, 'error': f'Unsupported action: {action}'}

    def _submit(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        tool_key = (payload.get('tool_key') or '').strip()
        if not tool_key:
            return {'ok': False, 'error': 'tool_key is required'}

        tool = self.store.get_tool(tool_key)
        if not tool:
            return {'ok': False, 'error': f'Unknown tool: {tool_key}'}

        spec = self._build_spec(tool, payload)
        request_payload = dict(payload)
        if spec.get('console'):
            request_payload['_console'] = spec['console']
        runtime_job_id = self.store.create_job(
            tool_key=tool_key,
            requested_by=payload.get('user', 'unknown'),
            session_id=payload.get('session_id', ''),
            mode=spec['mode'],
            input_path=spec['input_path'],
            output_path=spec['output_path'],
            log_path=spec['log_path'],
            command=spec['command'],
            resources=spec['resources'],
            request_payload=request_payload,
            status='QUEUED',
        )
        self.store.append_event(runtime_job_id, 'info', f'Prepared command for {tool_key}')
        if spec.get('status_detail'):
            self.store.append_event(runtime_job_id, 'warning', spec['status_detail'])
        self._launch(runtime_job_id, spec)
        return {
            'ok': True,
            'job_id': runtime_job_id,
            'status': 'QUEUED',
            'log_path': spec['log_path'],
            'output_path': spec['output_path'],
            'command': spec['command'],
            'console': spec.get('console', {}),
            'status_detail': spec.get('status_detail', ''),
        }

    def _launch(self, runtime_job_id: int, spec: Dict[str, Any]) -> None:
        launcher_log_path = spec.get('launcher_log_path') or spec['log_path']
        share_console_with_user = bool(spec.get('share_console_with_submitting_user'))
        os.makedirs(Path(launcher_log_path).parent, exist_ok=True)
        log_fp = open(launcher_log_path, 'a', encoding='utf-8', errors='replace')
        if share_console_with_user:
            try:
                os.chmod(launcher_log_path, 0o666)
            except OSError:
                pass
        header_lines = [
            '[broker] COMMAND: ' + ' '.join(spec['command']),
            '[broker] START: ' + self._utcnow(),
            '[broker] CWD: ' + spec['cwd'],
            '',
        ]
        log_fp.write('\n'.join(header_lines) + '\n')
        log_fp.flush()
        if spec['log_path'] != launcher_log_path:
            os.makedirs(Path(spec['log_path']).parent, exist_ok=True)
            with open(spec['log_path'], 'a', encoding='utf-8', errors='replace') as console_fp:
                if share_console_with_user:
                    try:
                        os.chmod(spec['log_path'], 0o666)
                    except OSError:
                        pass
                console_fp.write('\n'.join(header_lines) + '\n')
                console_fp.flush()

        creationflags = 0
        popen_kwargs: Dict[str, Any] = {'start_new_session': True}
        if os.name == 'nt':
            creationflags = subprocess.CREATE_NEW_PROCESS_GROUP
            popen_kwargs.pop('start_new_session', None)

        process = subprocess.Popen(
            spec['command'],
            cwd=spec['cwd'],
            env=spec['env'],
            stdout=log_fp,
            stderr=subprocess.STDOUT,
            creationflags=creationflags,
            **popen_kwargs,
        )
        with self.lock:
            self.processes[runtime_job_id] = process
        initial_status = 'PENDING' if spec.get('console', {}).get('mode') == 'tmux' else 'RUNNING'
        update_fields = {
            'status': initial_status,
            'pid': process.pid,
        }
        if initial_status == 'RUNNING':
            update_fields['started_at'] = self._utcnow()
        self.store.update_job(runtime_job_id, **update_fields)
        if initial_status == 'PENDING':
            self.store.append_event(runtime_job_id, 'info', f'Requested Slurm allocation via launcher PID {process.pid}')
        else:
            self.store.append_event(runtime_job_id, 'info', f'Launched PID {process.pid}')

        watcher = threading.Thread(
            target=self._watch_process,
            args=(runtime_job_id, process, log_fp, spec),
            daemon=True,
        )
        watcher.start()

        if spec.get('service_tool_key') and spec.get('service_host_file') and spec.get('service_port'):
            host_watcher = threading.Thread(
                target=self._watch_service_host,
                args=(
                    runtime_job_id,
                    spec['service_tool_key'],
                    spec['service_host_file'],
                    int(spec['service_port']),
                ),
                daemon=True,
            )
            host_watcher.start()

    def _watch_process(self, runtime_job_id: int, process: subprocess.Popen, log_fp, spec: Dict[str, Any]) -> None:
        return_code = process.wait()
        footer_lines = [
            '',
            '[broker] END: ' + self._utcnow(),
            '[broker] RETURN_CODE: ' + str(return_code),
        ]
        log_fp.write('\n'.join(footer_lines) + '\n')
        log_fp.flush()
        log_fp.close()
        launcher_log_path = spec.get('launcher_log_path') or spec['log_path']
        if spec['log_path'] != launcher_log_path:
            with open(spec['log_path'], 'a', encoding='utf-8', errors='replace') as console_fp:
                console_fp.write('\n'.join(footer_lines) + '\n')
                console_fp.flush()
        status = 'COMPLETED' if return_code == 0 else 'FAILED'
        self.store.update_job(
            runtime_job_id,
            status=status,
            return_code=return_code,
            completed_at=self._utcnow(),
        )
        self.store.append_event(runtime_job_id, 'info', f'Process finished with code {return_code}')
        with self.lock:
            self.processes.pop(runtime_job_id, None)

        if spec.get('service_tool_key'):
            self._update_service_url(spec['service_tool_key'], '')

    def _watch_service_host(self, runtime_job_id: int, tool_key: str, host_file: str, port: int) -> None:
        deadline = time.time() + 180
        while time.time() < deadline:
            job = self.store.get_job(runtime_job_id)
            if not job or job.get('status') in {'FAILED', 'CANCELLED', 'COMPLETED'}:
                return

            if os.path.exists(host_file):
                try:
                    service_host = Path(host_file).read_text(encoding='utf-8').strip()
                except OSError:
                    service_host = ''

                if service_host:
                    service_url = f'http://{service_host}:{port}'
                    self._update_service_url(tool_key, service_url)
                    self.store.append_event(runtime_job_id, 'info', f'{tool_key} service ready at {service_url}')
                    return
            time.sleep(2)

    def _update_service_url(self, tool_key: str, service_url: str) -> None:
        tool = self.store.get_tool(tool_key)
        if not tool:
            return

        payload = dict(tool)
        payload['metadata'] = tool.get('metadata', {}) or {}
        payload['service_url'] = service_url
        self.store.save_tool(payload, updated_by='system')

    def _cancel(self, runtime_job_id: int) -> Dict[str, Any]:
        with self.lock:
            process = self.processes.get(runtime_job_id)
        if not process:
            job = self.store.get_job(runtime_job_id)
            return {'ok': True, 'job': job, 'message': 'Job is not running'}

        if os.name == 'nt':
            process.terminate()
        else:
            os.killpg(process.pid, signal.SIGTERM)

        self.store.update_job(
            runtime_job_id,
            status='CANCELLED',
            completed_at=self._utcnow(),
            error_message='Cancelled by user',
        )
        self.store.append_event(runtime_job_id, 'warning', 'Cancelled by user request')
        return {'ok': True, 'job': self.store.get_job(runtime_job_id)}

    def _build_spec(self, tool: Dict[str, Any], payload: Dict[str, Any]) -> Dict[str, Any]:
        tool_key = tool['tool_key']
        paths = payload.get('paths', {}) or {}
        resources = payload.get('resources', {}) or {}
        requested_by = payload.get('user', 'unknown')
        user_password = (payload.get('user_password') or '').strip()
        timestamp = datetime.now(timezone.utc).strftime('%Y%m%d_%H%M%S')
        job_dir_name = f'{tool_key}_{timestamp}'
        run_dir = _runtime_work_root(self.workspace_root) / requested_by / job_dir_name
        run_dir.mkdir(parents=True, exist_ok=True)
        run_dir.chmod(0o777)  # allow the submitting user's srun job to write exit/log files back
        output_path, output_warning = self._resolve_output_path(paths.get('output_dir'), run_dir, requested_by)
        use_tmux_console = str(resources.get('scheduler', '')).lower() == 'slurm' and _srun_binary() is not None and os.name != 'nt'
        output_log_dir = _output_runtime_log_dir(output_path, job_dir_name)
        log_path = str(run_dir / ('runtime_console.log' if use_tmux_console else 'execution.log'))
        launcher_log_path = str(run_dir / 'launcher.log') if use_tmux_console else log_path
        launch_plan = self._tool_command(tool_key, tool, paths, output_path)
        launch_plan['output_path'] = output_path
        launch_plan['runtime_control_dir'] = str(run_dir)
        if output_log_dir is not None:
            launch_plan['pane_log_dir'] = str(output_log_dir)
            if use_tmux_console:
                launch_plan['project_console_log_path'] = str(output_log_dir / 'runtime_console.log')
        if user_password and os.name != 'nt':
            askpass_path = run_dir / '.ssh_askpass.sh'
            _write_askpass_script(user_password, askpass_path)
            launch_plan['ssh_run_as_user'] = requested_by
            launch_plan['ssh_askpass_path'] = str(askpass_path)
        pane_names = ['udp', 'interactive'] if launch_plan.get('kind') == 'udp_interactive_dual' else ['main']
        console = {
            'display_name': f"{tool.get('display_name', tool_key)} console",
            'mode': 'tmux' if use_tmux_console else 'log',
            'tmux_session_name': self._tmux_session_name(tool_key, requested_by, run_dir) if use_tmux_console else '',
            'pane_names': pane_names,
            'input_queue_path': str(run_dir / 'tmux_input.queue') if use_tmux_console else '',
            'log_path': log_path,
            'run_dir': str(output_log_dir or run_dir),
            'control_dir': str(run_dir),
            'supports_input': use_tmux_console,
        }
        if use_tmux_console:
            launch_plan['console_log_path'] = log_path
            launch_plan['input_queue_path'] = console['input_queue_path']
            launch_plan['pane_names'] = pane_names
        if str(resources.get('scheduler', '')).lower() == 'slurm' and launch_plan.get('service_tool_key') and launch_plan.get('service_port'):
            launch_plan['service_host_file'] = str(run_dir / 'service_host.txt')
        command = self._maybe_prefix_scheduler(tool_key, launch_plan, resources, run_dir, requested_by)

        spec_env = os.environ.copy()
        if launch_plan.get('ssh_askpass_path'):
            spec_env['SSH_ASKPASS'] = launch_plan['ssh_askpass_path']
            spec_env['SSH_ASKPASS_REQUIRE'] = 'force'
            spec_env.setdefault('DISPLAY', 'x')

        return {
            'command': command,
            'cwd': str(self.workspace_root),
            'env': spec_env,
            'mode': launch_plan['mode'],
            'input_path': launch_plan['input_path'],
            'output_path': output_path,
            'log_path': log_path,
            'launcher_log_path': launcher_log_path,
            'resources': resources,
            'service_tool_key': launch_plan.get('service_tool_key', ''),
            'service_port': launch_plan.get('service_port'),
            'service_host_file': launch_plan.get('service_host_file', ''),
            'console': console,
            'share_console_with_submitting_user': bool(launch_plan.get('ssh_run_as_user')),
            'status_detail': output_warning,
        }

    def _resolve_output_path(self, requested_output_dir: Any, run_dir: Path, requested_by: str) -> Tuple[str, str]:
        fallback_path = run_dir / 'output'
        fallback_path.mkdir(parents=True, exist_ok=True)

        candidate = str(requested_output_dir or '').strip()
        if not candidate:
            return str(fallback_path), ''

        # Accept the explicit path without a broker-side write test.
        # The broker service account may not have access to other users' project
        # spaces (e.g. GPO-IFV7XX group-restricted directories). The Slurm
        # launcher script will attempt `mkdir -p` at job run time; if that also
        # fails, the job exits immediately with a clear filesystem error.
        return candidate, ''

    def _tool_command(
        self,
        tool_key: str,
        tool: Dict[str, Any],
        paths: Dict[str, Any],
        output_path: str,
    ) -> Dict[str, Any]:
        image_path = _resolve_image_path(self.workspace_root, tool_key, tool.get('image_path', ''))
        if image_path is not None:
            image_path = _stage_bundle_file_for_user_job(self.workspace_root, image_path)

        input_mode = (paths.get('input_mode') or 'json').strip().lower()
        runtime_output_path = _normalize_container_path(output_path)
        interactive_plot_mode = (paths.get('interactive_plot_mode') or 'disabled').strip().lower()
        interactive_source_target = (paths.get('interactive_source_target') or tool_key).strip() or tool_key
        if interactive_source_target not in {'can_kpi', 'udp_kpi'}:
            interactive_source_target = 'udp_kpi'
        config_xml = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('config_xml', '').strip()))
        optional_config = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('optional_config', '').strip()))
        launcher_path = _runtime_root(self.workspace_root) / 'kpi_runtime_launcher.sh'

        if tool_key != 'hyperlink' and image_path is None:
            raise ValueError(f'Container image is not available for {tool_key}')

        if tool_key == 'udp_kpi' and interactive_plot_mode == 'enabled' and input_mode == 'json':
            json_path = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('json_path', '').strip()))
            if not json_path:
                raise ValueError('json_path is required for JSON mode')

            interactive_image = _resolve_image_path(self.workspace_root, 'interactive_plot', '')
            if interactive_image is None:
                raise ValueError('Container image is not available for interactive_plot')
            interactive_image = _stage_bundle_file_for_user_job(self.workspace_root, interactive_image)

            if optional_config:
                launcher_path = _stage_kpi_bundle_support(self.workspace_root, tool_key, interactive_plot_mode) / 'kpi_runtime_launcher.sh'
                launcher_args = [
                    '--target', tool_key,
                    '--source-target', interactive_source_target,
                    '--interactive-mode', interactive_plot_mode,
                    '--input-mode', input_mode,
                    '--output-dir', runtime_output_path,
                    '--json-path', json_path,
                ]
                if config_xml:
                    launcher_args.extend(['--config-xml', config_xml])
                launcher_args.extend(['--optional-config', optional_config])
                return {
                    'kind': 'single',
                    'command': _script_command(launcher_path, launcher_args),
                    'mode': input_mode,
                    'input_path': json_path,
                }

            effective_config_xml = config_xml
            if not effective_config_xml:
                effective_config_xml = _stage_bundle_path_for_user_job(
                    self.workspace_root,
                    _sanitize_runtime_path(
                    str(_default_interactive_config_path(self.workspace_root, interactive_source_target))
                    ),
                )

            zmq_port = str(paths.get('port') or os.environ.get('KPI_SERVER_PORT', '5560')).strip() or '5560'
            launcher_args = [
                '--target', tool_key,
                '--source-target', interactive_source_target,
                '--interactive-mode', interactive_plot_mode,
                '--input-mode', input_mode,
                '--output-dir', runtime_output_path,
                '--json-path', json_path,
                '--config-xml', effective_config_xml,
                '--port', zmq_port,
            ]
            fallback_command = _script_command(launcher_path, launcher_args)
            return {
                'kind': 'udp_interactive_dual',
                'command': fallback_command,
                'mode': input_mode,
                'input_path': json_path,
                'primary_command': _container_run_command(image_path, ['zmq', zmq_port]),
                'secondary_command': _container_run_command(
                    interactive_image,
                    [effective_config_xml, json_path, runtime_output_path],
                    {
                        'KPI_SERVER_HOST': '127.0.0.1',
                        'KPI_SERVER_PORT': zmq_port,
                    },
                ),
                'secondary_port': zmq_port,
            }

        if tool_key in {'can_kpi', 'udp_kpi'} and interactive_plot_mode == 'enabled':
            launcher_path = _stage_kpi_bundle_support(self.workspace_root, tool_key, interactive_plot_mode) / 'kpi_runtime_launcher.sh'
            if not launcher_path.exists():
                raise ValueError(f'Combined KPI launcher is not available: {launcher_path}')

            launcher_args = [
                '--target', tool_key,
                '--source-target', interactive_source_target,
                '--interactive-mode', interactive_plot_mode,
                '--input-mode', input_mode,
                '--output-dir', runtime_output_path,
            ]
            if config_xml:
                launcher_args.extend(['--config-xml', config_xml])
            if optional_config:
                launcher_args.extend(['--optional-config', optional_config])

            if input_mode == 'hdf':
                input_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('input_hdf', '').strip()))
                output_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('output_hdf', '').strip()))
                if not input_hdf or not output_hdf:
                    raise ValueError('input_hdf and output_hdf are required for HDF mode')
                launcher_args.extend(['--input-hdf', input_hdf, '--output-hdf', output_hdf])
                return {
                    'kind': 'single',
                    'command': _script_command(launcher_path, launcher_args),
                    'mode': input_mode,
                    'input_path': input_hdf,
                }

            json_path = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('json_path', '').strip()))
            if not json_path:
                raise ValueError('json_path is required for JSON mode')
            launcher_args.extend(['--json-path', json_path])
            return {
                'kind': 'single',
                'command': _script_command(launcher_path, launcher_args),
                'mode': input_mode,
                'input_path': json_path,
            }

        if tool_key in {'can_kpi', 'udp_kpi'}:
            if input_mode == 'hdf':
                input_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('input_hdf', '').strip()))
                output_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('output_hdf', '').strip()))
                if not input_hdf or not output_hdf:
                    raise ValueError('input_hdf and output_hdf are required for HDF mode')
                return {
                    'kind': 'single',
                    'command': _container_run_command(image_path, ['hdf', input_hdf, output_hdf, runtime_output_path]),
                    'mode': 'hdf',
                    'input_path': input_hdf,
                }
            json_path = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('json_path', '').strip()))
            if not json_path:
                raise ValueError('json_path is required for JSON mode')
            return {
                'kind': 'single',
                'command': _container_run_command(image_path, ['json', json_path, runtime_output_path]),
                'mode': 'json',
                'input_path': json_path,
            }

        if tool_key == 'interactive_plot':
            launcher_path = _stage_kpi_bundle_support(self.workspace_root, tool_key, 'only') / 'kpi_runtime_launcher.sh'
            if not launcher_path.exists():
                raise ValueError(f'Interactive Plot launcher is not available: {launcher_path}')

            launcher_args = [
                '--target', 'interactive_plot',
                '--source-target', interactive_source_target,
                '--interactive-mode', 'only',
                '--input-mode', input_mode,
                '--output-dir', runtime_output_path,
            ]
            if config_xml:
                launcher_args.extend(['--config-xml', config_xml])
            if optional_config:
                launcher_args.extend(['--optional-config', optional_config])

            if input_mode == 'hdf':
                input_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('input_hdf', '').strip()))
                output_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('output_hdf', '').strip()))
                if not input_hdf or not output_hdf:
                    raise ValueError('input_hdf and output_hdf are required for HDF mode')
                launcher_args.extend(['--input-hdf', input_hdf, '--output-hdf', output_hdf])
                return {
                    'kind': 'single',
                    'command': _script_command(launcher_path, launcher_args),
                    'mode': input_mode,
                    'input_path': input_hdf,
                }

            inputs_json = _stage_bundle_path_for_user_job(self.workspace_root, _sanitize_runtime_path(paths.get('json_path', '').strip()))
            if not inputs_json:
                raise ValueError('json_path is required for interactive plot JSON mode')
            launcher_args.extend(['--json-path', inputs_json])
            return {
                'kind': 'single',
                'command': _script_command(launcher_path, launcher_args),
                'mode': input_mode,
                'input_path': inputs_json,
            }

        if tool_key == 'rag':
            html_root = _normalize_container_path(paths.get('html_root', '').strip()) or _normalize_container_path(str(self.workspace_root / 'main_html'))
            if paths.get('ingest_only'):
                env_overrides = _service_environment(self.workspace_root, 'rag')
                env_overrides['HTML_ROOT_PATH'] = html_root
                return {
                    'kind': 'single',
                    'command': _container_run_command(image_path, ['--scrap', html_root], env_overrides),
                    'mode': 'ingest',
                    'input_path': html_root,
                }
            env_overrides = _service_environment(self.workspace_root, 'rag')
            env_overrides['HTML_ROOT_PATH'] = html_root
            return {
                'kind': 'single',
                'command': _container_run_command(image_path, ['--talk'], env_overrides),
                'mode': 'service',
                'input_path': html_root,
                'service_tool_key': 'rag',
                'service_port': int(env_overrides.get('FLASK_PORT', '5100')),
            }

        if tool_key == 'main_html':
            env_overrides = _service_environment(self.workspace_root, 'main_html')
            return {
                'kind': 'single',
                'command': _container_run_command(image_path, [], env_overrides),
                'mode': 'service',
                'input_path': '',
            }

        if tool_key == 'hyperlink':
            html_root = paths.get('html_root', '').strip()
            video_root = paths.get('video_root', '').strip()
            script = self.workspace_root / 'Hyperlink_tool' / 'code' / 'main.py'
            command = [sys.executable, str(script)]
            if html_root:
                command.append(html_root)
            if video_root:
                command.append(video_root)
            return {
                'kind': 'single',
                'command': command,
                'mode': 'viewer',
                'input_path': html_root,
            }

        metadata = tool.get('metadata', {}) or {}
        if image_path is not None:
            env_overrides = metadata.get('env', {}) if isinstance(metadata.get('env', {}), dict) else {}
            bind_paths_raw = metadata.get('bind_paths', [])
            if isinstance(bind_paths_raw, str):
                bind_paths = [item.strip() for item in bind_paths_raw.splitlines() if item.strip()]
            elif isinstance(bind_paths_raw, list):
                bind_paths = [str(item).strip() for item in bind_paths_raw if str(item).strip()]
            else:
                bind_paths = []

            default_mode = str(metadata.get('default_mode') or tool.get('category') or 'run').strip().lower()
            default_args_raw = metadata.get('default_args', [])
            if isinstance(default_args_raw, str):
                default_args = shlex.split(default_args_raw)
            elif isinstance(default_args_raw, list):
                default_args = [str(item) for item in default_args_raw]
            else:
                default_args = []

            primary_input = ''
            if default_args:
                command_args = default_args
            elif default_mode == 'json':
                json_path = _stage_bundle_path_for_user_job(self.workspace_root, _normalize_container_path(paths.get('json_path', '').strip()))
                if not json_path:
                    raise ValueError('json_path is required for JSON launch mode')
                command_args = [json_path, runtime_output_path]
                primary_input = json_path
            elif default_mode == 'hdf':
                input_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _normalize_container_path(paths.get('input_hdf', '').strip()))
                output_hdf = _stage_bundle_path_for_user_job(self.workspace_root, _normalize_container_path(paths.get('output_hdf', '').strip()))
                if not input_hdf or not output_hdf:
                    raise ValueError('input_hdf and output_hdf are required for HDF launch mode')
                command_args = [input_hdf, output_hdf, runtime_output_path]
                primary_input = input_hdf
            else:
                command_args = []
                primary_input = _stage_bundle_path_for_user_job(self.workspace_root, _normalize_container_path(paths.get('json_path', '').strip())) or _stage_bundle_path_for_user_job(self.workspace_root, _normalize_container_path(paths.get('input_hdf', '').strip()))

            return {
                'kind': 'single',
                'command': _container_run_command(image_path, command_args, env_overrides, bind_paths),
                'mode': default_mode,
                'input_path': primary_input,
            }

        raise ValueError(f'Unsupported tool command for {tool_key}')

    @staticmethod
    def _tmux_session_name(tool_key: str, requested_by: str, run_dir: Path) -> str:
        safe_user = ''.join(character if character.isalnum() or character in {'-', '_'} else '_' for character in requested_by)
        return f"{safe_user}_{tool_key}_{run_dir.name}"[:120]

    @staticmethod
    def _window_payload(
        command: List[str],
        pane_name: str,
        log_path: Path,
        exit_path: Path,
        shared_log_path: Optional[Path] = None,
        mirror_log_path: Optional[Path] = None,
        service_host_file: str = '',
        wait_for_port: str = '',
    ) -> str:
        pane_label = pane_name.upper()
        tee_targets = []
        if shared_log_path:
            tee_targets.append('"$SHARED_LOG"')
        if mirror_log_path:
            tee_targets.append('"$MIRROR_LOG"')
        if tee_targets:
            exec_redirect = (
                'exec > >(sed -u '
                + shlex.quote(f"s/^/[{pane_label}] /")
                + ' | tee -a '
                + ' '.join(tee_targets)
                + ' >> "$PANE_LOG") 2>&1'
            )
        else:
            exec_redirect = 'exec >> "$PANE_LOG" 2>&1'
        payload_parts = [
            'set -uo pipefail',
            f"PANE_LOG={shlex.quote(str(log_path))}",
            f"SHARED_LOG={shlex.quote(str(shared_log_path))}" if shared_log_path else "SHARED_LOG=''",
            f"MIRROR_LOG={shlex.quote(str(mirror_log_path))}" if mirror_log_path else "MIRROR_LOG=''",
            'mkdir -p "$(dirname \"$PANE_LOG\")"',
            'touch "$PANE_LOG"',
            'if [[ -n "$SHARED_LOG" ]]; then mkdir -p "$(dirname \"$SHARED_LOG\")"; touch "$SHARED_LOG"; fi',
            'if [[ -n "$MIRROR_LOG" ]]; then mkdir -p "$(dirname \"$MIRROR_LOG\")"; touch "$MIRROR_LOG"; fi',
            exec_redirect,
            f"printf '%s\\n' {shlex.quote(f'[{pane_label}] COMMAND: {_shell_join(command)}')}",
            f'printf "%s\\n" "[{pane_label}] START: $(date -Iseconds 2>/dev/null || date)"',
        ]
        if service_host_file:
            payload_parts.append(f"(hostname -f 2>/dev/null || hostname) > {shlex.quote(service_host_file)}")
        if wait_for_port:
            payload_parts.append(
                f"for attempt in $(seq 1 90); do (echo > /dev/tcp/127.0.0.1/{shlex.quote(wait_for_port)}) >/dev/null 2>&1 && break; sleep 2; done"
            )
            payload_parts.append(
                f"(echo > /dev/tcp/127.0.0.1/{shlex.quote(wait_for_port)}) >/dev/null 2>&1 || {{ echo 'Timed out waiting for port {wait_for_port}' >&2; status=1; printf '%s' \"$status\" > {shlex.quote(str(exit_path))}; exit \"$status\"; }}"
            )
        payload_parts.append('set +e')
        payload_parts.append(_shell_join(command))
        payload_parts.append('status=$?')
        payload_parts.append('set -e')
        payload_parts.append(f'printf "%s\\n" "[{pane_label}] EXIT_STATUS: $status"')
        payload_parts.append(f"printf '%s' \"$status\" > {shlex.quote(str(exit_path))}")
        payload_parts.append('exit "$status"')
        return '; '.join(payload_parts)

    @staticmethod
    def _tmux_input_worker(queue_path: Path, pane_names: List[str]) -> str:
        default_pane = pane_names[0] if pane_names else 'main'
        valid_panes = ' | '.join(pane_names) if pane_names else default_pane
        return f"""
INPUT_QUEUE={shlex.quote(str(queue_path))}
touch \"$INPUT_QUEUE\"
tmux_queue_worker() {{
    local seen_lines=0
    while tmux has-session -t \"$SESSION_NAME\" >/dev/null 2>&1; do
        local total_lines
        total_lines=\"$(wc -l < \"$INPUT_QUEUE\" 2>/dev/null | tr -d '[:space:]')\"
        total_lines=\"${{total_lines:-0}}\"
        if (( total_lines > seen_lines )); then
            while IFS=$'\t' read -r pane_name payload; do
                pane_name=\"${{pane_name:-{default_pane}}}\"
                case \"$pane_name\" in
                    {valid_panes}) ;;
                    *) pane_name={shlex.quote(default_pane)} ;;
                esac
                if [[ \"$payload\" == '__CTRL_C__' ]]; then
                    tmux send-keys -t \"$SESSION_NAME:$pane_name\" C-c
                elif [[ \"$payload\" == '__ENTER__' ]]; then
                    tmux send-keys -t \"$SESSION_NAME:$pane_name\" C-m
                elif [[ -n \"$payload\" ]]; then
                    tmux send-keys -t \"$SESSION_NAME:$pane_name\" -l -- \"$payload\"
                    tmux send-keys -t \"$SESSION_NAME:$pane_name\" C-m
                fi
            done < <(sed -n \"$((seen_lines + 1)),$total_lines p\" \"$INPUT_QUEUE\")
            seen_lines=$total_lines
        fi
        sleep 1
    done
}}
tmux_queue_worker &
INPUT_WATCHER_PID=$!
""".strip()

    def _write_slurm_launch_script(
        self,
        tool_key: str,
        launch_plan: Dict[str, Any],
        resources: Dict[str, Any],
        run_dir: Path,
        requested_by: str,
    ) -> Path:
        script_path = run_dir / 'slurm_tmux_launcher.sh'
        session_name = self._tmux_session_name(tool_key, requested_by, run_dir)
        singularity_module = (os.environ.get('SINGULARITY_MODULE') or 'singularity/3.11.4').strip()
        slurm_defaults = _cluster_slurm_defaults()
        account_value = str(resources.get('account') or slurm_defaults['account']).strip()
        partition_value = str(resources.get('partition') or slurm_defaults['partition']).strip()

        srun_command = [_srun_binary() or 'srun']
        if account_value:
            srun_command.append(f'--account={account_value}')
        if partition_value:
            srun_command.append(f'--partition={partition_value}')
        srun_command.extend([
            f"--nodes={resources.get('nodes') or 1}",
            f"--ntasks={resources.get('ntasks') or 1}",
            f"--cpus-per-task={resources.get('cpus') or 8}",
            f"--mem={resources.get('memory') or '72G'}",
            f"--time={resources.get('time_limit') or '18:00:00'}",
            f"--job-name={session_name}",
        ])
        immediate_seconds = _slurm_immediate_seconds(tool_key, resources)
        if immediate_seconds:
            srun_command.append(f'--immediate={immediate_seconds}')
        qos = str(resources.get('qos') or slurm_defaults['qos']).strip()
        if qos:
            srun_command.append(f'--qos={qos}')
        exclude_nodes = (resources.get('exclude') or os.environ.get('HPC_TOOLS_SLURM_EXCLUDE_NODES') or '').strip()
        if exclude_nodes:
            srun_command.append(f'--exclude={exclude_nodes}')
        gres_value = (resources.get('gres') or '').strip()
        if not gres_value and resources.get('gpu'):
            gres_value = 'gpu:1'
        if gres_value:
            srun_command.append(f'--gres={gres_value}')

        service_host_file = launch_plan.get('service_host_file', '')
        pane_log_dir = Path(launch_plan.get('pane_log_dir') or run_dir)
        console_log = Path(launch_plan.get('console_log_path') or run_dir / 'runtime_console.log')
        project_console_log = Path(launch_plan['project_console_log_path']) if launch_plan.get('project_console_log_path') else None
        input_queue = Path(launch_plan.get('input_queue_path') or run_dir / 'tmux_input.queue')
        pane_names = launch_plan.get('pane_names') or ['main']
        single_exit = run_dir / 'compute_main.exit'
        single_log = pane_log_dir / 'compute_main.log'
        udp_exit = run_dir / 'compute_udp.exit'
        udp_log = pane_log_dir / 'compute_udp.log'
        interactive_exit = run_dir / 'compute_interactive.exit'
        interactive_log = pane_log_dir / 'compute_interactive.log'

        output_dir_for_mkdir = (launch_plan.get('output_path') or '').strip()
        mkdir_step = f'mkdir -p {shlex.quote(output_dir_for_mkdir)}\n' if output_dir_for_mkdir else ''

        if launch_plan.get('kind') == 'udp_interactive_dual':
            udp_payload = self._window_payload(
                launch_plan['primary_command'],
                'udp',
                udp_log,
                udp_exit,
                shared_log_path=project_console_log or console_log,
                mirror_log_path=console_log if project_console_log else None,
            )
            interactive_payload = self._window_payload(
                launch_plan['secondary_command'],
                'interactive',
                interactive_log,
                interactive_exit,
                shared_log_path=project_console_log or console_log,
                mirror_log_path=console_log if project_console_log else None,
                wait_for_port=str(launch_plan.get('secondary_port') or '5560'),
            )
            input_worker = self._tmux_input_worker(input_queue, ['udp', 'interactive'])
            remote_script = f"""set -euo pipefail
if type module >/dev/null 2>&1; then
    module load slurm >/dev/null 2>&1 || true
    module load {shlex.quote(singularity_module)} >/dev/null 2>&1 || true
fi
{mkdir_step}SESSION_NAME={shlex.quote(session_name)}
UDP_EXIT={shlex.quote(str(udp_exit))}
INTERACTIVE_EXIT={shlex.quote(str(interactive_exit))}
INPUT_QUEUE={shlex.quote(str(input_queue))}
rm -f \"$UDP_EXIT\" \"$INTERACTIVE_EXIT\"
if command -v tmux >/dev/null 2>&1; then
    unset TMUX
    tmux kill-session -t \"$SESSION_NAME\" >/dev/null 2>&1 || true
    tmux new-session -d -s \"$SESSION_NAME\" -n udp
    tmux send-keys -t \"$SESSION_NAME:udp\" {shlex.quote('bash -lc ' + shlex.quote(udp_payload))} C-m
    tmux new-window -t \"$SESSION_NAME\" -n interactive
    tmux send-keys -t \"$SESSION_NAME:interactive\" {shlex.quote('bash -lc ' + shlex.quote(interactive_payload))} C-m
    {input_worker}
    udp_status=''
    interactive_status=''
    while true; do
        if [[ -f \"$INTERACTIVE_EXIT\" ]]; then
            interactive_status=\"$(cat \"$INTERACTIVE_EXIT\")\"
            break
        fi
        if [[ -f \"$UDP_EXIT\" ]]; then
            udp_status=\"$(cat \"$UDP_EXIT\")\"
            break
        fi
        sleep 2
    done
    kill \"${{INPUT_WATCHER_PID:-}}\" >/dev/null 2>&1 || true
    wait \"${{INPUT_WATCHER_PID:-}}\" >/dev/null 2>&1 || true
    tmux kill-session -t \"$SESSION_NAME\" >/dev/null 2>&1 || true
    if [[ -n \"$udp_status\" && \"$udp_status\" != '0' ]]; then
        echo 'UDP KPI server exited before Interactive Plot finished' >&2
        exit \"$udp_status\"
    fi
    if [[ -z \"$interactive_status\" ]]; then
        echo 'Interactive Plot did not finish cleanly' >&2
        exit 1
    fi
    exit \"$interactive_status\"
fi
bash -lc {shlex.quote(udp_payload)} &
udp_pid=$!
bash -lc {shlex.quote(interactive_payload)}
interactive_status=$?
kill \"$udp_pid\" >/dev/null 2>&1 || true
wait \"$udp_pid\" >/dev/null 2>&1 || true
exit \"$interactive_status\"
"""
        else:
            single_payload = self._window_payload(
                launch_plan['command'],
                'main',
                single_log,
                single_exit,
                shared_log_path=project_console_log or console_log,
                mirror_log_path=console_log if project_console_log else None,
                service_host_file=service_host_file,
            )
            input_worker = self._tmux_input_worker(input_queue, pane_names)
            remote_script = f"""set -euo pipefail
if type module >/dev/null 2>&1; then
    module load slurm >/dev/null 2>&1 || true
    module load {shlex.quote(singularity_module)} >/dev/null 2>&1 || true
fi
{mkdir_step}SESSION_NAME={shlex.quote(session_name)}
EXIT_FILE={shlex.quote(str(single_exit))}
INPUT_QUEUE={shlex.quote(str(input_queue))}
rm -f \"$EXIT_FILE\"
if command -v tmux >/dev/null 2>&1; then
    unset TMUX
    tmux kill-session -t \"$SESSION_NAME\" >/dev/null 2>&1 || true
    tmux new-session -d -s \"$SESSION_NAME\" -n main
    tmux send-keys -t \"$SESSION_NAME:main\" {shlex.quote('bash -lc ' + shlex.quote(single_payload))} C-m
    {input_worker}
    while [[ ! -f \"$EXIT_FILE\" ]]; do
        sleep 2
    done
    status=\"$(cat \"$EXIT_FILE\")\"
    kill \"${{INPUT_WATCHER_PID:-}}\" >/dev/null 2>&1 || true
    wait \"${{INPUT_WATCHER_PID:-}}\" >/dev/null 2>&1 || true
    tmux kill-session -t \"$SESSION_NAME\" >/dev/null 2>&1 || true
    exit \"$status\"
fi
bash -lc {shlex.quote(single_payload)}
"""

        script_content = (
            '#!/usr/bin/env bash\n'
            'set -euo pipefail\n\n'
            f"cd {shlex.quote(str(self.workspace_root))}\n"
            + (_shell_join(['ssh',
                            '-o', 'StrictHostKeyChecking=no',
                            '-o', 'PasswordAuthentication=yes',
                            '-o', 'NumberOfPasswordPrompts=1',
                            f'{launch_plan["ssh_run_as_user"]}@127.0.0.1']) + ' '
               if (launch_plan.get('ssh_run_as_user') or '').strip() else '')
            + _shell_join(srun_command)
            + ' bash -lc '
            + shlex.quote(remote_script)
            + '\n'
        )
        script_path.write_text(script_content, encoding='utf-8')
        script_path.chmod(0o755)
        return script_path

    def _maybe_prefix_scheduler(
        self,
        tool_key: str,
        launch_plan: Dict[str, Any],
        resources: Dict[str, Any],
        run_dir: Path,
        requested_by: str,
    ) -> List[str]:
        use_slurm = str(resources.get('scheduler', '')).lower() == 'slurm'
        strict_slurm = _strict_slurm_required(tool_key)
        if not use_slurm:
            if strict_slurm:
                raise ValueError('KPI and Interactive Plot launches must use Slurm on this HPCC deployment. Local login-node execution is disabled.')
            return launch_plan['command']
        if _srun_binary() is None or os.name == 'nt':
            if strict_slurm and os.name != 'nt':
                raise ValueError('Slurm execution was requested but srun is unavailable in the current environment. The broker blocked login-node KPI fallback.')
            return launch_plan['command']

        launcher_path = self._write_slurm_launch_script(tool_key, launch_plan, resources, run_dir, requested_by)
        return ['bash', str(launcher_path)]

    @staticmethod
    def _utcnow() -> str:
        return datetime.now(timezone.utc).isoformat().replace('+00:00', 'Z')


class BrokerHandler(socketserver.StreamRequestHandler):
    def handle(self) -> None:
        import traceback as _traceback
        raw = self.rfile.readline()
        if not raw:
            return
        try:
            payload = json.loads(raw.decode('utf-8'))
            response = self.server.runtime_broker.handle(payload)
        except Exception as exc:
            _traceback.print_exc()
            response = {'ok': False, 'error': str(exc)}
        self.wfile.write((json.dumps(response) + '\n').encode('utf-8'))


class ThreadedBrokerServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True

    def __init__(self, address: Tuple[str, int], handler_cls, runtime_broker: RuntimeBroker):
        super().__init__(address, handler_cls)
        self.runtime_broker = runtime_broker


def maybe_start_main_html(ui_command: str, launch_env: Dict[str, str]) -> Optional[subprocess.Popen]:
    workspace_root = _project_root()
    command: Optional[List[str]] = None
    if ui_command:
        command = shlex.split(ui_command)
    else:
        command = default_service_command(workspace_root, 'main_html')

    if not command:
        return None
    return subprocess.Popen(command, cwd=str(workspace_root), env=launch_env)


def maybe_start_rag(launch_env: Dict[str, str]) -> Optional[subprocess.Popen]:
    auto_start = (os.environ.get('HPCC_AUTO_START_RAG') or '1').strip().lower()
    if auto_start not in {'1', 'true', 'yes', 'y'}:
        return None

    workspace_root = _project_root()
    command = default_service_command(workspace_root, 'rag')
    if not command:
        return None

    rag_env = launch_env.copy()
    host_rag_script = (workspace_root / 'rag' / 'main.py').resolve()
    command_path = None
    if len(command) >= 2:
        try:
            command_path = Path(command[1]).resolve()
        except OSError:
            command_path = None
    if command_path != host_rag_script:
        rag_env.update(_service_environment(workspace_root, 'rag'))
    return subprocess.Popen(command, cwd=str(workspace_root), env=rag_env)


def main() -> None:
    # Set umask 022 so all dirs/files the broker creates are readable and traversable
    # by other users (e.g. the user's own srun job needs to traverse run_dir parents).
    if os.name != 'nt':
        os.umask(0o022)

    parser = argparse.ArgumentParser(description='HPCC runtime broker for main_html + SIMG tools')
    parser.add_argument('--host', default=os.environ.get('HPCC_BROKER_HOST', '127.0.0.1'))
    parser.add_argument('--port', type=int, default=int(os.environ.get('HPCC_BROKER_PORT', '9100')))
    parser.add_argument('--ui-command', default=os.environ.get('HPCC_MAIN_HTML_CMD', ''))
    parser.add_argument('--broker-only', action='store_true', help='Run the socket broker only and skip launching services')
    args = parser.parse_args()

    workspace_root = _project_root()
    store = RuntimeStore()
    store.ensure_defaults()
    runtime_broker = RuntimeBroker(workspace_root, store)

    if _wsl_available() and os.name == 'nt' and not args.broker_only:
        main_html_on_host = _should_run_main_html_on_host(workspace_root)
        rag_on_host = _should_run_rag_on_host(workspace_root)
        launch_env = os.environ.copy()
        launch_env['HPCC_BROKER_HOST'] = '127.0.0.1'
        launch_env['HPCC_BROKER_PORT'] = str(args.port)
        launch_env.setdefault(
            'RAG_SERVICE_URL',
            _rag_service_url(workspace_root, for_wsl_client=rag_on_host and not main_html_on_host),
        )
        launch_env.setdefault('HOST', '0.0.0.0')
        launch_env.setdefault('PORT', '5002')
        launch_env.setdefault('FLASK_HOST', '0.0.0.0')
        launch_env.setdefault('FLASK_PORT', '5100')
        os.environ.update({
            'HPCC_BROKER_HOST': launch_env['HPCC_BROKER_HOST'],
            'HPCC_BROKER_PORT': launch_env['HPCC_BROKER_PORT'],
            'RAG_SERVICE_URL': launch_env['RAG_SERVICE_URL'],
            'HOST': launch_env['HOST'],
            'PORT': launch_env['PORT'],
            'FLASK_HOST': launch_env['FLASK_HOST'],
            'FLASK_PORT': launch_env['FLASK_PORT'],
        })

        broker_process = _start_wsl_broker(workspace_root, args.port)
        time.sleep(2)
        wsl_service_host = _wsl_guest_ip()
        forwarders: List[PortForwardServer] = []
        if wsl_service_host:
            forwarders.extend(_start_windows_forwarders(
                wsl_service_host,
                [args.port],
                bind_host='127.0.0.1',
            ))
            service_bind_host = (os.environ.get('HPCC_WINDOWS_SERVICE_BIND_HOST') or '0.0.0.0').strip() or '0.0.0.0'
            forwarded_service_ports: List[int] = []
            if not main_html_on_host:
                forwarded_service_ports.append(int(launch_env['PORT']))
            if not rag_on_host:
                forwarded_service_ports.append(int(launch_env['FLASK_PORT']))
            if forwarded_service_ports:
                forwarders.extend(
                    _start_windows_forwarders(
                        wsl_service_host,
                        forwarded_service_ports,
                        bind_host=service_bind_host,
                    )
                )
        ui_process = maybe_start_main_html(args.ui_command, launch_env)
        rag_process = maybe_start_rag(launch_env)

        try:
            print(f'HPCC broker delegated to WSL on 127.0.0.1:{args.port}')
            print(f'HPCC broker launcher PID {broker_process.pid}')
            if wsl_service_host:
                print(f'Windows localhost forwarding active via WSL {wsl_service_host}')
                lan_host = _local_ipv4_address()
                if lan_host:
                    print(f'LAN dashboard: http://{lan_host}:{launch_env["PORT"]}/')
                    print(f'LAN hyperlink: http://{lan_host}:{launch_env["PORT"]}/hyperlink/')
            if ui_process is not None:
                print(f'main_html launched with PID {ui_process.pid}')
            if rag_process is not None:
                print(f'rag launched with PID {rag_process.pid}')
            while True:
                if broker_process.poll() is not None:
                    raise RuntimeError('WSL broker process exited unexpectedly')
                time.sleep(1)
        except KeyboardInterrupt:
            pass
        finally:
            if ui_process is not None and ui_process.poll() is None:
                ui_process.terminate()
            if rag_process is not None and rag_process.poll() is None:
                rag_process.terminate()
            if broker_process.poll() is None:
                broker_process.terminate()
            _stop_windows_forwarders(forwarders)
        return

    bind_host = args.host
    advertised_host = os.environ.get('HPCC_BROKER_ADVERTISE_HOST', '').strip()
    if not advertised_host:
        if _wsl_available():
            advertised_host = _wsl_windows_host_ip() or ''
            if advertised_host and bind_host == '127.0.0.1':
                bind_host = '0.0.0.0'
        if not advertised_host:
            advertised_host = '127.0.0.1' if bind_host in {'0.0.0.0', '::'} else bind_host
    launch_env = os.environ.copy()
    launch_env.setdefault('HPCC_BROKER_HOST', advertised_host)
    launch_env.setdefault('HPCC_BROKER_PORT', str(args.port))
    launch_env.setdefault('RAG_SERVICE_URL', _rag_service_url(workspace_root))
    launch_env.setdefault('HOST', '0.0.0.0')
    launch_env.setdefault('PORT', '5002')
    launch_env.setdefault('FLASK_HOST', '0.0.0.0')
    launch_env.setdefault('FLASK_PORT', '5100')
    os.environ.update({
        'HPCC_BROKER_HOST': launch_env['HPCC_BROKER_HOST'],
        'HPCC_BROKER_PORT': launch_env['HPCC_BROKER_PORT'],
        'RAG_SERVICE_URL': launch_env['RAG_SERVICE_URL'],
        'HOST': launch_env['HOST'],
        'PORT': launch_env['PORT'],
        'FLASK_HOST': launch_env['FLASK_HOST'],
        'FLASK_PORT': launch_env['FLASK_PORT'],
    })

    ui_process = None if args.broker_only else maybe_start_main_html(args.ui_command, launch_env)
    rag_process = None if args.broker_only else maybe_start_rag(launch_env)

    with ThreadedBrokerServer((bind_host, args.port), BrokerHandler, runtime_broker) as server:
        try:
            print(f'HPCC broker listening on {bind_host}:{args.port}')
            if advertised_host != bind_host:
                print(f'HPCC broker advertised to containers as {advertised_host}:{args.port}')
            if ui_process is not None:
                print(f'main_html launched with PID {ui_process.pid}')
            if rag_process is not None:
                print(f'rag launched with PID {rag_process.pid}')
            server.serve_forever()
        finally:
            if ui_process is not None and ui_process.poll() is None:
                ui_process.terminate()
            if rag_process is not None and rag_process.poll() is None:
                rag_process.terminate()


if __name__ == '__main__':
    main()