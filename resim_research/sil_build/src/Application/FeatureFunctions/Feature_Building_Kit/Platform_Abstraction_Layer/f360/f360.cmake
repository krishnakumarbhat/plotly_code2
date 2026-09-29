target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_FILE})
target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Link F360 Tracker related API
target_link_libraries(Feature_Building_Kit F360_Tracker_api_types)

# Link to MLMathLibrary for Macro functions
target_link_libraries(Feature_Building_Kit MLMathLibrary)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports)
target_include_directories(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/pa_context.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/pa_context.c)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_obj_in.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_vehicle_in.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_env_in.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_reuse.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_const_macros.h)
