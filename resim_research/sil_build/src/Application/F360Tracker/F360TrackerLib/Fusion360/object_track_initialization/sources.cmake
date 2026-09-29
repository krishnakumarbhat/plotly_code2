set(REL_PATH Fusion360/object_track_initialization)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_determine_final_vel_estimate.h
    ${REL_PATH}/include/f360_estimate_velocity_by_cloud.h
    ${REL_PATH}/include/f360_estimate_velocity_by_position_change.h
    ${REL_PATH}/include/f360_estimate_velocity_by_position_change_helpers.h
    ${REL_PATH}/include/f360_initialization_debug_data.h
    ${REL_PATH}/include/f360_initial_detection_checks.h
    ${REL_PATH}/include/f360_object_track_initialization.h
    ${REL_PATH}/include/f360_populate_track_properties.h
    ${REL_PATH}/include/f360_prioritize_clusters.h
    ${REL_PATH}/include/f360_test_stationary_hypothesis.h
    ${REL_PATH}/include/f360_post_estimate_cloud_only_init_countermeasure.h
    ${REL_PATH}/source/f360_determine_final_vel_estimate.cpp
    ${REL_PATH}/source/f360_estimate_velocity_by_cloud.cpp
    ${REL_PATH}/source/f360_estimate_velocity_by_position_change.cpp
    ${REL_PATH}/source/f360_estimate_velocity_by_position_change_helpers.cpp
    ${REL_PATH}/source/f360_initial_detection_checks.cpp
    ${REL_PATH}/source/f360_object_track_initialization.cpp
    ${REL_PATH}/source/f360_populate_track_properties.cpp
    ${REL_PATH}/source/f360_prioritize_clusters.cpp
    ${REL_PATH}/source/f360_test_stationary_hypothesis.cpp
    ${REL_PATH}/source/f360_post_estimate_cloud_only_init_countermeasure.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
