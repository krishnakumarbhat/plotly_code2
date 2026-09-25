if(NOT TARGET Z2_SRR)
   add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/raw_detections" "${CMAKE_CURRENT_BINARY_DIR}/raw_detections")
endif()
