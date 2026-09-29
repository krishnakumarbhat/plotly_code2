/**
 * @file cta_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 pre run logic for CTA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "cta_pre_run.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

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
 * @SDD{CSCSA-27731}
 * @verification{Check whether the warn settings are set correctly for different hmi settings.}
 */
static void Cta_Set_Hmi_Warn_Settings(Cta_Core_Input_T *p_cta_core_input /**< Cta Core input */,
                                      const Cta_Input_T *p_cta_input /**< Cta feature input */,
                                      const Cta_Core_Calibration_T *p_cta_cal /**< Cta calibrations */);

/**
 * @brief Constructs the CTA zone and fills it into the CTA core input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-27732}
 * @verification{}
 */
static void Cta_Construct_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**< Cta zone*/,
                               const Cta_Input_T *p_cta_input /**< Cta input */,
                               const Cta_Core_Calibration_T *p_cta_cal /**< Cta Calibration */,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data  */);

/**
 * @brief Constructs the CTA Front zone and fills it into the CTA core input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-27733}
 * @verification{}
 */
static void Cta_Construct_Front_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**< Cta zone */,
                                     const Cta_Core_Calibration_T *p_cta_cal /**< Cta Calibration */,
                                     const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Constructs the CTA Rear zone and fills it into the CTA core input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-27734}
 * @verification{}
 */
static void Cta_Construct_Rear_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**< Cta zone */,
                                    const Cta_Core_Calibration_T *p_cta_cal /**< Cta Calibration */,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Constructs the CTA Rear-Front zone and fills it into the CTA core input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-27735}
 * @verification{}
 */
static void Cta_Construct_Rear_Front_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**< Cta zone */,
                                          const Cta_Core_Calibration_T *p_cta_cal /**< Cta Calibration */,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Rivian_Input(const Cta_Input_T *p_cta_input);
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Cta_Init_Input(Cta_Input_T *p_cta_input)
{
   /* Set flags to defaults */
   p_cta_input->f_cta_enable        = FBK_FALSE;
   p_cta_input->f_front_cta_enable  = FBK_TRUE;
   p_cta_input->f_rear_cta_enable   = FBK_TRUE;
   p_cta_input->cta_warntrigger_hmi = CTA_RIVIAN_SRR6_WARNTRIGGER_NORMAL;
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
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_fbk_output->p_pa_data->vehicle_data;

   assert(NULL != p_cta_input);
   assert(NULL != p_cta_instance);
   assert(NULL != p_fbk_output);
   assert(NULL != p_pt_output);

   p_cta_instance->core_input.f_cta_switch  = p_cta_input->f_cta_enable;
   p_cta_instance->core_input.p_pa_data     = p_fbk_output->p_pa_data;
   p_cta_instance->core_input.p_pt_output   = p_pt_output;
   p_cta_instance->core_input.cta_stop_mode = CTA_STOP_MODE_TTC;
   /* Create CTA zone */
   Cta_Construct_Zone(&(p_cta_instance->core_input.cta_zone), p_cta_input, &p_cta_instance->calibration, p_vehicle_data);

   Cta_Set_Hmi_Warn_Settings(&p_cta_instance->core_input, p_cta_input, &p_cta_instance->calibration);

#ifdef BINARY_DEBUG
   Write_Debug_Cta_Rivian_Input(p_cta_input);
#endif /* BINARY_DEBUG */
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/


static void Cta_Set_Hmi_Warn_Settings(Cta_Core_Input_T *p_cta_core_input,
                                      const Cta_Input_T *p_cta_input,
                                      const Cta_Core_Calibration_T *p_cta_cal)
{
   float32_T tmp_ttc;
   uint8_t level_idx;
   uint8_t mode_idx;

   assert(NULL != p_cta_input);
   assert(NULL != p_cta_cal);

   /* Set warn settings to the core */
   switch (p_cta_input->cta_warntrigger_hmi)
   {
      case CTA_RIVIAN_SRR6_WARNTRIGGER_LATE:
         tmp_ttc = p_cta_cal->k_cta_ttc_warntrigger_late;
         break;
      case CTA_RIVIAN_SRR6_WARNTRIGGER_EARLY:
         tmp_ttc = p_cta_cal->k_cta_ttc_warntrigger_early;
         break;
      case CTA_RIVIAN_SRR6_WARNTRIGGER_NORMAL:
      default:
         /* Warntrigger mode normal*/
         tmp_ttc = p_cta_cal->k_cta_ttc_criticality_level[CTA_MODE_REAR][FBK_ZERO_UINT];
         break;
   }
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         p_cta_core_input->ttc_criticality_level[mode_idx][level_idx] = tmp_ttc;
      }
   }
}

