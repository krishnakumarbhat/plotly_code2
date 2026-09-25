cmake_minimum_required(VERSION 3.16)

add_library(ocg_common STATIC)

target_include_directories(
   ocg_common
      PUBLIC
         include
)

target_sources(
   ocg_common
      PRIVATE
         include/cmn_math_constants.h
         include/cmn_math_func.h
         include/cmn_fill_internals_helper.h
         source/cmn_math_func.cpp
         source/cmn_calc_circular_zone_idx.cpp
         source/cmn_fill_internals_helper.cpp
)

target_link_libraries(
   ocg_common
      PUBLIC
         occupancy_grid_iface
)

get_target_property(src_files ocg_common SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
