include(AS_create_QAC_project OPTIONAL)

if(COMMAND AS_create_QAC_project)
    SET(MLMathLibrary_QAC_output_folder "${CMAKE_CURRENT_BINARY_DIR}/QAC" CACHE PATH "Path to the QAC output folder for MLMathLibrary")

    SET(MLMathLibrary_QAC_suppressed_checks "0292;" CACHE STRING "Suppressed QAC checks")
    message(STATUS CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR})
    # Make sure QAC has the same macro definitions as the code
    get_target_property(MLMathLibrary_Compile_Definitions MLMathLibrary COMPILE_DEFINITIONS)
    AS_create_QAC_project(
        TARGET MLMathLibrary
        ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../ml_core"
        OUTPUT_PATH "${MLMathLibrary_QAC_output_folder}"
        SUPPRESSED_CHECKS ${MLMathLibrary_QAC_suppressed_checks}
        WARNING_FORMAT "standard"
        ADDITIONAL_PREPROCESSOR_MACROS ${MLMathLibrary_Compile_Definitions} FAST_ABSF_MACRO
        )
endif()

