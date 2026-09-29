#ifndef REUSE_H
#define REUSE_H
/*===================================================================*\
* Copyright 2016, Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential.
*--------------------------------------------------------------------
*
* Description: Defines all standard types and common macros.
*
* Applicable Standards (in order of precedence: highest first):
*
* Deviations from Delphi C Coding standards: None
*
*
\*===================================================================*/
/*===========================================================================*\
* Includes
\*===========================================================================*/
#include <stdint.h>
#include <stddef.h>
/*************************************************************************
* Standard Integer Types
*
* Use the following standard integer types for new development per
* Delphi C coding standards section 4.3.5.1.
*
* These are already defined in stdint.h.
*
* uint8_t
* uint16_t
* uint32_t
* uint64_t
* int8_t
* int16_t
* int32_t
* int64_t
**************************************************************************/


/*************************************************************************
* Standard Boolean Type
*
* Use the following standard boolean type for new development per
* Delphi C coding standards section 4.3.5.1.
*
* These are already defined in stdbool.h.
*
* #define false 0
* #define true 1
**************************************************************************/
typedef unsigned char boolean_T;      /* Forces boolean to be 1 byte */

/*************************************************************************
*                                                                        *
* Rules for using boolean_T (to be checked at code reviews):             *
* 1) boolean_T declared variables shall be named such that a state is    *
*    conveyed. Examples: lamp_is_on,  test_passed,  pin_is_high.  So     *
*    that if( lamp_is_on ) is unambiguous.                               *
* 2) boolean_T variables shall only be compared by themselves.           *
*    Never do this:  if( TRUE==lamp_is_on)  Do this: if( lamp_is_on )    *
* 3) boolean_T variables shall not be compared to each other.            *
*    Never do this:  if ( lamp_is_on == time_for_light )                 *
* 4) boolean_T variables can be assigned TRUE or FALSE                   *
*    Can do this:  lamp_is_on = TRUE;                                    *
*                                                                        *
*************************************************************************/


/**************************************************************************
* Floating Point types
***************************************************************************/
typedef float float32_T;
#ifdef AS_NON_CLEAN_TYPES
/* Remove float_tracker_T as soon as not needed anymore */
typedef float float_tracker_T;
typedef double float64_T;
#endif
/**************************************************************************
* Integer Point types
***************************************************************************/
#ifdef AS_NON_CLEAN_TYPES
typedef unsigned char uint8_T;
typedef unsigned short uint16_T;
typedef unsigned int uint32_T;
typedef unsigned long long uint64_T;
typedef char int8_T;
typedef short int16_T;
typedef int int32_T;
typedef long long int64_T;

/* Needed for calibration */
typedef unsigned short u16p0_T;
typedef unsigned char u8p0_T;
typedef unsigned char u8p0;
typedef unsigned int u32p0_T;
typedef signed int s31p0_T;
typedef signed int s32p0_T;
typedef uint8_t unsigned8_T;
typedef float Float32_T;

#endif
/**************************************************************************
* Bit Field Types
***************************************************************************/
#ifdef AS_NON_CLEAN_TYPES
typedef unsigned char bitfield8_T;  /* 8 bit bitfields */
typedef unsigned short bitfield16_T; /* 16 bit bitfields */
typedef unsigned int bitfield32_T; /* 32 bit bitfields */
#endif

typedef unsigned char bitfield8_t;  /* 8 bit bitfields */
typedef unsigned short bitfield16_t; /* 16 bit bitfields */
typedef unsigned int bitfield32_t; /* 32 bit bitfields */


/***************************************************************************
* Bit position constant equates
***************************************************************************/

#define BIT31           ((uint32_t)0x80000000)    /* 2147483648  */
#define BIT30           ((uint32_t)0x40000000)    /* 1073741824  */
#define BIT29           ((uint32_t)0x20000000)    /*  536870912  */
#define BIT28           ((uint32_t)0x10000000)    /*  268435456  */
#define BIT27           ((uint32_t)0x08000000)    /*  134217728  */
#define BIT26           ((uint32_t)0x04000000)    /*   67108864  */
#define BIT25           ((uint32_t)0x02000000)    /*   33554432  */
#define BIT24           ((uint32_t)0x01000000)    /*   16777216  */
#define BIT23           ((uint32_t)0x00800000)    /*    8388608  */
#define BIT22           ((uint32_t)0x00400000)    /*    4194304  */
#define BIT21           ((uint32_t)0x00200000)    /*    2097152  */
#define BIT20           ((uint32_t)0x00100000)    /*    1048576  */
#define BIT19           ((uint32_t)0x00080000)    /*     524288  */
#define BIT18           ((uint32_t)0x00040000)    /*     262144  */
#define BIT17           ((uint32_t)0x00020000)    /*     131072  */
#define BIT16           ((uint32_t)0x00010000)    /*      65536  */
#define BIT15           ((uint16_t)0x8000)        /*      32768  */
#define BIT14           ((uint16_t)0x4000)        /*      16384  */
#define BIT13           ((uint16_t)0x2000)        /*       8192  */
#define BIT12           ((uint16_t)0x1000)        /*       4096  */
#define BIT11           ((uint16_t)0x0800)        /*       2048  */
#define BIT10           ((uint16_t)0x0400)        /*       1024  */
#define BIT09           ((uint16_t)0x0200)        /*        512  */
#define BIT08           ((uint16_t)0x0100)        /*        256  */
#define BIT07           ((uint8_t)0x80)           /*        128  */
#define BIT06           ((uint8_t)0x40)           /*         64  */
#define BIT05           ((uint8_t)0x20)           /*         32  */
#define BIT04           ((uint8_t)0x10)           /*         16  */
#define BIT03           ((uint8_t)0x08)           /*          8  */
#define BIT02           ((uint8_t)0x04)           /*          4  */
#define BIT01           ((uint8_t)0x02)           /*          2  */
#define BIT00           ((uint8_t)0x01)           /*          1  */


#endif /*Reuse_H*/
