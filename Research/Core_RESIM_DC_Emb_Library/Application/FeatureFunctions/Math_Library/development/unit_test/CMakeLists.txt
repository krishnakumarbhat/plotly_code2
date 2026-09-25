# Generate library
add_executable(MLMathLibraryUnitTests "")

set_property(TARGET MLMathLibraryUnitTests PROPERTY CXX_STANDARD 14)

target_include_directories(MLMathLibraryUnitTests
    PRIVATE
        Basic_Test_Classes
        Test_Classes
        unit_test_helpers
        Source
)

target_sources(MLMathLibraryUnitTests
    PRIVATE
        main.cpp
        Basic_Test_Classes/Basic_Vectors.hpp
        unit_test_helpers/as_type.hpp
        unit_test_helpers/st_vector_2d_helper.hpp
        unit_test_helpers/st_float_range_helper.hpp
        unit_test_helpers/st_int_range_helper.hpp
        unit_test_helpers/custom_cmp_helper.hpp
        Source/ml_angle_hpp_test.cpp
        Source/ml_bit_manipulation_test.cpp
        Source/ml_trigonometry_triangle.cpp
        Source/ml_evaluate_normal_distribution_test.cpp
        Source/ml_vector_2d_angle_hpp_test.cpp
        Source/ml_vector_2d_hpp_test.cpp
        Source/ml_vector_3d_hpp_test.cpp
        Source/ml_vector_3d_test.cpp
        Source/Basic_Constants.cpp
        Source/st_version_number_check_test.cpp
        Source/reuse_header_test.cpp
        Source/st_set_trigonometric_table_test.cpp
        Source/st_checksum_test.cpp
        Source/shared_toolbox_version_test.cpp
        Source/st_angle_range_test.cpp
        Source/st_angle_test.cpp
        Source/st_checked_rounding_test.cpp
        Source/st_exp_test.cpp
        Source/st_interval_test.cpp
        Source/st_line_hesse_test.cpp
        Source/st_line_parameter_test.cpp
        Source/st_line_segment_test.cpp
        Source/st_line_test.cpp
        Source/st_lookup_table_2d_test.cpp
        Source/st_macros_test.cpp
        Source/st_math_test.cpp
        Source/st_matrix_test.cpp
        Source/st_moving_average_test.cpp
        Source/st_polygon_test.cpp
        Source/st_runtime_parameters_test.cpp
        Source/st_saturated_math_test.cpp
        Source/st_sieve_test.cpp
        Source/st_trigonometry_test.cpp
        Source/st_vector_2d_test.cpp
        Source/st_vector_2d_angle_test.cpp
        Source/fast_math_serialization_fixture.hpp)

IF(TARGET SharedDevelopmentTools)
    target_sources(MLMathLibraryUnitTests
        PRIVATE
            Source/Timing_Test.cpp
            Source/Timing_Demo/Target_Timing.c
            Source/Timing_Demo/Target_Timing.h)
ENDIF()

if(MLMathLibrary_offer_legacy_headers)
  target_sources(MLMathLibraryUnitTests
        PRIVATE
            Source/legacy/st_legacy_test.cpp)
endif()



add_definitions( -D_VARIADIC_MAX=10 )

get_target_property(MLMathLibraryUnitTests_Sources MLMathLibraryUnitTests SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MLMathLibraryUnitTests_Sources})

# Set properties
set_target_properties(MLMathLibraryUnitTests PROPERTIES
    FOLDER "Libraries"
)

target_link_libraries(MLMathLibraryUnitTests MLMathLibrary gtest)

IF(TARGET SharedDevelopmentTools)
    target_link_libraries(MLMathLibraryUnitTests SharedDevelopmentTools)
ENDIF()

#include(AS_bullseye)
IF(COMMAND AS_bullseye)
    AS_bullseye(
        INCLUDE_TARGETS
            MLMathLibrary
        EXCLUDE_TARGETS
            MLMathLibraryUnitTests
        COVSELECT_PATH "${CMAKE_CURRENT_BINARY_DIR}/bullseye.txt"
    )
ENDIF()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    # Compiling using clang, try to find clang-tidy
    find_program(MLMathLibrary_CLANG_TIDY_EXE
            NAMES "clang-tidy"
            DOC "Path to clang-tidy executable" )
    if(MLMathLibrary_CLANG_TIDY_EXE)
        # clang tidy is available, offer to use it
        option(MLMathLibrary_CLANG_TIDY "Run clang tidy on all MathLibrary targets" OFF)
    endif()
endif()
if(MLMathLibrary_CLANG_TIDY)
    if(NOT MLMathLibrary_CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else()
        set(DO_CLANG_TIDY "${MLMathLibrary_CLANG_TIDY_EXE}")
    endif()
endif()

 if(MLMathLibrary_CLANG_TIDY_EXE)
    set_target_properties(MLMathLibrary PROPERTIES C_CLANG_TIDY "${DO_CLANG_TIDY}")
    set_target_properties(MLMathLibraryUnitTests PROPERTIES C_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

add_test(
    NAME MLMathLibraryUnitTests
    COMMAND MLMathLibraryUnitTests
    --gtest_output=xml:MLMathLibraryUnitTests.xml
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_compile_options(MLMathLibraryUnitTests PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4 /wd4214>)
