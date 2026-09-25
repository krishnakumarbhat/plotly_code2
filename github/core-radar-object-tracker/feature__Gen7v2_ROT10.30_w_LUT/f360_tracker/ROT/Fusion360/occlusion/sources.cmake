set(REL_PATH Fusion360/occlusion)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_sensor_occlusion_info_helpers.h
    ${REL_PATH}/include/f360_update_by_tracks_helpers.h
    ${REL_PATH}/include/f360_get_scs_visible_edges.h
    ${REL_PATH}/include/f360_get_vcs_visible_edges.h
    ${REL_PATH}/include/f360_occlusion.h
    ${REL_PATH}/include/f360_occlusion_sector.h
    ${REL_PATH}/include/f360_occlusion_types.h
    ${REL_PATH}/include/f360_sensor_fov.h
    ${REL_PATH}/include/f360_sensor_occlusion_info.h
    ${REL_PATH}/include/f360_calc_point_scs_position.h
    ${REL_PATH}/source/f360_get_scs_visible_edges.cpp
    ${REL_PATH}/source/f360_calc_point_scs_position.cpp
    ${REL_PATH}/source/f360_get_vcs_visible_edges.cpp
    ${REL_PATH}/source/f360_occlusion.cpp
    ${REL_PATH}/source/f360_occlusion_sector.cpp
    ${REL_PATH}/source/f360_sensor_fov.cpp
    ${REL_PATH}/source/f360_sensor_occlusion_info.cpp
    ${REL_PATH}/source/f360_sensor_occlusion_info_helpers.cpp
    ${REL_PATH}/source/f360_update_by_tracks_helpers.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
