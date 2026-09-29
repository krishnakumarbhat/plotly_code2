/**
 * @file ced.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Core CED algorithm.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced.h"
#include "ced_common_functions.h"
#include "ced_create_zones.h"
#include "ced_debug_interface.h"
#include "ced_persistent_t.h"
#include "ced_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_output.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "fbk_vehicle_data_t.h"
#include "ml_interval.h"
#include "ml_line.h"
#include "ml_line_parameter.h"
#include "ml_line_parameter_t.h"
#include "ml_math.h"
#include "ml_polygon.h"
#include "ml_saturated_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief This function contains the main CED algorithm.
 *
 * The main CED algorithm checks all incoming tracker objects and filters out the objects that are relevant for the CED function.
 * In following steps those relevant objects paths are predicted for future movement and an alert level is set for objects that
 * might collide with an open door of the vehicle.
 *
 * @return void
 *
 * @SRS{SF-106,SF-50}
 * @SAE{SF-2402}
 * @SDD{SF-3563}
 * @verification{Create tests where alerts on each side are triggered.}
 */
static void Ced_Algorithm(Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                          Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                          const Ced_Core_Input_T *p_ced_core_input /**< CED core input data */,
                          const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */,
                          const Fbk_Output_T *p_fbk_output /**< FBK output data */);

/**
 * @brief Resets objects specific persistent data.
 *
 * @return void
 *
 * @SRS{SF-106,SF-50}
 * @SAE{SF-2402}
 * @SDD{SF-3575}
 * @verification{Create a test which checks whether the object persistent ced data is reset correctly to its default.}
 */
static void
Ced_Reset_Objects_Persistent(Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
                             const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table /**< List of objects from FBK  */);

/**
 * @brief Resets properties of CED object.
 *
 * @return void
 *
 * @SRS{SF-106,SF-50}
 * @SAE{SF-2402}
 * @SDD{SF-3573}
 * @verification{Create a test which checks whether the ced object data is reset correctly to its default.}
 */
static void Ced_Reset_Ced_Object(Ced_Object_T *p_ced_object /**< CED object */);

/**
 * @brief Resets all persistent data.
 *
 * @return void
 *
 * @SRS{SF-82}
 * @SAE{SF-2402}
 * @SDD{SF-3576}
 * @verification{Create a test which checks whether the ced persistent data is reset correctly to its default.}
 */
static void Ced_Reset_Persistent_Data(Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
                                      const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table /**< List of objects from FBK */);

/**
 * @brief Resets all Core output data at given side except for alert and direction.
 *
 * @return void
 *
 * @SRS{SF-106,SF-50}
 * @SAE{SF-2402}
 * @SDD{SF-3574}
 * @verification{Create a test which checks whether the core output side data is correctly reset to its default.}
 */
static void Ced_Reset_Core_Output_Side_Data(Ced_Core_Output_T *p_ced_core_output /**< CED core output */,
                                            uint8_t side_index /**< side index */);

/**
 * @brief Provide Path Tracking information to CED object if available.
 *
 * @return void
 *
 * @SRS{SF-95, SF-96}
 * @SAE{SF-2402}
 * @SDD{CSCSA-90083}
 * @verification{Create a test to check if CED object attributes are filled correctly when PT is available}
 */
