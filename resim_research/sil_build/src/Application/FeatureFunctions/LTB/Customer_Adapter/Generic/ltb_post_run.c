/**
 * @file ltb_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ltb_post_run.h"
#include "fbk_macros.h"
#include "ltb_core_output_t.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ltb_debug_writer.h"
#endif /* BINARY_DEBUG */

/*============================================================================*\
* EXPORTED FUNCTIONS
\*============================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ltb_Output(const Ltb_Output_T *p_ltb_output);
#endif /* BINARY_DEBUG */

// clang-format off
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ltb_Post_Run(const Ltb_Instance_T *p_ltb_instance, Ltb_Output_T *p_ltb_output, const Ltb_Input_T *p_ltb_input)
// clang-format on
{
   uint8_t side_index;
   const Ltb_Core_Output_T *p_ltb_core_output;
   assert(NULL != p_ltb_instance);
   assert(NULL != p_ltb_output);
   assert(NULL != p_ltb_input);

   p_ltb_core_output = &p_ltb_instance->core_output;

   for (side_index = FBK_ZERO_INT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {

      /* Update object properties */
      p_ltb_output->ltb_object[side_index].ltb_id                  = p_ltb_core_output->ltb_id[side_index];
      p_ltb_output->ltb_object[side_index].ltb_ttc_s               = p_ltb_core_output->ltb_ttc[side_index];
      p_ltb_output->ltb_object[side_index].ltb_ttb_s               = p_ltb_core_output->ltb_ttb[side_index];
      p_ltb_output->ltb_object[side_index].ltb_decel_estimate_mps2 = p_ltb_core_output->ltb_decel_estimate[side_index];
      p_ltb_output->ltb_object[side_index].ltb_distance_m          = p_ltb_core_output->ltb_distance[side_index];

      /* Update alert level properties */
      p_ltb_output->ltb_alert_level[side_index] = p_ltb_core_output->ltb_alert_level[side_index];
   }
   p_ltb_output->ltb_most_critical_side = p_ltb_core_output->ltb_most_critical_side;

#ifdef BINARY_DEBUG
   Write_Ltb_Output(p_ltb_output);
#endif
}

#ifdef BINARY_DEBUG
static void Write_Ltb_Output(const Ltb_Output_T *p_ltb_output)
{
   /* check input parameters */
   assert(NULL != p_ltb_output);

   /* Log Safe Exit specific data*/
   LTB_STORE_VAL_MGR_WPR("LTB_obj_id_left", p_ltb_output->ltb_object[FBK_SIDE_LEFT].ltb_id);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_id_right", p_ltb_output->ltb_object[FBK_SIDE_RIGHT].ltb_id);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_ttc_s_left", p_ltb_output->ltb_object[FBK_SIDE_LEFT].ltb_ttc_s);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_ttc_s_right", p_ltb_output->ltb_object[FBK_SIDE_RIGHT].ltb_ttc_s);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_ttb_s_left", p_ltb_output->ltb_object[FBK_SIDE_LEFT].ltb_ttb_s);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_ttb_s_right", p_ltb_output->ltb_object[FBK_SIDE_RIGHT].ltb_ttb_s);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_decel_estimate_mps2_left", p_ltb_output->ltb_object[FBK_SIDE_LEFT].ltb_decel_estimate_mps2);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_decel_estimate_mps2_right", p_ltb_output->ltb_object[FBK_SIDE_RIGHT].ltb_decel_estimate_mps2);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_distance_m_left", p_ltb_output->ltb_object[FBK_SIDE_LEFT].ltb_distance_m);
   LTB_STORE_VAL_MGR_WPR("LTB_obj_distance_m_right", p_ltb_output->ltb_object[FBK_SIDE_RIGHT].ltb_distance_m);
   LTB_STORE_VAL_MGR_WPR("LTB_alert_level_left", p_ltb_output->ltb_alert_level[FBK_SIDE_LEFT]);
   LTB_STORE_VAL_MGR_WPR("LTB_alert_level_right", p_ltb_output->ltb_alert_level[FBK_SIDE_RIGHT]);
   LTB_STORE_VAL_MGR_WPR("LTB_most_critical_side", p_ltb_output->ltb_most_critical_side);
}
#endif /* BINARY_DEBUG */
