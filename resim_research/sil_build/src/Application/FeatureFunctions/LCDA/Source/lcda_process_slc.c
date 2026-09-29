/**
 * @file lcda_process_slc.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implementation of the simultaneous lane change (Slc) module
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_process_slc.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_common_functions.h"
#include "lcda_debug_interface.h"
#include "ml_interval.h"
#include "ml_lookup_table_2d.h"
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
 * @brief Sets falgs indicating object's position relative to ego.
 *
 * @return void
 *
 * @SRS{SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6955}
 * @verification{Create an object such that the object has overlap, and when is also besides the ego and when miss the ego in the
 * future.}
 */
static void Lcda_Set_Object_Position_Flags(Slc_Object_T *p_curr_obj /**< Slc object data */,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                           const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Resets Slc core output on the given side.
 *
 * @return void
 *
 * @SRS{SF-1107,SF-1106}
 * @SAE{SF-2779}
 * @SDD{SF-6728}
 * @verification{Check whether Slc core output on the given side is reset correctly.}
 */
static void Lcda_Clear_Slc_Core_Output_On_Side(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output */,
                                               const uint8_t side /**< side index */);

/**
 * @brief Initializes Slc object data as well as the objects bsw zone.
 *
 * @return void
 *
 * @SRS{SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6732}
 * @verification{Check whether Slc object data is reset correctly.}
 */
static void Lcda_Init_Slc_Object_Data(Slc_Object_T *p_slc_object /**< Slc object data */,
                                      const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Maps Slc core output data to the persistent Elc data structure.
 *
 * @return void
 *
 * @SRS{SF-1106,SF-1111}
 * @SAE{SF-2779}
 * @SDD{SF-6730}
 * @verification{Check whether persistent Slc data is updated correctly.}
 */
static void Lcda_Fill_Side_Persistent_Slc_Data(Lcda_Slc_Persistent_T *p_slc_persistent /**< Slc persistent data */,
                                               const Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output */);

/**
 * @brief Sets most critical Slc object based on objects deceleration to reach the hosts speed.
 *
 * @return void
 *
 * @SRS{SF-1075,SF-1077}
 * @SAE{SF-2779}
 * @SDD{SF-6736}
 * @verification{Check whether most critical Slc objects data is updated correctly.}
 */
static void Lcda_Set_Most_Critical_Slc_Object(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output */,
                                              const Slc_Object_T *p_slc_object /**< Slc object data */);

/**
 * @brief Check if the passed object was the most critical object last cycle for at least one side.
 *
 * @return True when slc object was critical in the last cycle.
 *
 * @SRS{Sf-1081}
 * @SAE{SF-2779}
 * @SDD{SF-6737}
 * @verification{Test a scenario where the given object was critical in the last cycle. Only then True shall be returned.}
 */
static boolean_T
Lcda_Was_Most_Critical_Slc_Obj_Last_Cycle(const Slc_Object_T *p_slc_object /**< Slc object data */,
                                          const Lcda_Slc_Persistent_T *p_slc_persistent /**< Slc persistent data */);


/**
 * @brief  Check if the passed objects TTCs are below the longitudinal or lateral TTC thresholds. To avoid warn level toggeling,
 *         a hysteresis value is used if the object was critical last cycle.
 *
 * @return True when the ttc is below the longitudinal and lateral thresholds
 *
 * @SRS{SF-1081}
 * @SAE{SF-2779}
 * @SDD{SF-6735}
 * @verification{Test a scenario where the given slc objects ttc is below the lateral and longitudinal thresholds. Then true is
 * expected.}
 */
static boolean_T Lcda_Is_Ttc_Below_Thresholds(const Slc_Object_T *p_slc_object /**< Slc object data */,
                                              const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                              const Lcda_Warn_Settings_T *p_warn_settings /**< Lcda warning settings*/,
                                              const Lcda_Slc_Persistent_T *p_slc_persistent /**< Slc persistent data */);

/**
 * @brief  Checks whether the object is relevant for Slc.
 *
 * @return True when object is relevant for Slc
 *
 * @SRS{SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6733}
 * @verification{Create a object in a test with a status of mature or coasted but with a curvi heading and a longitudinal curvi
 * velocity and an eclipse value less than the used threshold. Only then true is expected.}
 */
static boolean_T Lcda_Is_Object_Relevant_For_Slc(const Lcda_Core_Input_T *p_lcda_core_input /**<Lcda core input*/,
                                                 const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                 const Lcda_Slc_Persistent_T *p_slc_persistent /**<Slc persistent data*/);

/**
 * @brief  Returns the lane change probability for a given object based on the lateral ttc.
 *
 * @return Lane change probability of type float32_T
 *
 * @SRS{SF-1025,SF-1026,SF-1027}
 * @SAE{SF-2779}
 * @SDD{SF-6731}
 * @verification{Check that for a valid lateral ttc the corresponding lane change probability is provided.}
 */
static float32_T Lcda_Get_Lane_Change_Prob(const float32_T lat_ttc /**< lateral ttc */,
                                           const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);


/**
 * @brief  This function returns if the input object is located behind a guardrail that was *detected by radar
 *         or camera data.Objects located on a guardrail are not considered to
 *         be behind the guardrail.
 *
 * @return True when object is behind guardrail
 *
 * @SRS{SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6734}
 * @verification{Check that for an object behind the guardrail that true is returned.}
 */
static boolean_T
Lcda_Is_Slc_Object_Behind_Guardrail(const Slc_Object_T *p_slc_object /**< Slc object data */,
                                    const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES] /**< guardrail data */);

/**
 * @brief Processes the Slc output with application of the Slc alert qualification and holding logic.
 *
 * @return void
 *
 * @SRS{CSCSA-68593,SF-1099,SF-1106,SF-1111}
 * @SAE{SF-2779}
 * @SDD{SF-6923}
 * @verification{Check that the holding counter is not increased if an active alert is present on the corresponding side.}
 */
static void Lcda_Process_Slc_Output(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output */,
                                    Lcda_Slc_Persistent_T *p_slc_persistent /**< Slc persistent data */,
                                    const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Calculates the ego speed dependent ego overlap offset used to determine if an object is classified as besides the ego.
 *
 * @return ego overlap offset
 *
 * @SRS{SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6928}
 * @verification{Test that the ego overlap offset for objects beside the ego is calculated properly}
 */
static float32_T Lcda_Get_Ego_Overlap_Offset(const float32_T ego_speed /**< Ego speed */,
                                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Lcda_Reset_Slc_Core(Lcda_Slc_Core_Output_T *p_slc_core_output, Lcda_Slc_Persistent_T *p_slc_persistent)
{
   uint8_t side;

   /* Assert */
   assert(NULL != p_slc_core_output);

   Lcda_Clear_Slc_Persistent(p_slc_persistent);

   p_slc_core_output->f_slc_is_enabled = FBK_FALSE;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Lcda_Clear_Slc_Core_Output_On_Side(p_slc_core_output, side);
   }
}

/* coverity[misra_c_2012_rule_8_7_violation] */
void Lcda_Create_Slc_Object_Zone(Slc_Object_T *p_slc_object,
                                 const uint8_t mature_count_in_slc_zone,
                                 const Lcda_Core_Input_T *p_core_input,
                                 const Lcda_Core_Calibration_T *p_cals)
{
   float32_T lane_width         = Fbk_Max(p_cals->k_lcda_min_lane_width, p_core_input->lane_width);
   float32_T lane_center_offset = p_core_input->lane_center_offset;
   float32_T object_width       = p_slc_object->p_tracker_data->width;

   float32_T hys_offsets_y[LCDA_NUMBER_OF_ZONE_POINTS] = {FBK_ZERO_F};
   uint8_t j;

   float32_T hysteresis_factor_by_object_width = FBK_ONE_F;

   Fbk_Field_Of_Interest_T slc_zone;
   Fbk_Field_Of_Interest_T slc_zone_hys;

   /* Asserts */
   assert(NULL != p_slc_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   slc_zone.size     = LCDA_NUMBER_OF_ZONE_POINTS;
   slc_zone_hys.size = LCDA_NUMBER_OF_ZONE_POINTS;

   /* Since the zone will be built for the right hand side initially, we mirror the lane center offset to correspond to the right
    * side */
   if (FBK_SIDE_LEFT == p_slc_object->ego_side)
   {
      lane_center_offset = -lane_center_offset;
   }

   /* Check that the cal value is not zero before dividing to get the hys factor */
   if (Fbk_Abs_F(p_cals->k_zone_hys_obj_width_correction) > THRESHOLD_IS_ZERO)
   {
      hysteresis_factor_by_object_width = Min(1.0f, Max(FBK_ZERO_F, object_width / p_cals->k_zone_hys_obj_width_correction));
   }

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      /* define x coordinate of cvw zone and hysteresis zone */
      slc_zone.points[j].x     = p_cals->k_slc_zone_x[j];
      slc_zone_hys.points[j].x = p_cals->k_slc_zone_x[j];

      /* define y co-ord of zone and calculate hys zone y-offset */
      slc_zone.points[j].y = (lane_width * p_cals->k_slc_zone_y[j]);
   }

   /* Limit zone width */
   Lcda_Limit_Outer_Zone_Points(&slc_zone, lane_width, p_cals);

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      float32_T offset_sign = FBK_ONE_F;

      /* Calculate the offset for the y co-ord of hysteresis zone */
      hys_offsets_y[j] = (lane_width * p_cals->k_slc_zone_y_hys[j] * hysteresis_factor_by_object_width);

      /* Apply min / max filter using Enforce_Range */
      hys_offsets_y[j] = Enforce_Range(hys_offsets_y[j], p_cals->k_cvw_zone_y_hys_min, p_cals->k_cvw_zone_y_hys_max);

      if (((uint8_t) FRONT_EGO_SIDE == j) || ((uint8_t) MIDDLE_EGO_SIDE == j) || ((uint8_t) REAR_EGO_SIDE == j))
      {
         offset_sign = -FBK_ONE_F;
      }

      /* define y co-ord of hys zone points by adding the hys offsets */
      slc_zone_hys.points[j].y = slc_zone.points[j].y + (offset_sign * hys_offsets_y[j]);

      /* Apply lane center offset */
      slc_zone.points[j].y     = slc_zone.points[j].y + lane_center_offset;
      slc_zone_hys.points[j].y = slc_zone_hys.points[j].y + lane_center_offset;
   }

   /* Note the zone is by default built for the right side so mirror the zone for the left side */
   if (FBK_SIDE_LEFT == p_slc_object->ego_side)
   {
      Lcda_Mirror_Zone_Across_Long_Axis(&slc_zone);
      Lcda_Mirror_Zone_Across_Long_Axis(&slc_zone_hys);
   }

   /* If the object has already been in the zone for some time then use the hysteresis zone */
   if (mature_count_in_slc_zone > p_cals->k_slc_min_mature_cycles)
   {
      p_slc_object->zone = slc_zone_hys;
   }
   else
   {
      p_slc_object->zone = slc_zone;
   }
}

/* coverity[misra_c_2012_rule_2_7_violation] */
void Lcda_Preprocess_Slc(Lcda_Slc_Core_Output_T *p_slc_core_output,
                         /* coverity[misra_c_2012_rule_2_7_violation][Unused during binary debug] */
                         const Lcda_Core_Input_T *p_core_input,
                         /* coverity[misra_c_2012_rule_2_7_violation][Unused during binary debug] */
                         const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t i_side;

   /* Asserts */
   assert(NULL != p_slc_core_output);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   Binary_Lcda_Debug_Pass_Slc_Default_Zone(p_core_input, p_cals);

   /* Initialize data of the most critical object for this cycle */
   for (i_side = FBK_ZERO_UINT; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      Lcda_Clear_Slc_Core_Output_On_Side(p_slc_core_output, i_side);
   }
}

void Lcda_Process_Slc_Object(Lcda_Slc_Core_Output_T *p_slc_core_output,
                             const Fbk_Object_Data_T *p_tracker_object,
                             const Lcda_Core_Input_T *p_core_input,
                             const Lcda_Core_Calibration_T *p_cals,
                             Lcda_Slc_Persistent_T *p_slc_persistent)
{
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   /* Asserts */
   assert(NULL != p_slc_core_output);
   assert(NULL != p_tracker_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_slc_persistent);

   p_vehicle_data = &p_core_input->p_pa_data->vehicle_data;

   if (Fbk_Is_True(Lcda_Is_Object_Relevant_For_Slc(p_core_input, p_tracker_object, p_cals, p_slc_persistent)))
   {
      Slc_Object_T curr_obj;
      Lcda_Coordinate_System_T coordinate_system;
      Lcda_Object_Location_Data_T obj_loc_data;

      coordinate_system = LCDA_USE_CURVI;

      /* Set common shared parameters as SLC */
      Binary_Lcda_Debug_Pass_Processed_Submodule(LCDA_SLC);

      /* Initialize SLC object information */
      Lcda_Init_Slc_Object_Data(&curr_obj, p_tracker_object);

      /* Determine ego side of current object */
      curr_obj.ego_side = Fbk_Get_Obj_Side(curr_obj.p_tracker_data->curvi_pos.y);

      Lcda_Create_Slc_Object_Zone(&curr_obj, p_slc_persistent->mature_count_in_slc_zone[curr_obj.p_tracker_data->id], p_core_input,
                                  p_cals);

      /* Set Object Location Data*/
      Lcda_Get_Object_Location_Data(&obj_loc_data, curr_obj.p_tracker_data, &curr_obj.zone, p_cals, coordinate_system);

      /* Check if target object is in zone*/
      curr_obj.f_obj_in_zone = obj_loc_data.obj_in_zone;

      if (Fbk_Is_True(curr_obj.f_obj_in_zone))
      {
         /* Increment the counter for object in the SLC zone */
         Lcda_Increment_Mature_Count_In_Zone(&(p_slc_persistent->mature_count_in_slc_zone[curr_obj.p_tracker_data->id]),
                                             curr_obj.p_tracker_data->status);

         /* Get object effective lateral velocity (compensated via ego lane lateral speed) */
         curr_obj.effective_lateral_speed = p_tracker_object->curvi_vel_rel.y + p_core_input->lane_lateral_speed[curr_obj.ego_side];

         if (Fbk_Abs_F(curr_obj.effective_lateral_speed) >= p_cals->k_slc_obj_lc_effective_speed_min)
         {
            /* Object is moving faster towards ego than ego is moving across lanes. We assume object is changing lanes. */
            curr_obj.f_obj_lane_change = FBK_TRUE;
         }

         /* Get longitudinal TTC */
         curr_obj.lon_ttc = Lcda_Get_Longitudinal_Ttc(curr_obj.p_tracker_data, p_vehicle_data->host_length);

         /* Get lateral TTC */
         curr_obj.lat_ttc = Lcda_Get_Lateral_Ttc(curr_obj.p_tracker_data, p_vehicle_data->host_width);

         /* Set flags indicating objects position relative to ego (overlap, beside, miss) */
         Lcda_Set_Object_Position_Flags(&curr_obj, p_vehicle_data, p_cals);

         /* Check if the longitudinal and lateral TTCs are below the given threshold */
         curr_obj.f_obj_ttc_below_threshold =
            Lcda_Is_Ttc_Below_Thresholds(&curr_obj, p_cals, &p_core_input->warn_settings, p_slc_persistent);

         curr_obj.f_obj_behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&curr_obj, p_core_input->guardrail_data);

         /* Check if the obj passes criteria to issue an alert */
         if ((p_slc_persistent->mature_count_in_slc_zone[curr_obj.p_tracker_data->id] >= p_cals->k_slc_min_mature_cycles)
             && Fbk_Is_True(curr_obj.f_obj_ttc_below_threshold) && Fbk_Is_False(curr_obj.f_obj_behind_guardrail)
             && Fbk_Is_True(curr_obj.f_obj_lane_change) && Fbk_Is_False(curr_obj.f_obj_misses_ego))
         {
            /* Compute lane change intention and probability */
            curr_obj.lane_change_prob = Lcda_Get_Lane_Change_Prob(curr_obj.lat_ttc, p_cals);

            /* Check if current object is the most critical object for its side */
            Lcda_Set_Most_Critical_Slc_Object(p_slc_core_output, &curr_obj);
         }
      }
      else /* Object is not in zone so clear the mature in zone count */
      {
         p_slc_persistent->mature_count_in_slc_zone[curr_obj.p_tracker_data->id] = FBK_ZERO_UINT;
      }

      /* Pass object attributes to debug structure */
      Binary_Lcda_Debug_Pass_Slc_Object_Attributes(&curr_obj);
   }
   else /* Object is not valid so clear the count in zone */
   {
      p_slc_persistent->mature_count_in_slc_zone[p_tracker_object->id] = FBK_ZERO_UINT;
   }
}


