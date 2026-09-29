#ifndef LOOKUPTABLE_H
#define LOOKUPTABLE_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "st_lookup_table_2d.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* Macro only present for backwards compatibility. Call function in st_lookup_table_2d.h directly.
* \ingroup Sieve
*/
#define GetValueFrom2dLookuptable(a,b,c,d) (Get_Value_From_2d_Lookup_Table(a,b,c,d))

#ifdef __cplusplus
}
#endif
#endif
