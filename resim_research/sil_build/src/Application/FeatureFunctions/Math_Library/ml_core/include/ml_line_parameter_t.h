#ifndef ML_LINE_PARAMETER_T_H
#define ML_LINE_PARAMETER_T_H
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
 * \defgroup line_parameter Line in parameter form
 * \ingroup line
 */

/**
* Defines a line in the parameter form
* \ingroup line_parameter
*/
typedef struct Line_Parameter_Tag {
   Vector_2d_T p0;
   Vector_2d_T direction;
} Line_Parameter_T;

#ifdef __cplusplus
}
#endif
#endif
