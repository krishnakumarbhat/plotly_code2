/**
 * @file lcda_process_cvw.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides warning in case of an object approaches on the adjacent lane
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_process_cvw.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_functions.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_common_functions.h"
#include "lcda_create_cvw_zone.h"
#include "lcda_debug_interface.h"
#include "lcda_types.h"
#include "ml_float_range_t.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>


/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Initializes cvw object data to its default.
 *
 * @return void
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6680}
 * @verification{Check that Cvw object data is correctly initialized.}
 */
static void Lcda_Init_Cvw_Object_Data(Cvw_Object_T *p_cvw_object /**< Cvw object */,
                                      const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Checks whether object is relevant for Cvw.
 *
 * @return True when object is a relevant candidate for Cvw
 *
 * @SRS{SF-1015,CSCSA-122140,CSCSA-164371,CSCSA-122139}
 * @SAE{SF-2779}
 * @SDD{SF-6682}
 * @verification{Create a test with a relevant Cvw object. Only then true is expected.}
 */
static boolean_T Lcda_Is_Object_Relevant_For_Cvw(const Lcda_Core_Input_T *p_lcda_core_input /**< Lcda core input */,
                                                 const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                 const Lcda_Cvw_Persistent_T *p_cvw_persistent /**< Cvw persistent data */);

/**
 * @brief Resets all object persistent cvw data.
 *
 * @return void
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6685}
 * @verification{Check whether object persistent cvw data is correctly reset to its default.}
 */
static void Lcda_Reset_Cvw_Persistent_Object_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent /**<Cvw persistent data*/,
                                                  const uint8_t obj_id /**< object identifier */);

/**
 * @brief Resets Cvw core output on given side.
 *
 * @return void
 *
 * @SRS{SF-992}
 * @SAE{SF-2779}
 * @SDD{SF-6675}
 * @verification{Check whether Cvw core output is reset correctly on given side.}
 */
static void Lcda_Clear_Cvw_Core_Output_On_Side(Lcda_Cvw_Core_Output_T *p_cvw_core_output /**< Cvw core output */,
                                               const uint8_t side /**< side index */);

/**
 * @brief takes the TTC threshold from warning settings and then
 *        adds a cvw ttc hysteresis if a Cvw warning was previously active.
 *        If a previous Bsw or Cvw warning was active, then it adds another gap_bridge to the ttc threshold.
 *
 * @return time to conflict of type float32_T
 *
 * @SRS{SF-1013,SF-1079}
 * @SAE{SF-2779}
 * @SDD{SF-6679}
 * @verification{Check based on whether cvw or bsw alert has been given, that the ttc is returned correctly.}
 */
static float32_T Lcda_Get_Time_To_Conflict_Threshold(
   const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/,
   const boolean_T f_prev_cvw_warning_active /**< flag indicating whether cvw warning was previously active */,
   const boolean_T f_prev_bsw_warning_active /**< flag indicating whether bsw warning was previously active */,
   const Lcda_Warn_Settings_T *p_warn_settings /**< Lcda warning settings */);

/**
 * @brief Based on objects attributes of the most critical cvw object are set.
 *
 * @return void
 *
 * @SRS{SF-1016,SF-1017,SF-1063}
 * @SAE{SF-2779}
 * @SDD{SF-6686}
 * @verification{Verify that attributes for the most critical object are correctly set.}
 */
static void Lcda_Set_Most_Critical_Cvw_Object(Lcda_Cvw_Core_Output_T *p_cvw_core_output /**< Cvw core output */,
                                              const Lcda_Core_Input_T *p_core_input, /**< Core input */
                                              const Cvw_Object_T *p_cvw_object /**< Cvw object */,
                                              const Lcda_Turn_Signal_T turn_signal_held /**< turn signal state */);

/**
 * @brief Processes the Cvw output with application of the Cvw warning holding logic.
 *
 * @return void
 *
 * @SRS{SF-1020,SF-1066}
 * @SAE{SF-2779}
 * @SDD{SF-6683}
 * @verification{Check that for an object which is currently creating a bsw alert no holding counter has increased.}
 */
static void Lcda_Process_Cvw_Output(Lcda_Cvw_Core_Output_T *p_cvw_core_output /**<Cvw core output*/,
                                    Lcda_Cvw_Persistent_T *p_cvw_persistent /**<Cvw persistent data*/,
                                    const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/,
                                    const Lcda_Turn_Signal_T turn_signal_held /**< turn signal state */);

/**
 * @brief Maps cvw core output data to the cvw persistent data.
 *
 * @return void
 *
 * @SRS{SF-1020}
 * @SAE{SF-2779}
 * @SDD{SF-6677}
 * @verification{Check that the mapping between cvw core output and cvw persistent data is correctly applied.}
 */
static void Lcda_Fill_Side_Persistent_Cvw_Data(
   Lcda_Cvw_Persistent_T *p_cvw_persistent /**<Cvw persistent data*/,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Lcda_Cvw_Core_Output_T *p_cvw_core_output /**<Cvw core output*/);

/**
 * @brief Check lane change flags in core input.
 *
 * @return true if lane change is detected and object is outside of ego lane.
 *
 * @SRS{SF-1064,CSCSA-122140,CSCSA-164371,CSCSA-122139}
 * @SAE{SF-2779}
 * @SDD{SF-6965}
 * @verification{Check that the lane change info is computed correctly.}
 */
static boolean_T Lcda_Is_Lane_Change_Detected(const Lcda_Core_Input_T *p_core_input,
                                              const Cvw_Object_T *p_cvw_object,
                                              const Lcda_Core_Calibration_T *p_cals);

