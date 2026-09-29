#ifndef ML_Max_Sieve_T_H
#define ML_Max_Sieve_T_H
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
* \brief A sieve used to filter out a max value of a data set.
* \ingroup Sieve
* \sdd{WI-13577}
*/
typedef struct Max_Sieve_Tag
{
   float32_T known_max; /**< Current known max value */
} Max_Sieve_T;


#ifdef __cplusplus
}
#endif
#endif
