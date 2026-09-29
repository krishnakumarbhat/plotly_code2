/**
 * @file lcda_process_elc.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Monitors adjacent lane for evasive lane change
 *
 * @copyright Copyright (C) 2019 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_process_elc.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_common_functions.h"
#include "lcda_debug_interface.h"
#include "ml_interval.h"
#include "ml_math.h"
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
 * @brief Resets Elc core output on the given side.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2779}
 * @SDD{SF-6704}
 * @verification{Check whether Elc core output on the given side is reset correctly.}
 */
static void Lcda_Clear_Elc_Core_Output_On_Side(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */,
                                               const uint8_t side /**< side index */);


/**
 * @brief Initializes Elc object data as well as the objects bsw zone.
 *
 * @return void
 *
 * @SRS{SF-1087}
 * @SAE{SF-2779}
 * @SDD{SF-6707}
 * @verification{Check whether Elc object data is reset correctly.}
 */
static void Lcda_Init_Elc_Object_Data(Elc_Object_T *p_elc_object /**< Elc object data */,
                                      const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Maps Elc core output data to the persistent Elc data structure.
 *
 * @return void
 *
 * @SRS{SF-1102}
 * @SAE{SF-2779}
 * @SDD{SF-6706}
 * @verification{Check whether persistent Elc data is updated correctly.}
 */
static void Lcda_Fill_Side_Persistent_Elc_Data(Lcda_Elc_Persistent_T *p_elc_persistent /**< Elc persistent data */,
                                               const Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */);

/**
 * @brief Sets most critical Elc object based on objects deceleration to reach the hosts speed.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2779}
 * @SDD{SF-6711}
 * @verification{Check whether most critical elc objects data is updated correctly.}
 */
static void Lcda_Set_Most_Critical_Elc_Object(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */,
                                              const Elc_Object_T *p_elc_object /**< Elc object data */);

/**
 * @brief Checks whether object was most critical object for Elc in the last cycle.
 *
 * @return True when object caused an Elc warning in the last cycle.
 *
 * @SRS{SF-1090,SF-1094}
 * @SAE{SF-2779}
 * @SDD{SF-6712}
 * @verification{Create a test with an object which has caused an Elc warning in the last cycle.}
 */
static boolean_T
Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(const Elc_Object_T *p_elc_object /**< Elc object data */,
                                          const Lcda_Elc_Persistent_T *p_elc_persistent /**< Elc persistent data*/);

/**
 * @brief Checks whether Ttc is below a internal determined threshold. This threshold might have an hysteresis applied,
 *        when the object has been Elc critical before.
 *
 * @return True when Ttc is below a given threshold
 *
 * @SRS{SF-1096,SF-1098}
 * @SAE{SF-2779}
 * @SDD{SF-6710}
 * @verification{Create tests where a ttc hysteresis is applied and and one case where it is not. When the given objects ttc is
 * below the threshold, true is expected.}
 */
static boolean_T Lcda_Is_Ttc_Below_Threshold(const Elc_Object_T *p_elc_object /**< Elc object data */,
                                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                             const Lcda_Elc_Persistent_T *p_elc_persistent /**< Elc persistent data */);

/**
 * @brief Check if the passed objects deceleration value is above a value that is deemed safe via cal setting.
 *        To avoid warn level toggeling, a hysteresis value is used if the object was critical last cycle.
 *
 * @return True when objects deceleration to reach host speed is below a given threshold
 *
 * @SRS{SF-1100,SF-1110}
 * @SAE{SF-2779}
 * @SDD{SF-6708}
 * @verification{Create tests where a deceleration hysteresis is applied and and one case where it is not. When the given objects
 * deceleration to reach host speed is below the threshold, true is expected.}
 */
static boolean_T Lcda_Is_Deceleration_Critical(const Elc_Object_T *p_elc_object /**< Elc object data */,
                                               const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                               const Lcda_Elc_Persistent_T *p_elc_persistent /**< Elc persistent data */);

/**
 * @brief Checks whether object is relevant for Elc.
 *
 * @return True when object is relevant for Elc
 *
 * @SRS{SF-1095}
 * @SAE{SF-2779}
 * @SDD{SF-6709}
 * @verification{Create a object in a test with a status of mature or coasted but with a curvi heading and a longitudinal curvi
 * velocity less than the used threshold. Only then true is expected.}
 */
static boolean_T Lcda_Is_Object_Relevant_For_Elc(const Lcda_Core_Input_T *p_lcda_core_input /**< lcda core input*/,
                                                 const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker data */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                 const Lcda_Elc_Persistent_T *p_elc_persistent /**< Elc persistent data */);

/**
 * @brief Processes the Elc output with application of the Elc alert holding logic.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2779}
 * @SDD{SF-6924}
 * @verification{Check that the holding counter is not increased if an active alert is present on the corresponding side.}
 */
static void Lcda_Process_Elc_Output(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output*/,
                                    Lcda_Elc_Persistent_T *p_elc_persistent /**< Elc persistent data*/,
                                    const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations*/);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Lcda_Reset_Elc_Core(Lcda_Elc_Core_Output_T *p_elc_core_output, Lcda_Elc_Persistent_T *p_elc_persistent)
{
   uint8_t side;

   /* Assert */
   assert(NULL != p_elc_core_output);

   Lcda_Clear_Elc_Persistent(p_elc_persistent);

   p_elc_core_output->f_elc_is_enabled = FBK_FALSE;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Lcda_Clear_Elc_Core_Output_On_Side(p_elc_core_output, side);
   }
}

