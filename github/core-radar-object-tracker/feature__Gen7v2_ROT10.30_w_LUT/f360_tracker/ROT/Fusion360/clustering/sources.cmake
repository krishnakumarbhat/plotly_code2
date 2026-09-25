set(REL_PATH Fusion360/clustering)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_clustering.h
    ${REL_PATH}/include/f360_clustering_detections.h
    ${REL_PATH}/include/f360_dbscan.h
    ${REL_PATH}/include/f360_find_and_prioritize_detections.h
    ${REL_PATH}/include/f360_get_new_cluster_id.h
    ${REL_PATH}/include/f360_initialize_clusters.h
    ${REL_PATH}/source/f360_clustering.cpp
    ${REL_PATH}/source/f360_clustering_detections.cpp
    ${REL_PATH}/source/f360_dbscan.cpp
    ${REL_PATH}/source/f360_find_and_prioritize_detections.cpp
    ${REL_PATH}/source/f360_get_new_cluster_id.cpp
    ${REL_PATH}/source/f360_initialize_clusters.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
