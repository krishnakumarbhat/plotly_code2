cmake_minimum_required(VERSION 3.6)

add_library(OLP_Core)

if (NOT DEFINED OLP_NUMBER_OF_OBJECTS_DEFINED)
    add_definitions(-DOLP_NUMBER_OF_OBJECTS=50)
endif()

if (NOT DEFINED OLP_OBJ_SOURCE_F360)
    add_definitions(-DOLP_OBJ_SOURCE_F360)
endif()

# Add include folders.
target_include_directories(
   OLP_Core 
      PUBLIC
	     ${CMAKE_CURRENT_LIST_DIR}/includes
		 ${CMAKE_CURRENT_LIST_DIR}/../../../FeatureFunctions/Math_Library/development/dependencies
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