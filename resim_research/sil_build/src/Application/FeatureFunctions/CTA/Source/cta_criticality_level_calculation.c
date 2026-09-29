/**
 * @file cta_criticality_level_calculation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the criticality level calculation functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "cta_criticality_level_calculation.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_line_hesse.h"
#include "ml_line_hesse_t.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
#include <assert.h>

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Set values of the p_object_highest_crit struct. Also a deep copy
 * for the tracker_output is executed.
 *
 * @return void
 *
 * @SRS{SF-231}
 * @SAE{SF-2459}
 * @SDD{SF-3787}
 * @verification{Check that the provided object is given the highest criticality level.}
 */
static void Cta_Set_Object_Highest_Crit(Cta_Object_Data_T *p_object_highest_crit /**< object with highest criticality*/,
                                        const Cta_Object_Data_T *p_object /**< CTA object data*/);

/**
 * @brief Applies an hysteresis on the threshold level logic.
 *
 * @return void
 *
 * @SRS{SF-221}
 * @SAE{SF-2459}
 * @SDD{SF-3782}
 * @verification{Check that an hysteresis is applied correctly.}
 */
static void Cta_Apply_Level_Thres_Hyst(
   Cta_Crit_Level_Calibration_T *p_extended_crit_level /**<criticality level calibrations to be extended by hysteresis*/,
   const Cta_Core_Calibration_T *p_cta_cal /**< cta_calibration */,
   const Cta_Mode_T cta_mode /**<*/,
   const uint8_t level_index /**<index of the considered level*/);

/**
 * @brief Evaluates criticality level of target from relevant object list based on criteria determined by
 * - TTC in x-axis
 * - radial distance
 * - intersection point in x-axis.
 *
 * @return True if current criticality level is reached
 *
 * @SRS{SF-210,SF-219,SF-225}
 * @SAE{SF-2459}
 * @SDD{SF-3783}
 * @verification{Check that the criticality level is determined correctly.}
 */
static boolean_T
Cta_Check_Single_Level(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                       const Cta_Crit_Level_Calibration_T
                          *p_extended_crit_level /**< criticality level calibrations which could be occupied with hysteresis*/,
                       const Cta_Core_Calibration_T *p_cta_cal /**< calibration data*/,
                       const boolean_T f_target_in_zone /**< flag indicating if target is within CTA zone*/,
                       const Cta_Mode_T cta_mode /**< mode of cta 0 - rear cta, 1 - front cta*/,
                       const uint8_t level_index /**< Level index*/);

/**
 * @brief Sets values within the threshold data struct corresponding to the respective
 * criticality level.
 *
 * @return void
 *
 * @SRS{SF-231}
 * @SAE{SF-2459}
 * @SDD{SF-3786}
 * @verification{Check that the values within the threshold data struct are set correctly.}
 */
static void Cta_Set_Additional_Thres_Data(Cta_Crit_Level_Calibration_T *p_extended_crit_level /**< additional threshold data*/,
                                          const Cta_Comparison_Data_T *p_cta_comparison_data /**< cta data for comparison*/,
                                          const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                          const Cta_Object_Data_T *p_object /**< cta object data*/,
                                          const Cta_Persistent_T *p_persistent /**< persistent data of cta feature*/,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                                          const uint8_t level_index /**< crit level of object within previous cycle*/,
                                          const Cta_Mode_T cta_mode /**< cta mode 0 - RCTA, 1 - FCTA*/);

/**
 * @brief Checks if the track status is sufficient. That means, that
 * the target needs to have the same track status over several cycles.
 *
 * @return True if the current track status is sufficient enough for further consideration of the specific object
 *
 * @SRS{SF-210,SF-219,SF-225}
 * @SAE{SF-2459}
 * @SDD{SF-3785}
 * @verification{Check that the function detects the track status sufficiency correctly.}
 */
static boolean_T Cta_Is_Track_Status_Sufficient(
   const Fbk_Object_Data_T *p_cta_obj_tracker_output /**< object data*/,
   const Cta_Crit_Level_Calibration_T *p_extended_crit_level /**< extended criticality level calibration */);

