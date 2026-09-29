/**\file
 * Tracker: U360
 * This file implements the Call to tracker output of
 * the U360 tracker.
 *
 * \Requirements
 * \reqtrace{}{}
 */

/*============================================================================*\
 *
 * Copyright 2020 Aptiv Technologies, Inc, All Rights Reserved
 * It is not allowed to reproduce or utilize parts of this document in any form
 * or by any means, including photocopying and microfilm, without permission in
 * written by Aptiv Technologies, Inc.
 *
\*============================================================================*/

/*============================================================================*\
 * Includes
\*============================================================================*/
#include "pa_context.h"
#include "u360_tracker_wrapper.h"

/*============================================================================*\
 * Global functions definition
\*============================================================================*/
void Pa_Get_U360_Perception_Data(Pa_Context_T *p_context)
{
   /* Get U360 vehicle data */
   p_context->p_vehicle_data = Get_Tracker_Veh_Ptr();

   /* Get U360 tracker data */
   p_context->p_tracker_output = Get_Tracker_Out_Ptr();

   /* Get vehicle config data */
   p_context->p_vehicle_config_data = Get_Vehicle_Config_Ptr();
}
