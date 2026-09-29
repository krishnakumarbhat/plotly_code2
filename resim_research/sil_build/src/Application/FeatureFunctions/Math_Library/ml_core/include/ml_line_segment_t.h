#ifndef ML_LINE_SEGMENT_T_H
#define ML_LINE_SEGMENT_T_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_vector_2d_t.h"


/**
 * \defgroup line_segment Line segment
 * \ingroup line
 */

/**
* \brief Defines a line segment that is in between the points p0 and p1
* \ingroup line_segment
*/
typedef struct Line_Segment_Tag {
   Vector_2d_T p0; /**< Start point of line segment */
   Vector_2d_T p1; /**< End point of line segment */
} Line_Segment_T;

#ifdef __cplusplus
}
#endif
#endif
