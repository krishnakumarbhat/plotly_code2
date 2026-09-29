target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(PT PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(PT PUBLIC ${CMAKE_CURRENT_LIST_DIR}/pt_constants.h)
target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_constants.c)
