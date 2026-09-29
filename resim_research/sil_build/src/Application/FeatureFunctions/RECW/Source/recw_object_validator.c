/**
 * @file recw_object_validator.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Sub module of Recw for the validity check of objects.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw_object_validator.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_shared_types.h"
#include "recw_car_wash_detection.h"
#include <assert.h>

/*===========================================================================*\
 * Typedefs
\*===========================================================================*/

/**
 * This struct summarizes the criteria which an object needs to fulfill in order to be a valid
 * Recw candidate.
 */
typedef struct
{
   Float_Range_T long_rel_vel_range;
   float32_T max_approach_angle;
   float32_T min_existence_probability;
   float32_T lane_filter_width;

} Recw_Obj_Valid_Crit_T;

/*===========================================================================*\
 * Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Checks whether relative longitudinal velocity is within a specified range
 *
 * @return true when objects longitudinal speed is withing the given float range
 *
 * @SRS{SF-1701}
 * @SAE{SF-2959}
 * @SDD{SF-7919}
 * @verification{}
 */
static boolean_T
Recw_Is_Long_Rel_Vel_In_Valid_Range(const Recw_Obj_Valid_Crit_T *p_recw_valid_criteria /**< Recw object validation criteria*/,
                                    const Recw_Object_T *p_recw_object /**<Recw object*/,
                                    const Recw_Core_Calibration_T *p_cals /**<Recw calibrations*/);

/**
 * @brief Checks whether the object is in the ego lane
 *
 * @return true when object is driving in ego lane
 *
 * @SRS{SF-1690,SF-1691,SF-1704}
 * @SAE{SF-2959}
 * @SDD{SF-7921}
 * @verification{}
 */
static boolean_T
Recw_Is_Object_In_Ego_Lane(Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                           const Recw_Obj_Valid_Crit_T *p_recw_valid_criteria /**< RECW object validation criteria */,
                           const Recw_Object_T *p_recw_object /**< RECW object data */,
                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                           const Recw_Core_Calibration_T *p_cals /**< RECW calibration data */);

/**
 * @brief Used to exclude slow, young and near objects. This is nessesary for situations at a traffic light.
 *
 * @return True when object is classified as traffic light ghost
 *
 * @SRS{SF-1693,SF-1714}
 * @SAE{SF-2959}
 * @SDD{SF-7922}
 * @verification{}
 */
static boolean_T Recw_Is_Traffic_Light_Ghost(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                             const Recw_Object_T *p_recw_object /**< RECW object */,
                                             const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */);

/**
 * @brief Checks whether the object heading is critical for recw object candidates
 *
 * @return True when objects heading is relevant for Recw
 *
 * @SRS{SF-1702}
 * @SAE{SF-2959}
 * @SDD{SF-7920}
 * @verification{}
 */
static boolean_T
Recw_Is_Obj_Heading_Relevant(const Recw_Obj_Valid_Crit_T *p_recw_valid_criteria /**< Recw object validation criteria*/,
                             const Recw_Object_T *p_recw_object /**<Recw object*/,
                             const Recw_Core_Calibration_T *p_cals /**<Recw calibrations*/);

/**
 * @brief Checks whether the object is located behind the ego and therefore relevant for RECW
 *
 * @return True if object is located behind the ego, false otherwise
 *
 * @SRS{SF-1695,SF-1700,SF-1716}
 * @SAE{SF-2959}
 * @SDD{SF-7918}
 * @verification{}
 */
static boolean_T Recw_Is_Obj_Behind_Ego(const Recw_Object_T *p_recw_object /**<Recw object*/);

/**
 * @brief Checks whether coasted cycle amount is exceeding the given threshold
 *
 * @return True in case that coasted cycles reached
 *
 * @SRS{SF-1715}
 * @SAE{SF-2959}
 * @SDD{SF-7916}
 * @verification{}
 */
static boolean_T Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached(const Recw_Object_T *p_recw_object /**<Recw object*/,
                                                               const Recw_Core_Calibration_T *p_cals /**<Recw calibrations*/);

/**
 * @brief Fills recw object validation criteria
 *
 * @return void
 *
 * @SRS{SF-1695,SF-1700,SF-1716}
 * @SAE{SF-2959}
 * @SDD{SF-7917}
 * @verification{}
 */
static void Recw_Fill_Object_Valid_Crit(Recw_Obj_Valid_Crit_T *p_recw_valid_criteria /**< Recw object validation criteria*/,
                                        const Recw_Persistent_T *p_persistent /**< persistent recw data*/,
                                        const Recw_Object_T *p_recw_object /**<Recw object*/,
                                        const Recw_Core_Calibration_T *p_cals /**<Recw calibrations*/);
/*===========================================================================*\
* Global Function Defintions
\*===========================================================================*/

