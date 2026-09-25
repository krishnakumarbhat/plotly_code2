cmake_minimum_required(VERSION 3.16)

project(Stationary_Geometries_Core)

set(sg_original_cmake_folder ${CMAKE_FOLDER})

##############################################################################
# Common compiler setup
##############################################################################
include(cmake/CreateCompilerCommonConfigurationTarget.cmake)
create_compiler_common_configuration_target()

##############################################################################
# Unittests
##############################################################################
if(SG_DEVELOPER_MODE)
    option(SG_UT "Unittest framework" OFF)

    if(SG_UT)
        set(CMAKE_FOLDER ${sg_original_cmake_folder}/unit_tests)
        add_subdirectory(unit_tests)
        set(CMAKE_FOLDER ${sg_original_cmake_folder})
    endif()
endif()

##############################################################################
# Interface
##############################################################################
add_subdirectory(iface)

##############################################################################
# Core
##############################################################################
option(SG_IFACE_ONLY "Provides only SG interface target" OFF)
if(NOT SG_IFACE_ONLY)
    add_subdirectory(core)
else()
    # add only rspp interface
    add_subdirectory(core/modules/rspp/iface)
endif()

set(CMAKE_FOLDER ${sg_original_cmake_folder})
