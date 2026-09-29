#ifndef RECW_OUTPUT_T_H
#define RECW_OUTPUT_T_H

/**
 * @file recw_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements output type definition for bmw_sp25 project.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "recw_bmw_sp25_types.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * Output structure of RECW for BMW SRR5 project
 *
 * @SRS{SF-1664}
 * @SAE{SF-2963}
 * @SDD{SF-7995}
 */
typedef struct
{

   uint8_t recw_status; /**< Flag indicating that RECW is enabled */

   /* Request RECW signals according to requirement*/
   uint8_t recw_status_collision_warning; /**< Flag indicating that there is an active RECW warning */
   uint8_t recw_status_precrash;          /**< Flag indicating that there is an active RECW pre crash alert */
   /* Event Data recorder according to requirement*/
   uint8_t edr_drasy_event_ID62_status_collision_warning_side_radar_rear;
   uint8_t edr_drasy_event_ID62_status_pre_crash_side_radar_rear;

   uint8_t recw_obj_id;               /**< ID of the object responsible for the RECW alert */
   uint32_t recw_obj_unique_id;       /**< Unique ID of the object responsible for the RECW alert */
   float32_T recw_ttc;                /**< [s] TTC (time to conflict) of the object responsible for the RECW alert */
   float32_T recw_obj_distance;       /**< [m] Distance of the object responsible for the RECW alert */
   float32_T recw_obj_approach_speed; /**< [m/s] Absolute relative velocity of the object responsible for the RECW alert */
   float32_T recw_obj_lat_pos;        /**< [m] Lateral position of the object responsible for the RECW alert */
   float32_T recw_obj_long_pos;       /**< [m] Logitudinal position of the object responsible for the RECW alert */
   float32_T recw_obj_heading;        /**< [rad] Heading of the object responsible for the RECW alert */
   float32_T recw_crash_probability;  /**< [%] Crash probabilty of the object responsible for the RECW alert */
   float32_T recw_overlap; /**< [%] Predicted overlap of the ego vehicle and the critical object responsible for the RECW alert */

   float32_T recw_ttc_warning_threshold; /**< [s] Max possible TTC warning threshold based on current relative speed between ego
                                            and target */

   uint8_t recw_obj_class;        /**< Object classification of the object responsible for the RECW alert */
   uint8_t recw_obj_class_cdc;    /**< Object classification of the object responsible for the RECW alert for CDC logging with
                                     different classes*/
   Recw_SM_State_T recw_sm_state; /**< RECW State from state machine */
   uint8_t unused;

} Recw_Output_T;

#endif /* RECW_OUTPUT_T_H */
