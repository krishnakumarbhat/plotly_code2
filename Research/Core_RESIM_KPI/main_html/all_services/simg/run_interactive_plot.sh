#!/bin/bash
# ==============================================================================
# Interactive Plot Tool - Slurm Submission Script
# ==============================================================================
# Usage:
#   ./run_interactive_plot.sh <config.xml> <inputs.json> [output_dir]
#   ./run_interactive_plot.sh --hdf <input.h5> <output.h5> [html_dir]
#   ./run_interactive_plot.sh --local <config.xml> <inputs.json> [output_dir]
# ==============================================================================

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SIMG_DIR="${SCRIPT_DIR}"
SIMG_FILE="${SIMG_DIR}/interactive_plot.simg"
DEFAULT_HTML_DIR="${SIMG_DIR}/html_db"

# Detect if running under WSL
is_wsl() {
    grep -qi microsoft /proc/version 2>/dev/null || [ -n "${WSL_DISTRO_NAME:-}" ]
}

# Detect environment and set paths
detect_environment() {
    # Check for Krakow cluster path
    if [[ -d "/net/8k3/e0fs01/irods/PLKRA-PROJECTS" ]]; then
        CLUSTER="krakow"
        CACHE_DIR="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA/${USER}/all_services/simg/.cache"
        LOG_DIR="${CACHE_DIR}/logs"
    # Check for Southfield cluster
    elif [[ -d "/net/srf" ]] || [[ "$HOSTNAME" == *"southfield"* ]] || [[ "$HOSTNAME" == *"srf"* ]]; then
        CLUSTER="southfield"
        CACHE_DIR="/net/srf/e0fs01/irods/USER_DATA/${USER}/all_services/simg/.cache"
        LOG_DIR="${CACHE_DIR}/logs"
    # Check for SLURM environment
    elif command -v srun >/dev/null 2>&1; then
        CLUSTER="slurm"
        CACHE_DIR="/scratch/${USER}/all_services/simg/.cache"
        LOG_DIR="/scratch/${USER}/logs"
    else
        # Local/WSL environment - use repo directory
        if is_wsl; then
            CLUSTER="wsl"
        else
            CLUSTER="local"
        fi
        CACHE_DIR="${SCRIPT_DIR}/html_db"
        LOG_DIR="${SCRIPT_DIR}/html_db"
    fi
    
    # Create directories if they don't exist
    mkdir -p "$CACHE_DIR" 2>/dev/null || true
    mkdir -p "$LOG_DIR" 2>/dev/null || true
}

detect_environment

# Build bind mounts based on environment
get_bind_mounts() {
    local BINDS=""
    
    # Always bind log directory to container's /app/simg/html_db
    BINDS="--bind ${LOG_DIR}:/app/simg/html_db"
    
    # Add cluster-specific binds
    case "$CLUSTER" in
        krakow)
            BINDS="$BINDS --bind /net:/net"
            [[ -d "/scratch" ]] && BINDS="$BINDS --bind /scratch:/scratch"
            ;;
        southfield)
            BINDS="$BINDS --bind /net:/net"
            [[ -d "/scratch" ]] && BINDS="$BINDS --bind /scratch:/scratch"
            ;;
        slurm)
            [[ -d "/net" ]] && BINDS="$BINDS --bind /net:/net"
            [[ -d "/scratch" ]] && BINDS="$BINDS --bind /scratch:/scratch"
            ;;
        local|wsl)
            # For local/WSL, bind the repo simg directory and mount point for data access
            BINDS="$BINDS --bind ${SIMG_DIR}:/app/simg"
            # Bind /mnt for Windows paths in WSL
            [[ -d "/mnt" ]] && BINDS="$BINDS --bind /mnt:/mnt"
            # If /net exists locally (rare), bind it too
            [[ -d "/net" ]] && BINDS="$BINDS --bind /net:/net"
            ;;
    esac
    
    echo "$BINDS"
}

# Resource allocation (defaults - may be adjusted by check_cluster_resources)
MEMORY="64G"
CPUS=8
TIME_LIMIT="04:00:00"
PARTITION="plcyf-com"
SINGULARITY_MODULE="singularity/3.11.4"

# Nodes to exclude (log nodes should not run compute jobs)
EXCLUDED_NODES="plcyf-com-prod-log01,plcyf-com-prod-log02"

