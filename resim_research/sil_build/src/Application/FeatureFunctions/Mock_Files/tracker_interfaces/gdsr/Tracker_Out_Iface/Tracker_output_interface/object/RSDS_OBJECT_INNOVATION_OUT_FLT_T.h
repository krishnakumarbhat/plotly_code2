/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef RSDS_OBJECT_INNOVATION_OUT_FLT_T_H
#define RSDS_OBJECT_INNOVATION_OUT_FLT_T_H

#include "reuse.h"
#include "Vector_2d.h"

/**
 * The filtered difference between predicted and measured state of the object.
 * Will get bigger if the object does not follow the prediction.
 * position, velocity, acceleration and size do have their own filter.
 * Heading and heading_rate are the square root of the diagonal entries in the Kalman covariance matrix.
 */
typedef struct
{
   Vector_2d_T position;     /**< [m] position innovation */
   Vector_2d_T velocity;     /**< [m/s] velocity innovation */
   Vector_2d_T acceleration; /**< [m/s^2] acceleration innovation */
   Vector_2d_T size;         /**< [m] size innovation */
   float32_T heading;        /**< [rad] heading innovation */
   float32_T heading_rate;   /**< [rad/s] heading rate innovation */
} RSDS_OBJECT_INNOVATION_OUT_FLT_T;

#endif
