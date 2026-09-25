#!/bin/sh

echo '#!/bin/sh' >>RUN_RESIM.sh
echo '\n' >>RUN_RESIM.sh
echo '#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------' >>RUN_RESIM.sh
echo '\n' >>RUN_RESIM.sh
echo ch_k=0 >>RUN_RESIM.sh
echo ch_f=0 >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh

echo if [  -f \"\$1\"  \-a \-d \"\$2\"  ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "echo \" \"" >>RUN_RESIM.sh
echo "else" >>RUN_RESIM.sh
echo "       ch_k=1" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh

echo if [ ! -f \"\$3\" ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "       ch_f=1" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh

echo "echo \$ch_k" >>RUN_RESIM.sh

echo exitcode=1 >>RUN_RESIM.sh
echo "#----------VALIDATING INPUT PARAMETERS-------------------------" >>RUN_RESIM.sh
echo if [ \$ch_k -eq 1 ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "       echo "INCORRECT FILE PATH "" >>RUN_RESIM.sh
echo "       exit \$exitcode" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh

echo "#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- " >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh

echo cd /OUTPUT/ >>RUN_RESIM.sh
echo export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:./ >>RUN_RESIM.sh
echo chmod +777 * >>RUN_RESIM.sh
echo if [ \$ch_f -eq 1 ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "        ./APT_SRR_RESIM /./sil_engine_config.xml \$1 \$2" >>RUN_RESIM.sh
echo else >>RUN_RESIM.sh
echo "        ./APT_SRR_RESIM \$3 \$1 \$2" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo exitcode=\$? >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh


echo if [ \$exitcode -eq 0 ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "        echo "Application Executed Successfully"" >>RUN_RESIM.sh
echo "        exit 0" >>RUN_RESIM.sh 
echo else >>RUN_RESIM.sh
echo "        echo "Application Executed With Error"" >>RUN_RESIM.sh
echo "        exit \$exitcode" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo '\n'  >>RUN_RESIM.sh

docker load -i dc_base.tar
chmod 777 RUN_RESIM.sh

echo FROM       dc_base >>Dockerfile
echo MAINTAINER  aptiv  >>Dockerfile
echo COPY OUTPUT /OUTPUT >>Dockerfile
echo COPY OUTPUT /OUTPUT >>Dockerfile
echo COPY RUN_RESIM.sh /RUN_RESIM.sh   >>Dockerfile
echo COPY DC_Lib_Control.xml /DC_Lib_Control.xml   >>Dockerfile
echo COPY BMW_SRR5_Input.json /BMW_SRR5_Input.json  >>Dockerfile
echo COPY Emb_Lib_Config.xml /Emb_Lib_Config.xml   >>Dockerfile
echo COPY SIL_Engine_Config.xml /sil_engine_config.xml   >>Dockerfile
echo COPY SM_Config /SM_Config >>Dockerfile


s_version=`grep s_version version.txt | cut -d ":" -f2`
release_version=`grep release_version version.txt | cut -d ":" -f2`

iName="dc_031024"

docker rmi -f $iName:sensor-srr.$s_version\-apt_srr_resim_$release_version >/dev/null 2>&1

docker build -t $iName --network=host .
docker tag $iName:latest $iName:sensor-srr.$s_version\-apt_srr_resim_$release_version
docker rmi -f $iName:latest
docker save -o $iName.tar $iName
chmod 777 $iName.tar
rm -rf *.simg
singularity build $iName.simg docker-archive://$iName.tar
#rm -rf Dockerfile
#rm -rf RUN_RESIM.sh
