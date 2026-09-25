set(REL_PATH Fusion360/track_validity)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_detect_multipath.h
    ${REL_PATH}/include/f360_determine_reflected_obj.h
    ${REL_PATH}/include/f360_is_host_reflected_track.h
    ${REL_PATH}/include/f360_is_host_reflected_track_helpers.h
    ${REL_PATH}/include/f360_is_reflective_guardrail_track.h
    ${REL_PATH}/include/f360_overall_confidence.h
    ${REL_PATH}/include/f360_overall_confidence_helpers.h
    ${REL_PATH}/include/f360_track_validity.h
    ${REL_PATH}/include/f360_update_exist_prob.h
    ${REL_PATH}/include/f360_update_exist_prob_helpers.h
    ${REL_PATH}/include/f360_update_object_confidence_levels.h
    ${REL_PATH}/include/f360_update_object_confidence_levels_helpers.h
    ${REL_PATH}/include/f360_identify_fast_approach_ghost_behind_host.h
    ${REL_PATH}/include/f360_update_aeb_confidence.h
    ${REL_PATH}/include/f360_update_aeb_confidence_helpers.h
    ${REL_PATH}/source/f360_detect_multipath.cpp
    ${REL_PATH}/source/f360_determine_reflected_obj.cpp
    ${REL_PATH}/source/f360_is_host_reflected_track.cpp
    ${REL_PATH}/source/f360_is_host_reflected_track_helpers.cpp
    ${REL_PATH}/source/f360_is_reflective_guardrail_track.cpp
    ${REL_PATH}/source/f360_overall_confidence.cpp
    ${REL_PATH}/source/f360_overall_confidence_helpers.cpp
    ${REL_PATH}/source/f360_track_validity.cpp
    ${REL_PATH}/source/f360_update_exist_prob.cpp
    ${REL_PATH}/source/f360_update_exist_prob_helpers.cpp
    ${REL_PATH}/source/f360_update_object_confidence_levels.cpp
    ${REL_PATH}/source/f360_update_object_confidence_levels_helpers.cpp
    ${REL_PATH}/source/f360_identify_fast_approach_ghost_behind_host.cpp
    ${REL_PATH}/source/f360_update_aeb_confidence.cpp
    ${REL_PATH}/source/f360_update_aeb_confidence_helpers.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
