target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ced_pre_run.h)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_post_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${CED_PROJECT_VARIANT}/${CED_PROJECT_VARIANT}.cmake)
