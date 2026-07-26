# Docker & Singularity Deployment Guide
## ResimHTMLReport - KPI Server & Interactive Plot

This guide provides step-by-step instructions for building and running the KPI Server and Interactive Plot applications using Docker and Singularity containers.

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                        Host Machine                              │
│  ┌─────────────────┐              ┌─────────────────────────┐   │
│  │  hdf_DB/        │              │  html/                   │   │
│  │  (HDF5 files)   │              │  (Generated HTML)        │   │
│  └────────┬────────┘              └──────────▲──────────────┘   │
│           │                                   │                  │
│  ─────────┼───────────────────────────────────┼──────────────── │
│           │           Docker Network          │                  │
│  ─────────┼───────────────────────────────────┼──────────────── │
│           ▼                                   │                  │
│  ┌─────────────────┐    ZMQ/Protobuf    ┌────┴────────────┐    │
│  │ Interactive     │◄──────────────────►│  KPI Server     │    │
│  │ Plot Container  │    Port 5555       │  Container      │    │
│  └─────────────────┘                    └─────────────────┘    │
└─────────────────────────────────────────────────────────────────┘
```

---

## Prerequisites

### For Docker:
- Docker Desktop installed (Windows/Mac) or Docker Engine (Linux)
- Docker Compose v2.x

### For Singularity:
- Singularity 3.x installed (Linux native or via WSL2 on Windows)

---

## Step-by-Step Docker Commands

### Step 1: Build Docker Images

```powershell
# Navigate to project directory
cd c:\git\10030156_03_Analysis_Framework_Suite\ResimulationTool\SOURCE\ApplicationProjects\ResimHTMLReport\Interactiveplot\plotly

# Build all images using docker-compose
docker-compose build

# Or build individually:
docker build -f Dockerfile.kpi -t kpi-server:latest .
docker build -f Dockerfile.interactiveplot -t interactiveplot:latest .
```

### Step 2: Start KPI Server (ZMQ Mode)

The KPI server must be running first to receive data from Interactive Plot.

```powershell
# Option A: Using docker-compose (recommended)
docker-compose up kpi-server

# Option B: Using docker run directly
docker run -it --rm ^
    --name kpi-server ^
    -p 5555:5555 ^
    -v "%cd%\hdf_DB:/app/hdf_DB" ^
    -v "%cd%\html:/app/html" ^
    -v "%cd%\logs:/app/logs" ^
    kpi-server:latest ^
    python -m KPI.kpi_server zmq 5555
```

### Step 3: Run Interactive Plot

In a **new terminal**, run the Interactive Plot container:

```powershell
# Option A: Using docker-compose
docker-compose up interactiveplot

# Option B: Using docker run (connects to KPI server via network)
docker run -it --rm ^
    --name interactiveplot ^
    --network kpi-zmq-network ^
    -e KPI_SERVER_HOST=kpi-server ^
    -e KPI_SERVER_PORT=5555 ^
    -v "%cd%\hdf_DB:/app/data" ^
    -v "%cd%\html:/app/html" ^
    -v "%cd%\plots:/app/plots" ^
    interactiveplot:latest ^
    python ResimHTMLReport.py --input /app/data/input.h5 --output /app/html
```

### Step 4: Generate Test Data (Optional)

```powershell
# Generate dummy HDF files for testing
docker-compose --profile tools run --rm kpi-generator
```

### Step 5: Run Batch Processing (JSON Mode)

```powershell
# Process multiple HDF pairs from KPIPlot.json
docker-compose --profile batch up kpi-json-processor
```

---

## Using the Build Script (Windows)

```powershell
# Show all available commands
.\docker-build.bat

# Build all images
.\docker-build.bat build

# Run KPI server in ZMQ mode
.\docker-build.bat run-kpi

# Run Interactive Plot (in new terminal)
.\docker-build.bat run-interactive

# Generate test HDF files
.\docker-build.bat run-generator

# Start all services
.\docker-build.bat up

# Stop all services
.\docker-build.bat down

# View logs
.\docker-build.bat logs

# Clean up
.\docker-build.bat clean
```

---

## Singularity Commands

### Step 1: Build Singularity Images

**Option A: Build from Definition Files (requires sudo)**

```bash
# On Linux or in WSL2
cd /path/to/plotly

# Build KPI Server image
sudo singularity build kpi_server.sif singularity_kpi.def

# Build Interactive Plot image
sudo singularity build interactiveplot.sif singularity_interactiveplot.def
```

**Option B: Convert from Docker Images**

```bash
# First build Docker images
docker-compose build

