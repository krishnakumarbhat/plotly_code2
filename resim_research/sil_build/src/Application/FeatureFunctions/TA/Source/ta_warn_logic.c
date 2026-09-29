/**
 * @file ta_warn_logic.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements logic for determining if a warning or info is determined by the TA algorithm.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_warn_logic.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_constants.h"
#include "ta_object_filter.h"
#include "ta_types.h"
#include <assert.h>

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/

/**
 * @brief Fills the TA core output struct using current object information.
 *
 * @return void
 *
 * @SRS{SF-2323,SF-2332}
 * @SAE{SF-3238}
 * @SDD{SF-8761}
 * @verification{Create a test to check whether all necessary attributes of p_ta_object are correctly transferred to core output.}
 */
static void Ta_Fill_Core_Output_With_Current_Obj(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                                 const Ta_Object_T *p_ta_object /**< TA Object */,
                                                 const uint8_t side_index /**< index of ego side */);

/**
 * @brief Sets the alert side based on objects position.
 *
 * @return void
 *
 * @SRS{SF-2320,SF-2317,SF-2299}
 * @SAE{SF-3238}
 * @SDD{SF-8789}
 * @verification{Check that when the lateral position component of the object is greater than zero that the alert side is set to
 * FBK_SIDE_RIGHT and FBK_SIDE_LEFT otherwise. The alert side shall only be set when an alert level is given.}
 */
static void Ta_Set_Obj_Position_Based_Alert_Side(Ta_Object_T *p_ta_object /**<[in, out] TA object*/);

/**
 * @brief Sets the alert level based on TTC.
 *
 * @return void
 *
 * @SRS{SF-2320,SF-2317,SF-2299}
 * @SAE{SF-3238}
 * @SDD{SF-8788}
 * @verification{Check that for different ttcs different alert levels are applied.}
 */
static void Ta_Set_Ttc_Based_Alert_Level(Ta_Object_T *p_ta_object, /**<[in, out] TA object*/
                                         const Ta_Core_Calibration_T *p_ta_cal /**<[in] Calibration parameters*/);

/**
 * @brief Sets the alert level based on TTP.
 *
 * @return void
 *
 * @SRS{SF-2320,SF-2317,SF-2299}
 * @SAE{SF-3238}
 * @SDD{SF-8790}
 * @verification{Check that for different ttps and whether an object is active different alert levels are applied.}
 */
static void Ta_Set_Ttp_Based_Alert_Level(Ta_Object_T *p_ta_object /**<[in, out] TA object*/,
                                         const Ta_Persistent_T *p_ta_persistent /**<[in] TA persistent data*/,
                                         const float32_T alert_threshold /**< [in] Alert threshold */,
                                         const Ta_Core_Calibration_T *p_ta_cal /**<[in] Calibration parameters*/);

/**
 * @brief Evaluates if an onject is more critical based on TTP and distance.
 *
 * @return true, if p_ta_object is more critical
 *
 * @SRS{SF-2321}
 * @SAE{SF-3238}
 * @SDD{SF-8629}
 * @verification{}
 */
static boolean_T Ta_Is_Ttp_More_Critical(const Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                         const Ta_Object_T *p_ta_object /**< TA object*/,
                                         const Ta_Core_Calibration_T *p_ta_cal /**< Calibration parameters*/,
                                         const uint8_t side_index /**< index of ego side */);

/**
 * @brief Evaluates if an onject is more critical based on TTC and distance.
 *
 * @return true, if p_ta_object is more critical
 *
 * @SRS{SF-2321}
 * @SAE{SF-3238}
 * @SDD{SF-8628}
 * @verification{}
 */
static boolean_T Ta_Is_Ttc_More_Critical(const Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                         const Ta_Object_T *p_ta_object /**< TA object*/,
                                         const uint8_t side_index /**< index of ego side */);


/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/

