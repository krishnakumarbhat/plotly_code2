/**
 * @file lcda_process_bsw.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides warning in case of an object appears within blind spot area
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_process_bsw.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_functions.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_output.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_common_functions.h"
#include "lcda_create_bsw_zone.h"
#include "lcda_debug_interface.h"
#include "lcda_process_cvw.h"
#include "lcda_types.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* typedefs
\*===========================================================================*/


/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Returns identifier of the object responsible for the bsw warning in the last cycle.
 *
 * @return id of object of type uint8_t
 *
 * @SRS{SF-1067,CSCSA-121708,CSCSA-164366}
 * @SAE{SF-2779}
 * @SDD{SF-6664}
 * @verification{Check whether the identifier is returned correctly when in the last cycle a warning was given.}
 */
static uint8_t Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side(const uint8_t side /**<side index to return object id on*/,
                                                         const Lcda_Bsw_Persistent_T *p_bsw_persistent);

/**
 * @brief Initializes Bsw object data as well as the objects bsw zone.
 *
 * @return void
 *
 * @SRS{SF-1062}
 * @SAE{SF-2779}
 * @SDD{SF-6649}
 * @verification{Check whether bsw object data is reset correctly.}
 */
static void Lcda_Init_Bsw_Object_Data(Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                      const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Checks whether given object is a relevant candidate for Bsw.
 *
 * @return True if object is relevant for bsw.
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{SF-6653}
 * @verification{Create a test where the object is relevant for BSW. Only then True shall be expected.}
 */
static boolean_T Lcda_Is_Object_Relevant_For_Bsw(const Lcda_Core_Input_T *p_lcda_core_input /**<Lcda core input*/,
                                                 const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                                 const Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data*/);

/**
 * @brief Resets object persistent bsw data to its default.
 *
 * @return void
 *
 * @SRS{SF-1056,SF-1062}
 * @SAE{SF-2779}
 * @SDD{SF-6656}
 * @verification{Create a test to check whether bsw object persistent data is reset correctly.}
 */
static void Lcda_Reset_Bsw_Persistent_Object_Data(Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                                  const uint8_t obj_id /**< object identifier */);

/**
 * @brief Resets bsw core output on a given side to its default.
 *
 * @return void
 *
 * @SRS{SF-1069}
 * @SAE{SF-2779}
 * @SDD{SF-6646}
 * @verification{Create a test to check whether bsw core output is reset correctly on given side.}
 */
static void Lcda_Clear_Bsw_Core_Output_On_Side(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                                               const Lcda_Core_Input_T *p_core_input /**< Bsw core input */,
                                               const uint8_t side /**< side index */);

/**
 * @brief Returns the bsw object specific zone which might be either the bsw zone or the bsw zone with hysteresis applied.
 *
 * @return void
 *
 * @SRS{SF-1057,SF-1112,SF-1113,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{SF-6647}
 * @verification{Create tests where either the object was bsw relevant in the last cycle and where an object was not bsw relevant
 * before. In the first case the hysteresis zone is expected. In the second the bare bsw zone shall be returned.}
 */
static void Lcda_Create_Bsw_Object_Zone(Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                        const Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                        const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                        const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                        const Lcda_Cvw_Persistent_T *p_cvw_persistent);
/**
 * @brief Updates the fallback state of the object which might be either a slow fallback or a fast fallback. For the transition of
 *        fallback fast to slow a qualification logic is applied.
 *
 * @return void
 *
 * @SRS{SF-1005,SF-1056,SF-1119,CSCSA-121708,CSCSA-164366}
 * @SAE{SF-2779}
 * @SDD{SF-6659}
 * @verification{Create tests for objects which are meeting the qualification for fast or slow fallback state and check whether the
 * state is returned correctly.}
 */
static void Lcda_Update_Fallback_State(Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                       const Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */);
/**
 * @brief Checks whether the time to leave the bsw zone is greater than a given threshold.
 *
 * @return True when the time to leave bsw zone is greater than a given threshold
 *
 * @SRS{SF-1005,CSCSA-121708,CSCSA-164366}
 * @SAE{SF-2779}
 * @SDD{SF-6651}
 * @verification{Check that the given time to leave the bsw zone is greater than the specified threshold. Only then True shall be
 * expected.}
 */
static boolean_T Lcda_Is_Fallback_Warning_In_Time(const Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                                  const float32_T host_vehicle_length /**< Host length */,
                                                  const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);
/**
 * @brief Checks all bsw criteria are passed so that a Bsw alert can be given for the object.
 *
 * @return True when either a warning is active and when the fallback check is passing or when then fall back check is passing
           as well as the count check, warning in time check, ego lane check and environment check.
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-164365,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{SF-6650}
 * @verification{Create a test to check whether all bsw alert criteria are passing for a given object.}
 */
static boolean_T Lcda_Is_Bsw_Alert_Criteria_Passed(const Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                                   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                                   const Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                                   const Lcda_Object_Location_Data_T *p_obj_loc_data, /**< Object Location data */
                                                   const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */,
                                                   const Lcda_Cvw_Persistent_T *p_cvw_persistent);


/**
 * @brief Sets the most critical bsw object.
 *
 * @return void
 *
 * @SRS{SF-1007,SF-1008,SF-1009,SF-1068,SF-1115,CSCSA-157535,CSCSA-157535}
 * @SAE{SF-2779}
 * @SDD{SF-6657}
 * @verification{Check that the given object is correctly set to the most critical object.}
 */
static void Lcda_Set_Most_Critical_Bsw_Object(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                                              const Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                              const Lcda_Turn_Signal_T turn_signal_held /**< Turn signal state */);

/**
 * @brief Processes the Bsw output with application of the bsw warning holding logic.
 *
 * @return void
 *
 * @SRS{SF-1067}
 * @SAE{SF-2779}
 * @SDD{SF-6654}
 * @verification{Check that for an object which is currently creating a bsw alert no holding counter has increased.}
 */
static void Lcda_Process_Bsw_Output(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                                    Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                    const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                    const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                    const Lcda_Turn_Signal_T turn_signal_held /**< Turn signal state */,
                                    const Fbk_Output_T *p_fbk_output);

/**
 * @brief Maps data from the core output to the bsw persistent structure.
 *
 * @return void
 *
 * @SRS{SF-1067}
 * @SAE{SF-2779}
 * @SDD{SF-6648}
 * @verification{Check that persistent data is updated correctly.}
 */
static void Lcda_Fill_Side_Persistent_Bsw_Data(Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                               const Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */);

/**
 * @brief In case of merged objects, object specific bsw data needs to be shifted.
 *
 * @return void
 *
 * @SRS{SF-1062}
 * @SAE{SF-2779}
 * @SDD{SF-6658}
 * @verification{Create a test where an object merge has occured. The bsw data shall then be available for the new object id.}
 */
static void Lcda_Update_Bsw_Data_For_Merged_Objects(Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                                                    const Fbk_Output_T *p_fbk_output);

/**
 * @brief Checks whether object is behind the guardrail.
 *
 * @return True when object is behind guardrail
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{SF-6652}
 * @verification{Create a test where the object is behind a guardrail. Then true is expected.}
 */
static boolean_T
Lcda_Is_Object_In_Environment_Conflict(const Bsw_Object_T *p_bsw_object /**< Bsw object data */,
                                       const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES] /**< guardrail data */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief Checks whether object fulfills specific front boundary conditions.
 *
 * @return True when object meets TOS and SOT criteria
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{CSCSA-70156}
 * @verification{Create a test where the object is in different location relative to front zone boundary and has different relative
 * velocity}
 */
static boolean_T Lcda_Zone_Front_Boundary_Specific_Conditions(const Bsw_Object_T *p_bsw_object /**< BSW object data */,
                                                              const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief Updates current and previous status of the object, if it is considered as a long or short object.
 *
 * @return status of object of type boolean_T
 *
 * @SRS{SF-1062,CSCSA-136477}
 * @SAE{SF-2779}
 * @SDD{CSCSA-70157}
 * @verification{Check wheteher the status is returned correctly based on recent and previous data.}
 */
static boolean_T Lcda_Is_Object_Considered_Long(Lcda_Bsw_Persistent_T *p_bsw_persistent, /**< Bsw persistent data */
                                                const Bsw_Object_T *p_bsw_object /**< BSW object data */,
                                                const Lcda_Core_Calibration_T *p_cals) /**< Lcda calibration data */;


/**
 * @brief Updates alert validity depending on objects lane change intention.
 *
 * @return False when object is in the ego lane and moving towards ego center (no lane change intention)
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366}
 * @SAE{SF-2779}
 * @SDD{CSCSA-39777}
 * @verification{Check that alert validity is updated correctly when the object changes lane.}
 */
static boolean_T
Lcda_Suppress_Alert_Object_Lane_Change_Intention(const Lcda_Core_Input_T *p_core_input /**< LCDA core input */,
                                                 const boolean_T f_alert_valid_current /**< Current alert status */,
                                                 const boolean_T f_alert_previous_state /**< Previous alert status */,
                                                 const boolean_T f_obj_in_ego_lane /**< Object in ego lane */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                                 const Bsw_Object_T *p_bsw_object /**< BSW object data */,
                                                 const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                 const Lcda_Coordinate_System_T coordinate_system /**< Coordinate System */);

/**
 * @brief Updates alert validity depending on objects position. Check that the object does not protrude beyond the boundaries of
 * the zone
 *
 * @return False when at least part of the object is outside the zone
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{CSCSA-39775}
 * @verification{Check that alert validity is updated correctly if object overhangs zone boundary from host side.}
 */
static boolean_T
Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(const boolean_T f_alert,               /**< Current validity of the object*/
                                               const Lcda_Core_Calibration_T *p_cals, /**< Lcda calibration data */
                                               const Bsw_Object_T *p_bsw_object,      /**< BSW object data */
                                               const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */);

/**
 * @brief Set Final Alert flag for BSW feature.
 *
 * @return False when at least one condition for alert is not met
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-164365,CSCSA-164365,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{CSCSA-27684}
 * @verification{Check that alert validity is updated correctly.}
 */
static boolean_T Lcda_Set_Bsw_Alert_Flag(const Lcda_Core_Input_T *p_core_input,
                                         const boolean_T f_warning_active,
                                         const boolean_T f_count_check_passed,
                                         const boolean_T f_fallback_warning_in_time,
                                         const boolean_T f_obj_in_ego_lane,
                                         const boolean_T f_obj_in_environment_conflict,
                                         const boolean_T f_fallback_check_passed,
                                         const boolean_T f_front_zone_boundary_conditions_passed,
                                         const boolean_T f_obj_overlap_below_threshold,
                                         const boolean_T f_obj_below_max_rel_vel,
                                         const Bsw_Object_T *p_bsw_object,
                                         const Fbk_Vehicle_Data_T *p_vehicle_data,
                                         const Lcda_Core_Calibration_T *p_cals,
                                         const Lcda_Coordinate_System_T coordinate_system);

/**
 * @brief Check if longitudinal velocity of the object is in range for TOS.
 *
 * @return True when velocity below maximum threshold
 *
 * @SRS{SF-1062,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{CSCSA-116577}
 * @verification{Check that max relative longitudinal velocity is verified correctly.}
 */
static boolean_T Lcda_Is_Flyby_Criteria_Passed(const boolean_T f_warning_active,
                                               const float32_T long_rel_vel,
                                               const Lcda_Core_Calibration_T *p_cals);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][p_vehicle_data/p_cals used in debug build only] */
void Lcda_Preprocess_Bsw(Lcda_Bsw_Core_Output_T *p_bsw_core_output, const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals, const Fbk_Output_T *p_fbk_output,
    Lcda_Bsw_Persistent_T* p_bsw_persistent)
/* clang-format on */
{
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_bsw_core_output);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_bsw_persistent);

   Lcda_Update_Bsw_Data_For_Merged_Objects(p_bsw_persistent, p_fbk_output);

   Binary_Lcda_Debug_Pass_Bsw_Default_Zone(p_core_input, &p_fbk_output->p_pa_data->vehicle_data, p_cals);

   /* Initialize data of the most critical object for this cycle */
   for (idx = FBK_ZERO_UINT; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      Lcda_Clear_Bsw_Core_Output_On_Side(p_bsw_core_output, p_core_input, idx);
   }
   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      p_bsw_core_output->f_obj_in_bsw_zone[idx] = FBK_FALSE;
   }
}

