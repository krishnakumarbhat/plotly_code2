#ifndef ML_LINE_PARAMETER_H
#define ML_LINE_PARAMETER_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_line_parameter_t.h"
#include "ml_vector_2d_t.h"


/**
* Returns a Line_Parameter_T defined by the given two points
* \return         Line_Parameter_T  defined by the given two points
* \ingroup line_parameter
* \sdd{WI-13968}
*/
Line_Parameter_T Create_Line_Parameter_Form(
   const Vector_2d_T * const p_point_a, /**< [in] First point on the line */
   const Vector_2d_T * const p_point_b /**< [in] Second point on the line */
);

/**
* Returns the y value of p_line at position x
* \return the y value of p_line at position x
* \ingroup line_parameter
* \sdd{WI-13971}
*/
float32_T Get_Y_Value_From_Line(
   const Line_Parameter_T *const p_line,  /**< [in] Line to compute a value for */
   const float32_T         x              /**< [in] x value to get the y value of the given line for*/
);


#ifdef __cplusplus
}
#endif
#endif
