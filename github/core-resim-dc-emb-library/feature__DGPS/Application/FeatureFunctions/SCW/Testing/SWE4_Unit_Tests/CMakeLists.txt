# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE SCW_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(SCW_Unit_Test)
add_executable(SCW_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT SCW_UNITTESTS_PATH)
   set(SCW_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT SCW_UNITTESTS_PATH)

target_link_libraries(SCW_Unit_Test SCW MLMathLibrary Feature_Building_Kit gmock gtest)

# Add functional test source files
target_include_directories(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Source)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Source/scw_test.cpp)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Source/scw_helper_functions_test.cpp)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Source/scw_iface_test.cpp)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Source/scw_calibration_test.cpp)

# Include Unit Tests
target_include_directories(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Test_Classes)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Test_Classes/scw_test.hpp)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Test_Classes/scw_helper_functions_test.hpp)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Test_Classes/scw_iface_test.hpp)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Test_Classes/scw_calibration_test.hpp)

# Include Helper function
target_include_directories(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Helper_Functions)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Helper_Functions/scw_input_generator.c)
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/Helper_Functions/scw_input_generator.h)

# Unit Test source file includes:
target_include_directories(SCW_Unit_Test PUBLIC ${SCW_UNITTESTS_PATH})
target_sources(SCW_Unit_Test PRIVATE ${SCW_UNITTESTS_PATH}/main.cpp)

# Include Customer specific Unit Test Framework
include(${SCW_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(SCW_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(SCW_Unit_Test_SOURCES SCW_Unit_Test SOURCES)
source_group(TREE ${SCW_UNITTESTS_PATH} FILES ${SCW_Unit_Test_SOURCES})

# Set properties
set_target_properties(SCW_Unit_Test PROPERTIES FOLDER "Feature Functions")

# Set as startup project

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT SCW_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS SCW_Unit_Test)
