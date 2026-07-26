"""
Main Flask Application for HPC Tools Platform
Provides web interface for KPI, DC HTML, Interactive Plot, and Hyperlink tools
with LLM chat integration and Slurm job management
"""
import os
import uuid
import logging
import sys
import threading
import subprocess
import socket
import getpass
import platform
import signal
import shutil
import importlib.util
import time
from pathlib import Path
from datetime import datetime

from flask import Flask, render_template, request, redirect, url_for, flash, jsonify, session, send_from_directory, send_file
from flask_compress import Compress
from flask_login import LoginManager, login_user, logout_user, login_required, current_user
from flask_migrate import Migrate
from sqlalchemy.exc import OperationalError

from config import config
from hpcc_broker_client import HpccBrokerClient
from models import db, User, JobHistory, ChatSession, ChatMessage, init_db
from rag_client import RagClient
from runtime_store import RuntimeStore
from utils import (
    SlurmManager,
    FileBrowser,
    extract_hdf_filename,
    generate_slurm_script,
    slurm_is_available,
    cluster_from_path,
    validate_cluster_credentials,
)
from env_utils import get_env, get_cache_dir, get_cluster_paths

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Initialize Flask app
app = Flask(__name__)
app.config.from_object(config[os.environ.get('FLASK_ENV', 'development')])
Compress(app)

# Initialize extensions
db.init_app(app)
migrate = Migrate(app, db)
login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'
login_manager.login_message = 'Please log in to access this page.'

# For local/container SQLite deployments, ensure the schema exists so auth and
# runtime history tables are created even when the app boots through Gunicorn.
try:
    db_uri = (app.config.get('SQLALCHEMY_DATABASE_URI') or '').lower()
    if db_uri.startswith('sqlite'):
        with app.app_context():
            init_db(app)
except Exception:
    logger.exception('Failed to initialize SQLite database')

# Initialize managers
slurm_manager = SlurmManager(
    partition=app.config.get('SLURM_PARTITION', 'compute'),
    account=app.config.get('SLURM_ACCOUNT', 'default')
)

file_browser = FileBrowser([
    app.config.get('DATA_BASE_PATH', '/data'),
    app.config.get('SCRATCH_DIR', '/scratch')
])


def _repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def _hyperlink_tool_base() -> str:
    repo_root = _repo_root()
    direct_path = repo_root / 'Hyperlink_tool' / 'code' / 'html_online'
    if direct_path.exists():
        return str(direct_path)
    legacy_path = repo_root / 'tools' / 'Hyperlink_tool' / 'code' / 'html_online'
    return str(legacy_path)


def _load_hyperlink_module(module_name: str, filename: str):
    module = sys.modules.get(module_name)
    if module is None:
        module_path = Path(_hyperlink_tool_base()) / filename
        if not module_path.exists():
            raise ImportError(f'Hyperlink module not found at {module_path}')
        spec = importlib.util.spec_from_file_location(module_name, module_path)
        if spec is None or spec.loader is None:
            raise ImportError(f'Could not load Hyperlink module from {module_path}')
        module = importlib.util.module_from_spec(spec)
        sys.modules[module_name] = module
        spec.loader.exec_module(module)
    return module


def _load_hyperlink_log_viewer_app():
    module = _load_hyperlink_module('hyperlink_html_online_main', 'main.py')
    log_viewer_app = getattr(module, 'LogViewerApp', None)
    if log_viewer_app is None:
        raise ImportError('Hyperlink viewer module does not define LogViewerApp')
    return log_viewer_app


def _load_hyperlink_vlm_module():
    return _load_hyperlink_module('hyperlink_html_online_vlm', 'vlm.py')

runtime_store = RuntimeStore()
runtime_store.ensure_defaults()
broker_client = HpccBrokerClient()
rag_client = RagClient(runtime_store)

_HPCC_RESOURCE_BASE = {
    'scheduler': 'slurm',
    'account': (os.environ.get('SLURM_ACCOUNT') or 'RNA-SDV-SRR7').strip(),
    'qos': (os.environ.get('SLURM_QOS') or '').strip(),
    'nodes': 1,
    'ntasks': 1,
    'partition': (os.environ.get('SLURM_PARTITION') or 'plcyf-com').strip(),
    'immediate': (os.environ.get('HPCC_SLURM_IMMEDIATE_SECONDS') or '').strip(),
}

RUNTIME_TOOL_DEFAULTS = {
    'can_kpi': {**_HPCC_RESOURCE_BASE, 'memory': '72G', 'cpus': 8, 'time_limit': '18:00:00'},
    'udp_kpi': {**_HPCC_RESOURCE_BASE, 'memory': '72G', 'cpus': 8, 'time_limit': '18:00:00'},
    'interactive_plot': {**_HPCC_RESOURCE_BASE, 'memory': '72G', 'cpus': 8, 'time_limit': '18:00:00'},
    'rag': {**_HPCC_RESOURCE_BASE, 'memory': '72G', 'cpus': 8, 'time_limit': '18:00:00', 'gpu': True, 'gres': 'gpu:1'},
}


def _cluster_auth_enabled() -> bool:
    auth_mode = (os.environ.get('HPC_TOOLS_AUTH_MODE') or 'auto').strip().lower()
    return auth_mode == 'cluster' or (auth_mode == 'auto' and slurm_is_available())


def _allow_local_kpi_scheduler() -> bool:
    explicit = (os.environ.get('HPCC_ALLOW_LOCAL_KPI') or '').strip().lower()
    if explicit:
        return explicit in {'1', 'true', 'yes', 'y', 'on'}
    return not _cluster_auth_enabled()


def _default_admin_disabled() -> bool:
    raw = (os.environ.get('HPC_TOOLS_DISABLE_DEFAULT_ADMIN') or '').strip().lower()
    if raw in {'1', 'true', 'yes', 'y', 'on'}:
        return True
    return _cluster_auth_enabled()


def _cluster_choices():
    return [
        ('any', 'Any cluster'),
        ('krakow', f"Krakow ({get_cluster_paths('krakow')['host']})"),
        ('southfield', f"Southfield ({get_cluster_paths('southfield')['host']})"),
    ]


def _resolve_cluster_request(cluster_target: str):
    target = (cluster_target or 'any').strip().lower()
    if target == 'krakow':
        return [('krakow', get_cluster_paths('krakow')['host'])], False
    if target == 'southfield':
        return [('southfield', get_cluster_paths('southfield')['host'])], False
    return [
        ('krakow', get_cluster_paths('krakow')['host']),
        ('southfield', get_cluster_paths('southfield')['host']),
    ], False


def _auth_template_context(cluster_target: str = 'any') -> dict:
    return {
        'cluster_auth_enabled': _cluster_auth_enabled(),
        'cluster_choices': _cluster_choices(),
        'selected_cluster_target': (cluster_target or 'any').strip().lower(),
    }


@app.before_request
def _enforce_session_policy():
    _hyperlink_reap_stale_sessions()

    if not current_user.is_authenticated:
        return None

    if not getattr(current_user, 'is_active', True):
        logout_user()
        flash('Your account has been deactivated. Please log in again.', 'error')
        return redirect(url_for('login'))

    if _default_admin_disabled() and getattr(current_user, 'net_id', '') == 'admin':
        logout_user()
        flash('Administrator fallback is disabled on HPCC. Sign in with your cluster Net ID and password.', 'info')
        return redirect(url_for('login'))

    return None


@app.after_request
def _set_hyperlink_cache_headers(response):
    path = request.path or ''
    if path.startswith('/hyperlink/data/video/'):
        response.headers.setdefault('Cache-Control', 'public, max-age=86400')
        response.headers.setdefault('Accept-Ranges', 'bytes')
    elif path.startswith('/hyperlink/data/html/') or path.startswith('/hyperlink/static/') or path.startswith('/static/css/') or path.startswith('/static/js/'):
        response.headers.setdefault('Cache-Control', 'public, max-age=3600')
    return response


@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))


# ============================================================================
# AUTHENTICATION ROUTES
# ============================================================================

@app.route('/')
def index():
    """Redirect to main dashboard"""
    if current_user.is_authenticated:
        return redirect(url_for('dashboard'))
    return redirect(url_for('login'))


@app.route('/login', methods=['GET', 'POST'])
def login():
    """User login page"""
    if current_user.is_authenticated:
        return redirect(url_for('dashboard'))
    
    if request.method == 'POST':
        net_id = request.form.get('net_id', '').strip()
        password = request.form.get('password', '')
        remember = request.form.get('remember', False)
        cluster_target = (request.form.get('cluster_target') or 'any').strip().lower()

        if _default_admin_disabled() and net_id == 'admin':
            flash('Administrator fallback is disabled on HPCC. Sign in with your cluster Net ID and password.', 'error')
            return render_template('login.html', **_auth_template_context(cluster_target))

        user = User.query.filter_by(net_id=net_id).first()

        use_cluster_auth = _cluster_auth_enabled()

        if use_cluster_auth:
            clusters, require_all = _resolve_cluster_request(cluster_target)
            ok, details = validate_cluster_credentials(net_id, password, clusters, require_all=require_all)

            if ok:
                if not user:
                    user = User(name=net_id, net_id=net_id)
                    user.set_password(password)
                    db.session.add(user)
                    db.session.commit()
                    flash('Cluster authentication succeeded. Your HPC Tools profile was created automatically.', 'success')

                if not user.is_active:
                    flash('Your account has been deactivated. Please contact administrator.', 'error')
                    return render_template('login.html', **_auth_template_context(cluster_target))

                # keep local password hash in sync with the current cluster password
                user.set_password(password)
                db.session.commit()

                login_user(user, remember=remember)
                user.update_last_login()

                next_page = request.args.get('next')
                return redirect(next_page or url_for('dashboard'))

            # Cluster auth failed
            short = '; '.join(
                [
                    f"{name}: {'OK' if info.get('ok') else 'FAIL'}"
                    for name, info in (details or {}).items()
                ]
            )
            flash(f'Invalid Net ID or password (cluster auth). {short}', 'error')
            return render_template('login.html', **_auth_template_context(cluster_target))

        # local auth fallback (useful for Windows debugging)
        if user and user.check_password(password):
            if not user.is_active:
                flash('Your account has been deactivated. Please contact administrator.', 'error')
                return render_template('login.html', **_auth_template_context(cluster_target))
            
            login_user(user, remember=remember)
            user.update_last_login()
            
            next_page = request.args.get('next')
            return redirect(next_page or url_for('dashboard'))
        else:
            flash('Invalid Net ID or password', 'error')
    
    return render_template('login.html', **_auth_template_context())


@app.route('/register', methods=['GET', 'POST'])
def register():
    """User registration page"""
    if current_user.is_authenticated:
        return redirect(url_for('dashboard'))

    if _cluster_auth_enabled():
        flash('Use your cluster Net ID and password on the login page. Your profile will be created automatically after successful cluster authentication.', 'info')
        return redirect(url_for('login'))
    
    if request.method == 'POST':
        name = request.form.get('name', '').strip()
        net_id = request.form.get('net_id', '').strip()
        password = request.form.get('password', '')
        
        # Validation
        errors = []
        if not name:
            errors.append('Name is required')
        if not net_id:
            errors.append('Net ID is required')
        if len(password) < 6:
            errors.append('Password must be at least 6 characters')
        
        if User.query.filter_by(net_id=net_id).first():
            errors.append('This Net ID is already registered')
        
        if errors:
            for error in errors:
                flash(error, 'error')
            return render_template('register.html')

        auth_mode = (os.environ.get('HPC_TOOLS_AUTH_MODE') or 'auto').strip().lower()
        use_cluster_auth = auth_mode == 'cluster' or (auth_mode == 'auto' and slurm_is_available())

        if use_cluster_auth:
            # Validate against both clusters (as requested). If you want "either" cluster, set require_all=False.
            clusters = [
                ('krakow', get_cluster_paths('krakow')['host']),
                ('southfield', get_cluster_paths('southfield')['host']),
            ]
            ok, details = validate_cluster_credentials(net_id, password, clusters, require_all=True)
            if not ok:
                short = '; '.join(
                    [
                        f"{name}: {'OK' if info.get('ok') else 'FAIL'}"
                        for name, info in (details or {}).items()
                    ]
                )
                flash(f'Cluster login failed (must succeed on Krakow and Southfield). {short}', 'error')
                return render_template('register.html')
        
        # Create user
        user = User(name=name, net_id=net_id)
        user.set_password(password)
        db.session.add(user)
        db.session.commit()

        flash('Registration successful. Please sign in with your Net ID and password.', 'success')
        return redirect(url_for('login'))
    
    return render_template('register.html')