static void Ced_Update_Object_Data(Ced_Object_T *p_ced_object /**< CED object */,
                                   const Ced_Core_Input_T *p_ced_core_input /**< CED core input data */,
                                   const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Checks basic validity of given object.
 *
 * @return object validity state
 *
 * @SRS{SF-110,SF-111}
 * @SAE{SF-2402}
 * @SDD{SF-3571}
 * @verification{Create an object which is not classified as reflection with a non default id, a coasted or mature status. Only in
 * those cases true shall be expected.}
 */
static boolean_T Ced_Is_Object_Valid(const Pa_Data_T *p_pa_data /**< PA data */,
                                     const uint8_t object_index /**< object index */,
                                     const Ced_Persistent_T *p_ced_persistent /**< CED persistent data */);

/**
 * @brief Checks if an object approaches the ego vehicle from the front.
 *
 * @return true if object is approaching the vehicle from the front.
 *
 * @SRS{SF-112,SF-113,SF-114,SF-115,SF-116,SF-117,CSCSA-122440,CSCSA-135438}
 * @SAE{SF-2402}
 * @SDD{SF-3444}
 * @verification{}
 */
static boolean_T Ced_Is_Front_Object_Relevant(const Ced_Object_T *p_ced_object /**< CED object */,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                              const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Checks if an object approaches the ego vehicle from the rear.
 *
 * @return true if object is approaching the vehicle from the rear.
 *
 * @SRS{SF-112,SF-113,SF-114,SF-115,SF-116,SF-117,CSCSA-122440,CSCSA-135438}
 * @SAE{SF-2402}
 * @SDD{SF-3445}
 * @verification{}
 */
static boolean_T Ced_Is_Rear_Object_Relevant(const Ced_Object_T *p_ced_object /**< CED object */,
                                             const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Checks objects relevance for CED by assessing object properties.
 *
 * Checks objects tracker properties against defined thresholds and overlap with funnel zones.
 *
 * @return object relevance state
 *
 * @SRS{SF-112,SF-113,SF-114,SF-115,SF-116,SF-117,CSCSA-123159}
 * @SAE{SF-2402}
 * @SDD{SF-3570}
 * @verification{Create an object in separate tests for the following scenarios: The objects properties existence probability,
 * longitudinal relative velocity, longitudinal velocity, absolute lateral velocity and absolute heading shall be in the specified
 * RCED or FCED validity ranges and the object shall be within the funnel zone. Only when rear ced mode or front ced mode as well
 * as the above conditions are fulfilled for the respective case, true shall be returned.}
 */
static boolean_T Ced_Is_Object_Relevant(Ced_Object_T *p_ced_object /**< CED object */,
                                        const Fbk_Field_Of_Interest_T *p_funnel_zone /**< CED funnel zone */,
                                        const Ced_Core_Input_T *p_ced_core_input /**< CED core input data */,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                        const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Checks objects center position against specified funnel zone.
 *
 * @return object funnel zone state
 *
 * @SRS{SF-85,SF-89,CSCSA-123159}
 * @SAE{SF-2402}
 * @SDD{SF-3568}
 * @verification{Create an object which has its center position in the funnel zone when the crash line offset for the longitudinal
 * position component is also considered. Only then this function shall return true.}
 */
static boolean_T Ced_Is_Object_In_Funnel_Zone(const Ced_Object_T *p_ced_object /**< CED object */,
                                              const Fbk_Field_Of_Interest_T *p_funnel_zone /**< CED funnel zone */,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                              const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Predict objects lat position and orientation at the long position of the crash line. Uses path tracking information if
 * available.
 *
 * @return void
 *
 * @SRS{SF-90,SF-91,SF-92,SF-95,SF-96,SF-109}
 * @SAE{SF-2402}
 * @SDD{SF-3572}
 * @verification{Create a ced relevant object and check that the object position is predicted correctly when path information is
 * available and when additional path information is not available.}
 */
static void Ced_Predict_Object_Pos_At_Crash_Line(Ced_Object_T *p_ced_object /**< CED object */,
                                                 const Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
                                                 const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                 const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Checks if given object is matched to a path from Path Tracking module.
 *
 * @return object path match state
 *
 * @SRS{SF-96}
 * @SAE{SF-2402}
 * @SDD{SF-3569}
 * @verification{Create a test where path tracking is enabled and where the object is matched to a longitudinal path only then true
 * shall be returned.}
 */
static boolean_T Ced_Is_Object_Matched_To_Path(const Ced_Object_T *p_ced_object /**< CED object */,
                                               const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Checks if a warning for an object should be suppressed if object is within the ego lane.
 *
 * @return true if warning should be suppressed.
 *
 * @SRS{SF-107}
 * @SAE{SF-2402}
 * @SDD{SF-3446}
 * @verification{}
 */
static boolean_T Ced_Is_Warning_Suppressed_Ego_Lane(const Ced_Object_T *p_ced_object /**< CED object */,
                                                    const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration*/);

/**
 * @brief Checks if a warning for an object should be suppressed if the reference point (corners of the object set according to the
 * FOV in which the object is located) is on the opposite side of the specified line.
 *
 * @return true if warning should be suppressed.
 *
 * @SRS{SF-108}
 * @SAE{SF-2402}
 * @SDD{CSCSA-31272}
 * @verification{}
 */
static boolean_T Ced_Is_Warning_Suppressed_Cross_Border(const Ced_Object_T *p_ced_object /**< CED object */,
                                                        const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration*/,
                                                        const Pa_Data_T *p_pa_data /**< PA data */
);

/**
 * @brief Checks if a warning for an object should be suppressed if object and warning are on different sides (due to path algo).
 *
 * @return true if warning should be suppressed.
 *
 * @SRS{SF-108}
 * @SAE{SF-2402}
 * @SDD{SF-3447}
 * @verification{}
 */
static boolean_T Ced_Is_Warning_Suppressed_Opposite_Side(const Ced_Object_T *p_ced_object /**< CED object */,
                                                         const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration*/);

/**
 * @brief Checks whether a newly created object behaves differently from its nearest path. If that is the case, the object alert
 * shall be suppressed.
 *
 * @return true when object alert should be suppressed based on nearest path information
 *
 * @SRS{SF-48}
 * @SAE{SF-2402}
 * @SDD{SF-3391}
 * @verification{Create a test where path tracking is enabled and where a path has been build up near a directly ced critical
 * object. Check that the alert is suppressed for this object.}
 */
static boolean_T Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(
   const Ced_Object_T *p_ced_object /**< CED object */, const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration*/);

/**
 * @brief Based on the given point index a point is calculated from the given zone. The point index corresponds to the
 * Ced_Points_To_Check_T enum definition.
 *
 * @return void
 *
 * @SRS{SF-87,SF-93}
 * @SAE{SF-2402}
 * @SDD{SF-3388}
 * @verification{Create a test to check whether the calculated point is set correctly based on the given zone.}
 */
static void
Ced_Calculate_Point_To_Check(Vector_2d_T *p_point_to_check /**< Point that will be calculated */,
                             const uint8_t index_point /**< Point index corresponding to Ced_Points_To_Check_T */,
                             const Fbk_Field_Of_Interest_T *p_zone_for_points /**< Zone to use for point calculation */);


/**
 * @brief Checks if any reference point of the object is within the CED warn zone.
 *
 * @return true if zone check succeeds.
 *
 * @SRS{SF-87,SF-93,CSCSA-122438}
 * @SAE{SF-2402}
 * @SDD{SF-3448}
 * @verification{}
 */
static void Ced_Is_Object_In_Zone(boolean_T *f_zone_overlap_right /**< ouput value: overlap with right zone */,
                                  boolean_T *f_zone_overlap_left /**< ouput value: overlap with left zone */,
                                  const Fbk_Field_Of_Interest_T *p_collision_zone /**< Collision zone */,
                                  const Fbk_Field_Of_Interest_T *p_object_zone /**< Object zone */);


/**
 * @brief Checks if any reference point of the CED zone is within the object zone.
 *
 * @return true if zone check succeeds.
 *
 * @SRS{SF-87,SF-93,CSCSA-122438}
 * @SAE{SF-2402}
 * @SDD{SF-3449}
 * @verification{}
 */
static void Ced_Is_Zone_In_Object(boolean_T *p_zone_overlap /**< ouput value: overlap with specified zone */,
                                  const uint8_t zone_side /**< side for which the intersection is checked */,
                                  const Fbk_Field_Of_Interest_T *p_collision_zone /**< Collision zone */,
                                  const Fbk_Field_Of_Interest_T *p_object_zone /**< Object zone */);
/**
 * @brief Checks the predicted objects position and orientation against the collision zones next to the vehicle.
 *
 * @return object collision criticality
 *
 * @SRS{SF-87,SF-93,CSCSA-122438}
 * @SAE{SF-2402}
 * @SDD{SF-3567}
 * @verification{Create an object where any of the objects corner or center points is intersecting with the collision zone. Only
 * then true shall be returned.}
 */
static boolean_T Ced_Is_Object_Collision_Critical(Ced_Object_T *p_ced_object /**< CED object */,
                                                  Fbk_Field_Of_Interest_T *p_collision_zone /**< CED collision zone */);

/**
 * @brief Check for an alert suppression reason.
 *
 * @return Ced_Alert_Suppression_T
 *
 * @SRS{SF-101,SF-103,SF-109,SF-123}
 * @SAE{SF-2402}
 * @SDD{SF-3440}
 * @verification{Check the functionality of every alert suppression mechanism.}
 */
static Ced_Alert_Suppression_T Ced_Get_Object_Alert_Suppression(Ced_Object_T *p_ced_object /**< CED object */,
                                                                const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */,
                                                                const Ced_Core_Input_T *p_ced_core_input /**<CED core input*/);

/**
 * @brief Set the Object Alert Level for given object.
 *
 * @return void
 *
 * @SRS{SF-101,SF-103,SF-109,SF-123}
 * @SAE{SF-2402}
 * @SDD{SF-3578}
 * @verification{Create an object with a time to crash line less than the specified threshold. Only when the respective thresholds
 * are below the specified thresholds and when reset functionalities do not trigger, an alert level shall be reached.}
 */
static void Ced_Set_Object_Alert_Level(Ced_Object_T *p_ced_object /**< CED object */,
                                       const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */,
                                       const Ced_Core_Input_T *p_ced_core_input /**< CED core input */,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Calculates the threshold hysteresis for closest distance of the provided object prediction to the host vehicle edge.
 *
 * @return void
 *
 * @SRS{CSCSA-261239}
 * @SAE{SF-2402}
 * @SDD{CSCSA-261204}
 * @verification{Create a test to check whetehera hysteresis value is returned.}
 */
static void Ced_Get_Lat_Distance_Hystereis(const Ced_Persistent_T *p_ced_persistent,
                                           const Ced_Core_Calibration_T *p_ced_cal,
                                           Ced_Object_T *p_ced_object);


/**
 * @brief Check whether the object is performing a parking manneuver on the host lane.
 *
 * @return true in case that a parking maneuver is performed by the approaching object.
 *
 * @SRS{SF-107}
 * @SAE{SF-2402}
 * @SDD{SF-3393}
 * @verification{Create a test to check whether true is only returned in case that a parking maneuver is occuring.}
 */
static boolean_T Ced_Is_Object_Parking_On_Ego_Lane(const Ced_Object_T *p_ced_object /**< ced object */,
                                                   const Ced_Core_Calibration_T *p_ced_cal /**< ced calibrations*/);

/**
 * @brief Fill the persistent data using the information from the given CED object.
 *
 * @return void
 *
 * @SRS{SF-106,SF-50}
 * @SAE{SF-2402}
 * @SDD{SF-3565}
 * @verification{Create a test to check whether object persistent data is set correctly.}
 */
static void Ced_Fill_Object_Persistent_Data(Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
                                            const Ced_Object_T *p_ced_object /**< CED object */);

/**
 * @brief Fill the persistent side data using the information of the core output.
 *
 * @return void
 *
 * @SRS{SF-106,SF-50}
 * @SAE{SF-2402}
 * @SDD{SF-3566}
 * @verification{Create a test to check whether ced side persistent data is set correctly.}
 */
static void Ced_Fill_Side_Persistent_Data(Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
                                          const Ced_Core_Output_T *p_ced_core_output /**< CED core output */);

/**
 * @brief Check if the given object is the object with lowest TTC value for any side it is relevant for.
 *
 * @return void
 *
 * @SRS{SF-98,SF-99,CSCSA-124084,CSCSA-124086}
 * @SAE{SF-2402}
 * @SDD{SF-3577}
 * @verification{Create an object which is side relevant and whose ttc is less than the previously stored one.}
 */
static void Ced_Set_Most_Critical_Object(Ced_Core_Output_T *p_ced_core_output /**< CED core output */,
                                         const Ced_Object_T *p_ced_object /**< CED object */);

/**
 * @brief Debounce alert level using qualifying and holding counters.
 *
 * @return void
 *
 * @SRS{SF-101,SF-102,SF-104,CSCSA-126067}
 * @SAE{SF-2402}
 * @SDD{SF-3564}
 * @verification{Create multiple tests for debouncing functionality: 1.) In case of suppression create a core output with an alert
 * which has not qualified long enough. 2.) In case of holding create a core output with an alert which has been held too long.Only
 * in those cases a alert level debouncing shall occure}
 */
static void Ced_Debounce_Alert_Level(
   Ced_Core_Output_T *p_ced_core_output /**< CED core output */,
   Ced_Persistent_T *p_ced_persistent /**< CED persistent data */,
   const Fbk_Output_T *p_fbk_output,
   const boolean_T
      ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE] /**< Information if alert holding on object can be skipped */,
   const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */,
   const Pa_Data_T *p_pa_data /**< PA data*/,
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Checks if an alert on a given object should be held.
 *
 * @return object alert holding state
 *
 * @SRS{SF-102,CSCSA-126067}
 * @SAE{SF-2402}
 * @SDD{SF-3579}
 * @verification{Create a test where multiple scenarios are tested: 1.) the check is disabled 2.) the check is enabled and object
 * status is not invalid, the heading and the longitudinal velocity are in valid ranges. Only in those cases true shall be
 * returned.}
 */
static boolean_T Ced_Should_Object_Alert_Be_Held(const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */,
                                                 const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                 const Pa_Data_T *p_pa_data /**< PA data */,
                                                 const Fbk_Output_T *p_fbk_output /**< FBK output data */,
                                                 const uint8_t object_id /**< object id */);

/**
 * @brief Checks the given object id for previous cycle alerts.
 *
 * @return True if object was alerted in previous cycle
 *
 * @SRS{SF-110,SF-111}
 * @SAE{SF-2402}
 * @SDD{SF-3389}
 * @verification{Function returns the correct value when the given id was alerted in the previous cycle.}
 */
static boolean_T Ced_Was_Object_Alerted_In_Prev_Cycle(const uint8_t object_id /**< Object id */,
                                                      const Ced_Persistent_T *p_ced_persistent /**< CED persistent data */);

/**
 * @brief Checks if the object is matched to a path with an active alert.
 *
 * @return True if object is matched to a path with an active alert
 *
 * @SRS{SF-90,SF-91,SF-92,SF-95,SF-96,SF-109}
 * @SAE{SF-2402}
 * @SDD{SF-3452}
 * @verification{Function returns the correct value when object is matched to a path with an active alert.}
 */
static boolean_T Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(
   const Ced_Object_T *p_ced_object /**< CED object */, const Ced_Persistent_T *p_ced_persistent /**< CED persistent data */);


/**
 * @brief Calculates the closest distance of the provided object prediction to the host vehicle edge.
 *
 * @return Closest lateral distance of predicted object to host vehicle edge in meters
 *
 * @SRS{SF-87,SF-93}
 * @SAE{SF-2402}
 * @SDD{SF-3451}
 * @verification{Function returns the correct lateral distance based on the provided predicted object information.}
 */
static float32_T Ced_Get_Closest_Predicted_Lateral_Distance(const Fbk_Field_Of_Interest_T *p_object_zone /**< CED object zone*/);

/**
 * @brief Calculates the flipped heading value depending on sign.
 *
 * @return Flipped heading value by 180 degrees.
 *
 * @SRS{SF-112}
 * @SAE{SF-2402}
 * @SDD{SF-3454}
 * @verification{Function returns the 180 deg flipped heading value.}
 */
static float32_T Ced_Flip_Heading_Value(const float32_T heading_value /**< Heading value [rad]*/);

/**
 * @brief Function adapts the object's predicted heading if object is in ego lane.
 * (DDG-4307)
 *
 * @return void
 *
 * @SRS{SF-92}
 * @SAE{SF-2402}
 * @SDD{CSCSA-165203}
 * @verification{Create test with different set of object positions and speeds and checkt if the predicted heading was changed}
 */
static void Ced_Adapt_Heading_For_Ego_Lane(Ced_Object_T *p_ced_object /**< CED object */,
                                           const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Function adapts the object's predicted heading basing on its lateral velocity while it is taking a slight turn.
 * (DDG-4100)
 *
 * @return void
 *
 * @SRS{SF-92}
 * @SAE{SF-2402}
 * @SDD{CSCSA-122419}
 * @verification{Create test with different set of object positions and speeds and checkt if the predicted heading was changed}
 */
static void Ced_Adapt_Heading_During_Slight_Turns(Ced_Object_T *p_ced_object /**< CED object */,
                                                  const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);

/**
 * @brief Function calculates lateral position with applied specific shift
 *
 * @return Shifted y position
 *
 * @SRS{SF-92}
 * @SAE{SF-2402}
 * @SDD{CSCSA-124424}
 * @verification{Check function on set of objects with different long positions}
 */
static float32_T Ced_Get_Lat_Position(const Ced_Object_T *p_ced_object /**< CED object */,
                                      const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */);


/*===========================================================================*\
 * Global Functions	Definition
 \*===========================================================================*/

void Ced_Core_Run(Ced_Core_Output_T *p_ced_core_output,
                  const Ced_Core_Input_T *p_ced_core_input,
                  Ced_Persistent_T *p_ced_persistance,
                  const Ced_Core_Calibration_T *p_ced_cal,
                  const Fbk_Output_T *p_fbk_output)
{
   /* Asserts */
   assert(NULL != p_ced_core_output);
   assert(NULL != p_ced_core_input);
   assert(NULL != p_ced_persistance);
   assert(NULL != p_ced_cal);
   assert(NULL != p_fbk_output);

   /* Reset debug data */
   Binary_Ced_Debug_Reset_Data();

   /* Check if CED algorithm should be called this cycle */
   if (Fbk_Is_False(p_ced_core_input->f_ced_enable))
   {
      Ced_Reset(p_ced_core_output, p_ced_persistance, p_fbk_output->p_index_id_lookup_table);
   }
   else
   {
      Ced_Algorithm(p_ced_core_output, p_ced_persistance, &(p_ced_core_input->p_pa_data->vehicle_data), p_ced_core_input,
                    p_ced_cal, p_fbk_output);
   }

   /* Pass general data to debug data */
   Binary_Ced_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistance, p_ced_cal);
}

void Ced_Reset(Ced_Core_Output_T *p_ced_core_output,
               Ced_Persistent_T *p_ced_persistent,
               const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table)
{
   uint8_t side_index;

   /* Assert */
   assert(NULL != p_ced_core_output);
   assert(NULL != p_ced_persistent);
   assert(NULL != p_index_id_lookup_table);

   /* Reset persistent data */
   Ced_Reset_Persistent_Data(p_ced_persistent, p_index_id_lookup_table);

   /* Reset core output for both sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      Ced_Reset_Core_Output_Side_Data(p_ced_core_output, side_index);
   }
}


/*===========================================================================*\
* Local Functions Definition
\*===========================================================================*/

static boolean_T Ced_Was_Object_Alerted_In_Prev_Cycle(const uint8_t object_id, const Ced_Persistent_T *p_ced_persistent)
{
   /* Return value */
   boolean_T f_object_was_alerted = FBK_FALSE;
   uint8_t side_index;

   /* Assert */
   assert(NULL != p_ced_persistent);

   /* Check if the provided object was critical in the previous cycle on one of the vehicle sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      if ((CED_NO_ALERT != p_ced_persistent->ced_side_alert_prev_cycle[side_index])
          && (object_id == p_ced_persistent->ced_side_id_prev_cycle[side_index]))
      {
         f_object_was_alerted = FBK_TRUE;
         break;
      }
   }

   return f_object_was_alerted;
}

static boolean_T Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(const Ced_Object_T *p_ced_object,
                                                                        const Ced_Persistent_T *p_ced_persistent)
{
   /* Return value */
   boolean_T f_object_matched_to_critical_path = FBK_FALSE;
   uint8_t side_index;

   /* Assert */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_persistent);

   if (NULL != p_ced_object->attributes.p_pt_match_info)
   {
      /* Check if the object path match index is equal to the path match index of the alerted object on one of the vehicle sides */
      for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
      {
         if ((CED_NO_ALERT != p_ced_persistent->ced_side_alert_prev_cycle[side_index])
             && (p_ced_object->attributes.p_pt_match_info->track_match
                 == p_ced_persistent->ced_side_path_match_index_prev_cycle[side_index])
             && (PT_DEFAULT_MATCH_INDEX != p_ced_object->attributes.p_pt_match_info->track_match))
         {
            f_object_matched_to_critical_path = FBK_TRUE;
            break;
         }
      }
   }
   return f_object_matched_to_critical_path;
}

static void Ced_Reset_Ced_Object(Ced_Object_T *p_ced_object)
{
   uint8_t i;

   /* Assert */
   assert(NULL != p_ced_object);

   /* Reset tracker object data. */
   Fbk_Reset_Object_Data(&(p_ced_object->tracker_data));

   /* Reset object attributes. */
   p_ced_object->attributes.ego_side                   = UNDEF_SIDE;
   p_ced_object->attributes.front_bumper_pos_long      = FBK_ZERO_F;
   p_ced_object->attributes.rear_bumper_pos_long       = FBK_ZERO_F;
   p_ced_object->attributes.position_predicted.x       = FBK_ZERO_F;
   p_ced_object->attributes.position_predicted.y       = FBK_ZERO_F;
   p_ced_object->attributes.heading_predicted          = FBK_ZERO_F;
   p_ced_object->attributes.length_predicted           = FBK_ZERO_F;
   p_ced_object->attributes.width_predicted            = FBK_ZERO_F;
   p_ced_object->attributes.closest_lat_dist_predicted = CED_INVALID_DISTANCE;
   p_ced_object->attributes.distance_to_crash_line     = CED_INVALID_DISTANCE;
   p_ced_object->attributes.time_to_crash_line         = CED_INVALID_TIME;
   p_ced_object->attributes.time_to_pass_crash_line    = CED_INVALID_TIME;
   p_ced_object->attributes.p_pt_match_info            = NULL;
   p_ced_object->attributes.p_pt_nearest_path_info     = NULL;
   p_ced_object->attributes.alert_level                = CED_NO_ALERT;
   p_ced_object->attributes.alert_side                 = INTERSEC_UNDEF_SIDE;
   p_ced_object->attributes.direction                  = FBK_SIDE_UNDEFINED;
   p_ced_object->attributes.f_skip_alert_holding       = FBK_FALSE;
   for (i = 0u; i < CED_ALERT_LEVELS; i++)
   {
      p_ced_object->attributes.zone_width_hys[i] = FBK_ZERO_F;
   }
}

static void Ced_Update_Object_Data(Ced_Object_T *p_ced_object,
                                   const Ced_Core_Input_T *p_ced_core_input,
                                   const Ced_Core_Calibration_T *p_ced_cal)
{
   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_core_input);

   /* Check if Path Tracking Data is available */
   if ((NULL != p_ced_core_input->p_pt_output) && (p_ced_cal->k_ced_f_path_tracking_enable))
   {
      p_ced_object->attributes.p_pt_match_info =
         &(p_ced_core_input->p_pt_output->path_obj_pair_output[p_ced_object->tracker_data.index]);
      p_ced_object->attributes.p_pt_nearest_path_info =
         &(p_ced_core_input->p_pt_output->nearest_path_output[p_ced_object->tracker_data.index]);
   }
   else
   {
      p_ced_object->attributes.p_pt_match_info        = NULL;
      p_ced_object->attributes.p_pt_nearest_path_info = NULL;
   }
}


static boolean_T Ced_Is_Object_Valid(const Pa_Data_T *p_pa_data, const uint8_t object_index, const Ced_Persistent_T *p_ced_persistent)
{
   boolean_T f_object_valid = FBK_FALSE;
   boolean_T f_obj_state_val;
   boolean_T f_obj_refl;
   boolean_T f_obj_al_prev_cycl;

   Pa_Obj_Status_T obj_state;

   /* Assert */
   assert(NULL != p_pa_data);
   assert(NULL != p_ced_persistent);

   obj_state       = p_pa_data->object_data[object_index].status;
   f_obj_state_val = (boolean_T) Fbk_Is_Obj_State_Valid(obj_state);

   if (f_obj_state_val)
   {
      f_obj_refl         = (boolean_T) (Fbk_Is_False(p_pa_data->object_data[object_index].f_reflection));
      f_obj_al_prev_cycl = (boolean_T) Ced_Was_Object_Alerted_In_Prev_Cycle(p_pa_data->object_data[object_index].id, p_ced_persistent);
      if (f_obj_refl || f_obj_al_prev_cycl)
      {
         f_object_valid = FBK_TRUE;
         Binary_Ced_Debug_Increase_Object_Importance_Counter(object_index);
      }
   }
   return f_object_valid;
}

static boolean_T Ced_Is_Rear_Object_Relevant(const Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_object_approaching_from_rear = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check if target is approaching from ego rear. */
   if ((p_ced_object->tracker_data.existence_probability >= p_ced_cal->k_ced_object_existence_probability_min)
       && (p_ced_object->tracker_data.vcs_vel_rel.x >= p_ced_cal->k_ced_object_long_vel_rel_min)
       && (p_ced_object->tracker_data.vcs_vel_rel.x <= p_ced_cal->k_ced_object_long_vel_rel_max)
       && (p_ced_object->tracker_data.vcs_vel.x >= p_ced_cal->k_ced_object_long_vel_min)
       && (p_ced_object->tracker_data.speed <= p_ced_cal->k_ced_object_vel_max)
       && (FBK_ZERO_F >= (p_ced_object->tracker_data.vcs_pos.x - p_ced_object->tracker_data.length))
       && (Fbk_Abs_F(p_ced_object->tracker_data.vcs_vel.y) <= p_ced_cal->k_ced_object_lat_vel_max)
       && (Fbk_Abs_F(p_ced_object->tracker_data.vcs_heading) <= p_ced_cal->k_ced_object_heading_abs_angle_max)
       && (p_ced_object->tracker_data.age >= p_ced_cal->k_ced_object_age_min))
   {
      f_object_approaching_from_rear = FBK_TRUE;
   }

   /* Return if target approaches from rear. */
   return f_object_approaching_from_rear;
}

static boolean_T Ced_Is_Front_Object_Relevant(const Ced_Object_T *p_ced_object,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data,
                                              const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_object_approaching_from_front = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);

   /* Check if target is approaching from ego front. */
   if ((p_ced_object->tracker_data.existence_probability >= p_ced_cal->k_ced_object_ftm_existence_probability_min)
       && (p_ced_object->tracker_data.vcs_vel_rel.x <= -p_ced_cal->k_ced_object_ftm_long_vel_rel_min)
       && (p_ced_object->tracker_data.vcs_vel_rel.x >= -p_ced_cal->k_ced_object_ftm_long_vel_rel_max)
       && (p_ced_object->tracker_data.vcs_vel.x <= -p_ced_cal->k_ced_object_ftm_long_vel_min)
       && (p_ced_object->tracker_data.speed <= p_ced_cal->k_ced_object_vel_max)
       && (-(p_vehicle_data->host_length) <= (p_ced_object->tracker_data.vcs_pos.x + p_ced_object->tracker_data.length))
       && (Fbk_Abs_F(p_ced_object->tracker_data.vcs_vel.y) <= p_ced_cal->k_ced_object_ftm_lat_vel_max)
       && (Fbk_Abs_F(p_ced_object->tracker_data.vcs_heading) >= p_ced_cal->k_ced_object_ftm_heading_abs_angle_min)
       && (p_ced_object->tracker_data.age >= p_ced_cal->k_ced_object_ftm_age_min))
   {
      f_object_approaching_from_front = FBK_TRUE;
   }

   /* Return if target approaches from front. */
   return f_object_approaching_from_front;
}

static boolean_T Ced_Is_Object_Relevant(Ced_Object_T *p_ced_object,
                                        const Fbk_Field_Of_Interest_T *p_funnel_zone,
                                        const Ced_Core_Input_T *p_ced_core_input,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                                        const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_object_relevant;
   boolean_T f_appr;
   boolean_T f_in_funn;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_funnel_zone);
   assert(NULL != p_ced_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);

   /* Determine object direction by checking object velocity */
   if (p_ced_object->tracker_data.vcs_vel.x >= FBK_ZERO_F)
   {
      /* Check targets approaching from ego rear */
      f_appr = (boolean_T) Ced_Is_Rear_Object_Relevant(p_ced_object, p_ced_cal);
      /* Check rear funnel zones */
      p_ced_object->attributes.direction = f_appr ? FBK_SIDE_REAR : FBK_SIDE_UNDEFINED;
      f_in_funn         = (boolean_T) Ced_Is_Object_In_Funnel_Zone(p_ced_object, p_funnel_zone, p_vehicle_data, p_ced_cal);
      f_object_relevant = (boolean_T) (p_ced_core_input->f_ced_rear_mode && f_appr && f_in_funn);
   }
   else
   {
      /* Check targets approaching from ego front */
      f_appr = (boolean_T) Ced_Is_Front_Object_Relevant(p_ced_object, p_vehicle_data, p_ced_cal);
      /* Check front funnel zones */
      p_ced_object->attributes.direction = f_appr ? FBK_SIDE_FRONT : FBK_SIDE_UNDEFINED;
      f_in_funn         = (boolean_T) Ced_Is_Object_In_Funnel_Zone(p_ced_object, p_funnel_zone, p_vehicle_data, p_ced_cal);
      f_object_relevant = (boolean_T) (f_in_funn && f_appr && p_ced_core_input->f_ced_front_mode);
   }

   if (Fbk_Is_True(f_object_relevant))
   {
      Binary_Ced_Debug_Increase_Object_Importance_Counter(p_ced_object->tracker_data.index);
   }

   return f_object_relevant;
}


static boolean_T Ced_Is_Object_In_Funnel_Zone(const Ced_Object_T *p_ced_object,
                                              const Fbk_Field_Of_Interest_T *p_funnel_zone,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data,
                                              const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_object_in_funnel_zone;
   Vector_2d_T point_to_check;
   float32_T crash_line_offset;
   Fbk_Object_Corners_T corners;
   Fbk_Field_Of_Interest_T funnel_zone_upd = *p_funnel_zone;
   float32_T dir_sign                      = FBK_ONE_F; /* Positive sign when object approaches from front. */

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_funnel_zone);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);

   /* Calculate crash line offset from vehicle data and cal value. Its purpose is to shift funnel zone from ego front
    * bumper towards ego rear bumper. */
   crash_line_offset = p_vehicle_data->host_length
                       * p_ced_cal->k_ced_crash_line_host_length_percentage[(uint8_t) p_ced_object->attributes.direction];

   if (FBK_SIDE_REAR == p_ced_object->attributes.direction)
   {
      dir_sign = -FBK_ONE_F;
   }

   /* Modify funnel zone depending on object direction and shift it by crashline */
   funnel_zone_upd.points[0].x = (dir_sign * funnel_zone_upd.points[0].x) - crash_line_offset;
   funnel_zone_upd.points[1].x = (dir_sign * funnel_zone_upd.points[1].x) - crash_line_offset;
   funnel_zone_upd.points[2].x = (dir_sign * funnel_zone_upd.points[2].x) - crash_line_offset;
   funnel_zone_upd.points[3].x = (dir_sign * funnel_zone_upd.points[3].x) - crash_line_offset;


   switch (p_ced_cal->k_ced_f_choose_ref_point_funnel_check)
   {
      case (uint8_t) CED_FRONT_BUMPER_REFERENCE:
         point_to_check.x = (p_ced_object->tracker_data.vcs_pos.x + Fbk_Half(p_ced_object->tracker_data.length));
         point_to_check.y = Fbk_Abs_F(Ced_Get_Lat_Position(p_ced_object, p_ced_cal));
         break;

      case (uint8_t) CED_NEAREST_CORNER_REFERENCE:
         Fbk_Calculate_Target_Corners(&corners, &(p_ced_object->tracker_data.vcs_pos), &(p_ced_object->tracker_data.vcs_heading),
                                      &(p_ced_object->tracker_data.length), &(p_ced_object->tracker_data.width));
         if (Ced_Get_Lat_Position(p_ced_object, p_ced_cal) > FBK_ZERO_F)
         {
            point_to_check.x = corners.points[FBK_FRONT_LEFT_CORNER].x;
            point_to_check.y = Fbk_Abs_F(corners.points[FBK_FRONT_LEFT_CORNER].y);
         }
         else
         {
            point_to_check.x = corners.points[FBK_FRONT_RIGHT_CORNER].x;
            point_to_check.y = Fbk_Abs_F(corners.points[FBK_FRONT_RIGHT_CORNER].y);
         }
         break;
      case (uint8_t) CED_DEFAULT_REFERENCE:
      default:
         point_to_check.x = p_ced_object->tracker_data.vcs_pos.x;
         point_to_check.y = Fbk_Abs_F(Ced_Get_Lat_Position(p_ced_object, p_ced_cal));
         /* Check the approximate middle of the objects side. The influence of the objects heading is negligible.  */
         point_to_check.y = Fbk_Max(EPSILON, (point_to_check.y - Fbk_Half(p_ced_object->tracker_data.width)));
         break;
   }

   f_object_in_funnel_zone =
      (boolean_T) Is_Point_In_Convex_Polygon_Ray_Casting_Method(funnel_zone_upd.points, funnel_zone_upd.size, &point_to_check);

   return f_object_in_funnel_zone;
}

static void Ced_Predict_Object_Pos_At_Crash_Line(Ced_Object_T *p_ced_object,
                                                 const Ced_Persistent_T *p_ced_persistent,
                                                 const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                 const Ced_Core_Calibration_T *p_ced_cal)
{
   /* Set up line segment */
   Line_Parameter_T line;

   float32_T distance_based_width_increase_factor;
   float32_T max_width_increase_factor;
   float32_T obj_bumper_offset;
   float32_T obj_rear_distance_to_crash_line;
   float32_T obj_long_acceleration_weighted;
   boolean_T f_obj_match;
   boolean_T f_obj_dist;
   boolean_T alert_prev_cycle;
   float32_T width_slope_offset = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_persistent);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);


   /* Set longitudinal prediction position */
   p_ced_object->attributes.position_predicted.x =
      -p_vehicle_data->host_length * p_ced_cal->k_ced_crash_line_host_length_percentage[(uint8_t) p_ced_object->attributes.direction];

   /* Set object bumper positions */
   obj_bumper_offset = Fbk_Half(p_ced_object->tracker_data.length) * Fast_Cos(p_ced_object->tracker_data.vcs_heading);
   p_ced_object->attributes.front_bumper_pos_long = p_ced_object->tracker_data.vcs_pos.x + obj_bumper_offset;
   p_ced_object->attributes.rear_bumper_pos_long  = p_ced_object->tracker_data.vcs_pos.x - obj_bumper_offset;

   /* Calculate distance and object front and rear (longitudinally) based on object direction */
   if (FBK_SIDE_FRONT == p_ced_object->attributes.direction)
   {
      /* Object approaching from front. */
      p_ced_object->attributes.distance_to_crash_line =
         -p_ced_object->attributes.position_predicted.x + p_ced_object->attributes.front_bumper_pos_long;
      obj_rear_distance_to_crash_line = -p_ced_object->attributes.position_predicted.x + p_ced_object->attributes.rear_bumper_pos_long;
      obj_long_acceleration_weighted = -p_ced_object->tracker_data.vcs_accel.x * p_ced_cal->k_ced_object_acceleration_weight;
   }
   else
   {
      /* Object approaching from rear. */
      p_ced_object->attributes.distance_to_crash_line =
         p_ced_object->attributes.position_predicted.x - p_ced_object->attributes.front_bumper_pos_long;
      obj_rear_distance_to_crash_line = p_ced_object->attributes.position_predicted.x - p_ced_object->attributes.rear_bumper_pos_long;
      obj_long_acceleration_weighted = p_ced_object->tracker_data.vcs_accel.x * p_ced_cal->k_ced_object_acceleration_weight;
   }

   /* Check for non-zero relative velocity in order to be able to calculate valid TTC and TTP */
   if (Fbk_Abs_F(p_ced_object->tracker_data.vcs_vel_rel.x) >= EPSILON)
   {
      /* Calculate time to reach the crash line (TTC) */
      p_ced_object->attributes.time_to_crash_line =
         Ced_Get_Time_To_Travel_Given_Distance(p_ced_object->attributes.distance_to_crash_line,
                                               Fbk_Abs_F(p_ced_object->tracker_data.vcs_vel_rel.x), obj_long_acceleration_weighted);

      /* Calculate time it takes for the objects whole length to pass the crash line (TTP) */
      p_ced_object->attributes.time_to_pass_crash_line = Ced_Get_Time_To_Travel_Given_Distance(
         obj_rear_distance_to_crash_line, Fbk_Abs_F(p_ced_object->tracker_data.vcs_vel_rel.x), obj_long_acceleration_weighted);
   }

   /* Check if path information available and distance to crash line is above threshold */
   f_obj_match = (boolean_T) Ced_Is_Object_Matched_To_Path(p_ced_object, p_ced_cal);
   f_obj_dist =
      (boolean_T) (p_ced_object->attributes.distance_to_crash_line >= p_ced_cal->k_ced_object_min_dist_to_crash_line_for_path_match);

   if (f_obj_match && f_obj_dist)
   {
      /* Path information available */
      float32_T range_at_crash_line =
         p_ced_object->attributes.p_pt_match_info->range_at_zero
         + (p_ced_cal->k_ced_crash_line_host_length_percentage[(uint8_t) p_ced_object->attributes.direction]
            * (p_ced_object->attributes.p_pt_match_info->range_at_host_edge - p_ced_object->attributes.p_pt_match_info->range_at_zero));

      /* The offset to the path at crash line can be weighted to adapt its influence */
      float32_T offset_to_path =
         p_ced_object->attributes.p_pt_match_info->range_to_current_path_part * p_ced_cal->k_ced_offset_to_path_weight;

      /* Set predicted position to range at crash line including offset to path */
      p_ced_object->attributes.position_predicted.y = range_at_crash_line + offset_to_path;

      /* Set predicted heading based on path heading at longitudinal zero */
      p_ced_object->attributes.heading_predicted = p_ced_object->attributes.p_pt_match_info->path_heading;

      /* Adapt heading for object taking slight turns in order to avoid FPs and FNs */
      Ced_Adapt_Heading_During_Slight_Turns(p_ced_object, p_ced_cal);

      /*Adapt heading for object in ego lane */
      Ced_Adapt_Heading_For_Ego_Lane(p_ced_object, p_ced_cal);

      /* Set maximum width increase factor for case of path match */
      max_width_increase_factor = p_ced_cal->k_ced_object_max_width_increase_factor_with_path_match;
   }
   else
   {
      /* Flip heading of front approaching object to get a continuous function, otherwise the heading value flips from -pi to pi. */
      if ((FBK_SIDE_FRONT == p_ced_object->attributes.direction))
      {
         p_ced_object->tracker_data.vcs_heading = Ced_Flip_Heading_Value(p_ced_object->tracker_data.vcs_heading);
      }

      /* No path information available */
      if (Fbk_Is_True(p_ced_cal->k_ced_f_enable_heading_exp_moving_average))
      {
         /* Calculate exponential moving average for object heading */
         p_ced_object->attributes.heading_predicted =
            (p_ced_cal->k_ced_object_heading_exp_moving_average_alpha * p_ced_object->tracker_data.vcs_heading)
            + ((FBK_ONE_F - p_ced_cal->k_ced_object_heading_exp_moving_average_alpha)
               * p_ced_persistent->ced_object_heading_predicted[p_ced_object->tracker_data.id]);
      }
      else
      {
         p_ced_object->attributes.heading_predicted = p_ced_object->tracker_data.vcs_heading;
      }

      /* Weight the predicted heading that will be used without path match. This can be used as a temporary fix to ignore an
       * inaccurate tracker heading for the predicted object. */
      p_ced_object->attributes.heading_predicted =
         p_ced_object->attributes.heading_predicted * p_ced_cal->k_ced_object_heading_predicted_weight;

      /* Adapt heading for object taking slight turns in order to avoid FPs and FNs */
      Ced_Adapt_Heading_During_Slight_Turns(p_ced_object, p_ced_cal);

      /*Adapt heading for object in ego lane */
      Ced_Adapt_Heading_For_Ego_Lane(p_ced_object, p_ced_cal);

      /* Flip heading of front approaching object back */
      if ((FBK_SIDE_FRONT == p_ced_object->attributes.direction))
      {
         p_ced_object->tracker_data.vcs_heading     = Ced_Flip_Heading_Value(p_ced_object->tracker_data.vcs_heading);
         p_ced_object->attributes.heading_predicted = Ced_Flip_Heading_Value(p_ced_object->attributes.heading_predicted);
      }

      /* Extrapolate exponential moving averaged heading from current position to crash line */
      line.p0.x        = p_ced_object->tracker_data.vcs_pos.x;
      line.p0.y        = Ced_Get_Lat_Position(p_ced_object, p_ced_cal);
      line.direction.x = Fast_Cos(p_ced_object->attributes.heading_predicted);
      line.direction.y = Fast_Sin(p_ced_object->attributes.heading_predicted);

      p_ced_object->attributes.position_predicted.y = Get_Y_Value_From_Line(&line, p_ced_object->attributes.position_predicted.x);

      /* Set maximum width increase factor for case of no path match */
      max_width_increase_factor = p_ced_cal->k_ced_object_max_width_increase_factor_without_path_match;
   }

   /* Set maximum width increase factor for case when there was an alert in previous cycle */
   alert_prev_cycle = Ced_Was_Object_Alerted_In_Prev_Cycle(p_ced_object->tracker_data.id, p_ced_persistent);
   if (alert_prev_cycle)
   {
      max_width_increase_factor *= p_ced_cal->k_ced_object_predicted_max_width_slope_reduce_factor;
      width_slope_offset = p_ced_cal->k_ced_object_predicted_max_width_slope_offset;
   }

   /* Increase width of predicted object to represent prediction inaccuracies based on distance and path match property */
   distance_based_width_increase_factor =
      (p_ced_object->attributes.distance_to_crash_line / p_ced_cal->k_ced_funnel_zone_length) * max_width_increase_factor;


   p_ced_object->attributes.width_predicted =
      (p_ced_object->tracker_data.width * (FBK_ONE_F + distance_based_width_increase_factor)) + width_slope_offset;

   /* Set predicted length of object (size increase in length not necessary or beneficial) */
   p_ced_object->attributes.length_predicted = p_ced_object->tracker_data.length;

   /* If object was critical in last cycle add safety margin to predicted object width in order to avoid warning interruptions */
   if (alert_prev_cycle)
   {
      p_ced_object->attributes.width_predicted =
         p_ced_object->attributes.width_predicted + p_ced_cal->k_ced_object_width_safety_margin_for_active_alert;
   }
   else
   {
      /* Otherwise if object is matched to a path with an active warning add safety margin to predicted object width in order to
       * avoid warning interruptions in convoys */
      if (Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(p_ced_object, p_ced_persistent))
      {
         p_ced_object->attributes.width_predicted =
            p_ced_object->attributes.width_predicted + p_ced_cal->k_ced_object_width_safety_margin_for_critical_path_match;
      }
   }
}

