# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that PT must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(PT)
add_library(PT ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT PT_ROOT_PATH)
   get_filename_component(PT_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(PT_ROOT_PATH "${PT_ROOT_PATH}" CACHE PATH "Path to PT Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_PT_Customers
    "Generic"
    "BMW_SP25"
    "Honda_SRR6"
    "STLA_Thunder"
    "Rivian_SRR6")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(PT ${PT_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_PT_Customers}")

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(PT ${PT_PROJECT_VARIANT})