@app.route('/logout')
@login_required
def logout():
    """User logout"""
    logout_user()
    flash('You have been logged out.', 'info')
    return redirect(url_for('login'))


# ============================================================================
# MAIN DASHBOARD ROUTES
# ============================================================================

@app.route('/html')
@login_required
def dashboard():
    """Main dashboard with chat and tools navigation"""
    # Get recent job history for current user
    recent_jobs = JobHistory.query.filter_by(user_id=current_user.id)\
        .order_by(JobHistory.created_at.desc())\
        .limit(10)\
        .all()
    for job in recent_jobs:
        _sync_runtime_job(job)
    
    # Get active chat session or create new one
    active_session = ChatSession.query.filter_by(
        user_id=current_user.id,
        is_active=True
    ).first()
    
    if not active_session:
        active_session = ChatSession(
            user_id=current_user.id,
            session_id=str(uuid.uuid4())
        )
        db.session.add(active_session)
        db.session.commit()
    
    tools = runtime_store.list_tools()
    try:
        broker_health = broker_client.ping()
    except Exception as exc:
        broker_health = {'ok': False, 'error': str(exc), 'status': 'offline'}
    
    return render_template('dashboard.html', 
                         recent_jobs=recent_jobs,
                         chat_session=active_session,
                         tools=tools,
                         runtime_tools=tools,
                         broker_health=broker_health)


# ============================================================================
# TOOL ROUTES
# ============================================================================

@app.route('/html/dc_html', methods=['GET', 'POST'])
@login_required
def tool_dc_html():
    """DC HTML Report Tool"""
    if request.method == 'POST':
        return submit_tool_job('dc_html')
    
    recent_jobs = get_user_tool_jobs('dc_html')
    return render_template('tools/dc_html.html', 
                         tool_name='DC HTML Report',
                         recent_jobs=recent_jobs)


@app.route('/html/interactive_plot', methods=['GET', 'POST'])
@login_required
def tool_interactive_plot():
    """Interactive Plot Tool"""
    if request.method == 'POST':
        return submit_tool_job('interactive_plot')
    
    recent_jobs = get_user_tool_jobs('interactive_plot')
    return render_template('tools/interactive_plot.html',
                         tool_name='Interactive Plot',
                         recent_jobs=recent_jobs)


@app.route('/html/kpi', methods=['GET', 'POST'])
@login_required
def tool_kpi():
    """KPI Analysis Tool"""
    if request.method == 'POST':
        return submit_runtime_kpi_job()
    
    recent_jobs = get_user_tool_jobs('kpi')
    return render_template('tools/kpi.html',
                         tool_name='KPI Analysis',
                         recent_jobs=recent_jobs,
                         runtime_tools=[tool for tool in runtime_store.list_tools() if tool['tool_key'] in {'can_kpi', 'udp_kpi', 'interactive_plot'}],
                         defaults=RUNTIME_TOOL_DEFAULTS,
                         allow_local_scheduler=_allow_local_kpi_scheduler())


@app.route('/html/hyperlink_tool', methods=['GET', 'POST'])
@login_required
def tool_hyperlink():
    """Hyperlink Tool - redirects to integrated viewer"""
    if request.method == 'POST':
        session['hyperlink_output_root'] = (request.form.get('output_root') or '').strip()
        session['hyperlink_html_root'] = (request.form.get('input_path') or '').strip()
        session['hyperlink_video_root'] = (request.form.get('video_path') or '').strip()
        session['hyperlink_ai_enabled'] = bool(request.form.get('hyperlink_ai_enabled'))

        job = JobHistory(
            user_id=current_user.id,
            tool_name='hyperlink_tool',
            input_path=session.get('hyperlink_html_root', ''),
            input_filename=extract_hdf_filename(session.get('hyperlink_html_root', '')),
            output_path=session.get('hyperlink_output_root', ''),
            parameters={
                'html_root': session.get('hyperlink_html_root', ''),
                'video_root': session.get('hyperlink_video_root', ''),
                'hyperlink_ai_enabled': _hyperlink_ai_enabled(),
            },
            status='COMPLETED',
            started_at=datetime.utcnow(),
            completed_at=datetime.utcnow(),
        )
        db.session.add(job)
        db.session.commit()
        return redirect(url_for('hyperlink_viewer_app'))
    
    recent_jobs = get_user_tool_jobs('hyperlink_tool')
    return render_template('tools/hyperlink_tool.html',
                         tool_name='Hyperlink Tool',
                         recent_jobs=recent_jobs,
                         hyperlink_ai_enabled=_hyperlink_ai_enabled())


@app.route('/html/hyperlink_viewer')
@login_required
def hyperlink_viewer():
    """Launch the Hyperlink Tool viewer (html_online)"""
    return redirect(url_for('hyperlink_viewer_app'))


@app.route('/html/runtime-map')
@login_required
def runtime_map():
    """Runtime SIMG mapping and launch topology."""
    return render_template(
        'runtime_map.html',
        runtime_graph=runtime_store.graph_payload(),
        broker_defaults=RUNTIME_TOOL_DEFAULTS,
    )


# ============================================================================
# HYPERLINK VIEWER - INTEGRATED HTML_ONLINE APP
# ============================================================================

# Setup cache directory for viewer data - now in simg/.cache_html
VIEWER_CACHE_DIR = get_cache_dir()
VIEWER_HTML_DIR = os.path.join(VIEWER_CACHE_DIR, 'html')
VIEWER_VIDEO_DIR = os.path.join(VIEWER_CACHE_DIR, 'video')
os.makedirs(VIEWER_HTML_DIR, exist_ok=True)
os.makedirs(VIEWER_VIDEO_DIR, exist_ok=True)

# In-memory cluster sessions for the Hyperlink viewer (keyed by session user key - net_id)
_HYPERLINK_SSH_LOCK = threading.Lock()
_HYPERLINK_SSH_SESSIONS = {}  # {net_id: session_dict}
_HYPERLINK_PROGRESS = {}  # {net_id: progress_dict}
_HYPERLINK_MAPPING_CACHE = {}  # {user_key: cache_entry}
_HYPERLINK_LAST_REAP_AT = 0.0
HYPERLINK_SESSION_TTL_SECONDS = int((os.environ.get('HPCC_HYPERLINK_SESSION_TTL_SECONDS') or '1800').strip() or '1800')
HYPERLINK_MAPPING_CACHE_TTL_SECONDS = int((os.environ.get('HPCC_HYPERLINK_MAPPING_CACHE_TTL_SECONDS') or '15').strip() or '15')


def _hyperlink_get_user_key():
    """Get the user key from session (net_id stored during connect)."""
    return session.get('hyperlink_net_id', '')


def _hyperlink_ai_enabled() -> bool:
    value = session.get('hyperlink_ai_enabled')
    if value is None:
        default_value = (os.environ.get('HPCC_HYPERLINK_AI_ENABLED') or '1').strip().lower()
        return default_value in {'1', 'true', 'yes', 'y'}
    if isinstance(value, bool):
        return value
    return str(value).strip().lower() in {'1', 'true', 'yes', 'y'}


def _hyperlink_resolve_output_root(raw: str) -> str:
    value = (raw or '').strip()
    if not value:
        return VIEWER_CACHE_DIR

    # Keep Unix-style absolute paths intact even on Windows.
    if value.startswith('/'):
        return value
    return os.path.abspath(value)


def _hyperlink_get_user_dirs():
    output_root = _hyperlink_resolve_output_root(session.get('hyperlink_output_root', ''))
    # Use net_id for per-user isolation
    net_id = _hyperlink_get_user_key()
    if net_id:
        user_cache = os.path.join(output_root, 'users', net_id)
        html_dir = os.path.join(user_cache, 'html')
        video_dir = os.path.join(user_cache, 'video')
    else:
        html_dir = os.path.join(output_root, 'html')
        video_dir = os.path.join(output_root, 'video')
    os.makedirs(html_dir, exist_ok=True)
    os.makedirs(video_dir, exist_ok=True)
    return output_root, html_dir, video_dir


def _hyperlink_rewrite_data_url(url: str) -> str:
    if not url:
        return url
    if url.startswith('/hyperlink/data/'):
        return url
    if url.startswith('/data/html/'):
        return '/hyperlink/data/html/' + url[len('/data/html/'):]
    if url.startswith('/data/video/'):
        return '/hyperlink/data/video/' + url[len('/data/video/'):]
    return url


def _hyperlink_resolve_video_abs_path(file_path: str) -> str:
    if file_path.startswith('/data/video/') or file_path.startswith('/hyperlink/data/video/'):
        rel = file_path.split('/data/video/')[-1].replace('\\', '/')
        _, _, video_dir = _hyperlink_get_user_dirs()
        abs_path = os.path.abspath(os.path.join(video_dir, rel))
        root_abs = os.path.abspath(video_dir)
        if os.path.commonpath([abs_path, root_abs]) != root_abs:
            raise ValueError('Invalid video path')
        return abs_path
    return os.path.abspath(file_path)


def _hyperlink_key_from_name(name: str):
    base = os.path.splitext(os.path.basename(name or ''))[0]
    if base.endswith('_web'):
        base = base[:-4]
    parts = base.split('_')
    if len(parts) >= 2:
        return '_'.join(parts[-2:])
    return None


def _hyperlink_cluster_host(cluster_name: str):
    name = (cluster_name or '').strip().lower()
    if name == 'krakow':
        return '10.214.45.45'
    if name == 'southfield':
        return '10.192.224.131'
    return None


def _hyperlink_close_session(user_key: str) -> None:
    sess = _HYPERLINK_SSH_SESSIONS.pop(user_key, None)
    _HYPERLINK_PROGRESS.pop(user_key, None)
    _HYPERLINK_MAPPING_CACHE.pop(user_key, None)
    if not sess:
        return
    try:
        if sess.get('sftp') is not None:
            sess['sftp'].close()
    except Exception:
        pass
    try:
        if sess.get('ssh') is not None:
            sess['ssh'].close()
    except Exception:
        pass


def _hyperlink_reap_stale_sessions(force: bool = False) -> None:
    global _HYPERLINK_LAST_REAP_AT
    now = time.time()
    if not force and now - _HYPERLINK_LAST_REAP_AT < 60:
        return
    _HYPERLINK_LAST_REAP_AT = now
    cutoff = now - HYPERLINK_SESSION_TTL_SECONDS

    with _HYPERLINK_SSH_LOCK:
        stale_keys = [
            user_key
            for user_key, sess in _HYPERLINK_SSH_SESSIONS.items()
            if force or float(sess.get('last_used_at', 0.0) or 0.0) < cutoff
        ]
        for user_key in stale_keys:
            _hyperlink_close_session(user_key)


def _hyperlink_get_live_session(user_key: str):
    if not user_key:
        return None
    _hyperlink_reap_stale_sessions()
    with _HYPERLINK_SSH_LOCK:
        sess = _HYPERLINK_SSH_SESSIONS.get(user_key)
        if sess:
            sess['last_used_at'] = time.time()
        return sess


def _hyperlink_cache_user_key() -> str:
    user_key = _hyperlink_get_user_key()
    if user_key:
        return user_key
    if current_user.is_authenticated:
        return f'user:{current_user.id}'
    return 'anonymous'


def _hyperlink_directory_signature(path: str) -> tuple[int, int]:
    try:
        stats = os.stat(path)
        return int(stats.st_mtime), int(stats.st_size)
    except OSError:
        return (0, 0)


def _hyperlink_cached_mapping(user_key: str, html_dir: str, video_dir: str):
    entry = _HYPERLINK_MAPPING_CACHE.get(user_key)
    if not entry:
        return None
    signature = (_hyperlink_directory_signature(html_dir), _hyperlink_directory_signature(video_dir))
    if entry.get('signature') != signature:
        return None
    if time.time() - float(entry.get('created_at', 0.0)) > HYPERLINK_MAPPING_CACHE_TTL_SECONDS:
        return None
    return entry.get('mapping')


def _hyperlink_store_mapping(user_key: str, html_dir: str, video_dir: str, mapping: dict) -> None:
    _HYPERLINK_MAPPING_CACHE[user_key] = {
        'created_at': time.time(),
        'signature': (_hyperlink_directory_signature(html_dir), _hyperlink_directory_signature(video_dir)),
        'mapping': mapping,
    }

# Get current environment info
_ENV = get_env()
logger.info(f"Running on: {_ENV.name}, Cache: {VIEWER_CACHE_DIR}")


