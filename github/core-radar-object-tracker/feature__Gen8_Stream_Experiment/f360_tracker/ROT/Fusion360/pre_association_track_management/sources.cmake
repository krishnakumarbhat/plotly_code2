set(REL_PATH Fusion360/pre_association_track_management)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_det_angle_jump_internals.h
    ${REL_PATH}/include/f360_detection_angle_jumps.h
    ${REL_PATH}/include/f360_mark_objects_entering_dead_zone.h
    ${REL_PATH}/include/f360_mark_objects_entering_dead_zone_helpers.h
    ${REL_PATH}/include/f360_object_based_angle_jump_detector.h
    ${REL_PATH}/include/f360_object_based_angle_jump_detector_internals.h
    ${REL_PATH}/include/f360_object_based_multibounce_detector.h
    ${REL_PATH}/include/f360_object_based_multibounce_detector_internals.h
    ${REL_PATH}/include/f360_object_based_radar_phenomena.h
    ${REL_PATH}/include/f360_object_based_radar_phenomena_internals.h
    ${REL_PATH}/include/f360_object_based_water_spray_detector.h
    ${REL_PATH}/include/f360_object_based_water_spray_detector_internals.h
    ${REL_PATH}/include/f360_pre_association_track_management.h
    ${REL_PATH}/source/f360_det_angle_jump_internals.cpp
    ${REL_PATH}/source/f360_detection_angle_jumps.cpp
    ${REL_PATH}/source/f360_mark_objects_entering_dead_zone.cpp
    ${REL_PATH}/source/f360_mark_objects_entering_dead_zone_helpers.cpp
    ${REL_PATH}/source/f360_object_based_angle_jump_detector.cpp
    ${REL_PATH}/source/f360_object_based_angle_jump_detector_internals.cpp
    ${REL_PATH}/source/f360_object_based_multibounce_detector.cpp
    ${REL_PATH}/source/f360_object_based_multibounce_detector_internals.cpp
    ${REL_PATH}/source/f360_object_based_radar_phenomena.cpp
    ${REL_PATH}/source/f360_object_based_radar_phenomena_internals.cpp
    ${REL_PATH}/source/f360_object_based_water_spray_detector.cpp
    ${REL_PATH}/source/f360_object_based_water_spray_detector_internals.cpp
    ${REL_PATH}/source/f360_pre_association_track_management.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
