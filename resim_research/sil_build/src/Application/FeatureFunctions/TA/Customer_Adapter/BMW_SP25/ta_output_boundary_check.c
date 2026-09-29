
/**
 * @file ta_output_boundary_check.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the output range checks for TA.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/**************************************************
 * Includes
 ***************************************************/

#include "ta_output_boundary_check.h" // IWYU pragma: keep
#include "assert.h"                   // IWYU pragma: keep
#include "fbk_macros.h"               // IWYU pragma: keep

/**************************************************
 * Defines
 ***************************************************/

/* Array sizes */

/* Range values */
#define TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL (0.0f)
#define TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL (10.0f)
#define TA_FTA_TARGET_GAP_MIN_VAL (0.0f)
#define TA_FTA_TARGET_GAP_MAX_VAL (60.0f)
#define TA_FTA_TTC_MIN_VAL (0.0f)
#define TA_FTA_TTC_MAX_VAL (100.0f)
#define TA_FTA_ALERT_LEVEL_MAX_VAL (4u)
#define TA_FTA_SYMBOL_REQUEST_MAX_VAL (4u)
#define TA_FTA_BRAKE_THRESHOLD_REDUCTION_MAX_VAL (3u)
#define TA_FTA_BRAKE_CONDITIONING_MAX_VAL (4u)
#define TA_FTA_MANEUVER_DIRECTION_MAX_VAL (1u)
#define TA_F_DIAGNOSTIC_MODE_MIN_VAL ((boolean_T) 0u)
#define TA_F_DIAGNOSTIC_MODE_MAX_VAL ((boolean_T) 1u)
#define TA_CURRENT_DECELERATION_ESTIMATE_MIN_VAL (0.0f)
#define TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL (10.0f)
#define TA_F_FTA_ENABLE_MAX_VAL (1u)
#define TA_F_RTA_ENABLE_MAX_VAL (1u)
#define TA_F_RTA_ENABLE_TURNING_AREA_MAX_VAL (1u)
#define TA_F_RTA_ENABLE_DYNAMIC_AREA_MAX_VAL (1u)
#define TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL (25u)
#define TA_RTA_TURNING_AREA_STATUS_MAX_VAL (25u)
#define TA_RTA_ALERT_LEFT_MAX_VAL (4u)
#define TA_RTA_ALERT_RIGHT_MAX_VAL (4u)

/**************************************************
 * Local function prototypes
 ***************************************************/

/**
 * @brief Checks whether outputs of module Ta_Output_Bmw are within their specified boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all outputs of module Ta_Output_Bmw are within their specified boundaries.}
 **/
static boolean_T Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(const Ta_Output_T *p_output /**< Ta output*/);

/**
 * @brief Checks whether outputs component Ta_Fta_Output of dimension 1 are within their specified boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional outputs of Ta_Fta_Output are within their specified boundaries.}
 **/
static boolean_T Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output /**< Ta output*/);

/**
 * @brief Checks whether outputs component Ta_Output of dimension 1 are within their specified boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional outputs of Ta_Output are within their specified boundaries.}
 **/
static boolean_T Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output /**< Ta output*/);

/**
 * @brief Checks whether outputs component Ta_Output_Enable_Flags of dimension 1 are within their specified boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional outputs of Ta_Output_Enable_Flags are within their specified boundaries.}
 **/
static boolean_T Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output /**< Ta output*/);

/**
 * @brief Checks whether outputs component Ta_Rta_Output of dimension 1 are within their specified boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a test with all 1 dimensional outputs of Ta_Rta_Output are within their specified boundaries.}
 **/
static boolean_T Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output /**< Ta output*/);

/**************************************************
 * Global function definition
 ***************************************************/

boolean_T Ta_Are_Outputs_In_Boundary(const Ta_Output_T *p_output)
{
   boolean_T f_ta_output_in_boundaries;

   assert(NULL != p_output);

   f_ta_output_in_boundaries = Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(p_output);

   return f_ta_output_in_boundaries;
}


/**************************************************
 * Local function definition
 ***************************************************/

