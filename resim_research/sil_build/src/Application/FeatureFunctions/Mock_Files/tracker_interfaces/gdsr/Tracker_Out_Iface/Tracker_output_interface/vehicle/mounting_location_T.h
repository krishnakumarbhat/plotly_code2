/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef MOUNTING_LOCATION_T_H
#define MOUNTING_LOCATION_T_H

#include "mounting_position_set_T.h"
#include "mounting_lateral_T.h"
#include "mounting_longitudinal_T.h"

/**
 * \defgroup radar_mounting Radar Mounting Positions
 * The radar sensor mounting positions supported by the gdsr tracker.
 * \image html mounting_location_T.png Supported radar mounting positions
 * \image latex mounting_location_T.png Supported radar mounting positions
 * \ingroup Enumerations
 */

/**
 * All possible radar sensor mounting positions supported by the gdsr tracker.
 *
 * \section mounting_location_T_bits Description of the bits in a mounting position of mounting_location_T
 *
 * Bit 7        | Bit 6 | Bit 5   Bit 4   Bit 3 | Bit 2   Bit 1   Bit 0
 * -------------|-------|-----------------------|----------------------
 * position set | free  | longitudinal mounting | lateral mounting
 *
 *
 * \subsection mounting_location_T_bit7 Bit 7
 *  If this bit is set a mounting position described by mounting_location_t is defined and no default value
 * \subsection mounting_location_T_bit6 Bit 6
 * This bit isn't used right now. free for adding information
 * \subsection mounting_location_T_bit3_4_5 Bit 3-5
 * Bits describing the longitudinal mounting position. Are set with enum \ref mounting_longitudinal_T
 * mounting_longitudinal_T       | decimal | bit 3 | bit 4 | bit 5
 * ------------------------------|---------|-------|-------|-------
 * MOUNTING_LONGITUDINAL_NOT_SET | 0       | 0     | 0     | 0
 * MOUNTING_FORWARD              | 8       | 1     | 0     | 0
 * MOUNTING_SIDE                 | 16      | 0     | 1     | 0
 * MOUNTING_REAR                 | 24      | 1     | 1     | 0
 *
 * Bit 5 is a buffer if more longitudinal mounting positions are needed
 *
 * \subsection mounting_location_T_bit0_1_2 Bit 0-2
 * Bits describing the lateral mounting position. Are set with enum \ref mounting_lateral_T
 * mounting_lateral_T       | decimal | bit 0 | bit 1 | bit 2
 * -------------------------|---------|-------|-------|-------
 * MOUNTING_LATERAL_NOT_SET | 0       | 0     | 0     | 0
 * MOUNTING_LEFT            | 1       | 1     | 0     | 0
 * MOUNTING_CENTER          | 2       | 0     | 1     | 0
 * MOUNTING_RIGHT           | 3       | 1     | 1     | 0
 *
 * Bit 2 is a buffer if more lateral mounting positions are needed
 *
 * \ingroup radar_mounting
 */
typedef enum
{
   MOUNTING_NOT_SET        = (MOUNTING_POSITION_NOT_SET),                                  /**< mounting_lateral_T = MOUNTING_LATERAL_NOT_SET <br> mounting_longitudinal_T = MOUNTING_LONGITUDINAL_NOT_SET <br> mounting_position_set_T = MOUNTING_POSITION_NOT_SET*/
   MOUNTING_LEFT_FORWARD   = (MOUNTING_LEFT + MOUNTING_FORWARD + MOUNTING_POSITION_SET),   /**< mounting_lateral_T = MOUNTING_LEFT            <br>  mounting_longitudinal_T = MOUNTING_FORWARD             <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_LEFT_SIDE      = (MOUNTING_LEFT + MOUNTING_SIDE + MOUNTING_POSITION_SET),      /**< mounting_lateral_T = MOUNTING_LEFT            <br>  mounting_longitudinal_T = MOUNTING_SIDE                <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_LEFT_REAR      = (MOUNTING_LEFT + MOUNTING_REAR + MOUNTING_POSITION_SET),      /**< mounting_lateral_T = MOUNTING_LEFT            <br>  mounting_longitudinal_T = MOUNTING_REAR                <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_CENTER_FORWARD = (MOUNTING_CENTER + MOUNTING_FORWARD + MOUNTING_POSITION_SET), /**< mounting_lateral_T = MOUNTING_CENTER          <br>  mounting_longitudinal_T = MOUNTING_FORWARD             <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_CENTER_REAR    = (MOUNTING_CENTER + MOUNTING_REAR + MOUNTING_POSITION_SET),    /**< mounting_lateral_T = MOUNTING_CENTER          <br>  mounting_longitudinal_T = MOUNTING_REAR                <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_RIGHT_FORWARD  = (MOUNTING_RIGHT + MOUNTING_FORWARD + MOUNTING_POSITION_SET),  /**< mounting_lateral_T = MOUNTING_RIGHT           <br>  mounting_longitudinal_T = MOUNTING_FORWARD             <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_RIGHT_SIDE     = (MOUNTING_RIGHT + MOUNTING_SIDE + MOUNTING_POSITION_SET),     /**< mounting_lateral_T = MOUNTING_RIGHT           <br>  mounting_longitudinal_T = MOUNTING_SIDE                <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
   MOUNTING_RIGHT_REAR     = (MOUNTING_RIGHT + MOUNTING_REAR + MOUNTING_POSITION_SET)      /**< mounting_lateral_T = MOUNTING_RIGHT           <br>  mounting_longitudinal_T = MOUNTING_REAR                <br> mounting_position_set_T = MOUNTING_POSITION_SET*/
} mounting_location_T;

/**
 * Describes array indexes for each of the mounting_location_T.
 * Use \ref getMountLocArrayIndex() to convert from mounting_location_T to mounting_location_array_index_T.
 * Update mounting_location_array_index_T, whenever there is an update in mounting_location_T.
 * \ingroup radar_mounting
 */
typedef enum
{
   MOUNTING_LOCATION_ARRAY_INDEX_LEFT_FORWARD   = 0, /**< index of \ref MOUNTING_LEFT_FORWARD */
   MOUNTING_LOCATION_ARRAY_INDEX_LEFT_SIDE      = 1, /**< index of \ref MOUNTING_LEFT_SIDE */
   MOUNTING_LOCATION_ARRAY_INDEX_LEFT_REAR      = 2, /**< index of \ref MOUNTING_LEFT_REAR */
   MOUNTING_LOCATION_ARRAY_INDEX_CENTER_FORWARD = 3, /**< index of \ref MOUNTING_CENTER_FORWARD */
   MOUNTING_LOCATION_ARRAY_INDEX_CENTER_REAR    = 4, /**< index of \ref MOUNTING_CENTER_REAR */
   MOUNTING_LOCATION_ARRAY_INDEX_RIGHT_FORWARD  = 5, /**< index of \ref MOUNTING_RIGHT_FORWARD */
   MOUNTING_LOCATION_ARRAY_INDEX_RIGHT_SIDE     = 6, /**< index of \ref MOUNTING_RIGHT_SIDE */
   MOUNTING_LOCATION_ARRAY_INDEX_RIGHT_REAR     = 7, /**< index of \ref MOUNTING_RIGHT_REAR */
   MOUNTING_LOCATION_MAX_ARRAY_SIZE             = 8  /**< Number of mounting locations */
} mounting_location_array_index_T;

#endif

