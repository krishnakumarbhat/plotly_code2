#ifndef ESA_TYPES_H
#define ESA_TYPES_H

/**
 * @file esa_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the ESA specific data types.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/**
 * @brief Number of points in a zone definition
 *
 * @SDD{CSCSA-65947}
 * @verification{}
 */
#define ESA_NUMBER_OF_ZONE_POINTS (6u)

/**
 * @brief Large value indicating that the object is located at infinity behind the host
 *
 * @SDD{CSCSA-65943}
 * @verification{}
 */
#define ESA_DEFAULT_OBJ_DIST ((float32_T) -1000.0f)

/**
 * @brief TTC value indicating that object is located at infinity behind host
 *
 * @SDD{CSCSA-65944}
 * @verification{}
 */
#define ESA_DEFAULT_LARGE_TTC (100.0f)

/*============================================================================*\
 * EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Esa_Zone_Points_T summarizes zone point indices.
 *
 * @SDD{CSCSA-65946}
 */
typedef enum
{
   ESA_FRONT_OUTER_SIDE = (0), /**< Zone point index for side far away from host and most frontal point pair*/
   ESA_REAR_OUTER_SIDE  = (ESA_NUMBER_OF_ZONE_POINTS / 2u - 1u), /**< Zone point index for side far away from host and rear point
                                                                    pair*/
   ESA_REAR_EGO_SIDE  = (ESA_NUMBER_OF_ZONE_POINTS / 2u),        /**< Zone point index for side near to host and rear point pair*/
   ESA_FRONT_EGO_SIDE = (ESA_NUMBER_OF_ZONE_POINTS - 1u) /**< Zone point index for side near to host and most frontal point pair*/
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Esa_Zone_Points_T;


/**
 * @brief Coordinate_System_T lists both available coordinate systems (VCS and curvi).
 *
 * @SDD{CSCSA-65948}
 */
typedef enum
{
   ESA_USE_VCS   = (0), /**< Use Vehicle Coordinate System (VCS) */
   ESA_USE_CURVI = (1)  /**< Use curvi coordinate system */

   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Coordinate_System_T;


/**
 * @brief ESA object
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65125}
 * @verification{}
 */
typedef struct
{
   Fbk_Field_Of_Interest_T zone; /**< Object specific Esa zone */

   float32_T long_ttc; /**< longitduninal TTC (time until targets front bumper collides with egos rear bumper; no acceleration
                          considered) */
   float32_T ttp;      /**< Time To Pass:  */
   float32_T obj_decel_to_reach_host_speed; /**< Object deceleration required to reach host speed */
   float32_T obj_long_dist;                 /**< Object longitudinal distance to the Host */

   uint8_t ego_side; /**< side where object is located*/

   boolean_T f_obj_in_zone;               /**< flag indicating whether object is in zone */
   boolean_T f_obj_ttc_below_threshold;   /**< flag indicating whether objects ttc is below the respective threshold */
   boolean_T f_obj_decel_above_threshold; /**< flag indicating whether objects needed deceleration to return to host speed is above
                                             the respective threshold */

   const Fbk_Object_Data_T *p_tracker_data; /**< Pointer to objects tracker properties */

} Esa_Object_T;


/**
 * @brief Esa_Trailer_Object_T summarizes object properties of a trailer attached to the host vehicle.
 *
 * @SDD{CSCSA-65128}
 */
typedef struct
{
   boolean_T f_trailer_present; /**< Flag indicating whether a trailer is attached to the host vehicle */

   float32_T length; /**< [m] Length of the trailer (Back edge of the trailer will be host length plus trailer length when going
                        straight) */
   float32_T width;  /**< [m] Width of the trailer */
   float32_T angle;  /**< [rad] Angle of the trailer */

} Esa_Trailer_Object_T;

#endif
