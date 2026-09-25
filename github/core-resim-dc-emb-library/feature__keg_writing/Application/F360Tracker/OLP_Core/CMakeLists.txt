cmake_minimum_required(VERSION 3.6)

add_library(OLP_Core)

option (OLP_OBJ_SOURCE_F360 "Set F360 Comp Resim source" FALSE)
if (OLP_OBJ_SOURCE_F360)
   if (NOT DEFINED OLP_NUMBER_OF_OBJECTS_DEFINED)
      add_definitions(-DOLP_NUMBER_OF_OBJECTS=250)
   endif()
   add_definitions(-DOLP_OBJ_SOURCE_F360)
else()
   if (NOT DEFINED OLP_NUMBER_OF_OBJECTS_DEFINED)
      add_definitions(-DOLP_NUMBER_OF_OBJECTS=250)
   endif()
endif()

# Add include folders.
target_include_directories(
   OLP_Core 
      PUBLIC
	     ${CMAKE_CURRENT_LIST_DIR}/includes
        ${CMAKE_CURRENT_LIST_DIR}/../../FeatureFunctions/Math_Library/development/dependencies
        ${CMAKE_CURRENT_LIST_DIR}/../../FeatureFunctions/Math_Library/ml_core/include
        ${CMAKE_CURRENT_LIST_DIR}/../../FeatureFunctions/Math_Library/ml_core/include/legacy
)

# Add source files.
target_sources(
   OLP_Core 
      PRIVATE
         ${CMAKE_CURRENT_LIST_DIR}/source/olp.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_iface.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_snail_trail_computation.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_compute_curvi_data.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_distance_based_curvature.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_ml_math.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_compute_cog.cpp
         ${CMAKE_CURRENT_LIST_DIR}/source/olp_calibration.cpp
)

target_link_libraries(OLP_Core PUBLIC MLMathLibrary)