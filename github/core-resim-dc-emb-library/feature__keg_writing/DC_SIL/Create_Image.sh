# *================================================================================*
# * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
# * Confidential - Restricted Aptiv information. Do not disclose.                  *
# * Creator : Mandeep Singh (mandeep.singh1@aptiv.com)                             *
# *================================================================================*


#!/bin/sh

echo $@

tar="resim_v2_platform_$2_dc_$3.tar"
simg="resim_v2_platform_$2_dc_$3.simg"
# removing old docker files if exist
if [ -f "$tar" ];
then
	echo "removing old docker image .tar file"
	rm "$tar"
fi

# removing old singularity image if exist
if [ -f "$simg" ];
then
	echo "removing old singularity image"
	rm "$simg"
fi

# *********************************************** start of writing RUN_RESIM.sh

# creating script RUN_RESIM.sh, which will be part of docker and help to run docker.
# creating this file via script and will remove after getting copied to docker image 
echo '#!/bin/sh' >>RUN_RESIM.sh
echo '' >>RUN_RESIM.sh
echo '#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------' >>RUN_RESIM.sh
echo '' >>RUN_RESIM.sh
echo "# ch_ipop	:Flag to check Input and Output path provided )" >>RUN_RESIM.sh
echo ch_ipop=0 >>RUN_RESIM.sh
echo '' >>RUN_RESIM.sh
echo "# ch_optfw	:Flag to check if FW Config.xml is passed or not during docker run, this is optional." >>RUN_RESIM.sh
echo "# If passed, it will replaced the FW config.xml present inside docker image." >>RUN_RESIM.sh
echo ch_optfw=0 >>RUN_RESIM.sh
echo ''  >>RUN_RESIM.sh

