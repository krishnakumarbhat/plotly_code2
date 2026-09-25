add_library(ced_iface ALIAS CED)
add_library(BMW_SP25_CED ALIAS CED)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_input_t.h)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_output_t.h)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_bmw_sp25_types.h)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_bmw_sp25_init.h)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_state_machine.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_pre_run.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_post_run.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_bmw_sp25_init.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ced_state_machine.c)

target_include_directories(CED PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_mirror_led.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_mirror_led.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_optical_element.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_optical_element.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_interior_light.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_interior_light.h)

target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_seat_doors.c)
target_sources(CED PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Ced_Post_Run_Boardnet/ced_post_run_boardnet_seat_doors.h)

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(CED_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/BMW_SP25_SFE")
