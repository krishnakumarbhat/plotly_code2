#ifndef LCDA_TYPES_H
#define LCDA_TYPES_H

/**
 * @file lcda_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Definition structs and defines used across all LCDA modules
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* Fbk includes*/
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_float_range_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * defines
\*===========================================================================*/

#define LCDA_CVW_DEFAULT_NO_ALERT_TTC (25.0f)
#define LCDA_DEFAULT_OBJ_DIST \
   ((float32_T) -1000.0f) /* Large value indicating that the object is located at infinity behind the host */
#define LCDA_DEFAULT_LARGE_TTC (100.0f)
/* coverity[misra_c_2012_rule_2_5_violation][Macro is used by aanother customer] */
#define LCDA_DEFAULT_LARGE_TTP (100.0f)
#define LCDA_DEFAULT_LARGE_TTLE (100.0f)
#define LCDA_HUGE_LATERAL_DISTANCE (1.0e6f)
#define LCDA_SLC_PROBABILITY_NONE ((float32_T) FBK_ZERO_F)
#define LCDA_NUMBER_OF_ZONE_POINTS (6u)

/* Number of maximum array size for all available objects */
#define LCDA_OBJ_MAX_ARRAY_SIZE ((uint8_t) PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT)


/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Turn_Signal_T summarizes turn signal states of the host.
 *
 * @SDD{SF-6787}
 */
typedef enum
{
   TURN_SIGNAL_NONE  = (0), /**< No turn signal is active*/
   TURN_SIGNAL_LEFT  = (1), /**< Turn signal to the left is active*/
   TURN_SIGNAL_RIGHT = (2)  /**< Turn signal to the right is active*/
} Lcda_Turn_Signal_T;

/**
 * @brief Lcda_Obj_Ref_Point_T summarizes reference point positions.
 *
 * @SDD{SF-6785}
 */
