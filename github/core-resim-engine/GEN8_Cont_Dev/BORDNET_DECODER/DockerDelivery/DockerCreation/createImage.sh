#!/bin/sh

mkdir OUTPUT
cd ../../CMake/build
rm -rf * >/dev/null
echo "BORDNET Build Process Started: "
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
cd ../../output
cp -r BordNetDecoder *.so ../DockerDelivery/DockerCreation/OUTPUT
cd ../DockerDelivery/DockerCreation/

echo '#!/bin/sh' >>RUN_BORDNET.sh
echo '\n' >>RUN_BORDNET.sh
echo '#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS INSIDE CONTAINER-------' >>RUN_BORDNET.sh
echo '\n' >>RUN_BORDNET.sh
echo ch_k=0 >>RUN_BORDNET.sh
echo '\n'  >>RUN_BORDNET.sh

echo if [  -f \"\$1\" ] >>RUN_BORDNET.sh
echo then >>RUN_BORDNET.sh
echo "echo \" \"" >>RUN_BORDNET.sh
echo "else" >>RUN_BORDNET.sh
echo "       ch_k=1" >>RUN_BORDNET.sh
echo fi >>RUN_BORDNET.sh
echo '\n'  >>RUN_BORDNET.sh

echo exitcode=1 >>RUN_BORDNET.sh
echo "#----------VALIDATING INPUT PARAMETERS-------------------------" >>RUN_BORDNET.sh
echo if [ \$ch_k -eq 1 ] >>RUN_BORDNET.sh
echo then >>RUN_BORDNET.sh
echo "       echo "INCORRECT COFIGURATION FILE PATH "" >>RUN_BORDNET.sh
echo "       exit \$exitcode" >>RUN_BORDNET.sh
echo fi >>RUN_BORDNET.sh
echo '\n'  >>RUN_BORDNET.sh

echo "#--------------Running Application inside container-------------------- " >>RUN_BORDNET.sh
echo '\n'  >>RUN_BORDNET.sh

echo cd /OUTPUT/ >>RUN_BORDNET.sh
echo export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:./ >>RUN_BORDNET.sh
echo "./BordNetDecoder \$1" >>RUN_BORDNET.sh
echo exitcode=\$? >>RUN_BORDNET.sh
echo '\n'  >>RUN_BORDNET.sh

echo if [ \$exitcode -eq 0 ] >>RUN_BORDNET.sh
echo then >>RUN_BORDNET.sh
echo "        echo "Traces Generated Successfully"" >>RUN_BORDNET.sh
echo "        exit 0" >>RUN_BORDNET.sh 
echo else >>RUN_BORDNET.sh
echo "        echo "Traces generated With Errror"" >>RUN_BORDNET.sh
echo "        exit \$exitcode" >>RUN_BORDNET.sh
echo fi >>RUN_BORDNET.sh
echo '\n'  >>RUN_BORDNET.sh
chmod 777 RUN_BORDNET.sh
echo FROM        ubuntu   >>Dockerfile
echo MAINTAINER  aptiv  >>Dockerfile
echo COPY OUTPUT /OUTPUT >>Dockerfile
echo COPY RUN_BORDNET.sh /RUN_BORDNET.sh   >>Dockerfile

release_version=`grep release_version version.txt | cut -d ":" -f2`

iName="tools_bordnet"

docker rmi -f tools_bordnet:$release_version >/dev/null 2>&1

docker build -t $iName --network=host .
docker tag tools_bordnet:latest tools_bordnet:$release_version
docker rmi -f tools_bordnet:latest
docker save -o tools_bordnet.tar tools_bordnet
chmod 777 tools_bordnet.tar
rm -rf Dockerfile
rm -rf RUN_BORDNET.sh
varDate=`date +'%m_%d_%Y'`
rm -rf ../BORDENT_DOCKER_$varDate >>/tmp/temp.txt
mkdir ../BORDENT_DOCKER_$varDate
mkdir ../BORDENT_DOCKER_$varDate/INPUT_LOGS
mkdir ../BORDENT_DOCKER_$varDate/INPUT_LOGS/log_test
mkdir ../BORDENT_DOCKER_$varDate/OUTPUT_DIR
cp -r *.tar ../BORDENT_DOCKER_$varDate
rm -rf *.tar
rm -rf OUTPUT
cd ../../Inputs/
cp -r *.xml *.json ../DockerDelivery/BORDENT_DOCKER_$varDate/INPUT_LOGS