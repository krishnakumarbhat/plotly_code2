target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(SCW PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_post_run.h)
target_sources(SCW PUBLIC ${CMAKE_CURRENT_LIST_DIR}/scw_pre_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${SCW_PROJECT_VARIANT}/${SCW_PROJECT_VARIANT}.cmake)
