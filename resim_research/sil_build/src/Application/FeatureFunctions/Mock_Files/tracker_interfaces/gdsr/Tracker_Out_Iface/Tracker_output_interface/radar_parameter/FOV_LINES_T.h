/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef FOV_LINES_T_H
#define FOV_LINES_T_H

#include "line_hesse_type.h"

/**
 * This structure is used to hold the FOV lines of a sensor
 */
typedef struct
{
   Line_Hesse_T fov_min_az; /**< hesse normal form of the minimum azimuth FOV line */
   Line_Hesse_T fov_max_az; /**< hesse normal form of the maximum azimuth FOV line */
   float min_az;            /**< [rad] angle of the minimum azimuth FOV line */
   float max_az;            /**< [rad] angle of the maximum azimuth FOV line */
} FOV_LINES_T;

#endif
