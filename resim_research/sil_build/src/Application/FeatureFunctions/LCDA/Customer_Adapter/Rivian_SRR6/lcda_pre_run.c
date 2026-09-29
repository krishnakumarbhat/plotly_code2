/**
 * @file lcda_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 pre run logic for LCDA.
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

#define LCDA_RIVIAN_DEFAULT_BSW_ADJUSTMENT_FACTOR (1.0f)

#define LCDA_RIVIAN_ZONE_WIDTH (2.5f)
#define LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE (0.5f)

#define LCDA_RIVIAN_ZONE_START_FRONT (2.44f)
#define LCDA_RIVIAN_ZONE_LENGTH_BEHIND_HOST (5.0f)

#define LCDA_RIVIAN_ZONE_HYSTERESIS_LONG (1.5f)
#define LCDA_RIVIAN_ZONE_HYSTERESIS_LAT (0.8f)
#define LCDA_RIVIAN_ZONE_INNER_HYSTERESIS_LAT (0.5f)
#define LCDA_RIVIAN_ZONE_LEN_WARNTRIGGER_DIFF (0.5f)

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Maps the hmi flag of the feature set from external to the warn settings of the core.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{Check whether the warn settings are set correctly for different hmi settings.}
 */
static void Lcda_Set_Hmi_Warn_Settings(Lcda_Core_Input_T *p_core_input /**< Core input of Lcda */,
                                       const Lcda_Input_T *p_lcda_input /**< Lcda feature input */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Create the initial fixed zones for BSW and CVW submodules according to Rivian requirements.
 *
 * @return void
 *
 *
 * @SRS{}
 *
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Rivian_Create_Initial_Zones(Lcda_Core_Input_T *p_core_input /**< LCDA Core Input */,
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
   p_lcda_input->f_lcda_enable_bsw = FBK_FALSE;
   p_lcda_input->f_lcda_enable_cvw = FBK_FALSE;

   /* Set trailer input to default */
   p_lcda_input->f_trailer_present = FBK_FALSE;
   p_lcda_input->trailer_length    = FBK_ZERO_F;
   p_lcda_input->trailer_width     = FBK_ZERO_F;
   p_lcda_input->trailer_angle     = FBK_ZERO_F;

   /* Set hmi input to default */
   p_lcda_input->lcda_warntrigger_hmi = LCDA_RIVIAN_SRR6_WARNTRIGGER_NORMAL;
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
   Lcda_Core_Input_T *p_core_input;
   const Lcda_Core_Calibration_T *p_cals;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_fbk_output);

   /* Fill vehicle data */
   p_core_input            = &p_lcda_instance->core_input;
   p_core_input->p_pa_data = p_fbk_output->p_pa_data;
   p_cals                  = &p_lcda_instance->calibration;
   p_vehicle_data          = &p_core_input->p_pa_data->vehicle_data;

   /* Create Rivian-specific zones */
   Lcda_Rivian_Create_Initial_Zones(p_core_input, p_vehicle_data, p_cals);


   /* Populate the core input */
   p_core_input->enabled_flags.f_lcda_enabled = p_lcda_input->f_lcda_enable;
   p_core_input->enabled_flags.f_bsw_enabled  = p_lcda_input->f_lcda_enable_bsw;
   p_core_input->enabled_flags.f_cvw_enabled =
      (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_cvw) && Fbk_Is_False(p_lcda_input->f_trailer_present));
   p_core_input->enabled_flags.f_slc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_elc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_dropback_enabled = FBK_FALSE;
   p_core_input->enabled_flags.f_fallback_enabled = FBK_TRUE;

   p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc;
   p_core_input->warn_settings.cvw_rel_vel_range = Create_Float_Range(p_cals->k_cvw_min_object_curvi_relative_speed[FBK_ZERO_UINT],
                                                                      p_cals->k_cvw_max_object_curvi_relative_speed);

   /* Set warn settings dependent on the given Hmi */
   Lcda_Set_Hmi_Warn_Settings(p_core_input, p_lcda_input, p_cals);

   /* Set lane width from camera info */
   p_core_input->lane_width         = p_vehicle_data->lane_width;
   p_core_input->lane_center_offset = p_vehicle_data->lane_center_offset;

   /* Set trailer data */
   p_core_input->trailer.f_trailer_present = p_lcda_input->f_trailer_present;
   p_core_input->trailer.length            = p_lcda_input->trailer_length;
   p_core_input->trailer.width             = p_lcda_input->trailer_width;
   p_core_input->trailer.angle             = p_lcda_input->trailer_angle;
}


