add_library(ltb_iface ALIAS LTB)
add_library(BMW_SP25_LTB ALIAS LTB)

target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LTB PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_input_t.h)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_output_t.h)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_state_machine.h)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_bmw_sp25_types.h)

target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_pre_run.c)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_post_run.c)
target_sources(LTB PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ltb_state_machine.c)
# This variable specifies the project specific feature folder (used for automatic make file generation)
set(LTB_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LTB")
