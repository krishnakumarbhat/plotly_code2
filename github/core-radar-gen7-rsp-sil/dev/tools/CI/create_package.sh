#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

echo "Advradar Gen7 RSP SIL release package start"

ZIP_NAME=${1:-}
if [ "$ZIP_NAME" == "" ]; then
  echo "Usage: $0 <zip_name>"
  exit 1
fi

echo "Creating zip archive"
find . -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.inc' -o -name '*.S' -o -name 'BUILD' -o -name 'WORKSPACE' -o -name 'version.yaml' -o -name '*.bzl' -o -name '*.cc' \) | zip -r "$ZIP_NAME" -@
echo "Package created: $ZIP_NAME"
