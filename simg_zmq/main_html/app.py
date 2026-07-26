"""
Main Flask Application for HPC Tools Platform
Provides web interface for KPI, DC HTML, Interactive Plot, and Hyperlink tools
with LLM chat integration and Slurm job management
"""
import os
import copy
import uuid
import logging
import json
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
import shlex
from pathlib import Path
from datetime import datetime, timezone
from typing import Optional

from flask import Flask, Response, render_template, request, redirect, url_for, flash, jsonify, session, send_from_directory, send_file, abort
from flask_compress import Compress
from flask_login import LoginManager, login_user, logout_user, login_required, current_user
from flask_migrate import Migrate
from sqlalchemy.exc import OperationalError

from config import config
from hpcc_broker_client import HpccBrokerClient
from jira_integration import JiraIntegration
from models import db, User, JobHistory, ChatSession, ChatMessage, HyperlinkSavedPair, init_db
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
from env_utils import get_env, get_cache_dir, get_cluster_paths, get_cluster_slurm_defaults

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
            with db.engine.begin() as connection:
                connection.exec_driver_sql('PRAGMA journal_mode=WAL;')
                connection.exec_driver_sql('PRAGMA synchronous=NORMAL;')
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

jira_integration = JiraIntegration()


def _repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def _asset_manifest_path() -> Path:
    return Path(app.static_folder) / 'dist' / 'asset-manifest.json'


def _load_asset_manifest() -> dict:
    manifest_path = _asset_manifest_path()
    if not manifest_path.exists():
        return {}
    try:
        return json.loads(manifest_path.read_text(encoding='utf-8'))
    except (OSError, json.JSONDecodeError):
        logger.warning('Failed to load static asset manifest from %s', manifest_path)
        return {}


def _asset_url(bundle_path: str, fallback: str = '') -> str:
    manifest = _load_asset_manifest()
    asset_entry = manifest.get(bundle_path) if isinstance(manifest, dict) else None
    bundle_file = Path(app.static_folder) / bundle_path
    selected_path = bundle_path if bundle_file.exists() else (fallback or bundle_path)
    version = asset_entry.get('hash') if isinstance(asset_entry, dict) else ''
    if version and selected_path == bundle_path:
        return url_for('static', filename=selected_path, v=version)
    return url_for('static', filename=selected_path)


def _hyperlink_tool_base() -> str:
    repo_root = _repo_root()
    direct_path = repo_root / 'Hyperlink_tool' / 'code' / 'html_online'
    if direct_path.exists():
        return str(direct_path)
    legacy_path = repo_root / 'tools' / 'Hyperlink_tool' / 'code' / 'html_online'
    return str(legacy_path)


def _first_writable_dir(*candidates) -> str:
    """Return the first candidate dir that exists (or can be created) AND is
    actually writable, verified with a real probe file (NFS ACLs can allow
    read/traverse while denying write, which os.makedirs() alone won't catch).
    """
    for candidate in candidates:
        if not candidate:
            continue
        try:
            os.makedirs(candidate, exist_ok=True)
            probe = os.path.join(candidate, f'.wtest_{uuid.uuid4().hex[:8]}')
            with open(probe, 'w') as fp:
                fp.write('x')
            os.remove(probe)
            return candidate
        except Exception:
            continue
    return ''


def _first_existing_file(*candidates) -> str:
    """Return the first candidate path that exists as a regular file."""
    for candidate in candidates:
        if candidate and os.path.isfile(candidate):
            return candidate
    return candidates[0] if candidates else ''


def _resim_script_source_path() -> str:
    """Locate rResim_Gen7.sh across dev and deployed (Singularity) layouts.

    Inside the deployed container, `_repo_root()` resolves via the
    /app/main_html bind-mount (e.g. /app), so `_repo_root().parent` is just
    "/" and never contains the script — that was the deployed "No such file
    or directory" bug. HPCC_BUNDLE_ROOT/HPCC_PROJECT_ROOT are bind-mounted at
    the SAME absolute path inside and outside the container, so they reliably
    point at the real deploy root (where generate_upload.py copies the
    script). Fall back to the local Windows dev layout (repo's parent dir).
    """
    candidates = []
    bundle_root = (os.environ.get('HPCC_BUNDLE_ROOT') or '').strip()
    if bundle_root:
        candidates.append(os.path.join(bundle_root, 'rResim_Gen7.sh'))
    project_root_env = (os.environ.get('HPCC_PROJECT_ROOT') or '').strip()
    if project_root_env:
        candidates.append(os.path.join(os.path.dirname(project_root_env.rstrip('/\\')), 'rResim_Gen7.sh'))
    candidates.append(str(_repo_root() / 'rResim_Gen7.sh'))
    return _first_existing_file(*candidates)



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


@app.context_processor
def _inject_asset_helpers():
    return {'asset_url': _asset_url}

runtime_store = RuntimeStore()
runtime_store.ensure_defaults()
broker_client = HpccBrokerClient()
rag_client = RagClient(runtime_store)

_SLURM_DEFAULTS = get_cluster_slurm_defaults()

_HPCC_RESOURCE_BASE = {
    'scheduler': 'slurm',
    'account': _SLURM_DEFAULTS['account'],
    'qos': _SLURM_DEFAULTS['qos'],
    'nodes': 1,
    'ntasks': 1,
    'partition': _SLURM_DEFAULTS['partition'],
    'immediate': '',
}


def _tool_resources(tool_name: str) -> dict:
    """Read resource defaults from resources.py (single source of truth)."""
    import sys as _sys
    _resources_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'resources.py')
    if os.path.exists(_resources_path):
        _dir = os.path.dirname(_resources_path)
        if _dir not in _sys.path:
            _sys.path.insert(0, _dir)
        try:
            import resources
            return dict(resources.tool_resources(tool_name))
        except Exception:
            pass
    fallback = {
        'can_kpi': {'memory': '32G', 'cpus': 8, 'time_limit': '02:00:00'},
        'udp_kpi': {'memory': '32G', 'cpus': 8, 'time_limit': '02:00:00'},
        'interactive_plot': {'memory': '64G', 'cpus': 8, 'time_limit': '04:00:00'},
        'rag': {'memory': '72G', 'cpus': 8, 'time_limit': '18:00:00', 'gpu': True, 'gres': 'gpu:1'},
        'hyperlink_tool': {'memory': '8G', 'cpus': 2, 'time_limit': '00:30:00'},
    }
    return dict(fallback.get(tool_name, fallback['udp_kpi']))


def _runtime_resources_from_tool_config(tool_name: str, *, default_memory: str, default_cpus: int, default_time_limit: str) -> dict:
    tool_config = app.config.get('TOOLS', {}).get(tool_name, {})
    return {
        'memory': str(tool_config.get('memory', default_memory)).strip(),
        'cpus': int(tool_config.get('cpus', default_cpus)),
        'time_limit': str(tool_config.get('time_limit', default_time_limit)).strip(),
    }


RUNTIME_TOOL_DEFAULTS = {
    'can_kpi': {**_HPCC_RESOURCE_BASE, **_tool_resources('can_kpi')},
    'udp_kpi': {**_HPCC_RESOURCE_BASE, **_tool_resources('udp_kpi')},
    'interactive_plot': {**_HPCC_RESOURCE_BASE, **_tool_resources('interactive_plot')},
    'rag': {**_HPCC_RESOURCE_BASE, **_tool_resources('rag')},
}
ACTIVE_RUNTIME_STATUSES = {'QUEUED', 'SUBMITTED', 'PENDING', 'RUNNING'}
PENDING_RUNTIME_STATUSES = {'QUEUED', 'SUBMITTED', 'PENDING'}


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


def _persist_cluster_password(user: Optional[User], password: str) -> None:
    if user is None or not password:
        return
    try:
        user.set_cluster_password(password)
    except Exception:
        logger.exception('Failed to persist cluster password for %s', getattr(user, 'net_id', 'unknown'))


def _hyperlink_credential_user(net_id: str = '') -> Optional[User]:
    requested_net_id = (net_id or '').strip()
    if current_user.is_authenticated:
        current_net_id = (current_user.net_id or '').strip()
        if requested_net_id and requested_net_id != current_net_id:
            return None
        return current_user

    session_net_id = (session.get('hyperlink_net_id') or '').strip()
    if requested_net_id and session_net_id and requested_net_id != session_net_id:
        return None

    resolved_net_id = requested_net_id or session_net_id
    if not resolved_net_id:
        return None
    return User.query.filter_by(net_id=resolved_net_id).first()


def _stored_cluster_password_for_current_user(net_id: str = '') -> str:
    user = _hyperlink_credential_user(net_id)
    if user is None:
        return ''
    return user.get_cluster_password()


def _user_is_disabled(user) -> bool:
    return getattr(user, 'is_active', True) is False


@app.before_request
def _enforce_session_policy():
    _hyperlink_reap_stale_sessions()

    if not current_user.is_authenticated:
        return None

    if _user_is_disabled(current_user):
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
    elif path.startswith('/static/dist/'):
        response.headers.setdefault('Cache-Control', 'public, max-age=31536000, immutable')
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
                    user = User(name=net_id, net_id=net_id, is_active=True)
                    db.session.add(user)
                    flash('Cluster authentication succeeded. Your HPC Tools profile was created automatically.', 'success')

                if _user_is_disabled(user):
                    flash('Your account has been deactivated. Please contact administrator.', 'error')
                    return render_template('login.html', **_auth_template_context(cluster_target))

                # keep local password hash in sync with the current cluster password
                user.set_password(password)
                _persist_cluster_password(user, password)
                db.session.commit()

                login_user(user, remember=remember)
                session['hyperlink_net_id'] = user.net_id
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
            if _user_is_disabled(user):
                flash('Your account has been deactivated. Please contact administrator.', 'error')
                return render_template('login.html', **_auth_template_context(cluster_target))
            
            login_user(user, remember=remember)
            session['hyperlink_net_id'] = user.net_id
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
    
    recent_jobs = get_user_tool_jobs('kpi', include_all=True)
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


@app.route('/html/resim_run_submit', methods=['POST'])
@login_required
def resim_run_submit():
    """Submit a resim run job from the Runtime Map page."""
    return submit_tool_job('resim_run')


