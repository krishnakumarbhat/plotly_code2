set(REL_PATH Fusion360/track_downselection)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_calc_trk_ttc.h
    ${REL_PATH}/include/f360_track_downselection.h
    ${REL_PATH}/include/f360_track_downselection_internal_functions.h
    ${REL_PATH}/source/f360_calc_trk_ttc.cpp
    ${REL_PATH}/source/f360_track_downselection.cpp
    ${REL_PATH}/source/f360_track_downselection_internal_functions.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
