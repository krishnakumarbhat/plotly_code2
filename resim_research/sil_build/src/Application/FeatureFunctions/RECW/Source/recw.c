/**
 * @file recw.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is main RECW algorithm source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "fbk_vehicle_data_t.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_car_wash_detection.h"
#include "recw_core_calibration_t.h"
#include "recw_crash_prob_steer_brake.h"
#include "recw_debug_interface.h"
#include "recw_object_validator.h"
#include "recw_output_debouncer.h"
#include "recw_persistent_t.h"
#include "recw_types.h"
#include <assert.h>

/*===========================================================================*\
 * Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Main function of RECW core algorithm
 *
 * @return void
 *
 * @SRS{SF-1697,SF-1659}
 * @SAE{SF-2959}
 * @SDD{SF-7840}
 * @verification{}
 */
static void Recw_Algorithm(Recw_Core_Output_T *p_core_output /**< RECW core output */,
                           Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                           const Recw_Core_Input_T *p_recw_core_input /**< RECW core input */,
                           const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */,
                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                           const Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS] /**< FBK guard rail data */);

/**
 * @brief Checks whether the host speed is in the allowed range for RECW.
 *
 * @return True when host speed is in allowed range.
 *
 * @SRS{SF-1662}
 * @SAE{SF-2959}
 * @SDD{SF-7837}
 * @verification{}
 */
