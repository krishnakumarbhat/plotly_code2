#!/bin/sh

#------CREATING  Run_Singularity.sh command---------

export HDF5_DISABLE_VERSION_CHECK=2
singularity exec $(pwd)/resim_v2_platform_srr_dc_20260116122250.simg /RUN_RESIM.sh $(pwd)/SIL_Input.txt $(pwd)/OUTPUT_DIR

#------------END OF SCRIPT-------------
