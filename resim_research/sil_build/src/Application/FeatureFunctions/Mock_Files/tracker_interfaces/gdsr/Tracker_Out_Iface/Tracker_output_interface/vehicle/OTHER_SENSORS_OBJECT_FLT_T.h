/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OTHER_SENSORS_OBJECT_FLT_T_H
#define OTHER_SENSORS_OBJECT_FLT_T_H

#include "reuse.h"
#include "mounting_location_T.h"

/**
 * \defgroup object_sharing Deprecated Functionality Object Sharing
 * This functionality is deprecated. The GDSR tracker does no longer read from the structures described in this group.
 * \ingroup host_vehicle
 */

/**
 * This functionality is deprecated.
 * \ingroup object_sharing
 */
#define NUMBER_OF_TRANSMITTED_OBJECTS    (6)

/**
 * This functionality is deprecated. The GDSR tracker does no longer read from this structure.
 * \ingroup object_sharing
 */
typedef struct
{
   float32_T speed;     /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
   float32_T heading;   /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
   float32_T pos_lat;   /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
   float32_T pos_long;  /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
   float32_T timestamp; /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
   uint8_t   valid;     /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
} OTHER_SENSORS_OBJECT_FLT_T;

/**
 * This functionality is deprecated. The GDSR tracker does no longer read from this structure.
 * \ingroup object_sharing
 */
typedef struct
{
   OTHER_SENSORS_OBJECT_FLT_T tracked_objects[NUMBER_OF_TRANSMITTED_OBJECTS]; /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
   mounting_location_T        sensor_mount_loc;                               /**< This functionality is deprecated. The GDSR tracker does no longer read from this variable. */
} OTHER_SENSOR_RECEIVED_DATA_FLT_T;

#endif

