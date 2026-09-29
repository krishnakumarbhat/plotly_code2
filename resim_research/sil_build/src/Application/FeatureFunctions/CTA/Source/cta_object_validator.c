
/**
 * @file cta_object_validator.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Function definition of object validation module of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_object_validator.h"
#include "cta_debug_interface.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
#include <assert.h>

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

/**
 * @brief Summarizes whether a hysteresis within validity check shall be applied depending on whether a
 * criticality level other than the default one has been reached in the previous cycle
 * @SDD{SF-3851}
 */
typedef enum
{
   HYST_LEVEL_NONE_PREV_WARN   = (0) /**<indicates that no hysteresis in validity check shall be applied*/,
   HYST_LEVEL_ACTIVE_PREV_WARN = (1) /**<indicates that hysteresis shall be applied for validity check*/
} Hysteresis_Level_T;

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief This struct summarizes the criteria which an object needs to fulfill in order to be a valid
 * Cross Traffic Alert candidate. On the criteria listed here also an hysteresis is applied.
 *
 *
 * @SDD{SF-3689}
 */
typedef struct
{
   Float_Range_T cta_heading_range;          /**< heading range in which the heading of an objects needs to be in*/
   Float_Range_T cta_heading_variance_range; /**< heading variance range in which the heading of an objects needs to be in*/
   Float_Range_T cta_speed_range;            /**< speed range in which the speed of an objects needs to be in*/
   float32_T min_lateral_approach_speed_obj; /**< minimum lateral speed an object needs to have*/
   float32_T min_existence_probability_obj;  /**< minimum existence probability an object needs to have*/
} CTA_Obj_Valid_Crit_T;

/*===========================================================================*\
* Local Functions Declaration
\*===========================================================================*/

/**
 * @brief Checks if the object is a reflection.
 * This check is optional and can be enabled/disabled by cal.
 *
 * @return True if object is a reflection
 *
 * @SRS{SF-199,SF-213}
 * @SAE{SF-2459}
 * @SDD{SF-3864}
 * @verification{Check that this function correctly detects the reflection state of the provided object.}
 */
static boolean_T Cta_Is_Object_A_Reflection(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                            const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);


/**
 * @brief Checks if coasting occurs since a certain amount of cycles. The object is not trustable
 * for CTA scenarios, if that is the case.
 *
 * @return True if a coasted object exceeds a threshold considering its stage age within coasting state
 *
 * @SRS{SF-200}
 * @SAE{SF-2459}
 * @SDD{SF-3858}
 * @verification{Check that this function correctly detects that the provided object is below the allowed number of coasting
 * cycles.}
 */
