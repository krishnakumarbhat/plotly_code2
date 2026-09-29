#ifndef GEOMETRIC_STRUCTS_2D_H
#define GEOMETRIC_STRUCTS_2D_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include "st_vector_2d_t.h"

/**
 * \brief Holds four points that together define a rectangle.
 * \ingroup polygon
 */
typedef struct Rectangle_Tag
{
   Vector_2d_T points[4]; /**< An array of four points defining a rectangle */
} Rectangle_T;

/**
* \brief A tetragon consisting of 4 Points
* \ingroup polygon
*/
typedef struct Tetragon_Tag
{
   Vector_2d_T corners[4]; /**< An array of four points defining a tetragon */
} Tetragon_T;

/**
 * \brief Defines a ray that starts at point p0 and goes to infinity in the given direction
 * \ingroup polygon
 */
typedef struct Ray_Parameter_Tag{
   Vector_2d_T start; /**< Start of ray */
   Vector_2d_T direction; /**< Direction of ray */
} Ray_Parameter_T;

#include "st_line_parameter_t.h"

#include "st_line_segment_t.h"

typedef struct Line_Segment_Tag Segment_T;

/**
* \brief Defines a line in the normal form aka the direction is defined as being normal to the vector n
* \ingroup line_normal
* This functionality has been deprecated! Use Line_Hesse_T in \ref line_hesse instead!
*/
typedef struct Line_Normal_Tag {
   Vector_2d_T p0; /**< Point on the line */
   Vector_2d_T n; /**< normal vector of line */
} Line_Normal_T;

/**
* \brief Defines the sides a point can be relative to a given line (the line has a direction)
* \ingroup line_normal
* This functionality has been deprecated! Use Side_Of_Line_Hesse_T in \ref line_hesse instead!
*/
typedef enum
{
   LINE_SIDE_LEFT = -1, /**< Left of line */
   LINE_SIDE_RIGHT = 1, /**< Right of line */
   LINE_SIDE_ONLINE = 0 /**< On the line */
}
Line_Side_T;

#ifdef __cplusplus
}
#endif
#endif