static void Cta_Construct_Zone(Fbk_Field_Of_Interest_T *p_cta_zone,
                               const Cta_Input_T *p_cta_input,
                               const Cta_Core_Calibration_T *p_cta_cal,
                               const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   if (Fbk_Is_True(p_cta_input->f_front_cta_enable) && Fbk_Is_False(p_cta_input->f_rear_cta_enable))
   {
      Cta_Construct_Front_Zone(p_cta_zone, p_cta_cal, p_vehicle_data);
   }
   else if (Fbk_Is_True(p_cta_input->f_rear_cta_enable) && Fbk_Is_False(p_cta_input->f_front_cta_enable))
   {
      Cta_Construct_Rear_Zone(p_cta_zone, p_cta_cal, p_vehicle_data);
   }
   else if (Fbk_Is_True(p_cta_input->f_rear_cta_enable) && Fbk_Is_True(p_cta_input->f_front_cta_enable))
   {
      Cta_Construct_Rear_Front_Zone(p_cta_zone, p_cta_cal, p_vehicle_data);
   }
   else
   {
      // TODO Add comment
      Cta_Construct_Rear_Front_Zone(p_cta_zone, p_cta_cal, p_vehicle_data);
   }
}

static void Cta_Construct_Rear_Zone(Fbk_Field_Of_Interest_T *p_cta_zone,
                                    const Cta_Core_Calibration_T *p_cta_cal,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data)
{

   /*   Zone Rear Mode  */
   /*   x = long, y = lat   */
   /*                       */
   /*                       */
   /*  [   ]                */
   /*  [ego]      ---1      */
   /*  [   ]   ---   |      */
   /*    0 ---       |      */
   /*    |           |      */
   /*    |           |      */
   /*    3 ----------2      */
   /*                       */
   p_cta_zone->points[0].x = -1.0f * p_vehicle_data->host_length;
   p_cta_zone->points[0].y = Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[1].x = (-1.0f * p_vehicle_data->host_length)
                             + (Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0]) * (p_cta_cal->k_cta_max_length_fov + 0.5f));
   p_cta_zone->points[1].y = p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[2].x = (-1.0f * p_vehicle_data->host_length)
                             - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[2].y = p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[3].x = (-1.0f * p_vehicle_data->host_length)
                             - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[3].y = Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->size = p_cta_cal->k_cta_amount_butterfly_points_in_use;
}

static void Cta_Construct_Front_Zone(Fbk_Field_Of_Interest_T *p_cta_zone,
                                     const Cta_Core_Calibration_T *p_cta_cal,
                                     const Fbk_Vehicle_Data_T *p_vehicle_data)
{

   /*   Zone Front Mode  */
   /*   x = long, y = lat   */
   /*                       */
   /*    1 ----------2      */
   /*    |           |      */
   /*    |           |      */
   /*    0 ---       |      */
   /*  [   ]  ---    |      */
   /*  [ego]     --- 3      */
   /*  [   ]                */
   /*                       */
   p_cta_zone->points[0].x = p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[0].y = Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[1].x = p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[1].y = p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[2].x = -Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0])
                             * (p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width));
   p_cta_zone->points[2].y = p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[3].x = 0.0f;
   p_cta_zone->points[3].y = Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->size = p_cta_cal->k_cta_amount_butterfly_points_in_use;
}

static void Cta_Construct_Rear_Front_Zone(Fbk_Field_Of_Interest_T *p_cta_zone,
                                          const Cta_Core_Calibration_T *p_cta_cal,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /*   Zone Rear and Front Mode  */
   /*   x = long, y = lat   */
   /*    0-----------1      */
   /*    |           |      */
   /*    |           |      */
   /*  [   ]         |      */
   /*  [ego]         |      */
   /*  [   ]         |      */
   /*    |           |      */
   /*    |           |      */
   /*    3 ----------2      */
   /*                       */
   p_cta_zone->points[0].x = p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[0].y = Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[1].x = p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[1].y = p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[2].x = (-1.0f * p_vehicle_data->host_length)
                             - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[2].y = p_cta_cal->k_cta_max_length_fov + Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->points[3].x = (-1.0f * p_vehicle_data->host_length)
                             - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_2) - 1u];
   p_cta_zone->points[3].y = Fbk_Half(p_vehicle_data->host_width);

   p_cta_zone->size = p_cta_cal->k_cta_amount_butterfly_points_in_use;
}

#ifdef BINARY_DEBUG

static void Write_Debug_Cta_Rivian_Input(const Cta_Input_T *p_cta_input)
{
   /* Check input parameters. */
   assert(NULL != p_cta_input);

   /* Log CTA core ouput. */
   CTA_STORE_VAL_MGR_WPR("f_rivian_cta_enable", p_cta_input->f_cta_enable);
   CTA_STORE_VAL_MGR_WPR("f_rivian_front_cta_enable", p_cta_input->f_front_cta_enable);
   CTA_STORE_VAL_MGR_WPR("f_rivian_rear_cta_enable", p_cta_input->f_rear_cta_enable);
}
#endif /* BINARY_DEBUG */
