/**
 * @file lcda_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
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
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/**
 * @brief Create the initial fixed zones for BSW and CVW submodules.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-122793}
 * @verification{Check proper construction of cvw and bsw initial zones.}
 */
static void Lcda_Create_Initial_Zones(Lcda_Core_Input_T *p_core_input /**< LCDA Core Input */,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data /**< Vehicle data */,
                                      const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Input(Lcda_Input_T *p_lcda_input)
{
   assert(NULL != p_lcda_input);
   /* Set flags to defaults */
   p_lcda_input->f_lcda_enable     = FBK_FALSE;
   p_lcda_input->f_bsw_enable      = FBK_FALSE;
   p_lcda_input->f_cvw_enable      = FBK_FALSE;
   p_lcda_input->f_slc_enable      = FBK_FALSE;
   p_lcda_input->f_elc_enable      = FBK_FALSE;
   p_lcda_input->f_dropback_enable = FBK_FALSE;
   p_lcda_input->f_fallback_enable = FBK_FALSE;
   p_lcda_input->hmi_cvw_dyn_ttc   = CVW_DYN_TTC_DISABLED;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Pre_Run_Init(Lcda_Instance_T *p_lcda_instance)
{
   /* Assert */
   assert(NULL != p_lcda_instance);
}

/* clang-format off */
/* coverity[misra_c_2012_rule_8_13_violation]  */
/* coverity[misra_c_2012_rule_2_7_violation]  */
void Lcda_Pre_Run(Lcda_Instance_T *p_lcda_instance /**< Lcda core input */,
                  const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                  const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   Lcda_Core_Input_T *p_core_input;
   const Lcda_Core_Calibration_T *p_cals;

   /* Asserts */
   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_fbk_output);

   /* Fill vehicle data */
   p_vehicle_data          = &p_fbk_output->p_pa_data->vehicle_data;
   p_cals                  = &p_lcda_instance->calibration;
   p_core_input            = &p_lcda_instance->core_input;
   p_core_input->p_pa_data = p_fbk_output->p_pa_data;

   /* Populate the core input */
   p_core_input->enabled_flags.f_lcda_enabled     = p_lcda_input->f_lcda_enable;
   p_core_input->enabled_flags.f_bsw_enabled      = p_lcda_input->f_bsw_enable;
   p_core_input->enabled_flags.f_cvw_enabled      = p_lcda_input->f_cvw_enable;
   p_core_input->enabled_flags.f_slc_enabled      = p_lcda_input->f_slc_enable;
   p_core_input->enabled_flags.f_elc_enabled      = p_lcda_input->f_elc_enable;
   p_core_input->enabled_flags.f_dropback_enabled = p_lcda_input->f_dropback_enable;
   p_core_input->enabled_flags.f_fallback_enabled = p_lcda_input->f_fallback_enable;

   p_core_input->warn_settings.cvw_ttc_speed_factor = FBK_ZERO_F;
   p_core_input->warn_settings.cvw_ttc_threshold    = p_cals->k_cvw_ttc;
   p_core_input->warn_settings.slc_ttc_thres_lon    = p_cals->k_slc_critical_lon_ttc;
   p_core_input->warn_settings.slc_ttc_thres_lat    = p_cals->k_slc_critical_lat_ttc;
   p_core_input->warn_settings.cvw_rel_vel_range = Create_Float_Range(p_cals->k_cvw_min_object_curvi_relative_speed[FBK_ZERO_UINT],
                                                                      p_cals->k_cvw_max_object_curvi_relative_speed);

   /* Three TTC threshold based on customization input signal */
   if (CVW_DYN_TTC_DISABLED != p_lcda_input->hmi_cvw_dyn_ttc)
   {
      p_core_input->warn_settings.cvw_ttc_threshold =
         p_cals->k_lcda_cvw_ttc_const[((uint8_t) p_lcda_input->hmi_cvw_dyn_ttc) - FBK_ONE_UINT];
      p_core_input->warn_settings.cvw_ttc_speed_factor =
         (FBK_ONE_F / (2.0f * p_cals->k_lcda_cvw_ttc_accel[((uint8_t) p_lcda_input->hmi_cvw_dyn_ttc) - FBK_ONE_UINT]));
   }

   /* Set lane width */
   p_core_input->lane_width         = p_vehicle_data->lane_width;
   p_core_input->lane_center_offset = p_vehicle_data->lane_center_offset;


   /* Create specific zones */
   Lcda_Create_Initial_Zones(p_core_input, p_vehicle_data, p_cals);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Create_Initial_Zones(Lcda_Core_Input_T *p_core_input,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t j;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_cals);


   /**
    * BSW zone is in shape of rectangle stretched between two points on its diagonal. Front ego side and rear outer side.
    *
    * front ego(x,y)
    *         0-------1
    *         |       |
    *         |       |
    *         |       |
    *         |       |
    *         |       |
    *         3-------2
    *               rear outer(x,y)
    */

   /* BSW zone */
   p_core_input->bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   p_core_input->initial_bsw_zone.size     = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_bsw_zone.points[FRONT_EGO_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_bsw_zone_front_ego_side_x, p_cals->k_bsw_zone_front_ego_side_y);
   p_core_input->initial_bsw_zone.points[FRONT_OUTER_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_bsw_zone_front_ego_side_x, p_cals->k_bsw_zone_rear_outer_side_y);
   p_core_input->initial_bsw_zone.points[MIDDLE_OUTER_SIDE] = Create_2d_Vector_Coordinates(
      Fbk_Half(p_cals->k_bsw_zone_front_ego_side_x + p_cals->k_bsw_zone_rear_outer_side_x), p_cals->k_bsw_zone_rear_outer_side_y);
   p_core_input->initial_bsw_zone.points[REAR_OUTER_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_bsw_zone_rear_outer_side_x, p_cals->k_bsw_zone_rear_outer_side_y);
   p_core_input->initial_bsw_zone.points[REAR_EGO_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_bsw_zone_rear_outer_side_x, p_cals->k_bsw_zone_front_ego_side_y);
   p_core_input->initial_bsw_zone.points[MIDDLE_EGO_SIDE] = Create_2d_Vector_Coordinates(
      Fbk_Half(p_cals->k_bsw_zone_front_ego_side_x + p_cals->k_bsw_zone_rear_outer_side_x), p_cals->k_bsw_zone_front_ego_side_y);

   /* BSW hysteresis zone */
   p_core_input->initial_bsw_zone_hys = p_core_input->initial_bsw_zone;
   p_core_input->initial_bsw_zone_hys.points[FRONT_EGO_SIDE].x += p_cals->k_bsw_zone_front_ego_side_x_hys;
   p_core_input->initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x += p_cals->k_bsw_zone_front_ego_side_x_hys;
   p_core_input->initial_bsw_zone_hys.points[REAR_OUTER_SIDE].x -= p_cals->k_bsw_zone_rear_outer_side_x_hys;
   p_core_input->initial_bsw_zone_hys.points[REAR_EGO_SIDE].x -= p_cals->k_bsw_zone_rear_outer_side_x_hys;

   p_core_input->initial_bsw_zone_hys.points[FRONT_EGO_SIDE].y -= p_cals->k_bsw_zone_front_ego_side_y_hys;
   p_core_input->initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].y += p_cals->k_bsw_zone_rear_outer_side_y_hys;
   p_core_input->initial_bsw_zone_hys.points[MIDDLE_OUTER_SIDE].y += p_cals->k_bsw_zone_rear_outer_side_y_hys;
   p_core_input->initial_bsw_zone_hys.points[REAR_OUTER_SIDE].y += p_cals->k_bsw_zone_rear_outer_side_y_hys;
   p_core_input->initial_bsw_zone_hys.points[REAR_EGO_SIDE].y -= p_cals->k_bsw_zone_front_ego_side_y_hys;
   p_core_input->initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y -= p_cals->k_bsw_zone_front_ego_side_y_hys;

   /* Add lane center offset to BSW zone points, except FRONT_EGO_SIDE point */
   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      p_core_input->initial_bsw_zone.points[j].y += p_core_input->lane_center_offset;
      p_core_input->initial_bsw_zone_hys.points[j].y += p_core_input->lane_center_offset;
   }

   /* CVW zone */
   p_core_input->initial_cvw_zone.size = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_cvw_zone.points[FRONT_OUTER_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_cvw_x0 - p_vehicle_data->host_length, p_cals->k_cvw_y0 + p_cals->k_cvw_y_width0);
   p_core_input->initial_cvw_zone.points[MIDDLE_OUTER_SIDE] = Create_2d_Vector_Coordinates(
      p_cals->k_cvw_x0 - p_vehicle_data->host_length + p_cals->k_cvw_x_length0, p_cals->k_cvw_y0 + p_cals->k_cvw_y_width0);
   p_core_input->initial_cvw_zone.points[REAR_OUTER_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_cvw_x0 - p_vehicle_data->host_length + p_cals->k_cvw_x_length0 + p_cals->k_cvw_x_length1,
                                   p_cals->k_cvw_y1 + p_cals->k_cvw_y_width1);
   p_core_input->initial_cvw_zone.points[REAR_EGO_SIDE] = Create_2d_Vector_Coordinates(
      p_cals->k_cvw_x0 - p_vehicle_data->host_length + p_cals->k_cvw_x_length0 + p_cals->k_cvw_x_length1, p_cals->k_cvw_y1);
   p_core_input->initial_cvw_zone.points[MIDDLE_EGO_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_cvw_x0 - p_vehicle_data->host_length + p_cals->k_cvw_x_length0, p_cals->k_cvw_y0);
   p_core_input->initial_cvw_zone.points[FRONT_EGO_SIDE] =
      Create_2d_Vector_Coordinates(p_cals->k_cvw_x0 - p_vehicle_data->host_length, p_cals->k_cvw_y0);
   /* CVW hysteresis zone */
   p_core_input->initial_cvw_zone_hys = p_core_input->initial_cvw_zone;
   p_core_input->initial_cvw_zone_hys.points[FRONT_OUTER_SIDE].y += p_cals->k_cvw_zone_y_hys[0] * p_cals->k_cvw_y_width0;
   p_core_input->initial_cvw_zone_hys.points[MIDDLE_OUTER_SIDE].y += p_cals->k_cvw_zone_y_hys[1] * p_cals->k_cvw_y_width0;
   p_core_input->initial_cvw_zone_hys.points[REAR_OUTER_SIDE].y += p_cals->k_cvw_zone_y_hys[2] * p_cals->k_cvw_y_width1;
   p_core_input->initial_cvw_zone_hys.points[REAR_EGO_SIDE].y -= p_cals->k_cvw_zone_y_hys[3] * p_cals->k_cvw_y_width1;
   p_core_input->initial_cvw_zone_hys.points[MIDDLE_EGO_SIDE].y -= p_cals->k_cvw_zone_y_hys[4] * p_cals->k_cvw_y_width0;
   p_core_input->initial_cvw_zone_hys.points[FRONT_EGO_SIDE].y -= p_cals->k_cvw_zone_y_hys[5] * p_cals->k_cvw_y_width0;
}
