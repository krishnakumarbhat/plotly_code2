# HPCC Runtime System Design

## Scope

This document captures the detailed system design for the current integrated HPCC runtime in `simg_zmq`.

It focuses on the platform made of:

- `main_html`
- `hpcc_main.py`
- `rag`
- KPI execution images
- integrated Hyperlink routes
- runtime registry and runtime logs

## Design Goals

### Primary Goals

1. Make `main_html` the single entrypoint.
2. Route KPI execution through one brokered control plane.
3. Keep chat decoupled behind an HTTP RAG service.
4. Keep Hyperlink embedded instead of opening a second UI shell.
5. Persist runtime configuration and job state in SQLite.
6. Make the runtime inspectable and editable from the UI.
7. Support HPCC login-node operation as the primary deployment path while still documenting the Windows + WSL development flow.

### Non-Goals

- Full multi-node distributed scheduling inside the broker itself
- Rich cluster policy management inside `hpcc_main.py`
- Replacing Slurm as the compute scheduler
- Making Hyperlink a separate standalone service

## System Context

### Actors

- End user using the browser
- Operator launching `python .\hpcc_main.py`
- Broker process controlling container/job lifecycle
- RAG service answering dashboard chat
- KPI containers executing compute-heavy jobs

### External Dependencies

- WSL
- Apptainer or Singularity
- local filesystem and bind mounts
- optional Slurm / `srun`
- browser access to ReactFlow CDN assets

## High-Level Design

The platform has three logical planes.

### 1. Experience Plane

Implemented by `main_html`.

Responsibilities:

- authentication
- navigation
- forms and validation
- status rendering
- runtime map editing
- integrated Hyperlink viewer routes
- RAG chat widget

### 2. Control Plane

Implemented by `hpcc_main.py` and `main_html/runtime_store.py`.

Responsibilities:

- runtime tool registry
- runtime job creation and status transitions
- execution log paths
- process lifecycle tracking
- service startup and supervision
- cross-boundary port forwarding on Windows + WSL

### 3. Execution Plane

Implemented by the tool images and the RAG service.

Responsibilities:

- compute-heavy KPI execution
- interactive plot generation
- RAG answering and ingestion
- Hyperlink asset and mapping resolution

## Low-Level Design

### main_html/app.py

Central request router.

Key roles:

- defines Flask routes
- initializes SQLite schema for containerized runs
- binds runtime store, broker client, and RAG client
- performs request validation and UI-to-broker payload translation
- synchronizes `JobHistory` with broker truth
- integrates Hyperlink viewer and mapping endpoints

### main_html/runtime_store.py

Owns the runtime registry and runtime event model.

Key roles:

- seeds default tool metadata
- stores image paths and service URLs
- stores runtime job lifecycle information
- derives `PENDING`, `RUNNING`, and final states for Slurm-backed jobs from `launcher.log`, `runtime_console.log`, and exit markers
- provides graph payloads to runtime map UI

### main_html/hpcc_broker_client.py

Minimal transport adapter.

Key roles:

- send socket requests
- normalize ping/submit/status/cancel interactions
- keep broker protocol separate from Flask route logic

### main_html/rag_client.py

Minimal HTTP adapter.

Key roles:

- talk to `/ask`
- hide transport details from the dashboard route layer

### hpcc_main.py

The main orchestrator.

Key roles:

- resolve images
- generate runtime commands
- maintain broker socket server
- write per-run `slurm_tmux_launcher.sh` wrappers
- maintain subprocess watchers
- mark Slurm-backed jobs `PENDING` until compute-host tmux panes emit `START`
- update runtime status on completion
- start services when not in `--broker-only` mode
- start Windows localhost forwarders for WSL-hosted ports

### rag/app/*

Service implementation for dashboard chat.

Key roles:

- configuration
- ingestion policy
- answer generation
- persistence of ingestion/query state

## Persistence And Data Ownership

### UI / Control DB

The `main_html` side owns the control-plane metadata.

Logical entities:

- `User`
- `ChatSession`
- `ChatMessage`
- `JobHistory`
- `runtime_tools`
- `runtime_jobs`
- `runtime_events`

### Why Two Job Models Exist

There are two related but distinct job records:

- `runtime_jobs`
  - broker-side source of truth for execution lifecycle
- `JobHistory`
  - UI-facing record used for dashboard/history rendering

The UI synchronizes the visible `JobHistory` status using broker data so the user sees the actual runtime state.

### RAG Data Ownership

The RAG service owns:

- ingest/query logs
- vector or lexical artifacts
- RAG-specific SQLite state

This keeps search/index concerns decoupled from the main UI DB.

## Sequence Flows

### Startup Flow

1. Operator runs `python .\hpcc_main.py`.
2. Launcher checks whether WSL mode is active.
3. Broker may be started inside WSL in `--broker-only` mode.
4. Windows localhost forwarders bind ports `5001`, `5100`, and `9100` to the WSL service host.
5. `main_html` and `rag` are launched.
6. Browser reaches the services through stable `127.0.0.1` ports.

