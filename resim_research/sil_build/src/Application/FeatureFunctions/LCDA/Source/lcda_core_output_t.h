#ifndef LCDA_CORE_OUTPUT_H
#define LCDA_CORE_OUTPUT_H

/**
 * @file lcda_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Definition of LCDA core output struct
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Status_T summarizes Enable states of the overall feature.
 *
 * @SDD{CSCSA-88129}
 */
typedef enum
{
   LCDA_STATUS_ENABLED                      = (0), /**< Overall feature is enabled */
   LCDA_STATUS_ACTIVE                       = (1), /**< Overall feature is in active state */
   LCDA_STATUS_DISABLED_BY_CAL              = (2), /**< Overall feature is disabled by calibration */
   LCDA_STATUS_DISABLED_BY_INPUT            = (3), /**< Overall feature is disabled by input */
   LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED    = (4), /**< Overall feature is disabled due to low host speed */
   LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED   = (5), /**< Overall feature is disabled due to high host speed */
   LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS = (6), /**< Overall feature is disabled by low curve radius */
   LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR   = (7)  /**< Overall feature is disabled by internal error */
} Lcda_Status_T;

/**
 * @brief Lcda_Alert_State_T summarizes alert level of Lcda submodules.
 *
 * @SDD{CSCSA-88130}
 */
typedef enum
{
   LCDA_ALERT_STATE_NONE    = (0), /**< Alert state is set to none*/
   LCDA_ALERT_STATE_LEVEL_1 = (1), /**< Alert state is set to level 1*/
   LCDA_ALERT_STATE_LEVEL_2 = (2)  /**< Alert state is set to level 2*/
} Lcda_Alert_State_T;

/**
 * @brief Lcda_Bsw_Core_Output_T structure summarizes Bsw output of the overall Lcda core output.
 *
 * @SDD{SF-6511}
 */
typedef struct
{
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES]; /**< Index of the tracker object that caused the alert. It is set to 255 when no alert
                                          exists */
   uint8_t bsw_id[FBK_NUMBER_OF_SIDES]; /**< ID of the tracker object that caused the alert. It is set to 0 when no alert exists */
   uint32_t bsw_unique_id[FBK_NUMBER_OF_SIDES]; /**< Unique ID of the tracker object that caused the alert. It is set to 0 when no
                                                   alert exists */
   float32_T bsw_distance[FBK_NUMBER_OF_SIDES]; /**< Distance of the object that caused the BSW alert */
   float32_T bsw_ttp[FBK_NUMBER_OF_SIDES];      /**< [s] TTP - Time to Pass
                                      Time until the critical object completely passes the front of Ego vehicle */
   float32_T bsw_ttle[FBK_NUMBER_OF_SIDES];     /**< [s] TTLE - Time to Lateral Exit
                                   Time until the critical object completely leaves the BSW zone in the lateral direction */
   boolean_T f_bsw_is_enabled;                  /**< Flag indicating that the BSW alert functionality is enabled */
   Lcda_Alert_State_T bsw_alert[FBK_NUMBER_OF_SIDES];     /**< Alert state. Possible values LCDA_ALERT_STATE_NONE,
                                                         LCDA_ALERT_STATE_LEVEL_1, LCDA_ALERT_STATE_LEVEL_2 */
   boolean_T f_obj_in_bsw_zone[PA_OBJ_NUMBER_OF_OBJECTS]; /**< flag indicating if the */
   Fbk_Field_Of_Interest_T bsw_zone[FBK_NUMBER_OF_SIDES]; /** BSW zone for most critical object per side */

} Lcda_Bsw_Core_Output_T;

/**
 * @brief Lcda_Cvw_Core_Output_T structure summarizes Cvw output of the overall Lcda core output.
 *
 * @SDD{SF-6516}
 */
