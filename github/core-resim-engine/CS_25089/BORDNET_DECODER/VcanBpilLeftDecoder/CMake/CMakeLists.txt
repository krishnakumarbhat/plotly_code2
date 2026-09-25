##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(project_name  "VCAN_BPILLeft_Decoder")
project(${project_name})

set(VCAN_BPIL_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

##include custom specific

set(SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCAN_BPILLeftDecoderBase.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/IVCANBPILLeftDecoder.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/IVCANBPILLeftDecoderAPI.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeft_Decoder_A310.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeft_Decoder_A370.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeft_Decoder_A390.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VcanBpilLeftVersionFactoryBmwSrr5.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/vcanBpilLeftDecoderA430.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/vcanBpilLeftDecoderA480.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/vcanBpilLeftDecoderA430_SOP3.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeft_Decoder_A310.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeft_Decoder_A370.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeft_Decoder_A390.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/vcanBpilLeftDecoderA430.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/vcanBpilLeftDecoderA480.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/vcanBpilLeftDecoderA430_SOP3.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/VCANBPILLeftDecoderAPI.cpp")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/Version.h")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/Resource.rc")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/ReleaseNotes_VCANBPILLeft.txt")
list(APPEND SOURCES "${VCAN_BPIL_PROJECT_ROOT_PATH}/CommonHeaders.h")

set(target_name ${project_name})
add_library(${target_name} SHARED "")

target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCES})

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _WIN_PLATFORM) 
endif(MSVC)
