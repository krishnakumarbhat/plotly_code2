/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the STLA_Thunder post run logic for CTA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "cta_post_run.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_customer_calibration_t.h"
#include "cta_types.h"
#include "fbk_array_interpolation.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Thunder_Output(const Cta_Output_T *p_cta_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Reset Rivian CTA output to the default values
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
static void Cta_Reset_Output(Cta_Output_T *p_cta_output);

/**
 * @brief Calculates the position based criticality zone for STLA Thunder.
 *
 * @return STLA Thunder Zone
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-92601}
 */
static CTA_STLA_CRIT_ZONE_T Cta_Get_Stla_Criticality_Zone(uint8_t obj_index,
                                                          const Pa_Data_T *p_pa_data,
                                                          const Cta_Customer_Calibration_T *p_cals,
                                                          uint8_t side);

static Rcp_Status_T Cta_Convert_Cta_status_to_Rcp(const Cta_Status_T cta_status);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run_Init(Cta_Instance_T *p_cta_instance)
{
   assert(NULL != p_cta_instance);
}

static void Cta_Reset_Output(Cta_Output_T *p_cta_output)
{
   /* Assert */
   assert(NULL != p_cta_output);

   p_cta_output->cta_obj_ttc_left                 = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_obj_ttc_right                = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_alert_level_left             = FBK_ZERO_UINT;
   p_cta_output->cta_alert_level_right            = FBK_ZERO_UINT;
   p_cta_output->cta_id_left                      = FBK_ZERO_UINT;
   p_cta_output->cta_id_right                     = FBK_ZERO_UINT;
   p_cta_output->cta_warn_hold_cnt_left           = FBK_ZERO_UINT;
   p_cta_output->cta_warn_hold_cnt_right          = FBK_ZERO_UINT;
   p_cta_output->cta_brake_hold_cnt_left          = FBK_ZERO_UINT;
   p_cta_output->cta_brake_hold_cnt_right         = FBK_ZERO_UINT;
   p_cta_output->cta_brake_supp_cnt_left          = FBK_ZERO_UINT;
   p_cta_output->cta_brake_supp_cnt_right         = FBK_ZERO_UINT;
   p_cta_output->f_brake_qualifier_left           = FBK_FALSE;
   p_cta_output->f_brake_qualifier_right          = FBK_FALSE;
   p_cta_output->f_cta_enabled                    = FBK_FALSE;
   p_cta_output->cta_stla_crit_zone_left          = CTA_STLA_CRIT_ZONE_NONE;
   p_cta_output->cta_stla_crit_zone_right         = CTA_STLA_CRIT_ZONE_NONE;
   p_cta_output->DBG_Crit_Zone_Right_P0_PositionX = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P0_PositionY = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P1_PositionX = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P1_PositionY = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P2_PositionX = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P2_PositionY = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P3_PositionX = FBK_ZERO_F;
   p_cta_output->DBG_Crit_Zone_Right_P3_PositionY = FBK_ZERO_F;
}

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
// clang-format on
{
   float32_T rear_bumper_pos;

   const Pa_Data_T *p_pa_data;
   const Cta_Core_Output_T *p_cta_core_output;
   const Cta_Core_Calibration_T *p_cta_cal;
   const Cta_Customer_Calibration_T *p_cta_custom_cal;

   /* Asserts */
   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_cta_output);

   p_cta_core_output = &p_cta_instance->core_output;
   p_pa_data         = p_cta_instance->core_input.p_pa_data;
   p_cta_cal         = &p_cta_instance->calibration;
   p_cta_custom_cal  = &p_cta_instance->customer_calibration;

   Cta_Reset_Output(p_cta_output);

   p_cta_output->cta_obj_ttc_left         = p_cta_core_output->cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->cta_obj_ttc_right        = p_cta_core_output->cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->cta_alert_level_left     = (uint8_t) p_cta_core_output->cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->cta_alert_level_right    = (uint8_t) p_cta_core_output->cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->cta_id_left              = p_cta_core_output->cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->cta_id_right             = p_cta_core_output->cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->cta_warn_hold_cnt_left   = p_cta_core_output->cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->cta_warn_hold_cnt_right  = p_cta_core_output->cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->cta_brake_hold_cnt_left  = p_cta_core_output->cta_brake_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->cta_brake_hold_cnt_right = p_cta_core_output->cta_brake_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->cta_brake_supp_cnt_left  = p_cta_core_output->cta_brake_supp_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->cta_brake_supp_cnt_right = p_cta_core_output->cta_brake_supp_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->f_brake_qualifier_left   = p_cta_core_output->f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT];
   p_cta_output->f_brake_qualifier_right  = p_cta_core_output->f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   p_cta_output->f_cta_enabled            = (CTA_STATUS_ACTIVE == p_cta_core_output->cta_status) ? FBK_TRUE : FBK_FALSE;

   p_cta_output->cta_stla_crit_zone_left = Cta_Get_Stla_Criticality_Zone(p_cta_core_output->cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT],
                                                                         p_pa_data, p_cta_custom_cal, FBK_SIDE_LEFT);
   p_cta_output->cta_stla_crit_zone_right = Cta_Get_Stla_Criticality_Zone(
      p_cta_core_output->cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT], p_pa_data, p_cta_custom_cal, FBK_SIDE_RIGHT);
   p_cta_output->rcp_status = Cta_Convert_Cta_status_to_Rcp(p_cta_core_output->cta_status);

   /* Write the butterfly zone coordinates to the output */
   rear_bumper_pos = -FBK_ONE_F * p_pa_data->vehicle_data.host_length;

   p_cta_output->DBG_Crit_Zone_Right_P0_PositionX =
      rear_bumper_pos - p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_1) - FBK_ONE_UINT];
   p_cta_output->DBG_Crit_Zone_Right_P0_PositionY = p_cta_cal->k_cta_butterfly_lat[0];

   p_cta_output->DBG_Crit_Zone_Right_P1_PositionX =
      rear_bumper_pos - p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][((uint8_t) CTA_CRIT_LEVEL_1) - FBK_ONE_UINT];
   p_cta_output->DBG_Crit_Zone_Right_P1_PositionY = p_cta_cal->k_cta_butterfly_lat[1];

   p_cta_output->DBG_Crit_Zone_Right_P2_PositionX =
      p_cta_output->DBG_Crit_Zone_Right_P1_PositionX
      - (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[1]));
   p_cta_output->DBG_Crit_Zone_Right_P2_PositionY = p_cta_cal->k_cta_max_length_fov;

   p_cta_output->DBG_Crit_Zone_Right_P3_PositionX =
      p_cta_output->DBG_Crit_Zone_Right_P0_PositionX
      + (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0]));
   p_cta_output->DBG_Crit_Zone_Right_P3_PositionY = p_cta_cal->k_cta_max_length_fov;


