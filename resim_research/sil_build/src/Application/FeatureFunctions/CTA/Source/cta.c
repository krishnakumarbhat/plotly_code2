/**
 * @file cta.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Core CTA algorithm.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "cta.h"
#include "cta_braking_logic.h"
#include "cta_common_functions.h"
#include "cta_conflict_zone_adapter.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_counters.h"
#include "cta_criticality_level_calculation.h"
#include "cta_debug_interface.h"
#include "cta_factory.h"
#include "cta_object_validator.h"
#include "cta_persistent_t.h"
#include "cta_struct_initializer.h"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "fbk_vehicle_data_t.h"
#include "ml_angle_normalize.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Used to check if a CTA alert should be given for a given object. The function only considers a single object at a time.
 *
 * @return void
 *
 * @SRS{SF-236,SF-198,SF-170}
 * @SAE{SF-2459}
 * @SDD{SF-3702}
 * @verification{Check that the single object check is not returning an alert level when executed with invalid signals.}
 */
static void Cta_Check_Single_Object(Cta_Instance_T *p_cta_instance /**< CTA Instance Pointer*/,
                                    Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params /**< conflict zone extension*/,
                                    Cta_Comparison_Data_T *p_cta_comparison_data /**< comparison data for criticality level*/,
                                    const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data /**< Information on host vehicle*/);

/**
 * @brief Loops over all given objects and calls subroutines to check, whether a Cta alert shall be given for the objects.
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3701}
 * @verification{Check that main algorithm is not returning any alert levels when executed with invalid signals.}
 */
static void Cta_Algorithm(Cta_Instance_T *p_cta_instance /**< CTA Instance pointer */,
                          Cta_Comparison_Data_T *p_cta_comparison_data /**< data used for criticality comparison*/,
                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< information on host vehicle*/);

/**
 * @brief Flips CTA zone if needed based on the approaching direction and if Fcta or Rcta is enabled.
 *
 * @return void
 *
 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3706}
 * @verification{Check that the provided CTA zone is flipped in the appropriate direction along the VCS axes.}
 */
static void Cta_Flip_Zone(const uint8_t approach_side /**< approach side of the considered objects*/,
                          Fbk_Field_Of_Interest_T *p_cta_zone /**<[in, out] CTA zone*/);

/**
 * @brief Calculate the longitudinal component of the intersection point.
 *
 * @return longitudinal component of intersection point for the given object in m
 *
 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3707}
 * @verification{Check that the longitudinal component of the intersection point is returned.}
 */
static void Cta_Get_Longitudinal_Intersection(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                              const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */,
                                              const Cta_Mode_T cta_mode /**< cta mode */);

/**
 * @brief Returns the target relative velocity vector.
 * This can either be accomplished by taking relative velocity from the tracker output or by sine and cosine
 * decomposition of the absolute speed and subtracting the ego speed.
 *
 * @return two dimensional relative velocity of an object in m/s
 *
 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3708}
 * @verification{Check that the target relative velocity vector is calculated and returned correctly.}
 */
static void Cta_Get_Relative_Velocity_Of_Object(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< Information on host vehicle*/,
                                                const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);


/**
 * @brief Checks whether an object has been created directly behind the host vehicle
 *        This might occure when an object is lost near the sensor field of view boundaries
 *        and recovers by another object id. Internally sets FBK_TRUE when an object is newly created near the longitudinal
 *        intersection line. This flag shall be kept for the rest of the life cycle of the object, even when it is recovering as
 valid cta candidate.
 *
 * @return void

 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3709}
 * @verification{Check that this function detects if an object was created near a suppression line.}
 */