/**
 * @brief Checks whether the object is outside of the sensor FOV.
 * This is done using the cross product of two vectors and checking whether the
 * reference point of an object is out of the FOV.
 *
 * @return True if objects front corners are outside of sensor fov
 *
 * @SRS{SF-237}
 * @SAE{SF-2459}
 * @SDD{SF-3784}
 * @verification{Check that the function detects an object outside of the sensor field of view correctly.}
 */
static boolean_T Cta_Is_Obj_Outside_Of_Sensor_Fov(const Cta_Object_Attributes_T *p_attributes /**< cta object attributes*/,
                                                  const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                                                  const Cta_Mode_T cta_mode /**< cta mode. 0 - Rear cta, 1 - Fcta*/);

/**
 * @brief Checks whether more suppression cycles should be applied.
 *
 * @return True if all conditions for more suppression cycles are fulfilled
 *
 * @SRS{SF-237}
 * @SAE{SF-2459}
 * @SDD{SF-3788}
 * @verification{Check that the addition of suppression cycles is determined correctly.}
 */
static boolean_T Cta_Shall_Addit_Pos_Suppr_Be_Appl(const Cta_Object_Attributes_T *p_attributes /**< cta object attributes*/,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                                                   const Line_Hesse_T line /**< side of point relative to line*/,
                                                   const Cta_Mode_T cta_mode /**< cta mode. 0 - rcta, 1 - fcta*/);

/*===========================================================================*\
* Global Functions Definitions
\*===========================================================================*/

