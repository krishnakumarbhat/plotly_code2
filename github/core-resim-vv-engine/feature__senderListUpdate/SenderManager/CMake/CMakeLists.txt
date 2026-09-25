##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME  "SenderManager")
project(${PROJECTNAME})

set(SOURCE_FILES ${SENDER_MANAGER_PATH}/source/ZmqSenderManager.cpp)
set(HEADER_FILES ${SENDER_MANAGER_PATH}/include/ZmqSenderManager.h)

add_library(${PROJECTNAME} STATIC "")

set(LIBS_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Libs")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECTNAME} PRIVATE ${SENDER_MANAGER_PATH}/include)
target_include_directories(${PROJECTNAME} PUBLIC ${LIBS_PATH}/zeromq/include/)

if(MSVC)
set(ZEROMQ_LIB_PATH "${LIBS_PATH}/zeromq/bin")
find_library(ZEROMQ_LIB NAMES libzmq-mt-4_3_1 PATHS "${ZEROMQ_LIB_PATH}" NO_DEFAULT_PATH)
target_link_libraries(${PROJECTNAME} ${ZEROMQ_LIB}) 
else()
set(ZEROMQ_LIB_PATH "${LIBS_PATH}/zeromq/Linux")
find_library(ZEROMQ_LIB NAME libzmq.so.5 PATHS "${ZEROMQ_LIB_PATH}" NO_DEFAULT_PATH)
target_link_libraries(${PROJECTNAME} ${ZEROMQ_LIB})
endif()


IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)