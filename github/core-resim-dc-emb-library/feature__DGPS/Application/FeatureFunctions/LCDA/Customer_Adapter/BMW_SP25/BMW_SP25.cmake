add_library(lcda_iface ALIAS LCDA)
add_library(BMW_SP25_LCDA ALIAS LCDA)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_input_t.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_output_t.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_state_machine.h)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_pre_run.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_post_run.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_state_machine.c)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/camera_data_t.h)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lane_model.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lane_model.c)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lane_model_camera_data.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lane_model_camera_data.c)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_bmw_sp25_types.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_bmw_sp25_debug_interface.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_bmw_sp25_debug_interface.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_bmw_sp25_debug_writer.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_bmw_sp25_debug_writer.c)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(LCDA_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_LCDA")
