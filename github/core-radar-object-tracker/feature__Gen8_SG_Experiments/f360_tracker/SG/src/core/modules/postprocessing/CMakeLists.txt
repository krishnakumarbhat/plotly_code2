cmake_minimum_required(VERSION 3.16)

add_library(SG-Postprocessing OBJECT)

target_include_directories(
   SG-Postprocessing
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Postprocessing
      PRIVATE
         iface/sg_dissociate_detections.h
         iface/sg_merge_contours.h
         iface/sg_remove_vertices.h
         iface/sg_select_and_remove_excess_contours.h
         iface/sg_dissociate_detections.h
         iface/sg_declutter_contours.h
         iface/sg_simplify_contours.h
	      iface/sg_split_or_shorten_contour.h
         inc/sg_remove_vertices_helpers.h
         inc/sg_presimplify_contours.h
         inc/sg_simplify_contours_helpers.h
         inc/sg_declutter_contours_helpers.h
         inc/sg_merge_contours_helpers.h

         src/sg_dissociate_detections.cpp
         src/sg_merge_contours.cpp
         src/sg_remove_vertices_helpers.cpp
         src/sg_remove_vertices.cpp
         src/sg_select_and_remove_excess_contours.cpp
         src/sg_dissociate_detections.cpp
         src/sg_presimplify_contours.cpp
         src/sg_declutter_contours.cpp
         src/sg_declutter_contours_helpers.cpp
         src/sg_simplify_contours_helpers.cpp
         src/sg_simplify_contours.cpp
         src/sg_merge_contours_helpers.cpp
	      src/sg_split_or_shorten_contour.cpp
)

target_link_libraries(
   SG-Postprocessing
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Contour-Initialization
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Postprocessing SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
