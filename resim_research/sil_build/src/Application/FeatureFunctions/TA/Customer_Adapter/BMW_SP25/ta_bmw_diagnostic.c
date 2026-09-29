/**
 * @file ta_bmw_diagnostic.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the BMW SRR5 diagnostic source file.
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ta_bmw_diagnostic.h"
#include "fbk_macros.h"
#include <assert.h>

#define TA_CM_TO_METER(x) ((x) *0.01f) /* Map cm-input value to meter-output value */

float32_T Ta_Get_Target_Shift_Offset_Long(const Ta_Input_T *p_ta_input)
{
   /* check input parameters */
   assert(NULL != p_ta_input);

   /* At least one of the input sets for one axis is always expected to be zero.
    * Shift the x offsets from BMW-VCS to APTIV-VCS
    * - X-Axis parameter set: fta_obj_offset_x_positive & fta_obj_offset_x_negative
    */
   return TA_CM_TO_METER((float32_T) (p_ta_input->fta_obj_offset_x_positive + p_ta_input->fta_obj_offset_x_negative));
}

float32_T Ta_Get_Target_Shift_Offset_Lat(const Ta_Input_T *p_ta_input)
{
   /* check input parameters */
   assert(NULL != p_ta_input);

   /* At least one of the input sets for one axis is always expected to be zero.
    * Shift the y offsets from BMW-VCS to APTIV-VCS (Flip y-Axis)
    * - Y-Axis parameter set: fta_obj_offset_y_positive & fta_obj_offset_y_negative
    */
   return -(TA_CM_TO_METER((float32_T) (p_ta_input->fta_obj_offset_y_positive + p_ta_input->fta_obj_offset_y_negative)));
}

boolean_T Ta_Is_Diagnostic_Mode_Enabled(const Ta_Input_T *p_ta_input, const Ta_Core_Calibration_T *p_ta_cals)
{
   boolean_T f_diagnostic_mode = FBK_FALSE;

   assert(NULL != p_ta_input);
   assert(NULL != p_ta_cals);

   /* Enable the tracker object shift when all of the following conditions are met:
    * - Activated by Application-Parameter
    * - PFGS is enabled
    * - Any of the Diagnostic-Job Inputs is != 0
    */
   if ((Fbk_Is_True(p_ta_cals->k_f_ta_enable_debug_mode)) && (Fbk_Is_True(p_ta_input->f_fta_enable))
       && ((FBK_ZERO_F != p_ta_input->fta_obj_offset_x_negative) || (FBK_ZERO_F != p_ta_input->fta_obj_offset_x_positive)
           || (FBK_ZERO_F != p_ta_input->fta_obj_offset_y_negative) || (FBK_ZERO_F != p_ta_input->fta_obj_offset_y_positive)))
   {
      f_diagnostic_mode = FBK_TRUE;
   }

   return f_diagnostic_mode;
}
