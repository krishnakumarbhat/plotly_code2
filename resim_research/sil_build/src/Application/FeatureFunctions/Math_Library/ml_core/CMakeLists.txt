# Set required environmental variable
cmake_minimum_required(VERSION 3.3)

if(Shared_Toolbox_USE_SPE2)
  message(FATAL_ERROR "Shared_Toolbox_USE_SPE2 does not exist anymore, use MLMathLibrary_USE_SPE2 instead")
endif()
# Add for SPE2
OPTION(MLMathLibrary_USE_SPE2 "Use SPE2 Codes in vector2d toolbox. SPE2 codes only work for NXP processors having the SPE2 extension." "OFF")

# Available options for the math tables
SET(MLMathLibrary_fast_math_table_content
  choose
  use_table
  use_runtime_table
  use_function
  )

# Display an error message in case the CACHE STRING variables for the
# math tables are not set
function(math_table_error_message var_name)
   get_directory_property(st_parent_directory PARENT_DIRECTORY)
   message(FATAL_ERROR
      " | ${var_name} must be set (CACHE STRING) to either use_table or use_function.\n"
      " | See the file ${CMAKE_CURRENT_LIST_DIR}/fast_math.md fore more information. \n"
      " | Also the file ${CMAKE_CURRENT_LIST_DIR}/readme.md should be helpful.\n"
      " | You basically need to do the following:\n"
      " | In the file \n"
      " |    '${st_parent_directory}/CMakeLists.txt' \n"
      " | locate the add_subdirectory()  command that adds the MathLibrary to the \n"
      " | project. Before that line add \n"
      " |     set(${var_name} \"choose\" CACHE STRING \"\")\n"
      " | with 'choose' being replaced with either 'use_table' or 'use_function'"
      )
endfunction()

# Backwards compatibility: Use old option as default and warn
set(MLMathLibrary_fast_math_exp_table_default "choose")
if(Shared_Toolbox_fast_math_exp_table)
  message(WARNING "Shared_Toolbox_fast_math_exp_table is deprecated, use MLMathLibrary_fast_math_exp_table instead")
  set(MLMathLibrary_fast_math_exp_table_default ${Shared_Toolbox_fast_math_exp_table})
endif()
# The MathLibrary has some look up tables that speed up mathematical computations.
# While speeding up the computation they also use a substantial amount of memory, both in ROM and RAM.
# Therefore the user of the MathLibrary can choose if these tables shall be present in the build.
# Since this is a decision that needs to be made consistent in software in the loop and embedded builds
# the MathLibrary enforces the decision to be made.
# If MLMathLibrary_fast_math_exp_table has not been set prior to the add_subdirectory() command that adds
# the MathLibrary cmake will stop configuring.
# Before the call to add_subdirectory() do the following:
# set(MLMathLibrary_fast_math_exp_table "choose" CACHE STRING "Shall the MathLibrary Fast_Exp macro use an exp table?")
# with "choose" being replaced with either "use_table" or "use_function"
set(MLMathLibrary_fast_math_exp_table ${MLMathLibrary_fast_math_exp_table_default} CACHE STRING "Shall the MathLibrary Fast_Exp macro use an exp table?")
set_property(CACHE MLMathLibrary_fast_math_exp_table PROPERTY STRINGS ${MLMathLibrary_fast_math_table_content})
if(${MLMathLibrary_fast_math_exp_table} STREQUAL "choose")
  math_table_error_message(MLMathLibrary_fast_math_exp_table)
endif()

# Backwards compatibility: Use old option as default and warn
set(MLMathLibrary_fast_math_trig_table_default "choose")
if(Shared_Toolbox_fast_math_trig_table)
  message(WARNING "Shared_Toolbox_fast_math_trig_table is deprecated, use MLMathLibrary_fast_math_trig_table instead")
  set(MLMathLibrary_fast_math_trig_table_default ${Shared_Toolbox_fast_math_trig_table})