static boolean_T Ced_Is_Object_Matched_To_Path(const Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_object_matched = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check if Path Tracking is enabled and available */
   if ((Fbk_Is_True(p_ced_cal->k_ced_f_path_tracking_enable)) && (NULL != p_ced_object->attributes.p_pt_match_info)
       && (PT_DEFAULT_MATCH_INDEX != p_ced_object->attributes.p_pt_match_info->track_match)
       && ((PATH_STATUS_MATURE <= p_ced_object->attributes.p_pt_match_info->path_state)
           || Fbk_Is_False(p_ced_cal->k_ced_f_use_only_mature_paths)))
   {
      /* Check if the matched path is longitudinal */
      if ((PATH_DIRECTION_LONG_FORWARD == p_ced_object->attributes.p_pt_match_info->path_direction)
          || (PATH_DIRECTION_LONG_BACKWARD == p_ced_object->attributes.p_pt_match_info->path_direction))
      {
         f_object_matched = FBK_TRUE;
      }
   }

   return f_object_matched;
}


static boolean_T Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(const Ced_Object_T *p_ced_object,
                                                                         const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_suppress_obj_alert_nearest_path = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check if Path Tracking is enabled and available */
   if (Fbk_Is_True(p_ced_cal->k_ced_f_path_tracking_enable) && (NULL != p_ced_object->attributes.p_pt_match_info)
       && (NULL != p_ced_object->attributes.p_pt_nearest_path_info))
   {
      boolean_T f_is_obj_matched_to_path;

      /*Check whether the object is matched to a path for a minimum amount of cycles.*/
      f_is_obj_matched_to_path = (boolean_T) ((PT_DEFAULT_MATCH_INDEX != p_ced_object->attributes.p_pt_match_info->track_match)
                                              && (p_ced_object->attributes.p_pt_match_info->track_match_age
                                                  >= p_ced_cal->k_ced_min_cycles_for_path_match_for_no_suppress));

      /* Check whether the nearest path, in case that one is existing, is close enough to the object, that the heading
       * difference between tracker heading and path heading is exceeding the given threshold and that the object is newly
       * created. In case that the object does not have a path match for a minimum amount of consecutive cycles but is
       * fulfilling the other criteria, it shall be suppressed, since this means, that the object is behaving differently from
       * the recorded trajectories due to e.g. initialization by tracker.*/
      if ((PT_DEFAULT_MATCH_INDEX != p_ced_object->attributes.p_pt_nearest_path_info->track_idx_nearest_path)
          && Fbk_Is_False(f_is_obj_matched_to_path)
          && (p_ced_object->attributes.p_pt_nearest_path_info->segment_heading_diff
              >= p_ced_cal->k_ced_suppress_pt_heading_diff_ced_alert_max)
          && (p_ced_object->attributes.p_pt_nearest_path_info->range_vcs_proj_to_path_segment
              > p_ced_cal->k_ced_suppress_range_to_nearest_path_max)
          && (p_ced_object->tracker_data.age <= p_ced_cal->k_ced_suppress_alert_object_age_max))
      {
         f_suppress_obj_alert_nearest_path = FBK_TRUE;
      }
   }

   return f_suppress_obj_alert_nearest_path;
}