boolean_T Recw_Is_Object_Relevant(Recw_Object_T *p_recw_object,
                                  Recw_Persistent_T *p_persistent,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data,
                                  const Recw_Core_Calibration_T *p_cals,
                                  Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags)
{
   boolean_T f_object_is_relevant = FBK_FALSE;
   Recw_Obj_Valid_Crit_T recw_valid_criteria;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_persistent);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   if (Recw_Is_Obj_Behind_Ego(p_recw_object))
   {
      Recw_Fill_Object_Valid_Crit(&recw_valid_criteria, p_persistent, p_recw_object, p_cals);

      p_recw_object->attributes.f_object_is_car_wash_ghost =
         Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, p_cals, p_recw_object, p_car_wash_scenario_flags);
      p_recw_object->attributes.f_obj_is_within_lane =
         Recw_Is_Object_In_Ego_Lane(p_persistent, &recw_valid_criteria, p_recw_object, p_vehicle_data, p_cals);

      if ((Fbk_Is_False(p_cals->k_recw_f_apply_lane_filter) || Fbk_Is_True(p_recw_object->attributes.f_obj_is_within_lane))
          && Fbk_Is_False(p_recw_object->attributes.f_object_is_car_wash_ghost)
          && Fbk_Is_False(Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached(p_recw_object, p_cals))
          && Fbk_Is_False(Recw_Is_Traffic_Light_Ghost(p_vehicle_data, p_recw_object, p_cals))
          && (Recw_Is_Long_Rel_Vel_In_Valid_Range(&recw_valid_criteria, p_recw_object, p_cals))
          && (Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, p_recw_object, p_cals))
          && (p_recw_object->tracker_data.existence_probability >= recw_valid_criteria.min_existence_probability)
          && (p_recw_object->tracker_data.speed >= p_cals->k_recw_min_speed_not_stationary)
          && (p_persistent->object_data[p_recw_object->tracker_data.id].age >= p_cals->k_recw_min_object_age)
          && (p_recw_object->tracker_data.width <= p_cals->k_recw_max_object_width_warn_on)
          && (p_recw_object->tracker_data.eclipse_value <= p_cals->k_recw_max_eclipse_value_for_valid_object)
          && (p_recw_object->tracker_data.accuracy_heading <= p_cals->k_recw_heading_accuracy_threshold))
      {
         f_object_is_relevant = FBK_TRUE;
      }
   }

   return f_object_is_relevant;
}


/*===========================================================================*\
 * Local Function Definitions
\*===========================================================================*/

static void Recw_Fill_Object_Valid_Crit(Recw_Obj_Valid_Crit_T *p_recw_valid_criteria,
                                        const Recw_Persistent_T *p_persistent,
                                        const Recw_Object_T *p_recw_object,
                                        const Recw_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_recw_valid_criteria);
   assert(NULL != p_recw_object);
   assert(NULL != p_cals);
   assert(NULL != p_persistent);

   p_recw_valid_criteria->long_rel_vel_range =
      Create_Float_Range(p_cals->k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1],
                         p_cals->k_recw_max_rel_velocity[p_persistent->recw_alert_prev_cycle]);
   p_recw_valid_criteria->max_approach_angle        = p_cals->k_recw_max_heading[p_persistent->recw_alert_prev_cycle];
   p_recw_valid_criteria->min_existence_probability = p_cals->k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_1];
   p_recw_valid_criteria->lane_filter_width         = p_cals->k_recw_lane_filter_width;

   if (p_recw_object->tracker_data.id == p_persistent->recw_id_prev_cycle)
   {
      p_recw_valid_criteria->max_approach_angle += p_cals->k_recw_max_heading_hys;
      p_recw_valid_criteria->long_rel_vel_range.min -= p_cals->k_recw_min_rel_velocity_hys[RECW_INDEX_ALERT_LEVEL_1];
      p_recw_valid_criteria->long_rel_vel_range.max += p_cals->k_recw_max_rel_velocity_hys;
      p_recw_valid_criteria->min_existence_probability -= p_cals->k_recw_min_existence_prob_hys;
      p_recw_valid_criteria->lane_filter_width += p_cals->k_recw_lane_filter_width_hys;
   }
}

static boolean_T Recw_Is_Obj_Heading_Relevant(const Recw_Obj_Valid_Crit_T *p_recw_valid_criteria,
                                              const Recw_Object_T *p_recw_object,
                                              const Recw_Core_Calibration_T *p_cals)
{
   boolean_T f_obj_heading_relevant = FBK_FALSE;

   if (((Fbk_Is_True(p_cals->k_recw_f_enable_heading_filter)
         && (Fbk_Abs_F(p_recw_object->attributes.filtered_heading) <= p_recw_valid_criteria->max_approach_angle))
        || (Fbk_Is_False(p_cals->k_recw_f_enable_heading_filter)
            && (Fbk_Abs_F(p_recw_object->tracker_data.vcs_heading) <= p_recw_valid_criteria->max_approach_angle)))
       && (Fbk_Abs_F(p_recw_object->tracker_data.vcs_heading - p_recw_object->attributes.filtered_heading)
           < p_cals->k_recw_max_allowed_heading_diff))
   {
      f_obj_heading_relevant = FBK_TRUE;
   }

   return f_obj_heading_relevant;
}

