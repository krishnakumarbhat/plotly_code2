#!/bin/sh

# This script prepares the files needed by the Lauterbach by copying them to a
# desired Windows-accessible directory.
#
# For example, this command will copy the binaries, source files, and
# lauterbach scripts for FLR7 to /mnt/c/wksp/SAF85xx_deploy where they can be
# accessed by T32 in Windows:
#  ./prepare_deploy.sh -v flr7 /mnt/c/wksp/SAF85xx_deploy

set -e
set -u
set -C

# global variables
SCRIPT_NAME="$(basename $0)"
SCRIPT_PATH="$(dirname $0)"
BZL_EXECROOT="bazel-Core_Radar_Gen7_SAF85xx"
VARIANT="srr7p"
COPY_BINS=1
COPY_SCRIPTS=1
COPY_SOURCES=0

EXTRA_BINS="pbl quickflash"
BIN_BASEPATH="bazel-out/k8-fastbuild/bin/outputs"
SOURCES_LIST="software external"

usage() {
  echo "Usage: ${SCRIPT_NAME} [options] [--] DESTINATION_PATH

Copy bazel output files to a Windows-accessible destination path.

Options:
  -h, --help             Display this help text.
  -v, --variant VAR      Set variant. Options are flr7, srr7p, srr7e, or all.
                         Default is srr7p.
  -b, --bazel-root PATH  Define the workspace exec root as PATH. By default,
                         the script will try to search for ${BZL_EXECROOT}
  -B, --no-bins          Don't copy the binary files
  -L, --no-scripts       Don't copy the Lauterbach scripts
  -s, --sources          Copy the source files (experimental, not recommended)
"
}

resolve_execroot() {
  local i

  if [ ! -d "${BZL_EXECROOT}" ]; then
    if [ -d "../${BZL_EXECROOT}" ]; then
      BZL_EXECROOT="../${BZL_EXECROOT}"
    elif [ -d "../../${BZL_EXECROOT}" ]; then
      BZL_EXECROOT="../../${BZL_EXECROOT}"
    else
      echo "${SCRIPT_NAME}: Error: Unable to find ${BZL_EXECROOT}"
      echo "Please use -b option to provide the path to the bazel exec root"
      return 1
    fi
  fi

  if [ $COPY_BINS -eq 1 ]; then
    if [ ! -d "${BZL_EXECROOT}/${BIN_BASEPATH}" ]; then
      echo "${SCRIPT_NAME}: Error: Unable to find ${BIN_BASEPATH} in bazel root"
      return 2
    fi
    if [ "${VARIANT}" != "all" ]; then
      if [ ! -d "${BZL_EXECROOT}/${BIN_BASEPATH}/${VARIANT}" ]; then
        echo "${SCRIPT_NAME}: Error: Unable to find ${VARIANT} in ${BIN_BASEPATH}"
        return 2
      fi
    fi
  fi

  if [ $COPY_SOURCES -eq 1 ]; then
    for i in ${SOURCES_LIST}; do
      if [ ! -d "${BZL_EXECROOT}/${i}" ]; then
        echo "${SCRIPT_NAME}: Error: Unable to find ${i} in bazel root"
        return 2
      fi
    done
  fi

  return 0
}

do_copy_bins() {
  local outputs
  local i
  outputs="$(basename "${BIN_BASEPATH}")"

  if [ -z "${outputs}" ]; then
    echo "${SCRIPT_NAME}: Error: ${BIN_BASEPATH} doesn't seem to be valid"
    return 3
  fi

  if [ -e "$1/${outputs}/" ]; then
    echo "${SCRIPT_NAME}: Warning: deleting $1/${outputs}"
    rm -rf "$1/${outputs}"
  fi

  if [ "${VARIANT}" = "all" ]; then
    cp -R -L "${BZL_EXECROOT}/${BIN_BASEPATH}" "$1/"
  else
    mkdir -p "$1/$outputs"
    cp -R -L "${BZL_EXECROOT}/${BIN_BASEPATH}/${VARIANT}" "$1/${outputs}/"

    for i in ${EXTRA_BINS}; do
      cp -R -L "${BZL_EXECROOT}/${BIN_BASEPATH}/${i}" "$1/${outputs}/"
    done
  fi
}

