cmake_minimum_required(VERSION 3.3)

ADD_LIBRARY(Tracker_api_object_legacy_output INTERFACE)

target_include_directories(Tracker_api_object_legacy_output INTERFACE "")



if(NOT TARGET Tracker_api_constants)
	MESSAGE(FATAL_ERROR "target Tracker_api_constants is not available")
ENDIF()

OPTION(TRACKER_SUPPRESS_ID "Suppress the id field in the tracker object output structures" OFF)
IF(TRACKER_SUPPRESS_ID)
	target_compile_definitions(Tracker_api_object_legacy_output INTERFACE TRACKER_SUPPRESS_ID)
ENDIF()

TARGET_LINK_LIBRARIES(Tracker_api_object_legacy_output INTERFACE Tracker_api_constants)
TARGET_LINK_LIBRARIES(Tracker_api_object_legacy_output INTERFACE Tracker_api_object_output)

if(TARGET SRR_CORE_LIB)
	TARGET_LINK_LIBRARIES(Tracker_api_object_legacy_output INTERFACE SRR_CORE_LIB) # to get Reuse.h
endif()

# The following is a hack to make the source files visible in the visual studio IDE
IF(TRACKER_DEVELOPER_MODE)
	SET(Tracker_api_object_legacy_output_HEADERS
		${CMAKE_CURRENT_LIST_DIR}/TRACKER_OUTPUT_T.h)

	add_custom_target(Tracker_api_object_legacy_output_ide SOURCES ${Tracker_api_object_legacy_output_HEADERS})
	IF(CMAKE_FOLDER)
		# it seems custom targets not automatically set their folder to CMAKE_FOLDER
		set_target_properties(Tracker_api_object_legacy_output_ide PROPERTIES FOLDER ${CMAKE_FOLDER})
	ELSE()
		set_target_properties(Tracker_api_object_legacy_output_ide PROPERTIES FOLDER "GDSR Tracker")
	ENDIF()
ENDIF()