static boolean_T Recw_Is_Host_Speed_In_Allowed_Range(const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */,
                                                     const Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                                     const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Checks whether an object shall be used in further RECW analysis
 *
 * @return True when object is mature or coasted and not a reflection
 *
 * @SRS{SF-1693,SF-1694}
 * @SAE{SF-2959}
 * @SDD{SF-7838}
 * @verification{}
 */
static boolean_T Recw_Is_Object_State_Fulfilled(const Fbk_Object_Data_T *p_object_data /**< FBK Object data */);

/**
 * @brief Calculates Ttc for the current object either with a model without attention to the
 * objects acceleration in case it is less than a given threshold or with consideration of the acceleration
 * when it is exceeding the threshold
 *
 * @return void
 *
 * @SRS{SF-1706}
 * @SAE{SF-2959}
 * @SDD{SF-7833}
 * @verification{}
 */
static void Recw_Calculate_Ttc(Recw_Object_T *p_recw_object /**< RECW object */,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                               const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */);

/**
 * @brief Calculates Ttc thresholds for the current object
 *
 * @return void
 *
 * @SRS{SF-1708}
 * @SAE{SF-2959}
 * @SDD{SF-7834}
 * @verification{}
 */
static void Recw_Calculate_Ttc_Thresholds(Recw_Object_T *p_recw_object /**< RECW object */,
                                          const Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                          const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */);

/**
 * @brief Calculates predicted overlap with host rear bumper for the current object
 *
 * @return void
 *
 * @SRS{CSCSA-38864}
 * @SAE{SF-2959}
 * @SDD{CSCSA-100398}
 * @verification{}
 */
static void Recw_Calculate_Overlap(Recw_Object_T *p_recw_object /**< RECW object */,
                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Filters the object heading by using the history of the positions of the objects
 *
 * @return void
 *
 * @SRS{SF-1697,SF-1659}
 * @SAE{SF-2959}
 * @SDD{SF-7836}
 * @verification{}
 */
static void Recw_Filter_Object_Heading(Recw_Object_T *p_recw_object /**< RECW object data */,
                                       Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                       const float32_T cycle_time /**< cycle time */);

/**
 * @brief Determines of a object is most critical and if so sets it in the core output.
 *
 * @return void
 *
 * @SRS{SF-1707}
 * @SAE{SF-2959}
 * @SDD{SF-7844}
 * @verification{}
 */
static void Recw_Set_Most_Critical_Object(Recw_Core_Output_T *p_core_output /**< RECW core output */,
                                          const Recw_Object_T *p_recw_object /**< RECW object data */);

/**
 * @brief Checks whether there is a object slower then a certain threshold. If true the blockage zone flag is set to true.
 *
 * @return True in case of slow object behind host vehicle
 *
 * @SRS{SF-1681}
 * @SAE{SF-2959}
 * @SDD{SF-7839}
 * @verification{}
 */
static boolean_T Recw_Is_Rear_Blocked_By_Object(const Recw_Object_T *p_recw_object /**< RECW object data */,
                                                const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */,
                                                const Recw_Persistent_T *p_persistent /**<RECW persistent data */,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Resets object persistent data.
 *
 * @return void
 *
 * @SRS{SF-1697,SF-1659}
 * @SAE{SF-2959}
 * @SDD{SF-7842}
 * @verification{}
 */
static void Recw_Reset_Object_Persistent(Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                         const uint8_t obj_id /**< object identifier */);

/**
 * @brief Resets attributes of recw object.
 *
 * @return void
 *
 * @SRS{SF-1697,SF-1659}
 * @SAE{SF-2959}
 * @SDD{SF-7843}
 * @verification{}
 */
static void Recw_Reset_Object(Recw_Object_T *p_recw_object /**< RECW object data */);

/**
 * @brief True if alert level shall be suppressed due to specific condition.
 *
 * @return boolean_T
 *
 * @SRS{SF-1697}
 * @SAE{SF-2959}
 * @SDD{CSCSA-308781}
 * @verification{Check if the alert level is suppressed when a specific condition is met}
 */

static boolean_T Recw_Is_Alert_Level_Suppressed(const Recw_Object_T *p_recw_object,    /**< RECW object data */
                                                const Recw_Core_Calibration_T *p_cals, /**< RECW calibration */
                                                const uint8_t index /**<index of RECW alert level */);

/**
 * @brief Sets the alert level of a recw object.
 *
 * @return void
 *
 * @SRS{SF-1709,SF-1708}
 * @SAE{SF-2959}
 * @SDD{SF-7845}
 * @verification{}
 */
static void Recw_Set_Object_Alert_Level(Recw_Object_T *p_recw_object /**< RECW object data */,
                                        const Recw_Core_Calibration_T *p_cals /**< RECW calibration */,
                                        const Recw_Persistent_T *p_persistent /**<RECW persistent data */);

/**
 * @brief Updates rear blockage information with information from current cycle.
 *
 * @return void
 *
 * @SRS{SF-1681}
 * @SAE{SF-2959}
 * @SDD{SF-8011}
 * @verification{}
 */
static void Recw_Update_Rear_Blockage(Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                      const boolean_T f_is_rear_blocked_current_cycle /**< Flag indicating current rear blockage */,
                                      const uint8_t rear_blockage_obj_idx_current_cycle /**< Index of currently blocking object */,
                                      const Recw_Core_Calibration_T *p_cals /**< RECW calibration */,
                                      const Pa_Data_T *p_pa_data);

/**
 * @brief Checks if the object that is responsible for the rear blockage is moving away from the blockage position.
 *
 * @return True if blocking object is moving away
 *
 * @SRS{SF-1681}
 * @SAE{SF-2959}
 * @SDD{SF-8012}
 * @verification{}
 */
static boolean_T Recw_Is_Blocking_Object_Moving_Away(Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                                     const Recw_Core_Calibration_T *p_cals /**< RECW calibration */,
                                                     const Pa_Data_T *p_pa_data);

/**
 * @brief Checks if an object is relevant for the rear blockage functionality. This is pre-check in order to calculate the
 * reference point only for relevant objects.
 *
 * @return True if object is relevant for rear blockage
 *
 * @SRS{SF-1681}
 * @SAE{SF-2959}
 * @SDD{SF-8013}
 * @verification{}
 */
static boolean_T Recw_Is_Object_Rear_Blockage_Relevant(const Recw_Object_T *p_recw_object /**< RECW object data */,
                                                       const Recw_Core_Calibration_T *p_cals /**< RECW calibration */,
                                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/*===========================================================================*\
* Global Function Defintions
\*===========================================================================*/

void Recw_Reset(Recw_Core_Output_T *p_core_output, Recw_Persistent_T *p_recw_persistent)
{
   /* Asserts */
   assert(NULL != p_core_output);

   /* reset persistent and output data */
   Recw_Reset_Persistent(p_recw_persistent);
   Recw_Reset_Core_Output(p_core_output);
}

void Recw_Core_Run(Recw_Core_Output_T *p_core_output,
                   const Recw_Core_Input_T *p_recw_core_input,
                   const Recw_Core_Calibration_T *p_cals,
                   Recw_Persistent_T *p_recw_persistent)
{
   Fbk_Vehicle_Data_T vehicle_data;

   /* Asserts */
   assert(NULL != p_recw_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_core_output);

   /* Reset debug data */
   Binary_Recw_Debug_Reset_Data();

   /* Fill vehicle data */
   vehicle_data = p_recw_core_input->p_pa_data->vehicle_data;

   /* Check if host speed is in allowed range */
   p_recw_persistent->f_host_speed_in_allowed_range = Recw_Is_Host_Speed_In_Allowed_Range(p_cals, p_recw_persistent, &vehicle_data);

   if (Fbk_Is_False(p_recw_core_input->f_enable_recw) || Fbk_Is_False(p_recw_persistent->f_host_speed_in_allowed_range))
   {
      Recw_Reset(p_core_output, p_recw_persistent);
   }
   else
   {
      Recw_Algorithm(p_core_output, p_recw_persistent, p_recw_core_input, p_cals, &vehicle_data,
                     p_recw_core_input->p_pa_data->guardrail_data);
   }

   /* Pass general data to debug data */
   Binary_Recw_Debug_Pass_Ego_Lane_Filter_Zones(&vehicle_data, p_cals);
   Binary_Recw_Debug_Pass_General_Data(p_recw_core_input, p_core_output, p_recw_persistent, p_cals);
}

void Recw_Reset_Core_Output(Recw_Core_Output_T *p_core_output)
{
   /* Assert */
   assert(NULL != p_core_output);

   p_core_output->recw_alert_level            = RECW_NO_ALERT;
   p_core_output->recw_id                     = PA_INVALID_OBJ_ID;
   p_core_output->recw_unique_id              = PA_INVALID_OBJ_ID;
   p_core_output->recw_index                  = FBK_ZERO_UINT;
   p_core_output->recw_ttc                    = RECW_MAX_TTC;
   p_core_output->recw_crash_prob_braking     = FBK_ZERO_F;
   p_core_output->recw_crash_prob_combined    = FBK_ZERO_F;
   p_core_output->recw_crash_prob_steering    = FBK_ZERO_F;
   p_core_output->ttc_threshold_alert_level_1 = FBK_ZERO_F;
   p_core_output->ttc_threshold_alert_level_2 = FBK_ZERO_F;
}

/*===========================================================================*\
 * Local Function Definitions
\*===========================================================================*/

static void Recw_Algorithm(Recw_Core_Output_T *p_core_output,
                           Recw_Persistent_T *p_persistent,
                           const Recw_Core_Input_T *p_recw_core_input,
                           const Recw_Core_Calibration_T *p_cals,
                           const Fbk_Vehicle_Data_T *p_vehicle_data,
                           const Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS])
{
   uint8_t object_index;
   boolean_T f_is_rear_blocked   = FBK_FALSE;
   uint8_t rear_blockage_obj_idx = PA_INVALID_OBJ_INDEX;


   /* Asserts */
   assert(NULL != p_core_output);
   assert(NULL != p_persistent);
   assert(NULL != p_recw_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_vehicle_data);
   assert(NULL != guardrail_data);

   Recw_Reset_Core_Output(p_core_output);

   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      if (Recw_Is_Object_State_Fulfilled(&p_recw_core_input->p_pa_data->object_data[object_index]))
      {
         Recw_Object_T recw_object;
         boolean_T object_relevant;
         boolean_T object_blocks_host_rear;
         Recw_Object_T *p_recw_object = &recw_object;

         /* Reset all attribute information for RECW object */
         Recw_Reset_Object(p_recw_object);
         p_recw_object->tracker_data = p_recw_core_input->p_pa_data->object_data[object_index];

         /* Update filtered object heading */
         Recw_Filter_Object_Heading(p_recw_object, p_persistent, p_recw_core_input->p_pa_data->time_diff_to_last_cycle);

         /* Update internal RECW object age */
         if (Fbk_Is_True(p_recw_object->tracker_data.f_merge_occured))
         {
            p_persistent->object_data[p_recw_object->tracker_data.id].age = FBK_ZERO_UINT;
         }
         else
         {
            Sat_Inc_Uint8(&(p_persistent->object_data[p_recw_object->tracker_data.id].age));
         }

         // Check weather object is relevant for RECW algorithm.
         object_relevant =
            Recw_Is_Object_Relevant(p_recw_object, p_persistent, p_vehicle_data, p_cals, &(p_persistent->car_wash_scenario_flags));
         if (object_relevant)
         {
            /* Calculate TTC of object */
            Recw_Calculate_Ttc(p_recw_object, p_vehicle_data, p_cals);

            /* Calculate object-dependent TTC thresholds */
            Recw_Calculate_Ttc_Thresholds(p_recw_object, p_persistent, p_cals);

            /* Calculate crash probabilities of object */
            Recw_Calculate_Crash_Probabilities(p_recw_object, p_vehicle_data, guardrail_data, p_cals);

            /* Calculate object's predicted overlap with host */
            Recw_Calculate_Overlap(p_recw_object, p_vehicle_data);

            if (p_recw_object->attributes.crash_prob_combined > p_cals->k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1])
            {
               /* Increase counter for consecutive cycles with minimal crash probability */
               Sat_Inc_Uint8(&(p_persistent->object_data[p_recw_object->tracker_data.id].consecutive_min_crash_prob_counter));
               if (p_persistent->object_data[p_recw_object->tracker_data.id].consecutive_min_crash_prob_counter
                   >= p_cals->k_recw_min_cycles_with_min_crash_prob)
               {
                  /* Calculate object alert level */
                  Recw_Set_Object_Alert_Level(p_recw_object, p_cals, p_persistent);

                  /* Check if object is most critical */
                  Recw_Set_Most_Critical_Object(p_core_output, p_recw_object);
               }
            }
            else
            {
               /* Reset counter for consecutive cycles with minimal crash probability */
               p_persistent->object_data[p_recw_object->tracker_data.id].consecutive_min_crash_prob_counter = FBK_ZERO_UINT;
            }
         }
         else
         {
            /* Reset counter for consecutive cycles with minimal crash probability */
            p_persistent->object_data[p_recw_object->tracker_data.id].consecutive_min_crash_prob_counter = FBK_ZERO_UINT;
         }

         /* Check if current object (or any previous checked object) blocks the ego rear */
         object_blocks_host_rear = Recw_Is_Rear_Blocked_By_Object(p_recw_object, p_cals, p_persistent, p_vehicle_data);
         if (object_blocks_host_rear)
         {
            f_is_rear_blocked     = FBK_TRUE;
            rear_blockage_obj_idx = object_index;
         }

         Binary_Recw_Debug_Pass_Object_Attributes(&p_recw_object->attributes, object_index);
      }
      else
      {
         /* Reset all object persistent data */
         Recw_Reset_Object_Persistent(p_persistent, p_recw_core_input->p_pa_data->object_data[object_index].id);
      }
   }

   /* Update the ego rear blockage with information from current cycle */
   Recw_Update_Rear_Blockage(p_persistent, f_is_rear_blocked, rear_blockage_obj_idx, p_cals, p_recw_core_input->p_pa_data);

   /* Apply alert qualification and holding */
   Recw_Debounce_Alert_Level(p_core_output, p_persistent, p_cals);
}

static void Recw_Update_Rear_Blockage(Recw_Persistent_T *p_persistent,
                                      const boolean_T f_is_rear_blocked_current_cycle,
                                      const uint8_t rear_blockage_obj_idx_current_cycle,
                                      const Recw_Core_Calibration_T *p_cals,
                                      const Pa_Data_T *p_pa_data)
{
   /* Asserts */
   assert(NULL != p_persistent);
   assert(NULL != p_cals);
   assert(NULL != p_pa_data);

   /* If ego speed is above rear blockage speed threshold or blocking object is moving away reset previous rear blockage */
   if ((Fbk_Abs_F(p_pa_data->vehicle_data.host_speed) > p_cals->k_recw_rear_blockage_ego_speed_threshold)
       || Recw_Is_Blocking_Object_Moving_Away(p_persistent, p_cals, p_pa_data))
   {
      p_persistent->f_is_rear_blocked                     = FBK_FALSE;
      p_persistent->recw_rear_blockage_qualifying_counter = FBK_ZERO_UINT;
      p_persistent->recw_rear_blockage_object_index       = PA_INVALID_OBJ_INDEX;
   }
   else
   {
      if (f_is_rear_blocked_current_cycle)
      {
         Sat_Inc_Uint8(&(p_persistent->recw_rear_blockage_qualifying_counter));
         if (p_persistent->recw_rear_blockage_qualifying_counter > p_cals->k_recw_rear_blockage_qualifying_cycles)
         {
            /* Set active rear blockage. This will stay active until ego speed increases above rear blockage speed threshold or
             * blocking object moves away. */
            p_persistent->f_is_rear_blocked               = FBK_TRUE;
            p_persistent->recw_rear_blockage_object_index = rear_blockage_obj_idx_current_cycle;
         }
      }
      else
      {
         p_persistent->recw_rear_blockage_qualifying_counter = FBK_ZERO_UINT;
      }
   }
}

static boolean_T Recw_Is_Blocking_Object_Moving_Away(Recw_Persistent_T *p_persistent,
                                                     const Recw_Core_Calibration_T *p_cals,
                                                     const Pa_Data_T *p_pa_data)
{
   boolean_T f_object_moving_away = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_persistent);
   assert(NULL != p_cals);

   if (p_persistent->f_is_rear_blocked && (PA_INVALID_OBJ_INDEX != p_persistent->recw_rear_blockage_object_index))
   {
      const Fbk_Object_Data_T *p_recw_rear_blockage_object = &p_pa_data->object_data[p_persistent->recw_rear_blockage_object_index];
      /* Check if the rear blocking object does not exist anymore. This is the case if the object status becomes invalid or (in
       * case when the F360 tracker reuses the object index without resetting the object status inbetween) if the object age
       * is not plausible. */
      if ((PA_OBJ_STATUS_INVALID == p_recw_rear_blockage_object->status)
          || (p_recw_rear_blockage_object->age < p_cals->k_recw_min_object_age))
      {
         /* Reset the rear blockage object index. Note that the rear blockage flag is unchanged, since the GDSR tracker cannot
          * track stationary objects and it is likely that the object is still there. However we cannot track it by its index
          * anymore, as it will be assigned a new object index once moving again. */
         p_persistent->recw_rear_blockage_object_index = PA_INVALID_OBJ_INDEX;
      }
      else
      {
         /* If we are sure that the blocking object does still exist, check if object is now outside of the blockage zone. */
         if ((Fbk_Abs_F(p_recw_rear_blockage_object->vcs_pos.y) > p_cals->k_recw_rear_blockage_width)
             || (p_recw_rear_blockage_object->vcs_pos.x
                 < -((2.0f * p_cals->k_recw_rear_blockage_length) + p_pa_data->vehicle_data.host_length)))
         {
            f_object_moving_away = FBK_TRUE;
         }
      }
   }

   return f_object_moving_away;
}