# Print runtime environment info (called inside the job for logging)
print_runtime_info() {
    echo "============================================================"
    echo "  RUNTIME ENVIRONMENT INFO"
    echo "============================================================"
    echo "Hostname:     $(hostname)"
    echo "Date:         $(date)"
    echo "User:         $USER"
    echo "Working Dir:  $(pwd)"
    if command -v free >/dev/null 2>&1; then
        echo "Memory Total: $(free -h | awk '/^Mem:/{print $2}')"
        echo "Memory Avail: $(free -h | awk '/^Mem:/{print $7}')"
    fi
    echo "CPU Cores:    $(nproc 2>/dev/null || echo 'unknown')"
    if [[ -n "${SLURM_JOB_ID:-}" ]]; then
        echo "SLURM Job ID: $SLURM_JOB_ID"
        echo "SLURM Node:   ${SLURM_NODELIST:-unknown}"
        echo "SLURM CPUs:   ${SLURM_CPUS_ON_NODE:-unknown}"
        echo "SLURM Mem:    ${SLURM_MEM_PER_NODE:-unknown} MB"
    fi
    echo "============================================================"
}

# Check cluster resources and adjust allocation if needed
check_cluster_resources() {
    local requested_mem=$1
    local requested_cpus=$2
    
    # Skip if sinfo not available
    if ! command -v sinfo >/dev/null 2>&1; then
        return 0
    fi
    
    echo -e "${BLUE}Checking cluster resource availability...${NC}"
    
    # Get available resources on partition (excluding log nodes)
    # Format: MEMORY CPUS
    local avail_info
    avail_info=$(sinfo -p "$PARTITION" -h -o "%m %c" --state=idle 2>/dev/null | head -1)
    
    if [[ -z "$avail_info" ]]; then
        echo -e "${YELLOW}Warning: Could not query cluster resources. Using defaults.${NC}"
        return 0
    fi
    
    local avail_mem_mb=$(echo "$avail_info" | awk '{print $1}')
    local avail_cpus=$(echo "$avail_info" | awk '{print $2}')
    
    # Convert requested memory to MB for comparison
    local req_mem_mb
    if [[ "$requested_mem" == *G ]]; then
        req_mem_mb=$(( ${requested_mem%G} * 1024 ))
    elif [[ "$requested_mem" == *M ]]; then
        req_mem_mb=${requested_mem%M}
    else
        req_mem_mb=$requested_mem
    fi
    
    local adjusted=false
    
    # Check and adjust memory
    if [[ -n "$avail_mem_mb" ]] && [[ "$req_mem_mb" -gt "$avail_mem_mb" ]]; then
        local new_mem_gb=$(( avail_mem_mb * 90 / 100 / 1024 ))  # Use 90% of available
        if [[ $new_mem_gb -lt 1 ]]; then new_mem_gb=1; fi
        echo -e "${YELLOW}Requested ${requested_mem} but max available is ${avail_mem_mb}MB. Adjusting to ${new_mem_gb}G${NC}"
        MEMORY="${new_mem_gb}G"
        adjusted=true
    fi
    
    # Check and adjust CPUs
    if [[ -n "$avail_cpus" ]] && [[ "$requested_cpus" -gt "$avail_cpus" ]]; then
        local new_cpus=$(( avail_cpus * 90 / 100 ))  # Use 90% of available
        if [[ $new_cpus -lt 1 ]]; then new_cpus=1; fi
        echo -e "${YELLOW}Requested ${requested_cpus} CPUs but max available is ${avail_cpus}. Adjusting to ${new_cpus}${NC}"
        CPUS=$new_cpus
        adjusted=true
    fi
    
    if [[ "$adjusted" == false ]]; then
        echo -e "${GREEN}Resources OK: ${MEMORY} memory, ${CPUS} CPUs available.${NC}"
    fi
}

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Progress tracking function
show_progress() {
    local current=$1
    local total=$2
    local start_time=$3
    local current_time=$(date +%s)
    local elapsed=$((current_time - start_time))
    
    local percent=$((current * 100 / total))
    local filled=$((percent / 2))
    local empty=$((50 - filled))
    
    if [ $current -gt 0 ]; then
        local eta=$(( (elapsed * (total - current)) / current ))
        local eta_min=$((eta / 60))
        local eta_sec=$((eta % 60))
        local eta_str="${eta_min}m ${eta_sec}s"
    else
        local eta_str="calculating..."
    fi
    
    local elapsed_min=$((elapsed / 60))
    local elapsed_sec=$((elapsed % 60))
    
    printf "\r${CYAN}Progress: [${NC}"
    printf "%${filled}s" | tr ' ' '█'
    printf "%${empty}s" | tr ' ' '░'
    printf "${CYAN}] ${percent}%% (${current}/${total}) | Elapsed: ${elapsed_min}m ${elapsed_sec}s | ETA: ${eta_str}${NC}"
}