static void Ced_Calculate_Point_To_Check(Vector_2d_T *p_point_to_check,
                                         const uint8_t index_point,
                                         const Fbk_Field_Of_Interest_T *p_zone_for_points)
{
   assert(NULL != p_point_to_check);
   assert(NULL != p_zone_for_points);

   /* First calculate the point to check from the zone edge points. */
   if (index_point == (uint8_t) CED_POINT_MIDDLE)
   {
      /* Calculated middle point of zone. */
      p_point_to_check->x =
         p_zone_for_points->points[CED_POINT_FRONT_LEFT].x
         + (Fbk_Half(p_zone_for_points->points[CED_POINT_REAR_RIGHT].x - p_zone_for_points->points[CED_POINT_FRONT_LEFT].x));
      p_point_to_check->y =
         p_zone_for_points->points[CED_POINT_FRONT_LEFT].y
         + (Fbk_Half(p_zone_for_points->points[CED_POINT_REAR_RIGHT].y - p_zone_for_points->points[CED_POINT_FRONT_LEFT].y));
   }
   else if (index_point == (uint8_t) CED_POINT_MIDDLE_LEFT)
   {
      /* Calculated middle point of left side of zone. */
      p_point_to_check->x =
         p_zone_for_points->points[CED_POINT_FRONT_LEFT].x
         + (Fbk_Half(p_zone_for_points->points[CED_POINT_REAR_LEFT].x - p_zone_for_points->points[CED_POINT_FRONT_LEFT].x));
      p_point_to_check->y =
         p_zone_for_points->points[CED_POINT_FRONT_LEFT].y
         + (Fbk_Half(p_zone_for_points->points[CED_POINT_REAR_LEFT].y - p_zone_for_points->points[CED_POINT_FRONT_LEFT].y));
   }
   else if (index_point == (uint8_t) CED_POINT_MIDDLE_RIGHT)
   {
      /* Calculated middle point of right side of zone. */
      p_point_to_check->x =
         p_zone_for_points->points[CED_POINT_FRONT_RIGHT].x
         + (Fbk_Half(p_zone_for_points->points[CED_POINT_REAR_RIGHT].x - p_zone_for_points->points[CED_POINT_FRONT_RIGHT].x));
      p_point_to_check->y =
         p_zone_for_points->points[CED_POINT_FRONT_RIGHT].y
         + (Fbk_Half(p_zone_for_points->points[CED_POINT_REAR_RIGHT].y - p_zone_for_points->points[CED_POINT_FRONT_RIGHT].y));
   }
   else
   {
      /* Use corresponding edge point of zone. */
      *p_point_to_check = p_zone_for_points->points[index_point];
   }
}

