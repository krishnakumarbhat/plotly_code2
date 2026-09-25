cmake_minimum_required(VERSION 3.16)

add_executable(sg_detection_clustering_ut)

target_sources(
   sg_detection_clustering_ut
      PRIVATE
         ut_sg_squeeze_detections.cpp
         ut_sg_cluster_detections.cpp
         ut_sg_cluster_detections_helpers.cpp
         ut_sg_cluster_detections_utils.h
         ut_sg_cluster_detections_utils.cpp
         ut_sg_update_detections_properties.cpp
         ut_sg_temporal_dbscan.cpp
         ut_sg_temporal_dbscan_utils.cpp
         ut_sg_temporal_expand_cluster.cpp
         ut_sg_merge_clusters.cpp
)

target_link_libraries(
   sg_detection_clustering_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Detection-Clustering
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
)

target_include_directories(
   sg_detection_clustering_ut
      PRIVATE $<TARGET_PROPERTY:SG-Detection-Clustering,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_detection_clustering_ut)