# HPCC Runtime Integration Guide

## Purpose

This document describes the current integrated HPCC runtime architecture for the `simg_zmq` repository. It explains how the UI, runtime broker, RAG service, KPI containers, and Hyperlink viewer work together as one operational system.

This is the primary document to use when you need to:

- understand what was changed
- build and launch the stack
- troubleshoot runtime behavior
- onboard a new developer or operator
- extend the runtime registry with new tools

## Executive Summary

The integrated runtime is built around one principle: `main_html` is the single front door.

Instead of launching separate terminals and isolated tools, the stack now behaves like this:

1. `simg_sh_hpcc/main_hpcc.sh` starts the broker in `--broker-only` mode on the login node.
2. The same launcher starts only `main_html.simg` on the login node and injects offline local-model settings.
3. KPI and other heavy runtime requests are forwarded from `main_html` to `hpcc_main.py` over a local JSON-over-socket protocol.
4. `hpcc_main.py` records the request, writes a per-run `slurm_tmux_launcher.sh`, and delegates compute work through `srun` plus `tmux` on worker nodes.
5. RAG is an on-demand service; when launched it publishes its compute-node host back into the runtime registry instead of assuming localhost.
6. Hyperlink is integrated directly into `main_html`, reuses the same session context, and keeps AI video description offline-only.

This makes the stack easier to operate, easier to document, and easier to extend.

## What Changed

### Before

- The repository contained multiple partially separate runtimes.
- Users had to know which tool to launch manually.
- The runtime image mapping was implicit.
- RAG was not integrated as the dashboard chat backend.
- Cross-service orchestration was not centralized.

### Now

- `main_html` is the single user-facing application.
- KPI and Hyperlink are the main exposed user flows.
- `hpcc_main.py` acts as the broker and runtime supervisor.
- Runtime tool metadata is persisted in SQLite through `main_html/runtime_store.py`.
- RAG is integrated through `main_html/rag_client.py`.
- Hyperlink remains in-process and does not require a separate shell.
- The runtime map is editable from the UI.
- The launcher handles Windows-to-WSL localhost forwarding explicitly for stable access to ports `5001`, `5100`, and `9100`.

## High-Level Runtime Model

### User-Facing Layer

- `main_html.simg`
  - login
  - dashboard
  - KPI submission form
  - Hyperlink entrypoint
  - job history
  - runtime map editor
  - RAG chat widget

### Control Layer

- `hpcc_main.py`
  - runtime broker
  - command builder
  - process supervisor
  - runtime log writer
  - Windows localhost forwarder for WSL-hosted services

### Execution Layer

- `kpi/can/can_kpi.simg`
- `kpi/udp/udp_kpi.simg`
- `kpi/int_plot/intplot_kpi.simg`
- `rag/rag.simg`

### Persistence Layer

- `main_html` SQLite database
  - auth tables
  - job history
  - runtime tool registry
  - runtime jobs
  - runtime events
- `rag` SQLite database and vector store
  - ingestion/query logs
  - vector or lexical store artifacts

## Runtime Components

### main_html

Primary entrypoint for human users.

Key responsibilities:

- authenticates users
- renders dashboard and tool pages
- accepts KPI form submissions
- stores Hyperlink session roots
- calls the broker for runtime operations
- calls the RAG service for dashboard chat
- synchronizes UI-facing job history with broker status

Important files:

- `main_html/app.py`
- `main_html/runtime_store.py`
- `main_html/hpcc_broker_client.py`
- `main_html/rag_client.py`
- `main_html/templates/dashboard.html`
- `main_html/templates/tools/kpi.html`
- `main_html/templates/tools/hyperlink_tool.html`
- `main_html/templates/runtime_map.html`

### hpcc_main.py

The orchestration core of the integrated platform.

Key responsibilities:

- starts the socket broker
- resolves image paths
- constructs runtime commands
- starts `main_html` and `rag` services when needed
- launches KPI and Interactive Plot jobs
- tracks process lifecycle and status transitions
- writes broker-side execution logs
- exposes runtime control on `9100`
- adds Windows localhost forwarders when broker/services run inside WSL

### Runtime Store

`main_html/runtime_store.py` is the editable runtime registry and runtime event log.

It persists:

- `runtime_tools`
- `runtime_jobs`
- `runtime_events`

Default registered tools:

- `main_html`
- `can_kpi`
- `udp_kpi`
- `interactive_plot`
- `hyperlink`
- `rag`

