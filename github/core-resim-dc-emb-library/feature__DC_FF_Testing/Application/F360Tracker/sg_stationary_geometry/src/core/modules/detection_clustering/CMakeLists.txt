cmake_minimum_required(VERSION 3.16)

add_library(SG-Detection-Clustering OBJECT)

target_include_directories(
   SG-Detection-Clustering
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Detection-Clustering
      PRIVATE
         iface/sg_squeeze_detections.h
         iface/sg_cluster_detections.h
         iface/sg_clustering_aliases.h
         iface/sg_cluster_detections_helpers.h
         iface/sg_expand_cluster_helpers.h
         inc/sg_temporal_dbscan.h
         inc/sg_merge_clusters.h
         inc/sg_update_detections_properties.h
         inc/sg_temporal_expand_cluster.h
         src/sg_squeeze_detections.cpp
         src/sg_cluster_detections.cpp
         src/sg_temporal_dbscan.cpp
         src/sg_merge_clusters.cpp
         src/sg_update_detections_properties.cpp
         src/sg_expand_cluster_helpers.cpp
         src/sg_temporal_expand_cluster.cpp
)

target_link_libraries(
   SG-Detection-Clustering
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Detection-Clustering SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})