void Lcda_Process_Bsw_Object(Lcda_Bsw_Core_Output_T *p_bsw_core_output,
                             const Fbk_Object_Data_T *p_tracker_object,
                             const Lcda_Core_Input_T *p_core_input,
                             const Lcda_Core_Calibration_T *p_cals,
                             const Lcda_Persistent_T *p_lcda_persistent,
                             Lcda_Bsw_Persistent_T *p_bsw_persistent,
                             const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   /* Asserts */
   assert(NULL != p_bsw_core_output);
   assert(NULL != p_tracker_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_lcda_persistent);
   assert(NULL != p_bsw_persistent);

   if (Lcda_Is_Object_Relevant_For_Bsw(p_core_input, p_tracker_object, p_cals, p_bsw_persistent))
   {
      Bsw_Object_T curr_obj;
      Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
      Lcda_Object_Location_Data_T obj_loc_data;

      /* Set common shared parameters as BSW */
      Binary_Lcda_Debug_Pass_Processed_Submodule(LCDA_BSW);

      if (Fbk_Is_True(p_cals->k_bsw_use_curvi_coordinates))
      {
         /* Use curvi coordinates for BSW */
         coordinate_system = LCDA_USE_CURVI;
      }

      /* Initialize BSW object information */
      Lcda_Init_Bsw_Object_Data(&curr_obj, p_tracker_object);

      /* Determine ego side of current object */
      if (LCDA_USE_CURVI == coordinate_system)
      {
         curr_obj.ego_side = Fbk_Get_Obj_Side(curr_obj.p_tracker_data->curvi_pos.y);
      }
      else
      {
         curr_obj.ego_side = Fbk_Get_Obj_Side(curr_obj.p_tracker_data->vcs_pos.y);
      }

      /* Check target length*/
      curr_obj.f_obj_long = Lcda_Is_Object_Considered_Long(p_bsw_persistent, &curr_obj, p_cals);

      /* Create BSW zone */
      Lcda_Create_Bsw_Object_Zone(&curr_obj, p_bsw_persistent, p_core_input, p_cals, p_cvw_persistent);

      /* Set Object Location Data*/
      Lcda_Get_Object_Location_Data(&obj_loc_data, curr_obj.p_tracker_data, &curr_obj.zone, p_cals, coordinate_system);

      /* Check if target object is in zone*/
      curr_obj.f_obj_in_zone                                               = obj_loc_data.obj_in_zone;
      p_bsw_core_output->f_obj_in_bsw_zone[curr_obj.p_tracker_data->index] = curr_obj.f_obj_in_zone;

      if (Fbk_Is_True(curr_obj.f_obj_in_zone))
      {

         /* Check the approximate front bumper position of the object */
         curr_obj.obj_front_position = Lcda_Get_Obj_Front_Position(curr_obj.p_tracker_data);

         Lcda_Update_Fallback_State(p_bsw_persistent, &curr_obj, p_cals, p_core_input);

         /* Increment the counter for object in the BSW zone */
         Lcda_Increment_Mature_Count_In_Zone(&(p_bsw_persistent->mature_count_in_bsw_zone[curr_obj.p_tracker_data->id]),
                                             curr_obj.p_tracker_data->status);

         if (Lcda_Is_Bsw_Alert_Criteria_Passed(&curr_obj, p_core_input, p_cals, p_bsw_persistent, &obj_loc_data, coordinate_system,
                                               p_cvw_persistent))
         {
            /* Check if current object is the most critical object for its side */
            Lcda_Set_Most_Critical_Bsw_Object(p_bsw_core_output, &curr_obj, p_lcda_persistent->turn_signal_held);
         }
      }
      else
      {
         /* track is not in zone, so clear persistent object data */
         Lcda_Reset_Bsw_Persistent_Object_Data(p_bsw_persistent, curr_obj.p_tracker_data->id);
      }

      /* Pass object attributes to debug structure */
      Binary_Lcda_Debug_Pass_Bsw_Object_Attributes(&curr_obj);
   }
   else
   {
      /* track is not valid, clear persistent object data */
      Lcda_Reset_Bsw_Persistent_Object_Data(p_bsw_persistent, p_tracker_object->id);
   }
}


