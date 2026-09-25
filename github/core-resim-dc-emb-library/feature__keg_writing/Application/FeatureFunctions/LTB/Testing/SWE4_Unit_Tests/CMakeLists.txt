# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE LTB_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(LTB_Unit_Test)
add_executable(LTB_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT LTB_UNITTESTS_PATH)
   set(LTB_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT LTB_UNITTESTS_PATH)

target_link_libraries(LTB_Unit_Test LTB MLMathLibrary Feature_Building_Kit gmock gtest)

# Add functional test source files
target_include_directories(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source/ltb_test.cpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source/ltb_iface_test.cpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source/ltb_calibration_test.cpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source/ltb_factory_test.cpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source/ltb_time_calculation_test.cpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Source/ltb_warn_logic_test.cpp)

# Include Unit Tests
target_include_directories(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes/ltb_test.hpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes/ltb_iface_test.hpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes/ltb_calibration_test.hpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes/ltb_factory_test.hpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes/ltb_time_calculation_test.hpp)
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/Test_Classes/ltb_warn_logic_test.hpp)

# Unit Test source file includes:
target_include_directories(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH})
target_sources(LTB_Unit_Test PRIVATE ${LTB_UNITTESTS_PATH}/main.cpp)

# Include Customer specific Unit Test Framework
include(${LTB_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(LTB_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(LTB_Unit_Test_Sources LTB_Unit_Test SOURCES)
source_group(TREE ${LTB_UNITTESTS_PATH} FILES ${LTB_Unit_Test_Sources})

# Set properties
set_target_properties(LTB_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT LTB_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS LTB_Unit_Test)
