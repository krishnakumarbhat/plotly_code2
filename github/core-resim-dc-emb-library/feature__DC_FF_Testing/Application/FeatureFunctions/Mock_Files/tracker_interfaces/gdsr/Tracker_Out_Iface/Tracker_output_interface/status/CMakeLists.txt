cmake_minimum_required(VERSION 3.3)

ADD_LIBRARY(Tracker_api_status_output INTERFACE)

target_include_directories(Tracker_api_status_output INTERFACE "")


if(NOT TARGET Tracker_api_constants)
	MESSAGE(FATAL_ERROR "target Tracker_api_constants is not available")
ENDIF()

TARGET_LINK_LIBRARIES(Tracker_api_status_output INTERFACE Tracker_api_constants)

if(TARGET SRR_CORE_LIB)
	TARGET_LINK_LIBRARIES(Tracker_api_status_output INTERFACE SRR_CORE_LIB) # to get Reuse.h
endif()

# The following is a hack to make the source files visible in the visual studio IDE
IF(TRACKER_DEVELOPER_MODE)
	SET(Tracker_api_status_output_HEADERS
		${CMAKE_CURRENT_LIST_DIR}/TRACKER_STATUS_T.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_cal_type_T.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_errors_T.h)

	add_custom_target(Tracker_api_status_output_ide SOURCES ${Tracker_api_status_output_HEADERS})
	IF(CMAKE_FOLDER)
		# it seems custom targets not automatically set their folder to CMAKE_FOLDER
		set_target_properties(Tracker_api_status_output_ide PROPERTIES FOLDER ${CMAKE_FOLDER})
	ELSE()
		set_target_properties(Tracker_api_status_output_ide PROPERTIES FOLDER "GDSR Tracker")
	ENDIF()
ENDIF()


