cmake_minimum_required(VERSION 3.16)

add_library(stationary_geometries)


OPTION(DISABLE_SG_ASSERT "Disable SG Asserts" ON)

if(DISABLE_SG_ASSERT)
add_compile_definitions("NDEBUG")
endif()

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
            inc/sg_core.h

            src/sg_stationary_geometries.cpp
            src/sg_stationary_geometries_impl.cpp
            src/sg_core.cpp
            src/sg_dumping.cpp
)

target_link_libraries(
    stationary_geometries
        PUBLIC stationary_geometries_iface
        
        PRIVATE SG-Calibrations
        PRIVATE sg_compiler_config
        PRIVATE SG-Common
        PRIVATE SG-Host
        PRIVATE SG-Math
        PRIVATE SG-Safety
)

target_link_libraries(
    stationary_geometries
        PRIVATE SG-Time-Update
        PRIVATE SG-Detection-Processing
        PRIVATE SG-Detection-Clustering
        PRIVATE SG-Measurement-Association
        PRIVATE SG-Contour-Initialization
        PRIVATE SG-Postprocessing
        PRIVATE SG-Measurement-Update
        PRIVATE SG-Contour-Downselection
        PRIVATE DC-Drivability-Classification
)

add_subdirectory(modules)

install(
    TARGETS
        stationary_geometries
    DESTINATION
        sg/lib
)



get_target_property(src_files stationary_geometries SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
