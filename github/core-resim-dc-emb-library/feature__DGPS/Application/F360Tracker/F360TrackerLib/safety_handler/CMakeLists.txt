cmake_minimum_required(VERSION 3.10)

project(safety_handler)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities/cmake/SetupTrackerVariants.cmake)

if(NOT DEFINED VARIANT_NS_STR)
   setup_f360_tracker_variant(${CMAKE_CURRENT_SOURCE_DIR}/.. VARIANT_NS_STR SAFETY_HANDLER_DEFAULT_VARIANT_A)
endif()

set(SAFETY_HANDLER_SRC
   source/sh_safety_handler.cpp
   source/sh_object_plausible_checks.cpp
   source/sh_boundingbox_helper_functions.cpp
)

set(SAFETY_HANDLER_HEADERS
   iface/sh_safety_handler.h
)

add_library(safety_handler STATIC ${SAFETY_HANDLER_SRC} ${SAFETY_HANDLER_HEADERS})

# safety_handler does not link F360-Tracker directly, so initialize the
# variant remap locally when the parent project has not already done it.
target_compile_definitions(safety_handler PUBLIC
   f360_variant_A=${VARIANT_NS_STR}
)

target_include_directories(safety_handler
   PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/iface
      ${CMAKE_CURRENT_SOURCE_DIR}/../SharedTrackerAPI/core
      ${CMAKE_CURRENT_SOURCE_DIR}/../SharedTrackerAPI/Logging
      ${CMAKE_CURRENT_SOURCE_DIR}/../SharedTrackerAPI/Types
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/source
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SAFETY_HANDLER_SRC} ${SAFETY_HANDLER_HEADERS})

if(${BUILD_UT})
   include(${CMAKE_CURRENT_SOURCE_DIR}/ut_config.cmake)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set(BUILD_COVERAGE OFF CACHE BOOL "Build UT Coverage")
   set(BUILD_DOCS OFF CACHE BOOL "Create documentation")
   if(${BUILD_COVERAGE})
      include(${CMAKE_CURRENT_SOURCE_DIR}/coverage_config.cmake)
   endif()
   if(${BUILD_DOCS})
     include(${CMAKE_CURRENT_SOURCE_DIR}/safety_handler_docs.cmake)
   endif()
endif()

string(APPEND LIB_NAME "safety_handler_variant_" ${F360_TRACKER_VARIANT})
set_target_properties(safety_handler PROPERTIES OUTPUT_NAME ${LIB_NAME})
