#ifndef ML_MIN_Max_Sieve_T_H
#define ML_MIN_Max_Sieve_T_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include "ml_max_sieve_t.h"
#include "ml_min_sieve_t.h"

/**
* \brief A sieve used to filter out a min as well as a max value of a data set.
* \ingroup Sieve
* \sdd{WI-13573}
*/
typedef struct Min_Max_Sieve_Tag
{
   Max_Sieve_T max_sieve; /**< Current known max value */
   Min_Sieve_T min_sieve; /**< Current known min value */
} Min_Max_Sieve_T;

#ifdef __cplusplus
}
#endif
#endif
