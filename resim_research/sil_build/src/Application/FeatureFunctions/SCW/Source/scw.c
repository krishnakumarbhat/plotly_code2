/**
 * @file scw.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the functions that are called by the platform SW
 * for initialization, cyclic execution and obtaining outputs of the SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "ml_polygon.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_debug_interface.h"
#include "scw_helper_functions.h"
#include "scw_persistent_t.h"
#include "scw_types.h"
#include <assert.h>

/*===========================================================================*\
* Function-like Macros
\*===========================================================================*/

/**
 * @brief Checks whether input value 'v' is in specified range <min, max>, including the hysteresis 'hyst', added to 'min'
 * and subtracted from 'max' if 'woor' == true (was OUT of range previously).
 *
 * @return True when the input value is in specified range
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
#define Scw_In_Range_Hyst(v, min, max, hyst, woor) (Fbk_Is_Float_In_Given_Range((v), (min), (max), (woor), (hyst), -(hyst)))


/**
 * @brief Checks whether input value 'v' is below the threshold 'th', including the hysteresis 'hyst', subtracted from 'th'
 * if 'wat' == true (was above the threshold previously).
 * !!! Only for v >= 0 because of min = FBK_ZERO_F !!!
 *
 * @return True when the input value is below specified threshold
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
#define Scw_Below_Threshold_Hyst(v, th, hyst, wat) (Fbk_Is_Float_In_Given_Range((v), FBK_ZERO_F, (th), (wat), FBK_ZERO_F, -(hyst)))


/**
 * @brief Checks whether input value 'v' is below the threshold 'th', including the hysteresis 'hyst', added to 'th'
 * if 'wbt' == true (was below the threshold previously).
 *
 * @return True when the input value is below specified threshold
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
#define Scw_Below_Threshold_Add_Hyst(v, th, hyst, wbt) \
   (Fbk_Is_Float_In_Given_Range(Fbk_Max((v), FBK_ZERO_F), FBK_ZERO_F, (th), (wbt), FBK_ZERO_F, (hyst)))


/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Creates the SCW zones from the calibration values.
 *
 * @return void
 *
 * @SRS{SF-2117,SF-2118}
 * @SAE{SF-2990}
 * @SDD{SF-8028}
 * @verification{Create a test to check if scw zones are created properly}
 */
static void Scw_Create_Zones(Fbk_Field_Of_Interest_T *p_scw_zone /**< Scw initial zone */,
                             Fbk_Field_Of_Interest_T *p_scw_hysteresis_zone /**< Scw hysteresis zone */,
                             const Scw_Core_Input_T *p_scw_core_input /**< Scw Core input */,
                             const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibrations */,
                             const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);


/**
 * @brief Checks the properties of the given dynamic object.
 *
 * @return True if it is a valid candidate for the SCW function
 *
 * @SRS{SF-2126,SF-2092}
 * @SAE{SF-2990}
 * @SDD{SF-8036}
 * @verification{Create a test to check if dynamic object is validated properly}
 */
static boolean_T Scw_Is_Dyn_Object_Valid(const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                         const uint8_t obj_index /**< object index */,
                                         const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */,
                                         const Scw_Persistent_T *const p_scw_persistent /**< Scw persistent data */);


/**
 * @brief This function checks the properties of the given dynamic object.
 *
 * @return True if it is a relevant candidate for the SCW function
 *
 * @SRS{SF-2091}
 * @SAE{SF-2990}
 * @SDD{SF-8035}
 * @verification{Create a test to check if dynamic object is correctly assessed as relevant}
 */
static boolean_T Scw_Is_Dyn_Object_Relevant(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                            const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                            const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */,
                                            Scw_Persistent_T *const p_scw_persistent /**< Scw persistent data */,
                                            const uint8_t side /**< approach side */);


/**
 * @brief Checks whether the object is in environment conflict due to guardrails.
 *
 * @return True when object is in environment conflict
 *
 * @SRS{SF-2134}
 * @SAE{SF-2990}
 * @SDD{SF-8033}
 * @verification{Create a test to check if the conflict of dynamic object with the environment is detected}
 */
static boolean_T Scw_Is_Dyn_Object_In_Environment_Conflict(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                                           const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                                           const uint8_t side /**< approach side */,
                                                           const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */);


/**
 * @brief Checks whether object velocity is in valid range.
 *
 * @return True when objects velocity is in permissible range
 *
 * @SRS{SF-2133,SF-2129}
 * @SAE{SF-2990}
 * @SDD{SF-8037}
 * @verification{Create a test to check if the velocity (relative and absolute) of dynamic object is evaluated properly}
 */
static boolean_T Scw_Is_Dyn_Object_Velocity_Correct(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                                    const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */,
                                                    Scw_Persistent_T *const p_scw_persistent /**< Scw persistent data */);


/**
 * @brief Checks whether object heading is in valid range.
 *
 * @return True when objects heading is in permissible range
 *
 * @SRS{SF-2091}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121175}
 * @verification{Create the test to check if the heading is detected in permissible range}
 */
static boolean_T Scw_Is_Dyn_Object_Heading_Correct(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                                   const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */,
                                                   Scw_Persistent_T *const p_scw_persistent /**< Scw persistent data */);


/**
 * @brief Checks whether object yawrate is in valid range.
 *
 * @return True when objects yawrate is in permissible range
 *
 * @SRS{CSCSA-270041}
 * @SAE{SF-2990}
 * @SDD{CSCSA-267748}
 * @verification{Create the test to check if the yawrate is detected in permissible range}
 */
static boolean_T Scw_Is_Dyn_Object_Yawrate_Correct(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                                   const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */,
                                                   Scw_Persistent_T *const p_scw_persistent /**< Scw persistent data */);


/**
 * @brief Calculates lateral distance between the object and the host, metal to metal.
 *
 * @return Lateral distance
 *
 * @SRS{CSCSA-121086}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121167}
 * @verification{Create a test to check if dynamic object lateral distance is calculated correctly}
 */
static float32_T Scw_Get_Dyn_Object_Lateral_Distance(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                                     const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */);


/**
 * @brief Universal function which calculates different kinds of TTx (Time to... collision, pass, etc.) of the dynamic object or
 * the guardrail.
 *
 * @return TTx
 *
 * @SRS{CSCSA-137484,CSCSA-137489,CSCSA-137483}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121170}
 * @verification{Create a test to check if dynamic object TTx is calculated correctly for particular mode}
 */
static float32_T
Scw_Get_TTx(const float32_T
               distance /**< distance between source and destianation, e.g. laterally nearest point of the target and the host */,
            const float32_T relative_velocity /**< relative velocity of source */,
            const Scw_TTx_Calculation_Mode_T mode /**< calculation mode */,
            const float32_T default_value_div_0 /**< default value of TTx when relative velocity == 0 or distance < 0 */,
            const float32_T default_value /**< default value when all conditions fails */);


/**
 * @brief Finds the most critical dynamic object (1 for each side). Main steps are:
 * For each tracker output object do the following:
 * 1. Check if it passes the the Is_Valid_Dyn_Candidate() check
 * 2. Determine on which side the object lies using the object lateral position
 * 3. Determine the zone to use, left/right (use the hysteresis zone if the object is already within the initial_zone at least
 *once)
 * 4. Check if the object center point is within the zone. If yes, then increment persistent count_in_zone_dyn
 * 5. If object is within the zone, then determine if it is the most critical object by calling Scw_Get_Most_Critical_Dyn_Obj()
 * 6. If object is valid, within the zone and most critical object for at least k_scw_candidate_mature_cycles_in_zone_threshold
 * then it is a critical object for this cycle
 *
 * @return void
 *
 * @SRS{SF-2136}
 * @SAE{SF-2990}
 * @SDD{SF-8030}
 * @verification{Create a test to check if the critical objects for each side were found correctly}
 */
static void
Scw_Get_Critical_Dynamic_Objs(Scw_Critical_Object_T critical_dyn_obj[FBK_NUMBER_OF_SIDES] /**< critical objects on both sides */,
                              Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                              const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                              const Fbk_Field_Of_Interest_T *const p_scw_zone /**< Scw initial zone */,
                              const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone /**< Scw hysteresis zone */,
                              const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */);


/**
 * @brief Given the indices of 2 objects, this function decides which is more critical and returns it's critical data.
 * The rules are:
 * 1. new_obj is more critical if it is closer laterally to the ego than curr_critical_obj
 * 2. If both new_obj and curr_critical_obj are at the same distance laterally from the ego, then the object that is
 * closer longitudinally (behind) the ego is chosen as the most critical object.
 *
 * @return index of most critical object of type uint8_t
 *
 * @SRS{SF-2121,SF-2132}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121169}
 * @verification{Create a test to check if the most critical dynamic object is chosen correctly}
 */
static void Scw_Get_Most_Critical_Dyn_Obj(Scw_Critical_Object_T *const p_curr_critical_obj /**< most critical object (one side) */,
                                          const Scw_Object_T *const p_scw_object /**< Scw internal object */);


/**
 * @brief Finds critical guardrails (1 for each side).
 *
 * @return void
 *
 * @SRS{SF-2137}
 * @SAE{SF-2990}
 * @SDD{CSCSA-125683}
 * @verification{Create a test to check if the critical guardrails for each side were found correctly}
 */
static void
Scw_Get_Critical_Guardrails(Scw_Critical_Object_T critical_guardrail[FBK_NUMBER_OF_SIDES] /**< critical guardrails on both sides */,
                            Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                            const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                            const Fbk_Vehicle_Data_T *const p_vehicle_data /**< Vehicle data */,
                            const Fbk_Field_Of_Interest_T *const p_scw_zone /**< Scw initial zone */,
                            const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone /**< Scw hysteresis zone */,
                            const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */);


/**
 * @brief Returns true if the given guardrail is within the SCW zone. The main steps are:
 * 1. Check if the guardrail satisfies the Scw_Is_Valid_Guardrail() criteria
 * 2. Determine on which side the guardrail lies using the lateral position
 * 3. Determine the zone to use, left/right (use the hysteresis zone if the guardrail is already within the initial_zone at least
 * once)
 * 4. Check if the guardrail ref point is within the SCW zone. Ref point x = half ego length; y = guardrail lateral pos
 * 5. The guardrail is critical if it is valid and within the zone for at least k_scw_guardrail_cycles_in_zone_threshold
 *
 * @return True when guardrail is critical
 *
 * @SRS{SF-2130,SF-2120,SF-2137}
 * @SAE{SF-2990}
 * @SDD{SF-8039}
 * @verification{Create a test to check if guardrail is correctly assessed as critical}
 */
