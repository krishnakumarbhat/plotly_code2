#ifndef ML_MATRIX_3X3_T_H
#define ML_MATRIX_3X3_T_H
/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#ifdef __cplusplus
namespace ml {
   extern "C"
   {
#endif

#include "reuse.h"

/**
 * A Matrix with the dimension \f$ |R^{3 x 3} \f$. See [Wikipedia: Matrix_(mathematics)](https:\\en.wikipedia.org/wiki/Matrix_(mathematics))
 * \ingroup Matrix
 */
typedef struct Matrix_3X3_Tag
{
   float32_T elements[3][3];
} Matrix_3X3_T;
#ifdef __cplusplus
}
}
/**
* \ingroup Matrix
*/
typedef ml::Matrix_3X3_T Matrix_3X3_T;
#endif
#endif
