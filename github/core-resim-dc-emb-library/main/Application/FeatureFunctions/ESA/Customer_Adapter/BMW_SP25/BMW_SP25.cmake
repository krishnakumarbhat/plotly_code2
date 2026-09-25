add_library(esa_iface ALIAS ESA)
add_library(BMW_SP25_ESA ALIAS ESA)

target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(ESA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_input_t.h)
target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_output_t.h)

target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_pre_run.c)
target_sources(ESA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/esa_post_run.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(ESA_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_ESA")