static boolean_T Scw_Is_Guardrail_Critical(Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                                           const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                           const Fbk_Vehicle_Data_T *const p_vehicle_data /**< FBK vehicle data */,
                                           const Fbk_Field_Of_Interest_T *const p_scw_zone /**< Scw initial zone */,
                                           const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone /**< Scw hysteresis zone */,
                                           const uint8_t side /**< approach side */,
                                           const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */);


/**
 * @brief Returns true when the given guardrail is a valid candidate for the SCW function
 *
 * @return True when guardrail is critical
 *
 * @SRS{SF-2134}
 * @SAE{SF-2990}
 * @SDD{SF-8040}
 * @verification{Create a test to check if the guardrail is validated properly}
 */
static boolean_T Scw_Is_Valid_Guardrail(const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                        const uint8_t side /**< approach side */,
                                        const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibrations */);


/**
 * @brief Calculates lateral distance between the guardrail and the host, metal to metal.
 *
 * @return Lateral distance
 *
 * @SRS{CSCSA-121088}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121173}
 * @verification{Create a test to check if the guardrail lateral distance is calculated correctly}
 */
static float32_T Scw_Get_Guardrail_Lateral_Distance(const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                                    const uint8_t side /**< side */);


/**
 * @brief Main core algorithm. The main steps are:
 * 1. Build the SCW initial and hysteresis zones on both sides of the ego
 * 2. If dynamic objects are enabled, then it calls the function to get the critical dynamic objects (1 for each side)
 * 3. For both sides, if no critical dynamic object is found, then it checks if there is a critical guardrail on the corresponding
 * side
 * 4. If no critical object (dynamic or guardrail) is found, then the output is cleared and set to default values
 *
 * @return void
 *
 * @SRS{SF-2114,SF-2127,SF-2119,SF-2122,SF-2121,SF-2132,CSCSA-121085,CSCSA-121086,CSCSA-121087,CSCSA-121088,CSCSA-121089,CSCSA-121091}
 * @SAE{SF-2990}
 * @SDD{SF-8045}
 * @verification{Create a test to check if the main scw function is working properly}
 */
static void Scw_Algorithm(Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
                          Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                          const Scw_Core_Input_T *p_scw_core_input /**< Scw core input */,
                          const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibrations */);


/**
 * @brief Determines the Crtiticality Level of the Alert based on Lateral Distance and Lateral TTC of the Critical Object or
 * Guardrail.
 *
 * @return Criticality level
 *
 * @SRS{CSCSA-121085,CSCSA-121086,CSCSA-121087,CSCSA-121088,CSCSA-121089}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121171}
 * @verification{Create a test to check if the criticality level is determined properly}
 */
static Scw_Alert_Level_T
Scw_Get_Criticality_Level(Scw_Persistent_T *p_scw_persistent /**< Scw persistents */,
                          const float32_T lateral_distance /**< lateral distance to the crtical object or guardrail */,
                          const float32_T lateral_ttc /**< lateral ttc of the crtical object or guardrail */,
                          const float32_T min_lateral_distance /**< min. lateral distance threshold */,
                          const float32_T max_lateral_distance /**< max. lateral distance threshold */,
                          const float32_T min_lateral_ttc /**< min. lateral ttc threshold */,
                          const float32_T max_lateral_ttc /**< max. lateral ttc threshold */,
                          const float32_T lateral_distance_hysteresis /**< lateral distance hysteresis */,
                          const float32_T lateral_ttc_hysteresis /**< lateral ttc hysteresis */,
                          const uint8_t side /**< side */);


/**
 * @brief Clears the critical object and sets its properties to default values.
 *
 * @return void
 *
 * @SRS{SF-2114,SF-2127,SF-2119,SF-2122,SF-2121,SF-2132}
 * @SAE{SF-2990}
 * @SDD{SF-8042}
 * @verification{Create a test to check if the core output for one particular side is reset properly}
 */
static void Scw_Reset_Core_Output_For_Side(Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
                                           const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibration */,
                                           const uint8_t side /**< approach side */);


/**
 * @brief Resets the core output to default values.
 *
 * @return void
 *
 * @SRS{SF-2114}
 * @SAE{SF-2990}
 * @SDD{SF-8041}
 * @verification{Create a test to check if the core output is reset properly}
 */
static void Scw_Reset_Core_Output(Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
                                  const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibration */);


/**
 * @brief Resets the persistent "prev_was" components for one specific side.
 *
 * @return void
 *
 * @SRS{SF-2114}
 * @SAE{SF-2990}
 * @SDD{CSCSA-305551}
 * @verification{Create a test to check if the "prev_was" persistent components are reset correctly}
 */
static void Scw_Reset_Prev_Was_for_Side(Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                                        const uint8_t side /**< side */);

/**
 * @brief Clears all the persistence information that is retained across cycles.
 *
 * @return void
 *
 * @SRS{SF-2114}
 * @SAE{SF-2990}
 * @SDD{SF-8044}
 * @verification{Create a test to check if the persistent data is reset properly}
 */
static void Scw_Reset_Persistent_Data(Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */);


/**
 * @brief Resets persistent data of the given object.
 *
 * @return void
 *
 * @SRS{SF-2136}
 * @SAE{SF-2990}
 * @SDD{SF-8043}
 * @verification{Create a test to check if the object persistent data is reset properly}
 */
static void Scw_Reset_Object_Persistent_Data(Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                                             const uint8_t object_id /**< object id */);


/**
 * @brief Resets critical object data.
 *
 * @return void
 *
 * @SRS{SF-2132}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121172}
 * @verification{Create a test to check if the critical object data is reset properly}
 */
static void Scw_Reset_Critical_Object_Data(Scw_Critical_Object_T *p_scw_critical_object /**< Scw critical object */,
                                           const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibration */);


/**
 * @brief Fills object information and presets additional object parameters.
 *
 * @return void
 *
 * @SRS{SF-2121}
 * @SAE{SF-2990}
 * @SDD{CSCSA-121174}
 * @verification{Create a test to check if the object information is created properly}
 */
static void Scw_Prepare_Object_Information(Scw_Object_T *const scw_object /**< Scw internal object */,
                                           const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                           const Scw_Core_Calibration_T *const p_scw_cal /**< Scw calibration */,
                                           const uint8_t object_index /**< object index */);


/**
 * @brief Checks if the feature is activated based on the ego speed.
 * If the host was previously above the activation threshold, then a hysteresis value
 * is subtracted from k_scw_min_host_speed to get the new threshold.
 *
 * @return True when feature is activated
 *
 * @SRS{SF-2125,SF-2124}
 * @SAE{SF-2990}
 * @SDD{SF-8038}
 * @verification{Create a test to check if scw is correctly assessed as activated}
 */
static boolean_T Scw_Is_Feature_Activated(Scw_Persistent_T *p_scw_persistent /**< Scw persistent data */,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                          const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibrations */);


/**
 * @brief Calculates object nearest lateral point.
 *
 * @return void
 *
 * @SRS{SF-2128}
 * @SAE{SF-2990}
 * @SDD{CSCSA-333861}
 * @verification{Create a test to check whether the correct nearest lateral corner/point of the object is returned.
 * Object corners calculation test is already covered by the Fbk_Calculate_Target_Corners procedure used.}
 */
static void Scw_Calculate_Nearest_Object_Corner_Lat(const Fbk_Object_Corners_T *p_object_corners /**< Scw object corners */,
                                                    const Fbk_Field_Of_Interest_T *p_scw_zone /**< Scw object zone */,
                                                    Scw_Object_T *p_scw_object /**< Scw object */,
                                                    const float32_T ego_length /* Ego length */);


/**
 * @brief Calculate object farthest longitudinal corner.
 *
 * @return void
 *
 * @SRS{SF-2128}
 * @SAE{SF-2990}
 * @SDD{CSCSA-333866}
 * @verification{Create a test to check whether the correct farthest longitudinal corner of the object is returned.
 * Object corners calculation test is already covered by the Fbk_Calculate_Target_Corners procedure used.}
 */
static void Scw_Calculate_Farthest_Object_Corner_Long(const Fbk_Object_Corners_T *p_object_corners /**< Scw object corners */,
                                                      Scw_Object_T *p_scw_object /**< Scw object */);

/**
 * @brief Calculates lateral coordinate of the point for provided longitudinal coordinate that lies on a line created from two
 * points
 *
 * @return void
 *
 * @SRS{SF-2128}
 * @SAE{SF-2990}
 * @SDD{CSCSA-333823}
 * @verification{Create a test to check whether the correct interpolated lateral coordinate is returned for given longitudinal
 * coordinate.}
 */
static float32_T Scw_Nearest_Point_Lateral_Interpolation(
   const float32_T x1, const float32_T y1, const float32_T x2, const float32_T y2, const float32_T x);


/**
 * @brief Checks whether the object is in the zone by iterative checking whether
 * one corner point is within the given zone.
 *
 * @return True when object is in zone
 *
 * @SRS{SF-2128}
 * @SAE{SF-2990}
 * @SDD{SF-8034}
 * @verification{Create a test to check if the dynamic object is correctly detected in zone}
 */
static boolean_T Scw_Is_Dyn_Object_In_Zone(Scw_Object_T *p_scw_object /**< Scw internal object */,
                                           const Fbk_Field_Of_Interest_T *p_scw_zone /**< Scw zone */,
                                           const float32_T host_length /* Host length */);


/**
 * @brief Determines if initial zone or hysteresis zone should be used for current entity (dynamic object or guardrail) and flips
 * zone if approach side is left.
 *
 * @return void
 *
 * @SRS{SF-2120}
 * @SAE{SF-2990}
 * @SDD{SF-8027}
 * @verification{Create a test to check if usage of the hysteresis is determined properly}
 */
static void
Scw_Calculate_Entity_Specific_Zone(Fbk_Field_Of_Interest_T *p_scw_entity_specific_zone /**< Scw entity specific zone */,
                                   const Fbk_Field_Of_Interest_T *const p_scw_zone /**< Scw initial zone */,
                                   const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone /**< Scw hysteresis zone */,
                                   const uint8_t scw_approach_side /**< approach side */,
                                   const uint8_t in_zone_count);


/**
 * @brief Initializes the Scw core input.
 *
 * @return void
 *
 * @SRS{SF-2114}
 * @SAE{SF-2990}
 * @SDD{SF-8032}
 * @verification{Create a test to check if the core input is initialized properly}
 */
