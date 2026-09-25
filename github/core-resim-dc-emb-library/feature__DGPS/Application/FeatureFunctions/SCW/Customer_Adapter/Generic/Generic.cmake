target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(SCW PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(SCW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/scw_input_t.h)
target_sources(SCW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/scw_output_t.h)

target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_post_run.c)
target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_pre_run.c)
