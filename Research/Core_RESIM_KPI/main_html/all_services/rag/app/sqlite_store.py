from __future__ import annotations

import json
import sqlite3
from pathlib import Path
from typing import Any


class SQLiteLogStore:
    def __init__(self, db_path: Path) -> None:
        self.db_path = db_path
        self._init_db()

    def _init_db(self) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute(
                """
                CREATE TABLE IF NOT EXISTS ingestions (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    source_path TEXT NOT NULL,
                    text_hash TEXT NOT NULL,
                    chunk_count INTEGER NOT NULL,
                    duplicate_round INTEGER NOT NULL,
                    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
                )
                """
            )
            self._ensure_column(conn, 'ingestions', 'source_name', 'TEXT')
            self._ensure_column(conn, 'ingestions', 'report_kind', 'TEXT')
            self._ensure_column(conn, 'ingestions', 'metadata_json', 'TEXT')
            self._ensure_column(conn, 'ingestions', 'updated_at', 'DATETIME')
            conn.execute(
                """
                CREATE TABLE IF NOT EXISTS queries (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    question TEXT NOT NULL,
                    answer TEXT NOT NULL,
                    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
                )
                """
            )
            conn.execute(
                """
                CREATE TABLE IF NOT EXISTS document_chunks (
                    chunk_id TEXT PRIMARY KEY,
                    source_path TEXT NOT NULL,
                    section_title TEXT,
                    section_kind TEXT,
                    chunk_index INTEGER NOT NULL,
                    chunk_text TEXT NOT NULL,
                    metadata_json TEXT,
                    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
                )
                """
            )

    @staticmethod
    def _ensure_column(conn: sqlite3.Connection, table_name: str, column_name: str, column_type: str) -> None:
        columns = {row[1] for row in conn.execute(f'PRAGMA table_info({table_name})').fetchall()}
        if column_name in columns:
            return
        conn.execute(f'ALTER TABLE {table_name} ADD COLUMN {column_name} {column_type}')

    def ingestion_exists(self, source_path: str, text_hash: str) -> bool:
        with sqlite3.connect(self.db_path) as conn:
            row = conn.execute(
                "SELECT 1 FROM ingestions WHERE source_path = ? AND text_hash = ? LIMIT 1",
                (source_path, text_hash),
            ).fetchone()
        return row is not None

    def log_ingestion(
        self,
        source_path: str,
        text_hash: str,
        chunk_count: int,
        duplicate_round: int,
        source_name: str = '',
        report_kind: str = '',
        metadata: dict[str, Any] | None = None,
        chunks: list[dict[str, Any]] | None = None,
    ) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("DELETE FROM ingestions WHERE source_path = ?", (source_path,))
            conn.execute(
                """
                INSERT INTO ingestions (
                    source_path, text_hash, chunk_count, duplicate_round,
                    source_name, report_kind, metadata_json, updated_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?, CURRENT_TIMESTAMP)
                """,
                (
                    source_path,
                    text_hash,
                    chunk_count,
                    duplicate_round,
                    source_name,
                    report_kind,
                    json.dumps(metadata or {}, ensure_ascii=True),
                ),
            )
            conn.execute("DELETE FROM document_chunks WHERE source_path = ?", (source_path,))
            for chunk in chunks or []:
                chunk_metadata = chunk.get('metadata') if isinstance(chunk.get('metadata'), dict) else {}
                conn.execute(
                    """
                    INSERT INTO document_chunks (
                        chunk_id, source_path, section_title, section_kind,
                        chunk_index, chunk_text, metadata_json
                    ) VALUES (?, ?, ?, ?, ?, ?, ?)
                    """,
                    (
                        str(chunk.get('id') or ''),
                        source_path,
                        str(chunk_metadata.get('section_title') or ''),
                        str(chunk_metadata.get('section_kind') or ''),
                        int(chunk_metadata.get('chunk_index') or 0),
                        str(chunk.get('text') or ''),
                        json.dumps(chunk_metadata, ensure_ascii=True),
                    ),
                )

    def log_query(self, question: str, answer: str) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute(
                "INSERT INTO queries (question, answer) VALUES (?, ?)",
                (question, answer),
            )

    def clear_ingestions(self) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("DELETE FROM ingestions")
            conn.execute("DELETE FROM document_chunks")

    def clear_queries(self) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("DELETE FROM queries")

    def stats(self) -> dict[str, Any]:
        with sqlite3.connect(self.db_path) as conn:
            ingestions = conn.execute("SELECT COUNT(*) FROM ingestions").fetchone()[0]
            queries = conn.execute("SELECT COUNT(*) FROM queries").fetchone()[0]
            chunks = conn.execute("SELECT COUNT(*) FROM document_chunks").fetchone()[0]
        return {"ingestions": ingestions, "queries": queries, "chunks": chunks}