static void Ced_Is_Object_In_Zone(boolean_T *f_zone_overlap_right,
                                  boolean_T *f_zone_overlap_left,
                                  const Fbk_Field_Of_Interest_T *p_collision_zone,
                                  const Fbk_Field_Of_Interest_T *p_object_zone)
{
   uint8_t i_point;
   boolean_T *p_f_zone_overlap_on_side;
   boolean_T f_already_interect;
   boolean_T f_is_in_polygon;
   Vector_2d_T point_to_check;

   /* Asserts */
   assert(NULL != f_zone_overlap_right);
   assert(NULL != f_zone_overlap_left);
   assert(NULL != p_collision_zone);
   assert(NULL != p_object_zone);

   /* Firstly perform the object point in collision zone check for all point defined in Ced_Points_To_Check_T */
   for (i_point = FBK_ZERO_UINT; i_point < (uint8_t) CED_NUMBER_OF_POINTS_TO_CHECK; i_point++)
   {
      /* Calculate point to check from object zone. */
      Ced_Calculate_Point_To_Check(&point_to_check, i_point, p_object_zone);

      /* Check on which side of the ego the point is located and set pointer to alert flag accordingly. */
      if (point_to_check.y >= FBK_ZERO_F)
      {
         p_f_zone_overlap_on_side = f_zone_overlap_right;
      }
      else
      {
         /* Flip point to right side if the point is located on the left side. This is necessary as the collision zone is defined
          * for the right side. */
         point_to_check.y         = -(point_to_check.y);
         p_f_zone_overlap_on_side = f_zone_overlap_left;
      }

      /* If no intersection on this side was found already, check if object point is in collision zone. */
      f_already_interect = (boolean_T) Fbk_Is_False(*p_f_zone_overlap_on_side);
      f_is_in_polygon = (boolean_T) Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_collision_zone->points, p_collision_zone->size,
                                                                                  &point_to_check);
      if (f_already_interect && f_is_in_polygon)
      {
         *p_f_zone_overlap_on_side = FBK_TRUE;
      }
   }
}

static void Ced_Is_Zone_In_Object(boolean_T *p_zone_overlap,
                                  const uint8_t zone_side,
                                  const Fbk_Field_Of_Interest_T *p_collision_zone,
                                  const Fbk_Field_Of_Interest_T *p_object_zone)
{
   uint8_t i_point;
   Vector_2d_T point_to_check;
   float32_T point_front, point_rear, sign;

   /* Asserts */
   assert(NULL != p_zone_overlap);
   assert(NULL != p_collision_zone);
   assert(NULL != p_object_zone);

   /* Depending on the side to check, we need to pick different zone points and correct for the sign. */
   if (FBK_SIDE_LEFT == zone_side)
   {
      point_front = p_object_zone->points[CED_POINT_FRONT_LEFT].y;
      point_rear  = p_object_zone->points[CED_POINT_REAR_LEFT].y;
      sign        = -1.0f;
   }
   else
   {
      point_front = p_object_zone->points[CED_POINT_FRONT_RIGHT].y;
      point_rear  = p_object_zone->points[CED_POINT_REAR_RIGHT].y;
      sign        = 1.0f;
   }

   /* If no intersection on the current side was found and if parts of the object zone are located on the current side of
    * the ego... */
   if ((Fbk_Is_False(*p_zone_overlap) && (((sign * point_front) > FBK_ZERO_F) || ((sign * point_rear) > FBK_ZERO_F))))
   {
      /* ...perform the collision zone point in object zone check for all points defined in Ced_Points_To_Check_T. */
      for (i_point = FBK_ZERO_UINT; i_point < (uint8_t) CED_NUMBER_OF_POINTS_TO_CHECK; i_point++)
      {
         /* Calculate point to check from collision zone. */
         Ced_Calculate_Point_To_Check(&point_to_check, i_point, p_collision_zone);

         /* Correct lateral coordinated based on the sign. */
         point_to_check.y *= sign;

         /* Check if reference point of the current sides collision zone is in object zone. */
         if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_object_zone->points, p_object_zone->size, &point_to_check))
         {
            *p_zone_overlap = FBK_TRUE;
            break;
         }
      }
   }
}

static boolean_T Ced_Is_Object_Collision_Critical(Ced_Object_T *p_ced_object, Fbk_Field_Of_Interest_T *p_collision_zone)
{
   boolean_T f_object_collision_critical = FBK_FALSE;
   boolean_T f_zone_overlap_left         = FBK_FALSE;
   boolean_T f_zone_overlap_right        = FBK_FALSE;
   Fbk_Field_Of_Interest_T object_zone;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_collision_zone);

   /* Calculate zone from CED object. */
   Fbk_Create_Field_Of_Interest_From_Object_Data(&object_zone, p_ced_object->attributes.position_predicted,
                                                 p_ced_object->attributes.length_predicted, p_ced_object->attributes.width_predicted,
                                                 p_ced_object->attributes.heading_predicted);

   /* Add hysteresis for collision zone - only for outer side*/
   p_collision_zone->points[1].y += p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1];
   p_collision_zone->points[2].y += p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1];

   /* Check if reference points of object are within the CED warn zone. */
   Ced_Is_Object_In_Zone(&f_zone_overlap_right, &f_zone_overlap_left, p_collision_zone, &object_zone);

   /* Check if reference points of CED warn zone are within the object. */
   Ced_Is_Zone_In_Object(&f_zone_overlap_right, FBK_SIDE_RIGHT, p_collision_zone, &object_zone);
   Ced_Is_Zone_In_Object(&f_zone_overlap_left, FBK_SIDE_LEFT, p_collision_zone, &object_zone);

   /* Set f_object_collision_critical and alert side based on results of the previous checks. */
   if (Fbk_Is_True(f_zone_overlap_left) || Fbk_Is_True(f_zone_overlap_right))
   {
      f_object_collision_critical = FBK_TRUE;
      Binary_Ced_Debug_Increase_Object_Importance_Counter(p_ced_object->tracker_data.index);

      if (Fbk_Is_True(f_zone_overlap_left) && Fbk_Is_True(f_zone_overlap_right))
      {
         p_ced_object->attributes.alert_side = INTERSEC_BOTH_SIDES;
      }
      else if (Fbk_Is_True(f_zone_overlap_left))
      {
         p_ced_object->attributes.alert_side = INTERSEC_LEFT_SIDE;
      }
      else
      {
         p_ced_object->attributes.alert_side = INTERSEC_RIGHT_SIDE;
      }

      /* Check how close the collision critical predicted object is to the host vehicle edge */
      p_ced_object->attributes.closest_lat_dist_predicted = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);
   }

   return f_object_collision_critical;
}

static boolean_T Ced_Is_Warning_Suppressed_Opposite_Side(const Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_object_on_opposite_side;
   boolean_T f_suppression_active;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check if the warned object is on the left side but a warning would be raised on the right side. */
   if ((INTERSEC_LEFT_SIDE == p_ced_object->attributes.alert_side) && (EGO_RIGHT_SIDE == p_ced_object->attributes.ego_side))
   {
      f_object_on_opposite_side = FBK_TRUE;
   }
   /* Check if the warned object is on the right side but a warning would be raised on the left side. */
   else if ((INTERSEC_RIGHT_SIDE == p_ced_object->attributes.alert_side) && (EGO_LEFT_SIDE == p_ced_object->attributes.ego_side))
   {
      f_object_on_opposite_side = FBK_TRUE;
   }
   /* Object and warning are on the same side. */
   else
   {
      f_object_on_opposite_side = FBK_FALSE;
   }

   /* Check if warnings for objects on the opposite side are per se not allowed. */
   if (((uint8_t) OPPOSITE_SIDE_ALERT_NOT_ALLOWED) == p_ced_cal->k_ced_allow_opposite_side_alerts)
   {
      f_suppression_active = FBK_TRUE;
   }
   /* If opposite side matching due to path algo is not allowed or the object is not matched to a path, suppress a warning. */
   else if ((((uint8_t) OPPOSITE_SIDE_ALERT_ONLY_ALLOWED_WITH_PATH_MATCH) == p_ced_cal->k_ced_allow_opposite_side_alerts)
            && ((NULL == p_ced_object->attributes.p_pt_match_info)
                || (PT_DEFAULT_MATCH_INDEX == p_ced_object->attributes.p_pt_match_info->track_match)))
   {
      f_suppression_active = FBK_TRUE;
   }
   /* Opposite side warning is allowed. */
   else
   {
      f_suppression_active = FBK_FALSE;
   }

   /* Only suppress warning if object is on opposite side and suppression is active. */
   return (boolean_T) (f_object_on_opposite_side && f_suppression_active);
}

static boolean_T Ced_Is_Warning_Suppressed_Cross_Border(const Ced_Object_T *p_ced_object,
                                                        const Ced_Core_Calibration_T *p_ced_cal,
                                                        const Pa_Data_T *p_pa_data)

{
   Fbk_Object_Corners_T target_corners;
   float32_T reference_point;
   float32_T lat_border;
   boolean_T f_suppression_active = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);
   assert(NULL != p_pa_data);

   if (p_ced_cal->k_ced_f_object_lat_on_one_side_of_border)
   {
      /* Calculate the corners of the object */
      Fbk_Calculate_Target_Corners(&target_corners, &p_ced_object->tracker_data.vcs_pos, &p_ced_object->tracker_data.vcs_heading,
                                   &p_ced_object->tracker_data.length, &p_ced_object->tracker_data.width);


      if (p_pa_data->object_data[p_ced_object->tracker_data.index].f_is_in_rl_sensor_fov)
      {
         lat_border      = -Fbk_Half(p_pa_data->vehicle_data.host_width) + p_ced_cal->k_ced_lat_pos_of_border;
         reference_point = target_corners.points[FBK_FRONT_RIGHT_CORNER].y;
         if (reference_point >= lat_border)
         {
            f_suppression_active = FBK_TRUE;
         }
      }
      else if (p_pa_data->object_data[p_ced_object->tracker_data.index].f_is_in_rr_sensor_fov)
      {
         lat_border      = Fbk_Half(p_pa_data->vehicle_data.host_width) - p_ced_cal->k_ced_lat_pos_of_border;
         reference_point = target_corners.points[FBK_FRONT_LEFT_CORNER].y;
         if (reference_point <= lat_border)
         {
            f_suppression_active = FBK_TRUE;
         }
      }
      else
      {
         /* Do nothing */
      }
   }

   /* Only suppress warning if object's corners are on opposite specific line side and suppression is active. */
   return f_suppression_active;
}