static void Ta_Fill_Core_Output_With_Current_Obj(Ta_Core_Output_T *p_ta_core_output,
                                                 const Ta_Object_T *p_ta_object,
                                                 const uint8_t side_index)
{
   /* Asserts */
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_object);
   assert((uint8_t) FBK_SIDE_UNDEFINED > side_index);

   /* Fill TA Core Output */
   p_ta_core_output->ta_id[side_index]                    = p_ta_object->tracker_data.id;
   p_ta_core_output->ta_index[side_index]                 = p_ta_object->tracker_data.index;
   p_ta_core_output->ta_alert_level[side_index]           = p_ta_object->attributes.alert_level;
   p_ta_core_output->ta_ttc[side_index]                   = p_ta_object->attributes.ttc;
   p_ta_core_output->ta_ttp[side_index]                   = p_ta_object->attributes.ttp;
   p_ta_core_output->ta_ttb[side_index]                   = p_ta_object->attributes.ttb;
   p_ta_core_output->ta_decel_estimate[side_index]        = p_ta_object->attributes.decel_to_avoid_coll;
   p_ta_core_output->ta_distance[side_index]              = p_ta_object->attributes.distance_to_ego;
   p_ta_core_output->ta_waypoint_at_collision[side_index] = p_ta_object->attributes.waypoint_at_collision;

   /* Fill zone flags */
   p_ta_core_output->ta_f_obj_in_danger_zone[side_index] = p_ta_object->attributes.f_obj_in_danger_zone;
   p_ta_core_output->ta_f_obj_in_info_zone[side_index]   = p_ta_object->attributes.f_obj_in_info_zone;
   p_ta_core_output->ta_f_obj_in_wing_zone[side_index]   = p_ta_object->attributes.f_obj_in_wing_zone;
}

static void Ta_Set_Obj_Position_Based_Alert_Side(Ta_Object_T *p_ta_object)
{
   assert(NULL != p_ta_object);

   if (TA_ALERT_STATE_NONE != p_ta_object->attributes.alert_level)
   {
      if (TA_ALERT_STATE_LEVEL_1 == p_ta_object->attributes.alert_level)
      {
         p_ta_object->attributes.alert_side =
            Fbk_Get_Obj_Side_Coord_Sys(&p_ta_object->tracker_data, p_ta_object->attributes.f_curvi_available);
      }
      else
      {
         p_ta_object->attributes.alert_side = Fbk_Get_Obj_Side_Coord_Sys(&p_ta_object->tracker_data, FBK_FALSE);
      }
   }
}

static void Ta_Set_Ttc_Based_Alert_Level(Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   if (Fbk_Is_False(p_ta_cal->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj)
       || (Fbk_Is_True(p_ta_cal->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj)
           && (PA_OBJ_STATUS_MATURE == p_ta_object->tracker_data.status)))
   {
      if ((TA_INVALID_TTC > p_ta_object->attributes.ttc) && (p_ta_object->attributes.ttc <= p_ta_cal->k_ta_alert_lvl_4_ttc_threshold)
          && (p_ta_object->attributes.decel_to_avoid_coll >= p_ta_cal->k_ta_alert_lvl_4_decel_threshold))
      {
         p_ta_object->attributes.alert_level = TA_ALERT_STATE_LEVEL_4;
      }
      else if ((TA_INVALID_TTC > p_ta_object->attributes.ttc)
               && (p_ta_object->attributes.ttc <= p_ta_cal->k_ta_alert_lvl_3_ttc_threshold)
               && (p_ta_object->attributes.ttb <= p_ta_cal->k_ta_alert_lvl_3_ttb_threshold))
      {
         p_ta_object->attributes.alert_level = TA_ALERT_STATE_LEVEL_3;
      }
      else if ((TA_INVALID_TTC > p_ta_object->attributes.ttc)
               && (p_ta_object->attributes.ttc <= p_ta_cal->k_ta_alert_lvl_2_ttc_threshold))
      {
         p_ta_object->attributes.alert_level = TA_ALERT_STATE_LEVEL_2;
      }
      else
      {
         /* Object does not qualify for any alert level above 1 */
      }
   }
}