static boolean_T Cta_Is_Coasted_Obj_Below_Ignore_Cycles(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                                        const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Checks if the age of object exceeds a certain threshold.
 * This check is optional and can be enabled/disabled by cal.
 *
 * @return True if object age is above a specified threshold
 *
 * @SRS{SF-202}
 * @SAE{SF-2459}
 * @SDD{SF-3865}
 * @verification{Check that this function correctly detects that the provided object is above the allowed age.}
 */
static boolean_T Cta_Is_Object_Age_Above_Threshold(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                                   const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Compares the speed of target with a float range and returns FBK_TRUE if the target speed is
 * within that range
 *
 * @return True if objects speed is within a given range
 *
 * @SRS{SF-203}
 * @SAE{SF-2459}
 * @SDD{SF-3867}
 * @verification{Check that this function correctly detects that the provided object is in the allowed speed range.}
 */
static boolean_T Cta_Is_Object_Speed_In_Allowed_Range(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                                      const Float_Range_T *p_cta_speed_range /**< range of speed values*/);

/**
 * @brief Checks if the object heading and heading variance are within a certain range, so that the heading is relevant for CTA
 * scenarios.
 *
 * @return True if object heading and heading variance are inside allowed range
 *
 * @SRS{SF-201,SF-132}
 * @SAE{SF-2459}
 * @SDD{SF-3866}
 * @verification{Check that this function correctly detects that the provided object is in the allowed heading range.}
 */
static boolean_T
Cta_Is_Object_Heading_Inside_Allowed_Range(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                           const Float_Range_T *p_cta_heading_range /**< given heading range*/,
                                           const Float_Range_T *p_cta_heading_variance_range /**< given heading variance range*/);

/**
 * @brief Checks if the lateral relative velocity exceeds a threshold, so that the object
 * is relevant for CTA scenarios.
 *
 * @return True if the lateral object velocity is above a given threshold
 *
 * @SRS{SF-204}
 * @SAE{SF-2459}
 * @SDD{SF-3859}
 * @verification{Check that this function correctly detects that the provided object is above the velocity threshold.}
 */
static boolean_T
Cta_Is_Lat_Obj_Vel_Above_Threshold(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                   const float32_T min_lateral_approach_speed_obj /**<minimum required lateral speed*/);

/**
 * @brief Checks if the existence probability exceeds a threshold so that the object is more trusted.
 *
 * @return FBK_TRUE if objects existence probability is above threshold
 *
 * @SRS{SF-205}
 * @SAE{SF-2459}
 * @SDD{SF-3862}
 * @verification{Check that this function correctly detects that the provided object is above the existence probability threshold.}
 */
static boolean_T
Cta_Is_Obj_Exist_Prob_Above_Threshold(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                      const float32_T min_existence_probability_obj /**< minimum required existence prob*/);

/**
 * @brief Shall update the object delay counter, which is needed to suppress
 * warnings for some cycles. When an object is matched to a path, the objects whose approach side match with the path
 * direction are not suppressed.
 *
 * @return void
 *
 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3714}
 * @verification{Check that the objects delay counter is updated correctly.}
 */
static void
Cta_Update_Validity_Suppression_Ctr(const Cta_Object_Data_T *p_object /**<  Object whose delay counter should be updated */,
                                    const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);


/**
 * @brief Checks if the suppression counter exceeds a certain threshold. If that is the case
 * then the object is more trustable for CTA scenarios.
 * This check is optional and can be enabled/disabled by cal.
 *
 * @return True if the suppression counter exceeds a given threshold
 *
 * @SRS{SF-206}
 * @SAE{SF-2459}
 * @SDD{SF-3854}
 * @verification{Check that this function correctly detects that the suppression counter exceeds the threshold.}
 */
static boolean_T Cta_Does_Suppr_Counter_Exceed_Threshold(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                                         const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Returns true if the obstruction probability is below a threshold.
 * This check is optional and can be enabled/disabled by cal.
 *
 * @return True if obstruction probability is below threshold
 *
 * @SRS{SF-207}
 * @SAE{SF-2459}
 * @SDD{SF-3868}
 * @verification{Check that this function correctly detects that the provided object is below the obstruction probability
 * threshold.}
 */
static boolean_T Cta_Is_Obstruction_Probability_Below_Threshold(
   const Cta_Object_Data_T *p_object /**< CTA object data*/, const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Returns true if the check against a potential ghost object has passed.
 * This check is optional and can be enabled/disabled by cal.
 *
 * @return True if check against potential ghost has passed
 *
 * @SRS{SF-208,SF-209,SF-214}
 * @SAE{SF-2459}
 * @SDD{SF-3857}
 * @verification{Check that this function correctly detects that the provided objects check against a potential ghost object has
 * passed.}
 */
static boolean_T Cta_Is_Check_Against_Potential_Ghost_Passed(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                                             const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Checks whether VRU object properties shall be valid for algorithm, so that the object can be used for further analysis.
 *
 * @return True when object is a valid CTA candidate.
 *
 * @SRS{CSCSA-145887}
 * @SAE{SF-2779}
 * @SDD{CSCSA-145874}
 * @verification{Create a test with an VRU object which shall be valid for algorithm.}
 */
static boolean_T Cta_Is_VRU_Object_Valid(const Cta_Object_Data_T *p_object /**< CTA object data*/,
                                         const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);

/**
 * @brief Returns a hysteresis level based on previous objects alert level.
 *
 * @return hysteresis level
 *
 * @SRS{SF-199,SF-200,SF-201,SF-132,SF-202,SF-203,SF-204,SF-205,SF-206,SF-207,SF-208,SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3856}
 * @verification{Check that the hysteresis level is returned correctly.}
 */
static Hysteresis_Level_T Cta_Get_Hysteresis_Level(const Cta_Object_Data_T *p_object /**< CTA object data*/);

/**
 * @brief Flags potential ghost objects. Flag is set to true,
 * if the difference of target heading and path heading with which the object is matched to
 * are greater than a defined threshold.
 *
 * @return True when an object is considered as potential ghost
 *
 * @SRS{SF-208,SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3863}
 * @verification{Check that this function correctly detects that the provided object is a potential ghost.}
 */
static boolean_T
Cta_Is_Object_A_Potential_Ghost(const Cta_Object_Data_T *p_object /**<  Object to be checked if it is a potential ghost */,
                                const Cta_Core_Calibration_T *p_cta_cal /**<  CTA_calibrations */);

/**
 * @brief Checks whether the heading difference of a path-object pair is
 * exceeding a given limit
 *
 * @return True if the heading difference exceeds a limit
 *
 * @SRS{SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3860}
 * @verification{Check that this function correctly detects that the provided objects heading difference is above the limit.}
 */
static boolean_T Cta_Is_Matchs_Head_Diff_Gt_Limits(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal);

/**
 * @brief Checks whether an object is fairly new and crosses already existing paths
 *
 * @return True when new object is crossing paths
 *
 * @SRS{SF-208}
 * @SAE{SF-2459}
 * @SDD{SF-3853}
 * @verification{Check that this function correctly detects that the provided object is new and crosses an existing path.}
 */
static boolean_T
Cta_Does_New_Obj_Cross_Nearest_Path(const Cta_Object_Data_T *p_object /**< new object to be checked whether it crosses a path*/,
                                    const Cta_Core_Calibration_T *p_cta_cal /**< calibrations*/);

/**
 * @brief Checks whether an object is matched to a path for a specific amount
 * of successive cycles
 *
 * @return True when new object is matched to path
 *
 * @SRS{SF-208,SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3861}
 * @verification{Check that this function correctly detects that the provided objects is matched to a path.}
 */
static boolean_T Cta_Is_New_Obj_Matched_To_Path(
   const Cta_Object_Data_T *p_object /**< object to be checked whether it is matched to a path for a minimum amount of cycles*/,
   const Cta_Core_Calibration_T *p_cta_cal /**< calibrations*/);

/**
 * @brief Checks if a potential ghost should be further considered.
 * For this the object is checked on several things:
 *	- Is the Track mature?
 *	- Does the object have a minimum age?
 *	- Does the object have a minimum stage age?
 *
 * @return True if a potential ghost should be considered
 *
 * @SRS{SF-208,SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3869}
 * @verification{Check that this function correctly detects that the provided object should be considered.}
 */
static boolean_T
Cta_Should_Potential_Ghost_Be_Considered(const Cta_Object_Data_T *p_object /**<  Object to be checked if it is a potential ghost */,
                                         const Cta_Core_Calibration_T *p_cta_cal /**<  CTA_calibrations */);

/**
 * @brief Fills the criteria values which the object needs to fulfill.
 * Also applies hysteresis to the criteria parameters.
 *
 * @return void
 *
 * @SRS{SF-199,SF-200,SF-201,SF-132,SF-202,SF-203,SF-204,SF-205,SF-206,SF-207,SF-208,SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3855}
 * @verification{Check that the criteria values are filled correctly.}
 */
static void
Cta_Fill_Object_Valid_Crit_With_Hyst(const Cta_Object_Data_T *p_object /**<  Object to be checked if it is a potential ghost */,
                                     const Cta_Core_Calibration_T *p_cta_cal /**<  CTA_calibrations */,
                                     CTA_Obj_Valid_Crit_T *p_obj_valid_criteria /**< object validation criteria*/);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
boolean_T Cta_Is_Object_Valid(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   CTA_Obj_Valid_Crit_T obj_crit;
   boolean_T f_object_valid = FBK_FALSE;
   boolean_T f_object_is_a_reflection;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   Cta_Fill_Object_Valid_Crit_With_Hyst(p_object, p_cta_cal, &obj_crit);

   f_object_is_a_reflection = Cta_Is_Object_A_Reflection(p_object, p_cta_cal);

   if (Fbk_Is_False(f_object_is_a_reflection) && (Cta_Is_Coasted_Obj_Below_Ignore_Cycles(p_object, p_cta_cal))
       && (Cta_Is_Object_Age_Above_Threshold(p_object, p_cta_cal))
       && (Cta_Is_Object_Speed_In_Allowed_Range(p_object, &(obj_crit.cta_speed_range)))
       && (Cta_Is_Object_Heading_Inside_Allowed_Range(p_object, &(obj_crit.cta_heading_range), &(obj_crit.cta_heading_variance_range)))
       && (Cta_Is_Lat_Obj_Vel_Above_Threshold(p_object, obj_crit.min_lateral_approach_speed_obj))
       && (Cta_Is_Obj_Exist_Prob_Above_Threshold(p_object, obj_crit.min_existence_probability_obj))
       && (Cta_Does_Suppr_Counter_Exceed_Threshold(p_object, p_cta_cal))
       && (Cta_Is_Obstruction_Probability_Below_Threshold(p_object, p_cta_cal))
       && (Cta_Is_Check_Against_Potential_Ghost_Passed(p_object, p_cta_cal)) && (Cta_Is_VRU_Object_Valid(p_object, p_cta_cal)))
   {
      f_object_valid = FBK_TRUE;
      Binary_Cta_Debug_Pass_Object_Validity_Flag(p_object, f_object_valid);
   }

   return f_object_valid;
}


static boolean_T Cta_Is_Object_A_Reflection(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_object_is_reflection = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if (Fbk_Is_True(p_object->tracker_data.f_reflection) && Fbk_Is_True(p_cta_cal->k_cta_f_check_reflection_signal))
   {
      f_object_is_reflection = FBK_TRUE;
   }

   return f_object_is_reflection;
}

static boolean_T Cta_Is_Coasted_Obj_Below_Ignore_Cycles(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_coasted_obj_below_ignore_cycles = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if ((Fbk_Is_Obj_Coasted_Status_Implausible(&p_object->tracker_data) || (PA_OBJ_STATUS_COASTED != p_object->tracker_data.status)))
   {
      f_coasted_obj_below_ignore_cycles = FBK_TRUE;
   }
   else
   {
      if (p_object->tracker_data.stage_age <= p_cta_cal->k_cta_cycles_coasted_to_ignore)
      {
         f_coasted_obj_below_ignore_cycles = FBK_TRUE;
      }
   }
   return f_coasted_obj_below_ignore_cycles;
}


static boolean_T Cta_Is_Object_Age_Above_Threshold(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_object_age_above_threshold = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if (Fbk_Is_False(p_cta_cal->k_cta_f_use_object_min_object_age_in_cycles)
       || (p_object->tracker_data.age > p_cta_cal->k_cta_min_object_age_check_valid))
   {
      f_object_age_above_threshold = FBK_TRUE;
   }

   return f_object_age_above_threshold;
}


static boolean_T Cta_Is_Object_Speed_In_Allowed_Range(const Cta_Object_Data_T *p_object, const Float_Range_T *p_cta_speed_range)
{
   boolean_T f_speed_in_range;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_speed_range);

   f_speed_in_range = Is_Float_Contained_In_Float_Range(p_object->tracker_data.speed, p_cta_speed_range);

   return f_speed_in_range;
}


static boolean_T Cta_Is_Object_Heading_Inside_Allowed_Range(const Cta_Object_Data_T *p_object,
                                                            const Float_Range_T *p_cta_heading_range,
                                                            const Float_Range_T *p_cta_heading_variance_range)
{
   boolean_T f_object_heading_in_range = FBK_FALSE;
   boolean_T f_cta_heading_in_range;
   boolean_T f_cta_heading_variance_in_range;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_heading_range);
   assert(NULL != p_cta_heading_variance_range);

   f_cta_heading_in_range = Is_Float_Contained_In_Float_Range(Fbk_Abs_F(p_object->attributes->CTA_heading), p_cta_heading_range);
   f_cta_heading_variance_in_range =
      Is_Float_Contained_In_Float_Range(p_object->tracker_data.heading_variance, p_cta_heading_variance_range);

   if (Fbk_Is_True(f_cta_heading_in_range) && Fbk_Is_True(f_cta_heading_variance_in_range))
   {
      f_object_heading_in_range = FBK_TRUE;
   }

   return f_object_heading_in_range;
}


static boolean_T Cta_Is_Lat_Obj_Vel_Above_Threshold(const Cta_Object_Data_T *p_object, const float32_T min_lateral_approach_speed_obj)
{
   boolean_T f_lat_obj_vel_above_threshold = FBK_FALSE;

   /* Assert */
   assert(NULL != p_object);

   if (Fbk_Abs_F(p_object->attributes->relative_velocity.y) >= min_lateral_approach_speed_obj)
   {
      f_lat_obj_vel_above_threshold = FBK_TRUE;
   }

   return f_lat_obj_vel_above_threshold;
}


static boolean_T Cta_Is_Obj_Exist_Prob_Above_Threshold(const Cta_Object_Data_T *p_object, const float32_T min_existence_probability_obj)
{
   boolean_T f_obj_exist_prob_above_threshold = FBK_FALSE;

   /* Assert */
   assert(NULL != p_object);

   if (p_object->tracker_data.existence_probability > min_existence_probability_obj)
   {
      f_obj_exist_prob_above_threshold = FBK_TRUE;
   }

   return f_obj_exist_prob_above_threshold;
}


static void Cta_Update_Validity_Suppression_Ctr(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   /* Assert */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if ((NULL != p_object->attributes->p_pt_match_info) && (Fbk_Is_True(p_cta_cal->k_cta_f_apply_path_tracking)))
   {
      /*Check whether a path match change occured. When this is fulfilled, the counter shall be reset.
     This leads to a race between the object reaching a criticality level and the suppression logic. However as soon as the object
     reached any criticality level, an upper level hysteresis will be applied, such that toggling is prevented.*/
      if (p_object->attributes->p_pt_match_info->track_match != p_object->attributes->p_pt_match_info->track_match_last_cycle)
      {
         p_object->persistent->obj_validity_suppression_counter = FBK_ZERO_UINT;
      }

      /* Check whether object is moving opposite to the given path direction*/
      if (((PATH_DIRECTION_LAT_LEFT == p_object->attributes->p_pt_match_info->path_direction)
           && (FBK_SIDE_LEFT == p_object->attributes->approach_side))
          || ((PATH_DIRECTION_LAT_RIGHT == p_object->attributes->p_pt_match_info->path_direction)
              && (FBK_SIDE_RIGHT == p_object->attributes->approach_side)))
      {
         Sat_Inc_Uint8(&p_object->persistent->obj_validity_suppression_counter);
      }
      else
      {
         /*In case that no path match or a path match with matching directions is established, the object suppression counter check
          * shall be disabled.*/
         p_object->persistent->obj_validity_suppression_counter = CTA_COUNTER_MAX;
      }
   }
   else
   {
      /*In case that CTA is running standalone, the suppression counter check shall be disabled.*/
      p_object->persistent->obj_validity_suppression_counter = CTA_COUNTER_MAX;
   }
}

static boolean_T Cta_Does_Suppr_Counter_Exceed_Threshold(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_suppr_counter_exceed_threshold = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if (Fbk_Is_False(p_cta_cal->k_cta_f_use_object_supress_counter)
       || (p_object->persistent->obj_validity_suppression_counter > p_cta_cal->k_cta_object_supress_counter))
   {
      f_suppr_counter_exceed_threshold = FBK_TRUE;
   }

   return f_suppr_counter_exceed_threshold;
}


static boolean_T Cta_Is_Obstruction_Probability_Below_Threshold(const Cta_Object_Data_T *p_object,
                                                                const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_obstr_prob_below_threshold = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if (Fbk_Is_False(p_cta_cal->k_cta_f_check_obstruction_probability_signal)
       || (p_object->tracker_data.obstruction_prob < p_cta_cal->k_cta_max_obstruction_probability))
   {
      f_obstr_prob_below_threshold = FBK_TRUE;
   }

   return f_obstr_prob_below_threshold;
}


static boolean_T Cta_Is_Check_Against_Potential_Ghost_Passed(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_check_for_potential_ghost_passed     = FBK_FALSE;
   boolean_T f_should_potential_ghost_be_considered = FBK_FALSE;
   boolean_T f_obj_is_a_potential_ghost;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   f_obj_is_a_potential_ghost = Cta_Is_Object_A_Potential_Ghost(p_object, p_cta_cal);

   if (Fbk_Is_True(f_obj_is_a_potential_ghost))
   {
      f_should_potential_ghost_be_considered = Cta_Should_Potential_Ghost_Be_Considered(p_object, p_cta_cal);
   }

   if (Fbk_Is_False(p_cta_cal->k_cta_f_use_ghost_detector) || Fbk_Is_False(f_obj_is_a_potential_ghost)
       || Fbk_Is_True(f_should_potential_ghost_be_considered))
   {
      f_check_for_potential_ghost_passed = FBK_TRUE;
   }

   return f_check_for_potential_ghost_passed;
}


static boolean_T Cta_Is_VRU_Object_Valid(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_vru_obj_is_valid = FBK_TRUE;
   float32_T obj_area           = p_object->tracker_data.width * p_object->tracker_data.length;
   Pa_Obj_Class_T obj_class     = p_object->tracker_data.obj_class;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   switch (obj_class)
   {
      case PA_OBJ_CLASS_UNKNOWN:
      case PA_OBJ_CLASS_PEDESTRIAN:
         if ((obj_area < p_cta_cal->k_cta_pedestrian_min_size)
             && ((p_object->tracker_data.speed) < p_cta_cal->k_cta_pedestrian_min_speed))
         {
            f_vru_obj_is_valid = FBK_FALSE;
         }

         break;

      case PA_OBJ_CLASS_2WHEEL:
         if ((obj_area < p_cta_cal->k_cta_2wheel_min_size) && ((p_object->tracker_data.speed) < p_cta_cal->k_cta_2wheel_min_speed))
         {
            f_vru_obj_is_valid = FBK_FALSE;
         }

         break;
      default:
         f_vru_obj_is_valid = FBK_TRUE;
         break;
   }

   return f_vru_obj_is_valid;
}


static Hysteresis_Level_T Cta_Get_Hysteresis_Level(const Cta_Object_Data_T *p_object)
{
   Hysteresis_Level_T hysteresis_level;

   /* Assert */
   assert(NULL != p_object);

   if (((uint8_t) CTA_CRIT_LEVEL_NONE != p_object->persistent->prev_cycle_crit_level[CTA_MODE_REAR])
       || ((uint8_t) CTA_CRIT_LEVEL_NONE != p_object->persistent->prev_cycle_crit_level[CTA_MODE_FRONT]))
   {
      hysteresis_level = HYST_LEVEL_ACTIVE_PREV_WARN;
   }
   else
   {
      hysteresis_level = HYST_LEVEL_NONE_PREV_WARN;
   }

   return hysteresis_level;
}


static boolean_T Cta_Is_Object_A_Potential_Ghost(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_object_is_a_potential_ghost = FBK_FALSE;
   boolean_T f_match_head_diff_gt_limits;
   boolean_T f_new_obj_cross_nearest_path;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   f_match_head_diff_gt_limits  = Cta_Is_Matchs_Head_Diff_Gt_Limits(p_object, p_cta_cal);
   f_new_obj_cross_nearest_path = Cta_Does_New_Obj_Cross_Nearest_Path(p_object, p_cta_cal);

   if (Fbk_Is_True(f_match_head_diff_gt_limits) || Fbk_Is_True(f_new_obj_cross_nearest_path))
   {
      f_object_is_a_potential_ghost = FBK_TRUE;
   }

   return f_object_is_a_potential_ghost;
}

static boolean_T Cta_Is_Matchs_Head_Diff_Gt_Limits(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_heading_gt_limit_for_valid_match = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if (NULL != p_object->attributes->p_pt_match_info)
   {
      if ((PT_DEFAULT_MATCH_INDEX != p_object->attributes->p_pt_match_info->track_match)
          && (Fbk_Abs_F(p_object->tracker_data.vcs_heading - p_object->attributes->p_pt_match_info->path_heading)
              > p_cta_cal->k_cta_ghost_condition_max_heading_diff_path_tracker))
      {
         f_heading_gt_limit_for_valid_match = FBK_TRUE;
      }
   }

   return f_heading_gt_limit_for_valid_match;
}


static boolean_T Cta_Does_New_Obj_Cross_Nearest_Path(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_new_obj_crossing_path = FBK_FALSE;
   boolean_T f_new_obj_matched_to_path;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   f_new_obj_matched_to_path = Cta_Is_New_Obj_Matched_To_Path(p_object, p_cta_cal);

   if (NULL != p_object->attributes->p_pt_nearest_path_info)
   {
      if (Fbk_Is_False(f_new_obj_matched_to_path)
          && (PT_DEFAULT_MATCH_INDEX != p_object->attributes->p_pt_nearest_path_info->track_idx_nearest_path)
          && (p_object->tracker_data.age < p_cta_cal->k_cta_min_qual_age_obj_crossing_paths)
          && (Fbk_Abs_F(p_object->attributes->p_pt_nearest_path_info->segment_heading_diff)
              > p_cta_cal->k_cta_max_seg_heading_diff_no_ghost)
          && (p_object->attributes->p_pt_nearest_path_info->range_vcs_proj_to_path_segment
              < p_cta_cal->k_cta_range_to_path_segment_ghost_qualif))
      {
         f_new_obj_crossing_path = FBK_TRUE;
      }
   }

   return f_new_obj_crossing_path;
}

static boolean_T Cta_Is_New_Obj_Matched_To_Path(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T f_new_obj_matched_to_path = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if (NULL != p_object->attributes->p_pt_match_info)
   {
      if ((PT_DEFAULT_MATCH_INDEX != p_object->attributes->p_pt_match_info->track_match)
          && (p_object->attributes->p_pt_match_info->track_match_age >= p_cta_cal->k_cta_cycles_valid_match_of_pot_ghost))
      {
         f_new_obj_matched_to_path = FBK_TRUE;
      }
   }

   return f_new_obj_matched_to_path;
}


static boolean_T Cta_Should_Potential_Ghost_Be_Considered(const Cta_Object_Data_T *p_object, const Cta_Core_Calibration_T *p_cta_cal)
{
   boolean_T ret_val = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);

   if ((PA_OBJ_STATUS_MATURE == p_object->tracker_data.status)
       && (p_object->tracker_data.age >= p_cta_cal->k_cta_ghost_validation_min_age)
       && (p_object->tracker_data.stage_age >= p_cta_cal->k_cta_ghost_validation_min_mature))
   {
      ret_val = FBK_TRUE;
   }

   return ret_val;
}


static void Cta_Fill_Object_Valid_Crit_With_Hyst(const Cta_Object_Data_T *p_object,
                                                 const Cta_Core_Calibration_T *p_cta_cal,
                                                 CTA_Obj_Valid_Crit_T *p_obj_valid_criteria)
{
   Hysteresis_Level_T hysteresis_level;
   float32_T heading_angle_hys;
   float32_T speed_hys_upper_border;
   float32_T speed_hys_lower_border;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);
   assert(NULL != p_obj_valid_criteria);

   /*Fill basic ranges.*/
   p_obj_valid_criteria->cta_heading_range.min = p_cta_cal->k_cta_heading_range[0u];
   p_obj_valid_criteria->cta_heading_range.max = p_cta_cal->k_cta_heading_range[1u];
   p_obj_valid_criteria->cta_speed_range       = Create_Float_Range(p_cta_cal->k_cta_min_speed, p_cta_cal->k_cta_max_speed);
   p_obj_valid_criteria->min_lateral_approach_speed_obj = p_cta_cal->k_cta_min_lateral_approach_speed;
   p_obj_valid_criteria->min_existence_probability_obj  = p_cta_cal->k_cta_min_rel_existence_probability;
   p_obj_valid_criteria->cta_heading_variance_range     = Create_Float_Range(FBK_ZERO_F, p_cta_cal->k_cta_max_heading_variance);

   /*Update the object validity suppression counter.*/
   Cta_Update_Validity_Suppression_Ctr(p_object, p_cta_cal);

   hysteresis_level = Cta_Get_Hysteresis_Level(p_object);

   if (HYST_LEVEL_ACTIVE_PREV_WARN == hysteresis_level)
   {
      /*Apply hysteresis to suppression counter such that an object does not get invalidated.*/
      p_object->persistent->obj_validity_suppression_counter = CTA_COUNTER_MAX;

      heading_angle_hys      = p_cta_cal->k_cta_heading_range[0] * p_cta_cal->k_cta_rel_warning_hysteresis;
      speed_hys_lower_border = p_obj_valid_criteria->cta_speed_range.min * p_cta_cal->k_cta_rel_warning_hysteresis;
      speed_hys_upper_border = p_obj_valid_criteria->cta_speed_range.max * p_cta_cal->k_cta_rel_warning_hysteresis;

      /*Apply hysteresis to allowed ranges.*/
      p_obj_valid_criteria->cta_speed_range.min -= speed_hys_lower_border;
      p_obj_valid_criteria->cta_speed_range.max += speed_hys_upper_border;
      p_obj_valid_criteria->min_lateral_approach_speed_obj -=
         p_obj_valid_criteria->min_lateral_approach_speed_obj * p_cta_cal->k_cta_rel_warning_hysteresis;
      p_obj_valid_criteria->min_existence_probability_obj -=
         p_obj_valid_criteria->min_existence_probability_obj * p_cta_cal->k_cta_rel_warning_hysteresis;
      p_obj_valid_criteria->cta_heading_range.min -= heading_angle_hys;
      p_obj_valid_criteria->cta_heading_range.max += heading_angle_hys;
      p_obj_valid_criteria->cta_heading_variance_range.max +=
         p_obj_valid_criteria->cta_heading_variance_range.max * p_cta_cal->k_cta_rel_warning_hysteresis;
   }
}
