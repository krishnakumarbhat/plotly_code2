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
    
    # Get cluster-specific path
    cluster_paths = get_cluster_paths()
    return cluster_paths['data_base']


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
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'your-secret-key-change-in-production'
    SQLALCHEMY_DATABASE_URI = _default_database_uri()
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    CACHE_DIR = get_cache_dir()
    PERMANENT_SESSION_LIFETIME = timedelta(hours=8)
    SLURM_PARTITION = _SLURM_DEFAULTS['partition']
    SLURM_ACCOUNT = _SLURM_DEFAULTS['account']
    SLURM_QOS = _SLURM_DEFAULTS['qos']
    SCRATCH_DIR = _get_scratch_dir()
    DATA_BASE_PATH = _get_data_base_path()
    UPLOAD_FOLDER = os.environ.get('UPLOAD_FOLDER') or _get_scratch_dir()
    ALLOWED_EXTENSIONS = {'h5', 'hdf5', 'mf4', 'csv', 'json', 'xml'}
    MAX_CONTENT_LENGTH = 500 * 1024 * 1024
    COMPRESS_ENABLED = True
    COMPRESS_MIMETYPES = ['text/html', 'text/css', 'text/javascript', 'application/javascript', 'application/json']
    COMPRESS_LEVEL = 6
    COMPRESS_MIN_SIZE = 500
    SINGULARITY_IMAGE_PATH = os.environ.get('SINGULARITY_IMAGE_PATH') or os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'simg'
    )
    SINGULARITY_MODULE = os.environ.get('SINGULARITY_MODULE') or 'singularity/3.11.4'
    LLM_SERVICE_URL = _get_llm_url()
    LLM_MODEL_NAME = os.environ.get('LLM_MODEL_NAME') or 'qwen'
    CHROMADB_PATH = os.environ.get('CHROMADB_PATH') or os.path.join(get_cache_dir(), 'chromadb_data')
    STATIC_CACHE_MAX_AGE = 31536000
    TEMPLATE_CACHE_MAX_AGE = 3600


class DevelopmentConfig(Config):
    """Development configuration"""
    DEBUG = True
    SQLALCHEMY_ECHO = True


class ProductionConfig(Config):
    """Production configuration"""
    DEBUG = False
    SQLALCHEMY_ECHO = False


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
