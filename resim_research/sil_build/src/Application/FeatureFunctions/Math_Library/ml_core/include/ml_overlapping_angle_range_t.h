#ifndef ML_OVERLAPPING_Angle_Range_T_H
#define ML_OVERLAPPING_Angle_Range_T_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include "ml_angle_range_t.h"

/**
* Get_Overlapping_Angle_Range computes the overlapping angle ranges of two angle ranges.
* This can lead to two angle ranges.
* \sa Angle_Range_T
* \ingroup angle_range
*/
#define MAX_OVERLAPPING_RANGES    (2)

/**
* \brief Get_Overlapping_Angle_Range computes the overlapping angle ranges of two angle ranges.
*
* Since this can lead to two angle ranges this structure is needed.
* \sa Angle_Range_T
* \ingroup angle_range
*/
typedef struct Overlapping_Angle_Range_Tag
{
   Angle_Range_T overlapping_ranges[MAX_OVERLAPPING_RANGES]; /**< the ranges derived from overlapping two ranges*/
   uint8_t       number_of_ranges; /**< Number of ranges available in overlapping_ranges */
} Overlapping_Angle_Range_T;

#ifdef __cplusplus
}
#endif
#endif
