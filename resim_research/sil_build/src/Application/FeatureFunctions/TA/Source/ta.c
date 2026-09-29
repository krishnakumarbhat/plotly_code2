/**
 * @file ta.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the functions that are called by the platform SW
 * for initialization, cyclic execution and obtaining outputs of the TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta.h"
#include "fbk_ego_traj_predictor.h"
#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_macros.h"
#include "fbk_obj_traj_predictor.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_traj_predictor_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_collision_filter.h"
#include "ta_common_functions.h"
#include "ta_constants.h"
#include "ta_debug_interface.h"
#include "ta_factory.h"
#include "ta_object_filter.h"
#include "ta_persistent_t.h"
#include "ta_types.h"
#include "ta_warn_logic.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/
/**
 * @brief local static variable holding the persistence data
 * @SDD{SF-8653}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/

/**
 * @brief Resets the TA object struct.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8649}
 * @verification{Check that the ta object is reset to its default.}
 */
static void Ta_Reset_Object(Ta_Object_T *p_ta_object /**< TA Object */,
                            const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Checks if object is valid.
 *
 * @return true if the object is valid for TA
 *
 * @SRS{SF-2334}
 * @SAE{SF-3238}
 * @SDD{SF-8647}
 * @verification{Check whether the object status as well as its id is valid.}
 */
static boolean_T Ta_Is_Object_Valid(const Pa_Data_T *p_pa_data /**< Tracker Output */,
                                    const uint8_t object_index /**< object index */);

/**
 * @brief Fills the TA Object using tracker and persistent data.
 *
 * @return void
 *
 * @SRS{SF-2301}
 * @SAE{SF-3238}
 * @SDD{SF-8646}
 * @verification{Check that the object information is updated correctly. Curvi data is only allowed to be set when it is
 * available.}
 */
static void Ta_Fill_Object_Information(Ta_Object_T *p_ta_object /**< TA Object */,
                                       const Ta_Core_Input_T *p_ta_core_input /**< TA Core Input */,
                                       const Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                       const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                                       const uint8_t object_index /**< object index */);

/**
 * @brief Fills the object specific persistent data.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8644}
 * @verification{Check that the alert mode is set correctly dependend on persistent data.}
 */
static void Ta_Fill_Object_Persistent_Data(Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                           const Ta_Object_T *p_ta_object /**< TA Object */,
                                           const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Resets the object specific persistent data.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8648}
 * @verification{Check that the alert mode is reset correctly.}
 */
static void Ta_Reset_Object_Persistent_Data(Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                            const uint8_t object_index /**< object index */);

/**
 * @brief Fills the side specific persistent data.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8645}
 * @verification{Check that the side dependend persistent data is reset.}
 */
static void Ta_Fill_Side_Persistent_Data(Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                         const Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */);

/**
 * @brief Updates the ego yaw angle
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8540}
 * @verification{Check that the ego yaw angle is set correctly.}
 */
static void Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section(Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                                             const Pa_Data_T *p_pa_data /**< PA Context */,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                             const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Debounces the alert levels using qualifying and holding counters.
 *
 * @return void
 *
 * @SRS{SF-2357,SF-2358}
 * @SAE{SF-3238}
 * @SDD{SF-8739}
 * @verification{Create tests where outputs need to qualify for an alert or where outputs are held for a successive amount of
 * cycles. Only if conditions are not fulfilled, the ta core output shall be reset.}
 */
static void Ta_Debounce_Alert_Level(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                    Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                    const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);
/**
 * @brief TA cyclic function
 *
 * Executes the TA calculations consisting of
 * - reset of data structures from previous run
 * - update of prediction of ego trajectory
 * - for relevant objects: update their trajectory and check for collision with ego
 * - evaluate possible collision scenario and set alert levels accordingly
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8651}
 * @verification{Check that main algorithm is not returning alert level when executed with invalid signals.}
 *
 */
static void Ta_Algorithm(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                         Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                         const Ta_Core_Input_T *p_ta_core_input /**< TA Core Input */,
                         const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                         Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance);

/**
 * @brief Reset persistent data of Ta.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-3238}
 * @SDD{SF-8620}
 * @verification{Check that the persistent defaults are reset in case that enable flag is set to false.}
 */
static void Ta_Reset_Persistent(Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                const Ta_Core_Calibration_T *p_ta_cals /**< TA calibrations*/);


/**
 * @brief Update the algorithm state of TA based on various object counters and the vehicle state relevancy.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-3238}
 * @SDD{SF-8627}
 * @verification{}
 */
static void Ta_Update_Algorithm_State(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */);

/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/

static void Ta_Update_Algorithm_State(Ta_Core_Output_T *p_ta_core_output)
{
   /* Assert */
   assert(NULL != p_ta_core_output);

   /* Set state based on number of objects and vehicle state relevance */
   if (FBK_ZERO_UINT == p_ta_core_output->ta_n_valid_objects)
   {
      p_ta_core_output->ta_algorithm_state = TA_STATE_NO_VALID_OBJECTS;
   }
   else if (Fbk_Is_False(p_ta_core_output->ta_f_vehicle_state_relevant))
   {
      p_ta_core_output->ta_algorithm_state = TA_STATE_VEHICLE_STATE_INVALID;
   }
   else if (FBK_ZERO_UINT == p_ta_core_output->ta_n_relevant_objects)
   {
      p_ta_core_output->ta_algorithm_state = TA_STATE_NO_RELEVANT_OBJECTS;
   }
   else if (FBK_ZERO_UINT == p_ta_core_output->ta_n_critical_objects)
   {
      p_ta_core_output->ta_algorithm_state = TA_STATE_NO_CRITICAL_OBJECTS;
   }
   else
   {
      p_ta_core_output->ta_algorithm_state = TA_STATE_CRITICAL_OBJECT_DETECTED;
   }
}

static void Ta_Reset_Persistent(Ta_Persistent_T *p_ta_persistent, const Ta_Core_Calibration_T *p_ta_cals)
{
   uint8_t object_idx;
   uint8_t side_idx;

   Ta_Init_Prediction_Time_Step(p_ta_persistent, p_ta_cals);

   for (object_idx = FBK_ZERO_UINT; object_idx < FBK_NUMBER_OF_SIDES; object_idx++)
   {
      Ta_Reset_Object_Persistent_Data(p_ta_persistent, object_idx);
   }

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      p_ta_persistent->ta_side_alert_prev_cycle[side_idx]         = TA_ALERT_STATE_NONE;
      p_ta_persistent->ta_side_id_prev_cycle[side_idx]            = PA_INVALID_OBJ_ID;
      p_ta_persistent->ta_side_index_prev_cycle[side_idx]         = PA_INVALID_OBJ_INDEX;
      p_ta_persistent->ta_side_alert_qualifying_counter[side_idx] = FBK_ZERO_UINT;
      p_ta_persistent->ta_side_alert_holding_counter[side_idx]    = FBK_ZERO_UINT;
   }
}

static void Ta_Reset_Object(Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Assert */
   assert(NULL != p_ta_object);

   /* Reset FBK object data */
   Fbk_Reset_Object_Data(&p_ta_object->tracker_data);

   /* Reset object trajectory */
   Ta_Reset_Trajectory(&p_ta_object->attributes.trajectory, p_ta_cal);

   /* Reset all ta object properties */
   p_ta_object->attributes.object_class_probability_vru = FBK_ZERO_F;
   p_ta_object->attributes.velocity_heading             = FBK_ZERO_F;
   p_ta_object->attributes.f_curvi_available            = FBK_FALSE;

   /* TA specific flags*/
   p_ta_object->attributes.f_vehicle_state_relevant = FBK_FALSE;
   p_ta_object->attributes.f_obj_ta_relevant        = FBK_FALSE;
   p_ta_object->attributes.f_obj_in_danger_zone     = FBK_FALSE;
   p_ta_object->attributes.f_obj_in_info_zone       = FBK_FALSE;
   p_ta_object->attributes.f_obj_in_wing_zone       = FBK_FALSE;

   /* Ta specific information */
   p_ta_object->attributes.ta_alert_mode    = TA_ALERT_MODE_NONE;
   p_ta_object->attributes.ego_heading_diff = FBK_ZERO_F;

   /* Object criticality */
   p_ta_object->attributes.waypoint_at_collision = Create_2d_Vector_Origin();
   p_ta_object->attributes.ttc                   = TA_INVALID_TTC;
   p_ta_object->attributes.ttp                   = TA_INVALID_TTP;
   p_ta_object->attributes.distance_to_ego       = TA_INVALID_DISTANCE;
   p_ta_object->attributes.alert_level           = TA_ALERT_STATE_NONE;
   p_ta_object->attributes.alert_side            = FBK_SIDE_UNDEFINED;

   /* Brake related */
   p_ta_object->attributes.decel_to_avoid_coll = FBK_ZERO_F;
   p_ta_object->attributes.ttb                 = TA_INVALID_TTB;
}

static boolean_T Ta_Is_Object_Valid(const Pa_Data_T *p_pa_data, const uint8_t object_index)
{
   /* Return flag */
   boolean_T f_object_valid = FBK_FALSE;
   /* Asserts */
   assert(NULL != p_pa_data);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Check general tracker object properties for validity */
   if ((p_pa_data->object_data[object_index].id > FBK_ZERO_UINT) && Fbk_Is_Obj_State_Valid(p_pa_data->object_data[object_index].status))
   {
      f_object_valid = FBK_TRUE;
   }

   return f_object_valid;
}

static void Ta_Fill_Object_Information(Ta_Object_T *p_ta_object,
                                       const Ta_Core_Input_T *p_ta_core_input,
                                       const Ta_Persistent_T *p_ta_persistent,
                                       const Ta_Core_Calibration_T *p_ta_cal,
                                       const uint8_t object_index)
{
   /* Pointer to tracker output */
   const Pa_Data_T *p_pa_data = p_ta_core_input->p_pa_data;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_core_input);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);
   assert(p_pa_data->object_data[object_index].existence_probability <= FBK_ONE_F);
   assert(p_pa_data->object_data[object_index].class_prob_pedestrian + p_pa_data->object_data[object_index].class_prob_2wheel
          <= FBK_ONE_F + EPSILON);

   /* Fill FBK object data */
   p_ta_object->tracker_data = p_pa_data->object_data[object_index];

   /* Fill additional info for TA object */
   p_ta_object->attributes.area = p_ta_object->tracker_data.length * p_ta_object->tracker_data.width;

   p_ta_object->tracker_data.vcs_accel.x = p_ta_object->tracker_data.vcs_accel.x * p_ta_cal->k_ta_obj_acceleration_long_weight;
   p_ta_object->tracker_data.vcs_accel.y = p_ta_object->tracker_data.vcs_accel.y * p_ta_cal->k_ta_obj_acceleration_lat_weight;

   p_ta_object->attributes.object_class_probability_vru =
      Fbk_Clamp(p_pa_data->object_data[object_index].class_prob_pedestrian + p_pa_data->object_data[object_index].class_prob_2wheel,
                FBK_ZERO_F, FBK_ONE_F);

   /* Check if curvi signals are available */
   if (PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL == p_ta_object->tracker_data.curvi_coordinates_calc_method)
   {
      p_ta_object->attributes.f_curvi_available = FBK_TRUE;
   }

   /* Check if object was active in previous cycle */
   p_ta_object->attributes.ta_alert_mode = p_ta_persistent->ta_alert_mode[p_ta_object->tracker_data.index];

   /* Calculate the objects heading difference to the ego vehicles last known straight section */
   p_ta_object->attributes.ego_heading_diff =
      Fbk_Abs_F(p_ta_persistent->ta_ego_yaw_angle_to_last_straight_section + p_ta_object->tracker_data.vcs_heading);

   /* Calculate velocity heading based on velocity vector */
   p_ta_object->attributes.velocity_heading = Vector_2d_Alg_Angle_From_Vector(&p_ta_object->tracker_data.vcs_vel).angle;

   /* Debug mode enables the manipulation of the TA object position */
   if (Fbk_Is_True(p_ta_cal->k_f_ta_enable_debug_mode) && Fbk_Is_True(p_ta_core_input->f_enable_debug_mode))
   {
      p_ta_object->tracker_data.vcs_pos.x = p_ta_object->tracker_data.vcs_pos.x + p_ta_core_input->debug_mode_obj_pos_long_offset;
      p_ta_object->tracker_data.vcs_pos.y = p_ta_object->tracker_data.vcs_pos.y + p_ta_core_input->debug_mode_obj_pos_lat_offset;
   }
}

