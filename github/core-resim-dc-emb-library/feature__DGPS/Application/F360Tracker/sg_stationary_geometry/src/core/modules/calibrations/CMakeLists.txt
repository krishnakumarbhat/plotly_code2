cmake_minimum_required(VERSION 3.16)

add_library(SG-Calibrations OBJECT)

target_include_directories(
    SG-Calibrations
        PUBLIC iface
        PUBLIC iface/calibrations
)

target_sources(
    SG-Calibrations
        PRIVATE
            iface/sg_interpolator.h
            iface/sg_calibration_manager.h
            iface/sg_calibration_initializer.h
            iface/sg_calibration_initializer_platform_high.h
            iface/sg_calibration_initializer_platform_medium.h
            iface/sg_calibration_initializer_platform_low.h
            iface/sg_calibration_initializer_platform_unlimited.h
            iface/sg_calibration_initializer_platform_reduced.h
            iface/sg_calibration_initializer_helpers.h
            iface/sg_calibration_initializer_scania.h
            iface/sg_calibrations.h
            iface/calibrations/sg_time_update_calibrations.h
            iface/calibrations/sg_time_update_calibrations_helpers.h
            iface/calibrations/sg_common_calibrations.h
            iface/calibrations/sg_common_calibrations_helpers.h
            iface/calibrations/sg_contour_downselection_calibrations.h
            iface/calibrations/sg_contour_initialization_calibrations.h
            iface/calibrations/sg_contour_postprocessing_calibrations.h
            iface/calibrations/sg_contour_postprocessing_calibrations_helpers.h
            iface/calibrations/sg_critical_region_calibrations.h
            iface/calibrations/sg_dc_fusion_calibrations.h
            iface/calibrations/sg_detection_clustering_calibrations.h
            iface/calibrations/sg_detection_clustering_calibrations_helpers.h
            iface/calibrations/sg_detection_processing_calibrations.h
            iface/calibrations/sg_detection_processing_calibrations_helpers.h
            iface/calibrations/sg_drivability_classification_calibrations.h
            iface/calibrations/sg_measurement_association_calibrations.h
            iface/calibrations/sg_measurement_update_calibrations.h

            src/sg_calibration_manager.cpp
            src/sg_calibration_initializer_platform_high.cpp
            src/sg_calibration_initializer_platform_medium.cpp
            src/sg_calibration_initializer_platform_low.cpp
            src/sg_calibration_initializer_platform_unlimited.cpp
            src/sg_calibration_initializer_platform_reduced.cpp
            src/sg_calibration_initializer_scania.cpp
)

target_link_libraries(
    SG-Calibrations
        PRIVATE sg_compiler_config
        PRIVATE SG-Common
        PRIVATE stationary_geometries_iface
)

get_target_property(src_files SG-Calibrations SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})