void Cta_Check_All_Level(Cta_Crit_Level_Calibration_T *p_extended_crit_level,
                         Cta_Comparison_Data_T *p_cta_comparison_data,
                         const Cta_Persistent_T *p_persistent,
                         const Cta_Object_Data_T *p_object,
                         const Cta_Core_Calibration_T *p_cta_cal,
                         const boolean_T f_target_in_zone,
                         const Fbk_Vehicle_Data_T *p_vehicle_data,
                         const Cta_Mode_T cta_mode)
{
   uint8_t level_index;
   Cta_Crit_Level_T obj_current_cycle_crit_level;
   Cta_Object_Data_T *p_object_highest_crit;
   boolean_T f_increase_counter = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_persistent);
   assert(NULL != p_extended_crit_level);
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   obj_current_cycle_crit_level = CTA_CRIT_LEVEL_NONE;
   p_object_highest_crit = &(p_cta_comparison_data->object_with_highest_crit[cta_mode][p_object->attributes->approach_side]);

   /* Loop through criticality level */
   for (level_index = 0; level_index < CTA_NUM_CRIT_LEVEL; level_index++)
   {
      boolean_T f_level_reached;

      Cta_Set_Additional_Thres_Data(p_extended_crit_level, p_cta_comparison_data, p_cta_cal, p_object, p_persistent,
                                    p_vehicle_data, level_index, cta_mode);

      /* Set hysteresis when object was critical to at least the same extend as before*/
      if (p_object->persistent->prev_cycle_crit_level[cta_mode] >= (level_index + FBK_ONE_UINT))
      {
         Cta_Apply_Level_Thres_Hyst(p_extended_crit_level, p_cta_cal, cta_mode, level_index);
      }

      f_level_reached = Cta_Check_Single_Level(p_object, p_extended_crit_level, p_cta_cal, f_target_in_zone, cta_mode, level_index);

      if (Fbk_Is_True(f_level_reached))
      {
         f_increase_counter = FBK_TRUE;
         /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast back to enum type] */
         /* coverity[misra_c_2012_rule_10_8_violation][Intentional cast back to enum type] */
         obj_current_cycle_crit_level = (Cta_Crit_Level_T) (level_index + FBK_ONE_UINT);
         /* check if current target has highest criticality level */
         if (obj_current_cycle_crit_level > p_cta_comparison_data->max_level[cta_mode][p_object->attributes->approach_side])
         {
            p_cta_comparison_data->max_level[cta_mode][p_object->attributes->approach_side] = obj_current_cycle_crit_level;
            Cta_Set_Object_Highest_Crit(p_object_highest_crit, p_object);
         }
         else if ((obj_current_cycle_crit_level == p_cta_comparison_data->max_level[cta_mode][p_object->attributes->approach_side])
                  && (p_object->attributes->ttc <= p_object_highest_crit->attributes->ttc))
         {
            Cta_Set_Object_Highest_Crit(p_object_highest_crit, p_object);
         }
         else
         {
            /* Do nothing*/
         }
      }
   }

   if (Fbk_Is_True(f_increase_counter))
   {
      /* Count number of alert cycles up. */
      Sat_Inc_Uint8(&(p_object->persistent->n_alert_cycles[cta_mode]));
   }

   p_object->persistent->prev_cycle_crit_level[cta_mode] = (uint8_t) obj_current_cycle_crit_level;
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static void Cta_Apply_Level_Thres_Hyst(Cta_Crit_Level_Calibration_T *p_extended_crit_level,
                                       const Cta_Core_Calibration_T *p_cta_cal,
                                       const Cta_Mode_T cta_mode,
                                       const uint8_t level_index)
{
   float32_T diff_level_isect;
   float32_T diff_hyst_intersect;

   /* Asserts */
   assert(NULL != p_extended_crit_level);

   /*Apply Level threshold hystereses*/

   /*Weight hysteresis for intersection zone with 0.5f for an absolute extension
    * in percent specified in used cal parameter (hiding in level calibration)*/
   diff_level_isect    = (p_extended_crit_level->max_long_point_criticality_level[cta_mode][level_index]
                       - p_extended_crit_level->min_long_point_criticality_level[cta_mode][level_index]);
   diff_hyst_intersect = 0.5f * p_cta_cal->k_cta_rel_warning_hysteresis * diff_level_isect;


   p_extended_crit_level->ttc_criticality_level[cta_mode][level_index] *= FBK_ONE_F + p_cta_cal->k_cta_rel_warning_hysteresis;
   p_extended_crit_level->min_long_point_criticality_level[cta_mode][level_index] -= diff_hyst_intersect;
   p_extended_crit_level->max_long_point_criticality_level[cta_mode][level_index] += diff_hyst_intersect;


   /* Apply hysteresis to additional thresholds*/
   p_extended_crit_level->max_eclipse_value = FBK_ONE_F;
   p_extended_crit_level->mature_cycles     = FBK_ZERO_UINT;
   p_extended_crit_level->minimum_age       = FBK_ZERO_UINT;
}

