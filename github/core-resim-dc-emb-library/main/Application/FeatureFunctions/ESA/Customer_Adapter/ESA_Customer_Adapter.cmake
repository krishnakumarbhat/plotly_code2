target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(ESA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(ESA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/esa_pre_run.h)
target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_post_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${ESA_PROJECT_VARIANT}/${ESA_PROJECT_VARIANT}.cmake)
