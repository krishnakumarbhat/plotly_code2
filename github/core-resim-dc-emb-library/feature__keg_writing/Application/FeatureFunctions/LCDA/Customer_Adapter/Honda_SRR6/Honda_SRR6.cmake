target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_input_t.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_output_t.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_honda_instance.h)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_pre_run.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_post_run.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_honda_instance.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(LCDA_make_alias "z7b/Feature_Functions/FF_LCDA")