static boolean_T Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(const Ta_Output_T *p_output)
{
   boolean_T f_ta_output_in_boundaries;

   assert(NULL != p_output);

   f_ta_output_in_boundaries = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   f_ta_output_in_boundaries =
      (boolean_T) (Fbk_Is_True(f_ta_output_in_boundaries) && Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output));
   f_ta_output_in_boundaries = (boolean_T) (Fbk_Is_True(f_ta_output_in_boundaries)
                                            && Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output));
   f_ta_output_in_boundaries =
      (boolean_T) (Fbk_Is_True(f_ta_output_in_boundaries) && Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output));

   return f_ta_output_in_boundaries;
}

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output)
{
   boolean_T f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds;

   assert(NULL != p_output);

   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (((TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL <= p_output->fta_brake_deceleration_request)
                    && (p_output->fta_brake_deceleration_request <= TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL)));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds = (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                                                                    && ((TA_FTA_TARGET_GAP_MIN_VAL <= p_output->fta_target_gap)
                                                                        && (p_output->fta_target_gap <= TA_FTA_TARGET_GAP_MAX_VAL)));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                   && ((TA_FTA_TTC_MIN_VAL <= p_output->fta_ttc) && (p_output->fta_ttc <= TA_FTA_TTC_MAX_VAL)));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds = (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                                                                    && (p_output->fta_alert_level <= TA_FTA_ALERT_LEVEL_MAX_VAL));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                   && (p_output->fta_symbol_request <= TA_FTA_SYMBOL_REQUEST_MAX_VAL));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                   && (p_output->fta_brake_threshold_reduction <= TA_FTA_BRAKE_THRESHOLD_REDUCTION_MAX_VAL));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                   && (p_output->fta_brake_conditioning <= TA_FTA_BRAKE_CONDITIONING_MAX_VAL));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                   && (p_output->fta_maneuver_direction <= TA_FTA_MANEUVER_DIRECTION_MAX_VAL));
   f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds)
                   && ((TA_F_DIAGNOSTIC_MODE_MIN_VAL == p_output->f_diagnostic_mode)
                       || (p_output->f_diagnostic_mode == TA_F_DIAGNOSTIC_MODE_MAX_VAL)));

   return f_ta_output_bmw_ta_fta_output_1_outputs_in_bounds;
}


/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output)
{
   boolean_T f_ta_output_bmw_ta_output_1_outputs_in_bounds;

   assert(NULL != p_output);

   f_ta_output_bmw_ta_output_1_outputs_in_bounds =
      (boolean_T) (((TA_CURRENT_DECELERATION_ESTIMATE_MIN_VAL <= p_output->ta_current_deceleration_estimate)
                    && (p_output->ta_current_deceleration_estimate <= TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL)));

   return f_ta_output_bmw_ta_output_1_outputs_in_bounds;
}


/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output)
{
   boolean_T f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds;

   assert(NULL != p_output);

   f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds = (boolean_T) ((p_output->f_fta_enable <= TA_F_FTA_ENABLE_MAX_VAL));
   f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds)
                   && (p_output->f_rta_enable <= TA_F_RTA_ENABLE_MAX_VAL));
   f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds)
                   && (p_output->f_rta_enable_turning_area <= TA_F_RTA_ENABLE_TURNING_AREA_MAX_VAL));
   f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds)
                   && (p_output->f_rta_enable_dynamic_area <= TA_F_RTA_ENABLE_DYNAMIC_AREA_MAX_VAL));

   return f_ta_output_bmw_ta_output_enable_flags_1_outputs_in_bounds;
}


/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
static boolean_T Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(const Ta_Output_T *p_output)
{
   boolean_T f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds;

   assert(NULL != p_output);

   f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds =
      (boolean_T) ((p_output->rta_dynamic_area_status <= TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL));
   f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds =
      (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds)
                   && (p_output->rta_turning_area_status <= TA_RTA_TURNING_AREA_STATUS_MAX_VAL));
   f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds = (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds)
                                                                    && (p_output->rta_alert_left <= TA_RTA_ALERT_LEFT_MAX_VAL));
   f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds = (boolean_T) (Fbk_Is_True(f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds)
                                                                    && (p_output->rta_alert_right <= TA_RTA_ALERT_RIGHT_MAX_VAL));

   return f_ta_output_bmw_ta_rta_output_1_outputs_in_bounds;
}