/**
 * @brief Creates cvw object zone where also an hysteresis might be applied.
 *
 * @return void
 *
 * @SRS{SF-1064,CSCSA-122140,CSCSA-164371}
 * @SAE{SF-2779}
 * @SDD{SF-6676}
 * @verification{Check that for a qualified object the zone with hysteresis and in the other case the standard zone is used.}
 */
static void Lcda_Create_Cvw_Object_Zone(
   Cvw_Object_T *p_cvw_object /**< Cvw object */,
   Lcda_Cvw_Persistent_T *p_cvw_persistent /**< Cvw persistent data */,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
   const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/);

/**
 * @brief Cvw persistent data gets prepared for objects where a merge occured.
 *
 * @return void
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6687}
 * @verification{Check that persistent data of merged objects has been correctly updated.}
 */
static void Lcda_Update_Cvw_Data_For_Merged_Objects(Lcda_Cvw_Persistent_T *p_cvw_persistent /**<Cvw persistent data*/,
                                                    const Fbk_Output_T *p_fbk_output);

/**
 * @brief Checks whether the given object is behind a guardrail.
 *        Objects located on a guardrail are not considered to be behind the guardrail
 *
 * @return True when the object is located behind given guardrail sources
 *
 * @SRS{SF-1015,CSCSA-122140,CSCSA-164371,CSCSA-122139}
 * @SAE{SF-2779}
 * @SDD{SF-6681}
 * @verification{Verify that true is returned in the case that the cvw object is placed behind the guardrail.}
 */
static boolean_T
Lcda_Is_Cvw_Object_Behind_Guardrail(const Cvw_Object_T *p_cvw_object /**<Cvw object*/,
                                    const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES] /**< guardrail data */);

/**
 * @brief Returns min mature cycles which are dependent on the executive mode of feature function
 *
 * @return k_cvw_min_mature_cycles_lc_intention when lane change detection is detected. k_cvw_min_mature_cycles in all other modes.
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6809}
 * @verification{Verify that dependent on the input mode, different thresholds are returned.}
 */
static uint8_t Lcda_Return_Mode_Dep_Min_Mature_Cycles(const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                      const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);


/**
 * @brief Checks whether the object shall be examined for most critical object analysis.
 *
 * @return True when mode dependent conditions are fulfilled
 *
 * @SRS{SF-1015,CSCSA-157535}
 * @SAE{SF-2779}
 * @SDD{SF-6810}
 * @verification{Verify that object is only allowed to be examined as most critical one, when it is not in an environmental *
 * conflict and when either the ttc condition or the critical distance condition is fulfilled.}
 */
static boolean_T Lcda_Shall_Obj_Be_Considered_As_Most_Critical(const Cvw_Object_T *p_cvw_obj /**< Cvw object */,
                                                               const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                               const float32_T cvw_ttc_threshold);

/**
 * @brief Calculates the critical distance when a lane change intention is given.
 *
 * @return void
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6811}
 * @verification{Verify that the correct critical distance is returned.}
 */
static void Lcda_Calculate_Critical_Distance(Cvw_Object_T *p_cvw_obj /**< Cvw object */,
                                             const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                             const Lcda_Cvw_Persistent_T *p_cvw_persistent /**< persistent data of Cvw */);

/**
 * @brief Returns which one of the two lane change intention zones should be used.
 *
 * @return true if small zone should be used, false otherwise
 *
 * @SRS{SF-1001}
 * @SAE{SF-2779}
 * @SDD{SF-6822}
 * @verification{}
 */
static boolean_T
Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(Lcda_Cvw_Persistent_T *p_cvw_persistent /**< persistent data of Cvw */,
                                                     const uint8_t side /**< Side index */,
                                                     const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                     const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Calculate dynamic CVW TTC threshold.
 *
 * @return TTC threshold
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-196335}
 * @verification{}
 */
static float32_T Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(const float32_T obj_long_vel_rel /**< Object longitudinal relative velocity */,
                                                     const float32_T cvw_ttc_threshold /**< CVW ttc threshold */,
                                                     const float32_T cvw_ttc_speed_factor /**< CVW ttc speed factor */,
                                                     const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Check if the object is in ego lane including additional logic.
 *
 * @return true if object in ego lane and lateral relative velocity negative
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{CSCSA-109113}
 * @verification{}
 */
static boolean_T Lcda_Is_Cvw_Object_In_Ego_Lane(const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                const Cvw_Object_T *p_cvw_obj /**< Current cvw object */,
                                                const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system used */);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Lcda_Reset_Cvw_Core(Lcda_Cvw_Core_Output_T *p_cvw_core_output,
                         Lcda_Persistent_T *p_lcda_persistent,
                         Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_lcda_persistent);

   /* Reset CVW persistent data */
   Lcda_Reset_Cvw_Persistent_Data(p_cvw_persistent);

   /* Clear CVW core output */
   p_cvw_core_output->f_cvw_is_enabled = FBK_FALSE;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Lcda_Clear_Cvw_Core_Output_On_Side(p_cvw_core_output, side);
   }

   p_lcda_persistent->f_cvw_prev_reset = FBK_TRUE;
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][p_vehicle_data/p_cals used in debug build only] */
void Lcda_Preprocess_Cvw(Lcda_Cvw_Core_Output_T *p_cvw_core_output, float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES], boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES], const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals,
    const Fbk_Output_T *p_fbk_output, Lcda_Cvw_Persistent_T *p_cvw_persistent)
