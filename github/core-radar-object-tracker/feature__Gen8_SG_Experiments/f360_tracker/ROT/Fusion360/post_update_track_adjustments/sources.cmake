set(REL_PATH Fusion360/post_update_track_adjustments)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_adjust_fltr_type_dependent_params.h
    ${REL_PATH}/include/f360_adjust_overlapping_confirmed_tracks.h
    ${REL_PATH}/include/f360_calc_heading_from_pos_diff.h
    ${REL_PATH}/include/f360_calc_obj_processed_size.h
    ${REL_PATH}/include/f360_calc_obj_size.h
    ${REL_PATH}/include/f360_cancel_new_updated_trk_overlapping_confirmed_trks.h
    ${REL_PATH}/include/f360_is_init_trk_bbox_overlapped_with_trusted_trk.h
    ${REL_PATH}/include/f360_object_track_management.h
    ${REL_PATH}/include/f360_object_track_management_internals.h
    ${REL_PATH}/include/f360_post_update_track_adjustments.h
    ${REL_PATH}/include/f360_update_object_average_rcs.h
    ${REL_PATH}/include/f360_update_object_maximum_rcs.h
    ${REL_PATH}/include/f360_update_object_track_properties.h
    ${REL_PATH}/include/f360_calc_obj_height.h
    ${REL_PATH}/include/f360_identify_objects_that_are_immediatly_behind_another_object.h
    ${REL_PATH}/source/f360_adjust_fltr_type_dependent_params.cpp
    ${REL_PATH}/source/f360_adjust_overlapping_confirmed_tracks.cpp
    ${REL_PATH}/source/f360_calc_heading_from_pos_diff.cpp
    ${REL_PATH}/source/f360_calc_obj_processed_size.cpp
    ${REL_PATH}/source/f360_calc_obj_size.cpp
    ${REL_PATH}/source/f360_cancel_new_updated_trk_overlapping_confirmed_trks.cpp
    ${REL_PATH}/source/f360_is_init_trk_bbox_overlapped_with_trusted_trk.cpp
    ${REL_PATH}/source/f360_object_track_management.cpp
    ${REL_PATH}/source/f360_object_track_management_internals.cpp
    ${REL_PATH}/source/f360_post_update_track_adjustments.cpp
    ${REL_PATH}/source/f360_update_object_average_rcs.cpp
    ${REL_PATH}/source/f360_update_object_maximum_rcs.cpp
    ${REL_PATH}/source/f360_update_object_track_properties.cpp
    ${REL_PATH}/source/f360_calc_obj_height.cpp
    ${REL_PATH}/source/f360_identify_objects_that_are_immediatly_behind_another_object.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
