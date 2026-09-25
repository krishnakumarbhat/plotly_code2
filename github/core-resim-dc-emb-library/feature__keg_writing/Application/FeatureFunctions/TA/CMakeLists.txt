# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that TA must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(TA)
add_library(TA ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT TA_ROOT_PATH)
   get_filename_component(TA_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(TA_ROOT_PATH "${TA_ROOT_PATH}" CACHE PATH "Path to TA Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_TA_Customers "Generic" "BMW_SP25" "Rivian_SRR6")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(TA ${TA_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_TA_Customers}")

# ######################################################################################################################
# Set compiler definitions and options
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(TA ${TA_PROJECT_VARIANT})