static void Ta_Fill_Object_Persistent_Data(Ta_Persistent_T *p_ta_persistent,
                                           const Ta_Object_T *p_ta_object,
                                           const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_object);

   /* Fill persistent information for next cycle */
   if (TA_ALERT_STATE_NONE != p_ta_object->attributes.alert_level)
   {
      /* check if FTA or RTA alert is raised */
      boolean_T f_alert_front = (boolean_T) (Fbk_Is_True(p_ta_object->attributes.f_obj_in_danger_zone));
      boolean_T f_alert_rear = (boolean_T) (p_ta_object->attributes.f_obj_in_info_zone || p_ta_object->attributes.f_obj_in_wing_zone);

      /* check if alert is raised for FRONT and REAR zone */
      if (f_alert_front && f_alert_rear)
      {
         p_ta_persistent->ta_alert_mode[p_ta_object->tracker_data.index] = TA_ALERT_MODE_BOTH;
      }
      /* check if alert is raised for FRONT or REAR zone, but treat them equally due to cal value */
      else if ((f_alert_front || f_alert_rear) && Fbk_Is_True(p_ta_cal->k_ta_always_overwrite_ta_mode_to_both))
      {
         p_ta_persistent->ta_alert_mode[p_ta_object->tracker_data.index] = TA_ALERT_MODE_BOTH;
      }
      /* check if alert is raised for FRONT zone */
      else if (Fbk_Is_True(f_alert_front))
      {
         p_ta_persistent->ta_alert_mode[p_ta_object->tracker_data.index] = TA_ALERT_MODE_FRONT;
      }
      /* check if alert is raised for REAR zone */
      else if (Fbk_Is_True(f_alert_rear))
      {
         p_ta_persistent->ta_alert_mode[p_ta_object->tracker_data.index] = TA_ALERT_MODE_REAR;
      }
      /* object is in no zone */
      else
      {
         /* Objects outside of all zones shouldn't have an alert level. */
      }
   }
   else if ((p_ta_persistent->ta_side_index_prev_cycle[FBK_SIDE_LEFT] == p_ta_object->tracker_data.index)
            || (p_ta_persistent->ta_side_index_prev_cycle[FBK_SIDE_RIGHT] == p_ta_object->tracker_data.index))
   {
      /* Object has caused previous alert -> Do not reset alert mode to keep object hysteresis active */
   }
   else
   {
      p_ta_persistent->ta_alert_mode[p_ta_object->tracker_data.index] = TA_ALERT_MODE_NONE;
   }
}