void Lcda_Postprocess_Slc(Lcda_Slc_Core_Output_T *p_slc_core_output,
                          const Lcda_Core_Calibration_T *p_cals,
                          Lcda_Slc_Persistent_T *p_slc_persistent)
{

   /* Asserts */
   assert(NULL != p_slc_core_output);
   assert(NULL != p_cals);

   Lcda_Process_Slc_Output(p_slc_core_output, p_slc_persistent, p_cals);

   /* Fill side persistent data */
   Lcda_Fill_Side_Persistent_Slc_Data(p_slc_persistent, p_slc_core_output);

   Binary_Lcda_Debug_Pass_Slc_Persistent_Data(p_slc_persistent);
}

/* coverity[misra_c_2012_rule_8_7_violation][This function is needed by the LCDA debug writer] */
void Lcda_Clear_Slc_Persistent(Lcda_Slc_Persistent_T *p_slc_persistent)
{
   uint8_t iobj;
   uint8_t side;

   /* Assert */
   assert(NULL != p_slc_persistent);

   /* Reset mature counts */
   for (iobj = FBK_ZERO_UINT; iobj <= PA_OBJ_NUMBER_OF_OBJECTS; iobj++)
   {
      p_slc_persistent->mature_count_in_slc_zone[iobj] = FBK_ZERO_UINT;
   }

   /* Clear the output from the previous cycle and all counters for both sides */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_slc_persistent->prev_slc_alert_obj_index[side]     = PA_INVALID_OBJ_INDEX;
      p_slc_persistent->prev_slc_alert_obj_id[side]        = PA_INVALID_OBJ_ID;
      p_slc_persistent->prev_slc_alert_unique_obj_id[side] = PA_INVALID_OBJ_ID;
      p_slc_persistent->slc_qualifying_counter[side]       = FBK_ZERO_UINT;
      p_slc_persistent->slc_hold_counter[side]             = FBK_ZERO_UINT;
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Set_Object_Position_Flags(Slc_Object_T *p_curr_obj,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data,
                                           const Lcda_Core_Calibration_T *p_cals)
{

   /* Check if object is overlapping general area of ego */
   if (((p_curr_obj->p_tracker_data->vcs_pos.x - Fbk_Half(p_curr_obj->p_tracker_data->length)) <= FBK_ZERO_F)
       && ((p_curr_obj->p_tracker_data->vcs_pos.x + Fbk_Half(p_curr_obj->p_tracker_data->length))
           >= -(p_vehicle_data->host_length + Lcda_Get_Ego_Overlap_Offset(p_vehicle_data->host_speed, p_cals))))
   {
      p_curr_obj->f_obj_overlap = FBK_TRUE;
   }

   /* Check if object is besides ego */
   if (Fbk_Is_True(p_curr_obj->f_obj_overlap)
       && ((p_curr_obj->p_tracker_data->vcs_pos.x + Fbk_Half(p_curr_obj->p_tracker_data->length)) >= -(p_vehicle_data->host_length)))
   {
      p_curr_obj->f_obj_besides_ego = FBK_TRUE;
   }

   if (Fbk_Is_True(p_curr_obj->f_obj_besides_ego))
   {
      /* Calculate object long position after time period of lateral TTC */
      float32_T obj_long_pos_after_lat_ttc =
         p_curr_obj->p_tracker_data->vcs_pos.x + (p_curr_obj->lat_ttc * p_curr_obj->p_tracker_data->vcs_vel_rel.x);
      if ((Fbk_Abs_F(obj_long_pos_after_lat_ttc) + Fbk_Half(p_curr_obj->p_tracker_data->length))
          >= (p_vehicle_data->host_length * 2.0f))
      {
         p_curr_obj->f_obj_misses_ego = FBK_TRUE;
      }
   }
}

static void Lcda_Init_Slc_Object_Data(Slc_Object_T *p_slc_object, const Fbk_Object_Data_T *p_tracker_object)
{
   /* Assert */
   assert(NULL != p_slc_object);
   assert(NULL != p_tracker_object);

   p_slc_object->lat_ttc                 = LCDA_DEFAULT_LARGE_TTC;
   p_slc_object->lon_ttc                 = LCDA_DEFAULT_LARGE_TTC;
   p_slc_object->effective_lateral_speed = FBK_ZERO_F;

   p_slc_object->f_obj_in_zone             = FBK_FALSE;
   p_slc_object->f_obj_besides_ego         = FBK_FALSE;
   p_slc_object->f_obj_overlap             = FBK_FALSE;
   p_slc_object->f_obj_ttc_below_threshold = FBK_FALSE;
   p_slc_object->f_obj_behind_guardrail    = FBK_FALSE;
   p_slc_object->f_obj_lane_change         = FBK_FALSE;
   p_slc_object->f_obj_misses_ego          = FBK_FALSE;

   p_slc_object->lane_change_prob = LCDA_SLC_PROBABILITY_NONE;
   p_slc_object->ego_side         = FBK_SIDE_UNDEFINED;

   p_slc_object->p_tracker_data = p_tracker_object;

   Fbk_Reset_Field_Of_Interest(&p_slc_object->zone);
}

static void Lcda_Clear_Slc_Core_Output_On_Side(Lcda_Slc_Core_Output_T *p_slc_core_output, const uint8_t side)
{
   /* Assert */
   assert(NULL != p_slc_core_output);

   p_slc_core_output->slc_alert[side]            = FBK_FALSE;
   p_slc_core_output->slc_index[side]            = PA_INVALID_OBJ_INDEX;
   p_slc_core_output->slc_id[side]               = PA_INVALID_OBJ_ID;
   p_slc_core_output->slc_unique_id[side]        = PA_INVALID_OBJ_ID;
   p_slc_core_output->slc_lat_ttc[side]          = LCDA_DEFAULT_LARGE_TTC;
   p_slc_core_output->slc_lon_ttc[side]          = LCDA_DEFAULT_LARGE_TTC;
   p_slc_core_output->slc_ttp[side]              = LCDA_DEFAULT_LARGE_TTP;
   p_slc_core_output->slc_lane_change_prob[side] = LCDA_SLC_PROBABILITY_NONE;
}

static void Lcda_Fill_Side_Persistent_Slc_Data(Lcda_Slc_Persistent_T *p_slc_persistent, const Lcda_Slc_Core_Output_T *p_slc_core_output)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_slc_persistent);
   assert(NULL != p_slc_core_output);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Save the SLC outputs for the next run */
      p_slc_persistent->prev_slc_alert_obj_index[side]     = p_slc_core_output->slc_index[side];
      p_slc_persistent->prev_slc_alert_obj_id[side]        = p_slc_core_output->slc_id[side];
      p_slc_persistent->prev_slc_alert_unique_obj_id[side] = p_slc_core_output->slc_unique_id[side];
   }
}

