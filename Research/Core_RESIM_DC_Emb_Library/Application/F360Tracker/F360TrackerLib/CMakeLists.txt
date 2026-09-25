cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(F360TrackerLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/cmake/ConfigureAddressSanitizer.cmake)
configure_address_sanitizer()

# Set up variant
set(F360_TRACKER_VARIANT "A" CACHE STRING "Tracker variant")
set_property(CACHE F360_TRACKER_VARIANT PROPERTY STRINGS A B C D E F G H I J K L M N O P Q R S)
message("F360 Tracker: Used variant -> ${F360_TRACKER_VARIANT}")
if(F360_TRACKER_VARIANT STREQUAL "A")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_a.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_A")
   add_definitions(-DDefault_variant_A_defined)
elseif(F360_TRACKER_VARIANT STREQUAL "B")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_b.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_B")
elseif(F360_TRACKER_VARIANT STREQUAL "C")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_c.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_C")
elseif(F360_TRACKER_VARIANT STREQUAL "D")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_d.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_D")
elseif(F360_TRACKER_VARIANT STREQUAL "E")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_e.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_E")
elseif(F360_TRACKER_VARIANT STREQUAL "F")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_f.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_F")
elseif(F360_TRACKER_VARIANT STREQUAL "G")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_g.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_G")
elseif(F360_TRACKER_VARIANT STREQUAL "H")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_h.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_H")
elseif(F360_TRACKER_VARIANT STREQUAL "I")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_i.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_I")
elseif(F360_TRACKER_VARIANT STREQUAL "J")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_j.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_J")
elseif(F360_TRACKER_VARIANT STREQUAL "K")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_k.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_K")
elseif(F360_TRACKER_VARIANT STREQUAL "L")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_l.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_L")
elseif(F360_TRACKER_VARIANT STREQUAL "M")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_m.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_M")
elseif(F360_TRACKER_VARIANT STREQUAL "N")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_n.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_N")
elseif(F360_TRACKER_VARIANT STREQUAL "O")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_o.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_O")
elseif(F360_TRACKER_VARIANT STREQUAL "P")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_p.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_P")
elseif(F360_TRACKER_VARIANT STREQUAL "Q")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_q.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_Q")
elseif(F360_TRACKER_VARIANT STREQUAL "R")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_r.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_R")
elseif(F360_TRACKER_VARIANT STREQUAL "S")
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/variants/f360_variant_definition_s.h ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_variant_definition.h COPYONLY)
   set(VARIANT_NS_STR "f360_variant_S")
endif()

# Add source files not in subfolders
set(SRC
   Fusion360/f360_tracker.cpp
   Fusion360/f360_calibrations.cpp
   Fusion360/f360_xtrk_logging.cpp
   Fusion360/f360_xtrk_logging.h
   Timing/f360_get_wall_time.cpp
)

# Set up all existing subfolders, each containing a sources.cmake file
set(SRC_DIRS
   Fusion360/autocode_reuse
   Fusion360/cluster_grouping
   Fusion360/clustering
   Fusion360/common
   Fusion360/detection_to_track_association
   Fusion360/e2e_protection
   Fusion360/include
   Fusion360/inputs_preprocessing
   Fusion360/internal_preprocessing
   Fusion360/logging
   Fusion360/measurement_update_tracks
   Fusion360/multipath_detector
   Fusion360/object_track_initialization
   Fusion360/occlusion
   Fusion360/post_update_track_adjustments
   Fusion360/pre_association_track_management
   Fusion360/sensor_postprocessing
   Fusion360/static_environment
   Fusion360/time_update_tracks
   Fusion360/track_classification
   Fusion360/track_downselection
   Fusion360/track_grouping
   Fusion360/track_validity
   Fusion360/update_relative_timestamps
   Fusion360/passenger_trailer_estimator
   Fusion360/cv_trailer_estimator
   Fusion360/trailer_manager
)

# Loop over all subfolders. Each .cmake file will update the SRC variable with source files from the corresponding folder
foreach(SRC_DIR ${SRC_DIRS})
   include(${SRC_DIR}/sources.cmake)
   set(INC_DIRS ${INC_DIRS} ${SRC_DIR}/include)
endforeach()
set(INC_DIRS ${INC_DIRS} Fusion360)
set(INC_DIRS ${INC_DIRS} Fusion360/include)


# State manager
set(SM_SRC
   StateManager/source/f360_input_diagnostics.cpp
   StateManager/source/f360_output_diagnostics.cpp
   StateManager/source/f360_safety_control_logic.cpp
   StateManager/source/State_Manager.cpp
)

# Tracker and State Manager interface files saved in IFACE_SRC
include(SharedTrackerAPI/sources.cmake)

# Create library and add all sources
set(F360_TRACKER_ALL_SOURCES ${SRC} ${SM_SRC} ${IFACE_SRC})

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "C6000" AND CMAKE_BUILD_TYPE STREQUAL "Release")
   add_c6x_library(
      TARGET F360-Tracker
      SOURCES ${F360_TRACKER_ALL_SOURCES}
   )
