#!/bin/sh

#------CREATING  CONTAINER AND EXECUTING SET OF LOG IN CONTAINER---------

	docker run -it -v $(pwd)/INPUT_LOGS/:/input/ -v $(pwd)/:/output/ gen7_srr_resim:sensor-srr.008.000.051-apt_srr_resim_23_11_100_14 /RUN_RESIM.sh "$@" ;echo $?
#------------END OF SCRIPT-------------
