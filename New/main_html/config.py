"""
Configuration settings for the HPC Flask Application
"""
import os
from datetime import timedelta
from pathlib import Path

# Import environment utilities
try:
    from env_utils import get_env, get_db_uri, get_cache_dir, get_cluster_paths, get_cluster_slurm_defaults
except ImportError:
    # Fallback if running from different location
    import sys
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from env_utils import get_env, get_db_uri, get_cache_dir, get_cluster_paths, get_cluster_slurm_defaults


_SLURM_DEFAULTS = get_cluster_slurm_defaults()


def _default_database_uri() -> str:
    """Return a sensible default DB URL using env_utils.
    
    Database is stored in simg/.cache_html/hpc_tools_dev.db
    """
    return get_db_uri()


def _get_data_base_path() -> str:
    """Get data base path based on environment"""
    env = get_env()
    
    # Check environment variable first
    env_path = os.environ.get('DATA_BASE_PATH')
    if env_path:
        return env_path
    
    # Get cluster-specific path (empty if not configured for this project)
    cluster_paths = get_cluster_paths()
    path = cluster_paths.get('data_base', '') or ''
    if path:
        return path
    
    return '/data'


def _get_scratch_dir() -> str:
    """Get scratch directory based on environment"""
    env = get_env()
    
    env_scratch = os.environ.get('SCRATCH_DIR')
    if env_scratch:
        return env_scratch
    
    if env.is_windows:
        return os.path.join(get_cache_dir(), 'scratch')
    
    return '/scratch/uploads'


def _get_llm_url() -> str:
    """Get LLM service URL based on environment"""
    env = get_env()
    
    env_url = os.environ.get('LLM_SERVICE_URL')
    if env_url:
        return env_url
    
    # Use cluster-specific LLM URL
    cluster_paths = get_cluster_paths()
    return f"http://{cluster_paths['host']}:8000/generate"


class Config:
    """Base configuration"""
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'your-secret-key-change-in-production'
    COMPRESS_ENABLED = True
    COMPRESS_LEVEL = int(os.environ.get('COMPRESS_LEVEL') or '6')
    COMPRESS_MIN_SIZE = int(os.environ.get('COMPRESS_MIN_SIZE') or '512')
    
    # Database Configuration - stored in simg/.cache_html/
    SQLALCHEMY_DATABASE_URI = _default_database_uri()
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    
    # Cache directory for app data
    CACHE_DIR = get_cache_dir()
    
    # Session Configuration
    PERMANENT_SESSION_LIFETIME = timedelta(hours=8)
    
    # HPC/Slurm Configuration
    SLURM_PARTITION = _SLURM_DEFAULTS['partition']
    SLURM_ACCOUNT = _SLURM_DEFAULTS['account']
    SLURM_QOS = _SLURM_DEFAULTS['qos']
    
    # File System Paths - auto-detected based on environment
    SCRATCH_DIR = _get_scratch_dir()
    DATA_BASE_PATH = _get_data_base_path()
    UPLOAD_FOLDER = os.environ.get('UPLOAD_FOLDER') or _get_scratch_dir()
    ALLOWED_EXTENSIONS = {'h5', 'hdf5', 'mf4', 'csv', 'json', 'xml'}
    MAX_CONTENT_LENGTH = 500 * 1024 * 1024  # 500MB max upload
    
    # Singularity Configuration
    # Default to simg folder in the project root
    SINGULARITY_IMAGE_PATH = os.environ.get('SINGULARITY_IMAGE_PATH') or os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'simg'
    )
    SINGULARITY_MODULE = os.environ.get('SINGULARITY_MODULE') or 'singularity/3.11.4'
    
    # LLM Configuration - auto-detected based on cluster
    LLM_SERVICE_URL = _get_llm_url()
    LLM_MODEL_NAME = os.environ.get('LLM_MODEL_NAME') or 'qwen'
    
    # ChromaDB Configuration - stored in cache dir
    CHROMADB_PATH = os.environ.get('CHROMADB_PATH') or os.path.join(get_cache_dir(), 'chromadb_data')
    
    # Tool Configurations with resource requirements
    TOOLS = {
        'interactive_plot': {
            'name': 'Interactive Plot',
            'description': 'Generate interactive HTML reports from HDF sensor data',
            'singularity_image': 'interactive_plot.simg',
            'script': 'run_interactive_plot.sh',
            'memory': '64G',
            'cpus': 8,
            'time_limit': '04:00:00'
        },
        'kpi': {
            'name': 'KPI Analysis',
            'description': 'KPI HTML generation with JSON batch and HDF pair modes',
            'singularity_image': 'kpi.simg',
            'script': 'run_kpi.sh',
            'memory': '32G',
            'cpus': 8,
            'time_limit': '02:00:00'
        },
        'hyperlink_tool': {
            'name': 'Hyperlink Tool (LogView)',
            'description': 'Serves HTML & video, provides mapping API',
            'singularity_image': 'all_services.simg',
            'script': 'run_hyperlink.sh',
            'memory': '8G',
            'cpus': 2,
            'time_limit': '00:30:00'
        }
    }


class DevelopmentConfig(Config):
    """Development configuration"""
    DEBUG = True
    SQLALCHEMY_ECHO = True


class ProductionConfig(Config):
    """Production configuration"""
    DEBUG = False
    SQLALCHEMY_ECHO = False
    COMPRESS_ENABLED = True


class TestingConfig(Config):
    """Testing configuration"""
    TESTING = True
    SQLALCHEMY_DATABASE_URI = 'sqlite:///:memory:'


config = {
    'development': DevelopmentConfig,
    'production': ProductionConfig,
    'testing': TestingConfig,
    'default': DevelopmentConfig
}