/* coverity[misra_c_2012_rule_8_7_violation] */
void Lcda_Create_Elc_Object_Zone(Elc_Object_T *p_elc_object,
                                 const uint8_t mature_count_in_elc_zone,
                                 const Lcda_Core_Input_T *p_core_input,
                                 const Lcda_Core_Calibration_T *p_cals)
{
   float32_T lane_width         = Fbk_Max(p_cals->k_lcda_min_lane_width, p_core_input->lane_width);
   float32_T lane_center_offset = p_core_input->lane_center_offset;
   float32_T object_width       = p_elc_object->p_tracker_data->width;

   float32_T hys_offsets_y[LCDA_NUMBER_OF_ZONE_POINTS] = {FBK_ZERO_F};
   uint8_t j;

   float32_T hysteresis_factor_by_object_width = FBK_ONE_F;

   Fbk_Field_Of_Interest_T elc_zone;
   Fbk_Field_Of_Interest_T elc_zone_hys;

   /* Asserts */
   assert(NULL != p_elc_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   elc_zone.size     = LCDA_NUMBER_OF_ZONE_POINTS;
   elc_zone_hys.size = LCDA_NUMBER_OF_ZONE_POINTS;

   /* Since the zone will be built for the right hand side initially, we mirror the lane center offset to correspond to the right
    * side */
   if (FBK_SIDE_LEFT == p_elc_object->ego_side)
   {
      lane_center_offset = -lane_center_offset;
   }

   /* Check that the cal value is not zero before dividing to get the hys factor */
   if (Fbk_Abs_F(p_cals->k_zone_hys_obj_width_correction) > THRESHOLD_IS_ZERO)
   {
      assert(p_cals->k_zone_hys_obj_width_correction > FBK_ZERO_F);
      hysteresis_factor_by_object_width = Min(1.0f, Max(FBK_ZERO_F, object_width / p_cals->k_zone_hys_obj_width_correction));
   }

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      /* define x coordinate of cvw zone and hysteresis zone */
      elc_zone.points[j].x     = p_cals->k_elc_zone_x[j];
      elc_zone_hys.points[j].x = p_cals->k_elc_zone_x[j];

      /* define y co-ord of zone and calculate hys zone y-offset */
      elc_zone.points[j].y = (lane_width * p_cals->k_elc_zone_y[j]);
   }

   /* Limit zone width */
   Lcda_Limit_Outer_Zone_Points(&elc_zone, lane_width, p_cals);

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      float32_T offset_sign = FBK_ONE_F;

      /* Calculate the offset for the y co-ord of hysteresis zone */
      hys_offsets_y[j] = (lane_width * p_cals->k_elc_zone_y_hys[j] * hysteresis_factor_by_object_width);

      /* Apply min / max filter using Enforce_Range */
      hys_offsets_y[j] = Enforce_Range(hys_offsets_y[j], p_cals->k_cvw_zone_y_hys_min, p_cals->k_cvw_zone_y_hys_max);

      if (((uint8_t) FRONT_EGO_SIDE == j) || ((uint8_t) MIDDLE_EGO_SIDE == j) || ((uint8_t) REAR_EGO_SIDE == j))
      {
         offset_sign = -FBK_ONE_F;
      }

      /* define y co-ord of hys zone points by adding the hys offsets */
      elc_zone_hys.points[j].y = elc_zone.points[j].y + (offset_sign * hys_offsets_y[j]);

      /* Apply lane center offset */
      elc_zone.points[j].y     = elc_zone.points[j].y + lane_center_offset;
      elc_zone_hys.points[j].y = elc_zone_hys.points[j].y + lane_center_offset;
   }

   /* Note the zone is by default built for the right side so mirror the zone for the left side */
   if (FBK_SIDE_LEFT == p_elc_object->ego_side)
   {
      Lcda_Mirror_Zone_Across_Long_Axis(&elc_zone);
      Lcda_Mirror_Zone_Across_Long_Axis(&elc_zone_hys);
   }

   /* If the object has already been in the zone for some time then use the hysteresis zone */
   if (mature_count_in_elc_zone > p_cals->k_elc_min_mature_cycles)
   {
      p_elc_object->zone = elc_zone_hys;
   }
   else
   {
      p_elc_object->zone = elc_zone;
   }
}