static boolean_T Recw_Is_Host_Speed_In_Allowed_Range(const Recw_Core_Calibration_T *p_cals,
                                                     const Recw_Persistent_T *p_persistent,
                                                     const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T min_host_speed                = p_cals->k_recw_min_host_speed[p_persistent->recw_alert_prev_cycle];
   float32_T max_host_speed                = p_cals->k_recw_max_host_speed[p_persistent->recw_alert_prev_cycle];
   boolean_T f_host_above_activation_speed = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_persistent);
   assert(NULL != p_vehicle_data);

   /* If host was previously above activation speed threshold, then apply hysteresis now */
   if (Fbk_Is_True(p_persistent->f_host_speed_in_allowed_range))
   {
      min_host_speed -= p_cals->k_recw_min_host_speed_hys;
      max_host_speed += p_cals->k_recw_max_host_speed_hys;
   }

   if ((p_vehicle_data->host_speed >= min_host_speed) && (p_vehicle_data->host_speed <= max_host_speed))
   {
      f_host_above_activation_speed = FBK_TRUE;
   }

   return f_host_above_activation_speed;
}

static boolean_T Recw_Is_Object_Rear_Blockage_Relevant(const Recw_Object_T *p_recw_object,
                                                       const Recw_Core_Calibration_T *p_cals,
                                                       const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_rear_blockage_relevant = FBK_FALSE;

   assert(NULL != p_recw_object);
   assert(NULL != p_cals);

   /* Here we use the object position to see if the object is close to the ego rear. For efficiency we use the same
    * calibraton values as for the reference point check and simply multiply them by two to get a first containment. */
   if ((p_recw_object->tracker_data.vcs_pos.x > -((2.0f * p_cals->k_recw_rear_blockage_length) + p_vehicle_data->host_length))
       && (p_recw_object->tracker_data.vcs_pos.x < -p_vehicle_data->host_length)
       && (Fbk_Abs_F(p_recw_object->tracker_data.vcs_pos.y) < p_cals->k_recw_rear_blockage_width))
   {
      f_rear_blockage_relevant = FBK_TRUE;
   }

   return f_rear_blockage_relevant;
}