static void
Cta_Check_If_Obj_Created_Near_Suppression_Line(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< Information on the host vehicle*/,
                                               const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Checks if rear object corners are exceeding a threshold
 *
 * @return FBK_TRUE if the rear object corners exceed the user defined threshold
 *
 * @SRS{SF-236}
 * @SAE{SF-2459}
 * @SDD{SF-3705}
 * @verification{Check that this function correctly detects the crossing of the long axis and the objects rear corners.}
 */
static boolean_T Cta_Do_Obj_Corners_Cross_Long_Axis(const Cta_Object_Data_T *p_object /**<  Object data*/,
                                                    const Fbk_Object_Corners_T *p_target_corners /**<  target_corners*/,
                                                    const Cta_Core_Calibration_T *p_cta_cal /**<  calibration_parameters*/,
                                                    const Fbk_Vehicle_Data_T *p_vehicle_data /**<  vehicle data*/);
/**
 * @brief Checks if a criticality level should be reset.
 *
 * @return FBK_TRUE if a warning shall be suppressed
 *
 * @SRS{SF-236}
 * @SAE{SF-2459}
 * @SDD{SF-3713}
 * @verification{Check that the function correctly detects a necessary criticality level reset.}
 */
static boolean_T Cta_Shall_Criticality_Level_Be_Reset(const Cta_Core_Calibration_T *p_cta_cal /**<  calibration parameters*/,
                                                      const Cta_Object_Data_T *p_object /**<  object data*/,
                                                      const Fbk_Object_Corners_T *p_target_object_corners /**<  target_corners*/,
                                                      const Fbk_Vehicle_Data_T *p_vehicle_data /**<  vehicle data*/);

/**
 * @brief Fills attributes of struct p_cta_core_output with enabled switch, warn level
 * and crash probabily values.
 *
 * @return void
 *
 * @SRS{SF-190}
 * @SAE{SF-2459}
 * @SDD{SF-3703}
 * @verification{Check that the CTA core output struct is filled correctly.}
 */
static void
Cta_Set_Core_Output(Cta_Instance_T *p_cta_instance /**< Core instance pointer */,
                    const Cta_Comparison_Data_T *p_cta_comparison_data /**< Comparison data for criticality level evaluation */,
                    const Fbk_Vehicle_Data_T *p_vehicle_data /**< Host vehicle information */,
                    const Cta_Status_T cta_status /**< CTA status */);

/**
 * @brief Checks the activation conditions for CTA and returns the status of the feature for this cycle.
 *
 * @return CTA status
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-4056}
 * @verification{Check if the correct status is returned if switch flag is on or velocity are in range}
 */
static Cta_Status_T Cta_Get_Status(const Fbk_Vehicle_Data_T *p_vehicle_data /**< Host vehcile information */,
                                   const Cta_Core_Calibration_T *p_cta_cal /**< Calibration parameters */,
                                   const Cta_Core_Input_T *p_cta_core_input /**< CTA core input */);

/**
 * @brief Functions sets the reference point basing on the following algorithm. At first, the nearest laterally point, which is in
 * zone, and which predicted x-axis intersection is in range is set as ref point. If there is no such point, the nearest laterally
 * point, which is in zone is taken. If there is no point in the zone, the closest point is choosen.
 *
 * @return void
 *
 * @SRS{SF-192,SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{CSCSA-16663}
 * @verification{Check that correct reference point (and distance) is returned for several scenario cases, with different
 * combinations of ego and object position and orientation.}
 */
static void Cta_Get_Reference_Point_And_Fill_Properties(const Cta_Object_Data_T *p_object /**< [in, out] cta object */,
                                                        const Fbk_Vehicle_Data_T *p_vehicle_data /**< Vehicle data */,
                                                        const Fbk_Object_Corners_T *p_target_corners /**< Object corners */,
                                                        const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/*===========================================================================*\
* Global Functions	Definition
\*===========================================================================*/

void Cta_Reset(Cta_Instance_T *p_cta_instance)
{
   Cta_Reset_Persistent(&p_cta_instance->persistent);

   Cta_Reset_Core_Output(&p_cta_instance->core_output);

   Cta_Reset_Obj_Persistent_Array(p_cta_instance->obj_persistent_array);
}

void Cta_Core_Run(Cta_Instance_T *p_cta_instance)
{
   Cta_Status_T cta_status;

   /* Fill vehicle data */
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_cta_instance->core_input.p_pa_data->vehicle_data;

   /* Asserts */
   assert(NULL != p_cta_instance);

   /* Reset debug data */
   Binary_Cta_Debug_Reset_Data();

   /* Get status of CTA */
   cta_status = Cta_Get_Status(p_vehicle_data, &p_cta_instance->calibration, &p_cta_instance->core_input);

   if (CTA_STATUS_ACTIVE == cta_status)
   {
      Cta_Comparison_Data_T cta_comparison_data;

      /*Reset pointer to most critical object from last cycle and max level of last cycle.*/
      Cta_Init_Comparison_Data(&cta_comparison_data, p_cta_instance);

      /*Execute main CTA algorithm*/
      Cta_Algorithm(p_cta_instance, &cta_comparison_data, p_vehicle_data);

      /*Set the output*/
      Cta_Set_Core_Output(p_cta_instance, &cta_comparison_data, p_vehicle_data, cta_status);
   }
   else
   {
      Cta_Reset(p_cta_instance);
      p_cta_instance->core_output.cta_status = cta_status;
   }

   /* Pass general data to debug data */
   Binary_Cta_Debug_Pass_General_Data(p_cta_instance);
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static void Cta_Check_Single_Object(Cta_Instance_T *p_cta_instance,
                                    Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params,
                                    Cta_Comparison_Data_T *p_cta_comparison_data,
                                    const Cta_Object_Data_T *p_object,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_obj_in_zone;
   boolean_T f_obj_valid;
   Fbk_Field_Of_Interest_T cta_zone_obj;
   Fbk_Field_Of_Interest_T obj_foi;
   Fbk_Object_Corners_T target_corners;
   Cta_Crit_Level_Calibration_T criticality_level_calibration;

   /* Asserts */
   assert(NULL != p_cta_instance);
   assert(NULL != p_object);
   assert(NULL != p_confl_zone_ext_params);

   Cta_Init_Crit_Level_Cals(&criticality_level_calibration, &p_cta_instance->core_input, &p_cta_instance->calibration, p_vehicle_data);
   Cta_Get_Relative_Velocity_Of_Object(p_object, p_vehicle_data, &p_cta_instance->calibration);
   Cta_Check_If_Obj_Created_Near_Suppression_Line(p_object, p_vehicle_data, &p_cta_instance->calibration);

   Binary_Cta_Debug_Pass_Initials_And_Paths(p_object);

   f_obj_valid = Cta_Is_Object_Valid(p_object, &p_cta_instance->calibration);

   /* check if target is valid candidate for cta alert */
   if (f_obj_valid)
   {
      /*In case of an approach from the left side, we need to mirror the zone at the longitudinal axis.*/
      cta_zone_obj = p_cta_instance->core_input.cta_zone;
      Cta_Flip_Zone(p_object->attributes->approach_side, &cta_zone_obj);

      /* calculation of the eight reference points of the current target and determination of the most relevant reference point */
      Fbk_Calculate_Target_Corners(&target_corners, &(p_object->tracker_data.vcs_pos), &(p_object->attributes->CTA_heading),
                                   &(p_object->tracker_data.length), &(p_object->tracker_data.width));

      Cta_Get_Reference_Point_And_Fill_Properties(p_object, p_vehicle_data, &target_corners, &p_cta_instance->calibration);

      /* Decide if use TTP or TTC to stop the alert*/
      switch (p_cta_instance->core_input.cta_stop_mode)
      {
         case CTA_STOP_MODE_TTC:
            p_object->attributes->f_stop_time_below_ths =
               (boolean_T) (p_object->attributes->ttc < p_cta_instance->calibration.k_cta_stop_alert_ttc);
            break;
         case CTA_STOP_MODE_TTP:
            p_object->attributes->f_stop_time_below_ths =
               (boolean_T) (p_object->attributes->ttp < p_cta_instance->calibration.k_cta_stop_alert_ttp);
            break;
         default:
            p_object->attributes->f_stop_time_below_ths = FBK_FALSE;
            break;
      }

      /* Check whether the object is in the zone*/
      Fbk_Create_Field_Of_Interest_From_Object_Data(&obj_foi, p_object->tracker_data.vcs_pos, p_object->tracker_data.length,
                                                    p_object->tracker_data.width, p_object->attributes->CTA_heading);

      f_obj_in_zone = (boolean_T) (Fbk_Are_Fields_Of_Interest_Overlapping(&cta_zone_obj, &obj_foi));
      /*Check whether the object is in the customer specific (might be a butterfly zone) zone. */
      if (Fbk_Is_True(f_obj_in_zone))
      {
         /* As soon as this function returns true, the suppressed object will not receive a criticality level in its life cycle for
          * the current mode.*/
         if (Cta_Shall_Criticality_Level_Be_Reset(&p_cta_instance->calibration, p_object, &(target_corners), p_vehicle_data))
         {
            p_object->persistent->f_prev_cta_alert_suppress = FBK_TRUE;
            /*Used for bridging of holding logic*/
            p_cta_instance->persistent.warning_holding_counter[CTA_MODE_REAR][p_object->attributes->approach_side] = CTA_COUNTER_MAX;
            p_cta_instance->persistent.warning_holding_counter[CTA_MODE_FRONT][p_object->attributes->approach_side] = CTA_COUNTER_MAX;
         }

         if (Fbk_Is_False(p_object->attributes->f_stop_time_below_ths) && Fbk_Is_False(p_object->persistent->f_prev_cta_alert_suppress))
         {
            /* Determine criticality of the object in both possible cta modes (RCTA and FCTA). */
            uint8_t cta_mode;

            for (cta_mode = FBK_ZERO_UINT; cta_mode < (uint8_t) CTA_NUM_MODES; cta_mode++)
            {
               /* Check whether the respective cta mode is activated. */
               if (Fbk_Is_True(p_cta_instance->calibration.k_cta_enable_modes[cta_mode]))
               {
                  /* coverity[misra_c_2012_rule_10_5_violation][Intentional casting from integer type to enum type] */
                  Cta_Mode_T curr_cta_mode = (Cta_Mode_T) cta_mode;

                  /* Get the (object heading compensated) longitudinal intersection point for the current mode*/
                  Cta_Get_Longitudinal_Intersection(p_object, &p_cta_instance->calibration, p_vehicle_data, curr_cta_mode);

                  /* Adapt the conflict zone*/
                  Cta_Adapt_Long_Crit_Level_Ranges(&criticality_level_calibration, p_confl_zone_ext_params, p_object,
                                                   &p_cta_instance->calibration, curr_cta_mode);

                  /* Check criticality level for a specific mode of CTA */
                  Cta_Check_All_Level(&criticality_level_calibration, p_cta_comparison_data, &p_cta_instance->persistent, p_object,
                                      &p_cta_instance->calibration, f_obj_in_zone, p_vehicle_data, curr_cta_mode);
               }
            }
         }
      }
      Binary_Cta_Debug_Pass_Internals(p_object, &criticality_level_calibration);

      /*Bridging holding logic of cta*/
      Cta_Check_Stop_Level_Holding(&p_cta_instance->persistent, p_object, f_obj_in_zone);
   } /* End Check Object List Entry */
   else
   {
      /*Reset the objects persistent data except the object validity suppression counter. Otherwise the validity check as well as
       * the counter reset would cause CTA to end up in a deadlock.*/
      uint8_t temp_validity_suppression_counter = p_object->persistent->obj_validity_suppression_counter;
      Cta_Reset_Single_Obj_Persistent(&p_cta_instance->obj_persistent_array[p_object->tracker_data.id]);
      p_object->persistent->obj_validity_suppression_counter = temp_validity_suppression_counter;
   }
   Binary_Cta_Debug_Pass_Obj_Persistent_Data(p_object);
}


static void Cta_Algorithm(Cta_Instance_T *p_cta_instance,
                          Cta_Comparison_Data_T *p_cta_comparison_data,
                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const Fbk_Object_Data_T *p_object_data;
   Cta_Object_Data_T object;
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params;
   uint8_t obj_index;

   /* Asserts */
   assert(NULL != p_cta_instance);

   Cta_Calc_Host_Dep_Ext_Fac(&confl_zone_ext_params, &p_cta_instance->calibration, p_vehicle_data);

   /*Begin of CTA algorithm*/
   Cta_Reset_Obj_Attribute_Array(p_cta_instance->obj_attributes_array);
   for (obj_index = FBK_ZERO_UINT; obj_index < PA_OBJ_NUMBER_OF_OBJECTS; obj_index++)
   {
      p_object_data = &p_cta_instance->core_input.p_pa_data->object_data[obj_index];
      /*Init persistent data for new or invalid objects*/
      if ((FBK_ONE_UINT == p_object_data->age) || (PA_OBJ_STATUS_NEW == p_object_data->status)
          || (PA_OBJ_STATUS_INVALID == p_object_data->status))
      {
         Cta_Reset_Single_Obj_Persistent(&p_cta_instance->obj_persistent_array[p_object_data->id]);
      }

      if (((PA_OBJ_STATUS_COASTED == p_object_data->status) || (PA_OBJ_STATUS_MATURE == p_object_data->status))
          && (p_object_data->f_moveable))
      {
         Cta_Fill_Current_Object(&object, p_cta_instance, p_vehicle_data, obj_index);
         Cta_Check_Single_Object(p_cta_instance, &confl_zone_ext_params, p_cta_comparison_data, &object, p_vehicle_data);
      }
   }
   Binary_Cta_Debug_Pass_Object_With_Highest_Criticality(p_cta_comparison_data);
}


static void Cta_Check_If_Obj_Created_Near_Suppression_Line(const Cta_Object_Data_T *p_object,
                                                           const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                           const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_obj_is_created_near_suppr_line;

   f_obj_is_created_near_suppr_line = FBK_FALSE;

   if (Fbk_Is_True(p_object->persistent->f_prev_cta_alert_suppress)
       || (((0.5f * p_vehicle_data->host_width) > Fbk_Abs_F(p_object->tracker_data.vcs_pos.y))
           && (p_object->tracker_data.age < p_cta_cal->k_cta_age_for_new_creation_below_long_intersection)))
   {
      f_obj_is_created_near_suppr_line = FBK_TRUE;
   }
   p_object->persistent->f_prev_cta_alert_suppress = f_obj_is_created_near_suppr_line;
}


static void Cta_Flip_Zone(const uint8_t approach_side, Fbk_Field_Of_Interest_T *p_cta_zone)
{
   uint8_t index;

   /* Asserts */
   assert(NULL != p_cta_zone);

   if (FBK_SIDE_LEFT == approach_side)
   {
      for (index = FBK_ZERO_UINT; index < p_cta_zone->size; index++)
      {
         p_cta_zone->points[index].y = -p_cta_zone->points[index].y;
      }
   }
}

static void Cta_Get_Longitudinal_Intersection(const Cta_Object_Data_T *p_object,
                                              const Cta_Core_Calibration_T *p_cta_cal,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data,
                                              const Cta_Mode_T cta_mode)
{
   /* the intersection points of a target with x- and y-axis,
    * for the right side the front right edge should be taken and
    * for the left side the front left edge*/
   float32_T long_isect_point = CTA_HIGH_DEFAULT_VAL;
   float32_T lat_isect_point;
   float32_T heading_zero_point_trigonometric_function;
   uint8_t side_idx;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   for (side_idx = FBK_SIDE_LEFT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      if (Fbk_Is_True(p_cta_cal->k_cta_f_use_rel_vel_isect_point_calc))
      {
         /* Calculation with relative velocity*/
         long_isect_point = p_object->attributes->ref_point_candidate[side_idx].point.x
                            + (p_object->attributes->ttc * p_object->attributes->relative_velocity.x);
      }
      else
      {
         /* Calculation with heading, which can be influenced by path information. This can be specific to the project.*/
         heading_zero_point_trigonometric_function = Normalize_Angle(p_object->attributes->CTA_heading, FBK_ZERO_F);

         if (((Fbk_Abs_F(heading_zero_point_trigonometric_function) >= EPSILON) && /*Check Zero Points sine*/
              (Fbk_Abs_F(heading_zero_point_trigonometric_function - PI) >= EPSILON)
              && (Fbk_Abs_F(heading_zero_point_trigonometric_function + PI) >= EPSILON)))
         {
            lat_isect_point = p_object->attributes->ref_point_candidate[side_idx].point.y
                              - (Fbk_Convert_Obj_Side_To_Sign(side_idx) * Fbk_Half(p_vehicle_data->host_width)
                                 * p_cta_cal->k_cta_intersection_line_host_width_percentage);

            long_isect_point =
               p_object->attributes->ref_point_candidate[side_idx].point.x
               - (lat_isect_point * (Fast_Cos(p_object->attributes->CTA_heading) / Fast_Sin(p_object->attributes->CTA_heading)));
         }
      }
      /*Set the longitudinal intersection point at first. This is not heading compensated here. */
      p_object->attributes->long_isect_point_candidate[side_idx][cta_mode] = long_isect_point;
   }

   /* intersection point is adapted by a value depending on target heading and ego width -> leads to higher feature
    * sensitivity */
   if (Fbk_Is_True(p_cta_cal->k_cta_f_apply_heading_compensation_on_intersection_point))
   {
      /*Internally compensate for the object heading in a specific mode*/
      Cta_Adapt_Intersec_Point_To_Object_Heading(p_object->attributes, p_vehicle_data, p_cta_cal, cta_mode);
   }
}

static boolean_T Cta_Do_Obj_Corners_Cross_Long_Axis(const Cta_Object_Data_T *p_object,
                                                    const Fbk_Object_Corners_T *p_target_corners,
                                                    const Cta_Core_Calibration_T *p_cta_cal,
                                                    const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_rear_corner_crossed_long_axis = FBK_FALSE;
   Fbk_Reference_Position_T right_corner;
   Fbk_Reference_Position_T left_corner;
   float32_T distance_ths;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_target_corners);
   assert(NULL != p_cta_cal);

   distance_ths = p_cta_cal->k_cta_dist_thres_crit_level_reset * p_vehicle_data->host_width;

   /*Choose corners for further analysis.*/
   if (Fbk_Is_True(p_cta_cal->k_cta_f_use_front_corners_dist_stop))
   {
      right_corner = FBK_FRONT_RIGHT_CORNER;
      left_corner  = FBK_FRONT_LEFT_CORNER;
   }
   else
   {
      right_corner = FBK_REAR_RIGHT_CORNER;
      left_corner  = FBK_REAR_LEFT_CORNER;
   }

   /*Check whether both rear corners are exceeding user defined longitudinal threshold*/
   if (FBK_SIDE_LEFT == p_object->attributes->approach_side)
   {
      if ((p_target_corners->points[right_corner].y > -distance_ths) && (p_target_corners->points[left_corner].y > -distance_ths))
      {
         f_rear_corner_crossed_long_axis = FBK_TRUE;
      }
   }
   else if (FBK_SIDE_RIGHT == p_object->attributes->approach_side)
   {
      if ((p_target_corners->points[right_corner].y < distance_ths) && (p_target_corners->points[left_corner].y < distance_ths))
      {
         f_rear_corner_crossed_long_axis = FBK_TRUE;
      }
   }
   else
   {
      /*Do nothing*/
   }

   return f_rear_corner_crossed_long_axis;
}


static boolean_T Cta_Shall_Criticality_Level_Be_Reset(const Cta_Core_Calibration_T *p_cta_cal,
                                                      const Cta_Object_Data_T *p_object,
                                                      const Fbk_Object_Corners_T *p_target_object_corners,
                                                      const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_critically_level_reset = FBK_FALSE;
   boolean_T f_dist_thr;
   /* Asserts */
   assert(NULL != p_cta_cal);
   assert(NULL != p_object);
   assert(NULL != p_target_object_corners);

   f_dist_thr = (boolean_T) (Fbk_Is_True(p_cta_cal->k_cta_f_enable_thres_crit_level_reset)
                             && (Cta_Do_Obj_Corners_Cross_Long_Axis(p_object, p_target_object_corners, p_cta_cal, p_vehicle_data)));

   if (p_object->attributes->f_stop_time_below_ths || f_dist_thr)
   {
      f_critically_level_reset = FBK_TRUE;
   }

   return f_critically_level_reset;
}

static void Cta_Get_Relative_Velocity_Of_Object(const Cta_Object_Data_T *p_object,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                const Cta_Core_Calibration_T *p_cta_cal)
{
   Vector_2d_T obj_relative_velocity;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cta_cal);

   /*% compute projection vector */
   if (Fbk_Is_True(p_cta_cal->k_cta_f_use_heading_for_relative_velocity_calculation)
       || (p_object->tracker_data.speed < p_cta_cal->k_cta_speed_thresh_for_rel_vel_calc))
   {
      obj_relative_velocity.x =
         (p_object->tracker_data.speed * Fast_Cos(p_object->attributes->CTA_heading)) - p_vehicle_data->host_speed;
      obj_relative_velocity.y = (p_object->tracker_data.speed * Fast_Sin(p_object->attributes->CTA_heading));
   }
   else
   {
      obj_relative_velocity.x = p_object->tracker_data.vcs_vel_rel.x;
      obj_relative_velocity.y = p_object->tracker_data.vcs_vel_rel.y;
   }

   p_object->attributes->relative_velocity = obj_relative_velocity;
}

static void Cta_Set_Core_Output(Cta_Instance_T *p_cta_instance,
                                const Cta_Comparison_Data_T *p_cta_comparison_data,
                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Cta_Status_T cta_status)
{
   const Cta_Object_Data_T *p_most_critical_obj;
   uint8_t side_idx;
   uint8_t cta_mode_idx;

   /* Asserts */
   assert(NULL != p_cta_instance);

   Cta_Reset_Core_Output(&p_cta_instance->core_output);

   /* Set enable flag */
   p_cta_instance->core_output.cta_status = cta_status;

   for (cta_mode_idx = FBK_ZERO_UINT; cta_mode_idx < (uint8_t) CTA_NUM_MODES; cta_mode_idx++)
   {
      /*Only fill information based on the activated state. */
      if (Fbk_Is_True(p_cta_instance->calibration.k_cta_enable_modes[cta_mode_idx]))
      {
         for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
         {
            p_most_critical_obj = &p_cta_comparison_data->object_with_highest_crit[cta_mode_idx][side_idx];

            /*Set Tracker information*/
            if ((NULL != p_most_critical_obj->attributes) && (PA_INVALID_OBJ_ID != p_most_critical_obj->tracker_data.id))
            {
               p_cta_instance->core_output.cta_id[cta_mode_idx][side_idx]        = p_most_critical_obj->tracker_data.id;
               p_cta_instance->core_output.cta_unique_id[cta_mode_idx][side_idx] = p_most_critical_obj->tracker_data.unique_id;
               p_cta_instance->core_output.cta_index[cta_mode_idx][side_idx]     = p_most_critical_obj->tracker_data.index;
            }

            /* Set object attributes which are passed to the customer adapter */
            if (NULL != p_most_critical_obj->attributes)
            {
               p_cta_instance->core_output.cta_obj_ttc[cta_mode_idx][side_idx] = p_most_critical_obj->attributes->ttc;
               p_cta_instance->core_output.cta_obj_ttp[cta_mode_idx][side_idx] = p_most_critical_obj->attributes->ttp;
               p_cta_instance->core_output.cta_long_intersection[cta_mode_idx][side_idx] =
                  p_most_critical_obj->attributes->long_isect_point[cta_mode_idx];
               p_cta_instance->core_output.cta_heading[cta_mode_idx][side_idx] = p_most_critical_obj->attributes->CTA_heading;
            }

            /* Set alert levels */
            // clang-format off
            p_cta_instance->core_output.cta_alert_level[cta_mode_idx][side_idx] =
               Cta_Process_Current_Alert_Level(&p_cta_instance->persistent, p_cta_comparison_data->max_level[cta_mode_idx][side_idx],
            /* coverity[misra_c_2012_rule_10_5_violation][Intentional casting from integer type to enum type] */
                                               (Cta_Mode_T) cta_mode_idx, side_idx, p_most_critical_obj, &p_cta_instance->calibration);
            // clang-format on
            /* Set CTB qualifier */
            if (Fbk_Is_True(p_cta_instance->calibration.k_cta_enable_ctb))
            {
               p_cta_instance->core_output.f_brake_qualifier[cta_mode_idx][side_idx] = Cta_Shall_Brake_Qualifier_Be_Set(
                  &(p_cta_instance->persistent.brake_holding_counter[cta_mode_idx][side_idx]),
                  &(p_cta_instance->persistent.brake_suppression_counter[cta_mode_idx][side_idx]),
                  &(p_cta_instance->persistent.previous_brake_qualifier[cta_mode_idx][side_idx]), p_most_critical_obj->attributes,
                  /* coverity[misra_c_2012_rule_10_5_violation][Intentional casting from integer type to enum type] */
                  p_cta_instance, side_idx, (Cta_Mode_T) cta_mode_idx, p_vehicle_data);

               if (NULL != p_most_critical_obj->attributes)
               {
                  p_cta_instance->core_output.f_standstill_qualifier[cta_mode_idx][side_idx] =
                     p_most_critical_obj->attributes->f_standstill_qualifier;
                  p_cta_instance->core_output.brake_deceleration[cta_mode_idx][side_idx] =
                     p_most_critical_obj->attributes->brake_deceleration;
               }
            }

            /* Set counters from persitent data  */
            p_cta_instance->core_output.cta_warn_hold_cnt[cta_mode_idx][side_idx] =
               p_cta_instance->persistent.warning_holding_counter[cta_mode_idx][side_idx];
            p_cta_instance->core_output.cta_brake_hold_cnt[cta_mode_idx][side_idx] =
               p_cta_instance->persistent.brake_holding_counter[cta_mode_idx][side_idx];
            p_cta_instance->core_output.cta_brake_supp_cnt[cta_mode_idx][side_idx] =
               p_cta_instance->persistent.brake_suppression_counter[cta_mode_idx][side_idx];
         }
      }
   }
}

static Cta_Status_T Cta_Get_Status(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                   const Cta_Core_Calibration_T *p_cta_cal,
                                   const Cta_Core_Input_T *p_cta_core_input)
{
   Cta_Status_T cta_status;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cta_cal);
   assert(NULL != p_cta_core_input);

   if (Fbk_Is_False(p_cta_core_input->f_cta_switch))
   {
      cta_status = CTA_STATUS_DISABLED;
   }
   else if (Fbk_Abs_F(p_vehicle_data->host_speed) > p_cta_cal->k_cta_ego_abs_speed_max)
   {
      cta_status = CTA_STATUS_DEACTIVATED_EGO_SPEED;
   }
   else
   {
      cta_status = CTA_STATUS_ACTIVE;
   }

   return cta_status;
}

