#ifndef FBK_FIELD_OF_INTEREST_H
#define FBK_FIELD_OF_INTEREST_H

/**
 * @file fbk_field_of_interest.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with definitions for the field of interest.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ml_float_range_t.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Macros
\*===========================================================================*/

/**
 * Defines the maximum allowed size of the field of interest.
 */
#define FBK_MAX_SIZE_OF_FOI (8u)

/**
 * Defines the most common size for the field of interest.
 */
#define FBK_FOI_SIZE_TETRAGON (4u)

/*===========================================================================*\
* Type definitions
\*===========================================================================*/

/**
 * @brief Defines a Polygon with a maximum defined number of polygon points
 * The polygon needs to be convex for further use as feature zone.
 */
typedef struct
{
   Vector_2d_T points[FBK_MAX_SIZE_OF_FOI]; /**< polygon points defining the FOI*/
   uint8_t size;                            /**< size of FOI*/
} Fbk_Field_Of_Interest_T;

/**
 * @brief Defines a bounding box that is aligned with VCS.
 */
typedef struct
{
   Float_Range_T x; /**< x or longitudinal value range */
   Float_Range_T y; /**< y or lateral value range */
} Fbk_Bounding_Box_T;

#endif /* FBK_FIELD_OF_INTEREST_H */