print_usage() {
    echo "Interactive Plot Tool - HPC/Local Submission Script"
    echo ""
    echo "Usage:"
    echo "  JSON Mode (SLURM):"
    echo "    $0 <config.xml> <inputs.json> [output_dir]"
    echo ""
    echo "  HDF Pair Mode (SLURM):"
    echo "    $0 --hdf <input.h5> <output.h5> [html_dir]"
    echo ""
    echo "  Local Mode (with progress tracking):"
    echo "    $0 --local <config.xml> <inputs.json> [output_dir]"
    echo "    $0 --local --hdf <input.h5> <output.h5> [html_dir]"
    echo ""
    echo "Options:"
    echo "  --help, -h    Show this help message"
    echo "  --hdf         Use HDF pair mode instead of JSON batch mode"
    echo "  --local       Run locally with progress tracking"
    echo ""
    echo "Resource Allocation (SLURM mode):"
    echo "  Memory:    ${MEMORY}"
    echo "  CPUs:      ${CPUS}"
    echo "  Time:      ${TIME_LIMIT}"
    echo "  Partition: ${PARTITION}"
    echo ""
    echo "Examples:"
    echo "  $0 HTMLConfig.xml Inputs.json /output/html"
    echo "  $0 --local HTMLConfig.xml Inputs.json /output/html"
    echo "  $0 --hdf input.h5 output.h5 /output/html"
}

# Check for help flag
if [[ "$1" == "--help" ]] || [[ "$1" == "-h" ]] || [[ -z "$1" ]]; then
    print_usage
    exit 0
fi

# Check for local mode
LOCAL_MODE=false
if [[ "$1" == "--local" ]]; then
    LOCAL_MODE=true
    shift
fi

# If we're not on a cluster (or Slurm isn't available), default to local mode.
if [[ "$LOCAL_MODE" == false ]]; then
    if [[ "$CLUSTER" == "local" || "$CLUSTER" == "wsl" ]] || (! command -v srun >/dev/null 2>&1 && ! command -v sbatch >/dev/null 2>&1); then
        LOCAL_MODE=true
    fi
fi

# Check if simg exists (only needed for container mode)
if [[ "$LOCAL_MODE" == false ]] && [[ ! -f "$SIMG_FILE" ]]; then
    echo -e "${RED}Error: Singularity image not found: $SIMG_FILE${NC}"
    echo "Please build the image first:"
    echo "  apptainer build --fakeroot interactive_plot.simg Singularity_InteractivePlot.def"
    exit 1
fi

# Create log directory
mkdir -p "$LOG_DIR" 2>/dev/null || LOG_DIR="/tmp/logs"
mkdir -p "$LOG_DIR"
mkdir -p "$DEFAULT_HTML_DIR"

# Determine mode and parse arguments
if [[ "$1" == "--hdf" ]]; then
    # HDF Pair Mode
    MODE="hdf"
    shift
    
    if [[ -z "$1" ]] || [[ -z "$2" ]]; then
        echo -e "${RED}Error: HDF mode requires both input and output HDF files${NC}"
        print_usage
        exit 1
    fi
    
    INPUT_HDF="$1"
    OUTPUT_HDF="$2"
    HTML_DIR="${3:-$DEFAULT_HTML_DIR}"
    
    echo -e "${GREEN}=== Interactive Plot Tool - HDF Pair Mode ===${NC}"
    echo "Input HDF:  $INPUT_HDF"
    echo "Output HDF: $OUTPUT_HDF"
    echo "HTML Dir:   $HTML_DIR"
    echo "Cluster:    $CLUSTER"
    echo "Log Dir:    $LOG_DIR"
    
    # Verify files exist
    if [[ ! -f "$INPUT_HDF" ]]; then
        echo -e "${RED}Error: Input HDF file not found: $INPUT_HDF${NC}"
        exit 1
    fi
    if [[ ! -f "$OUTPUT_HDF" ]]; then
        echo -e "${RED}Error: Output HDF file not found: $OUTPUT_HDF${NC}"
        exit 1
    fi
    
    BIND_MOUNTS=$(get_bind_mounts)
    SINGULARITY_CMD="singularity run $BIND_MOUNTS $SIMG_FILE \"$INPUT_HDF\" \"$OUTPUT_HDF\" \"$HTML_DIR\""
    
