set(REL_PATH Fusion360/detection_to_track_association)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_cond_deassoc_low_rr_dets.h
    ${REL_PATH}/include/f360_detection_association_countermeasures.h
    ${REL_PATH}/include/f360_detection_association_evaluation.h
    ${REL_PATH}/include/f360_detection_association_support_functions.h
    ${REL_PATH}/include/f360_detection_to_track_association.h
    ${REL_PATH}/include/f360_determine_association_hypothesis.h
    ${REL_PATH}/include/f360_dets_inside_bbox.h
    ${REL_PATH}/include/f360_mark_azimuth_range_rate_outliers.h
    ${REL_PATH}/include/f360_mark_detections_wheel_spin_from_objects.h
    ${REL_PATH}/include/f360_mark_dets_as_close_target_and_farside.h
    ${REL_PATH}/include/f360_nearby_wheel_spins.h
    ${REL_PATH}/include/f360_nearby_wheel_spins_support_functions.h
    ${REL_PATH}/include/f360_update_extended_bbox_offsets_for_object_in_dead_zone.h
    ${REL_PATH}/source/f360_cond_deassoc_low_rr_dets.cpp
    ${REL_PATH}/source/f360_detection_association_countermeasures.cpp
    ${REL_PATH}/source/f360_detection_association_evaluation.cpp
    ${REL_PATH}/source/f360_detection_association_support_functions.cpp
    ${REL_PATH}/source/f360_detection_to_track_association.cpp
    ${REL_PATH}/source/f360_determine_association_hypothesis.cpp
    ${REL_PATH}/source/f360_dets_inside_bbox.cpp
    ${REL_PATH}/source/f360_mark_azimuth_range_rate_outliers.cpp
    ${REL_PATH}/source/f360_mark_detections_wheel_spin_from_objects.cpp
    ${REL_PATH}/source/f360_mark_dets_as_close_target_and_farside.cpp
    ${REL_PATH}/source/f360_nearby_wheel_spins.cpp
    ${REL_PATH}/source/f360_nearby_wheel_spins_support_functions.cpp
    ${REL_PATH}/source/f360_update_extended_bbox_offsets_for_object_in_dead_zone.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
