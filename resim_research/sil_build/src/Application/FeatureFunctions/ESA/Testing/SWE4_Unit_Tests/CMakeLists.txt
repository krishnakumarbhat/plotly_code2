# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE ESA_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(ESA_Unit_Test)
add_executable(ESA_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT ESA_UNITTESTS_PATH)
   set(ESA_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT ESA_UNITTESTS_PATH)

target_link_libraries(ESA_Unit_Test ESA MLMathLibrary Feature_Building_Kit gmock gtest)

# Add functional test source files
target_include_directories(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Source)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Source/esa_calibration_test.cpp)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Source/esa_iface_test.cpp)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Source/esa_create_zone_test.cpp)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Source/esa_test.cpp)

# Include Unit Tests
target_include_directories(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Test_Classes)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Test_Classes/esa_calibration_test.hpp)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Test_Classes/esa_iface_test.hpp)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Test_Classes/esa_create_zone_test.hpp)
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/Test_Classes/esa_test.hpp)

# Unit Test source file includes:
target_include_directories(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH})
target_sources(ESA_Unit_Test PRIVATE ${ESA_UNITTESTS_PATH}/main.cpp)

# Include Customer specific Unit Test Framework
include(${ESA_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(ESA_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(ESA_Unit_Test_Sources ESA_Unit_Test SOURCES)
source_group(TREE ${ESA_UNITTESTS_PATH} FILES ${ESA_Unit_Test_Sources})

# Set properties
set_target_properties(ESA_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ESA_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS ESA_Unit_Test)
