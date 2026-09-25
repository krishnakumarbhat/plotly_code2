#!/bin/bash

cd "$(pwd)/VV_ENGINE/"
sudo sh ./VVEngine.sh

cd "../../../"
echo "deleting extracted Logic Model and Sensor Model"
rm -rf "${pwd}/LM2_FMU/Linux/LogicalModel"
rm -rf "${pwd}/SM2_FMU/Linux/Model2Id0"
rm -rf "${pwd}/SM2_FMU/Linux/Model2Id1"
rm -rf "${pwd}/SM2_FMU/Linux/Model2Id2"
rm -rf "${pwd}/SM2_FMU/Linux/Model2Id3"
rm -rf "${pwd}/SM2_FMU/Linux/Model2Id4"