static void Ta_Reset_Object_Persistent_Data(Ta_Persistent_T *p_ta_persistent, const uint8_t object_index)
{
   /* Asserts */
   assert(NULL != p_ta_persistent);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Reset persistent information */
   p_ta_persistent->ta_alert_mode[object_index] = TA_ALERT_MODE_NONE;
}

static void Ta_Fill_Side_Persistent_Data(Ta_Persistent_T *p_ta_persistent, const Ta_Core_Output_T *p_ta_core_output)
{
   /* Iterator */
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_core_output);

   /* Fill the persistent data for both vehicle sides */
   for (side_index = FBK_ZERO_INT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_ta_persistent->ta_side_alert_prev_cycle[side_index] = p_ta_core_output->ta_alert_level[side_index];
      p_ta_persistent->ta_side_id_prev_cycle[side_index]    = p_ta_core_output->ta_id[side_index];
      p_ta_persistent->ta_side_index_prev_cycle[side_index] = p_ta_core_output->ta_index[side_index];
   }
}

static void Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section(Ta_Persistent_T *p_ta_persistent,
                                                             const Pa_Data_T *p_pa_data,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                             const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_persistent);
   assert(NULL != p_pa_data);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ta_cal);

   /* Calculate ego yaw angle since last straight section */
   if (Fbk_Abs_F(p_vehicle_data->yawrate) >= p_ta_cal->k_ta_ego_yawangle_integration_yawrate_min)
   {
      /* Integrate ego yaw-angle changes per cycle */
      p_ta_persistent->ta_ego_yaw_angle_to_last_straight_section = p_ta_persistent->ta_ego_yaw_angle_to_last_straight_section
                                                                   + (p_vehicle_data->yawrate * p_pa_data->time_diff_to_last_cycle);
   }
   else
   {
      /* Reset ego yaw-angle for yaw-rate below threshold */
      p_ta_persistent->ta_ego_yaw_angle_to_last_straight_section = FBK_ZERO_F;
   }
}

