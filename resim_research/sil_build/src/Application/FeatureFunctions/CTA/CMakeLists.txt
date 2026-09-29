# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that CTA must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(CTA)
add_library(CTA ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT CTA_ROOT_PATH)
   get_filename_component(CTA_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(CTA_ROOT_PATH "${CTA_ROOT_PATH}" CACHE PATH "Path to CTA Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_CTA_Customers "Generic" "BMW_SP25" "Honda_SRR6" "STLA_Thunder" "Rivian_SRR6")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(CTA ${CTA_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_CTA_Customers}")

# ######################################################################################################################
# Feature specific linkage and mock files
# ######################################################################################################################
if(TARGET PT)
   target_link_libraries(CTA PT)
elseif(NOT CTA_UNIT_TESTS)
   message(WARNING "Target for Path Tracking (PT) not found. PT mock files will be included.")
   include(${CTA_ROOT_PATH}/Mock_Files/Mock_Files.cmake)
else()
   # For testing another PT mock interface is added such that outputs of PT can be modified
   include(${CTA_ROOT_PATH}/Testing/SWE4_Unit_Tests/Mock_Files/Mock_Files.cmake)
endif()

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(CTA ${CTA_PROJECT_VARIANT})
