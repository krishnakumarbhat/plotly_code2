cmake_minimum_required(VERSION 3.16)

add_library(SG-Calibrations OBJECT)

target_include_directories(
    SG-Calibrations
        PUBLIC iface
        PRIVATE inc
)

target_sources(
    SG-Calibrations
        PRIVATE
            iface/sg_interpolator.h
            iface/sg_calibration_manager.h
            iface/sg_calibrations.h

            src/sg_calibration_manager.cpp
)

target_link_libraries(
    SG-Calibrations
        PRIVATE sg_compiler_config
        PRIVATE SG-Common
        PRIVATE stationary_geometries_iface
)

get_target_property(src_files SG-Calibrations SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})