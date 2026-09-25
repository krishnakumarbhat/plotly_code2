#!/bin/bash
# *================================================================================*
# * Generates a profiling Singularity image (SIMG) for VV Engine.                  *
# * The image contains Valgrind, Heaptrack, and the ASan-instrumented binary.       *
# *                                                                                 *
# * IMPORTANT: Run this script from the SIMG repo root, NOT from inside this folder. *
# *     ./Memory_Profiling_Scripts/Generate_Profiling_Singularity.sh                            *
# *                                                                                 *
# * Prerequisites:                                                                  *
# *   1. FMUs and configs must be present (run Generate_Singularity.sh at least     *
# *      once to populate 02_SENSOR_MODEL/, 03_LOGIC_MODEL/, 04_MODEL_CONFIG/).     *
# *   2. Build the ASan binary from the dev repo:                                   *
# *        cd ~/git/Core_RESIM_VV_Engine && ./Build_MemCheck.sh                     *
# *      Then copy the output to the SIMG repo:                                     *
# *        mkdir -p 01_VV_ENGINE/Binaries/Linux_memcheck                            *
# *        cp ~/git/Core_RESIM_VV_Engine/output/x64_memcheck/SensorModelSilEngine \ *
# *           01_VV_ENGINE/Binaries/Linux_memcheck/                                  *
# *   3. The GCC version used to compile the ASan binary must match the GCC         *
# *      installed inside ubuntu:latest at image-build time (see note below).       *
# *                                                                                 *
# * GCC version note:                                                               *
# *   The ASan binary links dynamically against libasan. The profiling image        *
# *   installs 'gcc' from ubuntu:latest which provides the matching libasan.        *
# *   If your host GCC differs from the Ubuntu default, rebuild the ASan binary     *
# *   inside a matching Ubuntu container or use -static-libasan (requires editing   *
# *   CMakeLists.txt).                                                              *
# *================================================================================*

echo "### Generating VV Engine Profiling SIMG ###"

# directory containing this script — all four profiling scripts must be in the same folder
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

SM2_P_ROOT_PATH="02_SENSOR_MODEL/FMU"
LM2_ROOT_PATH="03_LOGIC_MODEL"
MODEL_ROOT_PATH="04_MODEL_CONFIG"
DOCKER_BIN="Binaries"
MEMCHECK_BIN_SRC="01_VV_ENGINE/Binaries/Linux_memcheck/SensorModelSilEngine"
ENGINE_VERSION="12.00.00.00"

mkdir -p "$DOCKER_BIN"

if [ ! -f "$MEMCHECK_BIN_SRC" ]; then
    echo "ERROR: ASan binary not found at: $MEMCHECK_BIN_SRC"
    echo "Build it with Build_MemCheck.sh then copy as described in the header."
    exit 1
fi

echo "Please enter which customer you want to select to run VV Engine for:"
echo "1 - CEER"
echo "2 - ADCAM"
read CUST_IN
CUSTOMER="UNK"

case $CUST_IN in
"1")
    CUSTOMER="CEER"
    CUST="ceer"

    echo "Please select the SRR SM2 version:"
    echo "1 - SRR_Master_SM2_OSI3.5.0_v0.2.13.6.0"
    read SM2
    case $SM2 in
        "1") SRR_SM2_P="SRR_Master_SM2_OSI3.5.0_v0.2.13.6.0/SRR_Master_SensorModel2.fmu" ;;
        *)   echo "Invalid Entry Quitting!!"; exit 1 ;;
    esac

    echo "Please Enter the CEER Model Config Variant:"
    echo "1 - P600"
    echo "2 - P700"
    echo "3 - P800"
    read RADAR_VARIANT_INt
    case $RADAR_VARIANT_INt in
        "1") VARIANT="p600" ;;
        "2") VARIANT="p700" ;;
        "3") VARIANT="p800" ;;
        *)   echo "Invalid Entry Quitting build!!"; exit 1 ;;
    esac

    LM2_PATH="$LM2_ROOT_PATH/$CUSTOMER/SRR_Master_LogicModel2_CEER.fmu"
    MODEL_PATH="$MODEL_ROOT_PATH/$CUSTOMER/$VARIANT/modelconfig_sil_CEER_With_FC.yaml"
    MODEL_FILENAME="modelconfig_sil_CEER_With_FC.yaml"

    cp -a "$SM2_P_ROOT_PATH/$SRR_SM2_P" "$DOCKER_BIN/"
    cp -a "$LM2_PATH" "$DOCKER_BIN/"
    ;;
"2")
    CUSTOMER="ADCAM"
    CUST="adcam"
    VARIANT="ifv600"

    SRR_SM2_P="SRR_Master_SM2_OSI3.5.0_v0.2.13.6.0/SRR_Master_SensorModel2.fmu"
    CAM_SM2_P="ADCAM_SM2_6.0.0/CAMERAECU_SM2_v6.0.0.fmu"
    ADCAM_LM2_PATH="$LM2_ROOT_PATH/ADCAM_LM2_7.0.0/CAMERAECU_LM2_v7.0.0.fmu"
    IFV600_LM2_PATH="$LM2_ROOT_PATH/IFV600_LM2/SRR_Master_LogicModel2_STLA_SMALL_IFV600.fmu"
    MODEL_PATH="$MODEL_ROOT_PATH/ADCAM/modelconfig_5radarSM2_1camSM2_1camLM2_1IFVLM2.yaml"
    MODEL_FILENAME="modelconfig_5radarSM2_1camSM2_1camLM2_1IFVLM2.yaml"

    cp -a "$SM2_P_ROOT_PATH/$SRR_SM2_P" "$DOCKER_BIN/"
    cp -a "$SM2_P_ROOT_PATH/$CAM_SM2_P" "$DOCKER_BIN/"
    cp -a "$ADCAM_LM2_PATH" "$DOCKER_BIN/"
    cp -a "$IFV600_LM2_PATH" "$DOCKER_BIN/"
    ;;
