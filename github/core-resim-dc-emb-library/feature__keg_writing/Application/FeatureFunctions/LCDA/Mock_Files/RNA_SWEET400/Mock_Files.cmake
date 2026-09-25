target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/TRACKER_OUTPUT_RNA_T.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/tracker_post_run_rna.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/tracker_post_run_rna.c)