static boolean_T Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached(const Recw_Object_T *p_recw_object, const Recw_Core_Calibration_T *p_cals)
{
   boolean_T f_obj_cycles_to_ignore_reached = FBK_FALSE;

   if ((PA_OBJ_STATUS_COASTED == p_recw_object->tracker_data.status)
       && (p_recw_object->tracker_data.stage_age > p_cals->k_recw_max_allowed_consecutive_coasted_cycles))
   {
      f_obj_cycles_to_ignore_reached = FBK_TRUE;
   }

   return f_obj_cycles_to_ignore_reached;
}

static boolean_T Recw_Is_Obj_Behind_Ego(const Recw_Object_T *p_recw_object)
{
   boolean_T f_obj_behind_ego = FBK_FALSE;

   float32_T obj_rear_bumper_pos = p_recw_object->tracker_data.vcs_pos.x - (0.5f * p_recw_object->tracker_data.length);

   /* We check if the obj rear bumper is still behind the ego front bumper. */
   if (obj_rear_bumper_pos < FBK_ZERO_F)
   {
      f_obj_behind_ego = FBK_TRUE;
   }

   return f_obj_behind_ego;
}

static boolean_T Recw_Is_Long_Rel_Vel_In_Valid_Range(const Recw_Obj_Valid_Crit_T *p_recw_valid_criteria,
                                                     const Recw_Object_T *p_recw_object,
                                                     const Recw_Core_Calibration_T *p_cals)
{
   boolean_T f_obj_long_rel_vel = FBK_FALSE;

   if ((Is_Float_Contained_In_Float_Range(p_recw_object->tracker_data.vcs_vel_rel.x, &p_recw_valid_criteria->long_rel_vel_range))
       && (Fbk_Abs_F(p_recw_object->tracker_data.vcs_vel_rel.x - p_recw_object->attributes.effective_rel_vel.x)
           < p_cals->k_recw_max_allowed_rel_vel_long_diff)
       && (Fbk_Abs_F(p_recw_object->tracker_data.vcs_vel_rel.y - p_recw_object->attributes.effective_rel_vel.y)
           < p_cals->k_recw_max_allowed_rel_vel_lat_diff))
   {
      f_obj_long_rel_vel = FBK_TRUE;
   }

   return f_obj_long_rel_vel;
}

static boolean_T Recw_Is_Object_In_Ego_Lane(Recw_Persistent_T *p_persistent,
                                            const Recw_Obj_Valid_Crit_T *p_recw_valid_criteria,
                                            const Recw_Object_T *p_recw_object,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data,
                                            const Recw_Core_Calibration_T *p_cals)
{
   boolean_T f_is_in_lane = FBK_FALSE;
   float32_T used_lon_pos = p_recw_object->tracker_data.curvi_pos.x;
   float32_T used_lat_pos = p_recw_object->tracker_data.curvi_pos.y;
   float32_T effective_lane_width;
   uint8_t obj_id = p_recw_object->tracker_data.id;

   /* Asserts */
   assert(NULL != p_persistent);
   assert(NULL != p_recw_valid_criteria);
   assert(NULL != p_recw_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   /* If absolute ego speed is below speed threshold use vcs coordinates instead of curvi coordinates. */
   if (Fbk_Abs_F(p_vehicle_data->host_speed) <= p_cals->k_recw_lane_filter_max_abs_ego_speed_vcs_coord)
   {
      used_lon_pos = p_recw_object->tracker_data.vcs_pos.x;
      used_lat_pos = p_recw_object->tracker_data.vcs_pos.y;
   }

   /* Calculate effective lane width according to slope calibration value. */
   effective_lane_width = p_recw_valid_criteria->lane_filter_width + (p_cals->k_recw_lane_width_slope * Fbk_Abs_F(used_lon_pos));

   if (Fbk_Abs_F(used_lat_pos) < (0.5f * effective_lane_width))
   {
      Sat_Inc_Uint8(&(p_persistent->object_data[obj_id].object_within_lane_counter));
      if (p_cals->k_recw_lane_filter_num_consecutive_cycles < (p_persistent->object_data[obj_id].object_within_lane_counter))
      {
         f_is_in_lane = FBK_TRUE;
      }
   }
   else
   {
      p_persistent->object_data[obj_id].object_within_lane_counter = FBK_ZERO_UINT;
   }

   return f_is_in_lane;
}


static boolean_T Recw_Is_Traffic_Light_Ghost(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                             const Recw_Object_T *p_recw_object,
                                             const Recw_Core_Calibration_T *p_cals)
{
   boolean_T f_is_traffic_light_ghost = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_recw_object);
   assert(NULL != p_cals);

   if (Fbk_Is_True(p_cals->k_recw_f_enable_traffic_light_ghost_detection))
   {
      if ((p_recw_object->tracker_data.age < p_cals->k_recw_min_age_for_close_slow_targets)
          && (p_recw_object->tracker_data.vcs_vel.x < p_cals->k_recw_min_abs_speed_for_young_close_targets)
          && ((p_recw_object->tracker_data.vcs_pos.x + p_vehicle_data->host_length) > -p_cals->k_recw_min_dist_for_young_slow_targets))
      {
         f_is_traffic_light_ghost = FBK_TRUE;
      }
   }

   return f_is_traffic_light_ghost;
}