/* clang-format on */
{
   uint8_t i_side;
   boolean_T f_previous_alert_active;

   /* Asserts */
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_fbk_output);
   assert(NULL != p_cvw_persistent);

   /* Update the CVW persistence when tracker objects are merged */
   Lcda_Update_Cvw_Data_For_Merged_Objects(p_cvw_persistent, p_fbk_output);

   Binary_Lcda_Debug_Pass_Cvw_Default_Zone(p_core_input, &p_core_input->p_pa_data->vehicle_data, p_cals, p_cvw_persistent);

   /* Initialize data of the most critical object for this cycle */
   for (i_side = FBK_ZERO_UINT; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      Lcda_Clear_Cvw_Core_Output_On_Side(p_cvw_core_output, i_side);
   }

   /* Define the TTC threshold for each side */
   for (i_side = FBK_ZERO_UINT; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      f_previous_alert_active =
         (boolean_T) (PA_INVALID_OBJ_ID != Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(i_side, p_cvw_persistent));
      cvw_ttc_threshold[i_side] = Lcda_Get_Time_To_Conflict_Threshold(p_cals, p_cvw_persistent->f_prev_cvw_active[i_side],
                                                                      f_previous_alert_active, &p_core_input->warn_settings);
      f_use_small_lc_intention_zone[i_side] =
         Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(p_cvw_persistent, i_side, p_core_input, p_cals);
   }
}

void Lcda_Process_Cvw_Object(Lcda_Cvw_Core_Output_T *p_cvw_core_output,
                             const float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES],
                             const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                             const Fbk_Object_Data_T *p_tracker_object,
                             const Lcda_Core_Input_T *p_core_input,
                             const Lcda_Core_Calibration_T *p_cals,
                             const Lcda_Persistent_T *p_lcda_persistent,
                             Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   /* Asserts */
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_tracker_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_lcda_persistent);

   p_vehicle_data = &p_core_input->p_pa_data->vehicle_data;

   if (Lcda_Is_Object_Relevant_For_Cvw(p_core_input, p_tracker_object, p_cals, p_cvw_persistent))
   {
      Cvw_Object_T curr_obj;
      Lcda_Coordinate_System_T coordinate_system;
      Lcda_Object_Location_Data_T obj_loc_data;
      float32_T ttc_threshold_with_speed;

      coordinate_system = LCDA_USE_CURVI;

      /* Set common shared parameters as CVW */
      Binary_Lcda_Debug_Pass_Processed_Submodule(LCDA_CVW);

      /* Initialize CVW object information */
      Lcda_Init_Cvw_Object_Data(&curr_obj, p_tracker_object);

      /* Determine ego side of current object */
      curr_obj.ego_side = Fbk_Get_Obj_Side(curr_obj.p_tracker_data->curvi_pos.y);

      /* Create the CVW Zone for the object */
      Lcda_Create_Cvw_Object_Zone(&curr_obj, p_cvw_persistent, f_use_small_lc_intention_zone, p_core_input, p_vehicle_data, p_cals);

      /* Calculate the time to collision for the object */
      curr_obj.ttc = Lcda_Get_Longitudinal_Ttc(curr_obj.p_tracker_data, p_vehicle_data->host_length);

      /* Set Object Location Data*/
      Lcda_Get_Object_Location_Data(&obj_loc_data, curr_obj.p_tracker_data, &curr_obj.zone, p_cals, coordinate_system);

      /* Check if target object is in alert zone */
      curr_obj.f_obj_in_zone = obj_loc_data.obj_in_zone;

      /* If the ttc is lower than the threshold and the object is in the zone it is a candidate for a CVW alert */
      /* Ttc threshold shall be not be checked in case that lane change intention is given*/
      if ((curr_obj.ttc >= FBK_ZERO_F)
          && ((curr_obj.ttc <= p_cals->k_cvw_candidate_ttc)
              || Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
          && (Fbk_Is_True(curr_obj.f_obj_in_zone)))
      {
         /* Get minimum required mature cycles */
         uint8_t min_mature_cycles_thres = Lcda_Return_Mode_Dep_Min_Mature_Cycles(p_core_input, p_cals);

         /* Increment the counter for object in the CVW zone */
         Lcda_Increment_Mature_Count_In_Zone(&(p_cvw_persistent->mature_count_in_cvw_zone[curr_obj.p_tracker_data->id]),
                                             curr_obj.p_tracker_data->status);

         if ((p_cvw_persistent->mature_count_in_cvw_zone[curr_obj.p_tracker_data->id] >= min_mature_cycles_thres)
             || Fbk_Is_True(p_cvw_persistent->f_prev_cvw_active[curr_obj.ego_side]))
         {
            /* Object is mature in zone */
            curr_obj.f_obj_mature_in_zone = FBK_TRUE;

            /* Check the approximate front bumper position of the object */
            curr_obj.obj_front_position     = Lcda_Get_Obj_Front_Position(curr_obj.p_tracker_data);
            curr_obj.obj_front_position_lat = Lcda_Get_Obj_Side_Distance_Lateral(curr_obj.p_tracker_data);

            /* Check the object position against known guardrails */
            curr_obj.f_obj_behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&curr_obj, p_core_input->guardrail_data);

            /* Check the object position against the ego lane */
            if (Fbk_Is_True(p_cals->k_lcda_f_enable_obj_in_ego_lane_check))
            {

               curr_obj.f_obj_in_ego_lane = Lcda_Is_Cvw_Object_In_Ego_Lane(p_core_input, p_cals, &curr_obj, coordinate_system);
            }

            if (Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
            {
               Lcda_Calculate_Critical_Distance(&curr_obj, p_vehicle_data, p_cals, p_cvw_persistent);
            }

            /* Calculate ttc threshold including speed factor*/
            ttc_threshold_with_speed =
               Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(curr_obj.p_tracker_data->curvi_vel_rel.x, cvw_ttc_threshold[curr_obj.ego_side],
                                                   p_core_input->warn_settings.cvw_ttc_speed_factor, p_cals);

            if (Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&curr_obj, p_core_input, p_vehicle_data, ttc_threshold_with_speed))
            {
               /* Check if current object is the most critical object for its side */
               Lcda_Set_Most_Critical_Cvw_Object(p_cvw_core_output, p_core_input, &curr_obj, p_lcda_persistent->turn_signal_held);
            }
            else
            {
               if (Fbk_Is_True(p_cals->k_cvw_f_most_crit_obj_must_be_closest_relevant_obj)
                   && (curr_obj.obj_front_position > p_cvw_core_output->cvw_distance[curr_obj.ego_side]))
               {
                  /* Reset CVW core output for an uncritical CVW object that is closer than the most critical object so far */
                  Lcda_Clear_Cvw_Core_Output_On_Side(p_cvw_core_output, curr_obj.ego_side);

                  /* Set current object as closest object so far */
                  p_cvw_core_output->cvw_distance[curr_obj.ego_side] = curr_obj.obj_front_position;
               }
            }
         }
      }
      else
      {
         /* TTC is too large or object is not in zone so clear persistent object data */
         Lcda_Reset_Cvw_Persistent_Object_Data(p_cvw_persistent, curr_obj.p_tracker_data->id);
      }

      /* Pass object attributes to debug structure */
      Binary_Lcda_Debug_Pass_Cvw_Object_Attributes(&curr_obj);
   }
   else
   {
      /* This is not a valid CVW candidate so clear persistent object data */
      Lcda_Reset_Cvw_Persistent_Object_Data(p_cvw_persistent, p_tracker_object->id);
   }
}

