#!/usr/bin/env python3
"""
Environment Detection and Path Utilities

This module provides utilities for detecting the runtime environment
(Windows, WSL, Cluster Krakow, Cluster Southfield, Docker, Singularity)
and resolving paths appropriately.

Usage:
    from env_utils import get_env, get_cache_dir, get_db_path, resolve_path
    
    env = get_env()
    print(f"Running on: {env.name}")
    print(f"Cache dir: {get_cache_dir()}")
    print(f"DB path: {get_db_path()}")
"""

import os
import sys
import platform
import stat
from pathlib import Path
from dataclasses import dataclass
from typing import Optional
from enum import Enum


class RuntimeEnv(Enum):
    """Runtime environment types"""
    WINDOWS = "windows"
    WSL = "wsl"
    LINUX = "linux"
    KRAKOW = "krakow"
    SOUTHFIELD = "southfield"
    DOCKER = "docker"
    SINGULARITY = "singularity"


@dataclass
class EnvInfo:
    """Environment information"""
    name: str
    runtime: RuntimeEnv
    is_cluster: bool
    is_container: bool
    is_windows: bool
    base_path: str
    cache_dir: str
    db_path: str


def _detect_runtime() -> RuntimeEnv:
    """Detect the current runtime environment"""
    
    # Check for Singularity container
    if os.path.exists('/.singularity.d') or os.environ.get('SINGULARITY_CONTAINER'):
        return RuntimeEnv.SINGULARITY
    
    # Check for Docker container
    if os.path.exists('/.dockerenv') or os.environ.get('DOCKER_CONTAINER'):
        return RuntimeEnv.DOCKER
    
    # Check Windows
    if platform.system().lower() == 'windows':
        return RuntimeEnv.WINDOWS
    
    # Check WSL
    if 'microsoft' in platform.uname().release.lower():
        return RuntimeEnv.WSL
    
    # Check for Krakow cluster paths
    if os.path.isdir('/net/8k3'):
        return RuntimeEnv.KRAKOW
    
    # Check for Southfield cluster paths
    if os.path.isdir('/mnt/usmidet'):
        return RuntimeEnv.SOUTHFIELD
    
    return RuntimeEnv.LINUX


def _get_project_root() -> Path:
    """Get the project root directory"""
    project_root_override = (os.environ.get('HPCC_PROJECT_ROOT') or '').strip()
    if project_root_override:
        return Path(project_root_override).resolve()

    bundle_root_override = (os.environ.get('HPCC_BUNDLE_ROOT') or '').strip()
    if bundle_root_override:
        bundle_root = Path(bundle_root_override).resolve()
        bundle_source = bundle_root / 'bundle_src'
        if bundle_source.is_dir():
            return bundle_source
        return bundle_root

    # Try to find project root by looking for app.py or main_html folder
    current = Path(__file__).parent.resolve()
    
    for _ in range(5):  # Go up max 5 levels
        if (current / 'main_html').is_dir() or (current / 'app.py').is_file():
            return current
        if (current / 'Singularity.def').is_file():
            return current
        parent = current.parent
        if parent == current:
            break
        current = parent
    
    # Fallback to current file's parent
    return Path(__file__).parent.resolve()