static void Scw_Init_Core_Input(Scw_Core_Input_T *p_scw_core_input /**< Scw core input */);


/**
 * @brief Checks whether lateral zone hysteresis shall be applied.
 *
 * @return True when criteria for lateral zone hysteresis application are met.
 *
 * @SRS{SF-2117}
 * @SAE{SF-2990}
 * @SDD{SF-8186}
 * @verification{Create a test to check if usage of the zone lateral hysteresis is determined properly}
 */
static boolean_T Scw_Shall_Lat_Zone_Hyst_Be_Applied(const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibration*/,
                                                    const uint8_t zone_point_index /**< index*/);


/**
 * @brief Checks whether longitudinal zone hysteresis shall be applied.
 *
 * @return True when criteria for longitudinal zone hysteresis application are met.
 *
 * @SRS{SF-2117}
 * @SAE{SF-2990}
 * @SDD{SF-8185}
 * @verification{Create a test to check if usage of the zone longitudinal hysteresis is determined properly}
 */
static boolean_T Scw_Shall_Long_Zone_Hyst_Be_Applied(const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibration*/,
                                                     const uint8_t zone_point_index /**< index*/);


/**
 * @brief Calculates lateral velocity or acceleration of the Host to the guardrail.
 *
 * @return Lateral velocity or acceleration.
 *
 * @SRS{CSCSA-137483}
 * @SAE{SF-2990}
 * @SDD{CSCSA-125685}
 * @verification{Create a test to check whether the lateral velocity and acceleration to the guardrail is calculated correctly}
 */
static float32_T
Scw_Calculate_Guardrail_Position_Derivative(const Scw_Persistent_T *p_scw_persistent /**< Scw persistents */,
                                            const Scw_Core_Input_T *p_scw_core_input /**< Scw core input */,
                                            const uint8_t order /**< derivative order, i.e. velocity or acceleration */,
                                            const uint8_t side /**< Ego side */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
void Scw_Reset(Scw_Core_Input_T *p_scw_core_input,
               Scw_Core_Output_T *p_scw_core_output,
               Scw_Persistent_T *p_scw_persistent,
               const Scw_Core_Calibration_T *p_scw_cal)
{
   /* Assert */
   assert(NULL != p_scw_core_output);

   Scw_Init_Core_Input(p_scw_core_input);

   /* Reset the SCW persistent data */
   Scw_Reset_Persistent_Data(p_scw_persistent);

   /* Reset the SCW core output */
   Scw_Reset_Core_Output(p_scw_core_output, p_scw_cal);
}

void Scw_Core_Run(Scw_Core_Output_T *p_scw_core_output,
                  Scw_Core_Input_T *p_scw_core_input,
                  Scw_Persistent_T *p_scw_persistent,
                  const Scw_Core_Calibration_T *p_scw_cal)
{
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_scw_core_output);
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_cal);

   /* Reset debug data */
   Binary_Scw_Debug_Reset_Data();

   /* Run SCW algorithm */
   if (Fbk_Is_True(p_scw_core_input->f_scw_enable))
   {
      p_vehicle_data = &(p_scw_core_input->p_pa_data->vehicle_data);

      if (Scw_Is_Feature_Activated(p_scw_persistent, p_vehicle_data, p_scw_cal))
      {
         Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_cal);
      }
      else
      {
         Scw_Reset(p_scw_core_input, p_scw_core_output, p_scw_persistent, p_scw_cal);
      }
   }
   else
   {
      Scw_Reset(p_scw_core_input, p_scw_core_output, p_scw_persistent, p_scw_cal);
   }

   /* Pass general data to debug data */
   Binary_Scw_Debug_Pass_General_Data(p_scw_core_input, p_scw_core_output, p_scw_persistent, p_scw_cal);
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Scw_Init_Core_Input(Scw_Core_Input_T *p_scw_core_input)
{

   /* Asserts */
   assert(NULL != p_scw_core_input);

   p_scw_core_input->f_scw_enable           = FBK_FALSE;
   p_scw_core_input->f_scw_enable_dynamic   = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail = FBK_FALSE;

   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position  = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence        = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type              = SCW_GUARDRAIL_INVALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.confidence       = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.type             = SCW_GUARDRAIL_INVALID;

   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position  = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.confidence        = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.type              = SCW_GUARDRAIL_INVALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.confidence       = FBK_ZERO_F;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.type             = SCW_GUARDRAIL_INVALID;

   /* Trailer information */
   p_scw_core_input->trailer.f_present = FBK_FALSE;
   p_scw_core_input->trailer.length    = FBK_ZERO_F;
   p_scw_core_input->trailer.width     = FBK_ZERO_F;
   p_scw_core_input->trailer.angle     = FBK_ZERO_F;
}

static boolean_T Scw_Is_Feature_Activated(Scw_Persistent_T *p_scw_persistent,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                                          const Scw_Core_Calibration_T *p_scw_cal)
{
   boolean_T f_active = FBK_FALSE;
   float32_T activation_speed;

   /* Asserts */
   assert(NULL != p_scw_persistent);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_scw_cal);

   activation_speed = p_scw_cal->k_scw_min_host_speed;

   /* apply hysteresis if feature was activated before */
   if (Fbk_Is_True(p_scw_persistent->prev_feature_activated))
   {
      activation_speed -= p_scw_cal->k_scw_min_host_speed_hys;
   }

   if (p_vehicle_data->host_speed >= activation_speed)
   {
      f_active = FBK_TRUE;
   }

   /* Save activation state in persistent data */
   p_scw_persistent->prev_feature_activated = f_active;

   return f_active;
}

static void Scw_Reset_Prev_Was_for_Side(Scw_Persistent_T *p_scw_persistent, const uint8_t side)
{
   if (side < FBK_NUMBER_OF_SIDES)
   {
      p_scw_persistent->prev_was_below_min_lat_ttc[side]      = FBK_FALSE;
      p_scw_persistent->prev_was_below_max_lat_ttc[side]      = FBK_FALSE;
      p_scw_persistent->prev_was_below_min_lat_distance[side] = FBK_FALSE;
      p_scw_persistent->prev_was_below_max_lat_distance[side] = FBK_FALSE;
   }
}

static void Scw_Reset_Persistent_Data(Scw_Persistent_T *p_scw_persistent)
{
   uint8_t object_id;
   uint8_t idx;
   uint8_t i;

   /* Assert */
   assert(NULL != p_scw_persistent);

   p_scw_persistent->prev_feature_activated = FBK_FALSE;

   for (idx = FBK_ZERO_UINT; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      p_scw_persistent->prev_obj_index[idx]     = PA_INVALID_OBJ_INDEX;
      p_scw_persistent->prev_obj_id[idx]        = PA_INVALID_OBJ_ID;
      p_scw_persistent->prev_obj_unique_id[idx] = PA_INVALID_OBJ_ID;
      p_scw_persistent->prev_obj_type[idx]      = SCW_OBJECT_TYPE_NONE;
      Scw_Reset_Prev_Was_for_Side(p_scw_persistent, idx);
   }

   for (object_id = FBK_ZERO_UINT; object_id < (PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT); object_id++)
   {
      Scw_Reset_Object_Persistent_Data(p_scw_persistent, object_id);
   }

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_GUARDRAILS; idx++)
   {
      p_scw_persistent->count_in_zone_grail[idx] = FBK_ZERO_UINT;
      for (i = FBK_ZERO_UINT; i < SCW_LAT_POS_BUFFER_SIZE; i++)
      {
         p_scw_persistent->core_grail_lat_position[idx][i] = FBK_ZERO_F;
      }
   }
}

static void Scw_Reset_Object_Persistent_Data(Scw_Persistent_T *p_scw_persistent, const uint8_t object_id)
{
   p_scw_persistent->dyn_obj_data[object_id].mature_in_zone_count              = FBK_ZERO_UINT;
   p_scw_persistent->dyn_obj_data[object_id].f_relevant_last_cycle             = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[object_id].f_in_speed_range                  = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[object_id].f_in_relative_long_velocity_range = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[object_id].f_in_heading_range                = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[object_id].f_in_yawrate_range                = FBK_TRUE;
}

static void Scw_Reset_Critical_Object_Data(Scw_Critical_Object_T *p_scw_critical_object, const Scw_Core_Calibration_T *p_scw_cal)
{
   p_scw_critical_object->index                = PA_INVALID_OBJ_INDEX;
   p_scw_critical_object->position_x           = SCW_MIN_POSITION_X;
   p_scw_critical_object->lateral_distance     = p_scw_cal->k_scw_lateral_distance_default;
   p_scw_critical_object->lateral_velocity     = FBK_ZERO_F;
   p_scw_critical_object->lateral_acceleration = FBK_ZERO_F;
   p_scw_critical_object->lateral_ttc          = p_scw_cal->k_scw_lateral_ttc_default;
   p_scw_critical_object->ttle                 = p_scw_cal->k_scw_ttle_default;
   p_scw_critical_object->ttp                  = p_scw_cal->k_scw_ttp_default;
}

static void Scw_Prepare_Object_Information(Scw_Object_T *const scw_object,
                                           const Scw_Core_Input_T *const p_scw_core_input,
                                           const Scw_Core_Calibration_T *const p_scw_cal,
                                           const uint8_t object_index)
{
   scw_object->tracker_data = p_scw_core_input->p_pa_data->object_data[object_index];
   Scw_Reset_Critical_Object_Data(&scw_object->extended_data, p_scw_cal);
   scw_object->extended_data.index = object_index;
   scw_object->nearest_corner_y    = Fbk_Get_Obj_Side_Sign(scw_object->tracker_data.vcs_pos.y) * SCW_MAX_LATERAL_DISTANCE;
   scw_object->rearmost_corner_x   = SCW_BIG_VALUE;
}

static void Scw_Reset_Core_Output(Scw_Core_Output_T *p_scw_core_output, const Scw_Core_Calibration_T *p_scw_cal)
{
   uint8_t side;

   /* Assert */
   assert(NULL != p_scw_core_output);
   assert(NULL != p_scw_cal);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Scw_Reset_Core_Output_For_Side(p_scw_core_output, p_scw_cal, side);
   }
}

