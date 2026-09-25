#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

echo "Core Radar SPBB release package start."

ZIP_NAME=${1:-}
if [ "$ZIP_NAME" == "" ]; then
  echo "Usage: $0 <zip_name>"
  exit 1
fi

if [ -f MODULE.bazel ]; then
  # Extract fields from version.yaml

  MAJOR="$(yq -r '.MajorVersion // empty' version.yaml 2>/dev/null \
  || awk -F': ' '/^[[:space:]]*MajorVersion[[:space:]]*:/{
       v=$2; sub(/#.*/,"",v); gsub(/^[ \t"]+|[ \t"]+$/, "", v); print v
     }' version.yaml)"

  MINOR="$(yq -r '.MinorVersion // empty' version.yaml 2>/dev/null \
    || awk -F': ' '/^[[:space:]]*MinorVersion[[:space:]]*:/{
        v=$2; sub(/#.*/,"",v); gsub(/^[ \t"]+|[ \t"]+$/, "", v); print v
      }' version.yaml)"

  PATCH="$(yq -r '.PatchVersion // empty' version.yaml 2>/dev/null \
    || awk -F': ' '/^[[:space:]]*PatchVersion[[:space:]]*:/{
        v=$2; sub(/#.*/,"",v); gsub(/^[ \t"]+|[ \t"]+$/, "", v); print v
      }' version.yaml)"

  VERSION="${MAJOR:-0}.${MINOR:-0}.${PATCH:-0}"

  if [[ -z "$MAJOR" || -z "$MINOR" || -z "$PATCH" ]]; then
    echo "ERROR: Failed to extract version fields from version.yaml" >&2
    exit 2
  fi

  echo "Updating MODULE.bazel version with $VERSION..."

  # Replace version line, preserving indentation
  sed -E 's/^([[:space:]]*)version = ".*",/\1version = "'"$VERSION"'",/' MODULE.bazel > MODULE.bazel.tmp && mv MODULE.bazel.tmp MODULE.bazel
else
  echo "MODULE.bazel not found, skipping version update."
fi

echo "Creating zip archive"

find . -type f \( -name '*.c' -o -name '*.h' -o -name '*.inc' -o -name '*.bazel' -o -name '*.S' -o -name '*.m' -o -name 'BUILD' -o -name 'changelog.md' -o -name 'WORKSPACE' -o -name 'version.yaml' -o -name '*.bzl' -o -name '*.cpp' -o -name '*.cc' \) | zip -r "$ZIP_NAME" -@

echo "Package created: $ZIP_NAME"
