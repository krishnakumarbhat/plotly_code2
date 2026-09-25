target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_output_t.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_iface.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_types.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_directions.h)
