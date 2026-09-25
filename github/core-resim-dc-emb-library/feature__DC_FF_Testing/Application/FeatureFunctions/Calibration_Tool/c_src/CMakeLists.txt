cmake_minimum_required(VERSION 3.14)
project(CT_CALTOOL)
add_library(CT_CALTOOL ${CMAKE_CURRENT_LIST_FILE})

# ##############################################################################
# Set some initial settings
# ##############################################################################
set(CT_CALTOOL_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT CT_CALTOOL_ROOT_PATH)
  get_filename_component(CT_CALTOOL_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}"
                         REALPATH)
  set(CT_CALTOOL_ROOT_PATH
      "${CT_CALTOOL_ROOT_PATH}"
      CACHE PATH "Path to Cal Tool C source home folder")
endif()

target_include_directories(CT_CALTOOL PUBLIC ${CT_CALTOOL_ROOT_PATH})

target_sources(
  CT_CALTOOL
  PUBLIC ${CT_CALTOOL_ROOT_PATH}/ct_calibration_header_t.h)
  
target_sources(
  CT_CALTOOL
  PRIVATE ${CT_CALTOOL_ROOT_PATH}/ct_endianness_switch.c
          ${CT_CALTOOL_ROOT_PATH}/ct_endianness_switch.h
	  ${CT_CALTOOL_ROOT_PATH}/ct_boundaries_check_function_helpers.c
          ${CT_CALTOOL_ROOT_PATH}/ct_boundaries_check_function_helpers.h)

get_target_property(CT_CALTOOL_Sources CT_CALTOOL SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CT_CALTOOL_Sources})
message(STATUS "Unit Tests: ${MODULE_UNIT_TESTS}")
# Link Cal tool to SRR_CORE_LIB for reuse.h
target_link_libraries(CT_CALTOOL SRR_CORE_LIB)