static void Ta_Debounce_Alert_Level(Ta_Core_Output_T *p_ta_core_output,
                                    Ta_Persistent_T *p_ta_persistent,
                                    const Ta_Core_Calibration_T *p_ta_cal)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

   for (side_index = FBK_ZERO_INT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      /* Calculate the difference between the previous and the current alert level */
      int32_t alert_level_delta = ((int32_t) p_ta_core_output->ta_alert_level[side_index])
                                  - ((int32_t) p_ta_persistent->ta_side_alert_prev_cycle[side_index]);

      /* Qualifying alert */
      if ((TA_ALERT_STATE_LEVEL_1 == p_ta_core_output->ta_alert_level[side_index])
          || (TA_ALERT_STATE_LEVEL_2 == p_ta_core_output->ta_alert_level[side_index]))
      {
         /* Increase qualifying counter */
         Sat_Inc_Uint8(&(p_ta_persistent->ta_side_alert_qualifying_counter[side_index]));

         if (p_ta_persistent->ta_side_alert_qualifying_counter[side_index] <= p_ta_cal->k_ta_alert_qualifying_cycles)
         {
            /* Qualifying counter below threshold, thus suppress this alert */
            Ta_Reset_Core_Output(p_ta_core_output, side_index);
         }
      }
      else if (TA_ALERT_STATE_LEVEL_3 == p_ta_core_output->ta_alert_level[side_index])
      {
         /* TA_ALERT_STATE_LEVEL_3 do not require qualification or other actions */
      }
      else if (TA_ALERT_STATE_LEVEL_4 == p_ta_core_output->ta_alert_level[side_index])
      {
         /* Optionally check if alert levels are consecutive */
         if (Fbk_Is_True(p_ta_cal->k_ta_f_only_allow_consecutive_ttc_based_alert_levels))
         {
            if (alert_level_delta > 1)
            {
               /* Alert levels not consecutive, thus suppress this alert */
               Ta_Reset_Core_Output(p_ta_core_output, side_index);
            }
         }
      }
      else
      {
         /* No active alert in this cycle, thus reset qualification counter */
         p_ta_persistent->ta_side_alert_qualifying_counter[side_index] = FBK_ZERO_INT;
      }

      /* Holding alert */
      if ((p_ta_core_output->ta_alert_level[side_index] < p_ta_persistent->ta_side_alert_prev_cycle[side_index])
          && ((p_ta_persistent->ta_side_alert_prev_cycle[side_index] < TA_ALERT_STATE_LEVEL_4) || (alert_level_delta < -1)
              || (Fbk_Is_False(p_ta_cal->k_ta_f_skip_holding_for_single_alert_level_drop))))
      {
         /* Increase holding counter */
         Sat_Inc_Uint8(&(p_ta_persistent->ta_side_alert_holding_counter[side_index]));

         if (p_ta_persistent->ta_side_alert_holding_counter[side_index] <= p_ta_cal->k_ta_alert_holding_cycles)
         {
            /* Holding counter below threshold, thus hold TA alert */

            /* Reset TA core output if object ID changed and overwrite with previous object ID */
            if (p_ta_core_output->ta_id[side_index] != p_ta_persistent->ta_side_id_prev_cycle[side_index])
            {
               Ta_Reset_Core_Output(p_ta_core_output, side_index);
               p_ta_core_output->ta_id[side_index]    = p_ta_persistent->ta_side_id_prev_cycle[side_index];
               p_ta_core_output->ta_index[side_index] = p_ta_persistent->ta_side_index_prev_cycle[side_index];
            }

            /* Overwrite TA core output alert level with previous alert level */
            p_ta_core_output->ta_alert_level[side_index] = p_ta_persistent->ta_side_alert_prev_cycle[side_index];
         }
      }
      else
      {
         /* No alert is held in this cycle, thus reset holding counter */
         p_ta_persistent->ta_side_alert_holding_counter[side_index] = FBK_ZERO_INT;
      }
   }
}

