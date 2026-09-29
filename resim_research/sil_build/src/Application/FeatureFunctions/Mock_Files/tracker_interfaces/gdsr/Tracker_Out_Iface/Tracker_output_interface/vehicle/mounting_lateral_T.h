/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef MOUNTING_LATERAL_T_H
#define MOUNTING_LATERAL_T_H

/**
 * A radar sensors lateral position.
 * \sa mounting_location_T
 * \ingroup radar_mounting
 */
typedef enum
{
   MOUNTING_LATERAL_NOT_SET = (0), /**< Lateral position not set */
   MOUNTING_LEFT            = (1), /**< Mounted on left side */
   MOUNTING_CENTER          = (2), /**< Mounted on the center */
   MOUNTING_RIGHT           = (3)  /**< Mounted on the right side */
} mounting_lateral_T;

#endif

