set(REL_PATH Fusion360/logging)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_populate_detections_log.h
    ${REL_PATH}/include/f360_populate_objects_log.h
    ${REL_PATH}/include/f360_populate_internal_clusters_log.h
    ${REL_PATH}/include/f360_populate_internal_cwd_log.h
    ${REL_PATH}/include/f360_populate_internal_detection_history_log.h
    ${REL_PATH}/include/f360_populate_internal_objects_log.h
    ${REL_PATH}/include/f360_populate_internal_reflection_buffer_log.h
    ${REL_PATH}/include/f360_populate_internal_trailer_detector_log.h
    ${REL_PATH}/include/f360_rot_object_output.h
    ${REL_PATH}/source/f360_populate_detections_log.cpp
    ${REL_PATH}/source/f360_populate_objects_log.cpp
    ${REL_PATH}/source/f360_populate_internal_clusters_log.cpp
    ${REL_PATH}/source/f360_populate_internal_cwd_log.cpp
    ${REL_PATH}/source/f360_populate_internal_detection_history_log.cpp
    ${REL_PATH}/source/f360_populate_internal_objects_log.cpp
    ${REL_PATH}/source/f360_populate_internal_reflection_buffer_log.cpp
    ${REL_PATH}/source/f360_populate_internal_trailer_detector_log.cpp
    ${REL_PATH}/source/f360_rot_object_output.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
