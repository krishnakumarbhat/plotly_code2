cmake_minimum_required(VERSION 3.3)

ADD_LIBRARY(Tracker_api_radar_parameter_output INTERFACE)

target_include_directories(Tracker_api_radar_parameter_output INTERFACE "")



if(NOT TARGET Tracker_api_constants)
	MESSAGE(FATAL_ERROR "target Tracker_api_constants is not available")
ENDIF()

TARGET_LINK_LIBRARIES(Tracker_api_radar_parameter_output INTERFACE Tracker_api_constants)
TARGET_LINK_LIBRARIES(Tracker_api_radar_parameter_output INTERFACE Tracker_api_vehicle_output)
TARGET_LINK_LIBRARIES(Tracker_api_radar_parameter_output INTERFACE Tracker_api_detections_output)

if(TARGET SRR_CORE_LIB)
	TARGET_LINK_LIBRARIES(Tracker_api_radar_parameter_output INTERFACE SRR_CORE_LIB) # to get Reuse.h
endif()

# The following is a hack to make the source files visible in the visual studio IDE
IF(TRACKER_DEVELOPER_MODE)
	SET(Tracker_api_radar_parameter_output_HEADERS
		${CMAKE_CURRENT_LIST_DIR}/RADAR_PARAMS_FLT_T.h
		${CMAKE_CURRENT_LIST_DIR}/DWELL_TYPE_T.h
		${CMAKE_CURRENT_LIST_DIR}/FOV_LINES_T.h
      ${CMAKE_CURRENT_LIST_DIR}/exclusion_zone_t.h
		${CMAKE_CURRENT_LIST_DIR}/sort_det_T.h
		${CMAKE_CURRENT_LIST_DIR}/VEHICLE_DATA_RADAR_PARAMETER_FLT_T.h)

	add_custom_target(Tracker_api_radar_parameter_output_ide SOURCES ${Tracker_api_radar_parameter_output_HEADERS})
	IF(CMAKE_FOLDER)
		# it seems custom targets not automatically set their folder to CMAKE_FOLDER
		set_target_properties(Tracker_api_radar_parameter_output_ide PROPERTIES FOLDER ${CMAKE_FOLDER})
	ELSE()
		set_target_properties(Tracker_api_radar_parameter_output_ide PROPERTIES FOLDER "GDSR Tracker")
	ENDIF()
ENDIF()
