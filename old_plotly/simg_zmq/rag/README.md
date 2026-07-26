# HTML RAG Project (Flask + SQLite + ChromaDB + LangGraph + LlamaIndex + Qwen)

## Setup
1. Configure Python environment.
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Download embedding model once (on any machine with internet), then run offline:
   ```bash
   python download_local_models.py
   ```
   This downloads `nomic-ai/nomic-embed-text-v1.5` and `nomic-ai/nomic-bert-2048` for local/offline embeddings.
   Optional faster/lighter model:
   ```bash
   python download_local_models.py --embedding bge-small
   ```
3. Copy env:
   ```bash
   copy .env.example .env
   ```
4. Install latest llama.cpp runtime binaries (required for Qwen3.5 GGUF on this setup):
   ```powershell
   New-Item -ItemType Directory -Force -Path tools\llama.cpp | Out-Null
   Invoke-WebRequest -Uri "https://github.com/ggml-org/llama.cpp/releases/download/b8192/llama-b8192-bin-win-cpu-x64.zip" -OutFile "tools\llama.cpp\llama-b8192-bin-win-cpu-x64.zip"
   Expand-Archive -Path "tools\llama.cpp\llama-b8192-bin-win-cpu-x64.zip" -DestinationPath "tools\llama.cpp" -Force
   ```
5. Optional iGPU (Intel/AMD) setup with Vulkan backend:
   ```powershell
   New-Item -ItemType Directory -Force -Path tools\llama.cpp-vulkan | Out-Null
   Invoke-WebRequest -Uri "https://github.com/ggml-org/llama.cpp/releases/download/b8192/llama-b8192-bin-win-vulkan-x64.zip" -OutFile "tools\llama.cpp-vulkan\llama-b8192-bin-win-vulkan-x64.zip"
   Expand-Archive -Path "tools\llama.cpp-vulkan\llama-b8192-bin-win-vulkan-x64.zip" -DestinationPath "tools\llama.cpp-vulkan" -Force
   .\tools\llama.cpp-vulkan\llama-server.exe --list-devices
   ```

6. Optional larger-context TurboQuant+ server (external custom llama.cpp fork):
   ```powershell
   git clone https://github.com/TheTom/llama-cpp-turboquant.git tools\llama.cpp-turboquant
   Set-Location tools\llama.cpp-turboquant
   git checkout feature/turboquant-kv-cache
   cmake -S . -B build -DGGML_VULKAN=ON -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release -j
   .\build\bin\llama-server.exe -m C:\models\Qwen_Qwen3.5-2B-Q5_K_S.gguf --host 127.0.0.1 --port 8081 -c 32768 -t 20 -b 1024 -ngl 24 -fa on --cache-type-k q8_0 --cache-type-v turbo4
   ```
   Use this with `LLAMA_SERVER_AUTOSTART=false` and `LLAMA_SERVER_BASE_URL=http://127.0.0.1:8081` in `.env` so the Flask app talks to the external TurboQuant server instead of spawning the stock runtime.

## CLI Usage
You can run the project in two modes: scraping mode and talk (Flask) mode.

- Scrape a path and ingest HTML files:

```bash
python main.py --scrap "C:\\path\\to\\html_root"
```

- Reset old index/logs and re-ingest from a new folder (recommended when switching datasets):

```bash
python main.py --scrap "C:\\path\\to\\html_root" --embed --reset-index
```

- Start the Flask talk server (RAG + LLM endpoints):

```bash
python main.py --talk
```

- Open chat UI in browser:

```text
http://127.0.0.1:5000/
```

- Run both: scrape then start server

```bash
python main.py --scrap "C:\\path\\to\\html_root" --talk

Notes:
- This project is configured for local-only models. No external model API is used at runtime.
- `--embed` performs full vector ingestion using local embedding model path in `EMBED_MODEL_PATH`.
- By default, ingestion is keyword-only (`HTML_FILE_KEYWORD`) and does not fall back to all HTML files.
- To include all HTML files when no keyword matches, set `ALLOW_ALL_HTML_FALLBACK=true`.

```bash
python main.py --scrap "C:\\path\\to\\html_root" --embed
```
```

