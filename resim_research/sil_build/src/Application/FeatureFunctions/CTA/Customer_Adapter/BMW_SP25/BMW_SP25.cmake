add_library(BMW_SP25_CTA ALIAS CTA)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_bmw_sp25_types.h)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_bmw_sp25_warn_state.h)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_input_t.h)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_output_t.h)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_state_machine.h)

target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_post_run.c)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_pre_run.c)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_bmw_sp25_warn_state.c)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_state_machine.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(CTA_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_CTA")
