cmake_minimum_required(VERSION 3.16)

add_executable(common_ut)

target_sources(
   common_ut
      PRIVATE
         ut_sg_detection.cpp
         ut_sg_detection_storage.cpp
         ut_sg_contour_storage.cpp
         ut_sg_detection_storage_cache.cpp
         ut_sg_detection_cache.cpp
         ut_sg_detection_storage_iterator.cpp
         ut_sg_detection_storage_view.cpp
         ut_sg_contour.cpp
         ut_sg_cluster.cpp
         ut_sg_clusters.cpp
         ut_sg_importance_calibrations.cpp
         ut_sg_timing_info.cpp
         ut_sg_detection_bins.cpp
         ut_sg_detection_storage_test_helpers.h
         ut_sg_detection_storage_test_helpers.cpp
         ut_sg_data_fusion.cpp
)

target_link_libraries(
  common_ut
    PRIVATE sg_ut_config
    PRIVATE SG-Common
    PRIVATE stationary_geometries_iface
    PRIVATE SG-Calibrations
)

include(GoogleTest)
gtest_discover_tests(common_ut)
