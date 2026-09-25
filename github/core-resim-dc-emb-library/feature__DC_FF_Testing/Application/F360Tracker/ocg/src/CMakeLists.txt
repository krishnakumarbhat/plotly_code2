cmake_minimum_required(VERSION 3.16)

project(Occupancy_Grid)

# Set up OCG variant
if(${TRACKER_TYPE} STREQUAL "SRR")
   set(OCG_VARIANT_NAME
       "SRR7px2_v1"
       CACHE STRING "Project name to determine which variant to include")
elseif(${TRACKER_TYPE} STREQUAL "MRR")
       set(OCG_VARIANT_NAME
           "FLR7x1_v1"
           CACHE STRING "Project name to determine which variant to include")
endif()

# Populate cmake-gui OCG_VARIANT_NAME drop down menu with all available OCG
# project names
set_property(CACHE OCG_VARIANT_NAME PROPERTY STRINGS 
   FLR4px1_v1 
   FLR4x1_v1
   FLR7x1_v1 
   FLR4p_plt_x1_v1
   SRR7px2_v1)

##############################################################################
# Common compiler setup
##############################################################################

# Option to add internal include directory
option(OCG_USE_INTERNAL_INCLUDES
       "Mock required headers using internal directory" OFF)
option(OCG_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

if (MSVC)
   add_compile_options(
      /W4 # warning level 4
   )
   if (OCG_WARNINGS_AS_ERRORS)
      add_compile_options(
         #/WX # treat warnings as errors
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
         #-Werror # treat warnings as errors
      )
   endif()
endif()


if(${OCG_USE_INTERNAL_INCLUDES})
   if(NOT DEFINED OCG_RSPP_VARIANT_NAME)
      if(${OCG_VARIANT_NAME} STREQUAL "FLR4px1_v1")
         set(OCG_RSPP_VARIANT_NAME "rspp_variant_H")
      elseif(${OCG_VARIANT_NAME} STREQUAL "FLR4p_plt_x1_v1")
         set(OCG_RSPP_VARIANT_NAME "rspp_variant_H")
      elseif(${OCG_VARIANT_NAME} STREQUAL "FLR4x1_v1")
         set(OCG_RSPP_VARIANT_NAME "rspp_variant_G")
      elseif(${OCG_VARIANT_NAME} STREQUAL "FLR7x1_v1")
         set(OCG_RSPP_VARIANT_NAME "rspp_variant_A")
      elseif(${OCG_VARIANT_NAME} STREQUAL "SRR7px2_v1")
         set(OCG_RSPP_VARIANT_NAME "rspp_variant_A")
      endif()
   endif()

   # Set up preprocessor
   add_definitions(-Drspp_variant_A=${OCG_RSPP_VARIANT_NAME})
endif()

#add_subdirectory(iface)
add_subdirectory(core)
                            
##############################################################################
# Unittests
##############################################################################
option(OCG_UT "Unittest framework" OFF)
if(OCG_UT) 
   add_subdirectory(unit_tests)
   enable_testing()
endif()