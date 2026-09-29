target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_post_run.h)
target_sources(LCDA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lcda_pre_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${LCDA_PROJECT_VARIANT}/${LCDA_PROJECT_VARIANT}.cmake)
