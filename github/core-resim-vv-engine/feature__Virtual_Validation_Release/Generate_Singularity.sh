#!/bin/bash 
echo ###Cheking for exisiting Instances of APTIV VV Engine###

SM2_P_ROOT_PATH="02_SENSOR_MODEL/FMU"
LM2_ROOT_PATH="03_LOGIC_MODEL"
MODEL_ROOT_PATH="04_MODEL_CONFIG"
DOCKER_BIN="Binaries"
mkdir -p $DOCKER_BIN

echo "Please select the SM2:"
echo "1 - SRR_Master_SM2_OSI3.5.0_v0.0.13.9.0"
read SM2
SM2_P="SRR_Master_SM2_OSI3.5.0_v0.0.13.9.0/SRR_Master_SensorModel2.fmu"
case $SM2 in
	"1")
		SM2_P="SRR_Master_SM2_OSI3.5.0_v0.0.13.9.0/SRR_Master_SensorModel2.fmu"
		break;;
	* )
	    echo "Invalid Entry Quitting!!"
	    exit
	    break;;
esac

cp -a $SM2_P_ROOT_PATH/$SM2_P "$DOCKER_BIN/"

echo "Please enter which customer you want to select to run vv engine for:"
echo "1 - CEER"
echo "2 - GPO"
echo "3 - AL"
read CUST_IN
CUSTOMER="UNK"

case $CUST_IN in

"1")
	CUSTOMER="CEER"
	CUST="ceer"
		
	echo "Please Enter the CEER Model Config Variant:"
	echo "1 - P600"
	echo "2 - P700"
	echo "3 - P800"
	read RADAR_VARIANT_INt
		
	
	case $RADAR_VARIANT_INt in
	"1") 	VARIANT="p600"
		break;;
	"2") 	VARIANT="p700"
		break;;
	"3") 	VARIANT="p800"
		break;;
	esac
	
	MODEL_FILENAME="modelconfig_sil_CEER_With_FC.yaml"
	LM2_PATH=$LM2_ROOT_PATH/$CUSTOMER/SRR_Master_LogicModel2_CEER.fmu
    MODEL_PATH=$MODEL_ROOT_PATH/$CUSTOMER/$VARIANT/modelconfig_sil_CEER_With_FC.yaml	
      
    break;;

"2")
	CUSTOMER="GPO"
	CUST="gpo"
		
	echo "Please Enter the GPO Model Config Variant:"
	echo "1 - Car"
	echo "2 - Highdet"
	echo "3 - Truck"
	read RADAR_VARIANT_INt
		
	
	case $RADAR_VARIANT_INt in
	"1") 	VARIANT="car"
		MODEL_FILENAME="modelconfig_sil_GPO_car.yaml"
		break;;
	"2") 	VARIANT="highdet"
		MODEL_FILENAME="modelconfig_sil_GPO_Highdet.yaml"
		break;;
	"3") 	VARIANT="truck"
		MODEL_FILENAME="modelconfig_sil_GPO_truck.yaml"
		break;;
	esac
	
	LM2_PATH=$LM2_ROOT_PATH/$CUSTOMER/SRR_Master_LogicModel2_GPO_Gen8.fmu
    MODEL_PATH=$MODEL_ROOT_PATH/$CUSTOMER/$MODEL_FILENAME	
      
    break;;

"3")
	CUSTOMER="AL"
	CUST="al"
		
	echo "Please Enter the AL Model Config Variant:"
	echo "1 - 40_42_44_46_5525_4X2_TT"
	echo "2 - 4825_10X2_DTLA"
	echo "3 - 4825_10X2_DTLA_HIL"
	echo "4 - BUS_Cheetah"
	echo "5 - BUS_13.5m"
	echo "6 - BUS_15m"
	read RADAR_VARIANT_INt
		
	
	case $RADAR_VARIANT_INt in
	"1") 	VARIANT="40_42_44_46"
		MODEL_FILENAME="modelconfig_sil_40_42_44_46_5525_4X2_TT.yaml"
		break;;
	"2") 	VARIANT="4825_10X2"
		MODEL_FILENAME="modelconfig_sil_4825_10X2_DTLA.yaml"
		break;;
	"3") 	VARIANT="4825_10X2_HIL"
		MODEL_FILENAME="modelconfig_sil_4825_10X2_DTLA_HIL.yaml"
		break;;
	"4") 	VARIANT="bus_cheetah"
		MODEL_FILENAME="modelconfig_sil_AL_BUS_Cheetah.yaml"
		break;;
	"5") 	VARIANT="bus_13_5m"
		MODEL_FILENAME="modelconfig_sil_BUS_13.5m.yaml"
		break;;
	"6") 	VARIANT="bus_15m"
		MODEL_FILENAME="modelconfig_sil_BUS_15m.yaml"
		break;;
	esac
	
	LM2_PATH=$LM2_ROOT_PATH/$CUSTOMER/SRR_Master_LogicModel2_AL.fmu
    MODEL_PATH=$MODEL_ROOT_PATH/$CUSTOMER/$MODEL_FILENAME	
      
    break;;

* )
    echo "Invalid Entry Quitting build!!"
    exit
    break;;
esac

#cd "$PWD"




cp -a "01_VV_ENGINE/Binaries/Linux/." "$DOCKER_BIN/"
cp -a "01_VV_ENGINE/Config/VVEngineConfig.yaml" "$DOCKER_BIN/"
cp -a $LM2_PATH "$DOCKER_BIN/"
cp -a $MODEL_PATH "$DOCKER_BIN/"
date=$(date '+%Y%m%d%H%M%S')
customername=$CUST$RADAR_VARIANT$RV


NEW_MODEL_PATH="/Binaries/$MODEL_FILENAME"
python3 Update_configuration.py "$CUST_IN" "$NEW_MODEL_PATH"
python3 Update_Customer_Configuration.py "$CUST_IN" "$NEW_MODEL_PATH"

#Unzipping needs to be based on model config
python3 unzip.py "$CUST_IN"

echo FROM        ubuntu   >>Dockerfile
echo MAINTAINER  aptiv  >>Dockerfile
echo ARG DEBIAN_FRONTEND=noninteractive >>Dockerfile
echo RUN apt-get update >>Dockerfile
echo RUN apt-get -yq install gdbserver >>Dockerfile
echo RUN apt-get -yq install python3 >>Dockerfile

docker build -t ubuntu_base --network=host .
docker save -o ubuntu_base.tar ubuntu_base



DOCKER_PATH="08_CloudBinaries_${VARIANT}"
if [ -d $DOCKER_PATH ]; then
    echo " - $DOCKER_PATH already exists" 1>&2
fi

chmod +x createImage.sh
./createImage.sh $customername $VARIANT $date 

mkdir -p $DOCKER_PATH
mkdir -p "$DOCKER_PATH/INPUT_LOGS"
cp -a "01_VV_ENGINE/Sample_Trace" "$DOCKER_PATH/INPUT_LOGS/"
cp -a "01_VV_ENGINE/Config/Flist_file.txt" "$DOCKER_PATH/"
mkdir -p "$DOCKER_PATH/OUTPUT_DIR"

mv "GEN_LOG.sh" "$DOCKER_PATH/"
mv "Run_Singularity.sh" "$DOCKER_PATH/"
mv "resim_vv_${customername}_${VARIANT}_${date}.tar" "$DOCKER_PATH/"
mv "resim_vv_${customername}_${VARIANT}_${date}.simg" "$DOCKER_PATH/"

#rm -rf Binaries
rm -rf ubuntu_base.tar

