#ifndef SCW_OUTPUT_T_H
#define SCW_OUTPUT_T_H

/**
 * @file scw_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the output data structure for SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "fbk_macros.h"
#include "ml_vector_2d.h"
#include "scw_core_output_t.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/
typedef struct
{
   Scw_Alert_Level_T alert_level; /**< May be interpreted as Alert Level or Criticality Level */
   uint8_t id;                    /**< Object ID */
   uint32_t unique_id;            /**< Object ID */
   Scw_Object_Type_T type;        /**< Type of the Object: dynamic, barrier or none */
   float32_T lateral_ttc_s;       /**< [s] Lateral TTC of the critical dynamic object or the Host to the critical barrier*/
   float32_T lateral_distance_m;  /**< [m] Lateral distance to the critical dynamic object or barrier, metal-metal */
   Vector_2d_T position_m;        /**< [m] VCS position of the critical object */
   Vector_2d_T velocity_mps; /**< [m/s] VCS velocity (long, lat) of the critical dynamic object or only lateral velocity of the
                                Host to the barrier (0, lat) */
   Vector_2d_T acceleration_mps2;   /**< [m/s^2] VCS acceleration (long, lat) of the critical dynamic object or only lateral
                                       acceleration of the Host to the barrier (0, lat) */
   float32_T width_m;               /**< [m] Width of the dynamic object, 0 for the guardrail */
   float32_T length_m;              /**< [m] Length of the dynamic object, 0 for the guardrail */
   float32_T heading_rad;           /**< [rad] Heading of the dynamic object, 0 for the guardrail */
   float32_T yawrate_radps;         /**< [rad/s] Yawrate of the dynamic object, 0 for the guardrail */
   float32_T existence_probability; /**< Existence probability of the critical object */
   uint16_t age;                    /**< Age of the critical object */

   float32_T ttle_s; /**< [s] TTLE - Time to Lateral Exit - of critical object, per side. */
   float32_T ttp_s;  /**< [s] TTP - Time to Pass - of critical object, per side. */
} Scw_Output_Object_T;

typedef struct
{
   boolean_T f_scw_enabled;                             /**< SCW feature enabled/disabled */
   boolean_T f_scw_dyn_enabled;                         /**< SCW dynamic objects enabled/disabled */
   boolean_T f_scw_guardrail_enabled;                   /**< SCW guardrails enabled/disabled */
   Scw_Output_Object_T scw_object[FBK_NUMBER_OF_SIDES]; /**< SCW Critcal objects data */
} Scw_Output_T;


#endif /* SCW_OUTPUT_T_H */