else()
   add_library(F360-Tracker ${F360_TRACKER_ALL_SOURCES})
endif()

# Set up preprocessor
add_definitions(-Df360_variant_A=${VARIANT_NS_STR})

# Set up SW tag
set(F360_SW_TAG_HEX "0x0000000" CACHE STRING "GIT commit hash")
message("F360 SW tag: " ${F360_SW_TAG_HEX})
add_definitions(-DF360_TRACKER_VERSION_BUILD_ID=${F360_SW_TAG_HEX})



if (MSVC)
   add_definitions(-D_SCL_SECURE_NO_WARNINGS)
   target_compile_options( F360-Tracker PRIVATE
      /W4 # warning level 4
      /WX # treat warnings as errors
   )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   target_compile_options( F360-Tracker PRIVATE
      -Wall
      -Wextra
      -pedantic
      -Wnon-virtual-dtor
      -Wswitch-default
      -Werror
   )
   message("Entered GNU")
   # Add option to build with PIC flag for projects that build tracker into a shared library
   option(BUILD_PIC "Build with Position Independent Code flag" OFF)
   if(${BUILD_PIC})
      target_compile_options(F360-Tracker PRIVATE -fPIC)
   endif()
   # Copyright notice is technically unused, so disable the warning for the specific file
   set_property(SOURCE Fusion360/f360_tracker.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-variable")
endif()

# Add interface directories
target_include_directories(F360-Tracker
   PRIVATE
      ${INC_DIRS}
   PUBLIC
      SharedTrackerAPI/core
      SharedTrackerAPI/Logging
      SharedTrackerAPI/Types
)

# Add external component interface definitions
include(rspp_config.cmake)
include(ocg_config.cmake)
# apply the varaint definition and rspp to safety_handler as well
add_subdirectory(safety_handler)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
   include(sg_config.cmake)
endif()

# Mirror folder structure in VS solution explorer
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${SM_SRC} ${IFACE_SRC})

# Create an interface folder with updated namespace
string(APPEND NS_STR1 ${VARIANT_NS_STR} "::")
string(APPEND NS_STR2 "namespace " ${VARIANT_NS_STR})
string(APPEND RSPP_NS_STR1 ${RSPP_VARIANT_NS_STR} "::")
string(APPEND GUARD_STR "_VARIANT_" ${F360_TRACKER_VARIANT} "_H")
string(APPEND IFACE_DIR_NAME "iface_variant_" ${F360_TRACKER_VARIANT})

set(IFACE_ALL ${IFACE_SRC} safety_handler/iface/sh_safety_handler.h) #to be moved to safety handler CMakeLists.txt when refactoring safety handler build script
foreach(IFACE_FILE ${IFACE_ALL})
   file(READ ${IFACE_FILE} FILE_CONTENTS)
   string(REPLACE "f360_variant_A::" ${NS_STR1} FILE_CONTENTS "${FILE_CONTENTS}")
   string(REPLACE "rspp_variant_A::" ${RSPP_NS_STR1} FILE_CONTENTS "${FILE_CONTENTS}")
   string(REPLACE "namespace f360_variant_A" ${NS_STR2} FILE_CONTENTS "${FILE_CONTENTS}")
   string(REPLACE "_VARIANT_A_H" ${GUARD_STR} FILE_CONTENTS "${FILE_CONTENTS}")
   string(REPLACE "SharedTrackerAPI" ${IFACE_DIR_NAME} IFACE_FILE "${IFACE_FILE}")
    string(REPLACE "safety_handler/iface" safety_handler/${IFACE_DIR_NAME} IFACE_FILE "${IFACE_FILE}") #to be moved to safety handler CMakeLists.txt when refactoring safety handler build script
   file(WRITE ${CMAKE_BINARY_DIR}/${IFACE_FILE} "${FILE_CONTENTS}")
endforeach()

# Update the lib filename
string(APPEND LIB_NAME "f360_variant_" ${F360_TRACKER_VARIANT})
set_target_properties(F360-Tracker PROPERTIES OUTPUT_NAME ${LIB_NAME})

# Unit test configuration
set(BUILD_UT OFF CACHE BOOL "Build Unit Tests")
if(${BUILD_UT})
   include(ut_config.cmake)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set(BUILD_COVERAGE_RESIM OFF CACHE BOOL "Build Resim Coverage")
   set(BUILD_COVERAGE OFF CACHE BOOL "Build UT Coverage")
   set(BUILD_DOCS OFF CACHE BOOL "Create documentation")

   if(${BUILD_COVERAGE})
      include(coverage_config.cmake)
   endif()

   if(${BUILD_DOCS})
     include(f360-docs.cmake)
   endif()
endif()

# Xtrk logging configuration
set(XTRKLOG_ADDITIONAL_INIT_DATA OFF CACHE BOOL "Log additional xtrk initialization data")
if(${XTRKLOG_ADDITIONAL_INIT_DATA})
   target_compile_options(F360-Tracker PRIVATE -DXTRKLOG_ADDITIONAL_INIT_DATA)
endif()

target_link_libraries(F360-Tracker PRIVATE stationary_geometries_iface)
