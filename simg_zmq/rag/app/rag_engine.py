from __future__ import annotations

import hashlib
import json
import logging
import math
import os
import re
import socket
import subprocess
import threading
import time
import urllib.error
import urllib.request
from pathlib import Path
from typing import Any, Optional

try:
    import chromadb
except ImportError:  # pragma: no cover - validated at runtime in the deployment image
    chromadb = None




class RagEngine:
    def __init__(
        self,
        chroma_path: str,
        collection_name: str,
        vector_backend: str,
        vector_store_json_path: str,
        qwen_gguf_path: str,
        qwen_fallback_gguf_path: str,
        qwen_model_id: str,
        llm_backend: str,
        llama_server_path: str,
        llama_server_base_url: str,
        llama_server_host: str,
        llama_server_port: int,
        llama_server_autostart: bool,
        llm_context_window: int,
        llm_max_new_tokens: int,
        llm_similarity_top_k: int,
        llm_n_threads: int,
        llm_n_batch: int,
        llm_n_gpu_layers: int,
        chunk_size: int,
        chunk_overlap: int,
        embedding_dimension: int,
    ) -> None:
        self._vector_backend = (vector_backend or 'simple').strip().lower()
        self._collection_name = (collection_name or 'runtime_validation_logs').strip() or 'runtime_validation_logs'
        self._chroma_path = Path(chroma_path).expanduser() if chroma_path else Path(vector_store_json_path).expanduser().parent / 'chroma.db'
        self._chroma_path.mkdir(parents=True, exist_ok=True)
        self._vector_store_json_path = Path(vector_store_json_path)
        self._vector_store_json_path.parent.mkdir(parents=True, exist_ok=True)
        self._top_k = max(1, llm_similarity_top_k)
        self._chunk_size = max(400, chunk_size)
        self._chunk_overlap = max(0, min(chunk_overlap, self._chunk_size // 2))
        self._embedding_dimension = max(64, int(embedding_dimension or 384))
        self._llm_backend = (llm_backend or 'simple').strip().lower()
        self._qwen_gguf_path = Path(qwen_gguf_path).expanduser() if qwen_gguf_path else None
        self._qwen_fallback_gguf_path = Path(qwen_fallback_gguf_path).expanduser() if qwen_fallback_gguf_path else None
        self._qwen_model_id = (qwen_model_id or '').strip()
        self._llama_server_path = Path(llama_server_path).expanduser() if llama_server_path else None
        self._llama_server_base_url = (llama_server_base_url or '').rstrip('/')
        self._llama_server_host = (llama_server_host or '127.0.0.1').strip() or '127.0.0.1'
        self._llama_server_port = int(llama_server_port or 8081)
        self._llama_server_autostart = bool(llama_server_autostart)
        self._llm_context_window = max(1024, int(llm_context_window or 4096))
        self._llm_max_new_tokens = max(64, int(llm_max_new_tokens or 384))
        self._llm_n_threads = max(1, int(llm_n_threads or (os.cpu_count() or 8)))
        self._llm_n_batch = max(32, int(llm_n_batch or 512))
        self._llm_n_gpu_layers = max(0, int(llm_n_gpu_layers or 0))
        self._llama_server_process: Optional[subprocess.Popen] = None
        self._llama_server_lock = threading.Lock()
        self._client = None
        self._collection = self._create_collection()
        self._documents = self._load_documents()

    @property
    def document_count(self) -> int:
        return len(self._documents)

    def _create_collection(self):
        if self._vector_backend != 'chroma' or chromadb is None:
            return None
        client = chromadb.PersistentClient(path=str(self._chroma_path))
        self._client = client
        return client.get_or_create_collection(
            name=self._collection_name,
            metadata={'hnsw:space': 'cosine'},
            embedding_function=None,
        )

    def _load_documents(self) -> list[dict[str, Any]]:
        if self._collection is not None:
            payload = self._collection.get(include=['documents', 'metadatas'])
            documents: list[dict[str, Any]] = []
            ids = payload.get('ids') or []
            texts = payload.get('documents') or []
            metadatas = payload.get('metadatas') or []
            for document_id, text, metadata in zip(ids, texts, metadatas):
                if not text or not isinstance(metadata, dict):
                    continue
                source_path = str(metadata.get('source_path') or '').strip()
                if not source_path:
                    continue
                documents.append(
                    {
                        'id': str(document_id),
                        'source_path': source_path,
                        'duplicate_round': int(metadata.get('duplicate_round') or 1),
                        'text': str(text),
                        'metadata': dict(metadata),
                    }
                )
            return documents

        if not self._vector_store_json_path.exists():
            return []
        try:
            raw_items = json.loads(self._vector_store_json_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            return []
        if not isinstance(raw_items, list):
            return []

        documents: list[dict[str, Any]] = []
        for item in raw_items:
            if not isinstance(item, dict):
                continue
            text = str(item.get("text") or "").strip()
            source_path = str(item.get("source_path") or "").strip()
            if not text or not source_path:
                continue
            documents.append(
                {
                    "id": str(item.get("id") or self._document_id(source_path, text, 1, len(documents))),
                    "source_path": source_path,
                    "duplicate_round": int(item.get("duplicate_round") or 1),
                    "text": text,
                    "metadata": item.get('metadata') or {},
                }
            )
        return documents

    def _persist_documents(self) -> None:
        if self._collection is not None:
            return
        payload = [
            {
                "id": document["id"],
                "source_path": document["source_path"],
                "duplicate_round": document["duplicate_round"],
                "text": document["text"],
                "metadata": document.get('metadata') or {},
            }
            for document in self._documents
        ]
        self._vector_store_json_path.write_text(
            json.dumps(payload, ensure_ascii=True, indent=2),
            encoding="utf-8",
        )

    @staticmethod
    def _document_id(source_path: str, text: str, duplicate_round: int, chunk_index: int) -> str:
        digest = hashlib.sha256(
            f"{source_path}|{duplicate_round}|{chunk_index}|{text}".encode("utf-8", errors="ignore")
        ).hexdigest()
        return digest[:16]

    def reset_vector_store(self) -> None:
        self._documents = []
        if self._client is not None:
            try:
                self._client.delete_collection(self._collection_name)
            except Exception:
                pass
            self._collection = self._create_collection()
        if self._vector_store_json_path.exists():
            self._vector_store_json_path.unlink()

    def add_text(self, text: str, source_path: str, duplicate_round: int = 1) -> int:
        sections = [{'title': Path(source_path).stem, 'kind': 'text', 'text': text, 'metadata': {}}]
        return len(self.add_sections(sections=sections, source_path=source_path, duplicate_round=duplicate_round))

    def add_sections(
        self,
        sections: list[dict[str, Any]],
        source_path: str,
        duplicate_round: int = 1,
    ) -> list[dict[str, Any]]:
        stored_chunks: list[dict[str, Any]] = []
        collection_ids: list[str] = []
        collection_documents: list[str] = []
        collection_metadatas: list[dict[str, Any]] = []
        collection_embeddings: list[list[float]] = []

        for chunk_index, section in enumerate(sections, start=1):
            text = str(section.get('text') or '').strip()
            if not text:
                continue

            chunk_id = self._document_id(source_path, text, duplicate_round, chunk_index)
            metadata = self._prepare_metadata(source_path, section, duplicate_round, chunk_index)
            record = {
                'id': chunk_id,
                'source_path': source_path,
                'duplicate_round': duplicate_round,
                'text': text,
                'metadata': metadata,
            }
            stored_chunks.append(record)
            self._documents.append(record)

            if self._collection is not None:
                collection_ids.append(chunk_id)
                collection_documents.append(text)
                collection_metadatas.append(metadata)
                collection_embeddings.append(self._embed_text(text))

        if self._collection is not None and collection_ids:
            kwargs = dict(ids=collection_ids, documents=collection_documents, metadatas=collection_metadatas, embeddings=collection_embeddings)
            self._collection.upsert(**kwargs)
        else:
            self._persist_documents()

        return stored_chunks

    def _prepare_metadata(
        self,
        source_path: str,
        section: dict[str, Any],
        duplicate_round: int,
        chunk_index: int,
    ) -> dict[str, Any]:
        section_metadata = section.get('metadata') if isinstance(section.get('metadata'), dict) else {}
        scenario_tags = section_metadata.get('scenario_tags') or []
        if isinstance(scenario_tags, str):
            scenario_tags = [tag for tag in scenario_tags.split('|') if tag]
        elif not isinstance(scenario_tags, list):
            scenario_tags = []

        return {
            'source_path': source_path,
            'source_name': str(section_metadata.get('source_name') or Path(source_path).name),
            'report_kind': str(section_metadata.get('report_kind') or 'report'),
            'document_title': str(section_metadata.get('document_title') or Path(source_path).stem),
            'section_title': str(section.get('title') or section_metadata.get('document_title') or Path(source_path).stem),
            'section_kind': str(section.get('kind') or 'section'),
            'scenario_tags': '|'.join(str(tag).strip() for tag in scenario_tags if str(tag).strip()),
            'duplicate_round': int(duplicate_round),
            'chunk_index': int(chunk_index),
        }

    def _embed_text(self, text: str) -> list[float]:
        embeddings = self._llama_embed_text(text)
        if embeddings:
            return embeddings
        return self._hash_embed_text(text)

    def _llama_embed_text(self, text: str) -> Optional[list[float]]:
        if not self._llama_server_base_url:
            return None
        url = self._llama_server_base_url + '/v1/embeddings'
        payload = json.dumps({'input': text, 'model': 'default'}).encode('utf-8')
        request = urllib.request.Request(
            url, data=payload, headers={'Content-Type': 'application/json'}, method='POST'
        )
        try:
            with urllib.request.urlopen(request, timeout=5) as response:
                body = json.loads(response.read().decode('utf-8'))
        except Exception:
            return None
        data = body.get('data') or []
        if not data:
            return None
        embedding = data[0].get('embedding')
        if embedding and len(embedding) == self._embedding_dimension:
            return embedding
        return None

    def _hash_embed_text(self, text: str) -> list[float]:
        vector = [0.0] * self._embedding_dimension
        tokens = self._tokenize(text)
        if not tokens:
            return vector

        for token in tokens:
            digest = hashlib.sha256(token.encode('utf-8', errors='ignore')).digest()
            index = int.from_bytes(digest[:4], 'big') % self._embedding_dimension
            sign = 1.0 if digest[4] % 2 == 0 else -1.0
            weight = 1.0 + min(len(token), 24) / 24.0
            vector[index] += sign * weight

        norm = math.sqrt(sum(component * component for component in vector)) or 1.0
        return [component / norm for component in vector]

    def _chunk_text(self, text: str) -> list[str]:
        normalized = re.sub(r"\s+", " ", text).strip()
        if not normalized:
            return []
        if len(normalized) <= self._chunk_size:
            return [normalized]

        chunks: list[str] = []
        start = 0
        while start < len(normalized):
            end = min(len(normalized), start + self._chunk_size)
            if end < len(normalized):
                boundary = normalized.rfind(" ", start, end)
                if boundary > start + 200:
                    end = boundary
            chunk = normalized[start:end].strip()
            if chunk:
                chunks.append(chunk)
            if end >= len(normalized):
                break
            start = max(start + 1, end - self._chunk_overlap)
        return chunks

    @staticmethod
    def _build_conversational_query(question: str, history: list[dict[str, str]] | None) -> str:
        if not history:
            return question

        fragments: list[str] = []
        for turn in history[-4:]:
            content = (turn.get("content") or "").strip()
            if content:
                fragments.append(content)
        if not fragments:
            return question
        return " ".join(fragments + [question])

    @staticmethod
    def _tokenize(text: str) -> list[str]:
        return [token for token in re.findall(r"[A-Za-z0-9_./:-]+", text.lower()) if len(token) > 1]

    @staticmethod
    def _is_greeting_or_smalltalk(question: str) -> bool:
        normalized = question.strip().lower()
        simple = {
            "hi",
            "hello",
            "hey",
            "good morning",
            "good afternoon",
            "good evening",
            "thanks",
            "thank you",
        }
        return normalized in simple or (len(normalized.split()) <= 3 and normalized.startswith(("hi", "hello", "hey")))

    @staticmethod
    def _extract_threshold_from_question(question: str) -> tuple[str, float] | None:
        question_lower = question.lower()
        if not any(metric in question_lower for metric in ("recall", "precision", "accuracy")):
            return None

        symbol_match = re.search(r"(>=|<=|>|<)\s*([0-9]*\.?[0-9]+)", question_lower)
        if symbol_match:
            return symbol_match.group(1), float(symbol_match.group(2))

        phrase_patterns: list[tuple[str, str]] = [
            (r"(?:greater than|more than|above)\s*([0-9]*\.?[0-9]+)", ">"),
            (r"(?:at least|minimum of|no less than)\s*([0-9]*\.?[0-9]+)", ">="),
            (r"(?:less than|below)\s*([0-9]*\.?[0-9]+)", "<"),
            (r"(?:at most|maximum of|no more than)\s*([0-9]*\.?[0-9]+)", "<="),
        ]
        for pattern, operator in phrase_patterns:
            match = re.search(pattern, question_lower)
            if match:
                return operator, float(match.group(1))
        return None

    @staticmethod
    def _normalize_metric_names(question: str) -> set[str]:
        lower = question.lower()
        metrics: set[str] = set()
        if "recall" in lower:
            metrics.add("recall")
        if "precision" in lower or "precsion" in lower:
            metrics.add("precision")
        if "accuracy" in lower:
            metrics.add("accuracy")
        return metrics

    @staticmethod
    def _compare(operator: str, value: float, threshold: float) -> bool:
        if operator == ">":
            return value > threshold
        if operator == ">=":
            return value >= threshold
        if operator == "<":
            return value < threshold
        if operator == "<=":
            return value <= threshold
        return False

    def _answer_metric_filter_query(self, question: str) -> str | None:
        threshold_data = self._extract_threshold_from_question(question)
        if threshold_data is None:
            return None

        requested_metrics = self._normalize_metric_names(question) or {"recall"}
        operator, threshold = threshold_data
        entries: list[tuple[str, float | None, float | None]] = []
        seen_labels: set[str] = set()

        for document in self._documents:
            text = document["text"]
            recall_match = re.search(r"mean_recall\s*=\s*([0-9]*\.?[0-9]+)", text, flags=re.IGNORECASE)
            precision_match = re.search(r"mean_precision\s*=\s*([0-9]*\.?[0-9]+)", text, flags=re.IGNORECASE)
            recall_value = float(recall_match.group(1)) if recall_match else None
            precision_value = float(precision_match.group(1)) if precision_match else None

            if "recall" in requested_metrics and recall_value is None:
                continue
            if "precision" in requested_metrics and precision_value is None:
                continue
            if "accuracy" in requested_metrics and recall_value is None:
                continue
            if "recall" in requested_metrics and not self._compare(operator, recall_value, threshold):
                continue
            if "precision" in requested_metrics and not self._compare(operator, precision_value, threshold):
                continue
            if "accuracy" in requested_metrics and not self._compare(operator, recall_value, threshold):
                continue

            label = Path(document["source_path"]).stem or "unknown_log"
            if label in seen_labels:
                continue
            seen_labels.add(label)
            entries.append((label, recall_value, precision_value))

        if not entries:
            metric_text = " and ".join(sorted(requested_metrics))
            return f"No indexed files matched {metric_text} {operator} {threshold:.4f}."

        lines = [f"Matches for {operator} {threshold:.4f}:"]
        for label, recall_value, precision_value in entries[:50]:
            parts: list[str] = []
            if recall_value is not None:
                parts.append(f"recall={recall_value:.4f}")
            if precision_value is not None:
                parts.append(f"precision={precision_value:.4f}")
            if "accuracy" in requested_metrics and recall_value is not None:
                parts.append(f"accuracy≈{recall_value:.4f}")
            lines.append(f"- {label}: {', '.join(parts)}")
        return "\n".join(lines)

    def _search(self, question: str, history: list[dict[str, str]] | None = None, limit: int | None = None) -> list[dict[str, Any]]:
        final_query = self._build_conversational_query(question, history)
        query_tokens = self._tokenize(final_query)
        if not query_tokens:
            return []

        candidate_documents: list[dict[str, Any]]
        if self._collection is not None:
            requested = max((limit or self._top_k) * 4, self._top_k * 4)
            query_kwargs = dict(n_results=requested, include=['documents', 'metadatas', 'distances'], query_embeddings=[self._embed_text(final_query)])
            result = self._collection.query(**query_kwargs)
            candidate_documents = []
            ids = (result.get('ids') or [[]])[0]
            texts = (result.get('documents') or [[]])[0]
            metadatas = (result.get('metadatas') or [[]])[0]
            distances = (result.get('distances') or [[]])[0]
            for document_id, text, metadata, distance in zip(ids, texts, metadatas, distances):
                if not text or not isinstance(metadata, dict):
                    continue
                candidate_documents.append(
                    {
                        'id': str(document_id),
                        'source_path': str(metadata.get('source_path') or ''),
                        'duplicate_round': int(metadata.get('duplicate_round') or 1),
                        'text': str(text),
                        'metadata': dict(metadata),
                        'distance': float(distance),
                    }
                )
        else:
            candidate_documents = list(self._documents)

        ranked: list[tuple[float, dict[str, Any]]] = []
        for document in candidate_documents:
            text_lower = document["text"].lower()
            path_lower = document["source_path"].lower()
            metadata = document.get('metadata') if isinstance(document.get('metadata'), dict) else {}
            metadata_text = ' '.join(
                str(metadata.get(key) or '')
                for key in ('source_name', 'report_kind', 'document_title', 'section_title', 'scenario_tags')
            ).lower()
            doc_tokens = set(self._tokenize(document["text"]))
            token_hits = sum(1 for token in query_tokens if token in doc_tokens)
            path_hits = sum(1 for token in query_tokens if token in path_lower)
            metadata_hits = sum(1 for token in query_tokens if token in metadata_text)
            phrase_bonus = 6 if question.lower() in text_lower else 0
            distance_bonus = max(0.0, 1.5 - float(document.get('distance', 1.5)))
            score = token_hits * 3 + path_hits * 4 + metadata_hits * 2 + phrase_bonus + distance_bonus
            if score > 0:
                ranked.append((score, document))

        ranked.sort(key=lambda item: (-item[0], item[1]["source_path"]))
        return [document for _, document in ranked[: (limit or self._top_k)]]

    def _answer_log_listing_query(self, question: str, matches: list[dict[str, Any]]) -> str | None:
        lower = question.lower()
        if not any(keyword in lower for keyword in ('log', 'logs', 'scenario', 'truck', 'validation')):
            return None

        grouped: dict[str, dict[str, Any]] = {}
        for document in matches:
            source_path = document.get('source_path') or ''
            if not source_path:
                continue
            metadata = document.get('metadata') if isinstance(document.get('metadata'), dict) else {}
            entry = grouped.setdefault(
                source_path,
                {
                    'source_name': metadata.get('source_name') or Path(source_path).name,
                    'report_kind': metadata.get('report_kind') or 'report',
                    'scenario_tags': metadata.get('scenario_tags') or '',
                    'section_title': metadata.get('section_title') or '',
                },
            )
            if metadata.get('scenario_tags'):
                entry['scenario_tags'] = metadata['scenario_tags']
            if metadata.get('section_title'):
                entry['section_title'] = metadata['section_title']

        if not grouped:
            return None

        lines = ['Related indexed logs:']
        for source_path, metadata in list(grouped.items())[:50]:
            extras: list[str] = []
            if metadata.get('report_kind'):
                extras.append(str(metadata['report_kind']))
            if metadata.get('scenario_tags'):
                extras.append(f"scenarios={metadata['scenario_tags']}")
            if metadata.get('section_title'):
                extras.append(f"section={metadata['section_title']}")
            suffix = f" ({', '.join(extras)})" if extras else ''
            lines.append(f"- {source_path}{suffix}")
        return '\n'.join(lines)

    @staticmethod
    def _split_sentences(text: str) -> list[str]:
        parts = re.split(r"(?<=[.!?])\s+", text)
        return [part.strip() for part in parts if part.strip()]

    def _best_snippet(self, text: str, query: str) -> str:
        query_tokens = set(self._tokenize(query))
        best_sentence = ""
        best_score = -1
        for sentence in self._split_sentences(text):
            sentence_tokens = set(self._tokenize(sentence))
            score = sum(1 for token in query_tokens if token in sentence_tokens)
            if score > best_score:
                best_sentence = sentence
                best_score = score
        if best_sentence:
            return best_sentence[:360]
        return text[:360]

    def _local_model_path(self) -> Optional[Path]:
        for candidate in (self._qwen_gguf_path, self._qwen_fallback_gguf_path):
            if candidate and candidate.exists():
                return candidate
        return None

    def _llama_server_ready(self) -> bool:
        if not self._llama_server_base_url:
            return False

        try:
            with socket.create_connection((self._llama_server_host, self._llama_server_port), timeout=1.5):
                pass
        except OSError:
            return False

        health_url = self._llama_server_base_url + '/health'
        try:
            with urllib.request.urlopen(health_url, timeout=2) as response:
                return 200 <= getattr(response, 'status', 200) < 500
        except urllib.error.HTTPError:
            return True
        except Exception:
            return True

    def _ensure_llama_server(self) -> bool:
        if self._llm_backend != 'llama_server':
            return False
        if self._llama_server_ready():
            return True
        if not self._llama_server_autostart:
            return False

        model_path = self._local_model_path()
        if model_path is None or self._llama_server_path is None or not self._llama_server_path.exists():
            return False

        with self._llama_server_lock:
            if self._llama_server_ready():
                return True

            command = [
                str(self._llama_server_path),
                '-m', str(model_path),
                '--host', self._llama_server_host,
                '--port', str(self._llama_server_port),
                '-c', str(self._llm_context_window),
                '-t', str(self._llm_n_threads),
                '-b', str(self._llm_n_batch),
                '-n', str(self._llm_max_new_tokens),
            ]
            if self._llm_n_gpu_layers > 0:
                command.extend(['-ngl', str(self._llm_n_gpu_layers)])

            creationflags = 0
            popen_kwargs: dict[str, Any] = {
                'stdout': subprocess.DEVNULL,
                'stderr': subprocess.STDOUT,
                'cwd': str(model_path.parent),
            }
            if os.name == 'nt':
                creationflags = subprocess.CREATE_NEW_PROCESS_GROUP | getattr(subprocess, 'CREATE_NO_WINDOW', 0)
                popen_kwargs['creationflags'] = creationflags
            else:
                popen_kwargs['start_new_session'] = True

            self._llama_server_process = subprocess.Popen(command, **popen_kwargs)

            deadline = time.time() + 180
            while time.time() < deadline:
                if self._llama_server_process.poll() is not None:
                    return False
                if self._llama_server_ready():
                    return True
                time.sleep(2)

        return self._llama_server_ready()

    def _answer_from_matches(self, question: str, matches: list[dict[str, Any]]) -> str:
        lines = ["Top matches from the indexed workspace:"]
        seen_sources: set[str] = set()
        for document in matches[:3]:
            source_path = document['source_path']
            if source_path in seen_sources:
                continue
            seen_sources.add(source_path)
            snippet = self._best_snippet(document['text'], question)
            metadata = document.get('metadata') if isinstance(document.get('metadata'), dict) else {}
            header = metadata.get('section_title') or Path(source_path).name
            lines.append(f"- {header}: {snippet}")
        lines.append("")
        lines.append("Sources:")
        for source_path in list(seen_sources)[:5]:
            lines.append(f"- {source_path}")
        return "\n".join(lines)

    def _llama_answer(self, question: str, history: list[dict[str, str]] | None, matches: list[dict[str, Any]]) -> Optional[str]:
        if not matches or not self._ensure_llama_server():
            return None

        context_sections: list[str] = []
        for document in matches[:3]:
            snippet = document['text'].strip()[:1200]
            context_sections.append(
                f"Source: {Path(document['source_path']).name}\nPath: {document['source_path']}\nContext: {snippet}"
            )

        messages: list[dict[str, str]] = [
            {
                'role': 'system',
                'content': (
                    'You answer questions about this workspace using only the supplied context. '
                    'If the context is insufficient, say that directly. Keep answers concise and concrete.'
                ),
            }
        ]
        for turn in (history or [])[-4:]:
            role = (turn.get('role') or '').strip().lower()
            content = (turn.get('content') or '').strip()
            if role in {'user', 'assistant', 'system'} and content:
                messages.append({'role': role, 'content': content})
        messages.append(
            {
                'role': 'user',
                'content': (
                    'Use the following workspace context to answer the question.\n\n'
                    + '\n\n'.join(context_sections)
                    + f'\n\nQuestion: {question}'
                ),
            }
        )

        model_path = self._local_model_path()
        payload = {
            'model': self._qwen_model_id or (model_path.stem if model_path is not None else 'local-qwen'),
            'messages': messages,
            'temperature': 0.1,
            'top_p': 0.9,
            'max_tokens': self._llm_max_new_tokens,
            'stream': False,
        }
        request = urllib.request.Request(
            self._llama_server_base_url + '/v1/chat/completions',
            data=json.dumps(payload).encode('utf-8'),
            headers={'Content-Type': 'application/json'},
            method='POST',
        )

        try:
            with urllib.request.urlopen(request, timeout=240) as response:
                body = json.loads(response.read().decode('utf-8'))
        except Exception:
            return None

        choices = body.get('choices') or []
        if not choices:
            return None

        message = choices[0].get('message') or {}
        content = message.get('content')
        if isinstance(content, list):
            content = ' '.join(str(part.get('text') or '') for part in content if isinstance(part, dict)).strip()
        elif content is not None:
            content = str(content).strip()
        return content or None

    def answer(self, question: str, history: list[dict[str, str]] | None = None) -> str:
        if self._is_greeting_or_smalltalk(question):
            return "Ask about KPI inputs, runtime wiring, HTML reports, or recent files in the indexed workspace."

        metric_answer = self._answer_metric_filter_query(question)
        if metric_answer:
            return metric_answer

        if not self._documents:
            return "No files are indexed yet. Start the service with a valid HTML_ROOT_PATH or call POST /ingest first."

        matches = self._search(question, history=history)
        if not matches:
            return "I could not find relevant indexed content for that question. Try a file name, tool name, KPI mode, or a more specific phrase."

        log_listing_answer = self._answer_log_listing_query(question, matches)
        if log_listing_answer:
            return log_listing_answer

        llm_answer = self._llama_answer(question, history=history, matches=matches)
        if llm_answer:
            return llm_answer

        return self._answer_from_matches(question, matches)
