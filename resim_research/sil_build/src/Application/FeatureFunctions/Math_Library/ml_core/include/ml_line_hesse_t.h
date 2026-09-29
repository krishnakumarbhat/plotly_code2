/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#ifndef ML_LINE_HESSE_T_H
#define ML_LINE_HESSE_T_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "reuse.h"
#include "ml_vector_2d_t.h"

/**
 * This structure is used to hold the normal vector, \f$\vec{n}\f$, and distance, \f$d\f$, of the normal vector from origin.
 * If a point \f$\vec{p}\f$, lies on the line then \f$\vec{p} \cdot \vec{n} - d = 0\f$.
 *  \ingroup line_hesse
 * \sa line_hesse
 */
typedef struct Line_Hesse_Tag
{
   Vector_2d_T norm_vector;        /**< normal vector, \f$\vec{n}\f$ */
   float32_T   distance_to_origin; /**< Distance between line and origin, \f$d\f$ */
} Line_Hesse_T;

/**
 * Depending on the normal direction of a line in Hesse normal form a given point must fall in one
 * of these enumerations.
 * \ingroup line_hesse
 * \sa line_hesse
 */
typedef enum Side_Of_Line_Hesse_Tag
{
   LINE_HESSE_SIDE_POSITIVE, /**< Point is on the side of the line the normal vector points to */
   LINE_HESSE_SIDE_NEGATIVE, /**< Point is on the side of the line the normal vector does point away from */
   LINE_HESSE_SIDE_ON_LINE   /**< Point is on the line */
} Side_Of_Line_Hesse_T;

#ifdef __cplusplus
}
#endif
#endif

