cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

enable_testing(C CXX)
set(CMAKE_CXX_STANDARD 14)
set(OSI_VERSION 3.5.0)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

message(STATUS "*********************|| SMSilEngineUnitTest ||****************************")

set(OUT_DIR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../output/${CONF_DIR}/")

set(PROJECT_NAME "SMSilEngineUnitTest")
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_FILE})

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# set Protobuf and OSI library versions
set(ProtoLibraryVersion "protobuf-3.6.1")
if(${OSI_VERSION} STREQUAL "3.5.0")
set(OSILibraryVersion "open-simulation-interface-3.5.0_w_Proto_3.6.1")
elseif(${OSI_VERSION} STREQUAL "3.2.0")
set(OSILibraryVersion "open-simulation-interface-3.2.0_w_Proto_3.6.1")
elseif(${OSI_VERSION} STREQUAL "3.1.2")
set(OSILibraryVersion "open-simulation-interface-3.1.2_w_Proto_3.6.1")
endif()



#set root folder path
set(SENSOR_MODEL_SIL_ENGINE_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")

#set module Lib path
#set source files
set(SOURCE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Source/")
set(FMI_UNZIP_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/FmiUnzip")
set(SRR_SM2_XML_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SrrSm2XmlParser")
set(MODEL_DESCRIPTION_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ModelDescriptionXmlParser")
set(UTILITY_FILES_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Utility")
set(OSI_FILE_READ_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/OsiFileRead")
set(OSI_FILE_WRITE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/OsiFileWrite")
set(SENSOR_FMU_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SensorFmu")
set(FMU_INTERFACE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/FmuInterface")
set(COMMON_INCLUDES_FILE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/CommonIncludeFiles")
set(SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SensorModelsilEngineConfigParser")
set(TIMING_PROFILE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/TimingProfile")
set(SRR_SM2_LM2_YAML_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SrrSm2Lm2YamlParser")
set(THIRD_PARTY_FILES_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty")
SET(UT_INCLUDE_DIR "${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/inc")
set(FMILIB_THIRDPARTYLIBS  "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty")
set(LIBS_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Libs")

#set include files
set(MDF_LOG_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/MdfLog")
set(MDF_INCLUDE_PATH "${THIRD_PARTY_FILES_PATH}/MDF_Include")
set(BOOST_INC "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/Include")
set(BOOST_INC "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS19/lib/windows")
set(BOOST_INC "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/Include/")


set(LIBS "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Libs/zeromq/include/zmq.hpp")

if(MSVC)
	target_compile_definitions(${PROJECT_NAME} PRIVATE _MBCS _WIN_PLATFORM) 
	target_compile_definitions(${PROJECT_NAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)
endif(MSVC)

#List of include files
target_include_directories(${PROJECT_NAME} PRIVATE ${OSI_FILE_WRITE_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${UTILITY_FILES_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${MODEL_DESCRIPTION_PARSER_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${FMI_UNZIP_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_FILES_PATH}/Minizip/minizip)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRR_SM2_LM2_YAML_PARSER_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${FMU_INTERFACE_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SENSOR_FMU_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${MDF_LOG_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${MDF_INCLUDE_PATH})

target_include_directories(${PROJECT_NAME} PRIVATE ${MDF_INCLUDE_PATH}/Interface)

target_include_directories(${PROJECT_NAME} PRIVATE ${TIMING_PROFILE_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${OSI_FILE_READ_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDES_FILE_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDES_FILE_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDES_FILE_PATH}/helper)


target_include_directories(${PROJECT_NAME} PRIVATE ${UT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Pugixml)
target_include_directories(${PROJECT_NAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Yaml/include)


target_include_directories(${PROJECT_NAME} PRIVATE ${SRR_SM2_XML_PARSER_PATH}/../Pugixml)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRR_SM2_LM2_YAML_PARSER_PATH}/../Utility/include)

target_include_directories(${PROJECT_NAME} PRIVATE ${OSI_FILE_READ_PATH}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS_PATH}/zeromq/include/)

#Third party include files
list(APPEND INCLUDE_FILES ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Pugixml)
list(APPEND INCLUDE_FILES ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Yaml/include/yaml-cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Yaml/include)

list(APPEND INCLUDE_FILES ${LIBS_PATH}/${OSI_VERSION}/include/osi3)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_PATH}/${OSILibraryVersion}/include/osi3)

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS_PATH}/zeromq/include/)

#list(APPEND INCLUDE_FILES ${LIBS_PATH}/${ProtoLibraryVersion}/include)

#List of source files

#Fmu_Interface
list(APPEND SOURCE_FILES ${FMU_INTERFACE_PATH}/source/Fmu_Interface.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/Fmu_interface_UT.cpp)

#modelDescriptionXmlParser
list(APPEND SOURCE_FILES ${MODEL_DESCRIPTION_PARSER_PATH}/source/modelDescriptionXmlParser.cpp)
list(APPEND SOURCE_FILES ${UTILITY_FILES_PATH}/source/Utility.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/modelDescriptionXmlParser_UT.cpp)

#SrrSm2XmlParser
list(APPEND SOURCE_FILES ${SRR_SM2_XML_PARSER_PATH}/source/SrrSm2XmlParser.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/srrSm2XmlParser_UT.cpp)

#srrSm2XmlParserModelList
list(APPEND SOURCE_FILES ${SRR_SM2_XML_PARSER_PATH}/source/srrSm2XmlParserModelList.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/srrSm2XmlParserModelList_UT.cpp)

#timingProfile
list(APPEND SOURCE_FILES ${TIMING_PROFILE_PATH}/source/timingProfile.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/timingProfile_UT.cpp)

#sensorModelSil
list(APPEND SOURCE_FILES ${SOURCE_PATH}/sensorModelSil.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/sensorModelSil_UT.cpp)

list(APPEND SOURCE_FILES ${SENSOR_FMU_PATH}/source/baseFmu.cpp)
list(APPEND SOURCE_FILES ${SENSOR_FMU_PATH}/source/logicalFmu.cpp)
list(APPEND SOURCE_FILES ${SENSOR_FMU_PATH}/source/sensorFmu.cpp)

#osiFileRead
list(APPEND SOURCE_FILES ${OSI_FILE_READ_PATH}/source/osiFileRead.cpp)
#list(APPEND SOURCE_FILES ${OSI_FILE_READ_PATH}/include/osiFileRead.h)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/osiFileRead_UT.cpp)
list(APPEND HEADER_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/inc/osiFileRead_UT.hpp)

#osiFileWrite
list(APPEND SOURCE_FILES ${OSI_FILE_WRITE_PATH}/source/osiFileWrite.cpp) 
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/osiFileWrite_UT.cpp)


#SrrSm2Lm2YamlParser
list(APPEND SOURCE_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/source/SrrSm2Lm2YamlParser.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/srrSm2Lm2YamlParser_UT.cpp)


#srrSm2Lm2YamlParserModelList
list(APPEND SOURCE_FILES ${SRR_SM2_LM2_YAML_PARSER_PATH}/source/srrSm2Lm2YamlParserModelList.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/srrSm2Lm2YamlParserModelList_UT.cpp)

list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/source/smSilEngineConfigParser.cpp)
list(APPEND SOURCE_FILES ${SENSOR_MODEL_SIL_ENGINE_UT_ROOT_PATH}/src/smSilEngineConfigParser_UT.cpp)


 
#set pugi, yaml Lib
set(LIB_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Pugixml/binaries/windows/PUGI_VS19")
set(YAML_DEBUG_LIB_PATH "${FMILIB_THIRDPARTYLIBS}/Yaml/binaries/Windows/Debug")
set(YAML_RELEASE_LIB_PATH "${FMILIB_THIRDPARTYLIBS}/Yaml/binaries/Windows/Release")
set(OSI_VERSION 3.5.0)
set(ProtoLibraryVersion "protobuf-3.6.1")
set(OSILibraryVersion "open-simulation-interface-3.5.0_w_Proto_3.6.1")

	

set(PROTOBUF_DEBUG_LIB_PATH "${LIBS_PATH}/${ProtoLibraryVersion}/Debug/lib")
set(PROTOBUF_RELEASE_LIB_PATH "${LIBS_PATH}/${ProtoLibraryVersion}/Release/lib")
set(OSI_DEBUG_LIB_PATH "${LIBS_PATH}/${OSILibraryVersion}/Debug/lib/osi3")
set(OSI_RELEASE_LIB_PATH "${LIBS_PATH}/${OSILibraryVersion}/Release/lib/osi3")

#find pugi, yaml lib
find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/Debug")
find_library(pugixml_release pugixml HINTS "${LIB_PATH}/Release")
find_library(YAML_DEBUG_LIB yaml-cppd PATHS ${YAML_DEBUG_LIB_PATH} NO_DEFAULT_PATH)
find_library(YAML_RELEASE_LIB yaml-cpp PATHS ${YAML_RELEASE_LIB_PATH} NO_DEFAULT_PATH)

find_library(PROTOBUF_DEBUG_LIB NAMES libprotobufd PATHS "${PROTOBUF_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
find_library(PROTOBUF_RELEASE_LIB NAMES libprotobuf PATHS "${PROTOBUF_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)

find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
find_library(OSI_PIC_DEBUG_LIB open_simulation_interface_pic PATHS "${OSI_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
find_library(OSI_PIC_RELEASE_LIB open_simulation_interface_pic PATHS "${OSI_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)


#link pugi, yaml lib
target_link_libraries(${PROJECT_NAME} debug "${pugixml_debug}" optimized "${pugixml_release}") 
target_link_libraries(${PROJECT_NAME} debug "${YAML_DEBUG_LIB}" optimized "${YAML_RELEASE_LIB}") 
target_link_libraries(${PROJECT_NAME} debug ${PROTOBUF_DEBUG_LIB} optimized ${PROTOBUF_RELEASE_LIB})
target_link_libraries(${PROJECT_NAME} debug ${OSI_STATIC_DEBUG_LIB} optimized ${OSI_STATIC_RELEASE_LIB})
target_link_libraries(${PROJECT_NAME} debug ${OSI_PIC_DEBUG_LIB} optimized ${OSI_PIC_RELEASE_LIB})


#osi
set(OSI_DEBUG_LIB_PATH "${LIBS_PATH}/${OSILibraryVersion}/Debug/lib/osi3")
set(OSI_RELEASE_LIB_PATH "${LIBS_PATH}/${OSILibraryVersion}/Release/lib/osi3")
find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
target_link_libraries(${PROJECT_NAME} debug ${OSI_STATIC_DEBUG_LIB} optimized ${OSI_STATIC_RELEASE_LIB})

#zeromq
set(ZEROMQ_LIB_PATH "${LIBS_PATH}/zeromq/bin")
find_library(ZEROMQ_LIB NAMES libzmq-mt-4_3_1 PATHS "${ZEROMQ_LIB_PATH}" NO_DEFAULT_PATH)
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LIB})

#target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
#target_include_directories(${PROJECT_NAME} PRIVATE ${OSI_FILE_READ_PATH}/include)
#target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS_PATH}/zeromq/include/)


#set(ZEROMQ_LIB_PATH "${LIBS_PATH}/zeromq/bin")
#find_library(ZEROMQ_LIB NAMES libzmq-mt-4_3_1 PATHS "${ZEROMQ_LIB_PATH}" NO_DEFAULT_PATH)
#target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LIB}) 

target_link_libraries(${PROJECT_NAME} "${yaml_lib}")
#UT Modules

# Online Gtest from Github
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_include_directories(${PROJECT_NAME} PRIVATE  ${INCLUDE_FILES} ${BOOST_INC})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}  gtest_main gmock_main )

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:NDEBUG>")


if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(BOOST_LIB_DIR_DEBUG "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS15/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS15/lib/windows/${CONF_DIR}/Release")
	elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		message("inside the ***********Visual Studio 16 2019*********")
		set(BOOST_LIB_DIR_DEBUG "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS19/lib/windows/x64/Debug")
		set(BOOST_LIB_DIR_RELEASE "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS19/lib/windows/${CONF_DIR}/Release")


		find_library(BOOST_LIB_DEBUG libboost_system-vc141-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_TIME libboost_date_time-vc141-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_REGEX libboost_regex-vc1429-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_CHRONO libboost_chrono-vc1429-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_THREAD libboost_thread-vc1429-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_SYSTEM_TWO libboost_system-vc1429-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_TIME_TWO libboost_date_time-vc1429-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_REGEX_TWO libboost_regex-vc141-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_THREAD_TWO libboost_thread-vc141-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")
		find_library(BOOST_LIB_DEBUG_CHRONO_TWO libboost_chrono-vc141-mt-gd-1_65_1 HINTS "${BOOST_LIB_DIR_DEBUG}")

		
		target_link_libraries(${PROJECT_NAME} debug "${BOOST_LIB_DEBUG}" 
													"${BOOST_LIB_DEBUG_TIME}"
													"${BOOST_LIB_DEBUG_REGEX}"
													"${BOOST_LIB_DEBUG_CHRONO}"
													"${BOOST_LIB_DEBUG_THREAD}"
													"${BOOST_LIB_DEBUG_SYSTEM_TWO}"
													"${BOOST_LIB_DEBUG_TIME_TWO}"
													"${BOOST_LIB_DEBUG_REGEX_TWO}"
													"${BOOST_LIB_DEBUG_THREAD_TWO}"
													"${BOOST_LIB_DEBUG_CHRONO_TWO}"
													) 

	else()
		set(BOOST_LIB_DIR_DEBUG "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/lib/windows/${CONF_DIR}/Release")
	endif()
else()
	set(BOOST_LIB_DIR_UNIX_DEBUG "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/lib/linux/${CONF_DIR}/Debug")
	set(BOOST_LIB_DIR_UNIX_RELEASE "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/lib/linux/${CONF_DIR}/Release")
endif()



string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TO_UPPER)
message("Config : ${CMAKE_BUILD_TYPE_TO_UPPER}")


IF(OUT_DIR_LIB)
		set_target_properties( ${PROJECT_NAME} PROPERTIES
							RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
							ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
							LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
			)
ENDIF(OUT_DIR_LIB)