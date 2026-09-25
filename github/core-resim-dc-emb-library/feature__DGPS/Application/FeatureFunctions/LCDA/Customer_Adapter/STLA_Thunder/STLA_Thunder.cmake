target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_input_t.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_output_t.h)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_pre_run.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_post_run.c)
