cmake_minimum_required(VERSION 3.16)

add_library(occupancy_grid_iface INTERFACE)

target_include_directories(occupancy_grid_iface
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(
  occupancy_grid_iface
  INTERFACE ocg_occupancy_grid.h
            ocg_occupancy_grid_types.h
            ocg_occupancy_grid_definition.h
            ocg_calibrations.h
            ocg_underdrivability_enum.h
            ocg_underdrivability_states.h
            ocg_underdrivability_type.h
            ocg_internals_type.h
            ocg_position.h
            ocg_reuse.h
            ocg_variant_type.h
            ocg_version.h
            ocg_internals_log_type.h
            ocg_output_log_type.h
			ocg_internals_log_type_converters.h)

add_subdirectory(ocg_variants)

target_link_libraries(occupancy_grid_iface INTERFACE ocg_constants)

if(${OCG_USE_INTERNAL_INCLUDES})

  target_include_directories(
    occupancy_grid_iface
    INTERFACE
      (${CMAKE_CURRENT_SOURCE_DIR}/../../external/rspp/${OCG_RSPP_VARIANT_NAME}/
      ))
  message(
    WARNING "Using default internal headers. Directory containing required "
            "include files should be supplied externaly for each platform.")
endif()
