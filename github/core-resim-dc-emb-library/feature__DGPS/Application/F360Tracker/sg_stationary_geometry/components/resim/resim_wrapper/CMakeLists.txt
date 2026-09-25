cmake_minimum_required(VERSION 3.16)

if (SG_DRIVABILITY_CLASSIFICATION_TOOLING_WRAPPER)
   add_library(stationary_geometries_wrapper STATIC)
else()
   add_library(stationary_geometries_wrapper SHARED)
endif()

target_include_directories(
   stationary_geometries_wrapper
      PUBLIC
         ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
   stationary_geometries_wrapper
      PRIVATE
         stationary_geometries_wrapper.h
         stationary_geometries_wrapper.cpp
         stationary_geometries_wrapper_lib.cpp
)

target_link_libraries(
   stationary_geometries_wrapper
      PRIVATE stationary_geometries
      PRIVATE sg_compiler_config
      PRIVATE COMPONENT_RESIM_CORE
      PRIVATE timer_resim
      PRIVATE SG-Rspp-iface
)

get_target_property(src_files stationary_geometries_wrapper SOURCES)

install(
    TARGETS
        stationary_geometries_wrapper
    DESTINATION 
        sg/lib
)