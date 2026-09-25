#!/bin/bash 

echo "###Checking for existing Instances of APTIV VV Engine###"

# Check if xmllint is installed, if not, try to install it
if ! which xmllint > /dev/null 2>&1; then
    echo "xmllint is not installed. Attempting to install libxml2-utils..."
    
    if which apt-get > /dev/null 2>&1; then
        sudo apt-get update
        sudo apt-get install -y libxml2-utils
    elif which yum > /dev/null 2>&1; then
        sudo yum install -y libxml2
    elif which dnf > /dev/null 2>&1; then
        sudo dnf install -y libxml2
    else
        echo "Error: Could not install xmllint automatically. Please install libxml2-utils manually and run the script again."
        exit 1
    fi
    
    # Check if installation was successful
    if ! which xmllint > /dev/null 2>&1; then
        echo "Error: xmllint installation failed. Please install libxml2-utils manually and run the script again."
        exit 1
    else
        echo "xmllint installed successfully."
    fi
fi

get_xml_value() {
    local xml_file=$1
    local xpath=$2
    xmllint --xpath "$xpath" "$xml_file" | sed 's/<[^>]*>//g'
}

INPUT_XML="input_path.xml"

DOCKER_BIN="Binaries"

mkdir -p $DOCKER_BIN
 
echo "Please enter which customer you want to select to run vv engine for:"
echo "1 - CEER"
read CUST_IN

CUSTOMER="UNK"
 