static boolean_T Cta_Check_Single_Level(const Cta_Object_Data_T *p_object,
                                        const Cta_Crit_Level_Calibration_T *p_extended_crit_level,
                                        const Cta_Core_Calibration_T *p_cta_cal,
                                        const boolean_T f_target_in_zone,
                                        const Cta_Mode_T cta_mode,
                                        const uint8_t level_index)
{
   boolean_T f_level_reached       = FBK_FALSE;
   boolean_T f_isec_in_range       = FBK_TRUE;
   Float_Range_T criticality_level = {0.0f, 0.0f};

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_extended_crit_level);

   /* Create float range using critalicity level lines and check whether reference point is in range */
   criticality_level = Create_Float_Range(p_extended_crit_level->min_long_point_criticality_level[cta_mode][level_index],
                                          p_extended_crit_level->max_long_point_criticality_level[cta_mode][level_index]);

   if (Is_Float_Contained_In_Float_Range(p_object->attributes->long_isect_point_candidate[FBK_SIDE_LEFT][cta_mode], &criticality_level))
   {
      p_object->attributes->ref_point                  = p_object->attributes->ref_point_candidate[FBK_SIDE_LEFT];
      p_object->attributes->long_isect_point[cta_mode] = p_object->attributes->long_isect_point_candidate[FBK_SIDE_LEFT][cta_mode];
   }

   else if (Is_Float_Contained_In_Float_Range(p_object->attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][cta_mode],
                                              &criticality_level))
   {
      p_object->attributes->ref_point                  = p_object->attributes->ref_point_candidate[FBK_SIDE_RIGHT];
      p_object->attributes->long_isect_point[cta_mode] = p_object->attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][cta_mode];
   }

   else
   {
      p_object->attributes->long_isect_point[cta_mode] =
         Fbk_Half(p_object->attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][cta_mode]
                  + p_object->attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][cta_mode]);
      f_isec_in_range = FBK_FALSE;
   }

   if ((Fbk_Is_True(f_target_in_zone))
       && (p_object->attributes->ttc < p_extended_crit_level->ttc_criticality_level[cta_mode][level_index]) && (f_isec_in_range)
       && (p_object->tracker_data.eclipse_value <= p_extended_crit_level->max_eclipse_value)
       && (p_object->tracker_data.age >= p_extended_crit_level->minimum_age)
       && (p_object->tracker_data.speed >= p_extended_crit_level->speed_criticality_level[cta_mode][level_index]))
   {
      Sat_Inc_Uint8(&p_object->persistent->crit_level_suppression_counter[cta_mode][level_index]);

      /*Check whether suppression counter for this respective criticality level is exceeding the qualification cycles. When an
       * object is also mature, the object reaches the respective criticality level.*/
      if ((p_object->persistent->crit_level_suppression_counter[cta_mode][level_index] > p_cta_cal->k_cta_cycle_count_suppress_true_warning)
          && (Cta_Is_Track_Status_Sufficient(&(p_object->tracker_data), p_extended_crit_level))
          && (p_object->persistent->n_alert_cycles[cta_mode] < (uint8_t) UINT8_MAX))
      {
         /*Limit alert cycles to UINT8_MAX*/
         f_level_reached = FBK_TRUE;
      }
   }
   else
   {
      /*Reset criticality suppression counter of the object for a specific criticality level.*/
      p_object->persistent->crit_level_suppression_counter[cta_mode][level_index] = FBK_ZERO_UINT;
   }

   return f_level_reached;
}

static void Cta_Set_Object_Highest_Crit(Cta_Object_Data_T *p_object_highest_crit, const Cta_Object_Data_T *p_object)
{
   /* Asserts */
   assert(NULL != p_object_highest_crit);
   assert(NULL != p_object);

   p_object_highest_crit->attributes   = p_object->attributes;
   p_object_highest_crit->persistent   = p_object->persistent;
   p_object_highest_crit->tracker_data = p_object->tracker_data;
}

