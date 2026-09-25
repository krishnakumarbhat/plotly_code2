cmake_minimum_required (VERSION 2.8.12)
set(PROJECTNAME testReceiver)

project (${PROJECTNAME})

set(target_name ${PROJECTNAME})

add_executable(${target_name} ${CMAKE_CURRENT_LIST_DIR}/../src/receiver.cpp)

target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Gateway/inc) 
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../pugixml)

target_link_libraries(${target_name} PRIVATE ${VSOMEIP_LIBRARIES} 
									  ${CMAKE_THREAD_LIBS_INIT}
									  someipGateway)

set_target_properties( ${target_name}    PROPERTIES
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

target_link_libraries(${target_name} PRIVATE debug "${pugixml_debug}"
									 optimized "${pugixml_release}"
									 )
if(UNIX)
	target_link_libraries (${target_name} PRIVATE ${CMAKE_DL_LIBS}) #${CMAKE_DL_LIBS} for dlopen and dlclose lib for Linux build
endif(UNIX)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/.. FILES ${CMAKE_CURRENT_LIST_DIR}/../src/receiver.cpp)