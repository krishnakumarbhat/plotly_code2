cmake_minimum_required(VERSION 3.16)

add_library(SG-Common OBJECT)

target_include_directories(
    SG-Common
        PUBLIC iface
)

target_sources(
    SG-Common
        PRIVATE
            iface/sg_common.h
            iface/sg_detection_cache.h
            iface/sg_detection_storage.h
            iface/sg_contour_storage.h
            iface/sg_contour.h
            iface/sg_cluster.h
            iface/embedded_list.h
            iface/fifo.h
            iface/unique_id_handler.h
            iface/memory_pool.h
            iface/container_view.h
            iface/sg_detection.h
            iface/sg_basic.h
            iface/sg_vertex.h
            iface/sg_timing_info.h
            iface/id_handler_incremental.h
            iface/sg_clusters.h

            src/sg_common.cpp
            src/sg_detection.cpp
            src/sg_detection_storage.cpp
            src/sg_detection_cache.cpp
            src/sg_contour_storage.cpp
            src/sg_clusters.cpp
            src/sg_importance_calibrations.cpp
)

target_link_libraries(
    SG-Common
         PRIVATE sg_compiler_config
         PRIVATE stationary_geometries_iface
         PUBLIC SG-Math
)

get_target_property(src_files SG-Common SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})