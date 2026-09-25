cmake_minimum_required(VERSION 3.16)

add_library(stationary_geometries)

target_include_directories(
    stationary_geometries
        PRIVATE
            inc
)

target_sources(
    stationary_geometries
        PRIVATE
            inc/sg_dumping.h
            inc/sg_stationary_geometries_impl.h
            inc/sg_core_iface.h
            inc/sg_core.h
            inc/sg_input_diagnostics.h
            inc/sg_output_diagnostics.h

            src/sg_stationary_geometries.cpp
            src/sg_stationary_geometries_impl.cpp
            src/sg_core.cpp
            src/sg_dumping.cpp
            src/sg_input_diagnostics.cpp
            src/sg_output_diagnostics.cpp
)

target_link_libraries(
    stationary_geometries
        PUBLIC stationary_geometries_iface
        PRIVATE sg_compiler_config
        PRIVATE SG-Calibrations
        PRIVATE SG-Common
        PRIVATE SG-Host
        PRIVATE SG-Math
        PRIVATE SG-Time-Update
        PRIVATE SG-Detection-Processing
        PRIVATE SG-Detection-Clustering
        PRIVATE SG-Measurement-Association
        PRIVATE SG-Contour-Initialization
        PRIVATE SG-Postprocessing
        PRIVATE SG-Measurement-Update
        PRIVATE SG-Contour-Downselection
        PRIVATE SG-Critical_Region
        PRIVATE SG-Rspp
        PRIVATE SG-Entry-Data_Processing
)

if(SG_DC_ENABLED)
    target_link_libraries(
        stationary_geometries
            PRIVATE DC-Drivability-Classification
    )
    target_sources(
        stationary_geometries
            PRIVATE src/sg_core_with_dc.cpp
            PRIVATE inc/sg_core_with_dc.h
    )
endif()

add_subdirectory(modules)

get_target_property(src_files stationary_geometries SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
