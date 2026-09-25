##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "VCANMAN_RIGHT_Decoder")
project(${PROJECTNAME})


set(VCAN_MAN_RIGHT_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/VCAN_MAN_R_Decoder.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/VCAN_MAN_R_Common.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/IVCANMANRDecoder.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/VCANMAN_R_DecoderBase.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/IVCANMANRDecoderAPI.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/ReleaseNotes_VCAN_MAN_R.txt")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/VCAN_MAN_R_Decoder.cpp")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/VCANMAN_R_DecoderAPI.cpp")
list(APPEND SOURCE_ALL "${VCAN_MAN_RIGHT_DECODER_ROOT_PATH}/DGPS_CAN_Msg_Types.h")



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
