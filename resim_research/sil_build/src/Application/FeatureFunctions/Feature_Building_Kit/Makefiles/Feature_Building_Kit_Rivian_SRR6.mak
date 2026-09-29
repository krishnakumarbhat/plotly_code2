##############################################################################
#
# COPYRIGHT, 2021, Aptiv All Rights reserved
#
##############################################################################

SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360/inc
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360/inc/data_ports
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360/src
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/index_lookup
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/object_ageing
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/debug_writer
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/boundaries
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/host_trail
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/fill_pa_data
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/zone
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/reference_point
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/interpolation
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/validation
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/trajectory_prediction
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Calibration/Feature_Building_Kit_Core
SUBDIRS += $(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Calibration/Rivian_SRR6

INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360/inc
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360/inc/data_ports
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer/f360/src
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Platform_Abstraction_Layer
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/index_lookup
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/object_ageing
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/debug_writer
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/boundaries
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/host_trail
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Interface/fill_pa_data
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/zone
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/reference_point
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/interpolation
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/validation
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Shared_Feature_Functions/trajectory_prediction
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Calibration/Feature_Building_Kit_Core
INCLUDE_DIR += -I$(HOME_DIR)/$(APPLICATION_DIR)/Rivian_SRR6_Feature_Building_Kit/Calibration/Rivian_SRR6

######## Generate linker files in _lnk folder  #######
######## Feature and customer specific part    #######

FF_CAL_SUBDIRS = ""
FF_OTHER_SUBDIRS = ""
FF_OTHER_SUBDIRS += Platform_Abstraction_Layer/f360
FF_OTHER_SUBDIRS += Platform_Abstraction_Layer/f360/inc
FF_OTHER_SUBDIRS += Platform_Abstraction_Layer/f360/inc/data_ports
FF_OTHER_SUBDIRS += Platform_Abstraction_Layer/f360/src
FF_OTHER_SUBDIRS += Platform_Abstraction_Layer
FF_OTHER_SUBDIRS += Interface
FF_OTHER_SUBDIRS += Interface/index_lookup
FF_OTHER_SUBDIRS += Interface/object_ageing
FF_OTHER_SUBDIRS += Interface/debug_writer
FF_OTHER_SUBDIRS += Interface/boundaries
FF_OTHER_SUBDIRS += Interface/host_trail
FF_OTHER_SUBDIRS += Interface/fill_pa_data
FF_OTHER_SUBDIRS += Shared_Feature_Functions
FF_OTHER_SUBDIRS += Shared_Feature_Functions/zone
FF_OTHER_SUBDIRS += Shared_Feature_Functions/reference_point
FF_OTHER_SUBDIRS += Shared_Feature_Functions/interpolation
FF_OTHER_SUBDIRS += Shared_Feature_Functions/validation
FF_OTHER_SUBDIRS += Shared_Feature_Functions/trajectory_prediction
FF_CAL_SUBDIRS += Calibration/Feature_Building_Kit_Core
FF_CAL_SUBDIRS += Calibration/Rivian_SRR6
LNK_FILE_PATH_BEGIN = _lnk/Rivian_SRR6/core2_feature_building_kit

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
