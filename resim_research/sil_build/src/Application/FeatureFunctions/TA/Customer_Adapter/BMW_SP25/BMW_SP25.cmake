add_library(ta_iface ALIAS TA)
add_library(BMW_SP25_TA ALIAS TA)

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(TA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_input_t.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_output_t.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_boardnet_t.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_sp25_types.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_state_machine.h)

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_enums.h)

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_diagnostic.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_diagnostic.c)

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_sp25_debug_interface.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_sp25_debug_interface.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_sp25_debug_writer.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_bmw_sp25_debug_writer.c)

target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_pre_run.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_post_run.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_state_machine.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_input_boundary_check.c)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_output_boundary_check.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(TA_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_TA")
