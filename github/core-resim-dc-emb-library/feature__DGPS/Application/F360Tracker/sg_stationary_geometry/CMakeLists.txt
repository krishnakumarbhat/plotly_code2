cmake_minimum_required(VERSION 3.16)

project(Stationary_Geometries)

##############################################################################
# General setup - pre changes / requirements
##############################################################################
set(original_cmake_folder ${CMAKE_FOLDER})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
   set_property(GLOBAL PROPERTY USE_FOLDERS ON)
   enable_testing()
endif()

option(SG_DEVELOPER_MODE "In \"Developer mode\" user has more features that support development" OFF)
mark_as_advanced(SG_DEVELOPER_MODE)

##############################################################################
# Variant setup
##############################################################################
include(tools/cmake/SetupVariant.cmake)
setup_variant()    

##############################################################################
# SG core
##############################################################################
set(CMAKE_FOLDER "${original_cmake_folder}/Stationary Geometries")
add_subdirectory("src")

##############################################################################
# Developer Mode
##############################################################################
if(SG_DEVELOPER_MODE)
    set(CMAKE_FOLDER "${original_cmake_folder}/components")
    add_subdirectory("components")

    set(CMAKE_FOLDER "${original_cmake_folder}/tools")
    add_subdirectory("tools")

    set(CMAKE_FOLDER "${original_cmake_folder}/tests")
    add_subdirectory("tests")
endif()

##############################################################################
# General setup - post changes
##############################################################################
include(tools/cmake/install-rules.cmake)

configure_file(src/.clang-format ${CMAKE_BINARY_DIR}/.clang-format)

set(CMAKE_FOLDER ${original_cmake_folder})