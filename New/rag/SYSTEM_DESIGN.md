# System Design

## Goals
- Scrape HTML files from a fixed path recursively.
- Ingest only files containing `sil_narrative` in file name (fallback to all `.html` if none found).
- Parse HTML text cleanly.
- Store ingestion/query logs in SQLite.
- Store embeddings/chunks in ChromaDB.
- Build RAG QA using LlamaIndex and Qwen safetensors model.
- Orchestrate query flow with LangGraph.
- Expose APIs using Flask.

## Components (One Class per File)
- `AppConfig` in `app/config.py`: environment and path configuration.
- `HtmlParser` in `app/html_parser.py`: HTML cleanup and text extraction.
- `SQLiteLogStore` in `app/sqlite_store.py`: ingestion and query logs.
- `RagEngine` in `app/rag_engine.py`: Chroma + LlamaIndex index/query.
- `HtmlIngestor` in `app/ingestor.py`: file discovery and ingestion policy.
- `QueryGraph` in `app/graph_flow.py`: LangGraph flow for QA.
- `RagApi` in `app/api.py`: Flask endpoints.
- `Application` in `app/main.py`: dependency wiring.

## Data Flow
1. `POST /ingest` calls `HtmlIngestor.ingest()`.
2. HTML files are parsed by `HtmlParser`.
3. Text hash is checked in SQLite by `SQLiteLogStore`.
4. If already present, text is inserted **2 times** to Chroma (`duplicate_round=1..2`).
5. `POST /ask` runs LangGraph node `answer_question`.
6. Node calls `RagEngine.answer()` with LlamaIndex query engine.
7. Answer and query are logged to SQLite.

## Storage
- SQLite file: `data/rag_logs.db`
- Chroma persistence: `data/chroma/`
- Chroma collection: `html_narrative_docs` (configurable)

## API
- `GET /health`
- `POST /ingest`
- `POST /ask` with JSON `{ "question": "..." }`
