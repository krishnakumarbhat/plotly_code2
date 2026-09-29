set(REL_PATH Fusion360/trailer_manager)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_trailer_manager.h
    ${REL_PATH}/include/f360_identify_and_flag_internal_reflections.h
    ${REL_PATH}/include/f360_mark_trailer_detections.h
    ${REL_PATH}/source/f360_trailer_manager.cpp
    ${REL_PATH}/source/f360_identify_and_flag_internal_reflections.cpp
    ${REL_PATH}/source/f360_mark_trailer_detections.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
