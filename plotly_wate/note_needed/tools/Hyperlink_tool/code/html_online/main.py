"""
Log Viewer - Online Mode
Starts a Flask server for interactive viewing with external HTML/CSS/JS.
"""
import os
import sys
import platform
import threading
import webbrowser
import json
from typing import Any, Dict, List, Optional

# Base directories - handle PyInstaller frozen mode
if getattr(sys, 'frozen', False):
    # Running as a PyInstaller bundle
    EXE_DIR = os.path.dirname(sys.executable)  # Directory where .exe is located
    BASE_DIR = EXE_DIR
    STATIC_DIR = os.path.join(EXE_DIR, "static")  # static folder beside .exe
else:
    # Running as a normal Python script
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    STATIC_DIR = os.path.join(BASE_DIR, "static")
    EXE_DIR = BASE_DIR

OUT_DIR = os.path.join(BASE_DIR, "out")
CWD = os.getcwd()


def get_cache_dir() -> str:
    """Get the cache directory for downloaded/user data (.cache_html)
    
    Priority:
    1. CACHE_HTML_DIR environment variable
    2. all_services/.cache_html (if running from all_services)
    3. ~/.cache_html (Linux) or C:\.cache_html (Windows)
    """
    # Check environment variable first
    env_cache = os.environ.get('CACHE_HTML_DIR')
    if env_cache:
        return env_cache
    
    # Try to find all_services root
    current = BASE_DIR
    for _ in range(5):  # Go up max 5 levels
        all_services_marker = os.path.join(current, 'app.py')
        if os.path.exists(all_services_marker):
            cache_dir = os.path.join(current, '.cache_html')
            os.makedirs(cache_dir, exist_ok=True)
            return cache_dir
        parent = os.path.dirname(current)
        if parent == current:
            break
        current = parent
    
    # Fallback to system default
    if platform.system().lower() == 'windows':
        cache_dir = r'C:\.cache_html'
    else:
        cache_dir = os.path.expanduser('~/.cache_html')
    return cache_dir


def is_windows() -> bool:
    """Check if running on Windows"""
    return platform.system().lower() == 'windows'


def resolve_path(path: str, base_dir: str = None) -> str:
    """Resolve a path appropriately for the current platform"""
    if os.path.isabs(path):
        return os.path.abspath(path)
    if base_dir:
        return os.path.abspath(os.path.join(base_dir, path))
    return os.path.abspath(os.path.join(CWD, path))


def _safe_join(root: str, rel_path: str) -> str:
    """Join root + rel_path safely (prevent path traversal)."""
    rel_path = (rel_path or "").replace('\\', '/').lstrip('/')
    dest = os.path.abspath(os.path.join(root, rel_path))
    root_abs = os.path.abspath(root)
    if os.path.commonpath([dest, root_abs]) != root_abs:
        raise ValueError("Invalid upload path")
    return dest


def _expected_remote_prefix(server_name: str) -> Optional[str]:
    name = (server_name or '').strip().lower()
    if name == 'krakow':
        return '/net'
    if name == 'southfield':
        return '/mnt'
    return None


def _validate_remote_path_for_server(server_name: str, path_value: str) -> None:
    expected = _expected_remote_prefix(server_name)
    if not expected:
        return
    if not (path_value or '').startswith(expected):
        raise ValueError(f"For server '{server_name}', path must start with '{expected}'")


