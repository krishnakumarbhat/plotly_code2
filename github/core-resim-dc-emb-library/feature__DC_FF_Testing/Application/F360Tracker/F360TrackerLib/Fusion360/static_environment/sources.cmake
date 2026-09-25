set(REL_PATH Fusion360/static_environment)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_downselect_longi_stat_clusters.h
    ${REL_PATH}/include/f360_longi_stat_cluster.h
    ${REL_PATH}/include/f360_longi_stat_curve_init.h
    ${REL_PATH}/include/f360_post_process_longi_stat_clusters.h
    ${REL_PATH}/include/f360_static_env_polys_support_functions.h
    ${REL_PATH}/include/f360_update_longi_stat_curves.h
    ${REL_PATH}/include/f360_cluster_objects_for_lsc.h
    ${REL_PATH}/include/f360_concrete_wall_estimator.h
    ${REL_PATH}/source/f360_cluster_objects_for_lsc.cpp
    ${REL_PATH}/source/f360_concrete_wall_estimator.cpp
    ${REL_PATH}/source/f360_downselect_longi_stat_clusters.cpp
    ${REL_PATH}/source/f360_longi_stat_cluster.cpp
    ${REL_PATH}/source/f360_longi_stat_curve_init.cpp
    ${REL_PATH}/source/f360_post_process_longi_stat_clusters.cpp
    ${REL_PATH}/source/f360_static_env_helpers.cpp
    ${REL_PATH}/source/f360_static_env_polys_support_functions.cpp
    ${REL_PATH}/source/f360_update_longi_stat_curves.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
