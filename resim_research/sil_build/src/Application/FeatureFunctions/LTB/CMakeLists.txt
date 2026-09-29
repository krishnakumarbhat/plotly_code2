# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that LTB must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(LTB)
add_library(LTB ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT LTB_ROOT_PATH)
   get_filename_component(LTB_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(LTB_ROOT_PATH "${LTB_ROOT_PATH}" CACHE PATH "Path to LTB Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_LTB_Customers "BMW_SP25" "Generic")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(LTB ${LTB_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_LTB_Customers}")

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(LTB ${LTB_PROJECT_VARIANT})
