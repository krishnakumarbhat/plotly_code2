target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CTA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_output_t.h)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_iface.h)
target_sources(CTA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pt_iface.c)
