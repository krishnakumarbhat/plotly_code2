##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 3.10)
link_directories(${OUTPUT_PATH})

if(${TRACKER_TYPE} STREQUAL "SRR")
	set(PROJECTNAME  "SRR_DC_SIL_LIB")
elseif(${TRACKER_TYPE} STREQUAL "MRR")
	set(PROJECTNAME  "MRR_DC_SIL_LIB")
endif()

project(${PROJECTNAME})
set(ROOT_PATH 							"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../")
set(RECU_SIL_SOURCE_CORE					"${ROOT_PATH}/DC_SIL_Library/SIL_INT/DC_SIL_SOURCE/SIL_CORE")
set(LIBS_DIR "${RECU_SIL_SOURCE_CORE}/SMIface/Libs")
set(RECU_SIL_LIB_PATH 							"${CMAKE_CURRENT_SOURCE_DIR}/../")
set(SOURCE_ALL 									"${RECU_SIL_LIB_PATH}/RECU_SIL_fw_wrapper.cpp")
list(APPEND SOURCE_ALL 							"${RECU_SIL_LIB_PATH}/RECU_SIL_fw_wrapper.h")
list(APPEND SOURCE_ALL 							"${RECU_SIL_LIB_PATH}/Resource.rc")
#Creating Static Library
set(target_name ${PROJECTNAME})
add_library(${target_name} SHARED "")


	
target_compile_definitions(${target_name} PRIVATE #TASKING_COMPILER 
                                                    RECU_SIL_PROXY_EXPORTS
                                                    ) 

if(CUSTOMER)
target_compile_definitions(${target_name} PRIVATE CUSTOMER_${CUSTOMER})
endif(CUSTOMER)

if(NOT CMAKE_CROSSCOMPILING)
    target_compile_definitions(${target_name} PRIVATE PC_RESIM)
    #target_compile_definitions(${target_name} PUBLIC _WIN32)
    #target_compile_definitions(${target_name} PUBLIC _XKEYCHECK_H)
    target_compile_definitions(${target_name} PRIVATE BINARY_DEBUG)
endif(NOT CMAKE_CROSSCOMPILING)

target_sources(${target_name} PRIVATE ${SOURCE_ALL})

source_group(TREE ${RECU_SIL_LIB_PATH} FILES	${SOURCE_ALL})

target_link_libraries(${target_name} CED)
target_link_libraries(${target_name} LCDA)
target_link_libraries(${target_name} CTA)
target_link_libraries(${target_name} PT)
target_link_libraries(${target_name} TA)
target_link_libraries(${target_name} RECW)
target_link_libraries(${target_name} SCW)
target_link_libraries(${target_name} LTB)
target_link_libraries(${target_name} ESA)
target_link_libraries(${target_name} AS-bin-writer-lib)
target_link_libraries(${target_name} pugixml)
target_link_libraries(${target_name} ecu_sync)
target_link_libraries(${target_name} sp_25_tracker_wrapper)
target_link_libraries(${target_name} ${CUST}_DC_RESIM)
target_link_libraries(${target_name} MLMathLibrary)

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

# link protobuf and OSI libraries
if(WIN32)
	set(PROTOBUF_DEBUG_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/Windows/Debug")
	set(PROTOBUF_RELEASE_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/Windows/Release")
	set(OSI_DEBUG_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/Windows/Debug")
	set(OSI_RELEASE_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/Windows/Release")
	find_library(PROTOBUF_DEBUG_LIB libprotobufd PATHS "${PROTOBUF_DEBUG_LIB_PATH}/lib")
	find_library(PROTOBUF_RELEASE_LIB libprotobuf PATHS "${PROTOBUF_RELEASE_LIB_PATH}/lib")
	find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}/lib/osi3")
	find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}/lib/osi3")
	target_link_libraries(${target_name} debug ${PROTOBUF_DEBUG_LIB} optimized ${PROTOBUF_RELEASE_LIB})
	target_link_libraries(${target_name} debug ${OSI_STATIC_DEBUG_LIB} optimized ${OSI_STATIC_RELEASE_LIB})
	target_link_libraries(${target_name} shlwapi.lib)
else()
	get_filename_component(ProtobufStaticLib "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/${protolibname}" ABSOLUTE)
	get_filename_component(OpenSimulationInterfaceStaticLib "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/osi3/libopen_simulation_interface_static.a" ABSOLUTE)
	target_link_libraries(${target_name} ${OpenSimulationInterfaceStaticLib})
	target_link_libraries(${target_name} ${ProtobufStaticLib} )
	target_link_libraries(${target_name} -lpthread -static-libgcc -static-libstdc++ stdc++fs)	
endif()

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
            RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
            ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

set_target_properties(${target_name} PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN,-Bsymbolic")

if(MSVC)
    target_compile_definitions(${target_name} PRIVATE _XKEYCHECK_H)
endif(MSVC)

set(VARIANT_TRACKER "f360_variant_A")
set(RSPP_VARIANT_TRACKER "rspp_variant_A")
add_definitions(-Df360_variant_A=${VARIANT_TRACKER})
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_TRACKER})
