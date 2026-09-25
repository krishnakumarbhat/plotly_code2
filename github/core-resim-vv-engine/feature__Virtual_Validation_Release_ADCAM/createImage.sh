# *================================================================================*
# * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
# * Confidential - Restricted Aptiv information. Do not disclose.                  *
# * Creator : Mandeep Singh (mandeep.singh1@aptiv.com)                             *
# *================================================================================*


#!/bin/sh

echo $@

tar="resim_vv_$1_$2_$3.tar"
simg="resim_vv_$1_$2_$3.simg"
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

# *********************************************** start of writing RUN_VV.sh

# creating script RUN_VV.sh, which will be part of docker and help to run docker.
# creating this file via script and will remove after getting copied to docker image 
echo '#!/bin/sh' >>RUN_VV.sh
echo '' >>RUN_VV.sh
echo '#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------' >>RUN_VV.sh
echo '' >>RUN_VV.sh
echo "# ch_ipop	:Flag to check Input and Output path provided )" >>RUN_VV.sh
echo ch_ipop=0 >>RUN_VV.sh
echo '' >>RUN_VV.sh
echo "# ch_optfw	:Flag to check if FW Config.xml is passed or not during docker run, this is optional." >>RUN_VV.sh
echo "# If passed, it will replaced the FW config.xml present inside docker image." >>RUN_VV.sh
echo ch_optfw=0 >>RUN_VV.sh
echo ''  >>RUN_VV.sh

