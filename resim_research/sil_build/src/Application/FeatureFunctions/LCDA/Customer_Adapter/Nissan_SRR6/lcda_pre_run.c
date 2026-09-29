/**
 * @file lcda_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Nissan SRR6 pre run logic for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_pre_run.h"
#include "fbk_field_of_interest.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_types.h"
#include "ml_interval.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

#include <assert.h>

#ifdef BINARY_DEBUG
#include "lcda_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

#define LCDA_GUARDRAIL_LOW_EXIST_PROB (0.7f)
#define LCDA_GUARDRAIL_HIGH_EXIST_PROB (0.9f)

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/


#ifdef BINARY_DEBUG
static void Write_Lcda_Input(const Lcda_Input_T *p_lcda_input);
#endif

/**
 * @brief Set core input guardrail data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6936}
 * @verification{Check whether the mapping of the guardrail data to the expected core input is correctly.}
 */
static void Lcda_Set_Guardrail_Data(Lcda_Core_Input_T *p_core_input /**< Core input of Lcda */);

/**
 * @brief Fill initial bsw zone.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6935}
 * @verification{Check initialization of BSW zone points.}
 */
static void Lcda_Get_Initial_Bsw_Zone_Points(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone_hys);

/**
 * @brief Fill initial cvw zone.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6942}
 * @verification{Check initialization of CVW zone points.}
 */
static void Lcda_Get_Initial_Cvw_Zone_Points(const Fbk_Vehicle_Data_T *p_vehicle_data,
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
   p_lcda_input->f_lcda_enable     = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_bsw = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_cvw = FBK_ZERO_UINT;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Pre_Run_Init(Lcda_Instance_T *p_lcda_instance)
{
   /* Assert */
   assert(NULL != p_lcda_instance);
}

/* clang-format off */
/* coverity[misra_c_2012_rule_8_13_violation][Unchanged function parameter is modified by other customer and cannot be const] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Lcda_Pre_Run(Lcda_Instance_T *p_lcda_instance /**< Lcda core input */,
                  const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                  const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   Lcda_Core_Input_T *p_core_input;
   const Lcda_Core_Calibration_T *p_cals;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_fbk_output);

   p_core_input            = &p_lcda_instance->core_input;
   p_cals                  = &p_lcda_instance->calibration;
   p_core_input->p_pa_data = p_fbk_output->p_pa_data;
   p_vehicle_data          = &p_core_input->p_pa_data->vehicle_data;

   /* Populate the core input */
   p_core_input->enabled_flags.f_lcda_enabled     = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable));
   p_core_input->enabled_flags.f_bsw_enabled      = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_bsw));
   p_core_input->enabled_flags.f_cvw_enabled      = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_cvw));
   p_core_input->enabled_flags.f_slc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_elc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_dropback_enabled = FBK_FALSE;
   p_core_input->enabled_flags.f_fallback_enabled = FBK_TRUE;

   p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc;
   p_core_input->warn_settings.cvw_rel_vel_range = Create_Float_Range(p_cals->k_cvw_min_object_curvi_relative_speed[FBK_ZERO_UINT],
                                                                      p_cals->k_cvw_max_object_curvi_relative_speed);

   /* TODO: Clarify Nissan expectation - needs to be set like that until calibration values are adapted to Nissan */
   p_core_input->bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   Lcda_Get_Initial_Bsw_Zone_Points(p_vehicle_data, p_cals, &(p_core_input->initial_bsw_zone), &(p_core_input->initial_bsw_zone_hys));
   Lcda_Get_Initial_Cvw_Zone_Points(p_vehicle_data, p_cals, &(p_core_input->initial_cvw_zone), &(p_core_input->initial_cvw_zone_hys));

   /* Set lane width from camera info */
   p_core_input->lane_width = p_cals->k_lm_lane_width_highway;
   /* TODO: Clarify if we receive that input within the vehicle_data interface and it gets from core0, if yes, change to
    * Pa_Veh_Get_Lane_Width(p_lcda_input->p_context) */

   p_core_input->lane_center_offset = p_cals->k_lm_lane_center_offset_default;
   /* TODO: Clarify if we receive that input within the vehicle_data interface and it gets from core0, if yes, change to
    * Pa_Veh_Get_Lane_Center_Offset(p_lcda_input->p_context) */

   Lcda_Set_Guardrail_Data(p_core_input);

#ifdef BINARY_DEBUG
   Write_Lcda_Input(p_lcda_input);
#endif /* BINARY_DEBUG */
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/


