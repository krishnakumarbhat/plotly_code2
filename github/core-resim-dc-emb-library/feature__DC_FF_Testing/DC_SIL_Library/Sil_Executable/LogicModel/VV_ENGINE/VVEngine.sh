#!/bin/bash

echo ###Starting execution of APTIV VV Engine###
echo

cd "$(pwd)/Binaries/Linux"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$(pwd)"
./SensorModelSilEngine ../../Config/VVEngineConfig.yaml ../../Config/Flist_file.txt