endif()
# The MathLibrary has some look up tables that speed up mathematical computations.
# While speeding up the computation they also use a substantial amount of memory, both in ROM and RAM.
# Therefore the user of the MathLibrary can choose if these tables shall be present in the build.
# Since this is a decision that needs to be made consistent in software in the loop and embedded builds
# the MathLibrary enforces the decision to be made.
# If MLMathLibrary_fast_math_trig_table has not been set prior to the add_subdirectory() command that adds
# the MathLibrary cmake will stop configuring.
# Before the call to add_subdirectory() do the following:
# set(MLMathLibrary_fast_math_trig_table "choose" CACHE STRING "Shall the MathLibrary Fast_Sin, Fast_Cos,.. macro use precomputed trigonometric function tables?")
# with "choose" being replaced with either "use_table" or "use_function"
set(MLMathLibrary_fast_math_trig_table ${MLMathLibrary_fast_math_trig_table_default} CACHE STRING "Shall the MathLibrary Fast_Sin, Fast_Cos,.. macro use precomputed trigonometric function tables?")
set(MLMathLibrary_fast_math_trig_table ${MLMathLibrary_fast_math_trig_table_default} CACHE STRING "Shall the MathLibrary Fast_Sin, Fast_Cos,.. macro use precomputed trigonometric function tables?")
set_property(CACHE MLMathLibrary_fast_math_trig_table PROPERTY STRINGS ${MLMathLibrary_fast_math_table_content})
if(${MLMathLibrary_fast_math_trig_table} STREQUAL "choose")
  math_table_error_message(MLMathLibrary_fast_math_trig_table)
endif()

add_library(MLMathLibrary "")

#backwards compatibility: Have a target with the old name aliasing the new name
add_library(Shared_Toolbox ALIAS MLMathLibrary)

set_property(TARGET MLMathLibrary PROPERTY C_STANDARD 90)

if(${MLMathLibrary_fast_math_exp_table} STREQUAL "use_table")
  target_compile_definitions(MLMathLibrary PUBLIC ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE=ML_MATH_LIBRARY_FAST_MATH_USE_TABLE)
elseif(${MLMathLibrary_fast_math_exp_table} STREQUAL "use_function")
  target_compile_definitions(MLMathLibrary PUBLIC ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE=ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION)
else()
  target_compile_definitions(MLMathLibrary PUBLIC ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE=ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE)
endif()

if(${MLMathLibrary_fast_math_trig_table} STREQUAL "use_table")
  target_compile_definitions(MLMathLibrary PUBLIC ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE=ML_MATH_LIBRARY_FAST_MATH_USE_TABLE)
elseif(${MLMathLibrary_fast_math_trig_table} STREQUAL "use_function")
  target_compile_definitions(MLMathLibrary PUBLIC ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE=ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION)
else()
  target_compile_definitions(MLMathLibrary PUBLIC ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE=ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE)
endif()

if(Shared_Toolbox_offer_legacy_headers)
  message(WARNING "Shared_Toolbox_offer_legacy_headers is deprecated, use MLMathLibrary_offer_legacy_headers instead")
endif()
OPTION(MLMathLibrary_offer_legacy_headers "Headers from before the naming convention was followed are present" ON)
if(MLMathLibrary_offer_legacy_headers)
  target_include_directories(MLMathLibrary PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/legacy)

  target_sources(MLMathLibrary
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Angle_Range.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Assert_Macros.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Basic_Macros.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Basic_Math.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Basic_Math_Factory.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Basic_Math_Structs.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Basic_Saturated_Math.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Compiler_Warning_Macros.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Geometric_2d_Factory.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Geometric_2d_Functions.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Geometric_2d_Structs.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/LookupTable.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Math_Infinity.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Math_Macros.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Math_Selector.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Moving_Average.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Runtime_Parameters.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/SHARED_TOOLBOX_VERSION_CONTENT.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/SHARED_TOOLBOX_VERSION_DATE.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Shared_Toolbox_All.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Shared_Toolbox_All_Structs.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Shared_Toolbox_Version_Check.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Sieve.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/VERSION_NUMBER_T.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Vector_2d.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Vector_2d_Algebra.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/Vector_2d_Factory.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/fastMath.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/line_hesse.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/line_hesse_type.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/math_infinity_silent.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/matrix.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/matrix_2x2_type.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/matrix_3x3_type.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/shared_toolbox_content.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/shared_toolbox_serial_buffer_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/shared_toolbox_serialization_error_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/shared_toolbox_version.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_angle.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_angle_range.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_angle_range_fuse_state_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_angle_range_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_angle_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_bool.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_checked_rounding.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_checksum.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_compiler_warning.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_exp.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_fast_math_table_macros.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_float_range_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_int_range_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_interval.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line_hesse.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line_hesse_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line_parameter.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line_parameter_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line_segment.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_line_segment_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_lookup_table_2d.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_macros.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_math.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_math_infinity.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_math_infinity_silent.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_matrix.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_matrix_2x2_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_matrix_3x3_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_max_sieve_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_min_max_sieve_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_min_sieve_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_moving_average.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_moving_average_filter_instance_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_overlapping_angle_range_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_polygon.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_boolean_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_error_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_float32_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_int16_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_int32_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_int8_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_legal_state_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_uint16_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_uint32_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameter_uint8_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_runtime_parameters.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_saturated_math.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_set_trigonometric_table.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_sieve.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_trigonometry.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_unit_conversion.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_vector_2d.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_vector_2d_angle.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_vector_2d_t.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_version_number_check.h
      ${CMAKE_CURRENT_LIST_DIR}/include/legacy/st_version_number_t.h
  )
