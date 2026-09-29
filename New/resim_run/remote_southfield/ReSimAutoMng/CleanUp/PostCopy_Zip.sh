#!/bin/bash
#SBATCH --job-name=PostCopyZip
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=2G
#SBATCH --time=02:00:00
#SBATCH -o /tmp/postcopy_%j.log
#SBATCH -e /tmp/postcopy_%j.log

COPY_LIST="$1"
TARGET_PATH="$2"
PASSWORD="Just2Open"

if [[ -z "$COPY_LIST" || -z "$TARGET_PATH" ]]; then
    echo "[ERROR] : Usage: PostCopy_Zip.sh <copy_list> <target_path>"
    exit 1
fi

if [[ ! -f "$COPY_LIST" ]]; then
    echo "[ERROR] : Copy list not found: $COPY_LIST"
    exit 1
fi

if ! command -v zip >/dev/null 2>&1; then
    echo "[ERROR] : zip command not found"
    exit 1
fi

# Build unique source paths in-memory from copy list.
if grep -q $'\t' "$COPY_LIST"; then
    UNIQUE_SOURCES=$(awk -F '\t' 'NF >= 1 {print $1}' "$COPY_LIST" | sort -u)
else
    UNIQUE_SOURCES=$(awk '{print $1}' "$COPY_LIST" | sort -u)
fi

while IFS= read -r src_path; do
    [[ -z "$src_path" ]] && continue

    if [[ ! -e "$src_path" ]]; then
        echo "[WARN] : Source path not found, skipping: $src_path"
        continue
    fi

    if [[ ! -d "$src_path" ]]; then
        echo "[WARN] : Source path is not a directory, skipping delete/zip: $src_path"
        continue
    fi

    src_name=$(basename "$src_path")
    parent_dir=$(dirname "$src_path")
    zip_file="${src_path%/}.zip"

    echo "[INFO] : Zipping source $src_path -> $zip_file"
    (
        cd "$parent_dir" || exit 1
        zip -rq -P "$PASSWORD" "$zip_file" "$src_name"
    )

    if [[ $? -ne 0 ]]; then
        echo "[FAILED] : Could not zip source path $src_path"
        exit 1
    fi

    if [[ ! -s "$zip_file" ]]; then
        echo "[FAILED] : Zip file missing or empty for $src_path"
        exit 1
    fi

    rm -rf "$src_path"
    if [[ $? -ne 0 ]]; then
        echo "[FAILED] : Zip created but could not delete source directory $src_path"
        exit 1
    fi
    echo "[INFO] : Deleted original directory $src_path"
done <<< "$UNIQUE_SOURCES"

rm -f "$COPY_LIST"

echo "[DONE] : Password-protected source zip creation completed at source locations"
