/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/

#ifndef SPACE_TIME_AND_DERIVATIVE_2D
#define SPACE_TIME_AND_DERIVATIVE_2D

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "Vector_2d.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * An objects position, velocity, acceleration, heading
 * \ingroup Tracker_Output_IteratorFlex Tracker_Output_Iterator
 */
typedef struct Space_Time_Derivative_Vector_2d_Tag
{
   Vector_2d_T position;          /**< [m] Center position    */
   Vector_2d_T velocity;          /** [m/s] velocity          */
   Vector_2d_T relative_velocity; /** [m/s] relative velocity */
   Vector_2d_T acceleration;      /**< [m/s^2] acceleration   */
   float32_T heading;             /**< [Rad] heading          */
} Space_Time_Derivative_Vector_2d_T;

#endif

