#!/usr/bin/env bash
#==============================================================================
# run_all_services.sh - Run the HPC Tools Platform Singularity container
#==============================================================================
# This script runs the all_services Singularity image on HPC clusters
#
# Usage:
#   ./run_all_services.sh [PORT]
#   ./run_all_services.sh --help
#
# Options:
#   PORT            Port to run on (default: 5001)
#   --debug         Run in debug mode
#   --shell         Open shell instead of running app
#   --help          Show this help message
#
# Supported clusters:
#   - Krakow (10.214.45.45)     - /net/8k3/...
#   - Southfield (10.192.224.131) - /mnt/usmidet/...
#==============================================================================

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Default configuration
IMAGE_NAME="all_services.simg"
IMAGE_PATH="$SCRIPT_DIR/$IMAGE_NAME"
PORT="${1:-5001}"
HOST="${HOST:-0.0.0.0}"
WORKERS="${WORKERS:-4}"
DEBUG_MODE=false
SHELL_MODE=false

# Parse optional flags
while [[ "${1:-}" == --* ]]; do
    case $1 in
        --debug)
            DEBUG_MODE=true
            shift
            ;;
        --shell)
            SHELL_MODE=true
            shift
            ;;
        --help|-h)
            head -20 "$0" | tail -15
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Re-read positional args after flag parsing
PORT="${1:-5001}"

#------------------------------------------------------------------------------
# Cleanup: Kill any existing processes on the target port to ensure clean start
#------------------------------------------------------------------------------
cleanup_existing_processes() {
    local port="$1"
    echo -e "${YELLOW}Cleaning up existing processes on port $port...${NC}"
    
    # Kill processes using the specified port (gunicorn, flask, python)
    # Using fuser if available (common on Linux)
    if command -v fuser >/dev/null 2>&1; then
        fuser -k "$port/tcp" 2>/dev/null || true
    fi
    
    # Also try using lsof (works on most Unix systems)
    if command -v lsof >/dev/null 2>&1; then
        local pids=$(lsof -t -i:"$port" 2>/dev/null || true)
        if [ -n "$pids" ]; then
            echo -e "${YELLOW}Killing processes on port $port: $pids${NC}"
            echo "$pids" | xargs -r kill -9 2>/dev/null || true
        fi
    fi
    
    # Kill any flask/gunicorn processes that might be orphaned
    pkill -f "gunicorn.*:$port" 2>/dev/null || true
    pkill -f "flask.*:$port" 2>/dev/null || true
    
    # Give processes time to terminate
    sleep 2
    
    echo -e "${GREEN}Cleanup complete.${NC}"
}

# Run cleanup before starting
cleanup_existing_processes "$PORT"

# Try to load Singularity module on HPC (if Modules is available).
# This is a no-op on systems without environment-modules.
if type module >/dev/null 2>&1; then
    # Prefer newer runtime on clusters that provide it.
    module load singularity/3.11.4 >/dev/null 2>&1 || module load singularity/3.8.0 >/dev/null 2>&1 || true
    # Slurm client commands (sbatch/srun/sacct/scancel) may be provided via module.
    # Best-effort: load a generic 'slurm' module if it exists.
    module load slurm >/dev/null 2>&1 || true
fi

# Detect cluster based on available paths
detect_cluster() {
    if [ -d "/net/8k3" ]; then
        echo "krakow"
    elif [ -d "/mnt/usmidet" ]; then
        echo "southfield"
    else
        echo "unknown"
    fi
}

# Get LLM server URL based on cluster
get_llm_url() {
    local cluster="$1"
    case "$cluster" in
        krakow)
            echo "http://10.214.45.45:8000/generate"
            ;;
        southfield)
            echo "http://10.192.224.131:8000/generate"
            ;;
        *)
            echo "http://localhost:8000/generate"
            ;;
    esac
}

if [ ! -f "$IMAGE_PATH" ]; then
    echo -e "${RED}ERROR: image not found: $IMAGE_PATH${NC}" >&2
    echo "Please ensure $IMAGE_NAME is in the same directory as this script." >&2
    exit 2
fi

RUNTIME_TOOL=""
if command -v apptainer >/dev/null 2>&1; then
    RUNTIME_TOOL="apptainer"