@app.route('/api/resim_run_submit', methods=['POST'])
@login_required
def api_resim_run_submit():
    """AJAX endpoint for Resim Run submission — runs .sh script via SSH as the logged-in user."""
    from utils import extract_project_from_path, cluster_from_path
    import shlex
    data = request.get_json(silent=True) or {}
    input_txt = (data.get('input_txt') or '').strip()
    simg_path = (data.get('simg_path') or '').strip()
    create_jira = data.get('create_jira') in (True, '1', 'true')
    jira_board = (data.get('jira_board') or '').strip() or 'FHW'
    jira_assignee = (data.get('jira_assignee') or '').strip()
    jira_notes = (data.get('jira_notes') or '').strip()

    if not input_txt:
        return jsonify({'ok': False, 'error': 'Input file (input.txt) path is required.'}), 400
    if not simg_path:
        return jsonify({'ok': False, 'error': 'Simg file path is required.'}), 400

    cluster_txt = cluster_from_path(input_txt)
    cluster_simg = cluster_from_path(simg_path)
    if not cluster_txt:
        return jsonify({'ok': False, 'error': 'Input file path must start with /net/ (Krakow) or /mnt/ (Southfield).'}), 400
    if not cluster_simg:
        return jsonify({'ok': False, 'error': 'Simg file path must start with /net/ (Krakow) or /mnt/ (Southfield).'}), 400
    if cluster_txt != cluster_simg:
        return jsonify({'ok': False, 'error': f'Both files must be in the same partition. Input is on {cluster_txt}, simg is on {cluster_simg}.'}), 400

    project_name, project_root = extract_project_from_path(input_txt)
    if not project_name:
        project_name = 'default'
    if not project_root:
        project_root = os.path.dirname(input_txt)

    # Root cause of "Log file not found": project_root / input dir can be
    # read-only for this user (e.g. another user's private NFS folder), which
    # made open(log_path, 'w') raise before the log file ever existed. Probe
    # real write access instead of trusting os.makedirs (a no-op on existing
    # dirs), and fall back to the app's own guaranteed-writable cache dir.
    fallback_log_dir = os.path.join(get_cache_dir(), 'resim_runs')
    log_dir = _first_writable_dir(os.path.dirname(input_txt), project_root, fallback_log_dir) or fallback_log_dir
    job_uuid = uuid.uuid4().hex[:8]
    log_path = os.path.join(log_dir, f'resim_run_{job_uuid}.log')

    resim_script_src = _resim_script_source_path()
    if not os.path.isfile(resim_script_src):
        return jsonify({'ok': False, 'error': f'rResim_Gen7.sh not found (looked at: {resim_script_src}).'}), 500

    # Fetch the logged-in user's stored cluster password
    user_password = _stored_cluster_password_for_current_user(current_user.net_id)
    if not user_password:
        return jsonify({'ok': False, 'error': 'No cluster password saved. Please submit a KPI/Interactive Plot job first to store your credentials.'}), 400

    # Write SSH_ASKPASS script in the log directory. Filename must be unique
    # per job: _write_askpass_script chmods it 0o500 (no write bit, even for
    # the owner), so a fixed shared filename would fail with EPERM on every
    # run after the first (open(..., 'w') needs write on the existing file).
    askpass_path = os.path.join(log_dir, f'.ssh_askpass_{job_uuid}.sh')
    _write_askpass_script(user_password, askpass_path)

    # The script also runs `module load slurm`, which requires the `module`
    # bash function from /etc/profile.d/modules.sh — not guaranteed to be
    # sourced in a non-interactive subprocess. SSH login shell will have it.
    # Also prepend xxd shim if available.
    bundle_root_env = (os.environ.get('HPCC_BUNDLE_ROOT') or '').strip()
    xxd_bin_dir = os.path.join(bundle_root_env, 'bin') if bundle_root_env else ''
    env_prefix = f"export PATH={shlex.quote(xxd_bin_dir)}:$PATH; " if xxd_bin_dir and os.path.isdir(xxd_bin_dir) else ''

    # SSH command: run as the logged-in user via their stored credentials.
    # -tt forces a remote pty (the vendor script prompts interactively, e.g.
    # "Proceed with Default Docker Configuration (Y/N)", and also relies on
    # isatty() for unbuffered output — without a pty its stdout is fully
    # buffered and nothing reaches the log until the (never-ending) prompt).
    # StrictHostKeyChecking=no/UserKnownHostsFile=/dev/null avoids an
    # interactive "are you sure you want to continue connecting" prompt on
    # the first-ever SSH to 127.0.0.1 for this user, which otherwise hangs
    # forever with no tty/askpass path to answer it.
    cmd = [
        'ssh', '-tt',
        '-o', 'StrictHostKeyChecking=no',
        '-o', 'UserKnownHostsFile=/dev/null',
        '-o', 'ConnectTimeout=20',
        f'{current_user.net_id}@127.0.0.1',
        'bash', '-lc',
        f"stty -echo 2>/dev/null; {env_prefix}cd {shlex.quote(project_root)} 2>/dev/null || cd {shlex.quote(log_dir)}; "
        f"{shlex.quote(resim_script_src)} {shlex.quote(input_txt)} {shlex.quote(simg_path)} highPrio"
    ]

    # Environment for SSH_ASKPASS
    env = {
        'SSH_ASKPASS': askpass_path,
        'SSH_ASKPASS_REQUIRE': 'force',
        'DISPLAY': ':0',
        'PATH': os.environ.get('PATH', ''),
    }

    job = JobHistory(
        user_id=current_user.id,
        tool_name='resim_run',
        input_path=input_txt,
        input_filename=os.path.basename(input_txt),
        output_path=project_root,
        output_log_path=log_path,
        parameters={
            'input_txt': input_txt,
            'simg_path': simg_path,
            'create_jira': create_jira,
            'jira_board': jira_board,
            'jira_assignee': jira_assignee,
            'jira_notes': jira_notes,
            'project': project_name,
            'project_root': project_root,
        },
        status='QUEUED',
    )
    db.session.add(job)
    db.session.commit()

    t = threading.Thread(
        target=_run_ssh_job_background,
        args=(job.id, cmd, env, log_path),
        kwargs={'askpass_path': askpass_path},
        daemon=True,
    )
    t.start()

    return jsonify({'ok': True, 'message': 'Submitted', 'job_id': job.id})


@app.route('/api/detect_project')
@login_required
def api_detect_project():
    """Detect project and cluster from a file path."""
    from utils import extract_project_from_path, cluster_from_path
    path = request.args.get('path', '').strip()
    if not path:
        return jsonify({'project': None, 'project_root': None, 'cluster': None})
    cluster = cluster_from_path(path)
    project_name, project_root = extract_project_from_path(path)
    return jsonify({
        'project': project_name,
        'project_root': project_root,
        'cluster': cluster,
    })


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
    """Get the current hyperlink user key.

    Prefer the authenticated platform login so saved pairs and cache paths remain
    stable even before a cluster session is opened.
    """
    if current_user.is_authenticated:
        return (current_user.net_id or '').strip()
    return (session.get('hyperlink_net_id', '') or '').strip()


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


def _hyperlink_overlay_tools():
    module = _load_hyperlink_module('hyperlink_html_online_main', 'main.py')
    return (
        getattr(module, 'find_overlay_video_name'),
        getattr(module, 'is_supported_video_file'),
        getattr(module, 'is_overlay_video_name'),
    )


def _hyperlink_saved_owner():
    owner_net_id = _hyperlink_get_user_key()
    if not owner_net_id:
        return '', None
    user_id = current_user.id if current_user.is_authenticated else None
    return owner_net_id, user_id


def _hyperlink_saved_pairs():
    owner_net_id, _ = _hyperlink_saved_owner()
    if not owner_net_id:
        return []
    return HyperlinkSavedPair.query.filter_by(owner_net_id=owner_net_id).order_by(
        HyperlinkSavedPair.last_used_at.desc(),
        HyperlinkSavedPair.id.desc(),
    ).all()


def _hyperlink_saved_pair_or_404(saved_id: int) -> HyperlinkSavedPair:
    owner_net_id, _ = _hyperlink_saved_owner()
    query = HyperlinkSavedPair.query.filter_by(id=saved_id)
    if owner_net_id:
        query = query.filter_by(owner_net_id=owner_net_id)
    return query.first_or_404()


def _hyperlink_saved_pair_cached(saved_pair: HyperlinkSavedPair) -> bool:
    _, html_dir, video_dir = _hyperlink_get_user_dirs()
    html_path = os.path.join(html_dir, saved_pair.html_folder)
    video_path = os.path.join(video_dir, saved_pair.video_name)
    return os.path.isdir(html_path) and os.path.isfile(video_path)


def _hyperlink_saved_overlay_cached(saved_pair: HyperlinkSavedPair) -> bool:
    _, _, video_dir = _hyperlink_get_user_dirs()
    overlay_name = (saved_pair.overlay_name or os.path.basename(saved_pair.remote_overlay_path or '')).strip()
    if not overlay_name:
        return False
    overlay_path = os.path.join(video_dir, overlay_name)
    return os.path.isfile(overlay_path)


def _hyperlink_remove_saved_pair_cache(saved_pair: HyperlinkSavedPair) -> None:
    _, html_dir, video_dir = _hyperlink_get_user_dirs()
    html_path = os.path.join(html_dir, saved_pair.html_folder)
    video_path = os.path.join(video_dir, saved_pair.video_name)
    overlay_name = (saved_pair.overlay_name or os.path.basename(saved_pair.remote_overlay_path or '')).strip()
    if os.path.isdir(html_path):
        shutil.rmtree(html_path, ignore_errors=True)
    if os.path.isfile(video_path):
        try:
            os.remove(video_path)
        except OSError:
            logger.warning('Could not remove cached hyperlink video %s', video_path)
    if overlay_name:
        overlay_path = os.path.join(video_dir, overlay_name)
        if os.path.isfile(overlay_path):
            try:
                os.remove(overlay_path)
            except OSError:
                logger.warning('Could not remove cached hyperlink overlay %s', overlay_path)


def _hyperlink_upsert_saved_pair(*, key: str, html_path: str, video_path: str, overlay_path: str = '', output_root: str) -> HyperlinkSavedPair:
    owner_net_id, user_id = _hyperlink_saved_owner()
    if not owner_net_id:
        raise ValueError('Unable to determine the current user for this hyperlink stream')

    folder_name = os.path.basename((html_path or '').rstrip('/'))
    video_name = os.path.basename(video_path or '')
    overlay_name = os.path.basename(overlay_path or '')
    now = datetime.utcnow()

    saved_pair = HyperlinkSavedPair.query.filter_by(
        owner_net_id=owner_net_id,
        remote_html_path=html_path,
        remote_video_path=video_path,
    ).first()

    if saved_pair is None:
        saved_pair = HyperlinkSavedPair(
            user_id=user_id,
            owner_net_id=owner_net_id,
            label=folder_name or key,
            pair_key=key,
            html_folder=folder_name or key,
            video_name=video_name,
            overlay_name=overlay_name or None,
            remote_html_path=html_path,
            remote_video_path=video_path,
            remote_overlay_path=overlay_path or None,
            output_root=output_root,
            last_used_at=now,
        )
    else:
        saved_pair.user_id = user_id
        saved_pair.label = folder_name or saved_pair.label or key
        saved_pair.pair_key = key or saved_pair.pair_key
        saved_pair.html_folder = folder_name or saved_pair.html_folder
        saved_pair.video_name = video_name or saved_pair.video_name
        saved_pair.overlay_name = overlay_name or saved_pair.overlay_name
        saved_pair.remote_overlay_path = overlay_path or saved_pair.remote_overlay_path
        saved_pair.output_root = output_root or saved_pair.output_root
        saved_pair.last_used_at = now

    db.session.add(saved_pair)
    db.session.commit()
    return saved_pair


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


def _hyperlink_open_cluster_session(user_key: str, *, cluster_name: str, password: str):
    host = _hyperlink_cluster_host(cluster_name)
    if not user_key or not host or not password:
        return None

    import paramiko

    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(
        hostname=host,
        username=user_key,
        password=password,
        timeout=20,
        auth_timeout=20,
        banner_timeout=20,
        allow_agent=False,
        look_for_keys=False,
    )
    sftp = client.open_sftp()

    with _HYPERLINK_SSH_LOCK:
        _hyperlink_close_session(user_key)
        _HYPERLINK_SSH_SESSIONS[user_key] = {
            'server': cluster_name,
            'host': host,
            'username': user_key,
            'last_used_at': time.time(),
            'ssh': client,
            'sftp': sftp,
        }
        return _HYPERLINK_SSH_SESSIONS[user_key]


