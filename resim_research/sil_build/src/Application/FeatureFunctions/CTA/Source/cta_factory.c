/**
 * @file cta_factory.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains Initialization procedures of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_factory.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pt_output_t.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief This function resets the attributes of the target.
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3812}
 * @verification{Check that the targets attributes are reset.}
 */
static void
Cta_Reset_Target_Attributes(Cta_Object_Attributes_T *p_target_attributes /**< target attributes array for each object*/);

/**
 * @brief This function determines the side of the considered CTA scenario which can either be
 * FBK_SIDE_LEFT or FBK_SIDE_RIGHT with help of given heading.
 *
 * @return void
 *
 * @SRS{SF-187}
 * @SAE{SF-2459}
 * @SDD{SF-3808}
 * @verification{Check that the objects side is calculated correctly.}
 */
static void Cta_Calculate_Side(const Cta_Object_Data_T *p_cta_object /**< cta object*/);

/**
 * @brief Used to check if the approach side from the object has changed from
 * left to right or from right to left. This indicates, that a change of movement direction
 * has occured. In that case the persistent variables shall be reset.
 *
 * @return True if a movement direction change has occured
 *
 * @SRS{SF-187}
 * @SAE{SF-2459}
 * @SDD{SF-3810}
 * @verification{Check that an approach side change is detected correctly.}
 */
static boolean_T Cta_Has_Approach_Side_Changed(const Cta_Object_Data_T *p_cta_object /**< CTA object data */);

/**
 * @brief Used when PT is disabled. Used to calculate average headings and
 * taking into account heading from previous cycles. Used to reduce tracker inaccuracy.
 * If the conditions are not met, it returns the heading from the tracker
 *
 * @return heading value
 *
 * @SRS{SF-187}
 * @SAE{SF-2459}
 * @SDD{SF-4059}
 * @verification{Check that an heading is calculated correctly.}
 */
static float32_T Cta_Heading_Filter(const Cta_Object_Data_T *p_cta_object /**< CTA object data */,
                                    const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Preprocesses persistent data of object struct and also provides
 * Cta with Path heading if available.
 *
 * @return void
 *
 * @SRS{SF-187}
 * @SAE{SF-2459}
 * @SDD{SF-3813}
 * @verification{Check that the CTA object data is processed correctly.}
 */
static void Cta_Update_Object_Data(const Cta_Object_Data_T *p_cta_object_to_be_filled /**< object to be modified*/,
                                   const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                   const Cta_Core_Input_T *p_cta_core_input /**< cta core input*/,
                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);

/**
 * @brief Resets the persistent variables.
 *
 * @return void
 *
 * @SRS{SF-187}
 * @SAE{SF-2459}
 * @SDD{SF-3811}
 * @verification{Check that the persistent variables are reset.}
 */
static void Cta_Reset_Object_Persistent(Cta_Object_Persistent_T *p_object_persistent /**< persistent values*/);


/**
 * @brief Initializes the data to which p_cta_object_data points to and sets its internal pointers
 * to NULL.
 *
 * @return void
 *
 * @SRS{SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3821}
 * @verification{Check that the object data is reset.}
 */
static void Cta_Init_Object(Cta_Object_Data_T *p_cta_object_data /**<object data*/);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/
void Cta_Fill_Current_Object(Cta_Object_Data_T *p_cta_object_to_be_filled,
                             Cta_Instance_T *p_cta_instance,
                             const Fbk_Vehicle_Data_T *p_vehicle_data,
                             const uint8_t obj_index)
{
   const Fbk_Object_Data_T *p_tracker_data;
   /* Asserts */
   assert(NULL != p_cta_object_to_be_filled);
   assert(NULL != p_cta_instance);
   assert(obj_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /*Get basic structs for Cta object*/
   p_tracker_data                          = &p_cta_instance->core_input.p_pa_data->object_data[obj_index];
   p_cta_object_to_be_filled->tracker_data = *p_tracker_data;

   p_cta_object_to_be_filled->persistent = &(p_cta_instance->obj_persistent_array[p_tracker_data->id]);
   p_cta_object_to_be_filled->attributes = &(p_cta_instance->obj_attributes_array[p_tracker_data->id]);

   /*Update parameters of Cta object*/
   Cta_Update_Object_Data(p_cta_object_to_be_filled, &p_cta_instance->calibration, &p_cta_instance->core_input, p_vehicle_data);
}

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Init_Comparison_Data(Cta_Comparison_Data_T *p_cta_comparison_data, Cta_Instance_T *p_cta_instance)
{
   uint8_t side_idx;
   uint8_t mode_idx;

   /* Initialize comparison data of CTA for all different modes and sides. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
      {
         Cta_Init_Object(&p_cta_comparison_data->object_with_highest_crit[mode_idx][side_idx]);
         p_cta_comparison_data->max_level[mode_idx][side_idx] = CTA_CRIT_LEVEL_NONE;
         /*Assign a memory address to tracker data on respective side*/
         p_cta_comparison_data->object_with_highest_crit[mode_idx][side_idx].tracker_data =
            p_cta_instance->cta_obj_tracker_high_crit[mode_idx][side_idx];
      }
   }
}

