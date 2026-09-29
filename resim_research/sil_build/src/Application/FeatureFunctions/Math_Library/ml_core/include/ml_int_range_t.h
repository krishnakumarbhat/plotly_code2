#ifndef ML_INT_RANGE_T_H
#define ML_INT_RANGE_T_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

/**
* \brief A 1d range using integers. Min and max can be equal. This is a corner case of a range.
* \ingroup interval
*/

typedef struct Int_Range_Tag
{
   int32_t min; /**< Minimal value in interval */
   int32_t max; /**< Maximal value in interval */
} Int_Range_T;

#ifdef __cplusplus
}
#endif
#endif
