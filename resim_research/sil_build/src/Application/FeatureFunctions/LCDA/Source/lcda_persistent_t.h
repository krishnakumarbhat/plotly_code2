#ifndef LCDA_PERSISTENT_T_H
#define LCDA_PERSISTENT_T_H

/**
 * @file lcda_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Definition structs and defines for persistent data used across all LCDA modules
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "lcda_types.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Persistent_T summarizes overall LCDA related persistent information.
 *
 * @SDD{SF-6522}
 */
typedef struct
{
   Lcda_Turn_Signal_T turn_signal_held;        /**< Held value of the turn_signal */
   uint8_t turn_signal_held_counter;           /**< Counter for holding the turn signal */
   boolean_T f_host_speed_in_activation_range; /**< Flag indicating if the host speed is in the activation range */
   boolean_T f_curve_radius_valid;             /**< Flag indicating curvelinear radius was correct for LCDA fetures */
   boolean_T f_bsw_prev_reset; /**< Flag indicating that the persistent variables and outputs have been reset for BLIS */
   boolean_T f_cvw_prev_reset; /**< Flag indicating that the persistent variables and outputs have been reset for CVW */


} Lcda_Persistent_T;

/**
 * @brief Lcda_Bsw_Persistent_T summarizes Bsw persistent data.
 *
 * @SDD{SF-6512}
 */
typedef struct
{
   uint8_t prev_bsw_alert_obj_id[FBK_NUMBER_OF_SIDES];         /**< ID of the target which previously triggered a BSW alert.
                                                            Value is 0 when no previous BSW alert. */
   uint32_t prev_bsw_alert_obj_unique_id[FBK_NUMBER_OF_SIDES]; /**< Unique ID of the target which previously triggered a BSW alert.
                                                    Value is 0 when no previous BSW alert. */

   uint8_t mature_count_in_bsw_zone[LCDA_OBJ_MAX_ARRAY_SIZE];     /**< Count of how many cycles an object was in the BSW zone. */
   Lcda_Fallback_State_T fallback_state[LCDA_OBJ_MAX_ARRAY_SIZE]; /**< Fallback state of the object. Can be FALLBACK_FAST or
                                                                      FALLBACK_SLOW. */

   uint8_t fallback_fast_to_slow_qual_ctr[LCDA_OBJ_MAX_ARRAY_SIZE]; /**< Qualification counter for slow fallback state*/


   uint8_t bsw_hold_counter[FBK_NUMBER_OF_SIDES];    /**< Counter used to hold alert after conditions are no longer met*/
   boolean_T f_prev_bsw_active[FBK_NUMBER_OF_SIDES]; /**< Flag indicating that a blis alert was active in the previous scan (used
                                                         to enable hysteresis)*/
   boolean_T f_prev_long_truck_status[LCDA_OBJ_MAX_ARRAY_SIZE]; /**< Flag indicating that truck was considered as long truck in
                                                                    previous cycle (used to enable hysteresis)*/

} Lcda_Bsw_Persistent_T;

/**
 * @brief Lcda_Cvw_Persistent_T summarizes Cvw persistent data.
 *
 * @SDD{SF-6517}
 */
typedef struct
{
   float32_T prev_curve_zone_factor[FBK_NUMBER_OF_SIDES]; /**< Curve zone factor that was applied in the previous cycle */

   uint8_t mature_count_in_cvw_zone[LCDA_OBJ_MAX_ARRAY_SIZE]; /**< Counter for how many scans an object has had mature status
                                                               while a valid cvw target */
   uint8_t prev_cvw_alert_obj_index[FBK_NUMBER_OF_SIDES]; /**< Index of the target which previously triggered a CVW alert. Value
                                                              is 255 when no previous CVW alert */

   uint8_t prev_cvw_alert_obj_id[FBK_NUMBER_OF_SIDES];         /**< ID of the target which previously triggered a cvw alert.
                                                            Value is 0 when no previous CVW alert */
   uint32_t prev_cvw_alert_unique_obj_id[FBK_NUMBER_OF_SIDES]; /**< ID of the target which previously triggered a cvw alert.
                                                    Value is 0 when no previous CVW alert */

   uint8_t cvw_hold_counter[FBK_NUMBER_OF_SIDES]; /**< Counter used to hold alert after conditions are no longer met */

   boolean_T f_prev_cvw_active[FBK_NUMBER_OF_SIDES]; /**< Flag indicating that a cvw alert was active in the previous scan (used
                                                      to enable hysteresis) */

   boolean_T f_prev_used_small_lc_intention_zone[FBK_NUMBER_OF_SIDES]; /**< Flag indicating that a the small lane change intention
                                                                           zone was used in previous cycle */
   uint8_t lc_intention_zone_change_counter[FBK_NUMBER_OF_SIDES];      /**< Counter used to qualify for a change of the used lane
                                                                           change intention zone */
} Lcda_Cvw_Persistent_T;

/**
 * @brief Lcda_Elc_Persistent_T summarizes Elc persistent data.
 *
 * @SDD{SF-6524}
 */
typedef struct
{
   uint8_t mature_count_in_elc_zone[LCDA_OBJ_MAX_ARRAY_SIZE]; /**< Count of how many cycles an object was in the ELC zone with
                                                                  status MATURE. */

   uint8_t prev_elc_alert_obj_index[FBK_NUMBER_OF_SIDES]; /**< Index of the target which previously triggered a ELC alert. Value
                                                              is 255 when no previous ELC alert */
   uint8_t prev_elc_alert_obj_id[FBK_NUMBER_OF_SIDES];    /**< ID of the target which previously triggered a ELC alert. Value is 0
                                                           when no previous ELC alert */

   uint8_t elc_hold_counter[FBK_NUMBER_OF_SIDES]; /**< Counter used to hold alert after conditions are no longer met*/

} Lcda_Elc_Persistent_T;

/**
 * @brief Lcda_Slc_Persistent_T summarizes Slc persistent data.
 *
 * @SDD{SF-6522}
 */
typedef struct
{
   uint8_t mature_count_in_slc_zone[LCDA_OBJ_MAX_ARRAY_SIZE]; /**< Count of how many cycles an object was in the SLC zone with
                                                                  status MATURE. */

   uint8_t slc_qualifying_counter[FBK_NUMBER_OF_SIDES];   /**< Alert qualification counter for SLC */
   uint8_t prev_slc_alert_obj_index[FBK_NUMBER_OF_SIDES]; /**< Index of the target which previously triggered a SLC alert. Value
                                                              is 255 when no previous SLC alert */
   uint8_t prev_slc_alert_obj_id[FBK_NUMBER_OF_SIDES];    /**< ID of the target which previously triggered a SLC alert. Value is 0
                                                              when no previous SLC alert */
   uint32_t prev_slc_alert_unique_obj_id[FBK_NUMBER_OF_SIDES]; /**< ID of the target which previously triggered a SLC alert. Value
                                                           is 0 when no previous SLC alert */

   uint8_t slc_hold_counter[FBK_NUMBER_OF_SIDES]; /**< Counter used to hold alert after conditions are no longer met*/

} Lcda_Slc_Persistent_T;

#endif /*LCDA_PERSISTENT_T_H */