static void Lcda_Set_Most_Critical_Slc_Object(Lcda_Slc_Core_Output_T *p_slc_core_output, const Slc_Object_T *p_slc_object)
{
   uint8_t side = p_slc_object->ego_side;

   /* Asserts */
   assert(NULL != p_slc_core_output);
   assert(NULL != p_slc_object);

   /* The object lane change probability is checked to determine the criticality compared to other SLC alert candidates. */
   if (p_slc_object->lane_change_prob > p_slc_core_output->slc_lane_change_prob[side])
   {
      p_slc_core_output->slc_alert[side]            = FBK_TRUE;
      p_slc_core_output->slc_index[side]            = p_slc_object->p_tracker_data->index;
      p_slc_core_output->slc_id[side]               = p_slc_object->p_tracker_data->id;
      p_slc_core_output->slc_unique_id[side]        = p_slc_object->p_tracker_data->unique_id;
      p_slc_core_output->slc_lane_change_prob[side] = p_slc_object->lane_change_prob;
      p_slc_core_output->slc_lat_ttc[side]          = p_slc_object->lat_ttc;
      p_slc_core_output->slc_lon_ttc[side]          = p_slc_object->lon_ttc;
      p_slc_core_output->slc_ttp[side]              = Lcda_Get_Longitudinal_Ttp(p_slc_object->p_tracker_data, LCDA_USE_CURVI);

      if (Fbk_Is_True(p_slc_object->f_obj_besides_ego))
      {
         p_slc_core_output->slc_lon_ttc[side] = LCDA_DEFAULT_LARGE_TTC;
      }
   }
}

