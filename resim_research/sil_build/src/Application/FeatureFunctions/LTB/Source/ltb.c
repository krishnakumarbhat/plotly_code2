/**
 * @file ltb.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Core LTB algorithm.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ltb.h"
#include "fbk_ego_traj_predictor.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_obj_traj_predictor.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_traj_predictor_t.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_common_functions.h"
#include "ltb_debug_interface.h"
#include "ltb_factory.h"
#include "ltb_persistent_t.h"
#include "ltb_time_calculation.h"
#include "ltb_types.h"
#include "ltb_warn_logic.h"
#include "ml_polygon.h"
#include "ml_vector_2d.h"
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
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief This function contains the main LTB algorithm.
 *
 * The main LTB algorithm checks all incoming tracker objects and filters out the objects that are relevant for the LTB function.
 * In following steps those relevant objects paths are predicted for future movement and an alert level is set based on object's
 * data that might collide with host car, when driver entering longitudinal traffic from confusing situations
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53884}
 * @verification{Create tests where alerts on each side are triggered.}
 */
static void Ltb_Algorithm(Ltb_Core_Output_T *p_ltb_core_output /**< LTB core output data */,
                          Ltb_Persistent_T *p_ltb_persistent /**< LTB persistent data */,
                          const Ltb_Core_Input_T *p_ltb_core_input /**< LTB core input data */,
                          const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB calibration */,
                          Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance);

/**
 * @brief Resets properties of LTB object.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53886}
 * @verification{Create a test which checks whether the ltb object data is reset correctly to its default.}
 */
static void Ltb_Reset_Ltb_Object(Ltb_Object_T *p_ltb_object /**< LTB object */,
                                 const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB Calibration */);

/**
 * @brief Resets all persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53889}
 * @verification{Create a test which checks whether the ltb persistent data is reset correctly to its default.}
 */
static void Ltb_Reset_Persistent_Data(Ltb_Persistent_T *p_ltb_persistent /**< LTB persistent data */,
                                      const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB Calibration */);

/**
 * @brief Checks basic validity of given object.
 *
 * @return object validity state
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53887}
 * @verification{Create an object which is not classified as reflection with a non default id, a coasted or mature status. Only in
 * those cases true shall be expected.}
 */
static boolean_T Ltb_Is_Object_Valid(const Fbk_Object_Data_T *p_object_data /**< object data */);

/**
 * @brief Updates the relevance flags for the given object.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53888}
 * @verification{Create an object which is relevant for LTB and also placed within the zone.}
 */
static void Ltb_Update_Object_Relevance(Ltb_Object_T *p_ltb_object /**< LTB Object */,
                                        const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB Calibration */);

/**
 * @brief Fills the FBK data struct about host parameters, using calibraiton and persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53892}
 * @verification{Check that the object information is updated correctly.}
 */
static void Ltb_Fill_Fbk_Predict_Ego_Struct(Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data*/,
                                            const Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent */,
                                            const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB Calibration */);
/**
 * @brief Fills the FBK data struct using calibraiton and persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53893}
 * @verification{Check that the object information is updated correctly.}
 */
static void Ltb_Fill_Fbk_Predict_Obj_Struct(Fbk_Object_Predict_Data_T *p_fbk_obj_data /**< Object data*/,
                                            const Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent */,
                                            const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB Calibration */);


/**
 * @brief Fill the persistent side data using the information of the core output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53890}
 * @verification{Create a test to check whether ltb side persistent data is set correctly.}
 */
static void Ltb_Fill_Persistent_Data(Ltb_Persistent_T *p_ltb_persistent /**< LTB persistent data */,
                                     const Ltb_Core_Output_T *p_ltb_core_output /**< LTB core output */);

/**
 * @brief Updates the ego yaw angle
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53891}
 * @verification{Check that the ego yaw angle is set correctly.}
 */
static void Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section(Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent */,
                                                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                              const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB Calibration */,
                                                              const float32_T time_diff_to_last_cycle /** Time to last cycle*/);


/*===========================================================================*\
 * Global Functions	Definition
 \*===========================================================================*/

