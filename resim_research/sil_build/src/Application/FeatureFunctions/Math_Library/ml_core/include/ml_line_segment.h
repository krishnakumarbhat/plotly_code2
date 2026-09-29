#ifndef ML_LINE_SEGMENT_H
#define ML_LINE_SEGMENT_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_line_segment_t.h"
#include "ml_vector_2d_t.h"

/**
* A line segment has a start and end point. The start and end points
* are defined by 2d vectors.
* This function takes the given start and given end points and returns a
* line segment type between the given 2 points.
* \throws Assertion if either x or y of the start and end points are NaN
* \return         line segment type between given 2 points
* \ingroup line_segment
* \sdd{WI-13961}
*/
Line_Segment_T Create_Line_Segment_Fom_Points(
   const Vector_2d_T *p_start_pt, /**< [in] Start point defining a line segment */
   const Vector_2d_T *p_end_pt /**< [in] End point defining a line segment */
);

#ifdef __cplusplus
}
#endif
#endif