@app.route('/hyperlink/')
@app.route('/hyperlink/<path:subpath>')
def hyperlink_viewer_app(subpath=''):
    """
    Serve the Hyperlink Tool html_online app.
    This integrates the viewer directly into the all_services platform.
    """
    # Base path for hyperlink tool
    hyperlink_base = _hyperlink_tool_base()
    static_dir = os.path.join(hyperlink_base, 'static')
    
    if subpath == '' or subpath == 'index.html':
        # Serve the main viewer
        viewer_path = os.path.join(static_dir, 'viewer.html')
        if os.path.exists(viewer_path):
            return send_from_directory(static_dir, 'viewer.html', conditional=True, max_age=300)
        return f"Viewer not found at {viewer_path}", 404
    
    # Serve static files (css/js)
    static_path = os.path.join(static_dir, subpath)
    if os.path.exists(static_path):
        return send_from_directory(static_dir, subpath, conditional=True, max_age=3600)
    
    return "File not found", 404


@app.route('/hyperlink/static/<path:filename>')
def hyperlink_static(filename):
    """Serve static assets for hyperlink viewer"""
    hyperlink_base = _hyperlink_tool_base()
    static_dir = os.path.join(hyperlink_base, 'static')
    return send_from_directory(static_dir, filename, conditional=True, max_age=3600)


# Also serve /static/ paths for the hyperlink viewer (viewer.html uses absolute /static/ paths)
@app.route('/static/css/<path:filename>')
def hyperlink_css(filename):
    """Serve CSS for hyperlink viewer (absolute path fallback)"""
    hyperlink_base = _hyperlink_tool_base()
    css_dir = os.path.join(hyperlink_base, 'static', 'css')
    if os.path.exists(os.path.join(css_dir, filename)):
        return send_from_directory(css_dir, filename, conditional=True, max_age=3600)
    # Fallback to main static
    return send_from_directory(os.path.join(os.path.dirname(__file__), 'static', 'css'), filename, conditional=True, max_age=3600)


@app.route('/static/js/<path:filename>')
def hyperlink_js(filename):
    """Serve JS for hyperlink viewer (absolute path fallback)"""
    hyperlink_base = _hyperlink_tool_base()
    js_dir = os.path.join(hyperlink_base, 'static', 'js')
    if os.path.exists(os.path.join(js_dir, filename)):
        return send_from_directory(js_dir, filename, conditional=True, max_age=3600)
    # Fallback to main static
    return send_from_directory(os.path.join(os.path.dirname(__file__), 'static', 'js'), filename, conditional=True, max_age=3600)


@app.route('/hyperlink/data/html/<path:filename>')
def hyperlink_html_data(filename):
    """Serve HTML data files for viewer"""
    _, html_dir, _ = _hyperlink_get_user_dirs()
    return send_from_directory(html_dir, filename, conditional=True, max_age=3600)


def _stream_video_file(file_path: str):
    """Stream video file with proper range request support for large files.
    
    This avoids worker timeouts by:
    1. Using generators for chunked streaming
    2. Proper handling of HTTP Range requests for video seeking
    3. Not loading entire file into memory
    4. Graceful error handling to prevent worker crashes
    """
    import mimetypes
    from flask import Response
    
    file_size = os.path.getsize(file_path)
    mime_type = mimetypes.guess_type(file_path)[0] or 'video/mp4'
    
    # Parse Range header
    range_header = request.headers.get('Range')
    
    if range_header:
        # Handle Range request (e.g., "bytes=0-1023")
        try:
            ranges = range_header.replace('bytes=', '').split('-')
            start = int(ranges[0]) if ranges[0] else 0
            end = int(ranges[1]) if ranges[1] else file_size - 1
        except (ValueError, IndexError):
            start, end = 0, file_size - 1
        
        # Clamp values
        start = max(0, min(start, file_size - 1))
        end = max(start, min(end, file_size - 1))
        length = end - start + 1
        
        def generate():
            chunk_size = 256 * 1024
            try:
                with open(file_path, 'rb') as f:
                    f.seek(start)
                    remaining = length
                    while remaining > 0:
                        chunk = f.read(min(chunk_size, remaining))
                        if not chunk:
                            break
                        remaining -= len(chunk)
                        yield chunk
            except (IOError, OSError, GeneratorExit) as e:
                # Client disconnected or file error - log and exit gracefully
                logger.debug(f"Video stream ended: {e}")
                return
        
        response = Response(
            generate(),
            status=206,
            mimetype=mime_type,
            direct_passthrough=True
        )
        response.headers['Content-Range'] = f'bytes {start}-{end}/{file_size}'
        response.headers['Content-Length'] = length
        response.headers['Accept-Ranges'] = 'bytes'
        response.headers['Cache-Control'] = 'public, max-age=86400'
        return response
    else:
        # Full file request - still stream to avoid memory issues
        def generate():
            chunk_size = 256 * 1024
            try:
                with open(file_path, 'rb') as f:
                    while True:
                        chunk = f.read(chunk_size)
                        if not chunk:
                            break
                        yield chunk
            except (IOError, OSError, GeneratorExit) as e:
                # Client disconnected or file error - log and exit gracefully
                logger.debug(f"Video stream ended: {e}")
                return
        
        response = Response(
            generate(),
            status=200,
            mimetype=mime_type,
            direct_passthrough=True
        )
        response.headers['Content-Length'] = file_size
        response.headers['Accept-Ranges'] = 'bytes'
        response.headers['Cache-Control'] = 'public, max-age=86400'
        return response


@app.route('/hyperlink/data/video/<path:filename>')
def hyperlink_video_data(filename):
    """Serve video data files for viewer with streaming support"""
    try:
        _, _, video_dir = _hyperlink_get_user_dirs()
        file_path = os.path.join(video_dir, filename)
        if not os.path.exists(file_path):
            logger.error(f"Video file not found: {file_path}")
            return "File not found", 404
        return _stream_video_file(file_path)
    except Exception as e:
        logger.error(f"Error serving video {filename}: {e}")
        return "Internal server error", 500


@app.route('/hyperlink/api/mappings')
def hyperlink_mappings():
    """API endpoint for viewer data mappings"""
    try:
        LogViewerApp = _load_hyperlink_log_viewer_app()

        _, html_dir, video_dir = _hyperlink_get_user_dirs()
        if os.path.isdir(html_dir) and os.path.isdir(video_dir):
            cache_key = _hyperlink_cache_user_key()
            cached_mapping = _hyperlink_cached_mapping(cache_key, html_dir, video_dir)
            if cached_mapping is not None:
                return jsonify({'success': True, 'mappings': cached_mapping})

            viewer_app = LogViewerApp(html_dir, video_dir)
            mapping = viewer_app.build_mapping() or {}
            # Rewrite /data/* URLs to platform-mounted /hyperlink/data/*
            for _k, entry in (mapping or {}).items():
                if not isinstance(entry, dict):
                    continue
                entry['video'] = _hyperlink_rewrite_data_url(entry.get('video', ''))
                entry['comment_path'] = _hyperlink_rewrite_data_url(entry.get('comment_path', ''))
                entry['html_files'] = [_hyperlink_rewrite_data_url(x) for x in (entry.get('html_files') or [])]
                images = entry.get('images') or {}
                if isinstance(images, dict):
                    for sensor, positions in images.items():
                        if not isinstance(positions, dict):
                            continue
                        for pos, img_path in list(positions.items()):
                            positions[pos] = _hyperlink_rewrite_data_url(img_path)
            _hyperlink_store_mapping(cache_key, html_dir, video_dir, mapping)
            return jsonify({'success': True, 'mappings': mapping})
    except ImportError as e:
        logger.warning(f"Could not import LogViewerApp: {e}")
    
    return jsonify({'success': True, 'mappings': {}})


@app.route('/hyperlink/api/save-comment', methods=['POST'])
def hyperlink_save_comment():
    """Save a comment for a video"""
    try:
        data = request.get_json()
        file_path = data.get('path')
        content = data.get('content', '')
        
        if not file_path:
            return jsonify({'success': False, 'error': 'No path provided'}), 400
        
        # Resolve path
        if file_path.startswith('/data/video/') or file_path.startswith('/hyperlink/data/video/'):
            rel = file_path.split('/data/video/')[-1]
            _, _, video_dir = _hyperlink_get_user_dirs()
            abs_path = os.path.join(video_dir, rel)
        else:
            abs_path = os.path.abspath(file_path)
        
        os.makedirs(os.path.dirname(abs_path), exist_ok=True)
        with open(abs_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        return jsonify({'success': True, 'path': abs_path})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)}), 500


@app.route('/hyperlink/api/get-comment', methods=['POST'])
def hyperlink_get_comment():
    """Get a comment for a video (viewer expects this endpoint)."""
    try:
        data = request.get_json() or {}
        file_path = data.get('path')
        if not file_path:
            return jsonify({'success': False, 'error': 'No path provided'}), 400

        if file_path.startswith('/data/video/') or file_path.startswith('/hyperlink/data/video/'):
            rel = file_path.split('/data/video/')[-1]
            _, _, video_dir = _hyperlink_get_user_dirs()
            abs_path = os.path.join(video_dir, rel)
        else:
            abs_path = os.path.abspath(file_path)

        if not os.path.exists(abs_path):
            return jsonify({'success': True, 'content': ''})

        with open(abs_path, 'r', encoding='utf-8') as f:
            content = f.read()
        return jsonify({'success': True, 'content': content})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)}), 500


@app.route('/hyperlink/api/vlm/process', methods=['POST'])
def hyperlink_vlm_process():
    """Generate or refresh a text description for one Hyperlink video."""
    try:
        if not _hyperlink_ai_enabled():
            return jsonify({
                'success': False,
                'status': 'disabled',
                'error': 'AI video description is disabled for this Hyperlink session.',
            }), 403

        data = request.get_json() or {}
        force = bool(data.get('force', False))
        video_path = (data.get('video_path') or data.get('path') or '').strip()
        _, _, video_dir = _hyperlink_get_user_dirs()

        vlm_module = _load_hyperlink_vlm_module()
        process_video_with_vlm = getattr(vlm_module, 'process_video_with_vlm', None)
        process_videos_with_vlm = getattr(vlm_module, 'process_videos_with_vlm', None)
        if process_video_with_vlm is None or process_videos_with_vlm is None:
            raise ImportError('Hyperlink VLM module is missing required entrypoints')

        if video_path:
            abs_video_path = _hyperlink_resolve_video_abs_path(video_path)
            result = process_video_with_vlm(abs_video_path, video_dir, force=force)
            return jsonify(result), (200 if result.get('success') else 500)

        processed, skipped, errors = process_videos_with_vlm(video_dir, force=force)
        return jsonify({
            'success': errors == 0,
            'status': 'processed',
            'processed': processed,
            'skipped': skipped,
            'errors': errors,
        }), (200 if errors == 0 else 500)
    except Exception as e:
        return jsonify({'success': False, 'status': 'error', 'error': str(e)}), 500


@app.route('/hyperlink/api/cluster/servers', methods=['GET'])
def hyperlink_get_servers():
    """Get available cluster servers"""
    import socket
    hostname = socket.gethostname()
    local_ip = socket.gethostbyname(hostname) if hostname else ''
    cluster_info = detect_cluster(local_ip, hostname)
    
    servers = {
        'Krakow': '10.214.45.45',
        'Southfield': '10.192.224.131'
    }
    
    return jsonify({
        'servers': servers,
        'cluster_info': cluster_info
    })


@app.route('/hyperlink/api/cluster/status', methods=['GET'])
def hyperlink_cluster_status():
    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    connected = bool(sess and sess.get('ssh'))
    server = sess.get('server') if sess else None
    return jsonify({
        'connected': connected,
        'server': server,
        'net_id': current_user.net_id if current_user.is_authenticated else (user_key or ''),
        'html_path': session.get('hyperlink_remote_html_path'),
        'video_path': session.get('hyperlink_remote_video_path'),
        'output_root': session.get('hyperlink_output_root') or VIEWER_CACHE_DIR,
        'vlm_enabled': _hyperlink_ai_enabled(),
    })


@app.route('/hyperlink/api/vlm/status', methods=['GET'])
def hyperlink_vlm_status():
    model_dir = (
        os.environ.get('HPCC_HYPERLINK_VLM_MODEL_DIR', '').strip()
        or os.environ.get('HYPERLINK_VLM_MODEL_DIR', '').strip()
    )
    return jsonify({
        'enabled': _hyperlink_ai_enabled(),
        'model_dir': model_dir,
        'net_id': current_user.net_id if current_user.is_authenticated else '',
    })


