cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

if( NOT INTERFACE_BUILD_TYPE )
	set(INTERFACE_BUILD_TYPE "SHARED")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TO_UPPER)

set(PROJECTNAME  "Interface_Main")
project(${PROJECTNAME})
# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/../../Code")

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Deliverables")

set(Interface_Main_PROJ "${SOURCE_PROJECT}/")
set(UDP_Logging_PROJ "${SOURCE_PROJECT}/libs/UDP_Logging")
set(MDF_Writer_Library_PROJ "${SOURCE_PROJECT}/libs/MDF_Writer_Library")
set(pugixml_PROJ "${SOURCE_PROJECT}/libs/pugixml-1.10")
# set Protobuf and OSI library versions

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

set(ProtoLibraryVersion "protobuf-3.6.1")
set(OSILibraryVersion "open-simulation-interface-3.5.0_w_Proto_3.6.1")
set(LIBS_DIR "${SOURCE_PROJECT}/libs")
get_filename_component(ProtobufStaticLib "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/${protolibname}" ABSOLUTE)
get_filename_component(OpenSimulationInterfaceStaticLib "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/osi3/libopen_simulation_interface_static.a" ABSOLUTE)
set(ZLIB_LIBRARY "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/")
set(ZLIB_INCLUDE_DIR "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include")


add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif()

set(SOURCES "${Interface_Main_PROJ}")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/LM2_Interface.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/LM2_Interface.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Interface_Enums.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Interface_Input_Struct.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Generic_Interface.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Generic_Interface.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/XML_Reader.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/XML_Reader.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/some_ip_output.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/some_ip_input_RX.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/some_ip_output.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Rte_Type_SomeIP_RX.h")
list(APPEND SOURCE_ALL "${SOURCES}/Common_Headers/fixmac.h")
list(APPEND SOURCE_ALL "${SOURCES}/Common_Headers/SIL_Library.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/BMW_High_Log_Convertor.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/BMW_High_Log_Convertor.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/BMW_Low_Log_Convertor.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/BMW_Low_Log_Convertor.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/TML_SRR5_Log_Convertor.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/TML_SRR5_Log_Convertor.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/BMW_MID_Log_Convertor.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/BMW_MID_Log_Convertor.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/DSPACE_Common_Log_Converter.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/DSPACE_Common_Log_Converter.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/Motional_Srr3_SIL_Log_Convertor.h")
list(APPEND SOURCE_ALL "${SOURCES}/Customer/Motional_Srr3_SIL_Log_Convertor.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/MUDP_Serializer.h")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/MUDP_Serializer.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/UdpSocket.hpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/UdpSocket.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Version.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/Generic/Version.hpp")
list(APPEND SOURCE_ALL "${SOURCES}/libs/MDF_Writer_Library/source/apt_mdf_log.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/libs/MDF_Writer_Library/inc/apt_mdf_log.h")


if(CMAKE_BUILD_TYPE_TO_UPPER STREQUAL "DEBUG")
    set(BUILD_FOLDER "Debug")
else()
    set(BUILD_FOLDER "Release")
endif()

set(headerfile1 "${SOURCES}/Common_Headers/customer_ID_config.h")
set(headerfile2 "${SOURCES}/Common_Headers/fixmac.h")
set(headerfile3 "${SOURCES}/Generic/LM2_Interface.h")
set(headerfile4 "${SOURCES}/Generic/Interface_Enums.h")
set(headerfile5 "${SOURCES}/Generic/Interface_Input_Struct.h")


if(MSVC)
    set(MDF_LIBS "${MDF_Writer_Library_PROJ}/${BUILD_FOLDER}/Mdf4Lib_x64.dll")
    set(MDF_Log "${MDF_Writer_Library_PROJ}/${BUILD_FOLDER}/Mdf_Log.lib")
    set(pugixml "${pugixml_PROJ}/${BUILD_FOLDER}/pugixml.lib")
    set(Interface_Lib "${CMAKE_BINARY_DIR}/${BUILD_FOLDER}/Interface_Main.dll")
else()
    set(MDF_LIBS "${MDF_Writer_Library_PROJ}/${BUILD_FOLDER}/libmdf4.so")
    set(MDF_Log "${MDF_Writer_Library_PROJ}/${BUILD_FOLDER}/libMdf_Log.a")
    set(pugixml "${pugixml_PROJ}/${BUILD_FOLDER}/libpugixml.a")
    set(Interface_Lib "${CMAKE_BINARY_DIR}/libInterface_Main.so")
endif()

