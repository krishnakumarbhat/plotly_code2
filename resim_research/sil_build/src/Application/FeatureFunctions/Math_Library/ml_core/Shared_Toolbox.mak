########################################################################
# Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
# Confidential - Restricted Aptiv information. Do not disclose.
########################################################################

## Set CWD to the current make files directory
CWD=$(abspath $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))

# Configure the MathLibrary make variables
SHARED_TOOLBOX_SUBDIRS_PREPEND = $(APPLICATION_DIR)/RR_Z2/RR_Z2_SRR/Application/Shared_Toolbox/Source/
SHARED_TOOLBOX_INCLUDE_DIR_PREPEND = -I$(APPLICATION_DIR)/RR_Z2/RR_Z2_SRR/Application/Shared_Toolbox/Source/
include $(CWD)/shared_toolbox_generic_legacy.mak

SUBDIRS += $(SHARED_TOOLBOX_SUBDIRS)
INCLUDE_DIR += $(SHARED_TOOLBOX_INCLUDE_DIR)