### RAG Service

`rag/rag.simg` is a standalone HTTP service used by the dashboard chat.

Key responsibilities:

- exposes `/health`
- exposes `/ask`
- optionally ingests HTML roots
- stores internal logs and vector or lexical artifacts

### Hyperlink

Hyperlink is intentionally not treated as a separate containerized service.

Instead, `main_html`:

- stores HTML/video roots in session and history
- serves the integrated viewer at `/hyperlink/`
- exposes helper endpoints like `/hyperlink/api/mappings`
- dynamically loads `Hyperlink_tool/code/html_online/main.py`

This keeps the browsing workflow inside the same authenticated UI.

## Deployment Topology

### Windows Development / Operator Mode

The validated local launch path is:

```powershell
python .\hpcc_main.py
```

In that mode the launcher can:

1. start the broker inside WSL using `--broker-only`
2. launch containerized `main_html` and `rag` through WSL `apptainer run`
3. bind Windows localhost forwarders for `5001`, `5100`, and `9100`

This design avoids relying on WSL auto-port-forwarding behavior, which can be inconsistent.

### HPCC / Cluster-Oriented Mode

The cluster packaging model uses:

- `scripts/wsl_build_hpcc_bundle.sh` to build images in WSL
- `simg_sh_hpcc/` as the bundle artifact directory
- `simg_sh_hpcc/run_hpcc_stack.sh` as the cluster-side launcher wrapper

Only `main_html` and Hyperlink should remain active on the login node. KPI and Interactive Plot work is expected to go through Slurm from the broker or through the tmux-backed direct wrappers in `simg_sh_hpcc/kpi/`.

### Runtime State Locations

| Path | Purpose |
|------|---------|
| `simg_sh_hpcc/main_html.simg` | Main UI container |
| `simg_sh_hpcc/kpi/can/can_kpi.simg` | CAN KPI image |
| `simg_sh_hpcc/kpi/udp/udp_kpi.simg` | UDP KPI image |
| `simg_sh_hpcc/kpi/int_plot/intplot_kpi.simg` | Interactive Plot image |
| `simg_sh_hpcc/rag/rag.simg` | RAG service container |
| `simg_sh_hpcc/kpi/inplot_udp.sh` | tmux wrapper for UDP KPI + Interactive Plot |
| `simg_sh_hpcc/kpi/inplot_can.sh` | tmux wrapper for CAN KPI + Interactive Plot |
| `simg_sh_hpcc/runtime_state/main_html/cache_html/` | `main_html` runtime cache and SQLite DB |
| `simg_sh_hpcc/runtime_state/rag/` | RAG runtime state |
| `simg_sh_hpcc/runs/` | per-run logs and output roots |

## Build Outputs

Run:

```bash
bash scripts/wsl_build_hpcc_bundle.sh
```

Expected outputs:

- `main_html.simg`
- `kpi/can/can_kpi.simg`
- `kpi/udp/udp_kpi.simg`
- `kpi/int_plot/intplot_kpi.simg`
- `rag/rag.simg`
- `main_hpcc.sh`, `kpi_runtime_launcher.sh`, and the direct wrapper scripts under `kpi/` and `rag/`

Main definition files:

- `Singularity.def`
- `KPI/can_kpi/can_singularity_KPI.def`
- `KPI/UDP_KPI/Singularity_KPI.def`
- `KPI/intplot_kpi/singularity_interactiveplot.def`
- `rag/Singularity_RAG.def`

## Runtime Ports

| Port | Protocol | Owner | Description |
|------|----------|-------|-------------|
| `5001` | HTTP | `main_html` | main browser UI on the login node |
| `5100` | HTTP | `rag` | RAG service when an on-demand compute worker is running |
| `9100` | TCP socket | `hpcc_main.py` | broker API on the login node |

## Core User Flows

### 1. Login And Dashboard

Flow:

1. User opens `/login`.
2. User authenticates.
3. Dashboard loads recent jobs, active chat session, runtime tool graph, and broker health.
4. Dashboard exposes links to KPI, Hyperlink, and Runtime Map.

### 2. KPI Submission Flow

Flow:

