# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE Feature_Building_Kit_Unit_Test must be a unique, constant
# Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(Feature_Building_Kit_Unit_Test)
add_executable(Feature_Building_Kit_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT Feature_Building_Kit_UNITTESTS_PATH)
   set(Feature_Building_Kit_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT Feature_Building_Kit_UNITTESTS_PATH)

target_link_libraries(Feature_Building_Kit_Unit_Test Feature_Building_Kit MLMathLibrary gmock gtest)

# Setup the Unit Test target Include Test Fixture classes
target_include_directories(Feature_Building_Kit_Unit_Test PUBLIC ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/pa_selector_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_macros_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_ref_point_calc_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_field_of_interest_factory_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_index_lookup_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_iface_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_array_interpolation_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_object_validation_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_obj_ageing_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_output_boundary_check_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_host_trail_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_calibration_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_functions_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_ego_traj_predictor_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_circular_shape_calculator_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_obj_traj_predictor_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_guardial_validation_test.hpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Test_Classes/fbk_vehicle_validation_test.hpp)

# Include Unit Tests
target_include_directories(Feature_Building_Kit_Unit_Test PUBLIC ${Feature_Building_Kit_UNITTESTS_PATH}/Source)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/pa_selector_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_macros_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_ref_point_calc_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_field_of_interest_factory_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_index_lookup_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_iface_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_array_interpolation_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_object_validation_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_obj_ageing_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_output_boundary_check_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_host_trail_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_calibration_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_functions_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_ego_traj_predictor_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_circular_shape_calculator_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_obj_traj_predictor_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_guardial_validation_test.cpp)
target_sources(Feature_Building_Kit_Unit_Test
               PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/Source/fbk_vehicle_validation_test.cpp)

target_include_directories(Feature_Building_Kit_Unit_Test PUBLIC ${Feature_Building_Kit_UNITTESTS_PATH})
target_sources(Feature_Building_Kit_Unit_Test PRIVATE ${Feature_Building_Kit_UNITTESTS_PATH}/main.cpp)

add_definitions(-D_VARIADIC_MAX=10)

# Generate library and add executable
get_target_property(Feature_Building_Kit_Unit_Test_SOURCES Feature_Building_Kit_Unit_Test SOURCES)
source_group(TREE ${Feature_Building_Kit_UNITTESTS_PATH} FILES ${Feature_Building_Kit_Unit_Test_SOURCES})

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(Feature_Building_Kit_Unit_Test PRIVATE ${_compile_defs})

# Compiler settings
target_compile_definitions(Feature_Building_Kit_Unit_Test PRIVATE CT_ACTIVATE_CAL_PRINT=1)

# Set properties
set_target_properties(Feature_Building_Kit_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Feature_Building_Kit_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS Feature_Building_Kit_Unit_Test)
