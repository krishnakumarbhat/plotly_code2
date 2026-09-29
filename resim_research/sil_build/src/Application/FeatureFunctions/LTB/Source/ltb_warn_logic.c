/**
 * @file ltb_warn_logic.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements logic for determining if a warning or info is determined by the LTB algorithm.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_warn_logic.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "ltb_common_functions.h"
#include "ltb_types.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/

/**
 * @brief Fills the LTB core output struct using current object information.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53945}
 * @verification{Create a test to check whether all necessary attributes of p_ltb_object are correctly transferred to core output.}
 */
static void Ltb_Fill_Core_Output_With_Current_Obj(Ltb_Core_Output_T *p_ltb_core_output /**< LTB Core Output */,
                                                  const Ltb_Object_T *p_ltb_object /**< LTB Object */,
                                                  const uint8_t side_index /**< index of ego side */);

/**
 * @brief Sets the alert level based on TTC.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53946}
 * @verification{Check that for different ttcs different alert levels are applied.}
 */
static void Ltb_Set_Ttc_Based_Alert_Level(Ltb_Object_T *p_ltb_object, /**<[in, out] LTB object*/
                                          const Ltb_Core_Calibration_T *p_ltb_cal /**<[in] Calibration parameters*/);

/**
 * @brief Sets the alert side based on objects position.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53947}
 * @verification{Check that when the lateral position component of the object is greater than zero that the alert side is set to
 * FBK_SIDE_RIGHT and FBK_SIDE_LEFT otherwise. The alert side shall only be set when an alert level is given.}
 */
static void Ltb_Set_Obj_Position_Based_Alert_Side(Ltb_Object_T *p_ltb_object /**<[in, out] LTB object*/);

/**
 * @brief Sets the alert side based on objects position.
 *
 * @return void
 *
 * @SRS{CSCSA-30390}
 * @SAE{CSCSA-70554}
 * @SDD{CSCSA-65659}
 * @verification{Create tests where outputs need to qualify for an alert for a successive amount of
 * cycles. Only if conditions are not fulfilled, the LTB core output shall be reset.}
 */
static void Ltb_Qualifying_Alert(Ltb_Core_Output_T *p_ltb_core_output /**< LTB Core Output */,
                                 Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent */,
                                 int32_t alert_level_delta /** Level difference between consecutive alerts */,
                                 uint8_t side_index /**< Ego side */,
                                 const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);

/**
 * @brief Sets the alert side based on objects position.
 *
 * @return void
 *
 * @SRS{CSCSA-30398}
 * @SAE{CSCSA-70554}
 * @SDD{CSCSA-65660}
 * @verification{Create tests where outputs are held for a successive amount of
 * cycles. Only if conditions are not fulfilled, the LTB core output shall be reset.}
 */
static void Ltb_Holding_Alert(Ltb_Core_Output_T *p_ltb_core_output /**< LTB Core Output */,
                              Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent */,
                              int32_t alert_level_delta /** Level difference between consecutive alerts */,
                              uint8_t side_index /**< Ego side */,
                              const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/

static void Ltb_Fill_Core_Output_With_Current_Obj(Ltb_Core_Output_T *p_ltb_core_output,
                                                  const Ltb_Object_T *p_ltb_object,
                                                  const uint8_t side_index)
{
   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_object);
   assert((uint8_t) FBK_SIDE_UNDEFINED > side_index);

   /* Fill LTB Core Output */
   p_ltb_core_output->ltb_id[side_index]                    = p_ltb_object->tracker_data.id;
   p_ltb_core_output->ltb_index[side_index]                 = p_ltb_object->tracker_data.index;
   p_ltb_core_output->ltb_alert_level[side_index]           = p_ltb_object->attributes.alert_level;
   p_ltb_core_output->ltb_ttc[side_index]                   = p_ltb_object->attributes.ttc;
   p_ltb_core_output->ltb_ttb[side_index]                   = p_ltb_object->attributes.ttb;
   p_ltb_core_output->ltb_decel_estimate[side_index]        = p_ltb_object->attributes.decel_to_avoid_coll;
   p_ltb_core_output->ltb_distance[side_index]              = p_ltb_object->attributes.distance_to_ego;
   p_ltb_core_output->ltb_waypoint_at_collision[side_index] = p_ltb_object->attributes.waypoint_at_collision;

   /* Fill zone flags */
   p_ltb_core_output->ltb_f_obj_in_zone[side_index] = p_ltb_object->attributes.f_obj_in_zone;
}


