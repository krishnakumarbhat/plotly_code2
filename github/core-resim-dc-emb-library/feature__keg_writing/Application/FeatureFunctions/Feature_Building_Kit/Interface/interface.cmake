target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fbk_iface.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fbk_iface.c)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fbk_iface_types.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fbk_instance.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fbk_output.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fbk_ego_traj_predictor_instance.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/sfl_status.h)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/index_lookup)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/index_lookup/fbk_index_lookup.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/index_lookup/fbk_index_lookup.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/object_ageing)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/object_ageing/fbk_obj_ageing.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/object_ageing/fbk_obj_ageing.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/debug_writer)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/debug_writer/fbk_debug_writer.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/debug_writer/fbk_debug_writer.c)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/debug_writer/fbk_debug_interface.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/debug_writer/fbk_debug_interface.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/boundaries)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/boundaries/fbk_output_boundary_check.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/boundaries/fbk_output_boundary_check.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/host_trail)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/host_trail/fbk_host_trail.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/host_trail/fbk_host_trail.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fill_pa_data)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fill_pa_data/fbk_fill_pa_data.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fill_pa_data/fbk_fill_pa_data.c)
