/**\file
 * Tracker: GDSR
 * This file implements the Call to tracker output of
 * the GDSR tracker.
 *
 * \Requirements
 * \reqtrace{}{}
 */

/*============================================================================*\
 *
 * Copyright 2022 Aptiv Technologies, Inc, All Rights Reserved
 * It is not allowed to reproduce or utilize parts of this document in any form
 * or by any means, including photocopying and microfilm, without permission in
 * written by Aptiv Technologies, Inc.
 *
\*============================================================================*/

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "pa_context.h"
#include "Tracker_Wrapper.h"

/*============================================================================*\
 * Global functions definition
\*============================================================================*/
void Pa_Get_Gdsr_Perception_Data(Pa_Context_T *p_context)
{
   /* get GDSR perception data */
   p_context->p_vehicle_data   = Get_Tracker_Veh_Ptr();
   p_context->p_tracker_output = Get_Tracker_Out_Ptr();
}
