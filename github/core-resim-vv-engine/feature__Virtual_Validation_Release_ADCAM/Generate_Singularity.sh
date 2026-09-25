#!/bin/bash
echo "### Checking for existing instances of APTIV VV Engine ###"

SM2_P_ROOT_PATH="02_SENSOR_MODEL/FMU"
LM2_ROOT_PATH="03_LOGIC_MODEL"
MODEL_ROOT_PATH="04_MODEL_CONFIG"
DOCKER_BIN="Binaries"
mkdir -p "$DOCKER_BIN"

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
        "1")
            SRR_SM2_P="SRR_Master_SM2_OSI3.5.0_v0.2.13.6.0/SRR_Master_SensorModel2.fmu"
            ;;
        *)
            echo "Invalid Entry Quitting!!"
            exit 1
            ;;
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
    CAM_SM2_P="ADCAM_SM2_6.0.2/CAMERAECU_SM2_v6.0.2.fmu"
    ADCAM_LM2_PATH="$LM2_ROOT_PATH/ADCAM_LM2_7.5.0/CAMERAECU_LM2_v7.5.0.fmu"
    IFV600_LM2_PATH="$LM2_ROOT_PATH/IFV600_V1P1/SRR_Master_LogicModel2_STLA_SMALL_IFV600.fmu"
    MODEL_PATH="$MODEL_ROOT_PATH/ADCAM/modelconfig_3radarSM2_1camSM2_1camLM2_1IFVLM2.yaml"
    MODEL_FILENAME="modelconfig_3radarSM2_1camSM2_1camLM2_1IFVLM2.yaml"

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
cp -a "01_VV_ENGINE/Config/VVEngineConfig.yaml" "$DOCKER_BIN/"
cp -a "$MODEL_PATH" "$DOCKER_BIN/"

ENGINE_VERSION="12.00.00.00"
customername="${CUST}"

NEW_MODEL_PATH="/Binaries/${MODEL_FILENAME}"
python3 Update_configuration.py "$CUST_IN"
python3 Update_Customer_Configuration.py "$CUST_IN" "$NEW_MODEL_PATH"

# Unzipping FMUs based on model config
python3 unzip.py "$CUST_IN"

# Build ubuntu base image (only needed when Docker is available)
if command -v docker &>/dev/null; then
    rm -f Dockerfile
    echo FROM        ubuntu   >>Dockerfile
    echo MAINTAINER  aptiv  >>Dockerfile
    echo ARG DEBIAN_FRONTEND=noninteractive >>Dockerfile
    echo RUN apt-get update >>Dockerfile
    echo RUN DEBIAN_FRONTEND=noninteractive apt-get -yq install gdbserver python3 libpcap0.8t64 libatomic1 >>Dockerfile
    docker build -t ubuntu_base --network=host .
    docker save -o ubuntu_base.tar ubuntu_base
    rm -f Dockerfile
fi

DOCKER_PATH="08_CloudBinaries_${customername}_${VARIANT}"
if [ -d "$DOCKER_PATH" ]; then
    echo " - $DOCKER_PATH already exists" 1>&2
fi

chmod +x createImage.sh
./createImage.sh "$customername" "$VARIANT" "$ENGINE_VERSION"

# Generate Run_Singularity.sh
# Uses --writable-tmpfs so LM2 FMUs can write cp_buffer.bin to /Binaries/LogicModelId*/
# RUN_VV.sh then copies those outputs to OUTPUT_DIR/ before the container exits.
rm -f Run_Singularity.sh
echo '#!/bin/sh' >> Run_Singularity.sh
echo '' >> Run_Singularity.sh
echo "singularity exec --writable-tmpfs \$(pwd)/resim_vv_${customername}_${VARIANT}_${ENGINE_VERSION}.simg /RUN_VV.sh \$(pwd)/Flist_file.txt \$(pwd)/OUTPUT_DIR" >> Run_Singularity.sh
chmod 777 Run_Singularity.sh

mkdir -p "$DOCKER_PATH"
mkdir -p "$DOCKER_PATH/INPUT_LOGS"
cp -a "01_VV_ENGINE/Sample_Trace" "$DOCKER_PATH/INPUT_LOGS/"
mkdir -p "$DOCKER_PATH/OUTPUT_DIR"

# Generate Flist_file.txt with absolute paths so logs are accessible inside the container
# (container CWD is /Binaries/ so relative paths from the source config won't resolve)
{
    while IFS= read -r line || [ -n "$line" ]; do
        [ -z "$line" ] && continue
        echo "$(realpath "$DOCKER_PATH/INPUT_LOGS/$line")"
    done < "01_VV_ENGINE/Config/Flist_file.txt"
} > "$DOCKER_PATH/Flist_file.txt"

mv "GEN_LOG.sh" "$DOCKER_PATH/"
mv "Run_Singularity.sh" "$DOCKER_PATH/"
[ -f "resim_vv_${customername}_${VARIANT}_${ENGINE_VERSION}.tar" ] && mv "resim_vv_${customername}_${VARIANT}_${ENGINE_VERSION}.tar" "$DOCKER_PATH/"
mv "resim_vv_${customername}_${VARIANT}_${ENGINE_VERSION}.simg" "$DOCKER_PATH/"

rm -rf Binaries
rm -rf ubuntu_base.tar
