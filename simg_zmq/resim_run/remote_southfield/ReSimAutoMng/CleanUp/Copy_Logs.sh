#!/bin/bash
#SBATCH --job-name=LogCopy
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=4G
#SBATCH --time=02:00:00
#SBATCH -o /tmp/copy_%A_%a.log
#SBATCH -e /tmp/copy_%A_%a.log

COPY_LIST="$1"           # path to the source/destination list file
LINE=$(sed -n "${SLURM_ARRAY_TASK_ID}p" "$COPY_LIST")

if [[ "$LINE" == *$'\t'* ]]; then
    IFS=$'\t' read -r SRC_ROOT SRC DST <<< "$LINE"
else
    # Backward compatibility with old two-column files: <move_path> <dest>
    SRC_ROOT=""
    SRC=$(echo "$LINE" | awk '{print $1}')
    DST=$(echo "$LINE" | awk '{print $2}')
fi

[[ -z "$SRC" || -z "$DST" ]] && { echo "[ERROR] : Empty line at task $SLURM_ARRAY_TASK_ID"; exit 1; }

mkdir -p "$(dirname "$DST")"

echo "[INFO] : Copying $SRC -> $DST"
# Trailing slash on SRC copies contents of the directory into DST (not the dir itself)
rsync -a --progress "${SRC}/" "${DST}/"
EXIT=$?

if [[ $EXIT -eq 0 ]]; then
    echo "[DONE] : $SRC"
else
    echo "[FAILED] : $SRC (rsync exit $EXIT)"
    exit $EXIT
fi