void Lcda_Postprocess_Cvw(Lcda_Cvw_Core_Output_T *p_cvw_core_output,
                          const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                          const Lcda_Core_Calibration_T *p_cals,
                          const Lcda_Persistent_T *p_lcda_persistent,
                          Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   /* Asserts */
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_cals);
   assert(NULL != p_lcda_persistent);

   Lcda_Process_Cvw_Output(p_cvw_core_output, p_cvw_persistent, p_cals, p_lcda_persistent->turn_signal_held);

   /* Fill side persistent data */
   Lcda_Fill_Side_Persistent_Cvw_Data(p_cvw_persistent, f_use_small_lc_intention_zone, p_cvw_core_output);

   Binary_Lcda_Debug_Pass_Cvw_Persistent_Data(p_cvw_persistent);
}

uint8_t Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(const uint8_t side, const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   /* Assert */
   assert(side < FBK_NUMBER_OF_SIDES);

   return p_cvw_persistent->prev_cvw_alert_obj_id[side];
}

boolean_T Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(const Lcda_Cvw_Persistent_T *p_cvw_persistent, const uint8_t obj_id, const uint8_t side)
{
   boolean_T f_alert_active = FBK_FALSE;

   /* Assert */
   assert((obj_id <= PA_OBJ_NUMBER_OF_OBJECTS) && (side < FBK_NUMBER_OF_SIDES));

   if ((p_cvw_persistent->f_prev_cvw_active[side]) && (obj_id == p_cvw_persistent->prev_cvw_alert_obj_id[side]))
   {
      f_alert_active = FBK_TRUE;
   }

   return f_alert_active;
}

void Lcda_Store_Pers_Cvw_Curve_Zone_Factor(Lcda_Cvw_Persistent_T *p_cvw_persistent, const float32_T factor, const uint8_t side)
{
   /* Assert */
   assert(side < FBK_NUMBER_OF_SIDES);

   p_cvw_persistent->prev_curve_zone_factor[side] = factor;
}


float32_T Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(const uint8_t side, const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   /* Assert */
   assert(side < FBK_NUMBER_OF_SIDES);

   return p_cvw_persistent->prev_curve_zone_factor[side];
}

/* coverity[misra_c_2012_rule_8_7_violation][This function is needed by the LCDA debug writer] */
void Lcda_Reset_Cvw_Persistent_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   uint8_t side;
   uint8_t obj_id;

   /* Assert */
   assert(NULL != p_cvw_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_cvw_persistent->cvw_hold_counter[side]                    = FBK_ZERO_UINT;
      p_cvw_persistent->f_prev_cvw_active[side]                   = FBK_FALSE;
      p_cvw_persistent->prev_cvw_alert_obj_index[side]            = PA_INVALID_OBJ_INDEX;
      p_cvw_persistent->prev_cvw_alert_obj_id[side]               = FBK_ZERO_UINT;
      p_cvw_persistent->prev_cvw_alert_unique_obj_id[side]        = FBK_ZERO_UINT;
      p_cvw_persistent->prev_curve_zone_factor[side]              = 1.0f;
      p_cvw_persistent->f_prev_used_small_lc_intention_zone[side] = FBK_FALSE;
      p_cvw_persistent->lc_intention_zone_change_counter[side]    = FBK_ZERO_UINT;
   }

   for (obj_id = FBK_ZERO_UINT; obj_id <= PA_OBJ_NUMBER_OF_OBJECTS; obj_id++)
   {
      Lcda_Reset_Cvw_Persistent_Object_Data(p_cvw_persistent, obj_id);
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static boolean_T Lcda_Is_Object_Relevant_For_Cvw(const Lcda_Core_Input_T *p_lcda_core_input,
                                                 const Fbk_Object_Data_T *p_tracker_object,
                                                 const Lcda_Core_Calibration_T *p_cals,
                                                 const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   boolean_T f_is_relevant_obj = FBK_FALSE;
   boolean_T f_previous_alert_active;
   boolean_T f_curvi_vel_rel_x_in_range;
   float32_T existence_prob_threshold;
   uint8_t side;

   /* Asserts */
   assert(NULL != p_tracker_object);
   assert(NULL != p_cals);

   side = Fbk_Get_Obj_Side(p_tracker_object->curvi_pos.y);

   f_previous_alert_active = (boolean_T) (PA_INVALID_OBJ_ID != Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(side, p_cvw_persistent));

   /* Get existence probability threshold*/
   existence_prob_threshold = Lcda_Get_Existence_Probability_Threshold(p_lcda_core_input, p_cvw_persistent->prev_cvw_alert_obj_id,
                                                                       p_tracker_object->id, p_cals);
   f_curvi_vel_rel_x_in_range =
      Fbk_Is_Float_In_Given_Range(p_tracker_object->curvi_vel_rel.x, p_lcda_core_input->warn_settings.cvw_rel_vel_range.min,
                                  p_lcda_core_input->warn_settings.cvw_rel_vel_range.max, f_previous_alert_active,
                                  -p_cals->k_cvw_object_curvi_relative_speed_hys, p_cals->k_cvw_object_curvi_relative_speed_hys);


   if (((PA_OBJ_STATUS_MATURE == p_tracker_object->status) || (PA_OBJ_STATUS_COASTED == p_tracker_object->status)
        || (Fbk_Is_True(p_cals->k_lcda_allow_track_status_new) && (PA_OBJ_STATUS_NEW == p_tracker_object->status)
            && (Fbk_Is_True(f_previous_alert_active))))
       && (Fbk_Abs_F(p_tracker_object->curvi_heading) <= p_cals->k_cvw_max_curvi_heading_abs)
       && (p_tracker_object->curvi_vel.x >= p_cals->k_cvw_min_obj_curvi_long_vel)
       && (p_tracker_object->existence_probability >= existence_prob_threshold) && (Fbk_Is_True(f_curvi_vel_rel_x_in_range)))
   {
      f_is_relevant_obj = FBK_TRUE;
   }

   return f_is_relevant_obj;
}

static void Lcda_Reset_Cvw_Persistent_Object_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent, const uint8_t obj_id)
{
   /* Assert */
   assert(NULL != p_cvw_persistent);

   p_cvw_persistent->mature_count_in_cvw_zone[obj_id] = FBK_ZERO_UINT;
}

