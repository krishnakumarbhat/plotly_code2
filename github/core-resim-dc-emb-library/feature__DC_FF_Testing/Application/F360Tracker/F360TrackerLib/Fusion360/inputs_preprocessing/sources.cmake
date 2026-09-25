set(REL_PATH Fusion360/inputs_preprocessing)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_inputs_preprocessing.h
    ${REL_PATH}/include/f360_host_props_update.h
    ${REL_PATH}/include/f360_sensor_capability.h
    ${REL_PATH}/source/f360_host_props_update.cpp
    ${REL_PATH}/source/f360_inputs_preprocessing.cpp
    ${REL_PATH}/source/f360_sensor_capability.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
