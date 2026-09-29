/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef CURVI_COORDINATES_CALC_METHOD_T_H
#define CURVI_COORDINATES_CALC_METHOD_T_H

typedef enum
{
   CURVI_COORDINATES_UNKNOWN                  = (0), /**< default value */
   CURVI_COORDINATES_BASED_ON_VCS             = (1), /**< neither snail train nor distance based curvature is used. Curvi coordinates are same as vcs coordinates*/
   CURVI_COORDINATES_SNAIL_TRAIL              = (2), /**< nearest snail trail point is used  */
   CURVI_COORDINATES_DISTANCE_BASED_CURVATURE = (3)  /**< distance based curvature is used*/
} curvi_coordinates_calc_method_T;

#endif