void Cta_Reset_Obj_Attribute_Array(Cta_Object_Attributes_T obj_attributes_array[PA_OBJ_NUMBER_OF_OBJECTS])
{
   uint8_t i;

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      Cta_Reset_Target_Attributes(&(obj_attributes_array[i]));
   }
}


void Cta_Reset_Obj_Persistent_Array(Cta_Object_Persistent_T obj_persistent_array[CTA_OBJ_MAX_ARRAY_SIZE])
{
   uint8_t i;

   for (i = 0; i < CTA_OBJ_MAX_ARRAY_SIZE; i++)
   {
      Cta_Reset_Object_Persistent(&obj_persistent_array[i]);
   }
}

void Cta_Reset_Single_Obj_Persistent(Cta_Object_Persistent_T *p_obj_persistent)
{
   Cta_Reset_Object_Persistent(p_obj_persistent);
}


void Cta_Init_Crit_Level_Cals(Cta_Crit_Level_Calibration_T *p_cta_crit_level_cals,
                              const Cta_Core_Input_T *p_cta_core_input,
                              const Cta_Core_Calibration_T *p_cta_cal,
                              const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t crit_level_idx;
   uint8_t cta_mode_idx;

   for (crit_level_idx = FBK_ZERO_UINT; crit_level_idx < CTA_NUM_CRIT_LEVEL; crit_level_idx++)
   {
      /* In case of RCTA add a transformation to the mid of the rear bumper such that calibrations can be used more easily in
       * application state.*/
      p_cta_crit_level_cals->max_long_point_criticality_level[CTA_MODE_REAR][crit_level_idx] =
         -p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_REAR][crit_level_idx] - p_vehicle_data->host_length;
      p_cta_crit_level_cals->min_long_point_criticality_level[CTA_MODE_REAR][crit_level_idx] =
         -p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][crit_level_idx] - p_vehicle_data->host_length;


      /* In case of FCTA the reference point is the same as in Vcs, thus only mapping is needed. */
      p_cta_crit_level_cals->max_long_point_criticality_level[CTA_MODE_FRONT][crit_level_idx] =
         p_cta_cal->k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][crit_level_idx];
      p_cta_crit_level_cals->min_long_point_criticality_level[CTA_MODE_FRONT][crit_level_idx] =
         p_cta_cal->k_cta_min_long_point_criticality_level[CTA_MODE_FRONT][crit_level_idx];

      /* Map ttc from input. Set speed thresholds */
      for (cta_mode_idx = FBK_ZERO_UINT; cta_mode_idx < (uint8_t) CTA_NUM_MODES; cta_mode_idx++)
      {
         p_cta_crit_level_cals->ttc_criticality_level[cta_mode_idx][crit_level_idx] =
            p_cta_core_input->ttc_criticality_level[cta_mode_idx][crit_level_idx];
         p_cta_crit_level_cals->speed_criticality_level[cta_mode_idx][crit_level_idx] =
            p_cta_cal->k_cta_speed_criticality_level[cta_mode_idx][crit_level_idx];
      }
   }

   p_cta_crit_level_cals->mature_cycles     = p_cta_cal->k_cta_min_mature_cycles_level_qualifiction;
   p_cta_crit_level_cals->max_eclipse_value = p_cta_cal->k_cta_max_object_eclipse_for_level_qualification;
   p_cta_crit_level_cals->minimum_age       = p_cta_cal->k_cta_min_object_age_thres;
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Cta_Init_Object(Cta_Object_Data_T *p_cta_object_data)
{
   /* Assert */
   assert(NULL != p_cta_object_data);

   p_cta_object_data->attributes = NULL;
   p_cta_object_data->persistent = NULL;
   Fbk_Reset_Object_Data(&(p_cta_object_data->tracker_data));
}