class LogViewerApp:
    """Core log viewer application - scans and processes log data for online mode"""
    
    def __init__(self, html_root: str, video_root: str, output_dir: str = None) -> None:
        self.html_root = os.path.abspath(html_root)
        self.video_root = os.path.abspath(video_root)
        self.output_dir = os.path.abspath(output_dir) if output_dir else OUT_DIR

    @staticmethod
    def _key(name: str) -> Optional[str]:
        """Extract key from filename"""
        base = os.path.splitext(os.path.basename(name))[0]
        for suffix in ["_web"]:
            if base.endswith(suffix):
                base = base[:-len(suffix)]
                break
        parts = base.split("_")
        return "_".join(parts[-2:]) if len(parts) >= 2 else None

    def _scan_html(self) -> Dict[str, List[str]]:
        """Scan HTML folders"""
        if not os.path.isdir(self.html_root):
            return {}
        m: Dict[str, List[str]] = {}
        for entry in os.scandir(self.html_root):
            if not entry.is_dir():
                continue
            k = self._key(entry.name)
            if not k:
                continue
            files = []
            for root, _, fnames in os.walk(entry.path):
                for f in fnames:
                    if os.path.splitext(f)[1].lower() in {".html", ".htm"}:
                        abs_path = os.path.join(root, f)
                        # URL path for Flask serving
                        rel = os.path.relpath(abs_path, self.html_root).replace("\\", "/")
                        files.append(f"/data/html/{rel}")
            if files:
                m[k] = sorted(files)
        return m

    def _scan_images(self) -> Dict[str, Dict[str, Dict[str, str]]]:
        """Scan image files"""
        if not os.path.isdir(self.html_root):
            return {}
        
        sensor_names = {"range", "velocity", "rangerate", "doppler", "azimuth", "elevation", "snr", "rcs"}
        sensor_positions = {"FL", "FR", "RL", "RR", "FC"}
        img_map: Dict[str, Dict[str, Dict[str, str]]] = {}
        
        for entry in os.scandir(self.html_root):
            if not entry.is_dir():
                continue
            k = self._key(entry.name)
            if not k:
                continue
            
            imgs: Dict[str, Dict[str, str]] = {}
            for root, _, fnames in os.walk(entry.path):
                folder_name = os.path.basename(root).upper()
                for f in fnames:
                    if os.path.splitext(f)[1].lower() != ".png":
                        continue
                    
                    file_base = os.path.splitext(f)[0].lower()
                    sensor_pos = next((pos for pos in sensor_positions if folder_name == pos or pos.lower() in file_base), None)
                    if not sensor_pos:
                        continue
                    
                    sensor_name = next((sn for sn in sensor_names if sn in file_base), "other")
                    if sensor_name not in imgs:
                        imgs[sensor_name] = {}
                    
                    abs_path = os.path.join(root, f)
                    rel = os.path.relpath(abs_path, self.html_root).replace("\\", "/")
                    imgs[sensor_name][sensor_pos] = f"/data/html/{rel}"
            
            if imgs:
                img_map[k] = imgs
        
        return img_map

    def _scan_text(self) -> Dict[str, Dict[str, str]]:
        """Scan text files"""
        text_dir = os.path.join(self.video_root, "log_txt")
        if not os.path.isdir(text_dir):
            return {}
        
        m: Dict[str, Dict[str, str]] = {}
        for entry in os.scandir(text_dir):
            if not entry.is_file() or os.path.splitext(entry.name)[1].lower() != ".txt":
                continue
            k = self._key(entry.name)
            if not k:
                continue
            try:
                with open(entry.path, "r", encoding="utf-8") as f:
                    content = f.read()
            except Exception:
                content = ""
            
            rel = os.path.relpath(entry.path, self.video_root).replace("\\", "/")
            path = f"/data/video/{rel}"
            m[k] = {"path": path, "content": content}
        return m

    def _scan_video(self) -> Dict[str, str]:
        """Scan video files"""
        if not os.path.isdir(self.video_root):
            return {}
        m: Dict[str, str] = {}
        for entry in os.scandir(self.video_root):
            if not entry.is_file() or os.path.splitext(entry.name)[1].lower() not in {".mp4", ".avi", ".mov", ".mkv"}:
                continue
            k = self._key(entry.name)
            if k:
                rel = os.path.relpath(entry.path, self.video_root).replace("\\", "/")
                m[k] = f"/data/video/{rel}"
        return m

    def _get_html_folder_name(self, key: str) -> str:
        """Get HTML folder name for a key"""
        for entry in os.scandir(self.html_root):
            if entry.is_dir() and self._key(entry.name) == key:
                return entry.name
        return key

    def build_mapping(self) -> Dict[str, Dict[str, Any]]:
        """Build complete mapping of all data for online mode"""
        html_map = self._scan_html()
        img_map = self._scan_images()
        video_map = self._scan_video()
        text_map = self._scan_text()

        html_keys = set(html_map)
        video_keys = set(video_map)
        matched = sorted(html_keys & video_keys)

        for k in sorted(html_keys - video_keys):
            print(f"No video found for key: {k}")
        for k in sorted(video_keys - html_keys):
            print(f"No HTML folder found for key: {k}")

        mapping: Dict[str, Dict[str, Any]] = {}
        print("Matched keys:")
        for k in matched:
            video_url = video_map[k]
            video_name = os.path.basename(video_url)
            
            text_info = text_map.get(k)
            if text_info:
                comment_path = text_info["path"]
                comment_content = text_info["content"]
            else:
                base_name = os.path.splitext(video_name)[0]
                comment_path = f"/data/video/log_txt/{base_name}.txt"
                comment_content = ""
            
            mapping[k] = {
                "video": video_url,
                "video_name": video_name,
                "html_files": html_map[k],
                "html_folder": self._get_html_folder_name(k),
                "images": img_map.get(k, {}),
                "comment_path": comment_path,
                "comment_content": comment_content,
            }
            print(f"  {k} -> {len(html_map[k])} HTML file(s), video: {video_url}")

        return mapping


