##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "SCAN_Extractor")
project(${project_name})

link_directories(${OUTPUT_PATH})

set(SCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(SOURCE_PATH_SCAN "${SCAN_PROJECT_ROOT_PATH}/Source")
set(INCLUDE_PATH_SCAN "${SCAN_PROJECT_ROOT_PATH}/Include")
set(INCLUDE_INTERFACE_PATH_SCAN "${SCAN_PROJECT_ROOT_PATH}/../SCAN_Decoder")
set(SCAN_COMMON_STRUCTURES "${SCAN_PROJECT_ROOT_PATH}/../Common_Structures")


list(APPEND SOURCES "${SOURCE_PATH_SCAN}/scanBaseA310.cpp")
list(APPEND SOURCES "${SOURCE_PATH_SCAN}/scanBaseA480.cpp")
list(APPEND SOURCES "${SOURCE_PATH_SCAN}/scanDecoder.cpp")

list(APPEND SOURCES "${INCLUDE_PATH_SCAN}/scanBaseA310.h")
list(APPEND SOURCES "${INCLUDE_PATH_SCAN}/scanBaseA480.h")
list(APPEND SOURCES "${INCLUDE_PATH_SCAN}/scanDecoder.h")
list(APPEND SOURCES "${INCLUDE_PATH_SCAN}/scanBase.h")

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries(${target_name} CrossPlatform)

if(UNIX)
target_link_libraries (${target_name} ${CMAKE_DL_LIBS})
endif(UNIX)


#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

target_sources(${target_name} PRIVATE ${SOURCES})
target_include_directories(${target_name} PRIVATE ${SCAN_PROJECT_ROOT_PATH}/../Common_Headers)
target_include_directories(${target_name} PRIVATE ${INCLUDE_PATH_SCAN} )
target_include_directories(${target_name} PUBLIC ${INCLUDE_INTERFACE_PATH_SCAN})
target_include_directories(${target_name} PRIVATE ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${SCAN_COMMON_STRUCTURES})

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${project_name} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)

# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)


if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)