#ifdef BINARY_DEBUG
   Write_Debug_Cta_Thunder_Output(p_cta_output);
#endif /* BINARY_DEBUG */
}

/*============================================================================*\
 * Local Function Definition
\*============================================================================*/

static CTA_STLA_CRIT_ZONE_T Cta_Get_Stla_Criticality_Zone(uint8_t obj_index,
                                                          const Pa_Data_T *p_pa_data,
                                                          const Cta_Customer_Calibration_T *p_cals,
                                                          uint8_t side)
{
   CTA_STLA_CRIT_ZONE_T crit_zone = CTA_STLA_CRIT_ZONE_NONE;
   float32_T long_position;
   float32_T lat_position;
   uint8_t long_idx;
   float32_T long_zones[7] = {0.0f};
   Float_Range_T lat_range;


   if (PA_INVALID_OBJ_INDEX != obj_index)
   {
      /* Get the obj position */
      lat_position  = p_pa_data->object_data[obj_index].vcs_pos.y;
      long_position = p_pa_data->object_data[obj_index].vcs_pos.x;

      /* Create lateral range for zone borders using calibration and sign converted from sign*/
      lat_range = Create_Float_Range(
         (Fbk_Convert_Obj_Side_To_Sign(side) * (Fbk_Half(p_pa_data->vehicle_data.host_width) + p_cals->k_stla_crit_zone_G_E_line)),
         (Fbk_Convert_Obj_Side_To_Sign(side) * Fbk_Half(p_pa_data->vehicle_data.host_width)));

      /* Check if the lateral position is within proper range*/
      if (Is_Float_Contained_In_Float_Range(lat_position, &lat_range))
      {

         /* Create array with long zone borders (asc order), then search where the object position is located. Index of upper
          * border is returned*/
         long_zones[0] = -FBK_INVALID_DISTANCE;
         long_zones[1] = -p_pa_data->vehicle_data.host_length - p_cals->k_stla_crit_zone_N_Q_line
                         - p_cals->k_stla_crit_zone_Q_QH_line;                                     /* Line QH */
         long_zones[2] = -p_pa_data->vehicle_data.host_length - p_cals->k_stla_crit_zone_N_Q_line; /* Line Q */
         long_zones[3] = -p_pa_data->vehicle_data.host_length;                                     /* Line N */
         long_zones[4] = -p_cals->k_stla_crit_zone_D_C_line;                                       /* Line C */
         long_zones[5] = FBK_ZERO_F;                                                               /* Line D */
         long_zones[6] = FBK_INVALID_DISTANCE;

         long_idx = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(long_zones, 7u, long_position);

         /* Set STLA position based criticality zone*/
         if (FBK_SIDE_LEFT == side) /* Left side */
         {
            switch (long_idx)
            {
               case 2:
                  crit_zone = CTA_STLA_CRIT_ZONE_7; /* Between Q and QH */
                  break;
               case 3:
                  crit_zone = CTA_STLA_CRIT_ZONE_5; /* Between N and Q */
                  break;
               case 4:
                  crit_zone = CTA_STLA_CRIT_ZONE_3; /* Between C and N */
                  break;
               case 5:
                  crit_zone = CTA_STLA_CRIT_ZONE_1; /* Between D and N */
                  break;
               default:
                  crit_zone = CTA_STLA_CRIT_ZONE_NONE;
                  break;
            }
         }
         else
         { /* Right side*/
            switch (long_idx)
            {
               case 2:
                  crit_zone = CTA_STLA_CRIT_ZONE_8; /* Between Q and QH */
                  break;
               case 3:
                  crit_zone = CTA_STLA_CRIT_ZONE_6; /* Between N and Q */
                  break;
               case 4:
                  crit_zone = CTA_STLA_CRIT_ZONE_4; /* Between C and N */
                  break;
               case 5:
                  crit_zone = CTA_STLA_CRIT_ZONE_2; /* Between D and N */
                  break;
               default:
                  crit_zone = CTA_STLA_CRIT_ZONE_NONE;
                  break;
            }
         }
      }
   }
   return crit_zone;
}

