#ifndef PA_CTX_H
#define PA_CTX_H

/**\file
 * Tracker: U360
 * This file declares the Tracker objects for the
 * U360 Tracker and host vehicle properties
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

#include "TRACKER_OUTPUT_SIDE_T.h"
#include "VEHICLE_CONFIG_FLT_T.h"
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
   /** Vehicle sopecific configuration data */
   VEHICLE_CONFIG_FLT_T *p_vehicle_config_data;

   /** Preprocessed vehicle data */
   VEHICLE_DATA_FLT_T *p_vehicle_data;

   /** Objectdata created by tracker */
   TRACKER_OUTPUT_SIDE_T *p_tracker_output;

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
void Pa_Get_U360_Perception_Data(Pa_Context_T *p_context /**<[out] Context pointer*/);

#endif
