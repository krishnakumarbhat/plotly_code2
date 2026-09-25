add_library(SIL_Library INTERFACE)

target_include_directories(SIL_Library INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_sources(SIL_Library INTERFACE ${CMAKE_CURRENT_LIST_DIR}/Radar_Config.h)
target_sources(SIL_Library INTERFACE ${CMAKE_CURRENT_LIST_DIR}/Tracker_Wrapper.h)