@app.route('/hyperlink/api/cluster/connect', methods=['POST'])
def hyperlink_cluster_connect():
    """Connect to a cluster using the provided NetID and password.

    Cluster is inferred from the provided remote paths:
    - /net... => Krakow
    - /mnt... => Southfield
    """
    data = request.get_json() or {}
    html_path = (data.get('html_path') or '').strip()
    video_path = (data.get('video_path') or '').strip()
    output_path = (data.get('output_path') or '').strip()
    net_id = (data.get('net_id') or '').strip()
    password = (data.get('password') or '').strip()

    if not html_path or not video_path:
        return jsonify({'success': False, 'error': 'Both remote HTML and Video paths are required'}), 400
    if not net_id:
        return jsonify({'success': False, 'error': 'Net ID is required'}), 400
    if not password:
        return jsonify({'success': False, 'error': 'Password required'}), 400

    c1 = cluster_from_path(html_path)
    c2 = cluster_from_path(video_path)
    cluster_name = c1 or c2
    if not cluster_name or (c1 and c2 and c1 != c2):
        return jsonify({'success': False, 'error': 'Paths must both start with /net (Krakow) or /mnt (Southfield)'}), 400

    host = _hyperlink_cluster_host(cluster_name)
    if not host:
        return jsonify({'success': False, 'error': f'Unsupported cluster: {cluster_name}'}), 400

    username = net_id

    try:
        import paramiko

        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(
            hostname=host,
            username=username,
            password=password,
            timeout=20,
            auth_timeout=20,
            banner_timeout=20,
            allow_agent=False,
            look_for_keys=False,
        )
        sftp = client.open_sftp()

        # Store net_id in session for user identification
        session['hyperlink_net_id'] = net_id
        user_key = net_id
        
        with _HYPERLINK_SSH_LOCK:
            _hyperlink_close_session(user_key)
            _HYPERLINK_SSH_SESSIONS[user_key] = {
                'server': cluster_name,
                'host': host,
                'username': username,
                'last_used_at': time.time(),
                'ssh': client,
                'sftp': sftp,
            }

        # Persist last-used values in the browser session
        session['hyperlink_remote_html_path'] = html_path
        session['hyperlink_remote_video_path'] = video_path
        if output_path:
            session['hyperlink_output_root'] = _hyperlink_resolve_output_root(output_path)

        return jsonify({'success': True, 'server': cluster_name})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)}), 500


@app.route('/hyperlink/api/cluster/disconnect', methods=['POST'])
def hyperlink_cluster_disconnect():
    user_key = _hyperlink_get_user_key()
    if user_key:
        with _HYPERLINK_SSH_LOCK:
            _hyperlink_close_session(user_key)
    session.pop('hyperlink_net_id', None)
    return jsonify({'success': True})


def _hyperlink_sftp_list_dir(sftp, path: str):
    import stat

    items = []
    for attr in sftp.listdir_attr(path):
        items.append({
            'name': attr.filename,
            'is_dir': stat.S_ISDIR(attr.st_mode),
            'size': int(getattr(attr, 'st_size', 0) or 0),
        })
    return items


def _hyperlink_sftp_download_dir(sftp, remote_dir: str, local_dir: str) -> None:
    import stat

    os.makedirs(local_dir, exist_ok=True)
    for attr in sftp.listdir_attr(remote_dir):
        r = remote_dir.rstrip('/') + '/' + attr.filename
        local_path = os.path.join(local_dir, attr.filename)
        if stat.S_ISDIR(attr.st_mode):
            _hyperlink_sftp_download_dir(sftp, r, local_path)
        else:
            os.makedirs(os.path.dirname(local_path), exist_ok=True)
            sftp.get(r, local_path)


@app.route('/hyperlink/api/cluster/scan-logs', methods=['POST'])
def hyperlink_cluster_scan_logs():
    data = request.get_json() or {}
    html_path = (data.get('html_path') or '').strip()
    video_path = (data.get('video_path') or '').strip()
    output_path = (data.get('output_path') or '').strip()

    if not html_path or not video_path:
        return jsonify({'success': False, 'error': 'Both remote HTML and Video paths are required'}), 400

    if output_path:
        session['hyperlink_output_root'] = _hyperlink_resolve_output_root(output_path)
    session['hyperlink_remote_html_path'] = html_path
    session['hyperlink_remote_video_path'] = video_path

    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    sftp = sess.get('sftp') if sess else None

    if sftp is None:
        return jsonify({'success': False, 'error': 'Not connected'}), 400

    try:
        html_items = _hyperlink_sftp_list_dir(sftp, html_path)
        video_items = _hyperlink_sftp_list_dir(sftp, video_path)

        html_folders = {item['name']: item for item in html_items if item.get('is_dir')}
        video_files = {}
        for item in video_items:
            if item.get('is_dir'):
                continue
            key = _hyperlink_key_from_name(item.get('name'))
            if key:
                video_files[key] = item.get('name')

        output_root, html_dir, video_dir = _hyperlink_get_user_dirs()
        logs = {}
        for folder_name in html_folders:
            key = _hyperlink_key_from_name(folder_name)
            if not key:
                continue
            if key not in video_files:
                continue

            cached_html = os.path.join(html_dir, folder_name)
            cached_video = os.path.join(video_dir, video_files[key])
            is_cached = os.path.isdir(cached_html) and os.path.isfile(cached_video)

            logs[key] = {
                'name': folder_name,
                'html_path': f"{html_path.rstrip('/')}/{folder_name}",
                'video_path': f"{video_path.rstrip('/')}/{video_files[key]}",
                'video_name': video_files[key],
                'cached': is_cached,
                'output_root': output_root,
            }

        return jsonify({'success': True, 'logs': logs})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)}), 500


@app.route('/hyperlink/api/cluster/download-log', methods=['POST'])
def hyperlink_cluster_download_log():
    data = request.get_json() or {}
    key = (data.get('key') or '').strip()
    html_path = (data.get('html_path') or '').strip()
    video_path = (data.get('video_path') or '').strip()
    output_path = (data.get('output_path') or '').strip()

    if not key or not html_path or not video_path:
        return jsonify({'success': False, 'error': 'Missing required fields'}), 400

    if output_path:
        session['hyperlink_output_root'] = _hyperlink_resolve_output_root(output_path)

    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    sftp = sess.get('sftp') if sess else None

    if sftp is None:
        return jsonify({'success': False, 'error': 'Not connected'}), 400

    try:
        output_root, html_dir, video_dir = _hyperlink_get_user_dirs()
        folder_name = os.path.basename(html_path.rstrip('/'))
        local_html_folder = os.path.join(html_dir, folder_name)
        local_video_file = os.path.join(video_dir, os.path.basename(video_path))
        os.makedirs(os.path.dirname(local_video_file), exist_ok=True)

        # Progress (best-effort) - keyed by user_key
        _HYPERLINK_PROGRESS[user_key] = {'active': True, 'phase': 'downloading', 'message': 'Downloading...'}

        _hyperlink_sftp_download_dir(sftp, html_path, local_html_folder)
        sftp.get(video_path, local_video_file)

        _HYPERLINK_PROGRESS[user_key] = {'active': False, 'phase': 'complete', 'message': 'Done'}
        _HYPERLINK_MAPPING_CACHE.pop(_hyperlink_cache_user_key(), None)

        return jsonify({'success': True, 'message': 'Downloaded', 'key': key, 'output_root': output_root})
    except Exception as e:
        if user_key:
            _HYPERLINK_PROGRESS[user_key] = {'active': False, 'phase': 'error', 'message': str(e)}
        return jsonify({'success': False, 'error': str(e)}), 500


@app.route('/hyperlink/api/cluster/progress', methods=['GET'])
def hyperlink_cluster_progress():
    user_key = _hyperlink_get_user_key()
    return jsonify(_HYPERLINK_PROGRESS.get(user_key, {'active': False}) if user_key else {'active': False})


@app.route('/hyperlink/api/local/upload', methods=['POST'])
def hyperlink_local_upload():
    """Upload local PC HTML/video into the per-user output root and activate it for viewing."""
    try:
        output_path = (request.form.get('output_path') or '').strip()
        if output_path:
            session['hyperlink_output_root'] = _hyperlink_resolve_output_root(output_path)
        output_root, html_dir, video_dir = _hyperlink_get_user_dirs()

        html_files = request.files.getlist('html_files')
        video_files = request.files.getlist('video_files')
        if not html_files and not video_files:
            return jsonify({'success': False, 'error': 'No files uploaded'}), 400

        def _safe_join(root: str, rel_path: str) -> str:
            rel_path = (rel_path or '').replace('\\', '/').lstrip('/')
            dest = os.path.abspath(os.path.join(root, rel_path))
            root_abs = os.path.abspath(root)
            if os.path.commonpath([dest, root_abs]) != root_abs:
                raise ValueError('Invalid upload path')
            return dest

        saved_html = 0
        for f in html_files:
            rel = f.filename or ''
            if not rel:
                continue
            dest = _safe_join(html_dir, rel)
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            f.save(dest)
            saved_html += 1

        saved_video = 0
        for f in video_files:
            name = os.path.basename((f.filename or '').replace('\\', '/'))
            if not name:
                continue
            dest = os.path.join(video_dir, name)
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            f.save(dest)
            saved_video += 1

        _HYPERLINK_MAPPING_CACHE.pop(_hyperlink_cache_user_key(), None)

        return jsonify({
            'success': True,
            'message': 'Upload complete',
            'output_root': output_root,
            'html_root': html_dir,
            'video_root': video_dir,
            'saved_html': saved_html,
            'saved_video': saved_video,
        })
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)}), 500


def detect_cluster(ip: str, hostname: str) -> dict:
    """
    Detect which cluster the app is running on.
    Returns dict with cluster info for frontend use.
    """
    cluster_info = {
        'name': 'local',
        'is_cluster': False,
        'krakow': False,
        'southfield': False,
        'needs_upload': True,
        'server_ip': ip
    }
    
    # Krakow cluster detection (10.214.x.x range)
    if ip.startswith('10.214.') or 'krakow' in hostname.lower():
        cluster_info['name'] = 'krakow'
        cluster_info['is_cluster'] = True
        cluster_info['krakow'] = True
        cluster_info['needs_upload'] = False
    
    # Southfield cluster detection (10.192.x.x range)
    elif ip.startswith('10.192.') or 'southfield' in hostname.lower():
        cluster_info['name'] = 'southfield'
        cluster_info['is_cluster'] = True
        cluster_info['southfield'] = True
        cluster_info['needs_upload'] = False
    
    return cluster_info


@app.route('/api/cluster-info')
@login_required
def get_cluster_info():
    """API endpoint to get current cluster info"""
    import socket
    hostname = socket.gethostname()
    local_ip = socket.gethostbyname(hostname) if hostname else ''
    cluster_info = detect_cluster(local_ip, hostname)
    cluster_info['user'] = current_user.net_id
    return jsonify(cluster_info)


def get_user_tool_jobs(tool_name: str, limit: int = 20):
    """Get recent jobs for a specific tool"""
    jobs = JobHistory.query.filter_by(
        user_id=current_user.id,
        tool_name=tool_name
    ).order_by(JobHistory.created_at.desc()).limit(limit).all()
    for job in jobs:
        _sync_runtime_job(job)
    return jobs


def _default_runtime_resources(tool_key: str) -> dict:
    values = dict(RUNTIME_TOOL_DEFAULTS.get(tool_key, {}))
    values['scheduler'] = 'slurm'
    return values


def _runtime_console_from_runtime_job(runtime_job):
    if not runtime_job:
        return {}
    request_payload = runtime_job.get('request') or {}
    if not isinstance(request_payload, dict):
        return {}
    console = request_payload.get('_console') or {}
    if not isinstance(console, dict):
        return {}
    return dict(console)


def _resolve_job_log_path(job: JobHistory, runtime_job=None):
    console = {}
    if isinstance(job.parameters, dict):
        stored_console = job.parameters.get('runtime_console') or {}
        if isinstance(stored_console, dict):
            console.update(stored_console)

    runtime_console = _runtime_console_from_runtime_job(runtime_job)
    if runtime_console:
        console.update(runtime_console)

    log_path = (console.get('log_path') or job.output_log_path or '').strip()
    if not log_path and job.output_path:
        log_path = os.path.join(job.output_path, f'local_{job.tool_name}_{job.id}.log')
    if not log_path and job.slurm_job_id:
        job_name = f"{job.tool_name}_{job.id}"
        log_path = f"/scratch/logs/slurm_{job_name}_{job.slurm_job_id}.out"
    return log_path, console


def _read_log_tail_text(log_path: str, n_lines: int) -> str:
    if not log_path or not os.path.exists(log_path):
        return ''
    try:
        with open(log_path, 'r', encoding='utf-8', errors='replace') as fp:
            lines = fp.readlines()
    except Exception as exc:
        return f'Failed to read log: {exc}'
    return ''.join(lines[-n_lines:])