if(${INTERFACE_BUILD_TYPE} STREQUAL "STATIC" )
add_library (${PROJECTNAME} STATIC "") 
target_compile_definitions(${PROJECTNAME} PRIVATE INTERFACE_BUILT_AS_STATIC)
else()
add_library (${PROJECTNAME} SHARED "") 
endif()

################################
### Setting dependencies     ###
################################
target_include_directories(${PROJECTNAME}
    PUBLIC
	${LIBS_DIR}
	${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include
	${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/include/osi3
)

if(NOT ${INTERFACE_BUILD_TYPE} STREQUAL "STATIC" )
	if(MSVC)
	add_custom_command(
			TARGET ${PROJECTNAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${headerfile1} ${headerfile2} ${headerfile3} ${headerfile4} ${headerfile5} ${MDF_LIBS} ${Interface_Lib} "${OUTPUT_PATH}")
	else()
	add_custom_command(
		TARGET ${PROJECTNAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy  ${headerfile1} ${headerfile2} ${headerfile3} ${headerfile4} ${headerfile5} ${MDF_LIBS} ${Interface_Lib} "${OUTPUT_PATH}")
	endif()

	add_custom_command(
		 TARGET ${PROJECTNAME}
		 POST_BUILD
		 COMMAND ${CMAKE_COMMAND} -E copy ${Interface_Main_PROJ}/Interface_Output_Control.xml "${OUTPUT_PATH}/")
endif()

set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
											    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
											    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}") 				

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

if(MSVC)
target_compile_definitions(${PROJECTNAME} PRIVATE PC_RESIM _XKEYCHECK_H _WIN_PLATFORM WINDOWS_IGNORE_PACKING_MISMATCH)
else()
target_compile_definitions(${PROJECTNAME} PRIVATE PC_RESIM _XKEYCHECK_H )
endif()

target_include_directories(${PROJECTNAME} PRIVATE ${SOURCES}/Common_Headers)
target_include_directories(${PROJECTNAME} PRIVATE ${SOURCES}/UDP_logging_headers)
target_include_directories(${PROJECTNAME} PRIVATE ${pugixml_PROJ}/inc)

target_link_libraries (${PROJECTNAME} ${MDF_Log} ${pugixml})

set_target_properties(${PROJECTNAME} PROPERTIES LINK_FLAGS "-Bsymbolic")

if(NOT (${INTERFACE_BUILD_TYPE} STREQUAL "STATIC" ))
	# link protobuf and OSI libraries
	if(MSVC)
		message("${ESCAPE}[31m[WARNING]: Below preprocessor directors are used to suppress the compiler warnings${ESCAPE}${defaultcolor}")
		message("${ESCAPE}[31m\t1. WINDOWS_IGNORE_PACKING_MISMATCH${ESCAPE}${defaultcolor}")
		message("${ESCAPE}[31m\t2. _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING${ESCAPE}${defaultcolor}")
		message("${ESCAPE}[31m\t3. _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING${ESCAPE}${defaultcolor}")
		target_compile_definitions(${PROJECTNAME} PRIVATE WINDOWS_IGNORE_PACKING_MISMATCH)
		target_compile_definitions(${PROJECTNAME} PRIVATE _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING)
		target_compile_definitions(${PROJECTNAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
		set(PROTOBUF_DEBUG_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/Debug/lib")
		set(PROTOBUF_RELEASE_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/Release/lib")
		set(OSI_DEBUG_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/Debug/lib/osi3")
		set(OSI_RELEASE_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/Release/lib/osi3")
		find_library(PROTOBUF_DEBUG_LIB NAMES libprotobufd PATHS "${PROTOBUF_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
		find_library(PROTOBUF_RELEASE_LIB NAMES libprotobuf PATHS "${PROTOBUF_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
		find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
		find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
		target_link_libraries(${PROJECTNAME} debug ${PROTOBUF_DEBUG_LIB} optimized ${PROTOBUF_RELEASE_LIB})
		target_link_libraries(${PROJECTNAME} debug ${OSI_STATIC_DEBUG_LIB} optimized ${OSI_STATIC_RELEASE_LIB})
	else()
		target_link_libraries(${PROJECTNAME} ${OpenSimulationInterfaceStaticLib})
		target_link_libraries(${PROJECTNAME} ${ProtobufStaticLib} )
	endif()
endif()

if(MSVC)
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MT /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")

	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MTd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")
												
	set_target_properties(${PROJECTNAME} PROPERTIES
                                        COMPILE_FLAGS "/Zp2"
                                        ) 
endif(MSVC)
if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif()