elif command -v singularity >/dev/null 2>&1; then
    RUNTIME_TOOL="singularity"
else
    # One more attempt after module load (some clusters only provide singularity via modules)
    if type module >/dev/null 2>&1; then
        module load singularity/3.8.0 >/dev/null 2>&1 || true
    fi
    if command -v apptainer >/dev/null 2>&1; then
        RUNTIME_TOOL="apptainer"
    elif command -v singularity >/dev/null 2>&1; then
        RUNTIME_TOOL="singularity"
    fi
    if [ -z "$RUNTIME_TOOL" ]; then
    echo -e "${RED}ERROR: neither 'apptainer' nor 'singularity' is available on PATH.${NC}" >&2
    exit 3
    fi
fi

# Preflight: verify the SIF is readable before attempting to run.
if ! "$RUNTIME_TOOL" sif list "$IMAGE_PATH" >/dev/null 2>&1; then
    echo -e "${RED}FATAL: Container image appears corrupted or unreadable:${NC} $IMAGE_PATH" >&2
    echo -e "${YELLOW}Common cause:${NC} partial/corrupted upload (e.g., dropped connection)." >&2
    echo "Fix: rebuild and re-sync the image (use --include-image; consider --parallel-upload)." >&2
    echo "Also verify size/checksum on cluster matches local file." >&2
    exit 4
fi

# Detect cluster
CLUSTER=$(detect_cluster)
LLM_URL=$(get_llm_url "$CLUSTER")

# Build bind args only for paths that exist on the host
BIND_ARGS=()
# Krakow paths
if [ -d /net ]; then
    BIND_ARGS+=(--bind /net:/net)
fi
# Southfield paths
if [ -d /mnt ]; then
    BIND_ARGS+=(--bind /mnt:/mnt)
fi
# Common paths
if [ -d /scratch ]; then
    BIND_ARGS+=(--bind /scratch:/scratch)
fi
if [ -d /home ]; then
    BIND_ARGS+=(--bind /home:/home)
fi
if [ -d /tmp ]; then
    BIND_ARGS+=(--bind /tmp:/tmp)
fi

# If Slurm client binaries exist on the host, bind their directory into the container
# so the web app (running inside the container) can execute srun/sbatch/sacct.
SLURM_BIN_DIR=""
if command -v srun >/dev/null 2>&1; then
    SLURM_BIN_DIR="$(dirname "$(command -v srun)")"
elif command -v sbatch >/dev/null 2>&1; then
    SLURM_BIN_DIR="$(dirname "$(command -v sbatch)")"
fi

if [ -n "$SLURM_BIN_DIR" ] && [ -d "$SLURM_BIN_DIR" ]; then
    # Bind the whole bin dir (usually contains sbatch/srun/sacct/scancel)
    BIND_ARGS+=(--bind "$SLURM_BIN_DIR:$SLURM_BIN_DIR")
fi

# Bind common Slurm config locations if present on the host.
if [ -d /etc/slurm ]; then
    BIND_ARGS+=(--bind /etc/slurm:/etc/slurm)
fi
if [ -f /etc/slurm.conf ]; then
    BIND_ARGS+=(--bind /etc/slurm.conf:/etc/slurm.conf)
fi

# Set cache directory - use a writable location
export CACHE_HTML_DIR="$SCRIPT_DIR/.cache_html"
export HTML_DB_DIR="$SCRIPT_DIR/html_db"
mkdir -p "$CACHE_HTML_DIR/html" "$CACHE_HTML_DIR/video" 2>/dev/null || true
mkdir -p "$HTML_DB_DIR" 2>/dev/null || true

# Bind the cache directory to the container's expected path
BIND_ARGS+=(--bind "$CACHE_HTML_DIR:/app/simg/.cache_html")
BIND_ARGS+=(--bind "$HTML_DB_DIR:/app/simg/html_db")

# Store host path so the app can display it to users
export HOST_SIMG_PATH="$SCRIPT_DIR"

if [ -z "${DATABASE_URL:-}" ]; then
    # Set default DATABASE_URL to use SQLite in .cache_html
    export DATABASE_URL="sqlite:////app/simg/.cache_html/hpc_tools_dev.db"
    echo -e "${BLUE}INFO: Using SQLite database at $CACHE_HTML_DIR/hpc_tools_dev.db${NC}"