case $CUST_IN in
"1")
    CUSTOMER="CEER"
    CUST="ceer"

    echo "Please Enter the CEER Radar Variant FMU you want to Run:"
    echo "1 - SRR"
    echo "2 - MRR"
    echo "3 - SRR_MRR"
    read RADAR_VARIANT_IN

    echo "Please Enter the CEER Radar Variant mode:"
    echo "1 - CDC"
    echo "2 - DETECTIONS_UDP"

    read MODE_IN
    
    case $MODE_IN in

    "1") MODE="CDC" ;;

    "2") MODE="DETECTIONS_UDP" ;;

    *) 

        echo "Invalid sil entry type. Exiting."

        exit 1

        ;;

    esac

    case $RADAR_VARIANT_IN in
    "1")
        echo "Radar Variant Selected as SRR"
        RADAR_VARIANT="srr"
        
        # Get paths directly from XML
        SRR_LIB_PATH=$(get_xml_value "$INPUT_XML" "//CEER/SRR_LIB_PATH/text()")
        RADAR_DLLS_PATH=$(get_xml_value "$INPUT_XML" "//CEER/RADAR_DLLS_PATH/text()")
        FW_LIBS_PATH=$(get_xml_value "$INPUT_XML" "//FW_LIBS_PATH/text()")
        CONFIG_FILES_PATH=$(get_xml_value "$INPUT_XML" "//CONFIG_FILES_PATH/text()")

        cp -a ".$SRR_LIB_PATH" "$DOCKER_BIN/"
        cp -a ".$RADAR_DLLS_PATH"/*.so "$DOCKER_BIN/"
        cp -a ".$FW_LIBS_PATH/APT_SRR_RESIM" "$DOCKER_BIN/"
        cp -a ".$FW_LIBS_PATH"/*.so* "$DOCKER_BIN/"
        cp -a ".$CONFIG_FILES_PATH"/*.xml "$DOCKER_BIN/"

        ;;

    "2")
        echo "Radar Variant Selected as MRR"
        RADAR_VARIANT="mrr"

        MRR_LIB_PATH=$(get_xml_value "$INPUT_XML" "//CEER/MRR_LIB_PATH/text()")
        RADAR_DLLS_PATH=$(get_xml_value "$INPUT_XML" "//CEER/RADAR_DLLS_PATH/text()")
        FW_LIBS_PATH=$(get_xml_value "$INPUT_XML" "//FW_LIBS_PATH/text()")
        CONFIG_FILES_PATH=$(get_xml_value "$INPUT_XML" "//CONFIG_FILES_PATH/text()")
        
        cp -a ".$MRR_LIB_PATH" "$DOCKER_BIN/"
        cp -a ".$RADAR_DLLS_PATH"/*.so "$DOCKER_BIN/"
        cp -a ".$FW_LIBS_PATH/APT_SRR_RESIM" "$DOCKER_BIN/"
        cp -a ".$FW_LIBS_PATH"/*.so* "$DOCKER_BIN/"
        cp -a ".$CONFIG_FILES_PATH"/*.xml "$DOCKER_BIN/"
        
        ;;

    "3")
        echo "Radar Variant Selected as SRR_MRR"
        RADAR_VARIANT="srr_mrr"

        SRR_LIB_PATH=$(get_xml_value "$INPUT_XML" "//CEER/SRR_LIB_PATH/text()")
        MRR_LIB_PATH=$(get_xml_value "$INPUT_XML" "//CEER/MRR_LIB_PATH/text()")
        RADAR_DLLS_PATH=$(get_xml_value "$INPUT_XML" "//CEER/RADAR_DLLS_PATH/text()")
        FW_LIBS_PATH=$(get_xml_value "$INPUT_XML" "//FW_LIBS_PATH/text()")
        CONFIG_FILES_PATH=$(get_xml_value "$INPUT_XML" "//CONFIG_FILES_PATH/text()")

        cp -a ".$SRR_LIB_PATH" "$DOCKER_BIN/"
        cp -a ".$MRR_LIB_PATH" "$DOCKER_BIN/"
        cp -a ".$RADAR_DLLS_PATH"/*.so "$DOCKER_BIN/"
        cp -a ".$FW_LIBS_PATH/APT_SRR_RESIM" "$DOCKER_BIN/"
        cp -a ".$FW_LIBS_PATH"/*.so* "$DOCKER_BIN/"
        cp -a ".$CONFIG_FILES_PATH"/*.xml "$DOCKER_BIN/"
        
        ;;    

    *)
        echo "Unsupported Variant"
        echo "Invalid Entry Quitting"
        exit 1
        ;;
    esac
    ;;

*)
    echo "Invalid Entry Quitting build!!"
    exit 1
    ;;
esac
cp "$(pwd)/build/versions.txt" "$DOCKER_BIN/"
SIL_CONFIG_PATH="Binaries/SIL_Engine_Config.xml"
 
python3 update_veh_sing_xml.py "$CUST_IN" "$RADAR_VARIANT_IN" "$SIL_CONFIG_PATH" "$MODE"
 
date=$(date '+%Y%m%d%H%M%S')
echo FROM        ubuntu   >>Dockerfile
echo MAINTAINER  aptiv  >>Dockerfile
echo ARG DEBIAN_FRONTEND=noninteractive >>Dockerfile
echo RUN apt-get update >>Dockerfile
echo RUN apt-get -yq install gdbserver >>Dockerfile
echo RUN apt-get -yq install python3 >>Dockerfile
 
docker build -t ubuntu_base --network=host .

docker save -o ubuntu_base.tar ubuntu_base
 
DOCKER_PATH="CloudBinaries"

if [ -d $DOCKER_PATH ]; then
    echo " - $DOCKER_PATH already exists" 1>&2
fi
 
chmod +x Create_Image.sh
./Create_Image.sh $CUST $RADAR_VARIANT $date

mkdir -p $DOCKER_PATH
chmod 755 $DOCKER_PATH
mkdir -p "$DOCKER_PATH/INPUT_LOGS"
cp -a "$(pwd)/$CONFIG_FILES_PATH/SIL_Input.txt" "$DOCKER_PATH/"

mkdir -p "$DOCKER_PATH/OUTPUT_DIR"
 
mv "GEN_LOG.sh" "$DOCKER_PATH/"
mv "Run_Singularity.sh" "$DOCKER_PATH/"
mv "resim_v2_platform_${RADAR_VARIANT}_dc_${date}.tar" "$DOCKER_PATH/"
mv "resim_v2_platform_${RADAR_VARIANT}_dc_${date}.simg" "$DOCKER_PATH/"
 
rm -rf Binaries
rm -rf ubuntu_base.tar
