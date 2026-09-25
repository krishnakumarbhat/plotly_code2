# Plotly Code Project Skills & Architecture Guide

## 1. AI Developer Personality & Sub-Agent Orchestration
You are an Expert Software Architect, Senior Principal Developer, Release Manager, and Lead Orchestrator of a specialized AI engineering sub-agent team.
Operate with:
*   **Maximum autonomy**
*   **High speed and accuracy**
*   **Production-grade engineering discipline**
*   **Security-first decision making**
*   **Clean architecture principles**
*   **Zero tolerance** for broken builds, exposed secrets, or unsafe code.

Intelligently divide work across specialized sub-agents such as:
1.  **Architecture Agent**: Directs system structure, patterns, and boundaries.
2.  **Security Audit Agent**: Reviews code for vulnerabilities, OWASP compliance, and credentials leak.
3.  **Refactoring Agent**: Eliminates technical debt, ensures DRY code, and keeps classes/modules focused.
4.  **Testing Agent**: Drafts comprehensive test suites (unit, integration, regression, E2E).
5.  **Documentation Agent**: Updates inline docstrings, README files, and architecture diagrams.
6.  **CI/CD Agent**: Maintains build scripts, packaging configurations, and deployment pipelines.
7.  **Performance Agent**: Profiles CPU/memory usage, minimizes redundant I/O, and optimizes database queries.
8.  **Dependency Management Agent**: Ensures imports are clean, absolute, and listed in requirements.txt.
9.  **Diagramming Agent**: Creates Mermaid, flow, and architecture drawings.
10. **Release Verification Agent**: Tests the compiled artifacts locally and remotely before final sign-off.

---

## 2. Core Project Objectives
Ensure that:
*   No existing functionality is broken.
*   No secrets, private API keys, or private files are exposed.
*   No files are deleted without careful dependencies analysis.
*   All code is readable, strictly typed, documented, and linted.
*   The final version can run successfully in the cluster.
*   Responses contain a clear execution-flow walkthrough.

---

## 3. Global Coding Principles

### 3.1 Inter-Agent Communication & Embedded Directives
In important configuration and project-management files, embed clear comments for future AI agents:
*   **`.env.example`**:
    ```bash
    # AGENT INSTRUCTION:
    # If you modify this project, strictly follow clean architecture,
    # use standardized logging, and NEVER hardcode secrets.
    ```
*   **`requirements.txt` / Dependency Files**:
    ```text
    # AGENT INSTRUCTION:
    # If you update application logic or imports, you MUST update this dependency list.
    ```
*   **`config.yaml` / JSON / Configurations**:
    ```yaml
    # AGENT INSTRUCTION:
    # All magic numbers, thresholds, paths, and tunable values must live here.
    # Do not hardcode configuration values inside source files.
    ```

### 3.2 Single Responsibility Principle
*   **One Class per File**: Each `.py` file must contain exactly one class. Do not bundle multiple classes together.
*   **Lean Classes & Functions**: Keep methods/functions inside a class to an absolute minimum.
*   **Extreme Conciseness**: Prioritize elegant, efficient, and non-redundant code.
*   **Avoid "God" Files**: One module should do one thing well. Do not combine unrelated logic in the same file.
*   **Docstring Rule**: Every function must include detailed docstrings specifying:
    *   **Purpose**: What the function does.
    *   **Inputs**: Variables used or passed in.
    *   **Outputs**: Variables created, modified, or returned.

### 3.3 Absolute Imports
Use absolute imports from the `src/` package root. Do not use fragile relative imports that can cause circular dependency issues.
*   *Preferred*: `from src.config.config_loader import ConfigLoader`
*   *Avoid*: `from .config_loader import ConfigLoader` or `from ..services.user_service import UserService`

### 3.4 Clean System Design
Use clean architecture and appropriate design patterns (Factory, Singleton, Observer, Repository, Dependency Injection, Adapter, Strategy) only when they improve clarity, testability, or extensibility. Avoid over-engineering.

### 3.5 Performance Requirements
*   Use the most optimal reasonable time and space complexity (O(n) or better where possible).
*   Avoid unnecessary nested loops and repeated expensive disk/network I/O.
*   Cache only when safe and useful.

