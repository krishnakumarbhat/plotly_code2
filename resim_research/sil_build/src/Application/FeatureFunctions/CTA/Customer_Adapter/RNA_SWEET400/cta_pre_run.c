/**
 * @file cta_pre_run.c
 * @brief Contains the RNA_SWEET400 pre run logic for CTA.
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

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief This function constructs CTA field of interest according to the calibration parameters
 * describing the zone in the polar coordinate system. CTA zone is symmetric along x-axis.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2460}
 * @SDD{SF-4024}
 */
static Fbk_Field_Of_Interest_T Cta_Construct_Zone(const Cta_Core_Calibration_T *p_cta_cal, const Fbk_Vehicle_Data_T *p_vehicle_data);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Cta_Init_Input(Cta_Input_T *p_cta_input /**< CTA Input */)
{
   /* Set flags to defaults */
   p_cta_input->f_cta_switch = FBK_ZERO_UINT;
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

void Cta_Pre_Run(Cta_Instance_T *p_cta_instance,
                 const Cta_Input_T *p_cta_input,
                 const Fbk_Output_T *p_fbk_output,
                 const Pt_Output_T *p_pt_output)
{
   uint8_t mode_idx;
   uint8_t level_idx;
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_fbk_output->p_pa_data->vehicle_data;

   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_fbk_output);
   assert(NULL != p_pt_output);

   p_cta_instance->core_input.f_cta_switch  = (boolean_T) (Fbk_Is_True(p_cta_input->f_cta_switch));
   p_cta_instance->core_input.p_pa_data     = p_fbk_output->p_pa_data;
   p_cta_instance->core_input.p_pt_output   = p_pt_output;
   p_cta_instance->core_input.cta_zone      = Cta_Construct_Zone(&p_cta_instance->calibration, p_vehicle_data);
   p_cta_instance->core_input.cta_stop_mode = CTA_STOP_MODE_TTC;

   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         p_cta_instance->core_input.ttc_criticality_level[mode_idx][level_idx] =
            p_cta_instance->calibration.k_cta_ttc_criticality_level[mode_idx][level_idx];
      }
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static Fbk_Field_Of_Interest_T Cta_Construct_Zone(const Cta_Core_Calibration_T *p_cta_cal, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Fbk_Field_Of_Interest_T cta_zone;

   cta_zone.points[0].x = (-1.0f * p_vehicle_data->host_length)
                          - p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   cta_zone.points[0].y = 0.0f;

   cta_zone.points[1].x = (-1.0f * p_vehicle_data->host_length)
                          - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   cta_zone.points[1].y = 0.0f;

   cta_zone.points[2].x = (-1.0f * p_vehicle_data->host_length)
                          - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u]
                          - (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[1]));
   cta_zone.points[2].y = p_cta_cal->k_cta_max_length_fov;

   cta_zone.points[3].x = (-1.0f * p_vehicle_data->host_length)
                          - p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u]
                          + (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0]));
   cta_zone.points[3].y = p_cta_cal->k_cta_max_length_fov;

   cta_zone.size = 4u;

   return cta_zone;
}
