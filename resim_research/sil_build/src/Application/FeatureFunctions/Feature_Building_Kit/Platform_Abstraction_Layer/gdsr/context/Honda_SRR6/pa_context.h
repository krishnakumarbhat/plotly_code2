#ifndef PA_CTX_H
#define PA_CTX_H

/**\file
 * Tracker: GDSR
 * This file declares the Tracker objects for the
 * GDSR Tracker and host vehicle properties
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

#include "fbk_iface_types.h"
#include "TRACKER_OUTPUT_T.h"
#include "VEHICLE_DATA_FLT_T.h"

/*============================================================================*\
* Typedefs
\*============================================================================*/


/**
 * Struct containing the tracker data.
 * \Requirements
 * \reqtrace{}{}
 */
typedef struct
{
   /* Data for synchronization*/
   Fbk_Age_Ctr_T *p_fbk_obj_ageing;

   /**Preprocessed vehicle data*/
   VEHICLE_DATA_FLT_T *p_vehicle_data;

   /**Objectdata created by tracker*/
   TRACKER_OUTPUT_T *p_tracker_output;

} Pa_Context_T;

/*============================================================================*\
 * Global Function Declaration
\*============================================================================*/

/**
 * Update the pointer for the tracker data.
 *
 *
 * \return         void
 *
 * \Requirements
 * \reqtrace{}{}
 */
void Pa_Get_Gdsr_Perception_Data(Pa_Context_T *p_context /**<[out] context pointer*/);

#endif