static boolean_T Lcda_Was_Most_Critical_Slc_Obj_Last_Cycle(const Slc_Object_T *p_slc_object,
                                                           const Lcda_Slc_Persistent_T *p_slc_persistent)
{
   uint8_t side;
   boolean_T result = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_slc_object);
   assert(NULL != p_slc_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      if (p_slc_object->p_tracker_data->index == p_slc_persistent->prev_slc_alert_obj_index[side])
      {
         result = FBK_TRUE;
         break;
      }
   }

   return result;
}

static boolean_T Lcda_Is_Ttc_Below_Thresholds(const Slc_Object_T *p_slc_object,
                                              const Lcda_Core_Calibration_T *p_cals,
                                              const Lcda_Warn_Settings_T *p_warn_settings,
                                              const Lcda_Slc_Persistent_T *p_slc_persistent)
{
   boolean_T f_obj_ttc_below_threshold;
   float32_T threshold_ttc_lon = p_warn_settings->slc_ttc_thres_lon;
   float32_T threshold_ttc_lat = p_warn_settings->slc_ttc_thres_lat;

   /* Asserts */
   assert(NULL != p_slc_object);
   assert(NULL != p_cals);
   assert(NULL != p_warn_settings);
   assert(NULL != p_slc_persistent);

   if (Lcda_Was_Most_Critical_Slc_Obj_Last_Cycle(p_slc_object, p_slc_persistent))
   {
      threshold_ttc_lon += p_cals->k_slc_critical_lon_ttc_hys;
      threshold_ttc_lat += p_cals->k_slc_critical_lat_ttc_hys;
   }

   /* Check if object is overlapping defined ego area */
   if (Fbk_Is_True(p_slc_object->f_obj_overlap))
   {
      /* Object is overlapping defined ego area -> Only lateral TTC is taken into account */
      f_obj_ttc_below_threshold = (boolean_T) ((p_slc_object->lat_ttc > FBK_ZERO_F) && (p_slc_object->lat_ttc <= threshold_ttc_lat));
   }
   else
   {
      /* Object is not overlapping ego area -> Both lateral and longitudinal TTC are relevant */
      f_obj_ttc_below_threshold =
         (boolean_T) (((p_slc_object->lon_ttc > FBK_ZERO_F) && (p_slc_object->lon_ttc <= threshold_ttc_lon))
                      && ((p_slc_object->lat_ttc > FBK_ZERO_F) && (p_slc_object->lat_ttc <= threshold_ttc_lat)));
   }

   return f_obj_ttc_below_threshold;
}

