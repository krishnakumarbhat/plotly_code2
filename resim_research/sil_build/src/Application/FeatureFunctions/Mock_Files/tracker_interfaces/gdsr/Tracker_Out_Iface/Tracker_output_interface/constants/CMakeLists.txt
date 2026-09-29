cmake_minimum_required(VERSION 3.3)

ADD_LIBRARY(Tracker_api_constants INTERFACE)

target_include_directories(Tracker_api_constants INTERFACE "")


# The following is a hack to make the source files visible in the visual studio IDE
IF(TRACKER_DEVELOPER_MODE)
	SET(Tracker_api_constants_SOURCES
      ${CMAKE_CURRENT_LIST_DIR}/tracker_constants_honda_srr6p.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_constants_nissan_srr6.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_constants_rna.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_constants_srr6p_single_sensor.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_constants_default.h
		${CMAKE_CURRENT_LIST_DIR}/tracker_constants_selector.h
		${CMAKE_CURRENT_LIST_DIR}/TRACKER_OUTPUT_INTERFACE_VERSION_CONTENT.h
		${CMAKE_CURRENT_LIST_DIR}/TRACKER_OUTPUT_INTERFACE_VERSION_DATE.h)
	add_custom_target(Tracker_api_constants_ide SOURCES ${Tracker_api_constants_SOURCES})
	IF(CMAKE_FOLDER)
		# it seems custom targets not automatically set their folder to CMAKE_FOLDER
		set_target_properties(Tracker_api_constants_ide PROPERTIES FOLDER ${CMAKE_FOLDER})
	ELSE()
		set_target_properties(Tracker_api_constants_ide PROPERTIES FOLDER "GDSR Tracker")
	ENDIF()
ENDIF()


