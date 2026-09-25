set(REL_PATH Fusion360/cluster_grouping)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_cluster_grouping.h
    ${REL_PATH}/include/f360_coarse_cluster_gate.h
    ${REL_PATH}/include/f360_fine_cluster_gate.h
    ${REL_PATH}/include/f360_get_unique_rdot_interval_ids.h
    ${REL_PATH}/include/f360_is_pos_dist_inside_ellipse.h
    ${REL_PATH}/include/f360_is_two_look_type_ok_combine.h
    ${REL_PATH}/include/f360_merge_two_clusters.h
    ${REL_PATH}/include/f360_try_to_dealiase_rdots_in_two_clusters.h
    ${REL_PATH}/include/f360_try_to_dealiase_rdots_in_two_clusters_support_functions.h
    ${REL_PATH}/include/f360_update_cluster_state.h
    ${REL_PATH}/source/f360_cluster_grouping.cpp
    ${REL_PATH}/source/f360_coarse_cluster_gate.cpp
    ${REL_PATH}/source/f360_fine_cluster_gate.cpp
    ${REL_PATH}/source/f360_get_unique_rdot_interval_ids.cpp
    ${REL_PATH}/source/f360_is_pos_dist_inside_ellipse.cpp
    ${REL_PATH}/source/f360_is_two_look_type_ok_combine.cpp
    ${REL_PATH}/source/f360_merge_two_clusters.cpp
    ${REL_PATH}/source/f360_try_to_dealiase_rdots_in_two_clusters.cpp
    ${REL_PATH}/source/f360_try_to_dealiase_rdots_in_two_clusters_support_functions.cpp
    ${REL_PATH}/source/f360_update_cluster_state.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
