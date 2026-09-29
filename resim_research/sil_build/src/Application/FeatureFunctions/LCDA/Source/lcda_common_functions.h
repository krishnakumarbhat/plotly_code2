#ifndef LCDA_COMMON_FUNCTIONS_H
#define LCDA_COMMON_FUNCTIONS_H

/**
 * @file lcda_common_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports shared functions to other Lcda submodules
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */


/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief Based on hosts turn signal and the active alert on a given side, the alert level shall be returned.
 *
 * @return Lcda_Alert_State_T
 *
 * @SRS{CSCSA-68352,CSCSA-68353}
 * @SAE{SF-2779}
 * @SDD{SF-6559}
 * @verification{Check that level 2 is reached when turn signal to the alerted side is given. When the turn signal is not given,
 * level 1 shall be reached. When no active alert is given, an alert level shall not be reached.}
 */
Lcda_Alert_State_T Lcda_Get_Alert_State(const uint8_t side /**< side index */,
                                        const boolean_T f_alert_active /**< flag indicating whether alert is active */,
                                        const Lcda_Turn_Signal_T turn_signal /**< hosts turn signal state */);

/**
 * @brief Returns true when any alert level is given.
 *
 * @return True when alert level is given.
 *
 * @SRS{SF-1020,SF-1064,SF-1067,SF-1057}
 * @SAE{SF-2779}
 * @SDD{SF-6564}
 * @verification{Check whether true is returned when level 1 or 2 is reached.}
 */
boolean_T Lcda_Is_Alert_On(const Lcda_Alert_State_T alert_state /**< alert state of Lcda */);

/**
 * @brief Mirrors the zone across longitudinal axis in vcs by multiplying -1 to the lateral components.
 *
 * @return void
 *
 * @SRSSF-1072,SF-1064,SF-1067,SF-1057}
 * @SAE{SF-2779}
 * @SDD{SF-6567}
 * @verification{Check whether the lateral components of the zone have a negative sign when each zone point is defined in the
 * positive half-plane.}
 */
void Lcda_Mirror_Zone_Across_Long_Axis(Fbk_Field_Of_Interest_T *p_zone /**< Lcda zone */);


/**
 * @brief Multiplies the zone length behind the ego by the given factor and calculates the
 *        new zone points with the new length.
 *        ASSUMPTION: Zone is standard Lcda zone represented by 6 points
 *        and points 2 and 3 lie behind or at the ego vehicle rear
 *
 * @return void
 *
 * @SRS{SF-997}
 * @SAE{SF-2779}
 * @SDD{SF-6568}
 * @verification{Check whether the enlargement of the zone is applied correctly to point 2 and 3.}
 */
void Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(const float32_T factor /**< Zone extension factor*/,
                                                    Fbk_Field_Of_Interest_T *p_zone /**< Lcda zone */,
                                                    const float32_T ego_length /**< Host length */);

/**
 * @brief Checks whether the object is placed in hosts lane.
 *
 * @return True when object is placed in hosts lane
 *
 * @SRS{SF-1015,SF-1062}
 * @SAE{SF-2779}
 * @SDD{SF-6565}
 * @verification{Check whether true is returned when the objects curvi position is lower than half of the effective lane width.}
 */
boolean_T Lcda_Is_Object_In_Ego_Lane(const float32_T lane_width /**< lane width */,
                                     const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                     const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                     const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */);

/**
 * @brief calculates the longitudinal ttc based on curvi coordinates.
 *        DEFAULT_LARGE_TTC is returned when either the relative velocity is less than or equal to zero
 *        (obj is slower than ego or same lateral speed) or when the longidutinal curvi position is greater than zero.
 *
 * @return longitudinal ttc of type float
 *
 * @SRS{SF-1080,SF-1082}
 * @SAE{SF-2779}
 * @SDD{SF-6562}
 * @verification{Check whether longitudinal ttc is returned correctly based on curvi coordinates.}
 */
float32_T Lcda_Get_Longitudinal_Ttc(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                    const float32_T ego_length /**< Host length */);


/**
 * @brief calculates the lateral ttc based on curvi coordinates.
 *        DEFAULT_LARGE_TTC is returned when the object is moving away from the ego (no collision possible)
 *        or if the object relative velocity is zero (moving at the same speed as the ego).
 *
 * @return lateral ttc of type float
 *
 * @SRS{SF-1080,SF-1084}
 * @SAE{SF-2779}
 * @SDD{SF-6561}
 * @verification{Check whether lateral ttc is returned correctly within the collision critical cases.}
 */
float32_T Lcda_Get_Lateral_Ttc(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                               const float32_T ego_width /**< Host width */);


/**
 * @brief calculates the longitudinal ttp based on vcs or curvi coordinates. This is the distance from the farthest rear object
 * corner to the front of the front host bumper. DEFAULT value is returned when either the relative velocity is less than
 * zero (obj is slower than ego or same lateral speed).
 *
 * @return longitudinal ttp of type float
 *
 * @SRS{SF-1115,SF-1116,SF-1111,CSCSA-137487,CSCSA-137488}
 * @SAE{SF-2779}
 * @SDD{CSCSA-136320}
 * @verification{Check whether longitudinal ttp is returned correctly.}
 */