static void Lcda_Clear_Cvw_Core_Output_On_Side(Lcda_Cvw_Core_Output_T *p_cvw_core_output, const uint8_t side)
{
   /* Assert */
   assert(NULL != p_cvw_core_output);

   p_cvw_core_output->cvw_alert[side]        = LCDA_ALERT_STATE_NONE;
   p_cvw_core_output->cvw_index[side]        = PA_INVALID_OBJ_INDEX;
   p_cvw_core_output->cvw_id[side]           = PA_INVALID_OBJ_ID;
   p_cvw_core_output->cvw_unique_id[side]    = PA_INVALID_OBJ_ID;
   p_cvw_core_output->cvw_ttc[side]          = LCDA_CVW_DEFAULT_NO_ALERT_TTC;
   p_cvw_core_output->cvw_ttp[side]          = LCDA_DEFAULT_LARGE_TTP;
   p_cvw_core_output->cvw_ttle[side]         = LCDA_DEFAULT_LARGE_TTLE;
   p_cvw_core_output->cvw_distance[side]     = LCDA_DEFAULT_OBJ_DIST;
   p_cvw_core_output->cvw_distance_lat[side] = -LCDA_DEFAULT_OBJ_DIST;
}

static float32_T Lcda_Get_Time_To_Conflict_Threshold(const Lcda_Core_Calibration_T *p_cals,
                                                     const boolean_T f_prev_cvw_warning_active,
                                                     const boolean_T f_prev_bsw_warning_active,
                                                     const Lcda_Warn_Settings_T *p_warn_settings)
{
   float32_T ttc_threshold = p_warn_settings->cvw_ttc_threshold;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_warn_settings);

   if (f_prev_bsw_warning_active)
   {
      ttc_threshold = p_warn_settings->cvw_ttc_threshold + p_cals->k_cvw_gap_bridge;
   }

   if (f_prev_cvw_warning_active)
   {
      ttc_threshold = p_warn_settings->cvw_ttc_threshold + p_cals->k_cvw_ttc_hys;
   }

   return ttc_threshold;
}

static void Lcda_Process_Cvw_Output(Lcda_Cvw_Core_Output_T *p_cvw_core_output,
                                    Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                    const Lcda_Core_Calibration_T *p_cals,
                                    const Lcda_Turn_Signal_T turn_signal_held)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_cvw_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      if (PA_INVALID_OBJ_INDEX != p_cvw_core_output->cvw_index[side])
      {
         /* Resetting CVW Alert hold counter */
         p_cvw_persistent->cvw_hold_counter[side] = FBK_ZERO_UINT;
      }
      /* If there is no alert for this cycle, then check if the alert from the previous cycle needs to be held */
      else if (p_cvw_persistent->f_prev_cvw_active[side]
               && (p_cvw_persistent->cvw_hold_counter[side] < p_cals->k_cvw_alert_holding_cycles))
      {
         Sat_Inc_Uint8(&(p_cvw_persistent->cvw_hold_counter[side]));

         Lcda_Clear_Cvw_Core_Output_On_Side(p_cvw_core_output, side);
         p_cvw_core_output->cvw_alert[side] = Lcda_Get_Alert_State(side, p_cvw_persistent->f_prev_cvw_active[side], turn_signal_held);
         p_cvw_core_output->cvw_index[side]     = p_cvw_persistent->prev_cvw_alert_obj_index[side];
         p_cvw_core_output->cvw_id[side]        = p_cvw_persistent->prev_cvw_alert_obj_id[side];
         p_cvw_core_output->cvw_unique_id[side] = p_cvw_persistent->prev_cvw_alert_unique_obj_id[side];
      }
      else
      {
         /* Clear the alerts and hold counter */
         p_cvw_persistent->cvw_hold_counter[side] = FBK_ZERO_UINT;
      }
   }
}

