#!/bin/sh
# Profiling entry point inside the Singularity container.
# Embedded at /RUN_PROFILING.sh by Generate_Profiling_Singularity.sh.
# Usage: /RUN_PROFILING.sh --mode <asan|valgrind|valgrind-supp|heaptrack> <flist_or_trace> <output_dir>

if [ "$1" != "--mode" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ]; then
    echo "Usage: /RUN_PROFILING.sh --mode <asan|valgrind|valgrind-supp|heaptrack> <flist_or_trace> <output_dir>"
    exit 1
fi

MODE="$2"
INPUT="$3"
OUTDIR="$4"

mkdir -p "$OUTDIR"
cd /Binaries/ || exit 1
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/Binaries/
export HDF5_DISABLE_VERSION_CHECK=2

SKIP_CHILDREN="/usr/bin/*,/bin/*,/lib/*"

case "$MODE" in
    asan)
        export LSAN_OPTIONS="report_objects=1:max_leaks=100"
        while IFS= read -r trace || [ -n "$trace" ]; do
            [ -z "$trace" ] && continue
            trace_name=$(basename "$trace" | sed 's/\.[^.]*$//')
            export ASAN_OPTIONS="detect_leaks=1:log_path=${OUTDIR}/asan_${trace_name}:verbosity=1:halt_on_error=0"
            ./SensorModelSilEngine_memcheck ./VVEngineConfig.yaml "$trace" "$OUTDIR" --single-trace
        done < "$INPUT"
        ;;
    valgrind)
        SUPP_ARG=""
        [ -f "${OUTDIR}/valgrind_fmu.supp" ] && SUPP_ARG="--suppressions=${OUTDIR}/valgrind_fmu.supp"
        while IFS= read -r trace || [ -n "$trace" ]; do
            [ -z "$trace" ] && continue
            trace_name=$(basename "$trace" | sed 's/\.[^.]*$//')
            valgrind \
                --tool=memcheck \
                --leak-check=full \
                --show-leak-kinds=all \
                --track-origins=yes \
                --trace-children=yes \
                --trace-children-skip="$SKIP_CHILDREN" \
                --num-callers=20 \
                $SUPP_ARG \
                --log-file="${OUTDIR}/valgrind_${trace_name}_%p.txt" \
                ./SensorModelSilEngine ./VVEngineConfig.yaml "$trace" "$OUTDIR" --single-trace
        done < "$INPUT"
        ;;
    valgrind-supp)
        while IFS= read -r trace || [ -n "$trace" ]; do
            [ -z "$trace" ] && continue
            trace_name=$(basename "$trace" | sed 's/\.[^.]*$//')
            valgrind \
                --tool=memcheck \
                --leak-check=full \
                --show-leak-kinds=all \
                --gen-suppressions=all \
                --trace-children=yes \
                --trace-children-skip="$SKIP_CHILDREN" \
                --num-callers=10 \
                --log-file="${OUTDIR}/valgrind_supp_${trace_name}_%p.txt" \
                ./SensorModelSilEngine ./VVEngineConfig.yaml "$trace" "$OUTDIR" --single-trace
        done < "$INPUT"
        ;;
    heaptrack)
        trace_name=$(basename "$INPUT" | sed 's/\.[^.]*$//')
        heaptrack --output "${OUTDIR}/heaptrack_${trace_name}" \
            ./SensorModelSilEngine ./VVEngineConfig.yaml "$INPUT" "$OUTDIR" --single-trace
        HEAPTRACK_FILE=$(ls -t "${OUTDIR}"/heaptrack_${trace_name}*.zst "${OUTDIR}"/heaptrack_${trace_name}*.gz 2>/dev/null | head -1)
        [ -n "$HEAPTRACK_FILE" ] && heaptrack_print "$HEAPTRACK_FILE" > "${OUTDIR}/heaptrack_${trace_name}_report.txt" 2>&1
        ;;
    *)
        echo "ERROR: Unknown mode '$MODE'. Use: asan, valgrind, valgrind-supp, heaptrack"
        exit 1
        ;;
esac