static void Scw_Reset_Core_Output_For_Side(Scw_Core_Output_T *p_scw_core_output,
                                           const Scw_Core_Calibration_T *p_scw_cal,
                                           const uint8_t side)
{
   /* Assert */
   assert(NULL != p_scw_core_output);

   p_scw_core_output->obj_id[side]                   = FBK_ZERO_UINT;
   p_scw_core_output->obj_unique_id[side]            = FBK_ZERO_UINT;
   p_scw_core_output->obj_type[side]                 = SCW_OBJECT_TYPE_NONE;
   p_scw_core_output->obj_index[side]                = PA_INVALID_OBJ_INDEX;
   p_scw_core_output->alert_level[side]              = SCW_NO_ALERT;
   p_scw_core_output->obj_lateral_distance[side]     = p_scw_cal->k_scw_lateral_distance_default;
   p_scw_core_output->obj_lateral_velocity[side]     = FBK_ZERO_F;
   p_scw_core_output->obj_lateral_acceleration[side] = FBK_ZERO_F;
   p_scw_core_output->obj_lateral_ttc[side]          = p_scw_cal->k_scw_lateral_ttc_default;
   p_scw_core_output->obj_ttle[side]                 = p_scw_cal->k_scw_ttle_default;
   p_scw_core_output->obj_ttp[side]                  = p_scw_cal->k_scw_ttp_default;
}


static void Scw_Algorithm(Scw_Core_Output_T *p_scw_core_output,
                          Scw_Persistent_T *p_scw_persistent,
                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                          const Scw_Core_Input_T *p_scw_core_input,
                          const Scw_Core_Calibration_T *p_scw_cal)
{
   uint8_t side;
   uint8_t idx;
   uint8_t max_lat_pos_buf_idx;
   Scw_Critical_Object_T critical_dyn_obj[FBK_NUMBER_OF_SIDES];
   Scw_Critical_Object_T critical_guardrail[FBK_NUMBER_OF_SIDES];

   Fbk_Field_Of_Interest_T scw_zone;
   Fbk_Field_Of_Interest_T scw_hysteresis_zone;
   float32_T ttc_extension = 0.0f;

   /* Asserts */
   assert(NULL != p_scw_core_output);
   assert(NULL != p_scw_persistent);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_cal);

   if (p_scw_cal->k_scw_f_enable_trailer_ttc_extension && p_scw_core_input->trailer.f_present
       && (p_scw_core_input->trailer.length > FBK_ZERO_F) && (p_scw_core_input->trailer.width > FBK_ZERO_F))
   {
      ttc_extension = p_scw_cal->k_scw_trailer_lat_ttc_extension;
   }
   /* Initilaize critical dynamic objects data */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Scw_Reset_Critical_Object_Data(&critical_dyn_obj[side], p_scw_cal);
      Scw_Reset_Critical_Object_Data(&critical_guardrail[side], p_scw_cal);
   }

   /* Create SCW zones from calibration values */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_cal, p_vehicle_data);
   Binary_Scw_Debug_Pass_Zones(&scw_zone, &scw_hysteresis_zone);

   /* Check for critical dynamic objects*/
   if (Fbk_Is_True(p_scw_core_input->f_scw_enable_dynamic))
   {
      Scw_Get_Critical_Dynamic_Objs(critical_dyn_obj, p_scw_persistent, p_scw_core_input, &scw_zone, &scw_hysteresis_zone, p_scw_cal);
   }

   /* Check for critical guardrails*/
   if (Fbk_Is_True(p_scw_core_input->f_scw_enable_guardrail))
   {
      Scw_Get_Critical_Guardrails(critical_guardrail, p_scw_persistent, p_scw_core_input, p_vehicle_data, &scw_zone,
                                  &scw_hysteresis_zone, p_scw_cal);
   }

   /* Check for both sides if a critical dynamic object was found */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* If this is true populate the scw core output */
      if (critical_dyn_obj[side].index < PA_OBJ_NUMBER_OF_OBJECTS)
      {
         if (p_scw_persistent->prev_obj_id[side] != p_scw_core_input->p_pa_data->object_data[critical_dyn_obj[side].index].id)
         {
            Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);
         }
         p_scw_core_output->obj_id[side]        = p_scw_core_input->p_pa_data->object_data[critical_dyn_obj[side].index].id;
         p_scw_core_output->obj_unique_id[side] = p_scw_core_input->p_pa_data->object_data[critical_dyn_obj[side].index].unique_id;
         p_scw_core_output->obj_index[side]     = critical_dyn_obj[side].index;
         p_scw_core_output->obj_type[side]      = SCW_OBJECT_TYPE_DYNAMIC;
         p_scw_core_output->alert_level[side]   = Scw_Get_Criticality_Level(
              p_scw_persistent, critical_dyn_obj[side].lateral_distance, critical_dyn_obj[side].lateral_ttc,
              p_scw_cal->k_scw_min_dynamic_lat_distance, p_scw_cal->k_scw_max_dynamic_lat_distance,
              (p_scw_cal->k_scw_min_dynamic_lat_ttc + ttc_extension), (p_scw_cal->k_scw_max_dynamic_lat_ttc + ttc_extension),
              p_scw_cal->k_scw_critical_lat_distance_hys, p_scw_cal->k_scw_critical_lat_ttc_hys, side);
         p_scw_core_output->obj_lateral_distance[side]     = critical_dyn_obj[side].lateral_distance;
         p_scw_core_output->obj_lateral_velocity[side]     = critical_dyn_obj[side].lateral_velocity;
         p_scw_core_output->obj_lateral_acceleration[side] = critical_dyn_obj[side].lateral_acceleration;
         p_scw_core_output->obj_lateral_ttc[side]          = critical_dyn_obj[side].lateral_ttc;
         p_scw_core_output->obj_ttle[side]                 = critical_dyn_obj[side].ttle;
         p_scw_core_output->obj_ttp[side]                  = critical_dyn_obj[side].ttp;
      }
      /* If there is no critical dynamic object and check if there is a critical guardrail */
      else if (critical_guardrail[side].index != PA_INVALID_OBJ_INDEX)
      {
         if (p_scw_persistent->prev_obj_type[side] != SCW_OBJECT_TYPE_GUARDRAIL)
         {
            Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);
         }
         p_scw_core_output->obj_id[side]        = SCW_OUTPUT_OBJ_ID_GUARDRAIL;
         p_scw_core_output->obj_unique_id[side] = SCW_OUTPUT_OBJ_ID_GUARDRAIL;
         p_scw_core_output->obj_index[side]     = critical_guardrail[side].index;
         p_scw_core_output->obj_type[side]      = SCW_OBJECT_TYPE_GUARDRAIL;
         p_scw_core_output->alert_level[side]   = Scw_Get_Criticality_Level(
              p_scw_persistent, critical_guardrail[side].lateral_distance, critical_guardrail[side].lateral_ttc,
              p_scw_cal->k_scw_min_guardrail_lat_distance, p_scw_cal->k_scw_max_guardrail_lat_distance,
              (p_scw_cal->k_scw_min_guardrail_lat_ttc + ttc_extension), (p_scw_cal->k_scw_max_guardrail_lat_ttc + ttc_extension),
              p_scw_cal->k_scw_critical_lat_distance_hys, p_scw_cal->k_scw_critical_lat_ttc_hys, side);
         p_scw_core_output->obj_lateral_distance[side]     = critical_guardrail[side].lateral_distance;
         p_scw_core_output->obj_lateral_velocity[side]     = critical_guardrail[side].lateral_velocity;
         p_scw_core_output->obj_lateral_acceleration[side] = critical_guardrail[side].lateral_acceleration;
         p_scw_core_output->obj_lateral_ttc[side]          = critical_guardrail[side].lateral_ttc;
         p_scw_core_output->obj_ttle[side]                 = critical_guardrail[side].ttle;
         p_scw_core_output->obj_ttp[side]                  = critical_guardrail[side].ttp;
      }
      /* If no critical object was found reset core output */
      else
      {
         Scw_Reset_Core_Output_For_Side(p_scw_core_output, p_scw_cal, side);
      }

      /* Save side persistent data */
      max_lat_pos_buf_idx = SCW_LAT_POS_BUFFER_SIZE - FBK_ONE_UINT;
      for (idx = FBK_ZERO_UINT; idx < max_lat_pos_buf_idx; idx++)
      {
         p_scw_persistent->core_grail_lat_position[side][idx] = p_scw_persistent->core_grail_lat_position[side][idx + FBK_ONE_UINT];
      }
      p_scw_persistent->core_grail_lat_position[side][idx] = p_scw_core_input->guardrail_data[side].radar.lateral_position;
      p_scw_persistent->prev_obj_type[side]                = p_scw_core_output->obj_type[side];
      p_scw_persistent->prev_obj_index[side]               = p_scw_core_output->obj_index[side];
      p_scw_persistent->prev_obj_id[side]                  = p_scw_core_output->obj_id[side];
      p_scw_persistent->prev_obj_unique_id[side]           = p_scw_core_output->obj_unique_id[side];
   }
}


static Scw_Alert_Level_T Scw_Get_Criticality_Level(Scw_Persistent_T *p_scw_persistent,
                                                   const float32_T lateral_distance,
                                                   const float32_T lateral_ttc,
                                                   const float32_T min_lateral_distance,
                                                   const float32_T max_lateral_distance,
                                                   const float32_T min_lateral_ttc,
                                                   const float32_T max_lateral_ttc,
                                                   const float32_T lateral_distance_hysteresis,
                                                   const float32_T lateral_ttc_hysteresis,
                                                   const uint8_t side)
{
   Scw_Alert_Level_T criticality_level;
   boolean_T lateral_distance_too_small;
   boolean_T lateral_ttc_too_small;

   p_scw_persistent->prev_was_below_min_lat_ttc[side] = Scw_Below_Threshold_Add_Hyst(
      lateral_ttc, min_lateral_ttc, lateral_ttc_hysteresis, p_scw_persistent->prev_was_below_min_lat_ttc[side]);
   p_scw_persistent->prev_was_below_max_lat_ttc[side] = Scw_Below_Threshold_Add_Hyst(
      lateral_ttc, max_lateral_ttc, lateral_ttc_hysteresis, p_scw_persistent->prev_was_below_max_lat_ttc[side]);
   p_scw_persistent->prev_was_below_min_lat_distance[side] = Scw_Below_Threshold_Add_Hyst(
      lateral_distance, min_lateral_distance, lateral_distance_hysteresis, p_scw_persistent->prev_was_below_min_lat_distance[side]);
   p_scw_persistent->prev_was_below_max_lat_distance[side] = Scw_Below_Threshold_Add_Hyst(
      lateral_distance, max_lateral_distance, lateral_distance_hysteresis, p_scw_persistent->prev_was_below_max_lat_distance[side]);
   lateral_distance_too_small =
      (boolean_T) (p_scw_persistent->prev_was_below_min_lat_distance[side] && Fbk_GE_F(lateral_ttc, FBK_ZERO_F)
                   && p_scw_persistent->prev_was_below_max_lat_ttc[side]);
   lateral_ttc_too_small = (boolean_T) (p_scw_persistent->prev_was_below_max_lat_distance[side]
                                        && Fbk_GE_F(lateral_ttc, FBK_ZERO_F) && p_scw_persistent->prev_was_below_min_lat_ttc[side]);

   if (lateral_distance_too_small || lateral_ttc_too_small)
   {
      criticality_level = SCW_ALERT_LEVEL_2;
   }
   else
   {
      criticality_level = SCW_ALERT_LEVEL_1;
   }

   return criticality_level;
}


