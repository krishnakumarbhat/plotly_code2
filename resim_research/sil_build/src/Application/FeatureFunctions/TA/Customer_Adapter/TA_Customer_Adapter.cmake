target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(TA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(TA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ta_pre_run.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_post_run.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_input_boundary_check.h)
target_sources(TA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ta_output_boundary_check.h)

include(${CMAKE_CURRENT_LIST_DIR}/${TA_PROJECT_VARIANT}/${TA_PROJECT_VARIANT}.cmake)