### KPI Submission Flow

1. User fills the KPI form.
2. `submit_runtime_kpi_job()` validates mode-specific fields.
3. `_submit_runtime_job()` sends payload to broker.
4. Broker stores `runtime_jobs` row and writes `slurm_tmux_launcher.sh` plus run-log paths.
5. Slurm-backed jobs stay `PENDING` until a compute-host tmux pane writes `[MAIN]`, `[UDP]`, or `[INTERACTIVE] START`.
6. Once compute starts, the broker/UI surface the run as `RUNNING`.
7. Exit markers and broker return codes drive `COMPLETED` or `FAILED`.
8. UI stores `JobHistory` with `runtime_job_id` and refreshes against broker-backed status.

### Chat Flow

1. User enters a dashboard question.
2. UI route calls `rag_client`.
3. `rag_client` sends HTTP request to `rag`.
4. `rag` returns answer and source information.
5. UI stores and renders conversation.

### Hyperlink Flow

1. User enters HTML/video roots.
2. UI stores roots in session and history.
3. Viewer is served from `/hyperlink/`.
4. Mapping endpoint loads `LogViewerApp` dynamically.
5. Viewer requests HTML/video/comment assets through integrated routes.

## UML Responsibility View

The drawio `UML` page is the visual source for the class and responsibility view. The main conceptual relationships are:

- `app.py` composes `RuntimeStore`, `HpccBrokerClient`, and `RagClient`
- `hpcc_main.py` composes `RuntimeBroker`
- `RuntimeBroker` depends on `RuntimeStore`
- Hyperlink integration dynamically loads `LogViewerApp`

Responsibility summary:

| Type | Primary Responsibility |
|------|------------------------|
| `RuntimeStore` | runtime configuration and broker-side state persistence |
| `HpccBrokerClient` | broker transport adapter |
| `RagClient` | RAG HTTP adapter |
| `RuntimeBroker` | command generation, launch, status transitions |
| `PortForwardServer` | stable Windows localhost exposure |
| `LogViewerApp` | Hyperlink mapping resolution |

## Deployment Decisions

### Why `main_html` Is The Front Door

The UI is the only place where users should need to think in terms of tasks, not services. Consolidating entry through `main_html` reduces operational complexity and user confusion.

### Why A Socket Broker Was Chosen

The broker protocol is intentionally small and local:

- easy to inspect
- easy to debug
- enough for launch/status/cancel
- no heavy service dependency required

### Why RAG Stays Separate

RAG has a different operational profile than the Flask UI. Decoupling it keeps model/index concerns independent from the web application lifecycle.

### Why Hyperlink Stays In-Process

Hyperlink is primarily a browsing and asset-mapping workflow. Keeping it inside `main_html` avoids another terminal, another container, and another auth/session boundary.

### Why Windows Localhost Forwarders Were Added

WSL auto-forwarding is not stable enough to be the only exposure mechanism for the runtime stack. Explicit forwarders inside `hpcc_main.py` make the host entrypoint deterministic.

## Failure Modes And Recovery

### Broker Down

Symptoms:

- dashboard broker status shows offline
- KPI submissions fail
- runtime launch actions fail

Recovery:

- restart `python .\hpcc_main.py`
- verify `127.0.0.1:9100`

### RAG Down

Symptoms:

- chat errors or empty responses
- `/health` fails

Recovery:

- verify `127.0.0.1:5100/health`
- inspect launcher log and RAG runtime state

### Runtime Map Loads But Graph Does Not Render

Symptoms:

- editor is visible but graph pane is empty or broken

Most likely causes:

- React/ReactDOM/CDN version mismatch
- browser cannot reach CDN ESM assets

### KPI Job Stuck In `QUEUED`

Symptoms:

- dashboard/history does not reflect actual broker state

Recovery path:

- confirm broker `status` response for `runtime_job_id`
- confirm `JobHistory` carries the correct runtime link
- confirm status synchronization is using broker truth

## Security And Data Handling Notes

- UI access is authenticated.
- Runtime launch commands are built server-side.
- File paths still require operator discipline because the system is intended for controlled internal environments.
- Hyperlink and KPI flows expose filesystem-backed workflows; deployment should stay inside trusted environments.

## Extension Guidance

If you add a new compute tool, make the change in this order:

1. add the runtime metadata seed in `runtime_store.py`
2. add execution command generation in `hpcc_main.py`
3. add UI or API entrypoints in `main_html/app.py`
4. decide whether the tool is visible from the dashboard or admin/runtime-map only
5. update `hpcc_integration.drawio`
6. update `README_hpcc_integration.md`

## Diagram Reference

Use `hpcc_integration.drawio` as the visual companion for this document.

Page mapping:

- `System Design` -> context and deployment boundaries
- `HLD` -> service and runtime topology
- `LLD` -> module and interface view
- `UML` -> class and responsibility view
- `Project Architecture` -> repository structure view
- `Runtime Flow` -> startup, job, and chat sequence flow