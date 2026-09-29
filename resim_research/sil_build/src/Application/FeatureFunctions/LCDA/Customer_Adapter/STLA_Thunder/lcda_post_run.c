/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the STLA_Thunder post run logic for LCDA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_field_of_interest.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_types.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>
#include <string.h>

#ifdef BINARY_DEBUG
#include "lcda_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

#define LCDA_MIN_HOST_SPEED_FOR_HOLDING_ALERT (0.278f)

/* Lcda_Processed_Module */
#define BSW (0u)
#define CVW (1u)

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Calculate position of the zone exit point. If it is in zone range, object is saved as predicted as stil valid, and
 * trigger an alert
 *
 * @return
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */

static void Lcda_Hold_Alert(const Pa_Data_T *p_pa_data,
                            const Lcda_Core_Calibration_T *p_cals,
                            const Fbk_Field_Of_Interest_T *zone,
                            const uint8_t mod,
                            const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table,
                            Lcda_Output_T *p_lcda_output);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output)
{
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memset function since it is not required.] */
   memset(p_lcda_output, 0, sizeof(Lcda_Output_T));
}

void Lcda_Post_Run_Init(void)
{
}


/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   const Lcda_Core_Calibration_T *p_cals;
   const Lcda_Core_Output_T *p_lcda_core_output;
   const Lcda_Core_Input_T *p_lcda_core_input;
   const Pa_Data_T *p_pa_data;
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   uint8_t side;
   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);
   assert(NULL != p_fbk_output);
   p_cals             = &p_lcda_instance->calibration;
   p_lcda_core_output = &p_lcda_instance->core_output;
   p_lcda_core_input  = &p_lcda_instance->core_input;
   p_pa_data          = p_lcda_core_input->p_pa_data;
   p_vehicle_data     = &p_pa_data->vehicle_data;

   p_lcda_output->f_bsw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->bsw_core_output.f_bsw_is_enabled);
   p_lcda_output->f_cvw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->cvw_core_output.f_cvw_is_enabled);
   p_lcda_output->lcda_status   = p_lcda_core_output->lcda_status;

   /* The alert will remain issued until either the host vehicle reaches a lower minimum speed (1 kph) or conditions for the alert
    * are no longer satisfied - object is no longer in zone*/
   if ((LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED == p_lcda_core_output->lcda_status)
       && (p_vehicle_data->host_speed > LCDA_MIN_HOST_SPEED_FOR_HOLDING_ALERT))
   {

      p_lcda_output->f_lcda_enabled = FBK_ZERO_UINT;

      /* BSW */
      for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
      {
         Lcda_Hold_Alert(p_pa_data, p_cals, &p_lcda_core_input->initial_bsw_zone_hys, BSW, p_fbk_output->p_index_id_lookup_table,
                         p_lcda_output);
      }

      if (Fbk_Is_False(p_lcda_output->f_bsw_hold_alert[FBK_SIDE_LEFT]))
      {
         p_lcda_output->bsw_distance_left = p_lcda_core_output->bsw_core_output.bsw_distance[FBK_SIDE_LEFT];
         p_lcda_output->bsw_alert_left    = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT];
         p_lcda_output->bsw_id_left       = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_LEFT];
      }

      if (Fbk_Is_False(p_lcda_output->f_bsw_hold_alert[FBK_SIDE_RIGHT]))
      {
         p_lcda_output->bsw_alert_right    = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT];
         p_lcda_output->bsw_id_right       = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];
         p_lcda_output->bsw_distance_right = p_lcda_core_output->bsw_core_output.bsw_distance[FBK_SIDE_RIGHT];
      }

      /* CVW */
      for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
      {
         Lcda_Hold_Alert(p_pa_data, p_cals, &p_lcda_core_input->initial_cvw_zone_hys, CVW, p_fbk_output->p_index_id_lookup_table,
                         p_lcda_output);
      }

      if (Fbk_Is_False(p_lcda_output->f_cvw_hold_alert[FBK_SIDE_LEFT]))
      {
         p_lcda_output->cvw_distance_left = p_lcda_core_output->cvw_core_output.cvw_distance[FBK_SIDE_LEFT];
         p_lcda_output->cvw_alert_left    = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT];
         p_lcda_output->cvw_id_left       = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
         p_lcda_output->cvw_ttc_left      = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
      }

      if (Fbk_Is_False(p_lcda_output->f_cvw_hold_alert[FBK_SIDE_RIGHT]))
      {
         p_lcda_output->cvw_alert_right    = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT];
         p_lcda_output->cvw_id_right       = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
         p_lcda_output->cvw_ttc_right      = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
         p_lcda_output->cvw_distance_right = p_lcda_core_output->cvw_core_output.cvw_distance[FBK_SIDE_RIGHT];
      }
   }
   /* Fill customer output from core output */
   else
   {
      if (LCDA_STATUS_ACTIVE == p_lcda_core_output->lcda_status)
      {
         p_lcda_output->f_lcda_enabled = FBK_ONE_UINT;
      }

      p_lcda_output->bsw_alert_left                   = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT];
      p_lcda_output->bsw_alert_right                  = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT];
      p_lcda_output->bsw_id_left                      = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_LEFT];
      p_lcda_output->bsw_id_right                     = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];
      p_lcda_output->f_bsw_hold_alert[FBK_SIDE_LEFT]  = FBK_FALSE;
      p_lcda_output->f_bsw_hold_alert[FBK_SIDE_RIGHT] = FBK_FALSE;
      p_lcda_output->bsw_distance_left                = p_lcda_core_output->bsw_core_output.bsw_distance[FBK_SIDE_LEFT];
      p_lcda_output->bsw_distance_right               = p_lcda_core_output->bsw_core_output.bsw_distance[FBK_SIDE_RIGHT];

      p_lcda_output->cvw_distance_left                = p_lcda_core_output->cvw_core_output.cvw_distance[FBK_SIDE_LEFT];
      p_lcda_output->cvw_alert_left                   = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT];
      p_lcda_output->cvw_id_left                      = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
      p_lcda_output->cvw_ttc_left                     = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
      p_lcda_output->f_cvw_hold_alert[FBK_SIDE_LEFT]  = FBK_FALSE;
      p_lcda_output->f_cvw_hold_alert[FBK_SIDE_RIGHT] = FBK_FALSE;
      p_lcda_output->cvw_alert_right                  = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT];
      p_lcda_output->cvw_id_right                     = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
      p_lcda_output->cvw_ttc_right                    = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
      p_lcda_output->cvw_distance_right               = p_lcda_core_output->cvw_core_output.cvw_distance[FBK_SIDE_RIGHT];
   }