# Then convert to Singularity
singularity build kpi_server.sif docker-daemon://kpi-server:latest
singularity build interactiveplot.sif docker-daemon://interactiveplot:latest
```

**Windows (via WSL2):**

```powershell
# Build from definition files via WSL2
.\docker-build.bat singularity-build

# Or convert Docker images to Singularity
.\docker-build.bat docker-to-sif
```

### Step 2: Run KPI Server (Singularity)

```bash
# Run in ZMQ mode (default)
singularity run \
    -B ./hdf_DB:/app/hdf_DB \
    -B ./html:/app/html \
    -B ./logs:/app/logs \
    kpi_server.sif

# Run with custom port
singularity run --app zmq kpi_server.sif 5556

# Run as background instance
singularity instance start \
    -B ./hdf_DB:/app/hdf_DB \
    -B ./html:/app/html \
    kpi_server.sif kpi_srv
```

### Step 3: Run Interactive Plot (Singularity)

```bash
# Run with environment variables for KPI server connection
KPI_SERVER_HOST=127.0.0.1 KPI_SERVER_PORT=5555 \
singularity run \
    -B ./hdf_DB:/app/data \
    -B ./html:/app/html \
    -B ./plots:/app/plots \
    interactiveplot.sif \
    python ResimHTMLReport.py --input /app/data/input.h5 --output /app/html
```

### Step 4: Run Additional Apps

```bash
# Run KPI in JSON batch mode
singularity run --app json kpi_server.sif KPIPlot.json /app/html

# Run KPI in HDF pair mode
singularity run --app hdf kpi_server.sif input.h5 output.h5 /app/html

# Generate test HDF files
singularity run --app generator kpi_server.sif
```

---

## Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `KPI_SERVER_HOST` | `127.0.0.1` | KPI server hostname (use `kpi-server` in Docker network) |
| `KPI_SERVER_PORT` | `5555` | ZMQ port for KPI server communication |
| `PYTHONUNBUFFERED` | `1` | Enable unbuffered Python output |

---

## Network Communication

### Docker Networking

Both containers communicate over the `kpi-zmq-network` bridge network:

1. **KPI Server** binds to `tcp://*:5555`
2. **Interactive Plot** connects to `tcp://kpi-server:5555`

The hostname `kpi-server` is resolved via Docker's internal DNS.

### Singularity Networking

Singularity containers share the host network by default. Use `localhost` or `127.0.0.1`:

```bash
# Start KPI server (binds to 0.0.0.0:5555)
singularity run kpi_server.sif

# Interactive Plot connects to localhost:5555
KPI_SERVER_HOST=127.0.0.1 singularity run interactiveplot.sif
```

---

## Data Flow

1. **Interactive Plot** processes HDF sensor data
2. **Interactive Plot** sends processing request to **KPI Server** via ZMQ/Protobuf
3. **KPI Server** receives request, processes KPI calculations
4. **KPI Server** generates KPI HTML report
5. **KPI Server** stores HTML path for retrieval
6. **Interactive Plot** can request HTML path via `receive_html_path_from_kpi_server()`

---

## Troubleshooting

### KPI Server not responding

```powershell
# Check if server is running
docker ps | findstr kpi-server

# Check server logs
docker logs kpi-server

# Test ZMQ connection
docker exec kpi-server python -c "import zmq; print('ZMQ OK')"
```

### Network issues between containers

```powershell
# Verify network exists
docker network ls | findstr kpi-zmq-network

# Verify containers are on same network
docker network inspect kpi-zmq-network
```

### Volume mount issues

```powershell
# Ensure directories exist
mkdir hdf_DB, html, logs, plots -Force

# Check mount permissions
docker run --rm -v "%cd%\hdf_DB:/app/hdf_DB" kpi-server:latest ls -la /app/hdf_DB
```

---

## Quick Reference

| Task | Command |
|------|---------|
| Build all | `docker-compose build` |
| Start KPI server | `docker-compose up kpi-server` |
| Start all services | `docker-compose up -d` |
| Stop all | `docker-compose down` |
| View logs | `docker-compose logs -f` |
| Generate test data | `docker-compose --profile tools run kpi-generator` |
| Clean up | `docker-compose down --rmi all -v` |
| Build Singularity | `sudo singularity build kpi_server.sif singularity_kpi.def` |
| Run Singularity | `singularity run -B ./hdf_DB:/app/hdf_DB kpi_server.sif` |
