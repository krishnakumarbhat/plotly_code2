##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "NISSAN_FR_Decoder")
project(${PROJECTNAME})


set(NISSAN_FR_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/NISSAN_FR_Decoder.h")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/NISSAN_FR_Common.h")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/INISSANFRDecoder.h")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/NISSANDecoderFRBase.h")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/INISSANFRDecoderAPI.h")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/ReleaseNotes_NISSAN_FR.txt")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/NISSAN_FR_Decoder.cpp")
list(APPEND SOURCE_ALL "${NISSAN_FR_DECODER_ROOT_PATH}/NISSANFR_DecoderAPI.cpp")



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
