set(REL_PATH Fusion360/multipath_detector)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_multipath_detector.h
    ${REL_PATH}/include/f360_reflector_object.h
    ${REL_PATH}/include/f360_reflector_selector.h
    ${REL_PATH}/source/f360_multipath_detector.cpp
    ${REL_PATH}/source/f360_reflector_object.cpp
    ${REL_PATH}/source/f360_reflector_selector.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
