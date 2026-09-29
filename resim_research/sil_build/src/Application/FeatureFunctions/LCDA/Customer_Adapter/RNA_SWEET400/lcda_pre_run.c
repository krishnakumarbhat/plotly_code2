/**
 * @file lcda_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the RNA_SWEET400 pre run logic for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_pre_run.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_types.h"
#include "ml_interval.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>
#include <string.h>

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/
static void Lcda_Get_Initial_Bsw_Zone_Points(const Pa_Data_T *p_pa_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone_hys);

static void Lcda_Get_Initial_Cvw_Zone_Points(const Pa_Data_T *p_pa_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_cvw_zone,
                                             Fbk_Field_Of_Interest_T *initial_cvw_zone_hys);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Input(Lcda_Input_T *p_lcda_input)
{
   assert(NULL != p_lcda_input);
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memset function since it is not required.] */
   memset(p_lcda_input, 0, sizeof(Lcda_Input_T));
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Pre_Run_Init(Lcda_Instance_T *p_lcda_instance /**< Lcda input */)
{
   /* Assert */
   assert(NULL != p_lcda_instance);
}

/* clang-format off */
void Lcda_Pre_Run(Lcda_Instance_T *p_lcda_instance /**< Lcda core input */,
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_input" points to a non-constant type.] */
                  const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                  const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   Lcda_Core_Input_T *p_core_input;
   const Pa_Data_T *p_pa_data;
   const Lcda_Core_Calibration_T *p_cals;

   assert(p_lcda_instance);
   assert(p_lcda_input);
   assert(p_fbk_output);

   p_core_input            = &p_lcda_instance->core_input;
   p_core_input->p_pa_data = p_fbk_output->p_pa_data;
   p_pa_data               = p_fbk_output->p_pa_data;
   p_cals                  = &p_lcda_instance->calibration;

   /* Populate the core input */
   p_core_input->enabled_flags.f_lcda_enabled = FBK_TRUE;
   p_core_input->enabled_flags.f_bsw_enabled  = FBK_TRUE;
   p_core_input->enabled_flags.f_cvw_enabled  = FBK_TRUE;

   p_core_input->enabled_flags.f_dropback_enabled = FBK_FALSE;
   p_core_input->enabled_flags.f_fallback_enabled = FBK_TRUE;

   /* Set the initial zone points. The core will use these as the initial zone points if bsw_zone_calculation_mode =
    * BSW_ZONE_CALC_FIXED_INPUT */
   p_core_input->bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   Lcda_Get_Initial_Bsw_Zone_Points(p_pa_data, p_cals, &(p_core_input->initial_bsw_zone), &(p_core_input->initial_bsw_zone_hys));
   Lcda_Get_Initial_Cvw_Zone_Points(p_pa_data, p_cals, &(p_core_input->initial_cvw_zone), &(p_core_input->initial_cvw_zone_hys));


   /* Set lane width from camera info */
   p_core_input->lane_width         = p_pa_data->vehicle_data.lane_width;
   p_core_input->lane_center_offset = p_pa_data->vehicle_data.lane_center_offset;

   p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc;
   p_core_input->warn_settings.cvw_rel_vel_range = Create_Float_Range(p_cals->k_cvw_min_object_curvi_relative_speed[FBK_ZERO_UINT],
                                                                      p_cals->k_cvw_max_object_curvi_relative_speed);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Get_Initial_Bsw_Zone_Points(const Pa_Data_T *p_pa_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone_hys)
{
   float32_T host_length        = p_pa_data->vehicle_data.host_length;
   float32_T half_host_width    = p_pa_data->vehicle_data.host_width / 2.0f;
   float32_T half_short_dynzone = (p_cals->k_bsw_x_length + p_cals->k_bsw_dynzone_range[0]) / 2.0f; /* middle of short BSW zone */

   initial_bsw_zone->size = LCDA_NUMBER_OF_ZONE_POINTS;

   initial_bsw_zone->points[0].x = p_cals->k_bsw_x0 - host_length;
   initial_bsw_zone->points[2].x = initial_bsw_zone->points[0].x + p_cals->k_bsw_x_length;
   initial_bsw_zone->points[1].x = initial_bsw_zone->points[0].x + half_short_dynzone; /* any chosen point | here: in the middle of
                                                                                      the shortest zone */
   initial_bsw_zone->points[3].x = initial_bsw_zone->points[2].x;
   initial_bsw_zone->points[4].x = initial_bsw_zone->points[1].x;
   initial_bsw_zone->points[5].x = initial_bsw_zone->points[0].x;

   initial_bsw_zone->points[5].y = p_cals->k_bsw_y0 + half_host_width;
   initial_bsw_zone->points[4].y = initial_bsw_zone->points[5].y;
   initial_bsw_zone->points[3].y = initial_bsw_zone->points[5].y;
   initial_bsw_zone->points[0].y = initial_bsw_zone->points[5].y + p_cals->k_bsw_y_width; /* all parameters with minus because of
                                                                                         the symmetrical reflection of y axis in
                                                                                         Renault CS */
   initial_bsw_zone->points[1].y = initial_bsw_zone->points[0].y;
   initial_bsw_zone->points[2].y = initial_bsw_zone->points[0].y;

   initial_bsw_zone_hys->size = LCDA_NUMBER_OF_ZONE_POINTS;

   initial_bsw_zone_hys->points[0].x = initial_bsw_zone->points[0].x + p_cals->k_bsw_x0_hys;
   initial_bsw_zone_hys->points[1].x = initial_bsw_zone->points[1].x;
   initial_bsw_zone_hys->points[2].x = initial_bsw_zone->points[2].x - p_cals->k_bsw_x1_hys;
   initial_bsw_zone_hys->points[3].x = initial_bsw_zone_hys->points[2].x;
   initial_bsw_zone_hys->points[4].x = initial_bsw_zone_hys->points[1].x;
   initial_bsw_zone_hys->points[5].x = initial_bsw_zone_hys->points[0].x;

   initial_bsw_zone_hys->points[0].y = initial_bsw_zone->points[0].y + p_cals->k_bsw_y1_hys; /* all parameters with minus because
                                                                                           of the symmetrical reflection of y axis
                                                                                           in Renault CS */
   initial_bsw_zone_hys->points[1].y = initial_bsw_zone_hys->points[0].y;
   initial_bsw_zone_hys->points[2].y = initial_bsw_zone_hys->points[0].y;
   initial_bsw_zone_hys->points[3].y = initial_bsw_zone->points[3].y - p_cals->k_bsw_y0_hys;
   initial_bsw_zone_hys->points[4].y = initial_bsw_zone_hys->points[3].y;
   initial_bsw_zone_hys->points[5].y = initial_bsw_zone_hys->points[3].y;
}


static void Lcda_Get_Initial_Cvw_Zone_Points(const Pa_Data_T *p_pa_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_cvw_zone,
                                             Fbk_Field_Of_Interest_T *initial_cvw_zone_hys)
{
   float32_T host_length     = p_pa_data->vehicle_data.host_length;
   float32_T half_host_width = p_pa_data->vehicle_data.host_width / 2.0f;

   initial_cvw_zone->size = LCDA_NUMBER_OF_ZONE_POINTS;

   initial_cvw_zone->points[0].x = p_cals->k_cvw_x0 - host_length;
   initial_cvw_zone->points[1].x = initial_cvw_zone->points[0].x + p_cals->k_cvw_x_length0;
   initial_cvw_zone->points[2].x = initial_cvw_zone->points[1].x + p_cals->k_cvw_x_length1;
   initial_cvw_zone->points[3].x = initial_cvw_zone->points[2].x;
   initial_cvw_zone->points[4].x = initial_cvw_zone->points[1].x;
   initial_cvw_zone->points[5].x = initial_cvw_zone->points[0].x;

   initial_cvw_zone->points[5].y = p_cals->k_cvw_y0 + half_host_width;
   initial_cvw_zone->points[4].y = initial_cvw_zone->points[5].y;
   initial_cvw_zone->points[3].y = p_cals->k_cvw_y1 + half_host_width;
   initial_cvw_zone->points[0].y = initial_cvw_zone->points[5].y + p_cals->k_cvw_y_width0;
   initial_cvw_zone->points[1].y = initial_cvw_zone->points[0].y;
   initial_cvw_zone->points[2].y = initial_cvw_zone->points[3].y + p_cals->k_cvw_y_width1;

   initial_cvw_zone_hys->size = LCDA_NUMBER_OF_ZONE_POINTS;

   initial_cvw_zone_hys->points[0].x = initial_cvw_zone->points[0].x;
   initial_cvw_zone_hys->points[1].x = initial_cvw_zone->points[1].x;
   initial_cvw_zone_hys->points[2].x = initial_cvw_zone->points[2].x;
   initial_cvw_zone_hys->points[3].x = initial_cvw_zone->points[3].x;
   initial_cvw_zone_hys->points[4].x = initial_cvw_zone->points[4].x;
   initial_cvw_zone_hys->points[5].x = initial_cvw_zone->points[5].x;

   initial_cvw_zone_hys->points[0].y = initial_cvw_zone->points[0].y + p_cals->k_cvw_zone_y_hys[0];
   initial_cvw_zone_hys->points[1].y = initial_cvw_zone->points[1].y + p_cals->k_cvw_zone_y_hys[1];
   initial_cvw_zone_hys->points[2].y = initial_cvw_zone->points[2].y + p_cals->k_cvw_zone_y_hys[2];
   initial_cvw_zone_hys->points[3].y = initial_cvw_zone->points[3].y - p_cals->k_cvw_zone_y_hys[3];
   initial_cvw_zone_hys->points[4].y = initial_cvw_zone->points[4].y - p_cals->k_cvw_zone_y_hys[4];
   initial_cvw_zone_hys->points[5].y = initial_cvw_zone->points[5].y - p_cals->k_cvw_zone_y_hys[5];
}
