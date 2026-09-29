set(REL_PATH Fusion360/track_grouping)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_are_two_objects_preconditions_ok_for_merge.h
    ${REL_PATH}/include/f360_calculate_merged_object_dimensions.h
    ${REL_PATH}/include/f360_compute_split_logic_signals.h
    ${REL_PATH}/include/f360_convert_bbox_vcs_to_tcs.h
    ${REL_PATH}/include/f360_merge_bbox_overlap_test.h
    ${REL_PATH}/include/f360_move_dets_from_killed_to_kept_object.h
    ${REL_PATH}/include/f360_split_objects_in_orth_direction.h
    ${REL_PATH}/include/f360_track_grouping.h
    ${REL_PATH}/include/f360_try_to_merge_two_objects.h
    ${REL_PATH}/include/f360_update_merged_objects_properties.h
    ${REL_PATH}/include/f360_verify_object_size.h
    ${REL_PATH}/source/f360_are_two_objects_preconditions_ok_for_merge.cpp
    ${REL_PATH}/source/f360_calculate_merged_object_dimensions.cpp
    ${REL_PATH}/source/f360_compute_split_logic_signals.cpp
    ${REL_PATH}/source/f360_convert_bbox_vcs_to_tcs.cpp
    ${REL_PATH}/source/f360_merge_bbox_overlap_test.cpp
    ${REL_PATH}/source/f360_move_dets_from_killed_to_kept_object.cpp
    ${REL_PATH}/source/f360_split_objects_in_orth_direction.cpp
    ${REL_PATH}/source/f360_track_grouping.cpp
    ${REL_PATH}/source/f360_try_to_merge_two_objects.cpp
    ${REL_PATH}/source/f360_update_merged_objects_properties.cpp
    ${REL_PATH}/source/f360_verify_object_size.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
