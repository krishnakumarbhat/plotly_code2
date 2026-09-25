#!/bin/bash

echo ###Starting execution of APTIV VV Engine###
echo

cd "$(pwd)/01_VV_ENGINE/Binaries/Linux"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$(pwd)"
./SensorModelSilEngine ../../Config/VVEngineConfig.yaml ../../Config/Flist_file.txt
