target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(RECW PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_FILE})
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_post_run.c)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_pre_run.c)
target_sources(RECW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/recw_input_t.h)
target_sources(RECW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/recw_output_t.h)
