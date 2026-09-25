cmake_minimum_required(VERSION 3.16)

add_library(stationary_geometries_iface INTERFACE)

target_include_directories(
    stationary_geometries_iface
        INTERFACE
            .
            input
            types
            types/contour
            types/dump
)

target_include_directories(
    stationary_geometries_iface
        INTERFACE
			../../../Wrappers/rspp_iface_variant_A/include
			../../../Wrappers/trackerLib_qnx_variant_A/iface_variant_A/core
			../../../Wrappers/trackerLib_qnx_variant_A/iface_variant_A/Logging
			../../../Wrappers/trackerLib_qnx_variant_A/iface_variant_A/Types
			
)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.22.0")
target_sources(
    stationary_geometries_iface
        PRIVATE
            sg_stationary_geometries.h
            sg_constants.h
            sg_reuse.h
            sg_sw_version.h

            types/sg_log_stream_info.h
            types/sg_output.h
            types/sg_reduced_output.h
            types/sg_input.h
            types/sg_rot_rspp_chooser.h
            types/sg_vertex_out.h
            types/sg_algorithm_step.h
            types/dc_algorithm_step.h
            types/sg_timer_base.h
            types/sg_drivability_class.h

            types/contour/sg_contour_out.h
            types/contour/sg_contour_type.h

            types/dump/sg_internals_dump.h
            types/dump/sg_timing_dump.h
            types/dump/sg_timing_details.h
            types/dump/sg_contour_dump.h
            types/dump/sg_vertex_dump.h
            types/dump/sg_contour_storage_dump.h
            types/dump/sg_detection_storage_dump.h
            types/dump/sg_detection_dump.h
            types/dump/dc_dump.h
            types/dump/dc_critical_region_dump.h
            types/dump/dc_vertices_dump.h
            types/dump/dc_features_dump.h
            types/dump/dc_contours_dump.h
)
if(SG_SAVE_ASSIGNED_DETS_TO_SUBSEGMENTS)
    target_sources(
        stationary_geometries_iface
            PRIVATE
                types/dump/dc_assigned_detections_dump.h
    )
endif()
get_target_property(src_files stationary_geometries_iface SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
endif()

target_link_libraries(
   stationary_geometries_iface
      INTERFACE rspp
      INTERFACE F360-Tracker
)

# Set up preprocessor

set(VARIANT_TRACKER "f360_variant_A")
set(RSPP_VARIANT_TRACKER "rspp_variant_A")

add_definitions(-Df360_variant_A=${VARIANT_TRACKER})
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_TRACKER})

get_target_property(src_files stationary_geometries_iface SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})