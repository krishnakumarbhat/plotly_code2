cmake_minimum_required(VERSION 3.16)

project(Occupancy_Grid)

set(OCG_VARIANT_NAME
    "FLR4px1_v1"
    CACHE STRING "Project name to determine which variant to include")

# Populate cmake-gui OCG_VARIANT_NAME drop down menu with all available OCG
# project names
set_property(CACHE OCG_VARIANT_NAME PROPERTY STRINGS 
   FLR4px1_v1 
   FLR4x1_v1
   FLR7x1_v1 
   FLR4p_plt_x1_v1
   SRR7px2_v1
   SRR8p_front)

##############################################################################
# Common compiler setup
##############################################################################

# Option to add internal include directory
option(OCG_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

if (MSVC)
   add_compile_options(
      /W4 # warning level 4
   )
   if (OCG_WARNINGS_AS_ERRORS)
      add_compile_options(
         /WX # treat warnings as errors
      )
   endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   add_compile_options(
      -Wall
      -Wextra
      -pedantic
   )
   if (OCG_WARNINGS_AS_ERRORS)
      add_compile_options(
         -Werror # treat warnings as errors
      )
   endif()
endif()

add_subdirectory(iface)
add_subdirectory(core)

##############################################################################
# Unittests
##############################################################################
option(OCG_UT "Unittest framework" OFF)
if(OCG_UT)
   enable_testing()
   add_subdirectory(unit_tests)
endif()