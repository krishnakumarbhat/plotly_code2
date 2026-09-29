# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that CED must be a unique, constant library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(CED)
add_library(CED ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT CED_ROOT_PATH)
   get_filename_component(CED_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(CED_ROOT_PATH "${CED_ROOT_PATH}" CACHE PATH "Path to CED Home folder")
endif()

# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_CED_Customers
    "BMW_SP25"
    "Generic"
    "Honda_SRR6"
    "STLA_Thunder"
    "Rivian_SRR6")

# ######################################################################################################################
# Build up basic feature structure
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Feature_Setup.cmake)
srf_feature_setup(CED ${CED_ROOT_PATH} ${Feature_Building_Kit_ROOT_PATH} "${All_Valid_CED_Customers}")

# ######################################################################################################################
# Feature specific linkage and mock files
# ######################################################################################################################
if(TARGET PT)
   target_link_libraries(CED PT)
elseif(NOT CED_UNIT_TESTS)
   message(WARNING "Target for Path Tracking (PT) not found. PT mock files will be included.")
   include(${CED_ROOT_PATH}/Mock_Files/Mock_Files.cmake)
else()
   # For testing another PT mock interface is added such that outputs of PT can be modified
   include(${CED_ROOT_PATH}/Testing/SWE4_Unit_Tests/Mock_Files/Mock_Files.cmake)
endif()

# ######################################################################################################################
# Set compiler definitions, options and target properties
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Set_Compile_Settings.cmake)
srf_set_compile_settings(CED ${CED_PROJECT_VARIANT})
