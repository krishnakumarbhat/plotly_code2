target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LTB PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LTB PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ltb_input_t.h)
target_sources(LTB PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ltb_output_t.h)

target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_pre_run.c)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_post_run.c)
