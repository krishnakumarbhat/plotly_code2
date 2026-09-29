/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef MOTION_STATUS_T_H
#define MOTION_STATUS_T_H

/** \ingroup Enumerations */

typedef enum
{
   MOTION_STATUS_INVALID,    /**< Motion status not set */
   MOTION_STATUS_STATIONARY, /**< Below the stationary range rate threshold */
   MOTION_STATUS_MOVING,     /**< Above the moving range rate threshold */
   MOTION_STATUS_AMBIGUOUS,           /**< rangerate is either stationary or moving: rangerate is ZERO		: because target is stationary OR targets velocity vector is perpendicular to the radial axis of sensor. */
   MOTION_STATUS_AMBIGUOUS_ALIASING 	/**< rangerate is either stationary or moving: rangerate is NONZERO	: when host velocity is higher than unambiguous range for rangerate, stationary targets might be classified as moving due to aliasing. Use without dealiasing. Dealiasing will lead to stationary detections */
} motion_status_T;

#endif

