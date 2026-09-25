##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME  "SrrSm2Lm2YamlParser")
project(${PROJECTNAME})

#srrsm2xml parser files
set(SOURCE_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/source/srrSm2Lm2YamlParser.cpp)
list(APPEND SOURCE_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/source/srrSm2Lm2YamlParserModelList.cpp)

set(HEADER_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/include/srrSm2Lm2YamlParser.h)
list(APPEND HEADER_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/include/srrSm2Lm2YamlParserModelList.h)
list(APPEND HEADER_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/include/SenderList.h)
list(APPEND HEADER_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/include/ReceiverList.h)
list(APPEND HEADER_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/include/ModelListContainer.h)

add_library(${PROJECTNAME} STATIC "")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_LM2_YAML_PARSER_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Yaml/include/)


target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_LM2_YAML_PARSER_PATH}/../Utility/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_LM2_YAML_PARSER_PATH}/../SrrSm2XmlParser/include)

target_link_libraries(${PROJECTNAME} Utility)
IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)


