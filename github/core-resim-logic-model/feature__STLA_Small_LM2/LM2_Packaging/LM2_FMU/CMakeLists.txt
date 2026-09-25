cmake_minimum_required(VERSION 3.5)

#Set the FMU Model Name and Project
if("${FMU_Model_Name}" STREQUAL "")
	set(FMU_Model_Name "SRR_Master_LogicModel2")
	project(${FMU_Model_Name})
else()
	add_definitions(-DFMU_Model_Name=${FMU_Model_Name})
	project(${FMU_Model_Name})
endif()

# set customer_fw , see customer_framework_version.txt in lm2 repo documentation folder for more details
if("${CUSTOMER_FW}" STREQUAL "")
	set(CUSTOMER_FW 0)
	add_definitions(-DCUSTOMER_FW=0)
else()
	add_definitions(-DCUSTOMER_FW=${CUSTOMER_FW})
endif()

# set update_version
if("${FW_UPDATE_VERSION}" STREQUAL "")
	set(FW_UPDATE_VERSION 1)
	add_definitions(-DFW_UPDATE_VERSION=1)
else()
	add_definitions(-DFW_UPDATE_VERSION=${FW_UPDATE_VERSION})
endif()

# set number of input ports
if("${NUM_INPUTS}" STREQUAL "")
	set(NUM_INPUTS 7)
	add_definitions(-DNUM_INPUTS=7)
else()
	add_definitions(-DNUM_INPUTS=${NUM_INPUTS})
endif()

# set number of output ports
if("${NUM_OUTPUTS}" STREQUAL "")
	set(NUM_OUTPUTS 7)
	add_definitions(-DNUM_OUTPUTS=7)
else()
	add_definitions(-DNUM_OUTPUTS=${NUM_OUTPUTS})
endif()

if(MSVC)
	set(OS_FOLDER_TYPE "Windows")
else()
	set(OS_FOLDER_TYPE "Linux")
	set(CMAKE_CXX_FLAGS "-fPIC")
endif()

add_definitions(-DLM2_CUSTOMER_VERSION=${RADAR_LM2_CUSTOMER_VERSION})
add_definitions(-DLM2_VERSION_MAJOR=${RADAR_LM2_VERSION_MAJOR})
add_definitions(-DLM2_VERSION_MINOR=${RADAR_LM2_VERSION_MINOR})
add_definitions(-DLM2_VERSION_WEEK_YEAR=${RADAR_LM2_VERSION_WEEK_YEAR})

if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	set(BUILD_FOLDER_TYPE "Release")
	set(protolibname "libprotobuf.a")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	set(BUILD_FOLDER_TYPE "Debug")
	set(protolibname "libprotobufd.a")
endif()

if("${CUSTOMER}" STREQUAL "")
	message(FATAL_ERROR "INVALID CUSTOMER") 
else()
	add_definitions(-DCUSTOMER_ENUM=${CUSTOMER_ID})
endif()