static boolean_T Recw_Is_Rear_Blocked_By_Object(const Recw_Object_T *p_recw_object,
                                                const Recw_Core_Calibration_T *p_cals,
                                                const Recw_Persistent_T *p_persistent,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_rear_is_blocked = FBK_FALSE;
   Fbk_Ref_Point_T ref_point_vcs;

   assert(NULL != p_recw_object);
   assert(NULL != p_cals);
   assert(NULL != p_persistent);
   assert(NULL != p_vehicle_data);

   if (Recw_Is_Object_Rear_Blockage_Relevant(p_recw_object, p_cals, p_vehicle_data))
   {
      Fbk_Object_Corners_T target_corners;
      Vector_2d_T host_vcs_ref_point = Create_2d_Vector_Coordinates(-p_vehicle_data->host_length, FBK_ZERO_F);

      Fbk_Calculate_Target_Corners(&(target_corners), &(p_recw_object->tracker_data.vcs_pos),
                                   &(p_recw_object->tracker_data.vcs_heading), &(p_recw_object->tracker_data.length),
                                   &(p_recw_object->tracker_data.width));

      Fbk_Calculate_Ref_Point(&ref_point_vcs, &(host_vcs_ref_point), &(target_corners), FBK_FALSE);

      if ((Fbk_Abs_F(ref_point_vcs.point.y) < (0.5f * p_cals->k_recw_rear_blockage_width))
          && (ref_point_vcs.point.x > -(p_cals->k_recw_rear_blockage_length + p_vehicle_data->host_length))
          && (ref_point_vcs.point.x < -(p_vehicle_data->host_length)) && (p_recw_object->tracker_data.vcs_vel.x > 0.0f)
          && ((PA_OBJ_CLASS_CAR == p_recw_object->tracker_data.obj_class)
              || (PA_OBJ_CLASS_TRUCK == p_recw_object->tracker_data.obj_class) || (p_persistent->f_is_rear_blocked)))
      {
         if ((p_recw_object->tracker_data.speed < p_cals->k_recw_rear_blockage_speed_threshold)
             && ((FBK_FRONT_MID == ref_point_vcs.ref_point_index) || (FBK_FRONT_RIGHT_CORNER == ref_point_vcs.ref_point_index)
                 || (FBK_FRONT_LEFT_CORNER == ref_point_vcs.ref_point_index)))
         {
            f_rear_is_blocked = FBK_TRUE;
         }
      }
   }

   return f_rear_is_blocked;
}

