/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef TRACK_STATUS_T_H
#define TRACK_STATUS_T_H

/**
 * \ingroup Enumerations
 * Tracking status of an entity.
 * Gives information about the current tracking status of an entity. Informs if the entity is valid, estimated or in a measured state.
 * 
 * 
 */
typedef enum
{
   TRACK_STATUS_INVALID = (0), /**< does not exist*/
   TRACK_STATUS_NEW     = (1), /**< unconfirmed */
   TRACK_STATUS_MATURE  = (2), /**< was confirmed by measurements in more than 1 cycle and a measurement update has been done in current cycle */
   TRACK_STATUS_COASTED = (3), /**< not confirmed by measurement in current cycle. Therefore available data is a prediction. */
   NUMBER_OF_OBJECT_STATUS = (4)
} track_status_T;

#endif

