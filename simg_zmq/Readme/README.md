# simg_zmq Documentation Hub

This folder now documents the current HPCC runtime stack that uses `main_html` as the single user-facing entrypoint, `hpcc_main.py` as the runtime broker/supervisor, containerized KPI and RAG services, and the integrated Hyperlink viewer.

The documentation in this folder is intended to describe the current architecture, deployment model, operational flow, and extension points for the stack that was validated with:

- `python .\\hpcc_main.py`
- `http://127.0.0.1:5001/html`
- `http://127.0.0.1:5100/health`
- `127.0.0.1:9100` broker access

## Documentation Set

### 1. Platform Overview

This file is the entrypoint into the documentation set.

### 2. Developer Runbook

See `README_dev.md` for:

- the current developer workflow
- the meaning of the large artifact files commonly left out of git
- exact build, upload, and run commands
- a practical explanation of when to rebuild images versus when to sync source only

### 3. Integration And Operations Guide

See `README_hpcc_integration.md` for:

- end-to-end runtime topology
- build and launch commands
- port mappings
- user-visible tool behavior
- broker and runtime-store behavior
- operational troubleshooting

### 4. System Design Deep Dive

See `HPCC_RUNTIME_SYSTEM_DESIGN.md` for:

- high-level design goals and constraints
- low-level component responsibilities
- startup and request sequence flows
- data ownership and persistence model
- architectural decisions and extension guidance

### 5. Diagram Pack

See `hpcc_integration.drawio` for multi-page diagrams covering:

- `System Design`
- `HLD`
- `LLD`
- `UML`
- `Project Architecture`
- `Runtime Flow`

## Current Platform Summary

The current stack is organized around a single runtime control plane.

### User Entry

- `main_html` is the browser entrypoint.
- The dashboard is the single front door.
- KPI and Hyperlink are the primary exposed user tools.
- RAG is integrated through the dashboard chat instead of being exposed as a separate standalone UI.

### Control Plane

- `hpcc_main.py` owns runtime orchestration.
- The broker accepts JSON-over-socket requests for `ping`, `submit`, `status`, and `cancel`.
- Runtime metadata is stored in SQLite through `main_html/runtime_store.py`.
- The launcher can delegate the broker into WSL while keeping a stable Windows localhost front door.

### Execution Plane

- `main_html.simg` serves the Flask UI.
- `rag/rag.simg` serves the RAG API.
- `kpi/can/can_kpi.simg`, `kpi/udp/udp_kpi.simg`, and `kpi/int_plot/intplot_kpi.simg` execute compute-oriented jobs.
- Hyperlink remains integrated in-process through `main_html` routes.

## Repository Areas

The most important folders for the integrated runtime are:

```text
simg_zmq/
|-- hpcc_main.py                     # Broker, launcher, Windows/WSL forwarding
|-- main_html/                      # Primary web application and runtime map
|   |-- app.py
|   |-- runtime_store.py
|   |-- hpcc_broker_client.py
|   |-- rag_client.py
|   `-- templates/
|-- rag/                            # RAG service and storage
|-- KPI/                            # CAN KPI, UDP KPI, and Interactive Plot implementations
|-- Hyperlink_tool/                 # Integrated viewer code
|-- scripts/                        # Build helpers
|-- simg_sh_hpcc/                   # Built images and runtime state
`-- Readme/                         # This documentation set
```

## Quick Start

### Build Images

Run inside WSL:

```bash
bash scripts/wsl_build_hpcc_bundle.sh
```

Expected outputs in `simg_sh_hpcc/`:

- `main_html.simg`
- `kpi/can/can_kpi.simg`
- `kpi/udp/udp_kpi.simg`
- `kpi/int_plot/intplot_kpi.simg`
- `rag/rag.simg`

### Start The Stack

Run on Windows from the repository root:

```powershell
python .\hpcc_main.py
```

### Verify Health

```powershell
Invoke-WebRequest -UseBasicParsing http://127.0.0.1:5001/login
Invoke-WebRequest -UseBasicParsing http://127.0.0.1:5100/health
```

## Runtime Ports

| Port | Owner | Purpose |
|------|-------|---------|
| `5001` | `main_html.simg` | Browser UI |
| `5100` | `rag.simg` | RAG HTTP API |
| `9100` | `hpcc_main.py` broker | Runtime control socket |

## Runtime State Locations

| Path | Purpose |
|------|---------|
| `simg_sh_hpcc/runtime_state/main_html/cache_html/` | `main_html` cache and SQLite database |
| `simg_sh_hpcc/runtime_state/rag/` | RAG SQLite data and vector store |
| `simg_sh_hpcc/runs/` | Broker-created per-run logs and outputs |

## Recommended Reading Order

1. Read `README_dev.md` if your immediate goal is to build, upload, run, or explain the current artifact set.
2. Read `README_hpcc_integration.md` to understand the end-to-end stack.
3. Read `HPCC_RUNTIME_SYSTEM_DESIGN.md` if you need component-level design detail.
4. Open `hpcc_integration.drawio` in diagrams.net for the architecture and sequence views.

## Scope Note

Some older files in the repository still describe previous layouts such as legacy `all_services`, Docker-first workflows, or older cluster packaging approaches. This documentation set is specifically about the current `main_html` + `hpcc_main.py` + `simg_sh_hpcc` architecture.