
target_include_directories(SharedDevelopmentTools PUBLIC ${CMAKE_CURRENT_LIST_DIR}/iface)
target_include_directories(SharedDevelopmentTools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_sources(SharedDevelopmentTools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/Timing.c)
target_sources(SharedDevelopmentTools PUBLIC ${CMAKE_CURRENT_LIST_DIR}/iface/Timing.h)

target_sources(SharedDevelopmentTools PRIVATE ${CMAKE_CURRENT_LIST_FILE})

