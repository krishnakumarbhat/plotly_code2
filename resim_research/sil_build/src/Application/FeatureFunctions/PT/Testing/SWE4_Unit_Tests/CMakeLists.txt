# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE PT_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(PT_Unit_Test)
add_executable(PT_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT PT_UNITTEST_PATH)
   set(PT_UNITTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

target_link_libraries(PT_Unit_Test PT MLMathLibrary Feature_Building_Kit gmock gtest)

# Setup the Unit Test target Include Test Fixture classes
target_include_directories(PT_Unit_Test PUBLIC ${PT_UNITTEST_PATH}/Test_Classes)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_object_matching_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_path_rotation_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_group_paths_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_common_functions_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_iface_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_modify_grid_cals_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_output_factory_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_persistent_handler_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_reset_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_shared_unit_test_constructors.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_host_lane_test.hpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Test_Classes/pt_calibration_test.hpp)

# Include Unit Tests
target_include_directories(PT_Unit_Test PUBLIC ${PT_UNITTEST_PATH}/Source)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_object_matching_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_path_rotation_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_group_paths_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_common_functions_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_iface_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_modify_grid_cals_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_output_factory_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_persistent_handler_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_reset_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_host_lane_test.cpp)
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/Source/pt_calibration_test.cpp)

# Include Customer Adapter specific test scenarios
include(${PT_UNITTEST_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

target_include_directories(PT_Unit_Test PUBLIC ${PT_UNITTEST_PATH})
target_sources(PT_Unit_Test PRIVATE ${PT_UNITTEST_PATH}/main.cpp)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(PT_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(PT_Unit_Test_SOURCES PT_Unit_Test SOURCES)
source_group(TREE ${PT_UNITTEST_PATH} FILES ${PT_Unit_Test_SOURCES})

# Set properties
set_target_properties(PT_Unit_Test PROPERTIES FOLDER "Feature Functions")

# Set as startup project
if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT PT_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS PT_Unit_Test)
