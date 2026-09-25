##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "VCANMAN_LEFT_Decoder")
project(${PROJECTNAME})


set(VCAN_MAN_LEFT_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/VCAN_MAN_L_Decoder.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/VCAN_MAN_L_Common.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/IVCANMANLDecoder.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/VCANMAN_L_DecoderBase.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/IVCANMANLDecoderAPI.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/ReleaseNotes_VCAN_MAN_L.txt")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/VCAN_MAN_L_Decoder.cpp")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/VCANMAN_L_DecoderAPI.cpp")
list(APPEND SOURCE_ALL "${VCAN_MAN_LEFT_DECODER_ROOT_PATH}/DGPS_CAN_Msg_Types.h")



set(target_name ${PROJECTNAME})
add_library(${target_name} SHARED "")


target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCE_ALL})

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}")												 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)
