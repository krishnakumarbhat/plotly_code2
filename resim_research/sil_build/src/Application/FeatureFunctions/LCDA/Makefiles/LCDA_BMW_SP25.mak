##############################################################################
#
# COPYRIGHT, 2021, Aptiv All Rights reserved
#
##############################################################################

SUBDIRS += $(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Source
SUBDIRS += $(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Customer_Adapter
SUBDIRS += $(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Customer_Adapter/BMW_SP25
SUBDIRS += $(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Calibration/LCDA_Core
SUBDIRS += $(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Calibration/BMW_SP25

INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Source
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Customer_Adapter
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Customer_Adapter/BMW_SP25
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Calibration/LCDA_Core
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA/Calibration/BMW_SP25

######## Generate linker files in _lnk folder  #######
######## Feature and customer specific part    #######

FF_CAL_SUBDIRS = ""
FF_OTHER_SUBDIRS = ""
FF_OTHER_SUBDIRS += Source
FF_OTHER_SUBDIRS += Customer_Adapter
FF_OTHER_SUBDIRS += Customer_Adapter/BMW_SP25
FF_CAL_SUBDIRS += Calibration/LCDA_Core
FF_CAL_SUBDIRS += Calibration/BMW_SP25
LNK_FILE_PATH_BEGIN = _lnk/BMW_SP25/core2_lcda

######## Generate linker files in _lnk folder  #######
######## Feature and customer independent part #######

FF_ALL_SUBDIRS = ${FF_CAL_SUBDIRS} ${FF_OTHER_SUBDIRS}

# Set FF_PATH to the parent directory of current make file directory
FF_PATH=$(abspath $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))/..)

# Add all source c files from calibration folders, replace the suffix with .o
FF_CAL_OBJECTS = $(foreach SUB_DIR,$(FF_CAL_SUBDIRS),$(patsubst %.c,%.o,$(wildcard ${FF_PATH}/$(SUB_DIR)/*.c)))
FF_CAL_OBJECTS := $(notdir ${FF_CAL_OBJECTS})

# Add all source c files from all other folders, replace the suffix with .o
FF_OTHER_OBJECTS = $(foreach SUB_DIR,$(FF_OTHER_SUBDIRS),$(patsubst %.c,%.o,$(wildcard ${FF_PATH}/$(SUB_DIR)/*.c)))
FF_OTHER_OBJECTS := $(notdir ${FF_OTHER_OBJECTS})

FF_ALL_OBJECTS = ${FF_CAL_OBJECTS} ${FF_OTHER_OBJECTS}

# generating bss-file
BSS_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_bss.lcf
$(file >${BSS_FILE_PATH},/*****************************************************************************)
$(file >>${BSS_FILE_PATH},* COPYRIGHT, 2021, Aptiv All Rights reserved)
$(file >>${BSS_FILE_PATH},*****************************************************************************/)
$(file >>${BSS_FILE_PATH},)
$(foreach OBJECT,$(FF_ALL_OBJECTS),$(file >>${BSS_FILE_PATH},${OBJECT} (.bss)))

# generating data-file
DATA_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_data.lcf
$(file >${DATA_FILE_PATH},/*****************************************************************************)
$(file >>${DATA_FILE_PATH},* COPYRIGHT, 2021, Aptiv All Rights reserved)
$(file >>${DATA_FILE_PATH},*****************************************************************************/)
$(file >>${DATA_FILE_PATH},)
$(foreach OBJECT,$(FF_OTHER_OBJECTS),$(file >>${DATA_FILE_PATH},${OBJECT} (.data)))

# generating cal-data-file
CAL_DATA_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_cal_data.lcf
$(file >${CAL_DATA_FILE_PATH},/*****************************************************************************)
$(file >>${CAL_DATA_FILE_PATH},* COPYRIGHT, 2021, Aptiv All Rights reserved)
$(file >>${CAL_DATA_FILE_PATH},*****************************************************************************/)
$(file >>${CAL_DATA_FILE_PATH},)
$(foreach OBJECT,$(FF_CAL_OBJECTS),$(file >>${CAL_DATA_FILE_PATH},${OBJECT} (.data)))

# generating txt-file
TXT_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_txt.lcf
$(file >${TXT_FILE_PATH},/*****************************************************************************)
$(file >>${TXT_FILE_PATH},* COPYRIGHT, 2021, Aptiv All Rights reserved)
$(file >>${TXT_FILE_PATH},*****************************************************************************/)
$(file >>${TXT_FILE_PATH},)
$(foreach OBJECT,$(FF_ALL_OBJECTS),$(file >>${TXT_FILE_PATH},${OBJECT} (.text_vle)))
