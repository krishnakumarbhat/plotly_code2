target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LTB PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LTB PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ltb_pre_run.h)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_post_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${LTB_PROJECT_VARIANT}/${LTB_PROJECT_VARIANT}.cmake)