static void Recw_Filter_Object_Heading(Recw_Object_T *p_recw_object, Recw_Persistent_T *p_persistent, const float32_T cycle_time)
{
   uint8_t obj_id = p_recw_object->tracker_data.id;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_persistent);

   p_recw_object->attributes.filtered_heading = p_recw_object->tracker_data.vcs_heading;

   if (p_persistent->object_data[obj_id].last_object_pos.x < RECW_INVALID_LON_POS)
   {
      Vector_2d_T diff_pos =
         Vector_2d_Alg_Diff(&p_recw_object->tracker_data.vcs_pos, &p_persistent->object_data[obj_id].last_object_pos);
      float32_T filter_coeff;

      if (p_persistent->object_data[obj_id].filter_counter < RECW_MIN_POS_DIFF_FILTER_CYCLES)
      {
         p_persistent->object_data[obj_id].filter_counter++;
      }

      filter_coeff = 1.0f / ((float32_T) p_persistent->object_data[obj_id].filter_counter);

      p_recw_object->attributes.filtered_diff_pos.x =
         ((1.0f - filter_coeff) * p_persistent->object_data[obj_id].last_object_pos_filtered.x) + (filter_coeff * diff_pos.x);
      p_recw_object->attributes.filtered_diff_pos.y =
         ((1.0f - filter_coeff) * p_persistent->object_data[obj_id].last_object_pos_filtered.y) + (filter_coeff * diff_pos.y);

      if ((RECW_MIN_POS_DIFF_FILTER_CYCLES == p_persistent->object_data[obj_id].filter_counter) && (cycle_time > EPSILON))
      {
         p_recw_object->attributes.effective_rel_vel.x = p_recw_object->attributes.filtered_diff_pos.x / cycle_time;
         p_recw_object->attributes.effective_rel_vel.y = p_recw_object->attributes.filtered_diff_pos.y / cycle_time;

         if (p_recw_object->attributes.filtered_diff_pos.x > RECW_MIN_FILTERED_DIFF_POS)
         {
            p_recw_object->attributes.filtered_heading =
               Fast_Atan2(p_recw_object->attributes.filtered_diff_pos.y, p_recw_object->attributes.filtered_diff_pos.x);
         }
      }
      else
      {
         p_recw_object->attributes.effective_rel_vel =
            Create_2d_Vector_Coordinates(p_recw_object->tracker_data.vcs_vel_rel.x, p_recw_object->tracker_data.vcs_vel_rel.y);
      }
   }

   p_persistent->object_data[obj_id].last_object_pos          = p_recw_object->tracker_data.vcs_pos;
   p_persistent->object_data[obj_id].last_object_pos_filtered = p_recw_object->attributes.filtered_diff_pos;
}

static boolean_T Recw_Is_Object_State_Fulfilled(const Fbk_Object_Data_T *p_object_data)
{
   Pa_Obj_Status_T tracker_obj_state;
   boolean_T f_is_obj_state_fulfilled = FBK_FALSE;

   /* Assert */
   assert(NULL != p_object_data);

   tracker_obj_state = p_object_data->status;

   if (Fbk_Is_Obj_State_Valid(tracker_obj_state) && (Fbk_Is_False(p_object_data->f_reflection)))
   {
      f_is_obj_state_fulfilled = FBK_TRUE;
   }

   return f_is_obj_state_fulfilled;
}