/*===========================================================================*\
* Local Function
 * Definitions
\*===========================================================================*/


static void Lcda_Set_Hmi_Warn_Settings(Lcda_Core_Input_T *p_core_input,
                                       const Lcda_Input_T *p_lcda_input,
                                       const Lcda_Core_Calibration_T *p_cals)
{
   Lcda_Rivian_Srr6_Hmi_Warntrigger_T hmi_warn_setting;

   assert(NULL != p_core_input);
   assert(NULL != p_lcda_input);
   assert(NULL != p_cals);

   /* Set hmi warntrigger */
   if (Fbk_Is_True(p_cals->k_lcda_use_default_warntrigger_hmi)
       && (LCDA_RIVIAN_SRR6_WARNTRIGGER_VERY_EARLY != p_lcda_input->lcda_warntrigger_hmi))
   {
      /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
      hmi_warn_setting = (Lcda_Rivian_Srr6_Hmi_Warntrigger_T) p_cals->k_lcda_default_warntrigger_hmi;
   }
   else
   {
      /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
      hmi_warn_setting = p_lcda_input->lcda_warntrigger_hmi;
   }

   /* Set warn settings to the core */
   p_core_input->warn_settings.bsw_len_factor = LCDA_RIVIAN_DEFAULT_BSW_ADJUSTMENT_FACTOR;
   switch (hmi_warn_setting)
   {
      case LCDA_RIVIAN_SRR6_WARNTRIGGER_LATE:
         p_core_input->initial_bsw_zone.points[2].x += LCDA_RIVIAN_ZONE_LEN_WARNTRIGGER_DIFF;
         p_core_input->initial_bsw_zone.points[3].x += LCDA_RIVIAN_ZONE_LEN_WARNTRIGGER_DIFF;
         p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc + p_cals->k_cvw_warntrigger_late;
         break;
      case LCDA_RIVIAN_SRR6_WARNTRIGGER_EARLY:
         p_core_input->initial_bsw_zone.points[2].x -= LCDA_RIVIAN_ZONE_LEN_WARNTRIGGER_DIFF;
         p_core_input->initial_bsw_zone.points[3].x -= LCDA_RIVIAN_ZONE_LEN_WARNTRIGGER_DIFF;
         p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc + p_cals->k_cvw_warntrigger_early;
         break;
      case LCDA_RIVIAN_SRR6_WARNTRIGGER_NORMAL:
      default:
         /* Warntrigger mode normal*/
         p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc;
         break;
   }
}