static void Cta_Reset_Target_Attributes(Cta_Object_Attributes_T *p_target_attributes)
{
   /* Assert */
   assert(NULL != p_target_attributes);

   p_target_attributes->approach_side                                              = FBK_SIDE_UNDEFINED;
   p_target_attributes->ttc                                                        = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->ttp                                                        = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->f_stop_time_below_ths                                      = FBK_FALSE;
   p_target_attributes->long_isect_point[CTA_MODE_REAR]                            = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->long_isect_point[CTA_MODE_FRONT]                           = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->long_isect_point_candidate[FBK_SIDE_LEFT][CTA_MODE_FRONT]  = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][CTA_MODE_FRONT] = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->long_isect_point_candidate[FBK_SIDE_LEFT][CTA_MODE_REAR]   = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][CTA_MODE_REAR]  = CTA_HIGH_DEFAULT_VAL;
   p_target_attributes->CTA_heading                                                = FBK_ZERO_F;
   p_target_attributes->relative_velocity                                          = Create_2d_Vector_Origin();
   p_target_attributes->ref_point_candidate[FBK_SIDE_LEFT].point                   = Create_2d_Vector_Origin();
   p_target_attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index         = FBK_NUM_OF_OBJECT_CORNERS;
   p_target_attributes->ref_point_candidate[FBK_SIDE_RIGHT].point                  = Create_2d_Vector_Origin();
   p_target_attributes->ref_point_candidate[FBK_SIDE_RIGHT].ref_point_index        = FBK_NUM_OF_OBJECT_CORNERS;
   p_target_attributes->ref_point.point                                            = Create_2d_Vector_Origin();
   p_target_attributes->ref_point.ref_point_index                                  = FBK_NUM_OF_OBJECT_CORNERS;
   p_target_attributes->ref_point_ttp.point                                        = Create_2d_Vector_Origin();
   p_target_attributes->ref_point_ttp.ref_point_index                              = FBK_NUM_OF_OBJECT_CORNERS;
   p_target_attributes->p_pt_match_info                                            = NULL;
   p_target_attributes->p_pt_nearest_path_info                                     = NULL;
   p_target_attributes->brake_deceleration                                         = FBK_ZERO_F;
   p_target_attributes->f_standstill_qualifier                                     = FBK_FALSE;
}

static void Cta_Reset_Object_Persistent(Cta_Object_Persistent_T *p_object_persistent)
{
   uint8_t level_idx;
   uint8_t mode_idx;
   /* Assert */
   assert(NULL != p_object_persistent);


   p_object_persistent->prev_approach_side               = FBK_SIDE_UNDEFINED;
   p_object_persistent->obj_validity_suppression_counter = FBK_ZERO_UINT;

   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         p_object_persistent->crit_level_suppression_counter[mode_idx][level_idx] = FBK_ZERO_UINT;
      }
      p_object_persistent->prev_cycle_crit_level[mode_idx] = (uint8_t) CTA_CRIT_LEVEL_NONE;
      p_object_persistent->f_prev_cta_alert_suppress       = FBK_FALSE;
      p_object_persistent->n_alert_cycles[mode_idx]        = FBK_ZERO_UINT;
   }
   p_object_persistent->cta_object_heading = FBK_ZERO_F;
}

static float32_T Cta_Heading_Filter(const Cta_Object_Data_T *p_cta_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   float32_T heading = p_cta_object->tracker_data.vcs_heading;

   /* Assert */
   assert(NULL != p_cta_object);
   assert(NULL != p_cta_cal);

   /* Enabled only if the object has already triggered an alert. Enabled only for rear mode */
   if (Fbk_Is_True(p_cta_cal->k_cta_f_enable_heading_exp_moving_average)
       && (p_cta_object->persistent->prev_cycle_crit_level[CTA_MODE_REAR] > (uint8_t) CTA_CRIT_LEVEL_NONE))
   {
      /* Calculate exponential moving average for object heading */
      heading =
         (p_cta_cal->k_cta_object_heading_exp_moving_average_alpha * p_cta_object->tracker_data.vcs_heading)
         + ((FBK_ONE_F - p_cta_cal->k_cta_object_heading_exp_moving_average_alpha) * p_cta_object->persistent->cta_object_heading);
   }

   return heading;
}

