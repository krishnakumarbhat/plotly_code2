#!/usr/bin/env bash
set -euo pipefail

# ==============================================================================
# Build All Singularity/Apptainer Images for All Services
# ==============================================================================
# This script builds 4 .simg files:
#   1. kpi.simg           - KPI Tool
#   2. interactive_plot.simg - Interactive Plot Tool
#   3. dc_html.simg       - DC HTML Report Tool
#   4. all_services.simg  - Main UI (optional, if Singularity.def exists)
#
# Run inside WSL:
#   cd /mnt/c/git/Core_RESIM_KPI/all_services
#   bash scripts/wsl_build_all_images.sh
#
# Options:
#   --kpi-only           Build only KPI image
#   --interactive-only   Build only Interactive Plot image
#   --dc-html-only       Build only DC HTML image
#   --main-only          Build only main all_services image
#   --no-main            Skip building main all_services image
#   --sync <user@host:/path>  Sync images to remote cluster after build
# ==============================================================================

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

# Image definitions
KPI_DEF="tools/KPI/Singularity_KPI.def"
INTERACTIVE_DEF="tools/InteractivePlot/Singularity_InteractivePlot.def"
DC_HTML_DEF="tools/dc_html/Singularity_DC_HTML.def"
MAIN_DEF="Singularity.def"

# Output images
SIMG_DIR="simg"
KPI_IMAGE="${SIMG_DIR}/kpi.simg"
INTERACTIVE_IMAGE="${SIMG_DIR}/interactive_plot.simg"
DC_HTML_IMAGE="${SIMG_DIR}/dc_html.simg"
MAIN_IMAGE="${SIMG_DIR}/all_services.simg"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Flags
BUILD_KPI=true
BUILD_INTERACTIVE=true
BUILD_DC_HTML=true
BUILD_MAIN=true
SYNC_DEST=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --kpi-only)
            BUILD_KPI=true
            BUILD_INTERACTIVE=false
            BUILD_DC_HTML=false
            BUILD_MAIN=false
            shift
            ;;
        --interactive-only)
            BUILD_KPI=false
            BUILD_INTERACTIVE=true
            BUILD_DC_HTML=false
            BUILD_MAIN=false
            shift
            ;;
        --dc-html-only)
            BUILD_KPI=false
            BUILD_INTERACTIVE=false
            BUILD_DC_HTML=true
            BUILD_MAIN=false
            shift
            ;;
        --main-only)
            BUILD_KPI=false
            BUILD_INTERACTIVE=false
            BUILD_DC_HTML=false
            BUILD_MAIN=true
            shift
            ;;
        --no-main)
            BUILD_MAIN=false
            shift
            ;;
        --sync)
            SYNC_DEST="$2"
            shift 2
            ;;
        --help|-h)
            echo "Build All Singularity Images"
            echo ""
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  --kpi-only           Build only KPI image"
            echo "  --interactive-only   Build only Interactive Plot image"
            echo "  --dc-html-only       Build only DC HTML image"
            echo "  --main-only          Build only main all_services image"
            echo "  --no-main            Skip building main all_services image"
            echo "  --sync <dest>        Sync images to remote after build"
            echo "  --help, -h           Show this help"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Detect runtime
if command -v apptainer >/dev/null 2>&1; then
    RUNTIME=apptainer
elif command -v singularity >/dev/null 2>&1; then
    RUNTIME=singularity
else
    echo -e "${RED}ERROR: neither 'apptainer' nor 'singularity' found in WSL.${NC}"
    echo "Ubuntu/Debian WSL install (recommended):"
    echo "  sudo apt-get update && sudo apt-get install -y apptainer"
    exit 1
fi

echo -e "${CYAN}==========================================${NC}"
echo -e "${CYAN}  Building Singularity Images${NC}"
echo -e "${CYAN}==========================================${NC}"
echo "Runtime: $RUNTIME"
echo "Root:    $ROOT_DIR"
echo ""

# Create simg directory
mkdir -p "$SIMG_DIR"

# Track build times
declare -A BUILD_TIMES
TOTAL_START=$(date +%s)

# Progress tracking function
show_build_progress() {
    local name=$1
    local status=$2
    local time=$3
    
    if [[ "$status" == "building" ]]; then
        echo -e "${YELLOW}[BUILDING]${NC} $name..."
    elif [[ "$status" == "success" ]]; then
        echo -e "${GREEN}[SUCCESS]${NC} $name (${time}s)"
    elif [[ "$status" == "skipped" ]]; then
        echo -e "${BLUE}[SKIPPED]${NC} $name"
    elif [[ "$status" == "failed" ]]; then
        echo -e "${RED}[FAILED]${NC} $name"
    fi
}

