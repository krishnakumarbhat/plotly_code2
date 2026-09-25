cmake_minimum_required (VERSION 2.8.12)
set(PROJECTNAME someipGateway)
project (${PROJECTNAME})
set(target_name ${PROJECTNAME})

add_library(${target_name} SHARED "")

target_link_libraries(${target_name} ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${target_name} MdfLog)

list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../src/someipGateway.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../src/someipGatewayA430.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../src/someipGatewayA450.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../src/someIp.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../inc/someipGateway.h)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../inc/someipGatewayA430.h)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../inc/someipGatewayA450.h)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/../inc/ISomeIp.h)

target_sources(${target_name} PRIVATE ${SOURCE_FILES})

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../inc)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files)

set_target_properties( ${target_name}    PROPERTIES
									ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/.. FILES ${SOURCE_FILES})
target_link_libraries(${target_name} CCA_ViGEM)
target_compile_definitions(${target_name} PRIVATE WRITE_TRANSMITTED_DATA)

if(UNIX)
find_package(Threads)
target_link_libraries(${target_name} ${CMAKE_THREAD_LIBS_INIT} )
endif(UNIX)