void Ltb_Core_Run(Ltb_Core_Output_T *p_ltb_core_output,
                  const Ltb_Core_Input_T *p_ltb_core_input,
                  const Ltb_Core_Calibration_T *p_ltb_cals,
                  Ltb_Persistent_T *p_ltb_persistent,
                  Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance)
{

   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_core_input);
   assert(NULL != p_ltb_cals);
   assert(NULL != p_ltb_persistent);

   /* Reset debug data */
   Binary_Ltb_Debug_Reset_Data();

   /* Check if LTB algorithm should be called this cycle */
   if (Fbk_Is_False(p_ltb_core_input->f_ltb_enable))
   {
      Ltb_Reset(p_ltb_core_output, p_ltb_cals, p_ltb_persistent);
   }
   else
   {
      Ltb_Algorithm(p_ltb_core_output, p_ltb_persistent, p_ltb_core_input, p_ltb_cals, p_ego_traj_predictor_instance);
   }

   /* Pass general data to debug data */
   Binary_Ltb_Debug_Set_Side_Specific_Object_Trajectories(p_ltb_core_input->p_pa_data, p_ltb_core_output);
   Binary_Ltb_Debug_Pass_General_Data(p_ltb_core_input, p_ltb_core_output, p_ltb_persistent, p_ltb_cals);
}

void Ltb_Reset(Ltb_Core_Output_T *p_ltb_core_output, const Ltb_Core_Calibration_T *p_ltb_cals, Ltb_Persistent_T *p_ltb_persistent)
{
   uint8_t side_index;

   /* Assert */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_cals);
   assert(NULL != p_ltb_persistent);

   /* Reset persistent data */
   Ltb_Reset_Persistent_Data(p_ltb_persistent, p_ltb_cals);

   /* Reset core output for both sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      Ltb_Reset_Core_Output_Side_Data(p_ltb_core_output, side_index);
   }
}


/*===========================================================================*\
* Local Functions Definition
\*===========================================================================*/

static void Ltb_Reset_Ltb_Object(Ltb_Object_T *p_ltb_object, const Ltb_Core_Calibration_T *p_ltb_cals)
{
   /* Assert */
   assert(NULL != p_ltb_object);

   /* Reset tracker object data. */
   Fbk_Reset_Object_Data(&(p_ltb_object->tracker_data));

   /* Resets the trajectory properties */
   Ltb_Reset_Trajectory(&p_ltb_object->attributes.trajectory, p_ltb_cals);

   /* Reset all ltb object properties */
   p_ltb_object->attributes.f_curvi_available = FBK_FALSE;

   /* LTB specific flags*/
   p_ltb_object->attributes.f_vehicle_state_relevant = FBK_FALSE;
   p_ltb_object->attributes.f_obj_ltb_relevant       = FBK_FALSE;
   p_ltb_object->attributes.f_obj_in_zone            = FBK_FALSE;

   /* Object criticality */
   p_ltb_object->attributes.waypoint_at_collision = Create_2d_Vector_Origin();
   p_ltb_object->attributes.ttc                   = LTB_INVALID_TTC;
   p_ltb_object->attributes.distance_to_ego       = LTB_INVALID_DISTANCE;
   p_ltb_object->attributes.alert_level           = NO_ALERT;
   p_ltb_object->attributes.alert_side            = FBK_SIDE_UNDEFINED;

   /* Brake related */
   p_ltb_object->attributes.decel_to_avoid_coll = FBK_ZERO_F;
   p_ltb_object->attributes.ttb                 = LTB_INVALID_TTB;
}


static boolean_T Ltb_Is_Object_Valid(const Fbk_Object_Data_T *p_object_data)
{
   boolean_T f_object_valid = FBK_FALSE;
   Pa_Obj_Status_T obj_state;

   /* Assert */
   assert(NULL != p_object_data);

   obj_state = p_object_data->status;
   if ((p_object_data->id > FBK_ZERO_UINT) && (Fbk_Is_Obj_State_Valid(obj_state)))
   {
      f_object_valid = FBK_TRUE;
   }

   return f_object_valid;
}

