target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(PT PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_constants.h)
target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_version_ford.h)

target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_constants.c)
target_sources(PT PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_version_ford.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(PT_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/Ford_DAT2_1_PT")
