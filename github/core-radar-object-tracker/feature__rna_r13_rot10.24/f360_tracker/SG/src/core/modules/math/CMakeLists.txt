cmake_minimum_required(VERSION 3.16)

add_library(SG-Math OBJECT)

target_include_directories(
    SG-Math
        PUBLIC iface
)

target_sources(
    SG-Math
        PRIVATE
            iface/sg_math.h
            iface/sg_matrix_operations.h
            iface/geometry/geo_circle.h
            iface/geometry/geo_distance.h
            iface/geometry/geo_is_inside.h
            iface/geometry/geo_length.h
            iface/geometry/geo_point.h
            iface/geometry/geo_rectangle.h
            iface/geometry/geo_intervals.h
            iface/geometry/geo_rotate.h
            iface/geometry/geo_segment.h
            iface/geometry/geo_angle.h
            iface/geometry/geo_line.h
            iface/geometry/geo_find_min_max.h
            iface/geometry/geo_point_on_segment_line.h
            src/geometry/geo_is_inside.cpp
            src/geometry/geo_rotate.cpp
            src/geometry/geo_line.cpp
            src/geometry/geo_find_min_max.cpp
            src/sg_math.cpp
)

target_link_libraries(
      SG-Math
         PRIVATE sg_compiler_config
)

get_target_property(src_files SG-Math SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