static void Ltb_Update_Object_Relevance(Ltb_Object_T *p_ltb_object, const Ltb_Core_Calibration_T *p_ltb_cals)
{
   /* LTB zones and related variables */
   Fbk_Field_Of_Interest_T ltb_zone_left;
   Fbk_Field_Of_Interest_T ltb_zone_right;
   Vector_2d_T point_to_check;


   /* Asserts */
   assert(NULL != p_ltb_object);
   assert(NULL != p_ltb_cals);

   /* Check if the host vehicle is in a suitable state for LTB, check normal thresholds etc.. */
   /* Update vehicle state flag */

   /* Create LTB zones once per cycle */
   Ltb_Create_Zone(&ltb_zone_left, &ltb_zone_right, p_ltb_cals);

   /* Check which object position to use */
   if (Fbk_Is_True(p_ltb_object->attributes.f_curvi_available))
   {
      point_to_check = p_ltb_object->tracker_data.curvi_pos;
   }
   else
   {
      point_to_check = p_ltb_object->tracker_data.vcs_pos;
   }

   /* Check if point_to_check is in field of interest and object fulfils the general principles */
   if ((Is_Point_In_Convex_Polygon_Ray_Casting_Method(ltb_zone_left.points, ltb_zone_left.size, &point_to_check)
        || Is_Point_In_Convex_Polygon_Ray_Casting_Method(ltb_zone_right.points, ltb_zone_right.size, &point_to_check))
       && (p_ltb_object->tracker_data.vcs_vel.x > p_ltb_cals->k_ltb_object_long_vel_min))
   {
      p_ltb_object->attributes.f_obj_in_zone      = FBK_TRUE;
      p_ltb_object->attributes.f_obj_ltb_relevant = FBK_TRUE;
   }
}

static void Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section(Ltb_Persistent_T *p_ltb_persistent,
                                                              const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                              const Ltb_Core_Calibration_T *p_ltb_cals,
                                                              const float32_T time_diff_to_last_cycle)
{
   /* Asserts */
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ltb_cals);

   /* Calculate ego yaw angle since last straight section */
   if (Fbk_Abs_F(p_vehicle_data->yawrate) >= p_ltb_cals->k_ltb_ego_yawangle_integration_yawrate_min)
   {
      /* Integrate ego yaw-angle changes per cycle */
      p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section =
         p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section + (p_vehicle_data->yawrate * time_diff_to_last_cycle);
   }
   else
   {
      /* Reset ego yaw-angle for yaw-rate below threshold */
      p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section = FBK_ZERO_F;
   }
}


static void Ltb_Fill_Persistent_Data(Ltb_Persistent_T *p_ltb_persistent, const Ltb_Core_Output_T *p_ltb_core_output)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_core_output);

   /* Fill persistent data for both sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_ltb_persistent->ltb_side_alert_prev_cycle[side_index] = p_ltb_core_output->ltb_alert_level[side_index];
      p_ltb_persistent->ltb_side_id_prev_cycle[side_index]    = p_ltb_core_output->ltb_id[side_index];
   }
}

static void Ltb_Fill_Fbk_Predict_Ego_Struct(Fbk_Ego_Predict_Data_T *p_fbk_ego_data,
                                            const Ltb_Persistent_T *p_ltb_persistent,
                                            const Ltb_Core_Calibration_T *p_ltb_cals)
{
   /* Assert */
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cals);

   /* Fill ego properties */
   p_fbk_ego_data->ego_acceleration_weight                = p_ltb_cals->k_ltb_ego_acceleration_weight;
   p_fbk_ego_data->ego_shape_gain_fixed                   = p_ltb_cals->k_ltb_ego_shape_gain_fixed;
   p_fbk_ego_data->ego_circle_offset                      = p_ltb_cals->k_ltb_ego_circle_offset;
   p_fbk_ego_data->ego_circle_host_length_factor          = p_ltb_cals->k_ltb_ego_circle_host_length_factor;
   p_fbk_ego_data->ego_pred_const_velocity_pred_steps_min = p_ltb_cals->k_ltb_ego_pred_const_velocity_pred_steps_min;
   p_fbk_ego_data->ego_deceleration_weight                = p_ltb_cals->k_ltb_ego_deceleration_weight;
   p_fbk_ego_data->ego_yaw_angle_to_last_straight_section = p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section;
   p_fbk_ego_data->ego_max_pred_yaw_angle                 = p_ltb_cals->k_ltb_ego_max_pred_yaw_angle;
   p_fbk_ego_data->ego_shape_gain_per_pred_step           = p_ltb_cals->k_ltb_ego_shape_gain_per_pred_step;
   p_fbk_ego_data->pred_step_dt                           = p_ltb_persistent->ltb_pred_step_dt;
   p_fbk_ego_data->prediction_steps_max                   = p_ltb_cals->k_ltb_prediction_steps_max;

   p_fbk_ego_data->acc_weight_depend_on_alert_lvl = FBK_FALSE;

   if ((ALERT_ACTIVE_LEVEL_3 == p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_LEFT])
       || (ALERT_ACTIVE_LEVEL_3 == p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT]))
   {
      /* Host vehicle is presumably decelerating due to LTB brake request.
       * Predict using constant velocity model after applicable number of prediction steps. */
      p_fbk_ego_data->acc_weight_depend_on_alert_lvl = FBK_TRUE;
   }
}