static void Ltb_Set_Ttc_Based_Alert_Level(Ltb_Object_T *p_ltb_object, const Ltb_Core_Calibration_T *p_ltb_cal)
{
   assert(NULL != p_ltb_object);
   assert(NULL != p_ltb_cal);

   if ((LTB_INVALID_TTC > p_ltb_object->attributes.ttc) && (p_ltb_object->attributes.ttc <= p_ltb_cal->k_ltb_alert_lvl_3_ttc_threshold)
       && (p_ltb_object->attributes.decel_to_avoid_coll >= p_ltb_cal->k_ltb_alert_lvl_3_decel_threshold))
   {
      p_ltb_object->attributes.alert_level = ALERT_ACTIVE_LEVEL_3;
   }
   else if ((LTB_INVALID_TTC > p_ltb_object->attributes.ttc)
            && (p_ltb_object->attributes.ttc <= p_ltb_cal->k_ltb_alert_lvl_2_ttc_threshold)
            && (p_ltb_object->attributes.ttb <= p_ltb_cal->k_ltb_alert_lvl_2_ttb_threshold))
   {
      p_ltb_object->attributes.alert_level = ALERT_ACTIVE_LEVEL_2;
   }
   else if ((LTB_INVALID_TTC > p_ltb_object->attributes.ttc)
            && (p_ltb_object->attributes.ttc <= p_ltb_cal->k_ltb_alert_lvl_1_ttc_threshold))
   {
      p_ltb_object->attributes.alert_level = ALERT_ACTIVE_LEVEL_1;
   }
   else
   {
      /* Object does not qualify for any alert */
   }
}

static void Ltb_Set_Obj_Position_Based_Alert_Side(Ltb_Object_T *p_ltb_object)
{
   assert(NULL != p_ltb_object);

   if (NO_ALERT != p_ltb_object->attributes.alert_level)
   {
      if (ALERT_ACTIVE_LEVEL_1 == p_ltb_object->attributes.alert_level)
      {
         p_ltb_object->attributes.alert_side =
            Fbk_Get_Obj_Side_Coord_Sys(&p_ltb_object->tracker_data, p_ltb_object->attributes.f_curvi_available);
      }
      else
      {
         p_ltb_object->attributes.alert_side = Fbk_Get_Obj_Side_Coord_Sys(&p_ltb_object->tracker_data, FBK_FALSE);
      }
   }
}

static void Ltb_Qualifying_Alert(Ltb_Core_Output_T *p_ltb_core_output,
                                 Ltb_Persistent_T *p_ltb_persistent,
                                 int32_t alert_level_delta,
                                 uint8_t side_index,
                                 const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);

   /* Qualifying alert */
   if ((ALERT_ACTIVE_LEVEL_1 == p_ltb_core_output->ltb_alert_level[side_index])
       || (ALERT_ACTIVE_LEVEL_2 == p_ltb_core_output->ltb_alert_level[side_index]))
   {
      /* Increase qualifying counter */
      Sat_Inc_Uint8(&(p_ltb_persistent->ltb_side_alert_qualifying_counter[side_index]));

      if (p_ltb_persistent->ltb_side_alert_qualifying_counter[side_index] <= p_ltb_cal->k_ltb_alert_qualifying_cycles)
      {
         /* Qualifying counter below threshold, thus suppress this alert */
         Ltb_Reset_Core_Output_Side_Data(p_ltb_core_output, side_index);
      }
   }
   else if (ALERT_ACTIVE_LEVEL_3 == p_ltb_core_output->ltb_alert_level[side_index])
   {
      /* Optionally check if alert levels are consecutive */
      if (Fbk_Is_True(p_ltb_cal->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels))
      {
         if (alert_level_delta > 1)
         {
            /* Alert levels not consecutive, thus suppress this alert */
            Ltb_Reset_Core_Output_Side_Data(p_ltb_core_output, side_index);
         }
      }
   }
   else
   {
      /* No active alert in this cycle, thus reset qualification counter */
      p_ltb_persistent->ltb_side_alert_qualifying_counter[side_index] = FBK_ZERO_INT;
   }
}

static void Ltb_Holding_Alert(Ltb_Core_Output_T *p_ltb_core_output,
                              Ltb_Persistent_T *p_ltb_persistent,
                              int32_t alert_level_delta,
                              uint8_t side_index,
                              const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);

   /* Check required conditions */
   if ((p_ltb_core_output->ltb_alert_level[side_index] < p_ltb_persistent->ltb_side_alert_prev_cycle[side_index])
       && ((p_ltb_persistent->ltb_side_alert_prev_cycle[side_index] < ALERT_ACTIVE_LEVEL_3) || (alert_level_delta < -1)
           || (Fbk_Is_False(p_ltb_cal->k_ltb_f_skip_holding_for_single_alert_level_drop))))
   {
      /* Increase holding counter */
      Sat_Inc_Uint8(&(p_ltb_persistent->ltb_side_alert_holding_counter[side_index]));

      /* Holding counter below threshold, thus hold LTB alert */
      if (p_ltb_persistent->ltb_side_alert_holding_counter[side_index] <= p_ltb_cal->k_ltb_alert_holding_cycles)
      {
         /* Reset LTB core output if object ID changed and overwrite with previous object ID */
         if (p_ltb_core_output->ltb_id[side_index] != p_ltb_persistent->ltb_side_id_prev_cycle[side_index])
         {
            Ltb_Reset_Core_Output_Side_Data(p_ltb_core_output, side_index);
            p_ltb_core_output->ltb_id[side_index] = p_ltb_persistent->ltb_side_id_prev_cycle[side_index];
         }

         /* Overwrite LTB core output alert level with previous alert level */
         p_ltb_core_output->ltb_alert_level[side_index] = p_ltb_persistent->ltb_side_alert_prev_cycle[side_index];
      }
   }
   else
   {
      /* No alert is held in this cycle, thus reset holding counter */
      p_ltb_persistent->ltb_side_alert_holding_counter[side_index] = FBK_ZERO_INT;
   }
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ltb_Set_Object_Criticality(Ltb_Object_T *p_ltb_object, const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Asserts */
   assert(NULL != p_ltb_object);
   assert(NULL != p_ltb_cal);

   /* Check object qualification for the alert levels based on TTC */
   Ltb_Set_Ttc_Based_Alert_Level(p_ltb_object, p_ltb_cal);

   /* Set alert side based on object position */
   Ltb_Set_Obj_Position_Based_Alert_Side(p_ltb_object);
}

