#ifndef ST_ANGLE_NORMALIZE_H
#define ST_ANGLE_NORMALIZE_H

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#ifdef __cplusplus
extern "C"
{
#endif

#include "reuse.h"
/**
* Resolves the \f$2\pi\f$ ambiguity in the first input by shifting it to be within \f$+-\pi\f$ of p_theta_ref.
* \par Example
* Let \f$\tau \f$ be a full rotation: \f$\tau = 2* \pi \f$
* - Assume p_theta_in to be 17.5 * \f$\tau\f$
* - Assume p_theta_ref to be 0.3 * \f$\tau\f$
* - The output will be 0.5 * \f$\tau\f$
*
* - Assume p_theta_in to be 0.5 * \f$\tau\f$
* - Assume p_theta_ref to be 17.3 * \f$\tau\f$
* - The output will be 17.5 * \f$\tau\f$
*
* Be cautious when calling this function with greatly differing theta_in and theta_ref angles. The result will
* be erroneous because of floating point inaccuracies.
* \return         Normalized angle
* \ingroup angle
* \sdd{WI-13810}
*/
float32_T Normalize_Angle(
   const float32_T theta_in  /**< [in] input angle which should be shifted*/,
   const float32_T theta_ref /**< [in] reference angle*/
);
#ifdef __cplusplus
}
#endif
#endif
