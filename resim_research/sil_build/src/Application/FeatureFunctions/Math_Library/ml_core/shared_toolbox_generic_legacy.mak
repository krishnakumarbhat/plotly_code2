########################################################################
# Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
# Confidential - Restricted Aptiv information. Do not disclose.
########################################################################

## Set CWD to the current make files directory
CWD=$(abspath $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))
include $(CWD)/shared_toolbox_generic.mak

SHARED_TOOLBOX_INCLUDE_DIR += $(SHARED_TOOLBOX_INCLUDE_DIR_PREPEND)include/legacy

SHARED_TOOLBOX_SUBDIRS += $(SHARED_TOOLBOX_SUBDIRS_PREPEND)src/legacy
SHARED_TOOLBOX_SUBDIRS += $(SHARED_TOOLBOX_SUBDIRS_PREPEND)include/legacy

