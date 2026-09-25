#!/usr/bin/env bash

set -u
set -o pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$SCRIPT_DIR/CMake"
BUILD_DIR="$SCRIPT_DIR/build/linux_make_x64"
GENERATOR="Unix Makefiles"
CONFIG="Release"

usage() {
  cat <<'EOF'
Usage:
  ./Build.sh
    Opens interactive numeric menu.

  ./Build.sh 0
    Print this usage.

  ./Build.sh 1 [1|2]
    Configure and build full workspace. Config: 1=Release, 2=Debug.

  ./Build.sh 2 [1|2]
    Configure and build BordNetDecoder only. Config: 1=Release, 2=Debug.

  ./Build.sh 3
    Remove generated build directory.

  ./Build.sh 4 [1|2] [1|2]
    Clean, configure, and build. Third arg selects target: 1=all (default), 2=app.
EOF
}

normalize_config_code() {
  local raw="$1"
  case "$raw" in
    1) CONFIG="Release" ;;
    2) CONFIG="Debug" ;;
    *)
      echo "[ERROR] Invalid configuration code: $raw"
      echo "        Allowed values: 1 (Release) or 2 (Debug)"
      return 1
      ;;
  esac
}

ask_config() {
  local cfgcode
  echo
  read -r -p "Select configuration [1=Release / 2=Debug, default=1]: " cfgcode
  if [[ -z "$cfgcode" ]]; then
    CONFIG="Release"
    return 0
  fi
  normalize_config_code "$cfgcode"
}

get_jobs() {
  if command -v nproc >/dev/null 2>&1; then
    nproc
    return
  fi

  if command -v getconf >/dev/null 2>&1; then
    getconf _NPROCESSORS_ONLN
    return
  fi

  echo 1
}

precheck() {
  if [[ ! -f "$SOURCE_DIR/CMakeLists.txt" ]]; then
    echo "[ERROR] CMake entry not found: $SOURCE_DIR/CMakeLists.txt"
    return 1
  fi

  if ! command -v cmake >/dev/null 2>&1; then
    echo "[ERROR] CMake not found in PATH."
    echo "        Install CMake and ensure it is available in your shell."
    return 1
  fi

  if ! command -v make >/dev/null 2>&1; then
    echo "[ERROR] GNU make not found in PATH."
    echo "        Install build-essential (or equivalent) and retry."
    return 1
  fi

  local cmake_version cmake_major
  cmake_version="$(cmake --version | awk '/^cmake version / {print $3; exit}')"
  if [[ -z "$cmake_version" ]]; then
    echo "[ERROR] Unable to detect CMake version."
    return 1
  fi

  cmake_major="${cmake_version%%.*}"
  if [[ -z "$cmake_major" || ! "$cmake_major" =~ ^[0-9]+$ ]]; then
    echo "[ERROR] Unable to parse CMake version: $cmake_version"
    return 1
  fi

  if (( cmake_major < 4 )); then
    echo "[ERROR] Detected CMake $cmake_version, but this repo requires CMake 4.0 or newer."
    echo "        Requirement comes from CMake/CMakeLists.txt: cmake_minimum_required(VERSION 4.0)."
    return 1
  fi
}

clean_build_dir() {
  if [[ -d "$BUILD_DIR" ]]; then
    echo "[INFO] Removing build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
    if [[ -d "$BUILD_DIR" ]]; then
      echo "[ERROR] Could not fully remove build directory."
      echo "        One or more files may still be locked or permission-restricted."
      return 1
    fi
  fi

  echo "[INFO] Clean complete."
}

configure_and_build() {
  local target="$1"
  local jobs

  precheck || return 1

  echo "[INFO] Configuring with $GENERATOR"
  if ! cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE="$CONFIG"; then
    echo "[ERROR] CMake configure failed."
    return 1
  fi

  jobs="$(get_jobs)"
  if [[ -n "$target" ]]; then
    echo "[INFO] Building target $target in $CONFIG mode"
    if ! cmake --build "$BUILD_DIR" --target "$target" -- -j"$jobs"; then
      echo "[ERROR] Build failed for target $target."
      return 1
    fi
  else
    echo "[INFO] Building full workspace in $CONFIG mode"
    if ! cmake --build "$BUILD_DIR" -- -j"$jobs"; then
      echo "[ERROR] Full build failed."
      return 1
    fi
  fi

  echo "[INFO] Build finished successfully."
}

menu() {
  local choice

  echo
  echo "=============================================="
  echo "         BordNet Tool Build Script (Linux)"
  echo "=============================================="
  echo "Generator: $GENERATOR"
  echo "Build Dir: $BUILD_DIR"
  echo "Default Config: $CONFIG"
  echo
  echo "This script can build:"
  echo "  1. Full workspace"
  echo "  2. BordNetDecoder only"
  echo "  3. Clean build directory"
  echo "  4. Rebuild full workspace"
  echo "  0. Help / usage"
  echo
  read -r -p "Please select one option [0/1/2/3/4]: " choice

  case "$choice" in
    0)
      usage
      ;;
    1)
      ask_config || return 1
      configure_and_build "" || return 1
      echo "[INFO] Done."
      ;;
    2)
      ask_config || return 1
      configure_and_build "BordNetDecoder" || return 1
      echo "[INFO] Done."
      ;;
    3)
      clean_build_dir || return 1
      echo "[INFO] Done."
      ;;
    4)
      ask_config || return 1
      clean_build_dir || return 1
      configure_and_build "" || return 1
      echo "[INFO] Done."
      ;;
    *)
      echo "[ERROR] Invalid selection."
      menu
      ;;
  esac
}

main() {
  local mode_code="${1:-}"
  local target_code="${3:-}"
  local target=""

  if [[ -z "$mode_code" ]]; then
    menu
    return $?
  fi

  if [[ "$mode_code" == "0" ]]; then
    usage
    return 0
  fi

  if [[ -n "${2:-}" ]]; then
    normalize_config_code "$2" || return 1
  fi

  case "$mode_code" in
    1)
      configure_and_build "" || return 1
      echo "[INFO] Done."
      ;;
    2)
      configure_and_build "BordNetDecoder" || return 1
      echo "[INFO] Done."
      ;;
    3)
      clean_build_dir || return 1
      echo "[INFO] Done."
      ;;
    4)
      target=""
      if [[ "$target_code" == "2" ]]; then
        target="BordNetDecoder"
      elif [[ -n "$target_code" && "$target_code" != "1" ]]; then
        echo "[ERROR] Invalid target selector: $target_code"
        echo "        Allowed values for third argument: 1 (all) or 2 (app)"
        return 1
      fi
      clean_build_dir || return 1
      configure_and_build "$target" || return 1
      echo "[INFO] Done."
      ;;
    *)
      echo "[ERROR] Unknown mode code: $mode_code"
      usage
      return 1
      ;;
  esac
}

main "$@"
