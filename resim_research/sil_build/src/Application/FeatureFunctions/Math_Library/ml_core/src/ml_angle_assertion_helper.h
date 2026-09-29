#ifndef ML_ANGLE_ASSERTION_HELPER_H
#define ML_ANGLE_ASSERTION_HELPER_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_math.h"

#define Angle_Is_Not_Nan(angle_input) (Is_Not_Nan((angle_input)->angle) && Is_Not_Nan((angle_input)->sin) && Is_Not_Nan((angle_input)->cos)) /* PRQA S 3453 */ /* Macro is used for assertions */

#endif
