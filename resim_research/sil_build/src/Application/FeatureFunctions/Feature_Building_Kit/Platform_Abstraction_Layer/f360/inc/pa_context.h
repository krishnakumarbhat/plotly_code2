#ifndef PA_CTX_H
#define PA_CTX_H

/**\file
 * Tracker: F360
 * This file declares the Tracker objects for the
 * F360 Tracker and host vehicle properties
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

#include "f360_object_log.h"
#include "T360_Types.h"
#include "VehicleInfoLog.h"

/*============================================================================*\
* Typedefs
\*============================================================================*/

/**
 * Struct containing the tracker data for all objects.
 */
typedef struct
{
   F360_Object_Log_T obj[MAX_F360_OBJECTS];
} F360_All_Objects_Log_T;


/**
 * Struct containing the tracker data.
 */
typedef struct
{
   /* Host vehicle state */
   Vehicle_Info_Log_T *p_vehicle_data;

   /* Objectdata created by tracker */
   F360_All_Objects_Log_T *p_tracker_output;

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
void Pa_Get_F360_Perception_Data(Pa_Context_T *p_context /**<[out] Context pointer*/);

#endif
