#!/bin/bash
# Builds the VV Engine profiling Singularity image.
# Called by Generate_Profiling_Singularity.sh.
# Usage: ./createImage_profiling.sh <customername> <variant> <engine_version>
#
# Expects in the same directory as this script:
#   RUN_PROFILING.sh              — embedded into the SIMG at /RUN_PROFILING.sh
#   Run_Singularity_Profiling.sh  — copied to cwd so Generate_Profiling_Singularity.sh can move it to the output folder
#
# Expects in the working directory (populated by Generate_Profiling_Singularity.sh):
#   Binaries/  — all runtime binaries including SensorModelSilEngine_memcheck
#
# Docker path:      uses ubuntu_profiling_base already built by Generate_Profiling_Singularity.sh.
# Singularity path: builds directly from a .def file (pulls ubuntu:latest, installs profiling tools inline).

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SIMG_NAME="resim_vv_$1_$2_$3_profiling"

[ -f "${SCRIPT_DIR}/RUN_PROFILING.sh" ]             || { echo "ERROR: RUN_PROFILING.sh not found next to createImage_profiling.sh"; exit 1; }
[ -f "${SCRIPT_DIR}/Run_Singularity_Profiling.sh" ] || { echo "ERROR: Run_Singularity_Profiling.sh not found next to createImage_profiling.sh"; exit 1; }

cp "${SCRIPT_DIR}/RUN_PROFILING.sh" .
chmod 777 RUN_PROFILING.sh

rm -f "${SIMG_NAME}.simg"

if command -v docker &>/dev/null; then
    echo ""
    echo "***** Docker build has started (profiling image) *****"
    docker rmi -f "${SIMG_NAME}:latest" >/dev/null 2>&1

    rm -f Dockerfile
    echo FROM  ubuntu_profiling_base   >>Dockerfile
    echo MAINTAINER  aptiv  >>Dockerfile
    echo COPY Binaries /Binaries >>Dockerfile
    echo COPY RUN_PROFILING.sh /RUN_PROFILING.sh >>Dockerfile
    echo RUN chmod -R a+rwX /Binaries >>Dockerfile
    echo RUN chmod +x /RUN_PROFILING.sh >>Dockerfile

    docker build -t "${SIMG_NAME}" --network=host .
    docker save -o "${SIMG_NAME}.tar" "${SIMG_NAME}"
    chmod 777 "${SIMG_NAME}.tar"
    rm -f Dockerfile
    echo "***** Docker build is done *****"
    echo ""

    echo "***** Building Singularity image from Docker tar *****"
    singularity build "${SIMG_NAME}.simg" "docker-archive://${SIMG_NAME}.tar"
    echo "***** Singularity profiling image build done *****"
    echo ""
else
    echo ""
    echo "***** Docker not found — building Singularity profiling image directly *****"

    rm -f singularity_profiling.def
    # NOTE: pulls the official ubuntu:latest image from Docker Hub (trusted registry).
    # Consider pinning to a digest (ubuntu:<tag>@sha256:...) for reproducible, verifiable provenance.
    cat >> singularity_profiling.def << 'SINGDEF'
Bootstrap: docker
From: ubuntu:latest

%files
    Binaries /Binaries
    RUN_PROFILING.sh /RUN_PROFILING.sh

%post
    apt-get update
    DEBIAN_FRONTEND=noninteractive apt-get -yq install \
        gdbserver python3 libpcap0.8t64 libatomic1 \
        valgrind heaptrack gcc
    chmod -R a+rwX /Binaries
    chmod +x /RUN_PROFILING.sh
SINGDEF

    singularity build --fakeroot "${SIMG_NAME}.simg" singularity_profiling.def
    chmod 777 "${SIMG_NAME}.simg" 2>/dev/null || true
    rm -f singularity_profiling.def
    echo "***** Singularity profiling image build done *****"
    echo ""
fi

cp "${SCRIPT_DIR}/Run_Singularity_Profiling.sh" .
chmod 777 Run_Singularity_Profiling.sh

rm -f RUN_PROFILING.sh