static void Scw_Get_Critical_Dynamic_Objs(Scw_Critical_Object_T critical_dyn_obj[FBK_NUMBER_OF_SIDES],
                                          Scw_Persistent_T *p_scw_persistent,
                                          const Scw_Core_Input_T *const p_scw_core_input,
                                          const Fbk_Field_Of_Interest_T *const p_scw_zone,
                                          const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone,
                                          const Scw_Core_Calibration_T *const p_scw_cal)
{
   uint8_t object_index;
   Fbk_Field_Of_Interest_T scw_zone_for_current_obj;
   boolean_T is_true;
   Scw_Persist_Obj_Data_T *persistent_obj_data;

   const Scw_TTx_Calculation_Mode_T lat_ttc_mode[FBK_NUMBER_OF_SIDES] = {LAT_TTC_LEFT, LAT_TTC_RIGHT};
   const Scw_TTx_Calculation_Mode_T ttle_mode[FBK_NUMBER_OF_SIDES]    = {TTLE_LEFT, TTLE_RIGHT};

   /* Asserts */
   assert(NULL != critical_dyn_obj);
   assert(NULL != p_scw_persistent);
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_zone);
   assert(NULL != p_scw_hysteresis_zone);
   assert(NULL != p_scw_cal);

   /* Go through the tracker output objects and determine if it is a valid candidate for SCW warning */
   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      is_true             = Scw_Is_Dyn_Object_Valid(p_scw_core_input, object_index, p_scw_cal, p_scw_persistent);
      persistent_obj_data = &(p_scw_persistent->dyn_obj_data[p_scw_core_input->p_pa_data->object_data[object_index].id]);

      if (is_true)
      {
         Scw_Object_T scw_object;
         uint8_t side;
         float32_T sign;

         Scw_Prepare_Object_Information(&scw_object, p_scw_core_input, p_scw_cal, object_index);

         /* Get which side of the ego the object is on */
         side = Fbk_Get_Obj_Side(scw_object.tracker_data.vcs_pos.y);
         sign = Fbk_Get_Obj_Side_Sign(scw_object.tracker_data.vcs_pos.y);

         is_true = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_cal, p_scw_persistent, side);
         if (is_true)
         {
            /* Update flag that indicates if object was relevant in last cycle */
            persistent_obj_data->f_relevant_last_cycle = FBK_TRUE;

            Scw_Calculate_Entity_Specific_Zone(&scw_zone_for_current_obj, p_scw_zone, p_scw_hysteresis_zone, side,
                                               persistent_obj_data->mature_in_zone_count);

            /* Check if any point of the obj is within the SCW zone */
            is_true = Scw_Is_Dyn_Object_In_Zone(&scw_object, &scw_zone_for_current_obj,
                                                p_scw_core_input->p_pa_data->vehicle_data.host_length);
            if (is_true)
            {
               /* Update the mature in zone counter based on the object status */
               if (PA_OBJ_STATUS_MATURE == scw_object.tracker_data.status)
               {
                  Sat_Inc_Uint8(&(persistent_obj_data->mature_in_zone_count));
               }

               if (persistent_obj_data->mature_in_zone_count >= p_scw_cal->k_scw_candidate_mature_cycles_in_zone_threshold)
               {
                  scw_object.extended_data.lateral_distance = Scw_Get_Dyn_Object_Lateral_Distance(&scw_object, p_scw_core_input);
                  scw_object.extended_data.lateral_ttc =
                     Scw_Get_TTx(scw_object.extended_data.lateral_distance, scw_object.tracker_data.vcs_vel_rel.y,
                                 lat_ttc_mode[side], p_scw_cal->k_scw_lateral_ttc_max, p_scw_cal->k_scw_lateral_ttc_default);
                  scw_object.extended_data.ttle =
                     Scw_Get_TTx((sign * (scw_zone_for_current_obj.points[0].y - scw_object.nearest_corner_y)),
                                 scw_object.tracker_data.vcs_vel_rel.y, ttle_mode[side], p_scw_cal->k_scw_ttle_max,
                                 p_scw_cal->k_scw_ttle_default);
                  scw_object.extended_data.ttp = Scw_Get_TTx((-scw_object.rearmost_corner_x), scw_object.tracker_data.vcs_vel_rel.x,
                                                             TTP_MODE, p_scw_cal->k_scw_ttp_max, p_scw_cal->k_scw_ttp_default);
                  Scw_Get_Most_Critical_Dyn_Obj(&critical_dyn_obj[side], &scw_object);
               }
            }
            else
            {
               /* Clear the count in zone if the obj is not in the zone */
               persistent_obj_data->mature_in_zone_count = FBK_ZERO_INT;
            }
         }
         else
         {
            /* Reset object persistent data */
            Scw_Reset_Object_Persistent_Data(p_scw_persistent, p_scw_core_input->p_pa_data->object_data[object_index].id);
         }
      }
      else
      {
         /* Reset object persistent data */
         Scw_Reset_Object_Persistent_Data(p_scw_persistent, p_scw_core_input->p_pa_data->object_data[object_index].id);
      }
   }
}

static void Scw_Get_Most_Critical_Dyn_Obj(Scw_Critical_Object_T *const p_curr_critical_obj, const Scw_Object_T *const p_scw_object)
{
   boolean_T update_critical_obj = FBK_FALSE;
   float32_T new_lateral_distance;
   float32_T curr_lateral_distance;

   /* Assert */
   assert(NULL != p_curr_critical_obj);
   assert(NULL != p_scw_object);

   new_lateral_distance  = p_scw_object->extended_data.lateral_distance;
   curr_lateral_distance = p_curr_critical_obj->lateral_distance;

   if (PA_INVALID_OBJ_INDEX != p_curr_critical_obj->index)
   {
      /* Check if the new object is laterally closer than the current object */
      if ((new_lateral_distance < curr_lateral_distance)
          || ((new_lateral_distance == curr_lateral_distance)
              && (p_scw_object->tracker_data.vcs_pos.x > p_curr_critical_obj->position_x))) /* one assumed for simplicity that the
                                                                                               critical objects are rather behind */
      {
         update_critical_obj = FBK_TRUE;
      }
   }
   else
   {
      update_critical_obj = FBK_TRUE;
   }

   if (Fbk_Is_True(update_critical_obj))
   {
      p_curr_critical_obj->index                = p_scw_object->extended_data.index;
      p_curr_critical_obj->position_x           = p_scw_object->tracker_data.vcs_pos.x;
      p_curr_critical_obj->lateral_distance     = p_scw_object->extended_data.lateral_distance;
      p_curr_critical_obj->lateral_ttc          = p_scw_object->extended_data.lateral_ttc;
      p_curr_critical_obj->ttle                 = p_scw_object->extended_data.ttle;
      p_curr_critical_obj->ttp                  = p_scw_object->extended_data.ttp;
      p_curr_critical_obj->lateral_velocity     = p_scw_object->tracker_data.vcs_vel.y;
      p_curr_critical_obj->lateral_acceleration = p_scw_object->tracker_data.vcs_accel.y;
   }
}


static boolean_T Scw_Is_Dyn_Object_Valid(const Scw_Core_Input_T *const p_scw_core_input,
                                         const uint8_t obj_index,
                                         const Scw_Core_Calibration_T *const p_scw_cal,
                                         const Scw_Persistent_T *const p_scw_persistent)
{
   boolean_T f_obj_is_valid  = FBK_FALSE;
   boolean_T f_obj_status_ok = FBK_FALSE;
   boolean_T f_obj_age_ok    = FBK_FALSE;
   const Fbk_Object_Data_T *p_object_data;

   /* Asserts */
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_cal);
   assert(NULL != p_scw_persistent);

   p_object_data = &(p_scw_core_input->p_pa_data->object_data[obj_index]);

   /* Coasted object should only be valid, if they already triggered an alert in mature status. */
   if ((PA_OBJ_STATUS_MATURE == p_object_data->status)
       || ((PA_OBJ_STATUS_COASTED == p_object_data->status) && (p_object_data->id == p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT])
           && (SCW_OBJECT_TYPE_DYNAMIC == p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]))
       || ((PA_OBJ_STATUS_COASTED == p_object_data->status) && (p_object_data->id == p_scw_persistent->prev_obj_id[FBK_SIDE_RIGHT])
           && (SCW_OBJECT_TYPE_DYNAMIC == p_scw_persistent->prev_obj_type[FBK_SIDE_RIGHT])))
   {
      f_obj_status_ok = FBK_TRUE;
   }

   if (p_object_data->age >= p_scw_cal->k_scw_min_candidate_age)
   {
      f_obj_age_ok = FBK_TRUE;
   }

   if (f_obj_status_ok && f_obj_age_ok)
   {
      f_obj_is_valid = FBK_TRUE;
      Binary_Scw_Debug_Increase_Object_Importance_Counter(p_object_data->id);
   }

   return f_obj_is_valid;
}

