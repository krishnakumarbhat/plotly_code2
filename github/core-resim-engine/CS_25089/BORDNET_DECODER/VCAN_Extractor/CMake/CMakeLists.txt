##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(project_name  "VCAN_Extractor")
project(${project_name})

set(VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(VCAN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VCANLow_Decoder")
set(VCANECU_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VCANECU_Decoder")
set(VCANBPILLEFT_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VcanBpilLeftDecoder")
set(VCANBPILRIGHT_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../VcanBpilRightDecoder")

set(SOURCES "${VCAN_PROJECT_ROOT_PATH}/Include/vcanDecoderLow.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Source/vcanDecoderLow.cpp")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Include/vcanDecoderEcu.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Source/vcanDecoderEcu.cpp")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Include/vcanDecoderBpilLeft.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Source/vcanDecoderBpilLeft.cpp")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Include/vcanDecoderBpilRight.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Source/vcanDecoderBpilRight.cpp")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Include/vcanDecoderBpil.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/Source/vcanDecoderBpil.cpp")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Source/vcanBpilLeftBaseA310.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Source/vcanBpilLeftBaseA370.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Source/vcanBpilLeftBaseA390.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Source/vcanBpilLeftBaseA430.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Source/vcanBpilLeftBaseA480.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Source/vcanBpilLeftBaseA430_SOP3.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBase.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBaseA310.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBaseA370.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBaseA390.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBaseA430.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBaseA480.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include/vcanBpilLeftBaseA430_SOP3.h")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Source/vcanBpilRightBaseA310.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Source/vcanBpilRightBaseA370.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Source/vcanBpilRightBaseA390.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Source/vcanBpilRightBaseA430.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Source/vcanBpilRightBaseA480.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Source/vcanBpilRightBaseA430_SOP3.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBase.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBaseA310.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBaseA370.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBaseA390.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBaseA430.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBaseA480.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include/vcanBpilRightBaseA430_SOP3.h")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Source/vcanEcuBaseA310.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Include/vcanEcuBase.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Include/vcanEcuBaseA310.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Source/vcanEcuBaseA390.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Include/vcanEcuBaseA390.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Source/vcanEcuBaseA480.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Include/vcanEcuBaseA480.h")

list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Source/vcanLowBaseA310.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Source/vcanLowBaseA350.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Source/vcanLowBaseA370.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Source/vcanLowBaseA390.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Source/vcanLowBaseA480.cpp")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include/vcanLowBase.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include/vcanLowBaseA310.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include/vcanLowBaseA350.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include/vcanLowBaseA370.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include/vcanLowBaseA390.h")
list(APPEND SOURCES "${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include/vcanLowBaseA480.h")



source_group(TREE ${VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)



target_include_directories(${target_name} PUBLIC ${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_LEFT/Include)
target_include_directories(${target_name} PUBLIC ${VCAN_PROJECT_ROOT_PATH}/VCAN_BPIL_RIGHT/Include)
target_include_directories(${target_name} PUBLIC ${VCAN_PROJECT_ROOT_PATH}/VCAN_ECU/Include)
target_include_directories(${target_name} PUBLIC ${VCAN_PROJECT_ROOT_PATH}/VCAN_LOW/Include)


target_include_directories(${target_name} PUBLIC ${VCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${VCAN_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${VCANECU_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${VCANBPILLEFT_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${VCANBPILRIGHT_INTERFACEFILES})


target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

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