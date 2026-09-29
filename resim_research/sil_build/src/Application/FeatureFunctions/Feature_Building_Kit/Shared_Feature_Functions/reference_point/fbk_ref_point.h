#ifndef FBK_REF_POINT_H
#define FBK_REF_POINT_H

/**
 * @file fbk_ref_point.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with reference point definitions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "ml_vector_2d_t.h"

/**
 * Simplified object corner indexing
 */
#define FBK_FOI_FRONT_LEFT_CORNER 0u  /**< Index of the front left corner*/
#define FBK_FOI_FRONT_RIGHT_CORNER 1u /**< Index of the front right corner*/
#define FBK_FOI_REAR_RIGHT_CORNER 2u  /**< Index of the rear right corner*/
#define FBK_FOI_REAR_LEFT_CORNER 3u   /**< Index of the rear left corner*/

/*===========================================================================*\
* Enums
\*===========================================================================*/

/**
 * Used for corner indexing of object
 */
typedef enum
{
   FBK_FRONT_LEFT_CORNER     = (0), /**< Index of the front left corner*/
   FBK_FRONT_MID             = (1), /**< Index of the front midst point*/
   FBK_FRONT_RIGHT_CORNER    = (2), /**< Index of the front right corner*/
   FBK_RIGHT_MID             = (3), /**< Index of the right midst point*/
   FBK_REAR_RIGHT_CORNER     = (4), /**< Index of the rear right corner*/
   FBK_REAR_MID              = (5), /**< Index of the rear midst point*/
   FBK_REAR_LEFT_CORNER      = (6), /**< Index of the rear left corner*/
   FBK_LEFT_MID              = (7), /**< Index of the left midst point*/
   FBK_NUM_OF_OBJECT_CORNERS = (8)  /**< Total amount of corners*/
} Fbk_Reference_Position_T;

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * Summarizes the object corners as well as the midpoint of the non diagonal lines between them
 */
typedef struct
{
   Vector_2d_T points[FBK_NUM_OF_OBJECT_CORNERS]; /**< represents one corner- or midpoint of the target*/
} Fbk_Object_Corners_T;

/**
 * Defines the reference point of a target.
 */
typedef struct
{
   Vector_2d_T point;                        /**< represents one corner- or midpoint of the target*/
   float32_T distance;                       /**< distance to host */
   Fbk_Reference_Position_T ref_point_index; /**< corner which represents the reference point*/
} Fbk_Ref_Point_T;
#endif /* FBK_REF_POINT_H */