build_image() {
    local def_file=$1
    local image_file=$2
    local build_dir=$3
    local name=$4
    
    local abs_image="$ROOT_DIR/$image_file"
    
    if [[ ! -f "$def_file" ]]; then
        echo -e "${YELLOW}Warning: Definition file not found: $def_file${NC}"
        show_build_progress "$name" "skipped" 0
        return 1
    fi
    
    show_build_progress "$name" "building" 0
    local start_time=$(date +%s)
    
    # IMPORTANT: build from repo root so %files entries like "tools/..." resolve correctly.
    # The definition files in this repo reference paths from project root.
    if [ "$(id -u)" -ne 0 ]; then
        ( cd "$ROOT_DIR" && sudo "$RUNTIME" build --force "$abs_image" "$def_file" )
    else
        ( cd "$ROOT_DIR" && "$RUNTIME" build --force "$abs_image" "$def_file" )
    fi
    
    local exit_code=$?
    local end_time=$(date +%s)
    local elapsed=$((end_time - start_time))
    
    if [[ $exit_code -eq 0 ]]; then
        show_build_progress "$name" "success" $elapsed
        BUILD_TIMES[$name]=$elapsed
        return 0
    else
        show_build_progress "$name" "failed" $elapsed
        return 1
    fi
}

# Build images
echo -e "${CYAN}Building images...${NC}"
echo ""

FAILED=0

# 1. Build KPI
if [[ "$BUILD_KPI" == true ]]; then
    if build_image "$KPI_DEF" "$KPI_IMAGE" "$ROOT_DIR/tools/KPI" "KPI Tool"; then
        echo ""
    else
        ((FAILED++))
    fi
fi

# 2. Build Interactive Plot
if [[ "$BUILD_INTERACTIVE" == true ]]; then
    if build_image "$INTERACTIVE_DEF" "$INTERACTIVE_IMAGE" "$ROOT_DIR/tools/InteractivePlot" "Interactive Plot"; then
        echo ""
    else
        ((FAILED++))
    fi
fi

# 3. Build DC HTML
if [[ "$BUILD_DC_HTML" == true ]]; then
    if build_image "$DC_HTML_DEF" "$DC_HTML_IMAGE" "$ROOT_DIR/tools/dc_html" "DC HTML Tool"; then
        echo ""
    else
        ((FAILED++))
    fi
fi

# 4. Build Main All Services
if [[ "$BUILD_MAIN" == true ]]; then
    if [[ -f "$MAIN_DEF" ]]; then
        if build_image "$MAIN_DEF" "$MAIN_IMAGE" "$ROOT_DIR" "All Services (Main UI)"; then
            echo ""
        else
            ((FAILED++))
        fi
    else
        echo -e "${YELLOW}Skipping main image: $MAIN_DEF not found${NC}"
    fi
fi

# Summary
TOTAL_END=$(date +%s)
TOTAL_TIME=$((TOTAL_END - TOTAL_START))
TOTAL_MIN=$((TOTAL_TIME / 60))
TOTAL_SEC=$((TOTAL_TIME % 60))

echo ""
echo -e "${CYAN}==========================================${NC}"
echo -e "${CYAN}  Build Summary${NC}"
echo -e "${CYAN}==========================================${NC}"
echo "Total build time: ${TOTAL_MIN}m ${TOTAL_SEC}s"
echo ""

# List built images
echo "Built images:"
for img in "$KPI_IMAGE" "$INTERACTIVE_IMAGE" "$DC_HTML_IMAGE" "$MAIN_IMAGE"; do
    if [[ -f "$ROOT_DIR/$img" ]]; then
        size=$(du -h "$ROOT_DIR/$img" | cut -f1)
        echo -e "  ${GREEN}✓${NC} $img ($size)"
    fi
done

if [[ $FAILED -gt 0 ]]; then
    echo ""
    echo -e "${RED}Failed builds: $FAILED${NC}"
fi

# Sync to remote if requested
if [[ -n "$SYNC_DEST" ]]; then
    echo ""
    echo -e "${CYAN}Syncing images to: $SYNC_DEST${NC}"
    
    for img in "$KPI_IMAGE" "$INTERACTIVE_IMAGE" "$DC_HTML_IMAGE" "$MAIN_IMAGE"; do
        if [[ -f "$ROOT_DIR/$img" ]]; then
            echo "  Copying $img..."
            scp "$ROOT_DIR/$img" "$SYNC_DEST/"
        fi
    done
    
    echo -e "${GREEN}Sync complete!${NC}"
fi

echo ""
echo -e "${GREEN}Build complete!${NC}"
echo ""
echo "Usage examples:"
echo "  # Run KPI on SLURM"
echo "  ./simg/run_kpi.sh json KPIPlot.json /output"
echo ""
echo "  # Run Interactive Plot on SLURM"
echo "  ./simg/run_interactive_plot.sh config.xml inputs.json /output"
echo ""
echo "  # Run DC HTML on SLURM"
echo "  ./simg/run_dc_html.sh HTMLConfig.xml Inputs.json /output"
echo ""
echo "  # Run DC HTML locally with progress"
echo "  ./simg/run_dc_html.sh --local HTMLConfig.xml Inputs.json /output"
