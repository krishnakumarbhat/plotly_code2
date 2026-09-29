#ifndef ML_FLOAT_RANGE_T_H
#define ML_FLOAT_RANGE_T_H
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
* \brief A 1d range using floats. Min and max can be equal. This is a corner case of a range.
* \ingroup interval
*/
typedef struct Float_Range_Tag
{
   float32_T min; /**< Minimal value in interval */
   float32_T max; /**< Maximal value in interval */
} Float_Range_T;

#ifdef __cplusplus
}
#endif
#endif
