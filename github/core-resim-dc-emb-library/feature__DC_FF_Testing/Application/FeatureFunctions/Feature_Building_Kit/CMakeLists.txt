# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target - Please note that Feature_Building_Kit must be a unique project name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(Feature_Building_Kit)
add_library(Feature_Building_Kit ${CMAKE_CURRENT_LIST_FILE})

# ######################################################################################################################
# Set some initial settings
# ######################################################################################################################
if(NOT Feature_Building_Kit_ROOT_PATH)
   get_filename_component(Feature_Building_Kit_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
   set(Feature_Building_Kit_ROOT_PATH "${Feature_Building_Kit_ROOT_PATH}"
       CACHE PATH "Path to Feature_Building_Kit Home folder")
endif()

set(Feature_Building_Kit_PROJECT_VARIANT "Generic" CACHE STRING "Project variant to be used.")
set(Feature_Building_Kit_PA_OBJ_NUMBER_OF_OBJECTS 50 CACHE STRING "Number of objects")
option(Feature_Building_Kit_UNIT_TESTS "Include Feature_Building_Kit unit tests into build" OFF)
option(Feature_Building_Kit_UPDATE_DEFAULT_CALS "Update default cal values" ON)
option(Feature_Building_Kit_USE_GENERIC_PA_INTERFACE "Use the generic PA interface" ON)
target_compile_definitions(Feature_Building_Kit PRIVATE "Feature_Building_Kit_USE_GENERIC_PA_INTERFACE")
# ######################################################################################################################
# Set valid customers
# ######################################################################################################################
set(All_Valid_Feature_Building_Kit_Customers
    "Generic"
    "BMW_SP25"
    "Honda_SRR6"
    "STLA_Thunder"
    "Rivian_SRR6")

set_property(CACHE Feature_Building_Kit_PROJECT_VARIANT PROPERTY STRINGS ${All_Valid_Feature_Building_Kit_Customers})

# ######################################################################################################################
# Check for cross compilation
# ######################################################################################################################
if(CMAKE_CROSSCOMPILING)
   if(RR_COMMON_INC OR RR_COMMON_SRC)
      if(EXISTS ${RR_COMMON_INC})
         include_directories(${RR_COMMON_INC})
      else(EXISTS ${RR_COMMON_INC})
         include_directories(${RR_COMMON_SRC})
      endif(EXISTS ${RR_COMMON_INC})
   else(RR_COMMON_INC OR RR_COMMON_SRC)
      set(REQUIRE_TOOLBOX_UNIT_TST_INCLUDES ON)
   endif(RR_COMMON_INC OR RR_COMMON_SRC)
endif(CMAKE_CROSSCOMPILING)

# ######################################################################################################################
# Linking to required libs
# ######################################################################################################################
if(TARGET Core_Interface)
   target_link_libraries(Feature_Building_Kit Core_Interface)
endif()

if(TARGET MLMathLibrary)
   target_link_libraries(Feature_Building_Kit MLMathLibrary)
endif()

if(TARGET SRR_CORE_LIB)
   target_link_libraries(Feature_Building_Kit SRR_CORE_LIB)
endif()

if(TARGET CT_CALTOOL)
   target_link_libraries(Feature_Building_Kit CT_CALTOOL)
endif()

if(TARGET SIL_Library)
   target_link_libraries(Feature_Building_Kit SIL_Library)
endif()

if(TARGET Z2_SRR)
   target_link_libraries(Feature_Building_Kit Z2_SRR)
endif()

if(TARGET z2_srr_iface)
   target_link_libraries(Feature_Building_Kit z2_srr_iface)
endif()

if(TARGET AS-bin-writer-lib AND NOT MODULE_UNIT_TESTS)
   target_link_libraries(Feature_Building_Kit AS-bin-writer-lib)
endif()
# bin-writer
target_link_libraries(Feature_Building_Kit AS-bin-writer-lib)

# ######################################################################################################################
# Include necessary c and h files
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/Platform_Abstraction_Layer/platforms.cmake)
include(${Feature_Building_Kit_ROOT_PATH}/Interface/interface.cmake)
include(${Feature_Building_Kit_ROOT_PATH}/Shared_Feature_Functions/shared_feature_functions.cmake)
include(${Feature_Building_Kit_ROOT_PATH}/Calibration/Feature_Building_Kit_Calibration.cmake)

