##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "CrossPlatform")
project(${PROJECTNAME})

set(CROSS_PLATFORM_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

list(APPEND SOURCE_ALL ${CROSS_PLATFORM_PATH}/genutil.cpp)
list(APPEND SOURCE_ALL ${CROSS_PLATFORM_PATH}/genutil.h)

set(target_name ${PROJECTNAME})

add_library(${target_name} STATIC "")

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})
target_include_directories(${PROJECTNAME} PUBLIC ${CROSS_PLATFORM_PATH})


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												) 				
endif(OUTPUT_PATH)

target_include_directories(${PROJECTNAME} PUBLIC ${COMMON_INC})
target_include_directories(${PROJECTNAME} PUBLIC ${COMMON_INC}/udp_headers)
if(MSVC)
	target_compile_definitions(${PROJECTNAME} PUBLIC _WIN_PLATFORM)
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)