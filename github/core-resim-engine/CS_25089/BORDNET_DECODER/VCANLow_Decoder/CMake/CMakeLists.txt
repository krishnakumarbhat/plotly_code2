##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "VCANLow_Decoder")
project(${PROJECTNAME})


set(VCAN_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A310.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLowCommon.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/IVCANLowDecoder.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A370.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A350.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A390.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A480.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VcanLowDecoderBase.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/IVCANLowDecoderAPI.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/ReleaseNotes_VCAN.txt")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A310.cpp")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A370.cpp")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A350.cpp")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A390.cpp")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLow_Decoder_A480.cpp")
list(APPEND SOURCE_ALL "${VCAN_DECODER_ROOT_PATH}/VCANLowDecoderAPI.cpp")



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
