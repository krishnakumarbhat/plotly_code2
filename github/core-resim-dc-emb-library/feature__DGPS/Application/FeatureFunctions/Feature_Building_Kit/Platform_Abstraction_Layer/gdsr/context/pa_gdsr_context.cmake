target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_FILE})
target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(Feature_Building_Kit
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${Feature_Building_Kit_PROJECT_VARIANT})
target_sources(Feature_Building_Kit
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${Feature_Building_Kit_PROJECT_VARIANT}/pa_context.c)
target_sources(Feature_Building_Kit
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${Feature_Building_Kit_PROJECT_VARIANT}/pa_context.h)