def _hyperlink_restore_session(user_key: str, *, cluster_name: str):
    sess = _hyperlink_get_live_session(user_key)
    if sess and sess.get('sftp') is not None:
        return sess

    password = _stored_cluster_password_for_current_user(user_key).strip()
    if not password:
        return None

    try:
        return _hyperlink_open_cluster_session(user_key, cluster_name=cluster_name, password=password)
    except Exception:
        logger.exception('Failed to restore hyperlink cluster session for %s on %s', user_key, cluster_name)
        return None


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
    """Serve video files with explicit range handling and stable partial reads."""
    import mimetypes

    route_started = time.perf_counter()

    mime_type = mimetypes.guess_type(file_path)[0] or 'video/mp4'
    file_size = os.path.getsize(file_path)
    range_header = (request.headers.get('Range') or '').strip()
    start = 0
    end = max(file_size - 1, 0)
    status_code = 200

    if range_header.startswith('bytes='):
        range_value = range_header.split('=', 1)[1].split(',', 1)[0].strip()
        start_str, _, end_str = range_value.partition('-')
        try:
            if start_str:
                start = int(start_str)
                end = int(end_str) if end_str else end
            elif end_str:
                suffix_length = int(end_str)
                start = max(file_size - suffix_length, 0)
            else:
                raise ValueError('Invalid range')
        except ValueError:
            response = Response(status=416)
            response.headers['Content-Range'] = f'bytes */{file_size}'
            return response

        end = min(end, file_size - 1)
        if start < 0 or start >= file_size or start > end:
            response = Response(status=416)
            response.headers['Content-Range'] = f'bytes */{file_size}'
            return response
        status_code = 206

    content_length = max(end - start + 1, 0)

    if request.method == 'HEAD':
        response = Response(status=status_code, mimetype=mime_type)
    elif status_code == 206:
        read_started = time.perf_counter()
        with open(file_path, 'rb') as handle:
            handle.seek(start)
            data = handle.read(content_length)
        logger.info(
            'Hyperlink video range read %s bytes=%s-%s size=%s read_ms=%.1f',
            os.path.basename(file_path),
            start,
            end,
            len(data),
            (time.perf_counter() - read_started) * 1000,
        )
        response = Response(data, status=206, mimetype=mime_type)
        content_length = len(data)
    else:
        response = send_file(file_path, mimetype=mime_type, conditional=True, max_age=86400)

    response.headers['Accept-Ranges'] = 'bytes'
    response.headers['Content-Length'] = str(content_length)
    if status_code == 206:
        response.headers['Content-Range'] = f'bytes {start}-{end}/{file_size}'
    response.headers['Cache-Control'] = 'public, max-age=86400'
    logger.info(
        'Hyperlink video response ready %s method=%s status=%s range=%s bytes=%s elapsed_ms=%.1f',
        os.path.basename(file_path),
        request.method,
        status_code,
        request.headers.get('Range', ''),
        content_length,
        (time.perf_counter() - route_started) * 1000,
    )
    return response


def _hyperlink_browser_video_cache_path(file_path: str) -> str:
    cache_dir = os.path.join(os.path.dirname(file_path), '.browser_cache')
    os.makedirs(cache_dir, exist_ok=True)
    stem, _ = os.path.splitext(os.path.basename(file_path))
    return os.path.join(cache_dir, stem + '.browser.mp4')


def _hyperlink_video_marker_sample(file_path: str) -> bytes:
    sample_size = 2 * 1024 * 1024
    file_size = os.path.getsize(file_path)
    with open(file_path, 'rb') as handle:
        head = handle.read(min(sample_size, file_size))
        if file_size <= sample_size:
            return head
        handle.seek(max(file_size - sample_size, 0))
        tail = handle.read(sample_size)
    return head + tail


def _hyperlink_needs_browser_transcode(file_path: str) -> bool:
    sample = _hyperlink_video_marker_sample(file_path)
    if b'mp4v' not in sample:
        return False
    return b'avc1' not in sample and b'avc3' not in sample


def _hyperlink_prepare_browser_video(file_path: str) -> str:
    if not _hyperlink_needs_browser_transcode(file_path):
        return file_path

    browser_path = _hyperlink_browser_video_cache_path(file_path)
    source_mtime = os.path.getmtime(file_path)
    if os.path.exists(browser_path) and os.path.getmtime(browser_path) >= source_mtime:
        return browser_path

    ffmpeg_bin = shutil.which('ffmpeg') or '/usr/bin/ffmpeg'
    temp_path = browser_path + '.tmp.mp4'
    command = [
        ffmpeg_bin,
        '-y',
        '-loglevel',
        'error',
        '-i',
        file_path,
        '-map',
        '0:v:0',
        '-map',
        '0:a?',
        '-c:v',
        'libx264',
        '-preset',
        'veryfast',
        '-pix_fmt',
        'yuv420p',
        '-c:a',
        'aac',
        '-movflags',
        '+faststart',
        temp_path,
    ]

    logger.info('Transcoding browser-safe video for %s', os.path.basename(file_path))
    try:
        completed = subprocess.run(
            command,
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=1800,
        )
    except Exception:
        logger.exception('Failed to launch ffmpeg for %s', file_path)
        return file_path

    if completed.returncode != 0 or not os.path.exists(temp_path):
        logger.error(
            'ffmpeg transcode failed for %s: %s',
            file_path,
            (completed.stderr or completed.stdout or '').strip(),
        )
        return file_path

    os.replace(temp_path, browser_path)
    return browser_path


@app.route('/hyperlink/data/video/<path:filename>', methods=['GET', 'HEAD'])
def hyperlink_video_data(filename):
    """Serve video data files for viewer with streaming support"""
    try:
        request_started = time.perf_counter()
        _, _, video_dir = _hyperlink_get_user_dirs()
        file_path = os.path.join(video_dir, filename)
        if not os.path.exists(file_path):
            logger.error(f"Video file not found: {file_path}")
            return "File not found", 404
        file_path = _hyperlink_prepare_browser_video(file_path)
        logger.info(
            'Hyperlink video request resolved %s user=%s method=%s range=%s resolve_ms=%.1f',
            filename,
            _hyperlink_get_user_key(),
            request.method,
            request.headers.get('Range', ''),
            (time.perf_counter() - request_started) * 1000,
        )
        return _stream_video_file(file_path)
    except Exception as e:
        logger.error(f"Error serving video {filename}: {e}")
        return "Internal server error", 500


@app.route('/hyperlink/api/video-chunk/<path:filename>', methods=['GET'])
def hyperlink_video_chunk(filename):
    """Serve authenticated video chunks over a simple 200 response."""
    try:
        _, _, video_dir = _hyperlink_get_user_dirs()
        file_path = os.path.join(video_dir, filename)
        if not os.path.exists(file_path):
            logger.error(f"Video chunk file not found: {file_path}")
            return jsonify({'error': 'File not found'}), 404

        file_path = _hyperlink_prepare_browser_video(file_path)

        file_size = os.path.getsize(file_path)
        start = max(int(request.args.get('start', '0') or 0), 0)
        chunk_size = max(int(request.args.get('size', str(16 * 1024)) or (16 * 1024)), 1)
        end = min(start + chunk_size, file_size)

        if start >= file_size:
            return jsonify({'error': 'Range out of bounds'}), 416

        with open(file_path, 'rb') as handle:
            handle.seek(start)
            data = handle.read(end - start)

        response = Response(data, status=200, mimetype='application/octet-stream')
        response.headers['Content-Length'] = str(len(data))
        response.headers['Cache-Control'] = 'no-store'
        response.headers['X-File-Size'] = str(file_size)
        response.headers['X-Chunk-Start'] = str(start)
        response.headers['X-Chunk-End'] = str(end)
        return response
    except Exception as e:
        logger.error(f"Error serving video chunk {filename}: {e}")
        return jsonify({'error': 'Internal server error'}), 500


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
                entry['overlay_video'] = _hyperlink_rewrite_data_url(entry.get('overlay_video', ''))
                entry['comment_path'] = _hyperlink_rewrite_data_url(entry.get('comment_path', ''))
                entry['html_files'] = [_hyperlink_rewrite_data_url(x) for x in (entry.get('html_files') or [])]
                images = entry.get('images') or {}
                if isinstance(images, dict):
                    for sensor, positions in images.items():
                        if not isinstance(positions, dict):
                            continue
                        for pos, img_path in list(positions.items()):
                            positions[pos] = _hyperlink_rewrite_data_url(img_path)
            saved_pairs_by_key = {saved_pair.pair_key: saved_pair for saved_pair in _hyperlink_saved_pairs()}
            response_mapping = {}
            for raw_key, base_entry in (mapping or {}).items():
                if not isinstance(base_entry, dict):
                    continue
                entry = copy.deepcopy(base_entry)
                saved_pair = saved_pairs_by_key.get(raw_key)
                response_key = raw_key
                entry['saved_id'] = None
                entry['saved_key'] = raw_key
                entry['saved_label'] = entry.get('html_folder') or raw_key
                entry['cached'] = True
                entry['remote_overlay_path'] = entry.get('remote_overlay_path') or ''
                entry['overlay_cached'] = bool(entry.get('overlay_video'))
                entry['has_overlay'] = bool(entry.get('overlay_video'))
                if saved_pair is not None:
                    response_key = str(saved_pair.id)
                    _, _, video_dir = _hyperlink_get_user_dirs()
                    saved_overlay_name = (saved_pair.overlay_name or os.path.basename(saved_pair.remote_overlay_path or '')).strip()
                    saved_overlay_url = ''
                    if saved_overlay_name:
                        saved_overlay_path = os.path.join(video_dir, saved_overlay_name)
                        if os.path.isfile(saved_overlay_path):
                            saved_overlay_url = _hyperlink_rewrite_data_url('/data/video/' + saved_overlay_name.replace('\\', '/'))
                    entry['saved_id'] = saved_pair.id
                    entry['saved_key'] = saved_pair.pair_key
                    entry['saved_label'] = saved_pair.label or entry.get('html_folder') or saved_pair.html_folder
                    entry['html_folder'] = saved_pair.label or entry.get('html_folder') or saved_pair.html_folder
                    entry['cached'] = _hyperlink_saved_pair_cached(saved_pair)
                    entry['remote_html_path'] = saved_pair.remote_html_path
                    entry['remote_video_path'] = saved_pair.remote_video_path
                    entry['remote_overlay_path'] = saved_pair.remote_overlay_path or ''
                    if saved_overlay_name:
                        entry['overlay_video_name'] = saved_overlay_name
                    if saved_overlay_url:
                        entry['overlay_video'] = saved_overlay_url
                    entry['overlay_cached'] = _hyperlink_saved_overlay_cached(saved_pair)
                    entry['has_overlay'] = bool(entry.get('overlay_video') or saved_pair.remote_overlay_path)
                    entry['saved_at'] = saved_pair.created_at.isoformat() if saved_pair.created_at else None
                    entry['last_used_at'] = saved_pair.last_used_at.isoformat() if saved_pair.last_used_at else None
                response_mapping[str(response_key)] = entry
            mapping = response_mapping
            _hyperlink_store_mapping(cache_key, html_dir, video_dir, mapping)
            return jsonify({'success': True, 'mappings': mapping})
    except ImportError as e:
        logger.warning(f"Could not import LogViewerApp: {e}")
    
    return jsonify({'success': True, 'mappings': {}})