static void Recw_Calculate_Ttc(Recw_Object_T *p_recw_object,
                               const Fbk_Vehicle_Data_T *p_vehicle_data,
                               const Recw_Core_Calibration_T *p_cals)
{
   float32_T lon_rel_vel;
   float32_T lon_rel_acc;
   float32_T lon_dist_rb_fb;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   p_recw_object->attributes.ttc = RECW_MAX_TTC;
   lon_dist_rb_fb = p_recw_object->tracker_data.vcs_pos.x + p_vehicle_data->host_length + (0.5f * p_recw_object->tracker_data.length);
   lon_rel_vel    = p_recw_object->tracker_data.vcs_vel_rel.x;
   lon_rel_acc    = p_recw_object->tracker_data.vcs_accel.x - p_vehicle_data->long_acc;

   if (Fbk_Abs_F(lon_rel_acc) < RECW_MIN_ACCEL_THRESHOLD)
   {
      /* check if the relative longitudinal velocity is close to zero */
      if (lon_rel_vel < EPSILON)
      {
         /* check if the longitudinal position of the target with respect to the
          * ego vehicle rear bumper is inside the interval of -0.5m and +0.5m
          */
         if ((lon_dist_rb_fb > -0.5f) && (lon_dist_rb_fb < 0.5f))
         {
            /* inside the interval */
            p_recw_object->attributes.ttc = FBK_ZERO_F;
         }
         else
         {
            /* outside of the interval */
            p_recw_object->attributes.ttc = RECW_MAX_TTC; /* TTC is set to default value */
         }
      }
      else
      {
         p_recw_object->attributes.ttc = ((-lon_dist_rb_fb) / lon_rel_vel) - p_cals->k_recw_average_sensor_latency;
      }
   }
   else
   {
      float32_T radicand;
      float32_T sqrt_term;

      radicand  = (lon_rel_vel * lon_rel_vel) - ((2.0f * lon_rel_acc) * lon_dist_rb_fb);
      sqrt_term = Fast_Sqrt(Fbk_Abs_F(radicand));

      if (radicand < 0.0f)
      {
         p_recw_object->attributes.ttc = ((-lon_rel_vel) / lon_rel_acc) - p_cals->k_recw_average_sensor_latency;
      }
      else
      {
         float32_T sol1 = (-lon_rel_vel + sqrt_term) / lon_rel_acc;
         float32_T sol2 = (-lon_rel_vel - sqrt_term) / lon_rel_acc;

         if ((sol1 > 0.0f) && (sol2 > 0.0f))
         {
            p_recw_object->attributes.ttc = Min(sol1, sol2) - p_cals->k_recw_average_sensor_latency;
         }
         else
         {
            p_recw_object->attributes.ttc = Max(sol1, sol2) - p_cals->k_recw_average_sensor_latency;
         }
      }
   }
}

static void Recw_Calculate_Ttc_Thresholds(Recw_Object_T *p_recw_object,
                                          const Recw_Persistent_T *p_persistent,
                                          const Recw_Core_Calibration_T *p_cals)
{
   uint8_t index;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_persistent);
   assert(NULL != p_cals);

   for (index = FBK_ZERO_UINT; index < (uint8_t) RECW_NUMBER_ALERT_LEVEL; index++)
   {
      float32_T rel_velocity              = p_recw_object->tracker_data.vcs_vel_rel.x;
      float32_T min_rel_vel               = p_cals->k_recw_min_rel_velocity[index];
      float32_T max_rel_vel               = p_cals->k_recw_max_rel_velocity[index + FBK_ONE_UINT];
      float32_T min_rel_vel_max_ttc_thres = p_cals->k_recw_min_rel_velocity_for_max_ttc_threshold[index];
      float32_T ratio;

      /* Check if hysteresis should be applied for this object */
      if (p_persistent->recw_id_prev_cycle == p_recw_object->tracker_data.id)
      {
         min_rel_vel -= p_cals->k_recw_min_rel_velocity_hys[index];
         max_rel_vel += p_cals->k_recw_max_rel_velocity_hys;
      }

      /* Calculate TTC threshold for current alert level */
      if ((rel_velocity > min_rel_vel) && (rel_velocity < min_rel_vel_max_ttc_thres))
      {
         /* TTC increases linearly in this range, thus first calculate ratio */
         ratio = (rel_velocity - min_rel_vel) / (min_rel_vel_max_ttc_thres - min_rel_vel);
      }
      else if ((rel_velocity >= min_rel_vel_max_ttc_thres) && (rel_velocity <= max_rel_vel))
      {
         /* Use max TTC threshold in this range */
         ratio = FBK_ONE_F;
      }
      else
      {
         /* Otherwise the TTC threshold should be zero */
         ratio = FBK_ZERO_F;
      }

      /* Set ttc threshold */
      p_recw_object->attributes.ttc_threshold[index] = ratio * p_cals->k_recw_max_ttc_threshold[index];
   }
}

