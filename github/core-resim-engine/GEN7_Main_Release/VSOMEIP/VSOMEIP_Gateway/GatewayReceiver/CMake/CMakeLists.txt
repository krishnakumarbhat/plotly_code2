#set project name
set(proj_name "GatewayReceiver")

#set project
project (${proj_name})

#create executable
add_executable(${proj_name} "")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#set root path
set(GATEWAY_RECEIVER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
#set module and Libs path
set(SOURCE_PATH "${GATEWAY_RECEIVER_PATH}/source/")
set(HEADER_PATH "${GATEWAY_RECEIVER_PATH}/include/")

# include source files
set(SRC_FILES ${SOURCE_PATH}/GatewayReceiver.cpp)
list(APPEND SRC_FILES ${SOURCE_PATH}/Receiver.cpp)
set(INC_FILES ${HEADER_PATH}/GatewayReceiver.h)
list(APPEND INC_FILES ${HEADER_PATH}/Receiver.h)


target_sources(${proj_name} PRIVATE ${SRC_FILES} ${INC_FILES})

target_include_directories(${proj_name} PRIVATE ${HEADER_PATH})
target_include_directories(${proj_name} PRIVATE ${APTIV_GATEWAY_ROOT_PATH}/Common_Files)
target_include_directories(${proj_name} PRIVATE ${APTIV_GATEWAY_ROOT_PATH}/Gateway/inc) 
target_include_directories(${proj_name} PRIVATE ${APTIV_GATEWAY_ROOT_PATH}/pugixml)


target_link_libraries(${proj_name} PRIVATE ${VSOMEIP_LIBRARIES}   
					   ${CMAKE_THREAD_LIBS_INIT}  
					   someipGateway)

set_target_properties( ${proj_name}    PROPERTIES
									ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									)

if(WIN32)
	find_library(pugixml_debug pugixml_dbg HINTS "${CMAKE_CURRENT_LIST_DIR}/../../pugixml/bin/Windows")
	find_library(pugixml_release pugixml HINTS "${CMAKE_CURRENT_LIST_DIR}/../../pugixml/bin/Windows")
elseif(UNIX)
	find_library(pugixml_debug libpugixml_dbg.a HINTS "${CMAKE_CURRENT_LIST_DIR}/../../pugixml/bin/Linux")
	find_library(pugixml_release libpugixml.a HINTS "${CMAKE_CURRENT_LIST_DIR}/../../pugixml/bin/Linux")
endif(WIN32)
	
target_link_libraries(${proj_name} PRIVATE debug "${pugixml_debug}"
									 optimized "${pugixml_release}"
									 )
									 								 
if(UNIX)
	target_link_libraries (${proj_name} PRIVATE ${CMAKE_DL_LIBS}) #${CMAKE_DL_LIBS} for dlopen and dlclose lib for Linux build
endif(UNIX)

source_group(TREE ${APTIV_GATEWAY_ROOT_PATH} FILES ${SRC_FILES} ${INC_FILES})