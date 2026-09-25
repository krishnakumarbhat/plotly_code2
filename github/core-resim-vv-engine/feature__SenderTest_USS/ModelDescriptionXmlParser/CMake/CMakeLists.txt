##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME  "ModelDescriptionXmlParser")
project(${PROJECTNAME})

set(SOURCE_FILES ${MODEL_DESCRIPTION_PARSER_PATH}/source/modelDescriptionXmlParser.cpp)
set(HEADER_FILES ${MODEL_DESCRIPTION_PARSER_PATH}/include/modelDescriptionXmlParser.h)

add_library(${PROJECTNAME} STATIC "")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECTNAME} PRIVATE ${MODEL_DESCRIPTION_PARSER_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${MODEL_DESCRIPTION_PARSER_PATH}/../Pugixml)
target_include_directories(${PROJECTNAME} PRIVATE ${MODEL_DESCRIPTION_PARSER_PATH}/../Utility/include)

IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)