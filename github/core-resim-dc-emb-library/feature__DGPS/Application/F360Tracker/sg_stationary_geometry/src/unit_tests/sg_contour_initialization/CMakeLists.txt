cmake_minimum_required(VERSION 3.16)

add_executable(sg_contour_initialization_ut)

target_sources(
   sg_contour_initialization_ut
      PRIVATE
         ut_sg_extract_cluster_ids.cpp
         ut_sg_contour_initialization.cpp
         ut_sg_init_bbox_stitch.cpp
         ut_sg_get_unoccluded_dets.cpp
         ut_sg_initialize_contours_helpers.cpp
         ut_sg_sort_clusters_by_priority.cpp
         ut_sg_try_initialize_contour.cpp
         ut_sg_initialize_contours_helpers.cpp
         ut_sg_initialize_contours.cpp
         ut_sg_dbscan.cpp
         ut_sg_expand_cluster.cpp
)

target_link_libraries(
   sg_contour_initialization_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Contour-Initialization
      PRIVATE SG-Detection-Clustering
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
)

target_include_directories(
   sg_contour_initialization_ut
      PRIVATE $<TARGET_PROPERTY:SG-Contour-Initialization,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_contour_initialization_ut)