void Lcda_Preprocess_Elc(Lcda_Elc_Core_Output_T *p_elc_core_output,
                         /* coverity[misra_c_2012_rule_2_7_violation][Unused during binary debug] */
                         const Lcda_Core_Input_T *p_core_input,
                         /* coverity[misra_c_2012_rule_2_7_violation][Unused during binary debug] */
                         const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t i_side;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_elc_core_output);

   Binary_Lcda_Debug_Pass_Elc_Default_Zone(p_core_input, p_cals);

   /* Initialize data of the most critical object for this cycle */
   for (i_side = FBK_ZERO_UINT; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      Lcda_Clear_Elc_Core_Output_On_Side(p_elc_core_output, i_side);
   }
}

void Lcda_Process_Elc_Object(Lcda_Elc_Core_Output_T *p_elc_core_output,
                             const Fbk_Object_Data_T *p_tracker_object,
                             const Lcda_Core_Input_T *p_core_input,
                             const Lcda_Core_Calibration_T *p_cals,
                             Lcda_Elc_Persistent_T *p_elc_persistent)
{
   /* Asserts */
   assert(NULL != p_elc_core_output);
   assert(NULL != p_tracker_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_elc_persistent);

   if (Fbk_Is_True(Lcda_Is_Object_Relevant_For_Elc(p_core_input, p_tracker_object, p_cals, p_elc_persistent)))
   {
      Elc_Object_T curr_obj;
      Lcda_Coordinate_System_T coordinate_system;
      Lcda_Object_Location_Data_T obj_loc_data;

      coordinate_system = LCDA_USE_CURVI;

      /* Set common shared parameters as ELC */
      Binary_Lcda_Debug_Pass_Processed_Submodule(LCDA_ELC);

      /* Initialize ELC object information */
      Lcda_Init_Elc_Object_Data(&curr_obj, p_tracker_object);

      /* Determine ego side of current object */
      curr_obj.ego_side = Fbk_Get_Obj_Side(curr_obj.p_tracker_data->curvi_pos.y);

      Lcda_Create_Elc_Object_Zone(&curr_obj, p_elc_persistent->mature_count_in_elc_zone[curr_obj.p_tracker_data->id], p_core_input,
                                  p_cals);

      /* Set Object Location Data*/
      Lcda_Get_Object_Location_Data(&obj_loc_data, curr_obj.p_tracker_data, &curr_obj.zone, p_cals, coordinate_system);
      curr_obj.f_obj_in_zone = obj_loc_data.obj_in_zone;

      if (Fbk_Is_True(curr_obj.f_obj_in_zone))
      {
         /* Increment the counter for object in the ELC zone */
         Lcda_Increment_Mature_Count_In_Zone(&(p_elc_persistent->mature_count_in_elc_zone[curr_obj.p_tracker_data->id]),
                                             curr_obj.p_tracker_data->status);

         /* Get longitudinal TTC */
         curr_obj.lon_ttc = Lcda_Get_Longitudinal_Ttc(curr_obj.p_tracker_data, p_core_input->p_pa_data->vehicle_data.host_length);

         if ((curr_obj.lon_ttc > THRESHOLD_IS_ZERO) && (curr_obj.lon_ttc < LCDA_DEFAULT_LARGE_TTC))
         {
            /* Calculate the deceleration required to reach the host vehicle speed */
            curr_obj.obj_decel_to_reach_host_speed = (curr_obj.p_tracker_data->curvi_vel_rel.x / curr_obj.lon_ttc);
         }

         /* Check if the longitudinal TTC is below the given threshold value */
         curr_obj.f_obj_ttc_below_threshold = Lcda_Is_Ttc_Below_Threshold(&curr_obj, p_cals, p_elc_persistent);

         /* Check if the currently required deceleration is above the given threshold value */
         curr_obj.f_obj_decel_above_threshold = Lcda_Is_Deceleration_Critical(&curr_obj, p_cals, p_elc_persistent);

         /* Check if the obj passes criteria to issue an alert */
         if ((p_elc_persistent->mature_count_in_elc_zone[curr_obj.p_tracker_data->id] >= p_cals->k_elc_min_mature_cycles)
             && (Fbk_Is_True(curr_obj.f_obj_ttc_below_threshold)) && (Fbk_Is_True(curr_obj.f_obj_decel_above_threshold)))
         {
            /* Check if current object is the most critical object for its side */
            Lcda_Set_Most_Critical_Elc_Object(p_elc_core_output, &curr_obj);
         }
      }
      else /* Object is not in zone so clear the mature in zone count */
      {
         p_elc_persistent->mature_count_in_elc_zone[curr_obj.p_tracker_data->id] = FBK_ZERO_UINT;
      }

      /* Pass object attributes to debug structure */
      Binary_Lcda_Debug_Pass_Elc_Object_Attributes(&curr_obj);
   }
   else /* Object is not valid so clear the count in zone */
   {
      p_elc_persistent->mature_count_in_elc_zone[p_tracker_object->id] = FBK_ZERO_UINT;
   }
}

