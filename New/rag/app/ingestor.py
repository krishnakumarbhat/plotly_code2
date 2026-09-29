from __future__ import annotations

import hashlib
import os
from pathlib import Path
from typing import Any

from app.html_parser import HtmlParser
from app.rag_engine import RagEngine
from app.sqlite_store import SQLiteLogStore


class HtmlIngestor:
    def __init__(
        self,
        html_root: Path,
        file_keyword: str,
        exclude_file_keyword: str,
        allow_all_html_fallback: bool,
        max_files_per_ingest: int,
        max_file_bytes: int,
        supported_extensions: list[str],
        exclude_dirs: set[str],
        parser: HtmlParser,
        rag_engine: RagEngine,
        sqlite_store: SQLiteLogStore,
    ) -> None:
        self.html_root = html_root
        self.file_keyword = file_keyword
        self.exclude_file_keyword = exclude_file_keyword
        self.allow_all_html_fallback = allow_all_html_fallback
        self.max_files_per_ingest = max_files_per_ingest
        self.max_file_bytes = max_file_bytes
        self.supported_extensions = {extension.lower() for extension in supported_extensions}
        self.exclude_dirs = {directory.lower() for directory in exclude_dirs}
        self.parser = parser
        self.rag_engine = rag_engine
        self.sqlite_store = sqlite_store

    def _find_html_files(self) -> list[Path]:
        keywords = [keyword.strip() for keyword in self.file_keyword.split(",") if keyword.strip()]
        exclude_keywords = [
            keyword.strip().lower()
            for keyword in self.exclude_file_keyword.split(",")
            if keyword.strip()
        ]
        html_files: list[Path] = []
        seen_paths: set[str] = set()

        for root, dirs, files in os.walk(self.html_root):
            dirs[:] = [directory for directory in dirs if directory.lower() not in self.exclude_dirs]
            root_path = Path(root)
            for name in files:
                path = root_path / name
                if path.suffix.lower() not in self.supported_extensions:
                    continue
                if self.max_file_bytes > 0 and path.stat().st_size > self.max_file_bytes:
                    continue
                path_name_lower = path.name.lower()
                if any(exclude_keyword in path_name_lower for exclude_keyword in exclude_keywords):
                    continue
                if keywords and not any(keyword.lower() in path_name_lower for keyword in keywords):
                    continue
                key = str(path).lower()
                if key in seen_paths:
                    continue
                seen_paths.add(key)
                html_files.append(path)

        if not html_files and self.allow_all_html_fallback and keywords:
            self.file_keyword = ""
            try:
                return self._find_html_files()
            finally:
                self.file_keyword = ",".join(keywords)

        html_files.sort(key=lambda path: str(path).lower())
        if self.max_files_per_ingest > 0:
            html_files = html_files[: self.max_files_per_ingest]

        return html_files

    def _sample_all_html_files(self, limit: int = 5) -> list[str]:
        found: list[str] = []
        seen_paths: set[str] = set()
        for root, dirs, files in os.walk(self.html_root):
            dirs[:] = [directory for directory in dirs if directory.lower() not in self.exclude_dirs]
            root_path = Path(root)
            for name in files:
                path = root_path / name
                if path.suffix.lower() not in self.supported_extensions:
                    continue
                if self.max_file_bytes > 0 and path.stat().st_size > self.max_file_bytes:
                    continue
                key = str(path).lower()
                if key in seen_paths:
                    continue
                seen_paths.add(key)
                found.append(str(path))
                if len(found) >= limit:
                    return found
        return found

    def ingest(self, run_vector: bool = False) -> dict[str, Any]:
        if not self.html_root.exists():
            raise FileNotFoundError(f"HTML root path not found: {self.html_root}")

        html_files = self._find_html_files()

        if not html_files:
            return {
                "html_root": str(self.html_root),
                "keyword": self.file_keyword,
                "fallback_to_all_html": self.allow_all_html_fallback,
                "files_processed": 0,
                "chunks_added": 0,
                "note": "No matching .html/.htm files found for keyword filter. Existing index remains unchanged.",
                "available_html_sample": self._sample_all_html_files(limit=8),
            }

        files_processed = 0
        total_chunks = 0
        skipped_duplicates = 0

        for file_path in html_files:
            parsed = self.parser.parse_file_sections(file_path)
            text = parsed['text']
            if not text:
                continue

            text_hash = hashlib.sha256(text.encode("utf-8", errors="ignore")).hexdigest()
            source_path = str(file_path)
            already_exists = self.sqlite_store.ingestion_exists(source_path, text_hash)
            if already_exists:
                skipped_duplicates += 1
                continue

            stored_chunks = self.rag_engine.add_sections(
                sections=parsed.get('sections') or [],
                source_path=source_path,
                duplicate_round=1,
            )
            chunk_count = len(stored_chunks)

            self.sqlite_store.log_ingestion(
                source_path=source_path,
                text_hash=text_hash,
                chunk_count=chunk_count,
                duplicate_round=1,
                source_name=str((parsed.get('metadata') or {}).get('source_name') or file_path.name),
                report_kind=str((parsed.get('metadata') or {}).get('report_kind') or 'report'),
                metadata=parsed.get('metadata') or {},
                chunks=stored_chunks,
            )
            total_chunks += chunk_count

            files_processed += 1

        return {
            "html_root": str(self.html_root),
            "keyword": self.file_keyword,
            "fallback_to_all_html": self.allow_all_html_fallback,
            "matched_files": len(html_files),
            "sample_files": [str(path) for path in html_files[:5]],
            "files_processed": files_processed,
            "chunks_added": total_chunks,
            "skipped_duplicates": skipped_duplicates,
        }
