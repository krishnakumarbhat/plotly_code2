target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(PT PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_constants.h)
target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_constants.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(PT_make_alias "z7b/Feature_Functions/FF_PT")