do_copy_scripts() {
  local outputs
  local i
  outputs="lauterbach"

  if [ ! -e "${SCRIPT_PATH}/${SCRIPT_NAME}" ]; then
    echo "${SCRIPT_NAME}: Error: ${SCRIPT_PATH} doesn't look like the script location"
    return 4
  fi

  if [ -z "${outputs}" ]; then
    echo "${SCRIPT_NAME}: Error: something went wrong -- ${outputs}"
    return 4
  fi

  if [ -e "$1/${outputs}/" ]; then
    echo "${SCRIPT_NAME}: Warning: deleting $1/${outputs}"
    rm -rf "$1/${outputs}"
  fi

  mkdir -p "$1/$outputs"
  cp -R -L "${SCRIPT_PATH}/." "$1/${outputs}/"
}

do_copy_sources() {
  local i

  for i in ${SOURCES_LIST}; do
    if [ -n "${i}" -a -n "$1" -a -e "$1/${i}/" ]; then
      echo "${SCRIPT_NAME}: Warning: deleting $1/${i}"
      rm -rf "$1/${i}"
    fi
    cp -R -L "${BZL_EXECROOT}/${i}" "$1/"
  done
}

do_copy() {
  #echo "destination path=$1 bazel root=${BZL_EXECROOT}"

  if [ ! -d "$1" ]; then
    mkdir -p "$1"
    if [ $? -ne 0 ]; then
      echo "${SCRIPT_NAME}: Error: unable to create $1"
      return 1
    fi
  fi

  if [ -z "$(find "$1" -type d -empty)" ]; then
    echo "${SCRIPT_NAME}: Warning: $1 is not empty"
  fi

  if [ $COPY_BINS -eq 1 ]; then
    do_copy_bins "$1"
  fi

  if [ $COPY_SCRIPTS -eq 1 ]; then
    do_copy_scripts "$1"
  fi

  if [ $COPY_SOURCES -eq 1 ]; then
    do_copy_sources "$1"
  fi
}

script_main() {
  local optstring
  local longopt
  local optresult
  local destination

  optstring="hv:b:BLs"
  longopt="help,variant:,bazel-root:,no-bins,no-scripts,sources"

  optresult=$(/usr/bin/getopt -o "${optstring}" --l "${longopt}" -n "${SCRIPT_NAME}" -- "$@")
  eval set -- "${optresult}"

  while [ $# -gt 0 ]; do
    case "$1" in
      -h | --help)
        shift
        usage
        return 0
        ;;
      -v | --variant)
        shift
        VARIANT="$1"
        shift
        continue
        ;;
      -b | --bazel-root)
        shift
        BZL_EXECROOT="$1"
        shift
        continue
        ;;
      -B | --no-bins)
        shift
        COPY_BINS=0
        continue
        ;;
      -L | --no-scripts)
        shift
        COPY_SCRIPTS=0
        continue
        ;;
      -s | --sources)
        shift
        COPY_SOURCES=1
        continue
        ;;
      --)
        shift
        break
        ;;
      *)
        echo "$1"
        shift
        ;;
    esac
  done

  if [ $# -gt 1 ]; then
    echo "Ambiguous destination path:"
    echo $@
    echo "
Please use quotes if there is a space in your path name or check for multiple
paths provided on the command line."
    return 1
  elif [ $# -eq 1 ]; then
    destination="$1"
    shift
  else
    echo "${SCRIPT_NAME}: Error, no destination path provided"
    return 1
  fi

  resolve_execroot
  if [ $? -ne 0 ]; then
    return $?
  fi

  do_copy "${destination}"
}

script_main "$@"
