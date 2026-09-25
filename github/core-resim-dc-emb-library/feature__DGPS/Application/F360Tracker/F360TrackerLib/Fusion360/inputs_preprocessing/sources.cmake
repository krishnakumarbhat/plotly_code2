set(REL_PATH Fusion360/inputs_preprocessing)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_coordinate_transformation.h
    ${REL_PATH}/include/f360_detection_props_update.h
    ${REL_PATH}/include/f360_host_props_update.h
    ${REL_PATH}/include/f360_input_validation.h
    ${REL_PATH}/include/f360_inputs_preprocessing.h
    ${REL_PATH}/include/f360_mark_detection_with_low_detection_conf.h
    ${REL_PATH}/include/f360_motion_status_classification.h
    ${REL_PATH}/include/f360_range_rate_compensation.h
    ${REL_PATH}/include/f360_sensor_capability.h
    ${REL_PATH}/include/f360_sensor_props_update.h
    ${REL_PATH}/include/f360_uncertainty_calculation.h
    ${REL_PATH}/source/f360_coordinate_transformation.cpp
    ${REL_PATH}/source/f360_detection_props_update.cpp
    ${REL_PATH}/source/f360_host_props_update.cpp
    ${REL_PATH}/source/f360_input_validation.cpp
    ${REL_PATH}/source/f360_inputs_preprocessing.cpp
    ${REL_PATH}/source/f360_mark_detection_with_low_detection_conf.cpp
    ${REL_PATH}/source/f360_motion_status_classification.cpp
    ${REL_PATH}/source/f360_range_rate_compensation.cpp
    ${REL_PATH}/source/f360_sensor_capability.cpp
    ${REL_PATH}/source/f360_sensor_props_update.cpp
    ${REL_PATH}/source/f360_uncertainty_calculation.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
