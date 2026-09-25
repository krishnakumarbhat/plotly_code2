cmake_minimum_required(VERSION 3.25)

set(PROJECTNAME  "Blf")
project(${PROJECTNAME})


set(LIB_DIRECTORY "${BLF_PROJ}/Libs")

#Linking directories
link_directories(${OUTPUT_PATH})


# Perform Linux specific configuration
if(UNIX)
  # Set RPath of executable to $ORIGIN if Linux so that libbinlog.so can be found in same directory
  set(CMAKE_INSTALL_RPATH "$ORIGIN")
  set(LIB_DIRECTORY "${LIB_DIRECTORY}/Linux64")
  message("Set Library directory to: ${LIB_DIRECTORY}") 
elseif(MSVC)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LIB_DIRECTORY "${LIB_DIRECTORY}/Win64")
  else()
    set(LIB_DIRECTORY "${LIB_DIRECTORY}/Win32")
  endif()
endif()

list(APPEND SOURCE_ALL "${BLF_PROJ}/bl.c")

add_library(${PROJECTNAME} STATIC "")

find_library(BINLOG NAMES binlog HINTS ${LIB_DIRECTORY})
find_file(BINLOG_BINARY NAMES binlog.dll libbinlog.so HINTS ${LIB_DIRECTORY})

#add_executable(bl
#bl.c)

target_include_directories(${PROJECTNAME} PRIVATE "${BLF_PROJ}/Include")  
target_link_libraries(${PROJECTNAME} PUBLIC ${BINLOG})

#install (TARGETS bl DESTINATION ${CMAKE_INSTALL_PREFIX})
#install (FILES ${BINLOG_BINARY} DESTINATION ${CMAKE_INSTALL_PREFIX})

add_custom_command(
  TARGET ${PROJECTNAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy 
    ${BINLOG_BINARY} 
    $<TARGET_FILE_DIR:${PROJECTNAME}>)

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)