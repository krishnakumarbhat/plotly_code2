#ifndef CED_CORE_OUTPUT_T_H
#define CED_CORE_OUTPUT_T_H

/**
 * @file ced_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core output data structure for CED.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ced_Core_Output_T structure
 *
 * @SRS{SF-106}
 * @SAE{SF-2452}
 * @SDD{SF-3557}
 */
typedef struct
{
   float32_T ced_ttc[FBK_NUMBER_OF_SIDES];                      /**< CED Time-to-Collision */
   float32_T ced_ttp[FBK_NUMBER_OF_SIDES];                      /**< CED Time-to-Pass */
   float32_T ced_object_predicted_lat_pos[FBK_NUMBER_OF_SIDES]; /**< CED lateral position of predicted object */

   uint8_t ced_id[FBK_NUMBER_OF_SIDES];                      /**< CED ID of object */
   uint32_t ced_unique_id[FBK_NUMBER_OF_SIDES];              /**< CED Unique ID of an object */
   uint8_t ced_index[FBK_NUMBER_OF_SIDES];                   /**< CED index of object */
   Ced_Alert_T ced_alert[FBK_NUMBER_OF_SIDES];               /**< CED alert level */
   uint8_t ced_object_path_match_index[FBK_NUMBER_OF_SIDES]; /**< CED object path match index */

   uint8_t ced_object_direction[FBK_NUMBER_OF_SIDES]; /**< CED object direction */

   float32_T ced_front_bumper_pos_long[FBK_NUMBER_OF_SIDES];             /**< [m] Object front bumper position (longitudinally) */
   float32_T ced_vcs_vel_rel_x[FBK_NUMBER_OF_SIDES];                     /**< [m] Object rear bumper position (longitudinally) */
   float32_T ced_object_closest_lat_dist_predicted[FBK_NUMBER_OF_SIDES]; /**< [m] CED closest lateral distance from the ego side to
                                                                            the predicted object points*/
} Ced_Core_Output_T;

#endif /* CED_CORE_OUTPUT_T_H */
