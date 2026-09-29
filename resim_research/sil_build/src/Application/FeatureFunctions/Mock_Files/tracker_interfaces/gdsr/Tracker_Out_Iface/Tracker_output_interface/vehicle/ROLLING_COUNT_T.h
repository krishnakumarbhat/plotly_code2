/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef ROLLING_COUNT_T_H
#define ROLLING_COUNT_T_H

#include "reuse.h"

/**
 * Rolling counter information.
 * \ingroup rolling_count
 */
typedef struct
{
   uint8_t count;     /**< rolling count of the current cycle*/
   uint8_t count_ref; /**< reference rolling count */
} ROLLING_COUNT_T;

#endif
