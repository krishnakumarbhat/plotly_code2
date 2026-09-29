#ifndef SIEVE_H
#define SIEVE_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "st_sieve.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define init_max_sieve(a) (Init_Max_Sieve(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define init_min_sieve(a) (Init_Min_Sieve(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define init_min_max_sieve(a) (Init_Min_Max_Sieve(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define init_max_sieve_set(a, b) (Init_Max_Sieve_Set(a, b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define init_min_sieve_set(a, b) (Init_Min_Sieve_Set(a, b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define init_min_max_sieve_set(a,b) (Init_Min_Max_Sieve_Set(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define max_sieve(a,b) (Max_Sieve(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define min_sieve(a,b) (Min_Sieve(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define min_max_sieve(a,b) (Min_Max_Sieve(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define min_max_sieve_set(a,b,c) (Min_Max_Sieve_Set(a,b,c))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define max_sieve_set(a,b,c) (Max_Sieve_Set(a,b,c))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define min_sieve_set(a,b,c) (Min_Sieve_Set(a,b,c))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_max_sieve_content(a) (Get_Max_Sieve_Content(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_min_sieve_content(a) (Get_Min_Sieve_Content(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_min_max_sieve_min_content(a,b) (Get_Min_Max_Sieve_Min_Content(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_min_max_sieve_max_content(a,b) (Get_Min_Max_Sieve_Max_Content(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_max_from_max_sieve_set(a) (Get_Max_From_Max_Sieve_Set(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_min_from_min_sieve_set(a) (Get_Min_From_Min_Sieve_Set(a))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_max_sieve_set_content_at_index(a,b) (Get_Max_Sieve_Set_Content_At_Index(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define get_min_sieve_set_content_at_index(a,b) (Get_Min_Sieve_Set_Content_At_Index(a,b))

/**
* Macro only present for backwards compatibility. Call function in st_sieve.h directly.
* \ingroup Sieve
*/
#define is_sieved_value_valid(a) (Is_Sieved_Value_Valid(a))

/**
* Used for backwards compatibility. Use Max_Sieve_T instead.
* \ingroup Sieve
*/
typedef struct Max_Sieve_Tag MAX_SIEVE_T;

/**
* Used for backwards compatibility. Use Min_Max_Sieve_T instead.
* \ingroup Sieve
*/
typedef struct Min_Max_Sieve_Tag MIN_MAX_SIEVE_T;

/**
* Used for backwards compatibility. Use Min_Sieve_T instead.
* \ingroup Sieve
*/
typedef struct Min_Sieve_Tag MIN_SIEVE_T;

#ifdef __cplusplus
}
#endif
#endif