@app.route('/hyperlink/api/saved-logs/<int:saved_id>', methods=['DELETE'])
def hyperlink_delete_saved_log(saved_id: int):
    saved_pair = _hyperlink_saved_pair_or_404(saved_id)
    _hyperlink_remove_saved_pair_cache(saved_pair)
    db.session.delete(saved_pair)
    db.session.commit()
    _HYPERLINK_MAPPING_CACHE.pop(_hyperlink_cache_user_key(), None)
    return jsonify({'success': True})


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
    """Generate a text description for a video via Slurm job on the cluster
    (runs Gemma-4 VLM inside rag.simg on a compute node)."""
    try:
        if not _hyperlink_ai_enabled():
            return jsonify({
                'success': False,
                'status': 'disabled',
                'error': 'AI video description is disabled for this Hyperlink session.',
            }), 403

        data = request.get_json() or {}
        force = bool(data.get('force', False))

        user_key = _hyperlink_get_user_key()
        sess = _hyperlink_get_live_session(user_key)
        if not sess or not sess.get('ssh'):
            return jsonify({
                'success': False,
                'status': 'disconnected',
                'error': 'Not connected to a cluster. Please connect first.',
            }), 409

        remote_video_path = (data.get('video_path') or session.get('hyperlink_remote_video_path') or '').strip()
        if not remote_video_path:
            return jsonify({
                'success': False,
                'status': 'no_video',
                'error': 'No remote video path available.',
            }), 400

        from vlm_client import VlmJobClient
        client = VlmJobClient(sess['ssh'])
        result = client.process_video(remote_video_path, force=force, poll=False)

        # If .txt already existed on cluster, try to download it to local cache
        if result.get('status') == 'skipped':
            _, _, video_dir = _hyperlink_get_user_dirs()
            local_txt = os.path.join(video_dir, os.path.basename(result['text_path']))
            try:
                sftp = sess['sftp']
                sftp.get(result['text_path'], local_txt)
            except Exception:
                pass
            if result.get('description'):
                return jsonify(result), 200
            return jsonify(result), 200

        return jsonify(result), (200 if result.get('success') else 500)
    except Exception as e:
        logger.exception('VLM process error')
        return jsonify({'success': False, 'status': 'error', 'error': str(e)}), 500


@app.route('/hyperlink/api/vlm/job/<job_id>', methods=['GET'])
def hyperlink_vlm_job_status(job_id: str):
    """Check the status of a VLM Slurm job."""
    try:
        user_key = _hyperlink_get_user_key()
        sess = _hyperlink_get_live_session(user_key)
        if not sess or not sess.get('ssh'):
            return jsonify({'status': 'disconnected', 'error': 'Cluster session lost'}), 409

        from vlm_client import VlmJobClient
        client = VlmJobClient(sess['ssh'])
        state = client.check_job(job_id)
        return jsonify({'job_id': job_id, 'status': state})
    except Exception as e:
        return jsonify({'status': 'error', 'error': str(e)}), 500


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
    net_id = (current_user.net_id or '').strip() if current_user.is_authenticated else (user_key or '')
    credential_user = _hyperlink_credential_user(net_id)
    return jsonify({
        'connected': connected,
        'server': server,
        'net_id': net_id,
        'has_saved_password': bool(credential_user and credential_user.has_cluster_password()),
        'authenticated': bool(current_user.is_authenticated),
        'html_path': session.get('hyperlink_remote_html_path'),
        'video_path': session.get('hyperlink_remote_video_path'),
        'overlay_path': session.get('hyperlink_remote_overlay_path'),
        'output_root': session.get('hyperlink_output_root') or VIEWER_CACHE_DIR,
        'vlm_enabled': _hyperlink_ai_enabled(),
    })