1. User opens `/html/kpi`.
2. User chooses `can_kpi`, `udp_kpi`, or `interactive_plot`.
3. User provides JSON-mode or HDF-mode paths.
4. `submit_runtime_kpi_job()` validates mode-specific fields.
5. `_submit_runtime_job()` sends payload to broker.
6. Broker records a `runtime_jobs` row and starts the selected image.
7. `JobHistory` stores the UI-facing record linked to `runtime_job_id`.
8. Dashboard and history pages synchronize status by asking the broker for the current job state.

### 3. Chat Flow

Flow:

1. User submits a question in dashboard chat.
2. `main_html` calls `rag_client`.
3. `rag_client` calls the bundled `rag/rag.simg` service over HTTP.
4. RAG returns answer plus source references.
5. The answer is shown in the dashboard and persisted in chat history.

### 4. Hyperlink Flow

Flow:

1. User opens `/html/hyperlink_tool`.
2. User sets HTML root, optional video root, and optional cache root.
3. `main_html` stores these values in session and history.
4. `/hyperlink/` serves the integrated Log Viewer.
5. `/hyperlink/api/mappings` resolves HTML/video mappings from `LogViewerApp`.

### 5. Runtime Map Flow

Flow:

1. User opens `/html/runtime-map`.
2. `runtime_store.graph_payload()` provides nodes and edges.
3. The page renders the graph via ReactFlow.
4. The selected node populates an editable mapping form.
5. `/api/runtime/tools` persists updates.
6. `/api/runtime/launch` can queue selected runtime nodes from the UI.

## Inputs And Outputs By Runtime Tool

### CAN KPI

- Inputs: JSON batch or input/output HDF pair
- Outputs: HTML KPI report directory

### UDP KPI

- Inputs: JSON batch, HDF pair, or ZMQ bridge mode
- Outputs: HTML KPI report directory plus broker log

### Interactive Plot

- Inputs: config XML + inputs JSON, or HDF pair
- Outputs: interactive HTML report directory

### Hyperlink

- Inputs: HTML report root and optional video directory
- Outputs: integrated viewer session and cached paths

### RAG

- Inputs: HTML root for ingestion and user questions over HTTP
- Outputs: answer payloads, ingestion/query logs, and vector/lexical store data

## Persistence Model

### UI And Runtime Metadata

Stored through the `main_html` SQLite database.

Key logical areas:

- users and authentication
- chat sessions and messages
- job history
- runtime tool registry
- runtime jobs
- runtime events

### RAG State

Stored under `simg_sh_hpcc/runtime_state/rag/`.

Typical artifacts:

- `rag_logs.db`
- `vector_store/`
- `vector_store.json`

## Operational Commands

### Build

```bash
bash scripts/wsl_build_hpcc_bundle.sh
```

### Launch

```powershell
python .\hpcc_main.py
```

### Health Checks

```powershell
Invoke-WebRequest -UseBasicParsing http://127.0.0.1:5001/login
Invoke-WebRequest -UseBasicParsing http://127.0.0.1:5100/health
```

### Broker Ping

```powershell
python -c "import socket,json; s=socket.create_connection(('127.0.0.1',9100),5); s.sendall((json.dumps({'action':'ping'})+'\\n').encode()); print(s.recv(65536).decode()); s.close()"
```

## Validation Status

The integrated runtime has been validated for:

- `main_html` on `127.0.0.1:5001`
- RAG on `127.0.0.1:5100`
- broker on `127.0.0.1:9100`
- runtime map rendering
- dashboard chat
- KPI broker submission and completed status rendering
- Hyperlink viewer route

## Known Constraints

- The runtime map uses CDN ESM assets; an air-gapped cluster browser will need local vendoring.
- The launcher currently assumes WSL + Apptainer for the Windows-hosted build/run path.
- `hpcc_main.py` supports local shell and optional `srun` prefixes, but cluster-specific Slurm policies may still require tuning.
- Hyperlink depends on the structure expected by `LogViewerApp`, so arbitrary directories may not produce a meaningful mapping unless they follow the viewer's naming conventions.

## How To Extend The Stack

To add a new runtime-managed tool:

1. add or update the tool entry in `main_html/runtime_store.py`
2. teach `hpcc_main.py` how to build the runtime command for that tool
3. add any needed UI route or launch form in `main_html/app.py`
4. decide whether the tool is user-facing, service-only, or admin-only
5. update `hpcc_integration.drawio` and this document

## Related Documents

- `README.md`
- `HPCC_RUNTIME_SYSTEM_DESIGN.md`
- `hpcc_integration.drawio`