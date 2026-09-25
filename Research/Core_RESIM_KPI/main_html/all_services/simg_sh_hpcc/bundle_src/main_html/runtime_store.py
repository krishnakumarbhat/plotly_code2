import json
import os
import re
import sqlite3
import hashlib
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

try:
    from .env_utils import get_db_path
except ImportError:
    from env_utils import get_db_path


class RuntimeStore:
    SYSTEM_BASELINE_VARIANT = 'hpcc_baseline'
    DEFAULT_VARIANT = 'default'
    ACTIVE_JOB_STATUSES = {'QUEUED', 'SUBMITTED', 'PENDING', 'RUNNING'}
    REUSABLE_JOB_STATUSES = {'QUEUED', 'SUBMITTED', 'PENDING', 'RUNNING', 'COMPLETED'}
    RUNTIME_DB_VERSION = '2026.04.27'
    ARTIFACT_SUFFIXES = {'.html', '.htm', '.hdf', '.hdf5', '.mf4', '.csv', '.json', '.xml', '.txt', '.log', '.md'}
    MAX_INDEXED_ARTIFACTS = 2048

    def __init__(self, db_path: Optional[str] = None):
        self.db_path = db_path or os.environ.get('HPCC_RUNTIME_DB') or get_db_path()
        project_root_override = (os.environ.get('HPCC_PROJECT_ROOT') or '').strip()
        self.repo_root = Path(project_root_override).resolve() if project_root_override else Path(__file__).resolve().parents[1]
        Path(self.db_path).parent.mkdir(parents=True, exist_ok=True)
        self._ensure_schema()

    def _runtime_dir(self) -> Path:
        bundle_root_override = (os.environ.get('HPCC_BUNDLE_ROOT') or '').strip()
        if bundle_root_override:
            return Path(bundle_root_override).resolve()
        if self.repo_root.name == 'bundle_src':
            bundle_root = self.repo_root.parent
            if (bundle_root / 'main_html.simg').exists() or (bundle_root / 'hpcc_main.pyz').exists():
                return bundle_root.resolve()
        if (self.repo_root / 'main_html.simg').exists() or (self.repo_root / 'main_hpcc.sh').exists():
            return self.repo_root
        return self.repo_root / 'simg_sh_hpcc'

    def _connect(self) -> sqlite3.Connection:
        connection = sqlite3.connect(self.db_path)
        connection.row_factory = sqlite3.Row
        connection.execute('PRAGMA foreign_keys = ON')
        connection.execute('PRAGMA busy_timeout = 5000')
        return connection

    def _configure_sqlite(self, connection: sqlite3.Connection) -> None:
        connection.execute('PRAGMA foreign_keys = ON')
        connection.execute('PRAGMA busy_timeout = 5000')
        try:
            connection.execute('PRAGMA journal_mode = WAL')
        except sqlite3.OperationalError:
            try:
                connection.execute('PRAGMA journal_mode = DELETE')
            except sqlite3.OperationalError:
                pass  # NFS filesystem; proceed without journal mode change
        try:
            connection.execute('PRAGMA synchronous = NORMAL')
        except sqlite3.OperationalError:
            pass

    def _ensure_schema(self) -> None:
        with self._connect() as connection:
            self._configure_sqlite(connection)
            connection.executescript(
                """
                CREATE TABLE IF NOT EXISTS runtime_meta (
                    key TEXT PRIMARY KEY,
                    value TEXT NOT NULL,
                    updated_at TEXT NOT NULL
                );

                CREATE TABLE IF NOT EXISTS runtime_tools (
                    tool_key TEXT PRIMARY KEY,
                    display_name TEXT NOT NULL,
                    category TEXT NOT NULL,
                    image_path TEXT,
                    entry_command TEXT,
                    service_url TEXT,
                    input_hint TEXT,
                    output_hint TEXT,
                    notes TEXT,
                    metadata_json TEXT,
                    updated_by TEXT,
                    updated_at TEXT NOT NULL
                );

                CREATE TABLE IF NOT EXISTS runtime_jobs (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    tool_key TEXT NOT NULL,
                    requested_by TEXT NOT NULL,
                    session_id TEXT,
                    status TEXT NOT NULL,
                    mode TEXT,
                    input_path TEXT,
                    output_path TEXT,
                    log_path TEXT,
                    command_json TEXT,
                    resources_json TEXT,
                    request_json TEXT,
                    pid INTEGER,
                    return_code INTEGER,
                    error_message TEXT,
                    created_at TEXT NOT NULL,
                    started_at TEXT,
                    completed_at TEXT
                );

                CREATE TABLE IF NOT EXISTS runtime_events (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    runtime_job_id INTEGER NOT NULL,
                    level TEXT NOT NULL,
                    message TEXT NOT NULL,
                    created_at TEXT NOT NULL,
                    FOREIGN KEY(runtime_job_id) REFERENCES runtime_jobs(id)
                );

                CREATE TABLE IF NOT EXISTS runtime_job_logs (
                    runtime_job_id INTEGER PRIMARY KEY,
                    source_log_path TEXT,
                    mirror_log_path TEXT,
                    content TEXT NOT NULL DEFAULT '',
                    byte_count INTEGER NOT NULL DEFAULT 0,
                    content_hash TEXT,
                    updated_at TEXT NOT NULL,
                    FOREIGN KEY(runtime_job_id) REFERENCES runtime_jobs(id)
                );

                CREATE TABLE IF NOT EXISTS runtime_job_artifacts (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    runtime_job_id INTEGER NOT NULL,
                    artifact_type TEXT NOT NULL,
                    artifact_path TEXT NOT NULL,
                    relative_path TEXT,
                    size_bytes INTEGER,
                    exists_flag INTEGER NOT NULL DEFAULT 1,
                    metadata_json TEXT,
                    created_at TEXT NOT NULL,
                    updated_at TEXT NOT NULL,
                    UNIQUE(runtime_job_id, artifact_path),
                    FOREIGN KEY(runtime_job_id) REFERENCES runtime_jobs(id)
                );

                CREATE TABLE IF NOT EXISTS runtime_graph_variants (
                    variant_key TEXT PRIMARY KEY,
                    display_name TEXT NOT NULL,
                    description TEXT,
                    graph_json TEXT NOT NULL,
                    is_default INTEGER NOT NULL DEFAULT 0,
                    updated_by TEXT,
                    updated_at TEXT NOT NULL
                );
                """
            )
            self._ensure_column(connection, 'runtime_jobs', 'request_fingerprint', 'TEXT')
            self._ensure_column(connection, 'runtime_jobs', 'execution_path', 'TEXT')
            self._ensure_column(connection, 'runtime_jobs', 'tool_version', 'TEXT')
            self._ensure_column(connection, 'runtime_jobs', 'db_version', 'TEXT')
            self._ensure_column(connection, 'runtime_jobs', 'mirror_log_path', 'TEXT')
            self._ensure_column(connection, 'runtime_jobs', 'reused_from_runtime_job_id', 'INTEGER')
            self._ensure_column(connection, 'runtime_jobs', 'last_seen_at', 'TEXT')
            self._set_meta(connection, 'runtime_db_version', self.RUNTIME_DB_VERSION)

    @staticmethod
    def _ensure_column(connection: sqlite3.Connection, table_name: str, column_name: str, column_type: str) -> None:
        rows = connection.execute(f'PRAGMA table_info({table_name})').fetchall()
        existing_columns = {row[1] for row in rows}
        if column_name in existing_columns:
            return
        connection.execute(f'ALTER TABLE {table_name} ADD COLUMN {column_name} {column_type}')

    def _set_meta(self, connection: sqlite3.Connection, key: str, value: str) -> None:
        connection.execute(
            """
            INSERT INTO runtime_meta (key, value, updated_at) VALUES (?, ?, ?)
            ON CONFLICT(key) DO UPDATE SET
                value = excluded.value,
                updated_at = excluded.updated_at
            """,
            (key, value, self._utcnow()),
        )

    def ensure_defaults(self) -> None:
        runtime_dir = self._runtime_dir()
        defaults = [
            {
                'tool_key': 'main_html',
                'display_name': 'Main HTML Service',
                'category': 'service',
                'image_path': str(runtime_dir / 'main_html.simg'),
                'entry_command': 'singularity run main_html.simg',
                'service_url': 'http://127.0.0.1:5002/html',
                'input_hint': 'Browser traffic only',
                'output_hint': 'Flask UI on port 5002',
                'notes': 'Main login + dashboard service',
                'metadata_json': {'port': 5002, 'node_color': '#d4efe8'},
            },
            {
                'tool_key': 'can_kpi',
                'display_name': 'CAN KPI',
                'category': 'batch',
                'image_path': str(runtime_dir / 'kpi' / 'can' / 'can_kpi.simg'),
                'entry_command': 'singularity run kpi/can/can_kpi.simg <mode> ...',
                'service_url': '',
                'input_hint': 'JSON batch or input/output HDF pair',
                'output_hint': 'HTML KPI report directory',
                'notes': 'Uses KPI/can_kpi/kpi_main.py',
                'metadata_json': {'default_mode': 'json', 'default_args': '', 'node_color': '#f3efe4'},
            },
            {
                'tool_key': 'udp_kpi',
                'display_name': 'UDP KPI',
                'category': 'batch',
                'image_path': str(runtime_dir / 'kpi' / 'udp' / 'udp_kpi.simg'),
                'entry_command': 'singularity run kpi/udp/udp_kpi.simg <mode> ...',
                'service_url': '',
                'input_hint': 'JSON batch, HDF pair, or ZMQ bridge mode',
                'output_hint': 'HTML KPI report directory',
                'notes': 'Uses KPI/UDP_KPI/kpi_server.py',
                'metadata_json': {'default_mode': 'json', 'default_args': '', 'node_color': '#e7f3ef'},
            },
            {
                'tool_key': 'interactive_plot',
                'display_name': 'Interactive Plot',
                'category': 'batch',
                'image_path': str(runtime_dir / 'kpi' / 'int_plot' / 'intplot_kpi.simg'),
                'entry_command': 'singularity run kpi/int_plot/intplot_kpi.simg <xml> <json> <out>',
                'service_url': '',
                'input_hint': 'Config XML + inputs JSON, or HDF pair',
                'output_hint': 'Interactive HTML plot directory',
                'notes': 'Can bridge to UDP KPI over ZMQ',
                'metadata_json': {'default_mode': 'json', 'default_args': '', 'node_color': '#eef5ee'},
            },
            {
                'tool_key': 'hyperlink',
                'display_name': 'Hyperlink Viewer',
                'category': 'viewer',
                'image_path': '',
                'entry_command': 'served inside main_html',
                'service_url': '/hyperlink/',
                'input_hint': 'HTML directory with optional video directory',
                'output_hint': 'Viewer session + copied html/video cache',
                'notes': 'Integrated directly into main_html routes',
                'metadata_json': {'default_args': '', 'node_color': '#f6ead8'},
            },
            {
                'tool_key': 'rag',
                'display_name': 'RAG Service',
                'category': 'service',
                'image_path': str(runtime_dir / 'rag' / 'rag.simg'),
                'entry_command': 'singularity run rag/rag.simg --talk',
                'service_url': '',
                'input_hint': 'HTML root for ingestion + user question',
                'output_hint': 'Answer JSON + Chroma/SQLite data',
                'notes': 'GPU-backed service launched on demand through the HPCC broker',
                'metadata_json': {'port': 5100, 'default_args': '--talk', 'gpu': True, 'node_color': '#dfe7f4'},
            },
        ]

        for item in defaults:
            existing = self.get_tool(item['tool_key'])
            should_refresh = (
                existing
                and (existing.get('updated_by') or 'system') == 'system'
                and (
                    existing.get('image_path') != item.get('image_path')
                    or existing.get('entry_command') != item.get('entry_command')
                    or existing.get('service_url') != item.get('service_url')
                )
            )
            if not existing or should_refresh:
                self.save_tool(item, updated_by='system')

        self._ensure_default_graph_variant()

    def list_tools(self) -> List[Dict[str, Any]]:
        with self._connect() as connection:
            rows = connection.execute(
                "SELECT * FROM runtime_tools ORDER BY CASE category WHEN 'service' THEN 0 WHEN 'batch' THEN 1 ELSE 2 END, display_name"
            ).fetchall()
        return [self._row_to_tool(row) for row in rows]

    def get_tool(self, tool_key: str) -> Optional[Dict[str, Any]]:
        with self._connect() as connection:
            row = connection.execute(
                'SELECT * FROM runtime_tools WHERE tool_key = ?',
                (tool_key,),
            ).fetchone()
        return self._row_to_tool(row) if row else None

    def save_tool(self, payload: Dict[str, Any], updated_by: str = 'system') -> Dict[str, Any]:
        timestamp = self._utcnow()
        metadata = payload.get('metadata_json', payload.get('metadata', {})) or {}
        with self._connect() as connection:
            connection.execute(
                """
                INSERT INTO runtime_tools (
                    tool_key, display_name, category, image_path, entry_command,
                    service_url, input_hint, output_hint, notes, metadata_json,
                    updated_by, updated_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                ON CONFLICT(tool_key) DO UPDATE SET
                    display_name = excluded.display_name,
                    category = excluded.category,
                    image_path = excluded.image_path,
                    entry_command = excluded.entry_command,
                    service_url = excluded.service_url,
                    input_hint = excluded.input_hint,
                    output_hint = excluded.output_hint,
                    notes = excluded.notes,
                    metadata_json = excluded.metadata_json,
                    updated_by = excluded.updated_by,
                    updated_at = excluded.updated_at
                """,
                (
                    payload['tool_key'],
                    payload['display_name'],
                    payload['category'],
                    payload.get('image_path', ''),
                    payload.get('entry_command', ''),
                    payload.get('service_url', ''),
                    payload.get('input_hint', ''),
                    payload.get('output_hint', ''),
                    payload.get('notes', ''),
                    json.dumps(metadata),
                    updated_by,
                    timestamp,
                ),
            )
        return self.get_tool(payload['tool_key'])

    def list_graph_variants(self) -> List[Dict[str, Any]]:
        with self._connect() as connection:
            rows = connection.execute(
                'SELECT * FROM runtime_graph_variants ORDER BY is_default DESC, display_name'
            ).fetchall()
        return [self._row_to_variant(row) for row in rows]

    def get_graph_variant(self, variant_key: Optional[str] = None) -> Optional[Dict[str, Any]]:
        with self._connect() as connection:
            if variant_key:
                row = connection.execute(
                    'SELECT * FROM runtime_graph_variants WHERE variant_key = ?',
                    (variant_key,),
                ).fetchone()
            else:
                row = connection.execute(
                    'SELECT * FROM runtime_graph_variants ORDER BY is_default DESC, display_name LIMIT 1'
                ).fetchone()
        return self._row_to_variant(row) if row else None

    def save_graph_variant(self, payload: Dict[str, Any], updated_by: str = 'system') -> Dict[str, Any]:
        variant_key = str(payload['variant_key']).strip()
        if variant_key == self.SYSTEM_BASELINE_VARIANT and updated_by != 'system':
            raise ValueError('The built-in HPCC baseline is read-only. Load it and save a new variation instead.')
        timestamp = self._utcnow()
        variants = self.list_graph_variants()
        is_default = bool(payload.get('is_default')) or not variants
        graph_state = self._merge_graph_state(payload.get('graph', {}), self.list_tools())

        with self._connect() as connection:
            if is_default:
                connection.execute('UPDATE runtime_graph_variants SET is_default = 0')

            connection.execute(
                """
                INSERT INTO runtime_graph_variants (
                    variant_key, display_name, description, graph_json,
                    is_default, updated_by, updated_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?)
                ON CONFLICT(variant_key) DO UPDATE SET
                    display_name = excluded.display_name,
                    description = excluded.description,
                    graph_json = excluded.graph_json,
                    is_default = excluded.is_default,
                    updated_by = excluded.updated_by,
                    updated_at = excluded.updated_at
                """,
                (
                    variant_key,
                    payload.get('display_name', variant_key),
                    payload.get('description', ''),
                    json.dumps(graph_state),
                    1 if is_default else 0,
                    updated_by,
                    timestamp,
                ),
            )

        return self.get_graph_variant(variant_key)

    def reset_graph_variant(self, variant_key: str, updated_by: str = 'system') -> Dict[str, Any]:
        variant_key = (variant_key or self.DEFAULT_VARIANT).strip()
        tools = self.list_tools()
        baseline_graph = self._default_graph_state(tools)
        existing = self.get_graph_variant(variant_key)
        if variant_key == self.SYSTEM_BASELINE_VARIANT:
            payload = {
                'variant_key': self.SYSTEM_BASELINE_VARIANT,
                'display_name': 'HPCC baseline',
                'description': 'Built-in HPCC runtime topology.',
                'graph': baseline_graph,
                'is_default': False,
            }
        else:
            payload = {
                'variant_key': variant_key,
                'display_name': (existing or {}).get('display_name', variant_key),
                'description': (existing or {}).get('description', 'Reset to the built-in HPCC runtime topology.'),
                'graph': baseline_graph,
                'is_default': bool((existing or {}).get('is_default')),
            }
        return self.save_graph_variant(payload, updated_by=updated_by)

    def create_job(
        self,
        tool_key: str,
        requested_by: str,
        session_id: str,
        mode: str,
        input_path: str,
        output_path: str,
        log_path: str,
        command: List[str],
        resources: Dict[str, Any],
        request_payload: Dict[str, Any],
        status: str = 'QUEUED',
    ) -> int:
        request_fingerprint = self.compute_request_fingerprint(tool_key, mode, request_payload)
        execution_path = str(Path(log_path).parent) if log_path else str(Path(output_path).parent) if output_path else ''
        tool_version = self._tool_version()
        with self._connect() as connection:
            cursor = connection.execute(
                """
                INSERT INTO runtime_jobs (
                    tool_key, requested_by, session_id, status, mode, input_path,
                    output_path, log_path, command_json, resources_json,
                    request_json, request_fingerprint, execution_path, tool_version,
                    db_version, mirror_log_path, created_at, last_seen_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                """,
                (
                    tool_key,
                    requested_by,
                    session_id,
                    status,
                    mode,
                    input_path,
                    output_path,
                    log_path,
                    json.dumps(command),
                    json.dumps(resources),
                    json.dumps(request_payload),
                    request_fingerprint,
                    execution_path,
                    tool_version,
                    self.RUNTIME_DB_VERSION,
                    str(self._job_mirror_log_path(-1, create_dir=False)),
                    self._utcnow(),
                    self._utcnow(),
                ),
            )
            runtime_job_id = int(cursor.lastrowid)
            mirror_path = str(self._job_mirror_log_path(runtime_job_id))
            connection.execute(
                'UPDATE runtime_jobs SET mirror_log_path = ? WHERE id = ?',
                (mirror_path, runtime_job_id),
            )
            connection.execute(
                """
                INSERT INTO runtime_job_logs (
                    runtime_job_id, source_log_path, mirror_log_path, content, byte_count, content_hash, updated_at
                ) VALUES (?, ?, ?, '', 0, ?, ?)
                ON CONFLICT(runtime_job_id) DO UPDATE SET
                    source_log_path = excluded.source_log_path,
                    mirror_log_path = excluded.mirror_log_path,
                    updated_at = excluded.updated_at
                """,
                (runtime_job_id, log_path, mirror_path, self._hash_text(''), self._utcnow()),
            )
            return runtime_job_id

    def update_job(self, runtime_job_id: int, **fields: Any) -> Optional[Dict[str, Any]]:
        if not fields:
            return self.get_job(runtime_job_id)
        columns = []
        values = []
        for key, value in fields.items():
            if key.endswith('_json') and not isinstance(value, str):
                value = json.dumps(value)
            columns.append(f'{key} = ?')
            values.append(value)
        columns.append('last_seen_at = ?')
        values.append(self._utcnow())
        values.append(runtime_job_id)
        with self._connect() as connection:
            connection.execute(
                f"UPDATE runtime_jobs SET {', '.join(columns)} WHERE id = ?",
                tuple(values),
            )
        return self.get_job(runtime_job_id)

    def append_event(self, runtime_job_id: int, level: str, message: str) -> None:
        with self._connect() as connection:
            connection.execute(
                'INSERT INTO runtime_events (runtime_job_id, level, message, created_at) VALUES (?, ?, ?, ?)',
                (runtime_job_id, level, message, self._utcnow()),
            )

    def get_job(self, runtime_job_id: int) -> Optional[Dict[str, Any]]:
        with self._connect() as connection:
            row = connection.execute(
                'SELECT * FROM runtime_jobs WHERE id = ?',
                (runtime_job_id,),
            ).fetchone()
        return self._row_to_job(row) if row else None

    def list_jobs(self, limit: int = 50) -> List[Dict[str, Any]]:
        with self._connect() as connection:
            rows = connection.execute(
                'SELECT * FROM runtime_jobs ORDER BY id DESC LIMIT ?',
                (limit,),
            ).fetchall()
        return [self._row_to_job(row) for row in rows]

    @classmethod
    def compute_request_fingerprint(cls, tool_key: str, mode: str, request_payload: Dict[str, Any]) -> str:
        payload = request_payload if isinstance(request_payload, dict) else {}
        raw_paths = payload.get('paths') if isinstance(payload.get('paths'), dict) else {}
        filtered_paths = {}
        for key, value in sorted(raw_paths.items()):
            if key in {'output_dir', 'output_path', 'log_path'}:
                continue
            normalized_value = str(value or '').strip()
            if normalized_value:
                filtered_paths[key] = normalized_value

        normalized_payload = {
            'tool_key': str(tool_key or '').strip(),
            'mode': str(mode or '').strip(),
            'variant': str(payload.get('variant') or '').strip(),
            'paths': filtered_paths,
            'ingest_only': bool(payload.get('ingest_only')),
        }
        return hashlib.sha256(
            json.dumps(normalized_payload, sort_keys=True, ensure_ascii=True).encode('utf-8', errors='ignore')
        ).hexdigest()

    def find_reusable_job(self, tool_key: str, mode: str, request_payload: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        request_fingerprint = self.compute_request_fingerprint(tool_key, mode, request_payload)
        placeholders = ', '.join('?' for _ in self.REUSABLE_JOB_STATUSES)
        with self._connect() as connection:
            row = connection.execute(
                f"""
                SELECT *
                FROM runtime_jobs
                WHERE request_fingerprint = ?
                  AND status IN ({placeholders})
                ORDER BY CASE status
                    WHEN 'RUNNING' THEN 0
                    WHEN 'PENDING' THEN 1
                    WHEN 'QUEUED' THEN 2
                    WHEN 'SUBMITTED' THEN 3
                    WHEN 'COMPLETED' THEN 4
                    ELSE 9
                END, id DESC
                LIMIT 1
                """,
                (request_fingerprint, *self.REUSABLE_JOB_STATUSES),
            ).fetchone()
        return self._row_to_job(row) if row else None

    def get_events(self, runtime_job_id: int) -> List[Dict[str, Any]]:
        with self._connect() as connection:
            rows = connection.execute(
                'SELECT level, message, created_at FROM runtime_events WHERE runtime_job_id = ? ORDER BY id',
                (runtime_job_id,),
            ).fetchall()
        return [dict(row) for row in rows]

    def get_job_log_record(self, runtime_job_id: int) -> Optional[Dict[str, Any]]:
        with self._connect() as connection:
            row = connection.execute(
                'SELECT * FROM runtime_job_logs WHERE runtime_job_id = ?',
                (runtime_job_id,),
            ).fetchone()
        if row is None:
            return None
        return {
            'runtime_job_id': row['runtime_job_id'],
            'source_log_path': row['source_log_path'] or '',
            'mirror_log_path': row['mirror_log_path'] or '',
            'content': row['content'] or '',
            'byte_count': int(row['byte_count'] or 0),
            'content_hash': row['content_hash'] or '',
            'updated_at': row['updated_at'] or '',
        }

    def list_job_artifacts(self, runtime_job_id: int) -> List[Dict[str, Any]]:
        with self._connect() as connection:
            rows = connection.execute(
                'SELECT * FROM runtime_job_artifacts WHERE runtime_job_id = ? ORDER BY artifact_type, relative_path, artifact_path',
                (runtime_job_id,),
            ).fetchall()
        artifacts: List[Dict[str, Any]] = []
        for row in rows:
            artifacts.append(
                {
                    'artifact_type': row['artifact_type'],
                    'artifact_path': row['artifact_path'],
                    'relative_path': row['relative_path'] or '',
                    'size_bytes': int(row['size_bytes'] or 0),
                    'exists': bool(row['exists_flag']),
                    'metadata': self._loads(row['metadata_json'], {}),
                    'created_at': row['created_at'],
                    'updated_at': row['updated_at'],
                }
            )
        return artifacts

    def _tool_version(self) -> str:
        explicit = str(os.environ.get('HPCC_TOOL_VERSION') or '').strip()
        if explicit:
            return explicit
        try:
            modified_at = datetime.fromtimestamp(self.repo_root.stat().st_mtime, tz=timezone.utc)
        except OSError:
            return 'unknown'
        return modified_at.strftime('%Y.%m.%d')

    def _job_state_dir(self, runtime_job_id: int, create_dir: bool = True) -> Path:
        base_dir = Path(self.db_path).resolve().parent / 'runtime_jobs' / str(runtime_job_id)
        if create_dir:
            base_dir.mkdir(parents=True, exist_ok=True)
        return base_dir

    def _job_mirror_log_path(self, runtime_job_id: int, create_dir: bool = True) -> Path:
        return self._job_state_dir(runtime_job_id, create_dir=create_dir) / 'runtime_console.log'

    @staticmethod
    def _hash_text(text: str) -> str:
        return hashlib.sha256((text or '').encode('utf-8', errors='ignore')).hexdigest()

    def _sync_job_log(self, runtime_job_id: int, source_log_path: str) -> str:
        mirror_path = self._job_mirror_log_path(runtime_job_id)
        source_path = Path(str(source_log_path or '').strip())
        existing_record = self.get_job_log_record(runtime_job_id) or {}

        if not source_path.exists() or not source_path.is_file():
            return str(mirror_path if mirror_path.exists() else source_path)

        source_size = source_path.stat().st_size
        stored_bytes = int(existing_record.get('byte_count') or 0)
        stored_content = str(existing_record.get('content') or '')
        if source_size == stored_bytes and mirror_path.exists():
            return str(mirror_path)

        if source_size < stored_bytes:
            new_content = source_path.read_text(encoding='utf-8', errors='replace')
            mirror_path.write_text(new_content, encoding='utf-8', errors='replace')
            byte_count = source_size
        else:
            with source_path.open('rb') as handle:
                handle.seek(stored_bytes)
                delta = handle.read()
            delta_text = delta.decode('utf-8', errors='replace')
            if stored_bytes == 0:
                new_content = delta_text
                mirror_path.write_text(delta_text, encoding='utf-8', errors='replace')
            else:
                new_content = stored_content + delta_text
                with mirror_path.open('a', encoding='utf-8', errors='replace') as handle:
                    handle.write(delta_text)
            byte_count = source_size

        now = self._utcnow()
        with self._connect() as connection:
            connection.execute(
                """
                INSERT INTO runtime_job_logs (
                    runtime_job_id, source_log_path, mirror_log_path, content, byte_count, content_hash, updated_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?)
                ON CONFLICT(runtime_job_id) DO UPDATE SET
                    source_log_path = excluded.source_log_path,
                    mirror_log_path = excluded.mirror_log_path,
                    content = excluded.content,
                    byte_count = excluded.byte_count,
                    content_hash = excluded.content_hash,
                    updated_at = excluded.updated_at
                """,
                (runtime_job_id, str(source_path), str(mirror_path), new_content, byte_count, self._hash_text(new_content), now),
            )
        return str(mirror_path)

    def _sync_job_artifacts(self, runtime_job_id: int, output_path: str) -> List[Dict[str, Any]]:
        root = Path(str(output_path or '').strip())
        # Python 3.9 on some RHEL builds raises PermissionError from both
        # Path.exists() and Path.is_dir() instead of returning False.
        # Use an explicit try/except to handle inaccessible output dirs.
        try:
            if not root.is_dir():
                return self.list_job_artifacts(runtime_job_id)
        except OSError:
            return self.list_job_artifacts(runtime_job_id)

        now = self._utcnow()
        remaining = self.MAX_INDEXED_ARTIFACTS
        with self._connect() as connection:
            # Mark all previously indexed artifacts as gone; the INSERT loop below
            # will set exists_flag = 1 for each path that still exists.  This avoids
            # a single large NOT IN (?, ?, …) clause that would exceed SQLite's
            # SQLITE_LIMIT_VARIABLE_NUMBER (999) when there are many artifacts.
            connection.execute(
                'UPDATE runtime_job_artifacts SET exists_flag = 0, updated_at = ? WHERE runtime_job_id = ?',
                (now, runtime_job_id),
            )
            for current_root, _, filenames in os.walk(root):
                for filename in sorted(filenames):
                    if remaining <= 0:
                        break
                    full_path = Path(current_root) / filename
                    lower_name = filename.lower()
                    if full_path.suffix.lower() not in self.ARTIFACT_SUFFIXES and 'report' not in lower_name:
                        continue

                    artifact_path = str(full_path)
                    remaining -= 1
                    try:
                        relative_path = os.path.relpath(artifact_path, str(root))
                    except ValueError:
                        relative_path = filename
                    try:
                        size_bytes = int(full_path.stat().st_size)
                    except OSError:
                        size_bytes = 0

                    metadata = {
                        'extension': full_path.suffix.lower(),
                        'filename': filename,
                    }
                    connection.execute(
                        """
                        INSERT INTO runtime_job_artifacts (
                            runtime_job_id, artifact_type, artifact_path, relative_path,
                            size_bytes, exists_flag, metadata_json, created_at, updated_at
                        ) VALUES (?, ?, ?, ?, ?, 1, ?, ?, ?)
                        ON CONFLICT(runtime_job_id, artifact_path) DO UPDATE SET
                            artifact_type = excluded.artifact_type,
                            relative_path = excluded.relative_path,
                            size_bytes = excluded.size_bytes,
                            exists_flag = excluded.exists_flag,
                            metadata_json = excluded.metadata_json,
                            updated_at = excluded.updated_at
                        """,
                        (
                            runtime_job_id,
                            self._artifact_type_for_path(full_path),
                            artifact_path,
                            relative_path,
                            size_bytes,
                            json.dumps(metadata),
                            now,
                            now,
                        ),
                    )
                if remaining <= 0:
                    break
        return self.list_job_artifacts(runtime_job_id)

    @staticmethod
    def _artifact_type_for_path(path: Path) -> str:
        lower_name = path.name.lower()
        if path.suffix.lower() in {'.html', '.htm'}:
            return 'html'
        if path.suffix.lower() in {'.hdf', '.hdf5', '.mf4'}:
            return 'hdf'
        if 'report' in lower_name:
            return 'report'
        if path.suffix.lower() == '.log':
            return 'log'
        return 'artifact'

    def _refresh_persisted_job(self, job: Dict[str, Any]) -> None:
        runtime_job_id = int(job['id'])
        request_payload = job.get('request') if isinstance(job.get('request'), dict) else {}
        request_fingerprint = job.get('request_fingerprint') or self.compute_request_fingerprint(job['tool_key'], job.get('mode') or '', request_payload)
        execution_path = str(Path(str(job.get('log_path') or '').strip()).parent) if str(job.get('log_path') or '').strip() else str(Path(str(job.get('output_path') or '').strip())) if str(job.get('output_path') or '').strip() else ''
        tool_version = job.get('tool_version') or self._tool_version()
        mirror_log_path = ''
        if str(job.get('log_path') or '').strip():
            mirror_log_path = self._sync_job_log(runtime_job_id, str(job['log_path']))
        artifacts = self._sync_job_artifacts(runtime_job_id, str(job.get('output_path') or ''))
        with self._connect() as connection:
            connection.execute(
                """
                UPDATE runtime_jobs
                SET request_fingerprint = ?, execution_path = ?, tool_version = ?,
                    db_version = ?, mirror_log_path = ?, last_seen_at = ?
                WHERE id = ?
                """,
                (
                    request_fingerprint,
                    execution_path,
                    tool_version,
                    self.RUNTIME_DB_VERSION,
                    mirror_log_path,
                    self._utcnow(),
                    runtime_job_id,
                ),
            )
        job['request_fingerprint'] = request_fingerprint
        job['execution_path'] = execution_path
        job['tool_version'] = tool_version
        job['db_version'] = self.RUNTIME_DB_VERSION
        job['mirror_log_path'] = mirror_log_path
        job['artifacts'] = artifacts
        log_record = self.get_job_log_record(runtime_job_id)
        if log_record is not None:
            job['console_log'] = log_record['content']
            job['log_size_bytes'] = log_record['byte_count']
            if not job['mirror_log_path']:
                job['mirror_log_path'] = log_record['mirror_log_path']

    def graph_payload(self, variant_key: Optional[str] = None) -> Dict[str, Any]:
        self._ensure_default_graph_variant()
        tools = self.list_tools()
        jobs = self.list_jobs(limit=20)
        variants = self.list_graph_variants()
        variant = self.get_graph_variant(variant_key) or self.get_graph_variant()
        merged_graph = self._merge_graph_state((variant or {}).get('graph', {}), tools)

        return {
            'tools': tools,
            'jobs': jobs,
            'variants': variants,
            'active_variant': (variant or {}).get('variant_key', 'default'),
            'selected_variant': variant or {},
            'baseline_variant_key': self.SYSTEM_BASELINE_VARIANT,
            'nodes': merged_graph['nodes'],
            'edges': merged_graph['edges'],
            'viewport': merged_graph['viewport'],
            'graph': merged_graph,
        }

    def _ensure_default_graph_variant(self) -> None:
        tools = self.list_tools()
        baseline_graph = self._default_graph_state(tools)
        baseline = self.get_graph_variant(self.SYSTEM_BASELINE_VARIANT)
        if baseline is None or not (baseline.get('graph') or {}).get('nodes'):
            self.save_graph_variant(
                {
                    'variant_key': self.SYSTEM_BASELINE_VARIANT,
                    'display_name': 'HPCC baseline',
                    'description': 'Built-in HPCC runtime topology.',
                    'graph': baseline_graph,
                    'is_default': False,
                },
                updated_by='system',
            )

        variants = self.list_graph_variants()
        if variants and any(variant.get('is_default') for variant in variants):
            return

        self.save_graph_variant(
            {
                'variant_key': self.DEFAULT_VARIANT,
                'display_name': 'Default topology',
                'description': 'Built-in HPCC runtime topology',
                'graph': baseline_graph,
                'is_default': True,
            },
            updated_by='system',
        )

    def _default_graph_state(self, tools: List[Dict[str, Any]]) -> Dict[str, Any]:
        return self._merge_graph_state(
            {
                'nodes': [
                    {'id': tool['tool_key'], 'position': self._default_position(tool['tool_key'], index)}
                    for index, tool in enumerate(tools)
                ],
                'edges': [
                    {'id': 'main-hyperlink', 'source': 'main_html', 'target': 'hyperlink', 'label': 'Open Hyperlink', 'notes': 'Hyperlink is opened from the main HTML dashboard.'},
                    {'id': 'main-can', 'source': 'main_html', 'target': 'can_kpi', 'label': 'CAN KPI request', 'notes': 'Main HTML queues CAN KPI runs through the broker.'},
                    {'id': 'main-udp', 'source': 'main_html', 'target': 'udp_kpi', 'label': 'UDP KPI request', 'notes': 'Main HTML queues UDP KPI runs through the broker.'},
                    {'id': 'main-int', 'source': 'main_html', 'target': 'interactive_plot', 'label': 'Interactive Plot only', 'notes': 'Interactive Plot can also run by itself from the KPI form.'},
                    {'id': 'main-rag', 'source': 'main_html', 'target': 'rag', 'label': 'Chat / RAG', 'notes': 'Chat requests are forwarded to the RAG service.'},
                    {'id': 'udp-int', 'source': 'udp_kpi', 'target': 'interactive_plot', 'label': 'ZMQ bridge', 'notes': 'UDP KPI can stream live KPI data to Interactive Plot over ZMQ.'},
                    {'id': 'can-int', 'source': 'can_kpi', 'target': 'interactive_plot', 'label': 'Shared output flow', 'notes': 'CAN KPI can run together with Interactive Plot in the coordinated broker flow.'},
                ],
                'viewport': {'x': 0, 'y': 0, 'zoom': 1},
            },
            tools,
        )

    def _merge_graph_state(self, graph_state: Dict[str, Any], tools: List[Dict[str, Any]]) -> Dict[str, Any]:
        raw_nodes = list((graph_state or {}).get('nodes') or [])
        raw_edges = list((graph_state or {}).get('edges') or [])
        viewport = dict((graph_state or {}).get('viewport') or {'x': 0, 'y': 0, 'zoom': 1})
        tool_keys = {tool['tool_key'] for tool in tools}

        merged_nodes: List[Dict[str, Any]] = []
        seen_nodes = set()
        for index, node in enumerate(raw_nodes):
            node_id = str(node.get('id', '')).strip()
            if not node_id or node_id in seen_nodes or node_id not in tool_keys:
                continue
            fallback_position = self._default_position(node_id, index)
            position = dict(node.get('position') or fallback_position)
            merged_nodes.append(
                {
                    'id': node_id,
                    'position': {
                        'x': float(position.get('x', fallback_position['x'])),
                        'y': float(position.get('y', fallback_position['y'])),
                    },
                }
            )
            seen_nodes.add(node_id)

        for index, tool in enumerate(tools):
            if tool['tool_key'] in seen_nodes:
                continue
            merged_nodes.append(
                {
                    'id': tool['tool_key'],
                    'position': self._default_position(tool['tool_key'], len(merged_nodes) + index),
                }
            )

        merged_edges: List[Dict[str, Any]] = []
        seen_edges = set()
        for edge in raw_edges:
            edge_id = str(edge.get('id', '')).strip() or f"edge-{edge.get('source', '')}-{edge.get('target', '')}"
            source = str(edge.get('source', '')).strip()
            target = str(edge.get('target', '')).strip()
            if not source or not target or source not in tool_keys or target not in tool_keys:
                continue
            if edge_id in seen_edges:
                continue
            merged_edges.append(
                {
                    'id': edge_id,
                    'source': source,
                    'target': target,
                    'label': str(edge.get('label', '')).strip() or f'{source} -> {target}',
                    'notes': str(edge.get('notes', '')).strip(),
                }
            )
            seen_edges.add(edge_id)

        return {
            'nodes': merged_nodes,
            'edges': merged_edges,
            'viewport': {
                'x': float(viewport.get('x', 0)),
                'y': float(viewport.get('y', 0)),
                'zoom': float(viewport.get('zoom', 1)),
            },
        }

    def _default_position(self, tool_key: str, index: int) -> Dict[str, float]:
        layout = {
            'main_html': {'x': 280.0, 'y': 40.0},
            'can_kpi': {'x': 40.0, 'y': 240.0},
            'udp_kpi': {'x': 280.0, 'y': 240.0},
            'interactive_plot': {'x': 520.0, 'y': 240.0},
            'rag': {'x': 520.0, 'y': 40.0},
            'hyperlink': {'x': 40.0, 'y': 40.0},
        }
        if tool_key in layout:
            return dict(layout[tool_key])

        row = index // 4
        col = index % 4
        return {'x': float(40 + (col * 240)), 'y': float(420 + (row * 180))}

    @staticmethod
    def _utcnow() -> str:
        return datetime.now(timezone.utc).isoformat().replace('+00:00', 'Z')

    @staticmethod
    def _loads(value: Optional[str], default: Any) -> Any:
        if not value:
            return default
        try:
            return json.loads(value)
        except (TypeError, json.JSONDecodeError):
            return default

    def _row_to_tool(self, row: sqlite3.Row) -> Dict[str, Any]:
        metadata = self._loads(row['metadata_json'], {})
        if not isinstance(metadata, dict):
            metadata = {}
        return {
            'tool_key': row['tool_key'],
            'display_name': row['display_name'],
            'category': row['category'],
            'image_path': row['image_path'],
            'entry_command': row['entry_command'],
            'service_url': row['service_url'],
            'input_hint': row['input_hint'],
            'output_hint': row['output_hint'],
            'notes': row['notes'],
            'metadata': metadata,
            'updated_by': row['updated_by'],
            'updated_at': row['updated_at'],
        }

    def _row_to_variant(self, row: sqlite3.Row) -> Dict[str, Any]:
        return {
            'variant_key': row['variant_key'],
            'display_name': row['display_name'],
            'description': row['description'] or '',
            'graph': self._loads(row['graph_json'], {'nodes': [], 'edges': [], 'viewport': {'x': 0, 'y': 0, 'zoom': 1}}),
            'is_default': bool(row['is_default']),
            'is_system': row['variant_key'] == self.SYSTEM_BASELINE_VARIANT,
            'updated_by': row['updated_by'],
            'updated_at': row['updated_at'],
        }

    def _row_to_job(self, row: sqlite3.Row) -> Dict[str, Any]:
        job = {
            'id': row['id'],
            'tool_key': row['tool_key'],
            'requested_by': row['requested_by'],
            'session_id': row['session_id'],
            'status': row['status'],
            'mode': row['mode'],
            'input_path': row['input_path'],
            'output_path': row['output_path'],
            'log_path': row['log_path'],
            'command': self._loads(row['command_json'], []),
            'resources': self._loads(row['resources_json'], {}),
            'request': self._loads(row['request_json'], {}),
            'pid': row['pid'],
            'return_code': row['return_code'],
            'error_message': row['error_message'],
            'request_fingerprint': row['request_fingerprint'],
            'execution_path': row['execution_path'],
            'tool_version': row['tool_version'],
            'db_version': row['db_version'],
            'mirror_log_path': row['mirror_log_path'],
            'reused_from_runtime_job_id': row['reused_from_runtime_job_id'],
            'last_seen_at': row['last_seen_at'],
            'created_at': row['created_at'],
            'started_at': row['started_at'],
            'completed_at': row['completed_at'],
        }
        job = self._derive_job_state(job)
        self._refresh_persisted_job(job)
        job['events'] = self.get_events(int(row['id']))
        if 'artifacts' not in job:
            job['artifacts'] = self.list_job_artifacts(int(row['id']))
        return job

    def _derive_job_state(self, job: Dict[str, Any]) -> Dict[str, Any]:
        if job.get('status') not in self.ACTIVE_JOB_STATUSES:
            return job

        log_path_value = str(job.get('log_path') or '').strip()
        if not log_path_value:
            return job

        log_path = Path(log_path_value)
        run_dir = log_path.parent
        launcher_log_path = run_dir / 'launcher.log'
        is_slurm_job = (
            str((job.get('resources') or {}).get('scheduler') or '').strip().lower() == 'slurm'
            or (run_dir / 'slurm_tmux_launcher.sh').exists()
            or (launcher_log_path.exists() and launcher_log_path != log_path)
        )
        if not is_slurm_job:
            return job

        console_head = self._read_text_window(log_path, max_bytes=16384, from_start=True)
        console_tail = self._read_text_window(log_path, max_bytes=32768, from_start=False)
        launcher_head = (
            self._read_text_window(launcher_log_path, max_bytes=16384, from_start=True)
            if launcher_log_path.exists() and launcher_log_path != log_path
            else console_head
        )
        launcher_tail = (
            self._read_text_window(launcher_log_path, max_bytes=32768, from_start=False)
            if launcher_log_path.exists() and launcher_log_path != log_path
            else console_tail
        )

        console_text = console_head + '\n' + console_tail
        start_time = self._extract_timestamp(
            console_text,
            r'^(?:\[(?:MAIN|UDP|INTERACTIVE)\]\s+)?\[(?:MAIN|UDP|INTERACTIVE)\] START: (.+)$',
        )
        has_runtime_output = self._has_runtime_output(console_text)
        if start_time and not job.get('started_at'):
            job['started_at'] = start_time
        elif has_runtime_output and not job.get('started_at'):
            job['started_at'] = self._path_mtime(log_path) or self._path_mtime(launcher_log_path)

        exit_status, exit_path = self._read_exit_status(run_dir)
        if exit_status is not None:
            job['status'] = 'COMPLETED' if exit_status == 0 else 'FAILED'
            if job.get('return_code') is None:
                job['return_code'] = exit_status
            if not job.get('completed_at'):
                job['completed_at'] = (
                    self._extract_timestamp(launcher_tail, r'^\[broker\] END: (.+)$')
                    or self._path_mtime(exit_path)
                    or self._path_mtime(log_path)
                )
            job.pop('status_detail', None)
            return job

        broker_return_code = self._extract_return_code(launcher_tail)
        if broker_return_code is not None:
            job['status'] = 'COMPLETED' if broker_return_code == 0 else 'FAILED'
            if job.get('return_code') is None:
                job['return_code'] = broker_return_code
            if not job.get('completed_at'):
                job['completed_at'] = (
                    self._extract_timestamp(launcher_tail, r'^\[broker\] END: (.+)$')
                    or self._path_mtime(launcher_log_path)
                    or self._path_mtime(log_path)
                )
            job.pop('status_detail', None)
            return job

        launcher_failure_code, launcher_failure_detail = self._extract_launcher_failure(launcher_head + '\n' + launcher_tail)
        if launcher_failure_code is not None:
            job['status'] = 'FAILED'
            if job.get('return_code') is None:
                job['return_code'] = launcher_failure_code
            if launcher_failure_detail and not job.get('error_message'):
                job['error_message'] = launcher_failure_detail
            if not job.get('completed_at'):
                job['completed_at'] = (
                    self._extract_timestamp(launcher_tail, r'^\[broker\] END: (.+)$')
                    or self._path_mtime(launcher_log_path)
                    or self._path_mtime(log_path)
                )
            job.pop('status_detail', None)
            return job

        if start_time or has_runtime_output:
            job['status'] = 'RUNNING'
            job.pop('status_detail', None)
            return job

        job['status'] = 'PENDING'
        job['status_detail'] = self._extract_wait_detail(launcher_head + '\n' + launcher_tail)
        return job

    @staticmethod
    def _read_text_window(path: Path, max_bytes: int, from_start: bool) -> str:
        if max_bytes <= 0 or not path.exists() or not path.is_file():
            return ''

        try:
            with path.open('rb') as handle:
                if from_start:
                    data = handle.read(max_bytes)
                else:
                    size = handle.seek(0, os.SEEK_END)
                    handle.seek(max(size - max_bytes, 0))
                    data = handle.read(max_bytes)
        except OSError:
            return ''

        return data.decode('utf-8', errors='replace')

    @staticmethod
    def _normalize_timestamp(raw_value: str) -> str:
        candidate = (raw_value or '').strip()
        if not candidate:
            return ''

        parsed = RuntimeStore._parse_datetime(candidate)
        if parsed is None:
            return ''

        if parsed.tzinfo is None:
            parsed = parsed.replace(tzinfo=timezone.utc)

        return parsed.astimezone(timezone.utc).isoformat().replace('+00:00', 'Z')

    @staticmethod
    def _parse_datetime(raw_value: str) -> Optional[datetime]:
        candidate = (raw_value or '').strip()
        if not candidate:
            return None

        normalized = candidate.replace('Z', '+00:00')
        parser = getattr(datetime, 'fromisoformat', None)
        if parser is not None:
            try:
                return parser(normalized)
            except ValueError:
                pass

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
                return datetime.strptime(normalized, fmt)
            except ValueError:
                continue
        return None

    def _extract_timestamp(self, text: str, pattern: str) -> str:
        match = re.search(pattern, text or '', re.MULTILINE)
        if not match:
            return ''
        return self._normalize_timestamp(match.group(1))

    @staticmethod
    def _has_runtime_output(text: str) -> bool:
        for line in (text or '').splitlines():
            normalized = line.strip()
            if not normalized:
                continue
            if re.match(
                r'^(?:\[(?:MAIN|UDP|INTERACTIVE)\]\s+)?\[(?:MAIN|UDP|INTERACTIVE)\] (COMMAND|START|EXIT_STATUS):',
                normalized,
            ):
                continue
            if normalized.startswith(('[MAIN]', '[UDP]', '[INTERACTIVE]')):
                return True
        return False

    @staticmethod
    def _extract_return_code(text: str) -> Optional[int]:
        match = re.search(r'^\[broker\] RETURN_CODE: (-?\d+)$', text or '', re.MULTILINE)
        if not match:
            return None
        try:
            return int(match.group(1))
        except ValueError:
            return None

    @staticmethod
    def _extract_launcher_failure(text: str):
        detail = ''
        for line in reversed((text or '').splitlines()):
            normalized = line.strip()
            lowered = normalized.lower()
            if not normalized:
                continue
            if normalized.startswith('srun: error:') or 'error connecting to /tmp/tmux-' in lowered:
                detail = normalized
                break

        match = re.search(r'exited with exit code (\d+)', text or '', re.IGNORECASE)
        if match:
            try:
                return int(match.group(1)), detail or match.group(0)
            except ValueError:
                return None, detail

        if detail:
            return 1, detail

        return None, ''

    @staticmethod
    def _path_mtime(path: Optional[Path]) -> str:
        if path is None:
            return ''
        try:
            modified_at = datetime.fromtimestamp(path.stat().st_mtime, tz=timezone.utc)
        except OSError:
            return ''
        return modified_at.isoformat().replace('+00:00', 'Z')

    @staticmethod
    def _extract_wait_detail(text: str) -> str:
        for line in (text or '').splitlines():
            normalized = line.strip()
            lowered = normalized.lower()
            if not normalized:
                continue
            if 'queued and waiting for resources' in lowered:
                return 'Waiting for Slurm resources'
            if normalized.startswith('srun: job '):
                return normalized
        return 'Waiting for Slurm allocation'

    @staticmethod
    def _read_exit_status(run_dir: Path):
        exit_codes: Dict[str, int] = {}
        exit_paths: Dict[str, Path] = {}
        for name in ('compute_main.exit', 'compute_interactive.exit', 'compute_udp.exit'):
            path = run_dir / name
            if not path.exists():
                continue
            try:
                raw_value = path.read_text(encoding='utf-8', errors='replace').strip()
                exit_codes[name] = int(raw_value)
                exit_paths[name] = path
            except (OSError, ValueError):
                continue

        if not exit_codes:
            return None, None

        for name in ('compute_main.exit', 'compute_interactive.exit', 'compute_udp.exit'):
            code = exit_codes.get(name)
            if code is not None and code != 0:
                return code, exit_paths.get(name)

        for name in ('compute_interactive.exit', 'compute_main.exit', 'compute_udp.exit'):
            code = exit_codes.get(name)
            if code is not None:
                return code, exit_paths.get(name)

        return None, None