def _sync_runtime_job(job: JobHistory):
    runtime_job_id = ((job.parameters or {}).get('runtime_job_id') if job.parameters else None)
    if not runtime_job_id:
        return None

    runtime_job = None
    try:
        runtime_job = broker_client.get_status(int(runtime_job_id)).get('job')
    except Exception:
        runtime_job = runtime_store.get_job(int(runtime_job_id))
    if not runtime_job:
        return None

    runtime_console = _runtime_console_from_runtime_job(runtime_job)
    job.status = runtime_job.get('status', job.status)
    job.output_log_path = runtime_console.get('log_path') or runtime_job.get('log_path') or job.output_log_path
    job.output_path = runtime_job.get('output_path') or job.output_path
    job.error_message = runtime_job.get('error_message') or job.error_message
    if runtime_console:
        parameters = dict(job.parameters or {})
        parameters['runtime_console'] = runtime_console
        job.parameters = parameters
    if runtime_job.get('started_at') and not job.started_at:
        job.started_at = datetime.fromisoformat(runtime_job['started_at'].replace('Z', ''))
    if runtime_job.get('completed_at'):
        job.completed_at = datetime.fromisoformat(runtime_job['completed_at'].replace('Z', ''))
    db.session.commit()
    return runtime_job


def _submit_runtime_job(tool_key: str, mode: str, paths: dict, resources: dict, job_tool_name: str, input_path: str, execution_label: str = ''):
    payload = {
        'tool_key': tool_key,
        'variant': tool_key,
        'mode': mode,
        'paths': paths,
        'resources': resources,
        'session_id': session.get('_id', ''),
        'user': current_user.net_id,
    }
    result = broker_client.submit_job(payload)
    resolved_execution_label = execution_label or tool_key

    job = JobHistory(
        user_id=current_user.id,
        tool_name=job_tool_name,
        input_path=input_path,
        input_filename=extract_hdf_filename(input_path),
        output_path=result.get('output_path', ''),
        output_log_path=result.get('log_path', ''),
        parameters={
            'runtime_job_id': result['job_id'],
            'execution_target': tool_key,
            'execution_label': resolved_execution_label,
            'mode': mode,
            'paths': paths,
            'resources': resources,
            'runtime_console': result.get('console', {}),
        },
        status='QUEUED',
    )
    db.session.add(job)
    db.session.commit()
    flash(f'{resolved_execution_label} request queued through HPCC broker. Runtime job: {result["job_id"]}', 'success')
    return redirect(url_for('dashboard'))


def submit_runtime_kpi_job():
    primary_target = (request.form.get('execution_target') or 'udp_kpi').strip()
    if primary_target not in {'can_kpi', 'udp_kpi'}:
        primary_target = 'udp_kpi'
    interactive_plot_mode = (request.form.get('interactive_plot_mode') or 'disabled').strip().lower()
    if interactive_plot_mode not in {'disabled', 'enabled', 'only'}:
        interactive_plot_mode = 'disabled'
    interactive_plot_enabled = interactive_plot_mode == 'enabled'
    interactive_plot_only = interactive_plot_mode == 'only'
    execution_target = 'interactive_plot' if interactive_plot_only else primary_target
    input_mode = (request.form.get('input_mode') or 'json').strip().lower()
    resource_target = 'interactive_plot' if interactive_plot_mode != 'disabled' else execution_target
    default_resources = _default_runtime_resources(resource_target)
    requested_scheduler = (request.form.get('scheduler') or default_resources.get('scheduler') or 'slurm').strip().lower()
    if requested_scheduler not in {'slurm', 'local'}:
        requested_scheduler = 'slurm'
    if requested_scheduler == 'local' and not _allow_local_kpi_scheduler():
        flash('Local KPI execution is disabled on the login node. Use Slurm or the direct tmux launchers from the HPCC bundle.', 'error')
        return redirect(request.referrer or url_for('tool_kpi'))
    resources = {
        'scheduler': requested_scheduler,
        'memory': (request.form.get('memory') or default_resources.get('memory', '32G')).strip(),
        'cpus': int(request.form.get('cpus') or default_resources.get('cpus', 8)),
        'time_limit': (request.form.get('time_limit') or default_resources.get('time_limit', '02:00:00')).strip(),
        'partition': (request.form.get('partition') or default_resources.get('partition', 'plcyf-com')).strip(),
        'account': (request.form.get('account') or default_resources.get('account', app.config.get('SLURM_ACCOUNT', 'RNA-SDV-SRR7'))).strip(),
        'qos': (request.form.get('qos') or default_resources.get('qos', app.config.get('SLURM_QOS', ''))).strip(),
        'nodes': int(request.form.get('nodes') or default_resources.get('nodes', 1)),
        'ntasks': int(request.form.get('ntasks') or default_resources.get('ntasks', 1)),
        'immediate': str(request.form.get('immediate') or default_resources.get('immediate', '')).strip(),
    }
    if resources['scheduler'] != 'slurm':
        resources['immediate'] = ''
    output_dir = (request.form.get('output_dir') or '').strip()
    paths = {
        'input_mode': input_mode,
        'output_dir': output_dir,
        'interactive_plot_mode': interactive_plot_mode,
        'interactive_source_target': primary_target,
    }
    config_xml = (request.form.get('config_xml') or '').strip()
    json_path = (request.form.get('kpi_json') or '').strip()
    input_hdf = (request.form.get('input_hdf') or '').strip()
    output_hdf = (request.form.get('output_hdf') or '').strip()

    if input_mode == 'json' and not json_path:
        flash('JSON path is required for the selected KPI submission mode.', 'error')
        return redirect(request.referrer or url_for('tool_kpi'))
    if input_mode == 'hdf' and (not input_hdf or not output_hdf):
        flash('Input and output HDF paths are required for HDF mode.', 'error')
        return redirect(request.referrer or url_for('tool_kpi'))

    paths.update({
        'config_xml': config_xml,
        'json_path': json_path,
        'input_hdf': input_hdf,
        'output_hdf': output_hdf,
    })
    primary_input = json_path or input_hdf

    optional_config = (request.form.get('optional_config') or '').strip()
    if optional_config:
        paths['optional_config'] = optional_config

    if interactive_plot_only:
        execution_label = 'Interactive Plot only'
    elif interactive_plot_enabled and primary_target == 'can_kpi':
        execution_label = 'CAN KPI + Interactive Plot'
    elif interactive_plot_enabled:
        execution_label = 'UDP KPI + Interactive Plot'
    elif primary_target == 'can_kpi':
        execution_label = 'CAN KPI'
    else:
        execution_label = 'UDP KPI'

    try:
        return _submit_runtime_job(execution_target, input_mode, paths, resources, 'kpi', primary_input, execution_label)
    except Exception as exc:
        logger.exception('Failed to submit runtime KPI job')
        flash(f'HPCC broker submission failed: {exc}', 'error')
        return redirect(request.referrer or url_for('tool_kpi'))


def _container_path_to_host_path(container_path: str) -> str:
    """Convert container path to host path for display to user.
    
    When running inside Singularity, the container sees /app/simg/...
    but the actual files are on the host at HOST_SIMG_PATH/...
    
    Examples:
        /app/simg/html_db/log.txt -> /mnt/c/.../simg/html_db/log.txt
    """
    host_simg_path = os.environ.get('HOST_SIMG_PATH', '')
    if not host_simg_path:
        # Not running in container with HOST_SIMG_PATH set, return as-is
        return container_path
    
    # Replace /app/simg with the actual host path
    if container_path.startswith('/app/simg/'):
        return container_path.replace('/app/simg/', f'{host_simg_path}/', 1)
    elif container_path.startswith('/app/simg'):
        return container_path.replace('/app/simg', host_simg_path, 1)
    
    return container_path


@app.template_filter('host_path')
def host_path_filter(path: str) -> str:
    """Jinja filter: convert container paths to host paths for display."""
    if not path:
        return ''
    return _container_path_to_host_path(path)


def _windows_path_to_wsl_path(path: str) -> str:
    """Convert Windows path to WSL path for use inside container.
    
    When running in Singularity container on WSL, Windows paths like
    C:\\git\\... need to be converted to /mnt/c/git/...
    
    Examples:
        C:\\git\\project\\file.json -> /mnt/c/git/project/file.json
        C:/git/project/file.json -> /mnt/c/git/project/file.json
        /mnt/c/git/file.json -> /mnt/c/git/file.json (unchanged)
    """
    if not path:
        return path
    
    # Already a Linux/WSL path
    if path.startswith('/'):
        return path
    
    # Convert Windows path separators
    path = path.replace('\\', '/')
    
    # Check for drive letter pattern (e.g., C:/ or D:/)
    if len(path) >= 2 and path[1] == ':':
        drive_letter = path[0].lower()
        rest_of_path = path[2:]  # Skip the ":"
        if rest_of_path.startswith('/'):
            rest_of_path = rest_of_path[1:]  # Remove leading /
        return f'/mnt/{drive_letter}/{rest_of_path}'
    
    return path


def _run_local_job_background(job_id: int, cmd, cwd: str, log_path: str, env=None):
    """Run a tool command locally in the background and update JobHistory."""
    def _safe_read_text(path: str, limit: int = 4096) -> str:
        try:
            with open(path, 'r', encoding='utf-8', errors='replace') as fp:
                return fp.read(limit).strip()
        except Exception:
            return ''

    def _write_execution_context(log_fp):
        # High-signal context for debugging: where/what/under which scheduler.
        log_fp.write(f"HOSTNAME: {socket.gethostname()}\n")
        log_fp.write(f"USER: {getpass.getuser()}\n")
        log_fp.write(f"PID: {os.getpid()}\n")
        try:
            log_fp.write(f"PPID: {os.getppid()}\n")
        except Exception:
            pass
        log_fp.write(f"PLATFORM: {platform.platform()}\n")
        log_fp.write(f"PYTHON: {sys.executable}\n")

        # Slurm context (may be empty if not launched under Slurm)
        slurm_keys = [
            'SLURM_JOB_ID',
            'SLURM_JOB_NAME',
            'SLURM_CLUSTER_NAME',
            'SLURM_PARTITION',
            'SLURM_NODELIST',
            'SLURM_JOB_NODELIST',
            'SLURM_CPUS_PER_TASK',
            'SLURM_CPUS_ON_NODE',
            'SLURM_MEM_PER_NODE',
            'SLURM_MEM_PER_CPU',
            'SLURM_SUBMIT_DIR',
        ]
        any_slurm = False
        for k in slurm_keys:
            v = os.environ.get(k)
            if v:
                any_slurm = True
                log_fp.write(f"{k}: {v}\n")
        if not any_slurm:
            log_fp.write("SLURM: not detected in environment\n")

        # cgroup/memory hints (helpful when rc is -9 / SIGKILL due to OOM)
        meminfo = _safe_read_text('/proc/meminfo', limit=2048)
        if meminfo:
            first_lines = '\n'.join(meminfo.splitlines()[:5])
            log_fp.write("/proc/meminfo (first lines):\n" + first_lines + "\n")

        # cgroup v2
        cg_mem_max = _safe_read_text('/sys/fs/cgroup/memory.max')
        cg_mem_cur = _safe_read_text('/sys/fs/cgroup/memory.current')
        if cg_mem_max or cg_mem_cur:
            if cg_mem_max:
                log_fp.write(f"cgroup memory.max: {cg_mem_max}\n")
            if cg_mem_cur:
                log_fp.write(f"cgroup memory.current: {cg_mem_cur}\n")

        log_fp.write("\n")

    def _format_return_code(rc: int) -> str:
        if rc is None:
            return 'None'
        if rc < 0:
            sig = -rc
            try:
                sig_name = signal.Signals(sig).name
            except Exception:
                sig_name = f"SIG{sig}"
            return f"{rc} (terminated by {sig_name})"
        return str(rc)

    try:
        with app.app_context():
            job = JobHistory.query.get(job_id)
            if not job:
                return
            job.started_at = datetime.utcnow()
            job.status = 'RUNNING'
            job.output_log_path = log_path
            db.session.commit()

        os.makedirs(os.path.dirname(log_path), exist_ok=True)
        with open(log_path, 'w', encoding='utf-8', errors='replace') as log_fp:
            log_fp.write('CMD: ' + ' '.join([str(x) for x in cmd]) + '\n')
            log_fp.write('CWD: ' + cwd + '\n')
            log_fp.write('START: ' + datetime.utcnow().isoformat() + 'Z\n\n')
            _write_execution_context(log_fp)
            log_fp.flush()

            proc = subprocess.Popen(
                cmd,
                cwd=cwd,
                stdout=log_fp,
                stderr=subprocess.STDOUT,
                text=True,
                env=env,
                start_new_session=True,
            )
            try:
                log_fp.write(f"CHILD_PID: {proc.pid}\n")
            except Exception:
                pass
            log_fp.flush()
            rc = proc.wait()

            # If the web app itself is running under a Slurm allocation, include a quick sacct snapshot.
            slurm_job_id = os.environ.get('SLURM_JOB_ID')
            if slurm_job_id and shutil.which('sacct'):
                try:
                    sacct = subprocess.run(
                        [
                            'sacct',
                            '-j',
                            slurm_job_id,
                            '--format=JobID,State,ExitCode,Elapsed,AllocCPUS,ReqMem,MaxRSS,MaxVMSize',
                            '--noheader',
                            '--parsable2',
                        ],
                        capture_output=True,
                        text=True,
                        timeout=8,
                    )
                    if sacct.stdout.strip() or sacct.stderr.strip():
                        log_fp.write('\nSLURM sacct (web allocation):\n')
                        if sacct.stdout.strip():
                            log_fp.write(sacct.stdout.strip() + '\n')
                        if sacct.stderr.strip():
                            log_fp.write('sacct stderr: ' + sacct.stderr.strip() + '\n')
                        log_fp.flush()
                except Exception:
                    # sacct is best-effort only
                    pass

            log_fp.write('\n\nEXIT_CODE: ' + _format_return_code(rc) + '\n')
            log_fp.write('END: ' + datetime.utcnow().isoformat() + 'Z\n')
            log_fp.flush()

        with app.app_context():
            job = JobHistory.query.get(job_id)
            if not job:
                return
            job.completed_at = datetime.utcnow()
            if rc == 0:
                job.status = 'COMPLETED'
            else:
                job.status = 'FAILED'
                display_path = _container_path_to_host_path(log_path)
                job.error_message = f'Process exit code {rc}. See log: {display_path}'
            db.session.commit()
    except Exception as exc:
        logger.exception('Local job runner failed')
        try:
            with app.app_context():
                job = JobHistory.query.get(job_id)
                if job:
                    job.completed_at = datetime.utcnow()
                    job.status = 'FAILED'
                    job.error_message = str(exc)
                    db.session.commit()
        except Exception:
            pass


