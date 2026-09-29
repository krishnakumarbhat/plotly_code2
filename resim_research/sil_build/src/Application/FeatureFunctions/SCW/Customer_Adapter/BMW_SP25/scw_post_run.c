/**
 * @file scw_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SP25 post run logic for SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw_post_run.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_checked_rounding.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_input_t.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"
#include "scw_state_machine.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "scw_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* File scope variables
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static float32_T Scw_Last_Lateral_Distance[FBK_NUMBER_OF_SIDES];

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Scw_Output(const Scw_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Transforms BMW SP25 object output for left side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRS{SF-2102}
 * @SAE{SF-2992}
 * @SDD{SF-8166}
 * @verification{}
 */
static void Scw_Transformation_To_BMW_Coord_Sys_Left(Scw_Output_T *p_scw_output /**< SCW Output */,
                                                     const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Transforms BMW SP25 object output for right side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRS{SF-2102}
 * @SAE{SF-2992}
 * @SDD{SF-8171}
 * @verification{}
 */
static void Scw_Transformation_To_BMW_Coord_Sys_Right(Scw_Output_T *p_scw_output /**< SCW Output */,
                                                      const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Transforms BMW SP25 guardrail output for left side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRS{SF-2102}
 * @SAE{SF-2992}
 * @SDD{SF-8170}
 * @verification{}
 */
static void Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Left(Scw_Output_T *p_scw_output /**< SCW Output */);

/**
 * @brief Transforms BMW SP25 guardrail output for right side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRS{SF-2102}
 * @SAE{SF-2992}
 * @SDD{SF-8169}
 * @verification{}
 */
static void Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Right(Scw_Output_T *p_scw_output /**< SCW Output */);

/**
 * @brief Sets dynamic object type information to Scw output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{SF-8183}
 * @verification{}
 */
static void Scw_Set_Dynamic_Object_Output(Scw_Output_T *p_scw_output /**< SCW output */,
                                          const Scw_Instance_T *p_scw_instance /**< SCW instance */,
                                          const uint8_t side /**< Ego side */);


/**
 * @brief Sets dynamic object type information to Scw output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{SF-8182}
 * @verification{}
 */
static void Scw_Set_Guardrail_Object_Output(Scw_Output_T *p_scw_output /**< Scw output */,
                                            const Scw_Instance_T *p_scw_instance /**< Scw instance */,
                                            const uint8_t side /**< Ego side */);

/**
 * @brief Resets SCW output to its default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{SF-8184}
 * @verification{}
 */
static void Scw_Reset_Output(Scw_Output_T *p_scw_output /**< SCW output*/,
                             const Scw_Core_Calibration_T *p_scw_cal /**< SCW calibration */);

/**
 * @brief Resets persistent data of BMW_SP25 SCW.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{SF-8188}
 * @verification{}
 */
static void Scw_Reset_Post_Run_Persistent(
   float32_T persistent_scw_last_lateral_distance[FBK_NUMBER_OF_SIDES] /**< persistent lateral distance data*/);

#ifdef BINARY_DEBUG
static void Write_Scw_Output(const Scw_Output_T *p_scw_output);
#endif


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_scw_instance" points to a non-constant type] */
void Scw_Post_Run_Init(Scw_Instance_T *p_scw_instance)
{
   assert(NULL != p_scw_instance);

   /*Reset Scw persistent data*/
   Scw_Reset_Post_Run_Persistent(Scw_Last_Lateral_Distance);
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Scw_Post_Run(const Scw_Instance_T *p_scw_instance, const Scw_Input_T *p_scw_input, Scw_Output_T *p_scw_output)
/* clang-format on */
{
   uint8_t side;
   const SCW_States_T *p_scw_status = Scw_Get_State_Output_Ptr();
   const Scw_Core_Calibration_T *p_scw_cal;

   /* Asserts */
   assert(NULL != p_scw_instance);
   assert(NULL != p_scw_input);
   assert(NULL != p_scw_output);

   p_scw_cal = &p_scw_instance->calibration;

   /* Reset the complete output to its default. */
   Scw_Reset_Output(p_scw_output, p_scw_cal);

   p_scw_output->f_scw_enabled           = p_scw_input->f_scw_enable;
   p_scw_output->f_scw_dyn_enabled       = p_scw_input->f_scw_enable_dynamic;
   p_scw_output->f_scw_guardrail_enabled = p_scw_input->f_scw_enable_guardrail;

   if (*p_scw_status != SCW_STATE_ACTIVE)
   {
      Scw_Reset_Output(p_scw_output, p_scw_cal);
   }
   else
   {
      for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
      {
         /* In case of any non default object type, update the respective sides object properties. */
         if (SCW_OBJECT_TYPE_DYNAMIC == p_scw_instance->core_output.obj_type[side])
         {
            Scw_Set_Dynamic_Object_Output(p_scw_output, p_scw_instance, side);
         }
         else if (SCW_OBJECT_TYPE_GUARDRAIL == p_scw_instance->core_output.obj_type[side])
         {
            Scw_Set_Guardrail_Object_Output(p_scw_output, p_scw_instance, side);
         }
         else
         {
            /*Keep the previously set default for the respective side*/
         }
      }
   }
   p_scw_output->scw_status = *p_scw_status;
   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Scw_Output(p_scw_output);
#endif
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Scw_Set_Dynamic_Object_Output(Scw_Output_T *p_scw_output, const Scw_Instance_T *p_scw_instance, const uint8_t side)
{
   const Scw_Core_Output_T *p_core_output;
   const Fbk_Object_Data_T *p_object_data;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   if ((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side))
   {
      p_core_output  = &(p_scw_instance->core_output);
      p_object_data  = &(p_scw_instance->core_input.p_pa_data->object_data[p_core_output->obj_index[side]]);
      p_vehicle_data = &(p_scw_instance->core_input.p_pa_data->vehicle_data);

      if (FBK_SIDE_LEFT == side)
      {
         p_scw_output->scw_object_type_left                  = (uint8_t) p_core_output->obj_type[side];
         p_scw_output->scw_object_id_left                    = p_core_output->obj_id[side];
         p_scw_output->scw_object_unique_id_left             = p_core_output->obj_unique_id[side];
         p_scw_output->scw_object_px_left                    = p_object_data->vcs_pos.x;
         p_scw_output->scw_object_py_left                    = p_object_data->vcs_pos.y;
         p_scw_output->scw_object_width_left                 = p_object_data->width;
         p_scw_output->scw_object_length_left                = p_object_data->length;
         p_scw_output->scw_object_heading_left               = p_object_data->vcs_heading;
         p_scw_output->scw_object_yawrate_left               = p_object_data->heading_rate;
         p_scw_output->scw_object_ttc_left                   = FBK_ZERO_F;
         p_scw_output->scw_object_vx_left                    = p_object_data->vcs_vel.x;
         p_scw_output->scw_object_vy_left                    = p_object_data->vcs_vel.y;
         p_scw_output->scw_object_ax_left                    = p_object_data->vcs_accel.x;
         p_scw_output->scw_object_ay_left                    = p_object_data->vcs_accel.y;
         p_scw_output->scw_object_existance_probability_left = (uint16_t) Ml_Roundf((100.0f * p_object_data->existence_probability));
         p_scw_output->scw_object_age_left                   = p_object_data->age;
         p_scw_output->scw_object_dy_left                    = p_core_output->obj_lateral_distance[side];
         p_scw_output->scw_object_lateral_ttc_left           = p_core_output->obj_lateral_ttc[side];
         p_scw_output->scw_object_ttp_left                   = p_core_output->obj_ttp[side];
         p_scw_output->scw_object_ttle_left                  = p_core_output->obj_ttle[side];
         p_scw_output->scw_alert_level_left                  = p_core_output->alert_level[side];
         Scw_Transformation_To_BMW_Coord_Sys_Left(p_scw_output, p_vehicle_data);
      }
      else
      {
         p_scw_output->scw_object_type_right                  = (uint8_t) p_core_output->obj_type[side];
         p_scw_output->scw_object_id_right                    = p_core_output->obj_id[side];
         p_scw_output->scw_object_unique_id_right             = p_core_output->obj_unique_id[side];
         p_scw_output->scw_object_px_right                    = p_object_data->vcs_pos.x;
         p_scw_output->scw_object_py_right                    = p_object_data->vcs_pos.y;
         p_scw_output->scw_object_width_right                 = p_object_data->width;
         p_scw_output->scw_object_length_right                = p_object_data->length;
         p_scw_output->scw_object_heading_right               = p_object_data->vcs_heading;
         p_scw_output->scw_object_yawrate_right               = p_object_data->heading_rate;
         p_scw_output->scw_object_ttc_right                   = FBK_ZERO_F;
         p_scw_output->scw_object_vx_right                    = p_object_data->vcs_vel.x;
         p_scw_output->scw_object_vy_right                    = p_object_data->vcs_vel.y;
         p_scw_output->scw_object_ax_right                    = p_object_data->vcs_accel.x;
         p_scw_output->scw_object_ay_right                    = p_object_data->vcs_accel.y;
         p_scw_output->scw_object_existance_probability_right = (uint16_t) Ml_Roundf((100.0f * p_object_data->existence_probability));
         p_scw_output->scw_object_age_right                   = p_object_data->age;
         p_scw_output->scw_object_dy_right                    = p_core_output->obj_lateral_distance[side];
         p_scw_output->scw_object_lateral_ttc_right           = p_core_output->obj_lateral_ttc[side];
         p_scw_output->scw_object_ttp_right                   = p_core_output->obj_ttp[side];
         p_scw_output->scw_object_ttle_right                  = p_core_output->obj_ttle[side];
         p_scw_output->scw_alert_level_right                  = p_core_output->alert_level[side];
         Scw_Transformation_To_BMW_Coord_Sys_Right(p_scw_output, p_vehicle_data);
      }
   }
}
static void Scw_Set_Guardrail_Object_Output(Scw_Output_T *p_scw_output, const Scw_Instance_T *p_scw_instance, const uint8_t side)
{
   const Scw_Core_Output_T *p_core_output;
   const Fbk_Guardrail_Data_T *p_guardrail_data;

   if ((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side))
   {
      p_core_output    = &(p_scw_instance->core_output);
      p_guardrail_data = &(p_scw_instance->core_input.p_pa_data->guardrail_data[side]);

      if (FBK_SIDE_LEFT == side)
      {
         p_scw_output->scw_object_type_left      = (uint8_t) p_core_output->obj_type[side];
         p_scw_output->scw_object_id_left        = FBK_ZERO_UINT;
         p_scw_output->scw_object_unique_id_left = FBK_ZERO_UINT;
         p_scw_output->scw_object_px_left        = FBK_ZERO_F;
         p_scw_output->scw_object_py_left        = p_guardrail_data->lat_pos;
         p_scw_output->scw_object_width_left     = FBK_ZERO_F;
         p_scw_output->scw_object_length_left    = FBK_ZERO_F;
         p_scw_output->scw_object_heading_left   = FBK_ZERO_F;
         p_scw_output->scw_object_yawrate_left   = FBK_ZERO_F;
         p_scw_output->scw_object_ttc_left       = FBK_ZERO_F;
         p_scw_output->scw_object_vx_left        = FBK_ZERO_F;
         p_scw_output->scw_object_vy_left        = p_core_output->obj_lateral_velocity[side];
         p_scw_output->scw_object_ax_left        = FBK_ZERO_F;
         p_scw_output->scw_object_ay_left        = p_core_output->obj_lateral_acceleration[side];
         p_scw_output->scw_object_existance_probability_left =
            (uint16_t) Ml_Roundf((100.0f * p_guardrail_data->existence_probability));
         p_scw_output->scw_object_age_left         = p_guardrail_data->age;
         p_scw_output->scw_object_dy_left          = p_core_output->obj_lateral_distance[side];
         p_scw_output->scw_object_lateral_ttc_left = p_core_output->obj_lateral_ttc[side];
         p_scw_output->scw_object_ttp_left         = p_core_output->obj_ttp[side];
         p_scw_output->scw_object_ttle_left        = p_core_output->obj_ttle[side];
         p_scw_output->scw_alert_level_left        = p_core_output->alert_level[side];
         Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Left(p_scw_output);
      }
      else
      {
         p_scw_output->scw_object_type_right      = (uint8_t) p_core_output->obj_type[side];
         p_scw_output->scw_object_id_right        = FBK_ZERO_UINT;
         p_scw_output->scw_object_unique_id_right = FBK_ZERO_UINT;
         p_scw_output->scw_object_px_right        = FBK_ZERO_F;
         p_scw_output->scw_object_py_right        = p_guardrail_data->lat_pos;
         p_scw_output->scw_object_width_right     = FBK_ZERO_F;
         p_scw_output->scw_object_length_right    = FBK_ZERO_F;
         p_scw_output->scw_object_heading_right   = FBK_ZERO_F;
         p_scw_output->scw_object_yawrate_right   = FBK_ZERO_F;
         p_scw_output->scw_object_ttc_right       = FBK_ZERO_F;
         p_scw_output->scw_object_vx_right        = FBK_ZERO_F;
         p_scw_output->scw_object_vy_right        = p_core_output->obj_lateral_velocity[side];
         p_scw_output->scw_object_ax_right        = FBK_ZERO_F;
         p_scw_output->scw_object_ay_right        = p_core_output->obj_lateral_acceleration[side];
         p_scw_output->scw_object_existance_probability_right =
            (uint16_t) Ml_Roundf((100.0f * p_guardrail_data->existence_probability));
         p_scw_output->scw_object_age_right         = p_guardrail_data->age;
         p_scw_output->scw_object_dy_right          = p_core_output->obj_lateral_distance[side];
         p_scw_output->scw_object_lateral_ttc_right = p_core_output->obj_lateral_ttc[side];
         p_scw_output->scw_object_ttp_right         = p_core_output->obj_ttp[side];
         p_scw_output->scw_object_ttle_right        = p_core_output->obj_ttle[side];
         p_scw_output->scw_alert_level_right        = p_core_output->alert_level[side];
         Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Right(p_scw_output);
      }
   }
}

static void Scw_Reset_Output(Scw_Output_T *p_scw_output, const Scw_Core_Calibration_T *p_scw_cal)
{
   /* Resets flags to their default. */
   p_scw_output->f_scw_enabled           = FBK_ZERO_UINT;
   p_scw_output->f_scw_dyn_enabled       = FBK_ZERO_UINT;
   p_scw_output->f_scw_guardrail_enabled = FBK_ZERO_UINT;
   /* Reset object properties on the left side of ego. */
   p_scw_output->scw_object_type_left                  = FBK_ZERO_UINT;
   p_scw_output->scw_object_id_left                    = FBK_ZERO_UINT;
   p_scw_output->scw_object_unique_id_left             = FBK_ZERO_UINT;
   p_scw_output->scw_object_px_left                    = FBK_ZERO_F;
   p_scw_output->scw_object_py_left                    = FBK_ZERO_F;
   p_scw_output->scw_object_width_left                 = FBK_ZERO_F;
   p_scw_output->scw_object_length_left                = FBK_ZERO_F;
   p_scw_output->scw_object_heading_left               = FBK_ZERO_F;
   p_scw_output->scw_object_yawrate_left               = FBK_ZERO_F;
   p_scw_output->scw_object_ttc_left                   = FBK_ZERO_F;
   p_scw_output->scw_object_vx_left                    = FBK_ZERO_F;
   p_scw_output->scw_object_vy_left                    = FBK_ZERO_F;
   p_scw_output->scw_object_ax_left                    = FBK_ZERO_F;
   p_scw_output->scw_object_ay_left                    = FBK_ZERO_F;
   p_scw_output->scw_object_existance_probability_left = FBK_ZERO_UINT;
   p_scw_output->scw_object_age_left                   = FBK_ZERO_UINT;
   /*Added for BMW Sp25*/
   p_scw_output->scw_object_criticality_left_output   = FBK_ZERO_UINT;
   p_scw_output->scw_object_time_stamp_left_output    = FBK_ZERO_F;
   p_scw_output->scw_object_fallback_time_left_output = FBK_ZERO_F;
   p_scw_output->scw_object_dy_left                   = p_scw_cal->k_scw_lateral_distance_default;
   p_scw_output->scw_object_lateral_ttc_left          = p_scw_cal->k_scw_lateral_ttc_default;
   p_scw_output->scw_object_ttp_left                  = p_scw_cal->k_scw_ttp_default;
   p_scw_output->scw_object_ttle_left                 = p_scw_cal->k_scw_ttle_default;
   p_scw_output->scw_alert_level_left                 = SCW_NO_ALERT;

   /* Reset object properties on the right side of ego. */
   p_scw_output->scw_object_type_right                  = FBK_ZERO_UINT;
   p_scw_output->scw_object_id_right                    = FBK_ZERO_UINT;
   p_scw_output->scw_object_unique_id_right             = FBK_ZERO_UINT;
   p_scw_output->scw_object_px_right                    = FBK_ZERO_F;
   p_scw_output->scw_object_py_right                    = FBK_ZERO_F;
   p_scw_output->scw_object_width_right                 = FBK_ZERO_F;
   p_scw_output->scw_object_length_right                = FBK_ZERO_F;
   p_scw_output->scw_object_heading_right               = FBK_ZERO_F;
   p_scw_output->scw_object_yawrate_right               = FBK_ZERO_F;
   p_scw_output->scw_object_ttc_right                   = FBK_ZERO_F;
   p_scw_output->scw_object_vx_right                    = FBK_ZERO_F;
   p_scw_output->scw_object_vy_right                    = FBK_ZERO_F;
   p_scw_output->scw_object_ax_right                    = FBK_ZERO_F;
   p_scw_output->scw_object_ay_right                    = FBK_ZERO_F;
   p_scw_output->scw_object_existance_probability_right = FBK_ZERO_UINT;
   p_scw_output->scw_object_age_right                   = FBK_ZERO_UINT;

   /*Added for BMW Sp25*/
   p_scw_output->scw_object_criticality_right_output   = FBK_ZERO_UINT;
   p_scw_output->scw_object_time_stamp_right_output    = FBK_ZERO_F;
   p_scw_output->scw_object_fallback_time_right_output = FBK_ZERO_F;
   p_scw_output->scw_object_dy_right                   = p_scw_cal->k_scw_lateral_distance_default;
   p_scw_output->scw_object_lateral_ttc_right          = p_scw_cal->k_scw_lateral_ttc_default;
   p_scw_output->scw_object_ttp_right                  = p_scw_cal->k_scw_ttp_default;
   p_scw_output->scw_object_ttle_right                 = p_scw_cal->k_scw_ttle_default;
   p_scw_output->scw_alert_level_right                 = SCW_NO_ALERT;
}

static void Scw_Transformation_To_BMW_Coord_Sys_Left(Scw_Output_T *p_scw_output, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   p_scw_output->scw_object_px_left      = p_scw_output->scw_object_px_left - p_vehicle_data->rear_axle_position;
   p_scw_output->scw_object_py_left      = -1.0f * p_scw_output->scw_object_py_left;
   p_scw_output->scw_object_vy_left      = -1.0f * p_scw_output->scw_object_vy_left;
   p_scw_output->scw_object_ay_left      = -1.0f * p_scw_output->scw_object_ay_left;
   p_scw_output->scw_object_heading_left = -1.0f * p_scw_output->scw_object_heading_left;
   p_scw_output->scw_object_yawrate_left = -1.0f * p_scw_output->scw_object_yawrate_left;
}

static void Scw_Transformation_To_BMW_Coord_Sys_Right(Scw_Output_T *p_scw_output, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   p_scw_output->scw_object_px_right      = p_scw_output->scw_object_px_right - p_vehicle_data->rear_axle_position;
   p_scw_output->scw_object_py_right      = -1.0f * p_scw_output->scw_object_py_right;
   p_scw_output->scw_object_vy_right      = -1.0f * p_scw_output->scw_object_vy_right;
   p_scw_output->scw_object_ay_right      = -1.0f * p_scw_output->scw_object_ay_right;
   p_scw_output->scw_object_heading_right = -1.0f * p_scw_output->scw_object_heading_right;
   p_scw_output->scw_object_yawrate_right = -1.0f * p_scw_output->scw_object_yawrate_right;
}

static void Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Left(Scw_Output_T *p_scw_output)
{
   p_scw_output->scw_object_py_left      = -1.0f * p_scw_output->scw_object_py_left;
   p_scw_output->scw_object_vy_left      = -1.0f * p_scw_output->scw_object_vy_left;
   p_scw_output->scw_object_ay_left      = -1.0f * p_scw_output->scw_object_ay_left;
   p_scw_output->scw_object_heading_left = -1.0f * p_scw_output->scw_object_heading_left;
   p_scw_output->scw_object_yawrate_left = -1.0f * p_scw_output->scw_object_yawrate_left;
}


static void Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Right(Scw_Output_T *p_scw_output)
{
   p_scw_output->scw_object_py_right      = -1.0f * p_scw_output->scw_object_py_right;
   p_scw_output->scw_object_vy_right      = -1.0f * p_scw_output->scw_object_vy_right;
   p_scw_output->scw_object_ay_right      = -1.0f * p_scw_output->scw_object_ay_right;
   p_scw_output->scw_object_heading_right = -1.0f * p_scw_output->scw_object_heading_right;
   p_scw_output->scw_object_yawrate_right = -1.0f * p_scw_output->scw_object_yawrate_right;
}


static void Scw_Reset_Post_Run_Persistent(float32_T persistent_scw_last_lateral_distance[FBK_NUMBER_OF_SIDES])
{
   assert(NULL != persistent_scw_last_lateral_distance);

   persistent_scw_last_lateral_distance[FBK_SIDE_LEFT]  = FBK_ZERO_F;
   persistent_scw_last_lateral_distance[FBK_SIDE_RIGHT] = FBK_ZERO_F;
}


#ifdef BINARY_DEBUG

static void Write_Scw_Output(const Scw_Output_T *p_scw_output)
{
   /* check input parameters */
   assert(NULL != p_scw_output);

   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_id_left", p_scw_output->scw_object_id_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_x_left", p_scw_output->scw_object_px_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_y_left", p_scw_output->scw_object_py_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_x_left", p_scw_output->scw_object_vx_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_y_left", p_scw_output->scw_object_vy_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_x_left", p_scw_output->scw_object_ax_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_y_left", p_scw_output->scw_object_ay_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_type_left", p_scw_output->scw_object_type_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_exist_prob_left", p_scw_output->scw_object_existance_probability_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_age_left", p_scw_output->scw_object_age_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_heading_left", p_scw_output->scw_object_heading_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_yawrate_left", p_scw_output->scw_object_yawrate_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_length_left", p_scw_output->scw_object_length_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_width_left", p_scw_output->scw_object_width_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_distance_left", p_scw_output->scw_object_dy_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_ttc_left", p_scw_output->scw_object_lateral_ttc_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttp_left", p_scw_output->scw_object_ttp_left);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttle_left", p_scw_output->scw_object_ttle_left);

   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_id_right", p_scw_output->scw_object_id_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_x_right", p_scw_output->scw_object_px_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_y_right", p_scw_output->scw_object_py_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_x_right", p_scw_output->scw_object_vx_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_y_right", p_scw_output->scw_object_vy_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_x_right", p_scw_output->scw_object_ax_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_y_right", p_scw_output->scw_object_ay_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_type_right", p_scw_output->scw_object_type_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_exist_prob_right", p_scw_output->scw_object_existance_probability_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_age_right", p_scw_output->scw_object_age_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_heading_right", p_scw_output->scw_object_heading_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_yawrate_right", p_scw_output->scw_object_yawrate_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_length_right", p_scw_output->scw_object_length_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_width_right", p_scw_output->scw_object_width_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_distance_right", p_scw_output->scw_object_dy_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_ttc_right", p_scw_output->scw_object_lateral_ttc_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttp_right", p_scw_output->scw_object_ttp_right);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttle_right", p_scw_output->scw_object_ttle_right);
}

#endif
