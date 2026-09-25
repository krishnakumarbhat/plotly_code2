cmake_minimum_required(VERSION 3.16)

add_library(SG-Contour-Initialization OBJECT)

target_include_directories(
   SG-Contour-Initialization
      PUBLIC iface
      PUBLIC inc
)

target_sources(
   SG-Contour-Initialization
      PRIVATE
         iface/sg_initialize_contours.h
         iface/sg_init_contour_bbox_stitch.h
         iface/sg_init_aliases.h

         inc/sg_extract_contoured_cluster_ids.h
         inc/sg_try_initialize_contour.h
         inc/sg_sort_clusters_by_priority.h
         inc/sg_get_unoccluded_dets.h
         inc/sg_initialize_contours_helpers.h
         inc/sg_dbscan.h
         inc/sg_expand_cluster.h

         src/sg_initialize_contours.cpp
         src/sg_sort_clusters_by_priority.cpp
         src/sg_try_initialize_contour.cpp
         src/sg_extract_contoured_cluster_ids.cpp
         src/sg_init_contour_bbox_stitch.cpp
         src/sg_get_unoccluded_dets.cpp
         src/sg_initialize_contours_helpers.cpp
         src/sg_dbscan.cpp
         src/sg_expand_cluster.cpp
)

target_link_libraries(
   SG-Contour-Initialization
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Detection-Clustering
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Contour-Initialization SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