static void Ta_Algorithm(Ta_Core_Output_T *p_ta_core_output,
                         Ta_Persistent_T *p_ta_persistent,
                         const Ta_Core_Input_T *p_ta_core_input,
                         const Ta_Core_Calibration_T *p_ta_cal,
                         Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance)
{
   /* Get context pointer */
   const Pa_Data_T *p_pa_data = p_ta_core_input->p_pa_data;

   /* Iterator variables */
   uint8_t object_index;
   uint8_t side_index;

   boolean_T f_vehicle_state_relevant = FBK_FALSE;
   uint8_t n_valid_objects            = FBK_ZERO_UINT;
   uint8_t n_relevant_objects         = FBK_ZERO_UINT;
   uint8_t n_critical_objects         = FBK_ZERO_UINT;

   /* Ego trajectory and vehicle data */
   Fbk_Vehicle_Data_T vehicle_data;
   Fbk_Trajectory_T ego_trajectory;
   Fbk_Ego_Predict_Data_T fbk_ego_data;
   Fbk_Object_Predict_Data_T fbk_obj_data;

   /* Asserts */
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_core_input);
   assert(NULL != p_ta_cal);
   assert(NULL != p_ego_traj_predictor_instance);

   /* Fill vehicle data */
   vehicle_data = p_pa_data->vehicle_data;

   /* Reset ego trajectory */
   Ta_Reset_Trajectory(&ego_trajectory, p_ta_cal);

   /* Reset ta core output */
   for (side_index = FBK_ZERO_INT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      Ta_Reset_Core_Output(p_ta_core_output, side_index);
   }

   /* Update ego yaw angle to last known straight section */
   Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section(p_ta_persistent, p_pa_data, &vehicle_data, p_ta_cal);

   /* Iterate through tracker objects */
   for (object_index = FBK_ZERO_INT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      /* TA Object struct and pointer */
      Ta_Object_T ta_object;
      Ta_Object_T *p_ta_object = &ta_object;

      /* Reset data structs */
      Ta_Reset_Object(p_ta_object, p_ta_cal);

      /* Tracker object validity check */
      if (Ta_Is_Object_Valid(p_pa_data, object_index))
      {
         /* Increase valid object counter */
         Sat_Inc_Uint8(&n_valid_objects);

         /* Object is valid -> Fill tracker and persistent information for ta_object */
         Ta_Fill_Object_Information(p_ta_object, p_ta_core_input, p_ta_persistent, p_ta_cal, object_index);

         /* Update Ta objects relevance flags by checking the objects general properties and zone overlaps */
         Ta_Update_Object_Relevance(p_ta_object, p_ta_core_input, &vehicle_data, p_ta_cal);

         /* Set flag for overall vehicle state relevance */
         if (Fbk_Is_True(p_ta_object->attributes.f_vehicle_state_relevant))
         {
            f_vehicle_state_relevant = FBK_TRUE;
         }

         /* Check if object is relevant for TA */
         if ((PA_INVALID_OBJ_ID != p_ta_object->tracker_data.id) && Fbk_Is_True(p_ta_object->attributes.f_obj_ta_relevant))
         {
            /* Increase relevant object counter */
            Sat_Inc_Uint8(&n_relevant_objects);

            /* Calculate object euclidean distance to VCS origin */
            Ta_Get_Object_Distance_to_Vcs_Origin(p_ta_object);

            /* Check if object is inside the danger or wing zone (while turning) */
            if (Fbk_Is_True(p_ta_object->attributes.f_obj_in_danger_zone)
                || (Fbk_Is_True(p_ta_object->attributes.f_obj_in_wing_zone)
                    && (Fbk_Abs_F(vehicle_data.curvature) > p_ta_cal->k_ta_straight_host_curvature_max)))
            {
               /* Predict ego trajectory once. */
               if (Fbk_Is_False(ego_trajectory.f_trajectory_valid))
               {
                  /* Fill FBK ego data struct and pointer */
                  Ta_Fill_Fbk_Predict_Ego_Struct(&fbk_ego_data, p_ta_persistent, p_ta_cal);

                  /* Predict ego trajectory */
                  Fbk_Predict_Ego_Trajectory(p_ego_traj_predictor_instance, &ego_trajectory, &vehicle_data, &fbk_ego_data);
               }

               /* Fill Fbk object data struct */
               Ta_Fill_Fbk_Predict_Obj_Struct(&fbk_obj_data, p_ta_persistent, p_ta_cal);

               /* Predict object trajectory */
               Fbk_Predict_Obj_Trajectory(&ta_object.attributes.trajectory, &ta_object.tracker_data, &fbk_obj_data);

               /* Calculate time-to-collision (TTC) based on trajectories */
               Ta_Get_Object_Ttc(p_ta_object, &ego_trajectory, p_ta_persistent, p_ta_cal);

               /* Check if object has a valid TTC value */
               if (TA_INVALID_TTC > p_ta_object->attributes.ttc)
               {
                  /* Calculate deceleration estimate */
                  Ta_Get_Ego_Deceleration_To_Avoid_Collision(p_ta_object, &ego_trajectory, p_ta_persistent, p_ta_cal);

                  /* Calculate time-to-brake (TTB) */
                  Ta_Get_Object_Ttb(p_ta_object, &ego_trajectory, p_ta_persistent, p_ta_cal);
               }
            }

            /* Check if object is inside the info zone */
            if (Fbk_Is_True(p_ta_object->attributes.f_obj_in_info_zone))
            {
               /* Calculate time-to-pass (TTP) */
               Ta_Get_Object_Ttp(p_ta_object, vehicle_data.curvature, p_ta_cal);
            }

            /* Set object criticality based on object properties */
            Ta_Set_Object_Criticality(p_ta_object, p_ta_core_input, p_ta_persistent, p_ta_cal);

            if (TA_ALERT_STATE_NONE != p_ta_object->attributes.alert_level)
            {
               /* Increase critical object counter */
               Sat_Inc_Uint8(&n_critical_objects);
            }

            /* Check if current object is the most critical object on its side */
            Ta_Set_Most_Critical_Object_Per_Side(p_ta_core_output, p_ta_object, p_ta_cal);
         }

         /* Fill object persistent data for TA object */
         Ta_Fill_Object_Persistent_Data(p_ta_persistent, p_ta_object, p_ta_cal);
      }
      else
      {
         /* Object is invalid -> Reset persistent data */
         Ta_Reset_Object_Persistent_Data(p_ta_persistent, object_index);
      }

      /* Pass object data to debug data structure */
      Binary_Ta_Debug_Pass_Object_Attributes(&p_ta_object->attributes, object_index);
   }

   /* Debounce output signals */
   Ta_Debounce_Alert_Level(p_ta_core_output, p_ta_persistent, p_ta_cal);

   /* Get most critical side overall */
   Ta_Set_Most_Critical_Side(p_ta_core_output);

   /* Fill side specific persistent data */
   Ta_Fill_Side_Persistent_Data(p_ta_persistent, p_ta_core_output);

   /* Set counter values and vehicle state flag in core output */
   p_ta_core_output->ta_f_vehicle_state_relevant = f_vehicle_state_relevant;
   p_ta_core_output->ta_n_valid_objects          = n_valid_objects;
   p_ta_core_output->ta_n_relevant_objects       = n_relevant_objects;
   p_ta_core_output->ta_n_critical_objects       = n_critical_objects;

   /* Update TA state */
   Ta_Update_Algorithm_State(p_ta_core_output);

   /* Pass ego trajectory data. */
   Binary_Ta_Debug_Pass_Ego_Data(&ego_trajectory);
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Core_Run(Ta_Core_Output_T *p_ta_core_output,
                 const Ta_Core_Input_T *p_ta_core_input,
                 const Ta_Core_Calibration_T *p_ta_cal,
                 Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance,
                 Ta_Persistent_T *p_ta_persistent)
{
   /* Asserts */
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_core_input);
   assert(NULL != p_ta_cal);

   /* Reset debug data */
   Binary_Ta_Debug_Reset_Data();

   if (Fbk_Is_True(p_ta_core_input->f_ta_enable))
   {
      Ta_Algorithm(p_ta_core_output, p_ta_persistent, p_ta_core_input, p_ta_cal, p_ego_traj_predictor_instance);
   }
   else
   {
      Ta_Reset(p_ta_core_output, p_ta_cal, p_ta_persistent);
   }

   /* Pass general data to debug data */
   Binary_Ta_Debug_Set_Side_Specific_Object_Trajectories(p_ta_core_input->p_pa_data, p_ta_core_output);
   Binary_Ta_Debug_Pass_General_Data(p_ta_core_input, p_ta_core_output, p_ta_persistent, p_ta_cal);
}

void Ta_Reset(Ta_Core_Output_T *p_ta_core_output, const Ta_Core_Calibration_T *p_ta_cal, Ta_Persistent_T *p_ta_persistent)
{
   /* Asserts */
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_cal);

   /* Reset core output for both sides */
   Ta_Reset_Core_Output(p_ta_core_output, FBK_SIDE_LEFT);
   Ta_Reset_Core_Output(p_ta_core_output, FBK_SIDE_RIGHT);

   /* Reset counters */
   p_ta_core_output->ta_n_valid_objects    = FBK_ZERO_UINT;
   p_ta_core_output->ta_n_relevant_objects = FBK_ZERO_UINT;
   p_ta_core_output->ta_n_critical_objects = FBK_ZERO_UINT;

   /* Reset flags */
   p_ta_core_output->ta_f_vehicle_state_relevant = FBK_FALSE;

   /* Reset algorithm state */
   p_ta_core_output->ta_algorithm_state = TA_STATE_ALGORITHM_DISABLED;

   /* Reset persistent data */
   Ta_Reset_Persistent(p_ta_persistent, p_ta_cal);
}
