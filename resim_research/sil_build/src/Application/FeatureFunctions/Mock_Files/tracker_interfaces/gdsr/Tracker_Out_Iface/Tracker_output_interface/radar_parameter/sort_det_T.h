/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef SORT_DET_T_H
#define SORT_DET_T_H

#include "reuse.h"
#include "GLOBAL_INDEX_T.h"

/**
 * This structure holds the output data from the tracker
 */
typedef struct
{
   GLOBAL_INDEX_T  gidx;  //!< 0-based index of detection
   float32_T range; //!< range of detection
} sort_det_T;

typedef struct
{
   int32_t i_sort_min;
   int32_t i_sort_max;
} SORT_DET_RANGE_T;

#endif

