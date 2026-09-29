set(REL_PATH Fusion360/occlusion)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_occlusion_types.h
    ${REL_PATH}/include/f360_occlusion.h
    ${REL_PATH}/source/f360_occlusion.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
