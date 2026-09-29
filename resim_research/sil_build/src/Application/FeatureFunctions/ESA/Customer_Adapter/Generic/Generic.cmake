target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(ESA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(ESA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/esa_input_t.h)
target_sources(ESA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/esa_output_t.h)

target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_pre_run.c)
target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_post_run.c)