void Lcda_Postprocess_Bsw(Lcda_Bsw_Core_Output_T *p_bsw_core_output,
                          Lcda_Bsw_Persistent_T *p_bsw_persistent,
                          const Lcda_Core_Input_T *p_core_input,
                          const Lcda_Core_Calibration_T *p_cals,
                          const Lcda_Persistent_T *p_lcda_persistent,
                          const Fbk_Output_T *p_fbk_output)
{
   /* Asserts */
   assert(NULL != p_bsw_core_output);
   assert(NULL != p_cals);
   assert(NULL != p_lcda_persistent);

   Lcda_Process_Bsw_Output(p_bsw_core_output, p_bsw_persistent, p_core_input, p_cals, p_lcda_persistent->turn_signal_held,
                           p_fbk_output);

   /* Fill side persistent data */
   Lcda_Fill_Side_Persistent_Bsw_Data(p_bsw_persistent, p_bsw_core_output);

   Binary_Lcda_Debug_Pass_Bsw_Persistent_Data(p_bsw_persistent);
}

void Lcda_Reset_Bsw_Core(Lcda_Bsw_Core_Output_T *p_bsw_core_output,
                         Lcda_Bsw_Persistent_T *p_bsw_persistent,
                         Lcda_Persistent_T *p_lcda_persistent,
                         const Lcda_Core_Input_T *p_core_input)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_bsw_core_output);
   assert(NULL != p_lcda_persistent);

   /* Reset BSW persistent data */
   Lcda_Reset_Bsw_Persistent_Data(p_bsw_persistent);

   /* Clear CVW core output */
   p_bsw_core_output->f_bsw_is_enabled = FBK_FALSE;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Lcda_Clear_Bsw_Core_Output_On_Side(p_bsw_core_output, p_core_input, side);
   }

   p_lcda_persistent->f_bsw_prev_reset = FBK_TRUE;
}