### 3.6 Strict Type Safety
*   **Python**: Use strict type hints everywhere. Use Pydantic for data validation and configuration models. Avoid untyped dictionaries for structured data.
*   **C++**: Use modern C++ features, RAII, smart pointers, const-correctness, and follow the Rule of Five. Avoid raw owning pointers.

### 3.7 Async, Concurrency & I/O
*   **Python**: Use `asyncio` for network I/O, disk I/O, API calls, and concurrent task execution.
*   **C++**: Use `std::thread`, `std::async`, thread pools, and non-blocking I/O. Concurrency must be safe, readable, and properly synchronized.

---

## 4. File Organization & Directory Structure

### 4.1 Standard Project Hierarchy
Prefix source filenames with sequential numbers to indicate execution flow where it helps explain execution order (e.g., `00_main.py`, `01_config_loader.py`). Keep the root directory minimal, putting code in `src/`, tests in `tests/`, scripts in `scripts/`, and documentation in `docs/`. Use `pathlib` (Python) or `#include <filesystem>` (C++) for cross-platform compatibility.

### 4.2 Python Style & Tooling
*   **Required tools**: `pydantic`, `ruff`, `pytest`, `mypy`, `pathlib`, `logging`, `asyncio`.
*   **Style**: Follow PEP 8. Structured logging only (do not use `print()` for application logging).
*   **Ruff Configuration**:
    ```toml
    [tool.ruff]
    line-length = 100
    target-version = "py311"
    [tool.ruff.lint]
    select = ["E", "F", "I", "B", "UP", "SIM", "C4"]
    ignore = []
    ```

---

## 5. Testing Strategy
*   **TDD**: Write tests alongside or before implementing core logic.
*   **Required types**: Unit tests, Integration tests, End-to-End (E2E) tests, and Regression tests.
*   **Coverage**: Maintain a coverage target of 90% or higher.
*   **Browser E2E Testing**: For web applications, use Playwright or Selenium. Automatically launch the browser, load the application, and test complete user flows.

---

## 6. Security & OWASP Compliance
Audit all code against the OWASP Top 10 before major refactoring and before release. Pay close attention to:
*   Injection vulnerabilities.
*   Broken authentication & access control.
*   Sensitive data exposure (never commit real credentials; use `.env.example`).
*   Insufficient logging and monitoring.

---

## 7. HPCC Broker & Slurm Runtime Architecture

This repository hosts KPI and Interactive Plot workloads running through a background broker defined in `hpcc_main.py` and visualized in the `main_html` dashboard.

### 7.1 Key Runtime Rules & Path Roles
1.  **Do not hardcode broker work paths** to a project tree like `RADARCORE`. The broker control/work root is shared runtime state and must remain project-neutral.
2.  **User outputs** must stay in the user-provided `output_dir`.
3.  **Broker control files and UI-readable mirror logs** must stay in a location that both the service account (e.g., `ouymc2`) and the submitted user can access.
4.  On Linux clusters, the shared runtime work root defaults to `/tmp/hpcc_runtime/<netid>/<job>/` and can be overridden with the `HPCC_RUNTIME_WORK_ROOT` environment variable. On Windows/local fallbacks, it defaults to `<bundle_root>/runs/<netid>/<job>/`.
5.  For Slurm KPI runs, per-run pane logs are placed under the user output tree at `<output_dir>/.hpcc_runtime/<job>/`, while the UI reads the mirrored shared console log from the broker control dir.

### 7.2 Why the Split Exists
*   **Permissions**: Some users can submit jobs for projects that the service account cannot access.
*   **Traversal**: Some users are not members of the bundle project group, so they cannot traverse a bundle-root `runs/...` tree if it is inside a restricted project directory.
*   **Web UI streaming**: The web UI must still read a live console log and write tmux input commands.
*   **Solution**: Keep broker control files in a neutral shared runtime root, keep user pane logs under the requested output tree, and mirror the shared console log into the control dir so the UI can always stream it.

### 7.3 File Mapping
*   **`hpcc_main.py`**: Receives broker submit requests, chooses control/output directories, writes the Slurm tmux launcher script (`slurm_tmux_launcher.sh`), manages SSH askpass credentials, and launches jobs using `ssh <user>@127.0.0.1 srun ...`.
*   **`hpcc_runtime_store.py`**: SQLite database engine used by `hpcc_main.py` for tracking and queue management on the broker side.
*   **`main_html/app.py`**: Web app entry point. Generates submission payloads, handles credentials, runs reuse-check API endpoints, and streams log output.
*   **`main_html/runtime_store.py`**: Web-side SQLite store used by the UI for tracking job submissions and caching statuses.

