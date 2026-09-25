#set minimum cmake version required
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 14)
set(OSI_VERSION 3.5.0)

include(CheckTypeSize)
check_type_size("void*"  SIZEOF_DATA_PTR)

if(${SIZEOF_DATA_PTR} EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
	message("Building for 64 bit.....")
elseif(${SIZEOF_DATA_PTR} EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x86")
	message("Building for 32 bit.....")
endif() 

set(OUT_DIR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../output/${CONF_DIR}/")

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

#Set the structure alignment
if(MSVC)
	add_compile_options("/Zp2")
endif(MSVC)

#set project name
set(proj_name "SensorModelSilEngine")

#set project
project (${proj_name})

#create executable
add_executable(${proj_name} "")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Set max stack size to 2000000
if(MSVC)
set(CMAKE_CXX_STACK_SIZE ${CMAKE_CXX_STACK_SIZE} 2000000)
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
#set root path
set(SENSOR_MODEL_SIL_ENGINE_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")

if(MSVC)
	set(OS_FOLDER_TYPE "Windows")
else()
	set(OS_FOLDER_TYPE "Linux")
	set(CMAKE_CXX_FLAGS "-fPIC")
endif()

if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	set(BUILD_FOLDER_TYPE "Release")
	set(protolibname "libprotobuf.a")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	set(BUILD_FOLDER_TYPE "Debug")
	set(protolibname "libprotobufd.a")
endif()

#set module and Libs path
set(SOURCE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Source/")
set(FILE_PARSER_WRITER_PATH "${SOURCE_PATH}/file_Parser_Writer/")
set(LIBS_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Libs")
set(FMILIB_THIRDPARTYLIBS  "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty")
set(FMI_UNZIP_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/FmiUnzip")
set(SRR_SM2_XML_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SrrSm2XmlParser")
set(MODEL_DESCRIPTION_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ModelDescriptionXmlParser")
set(UTILITY_FILES_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Utility")
set(OSI_FILE_READ_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/OsiFileRead")
set(OSI_FILE_WRITE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/OsiFileWrite")
set(SENDER_MANAGER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SenderManager")
set(SENSOR_FMU_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SensorFmu")
set(FMU_INTERFACE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/FmuInterface")
set(COMMON_INCLUDES_FILE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/CommonIncludeFiles")
#set(DSPACE_MF4_FILE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/DspaceMf4")
set(MDF_LOG_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/MdfLog")
set(MDF_INCLUDE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/MDF_Include")
set(BOOST_INC "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Boost/Include")
set(TIMING_PROFILE_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/TimingProfile")
set(SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SensorModelsilEngineConfigParser")
set(SRR_SM2_LM2_YAML_PARSER_PATH "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SrrSm2Lm2YamlParser")


#link Header files directory
target_link_libraries(${proj_name} Mdf_Include)
target_link_libraries(${proj_name} Common_Includes_Interface)


if(MSVC)
	target_compile_definitions(${proj_name} PRIVATE _MBCS _WIN_PLATFORM) 
	target_compile_definitions(${proj_name} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)
endif(MSVC)

target_include_directories(${proj_name} PRIVATE ${FILE_PARSER_WRITER_PATH}/Include)
target_include_directories(${proj_name} PRIVATE ${OSI_FILE_READ_PATH}/include)
target_include_directories(${proj_name} PRIVATE ${OSI_FILE_WRITE_PATH}/include)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Utility/include)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/FmiUnzip)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Minizip/minizip)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SrrSm2XmlParser/include)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/SrrSm2Lm2YamlParser/include)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ModelDescriptionXmlParser/include)
target_include_directories(${proj_name} PRIVATE ${FMU_INTERFACE_PATH}/include)
target_include_directories(${proj_name} PRIVATE ${SENSOR_FMU_PATH}/include)
#target_include_directories(${proj_name} PRIVATE ${DSPACE_MF4_FILE_PATH}/DspaceMf4Structure/include/DspaceInputStructures)
#target_include_directories(${proj_name} PRIVATE ${DSPACE_MF4_FILE_PATH}/DspaceMf4Structure/include)
#target_include_directories(${proj_name} PRIVATE ${DSPACE_MF4_FILE_PATH}/DspaceMf4Writer/include)
#target_include_directories(${proj_name} PRIVATE ${DSPACE_MF4_FILE_PATH}/MudpFrameCreator/include)
#target_include_directories(${proj_name} PRIVATE ${DSPACE_MF4_FILE_PATH}/MudpSerializer/include)
target_include_directories(${proj_name} PRIVATE ${MDF_LOG_PATH}/include)
target_include_directories(${proj_name} PRIVATE ${TIMING_PROFILE_PATH}/include)
target_include_directories(${proj_name} PRIVATE ${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/include)
target_include_directories(${proj_name} PRIVATE ${COMMON_INCLUDES_FILE_PATH})

#include header files for third party libraries
target_include_directories(${proj_name} PRIVATE  ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/Pugixml)
target_include_directories(${proj_name} PRIVATE  ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/Yaml/include/)
target_include_directories(${proj_name} PRIVATE ${LIBS_PATH}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include)
target_include_directories(${proj_name} PRIVATE ${LIBS_PATH}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/include/osi3)

# include source files
set(SOURCE_FILES ${SOURCE_PATH}/sensorModelSilEngine.cpp)
list(APPEND SOURCE_FILES ${SOURCE_PATH}/sensorModelSil.cpp)
list(APPEND SOURCE_FILES ${COMMON_INCLUDES_FILE_PATH}/helper/DataRoute.cpp)
set(HEADER_FILES ${SOURCE_PATH}/sensorModelSilEngine.h)
list(APPEND HEADER_FILES ${SOURCE_PATH}/sensorModelSil.h)
list(APPEND HEADER_FILES ${COMMON_INCLUDES_FILE_PATH}/include/DataRoute.h)


target_sources(${proj_name} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

source_group(TREE ${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH} FILES ${SRC_FILES})

target_compile_definitions(${proj_name} PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")
target_compile_definitions(${proj_name} PRIVATE "$<$<CONFIG:Release>:NDEBUG>")

add_subdirectory("${FMILIB_THIRDPARTYLIBS}/Zlib/zlib-1.2.6" 			"${CMAKE_CURRENT_BINARY_DIR}/zlib")
add_subdirectory("${FMILIB_THIRDPARTYLIBS}/Minizip/minizip/CMake" 		"${CMAKE_CURRENT_BINARY_DIR}/THIRDPARTYLIBS/Minizip")
add_subdirectory("${FMI_UNZIP_PATH}/CMake" 								"${CMAKE_CURRENT_BINARY_DIR}/FmiUnzip")
add_subdirectory("${UTILITY_FILES_PATH}/CMake" 							"${CMAKE_CURRENT_BINARY_DIR}/Utility")
add_subdirectory("${SRR_SM2_XML_PARSER_PATH}/CMake" 					"${CMAKE_CURRENT_BINARY_DIR}/SrrSm2XmlParser")
add_subdirectory("${OSI_FILE_READ_PATH}/CMake" 							"${CMAKE_CURRENT_BINARY_DIR}/OsiFileRead")
add_subdirectory("${OSI_FILE_WRITE_PATH}/CMake" 						"${CMAKE_CURRENT_BINARY_DIR}/OsiFileWrite")
add_subdirectory("${MODEL_DESCRIPTION_PARSER_PATH}/CMake" 				"${CMAKE_CURRENT_BINARY_DIR}/ModelDescriptionXmlParser") 
add_subdirectory("${SENDER_MANAGER_PATH}/CMake" 						"${CMAKE_CURRENT_BINARY_DIR}/SenderManager") 
add_subdirectory("${SENSOR_FMU_PATH}/CMake" 							"${CMAKE_CURRENT_BINARY_DIR}/SensorFmu") 
add_subdirectory("${FMU_INTERFACE_PATH}/CMake" 							"${CMAKE_CURRENT_BINARY_DIR}/Fmu_Interface") 
add_subdirectory("${COMMON_INCLUDES_FILE_PATH}/CMake" 					"${CMAKE_CURRENT_BINARY_DIR}/Common_Includes_File_PATH") 
#add_subdirectory("${DSPACE_MF4_FILE_PATH}/CMake" 						"${CMAKE_CURRENT_BINARY_DIR}/DspaceMf4") 
add_subdirectory("${MDF_INCLUDE_PATH}/CMake" 							"${CMAKE_CURRENT_BINARY_DIR}/Mdf_Include") 
add_subdirectory("${MDF_LOG_PATH}/CMake" 								"${CMAKE_CURRENT_BINARY_DIR}/MdfLog") 
add_subdirectory("${TIMING_PROFILE_PATH}/CMake" 						"${CMAKE_CURRENT_BINARY_DIR}/TimingProfile") 
add_subdirectory("${SENSOR_MODEL_SIL_ENGINE_CONFIG_PARSER_PATH}/CMake" 	"${CMAKE_CURRENT_BINARY_DIR}/SmSilEngineConfigParser") 
add_subdirectory("${SRR_SM2_LM2_YAML_PARSER_PATH}/CMake"				"${CMAKE_CURRENT_BINARY_DIR}/SrrSm2Lm2YamlParser")

target_link_libraries(${proj_name} FmuUnzip)
target_link_libraries(${proj_name} Utility)
target_link_libraries(${proj_name} SrrSm2XmlParser)
target_link_libraries(${proj_name} SenderManager)
target_link_libraries(${proj_name} SensorFmu)
target_link_libraries(${proj_name} ModelDescriptionXmlParser)
target_link_libraries(${proj_name} Fmu_Interface)
target_link_libraries(${proj_name} OsiFileRead)
target_link_libraries(${proj_name} OsiFileWrite)
target_link_libraries(${proj_name} MdfLog)
target_link_libraries(${proj_name} TimingProfile)
target_link_libraries(${proj_name} SmSilEngineConfigParser)
target_link_libraries(${proj_name} SrrSm2Lm2YamlParser)

if(MSVC)

	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
	
		set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Pugixml/binaries/windows/PUGI_VS15")
	elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Pugixml/binaries/windows/PUGI_VS19")
	else()
		set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Pugixml/binaries/windows")
	endif()

	# set protobuf and OSI libraries
	set(PROTOBUF_DEBUG_LIB_PATH "${LIBS_PATH}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/Debug/lib")
	set(PROTOBUF_RELEASE_LIB_PATH "${LIBS_PATH}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/Release/lib")
	set(OSI_DEBUG_LIB_PATH "${LIBS_PATH}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/Debug/lib/osi3")
	set(OSI_RELEASE_LIB_PATH "${LIBS_PATH}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/Release/lib/osi3")
	set(YAML_DEBUG_LIB_PATH "${FMILIB_THIRDPARTYLIBS}/Yaml/binaries/Windows/Debug")
	set(YAML_RELEASE_LIB_PATH "${FMILIB_THIRDPARTYLIBS}/Yaml/binaries/Windows/Release")

	#find pugiXMl libraries
	find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/Debug/")
	find_library(pugixml_release pugixml HINTS "${LIB_PATH}/Release/")
	
	#find osi and protobuf libraries
	find_library(PROTOBUF_DEBUG_LIB NAMES libprotobufd PATHS "${PROTOBUF_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(PROTOBUF_RELEASE_LIB NAMES libprotobuf PATHS "${PROTOBUF_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(YAML_DEBUG_LIB yaml-cppd PATHS ${YAML_DEBUG_LIB_PATH} NO_DEFAULT_PATH)
	find_library(YAML_RELEASE_LIB yaml-cpp PATHS ${YAML_RELEASE_LIB_PATH} NO_DEFAULT_PATH)

	#link Libraries
	target_link_libraries(${proj_name} debug "${pugixml_debug}" optimized "${pugixml_release}") 
	target_link_libraries(${proj_name} debug "${YAML_DEBUG_LIB}" optimized "${YAML_RELEASE_LIB}") 
	target_link_libraries(${proj_name} debug ${PROTOBUF_DEBUG_LIB} optimized ${PROTOBUF_RELEASE_LIB})
	target_link_libraries(${proj_name} debug ${OSI_STATIC_DEBUG_LIB} optimized ${OSI_STATIC_RELEASE_LIB})

else()			
	  set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Pugixml/binaries/linux")
	  set(YAML_LIB_PATH "${FMILIB_THIRDPARTYLIBS}/Yaml/binaries/Linux")
      find_library(pugixml_debug libpugixml.a HINTS "${LIB_PATH}/Debug/")
      find_library(pugixml_release libpugixml.a HINTS "${LIB_PATH}/Release/")
 get_filename_component(ProtobufStaticLib "${LIBS_PATH}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/${protolibname}" ABSOLUTE)
get_filename_component(OpenSimulationInterfaceStaticLib "${LIBS_PATH}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/osi3/libopen_simulation_interface_static.a" ABSOLUTE)
	  find_library(yaml_lib libyaml-cpp.a HINTS "${YAML_LIB_PATH}/Debug/")
      target_link_libraries(${proj_name} ${OpenSimulationInterfaceStaticLib}  ${ProtobufStaticLib} -lpthread -static-libgcc -static-libstdc++)
      target_link_libraries(${proj_name} debug "${pugixml_debug}"      optimized "${pugixml_release}")
	  target_link_libraries(${proj_name} "${yaml_lib}")
	  
	  set_target_properties(${proj_name} PROPERTIES LINK_FLAGS "-Bsymbolic")
endif()	

if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(BOOST_LIB_DIR_DEBUG "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS15/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS15/lib/windows/${CONF_DIR}/Release")
	elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		set(BOOST_LIB_DIR_DEBUG "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS19/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${SENSOR_MODEL_SIL_ENGINE_ROOT_PATH}/ThirdParty/BoostVS19/lib/windows/${CONF_DIR}/Release")
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

if(MSVC)
	if(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
		if(CONF_DIR STREQUAL "x64")	
			if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
				target_link_libraries(${proj_name}    	${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc140-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc140-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc140-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc140-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_system-vc140-mt-gd-1_65_1.lib)
			elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
				target_link_libraries(${proj_name}    	${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc1429-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc1429-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc1429-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc1429-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_system-vc1429-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_system-vc141-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc141-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc141-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc141-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc141-mt-gd-1_65_1.lib)
			else()
				target_link_libraries(${proj_name}  	${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_system-vc110-mt-gd-1_65_1.lib)
			endif()
		elseif(CONF_DIR STREQUAL "x86")				
				target_link_libraries(${proj_name} 	 	${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc110-mt-gd-1_65_1.lib
														${BOOST_LIB_DIR_DEBUG}/libboost_system-vc110-mt-gd-1_65_1.lib)
	
		endif(CONF_DIR STREQUAL "x64")
	elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE")
		if(CONF_DIR STREQUAL "x64")
			if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
				target_link_libraries(${proj_name}				${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc140-mt-1_65_1.lib)
			elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
				target_link_libraries(${proj_name}    			${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc141-mt-1_65_1.lib)
			else()
				target_link_libraries(${proj_name}				${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc110-mt-1_65_1.lib)	
																  									    
			endif()
		elseif(CONF_DIR STREQUAL "x86")
		
				target_link_libraries(${proj_name}			    ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc110-mt-1_65_1.lib
																)
			
		endif(CONF_DIR STREQUAL "x64")
	
endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")

else(MSVC)
	target_link_libraries(${proj_name} ${CMAKE_DL_LIBS})
	if(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")

	   if(CONF_DIR STREQUAL "x64")
			target_link_libraries(${proj_name}  	${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_chrono-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_date_time-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_regex-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_system-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_thread-gcc-mt-d-1_65_1.a)
	   elseif(CONF_DIR STREQUAL "x86")
			target_link_libraries(${proj_name} 	${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_chrono-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_date_time-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_regex-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_system-gcc-mt-d-1_65_1.a
													${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_thread-gcc-mt-d-1_65_1.a)
	   endif(CONF_DIR STREQUAL "x64")
	
	elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE")
	   if(CONF_DIR STREQUAL "x64")
			target_link_libraries(${proj_name} 		${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_chrono-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_date_time-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_regex-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_system-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_thread-gcc-mt-1_65_1.a)
	   elseif(CONF_DIR STREQUAL "x86")
			target_link_libraries(${proj_name} 		${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_chrono-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_date_time-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_regex-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_system-gcc-mt-1_65_1.a
													${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_thread-gcc-mt-1_65_1.a)
	   endif(CONF_DIR STREQUAL "x64")
	endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
endif(MSVC)

IF(OUT_DIR_LIB)
		set_target_properties( ${proj_name} PROPERTIES
							RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
							ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
							LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
			)
ENDIF(OUT_DIR_LIB)

#Set the startup project for Visual Studio solution
if(MSVC)
	set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "SensorModelSilEngine" )
	set_target_properties(${proj_name} PROPERTIES OUTPUT_NAME "SensorModelSilEngine")#Output .exe name ,it's exsiting in debug and release folder.
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${proj_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
	target_compile_options(${proj_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
    set_target_properties(${proj_name} PROPERTIES OUTPUT_NAME "SensorModelSilEngine")#Output .exe name ,it's exsiting in debug and release folder.
endif()

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${proj_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
 set_target_properties(${proj_name} PROPERTIES OUTPUT_NAME "SensorModelSilEngine")#Output .exe name ,it's exsiting in debug and release folder.
endif(UNIX)