static void Cta_Update_Object_Data(const Cta_Object_Data_T *p_cta_object_to_be_filled,
                                   const Cta_Core_Calibration_T *p_cta_cal,
                                   const Cta_Core_Input_T *p_cta_core_input,
                                   const Fbk_Vehicle_Data_T *p_vehicle_data)
{

   /* Asserts */
   assert(NULL != p_cta_object_to_be_filled);
   assert(NULL != p_cta_cal);

   /*Check if Path Tracking is available and if path is matched to respective object.
    * If thats the case use path_heading as Cta_heading*/
   if ((NULL != p_cta_core_input->p_pt_output) && (p_cta_cal->k_cta_f_apply_path_tracking))
   {
      p_cta_object_to_be_filled->attributes->p_pt_match_info =
         &(p_cta_core_input->p_pt_output->path_obj_pair_output[p_cta_object_to_be_filled->tracker_data.index]);
      p_cta_object_to_be_filled->attributes->p_pt_nearest_path_info =
         &(p_cta_core_input->p_pt_output->nearest_path_output[p_cta_object_to_be_filled->tracker_data.index]);
   }
   else
   {
      p_cta_object_to_be_filled->attributes->p_pt_match_info        = NULL;
      p_cta_object_to_be_filled->attributes->p_pt_nearest_path_info = NULL;
   }

   if ((Fbk_Is_True(p_cta_cal->k_cta_f_apply_path_tracking)) && (NULL != p_cta_object_to_be_filled->attributes->p_pt_match_info))
   {
      if (PT_DEFAULT_MATCH_INDEX != p_cta_object_to_be_filled->attributes->p_pt_match_info->track_match)
      {
         /* Check whether path tracking shall be used when we are moving backwards. This shall be mitigated when min host speed and
          * when object is within boundaries.*/
         if (Fbk_Is_True(p_cta_cal->k_cta_f_discard_pt_heading_when_moving)
             && (Fbk_Abs_F(p_vehicle_data->host_speed) >= p_cta_cal->k_cta_min_host_speed_to_discard_pt_info)
             && (Fbk_Abs_F(p_cta_object_to_be_filled->tracker_data.vcs_pos.y) <= p_cta_cal->k_cta_obj_dist_to_discard_pt_info))
         {
            p_cta_object_to_be_filled->attributes->CTA_heading = p_cta_object_to_be_filled->tracker_data.vcs_heading;
         }
         else
         {
            p_cta_object_to_be_filled->attributes->CTA_heading = p_cta_object_to_be_filled->attributes->p_pt_match_info->path_heading;
         }
      }
      else
      {
         p_cta_object_to_be_filled->attributes->CTA_heading = Cta_Heading_Filter(p_cta_object_to_be_filled, p_cta_cal);
      }
   }
   else
   {
      p_cta_object_to_be_filled->attributes->CTA_heading = Cta_Heading_Filter(p_cta_object_to_be_filled, p_cta_cal);
   }

   p_cta_object_to_be_filled->persistent->cta_object_heading = p_cta_object_to_be_filled->attributes->CTA_heading;

   /*Check whether an object has made a U-Turn maneuver. If thats the case,
    * the persistent object data shall be reset. Keep track of the approach side of an object*/
   Cta_Calculate_Side(p_cta_object_to_be_filled);
   if (Cta_Has_Approach_Side_Changed(p_cta_object_to_be_filled))
   {
      Cta_Reset_Object_Persistent(p_cta_object_to_be_filled->persistent);
   }
   p_cta_object_to_be_filled->persistent->prev_approach_side = p_cta_object_to_be_filled->attributes->approach_side;
}

static void Cta_Calculate_Side(const Cta_Object_Data_T *p_cta_object)
{
   /* Assert */
   assert(NULL != p_cta_object);

   if (p_cta_object->tracker_data.vcs_heading > FBK_ZERO_F)
   {
      p_cta_object->attributes->approach_side = FBK_SIDE_LEFT;
   }
   else
   {
      p_cta_object->attributes->approach_side = FBK_SIDE_RIGHT;
   }
}


static boolean_T Cta_Has_Approach_Side_Changed(const Cta_Object_Data_T *p_cta_object)
{
   boolean_T f_has_approach_side_changed = FBK_FALSE;

   /* Assert */
   assert(NULL != p_cta_object);

   if ((FBK_SIDE_UNDEFINED != p_cta_object->persistent->prev_approach_side)
       && (FBK_SIDE_UNDEFINED != p_cta_object->attributes->approach_side)
       && (((FBK_SIDE_LEFT == p_cta_object->attributes->approach_side)
            && (FBK_SIDE_RIGHT == p_cta_object->persistent->prev_approach_side))
           || ((FBK_SIDE_RIGHT == p_cta_object->attributes->approach_side)
               && (FBK_SIDE_LEFT == p_cta_object->persistent->prev_approach_side))))
   {
      f_has_approach_side_changed = FBK_TRUE;
   }

   return f_has_approach_side_changed;
}
