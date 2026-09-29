set(REL_PATH Fusion360/passenger_trailer_estimator)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_trailer_detector_flt_fus_output.h
    ${REL_PATH}/include/f360_pvtrailer_angle_estimation.h
    ${REL_PATH}/include/f360_pvtrailer_length_estimation.h
    ${REL_PATH}/include/f360_pvtrailer_width_estimation.h
    ${REL_PATH}/include/f360_pvtrailer_data.h
    ${REL_PATH}/include/f360_pvtrailer_main.h

    ${REL_PATH}/source/f360_pvtrailer_angle_estimation.cpp
    ${REL_PATH}/source/f360_pvtrailer_length_estimation.cpp
    ${REL_PATH}/source/f360_pvtrailer_width_estimation.cpp
    ${REL_PATH}/source/f360_pvtrailer_main.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
