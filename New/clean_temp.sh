#!/usr/bin/env bash
# Clean temp files: move results to permanent storage, then purge /local and /tmp
set -euo pipefail

SOURCE="${1:-/local/hpc_tools}"
DEST="${2:-}"
DRY_RUN="${DRY_RUN:-0}"

if [ -z "$DEST" ]; then
    if [ -d /net/8k3 ]; then DEST="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_services_3/store"
    elif [ -d /mnt/usmidet ]; then DEST="/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_3/store"
    else echo "No DEST"; exit 1
    fi
fi

echo "Saving results from $SOURCE to $DEST"
[ "$DRY_RUN" = 1 ] && echo "DRY RUN" && exit 0

mkdir -p "$DEST"

# Copy HTML reports and KPI outputs
for dir in html_db kpi_output reports; do
    src="$SOURCE/$dir"
    [ -d "$src" ] && cp -r "$src" "$DEST/"
done

# Copy runtime DB snapshots
for f in hpc_tools_dev.db rag_logs.db; do
    [ -f "$SOURCE/$f" ] && cp "$SOURCE/$f" "$DEST/"
done

# Clean /local
find "$SOURCE" -type f -mtime +1 -delete 2>/dev/null || true
find "$SOURCE" -type d -empty -delete 2>/dev/null || true

# Clean /tmp
find /tmp/hpc_tools* -type f -mmin +60 -delete 2>/dev/null || true
find /tmp/hpc_runtime_db_* -type f -mmin +60 -delete 2>/dev/null || true

# Clean /dev/shm if hpc_tools temp data exists there
find /dev/shm/hpc_tools* -type f -mmin +60 -delete 2>/dev/null || true
find /dev/shm/hpc_tools* -type d -empty -delete 2>/dev/null || true

echo "Done. Saved to $DEST"