static void Ltb_Fill_Fbk_Predict_Obj_Struct(Fbk_Object_Predict_Data_T *p_fbk_obj_data,
                                            const Ltb_Persistent_T *p_ltb_persistent,
                                            const Ltb_Core_Calibration_T *p_ltb_cals)
{
   /* Fill object properties */
   p_fbk_obj_data->obj_pred_speed_min           = p_ltb_cals->k_ltb_obj_pred_speed_min;
   p_fbk_obj_data->obj_shape_gain_fixed         = p_ltb_cals->k_ltb_obj_shape_gain_fixed;
   p_fbk_obj_data->obj_shape_gain_per_pred_step = p_ltb_cals->k_ltb_obj_shape_gain_per_pred_step;
   p_fbk_obj_data->pred_step_dt                 = p_ltb_persistent->ltb_pred_step_dt;
}

static void Ltb_Algorithm(Ltb_Core_Output_T *p_ltb_core_output,
                          Ltb_Persistent_T *p_ltb_persistent,
                          const Ltb_Core_Input_T *p_ltb_core_input,
                          const Ltb_Core_Calibration_T *p_ltb_cals,
                          Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance)
{
   /* Get data pointer */
   const Pa_Data_T *p_pa_data = p_ltb_core_input->p_pa_data;
   /* Index variables */
   uint8_t object_index;
   uint8_t side_index;

   /* Ego trajectory and vehicle data */
   Fbk_Vehicle_Data_T vehicle_data;
   Fbk_Trajectory_T ego_trajectory;
   Fbk_Ego_Predict_Data_T fbk_ego_data;
   Fbk_Object_Predict_Data_T fbk_obj_data;

   /* Asserts */
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_core_input);
   assert(NULL != p_ltb_cals);

   /* Reset ego trajectory */
   Ltb_Reset_Trajectory(&ego_trajectory, p_ltb_cals);

   /* Reset core output per side */
   for (side_index = FBK_ZERO_INT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      Ltb_Reset_Core_Output_Side_Data(p_ltb_core_output, side_index);
   }

   /* Fill vehicle data */
   vehicle_data = p_ltb_core_input->p_pa_data->vehicle_data;

   /* Update ego yaw angle to last known straight section */
   Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section(p_ltb_persistent, &vehicle_data, p_ltb_cals,
                                                     p_ltb_core_input->p_pa_data->time_diff_to_last_cycle);

   for (object_index = FBK_ZERO_INT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      /* LTB object struct and pointer */
      Ltb_Object_T ltb_object;
      Ltb_Object_T *p_ltb_object = &ltb_object;

      /* Reset data structs */
      Ltb_Reset_Ltb_Object(p_ltb_object, p_ltb_cals);

      /* Check basic validity signals */
      if (Ltb_Is_Object_Valid(&p_ltb_core_input->p_pa_data->object_data[object_index]))
      {
         /* Object is valid -> Fill tracker information for LTB_Object */
         p_ltb_object->tracker_data = p_pa_data->object_data[object_index];

         /* Check if curvi signals are available */
         if (PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL == p_ltb_object->tracker_data.curvi_coordinates_calc_method)
         {
            p_ltb_object->attributes.f_curvi_available = FBK_TRUE;
         }

         /* Update Ltb objects relevance flags by checking the objects general properties and zone overlaps */
         Ltb_Update_Object_Relevance(p_ltb_object, p_ltb_cals);

         /* Check if object is relevant for LTB */
         if ((PA_INVALID_OBJ_ID != p_ltb_object->tracker_data.id) && Fbk_Is_True(p_ltb_object->attributes.f_obj_ltb_relevant))
         {
            /* Calculate euclidean distance from object position to VCS origin */
            p_ltb_object->attributes.distance_to_ego = Vector_2d_Alg_Abs(&(p_ltb_object->tracker_data.vcs_pos));

            /* Fill FBK ego data struct and pointer */
            Ltb_Fill_Fbk_Predict_Ego_Struct(&fbk_ego_data, p_ltb_persistent, p_ltb_cals);

            /* Predict ego trajectory */
            Fbk_Predict_Ego_Trajectory(p_ego_traj_predictor_instance, &ego_trajectory, &vehicle_data, &fbk_ego_data);

            /* Fill Fbk object data struct */
            Ltb_Fill_Fbk_Predict_Obj_Struct(&fbk_obj_data, p_ltb_persistent, p_ltb_cals);

            /* Predict object trajectory */
            Fbk_Predict_Obj_Trajectory(&ltb_object.attributes.trajectory, &ltb_object.tracker_data, &fbk_obj_data);

            /* Calculate time-to-collision (TTC) based on trajectories */
            Ltb_Get_Object_Ttc(p_ltb_object, &ego_trajectory, p_ltb_persistent, p_ltb_cals);

            /* Check if object has a valid TTC value */
            if (LTB_INVALID_TTC > p_ltb_object->attributes.ttc)
            {
               /* Calculate deceleration estimate */
               Ltb_Get_Ego_Deceleration_To_Avoid_Collision(p_ltb_object, &ego_trajectory, p_ltb_persistent, p_ltb_cals);

               /* Calculate time-to-brake (TTB) */
               Ltb_Get_Object_Ttb(p_ltb_object, &ego_trajectory, p_ltb_persistent, p_ltb_cals);
            }

            /* Set object criticality based on object properties */
            Ltb_Set_Object_Criticality(p_ltb_object, p_ltb_cals);

            /* Check if current object is the most critical object on its side */
            Ltb_Set_Most_Critical_Object_Per_Side(p_ltb_core_output, p_ltb_object);
         }
      }

      /* Pass object data to debug data structure */
      Binary_Ltb_Debug_Pass_Object_Attributes(&(p_ltb_object->attributes), object_index);
   }

   /* Debounce output signals */
   Ltb_Debounce_Alert_Level(p_ltb_core_output, p_ltb_persistent, p_ltb_cals);

   /* Get most critical side overall */
   Ltb_Set_Most_Critical_Side(p_ltb_core_output);

   /* Fill side persistent data */
   Ltb_Fill_Persistent_Data(p_ltb_persistent, p_ltb_core_output);

   /* Pass ego trajectory data. */
   Binary_Ltb_Debug_Pass_Ego_Data(&ego_trajectory);
}


static void Ltb_Reset_Persistent_Data(Ltb_Persistent_T *p_ltb_persistent, const Ltb_Core_Calibration_T *p_ltb_cals)
{
   uint8_t side_index;

   Ltb_Init_Prediction_Time_Step(p_ltb_persistent, p_ltb_cals);

   /* Assert */
   assert(NULL != p_ltb_persistent);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_ltb_persistent->ltb_side_alert_prev_cycle[side_index]         = NO_ALERT;
      p_ltb_persistent->ltb_side_id_prev_cycle[side_index]            = PA_INVALID_OBJ_ID;
      p_ltb_persistent->ltb_side_alert_qualifying_counter[side_index] = FBK_ZERO_UINT;
      p_ltb_persistent->ltb_side_alert_holding_counter[side_index]    = FBK_ZERO_UINT;
   }
}
