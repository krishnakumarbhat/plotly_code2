target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cta_pre_run.h)

target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cta_post_run.h)

include(${CMAKE_CURRENT_LIST_DIR}/${CTA_PROJECT_VARIANT}/${CTA_PROJECT_VARIANT}.cmake)