# ######################################################################################################################
# Set compiler definitions and options
# ######################################################################################################################

# If not embedded build then define pc_resim
if(NOT CMAKE_CROSSCOMPILING)
   target_compile_definitions(Feature_Building_Kit PRIVATE CT_ACTIVATE_CAL_PRINT)
endif(NOT CMAKE_CROSSCOMPILING)

if(MSVC)
   # Debug settings
   target_compile_definitions(Feature_Building_Kit PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")
   target_compile_definitions(Feature_Building_Kit PRIVATE "$<$<CONFIG:Release>:NDEBUG>")
else()
   # Debug settings
   target_compile_options(Feature_Building_Kit PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

   if(TARGET COMPONENT_RESIM_CORE)
      target_compile_options(Feature_Building_Kit PRIVATE "-fPIC")
   endif()
   if(BUILD_FOR_32_BIT)
      set_target_properties(Feature_Building_Kit PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
   endif()
endif()

# Set IDE properties:
if(NOT CMAKE_FOLDER)
   # No IDE folder chosen already, set custom folder
   set_target_properties(Feature_Building_Kit PROPERTIES FOLDER "Libraries")
endif()

# ######################################################################################################################
# Targets for further Analysis
# ######################################################################################################################
if(Feature_Building_Kit_UNIT_TESTS)
   if(TARGET gtest)
      add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Testing/SWE4_Unit_Tests" "${CMAKE_CURRENT_BINARY_DIR}/UnitTest")
   else()
      message(
         WARNING "-- Feature_Building_Kit/CMake/CMakeLists.txt: Can not target GTEST since GTEST_PATH is not defined")
   endif()
endif()

# ######################################################################################################################
# Source Grouping and Set folder structure
# ######################################################################################################################
get_target_property(Feature_Building_Kit_SOURCES Feature_Building_Kit SOURCES)
source_group(TREE ${Feature_Building_Kit_ROOT_PATH} FILES ${Feature_Building_Kit_SOURCES})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ######################################################################################################################
# Create options to call automated Makefile and Linker File generation
# ######################################################################################################################
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Generate_Mak_Files.cmake)
include(${Feature_Building_Kit_ROOT_PATH}/CMake/Srf_Generate_Lcf_Files.cmake)

option(Feature_Building_Kit_GENERATE_MAK_FILES "Generate *.mak for this feature" OFF)
option(Feature_Building_Kit_GENERATE_LCF_FILES "Generate *.lcf for this feature" OFF)

# We do not require those files for customer Generic
if(NOT ${Feature_Building_Kit_PROJECT_VARIANT} STREQUAL "Generic")
   # Automatic generation of mak files
   if(Feature_Building_Kit_GENERATE_MAK_FILES)
      if(${Feature_Building_Kit_PROJECT_VARIANT} STREQUAL "BMW_SP25")
         set(Feature_Building_Kit_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/Feature_Building_Kit")
      elseif(${Feature_Building_Kit_PROJECT_VARIANT} STREQUAL "Honda_SRR6")
         set(Feature_Building_Kit_make_alias "z7b/Feature_Functions/Feature_Building_Kit")
      else()
         set(Feature_Building_Kit_make_alias "")
      endif()
      srf_generate_mak_files(
         "Feature_Building_Kit" "${Feature_Building_Kit_make_alias}" ${Feature_Building_Kit_ROOT_PATH}
         ${Feature_Building_Kit_PROJECT_VARIANT} ${Feature_Building_Kit_ROOT_PATH})
   endif()

   # Automatic generation of lcf files
   if(Feature_Building_Kit_GENERATE_LCF_FILES)
      srf_generate_lcf_files("Feature_Building_Kit" ${Feature_Building_Kit_ROOT_PATH}
                             ${Feature_Building_Kit_PROJECT_VARIANT})
   endif()
endif()
