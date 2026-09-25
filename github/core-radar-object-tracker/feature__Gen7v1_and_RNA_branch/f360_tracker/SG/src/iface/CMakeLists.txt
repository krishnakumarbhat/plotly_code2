cmake_minimum_required(VERSION 3.16)

add_library(stationary_geometries_iface INTERFACE)

target_include_directories(
    stationary_geometries_iface
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/types>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/types/contour>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/types/dump>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/variants>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
            $<INSTALL_INTERFACE:iface>
)

if(SG_DRIVABILITY_CLASSIFICATION_WRAPPER)
   # Done instead of $<$<BOOL:${SG_DRIVABILITY_CLASSIFICATION_WRAPPER}>:>
   # due to install generation rules -> install-rules.cmake
   target_sources(
       stationary_geometries_iface
         PRIVATE types/dump/dc_assigned_detections_dump.h
   )
endif()

target_link_libraries(
   stationary_geometries_iface
      INTERFACE rspp_iface
      INTERFACE F360-Tracker
)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.22.0")
   # Using GLOB is fine approach if a target is INTERFACE type.
   file(GLOB_RECURSE interface_files CONFIGURE_DEPENDS "*.h")
   file(GLOB_RECURSE cmake_template_files CONFIGURE_DEPENDS "*.in")

   target_sources(
      stationary_geometries_iface
         PRIVATE
            ${interface_files}
            ${cmake_template_files}
   )

   get_target_property(src_files stationary_geometries_iface SOURCES)
   source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
endif()


if(NOT DEFINED SG_VARIANT)
    message(FATAL_ERROR "SG: SG variant is not set")
endif()

if(NOT DEFINED RSPP_VARIANT)
    message(FATAL_ERROR "SG: RSPP variant is not set")
endif()

if(NOT DEFINED F360_TRACKER_VARIANT)
    message(FATAL_ERROR "SG: ROT/F360 variant is not set")
endif()

include(cmake/SetupStreams.cmake)
setup_streams()

include(cmake/GenerateVariantDependentFiles.cmake)
generate_variant_dependent_files()