## Local model config
- Embeddings (default): set `EMBED_MODEL_PATH` to local `model/nomic-embed-text-v1.5`.
- Vector store backend (default): `VECTOR_BACKEND=simple` (pure Python local file store, safe on Python 3.13).
- Vector store path for simple backend: `VECTOR_STORE_JSON_PATH=data/vector_store/vector_store.json`.
- Optional Chroma backend: set `VECTOR_BACKEND=chroma` (recommended only on Python versions where Chroma HNSW runtime is stable in your environment).
- Parsing language: set `PARSE_ENGLISH_ONLY=true` for English-only extracted text.
- Ingestion filters: set `HTML_FILE_KEYWORD=sil_narrative,sil_nartive,_sil_narrative,_sil_nartive` and keep `ALLOW_ALL_HTML_FALLBACK=false` for fast targeted runs.
- Exclusions: set `HTML_EXCLUDE_FILE_KEYWORD=dummy` to avoid test/dummy narrative files polluting retrieval.
- LLM primary: `QWEN_GGUF_PATH` (default `Qwen_Qwen3.5-2B-Q5_K_S.gguf`).
- LLM fallback: `QWEN_FALLBACK_GGUF_PATH` (set to another Qwen3.5 GGUF if available).
- LLM backend (default): `LLM_BACKEND=llama_server` for Qwen3.5 GGUF compatibility.
- llama-server binary path: `LLAMA_SERVER_PATH=tools/llama.cpp-vulkan/llama-server.exe` (falls back to CPU if Vulkan path is unavailable).
- llama-server endpoint: `LLAMA_SERVER_BASE_URL=http://127.0.0.1:8081` (`LLAMA_SERVER_AUTOSTART=true` auto-starts it from app).
- For larger context with TurboQuant+, run the external forked `llama-server` yourself and set `LLAMA_SERVER_AUTOSTART=false`, `LLAMA_SERVER_BASE_URL=http://127.0.0.1:8081`, `LLM_CONTEXT_WINDOW=32768`, and `LLM_N_BATCH=1024`.
- Recommended TurboQuant+ KV cache profile for low-bit GGUF models: `--cache-type-k q8_0 --cache-type-v turbo4`.
- If you see `unknown model architecture: 'qwen35'`, current `llama.cpp` runtime does not support this model format in your Python setup.
- Optional speed knobs in `.env`:
   - `LLM_MAX_NEW_TOKENS=640` (increase if long answers still truncate)
   - `LLM_CONTEXT_WINDOW=4096`
   - `LLM_SIMILARITY_TOP_K=6`
   - `LLM_N_THREADS=<cpu_cores>`
   - `LLM_N_BATCH=512` (reduce to `256` if memory pressure)
   - `LLM_N_GPU_LAYERS=0` for CPU mode; use `-1` only when GPU-offload runtime support is available
   - `CHUNK_SIZE=2000` (higher = fewer chunks, faster embed ingestion)
   - `CHUNK_OVERLAP=80`

Example `.env` overrides for the external TurboQuant+ server:
```env
LLM_BACKEND=llama_server
LLAMA_SERVER_PATH=tools/llama.cpp-turboquant/build/bin/llama-server.exe
LLAMA_SERVER_BASE_URL=http://127.0.0.1:8081
LLAMA_SERVER_AUTOSTART=false
LLM_CONTEXT_WINDOW=32768
LLM_N_BATCH=1024
LLM_N_GPU_LAYERS=24
```

## Endpoints
- `GET /health`
- `GET /` (chat website with session memory)
- `POST /ingest`
- `POST /ask`

Example ask:
```bash
curl -X POST http://127.0.0.1:5000/ask -H "Content-Type: application/json" -d "{\"question\":\"What logs are available?\"}"
```

## Notes
- Default HTML path:
  `C:\Users\ouymc2\Desktop\plotly_code\main_html\all_services\tools\intplot_kpi`
- Default file filter keyword: `sil_narrative`
- If same file content is already ingested, system inserts again with 2 duplicate rounds.
- If ingestion output says no matching files found, your index is unchanged and answers can come from older data.
- If context size is too large for runtime, the app automatically retries with reduced retrieved context instead of failing.
- For queries like `recall/precision/accuracy > threshold`, the app uses deterministic metric extraction from ingested narratives to avoid LLM numeric mistakes.