fi

echo "============================================================"
echo -e "  ${GREEN}HPC Tools Platform - Singularity Container${NC}"
echo "============================================================"
echo "Cluster:     $CLUSTER"
echo "Runtime:     $RUNTIME_TOOL"
echo "Image:       $IMAGE_PATH"
echo "Port:        $PORT"
echo "Workers:     $WORKERS"
echo "LLM URL:     $LLM_URL"
echo "Cache Dir:   $CACHE_HTML_DIR"
echo "Log Dir:     $HTML_DB_DIR"
echo "DB:          $DATABASE_URL"
echo "Bind Paths:  ${BIND_ARGS[*]:-none}"
echo "============================================================"
echo ""
echo -e "${BLUE}Serving: http://127.0.0.1:${PORT}/html${NC}"
echo -e "${BLUE}Hyperlink: http://127.0.0.1:${PORT}/hyperlink/${NC}"
echo ""

# Slurm note:
# all_services is intended to run on a login/service node.
# Compute-heavy tools (KPI/DC/Interactive Plot) should be launched by the web app
# using Slurm resource allocation (e.g., srun/sbatch) so they run on compute nodes.
if [ -z "${SLURM_JOB_ID:-}" ] && [ -z "${SLURM_JOBID:-}" ]; then
    echo -e "${YELLOW}INFO: Web app is not inside a Slurm allocation (expected for all_services).${NC}"
    echo -e "${YELLOW}Tools launched from the UI should allocate resources via Slurm.${NC}"
    echo ""
fi

# NOTE: Don't try to conditionally *generate* an assignment via parameter expansion.
# Bash parses environment assignments before expansion, so constructs like
#   ${SLURM_BIN_DIR:+APPTAINERENV_PATH="$SLURM_BIN_DIR:$PATH"}
# will not be treated as assignments and can break execution on cluster shells.
APPTAINERENV_PATH_VALUE="$PATH"
if [ -n "${SLURM_BIN_DIR:-}" ]; then
    APPTAINERENV_PATH_VALUE="$SLURM_BIN_DIR:$APPTAINERENV_PATH_VALUE"
fi

# Run in different modes
if [ "$SHELL_MODE" = true ]; then
    echo "Opening interactive shell..."
    "$RUNTIME_TOOL" shell --writable-tmpfs "${BIND_ARGS[@]}" "$IMAGE_PATH"
elif [ "$DEBUG_MODE" = true ]; then
    echo "Running in debug mode..."
    APPTAINERENV_HOST="$HOST" \
    APPTAINERENV_PORT="$PORT" \
    APPTAINERENV_LLM_SERVICE_URL="$LLM_URL" \
    APPTAINERENV_CACHE_HTML_DIR="$CACHE_HTML_DIR" \
    APPTAINERENV_HTML_DB_DIR="$HTML_DB_DIR" \
    APPTAINERENV_HOST_SIMG_PATH="$HOST_SIMG_PATH" \
    APPTAINERENV_DATABASE_URL="$DATABASE_URL" \
    APPTAINERENV_FLASK_DEBUG=1 \
    APPTAINERENV_PATH="$APPTAINERENV_PATH_VALUE" \
        "$RUNTIME_TOOL" run --writable-tmpfs "${BIND_ARGS[@]}" "$IMAGE_PATH"
else
    APPTAINERENV_HOST="$HOST" \
    APPTAINERENV_PORT="$PORT" \
    APPTAINERENV_WORKERS="$WORKERS" \
    APPTAINERENV_LLM_SERVICE_URL="$LLM_URL" \
    APPTAINERENV_CACHE_HTML_DIR="$CACHE_HTML_DIR" \
    APPTAINERENV_HTML_DB_DIR="$HTML_DB_DIR" \
    APPTAINERENV_HOST_SIMG_PATH="$HOST_SIMG_PATH" \
    APPTAINERENV_DATABASE_URL="$DATABASE_URL" \
    APPTAINERENV_PATH="$APPTAINERENV_PATH_VALUE" \
        "$RUNTIME_TOOL" run --writable-tmpfs "${BIND_ARGS[@]}" "$IMAGE_PATH"
fi