float32_T Lcda_Get_Longitudinal_Ttp(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                    const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */);


/**
 * @brief calculates the ttle based on vcs or curvi coordinates, as a quotient of the distance from the object corner,
 * which is nearest to the Host, to the outer lateral border of the zone and the object's relative lateral velocity.
 * DEFAULT value is returned when either the relative velocity is less than or equal to zero.
 *
 * @return longitudinal ttle of type float
 *
 * @SRS{SF-1115,SF-1116,CSCSA-186246,CSCSA-186248}
 * @SAE{SF-2779}
 * @SDD{CSCSA-184205}
 * @verification{Create the test to check whether ttle is calculated correctly.}
 */
float32_T Lcda_Get_Ttle(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                        const Fbk_Field_Of_Interest_T *p_zone /**< Zone for which the ttle is being calculated */,
                        const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */);


/**
 * @brief Returns the lateral distance of the nearest guardrail with existence probability
 * greater or equal min_exist_prob_radar_guardrail respectively min_exist_prob_camera_guardrail.
 * If no guardrail fulfills the conditions LCDA_HUGE_LATERAL_DISTANCE is returned.
 *
 * @return Lateral distance of nearest guardrail with sufficient existence probability
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6560}
 * @verification{Check that the nearest lateral component of existing guardrails is returned.}
 */
float32_T
Lcda_Get_Lateral_Distance_Guardrail(const uint8_t side /**< side index */,
                                    const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES] /**< guardrail data */);

/**
 * @brief increments persistent in zone counter.
 *
 * @return void
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6563}
 * @verification{Check that the counter is increased correctly without overflow}
 */
void Lcda_Increment_Mature_Count_In_Zone(uint8_t *p_mature_counter /**< In zone maturity counter to increment*/,
                                         const Pa_Obj_Status_T status /**< object status */);

/**
 * @brief Returns the existence probability threshold dependent on whether the object was critical before and dependent on the warn
 * setting mode in case a hysteresis shall be applied.
 *
 * @return existence probability threshold of type float32_T
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6807}
 * @verification{Check that the correct existence probability is returned in case of a situation where a hysteresis shall be
 * applied.}
 */
float32_T Lcda_Get_Existence_Probability_Threshold(
   const Lcda_Core_Input_T *p_core_input /**< Lcda core input*/,
   const uint8_t prev_obj_id[FBK_NUMBER_OF_SIDES] /**< persistent input array of an arbitrary submodule of lcda*/,
   const uint8_t obj_id /**<object id*/,
   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations*/);

/**
 * @brief Returns the opposite host vehicle side to the given host vehicle side
 *
 * @return side of the object of type uint8_t
 *
 * @SRS{SF-1015,SF-1057}
 * @SAE{SF-2779}
 * @SDD{SF-6929}
 * @verification{Check whether the opposite host vehicle side is set correctly.}
 */
uint8_t Lcda_Get_Opposite_Side(const uint8_t side /**< host vehicle side */);

/**
 * @brief Limits the given zone to the maximum lane width set in calibration value k_lcda_max_lane_width.
 *
 * @return void
 *
 * @SRS{SF-1053,SF-1060,SF-1072,SF-1085}
 * @SAE{SF-2779}
 * @SDD{SF-6994}
 * @verification{Check that zone width is limited by lane width}
 */
void Lcda_Limit_Outer_Zone_Points(Fbk_Field_Of_Interest_T *p_zone /**< Field of Interest */,
                                  const float32_T lane_width /**< Lane width */,
                                  const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations*/);


/**
 * @brief Returns the curvi longitudinal position of the front bumper of the target.
 *
 * @return front position of the object of type float32_T
 *
 * @SRS{SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6678}
 * @verification{Verify that the correct longitudinal position is returned for a given object.}
 */
float32_T Lcda_Get_Obj_Front_Position(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Returns the curvi lateral position of the front bumper of the target.
 *
 * @return front position of the object of type float32_T
 *
 * @SRS{SF-1116}
 * @SAE{SF-2779}
 * @SDD{CSCSA-70150}
 * @verification{Verify that the correct lateral position is returned for a given object.}
 */
float32_T Lcda_Get_Obj_Side_Distance_Lateral(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Helper function for Lcda_Get_Critical_Point
 *
 * @return void
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6953}
 *
 * @verification{Verify minima and maxima of the zones dimensions are returned.}
 */
void Lcda_Get_Zone_Maxima(float32_T *p_long_zone_min,
                          float32_T *p_long_zone_max,
                          float32_T *p_lat_zone_max,
                          const Fbk_Field_Of_Interest_T *p_zone);

/**
 * @brief Set numeric of Lcda_Object_Location_Data_T structure.
 *
 * @return void
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{CSCSA-27603}
 * @verification{Check that numeric values of Lcda_Object_Location_Data_T structure is updated correctly.}
 */
void Lcda_Get_Object_Location_Data(Lcda_Object_Location_Data_T *p_obj_loc_data,
                                   const Fbk_Object_Data_T *p_obj,
                                   const Fbk_Field_Of_Interest_T *p_zone_foi,
                                   const Lcda_Core_Calibration_T *p_cals,
                                   const Lcda_Coordinate_System_T coordinate_system);

#endif /* LCDA_COMMON_FUNCTIONS_H */