else
    # JSON Mode (default)
    MODE="json"
    
    if [[ -z "$1" ]] || [[ -z "$2" ]]; then
        echo -e "${RED}Error: JSON mode requires config.xml and inputs.json${NC}"
        print_usage
        exit 1
    fi
    
    CONFIG_XML="$1"
    INPUTS_JSON="$2"
    HTML_DIR="${3:-$DEFAULT_HTML_DIR}"
    
    echo -e "${GREEN}=== Interactive Plot Tool - JSON Mode ===${NC}"
    echo "Config XML:  $CONFIG_XML"
    echo "Inputs JSON: $INPUTS_JSON"
    echo "HTML Dir:    $HTML_DIR"
    echo "Cluster:     $CLUSTER"
    echo "Log Dir:     $LOG_DIR"
    
    # Verify files exist
    if [[ ! -f "$CONFIG_XML" ]]; then
        echo -e "${YELLOW}Warning: Config XML file not found: $CONFIG_XML${NC}"
    fi
    if [[ ! -f "$INPUTS_JSON" ]]; then
        echo -e "${RED}Error: Inputs JSON file not found: $INPUTS_JSON${NC}"
        exit 1
    fi
    
    BIND_MOUNTS=$(get_bind_mounts)
    SINGULARITY_CMD="singularity run $BIND_MOUNTS $SIMG_FILE \"$CONFIG_XML\" \"$INPUTS_JSON\" \"$HTML_DIR\""
fi

# Create output directory
mkdir -p "$HTML_DIR"

# Count pairs for progress tracking
if [[ "$MODE" == "json" ]]; then
    TOTAL_PAIRS=$(python3 -c "import json; data=json.load(open('$INPUTS_JSON')); print(len(data.get('InputHDF', data) if isinstance(data, dict) else len(data)))" 2>/dev/null || echo "?")
else
    TOTAL_PAIRS=1
fi

# Local mode execution
if [[ "$LOCAL_MODE" == true ]]; then
    echo ""
    echo -e "${BLUE}Running in LOCAL mode with progress tracking...${NC}"
    echo "Total pairs: $TOTAL_PAIRS"
    
    # Detect runtime
    if command -v apptainer >/dev/null 2>&1; then
        RUNTIME=apptainer
    elif command -v singularity >/dev/null 2>&1; then
        RUNTIME=singularity
    else
        RUNTIME=""
    fi
    
    START_TIME=$(date +%s)
    echo "Started at: $(date)"
    echo ""
    
    CURRENT=0
    
    # Run with or without container
    if [[ -n "$RUNTIME" ]] && [[ -f "$SIMG_FILE" ]]; then
        BIND_MOUNTS=$(get_bind_mounts)
        echo "Using binds: $BIND_MOUNTS"
        echo ""
        if [[ "$MODE" == "hdf" ]]; then
            $RUNTIME run $BIND_MOUNTS "$SIMG_FILE" "$INPUT_HDF" "$OUTPUT_HDF" "$HTML_DIR" 2>&1 | \
                while IFS= read -r line; do
                    echo "$line"
                    if [[ "$line" == *"Processing"* ]] || [[ "$line" == *"→"* ]]; then
                        ((CURRENT++)) || CURRENT=1
                        if [[ "$TOTAL_PAIRS" != "?" ]]; then
                            show_progress $CURRENT $TOTAL_PAIRS $START_TIME
                            echo ""
                        fi
                    fi
                done
        else
            $RUNTIME run $BIND_MOUNTS "$SIMG_FILE" "$CONFIG_XML" "$INPUTS_JSON" "$HTML_DIR" 2>&1 | \
                while IFS= read -r line; do
                    echo "$line"
                    if [[ "$line" == *"Processing"* ]] || [[ "$line" == *"→"* ]]; then
                        ((CURRENT++)) || CURRENT=1
                        if [[ "$TOTAL_PAIRS" != "?" ]]; then
                            show_progress $CURRENT $TOTAL_PAIRS $START_TIME
                            echo ""
                        fi
                    fi
                done
        fi
    else
        echo -e "${YELLOW}No container runtime found. Running Python directly...${NC}"
        cd "$SCRIPT_DIR/../tools"
        python3 -m InteractivePlot.e_presentation_layer.main_front_end \
            "$CONFIG_XML" "$INPUTS_JSON" "$HTML_DIR" 2>&1 | \
            while IFS= read -r line; do
                echo "$line"
                if [[ "$line" == *"Processing"* ]]; then
                    ((CURRENT++)) || CURRENT=1
                    if [[ "$TOTAL_PAIRS" != "?" ]]; then
                        show_progress $CURRENT $TOTAL_PAIRS $START_TIME
                        echo ""
                    fi
                fi
            done
    fi
    
    END_TIME=$(date +%s)
    TOTAL_TIME=$((END_TIME - START_TIME))
    TOTAL_MIN=$((TOTAL_TIME / 60))
    TOTAL_SEC=$((TOTAL_TIME % 60))
    
    echo ""
    echo -e "${GREEN}========================================${NC}"
    echo -e "${GREEN}Interactive Plot processing completed!${NC}"
    echo "Total time: ${TOTAL_MIN}m ${TOTAL_SEC}s"
    echo "Output directory: $HTML_DIR"
    echo -e "${GREEN}========================================${NC}"
    exit 0
