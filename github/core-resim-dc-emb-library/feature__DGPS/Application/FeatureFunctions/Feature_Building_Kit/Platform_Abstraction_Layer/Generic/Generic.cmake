target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_FILE})
target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Link to MLMathLibrary for Macro functions
target_link_libraries(Feature_Building_Kit MLMathLibrary)

if(NOT DEFINED Feature_Building_Kit_PA_OBJ_NUMBER_OF_OBJECTS)
   message(FATAL_ERROR "Feature_Building_Kit_PA_OBJ_NUMBER_OF_OBJECTS is not defined")
endif()
message(STATUS "Generic PA builded with ${Feature_Building_Kit_PA_OBJ_NUMBER_OF_OBJECTS} object")
target_compile_definitions(Feature_Building_Kit
                           PUBLIC PA_OBJ_NUMBER_OF_OBJECTS=${Feature_Building_Kit_PA_OBJ_NUMBER_OF_OBJECTS})

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports)
target_include_directories(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/pa_context.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_obj_in.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_vehicle_in.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_env_in.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_const_macros.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/data_ports/pa_reuse.h)