#ifdef BINARY_DEBUG
   Write_Lcda_Output(p_lcda_output);
#endif
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Hold_Alert(const Pa_Data_T *p_pa_data,
                            const Lcda_Core_Calibration_T *p_cals,
                            const Fbk_Field_Of_Interest_T *zone,
                            const uint8_t mod,
                            const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table,
                            Lcda_Output_T *p_lcda_output)
{
   /* Set the bsw zone lateral coordinates */
   boolean_T *hold_alert;
   float32_T cross_point_y_min;
   float32_T cross_point_y_max;
   float32_T cross_point_x[FBK_NUMBER_OF_SIDES];
   float32_T obj_front_pos;
   float32_T obj_ref_point_x;
   float32_T obj_ref_point_y;
   uint8_t side;
   uint8_t obj_id;
   uint8_t obj_idx;
   uint8_t bsw_alert_ids[FBK_NUMBER_OF_SIDES];
   uint8_t cvw_alert_ids[FBK_NUMBER_OF_SIDES];
   uint8_t *id_alert_left;
   uint8_t *id_alert_right;
   uint8_t *object_ids;

   assert(NULL != p_pa_data);
   assert(NULL != p_cals);
   assert(NULL != zone);
   assert(NULL != p_lcda_output);

   if (BSW == mod)
   {
      object_ids     = bsw_alert_ids;
      hold_alert     = p_lcda_output->f_bsw_hold_alert;
      id_alert_left  = &p_lcda_output->bsw_id_left;
      id_alert_right = &p_lcda_output->bsw_id_right;
   }
   else
   {
      object_ids     = cvw_alert_ids;
      hold_alert     = p_lcda_output->f_cvw_hold_alert;
      id_alert_left  = &p_lcda_output->cvw_id_left;
      id_alert_right = &p_lcda_output->cvw_id_right;
   }

   object_ids[FBK_SIDE_LEFT]  = *id_alert_left;
   object_ids[FBK_SIDE_RIGHT] = *id_alert_right;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_id  = object_ids[side];
      obj_idx = Fbk_Get_Object_Index_From_Id(p_index_id_lookup_table, obj_id);

      /* Check if object is not invalid*/
      if ((PA_INVALID_OBJ_INDEX != obj_idx) && (PA_OBJ_STATUS_INVALID != p_pa_data->object_data[obj_idx].status)
          && (p_pa_data->object_data[obj_idx].vcs_vel.x > p_cals->k_bsw_min_obj_long_vel))
      {

         obj_ref_point_x = p_pa_data->object_data[obj_idx].curvi_pos.x;
         obj_ref_point_y = Fbk_Abs_F(p_pa_data->object_data[obj_idx].curvi_pos.y);
         obj_front_pos   = obj_ref_point_x + Fbk_Half(p_pa_data->object_data[obj_idx].length);

         cross_point_y_max             = zone->points[FRONT_OUTER_SIDE].y + Fbk_Half(p_pa_data->object_data[obj_idx].width);
         cross_point_y_min             = zone->points[FRONT_EGO_SIDE].y + Fbk_Half(p_pa_data->object_data[obj_idx].width);
         cross_point_x[FBK_SIDE_FRONT] = zone->points[FRONT_EGO_SIDE].x;
         cross_point_x[FBK_SIDE_REAR]  = zone->points[REAR_OUTER_SIDE].x;

         /* Updates current and status of the object, if it is considered as a long or short object. */
         if (p_pa_data->object_data[obj_idx].length >= p_cals->k_bsw_min_length_long_object)
         {
            obj_ref_point_x -= Fbk_Half(p_pa_data->object_data[obj_idx].length);
         }
         else
         {
            obj_ref_point_x += Fbk_Half(p_pa_data->object_data[obj_idx].length);
         }


         /* Calculate if specified reference point is still in the zone*/
         if ((obj_ref_point_x > cross_point_x[FBK_SIDE_FRONT]) || (obj_front_pos < cross_point_x[FBK_SIDE_REAR])
             || (obj_ref_point_y > cross_point_y_max) || (obj_ref_point_y < cross_point_y_min))
         {
            hold_alert[side] = FBK_FALSE;
         }
         else
         {
            hold_alert[side] = FBK_TRUE;
            if (side == FBK_SIDE_LEFT)
            {
               *id_alert_left = obj_id;
            }
            else
            {
               *id_alert_right = obj_id;
            }
         }
      }
   }
}

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output)
{
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_lcda_enabled", p_lcda_output->f_lcda_enabled);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_bsw_enabled", p_lcda_output->f_bsw_enabled);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_cvw_enabled", p_lcda_output->f_cvw_enabled);

   LCDA_STORE_VAL_MGR_WPR("stla_thunder_bsw_alert_left", p_lcda_output->bsw_alert_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_bsw_id_left", p_lcda_output->bsw_id_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_bsw_alert_right", p_lcda_output->bsw_alert_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_bsw_id_right", p_lcda_output->bsw_id_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_bsw_hold_alert_left", p_lcda_output->f_bsw_hold_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_bsw_hold_alert_right", p_lcda_output->f_bsw_hold_alert[FBK_SIDE_REAR]);

   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_alert_left", p_lcda_output->cvw_alert_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_id_left", p_lcda_output->cvw_id_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_ttc_left", p_lcda_output->cvw_ttc_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_alert_right", p_lcda_output->cvw_alert_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_id_right", p_lcda_output->cvw_id_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_ttc_right", p_lcda_output->cvw_ttc_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_cvw_hold_alert_left", p_lcda_output->f_cvw_hold_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_f_cvw_hold_alert_right", p_lcda_output->f_cvw_hold_alert[FBK_SIDE_REAR]);

   LCDA_STORE_VAL_MGR_WPR("stla_thunder_bsw_distance_left", p_lcda_output->bsw_distance_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_bsw_distance_right", p_lcda_output->bsw_distance_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_distance_left", p_lcda_output->cvw_distance_left);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_cvw_distance_right", p_lcda_output->cvw_distance_right);
   LCDA_STORE_VAL_MGR_WPR("stla_thunder_lcda_status", p_lcda_output->lcda_status);
}
#endif
