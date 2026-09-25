# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that RECW must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(RECW)
add_library(RECW ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT RECW_ROOT_PATH)
   get_filename_component(RECW_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(RECW_ROOT_PATH "${RECW_ROOT_PATH}" CACHE PATH "Path to RECW Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_RECW_Customers "Generic" "BMW_SP25" "Rivian_SRR6")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(RECW ${RECW_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_RECW_Customers}")

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(RECW ${RECW_PROJECT_VARIANT})