static void Lcda_Rivian_Create_Initial_Zones(Lcda_Core_Input_T *p_core_input,
                                             const Fbk_Vehicle_Data_T *p_vehicle_data,
                                             const Lcda_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);

   /**
    * BSM Zone width: 2.5 m
    * Distance from outer of tire to inner edge of BSM Zone: 0.5 m
    * Distance from Rear
    * bumper to rear of BSM Zone: 5 m
    *
    * BSM Hysteresis Lateral Off: 3.8 m
    * Distance from outer of tire to inner
    * edge of BSM Zone: 0.5 m BSM Hysteresis Longitudinal OFF: 6.5 m
    */

   /* BSW zone */
   p_core_input->bsw_zone_calculation_mode  = BSW_ZONE_CALC_FIXED_INPUT;
   p_core_input->initial_bsw_zone.size      = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_bsw_zone.points[0] = Create_2d_Vector_Coordinates(
      -p_vehicle_data->host_length + LCDA_RIVIAN_ZONE_START_FRONT,
      Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE + LCDA_RIVIAN_ZONE_WIDTH);
   p_core_input->initial_bsw_zone.points[1] = Create_2d_Vector_Coordinates(
      -p_vehicle_data->host_length,
      Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE + LCDA_RIVIAN_ZONE_WIDTH);
   p_core_input->initial_bsw_zone.points[2] = Create_2d_Vector_Coordinates(
      (-p_vehicle_data->host_length - LCDA_RIVIAN_ZONE_LENGTH_BEHIND_HOST),
      Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE + LCDA_RIVIAN_ZONE_WIDTH);
   p_core_input->initial_bsw_zone.points[3] = Create_2d_Vector_Coordinates(
      (-p_vehicle_data->host_length - LCDA_RIVIAN_ZONE_LENGTH_BEHIND_HOST), Fbk_Half(p_vehicle_data->host_width));
   p_core_input->initial_bsw_zone.points[4] =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length, Fbk_Half(p_vehicle_data->host_width));
   p_core_input->initial_bsw_zone.points[5] =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length + LCDA_RIVIAN_ZONE_START_FRONT, Fbk_Half(p_vehicle_data->host_width));

   /* BSW hysteresis zone */
   p_core_input->initial_bsw_zone_hys = p_core_input->initial_bsw_zone;
   p_core_input->initial_bsw_zone_hys.points[0].y += LCDA_RIVIAN_ZONE_HYSTERESIS_LAT;
   p_core_input->initial_bsw_zone_hys.points[1].y += LCDA_RIVIAN_ZONE_HYSTERESIS_LAT;
   p_core_input->initial_bsw_zone_hys.points[2].y += LCDA_RIVIAN_ZONE_HYSTERESIS_LAT;
   p_core_input->initial_bsw_zone_hys.points[3].y -= LCDA_RIVIAN_ZONE_INNER_HYSTERESIS_LAT;
   p_core_input->initial_bsw_zone_hys.points[4].y -= LCDA_RIVIAN_ZONE_INNER_HYSTERESIS_LAT;
   p_core_input->initial_bsw_zone_hys.points[5].y -= LCDA_RIVIAN_ZONE_INNER_HYSTERESIS_LAT;

   p_core_input->initial_bsw_zone_hys.points[2].x -= LCDA_RIVIAN_ZONE_HYSTERESIS_LONG;
   p_core_input->initial_bsw_zone_hys.points[3].x -= LCDA_RIVIAN_ZONE_HYSTERESIS_LONG;

   /* CVW zone */
   p_core_input->initial_cvw_zone.size      = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_cvw_zone.points[0] = Create_2d_Vector_Coordinates(
      -p_vehicle_data->host_length + LCDA_RIVIAN_ZONE_START_FRONT,
      Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE + LCDA_RIVIAN_ZONE_WIDTH);
   p_core_input->initial_cvw_zone.points[1] = Create_2d_Vector_Coordinates(
      -p_vehicle_data->host_length,
      Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE + LCDA_RIVIAN_ZONE_WIDTH);
   p_core_input->initial_cvw_zone.points[2] = Create_2d_Vector_Coordinates(
      -p_cals->k_lcda_max_range,
      Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE + LCDA_RIVIAN_ZONE_WIDTH);
   p_core_input->initial_cvw_zone.points[3] = Create_2d_Vector_Coordinates(
      -p_cals->k_lcda_max_range, Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE);
   p_core_input->initial_cvw_zone.points[4] = Create_2d_Vector_Coordinates(
      -p_vehicle_data->host_length, Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE);
   p_core_input->initial_cvw_zone.points[5] =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length + LCDA_RIVIAN_ZONE_START_FRONT,
                                   Fbk_Half(p_vehicle_data->host_width) + LCDA_RIVIAN_ZONE_DISTANCE_TO_VEHICLE_SIDE);

   /* CVW hysteresis zone */
   p_core_input->initial_cvw_zone_hys = p_core_input->initial_cvw_zone;
   p_core_input->initial_cvw_zone_hys.points[0].y += LCDA_RIVIAN_ZONE_HYSTERESIS_LAT;
   p_core_input->initial_cvw_zone_hys.points[1].y += LCDA_RIVIAN_ZONE_HYSTERESIS_LAT;
   p_core_input->initial_cvw_zone_hys.points[2].y += LCDA_RIVIAN_ZONE_HYSTERESIS_LAT;
}
