#ifndef SCW_OUTPUT_T_H
#define SCW_OUTPUT_T_H

/**
 * @file scw_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW SP25 output data structure for SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 *
 */

#include "pa_reuse.h"
#include "scw_core_output_t.h"
#include "scw_state_machine.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief BMW SP25 specific SCW output structure
 *
 * @return void
 *
 * @SAE{SF-2994}
 * @SRS{SF-8175}
 */
typedef struct
{
   uint8_t f_scw_enabled;                           /**< Flag indicating if SCW is enabled */
   uint8_t f_scw_dyn_enabled;                       /**< Flag indicating if dynamic objects subfeature is enabled */
   uint8_t f_scw_guardrail_enabled;                 /**< Flag indicating if guardrail subfeature is enabled */
   Scw_Extended_Qualifier_T scw_extended_qualifier; /**< Extended qualifier */
   SCW_States_T scw_status;                         /*SCW State Machine Output*/
   /*Left Object Output*/
   uint8_t scw_object_type_left;  /**< info if critical object on the left side is dynamic object (1) or guardrail (2), (0) if no
                                     critical object */
   uint8_t scw_object_type_right; /**< info if critical object on the right side is dynamic object (1) or guardrail (2), (0) if no
                                     critical object */

   uint8_t scw_object_id_left;         /**< object ID of overall critical object on the left side if present */
   uint32_t scw_object_unique_id_left; /**< object Unique ID of overall critical object on the left side if present */
   float32_T scw_object_px_left;       /**< [m] longitudinal distance of overall critical object on the left side if present */
   float32_T scw_object_py_left;       /**< [m] lateral distance of overall critical object on the left side if present */
   float32_T scw_object_width_left;    /**< [m] object width of critical object on the left side if present */
   float32_T scw_object_length_left;   /**< [m] object length of critical object on the left side if present */
   float32_T scw_object_heading_left;  /**< [rad] object heading of critical object on the left side if present */
   float32_T scw_object_yawrate_left;  /**< [rad/s] object yawrate of critical object on the left side if present */
   float32_T scw_object_ttc_left;      /**< [s] calculated TTC of overall critical object on the left side if present */
   float32_T scw_object_vx_left;       /**< [m/s] longitudinal speed of overall critical object on the left side if present */
   float32_T scw_object_vy_left; /**< [m/s] lateral speed (absolute if dynamic object, relative if guardrail) of overall critical
                                    object on the left side if present */
   float32_T scw_object_ax_left; /**< [m/s^2] longitudinal relative acceleration of critical object on the left side if present */
   float32_T scw_object_ay_left; /**< [m/s^2] lateral relative acceleration of critical object on the left side if present */
   uint16_t scw_object_existance_probability_left; /**< existence probability of overall critical object on the left side if
                                                      present */
   uint16_t scw_object_age_left;                   /**< age of critical object on the left side if present */

   /*Added for BMW SP25*/
   float32_T scw_object_time_stamp_left_output;    /**< Timestamp*/
   uint8_t scw_object_criticality_left_output;     /**< Bitfield set based on the critical object presence*/
   float32_T scw_object_fallback_time_left_output; /**< Fallback time of the critical object*/
   float32_T scw_object_ttp_left;          /**< [s] calculated TTP of overall critical object on the left side if present */
   float32_T scw_object_ttle_left;         /**< [s] calculated TTLE of overall critical object on the left side if present */
   float32_T scw_object_dy_left;           /**< [m] lateral distance to the host on the right side, metal to metal */
   float32_T scw_object_lateral_ttc_left;  /**< [s] lateral TTC on the right side */
   Scw_Alert_Level_T scw_alert_level_left; /**< alert level depending on object's lateral distance and ttc on the left side */

   /*Right Object Output*/
   uint8_t scw_object_id_right;         /**< object ID of overall critical object on the right side if present */
   uint32_t scw_object_unique_id_right; /**< object Unique ID of overall critical object on the right side if present */
   float32_T scw_object_px_right;       /**< [m] longitudinal distance of overall critical object on the right side if present */
   float32_T scw_object_py_right;       /**< [m] lateral distance of overall critical object on the right side if present */
   float32_T scw_object_width_right;    /**< [m] object width of critical object on the right side if present */
   float32_T scw_object_length_right;   /**< [m] object length of critical object on the right side if present */
   float32_T scw_object_heading_right;  /**< [rad] object heading of critical object on the right side if present */
   float32_T scw_object_yawrate_right;  /**< [rad/s] object yawrate of critical object on the right side if present */
   float32_T scw_object_ttc_right;      /**< [s] calculated TTC of overall critical object on the right side if present */
   float32_T scw_object_vx_right;       /**< [m/s] longitudinal speed of overall critical object on the right side if present */
   float32_T scw_object_vy_right; /**< [m/s] lateral speed (absolute if dynamic object, relative if guardrail) of overall critical
                                     object on the right side if present */
   float32_T scw_object_ax_right; /**< [m/s^2] longitudinal relative acceleration of critical object on the right side if present */
   float32_T scw_object_ay_right; /**< [m/s^2] lateral relative acceleration of critical object on the right side if present */
   uint16_t scw_object_existance_probability_right; /**< existence probability of overall critical object on the right side if
                                                      present */
   uint16_t scw_object_age_right;                   /**< age of critical object on the right side if present */

   /*Added for BMW SP25*/
   float32_T scw_object_time_stamp_right_output;    /**< Timestamp */
   uint8_t scw_object_criticality_right_output;     /**< Bitfield set based on the critical object presence */
   float32_T scw_object_fallback_time_right_output; /**< Fallback time of the critical object */
   float32_T scw_object_ttp_right;          /**< [s] calculated TTP of overall critical object on the right side if present */
   float32_T scw_object_ttle_right;         /**< [s] calculated TTLE of overall critical object on the right side if present */
   float32_T scw_object_dy_right;           /** [m] lateral distance to the host on the right side, metal to metal */
   float32_T scw_object_lateral_ttc_right;  /**< [s] lateral TTC on the right side */
   Scw_Alert_Level_T scw_alert_level_right; /**< alert level depending on object's lateral distance and ttc on the right side */

} Scw_Output_T;

#endif /* SCW_OUTPUT_T_H */
