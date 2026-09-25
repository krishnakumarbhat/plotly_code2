#!/bin/bash
# Runs the VV Engine profiling SIMG from the profiling deliverable folder.
# Usage: ./Run_Singularity_Profiling.sh [mode] [trace_file]
#   mode       : asan | valgrind | valgrind-supp | heaptrack  (default: valgrind)
#   trace_file : required for heaptrack mode (single OSI trace, not a BList)
#
# Examples:
#   ./Run_Singularity_Profiling.sh valgrind
#   ./Run_Singularity_Profiling.sh asan
#   ./Run_Singularity_Profiling.sh heaptrack /path/to/trace.txt

MODE="${1:-valgrind}"
TRACE_FILE="$2"

# auto-detect the profiling SIMG in the current directory — no hardcoded name needed
SIMG=$(ls -t "$(pwd)"/*_profiling.simg 2>/dev/null | head -1)
if [ -z "$SIMG" ]; then
    echo "ERROR: No *_profiling.simg found in $(pwd)"
    exit 1
fi

INPUT_ARG="$(pwd)/Flist_file.txt"
if [ "$MODE" = "heaptrack" ]; then
    if [ -z "$TRACE_FILE" ]; then
        echo "ERROR: heaptrack mode requires a trace file as the second argument."
        echo "Usage: $0 heaptrack /path/to/trace.txt"
        exit 1
    fi
    INPUT_ARG="$TRACE_FILE"
fi

OUTDIR="$(pwd)/OUTPUT_DIR"

RUN_MARKER=$(mktemp)
[ "$MODE" = "valgrind-supp" ] && rm -f "${OUTDIR}"/valgrind_supp_*.txt

singularity exec --writable-tmpfs \
    --bind "${OUTDIR}:${OUTDIR}" \
    "$SIMG" \
    /RUN_PROFILING.sh --mode "$MODE" "$INPUT_ARG" "$OUTDIR"

if [ "$MODE" = "valgrind-supp" ]; then
    rm -f "$RUN_MARKER"
    echo "Extracting and deduplicating suppression blocks..."
    SUPP_FILE="${OUTDIR}/valgrind_fmu.supp"
    python3 - <<PYEOF
import re, hashlib, glob

# Only genuine loader/runtime noise gets suppressed. Blocks that reference any
# resolved application frame (engine or FMU) are rejected so real bugs are reported.
NOISE_RE = re.compile(r'obj:\*|fun:call_init|fun:_dl_|fun:dl_open_worker|fun:dlopen_doit|fun:__pthread_once|fun:pthread_once')
ALLOCATOR_RE = re.compile(r'fun:(malloc|calloc|realloc|_Znwm|_Znam)$')

def is_noise(block):
    frame_lines = [l.strip() for l in block.splitlines() if l.strip().startswith(('fun:', 'obj:'))]
    if not frame_lines:
        return False
    # The allocator itself (top frame) never indicates who owns the leak — judge
    # noise purely on the caller chain above it.
    if ALLOCATOR_RE.match(frame_lines[0]):
        frame_lines = frame_lines[1:]
    return bool(frame_lines) and all(NOISE_RE.match(l) for l in frame_lines)

files = glob.glob("${OUTDIR}/valgrind_supp_*.txt")
seen, unique = {}, []
per_file = {}
for p in files:
    blocks = re.findall(r'\{[^{}]*\}', open(p).read(), re.DOTALL)
    kept, rejected = 0, 0
    for b in blocks:
        if not is_noise(b):
            rejected += 1
            continue
        kept += 1
        k = hashlib.md5(re.sub(r'<insert_a_suppression_name_here>', '', b).strip().encode()).hexdigest()
        if k not in seen:
            seen[k] = True; unique.append(b)
    per_file[p] = (kept, rejected)
out = []
for i, b in enumerate(unique, 1):
    err = (re.search(r'Memcheck:(\w+)', b) or type('', (), {'group': lambda s,x: 'Err'})()).group(1)
    top = (re.search(r'fun:(\S+)', b) or type('', (), {'group': lambda s,x: 'unknown'})()).group(1)[:25]
    out.append(b.replace('<insert_a_suppression_name_here>', f'FMU_suppress_{i}_{err}_{top}').strip())
open("${SUPP_FILE}", 'w').write('\n'.join(out) + '\n')

for p, (kept, rejected) in per_file.items():
    with open(p, 'a') as fh:
        fh.write("\n=== SUPPRESSION GENERATION SUMMARY ===\n")
        fh.write(f"Noise-only blocks suppressed: {kept}\n")
        fh.write(f"Rejected (real engine/FMU code, not suppressed): {rejected}\n")
    print(f"--- {p} --- suppressed={kept} rejected={rejected}")

print(f"Written {len(unique)} unique noise-only blocks -> ${SUPP_FILE}")
PYEOF
elif [ "$MODE" = "valgrind" ]; then
    mapfile -t CURRENT_REPORTS < <(find "$OUTDIR" -maxdepth 1 -name 'valgrind_*.txt' ! -name 'valgrind_supp_*' -newer "$RUN_MARKER" | sort)
    rm -f "$RUN_MARKER"
    echo ""
    echo "=== LEAK SUMMARIES ==="
    for f in "${CURRENT_REPORTS[@]}"; do
        [ -f "$f" ] || continue
        summary=$(grep -A 6 "LEAK SUMMARY" "$f" 2>/dev/null)
        if [ -n "$summary" ]; then
            echo "--- $f ---"
            echo "$summary"
        fi
    done
elif [ "$MODE" = "asan" ]; then
    mapfile -t CURRENT_REPORTS < <(find "$OUTDIR" -maxdepth 1 -name 'asan_*' -newer "$RUN_MARKER" | sort)
    rm -f "$RUN_MARKER"
    echo ""
    echo "=== ERRORS AND LEAK SUMMARIES ==="
    for f in "${CURRENT_REPORTS[@]}"; do
        [ -f "$f" ] || continue
        filtered=$(grep -E "ERROR:|LEAK SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable|ERROR SUMMARY|#[0-9]+ 0x" "$f" 2>/dev/null)
        if [ -n "$filtered" ]; then
            echo "--- $f ---"
            echo "$filtered"
        fi
    done
fi