/* coverity[misra_c_2012_rule_8_7_violation][This function is needed by the LCDA debug writer] */
void Lcda_Reset_Bsw_Persistent_Data(Lcda_Bsw_Persistent_T *p_bsw_persistent)
{
   uint8_t side;
   uint8_t obj_id;

   /* Assert */
   assert(NULL != p_bsw_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_bsw_persistent->f_prev_bsw_active[side]            = FBK_FALSE;
      p_bsw_persistent->bsw_hold_counter[side]             = FBK_ZERO_UINT;
      p_bsw_persistent->prev_bsw_alert_obj_id[side]        = PA_INVALID_OBJ_ID;
      p_bsw_persistent->prev_bsw_alert_obj_unique_id[side] = PA_INVALID_OBJ_ID;
   }

   for (obj_id = FBK_ZERO_UINT; obj_id <= PA_OBJ_NUMBER_OF_OBJECTS; obj_id++)
   {
      Lcda_Reset_Bsw_Persistent_Object_Data(p_bsw_persistent, obj_id);
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static uint8_t Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side(const uint8_t side, const Lcda_Bsw_Persistent_T *p_bsw_persistent)
{
   /* Assert */
   assert(side < FBK_NUMBER_OF_SIDES);

   return p_bsw_persistent->prev_bsw_alert_obj_id[side];
}

static void Lcda_Init_Bsw_Object_Data(Bsw_Object_T *p_bsw_object, const Fbk_Object_Data_T *p_tracker_object)
{
   /* Asserts */
   assert(NULL != p_bsw_object);
   assert(NULL != p_tracker_object);

   p_bsw_object->ego_side           = FBK_SIDE_UNDEFINED;
   p_bsw_object->obj_front_position = LCDA_DEFAULT_OBJ_DIST;
   p_bsw_object->f_obj_in_zone      = FBK_FALSE;
   p_bsw_object->f_obj_long         = FBK_FALSE;
   p_bsw_object->p_tracker_data     = p_tracker_object;


   Fbk_Reset_Field_Of_Interest(&p_bsw_object->zone);
}

static boolean_T Lcda_Is_Object_Relevant_For_Bsw(const Lcda_Core_Input_T *p_lcda_core_input,
                                                 const Fbk_Object_Data_T *p_tracker_object,
                                                 const Lcda_Core_Calibration_T *p_cals,
                                                 const Lcda_Bsw_Persistent_T *p_bsw_persistent)
{
   /* Result */
   boolean_T f_is_relevant_obj = FBK_FALSE;

   float32_T existence_prob_threshold;
   boolean_T f_previous_alert_active;
   boolean_T f_valid_track_status;
   boolean_T f_previous_alert_on_new_track;
   boolean_T f_apply_hysteresis;
   boolean_T f_heading_in_range;
   boolean_T f_speed_in_range;
   uint8_t side;

   /* Asserts */
   assert(NULL != p_tracker_object);
   assert(NULL != p_cals);

   /* get side on which target is */
   side = Fbk_Get_Obj_Side(p_tracker_object->curvi_pos.y);

   /* check if a bsw alert was active last cycle on target side */
   f_previous_alert_active = (boolean_T) (PA_INVALID_OBJ_ID != Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side(side, p_bsw_persistent));

   /* check if track status is MATURE or COASTED */
   f_valid_track_status =
      (boolean_T) ((PA_OBJ_STATUS_MATURE == p_tracker_object->status) || (PA_OBJ_STATUS_COASTED == p_tracker_object->status));

   /* check if previously alert was active. If cal-flag is set, this is sufficient to treat a NEW target as bsw relevant */
   f_previous_alert_on_new_track =
      (boolean_T) (Fbk_Is_True(p_cals->k_lcda_allow_track_status_new) && (PA_OBJ_STATUS_NEW == p_tracker_object->status)
                   && Fbk_Is_True(f_previous_alert_active));

   /* Get existence probability threshold*/
   existence_prob_threshold = Lcda_Get_Existence_Probability_Threshold(p_lcda_core_input, p_bsw_persistent->prev_bsw_alert_obj_id,
                                                                       p_tracker_object->id, p_cals);

   /* check if hysteresis should be applied */
   f_apply_hysteresis = (boolean_T) Fbk_Is_True(p_tracker_object->id == p_bsw_persistent->prev_bsw_alert_obj_id[side]);

   /* check if object heading is within range */
   f_heading_in_range = Fbk_Is_Float_In_Given_Range(p_tracker_object->vcs_heading, -p_cals->k_bsw_max_heading_abs,
                                                    p_cals->k_bsw_max_heading_abs, f_apply_hysteresis,
                                                    -p_cals->k_bsw_max_heading_abs_hysteresis,
                                                    p_cals->k_bsw_max_heading_abs_hysteresis);

   /* check if object speed is within range */
   f_speed_in_range = Fbk_Is_Float_In_Given_Range(p_tracker_object->vcs_vel.x, p_cals->k_bsw_min_obj_long_vel,
                                                  p_cals->k_bsw_max_obj_long_vel, f_apply_hysteresis,
                                                  p_cals->k_bsw_min_obj_long_vel_hysteresis,
                                                  p_cals->k_bsw_max_obj_long_vel_hysteresis);

   /* Combine checks to verify if target is a valid bsw target */
   if ((Fbk_Is_True(f_valid_track_status) || Fbk_Is_True(f_previous_alert_on_new_track)) && Fbk_Is_True(f_heading_in_range)
       && Fbk_Is_True(f_speed_in_range) && (p_tracker_object->existence_probability >= existence_prob_threshold))
   {
      f_is_relevant_obj = FBK_TRUE;
   }

   return f_is_relevant_obj;
}

static void Lcda_Reset_Bsw_Persistent_Object_Data(Lcda_Bsw_Persistent_T *p_bsw_persistent, const uint8_t obj_id)
{
   /* Assert */
   assert(NULL != p_bsw_persistent);

   p_bsw_persistent->fallback_state[obj_id]                 = FALLBACK_FAST;
   p_bsw_persistent->mature_count_in_bsw_zone[obj_id]       = FBK_ZERO_UINT;
   p_bsw_persistent->fallback_fast_to_slow_qual_ctr[obj_id] = FBK_ZERO_UINT;
   p_bsw_persistent->f_prev_long_truck_status[obj_id]       = FBK_FALSE;
}

static void Lcda_Clear_Bsw_Core_Output_On_Side(Lcda_Bsw_Core_Output_T *p_bsw_core_output,
                                               const Lcda_Core_Input_T *p_core_input,
                                               const uint8_t side)
{
   /* Assert */
   assert(NULL != p_bsw_core_output);

   p_bsw_core_output->bsw_alert[side]     = LCDA_ALERT_STATE_NONE;
   p_bsw_core_output->bsw_index[side]     = PA_INVALID_OBJ_INDEX;
   p_bsw_core_output->bsw_id[side]        = PA_INVALID_OBJ_ID;
   p_bsw_core_output->bsw_unique_id[side] = PA_INVALID_OBJ_ID;
   p_bsw_core_output->bsw_distance[side]  = LCDA_DEFAULT_OBJ_DIST;
   p_bsw_core_output->bsw_ttp[side]       = LCDA_DEFAULT_LARGE_TTP;
   p_bsw_core_output->bsw_ttle[side]      = LCDA_DEFAULT_LARGE_TTLE;
   p_bsw_core_output->bsw_zone[side]      = p_core_input->initial_bsw_zone;
}

static void Lcda_Create_Bsw_Object_Zone(Bsw_Object_T *p_bsw_object,
                                        const Lcda_Bsw_Persistent_T *p_bsw_persistent,
                                        const Lcda_Core_Input_T *p_core_input,
                                        const Lcda_Core_Calibration_T *p_cals,
                                        const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{

   Fbk_Field_Of_Interest_T bsw_zone;
   Fbk_Field_Of_Interest_T bsw_zone_hys;
   uint8_t side = p_bsw_object->ego_side;
   uint8_t id   = p_bsw_object->p_tracker_data->id;

   /* Asserts */
   assert(NULL != p_bsw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_bsw_persistent);

   /* Create the BSW Zone */
   Lcda_Create_Bsw_Zone(&bsw_zone, &bsw_zone_hys, p_bsw_object, p_core_input, p_cals);

   /* Use the hysteresis zone if the object was previously in the BSW Zone */
   if (((Fbk_Is_True(p_bsw_persistent->f_prev_bsw_active[side]) && (p_bsw_persistent->mature_count_in_bsw_zone[id] > FBK_ZERO_UINT))
        || (Fbk_Is_True(p_cals->k_bsw_uses_cvw_alert_state_enabled) && Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(p_cvw_persistent, id, side)))
       && Fbk_Is_False(p_core_input->f_lane_change[Lcda_Get_Opposite_Side(p_bsw_object->ego_side)]))
   {
      p_bsw_object->zone = bsw_zone_hys;
   }
   else
   {
      p_bsw_object->zone = bsw_zone;
   }
}

static void Lcda_Update_Fallback_State(Lcda_Bsw_Persistent_T *p_bsw_persistent,
                                       const Bsw_Object_T *p_bsw_object,
                                       const Lcda_Core_Calibration_T *p_cals,
                                       const Lcda_Core_Input_T *p_core_input)
{
   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_bsw_persistent);

   /* Check if fallback handling is active */
   if (Fbk_Is_True(p_core_input->enabled_flags.f_fallback_enabled) && (p_bsw_object->p_tracker_data->curvi_vel_rel.x < FBK_ZERO_F))
   {
      /* If enabled, change default fallback status to slow */
      if (Fbk_Is_True(p_cals->k_bsw_f_fallback_default_status_slow) && (p_bsw_object->p_tracker_data->age < 2u))
      {
         p_bsw_persistent->fallback_state[p_bsw_object->p_tracker_data->id] = FALLBACK_SLOW;
      }

      /* determine fallback state */
      switch (p_bsw_persistent->fallback_state[p_bsw_object->p_tracker_data->id])
      {
         case FALLBACK_FAST:
         {
            /* if obj is falling back slowly, it will be classified as FALLBACK_SLOW and an info level will be triggered */
            if (p_bsw_object->p_tracker_data->curvi_vel_rel.x > p_cals->k_bsw_fallback_rel_vel_thres)
            {
               /*Qualify for the fall back state switch from fast to slow*/
               if (p_bsw_persistent->fallback_fast_to_slow_qual_ctr[p_bsw_object->p_tracker_data->id]
                   >= p_cals->k_bsw_fallback_fast_to_slow_qual_thres)
               {
                  p_bsw_persistent->fallback_state[p_bsw_object->p_tracker_data->id] = FALLBACK_SLOW;
               }
               Sat_Inc_Uint8(&(p_bsw_persistent->fallback_fast_to_slow_qual_ctr[p_bsw_object->p_tracker_data->id]));
            }
            break;
         }
         case FALLBACK_SLOW:
         {
            /* if relative velocity decreases further, the object can be be re-classified as FALLBACK_FAST again. The info level
             * will be deactivated */
            /* it is recommended to choose the calibration parameter k_bsw_stag_vel_hys in a way to make the reclassification
               virtually impossible. */
            if (p_bsw_object->p_tracker_data->curvi_vel_rel.x
                < (p_cals->k_bsw_fallback_rel_vel_thres - p_cals->k_bsw_fallback_rel_vel_thres_hys))
            {
               p_bsw_persistent->fallback_state[p_bsw_object->p_tracker_data->id] = FALLBACK_FAST;

               /*Reset fall back from fast to slow counter to its default*/
               p_bsw_persistent->fallback_fast_to_slow_qual_ctr[p_bsw_object->p_tracker_data->id] = FBK_ZERO_UINT;
            }
            break;
         }
         default:
         {
            /* this should never be reached */
            assert(FBK_FALSE);
            break;
         }
      }
   }
   else
   {
      /* Fallback handling is disabled, therefore we flag tracks always as FALLBACK_SLOW (warning relevant) */
      p_bsw_persistent->fallback_state[p_bsw_object->p_tracker_data->id] = FALLBACK_SLOW;
   }
}

static boolean_T Lcda_Is_Fallback_Warning_In_Time(const Bsw_Object_T *p_bsw_object,
                                                  const float32_T host_vehicle_length,
                                                  const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_fallback_in_time = FBK_TRUE;
   float32_T coarse_max_lon_pos_obj;
   float32_T lon_threshold;

   /* Asserts */
   assert(NULL != p_bsw_object);
   assert(NULL != p_cals);

   coarse_max_lon_pos_obj = p_bsw_object->p_tracker_data->vcs_pos.x + (0.5f * p_bsw_object->p_tracker_data->length);
   lon_threshold          = -(p_cals->k_bsw_suppress_late_warning_min_pos_behind_host + host_vehicle_length);

   if ((p_bsw_object->p_tracker_data->vcs_vel_rel.x < p_cals->k_bsw_suppress_late_warning_max_rel_vel)
       && (lon_threshold > coarse_max_lon_pos_obj))
   {
      uint8_t i;
      float32_T time_to_leave_zone;
      float32_T min_zone_lon_pos = p_bsw_object->zone.points[0].x;

      for (i = 1; i < p_bsw_object->zone.size; i++)
      {
         if (p_bsw_object->zone.points[i].x < min_zone_lon_pos)
         {
            min_zone_lon_pos = p_bsw_object->zone.points[i].x;
         }
      }

      /* vcs_vel_rel.x cannot be zero since this code is only reached if it is smaller cal value
       * k_bsw_suppress_late_warning_max_rel_vel (no positive value allowed).
       * We check division by zero anyway. */
      if (Fbk_Abs_F(p_bsw_object->p_tracker_data->vcs_vel_rel.x) > THRESHOLD_IS_ZERO)
      {
         time_to_leave_zone = (min_zone_lon_pos - coarse_max_lon_pos_obj) / p_bsw_object->p_tracker_data->vcs_vel_rel.x;

         if (time_to_leave_zone < p_cals->k_bsw_suppress_late_warning_max_time_till_leave)
         {
            f_fallback_in_time = FBK_FALSE;
         }
      }
   }

   return f_fallback_in_time;
}

static boolean_T Lcda_Is_Bsw_Alert_Criteria_Passed(const Bsw_Object_T *p_bsw_object,
                                                   const Lcda_Core_Input_T *p_core_input,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Bsw_Persistent_T *p_bsw_persistent,
                                                   const Lcda_Object_Location_Data_T *p_obj_loc_data,
                                                   const Lcda_Coordinate_System_T coordinate_system,
                                                   const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   boolean_T f_alert_valid;

   boolean_T f_warning_active;
   boolean_T f_count_check_passed;
   boolean_T f_fallback_check_passed;
   boolean_T f_fallback_warning_in_time;
   boolean_T f_obj_in_environment_conflict;
   boolean_T f_obj_in_ego_lane                       = FBK_FALSE;
   boolean_T f_front_zone_boundary_conditions_passed = FBK_TRUE;
   boolean_T f_obj_overlap_below_threshold           = FBK_FALSE;
   boolean_T f_obj_below_max_rel_vel;

   uint8_t side;
   uint8_t obj_id;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_bsw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);
   assert(NULL != p_bsw_persistent);
   assert(NULL != p_obj_loc_data);

   side   = p_bsw_object->ego_side;
   obj_id = p_bsw_object->p_tracker_data->id;

   p_vehicle_data = &p_core_input->p_pa_data->vehicle_data;

   f_warning_active = (boolean_T) (p_bsw_persistent->f_prev_bsw_active[side]
                                   || (Fbk_Is_True(p_cals->k_bsw_uses_cvw_alert_state_enabled)
                                       && Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(p_cvw_persistent, obj_id, side)));


   f_count_check_passed = (boolean_T) ((p_bsw_persistent->mature_count_in_bsw_zone[obj_id] >= p_cals->k_bsw_min_mature_cycles)
                                       || (p_bsw_object->p_tracker_data->age >= p_cals->k_bsw_alert_track_age));

   f_fallback_check_passed = (boolean_T) (FALLBACK_SLOW == p_bsw_persistent->fallback_state[obj_id]);

   f_fallback_warning_in_time = Lcda_Is_Fallback_Warning_In_Time(p_bsw_object, p_vehicle_data->host_length, p_cals);

   f_obj_in_environment_conflict = Lcda_Is_Object_In_Environment_Conflict(p_bsw_object, p_core_input->guardrail_data, p_cals);

   if (Fbk_Is_True(p_cals->k_lcda_f_enable_obj_in_ego_lane_check))
   {
      f_obj_in_ego_lane =
         Lcda_Is_Object_In_Ego_Lane(p_core_input->lane_width, p_bsw_object->p_tracker_data, p_cals, coordinate_system);
   }

   if (Fbk_Is_True(p_cals->k_bsw_enable_zone_front_boundary_specific_conditions))
   {
      f_front_zone_boundary_conditions_passed = Lcda_Zone_Front_Boundary_Specific_Conditions(p_bsw_object, p_cals);
   }

   if (Fbk_Is_True(p_cals->k_bsw_overlap_area_check_enable)
       && Fbk_Is_True(p_cals->k_lcda_zone_check_method == LCDA_ZONE_CHECK_FOI_OVERLAP))
   {
      f_obj_overlap_below_threshold = (boolean_T) (p_obj_loc_data->area_overlap_ratio < p_cals->k_bsw_overlap_area_threshold);
   }

   f_obj_below_max_rel_vel = Lcda_Is_Flyby_Criteria_Passed(f_warning_active, p_bsw_object->p_tracker_data->vcs_vel_rel.x, p_cals);


   f_alert_valid = Lcda_Set_Bsw_Alert_Flag(p_core_input, f_warning_active, f_count_check_passed, f_fallback_warning_in_time,
                                           f_obj_in_ego_lane, f_obj_in_environment_conflict, f_fallback_check_passed,
                                           f_front_zone_boundary_conditions_passed, f_obj_overlap_below_threshold,
                                           f_obj_below_max_rel_vel, p_bsw_object, p_vehicle_data, p_cals, coordinate_system);

   return f_alert_valid;
}

static void Lcda_Set_Most_Critical_Bsw_Object(Lcda_Bsw_Core_Output_T *p_bsw_core_output,
                                              const Bsw_Object_T *p_bsw_object,
                                              const Lcda_Turn_Signal_T turn_signal_held)
{
   uint8_t side = p_bsw_object->ego_side;

   /* Assert */
   assert(NULL != p_bsw_core_output);
   assert(NULL != p_bsw_object);

   /* The new object is considered most critical if no other critical object has been found or if the object is closer to the ego
    * regarding the longitudinal front bumper position.
    */
   if ((PA_INVALID_OBJ_INDEX == p_bsw_core_output->bsw_index[side])
       || (p_bsw_object->obj_front_position > p_bsw_core_output->bsw_distance[side]))
   {
      p_bsw_core_output->bsw_alert[side]     = Lcda_Get_Alert_State(side, FBK_TRUE, turn_signal_held);
      p_bsw_core_output->bsw_index[side]     = p_bsw_object->p_tracker_data->index;
      p_bsw_core_output->bsw_id[side]        = p_bsw_object->p_tracker_data->id;
      p_bsw_core_output->bsw_unique_id[side] = p_bsw_object->p_tracker_data->unique_id;
      p_bsw_core_output->bsw_distance[side]  = p_bsw_object->obj_front_position;
      p_bsw_core_output->bsw_ttp[side]       = Lcda_Get_Longitudinal_Ttp(p_bsw_object->p_tracker_data, LCDA_USE_VCS);
      p_bsw_core_output->bsw_ttle[side]      = Lcda_Get_Ttle(p_bsw_object->p_tracker_data, &(p_bsw_object->zone), LCDA_USE_VCS);
      p_bsw_core_output->bsw_zone[side]      = p_bsw_object->zone;
   }
}

static void Lcda_Process_Bsw_Output(Lcda_Bsw_Core_Output_T *p_bsw_core_output,
                                    Lcda_Bsw_Persistent_T *p_bsw_persistent,
                                    const Lcda_Core_Input_T *p_core_input,
                                    const Lcda_Core_Calibration_T *p_cals,
                                    const Lcda_Turn_Signal_T turn_signal_held,
                                    const Fbk_Output_T *p_fbk_output)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_bsw_core_output);
   assert(NULL != p_bsw_persistent);
   assert(NULL != p_cals);
   assert(NULL != p_fbk_output);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      if (PA_INVALID_OBJ_INDEX != p_bsw_core_output->bsw_index[side])
      {
         /* Resetting BSW Alert hold counter */
         p_bsw_persistent->bsw_hold_counter[side] = FBK_ZERO_UINT;
      }
      /* If there is no alert for this cycle, then check if the alert from the previous cycle needs to be held */
      else if (Fbk_Is_True(p_bsw_persistent->f_prev_bsw_active[side])
               && (p_bsw_persistent->bsw_hold_counter[side] < p_cals->k_bsw_alert_holding_cycles))
      {
         Sat_Inc_Uint8(&(p_bsw_persistent->bsw_hold_counter[side]));

         Lcda_Clear_Bsw_Core_Output_On_Side(p_bsw_core_output, p_core_input, side);
         p_bsw_core_output->bsw_alert[side] = Lcda_Get_Alert_State(side, p_bsw_persistent->f_prev_bsw_active[side], turn_signal_held);
         p_bsw_core_output->bsw_id[side]    = p_bsw_persistent->prev_bsw_alert_obj_id[side];
         p_bsw_core_output->bsw_unique_id[side] = p_bsw_persistent->prev_bsw_alert_obj_unique_id[side];
         p_bsw_core_output->bsw_index[side] =
            Fbk_Get_Object_Index_From_Id(p_fbk_output->p_index_id_lookup_table, p_bsw_core_output->bsw_id[side]);
      }
      else
      {
         /* Resetting BSW Alert hold counter */
         p_bsw_persistent->bsw_hold_counter[side] = FBK_ZERO_UINT;
      }
   }
}