static Rcp_Status_T Cta_Convert_Cta_status_to_Rcp(const Cta_Status_T cta_status)
{
   Rcp_Status_T result;

   switch (cta_status)
   {
      case CTA_STATUS_ACTIVE:
         result = RCP_STATUS_ACTIVE;
         break;
      case CTA_STATUS_DISABLED:
         result = RCP_STATUS_DISABLED;
         break;
      default:
         result = RCP_STATUS_DEACTIVATED_EGO_SPEED;
         break;
   }

   return result;
}

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Thunder_Output(const Cta_Output_T *p_cta_output)
{
   /* Check input parameters. */
   assert(NULL != p_cta_output);

   /* Log CTA ouput. */
   CTA_STORE_VAL_MGR_WPR("cta_out_alert_level_right", p_cta_output->cta_alert_level_right);
   CTA_STORE_VAL_MGR_WPR("cta_out_alert_level_left", p_cta_output->cta_alert_level_left);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_zone_left", p_cta_output->cta_stla_crit_zone_right);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_zone_right", p_cta_output->cta_stla_crit_zone_right);
   CTA_STORE_VAL_MGR_WPR("rcp_out_stla_status", p_cta_output->rcp_status);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P0_PositionX", p_cta_output->DBG_Crit_Zone_Right_P0_PositionX);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P0_PositionY", p_cta_output->DBG_Crit_Zone_Right_P0_PositionY);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P1_PositionX", p_cta_output->DBG_Crit_Zone_Right_P1_PositionX);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P1_PositionY", p_cta_output->DBG_Crit_Zone_Right_P1_PositionY);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P2_PositionX", p_cta_output->DBG_Crit_Zone_Right_P2_PositionX);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P2_PositionY", p_cta_output->DBG_Crit_Zone_Right_P2_PositionY);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P3_PositionX", p_cta_output->DBG_Crit_Zone_Right_P3_PositionX);
   CTA_STORE_VAL_MGR_WPR("cta_out_stla_DBG_Crit_Zone_Right_P3_PositionY", p_cta_output->DBG_Crit_Zone_Right_P3_PositionY);
}
#endif /* BINARY_DEBUG */
