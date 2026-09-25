#!/usr/bin/env bash

set -euo pipefail

JFROG_URL="${JFROG_BINARIES_URL:-https://jfrog.asux.aptiv.com/artifactory/core_resim-gpo-10055799-aptiv_resim_release_ext/ADCAM_SIL/}"
OUTPUT_DIR="${JFROG_OUTPUT_DIR:-./jfrog_binaries}"
JFROG_VERSION="${JFROG_VERSION:-}"

usage() {
    cat <<EOF
Usage: $(basename "$0") [output-directory]

Fetch files from the latest version folder below:
  $JFROG_URL

Authentication:
  Set JFROG_ACCESS_TOKEN to use Bearer token authentication.
    Alternatively set JFROG_USER and JFROG_PASSWORD for Basic authentication.
    Set JFROG_VERSION (for example, v7) to fetch a specific version folder.
  Set JFROG_OUTPUT_DIR or pass an output directory to change the destination.
EOF
}

if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
    usage
    exit 0
fi

if [[ $# -gt 1 ]]; then
    echo "Error: expected zero or one output-directory argument." >&2
    usage >&2
    exit 2
fi

if [[ $# == 1 ]]; then
    OUTPUT_DIR="$1"
fi

command -v curl >/dev/null 2>&1 || {
    echo "Error: curl is required." >&2
    exit 1
}
command -v python3 >/dev/null 2>&1 || {
    echo "Error: python3 is required to read the Artifactory listing." >&2
    exit 1
}
command -v unzip >/dev/null 2>&1 || {
    echo "Error: unzip is required to extract downloaded binaries." >&2
    exit 1
}

TEMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TEMP_DIR"' EXIT

LISTING_URL="${JFROG_URL%/}"
LISTING_URL="${LISTING_URL/\/artifactory\//\/artifactory\/api\/storage\/}"
LISTING_URL="${LISTING_URL}?list&deep=1"
LISTING_FILE="$TEMP_DIR/listing.json"

curl_args=(--fail --location --silent --show-error)
if [[ -n "${JFROG_ACCESS_TOKEN:-}" ]]; then
    curl_args+=(--header "Authorization: Bearer $JFROG_ACCESS_TOKEN")
elif [[ -n "${JFROG_USER:-}" && -n "${JFROG_PASSWORD:-}" ]]; then
    curl_args+=(--user "${JFROG_USER}:${JFROG_PASSWORD}")
fi

echo "Reading Artifactory listing from $JFROG_URL"
curl "${curl_args[@]}" "$LISTING_URL" --output "$LISTING_FILE"

mapfile -t files < <(python3 - "$LISTING_FILE" "$JFROG_VERSION" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as listing_file:
    listing = json.load(listing_file)

requested_version = sys.argv[2]
items = [item for item in listing.get("files", []) if not item.get("folder", False)]
if not items:
    raise SystemExit("Artifactory returned no files")

if requested_version:
    version = requested_version.strip("/")
else:
    versions = {}
    for item in items:
        parts = item.get("uri", "").lstrip("/").split("/", 1)
        if len(parts) == 2:
            versions[parts[0]] = max(versions.get(parts[0], ""), item.get("lastModified", ""))
    if not versions:
        raise SystemExit("Artifactory files are not inside a version folder")
    version = max(versions, key=versions.get)

selected = [item.get("uri", "").lstrip("/") for item in items
            if item.get("uri", "").lstrip("/").startswith(version + "/")]
if not selected:
    raise SystemExit(f"No files found for version folder: {version}")

print(version)
print("\n".join(selected))
PY
)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "Error: Artifactory returned no files." >&2
    exit 1
fi

LATEST_VERSION="${files[0]}"
files=("${files[@]:1}")
echo "Selected version folder: $LATEST_VERSION"

mkdir -p "$OUTPUT_DIR"
for relative_path in "${files[@]}"; do
    destination="$OUTPUT_DIR/$relative_path"
    mkdir -p "$(dirname "$destination")"
    echo "Fetching $relative_path"
    curl "${curl_args[@]}" "${JFROG_URL%/}/$relative_path" --output "$destination"
    case "$destination" in
        *.zip|*.ZIP)
            echo "Extracting $relative_path"
            unzip -q -o "$destination" -d "$(dirname "$destination")"
            ;;
    esac
done

echo "Fetched ${#files[@]} file(s) into $OUTPUT_DIR"