def main_online(html_root: str = None, video_root: str = None) -> None:
    """Run in online mode - starts Flask server with external HTML/CSS/JS"""
    from flask import Flask, request, jsonify, send_from_directory, redirect
    from flask_cors import CORS
    from cluster_connect import cluster, SERVERS, download_progress, reset_progress, update_progress
    from db import create_engine_if_configured, init_db, log_event, log_mapping
    
    # Server config (cluster-friendly defaults)
    host = os.environ.get('LOGVIEW_HOST', '0.0.0.0')
    port_env = os.environ.get('LOGVIEW_PORT', '').strip()
    try:
        port = int(port_env) if port_env else 5000
    except Exception:
        port = 5000
    no_browser = os.environ.get('LOGVIEW_NO_BROWSER', '').strip().lower() in {'1', 'true', 'yes'}
    
    # Get the cache directory for user data
    cache_dir = get_cache_dir()

    # Optional PostgreSQL event logging
    db_engine = None
    try:
        db_engine = create_engine_if_configured()
        if db_engine is not None:
            init_db(db_engine)
    except Exception as e:
        db_engine = None
        print(f"PostgreSQL logging disabled: {e}")
    
    # Resolve paths - first check if user provided paths, then check .cache_html
    if html_root:
        html_root = resolve_path(html_root, CWD)
    else:
        # Default to .cache_html/html
        html_root = os.path.join(cache_dir, 'html')
    
    if video_root:
        video_root = resolve_path(video_root, CWD)
    else:
        # Default to .cache_html/video
        video_root = os.path.join(cache_dir, 'video')
    
    # Create cache directories if they don't exist
    os.makedirs(html_root, exist_ok=True)
    os.makedirs(video_root, exist_ok=True)
    
    # Dynamic paths for downloaded content
    dynamic_html_root = html_root
    dynamic_video_root = video_root
    
    app = Flask(__name__, static_folder=STATIC_DIR)
    CORS(app)
    
    viewer_app = None
    if os.path.isdir(html_root) and os.path.isdir(video_root):
        viewer_app = LogViewerApp(html_root, video_root, OUT_DIR)
    
    @app.after_request
    def add_no_cache_headers(response):
        response.headers['Cache-Control'] = 'no-cache, no-store, must-revalidate'
        response.headers['Pragma'] = 'no-cache'
        response.headers['Expires'] = '0'
        return response
    
    @app.route('/')
    def serve_viewer():
        return send_from_directory(STATIC_DIR, 'viewer.html')

    # Common expectation is http://<host>/html
    # Keep the app rooted at '/' (so absolute asset URLs keep working) and redirect.
    @app.route('/html')
    @app.route('/html/')
    def serve_viewer_html_alias():
        return redirect('/')
    
    @app.route('/static/<path:filename>')
    def serve_static(filename):
        return send_from_directory(STATIC_DIR, filename)
    
    @app.route('/data/html/<path:filename>')
    def serve_html_data(filename):
        nonlocal dynamic_html_root
        return send_from_directory(dynamic_html_root, filename)
    
    @app.route('/data/video/<path:filename>')
    def serve_video_data(filename):
        nonlocal dynamic_video_root
        return send_from_directory(dynamic_video_root, filename)
    
    @app.route('/api/mappings')
    def get_mappings():
        nonlocal viewer_app
        if viewer_app:
            mapping = viewer_app.build_mapping()
            return jsonify({'success': True, 'mappings': mapping})
        return jsonify({'success': True, 'mappings': {}})
    
    @app.route('/api/save-comment', methods=['POST'])
    def save_comment():
        nonlocal dynamic_video_root
        try:
            data = request.get_json()
            file_path = data.get('path')
            content = data.get('content', '')
            
            if not file_path:
                return jsonify({'success': False, 'error': 'No path provided'}), 400
            
            # Resolve path
            if file_path.startswith('/data/video/'):
                rel = file_path[12:]  # Remove '/data/video/'
                abs_path = os.path.join(dynamic_video_root, rel)
            else:
                abs_path = os.path.abspath(file_path)
            
            os.makedirs(os.path.dirname(abs_path), exist_ok=True)
            with open(abs_path, 'w', encoding='utf-8') as f:
                f.write(content)
            
            return jsonify({'success': True, 'path': abs_path})
        except Exception as e:
            return jsonify({'success': False, 'error': str(e)}), 500
    
    @app.route('/api/get-comment', methods=['POST'])
    def get_comment():
        nonlocal dynamic_video_root
        try:
            data = request.get_json()
            file_path = data.get('path')
            
            if not file_path:
                return jsonify({'success': False, 'error': 'No path provided'}), 400
            
            # Resolve path
            if file_path.startswith('/data/video/'):
                rel = file_path[12:]
                abs_path = os.path.join(dynamic_video_root, rel)
            else:
                abs_path = os.path.abspath(file_path)
            
            if not os.path.exists(abs_path):
                return jsonify({'success': True, 'content': ''})
            
            with open(abs_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            return jsonify({'success': True, 'content': content})
        except Exception as e:
            return jsonify({'success': False, 'error': str(e)}), 500
    
    @app.route('/api/cluster/servers', methods=['GET'])
    def get_servers():
        return jsonify({'servers': list(SERVERS.keys())})

    @app.route('/api/local/upload', methods=['POST'])
    def local_upload():
        """Upload local PC HTML/video data to the server and activate it for viewing."""
        nonlocal viewer_app, dynamic_html_root, dynamic_video_root
        try:
            output_path = (request.form.get('output_path') or '').strip()

            output_root = resolve_path(output_path, CWD) if output_path else cache_dir
            html_root_out = os.path.join(output_root, 'html')
            video_root_out = os.path.join(output_root, 'video')
            os.makedirs(html_root_out, exist_ok=True)
            os.makedirs(video_root_out, exist_ok=True)

            html_files = request.files.getlist('html_files')
            video_files = request.files.getlist('video_files')
            if not html_files and not video_files:
                return jsonify({'success': False, 'error': 'No files uploaded'}), 400

            saved_html = 0
            for f in html_files:
                rel = f.filename or ''
                if not rel:
                    continue
                dest = _safe_join(html_root_out, rel)
                os.makedirs(os.path.dirname(dest), exist_ok=True)
                f.save(dest)
                saved_html += 1

            saved_video = 0
            for f in video_files:
                name = os.path.basename((f.filename or '').replace('\\', '/'))
                if not name:
                    continue
                dest = os.path.join(video_root_out, name)
                os.makedirs(os.path.dirname(dest), exist_ok=True)
                f.save(dest)
                saved_video += 1

            dynamic_html_root = html_root_out
            dynamic_video_root = video_root_out
            viewer_app = LogViewerApp(dynamic_html_root, dynamic_video_root, OUT_DIR)

            log_event(
                db_engine,
                event_type='local_upload',
                output_path=output_root,
                success=True,
                message=f'Uploaded html_files={saved_html}, video_files={saved_video}',
            )
            log_mapping(
                db_engine,
                mode='local',
                output_root=output_root,
                output_html_root=html_root_out,
                output_video_root=video_root_out,
                success=True,
                message=f'Uploaded html_files={saved_html}, video_files={saved_video}',
            )

            return jsonify({
                'success': True,
                'message': 'Upload complete',
                'output_root': output_root,
                'html_root': html_root_out,
                'video_root': video_root_out,
                'saved_html': saved_html,
                'saved_video': saved_video,
            })
        except Exception as e:
            log_event(db_engine, event_type='local_upload', success=False, message=str(e))
            log_mapping(db_engine, mode='local', success=False, message=str(e))
            return jsonify({'success': False, 'error': str(e)}), 500
    
    @app.route('/api/cluster/connect', methods=['POST'])
    def cluster_connect():
        try:
            data = request.get_json()
            server = data.get('server', '')
            username = data.get('username', '')
            password = data.get('password', '')
            password_confirm = data.get('password_confirm', '')
            
            if not all([server, username, password]):
                return jsonify({'success': False, 'error': 'Missing credentials'}), 400

            if password != password_confirm:
                return jsonify({'success': False, 'error': 'Password confirmation does not match'}), 400
            
            success, msg = cluster.connect(server, username, password)
            log_event(
                db_engine,
                event_type='cluster_connect',
                server=server,
                username=username,
                success=bool(success),
                message=msg,
            )
            return jsonify({'success': success, 'message': msg})
        except Exception as e:
            return jsonify({'success': False, 'error': str(e)}), 500
    
    @app.route('/api/cluster/disconnect', methods=['POST'])
    def cluster_disconnect():
        log_event(
            db_engine,
            event_type='cluster_disconnect',
            server=cluster.server_name if cluster.connected else None,
            username=cluster.username if cluster.connected else None,
            success=True,
        )
        cluster.disconnect()
        return jsonify({'success': True})
    
    @app.route('/api/cluster/status', methods=['GET'])
    def cluster_status():
        return jsonify({
            'connected': cluster.connected,
            'server': cluster.server_name if cluster.connected else None,
            'username': cluster.username if cluster.connected else None,
        })
    
    @app.route('/api/cluster/list', methods=['POST'])
    def cluster_list():
        try:
            data = request.get_json()
            path = data.get('path', '/')
            success, items = cluster.list_dir(path)
            return jsonify({'success': success, 'items': items, 'path': path})
        except Exception as e:
            return jsonify({'success': False, 'error': str(e)}), 500
    
    @app.route('/api/cluster/download', methods=['POST'])
    def cluster_download():
        nonlocal viewer_app, dynamic_html_root, dynamic_video_root
        try:
            data = request.get_json()
            html_path = data.get('html_path', '')
            video_path = data.get('video_path', '')
            local_save_path = data.get('local_path', 'db').strip() or 'db'
            
            if not html_path or not video_path:
                return jsonify({'success': False, 'error': 'Both remote paths required'}), 400
            
            reset_progress()
            download_progress["active"] = True
            
            local_save_path_abs = resolve_path(local_save_path, CWD)
            local_html = os.path.join(local_save_path_abs, 'html')
            local_video = os.path.join(local_save_path_abs, 'video')
            
            update_progress(phase="downloading", message=f"Downloading HTML from {html_path}...")
            success1, msg1 = cluster.download_directory(html_path, local_html)
            if not success1:
                download_progress["active"] = False
                return jsonify({'success': False, 'error': f'HTML download failed: {msg1}'})
            
            update_progress(phase="downloading", message=f"Downloading video from {video_path}...")
            success2, msg2 = cluster.download_directory(video_path, local_video)
            if not success2:
                download_progress["active"] = False
                return jsonify({'success': False, 'error': f'Video download failed: {msg2}'})
            
            # Update dynamic paths
            dynamic_html_root = local_html
            dynamic_video_root = local_video
            viewer_app = LogViewerApp(local_html, local_video, OUT_DIR)
            
            download_progress["active"] = False
            update_progress(phase="complete", message=f'{msg1}, {msg2}')
            
            return jsonify({
                'success': True,
                'message': f'{msg1}, {msg2}',
                'html_local': local_html,
                'video_local': local_video
            })
        except Exception as e:
            import traceback
            download_progress["active"] = False
            return jsonify({'success': False, 'error': f'{str(e)}: {traceback.format_exc()}'}), 500
    
    @app.route('/api/cluster/progress', methods=['GET'])
    def cluster_progress():
        return jsonify(download_progress)
    
    @app.route('/api/cluster/scan-logs', methods=['POST'])
    def scan_remote_logs():
        """Scan remote html/video paths for logs."""
        try:
            data = request.get_json()
            remote_path = (data.get('path') or '').strip()
            html_path = (data.get('html_path') or '').strip()
            video_path = (data.get('video_path') or '').strip()
            output_path = (data.get('output_path') or '').strip()

            if not html_path or not video_path:
                if not remote_path:
                    return jsonify({'success': False, 'error': 'No path provided'}), 400
                html_path = f"{remote_path}/html" if not remote_path.endswith('/') else f"{remote_path}html"
                video_path = f"{remote_path}/video" if not remote_path.endswith('/') else f"{remote_path}video"
            
            if not cluster.connected:
                return jsonify({'success': False, 'error': 'Not connected'}), 400

            _validate_remote_path_for_server(cluster.server_name, html_path)
            _validate_remote_path_for_server(cluster.server_name, video_path)
            
            success_html, html_items = cluster.list_dir(html_path)
            success_video, video_items = cluster.list_dir(video_path)
            
            if not success_html:
                return jsonify({'success': False, 'error': f'Cannot access html folder: {html_items}'})
            
            if not success_video:
                return jsonify({'success': False, 'error': f'Cannot access video folder: {video_items}'})
            
            # Build logs dictionary - match html folders with videos
            logs = {}
            
            # Get html folders
            html_folders = {item['name']: item for item in html_items if item.get('is_dir')}
            
            # Get video files
            video_files = {}
            for item in video_items:
                if not item.get('is_dir'):
                    name = item['name']
                    # Extract key from video filename
                    base = os.path.splitext(name)[0]
                    for suffix in ["_web"]:
                        if base.endswith(suffix):
                            base = base[:-len(suffix)]
                            break
                    parts = base.split("_")
                    if len(parts) >= 2:
                        key = "_".join(parts[-2:])
                        video_files[key] = name
            
            # Match html folders with videos
            cache_dir = get_cache_dir()
            output_root = resolve_path(output_path, CWD) if output_path else cache_dir
            local_html_root = os.path.join(output_root, 'html')
            local_video_root = os.path.join(output_root, 'video')
            
            for folder_name in html_folders:
                base = folder_name
                parts = base.split("_")
                if len(parts) >= 2:
                    key = "_".join(parts[-2:])
                    if key in video_files:
                        # Check if already cached
                        cached_html = os.path.join(local_html_root, folder_name)
                        cached_video = os.path.join(local_video_root, video_files[key])
                        is_cached = os.path.isdir(cached_html) and os.path.isfile(cached_video)
                        
                        logs[key] = {
                            'name': folder_name,
                            'html_path': f"{html_path}/{folder_name}",
                            'video_path': f"{video_path}/{video_files[key]}",
                            'video_name': video_files[key],
                            'cached': is_cached
                        }
            
            return jsonify({'success': True, 'logs': logs})
        except Exception as e:
            import traceback
            return jsonify({'success': False, 'error': f'{str(e)}: {traceback.format_exc()}'}), 500
    
    @app.route('/api/cluster/download-log', methods=['POST'])
    def download_single_log():
        """Download a single log (html folder + video file) to cache"""
        nonlocal viewer_app, dynamic_html_root, dynamic_video_root
        try:
            data = request.get_json()
            key = data.get('key', '')
            html_path = data.get('html_path', '')
            video_path = data.get('video_path', '')
            output_path = (data.get('output_path') or '').strip()
            
            if not key or not html_path or not video_path:
                return jsonify({'success': False, 'error': 'Missing required fields'}), 400
            
            if not cluster.connected:
                return jsonify({'success': False, 'error': 'Not connected'}), 400

            _validate_remote_path_for_server(cluster.server_name, html_path)
            _validate_remote_path_for_server(cluster.server_name, video_path)
            
            cache_dir = get_cache_dir()
            output_root = resolve_path(output_path, CWD) if output_path else cache_dir
            local_html_root = os.path.join(output_root, 'html')
            local_video_root = os.path.join(output_root, 'video')
            
            # Download HTML folder
            folder_name = os.path.basename(html_path)
            local_html_folder = os.path.join(local_html_root, folder_name)
            success1, msg1 = cluster.download_directory(html_path, local_html_folder)
            if not success1:
                return jsonify({'success': False, 'error': f'HTML download failed: {msg1}'})
            
            # Download video file
            video_name = os.path.basename(video_path)
            local_video_file = os.path.join(local_video_root, video_name)
            os.makedirs(local_video_root, exist_ok=True)
            success2, msg2 = cluster.download_file(video_path, local_video_file)
            if not success2:
                return jsonify({'success': False, 'error': f'Video download failed: {msg2}'})
            
            # Update viewer app to use cache directory
            dynamic_html_root = local_html_root
            dynamic_video_root = local_video_root
            viewer_app = LogViewerApp(local_html_root, local_video_root, OUT_DIR)

            log_event(
                db_engine,
                event_type='download_log',
                server=cluster.server_name,
                username=cluster.username,
                remote_html_path=html_path,
                remote_video_path=video_path,
                output_path=output_root,
                key=key,
                success=True,
                message=f'Downloaded {folder_name} and {video_name}',
            )

            log_mapping(
                db_engine,
                mode='cluster',
                server=cluster.server_name,
                username=cluster.username,
                remote_base_path=None,
                remote_html_path=html_path,
                remote_video_path=video_path,
                output_root=output_root,
                output_html_root=local_html_root,
                output_video_root=local_video_root,
                key=key,
                success=True,
                message=f'Downloaded {folder_name} and {video_name}',
            )
            
            return jsonify({
                'success': True,
                'message': f'Downloaded {folder_name}',
                'key': key
            })
        except Exception as e:
            import traceback
            log_event(
                db_engine,
                event_type='download_log',
                server=cluster.server_name if cluster.connected else None,
                username=cluster.username if cluster.connected else None,
                remote_html_path=html_path if 'html_path' in locals() else None,
                remote_video_path=video_path if 'video_path' in locals() else None,
                output_path=output_path if 'output_path' in locals() else None,
                key=key if 'key' in locals() else None,
                success=False,
                message=str(e),
            )
            log_mapping(
                db_engine,
                mode='cluster',
                server=cluster.server_name if cluster.connected else None,
                username=cluster.username if cluster.connected else None,
                remote_html_path=html_path if 'html_path' in locals() else None,
                remote_video_path=video_path if 'video_path' in locals() else None,
                output_root=output_path if 'output_path' in locals() else None,
                key=key if 'key' in locals() else None,
                success=False,
                message=str(e),
            )
            return jsonify({'success': False, 'error': f'{str(e)}: {traceback.format_exc()}'}), 500
    
    @app.route('/api/vlm/process', methods=['POST'])
    def vlm_process():
        nonlocal dynamic_video_root
        try:
            data = request.get_json() or {}
            force = data.get('force', False)

            # Import lazily so online viewer doesn't require heavy VLM deps unless used.
            try:
                from vlm import process_videos_with_vlm  # type: ignore
            except Exception as e:
                return jsonify({
                    'success': False,
                    'error': f'VLM dependencies not available in this environment: {e}'
                }), 500

            processed, skipped, errors = process_videos_with_vlm(dynamic_video_root, force=force)
            
            return jsonify({
                'success': errors == 0,
                'processed': processed,
                'skipped': skipped,
                'errors': errors
            })
        except Exception as e:
            return jsonify({'success': False, 'error': str(e)}), 500
    
    # Start server
    print(f"\n{'='*60}")
    print("Log Viewer - Online Mode")
    print(f"{'='*60}")
    print(f"Server: http://{host}:{port} (bind)")
    print(f"HTML root: {html_root if os.path.isdir(html_root) else 'Not found'}")
    print(f"Video root: {video_root if os.path.isdir(video_root) else 'Not found'}")
    print(f"Static files: {STATIC_DIR}")
    print(f"{'='*60}")
    print("Press Ctrl+C to stop the server")
    
    def open_browser():
        import time
        time.sleep(1)
        webbrowser.open(f'http://localhost:{port}')

    # Avoid opening a browser on headless/server environments (e.g., Singularity).
    if not no_browser and platform.system().lower() == 'windows':
        threading.Thread(target=open_browser, daemon=True).start()
    app.run(host=host, port=port, debug=False)


def main() -> None:
    """Main entry point for online mode"""
    import argparse

    parser = argparse.ArgumentParser(description='Log Viewer - Online Mode')
    parser.add_argument('--serve', action='store_true', help='Ignored (online mode is default)')
    parser.add_argument('--host', default=None, help='Bind host (default: env LOGVIEW_HOST or 0.0.0.0)')
    parser.add_argument('--port', type=int, default=None, help='Bind port (default: env LOGVIEW_PORT or 5000)')
    parser.add_argument('--no-browser', action='store_true', help='Disable auto-open browser')
    parser.add_argument('--vlm', action='store_true', help='Run VLM processing mode and exit')
    parser.add_argument('--force', action='store_true', help='Force regenerate VLM text output')
    parser.add_argument('html_root', nargs='?', default=None)
    parser.add_argument('video_root', nargs='?', default=None)
    parsed = parser.parse_args()

    vlm_mode = parsed.vlm
    force_vlm = parsed.force
    no_browser = parsed.no_browser
    
    if vlm_mode:
        # VLM mode - process videos and generate text descriptions
        from vlm import process_videos_with_vlm

        video_root = os.path.abspath(parsed.html_root) if parsed.html_root else os.path.join(BASE_DIR, 'db', 'video')
        
        if not os.path.isdir(video_root):
            print(f"Error: Video directory not found: {video_root}", file=sys.stderr)
            sys.exit(1)
        
        print(f"\n{'='*60}")
        print("VLM Mode: Generating text descriptions for videos")
        print(f"Video folder: {video_root}")
        print(f"Force regenerate: {force_vlm}")
        print(f"{'='*60}")
        
        processed, skipped, errors = process_videos_with_vlm(video_root, force=force_vlm)
        
        if errors > 0:
            sys.exit(1)
        sys.exit(0)
    
    # Online mode (default)
    html_root = parsed.html_root
    video_root = parsed.video_root

    # Pass through CLI flags via environment for main_online
    if parsed.host:
        os.environ['LOGVIEW_HOST'] = parsed.host
    if parsed.port is not None:
        os.environ['LOGVIEW_PORT'] = str(parsed.port)
    if no_browser:
        os.environ['LOGVIEW_NO_BROWSER'] = '1'
    main_online(html_root, video_root)


if __name__ == "__main__":
    main()
