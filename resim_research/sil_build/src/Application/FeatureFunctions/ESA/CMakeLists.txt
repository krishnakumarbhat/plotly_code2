# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that ESA must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(ESA)
add_library(ESA ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT ESA_ROOT_PATH)
   get_filename_component(ESA_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(ESA_ROOT_PATH "${ESA_ROOT_PATH}" CACHE PATH "Path to ESA Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_ESA_Customers "BMW_SP25" "Generic")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(ESA ${ESA_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_ESA_Customers}")

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(ESA ${ESA_PROJECT_VARIANT})
