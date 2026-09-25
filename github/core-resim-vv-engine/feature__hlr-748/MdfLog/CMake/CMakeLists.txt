##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "MdfLog")
project(${PROJECTNAME})

set(SOURCE_FILES ${MDF_LOG_PATH}/source/apt_mdf_log.cpp)

set(HEADER_FILES ${MDF_LOG_PATH}/include/apt_mdf_log.h)
list(APPEND HEADER_FILES ${MDF_LOG_PATH}/include/vector_mdf_licence.h)

add_library(${PROJECTNAME} STATIC "")

if(MSVC)
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
	target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)
endif(MSVC)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
source_group(TREE ${MDF_LOG_PATH} FILES ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECTNAME} PRIVATE Common_Includes_Interface)
target_link_libraries(${PROJECTNAME} PRIVATE Mdf_Include)
IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)