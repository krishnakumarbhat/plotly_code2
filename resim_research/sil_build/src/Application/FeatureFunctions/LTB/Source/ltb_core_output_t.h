#ifndef LTB_CORE_OUTPUT_T_H
#define LTB_CORE_OUTPUT_T_H

/**
 * @file ltb_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core output data structure for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "ltb_types.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ltb_Core_Output_T structure
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53951}
 */
typedef struct
{
   Vector_2d_T ltb_waypoint_at_collision[FBK_NUMBER_OF_SIDES]; /**< [m] coordinates of collision waypoint */
   float32_T ltb_ttc[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-collision */
   float32_T ltb_ttb[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-brake */
   float32_T ltb_decel_estimate[FBK_NUMBER_OF_SIDES];          /**< [m/s^2] deceleration estimate to avoid collision */
   float32_T ltb_distance[FBK_NUMBER_OF_SIDES];                /**< [m] object distance */

   Ltb_Alert_State_T ltb_alert_level[FBK_NUMBER_OF_SIDES]; /**< Overall LTB alert level */

   uint8_t ltb_id[FBK_NUMBER_OF_SIDES];    /**< LTB object tracker ID */
   uint8_t ltb_index[FBK_NUMBER_OF_SIDES]; /**< LTB object tracker index */
   uint8_t ltb_most_critical_side;         /**< most critical LTB side based on alert levels and lowest ttc */

   boolean_T ltb_f_obj_in_zone[FBK_NUMBER_OF_SIDES]; /**< flag indicating object is in danger zone */

} Ltb_Core_Output_T;

#endif /* LTB_CORE_OUTPUT_T_H */
