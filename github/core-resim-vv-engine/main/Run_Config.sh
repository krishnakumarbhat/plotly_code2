#!/bin/bash
echo "-------------------------------------------------------------"
echo "             /\   |  __ \__   __|_   _\ \    / / "
echo "            /  \  | |__) | | |    | |  \ \  / /  "
echo "           / /\ \ |  ___/  | |    | |   \ \/ /   "
echo "          / ____ \| |      | |   _| |_   \  /    "
echo "         /_/    \_\_|      |_|  |_____|   \/     "
echo "           VV_ENGINE SRR Virtual Validation        " 
echo "-------------------------------------------------------------"
echo ""

echo "-------------------------------------------------------------"
echo "1: BMW SIL_LOW Execution	Loading default config files"
echo "2: BMW SIL_MID Execution	Loading default config files"
echo "3: BMW SIL_HIGH Execution	Loading default config files"
echo "-------------------------------------------------------------"

read preset

if [ $preset -eq '1' ]; then
	echo "PWD: " $PWD
	sed -i '/^VVEngine:/{n;n;n;s/SpecialConfigPath:.*/SpecialConfigPath: "Config\/Linux\/SRR_SM2_LM2_MODEL_CONFIG.yaml"/;}' Config/VVEngineConfig.yaml
elif [ $preset -eq '2' ]; then
	echo "PWD: " $PWD
	#sed -i '/^VVEngine:/{n;s/Version:.*/Version: "2.0"/;}' VVEngineConfig.yaml
	sed -i '/^VVEngine:/{n;n;n;s/SpecialConfigPath:.*/SpecialConfigPath: "Config\/Linux\/SRR_SM2_LM2_MODEL_CONFIG_MID.yaml"/;}' Config/VVEngineConfig.yaml
elif [ $preset -eq '3' ]; then
	echo "PWD: " $PWD
	sed -i '/^VVEngine:/{n;n;n;s/SpecialConfigPath:.*/SpecialConfigPath: "Config\/Linux\/SRR_SM2_LM2_MODEL_CONFIG_HIGH.yaml"/;}' Config/VVEngineConfig.yaml
fi

cd output/x64
./SensorModelSilEngine ../../Config/VVEngineConfig.yaml ../../Blist_file.txt

echo "================================================================================="
echo "                          Execution Completed                                    "
echo "================================================================================="