def submit_tool_job(tool_name: str):
    """Submit a tool job to Slurm with support for different input modes"""
    import shlex
    parameters = request.form.to_dict()
    input_mode = parameters.get('input_mode', 'json')
    output_hdf = ''
    config_xml = ''
    
    # Handle different input modes based on tool
    if tool_name == 'interactive_plot':
        if input_mode == 'json':
            input_path = parameters.get('inputs_json', '').strip()
            config_xml = parameters.get('config_xml', '').strip()
            if not input_path:
                flash('Inputs JSON file is required', 'error')
                return redirect(request.referrer or url_for('dashboard'))
        else:  # HDF pair mode
            input_path = parameters.get('input_hdf', '').strip()
            output_hdf = parameters.get('output_hdf', '').strip()
            config_xml = ''
            if not input_path or not output_hdf:
                flash('Both input and output HDF files are required', 'error')
                return redirect(request.referrer or url_for('dashboard'))
        output_path = parameters.get('output_dir', '').strip()
        
    elif tool_name == 'kpi':
        if input_mode == 'json':
            input_path = parameters.get('kpi_json', '').strip()
            if not input_path:
                flash('KPI JSON file is required', 'error')
                return redirect(request.referrer or url_for('dashboard'))
        else:  # HDF pair mode
            input_path = parameters.get('input_hdf', '').strip()
            output_hdf = parameters.get('output_hdf', '').strip()
            if not input_path or not output_hdf:
                flash('Both input and output HDF files are required', 'error')
                return redirect(request.referrer or url_for('dashboard'))
        output_path = parameters.get('html_dir', '').strip()
        config_xml = ''
    
    elif tool_name == 'dc_html':
        # DC HTML requires XML config and JSON inputs
        config_xml = parameters.get('config_xml', '').strip()
        input_path = parameters.get('inputs_json', '').strip()
        output_path = parameters.get('output_path', '').strip()
        
        if not config_xml:
            flash('Configuration XML file (HTMLConfig.xml) is required', 'error')
            return redirect(request.referrer or url_for('dashboard'))
        if not input_path:
            flash('Inputs JSON file is required', 'error')
            return redirect(request.referrer or url_for('dashboard'))
        
    else:
        # Default behavior for other tools
        input_path = parameters.get('input_path', '').strip()
        output_path = parameters.get('output_path', '').strip()
        config_xml = ''
        if not input_path:
            flash('Input path is required', 'error')
            return redirect(request.referrer or url_for('dashboard'))
    
    # Get singularity image path - determine based on simg folder location
    simg_folder = app.config.get('SINGULARITY_IMAGE_PATH', '/app/simg')
    
    # Tool-specific singularity image mapping
    singularity_images = {
        'interactive_plot': 'interactive_plot.simg',
        'kpi': 'kpi.simg',
        'dc_html': 'dc_html.simg',
        'hyperlink_tool': 'all_services.simg'
    }
    
    singularity_image = os.path.join(
        simg_folder,
        singularity_images.get(tool_name, 'all_services.simg')
    )
    
    # Default output to html_db folder in simg directory
    if not output_path:
        output_path = os.path.join(simg_folder, 'html_db')

    # If user provided /net or /mnt paths, require only one password (NetID comes from login)
    clusters = set(
        filter(
            None,
            [
                cluster_from_path(input_path),
                cluster_from_path(config_xml),
                cluster_from_path(output_hdf),
                cluster_from_path(output_path),
            ],
        )
    )
    if clusters:
        if len(clusters) > 1:
            flash('Mixed cluster paths detected: do not mix /net (Krakow) and /mnt (Southfield) in one run.', 'error')
            return redirect(request.referrer or url_for('dashboard'))

        cluster_name = next(iter(clusters))
        cluster_password = (parameters.get('cluster_password') or '').strip()
        if not cluster_password:
            flash('Cluster password is required for /net or /mnt paths.', 'error')
            return redirect(request.referrer or url_for('dashboard'))

        host = get_cluster_paths(cluster_name).get('host')
        ok, details = validate_cluster_credentials(
            current_user.net_id,
            cluster_password,
            [(cluster_name, host)],
            require_all=True,
        )
        if not ok:
            err = (details.get(cluster_name) or {}).get('error')
            flash(f'Cluster login failed for {cluster_name}: {err}', 'error')
            return redirect(request.referrer or url_for('dashboard'))
    
    # Create job record
    job = JobHistory(
        user_id=current_user.id,
        tool_name=tool_name,
        input_path=input_path,
        input_filename=extract_hdf_filename(input_path),
        output_path=output_path,
        parameters=parameters,
        status='QUEUED'
    )
    db.session.add(job)
    db.session.commit()

    # Decide execution mode: Slurm vs local (for Windows/debugging)
    exec_mode = (os.environ.get('HPC_TOOLS_EXECUTION_MODE') or 'auto').strip().lower()
    is_windows = sys.platform.startswith('win')
    use_slurm = (exec_mode == 'slurm') or (exec_mode == 'auto' and (not is_windows) and slurm_is_available())

    if not use_slurm:
        tools_dir = str(_repo_root())
        
        # Convert Windows paths to WSL paths when running in container
        # This handles paths like C:\git\... -> /mnt/c/git/...
        input_path = _windows_path_to_wsl_path(input_path)
        output_path = _windows_path_to_wsl_path(output_path)
        config_xml = _windows_path_to_wsl_path(config_xml) if config_xml else config_xml
        
        # Prefer writing logs into the selected output dir
        log_dir = output_path
        try:
            os.makedirs(log_dir, exist_ok=True)
        except Exception:
            log_dir = os.path.join(simg_folder, 'html_db')
            os.makedirs(log_dir, exist_ok=True)

        log_path = os.path.join(log_dir, f'local_{tool_name}_{job.id}.log')

        if tool_name == 'kpi':
            if input_mode == 'json':
                cmd = [sys.executable, '-m', 'KPI.kpi_server', input_path, output_path]
            else:
                input_hdf = _windows_path_to_wsl_path(parameters.get('input_hdf', ''))
                output_hdf = _windows_path_to_wsl_path(parameters.get('output_hdf', ''))
                cmd = [sys.executable, '-m', 'KPI.kpi_server', input_hdf, output_hdf, output_path]
        elif tool_name == 'interactive_plot':
            if input_mode != 'json':
                job.status = 'FAILED'
                job.completed_at = datetime.utcnow()
                job.error_message = 'Local interactive_plot currently supports JSON mode only.'
                db.session.commit()
                flash('Local run failed: interactive_plot JSON mode only (for now).', 'error')
                return redirect(request.referrer or url_for('dashboard'))
            script = str(Path(tools_dir) / 'ResimHTMLReport.py')
            cmd = [sys.executable, script, config_xml, input_path, output_path]
        elif tool_name == 'dc_html':
            # DC HTML: python -m IPS.ResimHTMLReport <config_xml> <inputs_json> <output_dir>
            cmd = [sys.executable, '-m', 'IPS.ResimHTMLReport', config_xml, input_path, output_path]
        else:
            job.status = 'FAILED'
            job.completed_at = datetime.utcnow()
            job.error_message = f'Local execution not implemented for tool: {tool_name}'
            db.session.commit()
            flash(f'Local run not supported for {tool_name}', 'error')
            return redirect(request.referrer or url_for('dashboard'))

        child_env = None
        working_dir = tools_dir
        if tool_name == 'interactive_plot':
            # Workaround for older generated *_pb2.py files against newer protobuf runtimes.
            # Avoid requiring the user to set $env:PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION manually.
            child_env = dict(os.environ)
            child_env['PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION'] = 'python'
        elif tool_name == 'dc_html':
            # DC HTML needs to run from dc_html directory
            working_dir = str(Path(tools_dir) / 'dc_html')

        t = threading.Thread(
            target=_run_local_job_background,
            args=(job.id, cmd, working_dir, log_path, child_env),
            daemon=True,
        )
        t.start()
        # Show host path (not container path) so user can find the log file
        display_log_path = _container_path_to_host_path(log_path)
        flash(f'Local job started (no Slurm). Log: {display_log_path}', 'success')
        return redirect(request.referrer or url_for('dashboard'))
    
    # Prefer srun-based allocation from the long-running all_services process.
    # This keeps all_services on the login node, while tools run on compute nodes.
    launcher = (os.environ.get('HPC_TOOLS_SLURM_LAUNCHER') or 'srun').strip().lower()
    have_srun = shutil.which('srun') is not None
    have_sbatch = shutil.which('sbatch') is not None

    # Tool-specific resource requirements (can be overridden by env / config later)
    resource_configs = {
        'interactive_plot': {'memory': '64G', 'cpus': 8, 'time_limit': '04:00:00'},
        'kpi': {'memory': '32G', 'cpus': 8, 'time_limit': '02:00:00'},
        'dc_html': {'memory': '16G', 'cpus': 4, 'time_limit': '01:00:00'},
        'hyperlink_tool': {'memory': '8G', 'cpus': 2, 'time_limit': '00:30:00'}
    }
    resource_config = resource_configs.get(tool_name, {'memory': '16G', 'cpus': 4, 'time_limit': '01:00:00'})

    # Log file for the launcher (srun/sbatch stdout+stderr)
    log_dir = output_path
    try:
        os.makedirs(log_dir, exist_ok=True)
    except Exception:
        log_dir = os.path.join(simg_folder, 'html_db')
        os.makedirs(log_dir, exist_ok=True)

    log_path = os.path.join(log_dir, f'slurm_{tool_name}_{job.id}.log')

    # srun path (preferred)
    if (launcher == 'srun') and have_srun:
        partition = (app.config.get('SLURM_PARTITION') or 'plcyf-com').strip()
        account = (app.config.get('SLURM_ACCOUNT') or '').strip()
        exclude_nodes = (os.environ.get('HPC_TOOLS_SLURM_EXCLUDE_NODES') or 'plcyf-com-prod-log01,plcyf-com-prod-log02').strip()
        singularity_module = (app.config.get('SINGULARITY_MODULE') or 'singularity/3.11.4').strip()

        # Build a singularity command that will execute on the compute node.
        # NOTE: Do NOT rely on container-local /tmp scripts; compute nodes must be able to run everything via shared paths.
        bind_args = []
        if os.path.isdir('/net'):
            bind_args.append('--bind /net:/net')
        if os.path.isdir('/mnt'):
            bind_args.append('--bind /mnt:/mnt')
        if os.path.isdir('/scratch'):
            bind_args.append('--bind /scratch:/scratch')
        # Always allow writing to /tmp on compute nodes
        if os.path.isdir('/tmp'):
            bind_args.append('--bind /tmp:/tmp')

        bind_str = ' '.join(bind_args)

        # Tool run command (inside singularity image)
        if tool_name == 'kpi':
            if input_mode == 'json':
                tool_cmd = f"singularity run {bind_str} {shlex.quote(singularity_image)} json {shlex.quote(input_path)} {shlex.quote(output_path)}"
            else:
                input_hdf = (parameters.get('input_hdf') or '').strip()
                output_hdf = (parameters.get('output_hdf') or '').strip()
                tool_cmd = f"singularity run {bind_str} {shlex.quote(singularity_image)} hdf {shlex.quote(input_hdf)} {shlex.quote(output_hdf)} {shlex.quote(output_path)}"
        elif tool_name == 'interactive_plot':
            if input_mode == 'json':
                tool_cmd = f"singularity run {bind_str} {shlex.quote(singularity_image)} {shlex.quote(config_xml)} {shlex.quote(input_path)} {shlex.quote(output_path)}"
            else:
                input_hdf = (parameters.get('input_hdf') or '').strip()
                output_hdf = (parameters.get('output_hdf') or '').strip()
                tool_cmd = f"singularity run {bind_str} {shlex.quote(singularity_image)} {shlex.quote(input_hdf)} {shlex.quote(output_hdf)} {shlex.quote(output_path)}"
        elif tool_name == 'dc_html':
            tool_cmd = f"singularity run {bind_str} {shlex.quote(singularity_image)} {shlex.quote(config_xml)} {shlex.quote(input_path)} {shlex.quote(output_path)}"
        else:
            job.status = 'FAILED'
            job.completed_at = datetime.utcnow()
            job.error_message = f'Slurm srun execution not implemented for tool: {tool_name}'
            db.session.commit()
            flash(f'Slurm run not supported for {tool_name}', 'error')
            return redirect(request.referrer or url_for('dashboard'))

        remote_shell = (
            "set -euo pipefail; "
            "if type module >/dev/null 2>&1; then module load slurm >/dev/null 2>&1 || true; "
            f"module load {shlex.quote(singularity_module)} >/dev/null 2>&1 || true; fi; "
            "echo 'HOSTNAME: '$(hostname); "
            "echo 'DATE: '$(date); "
            "echo 'SLURM_JOB_ID: '${SLURM_JOB_ID:-}; "
            f"{tool_cmd}"
        )

        srun_cmd = [
            'srun',
            '--exclusive',
            '-N', '1',
            '-n', '1',
            f"--partition={partition}",
            f"--mem={resource_config.get('memory')}",
            f"--cpus-per-task={resource_config.get('cpus')}",
            f"--time={resource_config.get('time_limit')}",
            f"--job-name={tool_name}_{job.id}",
        ]
        if account:
            srun_cmd.append(f"--account={account}")
        if exclude_nodes:
            srun_cmd.append(f"--exclude={exclude_nodes}")

        srun_cmd += ['bash', '-lc', remote_shell]

        # Run srun in a background thread so the web server stays responsive.
        t = threading.Thread(
            target=_run_local_job_background,
            args=(job.id, srun_cmd, '/', log_path, None),
            daemon=True,
        )
        t.start()

        display_log_path = _container_path_to_host_path(log_path)
        job.status = 'QUEUED'
        job.output_log_path = log_path
        db.session.commit()
        flash(f'Slurm job started via srun. Log: {display_log_path}', 'success')
        return redirect(request.referrer or url_for('dashboard'))

    # sbatch fallback (older behavior)
    if not have_sbatch:
        job.status = 'FAILED'
        job.completed_at = datetime.utcnow()
        job.error_message = "Slurm not available: neither 'srun' nor 'sbatch' found on PATH."
        db.session.commit()
        flash(job.error_message, 'error')
        return redirect(request.referrer or url_for('dashboard'))

    # Build kwargs for script generation
    script_kwargs = {
        'input_mode': input_mode,
        'html_dir': output_path,
        'config_xml': config_xml if tool_name in ['interactive_plot', 'dc_html'] else '',
        'input_hdf': parameters.get('input_hdf', '') if input_mode == 'hdf' else '',
        'output_hdf': parameters.get('output_hdf', '') if input_mode == 'hdf' else ''
    }

    script_path = generate_slurm_script(
        tool_name=tool_name,
        input_path=input_path,
        output_path=output_path,
        singularity_image=singularity_image,
        **script_kwargs
    )

    success, slurm_job_id, error = slurm_manager.submit_job(
        script_path=script_path,
        job_name=f"{tool_name}_{job.id}",
        memory=resource_config.get('memory', '16G'),
        cpus=resource_config.get('cpus', 4),
        time_limit=resource_config.get('time_limit', '01:00:00')
    )

    if success:
        job.slurm_job_id = slurm_job_id
        job.status = 'SUBMITTED'
        job_name = f"{tool_name}_{job.id}"
        job.output_log_path = f"/scratch/logs/slurm_{job_name}_{slurm_job_id}.out"
        db.session.commit()
        flash(f'Job submitted successfully! Slurm Job ID: {slurm_job_id}', 'success')
    else:
        job.status = 'FAILED'
        job.error_message = error
        db.session.commit()
        flash(f'Job submission failed: {error}', 'error')

    return redirect(request.referrer or url_for('dashboard'))


