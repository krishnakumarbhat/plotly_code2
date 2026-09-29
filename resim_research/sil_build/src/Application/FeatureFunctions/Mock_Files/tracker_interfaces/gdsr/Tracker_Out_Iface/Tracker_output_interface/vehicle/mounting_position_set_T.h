/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef MOUNTING_POSITION_SET_T_H
#define MOUNTING_POSITION_SET_T_H

/**
 * If the radar sensor mounting position set.
 * \sa mounting_location_T
 * \ingroup radar_mounting
 */
typedef enum
{
   MOUNTING_POSITION_SET     = (128), /**< Mounting position has been set */
   MOUNTING_POSITION_NOT_SET = (0)    /**< Mounting position invalid */
} mounting_position_set_T;

#endif

