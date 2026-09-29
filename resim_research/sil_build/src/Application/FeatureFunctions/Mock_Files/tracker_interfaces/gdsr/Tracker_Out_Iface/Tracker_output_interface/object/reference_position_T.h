/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef REFERENCE_POSITION_T_H
#define REFERENCE_POSITION_T_H

/**
* \defgroup reference_position_t_h Reference position
 * Points on the corners of a tracked object.
 * \ingroup Enumerations
 */
 
/**
 * Points on the corners of a tracked object.
 * \ingroup reference_position_t_h
 */
typedef enum
{
   REFERENCE_POSITION_FRONT_LEFT  = (0), /**< 0*/
   REFERENCE_POSITION_FRONT       = (1), /**< 1*/
   REFERENCE_POSITION_FRONT_RIGHT = (2), /**< 2*/
   REFERENCE_POSITION_RIGHT       = (3), /**< 3*/
   REFERENCE_POSITION_REAR_RIGHT  = (4), /**< 4*/
   REFERENCE_POSITION_REAR        = (5), /**< 5*/
   REFERENCE_POSITION_REAR_LEFT   = (6), /**< 6*/
   REFERENCE_POSITION_LEFT        = (7), /**< 7*/
   REFERENCE_POSITION_INVALID     = (8)  /**< 8*/
} reference_position_T;

/**
 * Number of valid reference points.
 * \ingroup reference_position_t_h
 */
#define NUMBER_OF_REFERENCE_POINTS    (REFERENCE_POSITION_INVALID)

#endif