static boolean_T Ced_Is_Warning_Suppressed_Ego_Lane(const Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_warning_suppressed_due_to_ego_lane = FBK_FALSE;
   boolean_T f_object_on_lane;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check if ego lane alerts are not allowed. */
   if (Fbk_Is_False(p_ced_cal->k_ced_f_allow_ego_lane_alerts))
   {
      /* Check if object is within the ego lane. */
      if (EGO_LANE == p_ced_object->attributes.ego_side)
      {
         /* Suppress warning, if object is not matched to a path or parking within the ego lane. */
         f_object_on_lane = (boolean_T) Ced_Is_Object_Parking_On_Ego_Lane(p_ced_object, p_ced_cal);
         if (f_object_on_lane
             || ((NULL == p_ced_object->attributes.p_pt_match_info)
                 || (PT_DEFAULT_MATCH_INDEX == p_ced_object->attributes.p_pt_match_info->track_match)))
         {
            f_warning_suppressed_due_to_ego_lane = FBK_TRUE;
         }
      }
   }

   /* Return if warning on ego lane should be suppressed */
   return f_warning_suppressed_due_to_ego_lane;
}

static Ced_Alert_Suppression_T Ced_Get_Object_Alert_Suppression(Ced_Object_T *p_ced_object,
                                                                const Ced_Core_Calibration_T *p_ced_cal,
                                                                const Ced_Core_Input_T *p_ced_core_input)
{
   /* Return value */
   Ced_Alert_Suppression_T ced_alert_suppression = CED_SUPPRESS_NO_ALERT;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);
   assert(NULL != p_ced_core_input);

   /* Option to restrict both side alerts only to the objects current side */
   if (Fbk_Is_True(p_ced_cal->k_ced_f_handle_both_side_alerts_as_object_side)
       && (INTERSEC_BOTH_SIDES == p_ced_object->attributes.alert_side) && (EGO_LANE != p_ced_object->attributes.ego_side))
   {
      /* Map ego side to alert side. */
      switch (p_ced_object->attributes.ego_side)
      {
         case EGO_LEFT_SIDE:
            p_ced_object->attributes.alert_side = INTERSEC_LEFT_SIDE;
            break;

         case EGO_RIGHT_SIDE:
            p_ced_object->attributes.alert_side = INTERSEC_RIGHT_SIDE;
            break;

         default:
            p_ced_object->attributes.alert_side = INTERSEC_UNDEF_SIDE;
            break;
      }
   }

   /* Option to overwrite the alert level for objects setting alerts on the opposite side */
   if (Ced_Is_Warning_Suppressed_Opposite_Side(p_ced_object, p_ced_cal))
   {
      ced_alert_suppression = CED_SUPPRESS_OPPOSITE_SIDE_ALERT;
   }

   /* Option to overwrite the alert level for objects located laterally on two sides of a specific line */
   if (Ced_Is_Warning_Suppressed_Cross_Border(p_ced_object, p_ced_cal, p_ced_core_input->p_pa_data))
   {
      ced_alert_suppression = CED_SUPPRESS_CROSS_BORDER;
   }

   /* Option to overwrite the alert level for objects in the defined ego lane (k_ced_ego_lane_width) */
   if (Ced_Is_Warning_Suppressed_Ego_Lane(p_ced_object, p_ced_cal))
   {
      ced_alert_suppression = CED_SUPPRESS_EGO_LANE_ALERT;
   }

   /* Option to overwrite the alert level for real coasting objects. */
   if (Fbk_Is_False(p_ced_cal->k_ced_f_allow_coasted_object_alerts) && Fbk_Is_Obj_In_Any_Sensor_Fov(&(p_ced_object->tracker_data))
       && (PA_OBJ_STATUS_COASTED == p_ced_object->tracker_data.status))

   {
      ced_alert_suppression = CED_SUPPRESS_COASTED_OBJECT_ALERT;
   }

   /* Suppress object alert level based on path information. E.g. when the nearest path is different when compared to the
    * object*/
   if (Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(p_ced_object, p_ced_cal))
   {
      ced_alert_suppression = CED_SUPPRESS_BASED_ON_NEAREST_PATH_INFO;
   }

   /* Pass reason for potential alert suppression to debug writer */
   Binary_Ced_Debug_Pass_Object_Alert_Suppression_Reason(ced_alert_suppression, p_ced_object->tracker_data.index);

   return ced_alert_suppression;
}

static void Ced_Get_Lat_Distance_Hystereis(const Ced_Persistent_T *p_ced_persistent,
                                           const Ced_Core_Calibration_T *p_ced_cal,
                                           Ced_Object_T *p_ced_object)
{
   uint8_t side_idx;

   /* Asserts */
   assert(NULL != p_ced_persistent);
   assert(NULL != p_ced_cal);
   assert(NULL != p_ced_object);

   /* Add lateral distance hysteresis based on alert in previous cycle*/
   for (side_idx = 0; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      if (p_ced_object->tracker_data.id == p_ced_persistent->ced_side_id_prev_cycle[side_idx])
      {
         if (CED_ALERT_ACTIVE_LEVEL_3 == p_ced_persistent->ced_side_alert_prev_cycle[side_idx])
         {
            p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3] = p_ced_cal->k_ced_warning_pred_lat_dist_max_histeresis;
         }
         else if (CED_ALERT_ACTIVE_LEVEL_2 == p_ced_persistent->ced_side_alert_prev_cycle[side_idx])
         {
            p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2] = p_ced_cal->k_ced_warning_pred_lat_dist_max_histeresis;
         }
         else if (CED_ALERT_ACTIVE_LEVEL_1 == p_ced_persistent->ced_side_alert_prev_cycle[side_idx])
         {
            p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1] = p_ced_cal->k_ced_warning_pred_lat_dist_max_histeresis;
         }
         else
         {
            // do nothing
         }
      }
   }
}

static void Ced_Set_Object_Alert_Level(Ced_Object_T *p_ced_object,
                                       const Ced_Core_Calibration_T *p_ced_cal,
                                       const Ced_Core_Input_T *p_ced_core_input,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T qualifying_time;

   /* Asserts */
   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);
   assert(NULL != p_ced_core_input);
   assert(NULL != p_vehicle_data);

   qualifying_time = (float32_T) p_ced_cal->k_ced_alert_qualifying_cycles * p_ced_core_input->p_pa_data->time_diff_to_last_cycle;

   /* Set current object side */
   if (Fbk_Abs_F(Ced_Get_Lat_Position(p_ced_object, p_ced_cal)) >= Fbk_Half(p_ced_cal->k_ced_ego_lane_width))
   {
      p_ced_object->attributes.ego_side = (Ced_Get_Lat_Position(p_ced_object, p_ced_cal) <= FBK_ZERO_F) ? EGO_LEFT_SIDE
                                                                                                        : EGO_RIGHT_SIDE;
   }
   else
   {
      p_ced_object->attributes.ego_side = EGO_LANE;
   }

   /* Subtract half of the host vehicle width to get the distance to the vehicle edge */
   p_ced_object->attributes.closest_lat_dist_predicted =
      p_ced_object->attributes.closest_lat_dist_predicted - Fbk_Half(p_vehicle_data->host_width);

   /* Set alert level */
   if ((FBK_SIDE_UNDEFINED != p_ced_object->attributes.direction)
       && (p_ced_object->attributes.time_to_pass_crash_line >= p_ced_cal->k_ced_alert_ttp_min[p_ced_object->attributes.direction]))
   {
      if ((Fbk_Is_True(p_ced_cal->k_ced_f_third_warning_level_enable))
          && (p_ced_object->attributes.time_to_crash_line
              <= p_ced_cal->k_ced_third_warning_ttc_threshold[p_ced_object->attributes.direction])
          && (p_ced_object->attributes.closest_lat_dist_predicted
              <= (p_ced_cal->k_ced_third_warning_pred_lat_dist_max + p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3])))
      {
         p_ced_object->attributes.alert_level = CED_ALERT_ACTIVE_LEVEL_3;
      }
      else if ((Fbk_Is_True(p_ced_cal->k_ced_f_second_warning_level_enable))
               && (p_ced_object->attributes.time_to_crash_line
                   <= p_ced_cal->k_ced_second_warning_ttc_threshold[p_ced_object->attributes.direction])
               && (p_ced_object->attributes.closest_lat_dist_predicted
                   <= (p_ced_cal->k_ced_second_warning_pred_lat_dist_max
                       + p_ced_object->attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2])))
      {
         p_ced_object->attributes.alert_level = CED_ALERT_ACTIVE_LEVEL_2;
      }
      else if (p_ced_object->attributes.time_to_crash_line
               <= p_ced_cal->k_ced_first_warning_ttc_threshold[p_ced_object->attributes.direction])
      {
         p_ced_object->attributes.alert_level = CED_ALERT_ACTIVE_LEVEL_1;
      }
      else if (p_ced_object->attributes.time_to_crash_line
               <= (p_ced_cal->k_ced_first_warning_ttc_threshold[p_ced_object->attributes.direction] + qualifying_time))
      {
         p_ced_object->attributes.alert_level = CED_ALERT_QUALIFICATION;
      }
      else
      {
         p_ced_object->attributes.alert_level = CED_NO_ALERT;
      }

      /* Suppress object alert when one suppression mechanism was triggered */
      if (CED_SUPPRESS_NO_ALERT != Ced_Get_Object_Alert_Suppression(p_ced_object, p_ced_cal, p_ced_core_input))
      {
         p_ced_object->attributes.alert_level = CED_NO_ALERT;
      }
   }
   else
   {
      p_ced_object->attributes.f_skip_alert_holding = p_ced_cal->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp;
   }
}

static boolean_T Ced_Is_Object_Parking_On_Ego_Lane(const Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{
   boolean_T f_obj_is_parking_on_ego_lane = FBK_FALSE;

   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check whether the predicted object is in ego lane and whether a parking maneuver is occuring. */
   if ((Fbk_Abs_F(p_ced_object->attributes.position_predicted.y) <= Fbk_Half(p_ced_cal->k_ced_ego_lane_width))
       && (Fbk_Abs_F(p_ced_object->tracker_data.vcs_pos.x) <= (p_ced_cal->k_ced_ego_lane_parking_range))
       && (Fbk_Abs_F(p_ced_object->tracker_data.speed) <= (p_ced_cal->k_ced_ego_lane_parking_maneuver_speed)))
   {
      f_obj_is_parking_on_ego_lane = FBK_TRUE;
   }

   return f_obj_is_parking_on_ego_lane;
}

static float32_T Ced_Get_Closest_Predicted_Lateral_Distance(const Fbk_Field_Of_Interest_T *p_object_zone)
{
   float32_T closest_pred_lat_dist = CED_INVALID_DISTANCE;
   int8_t p0_lat_sign, p1_lat_sign, p2_lat_sign, p3_lat_sign;

   /* Assert */
   assert(NULL != p_object_zone);

   /*  0 ----- 1  */
   /*  |       |  */
   /*  |  obj  |  */
   /*  |       |  */
   /*  3 ----- 2  */

   /* Set sign of lateral object points */
   p0_lat_sign = Fbk_Sign(p_object_zone->points[0].y);
   p1_lat_sign = Fbk_Sign(p_object_zone->points[1].y);
   p2_lat_sign = Fbk_Sign(p_object_zone->points[2].y);
   p3_lat_sign = Fbk_Sign(p_object_zone->points[3].y);

   /* Check if all object points are on the same host vehicle side. */
   if ((p0_lat_sign == p1_lat_sign) && (p0_lat_sign == p2_lat_sign) && (p0_lat_sign == p3_lat_sign))
   {
      uint8_t point_idx;

      /* All lateral object points are on the same side. */
      for (point_idx = FBK_ZERO_UINT; point_idx < p_object_zone->size; point_idx++)
      {
         float32_T point_lat_pos_abs = Fbk_Abs_F(p_object_zone->points[point_idx].y);

         if (point_lat_pos_abs < closest_pred_lat_dist)
         {
            /* Point is closer than all previously checked points */
            closest_pred_lat_dist = point_lat_pos_abs;
         }
      }
   }
   else
   {
      /* Predicted object crosses the host vehicle center line -> closest lateral distance is set to zero. */
      closest_pred_lat_dist = FBK_ZERO_F;
   }

   return closest_pred_lat_dist;
}

static float32_T Ced_Flip_Heading_Value(const float32_T heading_value)
{
   float32_T flipped_heading;

   /* Flip heading depending on sign. */
   if (FBK_ONE_INT == Fbk_Sign(heading_value))
   {
      /* Heading value is positive -> Remove PI */
      flipped_heading = heading_value - PI;
   }
   else
   {
      /* Heading value is negative (or zero) -> Add PI */
      flipped_heading = heading_value + PI;
   }

   return flipped_heading;
}

static void Ced_Fill_Object_Persistent_Data(Ced_Persistent_T *p_ced_persistent, const Ced_Object_T *p_ced_object)
{
   /* Asserts */
   assert(NULL != p_ced_persistent);
   assert(NULL != p_ced_object);

   /* Fill persistent data for current object */
   if (FBK_SIDE_FRONT == p_ced_object->attributes.direction)
   {
      p_ced_persistent->ced_object_heading_predicted[p_ced_object->tracker_data.id] =
         Ced_Flip_Heading_Value(p_ced_object->attributes.heading_predicted);
   }
   else
   {
      p_ced_persistent->ced_object_heading_predicted[p_ced_object->tracker_data.id] = p_ced_object->attributes.heading_predicted;
   }
}

static void Ced_Fill_Side_Persistent_Data(Ced_Persistent_T *p_ced_persistent, const Ced_Core_Output_T *p_ced_core_output)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ced_persistent);
   assert(NULL != p_ced_core_output);

   /* Fill persistent data for both sides */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_ced_persistent->ced_side_alert_prev_cycle[side_index]            = p_ced_core_output->ced_alert[side_index];
      p_ced_persistent->ced_side_id_prev_cycle[side_index]               = p_ced_core_output->ced_id[side_index];
      p_ced_persistent->ced_side_unique_id_prev_cycle[side_index]        = p_ced_core_output->ced_unique_id[side_index];
      p_ced_persistent->ced_side_direction_prev_cycle[side_index]        = p_ced_core_output->ced_object_direction[side_index];
      p_ced_persistent->ced_side_path_match_index_prev_cycle[side_index] = p_ced_core_output->ced_object_path_match_index[side_index];
   }
}

