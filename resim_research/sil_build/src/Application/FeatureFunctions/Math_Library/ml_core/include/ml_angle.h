#ifndef ML_ANGLE_H
#define ML_ANGLE_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


/***
* There is functionality that needs the structures Vector_2d_T and Angle_T.
* Because user expectations for where these functions can be found may differ
* these reside in a separate header st_vector_2d_angle.h
* Both st_vector_2d.h and st_angle.h include this combined header to ensure that
* users always find the corresponding functions.
*/
#include "ml_vector_2d_angle.h"
#include "ml_angle_t.h"
#include "ml_angle_normalize.h"
#ifdef __cplusplus
extern "C"
{
#endif


/**
* Creates an angle structure based upon an angle given in [rad](https:\\en.wikipedia.org/wiki/Radian)
* \return           Angle_T based upon an angle given in rad
* \sdd{WI-13809}
*/
Angle_T Create_Angle(const float32_T angle_rad /**<* [in] angle to create an Angle_T from */ );

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
* \sdd{WI-13807}
*/
void Normalize_Angle_Struct(
   Angle_T  *p_theta_in /**< [in, out] input angle which should be shifted*/,
   const Angle_T  *p_theta_ref /**< [in] reference angle*/);

/**
* Computes the difference between given two angles
* \return Difference between the given angles
* \ingroup angle
* \sdd{WI-13813}
*/
Angle_T Angle_Diff(
   const Angle_T * const p_angle_a, /**< [in] minuend */
   const Angle_T * const p_angle_b  /**< [in] subtrahend <*/);

/**
* Computes the mean value between given two angles
* \return Mean between the given angles
* \ingroup angle
* \sdd{WI-13812}
*/
Angle_T Angle_Mean(
   const Angle_T * const p_angle_a, /**<  [in] first angle */
   const Angle_T * const p_angle_b  /**<  [in] second angle */);

#ifdef __cplusplus
}
#endif
#endif
