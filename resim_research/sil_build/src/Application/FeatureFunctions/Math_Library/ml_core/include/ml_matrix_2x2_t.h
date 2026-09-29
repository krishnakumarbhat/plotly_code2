#ifndef ML_MATRIX_2X2_T_H
#define ML_MATRIX_2X2_T_H

#include "reuse.h"
/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#ifdef __cplusplus
namespace ml {
   extern "C"
   {
#endif
/**
 * A Matrix with the dimension \f$ |R^{2 x 2} \f$. See [Wikipedia: Matrix_(mathematics)](https:\\en.wikipedia.org/wiki/Matrix_(mathematics))
 * \ingroup Matrix
 */
typedef struct Matrix_2X2_Tag
{
   float32_T elements[2][2];
} Matrix_2X2_T;
#ifdef __cplusplus
}
}
/**
* \ingroup Matrix
*/
typedef ml::Matrix_2X2_T Matrix_2X2_T;
#endif
#endif
