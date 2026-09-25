from __future__ import annotations

from pathlib import Path
from uuid import uuid4

from flask import Flask, jsonify, render_template, request

from app.graph_flow import QueryGraph
from app.ingestor import HtmlIngestor
from app.sqlite_store import SQLiteLogStore


class RagApi:
    def __init__(
        self,
        ingestor: HtmlIngestor,
        query_graph: QueryGraph,
        sqlite_store: SQLiteLogStore,
    ) -> None:
        self.ingestor = ingestor
        self.query_graph = query_graph
        self.sqlite_store = sqlite_store
        self.app = Flask(__name__)
        self._session_messages: dict[str, list[dict[str, str]]] = {}
        self._register_routes()

    def _register_routes(self) -> None:
        @self.app.get("/health")
        def health() -> tuple:
            return jsonify({"status": "ok", "db": self.sqlite_store.stats()}), 200

        @self.app.get("/")
        def index() -> str:
            return render_template("index.html")

        @self.app.post("/ingest")
        def ingest() -> tuple:
            payload = request.get_json(silent=True) or {}
            html_root = (payload.get("html_root", "") or "").strip()
            if html_root:
                self.ingestor.html_root = Path(html_root)
            if payload.get("reset_index"):
                self.query_graph.rag_engine.reset_vector_store()
                self.sqlite_store.clear_ingestions()
                self.sqlite_store.clear_queries()
            result = self.ingestor.ingest(run_vector=False)
            return jsonify(result), 200

        @self.app.post("/ask")
        def ask() -> tuple:
            payload = request.get_json(silent=True) or {}
            question = payload.get("question", "").strip()
            if not question:
                return jsonify({"error": "question is required"}), 400

            session_id = (payload.get("session_id", "") or "").strip() or str(uuid4())
            session_messages = self._session_messages.setdefault(session_id, [])
            try:
                answer = self.query_graph.run(question, history=session_messages)
            except Exception as exc:
                return jsonify({"error": str(exc)}), 500

            session_messages.append({"role": "user", "content": question})
            session_messages.append({"role": "assistant", "content": answer})
            if len(session_messages) > 20:
                self._session_messages[session_id] = session_messages[-20:]

            return jsonify({"session_id": session_id, "question": question, "answer": answer}), 200
