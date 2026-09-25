cmake_minimum_required(VERSION 3.14)
project(TA_Unit_Test)
add_executable(TA_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT TA_UNITTESTS_PATH)
   set(TA_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT TA_UNITTESTS_PATH)

target_link_libraries(TA_Unit_Test TA MLMathLibrary Feature_Building_Kit gmock gtest)

# include test fixture classes for core
target_include_directories(TA_Unit_Test PUBLIC ${TA_UNITTESTS_PATH}/Test_Classes)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_calibration_boundary_check_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_calibration_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_public_calibration_boundary_check_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_public_calibration_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_collision_filter_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_constants_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_common_functions_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_factory_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_intersection_analyzer_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_object_filter_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_warn_logic_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Test_Classes/ta_iface_test.hpp)

# include unit tests
target_include_directories(TA_Unit_Test PUBLIC ${TA_UNITTESTS_PATH}/Source)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_calibration_boundary_check_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_calibration_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_public_calibration_boundary_check_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_public_calibration_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_collision_filter_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_constants_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_common_functions_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_factory_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_intersection_analyzer_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_object_filter_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_warn_logic_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/Source/ta_iface_test.cpp)

# include customer adapter
include(${TA_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(TA_Unit_Test PRIVATE ${_compile_defs})

target_include_directories(TA_Unit_Test PUBLIC ${TA_UNITTESTS_PATH})
target_sources(TA_Unit_Test PRIVATE ${TA_UNITTESTS_PATH}/main.cpp)

# Generate library and add executable
get_target_property(TA_Unit_Test_Sources TA_Unit_Test SOURCES)
source_group(TREE ${TA_UNITTESTS_PATH} FILES ${TA_Unit_Test_Sources})

# Set properties
set_target_properties(TA_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT TA_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS TA_Unit_Test)
