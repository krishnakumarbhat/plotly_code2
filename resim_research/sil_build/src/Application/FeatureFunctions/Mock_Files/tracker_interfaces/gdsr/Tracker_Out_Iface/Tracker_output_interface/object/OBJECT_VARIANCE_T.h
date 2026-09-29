/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OBJECT_VARIANCE_T_H
#define OBJECT_VARIANCE_T_H

#include "reuse.h"
#include "Vector_2d.h"

/**
 * Variance derived from the raw detections and the objects updated state.
 */

typedef struct
{
   float32_T velocities_filtered; /**< [m^2/s^2] covariance between longitudinal and lateral velocities, filtered */
   float32_T positions_filtered;  /**< [m^2] covariance between longitudinal and lateral positions, filtered */
} OBJECT_COVARIANCE_T;

typedef struct
{
   Vector_2d_T velocity_filtered;   /**< [m^2/s^2] Velocity variance, filtered.
                                         Computes the associated detections variance according to the velocity vector of the object   */
   Vector_2d_T position_filtered;   /**< [m^2] Position variance, filtered.
                                         Variance of the detection closest to the reference point.*/
   Vector_2d_T size_filtered;       /**< [m^2] Size variance, filtered.
                                         Derived using measured bounding box and predicted/assumed size as raw data, corrected by some
                                       factors */
   OBJECT_COVARIANCE_T covariances; /**< Covariances structure, filtered */
   float32_T heading;               /**< [rad^2] Heading variance.
                                         Derived using a variance propagation of the velocity variance and the objects updated velocity. */
} OBJECT_VARIANCE_T;


#endif