static void Ced_Set_Most_Critical_Object(Ced_Core_Output_T *p_ced_core_output, const Ced_Object_T *p_ced_object)
{
   uint8_t side_index;
   boolean_T f_obj_is_side_relevant;

   /* Asserts */
   assert(NULL != p_ced_core_output);
   assert(NULL != p_ced_object);

   if (CED_NO_ALERT != p_ced_object->attributes.alert_level)
   {
      for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
      {
         if ((FBK_SIDE_LEFT == side_index) && (INTERSEC_LEFT_SIDE == p_ced_object->attributes.alert_side))
         {
            f_obj_is_side_relevant = FBK_TRUE;
         }
         else if ((FBK_SIDE_RIGHT == side_index) && (INTERSEC_RIGHT_SIDE == p_ced_object->attributes.alert_side))
         {
            f_obj_is_side_relevant = FBK_TRUE;
         }
         else if (INTERSEC_BOTH_SIDES == p_ced_object->attributes.alert_side)
         {
            f_obj_is_side_relevant = FBK_TRUE;
         }
         else
         {
            f_obj_is_side_relevant = FBK_FALSE;
         }

         if (Fbk_Is_True(f_obj_is_side_relevant))
         {
            /* Check if current object has lower TTC than previously seen on that side */
            if ((p_ced_object->attributes.time_to_crash_line < p_ced_core_output->ced_ttc[side_index])
                && ((p_ced_object->attributes.alert_level >= p_ced_core_output->ced_alert[side_index])
                    || (CED_ALERT_QUALIFICATION == p_ced_core_output->ced_alert[side_index])))
            {
               p_ced_core_output->ced_id[side_index]                       = p_ced_object->tracker_data.id;
               p_ced_core_output->ced_unique_id[side_index]                = p_ced_object->tracker_data.unique_id;
               p_ced_core_output->ced_index[side_index]                    = p_ced_object->tracker_data.index;
               p_ced_core_output->ced_alert[side_index]                    = p_ced_object->attributes.alert_level;
               p_ced_core_output->ced_ttc[side_index]                      = p_ced_object->attributes.time_to_crash_line;
               p_ced_core_output->ced_ttp[side_index]                      = p_ced_object->attributes.time_to_pass_crash_line;
               p_ced_core_output->ced_object_direction[side_index]         = p_ced_object->attributes.direction;
               p_ced_core_output->ced_object_predicted_lat_pos[side_index] = p_ced_object->attributes.position_predicted.y;
               p_ced_core_output->ced_object_closest_lat_dist_predicted[side_index] =
                  p_ced_object->attributes.closest_lat_dist_predicted;
               p_ced_core_output->ced_front_bumper_pos_long[side_index] = p_ced_object->attributes.front_bumper_pos_long;
               p_ced_core_output->ced_vcs_vel_rel_x[side_index]         = p_ced_object->tracker_data.vcs_vel_rel.x;

               /* Check if PT info is available for object */
               if (NULL != p_ced_object->attributes.p_pt_match_info)
               {
                  p_ced_core_output->ced_object_path_match_index[side_index] = p_ced_object->attributes.p_pt_match_info->track_match;
               }
               else
               {
                  p_ced_core_output->ced_object_path_match_index[side_index] = PT_DEFAULT_MATCH_INDEX;
               }

               Binary_Ced_Debug_Pass_Side_Data(p_ced_object, side_index);
            }
         }
      }
   }
}

static boolean_T Ced_Should_Object_Alert_Be_Held(const Ced_Core_Calibration_T *p_ced_cal,
                                                 const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                 const Pa_Data_T *p_pa_data,
                                                 const Fbk_Output_T *p_fbk_output,
                                                 const uint8_t object_id)
{

   /* Return value */
   boolean_T f_ced_should_object_alert_be_held = FBK_TRUE;

   uint8_t object_index;

   /* Asserts */
   assert(NULL != p_ced_cal);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_pa_data);
   assert(NULL != p_fbk_output);

   object_index = Fbk_Get_Object_Index_From_Id(p_fbk_output->p_index_id_lookup_table, object_id);

   if ((object_index < PA_OBJ_NUMBER_OF_OBJECTS) && (Fbk_Is_True(p_ced_cal->k_ced_f_suppress_alert_holding_for_uncritical_objects))
       && (PA_OBJ_STATUS_INVALID != p_pa_data->object_data[object_index].status)
       && (p_pa_data->object_data[object_index].vcs_pos.x < -p_vehicle_data->host_length))
   {
      /* Alert should not be held if heading or long velocity is out of allowed range */
      if ((Fbk_Abs_F(p_pa_data->object_data[object_index].vcs_heading) > p_ced_cal->k_ced_alert_holding_obj_abs_heading_max)
          || (p_pa_data->object_data[object_index].vcs_vel.x < p_ced_cal->k_ced_alert_holding_obj_long_vel_min))
      {
         f_ced_should_object_alert_be_held = FBK_FALSE;
      }
   }

   return f_ced_should_object_alert_be_held;
}

static void Ced_Debounce_Alert_Level(Ced_Core_Output_T *p_ced_core_output,
                                     Ced_Persistent_T *p_ced_persistent,
                                     const Fbk_Output_T *p_fbk_output,
                                     const boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE],
                                     const Ced_Core_Calibration_T *p_ced_cal,
                                     const Pa_Data_T *p_pa_data,
                                     const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_ced_core_output);
   assert(NULL != p_ced_persistent);
   assert(NULL != p_fbk_output);
   assert(NULL != p_ced_cal);
   assert(NULL != p_pa_data);
   assert(NULL != p_vehicle_data);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      /* Qualifying alert */
      if ((CED_NO_ALERT != p_ced_core_output->ced_alert[side_index])
          && (PA_INVALID_OBJ_INDEX != p_ced_core_output->ced_index[side_index]))
      {
         uint8_t required_qualifying_cycles;

         /* Increase qualifying counter */
         Sat_Inc_Uint8(&(p_ced_persistent->ced_side_alert_qualifying_counter[side_index]));

         /* Calculate required qualifying cycles based on longitudinal object velocity */
         if (Fbk_Abs_F(p_pa_data->object_data[p_ced_core_output->ced_index[side_index]].vcs_vel.x)
             <= p_ced_cal->k_ced_slow_objects_long_vel_max)
         {
            required_qualifying_cycles = p_ced_cal->k_ced_alert_qualifying_cycles_slow_objects;
         }
         else
         {
            required_qualifying_cycles = p_ced_cal->k_ced_alert_qualifying_cycles;
         }

         if ((CED_ALERT_QUALIFICATION == p_ced_core_output->ced_alert[side_index])
             || ((p_ced_core_output->ced_id[side_index] != p_ced_persistent->ced_side_id_prev_cycle[side_index])
                 && (p_ced_persistent->ced_side_alert_qualifying_counter[side_index] <= required_qualifying_cycles)))
         {
            /* Most critical object without alert or qualifying counter below threshold, thus reset core output */
            Ced_Reset_Core_Output_Side_Data(p_ced_core_output, side_index);
         }
      }
      else
      {
         /* No active alert in this cycle, thus reset qualification counter */
         p_ced_persistent->ced_side_alert_qualifying_counter[side_index] = FBK_ZERO_UINT;
      }


      /* Holding alert */
      if ((p_ced_core_output->ced_alert[side_index] < p_ced_persistent->ced_side_alert_prev_cycle[side_index])
          && Ced_Should_Object_Alert_Be_Held(p_ced_cal, p_vehicle_data, p_pa_data, p_fbk_output,
                                             p_ced_persistent->ced_side_id_prev_cycle[side_index])

          && Fbk_Is_False(ced_object_f_skip_alert_holding[p_ced_persistent->ced_side_id_prev_cycle[side_index]]))
      {
         /* Increase holding counter */
         Sat_Inc_Uint8(&(p_ced_persistent->ced_side_alert_holding_counter[side_index]));

         if (p_ced_persistent->ced_side_alert_holding_counter[side_index] <= p_ced_cal->k_ced_alert_holding_cycles)
         {
            /* Holding counter below threshold, thus hold CED alert */

            /* Reset CED core output if object ID changed and overwrite with previous object ID */
            if (p_ced_core_output->ced_id[side_index] != p_ced_persistent->ced_side_id_prev_cycle[side_index])
            {
               Ced_Reset_Core_Output_Side_Data(p_ced_core_output, side_index);
               p_ced_core_output->ced_id[side_index]        = p_ced_persistent->ced_side_id_prev_cycle[side_index];
               p_ced_core_output->ced_unique_id[side_index] = p_ced_persistent->ced_side_unique_id_prev_cycle[side_index];
               p_ced_core_output->ced_index[side_index]     = Fbk_Get_Object_Index_From_Id(
                      p_fbk_output->p_index_id_lookup_table, p_ced_persistent->ced_side_id_prev_cycle[side_index]);
            }

            /* Overwrite CED core output alert level with previous alert level */
            p_ced_core_output->ced_alert[side_index]            = p_ced_persistent->ced_side_alert_prev_cycle[side_index];
            p_ced_core_output->ced_object_direction[side_index] = p_ced_persistent->ced_side_direction_prev_cycle[side_index];
            p_ced_core_output->ced_object_path_match_index[side_index] =
               p_ced_persistent->ced_side_path_match_index_prev_cycle[side_index];
         }
      }
      else
      {
         /* No alert is held in this cycle, thus reset holding counter */
         p_ced_persistent->ced_side_alert_holding_counter[side_index] = FBK_ZERO_UINT;
      }
   }
}