void Lcda_Postprocess_Elc(Lcda_Elc_Core_Output_T *p_elc_core_output,
                          const Lcda_Core_Calibration_T *p_cals,
                          Lcda_Elc_Persistent_T *p_elc_persistent)
{
   /* Asserts */
   assert(NULL != p_elc_core_output);

   Lcda_Process_Elc_Output(p_elc_core_output, p_elc_persistent, p_cals);

   /* Fill side persistent data */
   Lcda_Fill_Side_Persistent_Elc_Data(p_elc_persistent, p_elc_core_output);

   Binary_Lcda_Debug_Pass_Elc_Persistent_Data(p_elc_persistent);
}

/* coverity[misra_c_2012_rule_8_7_violation][This function is needed by the LCDA debug writer] */
void Lcda_Clear_Elc_Persistent(Lcda_Elc_Persistent_T *p_elc_persistent)
{
   uint8_t iobj;
   uint8_t side;

   /* Assert */
   assert(NULL != p_elc_persistent);

   /* Reset mature counts */
   for (iobj = FBK_ZERO_UINT; iobj <= PA_OBJ_NUMBER_OF_OBJECTS; iobj++)
   {
      p_elc_persistent->mature_count_in_elc_zone[iobj] = FBK_ZERO_UINT;
   }

   /* Clear the output from the previous cycle for both sides and the holding counter */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_elc_persistent->prev_elc_alert_obj_index[side] = PA_INVALID_OBJ_INDEX;
      p_elc_persistent->prev_elc_alert_obj_id[side]    = PA_INVALID_OBJ_ID;
      p_elc_persistent->elc_hold_counter[side]         = FBK_ZERO_UINT;
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Init_Elc_Object_Data(Elc_Object_T *p_elc_object, const Fbk_Object_Data_T *p_tracker_object)
{
   /* Asserts */
   assert(NULL != p_elc_object);
   assert(NULL != p_tracker_object);

   p_elc_object->lon_ttc                       = LCDA_DEFAULT_LARGE_TTC;
   p_elc_object->obj_decel_to_reach_host_speed = FBK_ZERO_F;
   p_elc_object->ego_side                      = FBK_SIDE_UNDEFINED;
   p_elc_object->f_obj_in_zone                 = FBK_FALSE;
   p_elc_object->f_obj_ttc_below_threshold     = FBK_FALSE;
   p_elc_object->f_obj_decel_above_threshold   = FBK_FALSE;
   p_elc_object->p_tracker_data                = p_tracker_object;

   Fbk_Reset_Field_Of_Interest(&p_elc_object->zone);
}

static void Lcda_Clear_Elc_Core_Output_On_Side(Lcda_Elc_Core_Output_T *p_elc_core_output, const uint8_t side)
{
   /* Assert */
   assert(NULL != p_elc_core_output);

   p_elc_core_output->elc_alert[side]                     = FBK_FALSE;
   p_elc_core_output->elc_index[side]                     = PA_INVALID_OBJ_INDEX;
   p_elc_core_output->elc_id[side]                        = PA_INVALID_OBJ_ID;
   p_elc_core_output->elc_ttc[side]                       = LCDA_DEFAULT_LARGE_TTC;
   p_elc_core_output->elc_decel_to_reach_host_speed[side] = FBK_ZERO_F;
}

static void Lcda_Fill_Side_Persistent_Elc_Data(Lcda_Elc_Persistent_T *p_elc_persistent, const Lcda_Elc_Core_Output_T *p_elc_core_output)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_elc_persistent);
   assert(NULL != p_elc_core_output);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Save the ELC outputs for the next run */
      p_elc_persistent->prev_elc_alert_obj_index[side] = p_elc_core_output->elc_index[side];
      p_elc_persistent->prev_elc_alert_obj_id[side]    = p_elc_core_output->elc_id[side];
   }
}