echo if [ -f \"\$1\" ] >>RUN_VV.sh
echo then >>RUN_VV.sh
echo "echo ''" >>RUN_VV.sh
echo "else" >>RUN_VV.sh
echo "       ch_ipop=1" >>RUN_VV.sh
echo fi >>RUN_VV.sh

echo "echo \$ch_ipop" >>RUN_VV.sh

echo "#----------VALIDATING INPUT PARAMETERS-------------------------" >>RUN_VV.sh
echo if [ \$ch_ipop -eq 1 ] >>RUN_VV.sh
echo then >>RUN_VV.sh
echo "       echo \"[DOCKER] : INCORRECT PATH FOR PASSED ARGUMENT\"" >>RUN_VV.sh
echo "       exit \$exitcode" >>RUN_VV.sh
echo fi >>RUN_VV.sh
echo ''  >>RUN_VV.sh

echo "#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- " >>RUN_VV.sh
echo ''  >>RUN_VV.sh

echo exitcode=1 >>RUN_VV.sh
echo "# Resolve input/output to absolute paths before cd changes the working directory" >>RUN_VV.sh
echo 'case "$1" in /*) FLIST="$1" ;; *) FLIST="$(pwd)/$1" ;; esac' >>RUN_VV.sh
echo 'case "$2" in /*) OUTDIR="$2" ;; *) OUTDIR="$(pwd)/$2" ;; esac' >>RUN_VV.sh
echo '' >>RUN_VV.sh
echo '# Check if /Binaries/ is writable (requires --writable-tmpfs at launch).' >>RUN_VV.sh
echo '# If not, LM2 FMUs cannot write cp_buffer.bin — warn and continue.' >>RUN_VV.sh
echo '# Use Run_Singularity.sh (which adds --writable-tmpfs) to get full output.' >>RUN_VV.sh
echo 'if ! touch /Binaries/.write_test 2>/dev/null; then' >>RUN_VV.sh
echo '    echo "[WARN] /Binaries/ is read-only. LM2 working-dir outputs (cp_buffer.bin etc.) will not be saved."' >>RUN_VV.sh
echo '    echo "[WARN] Use Run_Singularity.sh or add --writable-tmpfs to your singularity exec command."' >>RUN_VV.sh
echo 'else' >>RUN_VV.sh
echo '    rm -f /Binaries/.write_test' >>RUN_VV.sh
echo 'fi' >>RUN_VV.sh
echo '' >>RUN_VV.sh
echo cd /Binaries/ >>RUN_VV.sh
echo export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:/Binaries/ >>RUN_VV.sh
echo export HDF5_DISABLE_VERSION_CHECK=2 >>RUN_VV.sh
echo "        ./SensorModelSilEngine ./VVEngineConfig.yaml \$FLIST \$OUTDIR" >>RUN_VV.sh
echo          exitcode=\$? >>RUN_VV.sh
echo ''  >>RUN_VV.sh

echo if [ \$exitcode -eq 0 ] >>RUN_VV.sh
echo then >>RUN_VV.sh
echo '        echo "[DOCKER] : Exit Code - $exitcode -> ReSim Application Executed Successfully"' >>RUN_VV.sh
echo "        #exit 0" >>RUN_VV.sh
echo else >>RUN_VV.sh
echo '        echo "[DOCKER] : Exit Code - $exitcode -> ReSim Application Executed With Error"' >>RUN_VV.sh
echo "        #exit \$exitcode" >>RUN_VV.sh
echo fi >>RUN_VV.sh
echo ''  >>RUN_VV.sh

# Copy LM2 working-directory outputs (e.g. cp_buffer.bin, interface files) to
# the host OUTPUT_DIR. /Binaries/LogicModelId* is writable via --writable-tmpfs
# during the run; copying here ensures outputs survive after the container exits.
echo '# Copy LM2 working-dir outputs to host OUTPUT_DIR' >>RUN_VV.sh
echo 'for logicdir in /Binaries/LogicModelId*/; do' >>RUN_VV.sh
echo '    [ -d "$logicdir" ] || continue' >>RUN_VV.sh
echo '    dirname=$(basename "$logicdir")' >>RUN_VV.sh
echo '    mkdir -p "$OUTDIR/$dirname"' >>RUN_VV.sh
echo '    cp -a "${logicdir}." "$OUTDIR/$dirname/"' >>RUN_VV.sh
echo 'done' >>RUN_VV.sh
echo ''  >>RUN_VV.sh
#docker load -i ubuntu_base.tar
chmod 777 RUN_VV.sh
# *********************************************** end of writing RUN_VV.sh

# *********************************************** start of image build **************************************************************
# Two paths depending on what is available on the build machine:
#   1. Docker available (typical WSL with Docker Desktop): build Docker image, save .tar,
#      convert to Singularity via docker-archive. The .tar is also kept as a deliverable.
#   2. Docker NOT available (bare Linux / HPC build node): build Singularity directly
#      from a .def file that pulls ubuntu:latest from DockerHub. No .tar produced.

rm -rf *.simg

if command -v docker &>/dev/null; then
    # ---- Docker path ----
    echo ""
    echo "***** Docker build has started *****"
    docker rmi -f resim_vv_$1_$2_$3:lm1-apt_srr_resim__lm1-sensor-csrr_ >/dev/null 2>&1

    rm -f Dockerfile
    echo FROM  ubuntu_base   >>Dockerfile
    echo MAINTAINER  aptiv  >>Dockerfile
    echo COPY Binaries /Binaries >>Dockerfile
    echo COPY RUN_VV.sh /RUN_VV.sh   >>Dockerfile
    echo RUN chmod -R a+rwX /Binaries >>Dockerfile

    docker build -t resim_vv_$1_$2_$3 --network=host .
    docker tag resim_vv_$1_$2_$3:latest resim_vv_$1_$2_$3:lm1-apt_srr_resim__lm1-sensor-csrr_
    docker rmi -f resim_vv_$1_$2_$3:latest
    docker save -o resim_vv_$1_$2_$3.tar resim_vv_$1_$2_$3
    chmod 777 resim_vv_$1_$2_$3.tar
    echo "***** Docker build is done *****"
    echo ""

    echo "***** Building Singularity image from Docker tar *****"
    singularity build resim_vv_$1_$2_$3.simg docker-archive://resim_vv_$1_$2_$3.tar
    echo "***** Singularity image build is done *****"
    echo ""

    rm -f Dockerfile
else
    # ---- Singularity-only path (no Docker daemon required) ----
    # Pulls ubuntu:latest from DockerHub; copies Binaries/ and RUN_VV.sh into the image.
    echo ""
    echo "***** Docker not found — building Singularity image directly from definition file *****"

    rm -f singularity_build.def
    # NOTE: pulls the official ubuntu:latest image from Docker Hub (trusted registry).
    # Consider pinning to a digest (ubuntu:<tag>@sha256:...) for reproducible, verifiable provenance.
    cat >> singularity_build.def << 'SINGDEF'
Bootstrap: docker
From: ubuntu:latest

%files
    Binaries /Binaries
    RUN_VV.sh /RUN_VV.sh

%post
    apt-get update
    DEBIAN_FRONTEND=noninteractive apt-get -yq install gdbserver python3 libpcap0.8t64 libatomic1
    chmod -R a+rwX /Binaries
    chmod +x /RUN_VV.sh
SINGDEF

    singularity build --fakeroot resim_vv_$1_$2_$3.simg singularity_build.def
    chmod 777 resim_vv_$1_$2_$3.simg 2>/dev/null || true
    rm -f singularity_build.def
    echo "***** Singularity image build is done *****"
    echo ""
fi
# *********************************************** end of image build **************************************************************
# *********************************************** start of GEN_LOG.sh script, required to run docker **************************************************************
rm -rf ./GEN_LOG.sh
echo "#!/bin/sh" >> GEN_LOG.sh
echo "" >> GEN_LOG.sh
echo "#------CREATING  CONTAINER AND EXECUTING SET OF LOG IN CONTAINER---------" >> GEN_LOG.sh
echo "" >> GEN_LOG.sh
echo "	docker run -it -v \$(pwd)/INPUT_LOGS/:/input/ -v \$(pwd)/:/output/ resim_vv_$1_$2_$3:lm1-apt_srr_resim__lm1-sensor-csrr_ /RUN_VV.sh \"\$@\" ;echo \$?" >> GEN_LOG.sh
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
echo "singularity exec \$(pwd)/resim_vv_$1_$2_$3.simg /RUN_VV.sh \$(pwd)/Flist_file.txt \$(pwd)/OUTPUT_DIR" >> Run_Singularity.sh
echo "" >> Run_Singularity.sh
echo "#------------END OF SCRIPT-------------" >> Run_Singularity.sh
chmod 777 Run_Singularity.sh
# *********************************************** end of Run_Singularity.sh script, required to run docker **************************************************************


rm -rf RUN_VV.sh		# removing RUN_VV.sh