static void Ced_Algorithm(Ced_Core_Output_T *p_ced_core_output,
                          Ced_Persistent_T *p_ced_persistent,
                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                          const Ced_Core_Input_T *p_ced_core_input,
                          const Ced_Core_Calibration_T *p_ced_cal,
                          const Fbk_Output_T *p_fbk_output)
{

   /* CED object struct and pointer */
   Ced_Object_T ced_object;

   /* Index variables */
   uint8_t object_index;
   uint8_t side_index;

   /* CED zones and related variables */
   Fbk_Field_Of_Interest_T ced_funnel_zone;
   Fbk_Field_Of_Interest_T ced_collision_zone;

   /* Information on each object if alert holding can be skipped due to low TTP  */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE] = {FBK_FALSE};

   /* Asserts */
   assert(NULL != p_ced_core_output);
   assert(NULL != p_ced_persistent);
   assert(NULL != p_ced_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);
   assert(NULL != p_fbk_output);

   /* Reset core output per side */
   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      Ced_Reset_Core_Output_Side_Data(p_ced_core_output, side_index);
   }

   /* Create CED zones once per cycle */
   Ced_Create_Funnel_Zone(&ced_funnel_zone, p_vehicle_data, p_ced_cal);
   Ced_Create_Collision_Zone(&ced_collision_zone, p_vehicle_data, p_ced_cal);
   Binary_Ced_Debug_Pass_Zones(&ced_funnel_zone, &ced_collision_zone, p_vehicle_data, p_ced_cal);

   /* Reset objects persistent data based on lookup index table*/
   Ced_Reset_Objects_Persistent(p_ced_persistent, p_fbk_output->p_index_id_lookup_table);

   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      const Fbk_Object_Data_T *p_object = &p_ced_core_input->p_pa_data->object_data[object_index];
      if (p_object->f_moveable)
      {
         /* Reset data structs */
         Ced_Reset_Ced_Object(&ced_object);

         /* Check basic validity signals */
         if (Ced_Is_Object_Valid(p_ced_core_input->p_pa_data, object_index, p_ced_persistent))
         {
            /* Object is valid -> Fill tracker information for CED_Object */
            ced_object.tracker_data       = p_ced_core_input->p_pa_data->object_data[object_index];
            ced_object.tracker_data.index = object_index;

            /* Check objects general properties, trajectory and zone overlap */
            if (Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, p_ced_core_input, p_vehicle_data, p_ced_cal))
            {
               /* Update Path Tracking info for this object */
               Ced_Update_Object_Data(&ced_object, p_ced_core_input, p_ced_cal);

               /* Predict object position at crash line and calculate time to reach the crash line */
               Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, p_ced_persistent, p_vehicle_data, p_ced_cal);

               /* Get the lateral distance hysteresis value for predicted position */
               Ced_Get_Lat_Distance_Hystereis(p_ced_persistent, p_ced_cal, &ced_object);

               /* Check the predicted object position against criticality zones */
               if (Ced_Is_Object_Collision_Critical(&ced_object, &ced_collision_zone))
               {
                  /* Object is collision critical and alert is set (based on ttc) */
                  Ced_Set_Object_Alert_Level(&ced_object, p_ced_cal, p_ced_core_input, p_vehicle_data);

                  /* Check if current object is the most critical object for any side */
                  Ced_Set_Most_Critical_Object(p_ced_core_output, &ced_object);
               }
            }

            /* Fill object persistent data for CED object */
            Ced_Fill_Object_Persistent_Data(p_ced_persistent, &ced_object);

            /* Fill information if alert holding can be skipped on object */
            ced_object_f_skip_alert_holding[p_ced_core_input->p_pa_data->object_data[object_index].id] =
               ced_object.attributes.f_skip_alert_holding;

            /* Pass object data to debug data structure */
            Binary_Ced_Debug_Pass_Object_Attributes(&ced_object.attributes, object_index);
            Binary_Ced_Debug_Pass_Object_To_Path_Match_Index(&ced_object, object_index);
         }
      }
   }

   /* Debounce output signal */
   Ced_Debounce_Alert_Level(p_ced_core_output, p_ced_persistent, p_fbk_output, ced_object_f_skip_alert_holding, p_ced_cal,
                            p_ced_core_input->p_pa_data, p_vehicle_data);


   /* Fill side persistent data */
   Ced_Fill_Side_Persistent_Data(p_ced_persistent, p_ced_core_output);
}


static void Ced_Reset_Objects_Persistent(Ced_Persistent_T *p_ced_persistent, const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table)
{

   /* Iterator value */
   uint8_t object_id;

   /* Assert */
   assert(NULL != p_ced_persistent);
   assert(NULL != p_index_id_lookup_table);

   for (object_id = FBK_ZERO_UINT; object_id <= PA_OBJ_NUMBER_OF_OBJECTS; ++object_id)
   {
      if (PA_INVALID_OBJ_INDEX == Fbk_Get_Object_Index_From_Id(p_index_id_lookup_table, object_id))
      {
         /* Reset persistent data for given object */
         p_ced_persistent->ced_object_heading_predicted[object_id] = FBK_ZERO_F;
      }
   }
}


static void Ced_Reset_Persistent_Data(Ced_Persistent_T *p_ced_persistent, const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table)
{
   uint8_t side_index;

   /* Assert */
   assert(NULL != p_ced_persistent);
   assert(NULL != p_index_id_lookup_table);

   Ced_Reset_Objects_Persistent(p_ced_persistent, p_index_id_lookup_table);


   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_ced_persistent->ced_side_alert_prev_cycle[side_index]            = CED_NO_ALERT;
      p_ced_persistent->ced_side_id_prev_cycle[side_index]               = PA_INVALID_OBJ_ID;
      p_ced_persistent->ced_side_unique_id_prev_cycle[side_index]        = PA_INVALID_OBJ_ID;
      p_ced_persistent->ced_side_direction_prev_cycle[side_index]        = FBK_SIDE_UNDEFINED;
      p_ced_persistent->ced_side_path_match_index_prev_cycle[side_index] = PT_DEFAULT_MATCH_INDEX;
      p_ced_persistent->ced_side_alert_qualifying_counter[side_index]    = FBK_ZERO_UINT;
      p_ced_persistent->ced_side_alert_holding_counter[side_index]       = FBK_ZERO_UINT;
   }
}

static void Ced_Reset_Core_Output_Side_Data(Ced_Core_Output_T *p_ced_core_output, uint8_t side_index)
{
   /* Assert */
   assert(NULL != p_ced_core_output);
   assert(side_index < FBK_NUMBER_OF_SIDES);

   /* Reset the core output for given side */
   p_ced_core_output->ced_id[side_index]                                = PA_INVALID_OBJ_ID;
   p_ced_core_output->ced_unique_id[side_index]                         = PA_INVALID_OBJ_ID;
   p_ced_core_output->ced_index[side_index]                             = PA_INVALID_OBJ_INDEX;
   p_ced_core_output->ced_alert[side_index]                             = CED_NO_ALERT;
   p_ced_core_output->ced_ttc[side_index]                               = CED_INVALID_TIME;
   p_ced_core_output->ced_ttp[side_index]                               = CED_INVALID_TIME;
   p_ced_core_output->ced_object_predicted_lat_pos[side_index]          = FBK_ZERO_F;
   p_ced_core_output->ced_object_closest_lat_dist_predicted[side_index] = FBK_ZERO_F;
   p_ced_core_output->ced_object_direction[side_index]                  = FBK_SIDE_UNDEFINED;
   p_ced_core_output->ced_object_path_match_index[side_index]           = PT_DEFAULT_MATCH_INDEX;
}

static void Ced_Adapt_Heading_For_Ego_Lane(Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{

   float32_T heading_factor;
   Fbk_Object_Corners_T corners;
   Vector_2d_T reference_point;
   Vector_2d_T further_point;
   if (Fbk_Is_True(p_ced_cal->k_ced_f_adapt_heading_ego_lane))
   {
      heading_factor = FBK_ONE_F;
      Fbk_Calculate_Target_Corners(&corners, &(p_ced_object->tracker_data.vcs_pos), &(p_ced_object->attributes.heading_predicted),
                                   &(p_ced_object->attributes.length_predicted), &(p_ced_object->attributes.width_predicted));
      if (Ced_Get_Lat_Position(p_ced_object, p_ced_cal) > FBK_ZERO_F)
      {
         reference_point = corners.points[FBK_FRONT_LEFT_CORNER];
         further_point   = corners.points[FBK_FRONT_RIGHT_CORNER];
      }
      else
      {
         reference_point = corners.points[FBK_FRONT_RIGHT_CORNER];
         further_point   = corners.points[FBK_FRONT_LEFT_CORNER];
      }
      if (((Fbk_Sign(corners.points[FBK_FRONT_LEFT_CORNER].y) != Fbk_Sign(corners.points[FBK_FRONT_RIGHT_CORNER].y))
           || (Fbk_Abs_F(further_point.y) <= Fbk_Half(p_ced_cal->k_ced_ego_lane_width)))
          && (Fbk_Abs_F(reference_point.x) <= (p_ced_cal->k_ced_ego_lane_parking_range)))
      {
         heading_factor = FBK_ZERO_F;
      }

      p_ced_object->attributes.heading_predicted = p_ced_object->attributes.heading_predicted * heading_factor;
   }
}

static void Ced_Adapt_Heading_During_Slight_Turns(Ced_Object_T *p_ced_object, const Ced_Core_Calibration_T *p_ced_cal)
{
   float32_T abs_lat_vel;
   float32_T heading_factor = FBK_ONE_F;

   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   /* Check if the position is greater than limits. Longitudinal coordinate is negative for the objects in funnel zone, thus the
    * absoute value can be skipped */
   if ((Fbk_Abs_F(p_ced_object->tracker_data.vcs_pos.y) > p_ced_cal->k_ced_slight_turn_position_limits[FBK_ONE_UINT])
       && (p_ced_object->tracker_data.vcs_pos.x < -p_ced_cal->k_ced_slight_turn_position_limits[FBK_ZERO_UINT]))
   {
      /*Get the absosule value of the lateral speed*/
      abs_lat_vel = Fbk_Abs_F(p_ced_object->tracker_data.vcs_vel.y);
      /* If the lateral speed is below the first threshold, set heading to zero*/
      if (abs_lat_vel < p_ced_cal->k_ced_slight_turn_lat_vel_table[FBK_ZERO_UINT])
      {
         heading_factor = FBK_ZERO_F;
      }
      /* If lateral speed is between two thresholds, heading will grow linearly with speed from zero to its initial value*/
      else if (abs_lat_vel < p_ced_cal->k_ced_slight_turn_lat_vel_table[FBK_ONE_UINT])
      {
         heading_factor =
            Get_Y_Value_From_Line_By_Coordinates(p_ced_cal->k_ced_slight_turn_lat_vel_table[FBK_ZERO_UINT], FBK_ZERO_F,
                                                 p_ced_cal->k_ced_slight_turn_lat_vel_table[FBK_ONE_UINT], FBK_ONE_F, abs_lat_vel);
      }
      /* If the lateral speed is over the second threshold, do not change the heading*/
      else
      {
         heading_factor = FBK_ONE_F;
      }
   }
   p_ced_object->attributes.heading_predicted = p_ced_object->attributes.heading_predicted * heading_factor;
}

static float32_T Ced_Get_Lat_Position(const Ced_Object_T *p_ced_object /**< CED object */,
                                      const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */)
{
   float32_T shifted_lat_position;

   assert(NULL != p_ced_object);
   assert(NULL != p_ced_cal);

   shifted_lat_position = p_ced_object->tracker_data.vcs_pos.y;

   /* Apply lateral shift depending on the longitudinal and lateral position. Set max shift for large distances, linear slope for
    * intermediate, and zero for low distances.
    *
    *             _____ max_shift
    *            /.
    *           / .
    *          /  .
    *   0 ____/   .
    *      ths1   ths2
    */


   if (Fbk_Is_True(p_ced_cal->k_ced_lat_pos_shift_enable)
       && (p_ced_object->tracker_data.width >= p_ced_cal->k_ced_lat_pos_shift_width_thresh))
   {
      float32_T ths_1;
      float32_T ths_2;
      float32_T distance;
      float32_T long_dist_factor;
      float32_T lat_dist_factor;
      float32_T shift;
      /* Set close and far lateral distance threshold*/
      ths_1 = p_ced_cal->k_ced_lat_pos_shift_lat_dist_thresholds[FBK_ZERO_UINT];
      ths_2 = p_ced_cal->k_ced_lat_pos_shift_lat_dist_thresholds[FBK_ONE_UINT];
      /* Crop lateral distance to proper range */
      distance = Enforce_Range(Fbk_Abs_F(p_ced_object->tracker_data.vcs_pos.y), ths_1, ths_2);
      /* Calculate lateral distance factor*/
      lat_dist_factor = Get_Y_Value_From_Line_By_Coordinates(ths_1, FBK_ZERO_F, ths_2, FBK_ONE_F, distance);

      /* Set close and far longitudinal distance threshold*/
      ths_1 = p_ced_cal->k_ced_lat_pos_shift_long_dist_thresholds[FBK_ZERO_UINT];
      ths_2 = p_ced_cal->k_ced_lat_pos_shift_long_dist_thresholds[FBK_ONE_UINT];
      /* Crop longitudinal distance to proper range */
      distance = Enforce_Range(Fbk_Abs_F(p_ced_object->tracker_data.vcs_pos.x), ths_1, ths_2);
      /* Calculate longitudinal distance factor*/
      long_dist_factor = Get_Y_Value_From_Line_By_Coordinates(ths_1, FBK_ZERO_F, ths_2, FBK_ONE_F, distance);

      /* Calculate final shift value by convolving lateral and longitudinal factors. See ticket description for more details.
       * DDG-4153 */
      shift = long_dist_factor * lat_dist_factor * p_ced_cal->k_ced_lat_pos_max_shift;
      /* Add or subtract shift depending on the lat position sign*/
      shifted_lat_position += (FBK_ZERO_F < shifted_lat_position) ? (shift) : (-shift);
   }
   return shifted_lat_position;
}
