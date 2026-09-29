/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#ifndef LINE_HESSE_H
#define LINE_HESSE_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "st_line_hesse.h"
#include "Geometric_2d_Structs.h"

/**
*  Returns a Hesse normal form equivalent to given line in normal form
* \return         Line_Hesse_T line
* \ingroup line_hesse
* \sdd{WI-13962}
* \sdd{WI-13977}
*/
Line_Hesse_T Line_Hesse_Create_Using_Line_Normal(const Line_Normal_T *p_line_normal /**< [in] Line in normal form to create a line in Hesse normal form from */);

#ifdef __cplusplus
}
#endif
#endif