static void Scw_Create_Zones(Fbk_Field_Of_Interest_T *p_scw_zone,
                             Fbk_Field_Of_Interest_T *p_scw_hysteresis_zone,
                             const Scw_Core_Input_T *p_scw_core_input,
                             const Scw_Core_Calibration_T *p_scw_cal,
                             const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t index;

   float32_T ego_length     = p_vehicle_data->host_length;
   float32_T ego_half_width = Fbk_Half(p_vehicle_data->host_width);
   Vector_2d_T zone_extension;

   /* Set up initial scw zone */
   /*  x = long, y = lat      */
   /*           3 ---- 0      */
   /*    ==|==  |      |      */
   /*   []/ \[] |      |      */
   /*    / ^ \  |      |      */
   /*   /| O |\ |      |      */
   /*   []_|_[] |      |      */
   /*   |_ego_| |      |      */
   /*           |      |      */
   /*           |      |      */
   /*           2 ---- 1      */
   /*                         */

   if (p_scw_cal->k_scw_f_enable_trailer_zone_extension && p_scw_core_input->trailer.f_present
       && (p_scw_core_input->trailer.length > FBK_ZERO_F) && (p_scw_core_input->trailer.width > FBK_ZERO_F))
   {
      /* Add safety margins behind and beside the trailer */
      zone_extension.x = p_scw_core_input->trailer.length + p_scw_cal->k_scw_trailer_zone_ext_safety_margin;
      zone_extension.y = Fbk_Max(Fbk_Half(p_scw_core_input->trailer.width) - ego_half_width, FBK_ZERO_F)
                         + p_scw_cal->k_scw_trailer_zone_ext_safety_margin_lat;
   }
   else
   {
      zone_extension.x = FBK_ZERO_F;
      zone_extension.y = FBK_ZERO_F;
   }

   /* Set up initial zone */
   Fbk_Create_Field_Of_Interest(p_scw_zone, p_scw_cal->k_scw_initial_zone_x, p_scw_cal->k_scw_initial_zone_y,
                                SCW_NUMBER_OF_ZONE_POINTS);

   if (p_scw_cal->k_scw_f_adjust_zones_to_ego_size)
   {
      /* Shift zone to side of ego */
      for (index = 0; index < SCW_NUMBER_OF_ZONE_POINTS; index++)
      {
         p_scw_zone->points[index].y = p_scw_zone->points[index].y + ego_half_width;
      }
      /* Extend zone by ego length and half of the width */
      p_scw_zone->points[SCW_ZONE_MIDDLE_RIGHT].x = p_scw_zone->points[SCW_ZONE_MIDDLE_RIGHT].x - Fbk_Half(ego_length);
      p_scw_zone->points[SCW_ZONE_MIDDLE_LEFT].x  = p_scw_zone->points[SCW_ZONE_MIDDLE_LEFT].x - Fbk_Half(ego_length);
      p_scw_zone->points[SCW_ZONE_REAR_RIGHT].x   = p_scw_zone->points[SCW_ZONE_REAR_RIGHT].x - ego_length;
      p_scw_zone->points[SCW_ZONE_REAR_LEFT].x    = p_scw_zone->points[SCW_ZONE_REAR_LEFT].x - ego_length;
   }

   for (index = SCW_ZONE_REAR_RIGHT; index <= SCW_ZONE_REAR_LEFT; index++)
   {
      p_scw_zone->points[index].x = Fbk_Max((p_scw_zone->points[index].x - zone_extension.x),
                                            (p_scw_zone->points[index].x - p_scw_cal->k_scw_max_zone_length));
   }
   for (index = SCW_ZONE_FRONT_RIGHT; index <= SCW_ZONE_REAR_RIGHT; index++)
   {
      p_scw_zone->points[index].y =
         Fbk_Min((p_scw_zone->points[index].y + zone_extension.y),
                 (p_scw_zone->points[(int8_t) SCW_NUMBER_OF_ZONE_POINTS - ((int8_t) index + FBK_ONE_INT)].y
                  + p_scw_cal->k_scw_max_zone_width));
   }

   /* Set up hysteresis zone (only use offsets that actually extend the zone) */
   *p_scw_hysteresis_zone = *p_scw_zone;
   for (index = FBK_ZERO_UINT; index < SCW_NUMBER_OF_ZONE_POINTS; index++)
   {
      if (Scw_Shall_Long_Zone_Hyst_Be_Applied(p_scw_cal, index))
      {
         p_scw_hysteresis_zone->points[index].x = p_scw_hysteresis_zone->points[index].x + p_scw_cal->k_scw_hys_zone_x_offset[index];
      }
      if (Scw_Shall_Lat_Zone_Hyst_Be_Applied(p_scw_cal, index))
      {
         p_scw_hysteresis_zone->points[index].y = p_scw_hysteresis_zone->points[index].y + p_scw_cal->k_scw_hys_zone_y_offset[index];
      }
   }
}

static boolean_T Scw_Is_Dyn_Object_Relevant(const Scw_Object_T *const p_scw_object,
                                            const Scw_Core_Input_T *const p_scw_core_input,
                                            const Scw_Core_Calibration_T *const p_scw_cal,
                                            Scw_Persistent_T *const p_scw_persistent,
                                            const uint8_t side)
{
   boolean_T f_obj_is_relevant = FBK_FALSE;
   boolean_T is_heading_ok;
   boolean_T is_yawrate_ok;
   boolean_T is_velocity_ok;
   boolean_T is_environment_conflict;

   /* Asserts */
   assert(NULL != p_scw_object);
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_cal);
   assert(NULL != p_scw_persistent);
   assert(side < FBK_NUMBER_OF_SIDES);

   is_heading_ok           = Scw_Is_Dyn_Object_Heading_Correct(p_scw_object, p_scw_cal, p_scw_persistent);
   is_yawrate_ok           = Scw_Is_Dyn_Object_Yawrate_Correct(p_scw_object, p_scw_cal, p_scw_persistent);
   is_velocity_ok          = Scw_Is_Dyn_Object_Velocity_Correct(p_scw_object, p_scw_cal, p_scw_persistent);
   is_environment_conflict = Scw_Is_Dyn_Object_In_Environment_Conflict(p_scw_object, p_scw_core_input, side, p_scw_cal);

   if (is_heading_ok && is_yawrate_ok && is_velocity_ok && Fbk_Is_False(is_environment_conflict)
       && (p_scw_object->tracker_data.existence_probability >= p_scw_cal->k_scw_min_candidate_existence_probability))
   {
      f_obj_is_relevant = FBK_TRUE;
      Binary_Scw_Debug_Increase_Object_Importance_Counter(p_scw_object->tracker_data.index);
   }

   return f_obj_is_relevant;
}

static boolean_T Scw_Is_Dyn_Object_In_Environment_Conflict(const Scw_Object_T *const p_scw_object,
                                                           const Scw_Core_Input_T *const p_scw_core_input,
                                                           const uint8_t side,
                                                           const Scw_Core_Calibration_T *const p_scw_cal)
{
   boolean_T f_guardrail_conflict = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_scw_object);
   assert(NULL != p_scw_core_input);
   assert(side < FBK_NUMBER_OF_SIDES);
   assert(NULL != p_scw_cal);

   if (Scw_Is_Valid_Guardrail(p_scw_core_input, side, p_scw_cal))
   {
      float32_T side_sign                  = Fbk_Convert_Obj_Side_To_Sign(side);
      float32_T lateral_guardrail_position = p_scw_core_input->guardrail_data[side].radar.lateral_position;
      float32_T obj_lateral_outer_edge =
         p_scw_object->tracker_data.vcs_pos.y + (side_sign * Fbk_Half(p_scw_object->tracker_data.width));

      /* guardrail conflict is detected, if outer object edge is behind the guardrail */
      if ((side_sign * obj_lateral_outer_edge) > (side_sign * lateral_guardrail_position))
      {
         f_guardrail_conflict = FBK_TRUE;
      }
   }

   return f_guardrail_conflict;
}

static boolean_T Scw_Is_Dyn_Object_Velocity_Correct(const Scw_Object_T *const p_scw_object,
                                                    const Scw_Core_Calibration_T *const p_scw_cal,
                                                    Scw_Persistent_T *const p_scw_persistent)
{
   boolean_T f_vel_is_correct = FBK_FALSE;
   boolean_T out_of_range;

   /* Asserts */
   assert(NULL != p_scw_object);
   assert(NULL != p_scw_cal);
   assert(NULL != p_scw_persistent);

   /* check for valid longitudinal velocity */
   out_of_range = p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_speed_range ? FBK_FALSE : FBK_TRUE;
   p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_speed_range =
      Scw_In_Range_Hyst(p_scw_object->tracker_data.speed, p_scw_cal->k_scw_candidate_velocity[SCW_MIN],
                        p_scw_cal->k_scw_candidate_velocity[SCW_MAX], p_scw_cal->k_scw_candidate_velocity_hys, out_of_range);

   /* check for valid relative longitudinal velocity */
   out_of_range = p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_relative_long_velocity_range ? FBK_FALSE
                                                                                                                  : FBK_TRUE;
   p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_relative_long_velocity_range =
      Scw_In_Range_Hyst(p_scw_object->tracker_data.vcs_vel_rel.x, p_scw_cal->k_scw_candidate_relative_velocity[SCW_MIN],
                        p_scw_cal->k_scw_candidate_relative_velocity[SCW_MAX], p_scw_cal->k_scw_candidate_velocity_hys, out_of_range);

   if (Fbk_Is_True(p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_speed_range)
       && Fbk_Is_True(p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_relative_long_velocity_range))
   {
      f_vel_is_correct = FBK_TRUE;
   }

   return f_vel_is_correct;
}


static boolean_T Scw_Is_Dyn_Object_Heading_Correct(const Scw_Object_T *const p_scw_object,
                                                   const Scw_Core_Calibration_T *const p_scw_cal,
                                                   Scw_Persistent_T *const p_scw_persistent)
{
   boolean_T out_of_range;

   /* Asserts */
   assert(NULL != p_scw_object);
   assert(NULL != p_scw_cal);
   assert(NULL != p_scw_persistent);

   /* check for valid heading */
   /* if lower threshold == 0 then we ignore it and use only upper threshold for absolute value of the heading */
   out_of_range = p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_heading_range ? FBK_FALSE : FBK_TRUE;
   p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_heading_range =
      Scw_In_Range_Hyst(p_scw_object->tracker_data.vcs_heading, p_scw_cal->k_scw_candidate_heading[SCW_MIN],
                        p_scw_cal->k_scw_candidate_heading[SCW_MAX], p_scw_cal->k_scw_candidate_heading_hys, out_of_range);

   return p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_heading_range;
}


static boolean_T Scw_Is_Dyn_Object_Yawrate_Correct(const Scw_Object_T *const p_scw_object,
                                                   const Scw_Core_Calibration_T *const p_scw_cal,
                                                   Scw_Persistent_T *const p_scw_persistent)
{
   boolean_T out_of_range;

   /* Asserts */
   assert(NULL != p_scw_object);
   assert(NULL != p_scw_cal);
   assert(NULL != p_scw_persistent);

   /* check for valid yawrate */
   out_of_range = p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_yawrate_range ? FBK_FALSE : FBK_TRUE;
   p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_yawrate_range =
      Scw_Below_Threshold_Hyst(Fbk_Abs_F(p_scw_object->tracker_data.heading_rate), p_scw_cal->k_scw_candidate_yawrate,
                               p_scw_cal->k_scw_candidate_yawrate_hys, out_of_range);
   return p_scw_persistent->dyn_obj_data[p_scw_object->tracker_data.id].f_in_yawrate_range;
}