static void Recw_Calculate_Overlap(Recw_Object_T *p_recw_object, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T obj_long_distance;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_vehicle_data);

   /* Calculate longitudinal distance from host's rear bumper to the front of object */
   obj_long_distance =
      p_recw_object->tracker_data.vcs_pos.x + Fbk_Half(p_recw_object->tracker_data.length) + p_vehicle_data->host_length;

   /* Calculate overlap only when object position is to the rear of host bumper */
   if (obj_long_distance <= FBK_ZERO_F)
   {
      /* Calculate object's heading factor */
      float32_T obj_heading_factor =
         (Fbk_Half(p_recw_object->tracker_data.length) - obj_long_distance) * Fast_Tan(p_recw_object->tracker_data.vcs_heading);

      /* Project object's corners to the rear bumper of host */
      float32_T obj_left_corner =
         p_recw_object->tracker_data.vcs_pos.y - Fbk_Half(p_recw_object->tracker_data.width) + obj_heading_factor;
      float32_T obj_right_corner =
         p_recw_object->tracker_data.vcs_pos.y + Fbk_Half(p_recw_object->tracker_data.width) + obj_heading_factor;

      /* Check for overlap */
      Float_Range_T obj_range  = Create_Float_Range(obj_left_corner, obj_right_corner);
      Float_Range_T host_range = Create_Float_Range(-Fbk_Half(p_vehicle_data->host_width), Fbk_Half(p_vehicle_data->host_width));

      if (Fbk_Is_True(Does_Float_Range_Overlap_Float_Range(&obj_range, &host_range)))
      {
         /* Calculate overlap level */
         /* For objects when object_width < host_width use object_width as denominator, so it gets full overlap */
         /* Limit the output value to 1.0f to avoid float imprecision */
         obj_range.min = Fbk_Max(obj_range.min, host_range.min);
         obj_range.max = Fbk_Min(obj_range.max, host_range.max);

         p_recw_object->attributes.overlap = Fbk_Min(
            ((obj_range.max - obj_range.min) / Fbk_Min(p_vehicle_data->host_width, p_recw_object->tracker_data.width)), FBK_ONE_F);
         p_recw_object->attributes.overlap_line_y_min = obj_range.min;
         p_recw_object->attributes.overlap_line_y_max = obj_range.max;
      }
      else
      {
         /* No overlap */
         p_recw_object->attributes.overlap            = FBK_ZERO_F;
         p_recw_object->attributes.overlap_line_y_min = FBK_ZERO_F;
         p_recw_object->attributes.overlap_line_y_max = FBK_ZERO_F;
      }
   }
   else
   {
      /* Object is not in the back of host */
      p_recw_object->attributes.overlap            = FBK_ZERO_F;
      p_recw_object->attributes.overlap_line_y_min = FBK_ZERO_F;
      p_recw_object->attributes.overlap_line_y_max = FBK_ZERO_F;
   }
}

static void Recw_Set_Most_Critical_Object(Recw_Core_Output_T *p_core_output, const Recw_Object_T *p_recw_object)
{
   /* Asserts */
   assert(NULL != p_core_output);
   assert(NULL != p_recw_object);

   if (RECW_NO_ALERT != p_recw_object->attributes.alert_level)
   {
      /* Check if crash probability and alert level of new object are higher */
      if ((p_recw_object->attributes.crash_prob_combined > p_core_output->recw_crash_prob_combined)
          && (p_recw_object->attributes.alert_level >= p_core_output->recw_alert_level))
      {
         p_core_output->recw_alert_level            = p_recw_object->attributes.alert_level;
         p_core_output->recw_id                     = p_recw_object->tracker_data.id;
         p_core_output->recw_unique_id              = p_recw_object->tracker_data.unique_id;
         p_core_output->recw_index                  = p_recw_object->tracker_data.index;
         p_core_output->recw_ttc                    = p_recw_object->attributes.ttc;
         p_core_output->recw_crash_prob_braking     = p_recw_object->attributes.crash_prob_braking;
         p_core_output->recw_crash_prob_combined    = p_recw_object->attributes.crash_prob_combined;
         p_core_output->recw_crash_prob_steering    = p_recw_object->attributes.crash_prob_steering;
         p_core_output->ttc_threshold_alert_level_1 = p_recw_object->attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1];
         p_core_output->ttc_threshold_alert_level_2 = p_recw_object->attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2];
      }
   }
}

static void Recw_Reset_Object_Persistent(Recw_Persistent_T *p_persistent, const uint8_t obj_id)
{
   /* Assert */
   assert(NULL != p_persistent);

   p_persistent->object_data[obj_id].consecutive_min_crash_prob_counter = FBK_ZERO_UINT;
   p_persistent->object_data[obj_id].age                                = FBK_ZERO_UINT;
   p_persistent->object_data[obj_id].object_within_lane_counter         = FBK_ZERO_UINT;
   p_persistent->object_data[obj_id].last_object_pos.x                  = RECW_INVALID_LON_POS;
   p_persistent->object_data[obj_id].last_object_pos.y                  = FBK_ZERO_F;
   p_persistent->object_data[obj_id].last_object_pos_filtered           = Create_2d_Vector_Origin();
   p_persistent->object_data[obj_id].filter_counter                     = FBK_ZERO_UINT;
}


