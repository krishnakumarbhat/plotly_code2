set(REL_PATH Fusion360/cv_trailer_estimator)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_cvt_calc_trailer_speed.h
    ${REL_PATH}/include/f360_cvt_determine_trailer_type.h
    ${REL_PATH}/include/f360_cvt_discrete_derivative.h
    ${REL_PATH}/include/f360_cvt_estimate_trailer_length.h
    ${REL_PATH}/include/f360_cvt_estimator.h
    ${REL_PATH}/include/f360_cvt_generate_measurement.h
    ${REL_PATH}/include/f360_cvt_one_link_ekf.h
    ${REL_PATH}/include/f360_cvt_runner.h
    ${REL_PATH}/include/f360_cvt_two_link_ekf.h
    ${REL_PATH}/include/f360_cvt_types.h
    ${REL_PATH}/include/f360_cvt_saturated_odometer.h

    ${REL_PATH}/source/f360_cvt_calc_trailer_speed.cpp
    ${REL_PATH}/source/f360_cvt_determine_trailer_type.cpp
    ${REL_PATH}/source/f360_cvt_discrete_derivative.cpp
    ${REL_PATH}/source/f360_cvt_estimate_trailer_length.cpp
    ${REL_PATH}/source/f360_cvt_estimator.cpp
    ${REL_PATH}/source/f360_cvt_generate_measurement.cpp
    ${REL_PATH}/source/f360_cvt_one_link_ekf.cpp
    ${REL_PATH}/source/f360_cvt_runner.cpp
    ${REL_PATH}/source/f360_cvt_two_link_ekf.cpp
    ${REL_PATH}/source/f360_cvt_saturated_odometer.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
