cmake_minimum_required(VERSION 3.16)

set(sg_original_cmake_folder ${CMAKE_FOLDER})

#######################################
# SG Resim wrapper
#######################################
add_subdirectory(resim_wrapper)

option(SG_DRIVABILITY_CLASSIFICATION_TOOLING_WRAPPER "Enable running Drivability Classification wrapper tool using Stationary Geometries Wrapper. Remember to change lib_name to dc_tooling_wrapper." OFF)
if (SG_DRIVABILITY_CLASSIFICATION_TOOLING_WRAPPER)
   add_subdirectory(dc_tooling_resim_wrapper)
endif()

#######################################
# Component resim
#######################################
set(MODULE_NAME "component_resim_executable" CACHE STRING "Name of the module under test" FORCE)
set(MODULE_UNIT_TESTS OFF CACHE BOOL "Forced to OFF by OCG" FORCE)
message(AUTHOR_WARNING "MODULE_NAME is set (overwritten) to component_resim_executable and UTs are turned off")

add_subdirectory(CR_ComponentResim)

add_dependencies(COMPONENT_RESIM_EXECUTABLE stationary_geometries_wrapper) # Force rebuild in case of code changes when runing resim.

if (SG_DRIVABILITY_CLASSIFICATION_TOOLING_WRAPPER)
   add_dependencies(COMPONENT_RESIM_EXECUTABLE dc_tooling_wrapper) # Force rebuild in case of code changes when runing resim.
endif()

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")

# It is required for multi-threading. Fix on SG side (should be done inside CR_ComponentResim).
target_link_options(COMPONENT_RESIM_EXECUTABLE PRIVATE "$<${gcc_like_cxx}:-lpthread>") 

#######################################
# General setup
#######################################
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT COMPONENT_RESIM_EXECUTABLE)

set(CMAKE_FOLDER ${sg_original_cmake_folder})