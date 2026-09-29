/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef EXCLUSION_ZONE_T_H
#define EXCLUSION_ZONE_T_H

#include "tracker_constants_selector.h"
#include "reuse.h"

/**
* Exclusion Zone information for a single sensor.
* \ingroup ac_exclusion_zone
*/
typedef struct
{
   boolean_T f_is_in_other_sensor_fov[NUMBER_OF_SENSORS]; /**< if true, sensor overlapps with other sensors fov */
   boolean_T f_exclusion_zone_at_min_az;                  /**< one exclusion zone is at the fov border boresight - fov_max_azimuth */
   boolean_T f_exclusion_zone_at_max_az;                  /**< one exclusion zone is at the fov border boresight + fov_max_azimuth */
   float     max_abs_azimuth_without_exclusion_zone;      /**< [rad] sensors maximum absolute non-exclusion azimuth angle  */
} Exclusion_Zone_T;

#endif