echo if [ -f \"\$1\" ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "echo ''" >>RUN_RESIM.sh
echo "else" >>RUN_RESIM.sh
echo "       ch_ipop=1" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh

echo "echo \$ch_ipop" >>RUN_RESIM.sh

echo "#----------VALIDATING INPUT PARAMETERS-------------------------" >>RUN_RESIM.sh
echo if [ \$ch_ipop -eq 1 ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo "       echo \"[DOCKER] : INCORRECT PATH FOR PASSED ARGUMENT\"" >>RUN_RESIM.sh
echo "       exit \$exitcode" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo ''  >>RUN_RESIM.sh

echo "#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- " >>RUN_RESIM.sh
echo ''  >>RUN_RESIM.sh

echo exitcode=1 >>RUN_RESIM.sh
echo cd /Binaries/ >>RUN_RESIM.sh
echo export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:/Binaries/ >>RUN_RESIM.sh
echo export HDF5_DISABLE_VERSION_CHECK=2 >>RUN_RESIM.sh
echo "        ./APT_SRR_RESIM ./SIL_Engine_Config.xml \$1 \$2" >>RUN_RESIM.sh
echo          exitcode=\$? >>RUN_RESIM.sh
echo ''  >>RUN_RESIM.sh

echo if [ \$exitcode -eq 0 ] >>RUN_RESIM.sh
echo then >>RUN_RESIM.sh
echo '        echo "[DOCKER] : Exit Code - $exitcode -> ReSim Application Executed Successfully"' >>RUN_RESIM.sh
echo "        #exit 0" >>RUN_RESIM.sh
echo else >>RUN_RESIM.sh
echo '        echo "[DOCKER] : Exit Code - $exitcode -> ReSim Application Executed With Error"' >>RUN_RESIM.sh
echo "        #exit \$exitcode" >>RUN_RESIM.sh
echo fi >>RUN_RESIM.sh
echo ''  >>RUN_RESIM.sh

#docker rmi -f ubuntu_base
#docker load -i ubuntu_base.tar
chmod 777 RUN_RESIM.sh
# *********************************************** end of writing RUN_RESIM.sh

# *********************************************** start of writing Dockerfile

# Docker can build images automatically by reading the instructions from a Dockerfile.
# A Dockerfile is a text document that contains all the commands a user could call on the command line to assemble an image.
# creating this file via script and will remove after getting copied to docker image 

echo FROM  ubuntu_base   >>Dockerfile
echo MAINTAINER  aptiv  >>Dockerfile
echo COPY Binaries /Binaries >>Dockerfile
echo COPY RUN_RESIM.sh /RUN_RESIM.sh   >>Dockerfile
# *********************************************** end of writing Dockerfile

# *********************************************** start of Docker Image Build **************************************************************
# setting name to be used further in Docker image and .tar files.

docker rmi -f resim_v2_platform_$2_dc_$3:lm1-apt_srr_resim__lm1-sensor-csrr_ >/dev/null 2>&1

# docker build process starts here
echo ""
echo "***** Docker build has started *****"
docker build -t resim_v2_platform_$2_dc_$3 --network=host .
docker tag resim_v2_platform_$2_dc_$3:latest resim_v2_platform_$2_dc_$3:lm1-apt_srr_resim__lm1-sensor-csrr_
docker rmi -f resim_v2_platform_$2_dc_$3:latest
docker save -o resim_v2_platform_$2_dc_$3.tar resim_v2_platform_$2_dc_$3
chmod 777 resim_v2_platform_$2_dc_$3.tar
echo "***** Docker build is done *****"
echo ""

# *********************************************** end of Docker Image Build **************************************************************
# *********************************************** start of Singularity Image Build **************************************************************
# here we will create singularity image from docker.
# this is on customer demand
# code is commented currently

rm -rf *.simg	# removing any existing singularity image in current folder.
echo "***** building singularity image from docker *****"
singularity build resim_v2_platform_$2_dc_$3.simg docker-archive://resim_v2_platform_$2_dc_$3.tar # building singularity image from docker image
echo "***** singularity build image is done *****"
echo ""

# *********************************************** end of Singularity Image Build **************************************************************
# *********************************************** start of GEN_LOG.sh script, required to run docker **************************************************************
rm -rf ./GEN_LOG.sh
echo "#!/bin/sh" >> GEN_LOG.sh
echo "" >> GEN_LOG.sh
echo "#------CREATING  CONTAINER AND EXECUTING SET OF LOG IN CONTAINER---------" >> GEN_LOG.sh
echo "" >> GEN_LOG.sh
echo "	docker run -it -v \$(pwd)/INPUT_LOGS/:/input/ -v \$(pwd)/:/output/ resim_v2_platform_$2_dc_$3:lm1-apt_srr_resim__lm1-sensor-csrr_ /RUN_RESIM.sh \"\$@\" ;echo \$?" >> GEN_LOG.sh
echo "" >> GEN_LOG.sh
echo "#------------END OF SCRIPT-------------" >> GEN_LOG.sh
chmod 777 GEN_LOG.sh
# *********************************************** end of GEN_LOG.sh script, required to run docker **************************************************************

# *********************************************** end of Singularity Image Build **************************************************************
# *********************************************** start of Run_Singularity.sh script, required to run singularity **************************************************************
rm -rf ./Run_Singularity.sh
echo "#!/bin/sh" >> Run_Singularity.sh
echo "" >> Run_Singularity.sh
echo "#------CREATING  Run_Singularity.sh command---------" >> Run_Singularity.sh
echo "" >> Run_Singularity.sh
echo "export HDF5_DISABLE_VERSION_CHECK=2" >> Run_Singularity.sh
echo "singularity exec \$(pwd)/resim_v2_platform_$2_dc_$3.simg /RUN_RESIM.sh \$(pwd)/SIL_Input.txt \$(pwd)/OUTPUT_DIR" >> Run_Singularity.sh
echo "" >> Run_Singularity.sh
echo "#------------END OF SCRIPT-------------" >> Run_Singularity.sh
chmod 777 Run_Singularity.sh
# *********************************************** end of Run_Singularity.sh script, required to run docker **************************************************************


rm -rf Dockerfile 		# removing Dockerfile 
rm -rf RUN_RESIM.sh		# removing RUN_RESIM.sh
