#!/usr/bin/bash

# script for generation release information text file


set -Eeuo pipefail

if test $# -ne 4
then
    echo "This script requires following arguments and returns release info to stdout:"
    echo
    echo "version      - string describing sw version (should contain at least major.minor.patch numbers)"
    echo "variant      - variant name (defined in Polarion CMP_SRS_StationaryGeometries)"
    echo "branch       - name of branch (e.g. dev) from which package is generated"
    echo "commit_hash  - full commit hash of the change used in release package"
    echo
    echo "All arguments are mandatory and passed as text."
    echo
    echo "Usage:"
    echo "./generate_release_info.sh version variant branch commit_hash"
    echo "Example:"
    echo "./generate_release_info.sh SG_01.00.00 PC_GCC_default dev 0631d82b62ad520bc96ce55051cbfa02008682cc"
    echo
else
    VERSION=$1
    SW_VARIANT=$2
    BRANCH=$3
    FULL_COMMIT_HASH=$4
    SHORT_COMMIT_HASH=`echo $FULL_COMMIT_HASH | cut -c1-8`

    # then do the job
    echo "Stationary geometries version & variant information"
    echo -n "Version: "
    echo ${VERSION}
    echo -n "Variant: "
    echo ${SW_VARIANT}
    echo

    echo "Version control information"
    echo -n "Branch: "
    echo ${BRANCH}
    echo -n "Short commit hash: "
    echo ${SHORT_COMMIT_HASH}
    echo -n "Full commit hash: "
    echo ${FULL_COMMIT_HASH}
    echo

    echo "Release package generation time"
    echo -n "ISO date: "
    echo `date -I`
    echo -n "Human readable date & time (local time of Jenkins server instance): "
    echo `date -R`
fi
