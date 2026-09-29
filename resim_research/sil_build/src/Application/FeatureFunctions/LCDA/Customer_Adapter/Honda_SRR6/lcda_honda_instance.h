#ifndef LCDA_HONDA_INSTANCE_H
#define LCDA_HONDA_INSTANCE_H

/**
 * @file lcda_honda_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the LCDA honda custom adapter instance data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */
#include "fbk_macros.h"
#include "lcda_output_t.h"
#include "reuse.h"
/**
 * @brief Lcda_Honda_Instance_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Lcda_Instance_T" but that tag is never used] */
typedef struct Lcda_Honda_Instance_T
{
   uint8_t bsw_alert_left;
   uint8_t bsw_alert_right;
   LCDA_CUST_SPEC_OUTPUT_T customer_output;

   uint32_t hold_obj_index[LCDA_HONDA_NUMBER_OF_OBJECTS];  /* indexes of the objects which caused alert holding */
   uint8_t hold_alert_level[LCDA_HONDA_NUMBER_OF_OBJECTS]; /* alert level triggered by the held object*/
   LKA_Object_T hold_object[LCDA_HONDA_NUMBER_OF_OBJECTS]; /* current copies of LKA objects, used in hold mode*/
   float32_T hold_time[LCDA_HONDA_NUMBER_OF_OBJECTS]; /* [s] holding time for honda alerts, for both CVW and BSW, right and left*/

   boolean_T f_narrow_beeper_prev_cycle[FBK_NUMBER_OF_SIDES];
   boolean_T f_beeper_prev_cycle[FBK_NUMBER_OF_SIDES];
   boolean_T f_lcda_level_3_alert_prev_cycle[FBK_NUMBER_OF_SIDES];
} Lcda_Honda_Instance_T;

Lcda_Honda_Instance_T *Lcda_Get_Lcda_Honda_Instance(void);
#endif /* LCDA_HONDA_INSTANCE_H */
