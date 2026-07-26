from __future__ import annotations

import os
from datetime import datetime, timezone
from typing import Optional

from sqlalchemy import (
    Boolean,
    Column,
    DateTime,
    Integer,
    MetaData,
    String,
    Table,
    Text,
    create_engine,
)
from sqlalchemy.engine import Engine


def _get_database_url() -> Optional[str]:
    url = (os.environ.get("HYPERLINK_DATABASE_URL") or os.environ.get("DATABASE_URL") or "").strip()
    if not url:
        return None
    # Enforce Postgres-only to avoid silently using SQLite.
    if not (url.startswith("postgresql://") or url.startswith("postgres://")):
        raise ValueError("Database URL must be PostgreSQL (postgresql://...)")
    return url


_METADATA = MetaData()

hyperlink_mappings = Table(
    "hyperlink_mappings",
    _METADATA,
    Column("id", Integer, primary_key=True, autoincrement=True),
    Column("created_at", DateTime(timezone=True), nullable=False),
    Column("mode", String(32), nullable=False),  # local | cluster
    Column("server", String(128), nullable=True),
    Column("username", String(128), nullable=True),
    Column("input_html", Text, nullable=True),
    Column("input_video", Text, nullable=True),
    Column("remote_base_path", Text, nullable=True),
    Column("remote_html_path", Text, nullable=True),
    Column("remote_video_path", Text, nullable=True),
    Column("output_root", Text, nullable=True),
    Column("output_html_root", Text, nullable=True),
    Column("output_video_root", Text, nullable=True),
    Column("key", String(128), nullable=True),
    Column("success", Boolean, nullable=False, default=True),
    Column("message", Text, nullable=True),
)

hyperlink_events = Table(
    "hyperlink_events",
    _METADATA,
    Column("id", Integer, primary_key=True, autoincrement=True),
    Column("created_at", DateTime(timezone=True), nullable=False),
    Column("event_type", String(64), nullable=False),
    Column("server", String(128), nullable=True),
    Column("username", String(128), nullable=True),
    Column("remote_base_path", Text, nullable=True),
    Column("remote_html_path", Text, nullable=True),
    Column("remote_video_path", Text, nullable=True),
    Column("output_path", Text, nullable=True),
    Column("key", String(128), nullable=True),
    Column("success", Boolean, nullable=False, default=True),
    Column("message", Text, nullable=True),
)


def create_engine_if_configured() -> Optional[Engine]:
    url = _get_database_url()
    if not url:
        return None
    return create_engine(url, pool_pre_ping=True)


def init_db(engine: Engine) -> None:
    _METADATA.create_all(engine)


def log_event(
    engine: Optional[Engine],
    *,
    event_type: str,
    server: Optional[str] = None,
    username: Optional[str] = None,
    remote_base_path: Optional[str] = None,
    remote_html_path: Optional[str] = None,
    remote_video_path: Optional[str] = None,
    output_path: Optional[str] = None,
    key: Optional[str] = None,
    success: bool = True,
    message: Optional[str] = None,
) -> None:
    if engine is None:
        return
    now = datetime.now(timezone.utc)
    with engine.begin() as conn:
        conn.execute(
            hyperlink_events.insert().values(
                created_at=now,
                event_type=event_type,
                server=server,
                username=username,
                remote_base_path=remote_base_path,
                remote_html_path=remote_html_path,
                remote_video_path=remote_video_path,
                output_path=output_path,
                key=key,
                success=success,
                message=message,
            )
        )


def log_mapping(
    engine: Optional[Engine],
    *,
    mode: str,
    server: Optional[str] = None,
    username: Optional[str] = None,
    input_html: Optional[str] = None,
    input_video: Optional[str] = None,
    remote_base_path: Optional[str] = None,
    remote_html_path: Optional[str] = None,
    remote_video_path: Optional[str] = None,
    output_root: Optional[str] = None,
    output_html_root: Optional[str] = None,
    output_video_root: Optional[str] = None,
    key: Optional[str] = None,
    success: bool = True,
    message: Optional[str] = None,
) -> None:
    if engine is None:
        return
    now = datetime.now(timezone.utc)
    with engine.begin() as conn:
        conn.execute(
            hyperlink_mappings.insert().values(
                created_at=now,
                mode=mode,
                server=server,
                username=username,
                input_html=input_html,
                input_video=input_video,
                remote_base_path=remote_base_path,
                remote_html_path=remote_html_path,
                remote_video_path=remote_video_path,
                output_root=output_root,
                output_html_root=output_html_root,
                output_video_root=output_video_root,
                key=key,
                success=success,
                message=message,
            )
        )