static void Recw_Reset_Object(Recw_Object_T *p_recw_object)
{
   /* Assert */
   assert(NULL != p_recw_object);

   /* Reset tracker data */
   Fbk_Reset_Object_Data(&(p_recw_object->tracker_data));

   /* Reset attributes */
   p_recw_object->attributes.crash_prob_braking  = FBK_ZERO_F;
   p_recw_object->attributes.crash_prob_steering = FBK_ZERO_F;
   p_recw_object->attributes.crash_prob_combined = FBK_ZERO_F;

   p_recw_object->attributes.needed_brake_acceleration    = FBK_ZERO_F;
   p_recw_object->attributes.needed_steering_acceleration = FBK_ZERO_F;

   p_recw_object->attributes.filtered_diff_pos = Create_2d_Vector_Origin();
   p_recw_object->attributes.effective_rel_vel = Create_2d_Vector_Origin();

   p_recw_object->attributes.filtered_heading   = FBK_ZERO_F;
   p_recw_object->attributes.overlap            = FBK_ZERO_F;
   p_recw_object->attributes.overlap_line_y_min = FBK_ZERO_F;
   p_recw_object->attributes.overlap_line_y_max = FBK_ZERO_F;

   p_recw_object->attributes.ttc                                     = RECW_MAX_TTC;
   p_recw_object->attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] = FBK_ZERO_F;
   p_recw_object->attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] = FBK_ZERO_F;

   p_recw_object->attributes.alert_level = RECW_NO_ALERT;

   p_recw_object->attributes.f_object_is_car_wash_ghost = FBK_FALSE;
   p_recw_object->attributes.f_obj_is_within_lane       = FBK_FALSE;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Recw_Reset_Persistent(Recw_Persistent_T *p_persistent)
{
   uint8_t obj_id;

   /* Assert */
   assert(NULL != p_persistent);

   for (obj_id = 0; obj_id < RECW_MAX_ID_ARRAY_SIZE; obj_id++)
   {
      Recw_Reset_Object_Persistent(p_persistent, obj_id);
   }

   p_persistent->recw_alert_qualifying_counter         = FBK_ZERO_UINT;
   p_persistent->recw_alert_holding_counter            = FBK_ZERO_UINT;
   p_persistent->recw_alert_duration_counter           = FBK_ZERO_UINT;
   p_persistent->recw_rear_blockage_qualifying_counter = FBK_ZERO_UINT;
   p_persistent->recw_rear_blockage_object_index       = PA_INVALID_OBJ_INDEX;
   p_persistent->recw_id_prev_cycle                    = PA_INVALID_OBJ_ID;
   p_persistent->recw_index_prev_cycle                 = FBK_ZERO_UINT;
   p_persistent->recw_alert_prev_cycle                 = RECW_NO_ALERT;
   p_persistent->recw_ttc_value_hold                   = FBK_ZERO_F;
   p_persistent->f_is_rear_blocked                     = FBK_FALSE;
   p_persistent->f_host_speed_in_allowed_range         = FBK_FALSE;

   /* init car wash flags */
   Recw_Init_Car_Wash_Flags(p_persistent->car_wash_scenario_flags.possible_car_wash_scenario_flags);
}
static boolean_T Recw_Is_Alert_Level_Suppressed(const Recw_Object_T *p_recw_object,
                                                const Recw_Core_Calibration_T *p_cals,
                                                const uint8_t index)
{
   boolean_T is_suppressed = FBK_FALSE;

   assert(NULL != p_recw_object);
   assert(NULL != p_cals);

   /* Suppress alert level 2 */
   if ((uint8_t) RECW_INDEX_ALERT_LEVEL_2 == index)
   {
      /* Suppress alert level for pedestrian */
      if (Fbk_Is_True(p_cals->k_recw_f_suppress_alert_lvl_2_for_pedestrian)
          && (PA_OBJ_CLASS_PEDESTRIAN == p_recw_object->tracker_data.obj_class))
      {
         is_suppressed = FBK_TRUE;
      }
   }

   return is_suppressed;
}

static void Recw_Set_Object_Alert_Level(Recw_Object_T *p_recw_object,
                                        const Recw_Core_Calibration_T *p_cals,
                                        const Recw_Persistent_T *p_persistent)
{
   uint8_t index;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_cals);
   assert(NULL != p_persistent);

   /* Set alert level to no alert before checking alert levels */
   p_recw_object->attributes.alert_level = RECW_NO_ALERT;

   /* Check for both alert levels in loop. First check for alert level 1, such that it is overwritten by alert level 2 in
    * case this is active. Due to the lower bound for the ttc an object can have alert level 2 without having alert level 1.
    */
   for (index = FBK_ZERO_UINT; index < (uint8_t) RECW_NUMBER_ALERT_LEVEL; index++)
   {
      if (p_recw_object->attributes.crash_prob_combined >= p_cals->k_recw_min_crash_prob[index])
      {
         /* Check if rear is blocked */
         boolean_T f_is_rear_blocked =
            (boolean_T) (Fbk_Is_True(p_cals->k_recw_f_use_rear_blockage[index]) && Fbk_Is_True(p_persistent->f_is_rear_blocked));

         /* Check if object is in coasted state */
         boolean_T f_is_object_coasted = (boolean_T) (Fbk_Is_False(p_cals->k_recw_f_allow_alert_on_coasted_objects[index])
                                                      && Fbk_Is_True(p_recw_object->tracker_data.status == PA_OBJ_STATUS_COASTED));

         /* Check if ttc is valid for alert output */
         boolean_T f_ttc_in_range = (boolean_T) ((p_recw_object->attributes.ttc <= p_recw_object->attributes.ttc_threshold[index])
                                                 && (p_recw_object->attributes.ttc >= p_cals->k_recw_min_ttc_for_alert_level[index]));

         /* Check if object has valid stage age */
         boolean_T f_valid_stage_age =
            (boolean_T) (p_recw_object->tracker_data.stage_age >= p_cals->k_recw_min_stage_age_for_alert_level[index]);

         /* Check object's existance probability only for alert level 2.
          * Existence probability for alert level 1 (with hysteresis) has been checked in object validator */
         boolean_T f_valid_existence_prob =
            (boolean_T) (((uint8_t) RECW_INDEX_ALERT_LEVEL_1 == index)
                         || (p_recw_object->tracker_data.existence_probability >= p_cals->k_recw_min_existence_prob[index]));

         /* Suppress alert level for special cases */
         boolean_T suppress_alert_level = Recw_Is_Alert_Level_Suppressed(p_recw_object, p_cals, index);

         /* Check object's overlap with host */
         boolean_T f_obj_has_min_overlap =
            (boolean_T) (p_recw_object->attributes.overlap >= p_cals->k_recw_min_overlap_for_alert_level[index]);

         /* Set alert level */
         if (Fbk_Is_False(f_is_rear_blocked) && Fbk_Is_False(f_is_object_coasted) && Fbk_Is_True(f_ttc_in_range)
             && Fbk_Is_True(f_valid_stage_age) && Fbk_Is_True(f_valid_existence_prob) && Fbk_Is_True(f_obj_has_min_overlap)
             && Fbk_Is_False(suppress_alert_level))
         {
            if ((uint8_t) RECW_INDEX_ALERT_LEVEL_1 == index)
            {
               p_recw_object->attributes.alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
            }
            else
            {
               p_recw_object->attributes.alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
            }
         }
      }
   }
}