static void Cta_Get_Reference_Point_And_Fill_Properties(const Cta_Object_Data_T *p_object,
                                                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                        const Fbk_Object_Corners_T *p_target_corners,
                                                        const Cta_Core_Calibration_T *p_cta_cal)
{
   float32_T min_ttc                    = FBK_INVALID_TIME;
   float32_T max_ttp                    = -FBK_INVALID_TIME;
   Fbk_Reference_Position_T idx_max_ttp = FBK_NUM_OF_OBJECT_CORNERS;
   uint8_t idx;
   float32_T time_to_intersect;
   float32_T side_shift;
   float32_T fabs_y_vel;


   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_target_corners);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cta_cal);

   p_object->attributes->ref_point_candidate[FBK_SIDE_LEFT].point            = p_target_corners->points[FBK_FRONT_LEFT_CORNER];
   p_object->attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index  = FBK_FRONT_LEFT_CORNER;
   p_object->attributes->ref_point_candidate[FBK_SIDE_RIGHT].point           = p_target_corners->points[FBK_FRONT_RIGHT_CORNER];
   p_object->attributes->ref_point_candidate[FBK_SIDE_RIGHT].ref_point_index = FBK_FRONT_RIGHT_CORNER;
   p_object->attributes->ref_point.point                                     = p_target_corners->points[FBK_FRONT_MID];
   p_object->attributes->ref_point.ref_point_index                           = FBK_FRONT_MID;

   fabs_y_vel = Fbk_Abs_F(p_object->attributes->relative_velocity.y);
   if (fabs_y_vel > EPSILON)
   {
      for (idx = (uint8_t) FBK_FRONT_LEFT_CORNER; idx < (uint8_t) FBK_NUM_OF_OBJECT_CORNERS; idx++)
      {
         time_to_intersect = (-p_target_corners->points[idx].y) / p_object->attributes->relative_velocity.y;
         if ((min_ttc > time_to_intersect) && (time_to_intersect > p_cta_cal->k_cta_ttc_calc_positive_ref_point))
         {
            min_ttc = time_to_intersect;
         }
         if (time_to_intersect > max_ttp)
         {
            max_ttp = time_to_intersect;
            /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast back to enum type] */
            idx_max_ttp = (Fbk_Reference_Position_T) idx;
         }
      }
      /* Set the TTP reference point and TTP value*/
      p_object->attributes->ref_point_ttp.ref_point_index = idx_max_ttp;
      p_object->attributes->ref_point_ttp.point           = p_target_corners->points[idx_max_ttp];
      side_shift = (p_cta_cal->k_cta_f_calc_ttp_ego_side_enabled) ? Fbk_Half(p_vehicle_data->host_width / fabs_y_vel) : FBK_ZERO_F;
      p_object->attributes->ttp = max_ttp - side_shift;

      /* Set the TTC reference point and TTC value*/
      side_shift = (p_cta_cal->k_cta_f_calc_ttc_ego_side_enabled) ? Fbk_Half(p_vehicle_data->host_width / fabs_y_vel) : FBK_ZERO_F;
      p_object->attributes->ttc = min_ttc - side_shift;
   }
   else
   {
      /* only theoretical case, should never be used because of heading and speed limits*/
      p_object->attributes->ref_point_ttp.ref_point_index = FBK_REAR_MID;
      p_object->attributes->ref_point_ttp.point           = p_target_corners->points[FBK_REAR_MID];
      p_object->attributes->ttp                           = CTA_HIGH_DEFAULT_VAL;
      p_object->attributes->ttc                           = CTA_HIGH_DEFAULT_VAL;
   }
}
