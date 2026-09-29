/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include "ml_vector_2d_assertion_helper.h"
#include <assert.h>
#include "ml_line_segment.h"
#include "ml_math.h"

Line_Segment_T Create_Line_Segment_Fom_Points(
   const Vector_2d_T *p_start_pt,
   const Vector_2d_T *p_end_pt)
{
   Line_Segment_T ret_seg;

   /* Check if any of the parameters are NaN */
   assert(Vector_Is_Not_Nan(p_start_pt));
   assert(Vector_Is_Not_Nan(p_end_pt));

   assert((p_start_pt->x != p_end_pt->x) || (p_start_pt->y != p_end_pt->y));

   ret_seg.p0 = *p_start_pt;
   ret_seg.p1 = *p_end_pt;

   return ret_seg;
}
