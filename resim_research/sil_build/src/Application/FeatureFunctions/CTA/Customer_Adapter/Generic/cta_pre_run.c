/**
 * @file cta_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for CTA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "cta_pre_run.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
#include <assert.h>

/*============================================================================*\
* EXPORTED FUNCTIONS
\*============================================================================*/

static void Cta_Construct_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**<pointer to cta zone*/,
                               const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data */);

static void Cta_Set_Time_To_Conflict_Criticality_Levels(Cta_Core_Input_T *p_cta_core_input, const Cta_Core_Calibration_T *p_cta_cal);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Cta_Init_Input(Cta_Input_T *p_cta_input /**< CTA Input */)
{
   assert(NULL != p_cta_input);
   /* Set flags to defaults */
   p_cta_input->f_cta_enable       = FBK_TRUE;
   p_cta_input->f_front_cta_enable = FBK_TRUE;
   p_cta_input->f_rear_cta_enable  = FBK_TRUE;
}

void Cta_Pre_Run_Init(Cta_Instance_T *p_cta_instance)
{
   uint8_t side_idx;
   uint8_t mode_idx;

   assert(NULL != p_cta_instance);

   /* Initialize comparison data of CTA for all different modes and sides. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
      {
         Fbk_Reset_Object_Data(&(p_cta_instance->cta_obj_tracker_high_crit[mode_idx][side_idx]));
      }
   }
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Cta_Pre_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, const Fbk_Output_T *p_fbk_output, const Pt_Output_T *p_pt_output)
/* clang-format on */
{
   Fbk_Vehicle_Data_T vehicle_data;
   const Pa_Data_T *p_pa_data;
   const Cta_Core_Calibration_T *p_cta_cal;
   Cta_Core_Input_T *p_cta_core_input;

   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_fbk_output);
   assert(NULL != p_pt_output);

   p_cta_core_input = &p_cta_instance->core_input;
   p_cta_cal        = &p_cta_instance->calibration;
   p_pa_data        = p_fbk_output->p_pa_data;

   /* Fill vehicle data */
   vehicle_data = p_pa_data->vehicle_data;

   p_cta_core_input->f_cta_switch = p_cta_input->f_cta_enable;
   p_cta_core_input->p_pa_data    = p_pa_data;
   p_cta_core_input->p_pt_output  = p_pt_output;

   if (p_cta_instance->calibration.k_cta_f_stop_mode_ttp == FBK_TRUE)
   {
      p_cta_core_input->cta_stop_mode = CTA_STOP_MODE_TTP;
   }
   else
   {
      p_cta_core_input->cta_stop_mode = CTA_STOP_MODE_TTC;
   }

   if (p_cta_core_input->f_cta_switch)
   {
      Cta_Construct_Zone(&p_cta_core_input->cta_zone, p_cta_cal, &vehicle_data);
      Cta_Set_Time_To_Conflict_Criticality_Levels(p_cta_core_input, p_cta_cal);
   }
}

// Constract zone like in thunder
static void Cta_Construct_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**<pointer to cta zone*/,
                               const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data */)
{
   /*   Zone Backward Mode  */
   /*   x = long, y = lat   */
   /*                       */
   /*                       */
   /*  [   ]                */
   /*  [ego]     --- 3      */
   /*  [   ]  ---    |      */
   /*    0 ---       |      */
   /*    |           |      */
   /*    |           |      */
   /*    1 ---       |      */
   /*         ---    |      */
   /*            --- 2      */
   p_cta_zone->size = p_cta_cal->k_cta_amount_butterfly_points_in_use;

   p_cta_zone->points[0].x =
      p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][((uint8_t) CTA_CRIT_LEVEL_2) - FBK_ONE_UINT];
   p_cta_zone->points[0].y = p_cta_cal->k_cta_butterfly_lat[0];


   p_cta_zone->points[1].x =
      (-FBK_ONE_F * p_vehicle_data->host_length)
      - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - FBK_ONE_UINT];
   p_cta_zone->points[1].y = p_cta_cal->k_cta_butterfly_lat[1];


   p_cta_zone->points[2].x =
      p_cta_zone->points[1].x - (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[1]));
   p_cta_zone->points[2].y = p_cta_cal->k_cta_max_length_fov;


   p_cta_zone->points[3].x =
      p_cta_zone->points[0].x + (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0]));
   p_cta_zone->points[3].y = p_cta_cal->k_cta_max_length_fov;
}


static void Cta_Set_Time_To_Conflict_Criticality_Levels(Cta_Core_Input_T *p_cta_core_input, const Cta_Core_Calibration_T *p_cta_cal)
{
   uint8_t level_idx;
   uint8_t mode_idx;

   assert(NULL != p_cta_core_input);
   assert(NULL != p_cta_cal);

   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         p_cta_core_input->ttc_criticality_level[mode_idx][level_idx] = p_cta_cal->k_cta_ttc_criticality_level[mode_idx][level_idx];
      }
   }
}
