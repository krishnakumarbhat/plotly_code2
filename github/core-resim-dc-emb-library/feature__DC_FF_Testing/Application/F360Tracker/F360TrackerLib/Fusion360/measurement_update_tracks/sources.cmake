set(REL_PATH Fusion360/measurement_update_tracks)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_measurement_update_tracks.h
    ${REL_PATH}/include/f360_msmt_update_support_functions_ctca.h
    ${REL_PATH}/include/f360_msmt_update_support_functions_cca_moveable.h
    ${REL_PATH}/include/f360_msmt_update_support_functions_cca_non_moveable.h
    ${REL_PATH}/include/f360_msmt_update_obj_trks_ctca.h
    ${REL_PATH}/include/f360_msmt_update_obj_trks_cca_moveable.h
    ${REL_PATH}/include/f360_msmt_update_obj_trks_cca_non_moveable.h
    ${REL_PATH}/include/f360_msmt_update_object_timestamp.h
    ${REL_PATH}/include/f360_msmt_update_support_functions_common.h
    ${REL_PATH}/include/f360_pseudo_estimations.h
    ${REL_PATH}/include/f360_pseudo_heading_estimation.h
    ${REL_PATH}/include/f360_pseudo_position_estimation.h
    ${REL_PATH}/include/f360_regularize_trk_hdg_spd.h
    ${REL_PATH}/source/f360_measurement_update_tracks.cpp
    ${REL_PATH}/source/f360_msmt_update_support_functions_ctca.cpp
    ${REL_PATH}/source/f360_msmt_update_support_functions_cca_moveable.cpp
    ${REL_PATH}/source/f360_msmt_update_support_functions_cca_non_moveable.cpp
    ${REL_PATH}/source/f360_msmt_update_obj_trks_ctca.cpp
    ${REL_PATH}/source/f360_msmt_update_obj_trks_cca_moveable.cpp
    ${REL_PATH}/source/f360_msmt_update_obj_trks_cca_non_moveable.cpp
    ${REL_PATH}/source/f360_msmt_update_object_timestamp.cpp
    ${REL_PATH}/source/f360_msmt_update_support_functions_common.cpp
    ${REL_PATH}/source/f360_pseudo_estimations.cpp
    ${REL_PATH}/source/f360_pseudo_heading_estimation.cpp
    ${REL_PATH}/source/f360_pseudo_position_estimation.cpp
    ${REL_PATH}/source/f360_regularize_trk_hdg_spd.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
