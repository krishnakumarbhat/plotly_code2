/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef HOST_WHEEL_DIRECTION_H
#define HOST_WHEEL_DIRECTION_H

/**
 * The direction in which the hosts wheels are turning.
 * \ingroup host_vehicle
 */
typedef enum
{
   HOST_WHEEL_DIRECTION_NOT_SET     = (0), /**< Host wheel direction is not available or faulty */
   HOST_WHEEL_DIRECTION_STAND_STILL = (1), /**< Host wheels are not turning */
   HOST_WHEEL_DIRECTION_FORWARD     = (2), /**< Host wheels are turning moving the host forward */
   HOST_WHEEL_DIRECTION_REVERSE     = (3)  /**< Host wheels are turning moving the host reverse */
} Host_Wheel_Direction_T;

#endif