static void Lcda_Fill_Side_Persistent_Cvw_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                               const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                                               const Lcda_Cvw_Core_Output_T *p_cvw_core_output)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_cvw_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Save the CVW outputs for the next run */
      p_cvw_persistent->prev_cvw_alert_obj_index[side]            = p_cvw_core_output->cvw_index[side];
      p_cvw_persistent->prev_cvw_alert_obj_id[side]               = p_cvw_core_output->cvw_id[side];
      p_cvw_persistent->prev_cvw_alert_unique_obj_id[side]        = p_cvw_core_output->cvw_unique_id[side];
      p_cvw_persistent->f_prev_cvw_active[side]                   = Lcda_Is_Alert_On(p_cvw_core_output->cvw_alert[side]);
      p_cvw_persistent->f_prev_used_small_lc_intention_zone[side] = f_use_small_lc_intention_zone[side];
   }
}

static void Lcda_Init_Cvw_Object_Data(Cvw_Object_T *p_cvw_object, const Fbk_Object_Data_T *p_tracker_object)
{
   /* Asserts */
   assert(NULL != p_cvw_object);
   assert(NULL != p_tracker_object);

   p_cvw_object->ttc                    = LCDA_CVW_DEFAULT_NO_ALERT_TTC;
   p_cvw_object->critical_distance      = LCDA_DEFAULT_OBJ_DIST;
   p_cvw_object->obj_front_position     = LCDA_DEFAULT_OBJ_DIST;
   p_cvw_object->obj_front_position_lat = LCDA_DEFAULT_OBJ_DIST;
   p_cvw_object->ego_side               = FBK_SIDE_UNDEFINED;
   p_cvw_object->f_obj_behind_guardrail = FBK_FALSE;
   p_cvw_object->f_obj_in_ego_lane      = FBK_FALSE;
   p_cvw_object->f_obj_in_zone          = FBK_FALSE;
   p_cvw_object->f_obj_mature_in_zone   = FBK_FALSE;
   p_cvw_object->p_tracker_data         = p_tracker_object;

   Fbk_Reset_Field_Of_Interest(&p_cvw_object->zone);
}

static void Lcda_Set_Most_Critical_Cvw_Object(Lcda_Cvw_Core_Output_T *p_cvw_core_output,
                                              const Lcda_Core_Input_T *p_core_input,
                                              const Cvw_Object_T *p_cvw_object,
                                              const Lcda_Turn_Signal_T turn_signal_held)
{
   uint8_t side = p_cvw_object->ego_side;

   /* Assert */
   assert(NULL != p_cvw_core_output);
   assert(NULL != p_core_input);
   assert(NULL != p_cvw_object);

   /* If proper flag is set, object lateral position is checked and closest object is taken. Positions are positive in both
    * directions*/
   /* By default, the object position is checked to determine the criticality compared to other CVW alert candidates.
    * Positions are negative as relevant objects are behind the host vehicle. */
   if (((CVW_CRIT_LAT_DIST == p_core_input->cvw_crit_mode[side])
        && (p_cvw_object->obj_front_position_lat < p_cvw_core_output->cvw_distance_lat[side]))
       || ((CVW_CRIT_LAT_DIST != p_core_input->cvw_crit_mode[side])
           && (p_cvw_object->obj_front_position > p_cvw_core_output->cvw_distance[side])))
   {
      p_cvw_core_output->cvw_alert[side]     = Lcda_Get_Alert_State(side, FBK_TRUE, turn_signal_held);
      p_cvw_core_output->cvw_index[side]     = p_cvw_object->p_tracker_data->index;
      p_cvw_core_output->cvw_id[side]        = p_cvw_object->p_tracker_data->id;
      p_cvw_core_output->cvw_unique_id[side] = p_cvw_object->p_tracker_data->unique_id;
      p_cvw_core_output->cvw_ttc[side]       = p_cvw_object->ttc;
      p_cvw_core_output->cvw_ttp[side]       = Lcda_Get_Longitudinal_Ttp(p_cvw_object->p_tracker_data, LCDA_USE_CURVI);
      p_cvw_core_output->cvw_ttle[side]      = Lcda_Get_Ttle(p_cvw_object->p_tracker_data, &(p_cvw_object->zone), LCDA_USE_CURVI);
      p_cvw_core_output->cvw_distance[side]  = p_cvw_object->obj_front_position;
      p_cvw_core_output->cvw_distance_lat[side] = p_cvw_object->obj_front_position_lat;
   }
}

static boolean_T Lcda_Is_Lane_Change_Detected(const Lcda_Core_Input_T *p_core_input,
                                              const Cvw_Object_T *p_cvw_object,
                                              const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_lane_change_detected = FBK_FALSE;
   boolean_T f_oppisite_lane_change;

   /* Check opposite object side for lane change flag
    * and that the object position is outside of the host lane. */
   f_oppisite_lane_change = (boolean_T) Fbk_Is_True(p_core_input->f_lane_change[Lcda_Get_Opposite_Side(p_cvw_object->ego_side)]);
   if (f_oppisite_lane_change
       && (Fbk_Abs_F(p_cvw_object->p_tracker_data->curvi_pos.y)
           >= Fbk_Half(Fbk_Max(p_cals->k_lcda_min_lane_width, p_core_input->lane_width))))
   {
      f_lane_change_detected = FBK_TRUE;
   }
   return f_lane_change_detected;
}