@app.route('/hyperlink/api/vlm/status', methods=['GET'])
def hyperlink_vlm_status():
    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    connected = bool(sess and sess.get('ssh'))
    return jsonify({
        'enabled': _hyperlink_ai_enabled(),
        'connected': connected,
        'backend': 'gemma-4-vl (rag.simg via Slurm)',
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
    overlay_path = (data.get('overlay_path') or '').strip()
    output_path = (data.get('output_path') or '').strip()
    net_id = (data.get('net_id') or '').strip()
    password = (data.get('password') or '').strip()

    current_net_id = (current_user.net_id or '').strip() if current_user.is_authenticated else ''
    if current_net_id:
        if net_id and net_id != current_net_id:
            return jsonify({'success': False, 'error': 'Stream session Net ID must match the logged-in user'}), 403
        net_id = current_net_id
    if not net_id:
        net_id = (session.get('hyperlink_net_id') or '').strip()
    if not password:
        password = _stored_cluster_password_for_current_user(net_id).strip()

    if not html_path or not video_path:
        return jsonify({'success': False, 'error': 'Both remote HTML and Video paths are required'}), 400
    if not net_id:
        return jsonify({'success': False, 'error': 'Net ID is required'}), 400
    if not password:
        return jsonify({'success': False, 'error': 'Password required. Sign in through the dashboard again or enter it once for this stream session.'}), 400

    c1 = cluster_from_path(html_path)
    c2 = cluster_from_path(video_path)
    c3 = cluster_from_path(overlay_path) if overlay_path else None
    cluster_name = c1 or c2
    if not cluster_name or (c1 and c2 and c1 != c2) or (c3 and c3 != cluster_name):
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

        credential_user = _hyperlink_credential_user(net_id)
        if credential_user is None and net_id:
            credential_user = User.query.filter_by(net_id=net_id).first()
            if credential_user is None:
                credential_user = User(name=net_id, net_id=net_id)
                db.session.add(credential_user)
        if credential_user is not None:
            try:
                credential_user.set_password(password)
                _persist_cluster_password(credential_user, password)
                db.session.commit()
            except Exception:
                db.session.rollback()
                logger.exception('Failed to persist hyperlink cluster credentials for %s', net_id)
        
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
        if overlay_path:
            session['hyperlink_remote_overlay_path'] = overlay_path
        else:
            session.pop('hyperlink_remote_overlay_path', None)
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


def _hyperlink_resolve_overlay_remote_path(sftp, overlay_path: str, *, log_name: str, primary_video_name: str):
    import stat

    resolved_path = (overlay_path or '').strip()
    if not resolved_path:
        raise ValueError('Overlay path is required')

    try:
        attr = sftp.stat(resolved_path)
    except Exception as exc:
        raise ValueError(f'Overlay path not found: {resolved_path}') from exc

    if not stat.S_ISDIR(attr.st_mode):
        return resolved_path, os.path.basename(resolved_path.rstrip('/'))

    find_overlay_video_name, is_supported_video_file, is_overlay_video_name = _hyperlink_overlay_tools()
    overlay_names = []
    for item in _hyperlink_sftp_list_dir(sftp, resolved_path):
        name = item.get('name') or ''
        if item.get('is_dir') or not is_supported_video_file(name) or not is_overlay_video_name(name):
            continue
        overlay_names.append(name)

    overlay_name = find_overlay_video_name(log_name, primary_video_name, overlay_names)
    if not overlay_name and len(overlay_names) == 1:
        overlay_name = overlay_names[0]
    if not overlay_name:
        raise ValueError(f'No matching overlay video found in directory: {resolved_path}')

    return f"{resolved_path.rstrip('/')}/{overlay_name}", overlay_name


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
    overlay_path = (data.get('overlay_path') or '').strip()
    output_path = (data.get('output_path') or '').strip()

    if not html_path or not video_path:
        return jsonify({'success': False, 'error': 'Both remote HTML and Video paths are required'}), 400

    if output_path:
        session['hyperlink_output_root'] = _hyperlink_resolve_output_root(output_path)
    session['hyperlink_remote_html_path'] = html_path
    session['hyperlink_remote_video_path'] = video_path
    if overlay_path:
        session['hyperlink_remote_overlay_path'] = overlay_path
    else:
        session.pop('hyperlink_remote_overlay_path', None)

    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    sftp = sess.get('sftp') if sess else None

    if sftp is None:
        cluster_name = cluster_from_path(html_path) or cluster_from_path(video_path)
        sess = _hyperlink_restore_session(user_key, cluster_name=cluster_name)
        sftp = sess.get('sftp') if sess else None

    if sftp is None:
        return jsonify({'success': False, 'error': 'Not connected'}), 400

    try:
        find_overlay_video_name, is_supported_video_file, is_overlay_video_name = _hyperlink_overlay_tools()
        html_items = _hyperlink_sftp_list_dir(sftp, html_path)
        video_items = _hyperlink_sftp_list_dir(sftp, video_path)
        overlay_items = _hyperlink_sftp_list_dir(sftp, overlay_path) if overlay_path else []

        html_folders = {item['name']: item for item in html_items if item.get('is_dir')}
        video_files = {}
        for item in video_items:
            name = item.get('name') or ''
            if item.get('is_dir') or not is_supported_video_file(name) or is_overlay_video_name(name):
                continue
            key = _hyperlink_key_from_name(name)
            if key:
                video_files[key] = name

        overlay_names = []
        for item in overlay_items:
            name = item.get('name') or ''
            if item.get('is_dir') or not is_supported_video_file(name) or not is_overlay_video_name(name):
                continue
            overlay_names.append(name)

        output_root, html_dir, video_dir = _hyperlink_get_user_dirs()
        saved_pairs = {
            (item.remote_html_path, item.remote_video_path): item
            for item in _hyperlink_saved_pairs()
        }
        logs = {}
        for folder_name in html_folders:
            key = _hyperlink_key_from_name(folder_name)
            if not key:
                continue
            if key not in video_files:
                continue

            remote_html_path = f"{html_path.rstrip('/')}/{folder_name}"
            remote_video_path = f"{video_path.rstrip('/')}/{video_files[key]}"
            saved_pair = saved_pairs.get((remote_html_path, remote_video_path))
            saved_overlay_name = ''
            saved_overlay_path = ''
            if saved_pair is not None:
                saved_overlay_name = (saved_pair.overlay_name or os.path.basename(saved_pair.remote_overlay_path or '')).strip()
                saved_overlay_path = (saved_pair.remote_overlay_path or '').strip()

            overlay_name = find_overlay_video_name(folder_name, video_files[key], overlay_names)
            remote_overlay_path = f"{overlay_path.rstrip('/')}/{overlay_name}" if overlay_path and overlay_name else ''
            if not overlay_name and saved_overlay_name:
                overlay_name = saved_overlay_name
            if not remote_overlay_path and saved_overlay_path:
                remote_overlay_path = saved_overlay_path

            cached_html = os.path.join(html_dir, folder_name)
            cached_video = os.path.join(video_dir, video_files[key])
            is_cached = os.path.isdir(cached_html) and os.path.isfile(cached_video)
            overlay_cached = bool(overlay_name) and os.path.isfile(os.path.join(video_dir, overlay_name))

            logs[key] = {
                'name': folder_name,
                'html_path': remote_html_path,
                'video_path': remote_video_path,
                'video_name': video_files[key],
                'overlay_path': remote_overlay_path,
                'overlay_name': overlay_name,
                'overlay_cached': overlay_cached,
                'has_overlay': bool(overlay_name or remote_overlay_path),
                'cached': is_cached,
                'saved': saved_pair is not None,
                'saved_id': saved_pair.id if saved_pair is not None else None,
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
    overlay_path = (data.get('overlay_path') or '').strip()
    output_path = (data.get('output_path') or '').strip()

    if not key or not html_path or not video_path:
        return jsonify({'success': False, 'error': 'Missing required fields'}), 400

    if output_path:
        session['hyperlink_output_root'] = _hyperlink_resolve_output_root(output_path)

    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    sftp = sess.get('sftp') if sess else None

    if sftp is None:
        cluster_name = cluster_from_path(html_path) or cluster_from_path(video_path)
        sess = _hyperlink_restore_session(user_key, cluster_name=cluster_name)
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
        overlay_name = os.path.basename(overlay_path) if overlay_path else ''
        if overlay_path:
            local_overlay_file = os.path.join(video_dir, overlay_name)
            os.makedirs(os.path.dirname(local_overlay_file), exist_ok=True)
            sftp.get(overlay_path, local_overlay_file)

        _HYPERLINK_PROGRESS[user_key] = {'active': False, 'phase': 'complete', 'message': 'Done'}
        saved_pair = _hyperlink_upsert_saved_pair(
            key=key,
            html_path=html_path,
            video_path=video_path,
            overlay_path=overlay_path,
            output_root=output_root,
        )
        _HYPERLINK_MAPPING_CACHE.pop(_hyperlink_cache_user_key(), None)

        return jsonify({
            'success': True,
            'message': 'Added to stream',
            'key': str(saved_pair.id),
            'saved_id': saved_pair.id,
            'overlay_name': overlay_name,
            'overlay_path': overlay_path,
            'output_root': output_root,
        })
    except Exception as e:
        if user_key:
            _HYPERLINK_PROGRESS[user_key] = {'active': False, 'phase': 'error', 'message': str(e)}
        return jsonify({'success': False, 'error': str(e)}), 500


@app.route('/hyperlink/api/saved-logs/<int:saved_id>/overlay', methods=['POST'])
def hyperlink_attach_saved_overlay(saved_id: int):
    saved_pair = _hyperlink_saved_pair_or_404(saved_id)
    data = request.get_json() or {}
    overlay_path = (data.get('overlay_path') or '').strip()
    if not overlay_path:
        return jsonify({'success': False, 'error': 'Overlay path is required'}), 400

    cluster_name = cluster_from_path(saved_pair.remote_html_path) or cluster_from_path(saved_pair.remote_video_path)
    overlay_cluster = cluster_from_path(overlay_path)
    if overlay_cluster and cluster_name and overlay_cluster != cluster_name:
        return jsonify({'success': False, 'error': 'Overlay path must be on the same cluster as the saved log'}), 400
    cluster_name = cluster_name or overlay_cluster
    if not cluster_name:
        return jsonify({'success': False, 'error': 'Could not determine which cluster to use for this overlay'}), 400

    user_key = _hyperlink_get_user_key()
    sess = _hyperlink_get_live_session(user_key)
    sftp = sess.get('sftp') if sess and sess.get('server') == cluster_name else None

    if sftp is None:
        sess = _hyperlink_restore_session(user_key, cluster_name=cluster_name)
        sftp = sess.get('sftp') if sess else None

    if sftp is None:
        return jsonify({'success': False, 'error': 'Not connected'}), 400

    try:
        _, _, video_dir = _hyperlink_get_user_dirs()
        log_name = saved_pair.html_folder or os.path.basename((saved_pair.remote_html_path or '').rstrip('/'))
        primary_video_name = saved_pair.video_name or os.path.basename(saved_pair.remote_video_path or '')
        overlay_path, overlay_name = _hyperlink_resolve_overlay_remote_path(
            sftp,
            overlay_path,
            log_name=log_name,
            primary_video_name=primary_video_name,
        )
        local_overlay_file = os.path.join(video_dir, overlay_name)
        os.makedirs(os.path.dirname(local_overlay_file), exist_ok=True)
        sftp.get(overlay_path, local_overlay_file)

        saved_pair.overlay_name = overlay_name
        saved_pair.remote_overlay_path = overlay_path
        saved_pair.last_used_at = datetime.utcnow()
        db.session.add(saved_pair)
        db.session.commit()
        _HYPERLINK_MAPPING_CACHE.pop(_hyperlink_cache_user_key(), None)

        return jsonify({
            'success': True,
            'saved_id': saved_pair.id,
            'overlay_name': overlay_name,
            'overlay_path': overlay_path,
        })
    except Exception as e:
        db.session.rollback()
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


def get_user_tool_jobs(tool_name: str, limit: int = 20, include_all: bool = False):
    """Get recent jobs for a specific tool."""
    query = JobHistory.query.filter_by(tool_name=tool_name)
    if not include_all:
        query = query.filter_by(user_id=current_user.id)
    jobs = query.order_by(JobHistory.created_at.desc()).limit(limit).all()
    for job in jobs:
        _sync_runtime_job(job)
    return jobs


def _job_owner(job: JobHistory) -> str:
    if getattr(job, 'user', None) is not None and job.user is not None:
        return job.user.net_id
    parameters = job.parameters if isinstance(job.parameters, dict) else {}
    return str(parameters.get('requested_by') or parameters.get('owner_net_id') or 'unknown')


def _can_manage_job(job: JobHistory) -> bool:
    return bool(job.user_id == current_user.id or getattr(current_user, 'role', 'user') == 'admin')


def _get_viewable_job(job_id: int) -> JobHistory:
    return JobHistory.query.get_or_404(job_id)


def _get_manageable_job(job_id: int) -> JobHistory:
    job = _get_viewable_job(job_id)
    if not _can_manage_job(job):
        abort(403)
    return job


def _build_runtime_submit_payload(tool_key: str, mode: str, paths: dict, resources: dict) -> dict:
    user_password = _stored_cluster_password_for_current_user(current_user.net_id) if _cluster_auth_enabled() else ''
    return {
        'tool_key': tool_key,
        'variant': tool_key,
        'mode': mode,
        'paths': paths,
        'resources': resources,
        'session_id': session.get('_id', ''),
        'user': current_user.net_id,
        'user_password': user_password,
    }


def _build_runtime_kpi_submission(form_source) -> dict:
    primary_target = (form_source.get('execution_target') or 'udp_kpi').strip()
    if primary_target not in {'can_kpi', 'udp_kpi'}:
        primary_target = 'udp_kpi'

    interactive_plot_mode = (form_source.get('interactive_plot_mode') or 'disabled').strip().lower()
    if interactive_plot_mode not in {'disabled', 'enabled', 'only'}:
        interactive_plot_mode = 'disabled'
    interactive_plot_enabled = interactive_plot_mode == 'enabled'
    interactive_plot_only = interactive_plot_mode == 'only'
    execution_target = 'interactive_plot' if interactive_plot_only else primary_target

    input_mode = (form_source.get('input_mode') or 'json').strip().lower()
    resource_target = 'interactive_plot' if interactive_plot_mode != 'disabled' else execution_target
    default_resources = _default_runtime_resources(resource_target)
    requested_scheduler = (form_source.get('scheduler') or default_resources.get('scheduler') or 'slurm').strip().lower()
    if requested_scheduler not in {'slurm', 'local'}:
        requested_scheduler = 'slurm'
    if requested_scheduler == 'local' and not _allow_local_kpi_scheduler():
        raise ValueError('Local KPI execution is disabled on the login node. Use Slurm or the HPCC bundle launchers.')

    resources = {
        'scheduler': requested_scheduler,
        'memory': (form_source.get('memory') or default_resources.get('memory', '32G')).strip(),
        'cpus': int(form_source.get('cpus') or default_resources.get('cpus', 8)),
        'time_limit': (form_source.get('time_limit') or default_resources.get('time_limit', '02:00:00')).strip(),
        'partition': (form_source.get('partition') or default_resources.get('partition', 'plcyf-com')).strip(),
        'account': (form_source.get('account') or default_resources.get('account', app.config.get('SLURM_ACCOUNT', 'RNA-SDV-SRR7'))).strip(),
        'qos': (form_source.get('qos') or default_resources.get('qos', app.config.get('SLURM_QOS', ''))).strip(),
        'nodes': int(form_source.get('nodes') or default_resources.get('nodes', 1)),
        'ntasks': int(form_source.get('ntasks') or default_resources.get('ntasks', 1)),
        'immediate': str(form_source.get('immediate') or default_resources.get('immediate', '')).strip(),
    }
    if resources['scheduler'] != 'slurm':
        resources['immediate'] = ''

    output_dir = (form_source.get('output_dir') or '').strip()
    paths = {
        'input_mode': input_mode,
        'output_dir': output_dir,
        'interactive_plot_mode': interactive_plot_mode,
        'interactive_source_target': primary_target,
    }
    config_xml = (form_source.get('config_xml') or '').strip()
    json_path = (form_source.get('kpi_json') or '').strip()
    input_hdf = (form_source.get('input_hdf') or '').strip()
    output_hdf = (form_source.get('output_hdf') or '').strip()

    if input_mode == 'json' and not json_path:
        raise ValueError('JSON path is required for the selected KPI submission mode.')
    if input_mode == 'hdf' and (not input_hdf or not output_hdf):
        raise ValueError('Input and output HDF paths are required for HDF mode.')

    paths.update(
        {
            'config_xml': config_xml,
            'json_path': json_path,
            'input_hdf': input_hdf,
            'output_hdf': output_hdf,
        }
    )
    optional_config = (form_source.get('optional_config') or '').strip()
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

    primary_input = json_path or input_hdf
    jira_fields = {
        'create_jira': form_source.get('create_jira') in ('1', 'true', True),
        'jira_board': (form_source.get('jira_board') or 'FHW').strip(),
        'jira_assignee': (form_source.get('jira_assignee') or '').strip(),
        'jira_notes': (form_source.get('jira_notes') or '').strip(),
    }
    return {
        'execution_target': execution_target,
        'input_mode': input_mode,
        'paths': paths,
        'resources': resources,
        'primary_input': primary_input,
        'execution_label': execution_label,
        'primary_target': primary_target,
        'interactive_plot_mode': interactive_plot_mode,
        'jira_fields': jira_fields,
    }


def _find_user_runtime_history(runtime_job_id: int) -> Optional[JobHistory]:
    candidates = JobHistory.query.filter_by(user_id=current_user.id).order_by(JobHistory.id.desc()).limit(200).all()
    for candidate in candidates:
        parameters = candidate.parameters if isinstance(candidate.parameters, dict) else {}
        if int(parameters.get('runtime_job_id') or 0) == runtime_job_id:
            _sync_runtime_job(candidate)
            return candidate
    return None


def _create_reused_history_job(runtime_job: dict, submission: dict) -> JobHistory:
    runtime_job_id = int(runtime_job['id'])
    existing = _find_user_runtime_history(runtime_job_id)
    if existing is not None:
        return existing

    request_payload = runtime_job.get('request') if isinstance(runtime_job.get('request'), dict) else {}
    runtime_console = request_payload.get('_console') if isinstance(request_payload.get('_console'), dict) else {}
    mirror_log_path = str(runtime_job.get('mirror_log_path') or runtime_job.get('log_path') or '').strip()
    if runtime_job.get('mirror_log_path'):
        runtime_console.setdefault('mirror_log_path', runtime_job['mirror_log_path'])
    if runtime_job.get('log_path'):
        runtime_console.setdefault('source_log_path', runtime_job['log_path'])

    parameters = {
        'runtime_job_id': runtime_job_id,
        'execution_target': submission['execution_target'],
        'execution_label': submission['execution_label'],
        'mode': submission['input_mode'],
        'paths': submission['paths'],
        'resources': submission['resources'],
        'runtime_console': runtime_console,
        'runtime_status_detail': runtime_job.get('status_detail', ''),
        'requested_by': runtime_job.get('requested_by') or 'unknown',
        'request_fingerprint': runtime_job.get('request_fingerprint') or '',
        'execution_path': runtime_job.get('execution_path') or '',
        'tool_version': runtime_job.get('tool_version') or '',
        'db_version': runtime_job.get('db_version') or '',
        'mirror_log_path': mirror_log_path,
        'runtime_artifacts': runtime_job.get('artifacts', []),
        'shared_result': True,
        'reused_from_runtime_job_id': runtime_job_id,
    }

    job = JobHistory(
        user_id=current_user.id,
        tool_name='kpi',
        input_path=submission['primary_input'],
        input_filename=extract_hdf_filename(submission['primary_input']),
        output_path=runtime_job.get('output_path', ''),
        output_log_path=mirror_log_path,
        status=runtime_job.get('status', 'COMPLETED'),
        error_message=runtime_job.get('error_message') or None,
        parameters=parameters,
        started_at=_parse_runtime_datetime(runtime_job.get('started_at') or ''),
        completed_at=_parse_runtime_datetime(runtime_job.get('completed_at') or ''),
    )
    db.session.add(job)
    db.session.commit()
    return job


def _serialize_runtime_match(runtime_job: dict, execution_label: str) -> dict:
    artifacts = runtime_job.get('artifacts') if isinstance(runtime_job.get('artifacts'), list) else []
    artifact_paths = [artifact.get('artifact_path', '') for artifact in artifacts if artifact.get('exists')]
    return {
        'runtime_job_id': runtime_job.get('id'),
        'status': runtime_job.get('status', ''),
        'requested_by': runtime_job.get('requested_by', 'unknown'),
        'execution_label': execution_label,
        'output_path': runtime_job.get('output_path', ''),
        'log_path': runtime_job.get('mirror_log_path') or runtime_job.get('log_path') or '',
        'execution_path': runtime_job.get('execution_path', ''),
        'tool_version': runtime_job.get('tool_version', ''),
        'db_version': runtime_job.get('db_version', ''),
        'artifact_count': len(artifact_paths),
        'artifact_paths': artifact_paths[:5],
    }


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

    mirror_log_path = (console.get('mirror_log_path') or (runtime_job or {}).get('mirror_log_path') or '').strip()
    source_log_path = (console.get('log_path') or (runtime_job or {}).get('log_path') or job.output_log_path or '').strip()
    if mirror_log_path:
        console['mirror_log_path'] = mirror_log_path
    if source_log_path:
        console['source_log_path'] = source_log_path

    log_path = mirror_log_path or source_log_path
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
    if runtime_job.get('mirror_log_path'):
        runtime_console.setdefault('mirror_log_path', runtime_job['mirror_log_path'])
    if runtime_job.get('log_path'):
        runtime_console.setdefault('source_log_path', runtime_job['log_path'])
    job.status = runtime_job.get('status', job.status)
    job.output_log_path = runtime_job.get('mirror_log_path') or runtime_console.get('mirror_log_path') or runtime_console.get('log_path') or runtime_job.get('log_path') or job.output_log_path
    job.output_path = runtime_job.get('output_path') or job.output_path
    job.error_message = runtime_job.get('error_message') or job.error_message
    if runtime_console or runtime_job.get('status_detail'):
        parameters = dict(job.parameters or {})
        parameters['runtime_console'] = runtime_console
        parameters['requested_by'] = runtime_job.get('requested_by') or parameters.get('requested_by') or _job_owner(job)
        parameters['request_fingerprint'] = runtime_job.get('request_fingerprint') or parameters.get('request_fingerprint', '')
        parameters['execution_path'] = runtime_job.get('execution_path') or parameters.get('execution_path', '')
        parameters['tool_version'] = runtime_job.get('tool_version') or parameters.get('tool_version', '')
        parameters['db_version'] = runtime_job.get('db_version') or parameters.get('db_version', '')
        parameters['mirror_log_path'] = runtime_job.get('mirror_log_path') or parameters.get('mirror_log_path', '')
        parameters['runtime_artifacts'] = runtime_job.get('artifacts') or parameters.get('runtime_artifacts', [])
        if runtime_job.get('status_detail'):
            parameters['runtime_status_detail'] = runtime_job['status_detail']
        else:
            parameters.pop('runtime_status_detail', None)
        job.parameters = parameters
    if runtime_job.get('started_at') and not job.started_at:
        parsed_started_at = _parse_runtime_datetime(runtime_job['started_at'])
        if parsed_started_at is not None:
            job.started_at = parsed_started_at
    if runtime_job.get('completed_at'):
        parsed_completed_at = _parse_runtime_datetime(runtime_job['completed_at'])
        if parsed_completed_at is not None:
            job.completed_at = parsed_completed_at
    db.session.commit()
    return runtime_job


def _parse_runtime_datetime(raw_value: str):
    candidate = (raw_value or '').strip()
    if not candidate:
        return None

    normalized = candidate.replace('Z', '+00:00')
    parser = getattr(datetime, 'fromisoformat', None)
    if parser is not None:
        try:
            parsed = parser(normalized)
        except ValueError:
            parsed = None
    else:
        parsed = None

    if parsed is None:
        if len(normalized) >= 6 and normalized[-6] in '+-' and normalized[-3] == ':':
            normalized = normalized[:-3] + normalized[-2:]
        formats = (
            '%Y-%m-%dT%H:%M:%S.%f%z',
            '%Y-%m-%dT%H:%M:%S%z',
            '%Y-%m-%d %H:%M:%S.%f%z',
            '%Y-%m-%d %H:%M:%S%z',
            '%Y-%m-%dT%H:%M:%S.%f',
            '%Y-%m-%dT%H:%M:%S',
            '%Y-%m-%d %H:%M:%S.%f',
            '%Y-%m-%d %H:%M:%S',
        )
        for fmt in formats:
            try:
                parsed = datetime.strptime(normalized, fmt)
                break
            except ValueError:
                continue

    if parsed is None:
        return None
    if parsed.tzinfo is None:
        return parsed
    return parsed.astimezone(timezone.utc).replace(tzinfo=None)


def _coerce_runtime_resources(resources: dict, fallback_tool_key: str) -> dict:
    fallback_key = fallback_tool_key if fallback_tool_key in RUNTIME_TOOL_DEFAULTS else 'interactive_plot'
    merged = dict(_default_runtime_resources(fallback_key))
    if isinstance(resources, dict):
        for key, value in resources.items():
            if value in (None, ''):
                continue
            merged[key] = value

    scheduler = str(merged.get('scheduler') or 'slurm').strip().lower()
    if scheduler not in {'slurm', 'local'}:
        scheduler = 'slurm'
    merged['scheduler'] = scheduler

    for key in ('cpus', 'nodes', 'ntasks'):
        default_value = _default_runtime_resources(fallback_key).get(key, 1)
        try:
            merged[key] = int(merged.get(key, default_value))
        except (TypeError, ValueError):
            merged[key] = int(default_value)

    for key in ('memory', 'time_limit', 'partition', 'account', 'qos', 'immediate'):
        default_value = _default_runtime_resources(fallback_key).get(key, '')
        merged[key] = str(merged.get(key, default_value) or '').strip()

    if merged['scheduler'] != 'slurm':
        merged['immediate'] = ''
    return merged


def _queue_runtime_job(tool_key: str, mode: str, paths: dict, resources: dict, job_tool_name: str, input_path: str, execution_label: str = '', jira_fields: Optional[dict] = None):
    payload = _build_runtime_submit_payload(tool_key, mode, paths, resources)
    result = broker_client.submit_job(payload)
    resolved_execution_label = execution_label or tool_key
    runtime_job = None
    try:
        runtime_job = broker_client.get_status(int(result['job_id'])).get('job')
    except Exception:
        runtime_job = runtime_store.get_job(int(result['job_id']))
    runtime_console = dict(result.get('console', {}))
    if isinstance(runtime_job, dict) and runtime_job.get('mirror_log_path'):
        runtime_console.setdefault('mirror_log_path', runtime_job['mirror_log_path'])
    if isinstance(runtime_job, dict) and runtime_job.get('log_path'):
        runtime_console.setdefault('source_log_path', runtime_job['log_path'])

    job_params = {
        'runtime_job_id': result['job_id'],
        'execution_target': tool_key,
        'execution_label': resolved_execution_label,
        'mode': mode,
        'paths': paths,
        'resources': resources,
        'runtime_console': runtime_console,
        'requested_by': (runtime_job or {}).get('requested_by', current_user.net_id),
        'request_fingerprint': (runtime_job or {}).get('request_fingerprint', ''),
        'execution_path': (runtime_job or {}).get('execution_path', ''),
        'tool_version': (runtime_job or {}).get('tool_version', ''),
        'db_version': (runtime_job or {}).get('db_version', ''),
        'runtime_artifacts': (runtime_job or {}).get('artifacts', []),
        'runtime_status_detail': result.get('status_detail', ''),
    }
    if jira_fields:
        job_params.update(jira_fields)

    job = JobHistory(
        user_id=current_user.id,
        tool_name=job_tool_name,
        input_path=input_path,
        input_filename=extract_hdf_filename(input_path),
        output_path=result.get('output_path', ''),
        output_log_path=(runtime_job or {}).get('mirror_log_path') or result.get('log_path', ''),
        parameters=job_params,
        status='QUEUED',
    )
    db.session.add(job)
    db.session.commit()
    return job, result, resolved_execution_label


def _submit_runtime_job(tool_key: str, mode: str, paths: dict, resources: dict, job_tool_name: str, input_path: str, execution_label: str = '', jira_fields: Optional[dict] = None):
    job, result, resolved_execution_label = _queue_runtime_job(
        tool_key,
        mode,
        paths,
        resources,
        job_tool_name,
        input_path,
        execution_label,
        jira_fields=jira_fields or {},
    )
    if result.get('status_detail'):
        flash(result['status_detail'], 'warning')
    flash(f'{resolved_execution_label} request queued through HPCC broker. Runtime job: {result["job_id"]}', 'success')
    return redirect(url_for('dashboard'))


def _rerun_history_job(job: JobHistory):
    parameters = dict(job.parameters or {})

    if job.tool_name == 'hyperlink_tool':
        html_root = (parameters.get('html_root') or job.input_path or '').strip()
        output_root = (job.output_path or parameters.get('output_root') or '').strip()
        video_root = (parameters.get('video_root') or '').strip()
        if not html_root:
            raise ValueError('Hyperlink history is missing the saved HTML root, so it cannot be re-opened.')

        session['hyperlink_html_root'] = html_root
        session['hyperlink_output_root'] = output_root
        session['hyperlink_video_root'] = video_root
        session['hyperlink_ai_enabled'] = bool(parameters.get('hyperlink_ai_enabled'))

        restored_job = JobHistory(
            user_id=current_user.id,
            tool_name='hyperlink_tool',
            input_path=html_root,
            input_filename=extract_hdf_filename(html_root),
            output_path=output_root,
            parameters={
                'html_root': html_root,
                'video_root': video_root,
                'hyperlink_ai_enabled': _hyperlink_ai_enabled(),
            },
            status='COMPLETED',
            started_at=datetime.utcnow(),
            completed_at=datetime.utcnow(),
        )
        db.session.add(restored_job)
        db.session.commit()
        return {
            'job': restored_job,
            'message': 'Hyperlink session restored.',
            'redirect_url': url_for('hyperlink_viewer_app'),
        }

    paths = parameters.get('paths') if isinstance(parameters.get('paths'), dict) else None
    if not paths and job.tool_name == 'interactive_plot':
        legacy_parameters = dict(parameters)
        input_mode = (legacy_parameters.get('input_mode') or ('json' if legacy_parameters.get('inputs_json') else 'hdf')).strip().lower()
        paths = {
            'input_mode': input_mode,
            'output_dir': (job.output_path or legacy_parameters.get('output_dir') or '').strip(),
            'config_xml': (legacy_parameters.get('config_xml') or '').strip(),
            'json_path': (legacy_parameters.get('inputs_json') or '').strip(),
            'input_hdf': (legacy_parameters.get('input_hdf') or '').strip(),
            'output_hdf': (legacy_parameters.get('output_hdf') or '').strip(),
        }
        optional_config = (legacy_parameters.get('optional_config') or legacy_parameters.get('plot_config') or '').strip()
        if optional_config:
            paths['optional_config'] = optional_config
        parameters = {
            **parameters,
            'paths': paths,
            'mode': input_mode,
            'execution_target': 'interactive_plot',
            'execution_label': 'Interactive Plot',
        }

    if not paths:
        raise ValueError('Re-trigger is only available for broker-backed KPI, Interactive Plot, and Hyperlink history rows.')

    execution_target = str(parameters.get('execution_target') or '').strip()
    if not execution_target:
        if job.tool_name == 'kpi':
            interactive_mode = (paths.get('interactive_plot_mode') or 'disabled').strip().lower()
            source_target = (paths.get('interactive_source_target') or 'udp_kpi').strip().lower()
            execution_target = 'interactive_plot' if interactive_mode == 'only' else source_target
        elif job.tool_name == 'interactive_plot':
            execution_target = 'interactive_plot'
        else:
            execution_target = job.tool_name

    if execution_target not in {'can_kpi', 'udp_kpi', 'interactive_plot', 'hyperlink'}:
        raise ValueError(f'Re-trigger is not available for {execution_target or job.tool_name}.')

    input_path = (job.input_path or paths.get('json_path') or paths.get('input_hdf') or paths.get('html_root') or '').strip()
    if not input_path:
        raise ValueError('This history row does not have enough saved input data to be re-triggered.')

    mode = str(parameters.get('mode') or paths.get('input_mode') or 'json').strip().lower()
    fallback_tool_key = execution_target if execution_target in RUNTIME_TOOL_DEFAULTS else 'interactive_plot'
    resources = _coerce_runtime_resources(parameters.get('resources') or {}, fallback_tool_key)
    execution_label = str(parameters.get('execution_label') or '').strip()
    rerun_job, result, resolved_execution_label = _queue_runtime_job(
        execution_target,
        mode,
        paths,
        resources,
        job.tool_name,
        input_path,
        execution_label,
    )
    return {
        'job': rerun_job,
        'message': f'{resolved_execution_label} request queued through HPCC broker. Runtime job: {result["job_id"]}',
        'redirect_url': url_for('view_job_log', job_id=rerun_job.id),
    }


def submit_runtime_kpi_job():
    try:
        submission = _build_runtime_kpi_submission(request.form)
    except ValueError as exc:
        flash(str(exc), 'error')
        return redirect(request.referrer or url_for('tool_kpi'))

    reuse_decision = (request.form.get('reuse_decision') or '').strip().lower()
    submit_payload = _build_runtime_submit_payload(
        submission['execution_target'],
        submission['input_mode'],
        submission['paths'],
        submission['resources'],
    )
    if reuse_decision != 'rerun':
        existing_runtime_job = runtime_store.find_reusable_job(
            submission['execution_target'],
            submission['input_mode'],
            submit_payload,
        )
        if existing_runtime_job is not None:
            shared_job = _create_reused_history_job(existing_runtime_job, submission)
            flash(
                f"Existing {submission['execution_label']} result from {existing_runtime_job.get('requested_by', 'unknown')} is already available. Opening the stored runtime console instead of starting a duplicate run.",
                'info',
            )
            return redirect(url_for('view_job_log', job_id=shared_job.id))

    try:
        return _submit_runtime_job(
            submission['execution_target'],
            submission['input_mode'],
            submission['paths'],
            submission['resources'],
            'kpi',
            submission['primary_input'],
            submission['execution_label'],
            jira_fields=submission.get('jira_fields'),
        )
    except Exception as exc:
        logger.exception('Failed to submit runtime KPI job')
        flash(f'HPCC broker submission failed: {exc}', 'error')
        return redirect(request.referrer or url_for('tool_kpi'))


@app.route('/api/runtime/kpi/reuse-check', methods=['POST'])
@login_required
def api_runtime_kpi_reuse_check():
    payload = request.get_json(silent=True) or {}
    try:
        submission = _build_runtime_kpi_submission(payload)
    except ValueError as exc:
        return jsonify({'success': False, 'error': str(exc)}), 400
    except Exception as exc:
        logger.warning('reuse-check build submission failed: %s', exc)
        return jsonify({'success': False, 'error': str(exc)}), 400

    try:
        existing_runtime_job = runtime_store.find_reusable_job(
            submission['execution_target'],
            submission['input_mode'],
            _build_runtime_submit_payload(
                submission['execution_target'],
                submission['input_mode'],
                submission['paths'],
                submission['resources'],
            ),
        )
    except Exception as exc:
        logger.warning('reuse-check store query failed: %s', exc)
        # Safe fallback: treat as no duplicate so submission can proceed
        existing_runtime_job = None

    if existing_runtime_job is None:
        return jsonify({'success': True, 'duplicate': False})

    return jsonify(
        {
            'success': True,
            'duplicate': True,
            'existing': _serialize_runtime_match(existing_runtime_job, submission['execution_label']),
        }
    )


@app.route('/api/kpi/alert', methods=['POST'])
@login_required
def api_kpi_alert():
    payload = request.get_json(silent=True) or {}
    accuracy = float(payload.get('accuracy', 100))
    hdf_path = (payload.get('hdf_path') or '').strip()
    log_path = (payload.get('log_path') or '').strip()
    html_path = (payload.get('html_path') or '').strip()
    sensor_id = (payload.get('sensor_id') or '').strip()
    details = payload.get('details', {})

    if accuracy >= 60:
        return jsonify({'success': True, 'action': 'skipped', 'reason': 'accuracy above threshold'})

    if not hdf_path:
        return jsonify({'success': False, 'error': 'hdf_path required'}), 400

    ticket_key = jira_integration.create_kpi_ticket(
        accuracy_score=accuracy,
        log_path=log_path,
        hdf_path=hdf_path,
        html_path=html_path,
        details_dict=details if isinstance(details, dict) else {},
        sensor_id=sensor_id,
    )

    return jsonify({
        'success': True,
        'ticket_key': ticket_key,
        'accuracy': accuracy,
    })


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


def _create_jira_ticket_for_job(job: 'JobHistory', log_path: str) -> None:
    """Create a JIRA ticket for a completed job if configured."""
    params = job.parameters or {}
    if not params.get('create_jira'):
        return

    from jira_integration import JiraIntegration
    jira = JiraIntegration()
    if not jira._enabled:
        logger.warning('JIRA not configured, skipping ticket creation')
        return

    assignee = (params.get('jira_assignee') or '').strip()
    notes = (params.get('jira_notes') or '').strip()
    input_txt = (params.get('input_txt') or job.input_path or '').strip()
    simg_path = (params.get('simg_path') or '').strip()
    board = (params.get('jira_board') or '').strip() or 'FHW'

    key = jira.create_resim_ticket(
        input_txt=input_txt,
        simg_path=simg_path,
        log_path=log_path,
        notes=notes,
        assignee=assignee,
        job_id=job.id,
        board=board,
        story_points=1,
    )
    if key:
        logger.info('Created JIRA ticket %s for job %s', key, job.id)
        params['jira_ticket_key'] = key
        job.parameters = params
        db.session.commit()


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

            # Create JIRA ticket if requested
            try:
                params = job.parameters or {}
                if params.get('create_jira'):
                    _create_jira_ticket_for_job(job, log_path)
            except Exception:
                logger.exception('JIRA ticket creation failed for job %s', job.id)
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


def _write_askpass_script(password: str, path: str) -> None:
    """Write a chmod 0o500 script that echoes the password for SSH_ASKPASS."""
    with open(path, 'w', encoding='utf-8') as fp:
        fp.write('#!/bin/sh\n')
        fp.write(f'printf %s {shlex.quote(password)}\n')
    os.chmod(path, 0o500)


_LOG_FAILURE_MARKERS = [
    ('permission denied', 'Permission denied (likely an NFS/ACL restriction on a third-party project path).'),
    ('no such file or directory', 'A required file/script was not found.'),
    ('command not found', 'A required command was not found in the remote shell.'),
    ('traceback (most recent call last)', 'The remote Python script raised an unhandled exception.'),
]


def _first_failure_marker_in_log(log_path: str) -> str:
    """Return a human-readable reason if the log contains a known failure
    signature, else ''. rResim_Gen7.sh has no `set -e` and always finishes
    with a trivial `deactivate`, so its process exit code is 0 even when the
    real work (source venv / run resim_main.py) failed — the exit code
    alone cannot be trusted for this tool.
    """
    try:
        with open(log_path, 'r', encoding='utf-8', errors='replace') as fp:
            text = fp.read().lower()
    except Exception:
        return ''
    for marker, reason in _LOG_FAILURE_MARKERS:
        if marker in text:
            return reason
    return ''


def _run_ssh_job_background(job_id: int, cmd, env, log_path: str, askpass_path: str = ''):
    """Run a command via SSH as the logged-in user in the background and update JobHistory."""
    def _safe_read_text(path: str, limit: int = 4096) -> str:
        try:
            with open(path, 'r', encoding='utf-8', errors='replace') as fp:
                return fp.read(limit).strip()
        except Exception:
            return ''

    def _write_execution_context(log_fp):
        log_fp.write(f"HOSTNAME: {socket.gethostname()}\n")
        log_fp.write(f"USER: {getpass.getuser()}\n")
        log_fp.write(f"PID: {os.getpid()}\n")
        try:
            log_fp.write(f"PPID: {os.getppid()}\n")
        except Exception:
            pass
        log_fp.write(f"PLATFORM: {platform.platform()}\n")
        log_fp.write(f"PYTHON: {sys.executable}\n")

        slurm_keys = [
            'SLURM_JOB_ID', 'SLURM_JOB_NAME', 'SLURM_CLUSTER_NAME',
            'SLURM_PARTITION', 'SLURM_NODELIST', 'SLURM_JOB_NODELIST',
            'SLURM_CPUS_PER_TASK', 'SLURM_CPUS_ON_NODE',
            'SLURM_MEM_PER_NODE', 'SLURM_MEM_PER_CPU',
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

        meminfo = _safe_read_text('/proc/meminfo', limit=2048)
        if meminfo:
            first_lines = '\n'.join(meminfo.splitlines()[:5])
            log_fp.write("/proc/meminfo (first lines):\n" + first_lines + "\n")

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
            log_fp.write('START: ' + datetime.utcnow().isoformat() + 'Z\n\n')
            _write_execution_context(log_fp)
            log_fp.flush()

            # The remote vendor script prompts interactively (e.g. "Proceed
            # with Default Docker Configuration (Y/N)"). Feed it a stream of
            # "y" answers via the ssh client's stdin (like `yes | ssh ...`)
            # so any confirmation prompt is auto-answered instead of hanging
            # forever with no one to type a response.
            yes_proc = subprocess.Popen(['yes', 'y'], stdout=subprocess.PIPE)
            proc = subprocess.Popen(
                cmd,
                cwd='/',
                stdin=yes_proc.stdout,
                stdout=log_fp,
                stderr=log_fp,
                env=env,
            )
            yes_proc.stdout.close()  # let yes_proc get SIGPIPE once proc exits

            # Safety net: never let a stuck SSH/remote-prompt hang the job (and
            # the polling UI) forever — fail cleanly instead.
            try:
                rc = proc.wait(timeout=1800)
            except subprocess.TimeoutExpired:
                proc.kill()
                rc = proc.wait()
                log_fp.write('\nTIMEOUT: process exceeded 1800s and was killed.\n')
            finally:
                yes_proc.kill()
                yes_proc.wait()

            log_fp.write(f'\nEND: {datetime.utcnow().isoformat()}Z\n')
            log_fp.write(f'EXIT_CODE: {_format_return_code(rc)}\n')
            log_fp.flush()

        with app.app_context():
            job = JobHistory.query.get(job_id)
            if job:
                job.completed_at = datetime.utcnow()
                # rResim_Gen7.sh has no `set -e` and always ends on a trivial
                # `deactivate`, so it exits 0 even when the real work (source
                # venv / run resim_main.py) failed with "Permission denied"
                # on a third-party project path — exit code alone is not
                # trustworthy for this tool. Sniff the captured output too.
                failure_reason = _first_failure_marker_in_log(log_path)
                if rc == 0 and not failure_reason:
                    job.status = 'COMPLETED'
                else:
                    job.status = 'FAILED'
                    display_path = _container_path_to_host_path(log_path)
                    if failure_reason:
                        job.error_message = f'{failure_reason} See log: {display_path}'
                    else:
                        job.error_message = f'Process exit code {rc}. See log: {display_path}'
                db.session.commit()

                params = job.parameters or {}
                if params.get('create_jira'):
                    _create_jira_ticket_for_job(job, log_path)
    except Exception as exc:
        logger.exception('SSH job runner failed')
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
    finally:
        # Minimize the exposure window for the plaintext-password askpass
        # helper script (chmod 0o500, but still readable by this unix user).
        if askpass_path:
            try:
                os.remove(askpass_path)
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
    
    elif tool_name == 'resim_run':
        input_txt = parameters.get('input_txt', '').strip()
        input_path = input_txt
        config_xml = ''
        output_path = ''
        if not input_txt:
            flash('Input file (input.txt) is required', 'error')
            return redirect(request.referrer or url_for('dashboard'))
        simg_path = parameters.get('simg_path', '').strip()
        if not simg_path:
            flash('Simg file path is required', 'error')
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
        'hyperlink_tool': 'all_services.simg',
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

    # Tool-specific resource requirements — single source of truth: resources.py
    resource_config = dict(_tool_resources(tool_name))

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
        'config_xml': config_xml if tool_name == 'interactive_plot' else '',
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
        if chat_session is None:
            chat_session = ChatSession(
                user_id=current_user.id,
                session_id=session_id
            )
            db.session.add(chat_session)
            db.session.commit()
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
                'KPI Guide depends on the RAG service and did not get a valid response.\n\n'
                f"Details: {rag_response.get('error', 'No details available')}"
            )
    except Exception as e:
        logger.error(f"KPI guide backend error: {e}")
        response = "KPI Guide is unavailable because the RAG service failed unexpectedly."
    
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
    job = _get_viewable_job(job_id)
    
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
    
    payload = job.to_dict()
    payload['requested_by'] = _job_owner(job)
    return jsonify(payload)


@app.route('/api/job/<int:job_id>/cancel', methods=['POST'])
@login_required
def cancel_job(job_id):
    """Cancel a running job"""
    job = _get_manageable_job(job_id)
    
    runtime_job_id = ((job.parameters or {}).get('runtime_job_id') if job.parameters else None)
    if runtime_job_id and job.status in ACTIVE_RUNTIME_STATUSES:
        result = broker_client.cancel_job(int(runtime_job_id))
        job.status = 'CANCELLED'
        job.completed_at = datetime.utcnow()
        db.session.commit()
        return jsonify({'success': True, 'message': result.get('message', 'Cancelled')})

    if job.slurm_job_id and job.status in ACTIVE_RUNTIME_STATUSES:
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
    job = _get_viewable_job(job_id)
    _sync_runtime_job(job)
    
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
    elif job.status in ACTIVE_RUNTIME_STATUSES:
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
        runtime_status_detail = ((job.parameters or {}).get('runtime_status_detail') if job.parameters else '')
        if progress_data['progress'] == 0 and job.status == 'RUNNING':
            progress_data['progress'] = 25
            progress_data['message'] = 'Processing...'
        elif job.status in PENDING_RUNTIME_STATUSES:
            progress_data['message'] = runtime_status_detail or 'Waiting in queue...'
            progress_data['message'] = 'Waiting in queue...'
    
    return jsonify(progress_data)


@app.route('/api/job/<int:job_id>/rerun', methods=['POST'])
@login_required
def rerun_job(job_id):
    job = _get_viewable_job(job_id)

    try:
        rerun_result = _rerun_history_job(job)
    except ValueError as exc:
        return jsonify({'success': False, 'error': str(exc)}), 400
    except Exception as exc:
        logger.exception('Failed to rerun job %s', job_id)
        return jsonify({'success': False, 'error': str(exc)}), 500

    return jsonify({
        'success': True,
        'message': rerun_result.get('message', 'Job queued'),
        'job_id': rerun_result['job'].id,
        'redirect_url': rerun_result.get('redirect_url', ''),
    })


@app.route('/api/job/<int:job_id>/console-tail')
@login_required
def get_job_console_tail(job_id):
    job = _get_viewable_job(job_id)

    runtime_job = _sync_runtime_job(job)
    log_path, console = _resolve_job_log_path(job, runtime_job)
    offset = max(request.args.get('offset', default=0, type=int), 0)
    payload = {
        'success': True,
        'status': job.status,
        'status_detail': ((job.parameters or {}).get('runtime_status_detail') if job.parameters else ''),
        'text': '',
        'offset': offset,
        'next_offset': offset,
        'log_exists': bool(log_path and os.path.exists(log_path)),
        'supports_input': bool(console.get('supports_input')) and job.status in ACTIVE_RUNTIME_STATUSES and _can_manage_job(job),
        'pane_names': console.get('pane_names') if isinstance(console.get('pane_names'), list) else ['main'],
        'tmux_session_name': console.get('tmux_session_name', ''),
        'console_name': console.get('display_name', 'Runtime console'),
        'requested_by': _job_owner(job),
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
    job = _get_manageable_job(job_id)

    runtime_job = _sync_runtime_job(job)
    _, console = _resolve_job_log_path(job, runtime_job)
    queue_path = (console.get('input_queue_path') or '').strip()
    allowed_panes = console.get('pane_names') if isinstance(console.get('pane_names'), list) else ['main']

    if not queue_path or not console.get('supports_input'):
        return jsonify({'success': False, 'error': 'This run does not expose a live tmux console.'}), 400
    if job.status not in ACTIVE_RUNTIME_STATUSES:
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
    job = _get_viewable_job(job_id)
    
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
    job = _get_viewable_job(job_id)

    runtime_job = _sync_runtime_job(job)
    log_path, console = _resolve_job_log_path(job, runtime_job)

    n_lines = request.args.get('n', default=400, type=int)
    n_lines = max(50, min(5000, n_lines))

    log_exists = bool(log_path and os.path.exists(log_path))
    tail_text = _read_log_tail_text(log_path, n_lines) if log_exists else ''
    initial_offset = os.path.getsize(log_path) if log_exists else 0

    display_log_path = host_path_filter(log_path) if log_path else ''
    display_source_log_path = host_path_filter(console.get('source_log_path')) if console.get('source_log_path') else ''
    tail_cmd = f"tail -f {display_log_path}" if display_log_path else ''
    tmux_cmd = ''
    if console.get('tmux_session_name'):
        tmux_cmd = f"tmux attach -t {console['tmux_session_name']}"

    return render_template(
        'job_log.html',
        job=job,
        log_path=log_path,
        display_log_path=display_log_path,
        display_source_log_path=display_source_log_path,
        log_exists=log_exists,
        tail_cmd=tail_cmd,
        tmux_cmd=tmux_cmd,
        tail_text=tail_text,
        n_lines=n_lines,
        initial_offset=initial_offset,
        console=console,
        runtime_job=runtime_job,
        job_owner=_job_owner(job),
        can_send_input=_can_manage_job(job),
        job_artifacts=(runtime_job or {}).get('artifacts') or ((job.parameters or {}).get('runtime_artifacts') if isinstance(job.parameters, dict) else []) or [],
    )


@app.route('/html/job/<int:job_id>/file/<path:rel_path>')
@login_required  
def serve_job_file(job_id, rel_path):
    """Serve a file from job output directory"""
    job = _get_viewable_job(job_id)
    
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
    owner_filter = (request.args.get('owner') or '').strip()
    
    query = JobHistory.query
    
    if tool_filter:
        query = query.filter_by(tool_name=tool_filter)
    if status_filter:
        query = query.filter_by(status=status_filter)
    if owner_filter:
        query = query.join(User).filter(User.net_id.ilike(f'%{owner_filter}%'))
    
    jobs = query.order_by(JobHistory.created_at.desc())\
        .paginate(page=page, per_page=20)
    for job in jobs.items:
        _sync_runtime_job(job)
    
    return render_template('history.html', 
                         jobs=jobs,
                         tool_filter=tool_filter,
                         status_filter=status_filter,
                         owner_filter=owner_filter)


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
    
    from utils import find_free_port
    port_env = os.environ.get('PORT', '')
    port = int(port_env) if port_env.isdigit() else find_free_port(5005)
    app.run(
        host='0.0.0.0',
        port=port,
        debug=app.config.get('DEBUG', False)
    )
