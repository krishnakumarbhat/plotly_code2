target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_FILE})
target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_const_macros.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_obj_in.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_vehicle_in.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_env_in.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_reuse.h)