static void Lcda_Create_Cvw_Object_Zone(Cvw_Object_T *p_cvw_object,
                                        Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                        const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                                        const Lcda_Core_Input_T *p_core_input,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                                        const Lcda_Core_Calibration_T *p_cals)
{
   Fbk_Field_Of_Interest_T cvw_zone;
   Fbk_Field_Of_Interest_T cvw_zone_hys;
   uint8_t min_mature_cycles_thres;
   boolean_T f_mature_count_more_than_thres;

   /* Asserts */
   assert(NULL != p_cvw_object);
   assert(NULL != p_cvw_persistent);
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   min_mature_cycles_thres = Lcda_Return_Mode_Dep_Min_Mature_Cycles(p_core_input, p_cals);

   /* Create the zone for the object. Because the CVW zone hys is adjusted based on the object width, we need to create the zone
    * for each object */
   Lcda_Create_Cvw_Zone(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, p_cvw_object, p_core_input, p_vehicle_data,
                        p_cals, p_cvw_persistent);

   /* Check if current object is mature and within the zone for more than minimum needed mature cycles */
   f_mature_count_more_than_thres =
      (boolean_T) (p_cvw_persistent->mature_count_in_cvw_zone[p_cvw_object->p_tracker_data->id] >= min_mature_cycles_thres);
   if (f_mature_count_more_than_thres && Fbk_Is_False(Lcda_Is_Lane_Change_Detected(p_core_input, p_cvw_object, p_cals)))
   {
      p_cvw_object->zone = cvw_zone_hys;
   }
   else
   {
      p_cvw_object->zone = cvw_zone;
   }
}


static void Lcda_Update_Cvw_Data_For_Merged_Objects(Lcda_Cvw_Persistent_T *p_cvw_persistent, const Fbk_Output_T *p_fbk_output)
{
   uint8_t id;
   uint8_t obj_idx;
   uint8_t merged_obj_id;

   /* Assert */
   assert(NULL != p_cvw_persistent);
   assert(NULL != p_fbk_output);

   /* If tracker objects were merged in this cycle, then carry over the corresponding maturity counters */
   for (id = FBK_ZERO_UINT; id < PA_OBJ_NUMBER_OF_OBJECTS; id++)
   {
      const Fbk_Object_Data_T *p_object_data;
      obj_idx = Fbk_Get_Object_Index_From_Id(p_fbk_output->p_index_id_lookup_table, id);
      if ((obj_idx != PA_INVALID_OBJ_INDEX) && (obj_idx < PA_OBJ_NUMBER_OF_OBJECTS))
      {
         p_object_data = &p_fbk_output->p_pa_data->object_data[obj_idx];
         if (p_object_data->f_merge_occured)
         {
            merged_obj_id = p_object_data->id_merged_obj;

            if ((PA_INVALID_OBJ_ID != merged_obj_id)
                && (p_cvw_persistent->mature_count_in_cvw_zone[merged_obj_id] > p_cvw_persistent->mature_count_in_cvw_zone[id]))
            {
               p_cvw_persistent->mature_count_in_cvw_zone[id] = p_cvw_persistent->mature_count_in_cvw_zone[merged_obj_id];
            }
         }
      }
   }
}

static boolean_T Lcda_Is_Cvw_Object_Behind_Guardrail(const Cvw_Object_T *p_cvw_object,
                                                     const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES])
{
   boolean_T f_behind_guardrail = FBK_FALSE;
   float32_T lateral_guardrail_position;
   float32_T obj_lateral_inner_edge;
   float32_T side_sign;

   /* Asserts */
   assert(NULL != p_cvw_object);
   assert(NULL != guardrail_data);

   side_sign = Fbk_Convert_Obj_Side_To_Sign(p_cvw_object->ego_side);

   /* get lateral guardrail position, if available */
   lateral_guardrail_position = Lcda_Get_Lateral_Distance_Guardrail(p_cvw_object->ego_side, guardrail_data);

   /* calculate lateral position of inner object edge neglecting the object heading */
   obj_lateral_inner_edge = p_cvw_object->p_tracker_data->curvi_pos.y - (side_sign * Fbk_Half(p_cvw_object->p_tracker_data->width));

   /* object is behind guardrail, if inner object edge is behind the guardrail */
   if ((side_sign * obj_lateral_inner_edge) > (side_sign * lateral_guardrail_position))
   {
      f_behind_guardrail = FBK_TRUE;
   }

   return f_behind_guardrail;
}

static uint8_t Lcda_Return_Mode_Dep_Min_Mature_Cycles(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t min_mature_cycles;

   if (Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
   {
      min_mature_cycles = p_cals->k_cvw_min_mature_cycles_lc_intention;
   }
   else
   {
      min_mature_cycles = p_cals->k_cvw_min_mature_cycles;
   }

   return min_mature_cycles;
}

static boolean_T Lcda_Shall_Obj_Be_Considered_As_Most_Critical(const Cvw_Object_T *p_cvw_obj,
                                                               const Lcda_Core_Input_T *p_core_input,
                                                               const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                               const float32_T cvw_ttc_threshold)
{
   float32_T dist_to_host;
   boolean_T f_environment_fulfilled;
   boolean_T f_ttc_condition_fulfilled;
   boolean_T f_critical_dist_fulfilled;

   assert(NULL != p_cvw_obj);
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);

   f_environment_fulfilled =
      (boolean_T) (Fbk_Is_False(p_cvw_obj->f_obj_in_ego_lane) && Fbk_Is_False(p_cvw_obj->f_obj_behind_guardrail));
   f_ttc_condition_fulfilled = (boolean_T) (Fbk_Is_False(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone)
                                            && (p_cvw_obj->ttc <= cvw_ttc_threshold));

   dist_to_host =
      -(p_cvw_obj->p_tracker_data->curvi_pos.x + (Fbk_Half(p_cvw_obj->p_tracker_data->length)) + p_vehicle_data->host_length);

   f_critical_dist_fulfilled = (boolean_T) (Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone)
                                            && (dist_to_host < p_cvw_obj->critical_distance)
                                            && (p_cvw_obj->p_tracker_data->curvi_vel_rel.x > FBK_ZERO_F));

   return (boolean_T) (f_environment_fulfilled && (f_ttc_condition_fulfilled || f_critical_dist_fulfilled));
}


