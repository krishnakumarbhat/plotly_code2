#ifndef ESA_OUTPUT_T_H
#define ESA_OUTPUT_T_H

/**
 * @file esa_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer output declaration.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "esa_core_output_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

/**
 * @brief defines the ESA critical object
 *
 * @SRD{CSCSA-122861}
 * @SAD{CSCSA-123110}
 * @SDD{CSCSA-123049}
 * @verification{Component test: Check generic output signals for defined test scenario.}
 */
typedef struct
{
   uint8_t id;
   uint8_t index;

   float32_T width_m;
   float32_T length_m;

   float32_T long_pos_m;
   float32_T lat_pos_m;
   float32_T long_speed_mps;
   float32_T lat_speed_mps;

   float32_T ttc_s;
   float32_T ttp_s;
   /* float32_T timestamp_s;   removed temporarily due to no signal from the Tracker  */

   float32_T decel_to_reach_host_speed_mps2;
   float32_T long_distance_m;

   float32_T existence_prob;

} Esa_Critical_Object_T;


/**
 * @brief Esa_Output_T structure.
 *
 *
 * @SRD{CSCSA-122861}
 * @SAD{CSCSA-123110}
 * @SDD{CSCSA-123048}
 * @verification{Component test: Check generic output signals for defined test scenario.}
 */
typedef struct
{

   Esa_Core_Status_T esa_status;
   boolean_T f_esa_alert[FBK_NUMBER_OF_SIDES];
   Esa_Critical_Object_T esa_object[FBK_NUMBER_OF_SIDES];

} Esa_Output_T;

#endif /* ESA_OUTPUT_T_H */
