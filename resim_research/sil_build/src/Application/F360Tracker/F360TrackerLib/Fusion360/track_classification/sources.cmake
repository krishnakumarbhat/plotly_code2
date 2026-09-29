set(REL_PATH Fusion360/track_classification)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_calculate_object_class_probabilities.h
    ${REL_PATH}/include/f360_is_object_suspected_stationary.h
    ${REL_PATH}/include/f360_is_object_suspected_stationary_helpers.h
    ${REL_PATH}/include/f360_object_occlusion_classification.h
    ${REL_PATH}/include/f360_object_motion_classification.h
    ${REL_PATH}/include/f360_object_motion_classification_helpers.h
    ${REL_PATH}/include/f360_track_classification.h
    ${REL_PATH}/include/f360_assign_underdrivability_status_to_tracks_ocg.h
    ${REL_PATH}/include/f360_assign_underdrivability_status_to_tracks_sg.h
    ${REL_PATH}/include/f360_classification_underdrivability_moving.h
    ${REL_PATH}/include/f360_object_underdrivability_classification.h
    ${REL_PATH}/include/f360_update_object_orientation_uncertainty.h
    ${REL_PATH}/include/f360_object_list_angle_jump_qualifier.h
	${REL_PATH}/include/f360_cluster_slow_moving_objects.h
    ${REL_PATH}/source/f360_calculate_object_class_probabilities.cpp
    ${REL_PATH}/source/f360_is_object_suspected_stationary.cpp
    ${REL_PATH}/source/f360_is_object_suspected_stationary_helpers.cpp
    ${REL_PATH}/source/f360_object_occlusion_classification.cpp
    ${REL_PATH}/source/f360_object_motion_classification.cpp
    ${REL_PATH}/source/f360_object_motion_classification_helpers.cpp
    ${REL_PATH}/source/f360_track_classification.cpp
    ${REL_PATH}/source/f360_assign_underdrivability_status_to_tracks_ocg.cpp
    ${REL_PATH}/source/f360_assign_underdrivability_status_to_tracks_sg.cpp
    ${REL_PATH}/source/f360_classification_underdrivability_moving.cpp
    ${REL_PATH}/source/f360_object_underdrivability_classification.cpp
    ${REL_PATH}/source/f360_update_object_orientation_uncertainty.cpp
    ${REL_PATH}/source/f360_object_list_angle_jump_qualifier.cpp
	${REL_PATH}/source/f360_cluster_slow_moving_objects.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