static boolean_T Lcda_Is_Object_Relevant_For_Slc(const Lcda_Core_Input_T *p_lcda_core_input,
                                                 const Fbk_Object_Data_T *p_tracker_object,
                                                 const Lcda_Core_Calibration_T *p_cals,
                                                 const Lcda_Slc_Persistent_T *p_slc_persistent)
{
   boolean_T f_is_relevant_obj = FBK_FALSE;
   float32_T existence_prob_threshold;

   /* Asserts */
   assert(NULL != p_tracker_object);
   assert(NULL != p_cals);

   /* Get existence probability threshold*/
   existence_prob_threshold = Lcda_Get_Existence_Probability_Threshold(p_lcda_core_input, p_slc_persistent->prev_slc_alert_obj_id,
                                                                       p_tracker_object->id, p_cals);


   if (((PA_OBJ_STATUS_MATURE == p_tracker_object->status) || (PA_OBJ_STATUS_COASTED == p_tracker_object->status))
       && (Fbk_Abs_F(p_tracker_object->curvi_heading) <= p_cals->k_slc_max_curvi_heading_abs)
       && (Fbk_Abs_F(p_tracker_object->curvi_vel.x) >= p_cals->k_slc_min_obj_curvi_long_vel_abs)
       && (p_tracker_object->eclipse_value <= p_cals->k_slc_max_obj_eclipse)
       && (p_tracker_object->existence_probability >= existence_prob_threshold))
   {
      f_is_relevant_obj = FBK_TRUE;
   }

   return f_is_relevant_obj;
}

