
/**
 * @file ta_input_boundary_check.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the input range checks for TA.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/**************************************************
 * Includes
 ***************************************************/

#include "ta_input_boundary_check.h" // IWYU pragma: keep
#include "assert.h"                  // IWYU pragma: keep
#include "fbk_macros.h"              // IWYU pragma: keep

/**************************************************
 * Defines
 ***************************************************/

/* Array sizes */

/* Range values */
#define TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL (0.0f)
#define TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL (5000.0f)
#define TA_FTA_OBJ_OFFSET_X_NEGATIVE_MIN_VAL (-5000.0f)
#define TA_FTA_OBJ_OFFSET_X_NEGATIVE_MAX_VAL (0.0f)
#define TA_FTA_OBJ_OFFSET_Y_POSITIVE_MIN_VAL (0.0f)
#define TA_FTA_OBJ_OFFSET_Y_POSITIVE_MAX_VAL (1000.0f)
#define TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MIN_VAL (-1000.0f)
#define TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MAX_VAL (0.0f)
#define TA_F_RTA_ENABLE_MAX_VAL (1u)
#define TA_F_FTA_ENABLE_MAX_VAL (1u)
#define TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL (20u)
#define TA_FTA_STEERING_ANGLE_MAX_RIGHT_MAX_VAL (20u)

/**************************************************
 * Local function prototypes
 ***************************************************/

/**
 * @brief Checks whether inputs of module Ta_Bmw_Input are within their specified boundaries
 *
 * @return True when containing inputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all inputs of module Ta_Bmw_Input are within their specified boundaries.}
 **/
static boolean_T Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(const Ta_Input_T *p_input /**< Ta input*/);

/**
 * @brief Checks whether inputs component Ta_Bmw_Input_Debug_Mode of dimension 1 are within their specified boundaries
 *
 * @return True when containing inputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional inputs of Ta_Bmw_Input_Debug_Mode are within their specified boundaries.}
 **/
static boolean_T Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(const Ta_Input_T *p_input /**< Ta input*/);

/**
 * @brief Checks whether inputs component Ta_Bmw_Input_Enable_Flags of dimension 1 are within their specified boundaries
 *
 * @return True when containing inputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional inputs of Ta_Bmw_Input_Enable_Flags are within their specified boundaries.}
 **/
static boolean_T Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(const Ta_Input_T *p_input /**< Ta input*/);

/**
 * @brief Checks whether inputs component Ta_Bmw_Input_Steering_Angle_Thresholds of dimension 1 are within their specified
 *boundaries
 *
 * @return True when containing inputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional inputs of Ta_Bmw_Input_Steering_Angle_Thresholds are within their specified
 *boundaries.}
 **/
static boolean_T
Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(const Ta_Input_T *p_input /**< Ta input*/);

/**************************************************
 * Global function definition
 ***************************************************/

boolean_T Ta_Are_Inputs_In_Boundary(const Ta_Input_T *p_input)
{
   boolean_T f_ta_input_in_boundaries;

   assert(NULL != p_input);

   f_ta_input_in_boundaries = Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(p_input);

   return f_ta_input_in_boundaries;
}


/**************************************************
 * Local function definition
 ***************************************************/

static boolean_T Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(const Ta_Input_T *p_input)
{
   boolean_T f_ta_input_in_boundaries;

   assert(NULL != p_input);

   f_ta_input_in_boundaries = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   f_ta_input_in_boundaries = (boolean_T) (Fbk_Is_True(f_ta_input_in_boundaries)
                                           && Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input));
   f_ta_input_in_boundaries =
      (boolean_T) (Fbk_Is_True(f_ta_input_in_boundaries)
                   && Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input));

   return f_ta_input_in_boundaries;
}

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(const Ta_Input_T *p_input)
{
   boolean_T f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds;

   assert(NULL != p_input);

   f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds =
      (boolean_T) (((TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL <= p_input->fta_obj_offset_x_positive)
                    && (p_input->fta_obj_offset_x_positive <= TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL)));
   f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds)
                   && ((TA_FTA_OBJ_OFFSET_X_NEGATIVE_MIN_VAL <= p_input->fta_obj_offset_x_negative)
                       && (p_input->fta_obj_offset_x_negative <= TA_FTA_OBJ_OFFSET_X_NEGATIVE_MAX_VAL)));
   f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds)
                   && ((TA_FTA_OBJ_OFFSET_Y_POSITIVE_MIN_VAL <= p_input->fta_obj_offset_y_positive)
                       && (p_input->fta_obj_offset_y_positive <= TA_FTA_OBJ_OFFSET_Y_POSITIVE_MAX_VAL)));
   f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds)
                   && ((TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MIN_VAL <= p_input->fta_obj_offset_y_negative)
                       && (p_input->fta_obj_offset_y_negative <= TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MAX_VAL)));

   return f_ta_bmw_input_ta_bmw_input_debug_mode_1_inputs_in_bounds;
}


/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(const Ta_Input_T *p_input)
{
   boolean_T f_ta_bmw_input_ta_bmw_input_enable_flags_1_inputs_in_bounds;

   assert(NULL != p_input);

   f_ta_bmw_input_ta_bmw_input_enable_flags_1_inputs_in_bounds = (boolean_T) ((p_input->f_rta_enable <= TA_F_RTA_ENABLE_MAX_VAL));
   f_ta_bmw_input_ta_bmw_input_enable_flags_1_inputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_bmw_input_ta_bmw_input_enable_flags_1_inputs_in_bounds)
                   && (p_input->f_fta_enable <= TA_F_FTA_ENABLE_MAX_VAL));

   return f_ta_bmw_input_ta_bmw_input_enable_flags_1_inputs_in_bounds;
}


/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(const Ta_Input_T *p_input)
{
   boolean_T f_ta_bmw_input_ta_bmw_input_steering_angle_thresholds_1_inputs_in_bounds;

   assert(NULL != p_input);

   f_ta_bmw_input_ta_bmw_input_steering_angle_thresholds_1_inputs_in_bounds =
      (boolean_T) ((p_input->fta_steering_angle_max_left <= TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL));
   f_ta_bmw_input_ta_bmw_input_steering_angle_thresholds_1_inputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_bmw_input_ta_bmw_input_steering_angle_thresholds_1_inputs_in_bounds)
                   && (p_input->fta_steering_angle_max_right <= TA_FTA_STEERING_ANGLE_MAX_RIGHT_MAX_VAL));

   return f_ta_bmw_input_ta_bmw_input_steering_angle_thresholds_1_inputs_in_bounds;
}
