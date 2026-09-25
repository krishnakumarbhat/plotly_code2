cmake_minimum_required(VERSION 3.16)

project(Stationary_Geometries)

##############################################################################
# General setup - pre changes and main SG options
##############################################################################
set(sg_original_cmake_folder ${CMAKE_FOLDER})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install_dir" CACHE PATH "Forced by SG to default location" FORCE)
endif()

if(NOT DEFINED RSPP_VARIANT)
    set(RSPP_VARIANT "A")
    message(WARNING "SG: RSPP variant not known at this point.\n Set to ${RSPP_VARIANT}")
endif()

##############################################################################
# SG core
##############################################################################
set(CMAKE_FOLDER "${sg_original_cmake_folder}/Stationary Geometries")
add_subdirectory("src" sg_core)

##############################################################################
# General setup - post changes
##############################################################################
# include(tools/cmake/install-rules.cmake)

if(MSVC)
   include(tools/cmake/vs_support_tools.cmake)
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
   set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(CMAKE_FOLDER ${sg_original_cmake_folder})