static void Lcda_Fill_Side_Persistent_Bsw_Data(Lcda_Bsw_Persistent_T *p_bsw_persistent, const Lcda_Bsw_Core_Output_T *p_bsw_core_output)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_bsw_persistent);
   assert(NULL != p_bsw_core_output);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Save the BSW outputs for the next run */
      p_bsw_persistent->prev_bsw_alert_obj_id[side]        = p_bsw_core_output->bsw_id[side];
      p_bsw_persistent->prev_bsw_alert_obj_unique_id[side] = p_bsw_core_output->bsw_unique_id[side];
      p_bsw_persistent->f_prev_bsw_active[side]            = Lcda_Is_Alert_On(p_bsw_core_output->bsw_alert[side]);
   }
}

static void Lcda_Update_Bsw_Data_For_Merged_Objects(Lcda_Bsw_Persistent_T *p_bsw_persistent, const Fbk_Output_T *p_fbk_output)
{
   uint8_t id;
   uint8_t obj_idx;
   uint8_t merged_obj_id;

   /* Assert */
   assert(NULL != p_bsw_persistent);
   assert(NULL != p_fbk_output);

   /* If tracker objects were merged in this cycle, then copy over the corresponding BSW info */
   for (id = FBK_ZERO_UINT; id < PA_OBJ_NUMBER_OF_OBJECTS; id++)
   {
      const Fbk_Object_Data_T *p_object_data;
      obj_idx = Fbk_Get_Object_Index_From_Id(p_fbk_output->p_index_id_lookup_table, id);
      if (obj_idx != PA_INVALID_OBJ_INDEX)
      {
         p_object_data = &p_fbk_output->p_pa_data->object_data[obj_idx];
         if (Fbk_Is_True(p_object_data->f_merge_occured))
         {
            merged_obj_id = p_object_data->id_merged_obj;

            if ((PA_INVALID_OBJ_ID != merged_obj_id)
                && (p_bsw_persistent->mature_count_in_bsw_zone[merged_obj_id] > p_bsw_persistent->mature_count_in_bsw_zone[id]))
            {
               p_bsw_persistent->mature_count_in_bsw_zone[id] = p_bsw_persistent->mature_count_in_bsw_zone[merged_obj_id];
            }
         }
      }
   }
}

