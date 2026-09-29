cmake_minimum_required(VERSION 3.3)

ADD_LIBRARY(Tracker_api_object_output INTERFACE)

target_include_directories(Tracker_api_object_output INTERFACE "")


if(NOT TARGET Tracker_api_constants)
	MESSAGE(FATAL_ERROR "target Tracker_api_constants is not available")
ENDIF()

TARGET_LINK_LIBRARIES(Tracker_api_object_output INTERFACE Tracker_api_constants)

if(TARGET SRR_CORE_LIB)
	TARGET_LINK_LIBRARIES(Tracker_api_object_output INTERFACE SRR_CORE_LIB) # to get Reuse.h
endif()

OPTION(TRACKER_SUPPRESS_ID "Suppress the id field in the tracker object output structures" OFF)
IF(TRACKER_SUPPRESS_ID)
	target_compile_definitions(Tracker_api_object_output INTERFACE TRACKER_SUPPRESS_ID)
ENDIF()
# The following is a hack to make the source files visible in the visual studio IDE
IF(TRACKER_DEVELOPER_MODE)
	SET(Tracker_api_object_output_HEADERS
		${CMAKE_CURRENT_LIST_DIR}/object_class_T.h
		${CMAKE_CURRENT_LIST_DIR}/OBJECT_CLASS_PROBABILITY_T.h
		${CMAKE_CURRENT_LIST_DIR}/SENSOR_FLAG_T.h
		${CMAKE_CURRENT_LIST_DIR}/RSDS_OBJECT_ACCURACY_T.h
		${CMAKE_CURRENT_LIST_DIR}/RSDS_OBJECT_INNOVATION_OUT_FLT_T.h
		${CMAKE_CURRENT_LIST_DIR}/OBJECT_VARIANCE_T.h
		${CMAKE_CURRENT_LIST_DIR}/reference_position_T.h
		${CMAKE_CURRENT_LIST_DIR}/track_status_T.h
		${CMAKE_CURRENT_LIST_DIR}/curvi_coordinates_calc_method_T.h
		${CMAKE_CURRENT_LIST_DIR}/SpaceTimeDerivative_2D.h)

	add_custom_target(Tracker_api_object_output_ide SOURCES ${Tracker_api_object_output_HEADERS})
	IF(CMAKE_FOLDER)
		# it seems custom targets not automatically set their folder to CMAKE_FOLDER
		set_target_properties(Tracker_api_object_output_ide PROPERTIES FOLDER ${CMAKE_FOLDER})
	ELSE()
		set_target_properties(Tracker_api_object_output_ide PROPERTIES FOLDER "GDSR Tracker")
	ENDIF()
ENDIF()