static void Cta_Set_Additional_Thres_Data(Cta_Crit_Level_Calibration_T *p_extended_crit_level,
                                          const Cta_Comparison_Data_T *p_cta_comparison_data,
                                          const Cta_Core_Calibration_T *p_cta_cal,
                                          const Cta_Object_Data_T *p_object,
                                          const Cta_Persistent_T *p_persistent,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                                          const uint8_t level_index,
                                          const Cta_Mode_T cta_mode)
{
   /* helper flags*/
   boolean_T f_prev_none;
   boolean_T f_out_sens;

   /* Asserts */
   assert(NULL != p_extended_crit_level);
   assert(NULL != p_cta_cal);
   assert(NULL != p_object);
   assert(NULL != p_persistent);

   p_extended_crit_level->max_eclipse_value = p_cta_cal->k_cta_max_object_eclipse_for_level_qualification;
   p_extended_crit_level->mature_cycles     = p_cta_cal->k_cta_min_mature_cycles_level_qualifiction;

   f_prev_none = (boolean_T) (((uint8_t) CTA_CRIT_LEVEL_NONE) == p_object->persistent->prev_cycle_crit_level[cta_mode]);
   f_out_sens  = Cta_Is_Obj_Outside_Of_Sensor_Fov(p_object->attributes, p_cta_cal, p_vehicle_data, cta_mode);

   if (f_prev_none && f_out_sens)
   {
      p_extended_crit_level->mature_cycles =
         (uint16_t) (p_extended_crit_level->mature_cycles + p_cta_cal->k_cta_addit_mature_cycles_outside_sensor_fov);
      p_extended_crit_level->minimum_age = p_cta_cal->k_cta_min_age_obj_outside_sensor_fov;
   }
   else if ((CTA_CRIT_LEVEL_NONE == p_cta_comparison_data->max_level[cta_mode][p_object->attributes->approach_side])
            && (NULL == p_object->attributes->p_pt_match_info))
   {
      p_extended_crit_level->minimum_age = p_cta_cal->k_cta_min_object_age_thres;

      if (p_object->attributes->ttc > p_cta_cal->k_cta_min_ttc_additional_mature_qualification)
      {
         p_extended_crit_level->mature_cycles =
            (uint16_t) (p_extended_crit_level->mature_cycles + p_cta_cal->k_cta_additional_qualification_mature_cycles);
      }
   }
   else if (CTA_CRIT_LEVEL_NONE == p_cta_comparison_data->max_level[cta_mode][p_object->attributes->approach_side])
   {
      if ((PATH_DIRECTION_LAT_LEFT != p_object->attributes->p_pt_match_info->path_direction)
          && (PATH_DIRECTION_LAT_RIGHT != p_object->attributes->p_pt_match_info->path_direction))
      {
         p_extended_crit_level->minimum_age = p_cta_cal->k_cta_min_object_age_thres;

         if (p_object->attributes->ttc > p_cta_cal->k_cta_min_ttc_additional_mature_qualification)
         {
            p_extended_crit_level->mature_cycles =
               (uint16_t) (p_extended_crit_level->mature_cycles + p_cta_cal->k_cta_additional_qualification_mature_cycles);
         }
      }
      else
      {
         p_extended_crit_level->minimum_age = FBK_ZERO_UINT;
      }
   }
   else
   {
      p_extended_crit_level->minimum_age = FBK_ZERO_UINT;
   }

   /*Check whether the most critical object of a respective side
   has been merged to another object. This shall prevent warning interruption*/
   if (Fbk_Is_True(p_object->tracker_data.f_merge_occured)
       && (p_persistent->previous_most_critical_obj_id[cta_mode][p_object->attributes->approach_side]
           == p_object->tracker_data.id_merged_obj))
   {
      if ((uint8_t) p_persistent->previous_crit_level[cta_mode][p_object->attributes->approach_side] >= level_index)
      {
         p_object->persistent->crit_level_suppression_counter[cta_mode][level_index] = CTA_COUNTER_MAX;
      }
      p_extended_crit_level->minimum_age   = FBK_ZERO_UINT;
      p_extended_crit_level->mature_cycles = FBK_ZERO_UINT;
   }
}

static boolean_T Cta_Is_Track_Status_Sufficient(const Fbk_Object_Data_T *p_cta_obj_tracker_output,
                                                const Cta_Crit_Level_Calibration_T *p_extended_crit_level)
{
   boolean_T ret;

   /* Asserts */
   assert(NULL != p_cta_obj_tracker_output);
   assert(NULL != p_extended_crit_level);

   if (p_extended_crit_level->mature_cycles > FBK_ZERO_UINT)
   {
      if ((PA_OBJ_STATUS_MATURE == p_cta_obj_tracker_output->status)
          && (p_cta_obj_tracker_output->stage_age >= p_extended_crit_level->mature_cycles))
      {
         ret = FBK_TRUE;
      }
      else
      {
         ret = FBK_FALSE;
      }
   }
   else
   {
      ret = FBK_TRUE;
   }

   return ret;
}