static void Lcda_Set_Guardrail_Data(Lcda_Core_Input_T *p_core_input)
{
   uint8_t side;
   float32_T exist_prob_mock;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      const Fbk_Guardrail_Data_T *p_guardrail_data = &p_core_input->p_pa_data->guardrail_data[side];

      if (Fbk_Is_True(p_guardrail_data->f_present))
      {
         exist_prob_mock = LCDA_GUARDRAIL_LOW_EXIST_PROB;

         if (PA_OBJ_STATUS_MATURE == p_guardrail_data->status)
         {
            exist_prob_mock = LCDA_GUARDRAIL_HIGH_EXIST_PROB;
         }

         p_core_input->guardrail_data[side].radar.lateral_position = p_guardrail_data->lat_pos;
         p_core_input->guardrail_data[side].radar.confidence       = exist_prob_mock;
         p_core_input->guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;
      }
      else
      {
         p_core_input->guardrail_data[side].radar.lateral_position = FBK_ZERO_F;
         p_core_input->guardrail_data[side].radar.confidence       = FBK_ZERO_F;
         p_core_input->guardrail_data[side].radar.status           = LCDA_GUARDRAIL_INVALID;
      }
   }
}


static void Lcda_Get_Initial_Bsw_Zone_Points(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone,
                                             Fbk_Field_Of_Interest_T *initial_bsw_zone_hys)
{
   float32_T host_length        = p_vehicle_data->host_length;
   float32_T half_host_width    = p_vehicle_data->host_width / 2.0f;
   float32_T half_short_dynzone = (p_cals->k_bsw_x_length + p_cals->k_bsw_dynzone_range[0]) / 2.0f; /* middle of short BSW zone */

   initial_bsw_zone->size = LCDA_NUMBER_OF_ZONE_POINTS;

   initial_bsw_zone->points[0].x = p_cals->k_bsw_x0 - host_length;
   initial_bsw_zone->points[1].x = initial_bsw_zone->points[0].x + half_short_dynzone;
   initial_bsw_zone->points[2].x = initial_bsw_zone->points[0].x + p_cals->k_bsw_x_length;
   initial_bsw_zone->points[3].x = initial_bsw_zone->points[2].x;
   initial_bsw_zone->points[4].x = initial_bsw_zone->points[1].x;
   initial_bsw_zone->points[5].x = initial_bsw_zone->points[0].x;

   /* all parameters with minus because of the symmetrical reflection of y axis in Renault CS */
   initial_bsw_zone->points[5].y = p_cals->k_bsw_y0 + half_host_width;
   initial_bsw_zone->points[4].y = initial_bsw_zone->points[5].y;
   initial_bsw_zone->points[3].y = initial_bsw_zone->points[5].y;
   initial_bsw_zone->points[0].y = initial_bsw_zone->points[5].y + p_cals->k_bsw_y_width;
   initial_bsw_zone->points[1].y = initial_bsw_zone->points[0].y;
   initial_bsw_zone->points[2].y = initial_bsw_zone->points[0].y;

   initial_bsw_zone_hys->size = LCDA_NUMBER_OF_ZONE_POINTS;

   initial_bsw_zone_hys->points[0].x = initial_bsw_zone->points[0].x + p_cals->k_bsw_x0_hys;
   initial_bsw_zone_hys->points[1].x = initial_bsw_zone->points[1].x;
   initial_bsw_zone_hys->points[2].x = initial_bsw_zone->points[2].x - p_cals->k_bsw_x1_hys;
   initial_bsw_zone_hys->points[3].x = initial_bsw_zone_hys->points[2].x;
   initial_bsw_zone_hys->points[4].x = initial_bsw_zone_hys->points[1].x;
   initial_bsw_zone_hys->points[5].x = initial_bsw_zone_hys->points[0].x;

   /* all parameters with minus because of the symmetrical reflection of y axis in Renault CS */
   initial_bsw_zone_hys->points[0].y = initial_bsw_zone->points[0].y + p_cals->k_bsw_y1_hys;
   initial_bsw_zone_hys->points[1].y = initial_bsw_zone_hys->points[0].y;
   initial_bsw_zone_hys->points[2].y = initial_bsw_zone_hys->points[0].y;
   initial_bsw_zone_hys->points[3].y = initial_bsw_zone->points[3].y - p_cals->k_bsw_y0_hys;
   initial_bsw_zone_hys->points[4].y = initial_bsw_zone_hys->points[3].y;
   initial_bsw_zone_hys->points[5].y = initial_bsw_zone_hys->points[3].y;
}


static void Lcda_Get_Initial_Cvw_Zone_Points(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             Fbk_Field_Of_Interest_T *initial_cvw_zone,
                                             Fbk_Field_Of_Interest_T *initial_cvw_zone_hys)
{
   float32_T host_length     = p_vehicle_data->host_length;
   float32_T half_host_width = p_vehicle_data->host_width / 2.0f;

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

#ifdef BINARY_DEBUG

static void Write_Lcda_Input(const Lcda_Input_T *p_lcda_input)
{
   /* check input parameters */
   assert(NULL != p_lcda_input);

   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_f_lcda_enable", p_lcda_input->f_lcda_enable);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_f_lcda_enable_bsw", p_lcda_input->f_lcda_enable_bsw);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_f_lcda_enable_cvw", p_lcda_input->f_lcda_enable_cvw);
}

#endif /* BINARY_DEBUG */
