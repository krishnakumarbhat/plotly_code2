# Include Customer specific adapter

target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_FILE})
target_include_directories(RECW PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_post_run.h)
target_sources(RECW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/recw_pre_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${RECW_PROJECT_VARIANT}/${RECW_PROJECT_VARIANT}.cmake)