# ============================================================================
# CHAT API ROUTES
# ============================================================================

@app.route('/api/chat', methods=['POST'])
@login_required
def chat_api():
    """Handle chat messages with LLM"""
    data = request.get_json()
    message = data.get('message', '').strip()
    session_id = data.get('session_id')
    
    if not message:
        return jsonify({'error': 'Message is required'}), 400
    
    # Get or create chat session
    if session_id:
        chat_session = ChatSession.query.filter_by(
            session_id=session_id,
            user_id=current_user.id
        ).first()
    else:
        chat_session = ChatSession(
            user_id=current_user.id,
            session_id=str(uuid.uuid4())
        )
        db.session.add(chat_session)
        db.session.commit()
    
    # Save user message
    user_message = ChatMessage(
        session_id=chat_session.id,
        role='user',
        content=message
    )
    db.session.add(user_message)
    db.session.commit()
    
    try:
        rag_response = rag_client.ask(message, chat_session.session_id)
        if rag_response.get('ok'):
            response = rag_response.get('answer', '')
        else:
            response = (
                'The workspace help service is not reachable right now. '
                'You can still use KPI and Hyperlink while the runtime map is being configured.\n\n'
                f"Details: {rag_response.get('error', 'No details available')}"
            )
    except Exception as e:
        logger.error(f"Workspace help backend error: {e}")
        response = "The workspace help service is unavailable right now. Please try again later."
    
    # Save assistant message
    assistant_message = ChatMessage(
        session_id=chat_session.id,
        role='assistant',
        content=response
    )
    db.session.add(assistant_message)
    db.session.commit()
    
    return jsonify({
        'response': response,
        'session_id': chat_session.session_id
    })


# ============================================================================
# JOB STATUS API ROUTES
# ============================================================================

@app.route('/api/job/<int:job_id>/status')
@login_required
def get_job_status(job_id):
    """Get status of a specific job"""
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()
    
    runtime_job = _sync_runtime_job(job)

    # If job has Slurm ID, check actual status
    if not runtime_job and job.slurm_job_id and job.status not in ['COMPLETED', 'FAILED', 'CANCELLED']:
        slurm_status = slurm_manager.check_job_status(job.slurm_job_id)
        
        new_status = slurm_status.get('status', job.status)
        if new_status != job.status:
            job.status = new_status
            if new_status in ['COMPLETED', 'FAILED']:
                job.completed_at = datetime.utcnow()
            db.session.commit()
    
    return jsonify(job.to_dict())


@app.route('/api/job/<int:job_id>/cancel', methods=['POST'])
@login_required
def cancel_job(job_id):
    """Cancel a running job"""
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()
    
    runtime_job_id = ((job.parameters or {}).get('runtime_job_id') if job.parameters else None)
    if runtime_job_id and job.status in ['QUEUED', 'SUBMITTED', 'RUNNING']:
        result = broker_client.cancel_job(int(runtime_job_id))
        job.status = 'CANCELLED'
        job.completed_at = datetime.utcnow()
        db.session.commit()
        return jsonify({'success': True, 'message': result.get('message', 'Cancelled')})

    if job.slurm_job_id and job.status in ['QUEUED', 'SUBMITTED', 'RUNNING']:
        success, message = slurm_manager.cancel_job(job.slurm_job_id)
        
        if success:
            job.status = 'CANCELLED'
            job.completed_at = datetime.utcnow()
            db.session.commit()
            return jsonify({'success': True, 'message': message})
        else:
            return jsonify({'success': False, 'error': message}), 400
    
    return jsonify({'success': False, 'error': 'Job cannot be cancelled'}), 400


@app.route('/api/job/<int:job_id>/progress')
@login_required
def get_job_progress(job_id):
    """Get progress information for a running job"""
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()
    
    # Default progress response
    progress_data = {
        'job_id': job.id,
        'status': job.status,
        'progress': 0,
        'message': 'Initializing...'
    }
    
    if job.status == 'COMPLETED':
        progress_data['progress'] = 100
        progress_data['message'] = 'Completed successfully'
        progress_data['output_path'] = job.output_path
    elif job.status == 'FAILED':
        progress_data['progress'] = 0
        progress_data['message'] = job.error_message or 'Job failed'
    elif job.status in ['RUNNING', 'SUBMITTED', 'QUEUED']:
        # Try to read progress from log file if available
        log_path = job.output_log_path
        if not log_path and job.output_path:
            # Check for local job log
            log_path = os.path.join(job.output_path, f'local_{job.tool_name}_{job.id}.log')
        
        if log_path and os.path.exists(log_path):
            try:
                with open(log_path, 'r') as f:
                    lines = f.readlines()
                    # Look for progress indicators in the log
                    for line in reversed(lines[-50:]):  # Check last 50 lines
                        line = line.strip()
                        # Parse progress patterns like "[50%]" or "Progress: 50%"
                        if '%' in line:
                            import re
                            match = re.search(r'(\d+(?:\.\d+)?)\s*%', line)
                            if match:
                                progress_data['progress'] = min(99, float(match.group(1)))
                                progress_data['message'] = line[:100]
                                break
                        # Parse processing messages
                        elif 'Processing' in line or 'Generating' in line:
                            progress_data['message'] = line[:100]
                            progress_data['progress'] = 50  # Indeterminate progress
                            break
            except Exception:
                pass
        
        # Estimate progress based on time for queued/running jobs
        if progress_data['progress'] == 0 and job.status == 'RUNNING':
            progress_data['progress'] = 25
            progress_data['message'] = 'Processing...'
        elif job.status == 'QUEUED' or job.status == 'SUBMITTED':
            progress_data['message'] = 'Waiting in queue...'
    
    return jsonify(progress_data)


@app.route('/api/job/<int:job_id>/console-tail')
@login_required
def get_job_console_tail(job_id):
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()

    runtime_job = _sync_runtime_job(job)
    log_path, console = _resolve_job_log_path(job, runtime_job)
    offset = max(request.args.get('offset', default=0, type=int), 0)
    payload = {
        'success': True,
        'status': job.status,
        'text': '',
        'offset': offset,
        'next_offset': offset,
        'log_exists': bool(log_path and os.path.exists(log_path)),
        'supports_input': bool(console.get('supports_input')) and job.status in ['QUEUED', 'RUNNING', 'SUBMITTED'],
        'pane_names': console.get('pane_names') if isinstance(console.get('pane_names'), list) else ['main'],
        'tmux_session_name': console.get('tmux_session_name', ''),
        'console_name': console.get('display_name', 'Runtime console'),
        'display_log_path': host_path_filter(log_path) if log_path else '',
    }
    if not payload['log_exists']:
        return jsonify(payload)

    size = os.path.getsize(log_path)
    if offset > size:
        offset = 0
        payload['offset'] = 0
    try:
        with open(log_path, 'rb') as fp:
            fp.seek(offset)
            chunk = fp.read(262144)
    except Exception as exc:
        return jsonify({'success': False, 'error': str(exc)}), 500

    payload['text'] = chunk.decode('utf-8', errors='replace')
    payload['next_offset'] = offset + len(chunk)
    return jsonify(payload)


