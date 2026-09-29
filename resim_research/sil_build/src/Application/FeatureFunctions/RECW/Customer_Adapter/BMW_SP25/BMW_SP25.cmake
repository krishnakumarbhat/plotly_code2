add_library(recw_iface ALIAS RECW)
add_library(BMW_SP25_RECW ALIAS RECW)

target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(RECW PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_post_run.c)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_pre_run.c)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_input_t.h)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_output_t.h)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_bmw_sp25_types.h)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_state_machine.c)
target_sources(RECW PRIVATE ${CMAKE_CURRENT_LIST_DIR}/recw_state_machine.h)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(RECW_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_RECW")
