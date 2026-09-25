##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "SrrSm2XmlParser")
project(${PROJECTNAME})

#srrsm2xml parser files
set(SOURCE_FILES ${SRR_SM2_XML_PARSER_PATH}/source/srrSm2XmlParser.cpp)
list(APPEND SOURCE_FILES ${SRR_SM2_XML_PARSER_PATH}/source/srrSm2XmlParserModelList.cpp)

set(HEADER_FILES ${SRR_SM2_XML_PARSER_PATH}/include/srrSm2XmlParser.h)
list(APPEND HEADER_FILES ${SRR_SM2_XML_PARSER_PATH}/include/srrSm2XmlParserModelList.h)
list(APPEND HEADER_FILES ${SRR_SM2_XML_PARSER_PATH}/include/sensorSimulation.h)

add_library(${PROJECTNAME} STATIC "")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/../Pugixml)
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/../Utility/include)

target_link_libraries(${PROJECTNAME} Utility)
IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)