void Ltb_Set_Most_Critical_Object_Per_Side(Ltb_Core_Output_T *p_ltb_core_output, const Ltb_Object_T *p_ltb_object)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_object);

   /* Iterate over ego vehicle sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      if (side_index == (uint8_t) p_ltb_object->attributes.alert_side)
      {
         /* LTB overall output */
         if (LTB_INVALID_TTC > p_ltb_object->attributes.ttc)
         {
            if ((p_ltb_object->attributes.ttc < p_ltb_core_output->ltb_ttc[side_index])
                || ((Fbk_Equal_F(p_ltb_object->attributes.ttc, p_ltb_core_output->ltb_ttc[side_index]))
                    && (p_ltb_object->attributes.distance_to_ego < p_ltb_core_output->ltb_distance[side_index])))
            {
               Ltb_Fill_Core_Output_With_Current_Obj(p_ltb_core_output, p_ltb_object, side_index);
            }
         }
      }
   }
}

void Ltb_Set_Most_Critical_Side(Ltb_Core_Output_T *p_ltb_core_output)
{
   /* Asserts */
   assert(NULL != p_ltb_core_output);

   if (p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT] > p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT])
   {
      /* left side has higher alert level */
      p_ltb_core_output->ltb_most_critical_side = FBK_SIDE_LEFT;
   }
   else if (p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT] > p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT])
   {
      /* right side has higher alert level */
      p_ltb_core_output->ltb_most_critical_side = FBK_SIDE_RIGHT;
   }
   else
   {
      /* left and right side have same alert level */
      if ((NO_ALERT != p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT])
          && (NO_ALERT != p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT]))
      {
         /* Alert level is active -> check for lowest TTC side */
         if (p_ltb_core_output->ltb_ttc[FBK_SIDE_LEFT] < p_ltb_core_output->ltb_ttc[FBK_SIDE_RIGHT])
         {
            /* left side has lowest TTC */
            p_ltb_core_output->ltb_most_critical_side = FBK_SIDE_LEFT;
         }
         else if (p_ltb_core_output->ltb_ttc[FBK_SIDE_RIGHT] < p_ltb_core_output->ltb_ttc[FBK_SIDE_LEFT])
         {
            /* right side has lowest TTC */
            p_ltb_core_output->ltb_most_critical_side = FBK_SIDE_RIGHT;
         }
         else
         {
            /* Same TTC on both sides */
            if ((LTB_INVALID_TTC > p_ltb_core_output->ltb_ttc[FBK_SIDE_LEFT])
                && (LTB_INVALID_TTC > p_ltb_core_output->ltb_ttc[FBK_SIDE_RIGHT]))
            {
               /* TTCs are valid */
               if (p_ltb_core_output->ltb_distance[FBK_SIDE_LEFT] < p_ltb_core_output->ltb_distance[FBK_SIDE_RIGHT])
               {
                  /* left side object is closer */
                  p_ltb_core_output->ltb_most_critical_side = FBK_SIDE_LEFT;
               }
               else
               {
                  /* right side object is closer */
                  p_ltb_core_output->ltb_most_critical_side = FBK_SIDE_RIGHT;
               }
            }
         }
      }
   }
}

void Ltb_Debounce_Alert_Level(Ltb_Core_Output_T *p_ltb_core_output,
                              Ltb_Persistent_T *p_ltb_persistent,
                              const Ltb_Core_Calibration_T *p_ltb_cal)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);


   for (side_index = FBK_ZERO_INT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      /* Calculate the difference between the previous and the current alert level */
      int32_t alert_level_delta = ((int32_t) p_ltb_core_output->ltb_alert_level[side_index])
                                  - ((int32_t) p_ltb_persistent->ltb_side_alert_prev_cycle[side_index]);
      /* Qualifying alert logic */
      Ltb_Qualifying_Alert(p_ltb_core_output, p_ltb_persistent, alert_level_delta, side_index, p_ltb_cal);
      /* Holding alert logic  */
      Ltb_Holding_Alert(p_ltb_core_output, p_ltb_persistent, alert_level_delta, side_index, p_ltb_cal);
   }
}