def _ensure_private_directory(path: Path) -> Path:
    path.mkdir(parents=True, exist_ok=True)
    if os.name != 'nt':
        try:
            path.chmod(stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
        except OSError:
            pass
    return path


def _prefer_hidden_cache_dir(preferred: Path, legacy: Optional[Path] = None) -> Path:
    if legacy and legacy.exists() and not preferred.exists():
        try:
            legacy.rename(preferred)
        except OSError:
            pass
    return _ensure_private_directory(preferred)


def _get_cache_dir(runtime: RuntimeEnv, project_root: Path) -> str:
    """Get the cache directory based on environment"""
    
    # Check environment variable first
    env_cache = os.environ.get('CACHE_HTML_DIR')
    if env_cache:
        return env_cache

    bundle_root_override = (os.environ.get('HPCC_BUNDLE_ROOT') or '').strip()
    if bundle_root_override:
        runtime_state_dir = Path(bundle_root_override).resolve() / 'runtime_state' / 'main_html'
        cache_dir = _prefer_hidden_cache_dir(
            runtime_state_dir / '.cache_html',
            runtime_state_dir / 'cache_html',
        )
        return str(cache_dir)
    
    # For container environments, default to /tmp because SIF images are read-only at runtime.
    if runtime in (RuntimeEnv.SINGULARITY, RuntimeEnv.DOCKER):
        cache_dir = _ensure_private_directory(Path('/tmp/hpc_tools/.cache_html'))
        return str(cache_dir)
    
    # For cluster environments, use simg/.cache_html in project
    if runtime in (RuntimeEnv.KRAKOW, RuntimeEnv.SOUTHFIELD):
        simg_dir = project_root / 'simg'
        if simg_dir.is_dir():
            cache_dir = simg_dir / '.cache_html'
        else:
            cache_dir = project_root / '.cache_html'
        return str(_ensure_private_directory(cache_dir))
    
    # For Windows/WSL/Linux development
    cache_dir = project_root / 'simg' / '.cache_html'
    return str(_ensure_private_directory(cache_dir))


def _get_db_path(cache_dir: str) -> str:
    """Get the database path - always in .cache_html"""
    return os.path.join(cache_dir, 'hpc_tools_dev.db')


def get_env() -> EnvInfo:
    """Get current environment information"""
    runtime = _detect_runtime()
    project_root = _get_project_root()
    cache_dir = _get_cache_dir(runtime, project_root)
    db_path = _get_db_path(cache_dir)
    
    return EnvInfo(
        name=runtime.value,
        runtime=runtime,
        is_cluster=runtime in (RuntimeEnv.KRAKOW, RuntimeEnv.SOUTHFIELD),
        is_container=runtime in (RuntimeEnv.SINGULARITY, RuntimeEnv.DOCKER),
        is_windows=runtime == RuntimeEnv.WINDOWS,
        base_path=str(project_root),
        cache_dir=cache_dir,
        db_path=db_path
    )


def get_cache_dir() -> str:
    """Get the cache directory for the current environment"""
    return get_env().cache_dir


def get_db_path() -> str:
    """Get the database path for the current environment"""
    return get_env().db_path


def get_db_uri() -> str:
    """Get the SQLAlchemy database URI"""
    env = get_env()
    
    # Check environment variable first
    db_url = os.environ.get('DATABASE_URL')
    if db_url:
        return db_url
    
    # Use SQLite in cache directory
    db_path = env.db_path
    return f"sqlite:///{db_path}"


def resolve_path(path: str, base_dir: str = None) -> str:
    """
    Resolve a path appropriately for the current environment.
    
    Args:
        path: Path to resolve (can be relative or absolute)
        base_dir: Base directory for relative paths
        
    Returns:
        Absolute path string
    """
    if os.path.isabs(path):
        return os.path.abspath(path)
    
    if base_dir:
        return os.path.abspath(os.path.join(base_dir, path))
    
    return os.path.abspath(os.path.join(os.getcwd(), path))


def get_cluster_paths(cluster: str = None) -> dict:
    """
    Get standard paths for a cluster.
    
    Args:
        cluster: Cluster name ('krakow' or 'southfield'). 
                 If None, auto-detects from environment.
    
    Returns:
        Dict with 'data_base', 'scratch', 'user_data' paths
    """
    env = get_env()
    
    if cluster is None:
        if env.runtime == RuntimeEnv.KRAKOW:
            cluster = 'krakow'
        elif env.runtime == RuntimeEnv.SOUTHFIELD:
            cluster = 'southfield'
        else:
            cluster = 'krakow'  # Default
    
    if cluster == 'krakow':
        return {
            'data_base': '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA',
            'scratch': '/scratch',
            'host': '10.214.45.45'
        }
    elif cluster == 'southfield':
        return {
            'data_base': '/mnt/usmidet/projects/RADARCORE/2-Sim/USER_DATA/ouymc',
            'scratch': '/scratch',
            'host': '10.192.224.131'
        }
    else:
        return {
            'data_base': '/data',
            'scratch': '/tmp',
            'host': 'localhost'
        }


def get_cluster_slurm_defaults(cluster: str = None) -> dict:
    env = get_env()

    if cluster is None:
        if env.runtime == RuntimeEnv.KRAKOW:
            cluster = 'krakow'
        elif env.runtime == RuntimeEnv.SOUTHFIELD:
            cluster = 'southfield'
        else:
            cluster = 'generic'

    if cluster == 'southfield':
        defaults = {
            'partition': 'defq',
            'account': 'radarcore',
            'qos': '',
        }
    elif cluster == 'krakow':
        defaults = {
            'partition': 'plcyf-com',
            'account': 'RNA-SDV-SRR7',
            'qos': '',
        }
    else:
        defaults = {
            'partition': 'compute',
            'account': 'default',
            'qos': '',
        }

    return {
        'partition': (os.environ.get('SLURM_PARTITION') or defaults['partition']).strip(),
        'account': (os.environ.get('SLURM_ACCOUNT') or defaults['account']).strip(),
        'qos': (os.environ.get('SLURM_QOS') or defaults['qos']).strip(),
    }


def get_static_dir() -> str:
    """Get the static files directory"""
    env = get_env()
    project_root = Path(env.base_path)
    
    # Try main_html/static first
    main_html_static = project_root / 'main_html' / 'static'
    if main_html_static.is_dir():
        return str(main_html_static)
    
    # Try static in project root
    static = project_root / 'static'
    if static.is_dir():
        return str(static)
    
    return str(main_html_static)


def get_templates_dir() -> str:
    """Get the templates directory"""
    env = get_env()
    project_root = Path(env.base_path)
    
    # Try main_html/templates first
    main_html_templates = project_root / 'main_html' / 'templates'
    if main_html_templates.is_dir():
        return str(main_html_templates)
    
    # Try templates in project root
    templates = project_root / 'templates'
    if templates.is_dir():
        return str(templates)
    
    return str(main_html_templates)


# Print environment info if run directly
if __name__ == '__main__':
    env = get_env()
    print("=" * 60)
    print("  Environment Detection")
    print("=" * 60)
    print(f"Runtime:      {env.name}")
    print(f"Is Cluster:   {env.is_cluster}")
    print(f"Is Container: {env.is_container}")
    print(f"Is Windows:   {env.is_windows}")
    print(f"Base Path:    {env.base_path}")
    print(f"Cache Dir:    {env.cache_dir}")
    print(f"DB Path:      {env.db_path}")
    print(f"DB URI:       {get_db_uri()}")
    print("=" * 60)
