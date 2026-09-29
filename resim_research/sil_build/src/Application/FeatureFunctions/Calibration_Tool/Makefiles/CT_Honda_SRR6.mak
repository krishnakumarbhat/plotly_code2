##############################################################################
#
# COPYRIGHT, 2024, Aptiv All Rights reserved
#
##############################################################################

SUBDIRS += $(APPLICATION_DIR)/z7b/Feature_Functions/Calibration_Tool/c_src

INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/z7b/Feature_Functions/Calibration_Tool/c_src

######## Generate linker files in _lnk folder  #######
######## Feature and customer specific part    #######

FF_CAL_SUBDIRS = ""
FF_OTHER_SUBDIRS = ""
FF_OTHER_SUBDIRS += c_src
FF_CAL_SUBDIRS = ""
LNK_FILE_PATH_BEGIN = _lnk/Honda_SRR6/CT_Calibration_Tool

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
$(file >>${BSS_FILE_PATH},* COPYRIGHT, 2024, Aptiv All Rights reserved)
$(file >>${BSS_FILE_PATH},*****************************************************************************/)
$(file >>${BSS_FILE_PATH},)
$(foreach OBJECT,$(FF_ALL_OBJECTS),$(file >>${BSS_FILE_PATH},${OBJECT} (.bss)))

# generating data-file
DATA_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_data.lcf
$(file >${DATA_FILE_PATH},/*****************************************************************************)
$(file >>${DATA_FILE_PATH},* COPYRIGHT, 2024, Aptiv All Rights reserved)
$(file >>${DATA_FILE_PATH},*****************************************************************************/)
$(file >>${DATA_FILE_PATH},)
$(foreach OBJECT,$(FF_OTHER_OBJECTS),$(file >>${DATA_FILE_PATH},${OBJECT} (.data)))

# generating cal-data-file
CAL_DATA_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_cal_data.lcf
$(file >${CAL_DATA_FILE_PATH},/*****************************************************************************)
$(file >>${CAL_DATA_FILE_PATH},* COPYRIGHT, 2024, Aptiv All Rights reserved)
$(file >>${CAL_DATA_FILE_PATH},*****************************************************************************/)
$(file >>${CAL_DATA_FILE_PATH},)
$(foreach OBJECT,$(FF_CAL_OBJECTS),$(file >>${CAL_DATA_FILE_PATH},${OBJECT} (.data)))

# generating txt-file
TXT_FILE_PATH = ${FF_PATH}/${LNK_FILE_PATH_BEGIN}_txt.lcf
$(file >${TXT_FILE_PATH},/*****************************************************************************)
$(file >>${TXT_FILE_PATH},* COPYRIGHT, 2024, Aptiv All Rights reserved)
$(file >>${TXT_FILE_PATH},*****************************************************************************/)
$(file >>${TXT_FILE_PATH},)
$(foreach OBJECT,$(FF_ALL_OBJECTS),$(file >>${TXT_FILE_PATH},${OBJECT} (.text_vle)))
