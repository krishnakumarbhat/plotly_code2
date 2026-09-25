# Create project and solution
cmake_minimum_required(VERSION 3.10)
set(target_name AS-bin-writer-lib)

option(BINWRITER_DEVELOPER_MODE "Binwriter developers treat warnings as errors and see all warnings. Even if BINWRITER_MOCK is ON they see a bin writer project in MSVC." OFF)

set(BINWRITER_INSTALL_PREFIX ${OUT_DIR_BIN} CACHE PATH "Binwriter install prefix" FORCE)
set(BINWRITER_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../lib" CACHE PATH "Path to bin writer lib folder")
option(BINWRITER_MOCK "Binwriter is not active, only mock .h files are provided" ON)


	
add_library(${target_name} STATIC "")


set(BINWRITER_MAX_FUSED_SENSORS 4 CACHE STRING "The number of sensors that are fused")
set(BINWRITER_MAX_VARIABLES 1024 CACHE STRING "The number of variables and the number of arrays that the bin writer can handle ")
set(BINWRITER_MAX_ARRAY_SIZE 1024 CACHE STRING "The size of debug arrays that the bin writer can handle")

target_compile_definitions(${target_name} PUBLIC -DAS_BIN_WRITER_MAX_FUSED_SENSORS=${BINWRITER_MAX_FUSED_SENSORS})
target_compile_definitions(${target_name} PUBLIC -DAS_BIN_WRITER_MAX_VARIABLES=${BINWRITER_MAX_VARIABLES})
target_compile_definitions(${target_name} PUBLIC -DAS_BIN_WRITER_MAX_ARRAY_SIZE=${BINWRITER_MAX_ARRAY_SIZE})

include(GenerateExportHeader)
generate_export_header(${target_name})
target_sources(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/as-bin-writer-lib_export.h)
target_include_directories(${target_name} SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Build a bin writer lib that provides the functionality to write to bin files.
if(BINWRITER_DEVELOPER_MODE)
	option(BINWRITER_SUPPRESS_ADDITIONAL_WARNINGS "Suppress some warnings we are normally not interested in." ON)
endif(BINWRITER_DEVELOPER_MODE)

set(BINWRITER_MSVS_WARNING_LEVEL_STANDARD "4")
set(BINWRITER_MSVS_WARNING_LEVEL ${BINWRITER_MSVS_WARNING_LEVEL_STANDARD} CACHE STRING "Visual Studio Warning level")
if(NOT ${BINWRITER_MSVS_WARNING_LEVEL} EQUAL BINWRITER_MSVS_WARNING_LEVEL_STANDARD)
	# only give status if not standard value
	message(STATUS "-- BINWRITER_MSVS_WARNING_LEVEL=${BINWRITER_MSVS_WARNING_LEVEL}")
endif()


#target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/lib_mock/AS_bin_writer_wrapper.h)
target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/AS_bin_writer_wrapper.h)
target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/AS_bin_writer_wrapper_plainC.h)
target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/binary_writer_lib.h)
target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/AS_bin_writer_wrapper.cpp)
target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/binary_writer_lib.cpp)
target_sources(${target_name} PRIVATE ${BINWRITER_ROOT_PATH}/binary_writer_lib_sizes.h)


target_include_directories(${target_name} SYSTEM PUBLIC ${BINWRITER_ROOT_PATH})


if(NOT CMAKE_FOLDER)
	set_target_properties (${target_name} PROPERTIES FOLDER "Libraries")
endif()

target_compile_definitions(${target_name} PUBLIC -DBINARY_DEBUG)
if(MSVC)
	# PC_RESIM is needed because we are linking to the Shared Toolbox and the the Shared Toolbox is including fixmac.h
	target_compile_definitions(${target_name} PRIVATE -DPC_RESIM)
	target_compile_options(${target_name} PRIVATE /W${BINWRITER_MSVS_WARNING_LEVEL})
	if(BINWRITER_DEVELOPER_MODE)
		target_compile_options(${target_name} PRIVATE /WX)
	endif(BINWRITER_DEVELOPER_MODE)
	if(BINWRITER_SUPPRESS_ADDITIONAL_WARNINGS OR NOT BINWRITER_DEVELOPER_MODE)
		# suppress warning for "overflow in constant arithmetic"
		target_compile_options(${target_name} PRIVATE /wd4756)
		# suppress warning for "overflow in floating-point constant arithmetic"
		target_compile_options(${target_name} PRIVATE /wd4056)
	endif()
endif()
if(BINWRITER_INSTALL_PREFIX)
	set_target_properties (${target_name} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINWRITER_INSTALL_PREFIX}
	)
	install(TARGETS ${target_name} DESTINATION ${BINWRITER_INSTALL_PREFIX})
endif()

if(MSVC)
	get_target_property(AS-bin-writer-lib_SOURCES ${target_name} SOURCES)
	LIST(REMOVE_ITEM AS-bin-writer-lib_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/as-bin-writer-lib_export.h)
	source_group(TREE ${BINWRITER_ROOT_PATH} FILES ${AS-bin-writer-lib_SOURCES})
endif()