static boolean_T Cta_Is_Obj_Outside_Of_Sensor_Fov(const Cta_Object_Attributes_T *p_attributes,
                                                  const Cta_Core_Calibration_T *p_cta_cal,
                                                  const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                  const Cta_Mode_T cta_mode)
{
   Line_Hesse_T line;
   Angle_T exlusion_angle;
   Vector_2d_T origin;
   Vector_2d_T end_of_line;
   boolean_T f_result = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_attributes);
   assert(NULL != p_cta_cal);

   exlusion_angle = Create_Angle(Fbk_Abs_F(p_cta_cal->k_cta_sensor_fov_border[cta_mode]));

   /*Set up longitudinal origin component (first point)*/
   if (CTA_MODE_REAR == cta_mode)
   {
      origin.x = -FBK_ONE_F * p_vehicle_data->host_length;
   }
   else
   {
      origin.x = FBK_ZERO_F;
   }

   /*Choose lateral origin approximately on sensor and second point for the line construction*/
   if (FBK_SIDE_RIGHT == p_attributes->approach_side)
   {
      origin.y = (0.5f * p_vehicle_data->host_width) * p_cta_cal->k_cta_host_width_sensor_fov_suppr_factor;
      if (CTA_MODE_REAR == cta_mode)
      {
         end_of_line = Create_2d_Vector_Coordinates(origin.x + exlusion_angle.cos, origin.y + exlusion_angle.sin);
      }
      else
      {
         end_of_line = Create_2d_Vector_Coordinates(origin.x - exlusion_angle.cos, origin.y + exlusion_angle.sin);
      }
   }
   else
   {
      origin.y = (-0.5f * p_vehicle_data->host_width) * p_cta_cal->k_cta_host_width_sensor_fov_suppr_factor;

      if (CTA_MODE_REAR == cta_mode)
      {
         end_of_line = Create_2d_Vector_Coordinates(origin.x + exlusion_angle.cos, origin.y - exlusion_angle.sin);
      }
      else
      {
         end_of_line = Create_2d_Vector_Coordinates(origin.x - exlusion_angle.cos, origin.y - exlusion_angle.sin);
      }
   }

   /*Calculate on which side the object point is located and check if object is outside fov*/
   line = Line_Hesse_Create_Using_Two_Points(&(origin), &(end_of_line));
   if (Cta_Shall_Addit_Pos_Suppr_Be_Appl(p_attributes, p_vehicle_data, line, cta_mode))
   {
      f_result = FBK_TRUE;
   }

   return f_result;
}

static boolean_T Cta_Shall_Addit_Pos_Suppr_Be_Appl(const Cta_Object_Attributes_T *p_attributes,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                   const Line_Hesse_T line,
                                                   const Cta_Mode_T cta_mode)
{
   boolean_T f_result = FBK_TRUE;
   boolean_T f_outside_sensor_fov_fcta;
   boolean_T f_outside_sensor_fov_rcta;
   Side_Of_Line_Hesse_T side_of_point;
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_attributes);

   for (idx = FBK_SIDE_LEFT; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      side_of_point = Line_Hesse_Get_Side_of_Point(&(line), &(p_attributes->ref_point_candidate[idx].point), FBK_ZERO_F);
      f_outside_sensor_fov_fcta =
         (boolean_T) ((CTA_MODE_FRONT == cta_mode) && (p_attributes->ref_point_candidate[idx].point.x < THRESHOLD_IS_ZERO)
                      && (((FBK_SIDE_LEFT == p_attributes->approach_side) && (LINE_HESSE_SIDE_NEGATIVE == side_of_point))
                          || ((FBK_SIDE_RIGHT == p_attributes->approach_side) && (LINE_HESSE_SIDE_POSITIVE == side_of_point))));

      f_outside_sensor_fov_rcta =
         (boolean_T) ((CTA_MODE_REAR == cta_mode)
                      && (p_attributes->ref_point_candidate[idx].point.x > (-(p_vehicle_data->host_length)))
                      && (((FBK_SIDE_LEFT == p_attributes->approach_side) && (LINE_HESSE_SIDE_POSITIVE == side_of_point))
                          || ((FBK_SIDE_RIGHT == p_attributes->approach_side) && (LINE_HESSE_SIDE_NEGATIVE == side_of_point))));

      // If any of point is inside fov, set flag to false
      if (Fbk_Is_False(f_outside_sensor_fov_fcta || f_outside_sensor_fov_rcta))
      {
         f_result = FBK_FALSE;
         break;
      }
   }

   return f_result;
}