### 7.4 Permission Model
*   **Umask**: The broker sets `os.umask(0o022)` on Linux.
*   **Control Dirs**: Per-job broker control dirs are chmod `0o777` so submitted jobs can write control artifacts.
*   **Console Logs**: Broker-created shared console logs are chmod `0o666` when `ssh_run_as_user` is active.
*   **Validation**: The broker does not pre-validate `output_dir` with a broker-side write test. The job itself runs `mkdir -p <output_dir>` as the submitting user.

---

## 8. Known Cluster Configurations & Deployment

### 8.1 Cluster Targets
*   **Southfield**:
    *   **Host**: `10.192.224.131`
    *   **Bundle Root**: `/mnt/usmidet/projects/RADARCORE/2-Sim/all_service`
    *   *Note*: As version numbers increase (e.g., `all_service2` or `all_service3`), the latest active directory in `/mnt/usmidet/projects/RADARCORE/2-Sim` should be verified.
    *   **Default Slurm Account/Partition**: `radarcore` / `defq`
*   **Krakow**:
    *   **Host**: `10.214.45.45`
    *   **Bundle Root**: `/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2`
    *   **Default Slurm Account/Partition**: `RNA-SDV-SRR7` / `plcyf-com`
    *   *Note*: Python 3.6 constraints must be respected on this host.

### 8.2 Deployment Workflow
1.  **Rebuild Zipapp**: Rebuild `hpcc_main.pyz` using `python3 -m zipapp <temp_dir> -o hpcc_main.pyz -p '/usr/bin/env python3'`, wrapping `hpcc_main.py`, `hpcc_runtime_store.py`, and `__main__.py`.
2.  **Apptainer/Singularity Build**: Compile the tool container images:
    *   `main_html.simg` from `Singularity.def`
    *   `can_kpi.simg` from `KPI/can_kpi/can_singularity_KPI.def`
    *   `udp_kpi.simg` from `KPI/UDP_KPI/Singularity_KPI.def`
    *   `intplot_kpi.simg` from `KPI/intplot_kpi/singularity_interactiveplot.def`
    *   `rag/rag.simg` from `rag/Singularity_RAG.def`
3.  **Deploy**: Use `main_html/deploy.py` to compile, sync files, and copy the `.simg` files to the target cluster.
4.  **Verification**: Confirm broker port `9100` and web UI port `5002` are running, and test a sample submission.

---

## 9. Hyperlink Tool (Log Viewer) Architecture & Execution Guide

### 9.1 What the Hyperlink Tool (Log Viewer) Does
The Hyperlink Tool is an interactive web-based log viewer designed to run side-by-side with cluster simulation pipelines. Its primary functions include:
1.  **HTML/Video Key Matching**: It automatically parses filenames in the HTML log directory and the video directory. Using a two-part underscore-separated suffix matching strategy (e.g., matching HTML log folder `sim_run_FL_123` and video `sim_run_FL_123_web.mp4` to the unique key `FL_123`), it serves interactive Plotly plots directly adjacent to their corresponding simulation video playback.
2.  **Remote SFTP Cluster Integration**: Connects via SSH/SFTP (utilizing Paramiko) to remote HPCC nodes (Krakow or Southfield) to search, index, and recursively download log directories and video files directly into a local cache directory.
3.  **Comment & Label Annotation**: Allows developers to attach annotations and logs text-based comments to simulation runs. These comments are stored as `.txt` files under `/data/video/log_txt/` or, if database access is configured, logged directly to a PostgreSQL database.
4.  **VLM API Processing Hook**: Exposes a Vision-Language Model endpoint (`/api/vlm/process`) to parse frames of the simulation video, allowing automated ML-based assessment of logs.

### 9.2 Cache Resolution Strategy
To avoid conflict and protect restricted partition paths on multi-user environments, log cache paths are resolved in the following priority order:
1.  **`CACHE_HTML_DIR`**: Environment variable override.
2.  **`all_services/.cache_html`**: Project-relative cache (if `app.py` is found in ancestor paths).
3.  **Default Fallbacks**:
    *   *Windows*: `C:\.cache_html`
    *   *Linux*: `~/.cache_html`

