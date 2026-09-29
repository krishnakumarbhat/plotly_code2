#ifndef REUSE_H
#define REUSE_H
/*=============================================================================
 *
 * Copyright (C) 2022 Aptiv. All rights reserved.
 * Confidential – Restricted Aptiv information. Do not disclose.
 *
 *============================================================================
 *@doc
 *
 *@module reuse.h | reuse.h
 *
 *The purpose of this module is to provide common definitions of types for all
 *processor/compiler sets.
 *
 *<nl> Put a brief description here
 *This module defines the standard reusable types and functions
 *common to all reusable software:
 *
 *@normal   Copyright (C) 2022 Aptiv. All rights reserved.
 *          Confidential – Restricted Aptiv information. Do not disclose.
 *
 *SPECIFICATION REVISION:
 *  IFS_ProjectCompiler.doc-004
 *  INTERNATIONAL STANDARD �ISO/IEC ISO/IEC 9899:1999 (E) Programming languages-C
 *
 *============================================================================
 *Configurable Development Software Module:
 *DO NOT MODIFY THIS FILE. It contains no configurable parameters.
 *============================================================================= */

/*=============================================================================
 * Processor and Compiler types
 *=============================================================================*/

/*=============================================================================
 * Include Files
 *=============================================================================*/
#include <stdbool.h>
#include <stdint.h>

/*=============================================================================
 * Global Define Constants
 *=============================================================================*/

#ifndef NULL
   #define NULL ((void *)0U)
#endif

#ifdef _DEBUG_
INLINE
void ASSERT(bool inIsValid)
{
   if (!inIsValid)
   {
      while (1)
         ;
      *should stick the debug instruction here.
   }
}
#else
   #define ASSERT(inIsValid) *Non Debug
#endif

/*=============================================================================
 * /------------------------------------------------------------------------
 * |                 Standard Preprocessor Definitions
 * \------------------------------------------------------------------------
 * ============================================================================
 * StandardPreprocessor Definitions are placed here to allow specific
 * compiler/processor choices to override the defaults.
 *=============================================================================*/

#ifndef CODED_FALSE
   #define CODED_FALSE ((uint8_t)0x55)
#endif

#ifndef CODED_TRUE
   #define CODED_TRUE ((uint8_t)0xAA)
#endif

/* Maximum of two, function like macro */
#ifndef MAX
   #define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/* Minimum of two, function like macro */
#ifndef MIN
   #define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/* Absolute of value, function like macro */
#define Abs(a) (((a) < 0) ? -(a) : (a))

#ifndef BIT_T
   #define BIT_T
/* @enum Bit_T | Defines the position of the bits */
typedef enum
{
   BIT_0,  /* @emem Bit Position  0 */
   BIT_1,  /* @emem Bit Position  1 */
   BIT_2,  /* @emem Bit Position  2 */
   BIT_3,  /* @emem Bit Position  3 */
   BIT_4,  /* @emem Bit Position  4 */
   BIT_5,  /* @emem Bit Position  5 */
   BIT_6,  /* @emem Bit Position  6 */
   BIT_7,  /* @emem Bit Position  7 */
   BIT_8,  /* @emem Bit Position  8 */
   BIT_9,  /* @emem Bit Position  9 */
   BIT_10, /* @emem Bit Position 10 */
   BIT_11, /* @emem Bit Position 11 */
   BIT_12, /* @emem Bit Position 12 */
   BIT_13, /* @emem Bit Position 13 */
   BIT_14, /* @emem Bit Position 14 */
   BIT_15, /* @emem Bit Position 15 */
   BIT_16, /* @emem Bit Position 16 */
   BIT_17, /* @emem Bit Position 17 */
   BIT_18, /* @emem Bit Position 18 */
   BIT_19, /* @emem Bit Position 19 */
   BIT_20, /* @emem Bit Position 20 */
   BIT_21, /* @emem Bit Position 21 */
   BIT_22, /* @emem Bit Position 22 */
   BIT_23, /* @emem Bit Position 23 */
   BIT_24, /* @emem Bit Position 24 */
   BIT_25, /* @emem Bit Position 25 */
   BIT_26, /* @emem Bit Position 26 */
   BIT_27, /* @emem Bit Position 27 */
   BIT_28, /* @emem Bit Position 28 */
   BIT_29, /* @emem Bit Position 29 */
   BIT_30, /* @emem Bit Position 30 */
   BIT_31, /* @emem Bit Position 31 */
   BIT_MAX /* @emem 32 Bits */
} Bit_T;
#endif

/* bit fields */
typedef unsigned char bitfield8_t;   /* 8 bit bitfields */
typedef unsigned short bitfield16_t; /* 16 bit bitfields */
typedef unsigned int bitfield32_t;   /* 32 bit bitfields */

#ifndef _TMS320C6X
typedef float float32_t;
#else
   #include "vect.h" // On the DSP, vect.h defines float32_t
#endif

typedef double float64_t;

/* defines for bytewise shifts  */
#ifndef ONE_BYTE_SHIFT
#define ONE_BYTE_SHIFT   (8U)
#endif
#ifndef TWO_BYTE_SHIFT
#define TWO_BYTE_SHIFT   (16U)
#endif
#ifndef THREE_BYTE_SHIFT
#define THREE_BYTE_SHIFT (24U)
#endif

#ifndef EOK
   #define EOK (0)
#endif

#endif /* REUSE_H */
