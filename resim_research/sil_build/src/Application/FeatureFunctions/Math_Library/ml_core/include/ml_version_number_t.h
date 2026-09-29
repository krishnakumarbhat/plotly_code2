#ifndef ML_VERSION_NUMBER_T_H
#define ML_VERSION_NUMBER_T_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

#ifdef _MSC_VER
#pragma warning(disable:4214)
#endif

/* Since a requirement for this structure is to be only two bytes wide using bitfield is the only option to squeeze the content in */
/* PRQA S 635 EOF */

/**
* \brief Version number to be used by software modules and AS Process Tools
*
* \ingroup versioning
*/
typedef struct Version_Number_Tag
{
   uint16_t year : 5; /**< Year of the release */ /* PRQA S 0635 */ /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
   uint16_t month : 4; /**< Month of the release */ /* PRQA S 0635 */ /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
   uint16_t day : 5; /**< Day of the release */ /* PRQA S 0635 */ /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
   uint16_t iteration : 2; /**< If several versions are released on the same day the iteration is changed */ /* PRQA S 0635 */ /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
} Version_Number_T;

#ifdef __cplusplus
}
#endif
#endif

