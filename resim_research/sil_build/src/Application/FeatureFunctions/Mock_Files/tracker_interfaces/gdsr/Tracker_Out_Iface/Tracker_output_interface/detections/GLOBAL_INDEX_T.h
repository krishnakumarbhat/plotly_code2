/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef GLOBAL_INDEX_T_H
#define GLOBAL_INDEX_T_H
#include "reuse.h"

typedef struct
{
    uint8_t idx; /* index */
    uint8_t f_gidx_set : 1; /* used to indicate unset gids for associations */
    uint8_t sensor_idx : 7; /* Sensor of origin */
}GLOBAL_INDEX_T;

#endif
