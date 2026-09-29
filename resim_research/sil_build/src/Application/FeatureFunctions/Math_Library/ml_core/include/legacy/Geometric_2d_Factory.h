#ifndef GEOMETRIC_2D_FACTORY_H
#define GEOMETRIC_2D_FACTORY_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "st_vector_2d_t.h"
#include "Geometric_2d_Structs.h"
#include "st_line_segment.h"
#include "st_line_parameter.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

#define Create_Line_Segment(a, b) (Create_Line_Segment_Fom_Points(&(a),&(b)))

/**
* Returns a Line_Normal_T defined by the given two points
* \return         a Line_Normal_T defined by the given two points
* \ingroup line_normal
* This functionality has been deprecated! Use Line_Hesse_Create() in \ref line_hesse instead!
*/
Line_Normal_T Create_Line_Normal(
   const Vector_2d_T *p_p0, /**< First point on the line */
   const Vector_2d_T *p_p1 /**< Second point on the line */);

#ifdef __cplusplus
}
#endif
#endif

