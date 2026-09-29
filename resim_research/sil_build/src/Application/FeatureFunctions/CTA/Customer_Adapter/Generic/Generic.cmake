target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cta_input_t.h)
target_sources(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cta_output_t.h)

target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_post_run.c)

target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_pre_run.c)
