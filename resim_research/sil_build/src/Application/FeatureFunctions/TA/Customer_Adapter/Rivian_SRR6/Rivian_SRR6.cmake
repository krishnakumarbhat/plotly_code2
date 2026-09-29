target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(TA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_input_t.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_output_t.h)

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_pre_run.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_post_run.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_input_boundary_check.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_output_boundary_check.c)
