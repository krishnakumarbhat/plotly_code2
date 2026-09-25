#!/bin/sh

#------CREATING  CONTAINER AND EXECUTING SET OF LOG IN CONTAINER---------

	docker run -it -v $(pwd)/INPUT_LOGS/:/input/ -v $(pwd)/:/output/ tools_bordnet:V1 /RUN_BORDNET.sh $1 ;echo $?
#------------END OF SCRIPT-------------
