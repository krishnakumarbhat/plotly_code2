/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef MOUNTING_LONGITUDINAL_T_H
#define MOUNTING_LONGITUDINAL_T_H

/**
 * A radar sensors longitudinal position.
 * \sa mounting_location_T
 * \ingroup radar_mounting
 */
typedef enum
{
   MOUNTING_LONGITUDINAL_NOT_SET = (0),  /**< Longitudinal mounting position not set */
   MOUNTING_FORWARD              = (8),  /**< Mounted on front */
   MOUNTING_SIDE                 = (16), /**< Mounted in the center */
   MOUNTING_REAR                 = (24)  /**< Mounted on the rear */
} mounting_longitudinal_T;

#endif