static boolean_T Lcda_Is_Object_In_Environment_Conflict(const Bsw_Object_T *p_bsw_object,
                                                        const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES],
                                                        const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_guardrail_conflict = FBK_FALSE;
   float32_T lateral_guardrail_position;
   float32_T obj_lateral_outer_edge;
   float32_T side_sign;

   /* Asserts */
   assert(NULL != p_bsw_object);
   assert(NULL != guardrail_data);
   assert(NULL != p_cals);

   side_sign = Fbk_Convert_Obj_Side_To_Sign(p_bsw_object->ego_side);

   /* calculate lateral guardrail position including safety margin */
   lateral_guardrail_position = Lcda_Get_Lateral_Distance_Guardrail(p_bsw_object->ego_side, guardrail_data)
                                + (side_sign * p_cals->k_bsw_guardrail_distance_safety_margin);

   /* calculate lateral position of outer object edge neglecting the object heading */
   obj_lateral_outer_edge = p_bsw_object->p_tracker_data->vcs_pos.y + (0.5f * side_sign * p_bsw_object->p_tracker_data->width);

   /* guardrail conflict is detected, if outer object edge is behind the guardrail (including safety margin) */
   if ((side_sign * obj_lateral_outer_edge) > (side_sign * lateral_guardrail_position))
   {
      f_guardrail_conflict = FBK_TRUE;
   }

   return f_guardrail_conflict;
}