fi

# SLURM mode execution (allocate resources)
if ! command -v srun >/dev/null 2>&1; then
    echo -e "${RED}Error: 'srun' not found; cannot allocate Slurm resources from this environment.${NC}"
    echo "Tip: re-run with --local to execute directly."
    exit 1
fi

# Check cluster resources and adjust if needed
check_cluster_resources "$MEMORY" "$CPUS"

# Build the command that will run inside the job (includes runtime info printing)
RUNTIME_INFO_CMD='echo "============================================================"; echo "  RUNTIME ENVIRONMENT INFO"; echo "============================================================"; echo "Hostname:     $(hostname)"; echo "Date:         $(date)"; echo "User:         $USER"; echo "Working Dir:  $(pwd)"; free -h 2>/dev/null || true; echo "CPU Cores:    $(nproc)"; echo "SLURM Job ID: $SLURM_JOB_ID"; echo "SLURM Node:   $SLURM_NODELIST"; echo "SLURM CPUs:   $SLURM_CPUS_ON_NODE"; echo "SLURM Mem:    ${SLURM_MEM_PER_NODE:-unknown} MB"; echo "============================================================"'

echo ""
echo "Running via srun (allocating resources)..."
echo "  Partition:      $PARTITION"
echo "  Memory:         $MEMORY"
echo "  CPUs:           $CPUS"
echo "  Time:           $TIME_LIMIT"
echo "  Excluded Nodes: $EXCLUDED_NODES"
echo ""

# Execute with srun. Output is written to LOG_DIR with Slurm %j substitution.
JOB_ID=$(srun --parsable --exclusive -N 1 -n 1 \
    --partition="$PARTITION" \
    --mem="$MEMORY" \
    --cpus-per-task="$CPUS" \
    --time="$TIME_LIMIT" \
    --exclude="$EXCLUDED_NODES" \
    --output="$LOG_DIR/interactive_plot_%j.out" \
    --error="$LOG_DIR/interactive_plot_%j.err" \
    --job-name="interactive_plot" \
    bash -lc "$RUNTIME_INFO_CMD; if type module >/dev/null 2>&1; then module load slurm >/dev/null 2>&1 || true; module load $SINGULARITY_MODULE >/dev/null 2>&1 || true; fi; $SINGULARITY_CMD")
RC=$?

if [[ $RC -ne 0 ]]; then
    echo -e "${RED}srun failed with exit code $RC${NC}"
    echo "See logs in: $LOG_DIR (interactive_plot_${JOB_ID}.out/.err)"
    exit $RC
fi

echo -e "${GREEN}Job completed successfully!${NC}"
echo "  Job ID: $JOB_ID"
echo "  Logs:   $LOG_DIR/interactive_plot_${JOB_ID}.out (.err)"
