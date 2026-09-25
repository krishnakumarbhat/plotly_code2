##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "VCAN_MAN_Extractor")
project(${project_name})

set(VCAN_MAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(VCAN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VCANMAN_FC_Decoder")
set(VCAN_MAN_LEFT_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VCANMAN_LEFT_Decoder")
set(VCAN_MAN_RIGHT_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VCANMAN_RIGHT_Decoder")

set(SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/Include/vcanDecoderManFC.h")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/Source/vcanDecoderManFC.cpp")

list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/Include/vcanDecoderManLeft.h")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/Source/vcanDecoderManLeft.cpp")

list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/Include/vcanDecoderManRight.h")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/Source/vcanDecoderManRight.cpp")

list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_FC/Source/vcanManBaseFC.cpp")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_FC/Include/vcanManBase.h")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_FC/Include/vcanMANBaseFC.h")

list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_LEFT/Source/vcanManBase_Left.cpp")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_LEFT/Include/vcanManBase_L.h")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_LEFT/Include/vcanMANBase_Left.h")

list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_RIGHT/Source/vcanManBase_Right.cpp")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_RIGHT/Include/vcanManBase_R.h")
list(APPEND SOURCES "${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_RIGHT/Include/vcanMANBase_Right.h")

source_group(TREE ${VCAN_MAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_FC/Include)
target_include_directories(${target_name} PUBLIC ${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_LEFT/Include)
target_include_directories(${target_name} PUBLIC ${VCAN_MAN_PROJECT_ROOT_PATH}/VCAN_MAN_RIGHT/Include)


target_include_directories(${target_name} PUBLIC ${VCAN_MAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${VCAN_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${VCAN_MAN_LEFT_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${VCAN_MAN_RIGHT_INTERFACEFILES})


target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${project_name} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)


if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)