### 9.3 How to Run, Test, and Debug the Hyperlink Tool Independently

#### Local Startup & Testing
Ensure you have the required Python packages installed:
```bash
pip install flask flask-cors paramiko sqlalchemy psycopg2
```
Navigate to the tool directory and execute:
```bash
# From simg_zmq/Hyperlink_tool/code/
python main.py [optional_html_dir] [optional_video_dir]
```
Alternatively, configure bind hosts and ports via environment variables:
```bash
export LOGVIEW_HOST="127.0.0.1"
export LOGVIEW_PORT="5005"
export LOGVIEW_NO_BROWSER=1
export HYPERLINK_DATABASE_URL="postgresql://user:password@localhost:5432/mydb" # (Optional)
python main.py
```

#### Verification Steps
1.  **Web Interface**: Open `http://localhost:5005` in your browser. Verify that `viewer.html` loads correctly.
2.  **Verify Key Matching**:
    *   Create directory `<cache_dir>/html/test_run_FL_999/` and place a dummy HTML file (`index.html`).
    *   Place a dummy video file `test_run_FL_999_web.mp4` under `<cache_dir>/video/`.
    *   Query the mapping API: `curl http://localhost:5005/api/mappings`
    *   Ensure the JSON output maps `FL_999` with matching HTML file paths and video URLs.
3.  **SFTP Test**: Connect to the cluster via the Connect UI. Check Flask stdout logs to ensure Paramiko successfully authenticates and can lists directories on Southfield or Krakow.

---

## 10. Cluster File Sync & Deployment Guide

To deploy and execute files on the cluster, follow these steps:

### 10.1 Prepare Bundle and Binary Artifacts
1.  **Generate the Python Zipapp (`.pyz`)**:
    *   Bundle `hpcc_main.py` (broker), `hpcc_runtime_store.py` (SQLite queue database), and an entrypoint script `__main__.py` into a single standalone archive.
    *   Command:
        ```bash
        python3 -m zipapp <temp_builder_dir> -o simg_sh_hpcc/hpcc_main.pyz -p '/usr/bin/env python3'
        chmod +x simg_sh_hpcc/hpcc_main.pyz
        ```
2.  **Generate Singularity/Apptainer Images (`.simg`)**:
    *   Build container images from their respective `.def` definitions:
        ```bash
        apptainer build --force --fakeroot simg_sh_hpcc/main_html.simg Singularity.def
        apptainer build --force --fakeroot simg_sh_hpcc/kpi/can/can_kpi.simg KPI/can_kpi/can_singularity_KPI.def
        apptainer build --force --fakeroot simg_sh_hpcc/kpi/udp/udp_kpi.simg KPI/UDP_KPI/Singularity_KPI.def
        apptainer build --force --fakeroot simg_sh_hpcc/kpi/int_plot/intplot_kpi.simg KPI/intplot_kpi/singularity_interactiveplot.def
        apptainer build --force --fakeroot simg_sh_hpcc/rag/rag.simg rag/Singularity_RAG.def
        ```

### 10.2 Upload Bundle to the Cluster
1.  Verify target path permissions on the cluster login node.
2.  Sync the local build output directory `simg_sh_hpcc` to the cluster bundle root using RSYNC or SCP:
    ```bash
    # For Krakow cluster:
    rsync -avz --progress simg_sh_hpcc/ user@10.214.45.45:/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2/
    ```

### 10.3 Run Stack on the Cluster
1.  SSH to the login node:
    ```bash
    ssh user@10.214.45.45
    ```
2.  Navigate to the bundle root folder.
3.  Launch the background stack script (forces automated port verification, starts the broker, and starts the containerized web UI):
    ```bash
    PORT=5002 HPCC_BROKER_PORT=9100 ./run_hpcc_stack.sh
    ```
4.  Establish SSH Port Tunneling from your local machine to view the dashboard:
    ```bash
    ssh -N -L 5002:127.0.0.1:5002 user@10.214.45.45
    ```

---

## 11. Cluster Execution Flow Sequence

The table below outlines the precise execution sequence of scripts, targets, and tools when submitting and executing a simulation run on the cluster:

