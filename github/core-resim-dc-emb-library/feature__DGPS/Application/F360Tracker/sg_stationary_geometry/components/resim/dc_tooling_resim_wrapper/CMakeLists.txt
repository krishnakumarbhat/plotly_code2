cmake_minimum_required(VERSION 3.16)

add_library(dc_tooling_wrapper SHARED)

target_include_directories(
   dc_tooling_wrapper
      PUBLIC
         ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
   dc_tooling_wrapper
      PRIVATE
         dc_tooling_wrapper.h
         dc_tooling_wrapper.cpp
         dc_tooling_wrapper_lib.cpp
)

target_link_libraries(
   dc_tooling_wrapper
      PRIVATE DC-Drivability-Classification-Wrapper
      PRIVATE stationary_geometries_wrapper
      PRIVATE stationary_geometries
      PRIVATE sg_compiler_config
      PRIVATE COMPONENT_RESIM_CORE
      PRIVATE timer_resim
)

get_target_property(sg_wrapper_binary_dir stationary_geometries_wrapper BINARY_DIR)
set_target_properties(
   dc_tooling_wrapper 
      PROPERTIES
         RUNTIME_OUTPUT_DIRECTORY "${sg_wrapper_binary_dir}"

)

install(
    TARGETS
        dc_tooling_wrapper
    DESTINATION 
        sg/lib
)