static void Lcda_Set_Most_Critical_Elc_Object(Lcda_Elc_Core_Output_T *p_elc_core_output, const Elc_Object_T *p_elc_object)
{
   uint8_t side = p_elc_object->ego_side;

   /* Asserts */
   assert(NULL != p_elc_core_output);
   assert(NULL != p_elc_object);

   /* The objects deceleration to reach the hosts speed is checked to determine the criticality compared to other ELC alert
    * candidates. */
   if (p_elc_object->obj_decel_to_reach_host_speed > p_elc_core_output->elc_decel_to_reach_host_speed[side])
   {
      p_elc_core_output->elc_alert[side]                     = FBK_TRUE;
      p_elc_core_output->elc_index[side]                     = p_elc_object->p_tracker_data->index;
      p_elc_core_output->elc_id[side]                        = p_elc_object->p_tracker_data->id;
      p_elc_core_output->elc_ttc[side]                       = p_elc_object->lon_ttc;
      p_elc_core_output->elc_decel_to_reach_host_speed[side] = p_elc_object->obj_decel_to_reach_host_speed;
   }
}

static boolean_T Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(const Elc_Object_T *p_elc_object,
                                                           const Lcda_Elc_Persistent_T *p_elc_persistent)
{
   uint8_t side;
   boolean_T result = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_elc_object);
   assert(NULL != p_elc_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      if (p_elc_object->p_tracker_data->id == p_elc_persistent->prev_elc_alert_obj_id[side])
      {
         result = FBK_TRUE;
      }
   }

   return result;
}

static boolean_T Lcda_Is_Ttc_Below_Threshold(const Elc_Object_T *p_elc_object,
                                             const Lcda_Core_Calibration_T *p_cals,
                                             const Lcda_Elc_Persistent_T *p_elc_persistent)
{
   boolean_T f_obj_ttc_below_threshold;
   float32_T threshold_ttc_lon;

   /* Asserts */
   assert(NULL != p_elc_object);
   assert(NULL != p_cals);
   assert(NULL != p_elc_persistent);

   if (Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(p_elc_object, p_elc_persistent))
   {
      threshold_ttc_lon = p_cals->k_elc_critical_longitudinal_ttc_hys;
   }
   else
   {
      threshold_ttc_lon = p_cals->k_elc_critical_longitudinal_ttc;
   }

   f_obj_ttc_below_threshold = (boolean_T) ((p_elc_object->lon_ttc > FBK_ZERO_F) && (p_elc_object->lon_ttc <= threshold_ttc_lon));

   return f_obj_ttc_below_threshold;
}

