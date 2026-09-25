# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that SCW must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(SCW)
add_library(SCW ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT SCW_ROOT_PATH)
   get_filename_component(SCW_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(SCW_ROOT_PATH "${SCW_ROOT_PATH}" CACHE PATH "Path to SCW Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_SCW_Customers "Generic" "BMW_SP25")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(SCW ${SCW_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_SCW_Customers}")

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(SCW ${SCW_PROJECT_VARIANT})