static float32_T Lcda_Get_Lane_Change_Prob(const float32_T lat_ttc, const Lcda_Core_Calibration_T *p_cals)
{
   float32_T lane_change_prob = LCDA_SLC_PROBABILITY_NONE;

   /* Assert */
   assert(NULL != p_cals);

   /*This part can only be reached if the ttc conditions are met*/
   if ((lat_ttc > FBK_ZERO_F) && (lat_ttc <= (p_cals->k_slc_critical_lat_ttc + p_cals->k_slc_critical_lat_ttc_hys)))
   {
      lane_change_prob = Get_Value_From_2d_Lookup_Table(p_cals->k_slc_lateral_ttc_lookup, p_cals->k_slc_lane_change_prob_lookup,
                                                        LCDA_K_SLC_LATERAL_TTC_LOOKUP_ARRAY_SIZE_DIM0, lat_ttc);
   }

   return lane_change_prob;
}

static boolean_T Lcda_Is_Slc_Object_Behind_Guardrail(const Slc_Object_T *p_slc_object,
                                                     const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES])
{
   boolean_T f_behind_guardrail = FBK_FALSE;
   float32_T lateral_guardrail_position;
   float32_T obj_lateral_inner_edge;
   float32_T side_sign;

   /* Asserts */
   assert(NULL != guardrail_data);

   side_sign = Fbk_Convert_Obj_Side_To_Sign(p_slc_object->ego_side);

   /* get lateral guardrail position, if available */
   lateral_guardrail_position = Lcda_Get_Lateral_Distance_Guardrail(p_slc_object->ego_side, guardrail_data);

   /* calculate lateral position of inner object edge neglecting the object heading */
   obj_lateral_inner_edge = p_slc_object->p_tracker_data->vcs_pos.y - (0.5f * side_sign * p_slc_object->p_tracker_data->width);

   /* object is behind guardrail, if inner object edge is behind the guardrail */
   if ((side_sign * obj_lateral_inner_edge) > (side_sign * lateral_guardrail_position))
   {
      f_behind_guardrail = FBK_TRUE;
   }

   return f_behind_guardrail;
}

