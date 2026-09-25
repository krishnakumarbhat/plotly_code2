cmake_minimum_required(VERSION 3.16)

add_executable(sg_math_ut)

target_sources(
   sg_math_ut
      PRIVATE
         ut_sg_math.cpp
         ut_sg_matrix_operations.cpp
         geometry/ut_geo_point.cpp
         geometry/ut_geo_rectangle.cpp
         geometry/ut_geo_intervals.cpp
         geometry/ut_geo_rotate_point.cpp
         geometry/ut_geo_segment.cpp
         geometry/ut_geo_length.cpp
         geometry/ut_geo_circle.cpp
         geometry/ut_geo_is_inside.cpp
         geometry/ut_geo_rotate_rectangle.cpp
         geometry/ut_geo_distance.cpp
         geometry/ut_geo_angle.cpp
         geometry/ut_geo_find_min_max.cpp
         geometry/ut_geo_line.cpp
         geometry/ut_geo_footpoint.cpp
         geometry/ut_point_on_segment_line.cpp
         geometry/ut_geo_projection.cpp
)

target_link_libraries(
  sg_math_ut
    PRIVATE sg_ut_config
    PRIVATE stationary_geometries
    PRIVATE SG-Common
    PRIVATE SG-Math
)

include(GoogleTest)
gtest_discover_tests(sg_math_ut)
