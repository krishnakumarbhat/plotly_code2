target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ced_generic_types.h)

target_sources(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ced_input_t.h)
target_sources(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ced_output_t.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_pre_run.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_post_run.c)