static boolean_T Lcda_Zone_Front_Boundary_Specific_Conditions(const Bsw_Object_T *p_bsw_object, const Lcda_Core_Calibration_T *p_cals)
{

   boolean_T f_alert_criteria_passed = FBK_TRUE;

   float32_T long_zone_min          = p_bsw_object->zone.points[0].x;
   float32_T long_zone_max          = p_bsw_object->zone.points[0].x;
   float32_T lat_zone_max           = p_bsw_object->zone.points[0].y;
   float32_T bsw_front_limit        = long_zone_max;
   float32_T obj_long_vel_rel       = p_bsw_object->p_tracker_data->vcs_vel_rel.x;
   float32_T obj_curvi_long_vel_rel = p_bsw_object->p_tracker_data->curvi_vel_rel.x;
   float32_T corrected_delay_time   = p_cals->k_bsw_object_position_correction_delay_time[FBK_ZERO_UINT];
   float32_T sot_n_line             = p_cals->k_bsw_n_line_position_for_long_object_sot_scenario;

   /*assign a type of the front limit for bsw zone that will be used*/
   switch (p_cals->k_bsw_stop_alert_reaching_front_custom_limit_mode)
   {
      case (uint8_t) BSW_FRONT:
         Lcda_Get_Zone_Maxima(&long_zone_min, &long_zone_max, &lat_zone_max, &p_bsw_object->zone);
         bsw_front_limit = long_zone_max;
         break;
      case (uint8_t) HOST_FRONT:
         bsw_front_limit = FBK_ZERO_F;
         break;
      case (uint8_t) CUSTOM:
         bsw_front_limit = p_cals->k_bsw_line_to_stop_TOS_alert;
         break;
      case (uint8_t) COMPENSATED:

         if (p_bsw_object->p_tracker_data->vcs_vel_rel.x > p_cals->k_bsw_object_position_correction_threshold)
         {
            corrected_delay_time = p_cals->k_bsw_object_position_correction_delay_time[FBK_ONE_UINT];
         }
         bsw_front_limit = -(corrected_delay_time * obj_curvi_long_vel_rel);
         break;

      default:
         /* this should never be reached */
         assert(FBK_FALSE);
         break;
   }

   /* TOS execute only when the target is overtaking a host and crossed the custom front boundary*/
   if ((obj_long_vel_rel >= p_cals->k_bsw_min_speed_for_tos_scenario) && (p_bsw_object->obj_front_position >= bsw_front_limit))
   {
      f_alert_criteria_passed = FBK_FALSE;

      /* Thunder specific requirement to distinguish a bsw alert behaviour for different/predefined types of objects (short and
      long), hysteresis applied to the object length to prevent the alert from toggling (caused by changing tracker data) */
      if (Fbk_Is_True(p_cals->k_bsw_hold_alert_long_object))
      {
         /* For long object hold bsw alert until whole object leave the zone, for short object take bsw alert off when custom front
          * boundary is crossed */
         if (Fbk_Is_True(p_bsw_object->f_obj_long))
         {
            f_alert_criteria_passed = FBK_TRUE;
         }
      }
   }


   /* SOT execute only when subject is overtaking target, alert would be triggered when all of the object is within the zone*/
   if (Fbk_Is_True(p_cals->k_bsw_enable_specific_front_sot_conditions) && (obj_long_vel_rel < FBK_ZERO_F))
   {
      if (Fbk_Is_True(p_cals->k_bsw_f_use_front_zone_as_n_line))
      {
         sot_n_line = long_zone_max;
      }

      f_alert_criteria_passed =
         (boolean_T) ((p_bsw_object->obj_front_position < long_zone_max)
                      || ((p_bsw_object->f_obj_long
                           && ((p_bsw_object->obj_front_position - p_bsw_object->p_tracker_data->length) < sot_n_line))));
   }

   return f_alert_criteria_passed;
}

static boolean_T Lcda_Is_Object_Considered_Long(Lcda_Bsw_Persistent_T *p_bsw_persistent,
                                                const Bsw_Object_T *p_bsw_object,
                                                const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t obj_id          = p_bsw_object->p_tracker_data->id;
   boolean_T f_long_status = FBK_FALSE;

   /* long object */
   if ((p_bsw_object->p_tracker_data->length >= p_cals->k_bsw_min_length_long_object)
       || (Fbk_Is_True(p_bsw_persistent->f_prev_long_truck_status[obj_id])
           && (p_bsw_object->p_tracker_data->length
               > (p_cals->k_bsw_min_length_long_object - p_cals->k_bsw_min_length_long_object_hys))))
   {
      f_long_status = FBK_TRUE;
   }
   /* short object */
   if ((Fbk_Is_False(p_bsw_persistent->f_prev_long_truck_status[obj_id])
        && (p_bsw_object->p_tracker_data->length < p_cals->k_bsw_min_length_long_object)))
   {
      f_long_status = FBK_FALSE;
   }

   p_bsw_persistent->f_prev_long_truck_status[obj_id] = f_long_status;

   return f_long_status;
}

static boolean_T Lcda_Suppress_Alert_Object_Lane_Change_Intention(const Lcda_Core_Input_T *p_core_input,
                                                                  const boolean_T f_alert_valid_current,
                                                                  const boolean_T f_alert_previous_state,
                                                                  const boolean_T f_obj_in_ego_lane,
                                                                  const Lcda_Core_Calibration_T *p_cals,
                                                                  const Bsw_Object_T *p_bsw_object,
                                                                  const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                  const Lcda_Coordinate_System_T coordinate_system)
{
   float32_T side_sign;
   float32_T object_lat_pos;
   float32_T object_lat_rel_vel;
   float32_T zone_lat_inner_margin;
   float32_T zone_lat_overlap;
   float32_T obj_outer_side_pos;
   float32_T dist_bumpers;
   boolean_T f_alert_valid_updated = f_alert_valid_current;
   boolean_T f_adv_pos_suppress    = FBK_TRUE;
   boolean_T f_lat_overlap_above_thresh, f_long_dist_bumpers_below_thresh, f_vel_lat_above_thresh;


   assert(NULL != p_cals);
   assert(NULL != p_bsw_object);
   assert(NULL != p_vehicle_data);


   /* If enabled, suppress alert when object is in ego lane and moving towards lateral ego center */
   if ((Fbk_Is_True(f_alert_valid_updated) || Fbk_Is_True(f_alert_previous_state)) && Fbk_Is_True(f_obj_in_ego_lane)
       && Fbk_Is_True(p_cals->k_lcda_f_enable_suppress_alert_object_no_lane_change_intention))
   {

      if (LCDA_USE_VCS == coordinate_system)
      {
         object_lat_pos     = p_bsw_object->p_tracker_data->vcs_pos.y;
         object_lat_rel_vel = p_bsw_object->p_tracker_data->vcs_vel_rel.y;
      }
      else
      {
         object_lat_pos     = p_bsw_object->p_tracker_data->curvi_pos.y;
         object_lat_rel_vel = p_bsw_object->p_tracker_data->curvi_vel_rel.y;
      }

      side_sign = Fbk_Get_Obj_Side_Sign(object_lat_pos);

      /* If enabled extends logic for lane change intention alert suppress, it considers longitudinal distance between front bumper
       * of the target and rear bumper of the host, lateral zone overlaping ratio, and lateral velocity */
      if (Fbk_Is_True(p_cals->k_bsw_f_enable_adv_pos_data_lane_change_intention))
      {
         obj_outer_side_pos = Fbk_Abs_F(object_lat_pos) + Fbk_Half(p_bsw_object->p_tracker_data->width);

         zone_lat_inner_margin = (Fbk_Is_True(p_cals->k_bsw_f_use_zone_without_hysteresis_lane_change_intention))
                                    ? (p_cals->k_bsw_y0 + Fbk_Half(p_vehicle_data->host_width))
                                    : Fbk_Abs_F(p_bsw_object->zone.points[REAR_EGO_SIDE].y);

         zone_lat_overlap = (obj_outer_side_pos - zone_lat_inner_margin) / p_bsw_object->p_tracker_data->width;
         dist_bumpers = Fbk_Abs_F(p_bsw_object->obj_front_position) - p_vehicle_data->host_length - p_core_input->trailer.length;

         f_lat_overlap_above_thresh       = (boolean_T) (zone_lat_overlap > p_cals->k_bsw_lane_change_intention_pos_lat_thres);
         f_long_dist_bumpers_below_thresh = (boolean_T) (dist_bumpers < p_cals->k_bsw_lane_change_intention_pos_long_thres);

         f_vel_lat_above_thresh = (boolean_T) ((side_sign * object_lat_rel_vel) > p_cals->k_lcda_lane_change_intention_vel_lat_thresh);
         if (Fbk_Is_True(f_alert_previous_state))
         {
            f_vel_lat_above_thresh =
               (boolean_T) ((side_sign * object_lat_rel_vel) > (p_cals->k_lcda_lane_change_intention_vel_lat_thresh
                                                                - p_cals->k_bsw_lane_change_intention_vel_lat_hys));
         }


         f_adv_pos_suppress = (boolean_T) ((f_lat_overlap_above_thresh && f_long_dist_bumpers_below_thresh) || f_vel_lat_above_thresh);
      }
      /* If conditions are met the alert would be suppressed -> alert_valid = False */
      if (Fbk_Is_False(f_adv_pos_suppress))
      {
         f_alert_valid_updated = FBK_FALSE;
      }
   }
   return f_alert_valid_updated;
}