set(INTERFACE_SOURCE_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/../../Code")
set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Deliverables")

#Set path to Cmake folder
set(PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(LIBS_DIR "${PROJECT_ROOT_PATH}/../../Code/libs")
message(${PROJECT_ROOT_PATH})
# set Protobuf and OSI library versions
set(ProtoLibraryVersion "protobuf-3.6.1")
set(OSILibraryVersion "open-simulation-interface-3.5.0_w_Proto_3.6.1")
get_filename_component(ProtobufStaticLib "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/${protolibname}" ABSOLUTE)
get_filename_component(OpenSimulationInterfaceStaticLib "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/osi3/libopen_simulation_interface_static.a" ABSOLUTE)
set(ZLIB_LIBRARY "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/")
set(ZLIB_INCLUDE_DIR "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include")

set(LM2_FMU_PATH "${PROJECT_ROOT_PATH}")

set(FMI2_SOURCE "${PROJECT_ROOT_PATH}/Code/FMI2")
set(FMU_SOURCE "${PROJECT_ROOT_PATH}/Code")
set(LM2_CUST_INCLUDE "${PROJECT_ROOT_PATH}/Customer/${CUSTOMER}/Include")
set(LM2_CUST_SOURCE "${PROJECT_ROOT_PATH}/Customer/${CUSTOMER}/Source")

string(TIMESTAMP FMUTIMESTAMP UTC)
string(MD5 FMUGUID "${FMU_Model_Name}_${FMUTIMESTAMP}")
configure_file(${LM2_FMU_PATH}/modelDescription.in.xml ${LM2_FMU_PATH}/modelDescription.xml @ONLY)

# FMU CORE source and header files
file(GLOB_RECURSE LM2_FMU_SOURCES "${FMU_SOURCE}/*.cpp")
file(GLOB_RECURSE LM2_FMU_HEADERS "${FMU_SOURCE}/*.h")

# FMU CUST source and header files
file(GLOB_RECURSE LM2_FMI_SOURCES "${FMI2_SOURCE}/*.cpp")
file(GLOB_RECURSE LM2_FMI_HEADERS "${FMI2_SOURCE}/*.h")

set(INTERFACE_BUILD_TYPE "STATIC")


set(Interface_Main "${CMAKE_CURRENT_SOURCE_DIR}/../LM2_Interface")
add_subdirectory(${Interface_Main} "${CMAKE_CURRENT_BINARY_DIR}/Interface_Main")

set(FMU_SOURCES_FILES 	${FMU_SOURCE}/FMI_interface.cpp
						${FMU_SOURCE}/LogicModelBase2.cpp
						${FMU_SOURCE}/LogicModelBase2.h
						${FMU_SOURCE}/LogicModel2.cpp
						${FMU_SOURCE}/LogicModel2.h
)
if(MSVC)
add_compile_options("/Zp2")  
endif(MSVC)
		
add_library(${FMU_Model_Name} SHARED ${FMU_SOURCES_FILES})
target_link_options(${FMU_Model_Name} PRIVATE "LINKER:-Bsymbolic")
if(NOT MSVC)
	set_target_properties(${FMU_Model_Name} PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN")
	set_target_properties(${FMU_Model_Name} PROPERTIES PREFIX "")
	set_property(TARGET ${FMU_Model_Name} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--disable-new-dtags")
endif()

target_compile_definitions(${FMU_Model_Name} PRIVATE INTERFACE_BUILT_AS_STATIC)
include_directories(${FMI2_SOURCE}
					${FMU_SOURCE}
					${CMAKE_CURRENT_SOURCE_DIR}/../../Code/Generic
					${CMAKE_CURRENT_SOURCE_DIR}/../../Code/Common_Headers
					${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include
					${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/include/osi3)
					
target_link_libraries(${FMU_Model_Name} Interface_Main)

set_target_properties(${FMU_Model_Name} PROPERTIES LINK_FLAGS "-Bsymbolic")

if(MSVC)
    message("${ESCAPE}[31m[WARNING]: Below preprocessor directors are used to suppress the compiler warnings${ESCAPE}${defaultcolor}")
	message("${ESCAPE}[31m\t1. WINDOWS_IGNORE_PACKING_MISMATCH${ESCAPE}${defaultcolor}")
	message("${ESCAPE}[31m\t2. _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING${ESCAPE}${defaultcolor}")
	message("${ESCAPE}[31m\t3. _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING${ESCAPE}${defaultcolor}")
	target_compile_definitions(${FMU_Model_Name} PRIVATE WINDOWS_IGNORE_PACKING_MISMATCH)
	target_compile_definitions(${FMU_Model_Name} PRIVATE _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING)
    target_compile_definitions(${FMU_Model_Name} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
	set(PROTOBUF_DEBUG_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/Debug/lib")
	set(PROTOBUF_RELEASE_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/Release/lib")
	set(OSI_DEBUG_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/Debug/lib/osi3")
	set(OSI_RELEASE_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/Release/lib/osi3")
	find_library(PROTOBUF_DEBUG_LIB NAMES libprotobufd PATHS "${PROTOBUF_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(PROTOBUF_RELEASE_LIB NAMES libprotobuf PATHS "${PROTOBUF_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}" NO_DEFAULT_PATH)
	find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}" NO_DEFAULT_PATH)
	target_link_libraries(${FMU_Model_Name} debug ${PROTOBUF_DEBUG_LIB} optimized ${PROTOBUF_RELEASE_LIB})
	target_link_libraries(${FMU_Model_Name} debug ${OSI_STATIC_DEBUG_LIB} optimized ${OSI_STATIC_RELEASE_LIB})
else()
	target_link_libraries(${FMU_Model_Name} ${OpenSimulationInterfaceStaticLib})
	target_link_libraries(${FMU_Model_Name} ${ProtobufStaticLib} )
endif()

target_compile_definitions(${FMU_Model_Name} PRIVATE "FMU_SHARED_OBJECT")
target_compile_definitions(${FMU_Model_Name} PRIVATE "FMU_GUID=\"${FMUGUID}\"")



if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FMI_BINARIES_PLATFORM "win64")
    else()
        set(FMI_BINARIES_PLATFORM "win32")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FMI_BINARIES_PLATFORM "linux64")
    else()
        set(FMI_BINARIES_PLATFORM "linux32")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FMI_BINARIES_PLATFORM "darwin64")
    else()
        set(FMI_BINARIES_PLATFORM "darwin32")
    endif()
endif()

find_package(Python COMPONENTS Interpreter Development REQUIRED)

add_custom_command(TARGET ${FMU_Model_Name}
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E echo "Building FMU for ${CUSTOMER}"

# Copy Windows DLL to deliverables folder
COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${FMU_Model_Name}> ${LM2_FMU_PATH}/../Deliverables

# FMU preparation
COMMAND ${CMAKE_COMMAND} -E echo "Preparing FMU files using Python script"
COMMAND ${Python_EXECUTABLE} ${LM2_FMU_PATH}/prepare_fmu.py 
		$<TARGET_FILE_DIR:${FMU_Model_Name}> 
		${LM2_FMU_PATH}/../Deliverables 
		${PROJECT_ROOT_PATH} 
		${CUSTOMER} 
		${FMU_Model_Name}

# Copy model description and create FMU
COMMAND ${CMAKE_COMMAND} -E copy ${LM2_FMU_PATH}/modelDescription.xml $<TARGET_FILE_DIR:${FMU_Model_Name}>/buildfmu
COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${FMU_Model_Name}> $<$<PLATFORM_ID:Windows>:$<$<CONFIG:Debug>:$<TARGET_PDB_FILE:${FMU_Model_Name}>>>  $<TARGET_FILE_DIR:${FMU_Model_Name}>/buildfmu/binaries/${FMI_BINARIES_PLATFORM}
COMMAND ${CMAKE_COMMAND} -E chdir $<TARGET_FILE_DIR:${FMU_Model_Name}>/buildfmu ${CMAKE_COMMAND} -E tar "cfv" "../${FMU_Model_Name}_${CUSTOMER}.fmu" --format=zip "modelDescription.xml" "binaries/win64" "binaries/linux64"
COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${FMU_Model_Name}>/${FMU_Model_Name}_${CUSTOMER}.fmu ${LM2_FMU_PATH}/../Deliverables
)
