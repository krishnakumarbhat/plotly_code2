# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE RECW_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(RECW_Unit_Test)
add_executable(RECW_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT RECW_UNITTESTS_PATH)
   set(RECW_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT RECW_UNITTESTS_PATH)

target_link_libraries(RECW_Unit_Test RECW MLMathLibrary Feature_Building_Kit gmock gtest)

# Add functional test source files
target_include_directories(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_iface_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_crash_prob_steer_brake_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_car_wash_detection_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_object_validator_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_output_debouncer_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Source/recw_calibration_test.cpp)

target_include_directories(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_iface_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_crash_prob_steer_brake_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_car_wash_detection_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_object_validator_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_output_debouncer_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/Test_Classes/recw_calibration_test.hpp)

target_include_directories(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH})
target_sources(RECW_Unit_Test PRIVATE ${RECW_UNITTESTS_PATH}/main.cpp)

# Include Customer specific Unit Test Framework
include(${RECW_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(RECW_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(RECW_Unit_Test_SOURCES RECW_Unit_Test SOURCES)
source_group(TREE ${RECW_UNITTESTS_PATH} FILES ${RECW_Unit_Test_SOURCES})

# Set properties
set_target_properties(RECW_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT RECW_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS RECW_Unit_Test)
