/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef PRNDL_STATE_T_H
#define PRNDL_STATE_T_H

/**
 * Host vehicle gear setting
 * \ingroup host_vehicle
 */
typedef enum
{
   PRNDL_STATE_PARK    = (0), /**< Host is in parking gear */
   PRNDL_STATE_REVERSE = (1), /**< Host is in reverse gear */
   PRNDL_STATE_NEUTRAL = (2), /**< Host is in neutral gear */
   PRNDL_STATE_DRIVE   = (3), /**< Host is in a driving gear */
   PRNDL_STATE_FOURTH  = (4), /**< Host is in fourth gear */
   PRNDL_STATE_THIRD   = (5), /**< Host is in third gear */
   PRNDL_STATE_LOW     = (6)  /**< Host is in a low gear */
} PRNDL_state_T;

#endif

