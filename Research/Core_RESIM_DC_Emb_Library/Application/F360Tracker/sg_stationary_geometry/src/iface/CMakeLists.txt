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
			../../../rspp/include
			../../../F360TrackerLib/SharedTrackerAPI/core
			../../../F360TrackerLib/SharedTrackerAPI/Logging
			../../../F360TrackerLib/SharedTrackerAPI/Types
			
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
      INTERFACE F360-Tracker
)
