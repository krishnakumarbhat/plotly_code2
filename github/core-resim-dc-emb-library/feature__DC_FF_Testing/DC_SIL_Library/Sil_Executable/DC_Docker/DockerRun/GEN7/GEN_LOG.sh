#!/bin/sh

#------CREATING  CONTAINER AND EXECUTING SET OF LOG IN CONTAINER---------

	docker run -it -v $(pwd)/INPUT_LOGS/:/input/ -v $(pwd)/:/output/ dc_031024:sensor-srr.008.000.051-apt_srr_resim_24_29_105_12 /RUN_RESIM.sh "$@" ;echo $?
#------------END OF SCRIPT-------------