static boolean_T Lcda_Is_Deceleration_Critical(const Elc_Object_T *p_elc_object,
                                               const Lcda_Core_Calibration_T *p_cals,
                                               const Lcda_Elc_Persistent_T *p_elc_persistent)
{
   boolean_T f_obj_decel_above_threshold;
   float32_T threshold_decel;

   /* Asserts */
   assert(NULL != p_elc_object);
   assert(NULL != p_cals);
   assert(NULL != p_elc_persistent);

   if (Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(p_elc_object, p_elc_persistent))
   {
      threshold_decel = p_cals->k_elc_obj_safe_deceleration_threshold_hys;
   }
   else
   {
      threshold_decel = p_cals->k_elc_obj_safe_deceleration_threshold;
   }

   f_obj_decel_above_threshold = (boolean_T) ((p_elc_object->obj_decel_to_reach_host_speed > FBK_ZERO_F)
                                              && (p_elc_object->obj_decel_to_reach_host_speed >= threshold_decel));

   return f_obj_decel_above_threshold;
}


static boolean_T Lcda_Is_Object_Relevant_For_Elc(const Lcda_Core_Input_T *p_lcda_core_input,
                                                 const Fbk_Object_Data_T *p_tracker_object,
                                                 const Lcda_Core_Calibration_T *p_cals,
                                                 const Lcda_Elc_Persistent_T *p_elc_persistent)
{
   boolean_T f_is_relevant_obj = FBK_FALSE;
   float32_T existence_prob_threshold;
   /* Asserts */
   assert(NULL != p_tracker_object);
   assert(NULL != p_cals);

   /* Get existence probability threshold*/
   existence_prob_threshold = Lcda_Get_Existence_Probability_Threshold(p_lcda_core_input, p_elc_persistent->prev_elc_alert_obj_id,
                                                                       p_tracker_object->id, p_cals);

   if (((PA_OBJ_STATUS_MATURE == p_tracker_object->status) || (PA_OBJ_STATUS_COASTED == p_tracker_object->status))
       && (Fbk_Abs_F(p_tracker_object->curvi_heading) <= p_cals->k_elc_max_curvi_heading_abs)
       && (Fbk_Abs_F(p_tracker_object->curvi_vel.x) >= p_cals->k_elc_min_obj_curvi_long_vel_abs)
       && (p_tracker_object->existence_probability >= existence_prob_threshold))
   {
      f_is_relevant_obj = FBK_TRUE;
   }

   return f_is_relevant_obj;
}

static void Lcda_Process_Elc_Output(Lcda_Elc_Core_Output_T *p_elc_core_output,
                                    Lcda_Elc_Persistent_T *p_elc_persistent,
                                    const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_elc_core_output);
   assert(NULL != p_elc_persistent);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      if (Fbk_Is_True(p_elc_core_output->elc_alert[side_index]))
      {
         /* Reset ELC Alert holding counter */
         p_elc_persistent->elc_hold_counter[side_index] = FBK_ZERO_UINT;
      }
      /* If there is no alert for this cycle, then check if the alert from the previous cycle needs to be held */
      else if ((PA_INVALID_OBJ_ID != p_elc_persistent->prev_elc_alert_obj_id[side_index])
               && (p_elc_persistent->elc_hold_counter[side_index] < p_cals->k_elc_alert_holding_cycles))
      {
         Sat_Inc_Uint8(&(p_elc_persistent->elc_hold_counter[side_index]));

         Lcda_Clear_Elc_Core_Output_On_Side(p_elc_core_output, side_index);
         p_elc_core_output->elc_alert[side_index] = FBK_TRUE;
         p_elc_core_output->elc_index[side_index] = p_elc_persistent->prev_elc_alert_obj_index[side_index];
         p_elc_core_output->elc_id[side_index]    = p_elc_persistent->prev_elc_alert_obj_id[side_index];
      }
      else
      {
         /* Reset ELC Alert holding counter */
         p_elc_persistent->elc_hold_counter[side_index] = FBK_ZERO_UINT;
      }
   }
}
