#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer output declaration.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "ced_generic_types.h"
#include "ced_output_t.h"
#include "ced_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/


/**
 * @brief Generic CED object with highest criticality .
 *
 *
 * @SRS{CSCSA-122155}
 * @SAD{CSCSA-121741}
 * @SDD{CSCSA-122285}
 */
typedef struct
{
   uint8_t id;            /**< CED ID of object */
   uint32_t unique_id;    /**< unique ID of object tracker */
   Pa_Obj_Class_T type;   /**< type of CED object  */
   float32_T length_m;    /**< CED object length */
   float32_T width_m;     /**< CED object width */
   float32_T lat_pos_m;   /**< lateral position of CED object */
   float32_T long_pos_m;  /**< longitudinal position of CED object */
   float32_T speed_mps;   /**< CED object speed */
   float32_T heading_rad; /**< CED object heading */

   Ced_Target_Travel_Direction_T direction; /* direction of approaching object */
   float32_T predicted_lat_pos_m;           /**< CED lateral position of predicted object */
   float32_T ttc_s;                         /**< CED Time-to-Collision */
   float32_T ttp_s;                         /**< CED Time-to-Pass */


} Ced_Critical_Object_T;


/**
 * @brief Generic Ced_Output_T structure.
 *
 *
 * @SRS{CSCSA-122155}
 * @SAD{CSCSA-121741}
 * @SDD{CSCSA-122323}
 */
typedef struct
{
   boolean_T f_ced_enable;                                /* Flag indicating the status of the CED function */
   Ced_Alert_T ced_alert[FBK_NUMBER_OF_SIDES];            /**< CED alert level */
   Ced_Critical_Object_T ced_object[FBK_NUMBER_OF_SIDES]; /* Ced alert object with parameters */

} Ced_Output_T;

#endif /* CED_OUTPUT_T_H */