static void Lcda_Calculate_Critical_Distance(Cvw_Object_T *p_cvw_obj,
                                             const Fbk_Vehicle_Data_T *p_vehicle_data,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   assert(NULL != p_cvw_obj);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);
   assert(NULL != p_cvw_persistent);

   if (PA_INVALID_OBJ_ID == Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(p_cvw_obj->ego_side, p_cvw_persistent))
   {
      float32_T crit_distance;

      /* Calculate critical distance and apply hysteresis in case that cvw alert was active last cycle*/
      crit_distance = (p_cvw_obj->p_tracker_data->curvi_vel_rel.x * p_cals->k_cvw_time_obj_start_decel_after_lane_change)
                      + ((0.5f * p_cvw_obj->p_tracker_data->curvi_vel_rel.x * p_cvw_obj->p_tracker_data->curvi_vel_rel.x)
                         / p_cals->k_cvw_critical_obj_decel_after_lane_change)
                      + (p_vehicle_data->host_speed * p_cals->k_cvw_time_diff_after_obj_decel);

      /* Apply hysteresis */
      if (Fbk_Is_True(p_cvw_persistent->f_prev_cvw_active[p_cvw_obj->ego_side]))
      {
         crit_distance = (crit_distance * p_cals->k_cvw_crit_dist_hys_factor) + p_cals->k_cvw_crit_dist_additive_hys;
      }

      p_cvw_obj->critical_distance = crit_distance;
   }
}

static boolean_T Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                                                      const uint8_t side,
                                                                      const Lcda_Core_Input_T *p_core_input,
                                                                      const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_use_small_zone       = FBK_FALSE;
   boolean_T f_prev_used_small_zone = p_cvw_persistent->f_prev_used_small_lc_intention_zone[side];

   /* Determine zone to use from guardrail data */
   if (((LCDA_GUARDRAIL_INVALID == p_core_input->guardrail_data[side].radar.status)
        && Fbk_Is_True(p_cals->k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present))
       || ((LCDA_GUARDRAIL_VALID == p_core_input->guardrail_data[side].radar.status)
           && (Fbk_Abs_F(p_core_input->guardrail_data[side].radar.lateral_position)
               >= p_cals->k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone)))
   {
      f_use_small_zone = FBK_TRUE;
   }

   /* If zone has changed compared to previous cycle see if counter for change is above threshold */
   if (f_use_small_zone != f_prev_used_small_zone)
   {
      /* Increase counter*/
      Sat_Inc_Uint8(&(p_cvw_persistent->lc_intention_zone_change_counter[side]));

      if (p_cvw_persistent->lc_intention_zone_change_counter[side] < p_cals->k_lcda_lc_intention_cycles_for_zone_change_threshold)
      {
         /* If counter is below threshold use same zone as in last cycle */
         f_use_small_zone = f_prev_used_small_zone;
      }
      else
      {
         /* Reset counter */
         p_cvw_persistent->lc_intention_zone_change_counter[side] = FBK_ZERO_UINT;
      }
   }
   else
   {
      /* Reset counter */
      p_cvw_persistent->lc_intention_zone_change_counter[side] = FBK_ZERO_UINT;
   }

   return f_use_small_zone;
}

static boolean_T Lcda_Is_Cvw_Object_In_Ego_Lane(const Lcda_Core_Input_T *p_core_input,
                                                const Lcda_Core_Calibration_T *p_cals,
                                                const Cvw_Object_T *p_cvw_obj,
                                                const Lcda_Coordinate_System_T coordinate_system)
{
   float32_T rel_vel_side;
   boolean_T f_obj_in_ego_lane;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_cvw_obj);

   f_obj_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(p_core_input->lane_width, p_cvw_obj->p_tracker_data, p_cals, coordinate_system);

   rel_vel_side = Fbk_Convert_Obj_Side_To_Sign(p_cvw_obj->ego_side) * p_cvw_obj->p_tracker_data->curvi_vel_rel.y;

   /* If enabled alert will be suppresed when object moves towards ego center and rel_vel_above threshold, when target moves
    * towards CVW zone alert will be triggered immediately after crossing zone - ego lane check won't affect that */
   if (Fbk_Is_True(p_cals->k_lcda_f_enable_rel_vel_logic_in_ego_lane_check)
       && (rel_vel_side > p_cals->k_lcda_lane_change_intention_vel_lat_thresh))
   {
      f_obj_in_ego_lane = FBK_FALSE;
   }

   return f_obj_in_ego_lane;
}

static float32_T Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(const float32_T obj_long_vel_rel,
                                                     const float32_T cvw_ttc_threshold,
                                                     const float32_T cvw_ttc_speed_factor,
                                                     const Lcda_Core_Calibration_T *p_cals)
{
   float32_T ttc_threshold_with_speed;
   float32_T ttc_thresh_compens;

   ttc_thresh_compens = (obj_long_vel_rel > p_cals->k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh)
                           ? p_cals->k_lcda_dyn_cvw_ttc_compens_time[FBK_ONE_UINT]
                           : p_cals->k_lcda_dyn_cvw_ttc_compens_time[FBK_ZERO_UINT];

   if (Fbk_Is_True(p_cals->k_lcda_f_enable_dyn_cvw_ttc_threshold))
   {
      ttc_threshold_with_speed = (obj_long_vel_rel * cvw_ttc_speed_factor) + cvw_ttc_threshold
                                 + (p_cals->k_lcda_dyn_cvw_ttc_speed_parameter / obj_long_vel_rel) + ttc_thresh_compens;
   }
   else
   {
      ttc_threshold_with_speed = cvw_ttc_threshold + ttc_thresh_compens;
   }
   return ttc_threshold_with_speed;
}
