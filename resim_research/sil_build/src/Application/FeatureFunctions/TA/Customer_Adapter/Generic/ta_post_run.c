/**
 * @file ta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ta_post_run.h"
#include "fbk_macros.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_core_output_t.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ta_Output(const Ta_Output_T *p_ta_output);
#endif /* BINARY_DEBUG */

/*============================================================================*/
/*
 * EXPORTED FUNCTIONS
 */
/*============================================================================*/

void Ta_Post_Run_Init(void)
{
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation]["p_ta_instance" does not modify the object it points to] */
void Ta_Post_Run(Ta_Instance_T *p_ta_instance, const Ta_Input_T *p_ta_input, Ta_Output_T *p_ta_output)
{
   uint8_t side_index;
   const Ta_Core_Output_T *p_ta_core_output;

   /* Asserts */
   assert(NULL != p_ta_instance);
   assert(NULL != p_ta_input);
   assert(NULL != p_ta_output);

   p_ta_core_output = &p_ta_instance->core_output;

   p_ta_output->f_ta_enable = p_ta_input->f_ta_enable;

   p_ta_output->ta_f_vehicle_state_relevant = p_ta_core_output->ta_f_vehicle_state_relevant;
   p_ta_output->ta_most_critical_side       = p_ta_core_output->ta_most_critical_side;
   p_ta_output->ta_n_valid_objects          = p_ta_core_output->ta_n_valid_objects;
   p_ta_output->ta_n_relevant_objects       = p_ta_core_output->ta_n_relevant_objects;
   p_ta_output->ta_n_critical_objects       = p_ta_core_output->ta_n_critical_objects;

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {

      uint8_t obj_index = p_ta_core_output->ta_index[side_index];

      if (PA_INVALID_OBJ_INDEX != obj_index)
      {
         /* Map object properties */
         p_ta_output->ta_object[side_index].ta_waypoint_at_collision_m = p_ta_core_output->ta_waypoint_at_collision[side_index];
         p_ta_output->ta_object[side_index].ta_ttc_s                   = p_ta_core_output->ta_ttc[side_index];
         p_ta_output->ta_object[side_index].ta_ttp_s                   = p_ta_core_output->ta_ttp[side_index];
         p_ta_output->ta_object[side_index].ta_ttb_s                   = p_ta_core_output->ta_ttb[side_index];
         p_ta_output->ta_object[side_index].ta_decel_estimate_mps2     = p_ta_core_output->ta_decel_estimate[side_index];
         p_ta_output->ta_object[side_index].ta_distance_m              = p_ta_core_output->ta_distance[side_index];

         /* Map alert level properties */
         p_ta_output->ta_alert_level[side_index]     = p_ta_core_output->ta_alert_level[side_index];
         p_ta_output->ta_object[side_index].ta_id    = p_ta_core_output->ta_id[side_index];
         p_ta_output->ta_object[side_index].ta_index = p_ta_core_output->ta_index[side_index];

         /* Map zone flags */
         p_ta_output->ta_object[side_index].ta_f_obj_in_danger_zone = p_ta_core_output->ta_f_obj_in_danger_zone[side_index];
         p_ta_output->ta_object[side_index].ta_f_obj_in_info_zone   = p_ta_core_output->ta_f_obj_in_info_zone[side_index];
         p_ta_output->ta_object[side_index].ta_f_obj_in_wing_zone   = p_ta_core_output->ta_f_obj_in_wing_zone[side_index];
      }
   }

#ifdef BINARY_DEBUG
   Write_Ta_Output(p_ta_output);
#endif
}

#ifdef BINARY_DEBUG
static void Write_Ta_Output(const Ta_Output_T *p_ta_output)
{
   /* check input parameters */
   assert(NULL != p_ta_output);

   /* Log the TA customer output. */
   TA_STORE_VAL_MGR_WPR("TA_Gen_f_ta_enable", p_ta_output->f_ta_enable);
   TA_STORE_VAL_MGR_WPR("TA_Gen_most_critical_side", p_ta_output->ta_most_critical_side);
   TA_STORE_VAL_MGR_WPR("TA_Gen_f_vehicle_state_relevant", p_ta_output->ta_f_vehicle_state_relevant);
   TA_STORE_VAL_MGR_WPR("TA_Gen_n_valid_objects", p_ta_output->ta_n_valid_objects);
   TA_STORE_VAL_MGR_WPR("TA_Gen_n_relevant_objects", p_ta_output->ta_n_relevant_objects);
   TA_STORE_VAL_MGR_WPR("TA_Gen_n_critical_objects", p_ta_output->ta_n_critical_objects);
   TA_STORE_VAL_MGR_WPR("TA_Gen_ta_algorithm_state", p_ta_output->ta_algorithm_state);
   TA_STORE_VAL_MGR_WPR("TA_Gen_left_ta_alert_level", p_ta_output->ta_alert_level[FBK_SIDE_LEFT]);
   TA_STORE_VAL_MGR_WPR("TA_Gen_right_ta_alert_level", p_ta_output->ta_alert_level[FBK_SIDE_RIGHT]);

   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_waypoint_at_collision_m_x",
                        p_ta_output->ta_object[FBK_SIDE_LEFT].ta_waypoint_at_collision_m.x);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_waypoint_at_collision_m_y",
                        p_ta_output->ta_object[FBK_SIDE_LEFT].ta_waypoint_at_collision_m.y);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_ttc_s", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttc_s);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_ttp_s", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttp_s);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_ttb_s", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttb_s);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_decel_estimate_mps2", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_decel_estimate_mps2);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_distance_m", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_distance_m);

   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_id", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_id);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_index", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_index);

   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_f_obj_in_danger_zone", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_f_obj_in_danger_zone);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_f_obj_in_info_zone", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_f_obj_in_info_zone);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_left_ta_f_obj_in_wing_zone", p_ta_output->ta_object[FBK_SIDE_LEFT].ta_f_obj_in_wing_zone);

   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_waypoint_at_collision_m_x",
                        p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_waypoint_at_collision_m.x);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_waypoint_at_collision_m_y",
                        p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_waypoint_at_collision_m.y);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_ttc_s", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttc_s);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_ttp_s", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttp_s);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_ttb_s", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttb_s);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_decel_estimate_mps2", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_decel_estimate_mps2);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_distance_m", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_distance_m);

   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_id", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_id);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_index", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_index);

   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_f_obj_in_danger_zone", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_f_obj_in_danger_zone);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_f_obj_in_info_zone", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_f_obj_in_info_zone);
   TA_STORE_VAL_MGR_WPR("TA_Gen_obj_right_ta_f_obj_in_wing_zone", p_ta_output->ta_object[FBK_SIDE_RIGHT].ta_f_obj_in_wing_zone);
}
#endif /* BINARY_DEBUG */
