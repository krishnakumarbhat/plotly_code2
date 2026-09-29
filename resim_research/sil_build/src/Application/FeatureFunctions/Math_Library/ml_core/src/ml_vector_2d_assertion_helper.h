#ifndef ML_VECTOR_2D_ASSERTION_HELPER_H
#define ML_VECTOR_2D_ASSERTION_HELPER_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_math.h"

#define Vector_Is_Not_Nan(vector) (Is_Not_Nan((vector)->x) && Is_Not_Nan((vector)->y)) /* PRQA S 3453 */ /* Macro is used for assertions */

#endif