typedef enum
{
   LCDA_OBJ_USE_FRONT      = (0), /**< Use front of object for reference point calculation */
   LCDA_OBJ_USE_REAR       = (1), /**< Use rear of object for reference point calculation */
   LCDA_OBJ_USE_CENTER     = (2), /**< Use center of object for reference point calculation */
   LCDA_OBJ_USE_ZONECENTER = (3)  /**< Use zone center for reference point calculation */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Lcda_Obj_Ref_Point_T;

/**
 * @brief Lcda_Zone_Points_T summarizes zone point indices.
 *
 * @SDD{SF-6788}
 */
typedef enum
{
   FRONT_OUTER_SIDE  = (0), /**< Zone point index for side farer away from host and most frontal point pair*/
   MIDDLE_OUTER_SIDE = (1), /**< Zone point index for side farer away from host and middle point pair*/
   REAR_OUTER_SIDE   = (2), /**< Zone point index for side farer away from host and rear point pair*/
   REAR_EGO_SIDE     = (3), /**< Zone point index for side near to host and rear point pair*/
   MIDDLE_EGO_SIDE   = (4), /**< Zone point index for side near to host and middle point pair*/
   FRONT_EGO_SIDE    = (5)  /**< Zone point index for side near to host and most frontal point pair*/
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Lcda_Zone_Points_T;

/**
 * @brief Lcda_Bsw_Zone_Calculation_Mode_T summarizes Bsw zone calculation modes.
 *
 * @SDD{SF-6781}
 */
typedef enum
{
   BSW_ZONE_CALC_DEFAULT        = (0), /**< Use default calculation method for BSW zone*/
   BSW_ZONE_CALC_VL_LW          = (1), /**< Use calculation method based on vehicle length and lane width for BSW zone*/
   BSW_ZONE_CALC_FIXED_INPUT    = (2), /**< Use calculation method based on fixed input values of Bsw*/
   BSW_ZONE_CALC_FIXED_ZONE_VCS = (3)  /**< Use calculation method based on fixed zone calibration values */
} Lcda_Bsw_Zone_Calculation_Mode_T;

/**
 * @brief Lcda_Cvw_Criticallity_Mode_T summarizes CVW criticallity modes.
 *
 * @SDD{CSCSA-88615}
 */
typedef enum
{
   CVW_CRIT_LONG_DIST = (0),
   CVW_CRIT_LAT_DIST  = (1)
} Lcda_Cvw_Criticality_Mode_T;

/**
 * @brief Lcda_Cvw_Zone_Calculation_Mode_T summarizes Cvw zone calculation modes.
 *
 * @SDD{SF-6782}
 */
typedef enum
{
   CVW_ZONE_CALC_DEFAULT = (0), /**< Cvw zone calculation method is set to the default method */
   CVW_ZONE_CALC_VL_LW   = (1), /**< Cvw zone calculation method is set to a method which is dependent on vehicle length and lane
                                   width */
   CVW_ZONE_CALC_FIXED_INPUT = (2), /**< Cvw zone calculation method is set to a method which is */
   CVW_ZONE_CALC_FIXED_CALS  = (3)  /**< Use calculation method based on fixed calibration values of Bsw*/

   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Lcda_Cvw_Zone_Calculation_Mode_T;

/**
 * @brief Lcda_Coordinate_System_T lists both available coordinate systems (VCS and curvi).
 *
 * @SDD{CSCSA-88616}
 */
typedef enum
{
   LCDA_USE_VCS   = (0), /**< Use Vehicle Coordinate System (VCS) */
   LCDA_USE_CURVI = (1)  /**< Use curvi coordinate system */

   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Lcda_Coordinate_System_T;

/**
 * @brief Lcda_Zone_Checks_T summarizes different ways to check an objects zone occupation.
 *
 * @SDD{CSCSA-88617}
 */
typedef enum
{
   LCDA_ZONE_CHECK_FOI_OVERLAP = (0), /**< Check whether an objects partially or completely overlaps its zone. */
   LCDA_ZONE_CHECK_REF_POINT   = (1)  /**< Only check whether the calculated reference point is inside of the zone. */

   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Lcda_Zone_Checks_T;

/**
 * @brief Lcda_Fallback_State_T summarizes fallback states of objects.
 *
 * @SDD{SF-6783}
 */
typedef enum
{
   FALLBACK_FAST = (0), /**< Object can be indicated as fast fallback which resolves in a high relative velocity */
   FALLBACK_SLOW = (1)  /**< Object can be indicated as slow fallback which resolves in a low relative velocity */
} Lcda_Fallback_State_T;

/**
 * @brief Lcda_Front_BSW_Custom_Limit_mode_T summarizes different ways to set bsw custom front limit.
 *
 * @SDD{CSCSA-88618}
 */
typedef enum
{
   BSW_FRONT   = (0), /**< Custom front bsw zone boundary set to the bsw zone front boundary */
   HOST_FRONT  = (1), /**< Custom front bsw zone boundary shifted to the host front bumper */
   CUSTOM      = (2), /**< Custom front bsw zone boundary shifted to calibration parameter value */
   COMPENSATED = (3)  /**< Custom front bsw zone boundary compensated for tracker output delay counted from the HOST_FRONT */

   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Lcda_Front_Bsw_Custom_Limit_mode_T;

/**
 * @brief Lcda_Guardrail_Status_T summarizes validity states of Lcda.
 *
 * @SDD{SF-6784}
 */
typedef enum
{
   LCDA_GUARDRAIL_INVALID = (0),   /**< status indicating that guardrail is invalid*/
   LCDA_GUARDRAIL_VALID   = (0x01) /**< status indicating that guardrail is valid*/
} Lcda_Guardrail_Status_T;

/**
 * @brief Lcda_Warn_Settings_T summarizes warning thresholds for Lcda submodules.
 *
 * @SDD{SF-6526}
 */
typedef struct
{
   float32_T bsw_len_factor;                       /**< Length factor for the Bsw zone*/
   float32_T cvw_ttc_threshold;                    /**< Ttc threshold for Cvw*/
   float32_T cvw_ttc_speed_factor;                 /**< Ttc speed factor for Cvw*/
   float32_T slc_ttc_thres_lon;                    /**< Longitudinal ttc threshold for Slc*/
   float32_T slc_ttc_thres_lat;                    /**< Lateral ttc threshold for Slc*/
   Float_Range_T cvw_rel_vel_range;                /**< Min and Max relative velocity threshold for CVW */
   boolean_T f_use_cvw_lane_change_intention_zone; /**< Flag indicating whether another cal set shall be used.*/
} Lcda_Warn_Settings_T;

/**
 * @brief Lcda_Guardrail_Information_T summarizes Lcda internal guardrail information.
 *
 * @SDD{SF-6520}
 */
typedef struct
{
   float32_T lateral_position;     /**< Lateral position of the guardrail*/
   float32_T confidence;           /**< metric for the existing probability of guardrail*/
   Lcda_Guardrail_Status_T status; /**< validity flag*/
} Lcda_Guardrail_Information_T;

/**
 * @brief Lcda_Guardrail_Sources_T summarizes possible guardrail module informations.
 *
 * @SDD{SF-6521}
 */
typedef struct
{
   Lcda_Guardrail_Information_T radar;  /**< Radar guardrail information*/
   Lcda_Guardrail_Information_T camera; /**< Camera guardrail information*/
} Lcda_Guardrail_Sources_T;

/**
 * @brief Bsw_Object_T summarizes object properties of Bsw sub module.
 *
 * @SDD{SF-6508}
 */
typedef struct
{
   Fbk_Field_Of_Interest_T zone;            /**< Object specific Bsw zone */
   uint8_t ego_side;                        /**< side where object is located*/
   float32_T obj_front_position;            /**< front position of object */
   boolean_T f_obj_in_zone;                 /**< Flag indicating whether object is in zone */
   boolean_T f_obj_long;                    /**< Falg indicating that BSW object is considered as long*/
   const Fbk_Object_Data_T *p_tracker_data; /**< Pointer to objects tracker properties*/
   float32_T area_overlap_ratio;            /**< Ratio of the zone-object intersection area and object area  */
} Bsw_Object_T;

/**
 * @brief Cvw_Object_T summarizes object properties of Cvw sub module.
 *
 * @SDD{SF-6509}
 */
typedef struct
{
   Fbk_Field_Of_Interest_T zone; /**< Object specific Cvw zone */

   float32_T ttc;                /**< Cvw ttc*/
   float32_T critical_distance;  /**< Cvw critical distance for which this object needs to decelerate in case of hosts lane change
                                    intention*/
   float32_T obj_front_position; /**< front position of object used in Cvw*/
   float32_T obj_front_position_lat; /**< Lateral front position of object used in Cvw*/

   uint8_t ego_side; /**< side where object is located*/

   boolean_T f_obj_mature_in_zone;   /**< flag indicating whether object is mature in zone */
   boolean_T f_obj_behind_guardrail; /**< flag indicating whether object is behind guardrail*/
   boolean_T f_obj_in_zone;          /**< flag indicating whether object is in zone */
   boolean_T f_obj_in_ego_lane;      /**< flag indicating whether object is hosts lane*/

   const Fbk_Object_Data_T *p_tracker_data; /**< Pointer to objects tracker properties*/

} Cvw_Object_T;

/**
 * @brief Elc_Object_T summarizes object properties of Elc sub module.
 *
 * @SDD{SF-6510}
 */
typedef struct
{
   Fbk_Field_Of_Interest_T zone; /**< Object specific Elc zone */

   float32_T lon_ttc; /**< longitduninal TTC (time until targets front bumper collides with egos rear bumper; no acceleration
                         considered) */
   float32_T obj_decel_to_reach_host_speed; /**< Object deceleration required to reach host speed */

   uint8_t ego_side; /**< side where object is located*/

   boolean_T f_obj_in_zone;               /**< flag indicating whether object is in zone */
   boolean_T f_obj_ttc_below_threshold;   /**< flag indicating whether objects ttc is below the respective threshold*/
   boolean_T f_obj_decel_above_threshold; /**< flag indicating whether objects needed deceleration to return to host speed is above
                                             the respective threshold*/

   const Fbk_Object_Data_T *p_tracker_data; /**< Pointer to objects tracker properties*/

} Elc_Object_T;

/**
 * @brief Slc_Object_T summarizes object properties of Slc sub module.
 *
 * @SDD{SF-6527}
 */
typedef struct
{
   Fbk_Field_Of_Interest_T zone; /**< Object specific Slc zone */

   float32_T lon_ttc; /**< longitudinal TTC (time until targets front bumper collides with egos rear bumper; no acceleration
                        considered) */
   float32_T lat_ttc; /**< lateral TTC (time until targets side collides with egos side; no acceleration considered) */
   float32_T lane_change_prob; /**< probability based on lateral TTC to estimate a crash (-> customer?) */
   float32_T effective_lateral_speed /**< lateral object speed that is compensated by the host vehicle moving across the lanes */;

   uint8_t ego_side; /**< side where object is located*/

   boolean_T f_obj_in_zone;             /**< flag indicating whether object is in zone*/
   boolean_T f_obj_besides_ego;         /**< flag indicating whether object is beside the host vehicle */
   boolean_T f_obj_overlap;             /**< flag indicating whether object is overlapping the host vehicle */
   boolean_T f_obj_ttc_below_threshold; /**< flag indicating whether object is below Slc threshold*/
   boolean_T f_obj_behind_guardrail;    /**< flag indicating whether object is behind guardrail*/
   boolean_T f_obj_lane_change;         /**< flag indicating whether object is changing lanes */
   boolean_T f_obj_misses_ego;          /**< flag indicating whether object is missing the host vehicle due to high delta speed */

   const Fbk_Object_Data_T *p_tracker_data; /**< Pointer to objects tracker properties*/

} Slc_Object_T;

/**
 * @brief Lcda_Trailer_Object_T summarizes object properties of a trailer attached to the host vehicle.
 *
 * @SDD{CSCSA-88605}
 */
typedef struct
{
   boolean_T f_trailer_present; /**< Flag indicating whether a trailer is attached to the host vehicle */

   float32_T length; /**< [m] Length of the trailer (Back edge of the trailer will be host length plus trailer length when going
                        straight) */
   float32_T width;  /**< [m] Width of the trailer */

   float32_T angle; /**< [rad] Angle of the trailer */

} Lcda_Trailer_Object_T;

/**
 * @brief Lcda_Object_Location_Data_T describes position of obj wrt. zone.
 *
 * @SDD{CSCSA-88606}
 */
typedef struct
{
   boolean_T obj_in_zone;
   float32_T area_overlap_ratio;

} Lcda_Object_Location_Data_T;

#endif /*LCDA_TYPES_H */