static float32_T Scw_Get_Dyn_Object_Lateral_Distance(const Scw_Object_T *const p_scw_object /**< Scw internal object */,
                                                     const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */)
{
   float32_T lateral_distance;
   float32_T width;

   /* Asserts */
   assert(NULL != p_scw_object);
   assert(NULL != p_scw_core_input);

   if (p_scw_core_input->trailer.f_present && (p_scw_core_input->trailer.length > FBK_ZERO_F)
       && (p_scw_core_input->trailer.width > FBK_ZERO_F))
   {
      width = Fbk_Max(p_scw_core_input->p_pa_data->vehicle_data.host_width, p_scw_core_input->trailer.width);
   }
   else
   {
      width = p_scw_core_input->p_pa_data->vehicle_data.host_width;
   }
   lateral_distance = (Fbk_Get_Obj_Side_Sign(p_scw_object->tracker_data.vcs_pos.y) * p_scw_object->nearest_corner_y) - Fbk_Half(width);

   return lateral_distance;
}


static float32_T Scw_Get_TTx(const float32_T distance,
                             const float32_T relative_velocity,
                             const Scw_TTx_Calculation_Mode_T mode,
                             const float32_T default_value_div_0,
                             const float32_T default_value)
{
   float32_T ttx;

   /* Asserts */
   assert((SCW_TTX_MODE_0 == (uint8_t) mode) || (SCW_TTX_MODE_1 == (uint8_t) mode));

   /** source is stationary in particular direction relative to destination or the relative velocity couldn't be calculated */
   if (Fbk_Equal_F(relative_velocity, FBK_ZERO_F))
   {
      ttx = default_value_div_0;
   }
   /** source moving towards the destination and there's NO lateral overlap */
   else if ((distance > FBK_ZERO_F)
            && (((SCW_TTX_MODE_0 == (uint8_t) mode) && (relative_velocity > FBK_ZERO_F))
                || ((SCW_TTX_MODE_1 == (uint8_t) mode) && (relative_velocity < FBK_ZERO_F))))
   {
      ttx = Fbk_Min(distance / Fbk_Abs_F(relative_velocity), default_value_div_0);
   }
   else
   {
      ttx = default_value;
   }

   return ttx;
}


static void Scw_Get_Critical_Guardrails(Scw_Critical_Object_T critical_guardrail[FBK_NUMBER_OF_SIDES],
                                        Scw_Persistent_T *p_scw_persistent,
                                        const Scw_Core_Input_T *const p_scw_core_input,
                                        const Fbk_Vehicle_Data_T *const p_vehicle_data,
                                        const Fbk_Field_Of_Interest_T *const p_scw_zone,
                                        const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone,
                                        const Scw_Core_Calibration_T *const p_scw_cal)
{
   uint8_t side;
   const Scw_TTx_Calculation_Mode_T lat_ttc_mode[FBK_NUMBER_OF_SIDES] = {LAT_TTC_LEFT, LAT_TTC_RIGHT};
   boolean_T is_true;

   /* Asserts */
   assert(NULL != critical_guardrail);
   assert(NULL != p_scw_persistent);
   assert(NULL != p_scw_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_scw_zone);
   assert(NULL != p_scw_hysteresis_zone);
   assert(NULL != p_scw_cal);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      is_true = Scw_Is_Guardrail_Critical(p_scw_persistent, p_scw_core_input, p_vehicle_data, p_scw_zone, p_scw_hysteresis_zone,
                                          side, p_scw_cal);
      if (is_true)
      {
         critical_guardrail[side].index            = FBK_ZERO_UINT;
         critical_guardrail[side].lateral_distance = Scw_Get_Guardrail_Lateral_Distance(p_scw_core_input, side);
         critical_guardrail[side].lateral_velocity =
            Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, SCW_VELOCITY, side);
         critical_guardrail[side].lateral_acceleration =
            Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, SCW_ACCELERATION, side);
         critical_guardrail[side].lateral_ttc =
            Scw_Get_TTx(critical_guardrail[side].lateral_distance, critical_guardrail[side].lateral_velocity, lat_ttc_mode[side],
                        p_scw_cal->k_scw_lateral_ttc_max, p_scw_cal->k_scw_lateral_ttc_default);
         critical_guardrail[side].ttle = p_scw_cal->k_scw_ttle_default;
         critical_guardrail[side].ttp  = p_scw_cal->k_scw_ttp_default;
      }
   }
}


static boolean_T Scw_Is_Valid_Guardrail(const Scw_Core_Input_T *const p_scw_core_input,
                                        const uint8_t side,
                                        const Scw_Core_Calibration_T *const p_scw_cal)
{
   boolean_T f_guardrail_valid = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_scw_core_input);
   assert(side < FBK_NUMBER_OF_SIDES);
   assert(NULL != p_scw_cal);

   if ((SCW_GUARDRAIL_VALID == p_scw_core_input->guardrail_data[side].radar.type)
       && (p_scw_core_input->guardrail_data[side].radar.confidence >= p_scw_cal->k_scw_min_exist_prob_radar_guardrail))
   {
      f_guardrail_valid = FBK_TRUE;
   }

   return f_guardrail_valid;
}

static boolean_T Scw_Is_Guardrail_Critical(Scw_Persistent_T *p_scw_persistent,
                                           const Scw_Core_Input_T *const p_scw_core_input,
                                           const Fbk_Vehicle_Data_T *const p_vehicle_data,
                                           const Fbk_Field_Of_Interest_T *const p_scw_zone,
                                           const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone,
                                           const uint8_t side,
                                           const Scw_Core_Calibration_T *const p_scw_cal)
{
   boolean_T f_guardrail_critical = FBK_FALSE;
   Vector_2d_T grail_ref_point;
   Fbk_Field_Of_Interest_T scw_zone_for_current_guardrail;
   boolean_T is_true;

   /* Asserts */
   assert(NULL != p_scw_persistent);
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_zone);
   assert(NULL != p_scw_hysteresis_zone);
   assert(side < FBK_NUMBER_OF_SIDES);
   assert(NULL != p_scw_cal);

   /* Use the vehicle center point as the guard rail reference point - x */

   grail_ref_point.x = Fbk_Half(-p_vehicle_data->host_length);

   if (Scw_Is_Valid_Guardrail(p_scw_core_input, side, p_scw_cal))
   {
      grail_ref_point.y = p_scw_core_input->guardrail_data[side].radar.lateral_position;

      /* Decide whether to use the initial zone or hysteresis zone */
      Scw_Calculate_Entity_Specific_Zone(&(scw_zone_for_current_guardrail), p_scw_zone, p_scw_hysteresis_zone, side,
                                         p_scw_persistent->count_in_zone_grail[side]);

      /* Check if the guardrail is within the zone */
      is_true = Is_Point_In_Convex_Polygon_Ray_Casting_Method(scw_zone_for_current_guardrail.points,
                                                              scw_zone_for_current_guardrail.size, &grail_ref_point);
      if (is_true)
      {
         /* Increment the count in zone if the guardrail is within the zone */
         Sat_Inc_Uint8(&(p_scw_persistent->count_in_zone_grail[side]));

         /* If the guardrail has been in the zone for enough cycles, then it is critical */
         if (p_scw_persistent->count_in_zone_grail[side] > p_scw_cal->k_scw_guardrail_cycles_in_zone_threshold)
         {
            f_guardrail_critical = FBK_TRUE;
         }
      }
      else
      {
         /* Clear the count in zone if the guardrail is not in the zone */
         p_scw_persistent->count_in_zone_grail[side] = FBK_ZERO_INT;
      }
   }
   else
   {
      /* Clear the count in zone if the guardrail is not valid */
      p_scw_persistent->count_in_zone_grail[side] = FBK_ZERO_INT;
   }

   return f_guardrail_critical;
}


static float32_T Scw_Get_Guardrail_Lateral_Distance(const Scw_Core_Input_T *const p_scw_core_input /**< Scw core input */,
                                                    const uint8_t side /**< side */)
{
   float32_T guardrail_lateral_position;
   float32_T lateral_distance;
   float32_T half_vehicle_width;

   /* Asserts */
   assert(NULL != p_scw_core_input);
   assert(side < FBK_NUMBER_OF_SIDES);

   guardrail_lateral_position = p_scw_core_input->guardrail_data[side].radar.lateral_position;
   half_vehicle_width         = Fbk_Half(p_scw_core_input->p_pa_data->vehicle_data.host_width);
   lateral_distance = (Fbk_Get_Obj_Side_Sign(guardrail_lateral_position) * guardrail_lateral_position) - half_vehicle_width;
   lateral_distance = Fbk_Max(lateral_distance, FBK_ZERO_F); /* negative distance makes no sense, the guardrail can't run through
                                                                the Host */
   return lateral_distance;
}


static void Scw_Calculate_Entity_Specific_Zone(Fbk_Field_Of_Interest_T *p_scw_entity_specific_zone,
                                               const Fbk_Field_Of_Interest_T *const p_scw_zone,
                                               const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone,
                                               const uint8_t scw_approach_side,
                                               const uint8_t in_zone_count)
{
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_scw_entity_specific_zone);
   assert(NULL != p_scw_zone);
   assert(NULL != p_scw_hysteresis_zone);
   assert(scw_approach_side < FBK_NUMBER_OF_SIDES);

   /* If the object was already in the zone once, then use the hysteresis zone*/
   if (in_zone_count > FBK_ZERO_UINT)
   {
      *p_scw_entity_specific_zone = *p_scw_hysteresis_zone;
   }
   else
   {
      *p_scw_entity_specific_zone = *p_scw_zone;
   }

   /* When the approach side is the left side, the defined zone shall be flipped to the left*/
   if (FBK_SIDE_LEFT == scw_approach_side)
   {
      for (idx = FBK_ZERO_UINT; idx < p_scw_entity_specific_zone->size; idx++)
      {
         p_scw_entity_specific_zone->points[idx].y = -p_scw_entity_specific_zone->points[idx].y;
      }
   }
}

