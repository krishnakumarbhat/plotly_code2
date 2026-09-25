cmake_minimum_required(VERSION 3.16)

add_executable(sg_postprocessing_ut)

target_sources(
   sg_postprocessing_ut
      PRIVATE
         ut_sg_postprocessing_helpers.h
         ut_sg_remove_vertices.cpp
         ut_sg_select_and_remove_excess_contours.cpp
         ut_sg_split_or_shorten_contour.cpp
         ut_sg_dissociate_detections.cpp
         ut_sg_get_contour_to_host_distance.cpp
         ut_sg_mark_out_of_range_vertices.cpp
         ut_sg_mark_uncertain_vertices.cpp
         ut_sg_mark_unreliable_vertices.cpp
         ut_sg_split_contour_at_specified_indices.cpp
         ut_sg_get_indices_of_mask.cpp
         ut_sg_is_split_scenario.cpp
         ut_sg_presimplify_contours.cpp
         ut_sg_declutter_contours.cpp
         ut_sg_declutter_contours_helpers.cpp
         ut_sg_simplify_contours_helpers.cpp
         ut_sg_postprocessing_helpers.cpp
         ut_sg_simplify_contours.cpp;
         ut_sg_merge_contours.cpp
         ut_sg_merge_two_contours.cpp
         ut_sg_find_and_merge_pair.cpp
         ut_sg_flip_contour.cpp
)

target_link_libraries(
   sg_postprocessing_ut
      PRIVATE sg_ut_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Postprocessing
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Contour-Initialization
      PRIVATE SG-Detection-Clustering
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Calibrations
      PRIVATE SG-Detection-Processing
      PRIVATE SG-Dummy-Generator
)

target_include_directories(
   sg_postprocessing_ut
      PRIVATE $<TARGET_PROPERTY:SG-Postprocessing,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_postprocessing_ut)