endif()

target_include_directories(MLMathLibrary PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(MLMathLibrary
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_angle.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_angle_range.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_angle_range_fuse_state_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_angle_range_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_angle_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_bit_manipulation.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_bool.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_checked_rounding.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_checksum.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_compiler_warning.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_evaluate_normal_distribution.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_exp.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_fast_math_table_macros.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_float_range_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_int_range_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_interval.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line_hesse.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line_hesse_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line_parameter.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line_parameter_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line_segment.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_line_segment_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_lookup_table_2d.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_macros.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_math.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_math_infinity.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_math_infinity_silent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_matrix.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_matrix_2x2_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_matrix_3x3_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_max_sieve_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_min_max_sieve_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_min_sieve_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_moving_average.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_moving_average_filter_instance_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_overlapping_angle_range_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_polygon.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_boolean_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_error_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_float32_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_int16_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_int32_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_int8_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_legal_state_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_uint16_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_uint32_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameter_uint8_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_runtime_parameters.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_saturated_math.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_serial_buffer_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_serialization_error_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_set_trigonometric_table.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_sieve.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_trigonometry.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_unit_conversion.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_vector_2d.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_vector_2d_angle.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_vector_2d_t.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_version.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_version_number_check.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ml_version_number_t.h
)

target_sources(MLMathLibrary
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_angle.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_angle_assertion_helper.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_angle_range.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_atan_tbl_predef.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_bit_manipulation.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_checked_rounding.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_checksum.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_cos_tbl_predef.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_evaluate_normal_distribution.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_exp.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_exp_tbl_predef.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_interval.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_line.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_line_hesse.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_line_parameter.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_line_segment.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_lookup_table_2d.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_math.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_matrix.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_moving_average.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_polygon.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_runtime_parameters.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_saturated_math.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_set_trigonometric_table.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_sieve.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_trig_tbl_stream_s32r274_windriver.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_trigonometry.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_vector_2d.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_vector_2d_angle.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_vector_2d_assertion_helper.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_vector_2d_spe.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_vector_3d.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_version.c
    ${CMAKE_CURRENT_LIST_DIR}/readme.md
    ${CMAKE_CURRENT_LIST_DIR}/fast_math.md
)
if(${MLMathLibrary_fast_math_trig_table} STREQUAL "use_runtime_table" OR ${MLMathLibrary_fast_math_exp_table} STREQUAL "use_runtime_table")
   target_sources(MLMathLibrary
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/src/ml_serialization.h
      ${CMAKE_CURRENT_LIST_DIR}/src/ml_serialization.c
   )
endif()
if(MLMathLibrary_offer_legacy_headers)
   target_sources(MLMathLibrary
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/src/legacy/st_legacy.c)
endif()

OPTION(MLMathLibrary_SharedDevelopmentTools_ACTIVE "Generate SharedDevelopmentTools target" OFF)
IF(MLMathLibrary_SharedDevelopmentTools_ACTIVE)
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Development_tools "${CMAKE_CURRENT_BINARY_DIR}/Development_tools")
ENDIF()

get_target_property(MLMathLibrary_Sources MLMathLibrary SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MLMathLibrary_Sources})