static float32_T Scw_Nearest_Point_Lateral_Interpolation(
   const float32_T x1, const float32_T y1, const float32_T x2, const float32_T y2, const float32_T x)
{
   float32_T y_ret;


   if (Fbk_Abs_F(x2 - x1) <= EPSILON)
   {
      y_ret = Fbk_Half(y1 + y2);
   }
   else
   {
      y_ret = y1 + ((x - x1) * ((y2 - y1) / (x2 - x1)));
   }

   return y_ret;
}

static void Scw_Calculate_Nearest_Object_Corner_Lat(const Fbk_Object_Corners_T *p_object_corners,
                                                    const Fbk_Field_Of_Interest_T *p_scw_zone,
                                                    Scw_Object_T *p_scw_object,
                                                    const float32_T ego_length)
{
   float32_T sign;
   uint8_t obj_side;
   boolean_T nearest_corner_in_zone = FBK_FALSE;
   float32_T nearest_point_rear_bumper;
   float32_T nearest_point_front_bumper;
   uint8_t front_inner_corner;
   uint8_t rear_inner_corner;
   float32_T front_inner_coord;
   float32_T rear_inner_coord;
   uint8_t obj_side_corner[FBK_NUMBER_OF_SIDES][FBK_NUMBER_OF_SIDES];

   assert(NULL != p_object_corners);

   obj_side_corner[FBK_SIDE_LEFT][FBK_SIDE_FRONT]  = (uint8_t) FBK_FRONT_RIGHT_CORNER;
   obj_side_corner[FBK_SIDE_LEFT][FBK_SIDE_REAR]   = (uint8_t) FBK_REAR_RIGHT_CORNER;
   obj_side_corner[FBK_SIDE_RIGHT][FBK_SIDE_FRONT] = (uint8_t) FBK_FRONT_LEFT_CORNER;
   obj_side_corner[FBK_SIDE_RIGHT][FBK_SIDE_REAR]  = (uint8_t) FBK_REAR_LEFT_CORNER;

   sign     = Fbk_Get_Obj_Side_Sign(p_scw_object->tracker_data.vcs_pos.y);
   obj_side = Fbk_Get_Obj_Side(p_scw_object->tracker_data.vcs_pos.y);

   p_scw_object->nearest_corner_y = SCW_BIG_VALUE;

   front_inner_corner = obj_side_corner[obj_side][FBK_SIDE_FRONT];
   rear_inner_corner  = obj_side_corner[obj_side][FBK_SIDE_REAR];

   front_inner_coord = Fbk_Abs_F(p_object_corners->points[front_inner_corner].y);
   rear_inner_coord  = Fbk_Abs_F(p_object_corners->points[rear_inner_corner].y);

   /* Verify front inner corner in zone */
   if ((boolean_T) Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_scw_zone->points, p_scw_zone->size,
                                                                 &p_object_corners->points[front_inner_corner])
       && (front_inner_coord < p_scw_object->nearest_corner_y))
   {
      p_scw_object->nearest_corner_y = Fbk_Abs_F(p_object_corners->points[front_inner_corner].y);
      nearest_corner_in_zone         = FBK_TRUE;
   }
   /* Verify rear inner corner in zone */
   if ((boolean_T) Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_scw_zone->points, p_scw_zone->size,
                                                                 &p_object_corners->points[rear_inner_corner])
       && (rear_inner_coord < p_scw_object->nearest_corner_y))
   {
      p_scw_object->nearest_corner_y = Fbk_Abs_F(p_object_corners->points[rear_inner_corner].y);
      nearest_corner_in_zone         = FBK_TRUE;
   }

   /* If there is no object corner (inner side) in zone, then consider points on object inner side that will intersect with
lines of front and rear ego bumpers*/


   if (Fbk_Is_False(nearest_corner_in_zone))
   {
      nearest_point_rear_bumper = Scw_Nearest_Point_Lateral_Interpolation(
         p_object_corners->points[rear_inner_corner].x, Fbk_Abs_F(p_object_corners->points[rear_inner_corner].y),
         p_object_corners->points[front_inner_corner].x, Fbk_Abs_F(p_object_corners->points[front_inner_corner].y), -ego_length);

      nearest_point_front_bumper = Scw_Nearest_Point_Lateral_Interpolation(
         p_object_corners->points[rear_inner_corner].x, Fbk_Abs_F(p_object_corners->points[rear_inner_corner].y),
         p_object_corners->points[front_inner_corner].x, Fbk_Abs_F(p_object_corners->points[front_inner_corner].y), FBK_ZERO_F);

      p_scw_object->nearest_corner_y = sign * Fbk_Min(nearest_point_rear_bumper, nearest_point_front_bumper);
   }
   else
   {
      p_scw_object->nearest_corner_y = sign * p_scw_object->nearest_corner_y;
   }
}


static void Scw_Calculate_Farthest_Object_Corner_Long(const Fbk_Object_Corners_T *p_object_corners, Scw_Object_T *p_scw_object)
{
   uint8_t point_idx;

   assert(NULL != p_object_corners);

   for (point_idx = (uint8_t) FBK_FRONT_LEFT_CORNER; point_idx < (uint8_t) FBK_NUM_OF_OBJECT_CORNERS; point_idx++)
   {
      if (p_object_corners->points[point_idx].x < p_scw_object->rearmost_corner_x)
      {
         p_scw_object->rearmost_corner_x = p_object_corners->points[point_idx].x;
      }
   }
}


static boolean_T Scw_Is_Dyn_Object_In_Zone(Scw_Object_T *p_scw_object,
                                           const Fbk_Field_Of_Interest_T *p_scw_zone,
                                           const float32_T host_length)
{
   boolean_T f_obj_is_in_zone = FBK_FALSE;
   Fbk_Object_Corners_T object_corners;
   Fbk_Field_Of_Interest_T object_foi;
   boolean_T is_true;

   /* Asserts */
   assert(NULL != p_scw_zone);
   assert(NULL != p_scw_object);

   /*Calculate target corners*/
   Fbk_Calculate_Target_Corners(&object_corners, &(p_scw_object->tracker_data.vcs_pos), &(p_scw_object->tracker_data.vcs_heading),
                                &(p_scw_object->tracker_data.length), &(p_scw_object->tracker_data.width));
   Scw_Set_Up_Object_Zone(&object_foi, &object_corners);

   Scw_Calculate_Farthest_Object_Corner_Long(&object_corners, p_scw_object);
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, p_scw_zone, p_scw_object, host_length);


   is_true = Fbk_Are_Fields_Of_Interest_Overlapping(&object_foi, p_scw_zone);
   if (is_true)
   {
      f_obj_is_in_zone = FBK_TRUE;
      Binary_Scw_Debug_Increase_Object_Importance_Counter(p_scw_object->tracker_data.index);
   }

   return f_obj_is_in_zone;
}

static boolean_T Scw_Shall_Lat_Zone_Hyst_Be_Applied(const Scw_Core_Calibration_T *p_scw_cal, const uint8_t zone_point_index)
{
   boolean_T f_apply_lat_hyst = FBK_FALSE;

   if ((((SCW_ZONE_FRONT_RIGHT == zone_point_index) || (SCW_ZONE_MIDDLE_RIGHT == zone_point_index)
         || (SCW_ZONE_REAR_RIGHT == zone_point_index))
        && (p_scw_cal->k_scw_hys_zone_y_offset[zone_point_index] > FBK_ZERO_F))
       || (((SCW_ZONE_FRONT_LEFT == zone_point_index) || (SCW_ZONE_MIDDLE_LEFT == zone_point_index)
            || (SCW_ZONE_REAR_LEFT == zone_point_index))
           && (p_scw_cal->k_scw_hys_zone_y_offset[zone_point_index] < FBK_ZERO_F)))
   {
      f_apply_lat_hyst = FBK_TRUE;
   }

   return f_apply_lat_hyst;
}

static boolean_T Scw_Shall_Long_Zone_Hyst_Be_Applied(const Scw_Core_Calibration_T *p_scw_cal, const uint8_t zone_point_index)
{
   boolean_T f_apply_long_hyst = FBK_FALSE;

   if ((((SCW_ZONE_FRONT_RIGHT == zone_point_index) || (SCW_ZONE_FRONT_LEFT == zone_point_index))
        && (p_scw_cal->k_scw_hys_zone_x_offset[zone_point_index] > FBK_ZERO_F))
       || (((SCW_ZONE_REAR_RIGHT == zone_point_index) || (SCW_ZONE_REAR_LEFT == zone_point_index)
            || (SCW_ZONE_MIDDLE_RIGHT == zone_point_index) || (SCW_ZONE_MIDDLE_LEFT == zone_point_index))
           && (p_scw_cal->k_scw_hys_zone_x_offset[zone_point_index] < FBK_ZERO_F)))
   {
      f_apply_long_hyst = FBK_TRUE;
   }

   return f_apply_long_hyst;
}


static float32_T Scw_Calculate_Guardrail_Position_Derivative(const Scw_Persistent_T *p_scw_persistent,
                                                             const Scw_Core_Input_T *p_scw_core_input,
                                                             const uint8_t order,
                                                             const uint8_t side)
{
   float32_T derivative;
   float32_T pos_y;
   /* we assume for simplicity the constant time difference between two consecutive cycles, i.e. excution period is contant */
   float32_T delta_t = p_scw_core_input->p_pa_data->time_diff_to_last_cycle;

   if (((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side))
       && (delta_t > FBK_ZERO_F)
       /* lateral velocity can be calculated at least in the second cycle of existence in the zone */
       /* lateral acceleration can be calculated at least in the third cycle of existence in the zone */
       && (p_scw_persistent->count_in_zone_grail[side] > order))
   {
      pos_y = p_scw_core_input->guardrail_data[side].radar.lateral_position;
      switch (order)
      {
         case SCW_VELOCITY:
            derivative = (pos_y - p_scw_persistent->core_grail_lat_position[side][SCW_LAT_POS_BUFFER_SIZE - FBK_ONE_UINT]) / delta_t;
            break;
         case SCW_ACCELERATION:
            derivative = (pos_y - (2.0f * p_scw_persistent->core_grail_lat_position[side][SCW_LAT_POS_BUFFER_SIZE - FBK_ONE_UINT])
                          + p_scw_persistent->core_grail_lat_position[side][SCW_LAT_POS_BUFFER_SIZE - 2u])
                         / (delta_t * delta_t);
            break;
         default:
            derivative = FBK_ZERO_F;
            break;
      }
   }
   /* otherwise zero */
   else
   {
      derivative = FBK_ZERO_F;
   }

   return derivative;
}
