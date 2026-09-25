set(REL_PATH Fusion360/passenger_trailer_estimator)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_trailer_detector_constants.h
    ${REL_PATH}/include/f360_trailer_detector_core.h
    ${REL_PATH}/include/f360_trailer_detector_flt_fus_output.h
    ${REL_PATH}/include/f360_trailer_detector_outputs.h
    ${REL_PATH}/include/f360_trailer_detector_TA.h
    ${REL_PATH}/include/f360_trailer_detector_TL.h
    ${REL_PATH}/include/f360_trailer_detector_TL_SVM.h
    ${REL_PATH}/include/f360_trailer_detector_TP.h
    ${REL_PATH}/include/f360_trailer_detector_TW.h

    ${REL_PATH}/source/f360_ta_run.cpp
    ${REL_PATH}/source/f360_tl_run.cpp
    ${REL_PATH}/source/f360_tp_run.cpp
    ${REL_PATH}/source/f360_trailer_detector_core.cpp
    ${REL_PATH}/source/f360_tw_run.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
