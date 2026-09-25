#!/bin/sh

#------CREATING  CONTAINER AND EXECUTING SET OF LOG IN CONTAINER---------

	docker run -it -v $(pwd)/INPUT_LOGS/:/input/ -v $(pwd)/:/output/ resim_v2_platform_srr_dc_20260116122250:lm1-apt_srr_resim__lm1-sensor-csrr_ /RUN_RESIM.sh "$@" ;echo $?

#------------END OF SCRIPT-------------
