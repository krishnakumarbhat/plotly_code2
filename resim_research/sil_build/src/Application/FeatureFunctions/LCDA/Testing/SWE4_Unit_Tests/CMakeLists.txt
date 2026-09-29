# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE LCDA_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(LCDA_Unit_Test)
add_executable(LCDA_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT LCDA_UNITTESTS_PATH)
   set(LCDA_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT LCDA_UNITTESTS_PATH)

target_link_libraries(LCDA_Unit_Test LCDA MLMathLibrary Feature_Building_Kit gmock gtest)

target_include_directories(LCDA_Unit_Test PUBLIC ${LCDA_UNITTESTS_PATH}/Source)
target_include_directories(LCDA_Unit_Test PUBLIC ${LCDA_UNITTESTS_PATH}/Test_Classes)
target_include_directories(LCDA_Unit_Test PUBLIC ${LCDA_UNITTESTS_PATH}/Helper_Functions)

# Unit Test file includes:
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_process_bsw_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_process_cvw_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_process_elc_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_process_slc_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_common_functions_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_create_bsw_zone_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_create_cvw_zone_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_iface_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Test_Classes/lcda_calibration_test.hpp)

# Helper function includes:
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Helper_Functions/lcda_input_generator.h)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Helper_Functions/lcda_input_generator.c)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Helper_Functions/lcda_test_helpers.h)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Helper_Functions/lcda_test_helpers.c)

# Unit Test source file includes:
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_process_bsw_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_process_cvw_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_process_elc_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_process_slc_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_common_functions_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_create_bsw_zone_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_create_cvw_zone_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_iface_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/Source/lcda_calibration_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${LCDA_UNITTESTS_PATH}/main.cpp)

# Customer specific tests
include(${LCDA_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(LCDA_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(LCDA_Unit_Test_SOURCES LCDA_Unit_Test SOURCES)
source_group(TREE ${LCDA_UNITTESTS_PATH} FILES ${LCDA_Unit_Test_SOURCES})

if(LCDA_SUPRESS_CURRENTLY_FAILING_UNIT_TEST)
   target_compile_definitions(${target_name} PRIVATE LCDA_SUPRESS_CURRENTLY_FAILING_UNIT_TEST)
endif()

# Set properties
set_target_properties(LCDA_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT LCDA_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS LCDA_Unit_Test)
