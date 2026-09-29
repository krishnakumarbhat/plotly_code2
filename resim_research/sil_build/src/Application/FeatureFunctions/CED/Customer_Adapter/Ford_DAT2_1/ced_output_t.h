#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Ford DAT2.1 customer output.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/
#include "pa_reuse.h"

/*===========================================================================*\
 * Typedefs
\*===========================================================================*/
/**
 * @brief Contains the approach types for Ford project.
 */
typedef enum
{
   CED_FORD_APPROACH_REAR_LEFT       = (0), /**< Rear left approach */
   CED_FORD_APPROACH_REAR_RIGHT      = (1), /**< Rear right approach */
   CED_FORD_APPROACH_FRONT_LEFT      = (2), /**< Front left approach */
   CED_FORD_APPROACH_FRONT_RIGHT     = (3), /**< Front right approach */
   CED_FORD_NUMBER_OF_APPROACH_TYPES = (4)  /**< Number of approach types */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Ford_Approach_Types_T;

/**
 * @brief Ced_Output_T structure.
 *
 *
 * @SRS{SF-60}
 * @SAE{SF-2406}
 * @SDD{SF-3436}
 * @verification{}
 */
typedef struct
{
   uint8_t ced_alert[CED_FORD_NUMBER_OF_APPROACH_TYPES]; /** Alert indicator */
   uint8_t ced_id[CED_FORD_NUMBER_OF_APPROACH_TYPES];    /** Id of object with highest severity */

   float32_T ced_ttc[CED_FORD_NUMBER_OF_APPROACH_TYPES];            /** TTC for most critical object in [s] */
   float32_T ced_ttp[CED_FORD_NUMBER_OF_APPROACH_TYPES];            /** TTP for most critical object in [s] */
   float32_T ced_object_speed[CED_FORD_NUMBER_OF_APPROACH_TYPES];   /** Speed of most critical object in [m/s] */
   float32_T ced_object_heading[CED_FORD_NUMBER_OF_APPROACH_TYPES]; /** Heading of most critical object in [rad] */
   float32_T ced_object_length[CED_FORD_NUMBER_OF_APPROACH_TYPES];  /** Length of most critical object in [m] */
   float32_T ced_object_width[CED_FORD_NUMBER_OF_APPROACH_TYPES];   /** Width of most critical object in [m] */

} Ced_Output_T;

#endif /* CED_OUTPUT_T_H */
