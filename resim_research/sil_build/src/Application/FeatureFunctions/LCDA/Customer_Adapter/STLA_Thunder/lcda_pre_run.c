/**
 * @file lcda_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the STLA_Thunder pre run logic for LCDA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
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

/*===========================================================================*\
* Local Defines
\*===========================================================================*/


/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/**
 * @brief Create the initial fixed zones for BSW and CVW submodules according to PSTH-57846.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Thunder_Create_Initial_Zones(Lcda_Core_Input_T *p_core_input /**< LCDA Core Input */,
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
   p_lcda_input->f_lcda_enable_bsw = FBK_FALSE;
   p_lcda_input->f_lcda_enable_cvw = FBK_FALSE;
   p_lcda_input->f_trailer_present = FBK_FALSE;

   /* Set trailer input to default */
   p_lcda_input->f_trailer_present = FBK_FALSE;
   p_lcda_input->trailer_length    = FBK_ZERO_F;
   p_lcda_input->trailer_width     = FBK_ZERO_F;
   p_lcda_input->trailer_angle     = FBK_ZERO_F;
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
   const Pa_Data_T *p_pa_data;
   Lcda_Core_Input_T *p_core_input;
   const Lcda_Core_Calibration_T *p_cals;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_fbk_output);

   p_pa_data               = p_fbk_output->p_pa_data;
   p_core_input            = &p_lcda_instance->core_input;
   p_core_input->p_pa_data = p_pa_data;
   p_cals                  = &p_lcda_instance->calibration;
   p_vehicle_data          = &p_pa_data->vehicle_data;


   /* Create Thunder-specific zones */
   Lcda_Thunder_Create_Initial_Zones(p_core_input, p_cals);

   /* Populate the core input */
   p_core_input->enabled_flags.f_lcda_enabled = p_lcda_input->f_lcda_enable;
   p_core_input->enabled_flags.f_bsw_enabled  = p_lcda_input->f_lcda_enable_bsw;
   p_core_input->enabled_flags.f_cvw_enabled =
      (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_cvw) && Fbk_Is_False(p_lcda_input->f_trailer_present));
   p_core_input->enabled_flags.f_slc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_elc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_dropback_enabled = FBK_FALSE;
   p_core_input->enabled_flags.f_fallback_enabled = (boolean_T) (p_cals->k_lcda_f_enable_fallback_handler);

   p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc;

   /* Trailer information */
   p_core_input->trailer.f_trailer_present = p_lcda_input->f_trailer_present;
   p_core_input->trailer.length            = p_lcda_input->trailer_length;
   p_core_input->trailer.width             = p_lcda_input->trailer_width;
   p_core_input->trailer.angle             = p_lcda_input->trailer_angle;

   /* Set lane width from camera info */
   p_core_input->lane_width         = p_vehicle_data->lane_width;
   p_core_input->lane_center_offset = p_vehicle_data->lane_center_offset;

   p_core_input->warn_settings.cvw_rel_vel_range = Create_Float_Range(p_cals->k_cvw_min_object_curvi_relative_speed[FBK_ZERO_UINT],
                                                                      p_cals->k_cvw_max_object_curvi_relative_speed);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Thunder_Create_Initial_Zones(Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_core_input);

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

   /* CVW zone */
   p_core_input->initial_cvw_zone.size                      = p_core_input->initial_bsw_zone.size;
   p_core_input->initial_cvw_zone.points[FRONT_EGO_SIDE]    = p_core_input->initial_bsw_zone.points[FRONT_EGO_SIDE];
   p_core_input->initial_cvw_zone.points[FRONT_OUTER_SIDE]  = p_core_input->initial_bsw_zone.points[FRONT_OUTER_SIDE];
   p_core_input->initial_cvw_zone.points[MIDDLE_OUTER_SIDE] = Create_2d_Vector_Coordinates(
      Fbk_Half(p_cals->k_bsw_zone_front_ego_side_x - p_cals->k_lcda_max_range), p_cals->k_bsw_zone_rear_outer_side_y);
   p_core_input->initial_cvw_zone.points[REAR_OUTER_SIDE] =
      Create_2d_Vector_Coordinates(-p_cals->k_lcda_max_range, p_cals->k_bsw_zone_rear_outer_side_y);
   p_core_input->initial_cvw_zone.points[REAR_EGO_SIDE] =
      Create_2d_Vector_Coordinates(-p_cals->k_lcda_max_range, p_cals->k_bsw_zone_front_ego_side_y);
   p_core_input->initial_cvw_zone.points[MIDDLE_EGO_SIDE] = Create_2d_Vector_Coordinates(
      Fbk_Half(p_cals->k_bsw_zone_front_ego_side_x - p_cals->k_lcda_max_range), p_cals->k_bsw_zone_front_ego_side_y);

   /* CVW hysteresis zone */
   p_core_input->initial_cvw_zone_hys = p_core_input->initial_cvw_zone;
   p_core_input->initial_cvw_zone_hys.points[FRONT_EGO_SIDE].y -= p_cals->k_bsw_zone_front_ego_side_y_hys;
   p_core_input->initial_cvw_zone_hys.points[FRONT_OUTER_SIDE].y += p_cals->k_bsw_zone_rear_outer_side_y_hys;
   p_core_input->initial_cvw_zone_hys.points[MIDDLE_OUTER_SIDE].y += p_cals->k_bsw_zone_rear_outer_side_y_hys;
   p_core_input->initial_cvw_zone_hys.points[REAR_OUTER_SIDE].y += p_cals->k_bsw_zone_rear_outer_side_y_hys;
   p_core_input->initial_cvw_zone_hys.points[REAR_EGO_SIDE].y -= p_cals->k_bsw_zone_front_ego_side_y_hys;
   p_core_input->initial_cvw_zone_hys.points[MIDDLE_EGO_SIDE].y -= p_cals->k_bsw_zone_front_ego_side_y_hys;
}
