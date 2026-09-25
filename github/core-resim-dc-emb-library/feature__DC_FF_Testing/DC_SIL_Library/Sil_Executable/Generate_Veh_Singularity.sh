#!/bin/bash 

echo "###Checking for existing Instances of APTIV VV Engine###"
 
CONFIG_ROOT_PATH="Config"

FW_ROOT_PATH="FW_Release"

RECU_ROOT_PATH="RECU_dll"

RADAR_DLL_ROOT_PATH="Radar_dlls"

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

    echo "Please Enter the CEER Variant BUILD TYPE:"

    echo "1 - Release"

    echo "2 - Debug"

    read BUILD_TYPE

    case $BUILD_TYPE in

    "1") TYPE="Release" ;;

    "2") TYPE="Debug" ;;

    *) 

        echo "Invalid build type. Exiting."

        exit 1

        ;;

    esac

    case $RADAR_VARIANT_IN in

    "1")

        echo "Radar Variant Selected as SRR"

        RADAR_VARIANT="srr"

        RECU_PATH=$RECU_ROOT_PATH/$CUSTOMER/SRR/$TYPE

        CONFIG_PATH=$CONFIG_ROOT_PATH

        cp -a "$RECU_PATH/libSRR_DC_SIL_LIB.so" "$DOCKER_BIN/"

        cp -a Radar_dlls/*.so "$DOCKER_BIN/"

        cp -a $FW_ROOT_PATH/APT_SRR_RESIM "$DOCKER_BIN/"

        cp -a $FW_ROOT_PATH/*.so "$DOCKER_BIN/"

        cp -a SM_dll/*.so "$DOCKER_BIN/"
        cp -a $CONFIG_PATH/*.xml "$DOCKER_BIN/"

        ;;

    "2")

        echo "Radar Variant Selected as MRR"

        RADAR_VARIANT="mrr"

        RECU_PATH=$RECU_ROOT_PATH/$CUSTOMER/MRR/$TYPE

        CONFIG_PATH=$CONFIG_ROOT_PATH

        cp -a "$RECU_PATH/libMRR_DC_SIL_LIB.so" "$DOCKER_BIN/"

        cp -a Radar_dlls/*.so "$DOCKER_BIN/"

        cp -a $FW_ROOT_PATH/APT_SRR_RESIM "$DOCKER_BIN/"

        cp -a $FW_ROOT_PATH/*.so "$DOCKER_BIN/"

        cp -a SM_dll/*.so "$DOCKER_BIN/"
        cp -a $CONFIG_PATH/*.xml "$DOCKER_BIN/"
        ;;

    "3")

        echo "Radar Variant Selected as SRR_MRR"

        RADAR_VARIANT="srr_mrr"

        RECU_PATH=$RECU_ROOT_PATH/$CUSTOMER

        CONFIG_PATH=$CONFIG_ROOT_PATH

        cp -a $RECU_PATH/SRR/$TYPE/libSRR_DC_SIL_LIB.so "$DOCKER_BIN/"

        cp -a $RECU_PATH/MRR/$TYPE/libMRR_DC_SIL_LIB.so "$DOCKER_BIN/"

        cp -a Radar_dlls/*.so "$DOCKER_BIN/"

        cp -a $FW_ROOT_PATH/APT_SRR_RESIM "$DOCKER_BIN/"

        cp -a $FW_ROOT_PATH/*.so "$DOCKER_BIN/"

        cp -a SM_dll/*.so "$DOCKER_BIN/"

        cp -a $CONFIG_PATH/*.xml "$DOCKER_BIN/"

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
SIL_CONFIG_PATH="Binaries/SIL_Engine_Config.xml"
 
python3 update_veh_sing_xml.py "$CUST_IN" "$RADAR_VARIANT_IN" "$SIL_CONFIG_PATH"
 
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
 
chmod +x createImage.sh
./createImage.sh $CUST $RADAR_VARIANT $date
 
mkdir -p $DOCKER_PATH

mkdir -p "$DOCKER_PATH/INPUT_LOGS"
cp -a "Config/SIL_Input.txt" "$DOCKER_PATH/"

mkdir -p "$DOCKER_PATH/OUTPUT_DIR"
 
mv "GEN_LOG.sh" "$DOCKER_PATH/"

mv "Run_Singularity.sh" "$DOCKER_PATH/"

mv "resim_vv_${CUST}_${RADAR_VARIANT}_${date}.tar" "$DOCKER_PATH/"

mv "resim_vv_${CUST}_${RADAR_VARIANT}_${date}.simg" "$DOCKER_PATH/"
 
#rm -rf Binaries

rm -rf ubuntu_base.tar
 