typedef struct
{
   float32_T cvw_ttc[FBK_NUMBER_OF_SIDES];          /**< TTC of the object that caused the CVW alert */
   float32_T cvw_ttp[FBK_NUMBER_OF_SIDES];          /**< [s] TTP - Time to Pass
                                          Time until the critical object completely passes the front of Ego vehicle */
   float32_T cvw_ttle[FBK_NUMBER_OF_SIDES];         /**< [s] TTLE - Time to Lateral Exit
                                       Time until the critical object completely leaves the CVW zone in the lateral direction */
   float32_T cvw_distance[FBK_NUMBER_OF_SIDES];     /**< Distance of the object that caused the CVW alert */
   float32_T cvw_distance_lat[FBK_NUMBER_OF_SIDES]; /**< Lateral distance of the object that caused the CVW alert */
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]; /**< Index of the tracker object that caused the alert. It is set to 255 when no alert
                                          exists */
   uint8_t cvw_id[FBK_NUMBER_OF_SIDES]; /**< ID of the tracker object that caused the alert. It is set to 0 when no alert exists */
   uint32_t cvw_unique_id[FBK_NUMBER_OF_SIDES]; /**< Unique ID of the tracker object that caused the alert. It is set to 0 when no
                                                   alert exists */
   boolean_T f_cvw_is_enabled;                  /**< Flag indicating that the CVW alert functionality is enabled */
   Lcda_Alert_State_T cvw_alert[FBK_NUMBER_OF_SIDES]; /**< CVW alert state. Possible values LCDA_ALERT_STATE_NONE,
                                                LCDA_ALERT_STATE_LEVEL_1, LCDA_ALERT_STATE_LEVEL_2 */
} Lcda_Cvw_Core_Output_T;

/**
 * @brief Lcda_Slc_Core_Output_T structure summarizes Slc output of the overall Lcda core output.
 *
 * @SDD{SF-6523}
 */
typedef struct
{
   float32_T slc_lon_ttc[FBK_NUMBER_OF_SIDES]; /**< [s] Calculated longitudinal TTC of critical SLC object on the left/right side
                                              if present */
   float32_T slc_lat_ttc[FBK_NUMBER_OF_SIDES]; /**< [s] Calculated lateral TTC of critical SLC object on the left/right side if
                                               present */
   float32_T slc_ttp[FBK_NUMBER_OF_SIDES];     /**< [s] TTP - Time to Pass
                                     Time until the critical object completely passes the front of Ego vehicle */
   float32_T slc_lane_change_prob[FBK_NUMBER_OF_SIDES]; /**< Lane change probability of critical SLC object on the left/right side
                                                       if present */
   uint8_t slc_index[FBK_NUMBER_OF_SIDES]; /**< Index of critical SLC object on the left/right side if present (255 for none)*/
   uint8_t slc_id[FBK_NUMBER_OF_SIDES];    /**< ID of critical SLC object on the left/right side if present (0 for none)*/
   uint32_t slc_unique_id[FBK_NUMBER_OF_SIDES]; /**< Unique ID of critical SLC object on the left/right side if present (0 for
                                                   none)*/
   boolean_T f_slc_is_enabled;                  /**< Flag indicating that Sim Lane Change functionality is enabled */
   boolean_T slc_alert[FBK_NUMBER_OF_SIDES];    /**< Flag indicating if there is a critical SLC object for left/right side */
} Lcda_Slc_Core_Output_T;

/**
 * @brief Lcda_Elc_Core_Output_T structure summarizes Elc output of the overall Lcda core output.
 *
 * @SDD{SF-6518}
 */
typedef struct
{
   float32_T elc_ttc[FBK_NUMBER_OF_SIDES]; /**< [s] Calculated TTC of critical ELC object on the left/right side if present */
   float32_T elc_decel_to_reach_host_speed[FBK_NUMBER_OF_SIDES]; /**< [m/s^2] Required deceleration of critical ELC object to
                                                                reach host speed */
   uint8_t elc_index[FBK_NUMBER_OF_SIDES];   /**< Index of critical ELC object on the left/right side if present (255 for none)*/
   uint8_t elc_id[FBK_NUMBER_OF_SIDES];      /**< ID of critical ELC object on the left/right side if present (0 for none)*/
   boolean_T f_elc_is_enabled;               /**< Flag indicating that Evasive Lane Change functionality is enabled */
   boolean_T elc_alert[FBK_NUMBER_OF_SIDES]; /**< Flag indicating if there is a critical ELC object for left/right side */
} Lcda_Elc_Core_Output_T;


/**
 * @brief  Lcda_Core_Output_T structure gathers the submodule outputs in the core output interface.
 *
 * @SRS{SF-1071,SF-1101,SF-1111,SF-1115,SF-1116}
 * @SAE{SF-2812,SF-2813}
 * @SDD{SF-6515}
 */
typedef struct
{
   Lcda_Bsw_Core_Output_T bsw_core_output; /**< BSW output */
   Lcda_Cvw_Core_Output_T cvw_core_output; /**< CVW output */
   Lcda_Slc_Core_Output_T slc_core_output; /**< SLC output */
   Lcda_Elc_Core_Output_T elc_core_output; /**< ELC output */
   Lcda_Status_T lcda_status;              /**< Status of the LCDA as defined in enum Lcda_Status_T  */
} Lcda_Core_Output_T;

#endif /* LCDA_CORE_OUTPUT_H */