@app.route('/api/job/<int:job_id>/console-input', methods=['POST'])
@login_required
def send_job_console_input(job_id):
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()

    runtime_job = _sync_runtime_job(job)
    _, console = _resolve_job_log_path(job, runtime_job)
    queue_path = (console.get('input_queue_path') or '').strip()
    allowed_panes = console.get('pane_names') if isinstance(console.get('pane_names'), list) else ['main']

    if not queue_path or not console.get('supports_input'):
        return jsonify({'success': False, 'error': 'This run does not expose a live tmux console.'}), 400
    if job.status not in ['QUEUED', 'RUNNING', 'SUBMITTED']:
        return jsonify({'success': False, 'error': 'This run is no longer active.'}), 400

    data = request.get_json(silent=True) or {}
    pane = str(data.get('pane') or '').strip()
    if pane not in allowed_panes:
        pane = allowed_panes[0] if allowed_panes else 'main'

    signal_name = str(data.get('signal') or '').strip().lower()
    if signal_name == 'interrupt':
        payload_text = '__CTRL_C__'
    elif signal_name == 'enter':
        payload_text = '__ENTER__'
    else:
        raw_input = str(data.get('input') or '')
        payload_text = ' '.join(raw_input.replace('\r', '\n').splitlines()).replace('\t', ' ').strip()
        if not payload_text:
            return jsonify({'success': False, 'error': 'Console input cannot be empty.'}), 400

    os.makedirs(os.path.dirname(queue_path), exist_ok=True)
    with open(queue_path, 'a', encoding='utf-8', errors='replace') as fp:
        fp.write(f'{pane}\t{payload_text}\n')

    return jsonify({'success': True, 'pane': pane})


@app.route('/html/job/<int:job_id>/output')
@login_required
def view_job_output(job_id):
    """View output files for a completed job"""
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()
    
    if not job.output_path:
        flash('No output path available for this job', 'error')
        return redirect(url_for('job_history'))
    
    output_path = job.output_path
    files = []
    
    try:
        if os.path.isdir(output_path):
            for root, dirs, filenames in os.walk(output_path):
                for filename in filenames:
                    filepath = os.path.join(root, filename)
                    rel_path = os.path.relpath(filepath, output_path)
                    file_info = {
                        'name': filename,
                        'rel_path': rel_path,
                        'full_path': filepath,
                        'size': os.path.getsize(filepath),
                        'is_html': filename.lower().endswith('.html'),
                        'modified': datetime.fromtimestamp(os.path.getmtime(filepath))
                    }
                    files.append(file_info)
            
            # Sort: HTML files first, then by name
            files.sort(key=lambda x: (not x['is_html'], x['name'].lower()))
    except Exception as e:
        flash(f'Error reading output directory: {str(e)}', 'error')
        return redirect(url_for('job_history'))
    
    return render_template('tools/job_output.html',
                         job=job,
                         output_path=output_path,
                         files=files)


@app.route('/html/job/<int:job_id>/log')
@login_required
def view_job_log(job_id):
    """View the tail of a job's log file (local or Slurm)."""
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()

    runtime_job = _sync_runtime_job(job)
    log_path, console = _resolve_job_log_path(job, runtime_job)

    n_lines = request.args.get('n', default=400, type=int)
    n_lines = max(50, min(5000, n_lines))

    log_exists = bool(log_path and os.path.exists(log_path))
    tail_text = _read_log_tail_text(log_path, n_lines) if log_exists else ''
    initial_offset = os.path.getsize(log_path) if log_exists else 0

    display_log_path = host_path_filter(log_path) if log_path else ''
    tail_cmd = f"tail -f {display_log_path}" if display_log_path else ''
    tmux_cmd = ''
    if console.get('tmux_session_name'):
        tmux_cmd = f"tmux attach -t {console['tmux_session_name']}"

    return render_template(
        'job_log.html',
        job=job,
        log_path=log_path,
        display_log_path=display_log_path,
        log_exists=log_exists,
        tail_cmd=tail_cmd,
        tmux_cmd=tmux_cmd,
        tail_text=tail_text,
        n_lines=n_lines,
        initial_offset=initial_offset,
        console=console,
    )


@app.route('/html/job/<int:job_id>/file/<path:rel_path>')
@login_required  
def serve_job_file(job_id, rel_path):
    """Serve a file from job output directory"""
    job = JobHistory.query.filter_by(
        id=job_id,
        user_id=current_user.id
    ).first_or_404()
    
    if not job.output_path:
        return "No output path", 404
    
    # Security check: ensure the requested file is within the output directory
    full_path = os.path.normpath(os.path.join(job.output_path, rel_path))
    if not full_path.startswith(os.path.normpath(job.output_path)):
        return "Access denied", 403
    
    if not os.path.exists(full_path):
        return "File not found", 404
    
    # For HTML files, serve inline. For others, allow download
    if full_path.lower().endswith('.html'):
        return send_file(full_path, mimetype='text/html')
    else:
        return send_file(full_path, as_attachment=True)


@app.route('/api/jobs')
@login_required
def get_user_jobs():
    """Get all jobs for current user"""
    page = request.args.get('page', 1, type=int)
    per_page = request.args.get('per_page', 20, type=int)
    tool_name = request.args.get('tool')
    
    query = JobHistory.query.filter_by(user_id=current_user.id)
    
    if tool_name:
        query = query.filter_by(tool_name=tool_name)
    
    jobs = query.order_by(JobHistory.created_at.desc())\
        .paginate(page=page, per_page=per_page)
    
    return jsonify({
        'jobs': [job.to_dict() for job in jobs.items],
        'total': jobs.total,
        'pages': jobs.pages,
        'current_page': page
    })


@app.route('/api/runtime/tools', methods=['GET', 'POST'])
@login_required
def api_runtime_tools():
    if request.method == 'POST':
        payload = request.get_json(silent=True) or {}
        payload.setdefault('tool_key', payload.get('toolKey', ''))
        payload.setdefault('display_name', payload.get('displayName', payload.get('tool_key', '')))
        payload.setdefault('category', payload.get('category', 'batch'))
        if not payload.get('tool_key'):
            return jsonify({'ok': False, 'error': 'tool_key is required'}), 400
        tool = runtime_store.save_tool(payload, updated_by=current_user.net_id)
        variant_key = (payload.get('variant_key') or '').strip() or None
        return jsonify({'ok': True, 'tool': tool, 'graph': runtime_store.graph_payload(variant_key)})

    variant_key = (request.args.get('variant') or '').strip() or None
    graph_payload = runtime_store.graph_payload(variant_key)
    try:
        broker_status = broker_client.ping()
    except Exception as exc:
        broker_status = {'ok': False, 'status': 'offline', 'error': str(exc)}
    return jsonify({'ok': True, 'graph': graph_payload, 'broker': broker_status})


@app.route('/api/runtime/graph', methods=['GET', 'POST'])
@login_required
def api_runtime_graph():
    if request.method == 'POST':
        payload = request.get_json(silent=True) or {}
        variant_key = (payload.get('variant_key') or '').strip()
        display_name = (payload.get('display_name') or '').strip() or variant_key
        if not variant_key:
            return jsonify({'ok': False, 'error': 'variant_key is required'}), 400
        try:
            variant = runtime_store.save_graph_variant(
                {
                    'variant_key': variant_key,
                    'display_name': display_name,
                    'description': (payload.get('description') or '').strip(),
                    'graph': payload.get('graph', {}),
                    'is_default': bool(payload.get('is_default')),
                },
                updated_by=current_user.net_id,
            )
        except ValueError as exc:
            return jsonify({'ok': False, 'error': str(exc)}), 400
        return jsonify({'ok': True, 'variant': variant, 'graph': runtime_store.graph_payload(variant_key)})

    variant_key = (request.args.get('variant') or '').strip() or None
    return jsonify({'ok': True, 'graph': runtime_store.graph_payload(variant_key)})


@app.route('/api/runtime/graph/reset', methods=['POST'])
@login_required
def api_runtime_graph_reset():
    payload = request.get_json(silent=True) or {}
    variant_key = (payload.get('variant_key') or '').strip() or 'default'
    try:
        variant = runtime_store.reset_graph_variant(variant_key, updated_by=current_user.net_id)
    except ValueError as exc:
        return jsonify({'ok': False, 'error': str(exc)}), 400
    return jsonify({'ok': True, 'variant': variant, 'graph': runtime_store.graph_payload(variant_key)})


@app.route('/api/runtime/launch', methods=['POST'])
@login_required
def api_runtime_launch():
    payload = request.get_json(silent=True) or {}
    tool_key = (payload.get('tool_key') or '').strip()
    paths = payload.get('paths', {}) or {}
    resources = payload.get('resources', {}) or {}
    if not tool_key:
        return jsonify({'ok': False, 'error': 'tool_key is required'}), 400

    try:
        result = broker_client.submit_job({
            'tool_key': tool_key,
            'mode': payload.get('mode', ''),
            'paths': paths,
            'resources': resources or _default_runtime_resources(tool_key),
            'session_id': session.get('_id', ''),
            'user': current_user.net_id,
        })
    except Exception as exc:
        return jsonify({'ok': False, 'error': str(exc)}), 500

    job = JobHistory(
        user_id=current_user.id,
        tool_name=tool_key if tool_key != 'hyperlink' else 'hyperlink_tool',
        input_path=paths.get('json_path') or paths.get('input_hdf') or paths.get('html_root') or '',
        input_filename=extract_hdf_filename(paths.get('json_path') or paths.get('input_hdf') or paths.get('html_root') or ''),
        output_path=result.get('output_path', ''),
        output_log_path=result.get('log_path', ''),
        parameters={
            'runtime_job_id': result['job_id'],
            'execution_target': tool_key,
            'paths': paths,
            'resources': resources,
            'runtime_console': result.get('console', {}),
        },
        status='QUEUED',
    )
    db.session.add(job)
    db.session.commit()
    return jsonify({'ok': True, 'result': result, 'job_id': job.id})


# ============================================================================
# FILE BROWSER API ROUTES
# ============================================================================

@app.route('/api/browse')
@login_required
def browse_files():
    """Browse files on the cluster"""
    path = request.args.get('path', app.config['DATA_BASE_PATH'])
    extensions = request.args.get('extensions', '').split(',')
    extensions = [e.strip() for e in extensions if e.strip()]
    
    try:
        result = file_browser.list_directory(path, extensions or None)
        result['parent'] = file_browser.get_parent_path(path)
        return jsonify(result)
    except PermissionError as e:
        return jsonify({'error': str(e)}), 403
    except FileNotFoundError as e:
        return jsonify({'error': str(e)}), 404
    except Exception as e:
        return jsonify({'error': str(e)}), 500


# ============================================================================
# HISTORY ROUTES
# ============================================================================

@app.route('/html/history')
@login_required
def job_history():
    """View complete runtime console history"""
    page = request.args.get('page', 1, type=int)
    tool_filter = request.args.get('tool', '')
    status_filter = request.args.get('status', '')
    
    query = JobHistory.query.filter_by(user_id=current_user.id)
    
    if tool_filter:
        query = query.filter_by(tool_name=tool_filter)
    if status_filter:
        query = query.filter_by(status=status_filter)
    
    jobs = query.order_by(JobHistory.created_at.desc())\
        .paginate(page=page, per_page=20)
    for job in jobs.items:
        _sync_runtime_job(job)
    
    return render_template('history.html', 
                         jobs=jobs,
                         tool_filter=tool_filter,
                         status_filter=status_filter)


# ============================================================================
# ERROR HANDLERS
# ============================================================================

@app.errorhandler(404)
def not_found_error(error):
    return render_template('errors/404.html'), 404


@app.errorhandler(500)
def internal_error(error):
    logger.exception('Unhandled exception', exc_info=error)
    db.session.rollback()
    return render_template('errors/500.html'), 500


# ============================================================================
# CLI COMMANDS
# ============================================================================

@app.cli.command('init-db')
def init_database():
    """Initialize the database"""
    init_db(app)
    print('Database initialized!')


@app.cli.command('create-admin')
def create_admin():
    """Create admin user"""
    from getpass import getpass
    
    net_id = input('Admin Net ID: ')
    name = input('Admin Name: ')
    password = getpass('Password: ')
    
    user = User(net_id=net_id, name=name, role='admin')
    user.set_password(password)
    db.session.add(user)
    db.session.commit()
    print(f'Admin user {net_id} created!')


# ============================================================================
# MAIN ENTRY POINT
# ============================================================================

if __name__ == '__main__':
    with app.app_context():
        try:
            db.create_all()
        except OperationalError as exc:
            logger.error('Database connection failed.')
            logger.error('Configured SQLALCHEMY_DATABASE_URI=%s', app.config.get('SQLALCHEMY_DATABASE_URI'))
            logger.error('Fix options:')
            logger.error('  1) Start Postgres via Docker: `docker compose up -d postgres` (or `docker-compose up -d postgres`)')
            logger.error('  2) Set DATABASE_URL to a reachable Postgres instance')
            logger.error('  3) On Windows, leave DATABASE_URL unset to use local SQLite (default)')
            logger.debug('OperationalError details: %s', exc)
            sys.exit(1)
    
    app.run(
        host='0.0.0.0',
        port=5002,
        debug=app.config.get('DEBUG', False)
    )
