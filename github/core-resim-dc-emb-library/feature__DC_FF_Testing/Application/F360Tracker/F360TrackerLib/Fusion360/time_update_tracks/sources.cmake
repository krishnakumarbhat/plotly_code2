set(REL_PATH Fusion360/time_update_tracks)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_kill_coasted_tracks.h
    ${REL_PATH}/include/f360_object_list_timestamp_update.h
    ${REL_PATH}/include/f360_predict_exist_prob.h
    ${REL_PATH}/include/f360_priority_update_tracks.h
    ${REL_PATH}/include/f360_time_update_object_tracks_CCA.h
    ${REL_PATH}/include/f360_time_update_object_tracks_CTCA.h
    ${REL_PATH}/include/f360_time_update_tracks.h
    ${REL_PATH}/include/f360_calculate_jacobian_CTCA.h
    ${REL_PATH}/source/f360_object_list_timestamp_update.cpp
    ${REL_PATH}/source/f360_predict_exist_prob.cpp
    ${REL_PATH}/source/f360_priority_update_tracks.cpp
    ${REL_PATH}/source/f360_time_update_object_tracks_CCA.cpp
    ${REL_PATH}/source/f360_time_update_object_tracks_CTCA.cpp
    ${REL_PATH}/source/f360_time_update_tracks.cpp
    ${REL_PATH}/source/f360_calculate_jacobian_CTCA.cpp
    ${REL_PATH}/source/f360_kill_coasted_tracks.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
