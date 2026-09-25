cmake_minimum_required(VERSION 3.16)

project(Stationary_Geometries_Core)

set(sg_original_cmake_folder ${CMAKE_FOLDER})

##############################################################################
# Common compiler setup
##############################################################################
include(cmake/CreateCompilerCommonConfigurationTarget.cmake)
create_compiler_common_configuration_target()

##############################################################################
# Interface + Core
##############################################################################
add_subdirectory(core)
add_subdirectory(iface)

set(CMAKE_FOLDER ${sg_original_cmake_folder})