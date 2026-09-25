##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "SmSilEngineConfigParser")
project(${PROJECTNAME})

set(SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/source/smSilEngineConfigParser.cpp)
set(HEADER_FILES ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/include/smSilEngineConfigParser.h)
list(APPEND HEADER_FILES ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/include/configuration.h)

add_library(${PROJECTNAME} STATIC "")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/../Pugixml)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Yaml/include/)
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_LM2_YAML_PARSER_PATH}/../Utility/include)

IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)