static void Ta_Set_Ttp_Based_Alert_Level(Ta_Object_T *p_ta_object,
                                         const Ta_Persistent_T *p_ta_persistent,
                                         const float32_T alert_threshold,
                                         const Ta_Core_Calibration_T *p_ta_cal)
{
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

   if (TA_INVALID_TTP > p_ta_object->attributes.ttp)
   {
      /* Check TTP thresholds */
      if (Fbk_Is_True(p_ta_cal->k_ta_f_apply_ttp_hysteresis_globally)
          && (TA_ALERT_STATE_NONE
              != p_ta_persistent->ta_side_alert_prev_cycle[(
                 uint8_t) (Fbk_Get_Obj_Side_Coord_Sys(&p_ta_object->tracker_data, p_ta_object->attributes.f_curvi_available))])
          && (p_ta_object->attributes.ttp <= (alert_threshold + p_ta_cal->k_ta_active_obj_ttp_offset)))
      {
         /* If the corresponding cal value is set, apply the TTP hysteresis on all objects (instead of the warned object alone). */
         p_ta_object->attributes.alert_level = TA_ALERT_STATE_LEVEL_1;
      }
      else if (Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_BOTH)
               && (p_ta_object->attributes.ttp <= (alert_threshold + p_ta_cal->k_ta_active_obj_ttp_offset)))
      {
         /* For the currently warned object, apply TTP hysteresis. */
         p_ta_object->attributes.alert_level = TA_ALERT_STATE_LEVEL_1;
      }
      else if (Fbk_Is_False(Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_BOTH)) && (p_ta_object->attributes.ttp <= alert_threshold))
      {
         /* Check if TTP threshold is reached. */
         p_ta_object->attributes.alert_level = TA_ALERT_STATE_LEVEL_1;
      }
      else
      {
         /* Object does not qualify for alert level 1 */
      }
   }
}

static boolean_T Ta_Is_Ttp_More_Critical(const Ta_Core_Output_T *p_ta_core_output,
                                         const Ta_Object_T *p_ta_object,
                                         const Ta_Core_Calibration_T *p_ta_cal,
                                         const uint8_t side_index)
{
   boolean_T f_more_critical_ttp;
   if ((TA_INVALID_TTP > p_ta_object->attributes.ttp) && Fbk_Is_True(p_ta_cal->k_rta_f_higher_obj_crit_based_on_lower_ttp)
       && (p_ta_object->attributes.ttp < p_ta_core_output->ta_ttp[side_index]))
   {
      /* Object is deemed more critical based on lower TTP value */
      f_more_critical_ttp = FBK_TRUE;
   }
   else if ((TA_INVALID_TTP > p_ta_object->attributes.ttp) && Fbk_Is_False(p_ta_cal->k_rta_f_higher_obj_crit_based_on_lower_ttp)
            && (p_ta_object->attributes.distance_to_ego < p_ta_core_output->ta_distance[side_index]))
   {
      /* Object is deemed more critical based on lower distance to host vehicle */
      f_more_critical_ttp = FBK_TRUE;
   }
   else
   {
      /* No TTP available for this object */
      f_more_critical_ttp = FBK_FALSE;
   }
   return f_more_critical_ttp;
}

static boolean_T Ta_Is_Ttc_More_Critical(const Ta_Core_Output_T *p_ta_core_output, const Ta_Object_T *p_ta_object, const uint8_t side_index)
{
   boolean_T f_more_critical_ttc;
   if ((TA_INVALID_TTC > p_ta_object->attributes.ttc) && (p_ta_object->attributes.ttc < p_ta_core_output->ta_ttc[side_index]))
   {
      f_more_critical_ttc = FBK_TRUE;
   }
   else if ((TA_INVALID_TTC > p_ta_object->attributes.ttc)
            && (Fbk_Equal_F(p_ta_object->attributes.ttc, p_ta_core_output->ta_ttc[side_index]))
            && (p_ta_object->attributes.distance_to_ego < p_ta_core_output->ta_distance[side_index]))
   {
      /* Same TTC as previous most critical object -> check if distance is smaller */
      f_more_critical_ttc = FBK_TRUE;
   }
   else
   {
      /* Keep current object as most critical for this side */
      f_more_critical_ttc = FBK_FALSE;
   }
   return f_more_critical_ttc;
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Set_Object_Criticality(Ta_Object_T *p_ta_object,
                               const Ta_Core_Input_T *p_ta_core_input,
                               const Ta_Persistent_T *p_ta_persistent,
                               const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   /* Check object qualification for the alert level based on TTP */
   Ta_Set_Ttp_Based_Alert_Level(p_ta_object, p_ta_persistent, p_ta_core_input->alert_ttp_threshold, p_ta_cal);

   /* Check object qualification for the alert levels based on TTC */
   Ta_Set_Ttc_Based_Alert_Level(p_ta_object, p_ta_cal);

   /* Set alert side based on object position */
   Ta_Set_Obj_Position_Based_Alert_Side(p_ta_object);
}

void Ta_Set_Most_Critical_Object_Per_Side(Ta_Core_Output_T *p_ta_core_output,
                                          const Ta_Object_T *p_ta_object,
                                          const Ta_Core_Calibration_T *p_ta_cal)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_object);

   /* Iterate over ego vehicle sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      if (side_index == (uint8_t) p_ta_object->attributes.alert_side)
      {
         /* TA overall output */
         if (p_ta_object->attributes.alert_level >= p_ta_core_output->ta_alert_level[side_index])
         {
            if (Ta_Is_Ttp_More_Critical(p_ta_core_output, p_ta_object, p_ta_cal, side_index)
                || Ta_Is_Ttc_More_Critical(p_ta_core_output, p_ta_object, side_index))
            {
               /* The object is more critical based on TTP, TTC and distance */
               Ta_Fill_Core_Output_With_Current_Obj(p_ta_core_output, p_ta_object, side_index);
            }
         }
      }
   }
}

