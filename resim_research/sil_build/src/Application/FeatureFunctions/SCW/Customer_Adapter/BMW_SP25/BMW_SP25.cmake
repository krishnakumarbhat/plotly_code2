add_library(scw_iface ALIAS SCW)
add_library(BMW_SP25_SCW ALIAS SCW)

target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(SCW PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_input_t.h)
target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_output_t.h)
target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_state_machine.h)

target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_post_run.c)
target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_pre_run.c)
target_sources(SCW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scw_state_machine.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(SCW_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_SCW")
