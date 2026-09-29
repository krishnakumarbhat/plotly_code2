/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef RSDS_OBJECT_ACCURACY_FLT_T_H
#define RSDS_OBJECT_ACCURACY_FLT_T_H

#include "reuse.h"
#include "Vector_2d.h"

/**
 * The accuracy shall describe how close the tracked object is to the real object.
 * Since the 'real' object is not known to the tracker the accuracy can only be estimated.
 * The estimation of accuracy is based on the variance and penalty factors based on estimated
 * errors depending on e.g. azimuth in sensor FOV.
 */
typedef struct
{
   Vector_2d_T position;     /**< [m] position accuracy */
   Vector_2d_T velocity;     /**< [m/s] velocity accuracy */
   Vector_2d_T acceleration; /**< [m/s^2] acceleration accuracy */
   Vector_2d_T size;         /**< [m] size accuracy */
   float32_T heading;        /**< [rad] heading accuracy */
   float32_T heading_rate;   /**< [rad/s] heading rate accuracy */
} RSDS_OBJECT_ACCURACY_FLT_T;

#endif