static boolean_T Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(const boolean_T f_alert,
                                                                const Lcda_Core_Calibration_T *p_cals,
                                                                const Bsw_Object_T *p_bsw_object,
                                                                const Lcda_Coordinate_System_T coordinate_system)
{
   Fbk_Object_Corners_T target_corners;
   Vector_2d_T p_object_lat_pos;
   float32_T p_object_heading;
   float32_T front_corner_lat;
   float32_T rear_corner_lat;
   boolean_T f_alert_valid_updated = f_alert;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_bsw_object);
   assert(NULL != p_cals);

   if ((Fbk_Is_True(p_cals->k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge)) && (Fbk_Is_True(f_alert_valid_updated)))
   {
      /* Calculate corners of the target depending on the used coordinate system */
      if (LCDA_USE_VCS == coordinate_system)
      {
         p_object_lat_pos = p_bsw_object->p_tracker_data->vcs_pos;
         p_object_heading = p_bsw_object->p_tracker_data->vcs_heading;
      }
      else
      {
         p_object_lat_pos = p_bsw_object->p_tracker_data->curvi_pos;
         p_object_heading = p_bsw_object->p_tracker_data->curvi_heading;
      }

      Fbk_Calculate_Target_Corners(&target_corners, &p_object_lat_pos, &p_object_heading, &p_bsw_object->p_tracker_data->length,
                                   &p_bsw_object->p_tracker_data->width);

      /* Define reference corners (on the side closest to the ego) which will be used to verify zone occupance  */
      if (FBK_SIDE_LEFT == p_bsw_object->ego_side)
      {
         front_corner_lat = Fbk_Abs_F(target_corners.points[FBK_FRONT_RIGHT_CORNER].y);
         rear_corner_lat  = Fbk_Abs_F(target_corners.points[FBK_REAR_RIGHT_CORNER].y);
      }
      else
      {
         front_corner_lat = Fbk_Abs_F(target_corners.points[FBK_FRONT_LEFT_CORNER].y);
         rear_corner_lat  = Fbk_Abs_F(target_corners.points[FBK_REAR_LEFT_CORNER].y);
      }

      /* Verify if any reference corner of the target is out of the zone */
      if ((Fbk_Abs_F(p_bsw_object->zone.points[FRONT_EGO_SIDE].y) > front_corner_lat)
          || (Fbk_Abs_F(p_bsw_object->zone.points[FRONT_EGO_SIDE].y) > rear_corner_lat))
      {
         f_alert_valid_updated = FBK_FALSE;
      }
   }

   return f_alert_valid_updated;
}

static boolean_T Lcda_Set_Bsw_Alert_Flag(const Lcda_Core_Input_T *p_core_input,
                                         const boolean_T f_warning_active,
                                         const boolean_T f_count_check_passed,
                                         const boolean_T f_fallback_warning_in_time,
                                         const boolean_T f_obj_in_ego_lane,
                                         const boolean_T f_obj_in_environment_conflict,
                                         const boolean_T f_fallback_check_passed,
                                         const boolean_T f_front_zone_boundary_conditions_passed,
                                         const boolean_T f_obj_overlap_below_threshold,
                                         const boolean_T f_obj_below_max_rel_vel,
                                         const Bsw_Object_T *p_bsw_object,
                                         const Fbk_Vehicle_Data_T *p_vehicle_data,
                                         const Lcda_Core_Calibration_T *p_cals,
                                         const Lcda_Coordinate_System_T coordinate_system)
{

   boolean_T f_alert_valid;
   boolean_T f_helper_for_branch_cov;
   boolean_T f_obj_not_in_lane;

   /* Combine all checks. */
   f_obj_not_in_lane       = (boolean_T) (Fbk_Is_False(f_obj_in_ego_lane) || p_cals->k_lcda_f_enable_alert_obj_in_ego_lane);
   f_helper_for_branch_cov = (boolean_T) (f_warning_active
                                          || (f_count_check_passed && f_fallback_warning_in_time && f_obj_not_in_lane
                                              && Fbk_Is_False(f_obj_in_environment_conflict)));

   f_alert_valid = (boolean_T) (f_helper_for_branch_cov && f_fallback_check_passed && f_front_zone_boundary_conditions_passed
                                && Fbk_Is_False(f_obj_overlap_below_threshold) && f_obj_below_max_rel_vel);

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(p_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane,
                                                                    p_cals, p_bsw_object, p_vehicle_data, coordinate_system);
   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, p_cals, p_bsw_object, coordinate_system);

   return f_alert_valid;
}

static boolean_T Lcda_Is_Flyby_Criteria_Passed(const boolean_T f_warning_active,
                                               const float32_T long_rel_vel,
                                               const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T vel_below_thresh = FBK_FALSE;
   float32_T vel_hys          = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_cals);

   if (Fbk_Is_True(f_warning_active))
   {
      vel_hys = p_cals->k_bsw_obj_max_rel_vel_hys;
   }

   /* absolute value not used intentionaly, for SOT scenario it is covered by fallback logic */
   if (long_rel_vel < (p_cals->k_bsw_obj_max_rel_vel_thresh + vel_hys))
   {
      vel_below_thresh = FBK_TRUE;
   }

   return vel_below_thresh;
}
