##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "Fmu_Interface")
project(${PROJECTNAME})

set(HEADER_FILES ${FMU_INTERFACE_PATH}/include/Fmu_Interface.h)
set(SOURCE_FILES ${FMU_INTERFACE_PATH}/source/Fmu_Interface.cpp)


add_library(${PROJECTNAME} STATIC "")

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_include_directories(${PROJECTNAME} PRIVATE ${BOOST_INC})
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/../ModelDescriptionXmlParser/include)
target_include_directories(${PROJECTNAME} PRIVATE ${FMU_INTERFACE_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/../Pugixml)
target_include_directories(${PROJECTNAME} PRIVATE ${TIMING_PROFILE_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/../Utility/include)

IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)