void Ta_Set_Most_Critical_Side(Ta_Core_Output_T *p_ta_core_output)
{
   /* Asserts */
   assert(NULL != p_ta_core_output);

   if (p_ta_core_output->ta_alert_level[FBK_SIDE_LEFT] > p_ta_core_output->ta_alert_level[FBK_SIDE_RIGHT])
   {
      /* left side has higher alert level */
      p_ta_core_output->ta_most_critical_side = FBK_SIDE_LEFT;
   }
   else if (p_ta_core_output->ta_alert_level[FBK_SIDE_RIGHT] > p_ta_core_output->ta_alert_level[FBK_SIDE_LEFT])
   {
      /* right side has higher alert level */
      p_ta_core_output->ta_most_critical_side = FBK_SIDE_RIGHT;
   }
   else
   {
      /* left and right side have same alert level */
      if ((TA_ALERT_STATE_NONE != p_ta_core_output->ta_alert_level[FBK_SIDE_LEFT])
          && (TA_ALERT_STATE_NONE != p_ta_core_output->ta_alert_level[FBK_SIDE_RIGHT]))
      {
         /* Alert level is active -> check for lowest TTC side */
         if (p_ta_core_output->ta_ttc[FBK_SIDE_LEFT] < p_ta_core_output->ta_ttc[FBK_SIDE_RIGHT])
         {
            /* left side has lowest TTC */
            p_ta_core_output->ta_most_critical_side = FBK_SIDE_LEFT;
         }
         else if (p_ta_core_output->ta_ttc[FBK_SIDE_RIGHT] < p_ta_core_output->ta_ttc[FBK_SIDE_LEFT])
         {
            /* right side has lowest TTC */
            p_ta_core_output->ta_most_critical_side = FBK_SIDE_RIGHT;
         }
         else
         {
            /* Same TTC on both sides */
            if ((TA_INVALID_TTC > p_ta_core_output->ta_ttc[FBK_SIDE_LEFT])
                && (TA_INVALID_TTC > p_ta_core_output->ta_ttc[FBK_SIDE_RIGHT]))
            {
               /* TTCs are valid */
               if (p_ta_core_output->ta_distance[FBK_SIDE_LEFT] < p_ta_core_output->ta_distance[FBK_SIDE_RIGHT])
               {
                  /* left side object is closer */
                  p_ta_core_output->ta_most_critical_side = FBK_SIDE_LEFT;
               }
               else
               {
                  /* right side object is closer */
                  p_ta_core_output->ta_most_critical_side = FBK_SIDE_RIGHT;
               }
            }
            else
            {
               /* TTCs invalid -> check TTP */
               if (p_ta_core_output->ta_ttp[FBK_SIDE_LEFT] < p_ta_core_output->ta_ttp[FBK_SIDE_RIGHT])
               {
                  /* left side object has lower TTP */
                  p_ta_core_output->ta_most_critical_side = FBK_SIDE_LEFT;
               }
               else
               {
                  /* right side object has lower TTP */
                  p_ta_core_output->ta_most_critical_side = FBK_SIDE_RIGHT;
               }
            }
         }
      }
   }
}
