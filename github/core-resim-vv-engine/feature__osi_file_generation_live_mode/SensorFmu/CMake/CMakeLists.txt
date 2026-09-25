##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME  "SensorFmu")
project(${PROJECTNAME})

set(SOURCE_FILES ${SENSOR_FMU_PATH}/source/sensorFmu.cpp)
list(APPEND SOURCE_FILES ${SENSOR_FMU_PATH}/source/baseFmu.cpp)
list(APPEND SOURCE_FILES ${SENSOR_FMU_PATH}/source/logicalFmu.cpp)
set(HEADER_FILES ${SENSOR_FMU_PATH}/include/sensorFmu.h)
list(APPEND HEADER_FILES ${SENSOR_FMU_PATH}/include/baseFmu.h)
list(APPEND HEADER_FILES ${SENSOR_FMU_PATH}/include/logicalFmu.h)

add_library(${PROJECTNAME} STATIC "")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_include_directories(${PROJECTNAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/../Utility/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/../ModelDescriptionXmlParser/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/include/fmuInterface)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${SENSOR_FMU_PATH}/../Pugixml)
target_include_directories(${PROJECTNAME} PRIVATE ${FMU_INTERFACE_PATH}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${TIMING_PROFILE_PATH}/include)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECTNAME} PRIVATE Fmu_Interface)
target_link_libraries(${PROJECTNAME} PRIVATE Common_Includes_Interface)

IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)
