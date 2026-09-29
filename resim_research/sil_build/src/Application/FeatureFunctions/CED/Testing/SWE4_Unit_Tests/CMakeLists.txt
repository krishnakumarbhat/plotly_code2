# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE CED_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(CED_Unit_Test)
add_executable(CED_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT CED_UNITTESTS_PATH)
   set(CED_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT CED_UNITTESTS_PATH)

target_link_libraries(CED_Unit_Test CED MLMathLibrary Feature_Building_Kit gmock gtest)

# Add functional test source files
target_include_directories(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Source)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Source/ced_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Source/ced_iface_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Source/ced_create_zones_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Source/ced_calibration_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Source/ced_common_functions_test.cpp)

# Include Unit Tests
target_include_directories(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Test_Classes)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Test_Classes/ced_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Test_Classes/ced_iface_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Test_Classes/ced_create_zones_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Test_Classes/ced_calibration_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/Test_Classes/ced_common_functions_test.hpp)

# Unit Test source file includes:
target_include_directories(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH})
target_sources(CED_Unit_Test PRIVATE ${CED_UNITTESTS_PATH}/main.cpp)

# Include Customer specific Unit Test Framework
include(${CED_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(CED_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(CED_Unit_Test_Sources CED_Unit_Test SOURCES)
source_group(TREE ${CED_UNITTESTS_PATH} FILES ${CED_Unit_Test_Sources})

# Set properties
set_target_properties(CED_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT CED_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS CED_Unit_Test)
