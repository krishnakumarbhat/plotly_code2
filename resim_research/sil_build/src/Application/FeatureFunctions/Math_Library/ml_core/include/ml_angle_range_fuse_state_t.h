#ifndef ML_ANGLE_RANGE_FUSE_STATE_T_H
#define ML_ANGLE_RANGE_FUSE_STATE_T_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

/**
* Return type of Angle_Range_Fuse().
* \ingroup angle_range
*/
typedef struct Angle_Range_Fuse_State_Tag
{
   boolean_T f_success;      /**< TRUE if the requested merge could be performed */
   boolean_T f_two_pi;       /**< TRUE if the resulting Angle_Range_T spans full 2 pi */
   boolean_T f_start_from_a; /**< TRUE if the resulting Angle_Range_T contains the start value from p_range_a */
   boolean_T f_start_from_b; /**< TRUE if the resulting Angle_Range_T contains the start value from p_range_b */
   boolean_T f_end_from_a;   /**< TRUE if the resulting Angle_Range_T contains the end value from p_range_a */
   boolean_T f_end_from_b;   /**< TRUE if the resulting Angle_Range_T contains the end value from p_range_b */
}Angle_Range_Fuse_State_T;


#ifdef __cplusplus
}
#endif
#endif
