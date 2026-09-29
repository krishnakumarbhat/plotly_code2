# Add types directory
if(NOT TARGET U360_Tracker_api_types)
   add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/types" "${CMAKE_CURRENT_BINARY_DIR}/types")
endif()
