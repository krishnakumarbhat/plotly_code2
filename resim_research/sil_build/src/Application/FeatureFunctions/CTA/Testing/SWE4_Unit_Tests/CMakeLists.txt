# ######################################################################################################################
# REQUIRED BLOCK minimum version, project, target NOTE CTA_Unit_Test must be a unique, constant Module/Library name
# ######################################################################################################################
cmake_minimum_required(VERSION 3.14)
project(CTA_Unit_Test)
add_executable(CTA_Unit_Test ${CMAKE_CURRENT_LIST_FILE})

# include file directories
if(NOT CTA_UNITTESTS_PATH)
   set(CTA_UNITTESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif(NOT CTA_UNITTESTS_PATH)

target_link_libraries(CTA_Unit_Test CTA MLMathLibrary Feature_Building_Kit gmock gtest)

# Setup the Unit Test target Include Test Fixture classes
include(${CTA_UNITTESTS_PATH}/Source/Source.cmake)
# Include Unit Tests
include(${CTA_UNITTESTS_PATH}/Test_Classes/Test_Classes.cmake)

target_include_directories(CTA_Unit_Test PUBLIC ${CTA_UNITTESTS_PATH})
target_sources(CTA_Unit_Test PRIVATE ${CTA_UNITTESTS_PATH}/main.cpp)

# Include Customer specific Unit Test Framework
include(${CTA_UNITTESTS_PATH}/Unit_Test_Customer/Unit_Test_Customer.cmake)

add_definitions(-D_VARIADIC_MAX=10)

# Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
# perspective, in case that a conditional define or conditional include is used.
get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
target_compile_definitions(CTA_Unit_Test PRIVATE ${_compile_defs})

# Generate library and add executable
get_target_property(CTA_Unit_Test_SOURCES CTA_Unit_Test SOURCES)
source_group(TREE ${CTA_UNITTESTS_PATH} FILES ${CTA_Unit_Test_SOURCES})

# Set properties
set_target_properties(CTA_Unit_Test PROPERTIES FOLDER "Feature Functions")

if(MSVC)
   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT CTA_Unit_Test)
endif(MSVC)

# Install target
install(TARGETS CTA_Unit_Test)