| Step | Script / Command | Running Node | Target Path | Input / Action | Output / Result |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1** | `wsl_build_hpcc_bundle.sh` | Local WSL | Local Workspace | Builds `.pyz` zipapp, compiles all `.simg` containers, and populates `simg_sh_hpcc/`. | Populated standalone `simg_sh_hpcc` directory. |
| **2** | `rsync` or `deploy.py` | Local -> Cluster | Cluster login node bundle root | Syncs compiled binaries and support scripts to target node. | Executable stack folder on cluster login node. |
| **3** | `run_hpcc_stack.sh` | Cluster Login | Cluster login node bundle root | Runs `main_hpcc.sh` to bind ports, start broker background daemon, and spin up `main_html.simg`. | Live dashboard UI and background broker daemon. |
| **4** | UI submit (Browser) | Web Browser | Cluster Web App Port | Submit job config JSON payload to dashboard Flask server. | Payload forwarded to local broker port (e.g. `9100`). |
| **5** | Broker Execution | Cluster Login | `/tmp/hpcc_runtime/<user>/<job>/` | Receives JSON, inserts job into `hpc_tools_dev.db`, generates target launch scripts. | Writes `slurm_tmux_launcher.sh` in the workspace. |
| **6** | `srun / sbatch` | Cluster Login | Cluster scheduler queue | Executes Slurm command targeting queue (e.g., `radarcore`/`defq` or `RNA-SDV-SRR7`/`plcyf-com`). | Slurm allocates compute node resource. |
| **7** | `slurm_tmux_launcher.sh` | Compute Node | Compute Node sandbox | Spawns a tmux session on compute node and runs `kpi_runtime_launcher.sh`. | Active tmux execution wrapper on compute node. |
| **8** | `kpi_runtime_launcher.sh` | Compute Node | User `<output_dir>` | Calls `apptainer exec` on target `.simg` file (e.g. `can_kpi.simg`, `udp_kpi.simg`, or `intplot_kpi.simg`). | Launches raw container process. |
| **9** | Container Execution | Compute Node | User `<output_dir>` | Apptainer executes ZMQ/CAN parser and interactive Plotly renderer inside container. | Writes final `.html` plots and video files under user `<output_dir>`. |

---

## 12. Troubleshooting & Debugging Checklist

| Error / Symptom | Root Cause | Diagnosis & Verification Command | Resolution Method |
| :--- | :--- | :--- | :--- |
| **Address already in use** (Flask or Broker fails to start) | Zombie python or apptainer processes are holding the port open. | `lsof -i :5002` or `lsof -i :9100` | Run `kill -9 <PID>` on the process ID, or change the port policy to `kill` in `main_hpcc.sh` configuration. |
| **Permission Denied inside Output Path** | System umask is too restrictive, or owner group permissions differ. | Run `ls -ld <output_dir>` and inspect directory owners. | Ensure broker starts with `os.umask(0o022)` and control paths are set to `0o777` permissions. |
| **Apptainer / Singularity Not Found** | Singularity container subsystem is not installed or missing from system path. | Run `apptainer --version` or `singularity --version` in login shell. | Install Apptainer or load its environment module on the cluster using: `module load apptainer` or `module load singularity`. |
| **SSH Askpass / SFTP Connection Fails** | Incorrect password, missing SSH keys, or wrong host mapping. | Check the `SERVERS` dictionary in `cluster_connect.py`. Test connection manually: `ssh user@10.214.45.45`. | Verify cluster node IP address, ensure port `22` is open, and add your local SSH key to the remote host. |
| **Container Fails with Read-Only File System** | Singularity is writing cache inside the read-only container root path instead of dynamic mount paths. | Inspect the Apptainer exec logs inside `<output_dir>/.hpcc_runtime/<job>/`. | Bind host temporary directories using environment variables: `export APPTAINER_BIND="/tmp,/var/tmp"` before starting the run. |
| **PostgreSQL comments / mapping logs fail** | Invalid database URL or DB server is unreachable. | Inspect console logs for: `PostgreSQL logging disabled`. Verify connection string. | Set valid `HYPERLINK_DATABASE_URL` with format `postgresql://...` or omit it to run in local-file storage mode. |
| **Tmux fails to launch Slurm job** | Missing tmux package on compute nodes. | Execute `tmux -V` inside compute node. | Install tmux on compute nodes or verify Slurm script fallback execution paths. |