*)
    echo "Invalid Entry Quitting build!!"
    exit 1
    ;;
esac

cp -a "01_VV_ENGINE/Binaries/Linux/." "$DOCKER_BIN/"
cp -a "$MEMCHECK_BIN_SRC" "${DOCKER_BIN}/SensorModelSilEngine_memcheck"
cp -a "01_VV_ENGINE/Config/VVEngineConfig.yaml" "$DOCKER_BIN/"
cp -a "$MODEL_PATH" "$DOCKER_BIN/"

# Bundle libasan.so.5 from the host so the ASan binary can load it inside the container
# regardless of which GCC version ubuntu:latest provides.
for _lib in libasan.so.5 libasan.so.5.0.0; do
    _src=$(find /usr/lib -name "$_lib" 2>/dev/null | head -1)
    [ -n "$_src" ] && cp "$_src" "$DOCKER_BIN/"
done

NEW_MODEL_PATH="/Binaries/${MODEL_FILENAME}"
python3 Update_configuration.py "$CUST_IN"
python3 Update_Customer_Configuration.py "$CUST_IN" "$NEW_MODEL_PATH"
python3 unzip.py "$CUST_IN"

SIMG_NAME="resim_vv_${CUST}_${VARIANT}_${ENGINE_VERSION}_profiling"

# ---- build ubuntu_profiling_base (Docker path only) then delegate to createImage_profiling.sh ----
if command -v docker &>/dev/null; then
    rm -f Dockerfile
    echo FROM        ubuntu   >>Dockerfile
    echo MAINTAINER  aptiv  >>Dockerfile
    echo ARG DEBIAN_FRONTEND=noninteractive >>Dockerfile
    echo RUN apt-get update >>Dockerfile
    echo RUN DEBIAN_FRONTEND=noninteractive apt-get -yq install gdbserver python3 libpcap0.8t64 libatomic1 valgrind heaptrack gcc >>Dockerfile
    docker build -t ubuntu_profiling_base --network=host .
    rm -f Dockerfile
fi

[ -f "${SCRIPT_DIR}/createImage_profiling.sh" ] || { echo "ERROR: createImage_profiling.sh not found next to this script."; exit 1; }
chmod +x "${SCRIPT_DIR}/createImage_profiling.sh"
"${SCRIPT_DIR}/createImage_profiling.sh" "$CUST" "$VARIANT" "$ENGINE_VERSION"

# ---- organise outputs into a dedicated profiling deliverable folder ----
OUTPUT_DIR="08_CloudBinaries_${CUST}_${VARIANT}_profiling"
mkdir -p "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR/INPUT_LOGS"
cp -a "01_VV_ENGINE/Sample_Trace" "$OUTPUT_DIR/INPUT_LOGS/" 2>/dev/null || true
mkdir -p "$OUTPUT_DIR/OUTPUT_DIR"

{
    while IFS= read -r line || [ -n "$line" ]; do
        [ -z "$line" ] && continue
        echo "$(realpath "$OUTPUT_DIR/INPUT_LOGS/$line")"
    done < "01_VV_ENGINE/Config/Flist_file.txt"
} > "$OUTPUT_DIR/Flist_file.txt"

mv "Run_Singularity_Profiling.sh" "$OUTPUT_DIR/"
chmod 777 "$OUTPUT_DIR/Run_Singularity_Profiling.sh"

mv "${SIMG_NAME}.simg" "$OUTPUT_DIR/"
[ -f "${SIMG_NAME}.tar" ] && mv "${SIMG_NAME}.tar" "$OUTPUT_DIR/"

rm -rf Binaries
rm -f ubuntu_profiling_base.tar 2>/dev/null || true

echo ""
echo "Profiling SIMG ready:"
echo "  Folder:     $OUTPUT_DIR/"
echo "  SIMG:       ${OUTPUT_DIR}/${SIMG_NAME}.simg"
echo "  Run script: ${OUTPUT_DIR}/Run_Singularity_Profiling.sh"
echo ""
echo "Usage:"
echo "  cd $OUTPUT_DIR"
echo "  ./Run_Singularity_Profiling.sh valgrind"
echo "  ./Run_Singularity_Profiling.sh asan"
echo "  ./Run_Singularity_Profiling.sh heaptrack /path/to/trace.txt"
echo ""
echo "For HPC, copy the SIMG to the cluster and use:"
echo "  profiling_simg/run_valgrind_hpc.sh    (edit paths inside first)"
echo "  profiling_simg/run_asan_hpc.sh        (edit paths inside first)"
echo "  profiling_simg/run_heaptrack_hpc.sh   (edit paths inside first)"