static void Lcda_Process_Slc_Output(Lcda_Slc_Core_Output_T *p_slc_core_output,
                                    Lcda_Slc_Persistent_T *p_slc_persistent,
                                    const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_slc_core_output);
   assert(NULL != p_slc_persistent);
   assert(NULL != p_cals);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      /* Alert qualification */
      if (Fbk_Is_True(p_slc_core_output->slc_alert[side_index]))
      {
         /* Qualifying counter increases */
         Sat_Inc_Uint8(&p_slc_persistent->slc_qualifying_counter[side_index]);

         if (p_slc_persistent->slc_qualifying_counter[side_index] <= p_cals->k_slc_alert_qualifying_counter)
         {
            /* Side alert will be suppressed */
            Lcda_Clear_Slc_Core_Output_On_Side(p_slc_core_output, side_index);
         }
      }
      else
      {
         /* Reset qualifying counter */
         p_slc_persistent->slc_qualifying_counter[side_index] = FBK_ZERO_UINT;
      }

      /* Alert holding */
      if (Fbk_Is_True(p_slc_core_output->slc_alert[side_index]))
      {
         /* Reset SLC Alert holding counter */
         p_slc_persistent->slc_hold_counter[side_index] = FBK_ZERO_UINT;
      }
      /* If there is no alert for this cycle, then check if the alert from the previous cycle needs to be held */
      else if ((PA_INVALID_OBJ_ID != p_slc_persistent->prev_slc_alert_obj_id[side_index])
               && (p_slc_persistent->slc_hold_counter[side_index] < p_cals->k_slc_alert_holding_cycles))
      {
         Sat_Inc_Uint8(&(p_slc_persistent->slc_hold_counter[side_index]));

         Lcda_Clear_Slc_Core_Output_On_Side(p_slc_core_output, side_index);
         p_slc_core_output->slc_alert[side_index]     = FBK_TRUE;
         p_slc_core_output->slc_index[side_index]     = p_slc_persistent->prev_slc_alert_obj_index[side_index];
         p_slc_core_output->slc_id[side_index]        = p_slc_persistent->prev_slc_alert_obj_id[side_index];
         p_slc_core_output->slc_unique_id[side_index] = p_slc_persistent->prev_slc_alert_unique_obj_id[side_index];
      }
      else
      {
         /* Reset SLC Alert holding counter */
         p_slc_persistent->slc_hold_counter[side_index] = FBK_ZERO_UINT;
      }
   }
}

static float32_T Lcda_Get_Ego_Overlap_Offset(const float32_T ego_speed, const Lcda_Core_Calibration_T *p_cals)
{
   float32_T ego_overlap_offset;

   /* Assert */
   assert(NULL != p_cals);
   assert(LCDA_K_SLC_LOOKUP_EGO_SPEED_ARRAY_SIZE_DIM0 == LCDA_K_SLC_LOOKUP_EGO_OVERLAP_OFFSET_ARRAY_SIZE_DIM0);

   ego_overlap_offset = Get_Value_From_2d_Lookup_Table(p_cals->k_slc_lookup_ego_speed, p_cals->k_slc_lookup_ego_overlap_offset,
                                                       LCDA_K_SLC_LOOKUP_EGO_OVERLAP_OFFSET_ARRAY_SIZE_DIM0, ego_speed);

   return ego_overlap_offset;
}
