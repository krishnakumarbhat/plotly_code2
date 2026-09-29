
/* Ok.. lets start ..!! 21 Jan 2005 :)*/
/* Code reused from the CADS 2 prog..*/

#ifndef FIXMAC_H
#define FIXMAC_H

/*===================================================================*\
 * Copyright 2004, Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential.
 *--------------------------------------------------------------------
 * Module Name:     fixmac.h
 * Created By:      afh124
 * Created Date:    Wed Nov 03 15:44:49 2004
 * %version:        2 %
 * %cvtype:         incl %
 * %instance:       kok_css2_400 %
 * %derived_by:     xzxcrv %
 * %date_modified:  Tue Mar 27 12:51:49 2018 %
 *--------------------------------------------------------------------
 *
 * Description:
 *   The file contains the generic "C" based fixed point
 *   math functions/inlines for 32 bit processor
 *
 * Traces to: Rick smith's CSS fixed point math library for HC12
 *
 *
 * Applicable Standards (in order of precedence: highest first):
 *     SW REF 264.15D "DE Systems C Coding Standards" dated 12/23/01
 *
 * Deviations from Delco C Coding standards:
 *   1. C46 - Abbreviations are not documented in this source file.
 *
 *   2. C54 - Function header block is placed at the beginning of
 *      function definition instead before function proto type
 *      declaration.
 *
 *   3. C58 - Function header blocks only list global variables that
 *      are not accessed by macros or functions.
 *
 *   4. C60 - The function is pre-emptible or re-entrant is not
 *      applicable to this program.
\*===================================================================*/
/*
 *    NO CASTING SHOULD BE USED OTHER THAN WHAT THE Fix() MACRO DOES.
 *
 *    #include "fixmac.h" MUST BE IN YOUR C FILE OR THESE MACROS WON'T
 *                        WORK! (but they will still compile and give
 *                        you wrong results!)
 *
 *
 *    This file contains routines necessary to allow a user to perform base 2
 *    precision fixed point math from the "C" language level.
 *
 *    With the exception of the Fix() macro, the math macros contain no
 *    smarts. The user must understand the math operation they wish to
 *    perform in binary.  The user can then use these routines
 *    to perform the math operations.  These routines provide
 *    a higher level of control than is possible in "C" yet are
 *    more abstracted than assembly.
 *
 *    The Fix() macro provides the user with an abstracted method for
 *    moving the decimal point position of the number.  The user must
 *    ensure that the target type's size is able to accommadate all possible
 *    values of the source type variable.
 *
 *    The math macros call inline assembly routines. The parameter and return
 *    sizes of the variables that the user supplies to these routines
 *    MUST MATCH!
 *
 *  CAUTION:
 *    -Using wrongly matched sizes of variables will cause wrong results.
 *    -Casting variables in combination with using these macros can cause wrong
 *     results. You do not need to do any casting; use Fix instead.
 *
 *  LEGEND:
 *  U = unsigned, S = signed, H = high, M = middle, L = low,
 *  BPP = Binary point position. (radix point base 2)
 *
 *  The names of the macros reflect the parameters used, the order of
 *  the parameters and the returned value.
 *
 *   returnValue_Macroname_firstParam_secondParam
 *
 *   Note: for Division macros the numerator is always the firstParam,
 *   and the denominator is always the secondParam.
 *
 *    Operation          Use Macro        Notes
 *    ---------          ---------        -----
 *    u8 x u8 = u8h      U8H_Mul_U8_U8
 *    u8 x u8 = u8l      U8L_Mul_U8_U8
 *    u8 x u8 = u16      U16_Mul_U8_U8
 *
 *    u8 x s8 = s8h      S8H_Mul_U8_S8
 *    u8 x s8 = s8l      S8L_Mul_U8_S8
 *    u8 x s8 = s16      S16_Mul_U8_S8
 *
 *    s8 x s8 = s8h      S8H_Mul_S8_S8
 *    s8 x s8 = s8l      S8L_Mul_S8_S8
 *    s8 x s8 = s16      S16_Mul_S8_S8
 *
 *    u8 x u16 = u16l    U16L_Mul_U8_U16
 *    u8 x u16 = u16h    U16H_Mul_U8_U16  high 16 bits of 24 bit result.
 *    u8 x u16 = u24     U32_Mul_U8_U16
 *
 *    s8 x u16 = s16l    S16L_Mul_S8_U16
 *    s8 x u16 = s16h    S16H_Mul_S8_U16
 *    s8 x u16 = s24     S32_Mul_S8_U16
 *
 *    u8 x s16 = s16l S16L_Mul_U8_S16
 *    u8 x s16 = s16h S16H_Mul_U8_S16
 *    u8 x s16 = s24     S32_Mul_U8_S16
 *
 *    u16 x u16 = u16l   U16L_Mul_U16_U16
 *    u16 x u16 = u16m   U16M_Mul_U16_U16  Middle 16 bits of the 32 bit result.
 *    u16 x u16 = u16h   U16H_Mul_U16_U16
 *    u16 x u16 = u32    u32_Mul_U16_U16
 *
 *    s16 x u16 = s16l   S16L_Mul_S16_U16
 *    s16 x u16 = s16m   S16M_Mul_S16_U16  Middle 16 bits of the 32 bit result.
 *    s16 x u16 = s16h   S16H_Mul_S16_U16
 *    s16 x u16 = s32    S32_Mul_S16_U16
 *
 *    s16 x s16 = s16l   S16L_Mul_S16_S16
 *    s16 x s16 = s16m   S16M_Mul_S16_S16  Middle 16 bits of the 32 bit result.
 *    s16 x s16 = s16h   s16H_Mul_S16_S16
 *    s16 x s16 = s32    s32_Mul_s16_s16
 *
 *    u16 / u16 = u16    U16_Div_U16_U16
 *    s16 / s16 = s16    S16_Div_S16_S16
 *    u32 / u16 = u16    U16_Div_U32_U16
 *    s32 / s16 = s16    S16_Div_S32_S16
 *
 *    u16 / u16 = u16ratio  U16_Fdiv_U16_U16
 *                BPPresult=BPPnumerator*16-BPPdenominator.
 *
 * GENERAL FIXED POINT USAGE:
 *
 *    Addition,Subtraction - Align BPP prior to operation. Care must be
 *       taken when adding a signed to an unsigned. ( the unsigned number
 *       can not be greater than 127 for example in S8 + U8.)
 *
 *    Multiplication - Generally just perform the multiply first and
 *       Fix the result afterwards, optimizing by possibly not taking
 *       the full result size ( U8 * U16 = u24 but take U16L). You have to
 *       account for the BPP in your Fix (i.e. if you need u1p7 * u6p2 = u6p2,
 *       then p7 + p2 = p9, but if you take the only the high byte, the
 *       final fix statement will be: Fix(result,u7p1,u6p2) because p9-8=p1.
 *
 *    Division - Generally you Fix the numerator prior to the divide
 *       based on the results BPP. Example: if you need u10p6 / u10p6 = u2p14,
 *       then you could Fix the numerator to u12p20 first, and do
 *       a U16_Div_U32_U16. The result is p20 - p6 = p14 and since the
 *       the result is U16 the result type is u2p14. If the numberator can
 *       not be fixed to a DDP left enough to get the desired result type,
 *       other options might include Fixing the denominator to move it's
 *       BPP farther right.
 *
 *  Overflow Limiting functions for Addition and Subtraction:
 *
 *    U8_Add_Limit
 *    U8_Sub_Limit
 *    S8_Add_Limit
 *    S8_Sub_Limit
 *    U16_Add_Limit
 *    U16_Sub_Limit
 *    S16_Add_Limit
 *    S16_Sub_Limit
 *    U32_Add_Limit
 *    U32_Sub_Limit
 *    S32_Add_Limit
 *    S32_Sub_Limit
 *    U64_Add_Limit
 *    U64_Sub_Limit
 *    S64_Add_Limit
 *    S64_Sub_Limit
 *
 * LIMIT FUNCTION RESTRICTIONS:
 *
 *   These routines work ONLY on an addition or
 *   subtraction expression.
 *
 *
 * FYI: The technique used in some of the multiply routines involves
 *   removing the sign from a number prior to the multiply and
 *   restoring the sign to the result. A sign byte is being used in which
 *   the byte=FF means signed and byte=0 means unsigned. This byte is
 *   exclusive or'd with the signed number to perform a 1's complement.
 *   The byte is then ASRd to fill the carry without changing the byte's
 *   value. The carry is added to the signed number to perform the 2's
 *   complement. If the sign byte=0 then the previous operations do nothing.
 *   This allows the removal of a sign with no branching required.
 *
 ****************************************************************************/
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/************************************
 * Pre-defined Standard Types
 ************************************/
/*************************************************************************/
#ifndef PC_RESIM
// #ifndef bool
//    typedef unsigned char bool;  /* PRQA S 4602 ++ */ /* <stdbool.h> is not included in this project*/
// #endif
#endif

#ifndef boolean_T
typedef unsigned char boolean_T;
#endif

#ifndef unsigned8_T
typedef unsigned char unsigned8_T;
#endif

#ifndef unsigned16_T
typedef unsigned short int unsigned16_T;
#endif

#ifndef unsigned32_T
typedef unsigned int unsigned32_T;
#endif

#ifndef unsigned40_T
typedef unsigned long long unsigned40_T;
#endif

#ifndef unsigned64_T
typedef unsigned long long unsigned64_T;
#endif

#ifndef signed8_T
typedef signed char signed8_T;
#endif

#ifndef signed16_T
typedef signed short signed16_T;
#endif

#ifndef signed32_T
typedef signed int signed32_T;
#endif

#ifndef signed40_T
typedef signed long long signed40_T;
#endif

#ifndef signed64_T
typedef signed long long signed64_T;
#endif

/*************************************************************************/
#ifdef PC_RESIM
#ifndef boolean
typedef unsigned char boolean;
#endif

#ifndef uint8
typedef unsigned char uint8;
#endif

#ifndef uint16
typedef unsigned short uint16;
#endif

#ifndef uint32
typedef unsigned long uint32;
#endif

#ifndef sint8
typedef signed char sint8;
#endif

#ifndef sint16
typedef signed short sint16;
#endif

#ifndef sint32
typedef signed long sint32;
#endif
#endif
/*************************************************************************/

#ifndef uint8_T
typedef unsigned char uint8_T;
#endif

#ifndef uint16_T
typedef unsigned short uint16_T;
#endif

#ifndef uint32_T
typedef unsigned int uint32_T;
#endif

#ifndef uint64_T
typedef unsigned long long uint64_T;
#endif

#ifndef int8_T
typedef signed char int8_T;
#endif

#ifndef int16_T
typedef signed short int16_T;
#endif

#ifndef int32_T
typedef signed int int32_T;
#endif

#if defined(_WIN32) || defined(TASKING_COMPILER)
#ifndef int64_T
typedef long long int64_T;
#endif

#if 1 // pks
#ifndef int64_t
typedef long long int64_t;
#endif
#endif
#endif

/*************************************************************************/

#ifndef bitfield8_T
typedef unsigned char bitfield8_T;
#endif

#ifndef bit_field_T
typedef unsigned char bit_field_T;
#endif

#ifndef bitfield16_T
typedef unsigned short bitfield16_T;
#endif

#ifndef bitfield32_T
typedef unsigned int bitfield32_T;
#endif

#ifndef bitfield8_t
typedef unsigned char bitfield8_t;
#endif

#ifndef bitfield16_t
typedef unsigned short bitfield16_t;
#endif

#ifndef bitfield32_t
typedef unsigned int bitfield32_t;
#endif

/*************************************************************************/

#ifndef float32_t
typedef float float32_t;
#endif

#ifndef float32_T
typedef float float32_T;
#endif

#ifndef Float32_T
typedef float Float32_T;
#endif

#ifndef float_tracker_T
typedef float float_tracker_T;
#endif

#ifndef real32_T
typedef float real32_T;
#endif

#ifndef float64_T
typedef double float64_T;
#endif

#ifndef Float64_T
typedef double Float64_T;
#endif

#ifndef real64_T
typedef double real64_T;
#endif

#ifndef real32_t
typedef float real32_t;
#endif

#ifndef real64_t
typedef double real64_t;
#endif

#ifndef double64_T
typedef double double64_T;
#endif
/*************************************************************************/

#ifndef int8_t
typedef signed char int8_t;
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef int16_t
typedef signed short int16_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef int32_t
typedef signed int int32_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#if defined(_WIN32) || defined(TASKING_COMPILER)
#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif
#elif defined(__GNUC__)
#ifndef uint64_t
typedef unsigned long int uint64_t;
#endif
#endif
/*************************************************************************/

/* Pointer to function type */
typedef void (*ptr_to_function_T)(void);
typedef void (*function_pointer_T)(void);
typedef void (*const tIsrFunc)(void);

/*************************************************************************/

#ifndef interrupt_state_t
typedef unsigned int interrupt_state_t;
#endif

#ifndef IO_Configuration_T
typedef unsigned int IO_Configuration_T;
#endif

/*************************************************************************/
#define ONE_BIT_SHIFT          (1)
#define TWO_BIT_SHIFT          (2)
#define THREE_BIT_SHIFT        (3)
#define FOUR_BIT_SHIFT         (4)
#define FIVE_BIT_SHIFT         (5)
#define TWENTY_BIT_SHIFT       (20)
#define TWENTY_EIGHT_BIT_SHIFT (28)

#ifndef ONE_BYTE_SHIFT
#define ONE_BYTE_SHIFT (8)
#endif
#ifndef TWO_BYTE_SHIFT
#define TWO_BYTE_SHIFT (16)
#endif
#ifndef THREE_BYTE_SHIFT
#define THREE_BYTE_SHIFT (24)
#endif

#define BYTE_0  (0)
#define BYTE_1  (1)
#define BYTE_2  (2)
#define BYTE_3  (3)
#define BYTE_4  (4)
#define BYTE_5  (5)
#define BYTE_6  (6)
#define BYTE_7  (7)
#define BYTE_8  (8)
#define BYTE_9  (9)
#define BYTE_10 (10)
#define BYTE_11 (11)
#define BYTE_12 (12)
#define BYTE_13 (13)
#define BYTE_14 (14)
#define BYTE_15 (15)
#define BYTE_16 (16)
#define BYTE_17 (17)
#define BYTE_18 (18)
#define BYTE_19 (19)
#define BYTE_20 (20)
#define BYTE_21 (21)
#define BYTE_22 (22)
#define BYTE_23 (23)
#define BYTE_24 (24)
#define BYTE_25 (25)

#define HIGH_VARIANT 0x03
#define MID_VARIANT  0x02

/*per 7.18 of the ANSI/ISO Standard */
#ifndef UINT8_MIN
#define UINT8_MIN (0) /* Minimum value for uint8_t */
#endif
#ifndef UINT8_MAX
#define UINT8_MAX (0xffU) /* Maximum value for uint8_t */
#endif

#ifndef UINT16_MIN
#define UINT16_MIN (0) /* Minimum value for uint16_t */
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (0xffffU) /* Maximum value for uint16_t */
#endif

#ifndef UINT24_MIN
#define UINT24_MIN (0) /* Minimum value for uint32_t containing 24 bits */
#endif
#ifndef UINT24_MAX
#define UINT24_MAX (0x00ffffffU) /* Maximum value for uint32_t containing 24 bits */
#endif

#ifndef UINT32_MIN
#define UINT32_MIN (0) /* Minimum value for uint32_t */
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (0xffffffffU) /* Maximum value for uint32_t */
#endif

#ifndef INT8_MIN
#define INT8_MIN (-INT8_MAX - 1) /* Minimum value for int8_t */
#endif
#ifndef INT8_MAX
#define INT8_MAX (127) /* Maximum value for int8_t */
#endif

#ifndef INT16_MIN
#define INT16_MIN (-INT16_MAX - 1) /* Minimum value for int16_t */
#endif
#ifndef INT16_MAX
#define INT16_MAX (32767) /* Maximum value for int16_t */
#endif

#ifndef INT24_MIN
#define INT24_MIN (-INT24_MAX - 1) /* Minimum value for int32_t containing 24 bits */
#endif
#ifndef INT24_MAX
#define INT24_MAX (8388607) /* Maximum value for int32_t containing 24 bits */
#endif

#ifndef INT32_MIN
#define INT32_MIN (-INT32_MAX - 1) /* Minimum value for int32_t */
#endif
#ifndef INT32_MAX
#define INT32_MAX (2147483647) /* Maximum value for int32_t */
#endif

#ifndef U8_MAX
#define U8_MAX (0xFF)
#endif
#ifndef U8_MIN
#define U8_MIN (0)
#endif

#ifndef U16_MAX
#define U16_MAX (0xFFFF)
#endif
#ifndef U16_MIN
#define U16_MIN (0)
#endif

#ifndef U32_MAX
#define U32_MAX (0xFFFFFFFF)
#endif
#ifndef U32_MIN
#define U32_MIN (0)
#endif

#ifndef U40_MAX
#define U40_MAX (0xFFFFFFFFFF)
#endif
#ifndef U40_MIN
#define U40_MIN (0)
#endif

#ifndef U64_MAX
#define U64_MAX (0xFFFFFFFFFFFFFFFF)
#endif
#ifndef U64_MIN
#define U64_MIN (0)
#endif

#ifndef S8_MAX
#define S8_MAX (127) /* 0x7F */
#endif
#ifndef S8_MIN
#define S8_MIN (-128) /* 0x80 */
#endif

#ifndef S16_MAX
#define S16_MAX (32767) /* 0x7FFF */
#endif
#ifndef S16_MIN
#define S16_MIN (-32768) /* 0x8000 */
#endif

#ifndef S32_MAX
#define S32_MAX (2147483647L) /* 0x7FFFFFFF */
#endif
#ifndef S32_MIN
#define S32_MIN (-2147483648L) /* 0x80000000 */
#endif

#ifndef S40_MAX
#define S40_MAX (549755813887) /* 0x7FFFFFFFFF */
#endif
#ifndef S40_MIN
#define S40_MIN (-549755813888) /* 0x8000000000 */
#endif

#ifndef S64_MAX
#define S64_MAX (9223372036854775807) /* 7FFFFFFFFFFFFFFF */
#endif
#ifndef S64_MIN
#define S64_MIN (-9223372036854775808) /* 0x8000000000000000 */
#endif

#ifndef FLOAT_MAX
#define FLOAT_MAX 3.402823466e+38f
#endif

/*
// Maximum function.
*/
#ifndef Max
#define Max(a, b) (((a) > (b)) ? (a) : (b)) /* PRQA S 3453 ++ */
#endif

/*
// Minimum function.
*/
#ifndef Min
#define Min(a, b) (((a) < (b)) ? (a) : (b))
#endif

/*
// Absolute function.
*/
#ifndef Abs
#define Abs(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef __cplusplus
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef false
#define false 0 /* PRQA S 4600 ++ */ /* <stdbool.h> is not included in this project*/
#endif

#ifndef true
#define true 1
#endif
#endif
/* CODED_FALSE and CODED_TRUE should be used for safety critical checks */
#ifndef CODED_FALSE
#define CODED_FALSE 0x55
#endif

#ifndef CODED_TRUE
#define CODED_TRUE 0xAA
#endif

/* Present in <stddef.h> */
/*
#ifndef NULL
#define NULL  ((void*)0)
#endif
*/

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

#if defined(PC_RESIMULATION)
#ifndef INLINE
#if defined(WIN32)
#define INLINE __inline
#else
#define INLINE static // let's you define functions in header files that are compatible with inlining
#endif
#endif
#endif

/* FIX to Float convertion  */
#define FIX16toFLT(val, BPP) ((float)(val) / (float)(unsigned16_T)(1 << (BPP)))
#define FIX32toFLT(val, BPP) ((float)(val) / (float)(unsigned32_T)(1 << (BPP)))
#define FIX64toFLT(val, BPP) ((float)(val) / (float)(unsigned64_T)(1 << (BPP)))
#define Fix2Double(value, from_type) \
   (((BPP_##from_type) > 0) ? ((double)(value) / (double)(1ull << Max(BPP_##from_type, 0))) : ((double)(value) * (double)(1ull << Max(-1 * (BPP_##from_type), 0)))) /* PRQA S 0881 ++*/ /*Reason for QAC 0881 suppression: Two concatenations are not used to form a MACRO */
#define Float2Fix(var, type) (Init_Fixed_Point(var, type))
#define Fix2Float(value, from_type) \
   (((BPP_##from_type) > 0) ? ((float)(value) / (float)(1ull << Max(BPP_##from_type, 0))) : ((float)(value) * (float)(1ull << Max(-1 * (BPP_##from_type), 0))))

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * The following fixed point type definitions are made up of 3 elements:
 * 1) The typedef itself.
 * 2) The Binary point position for this type.
 * 3) The indicator of whether the type is signed or not.
 * These are used within the Fix macro. The Fix macro appends BBP_ and
 * SIGNED_ to the types given to it.
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/**********************************/
/* Extended Types.                */
/**********************************/

/***********************************/
/* Unsigned 8 bit Extended Types.  */
/***********************************/
typedef unsigned8_T u9pm1_T; /*  bbbbbbbb_. */
#define BPP_u9pm1_T    (-1)
#define SIGNED_u9pm1_T FALSE

typedef unsigned8_T u10pm2_T; /*  bbbbbbbb__. */
#define BPP_u10pm2_T    (-2)
#define SIGNED_u10pm2_T FALSE

typedef unsigned8_T um6p14_T; /*  .______bbbbbbbb */
#define BPP_um6p14_T    14
#define SIGNED_um6p14_T FALSE

typedef unsigned8_T um7p15_T; /*  ._______bbbbbbbb */
#define BPP_um7p15_T    15
#define SIGNED_um7p15_T FALSE

typedef unsigned8_T um1p9_T;
#define BPP_um1p9_T    9
#define SIGNED_um1p9_T FALSE

/***********************************/
/* signed 8 bit Extended Types.  */
/***********************************/
typedef signed8_T sm1p8_T; /*  ._bbbbbbbb */
#define BPP_sm1p8_T    8
#define SIGNED_sm1p8_T TRUE

typedef signed8_T s8pm1_T; /*  sbbbbbbb_. */
#define BPP_s8pm1_T    (-1)
#define SIGNED_s8pm1_T TRUE

typedef signed8_T s9pm2_T; /*  sbbbbbbb__. */
#define BPP_s9pm2_T    (-2)
#define SIGNED_s9pm2_T TRUE

/***********************************/
/* Unsigned 16 bit Extended Types. */
/***********************************/
typedef unsigned16_T um1p17_T; /*  ._bbbbbbbbbbbbbbbb */
#define BPP_um1p17_T    17
#define SIGNED_um1p17_T FALSE

typedef unsigned16_T um7p23_T; /*  ._______bbbbbbbbbbbbbbbb */
#define BPP_um7p23_T    23
#define SIGNED_um7p23_T FALSE

typedef unsigned16_T um9p25_T; /*  ._________bbbbbbbbbbbbbbbb */
#define BPP_um9p25_T    25
#define SIGNED_um9p25_T FALSE

typedef unsigned16_T um5p21_T; /*._____bbbbbbbbbbbbbbbb */
#define BPP_um5p21_T    21
#define SIGNED_um5p21_T FALSE

typedef unsigned16_T um6p22_T; /*.______bbbbbbbbbbbbbbbb */
#define BPP_um6p22_T    22
#define SIGNED_um6p22_T FALSE

typedef unsigned16_T um8p24_T; /*._______bbbbbbbbbbbbbbbb */
#define BPP_um8p24_T    24
#define SIGNED_um8p24_T FALSE

typedef unsigned16_T um12p28_T; /*.___________bbbbbbbbbbbbbbbb */
#define BPP_um12p28_T    28
#define SIGNED_um12p28_T FALSE

typedef unsigned16_T um3p19_T; /*  .___bbbbbbbbbbbbbbbb */
#define BPP_um3p19_T    19
#define SIGNED_um3p19_T FALSE

typedef unsigned16_T um4p20_T; /*  .____bbbbbbbbbbbbbbbb */
#define BPP_um4p20_T    20
#define SIGNED_um4p20_T FALSE

/***********************************/
/* Signed 16 bit Extended Types.   */
/***********************************/
typedef signed16_T sm1p16_T; /*  ._bbbbbbbbbbbbbbbb */
#define BPP_sm1p16_T    16
#define SIGNED_sm1p16_T TRUE

typedef signed16_T sm2p17_T; /*  .__bbbbbbbbbbbbbbbb */
#define BPP_sm2p17_T    17
#define SIGNED_sm2p17_T TRUE

typedef signed16_T sm3p18_T; /*  .___bbbbbbbbbbbbbbbb */
#define BPP_sm3p18_T    18
#define SIGNED_sm3p18_T TRUE

typedef signed16_T sm4p19_T; /*  .____bbbbbbbbbbbbbbbb */
#define BPP_sm4p19_T    19
#define SIGNED_sm4p19_T TRUE

typedef signed16_T sm5p20_T; /* ._____bbbbbbbbbbbbbbb  */
#define BPP_sm5p20_T    20   /* .1    5             20 */
#define SIGNED_sm5p20_T TRUE

/***********************************/
/* Unsigned 32 bit Extended Types. */
/***********************************/

typedef unsigned32_T um2p34_T; /*  .__bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_um2p34_T    34     /*  .1_3______________________________34*/
#define SIGNED_um2p34_T FALSE

typedef unsigned32_T um3p35_T; /*  .___bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_um3p35_T    35     /*  .1__4______________________________35*/
#define SIGNED_um3p35_T FALSE

typedef unsigned32_T um8p40_T; /*  .________bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_um8p40_T    40     /*  .1_______9______________________________40*/
#define SIGNED_um8p40_T FALSE

/***********************************/
/* signed 32 bit Extended Types. */
/***********************************/
typedef signed32_T sm1p32_T; /* s._bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_sm1p32_T    32   /* s._2_____________________________32*/
#define SIGNED_sm1p32_T TRUE

typedef signed32_T sm2p33_T; /* s.__bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_sm2p33_T    33   /* s.__3_____________________________32*/
#define SIGNED_sm2p33_T TRUE

typedef signed32_T sm3p34_T; /* s.___bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_sm3p34_T    34   /* s.___4_____________________________34*/
#define SIGNED_sm3p34_T TRUE

typedef signed32_T sm5p36_T; /* s.____bbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_sm5p36_T    36   /* s.____5___________________________32*/
#define SIGNED_sm5p36_T TRUE

typedef signed32_T sm6p37_T; /* s.______bbbbbbbbbbbbbbbbbbbbbbbbbbbbb */
#define BPP_sm6p37_T    37   /* s._____6___________________________32*/
#define SIGNED_sm6p37_T TRUE

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * All of the following generic types are generated automatically.
 * Please do not place any custom or extended types below this
 * comment.
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/**********************************/
/* Unsigned 8 bit Generic Types.  */
/**********************************/

typedef unsigned8_T u8p0_T;
#define BPP_u8p0_T    0
#define SIGNED_u8p0_T FALSE

typedef unsigned8_T u7p1_T;
#define BPP_u7p1_T    1
#define SIGNED_u7p1_T FALSE

typedef unsigned8_T u6p2_T;
#define BPP_u6p2_T    2
#define SIGNED_u6p2_T FALSE

typedef unsigned8_T u5p3_T;
#define BPP_u5p3_T    3
#define SIGNED_u5p3_T FALSE

typedef unsigned8_T u4p4_T;
#define BPP_u4p4_T    4
#define SIGNED_u4p4_T FALSE

typedef unsigned8_T u3p5_T;
#define BPP_u3p5_T    5
#define SIGNED_u3p5_T FALSE

typedef unsigned8_T u2p6_T;
#define BPP_u2p6_T    6
#define SIGNED_u2p6_T FALSE

typedef unsigned8_T u1p7_T;
#define BPP_u1p7_T    7
#define SIGNED_u1p7_T FALSE

typedef unsigned8_T u0p8_T;
#define BPP_u0p8_T    8
#define SIGNED_u0p8_T FALSE

typedef unsigned8_T um2p10_T;
#define BPP_um2p10_T    10
#define SIGNED_um2p10_T FALSE
/**********************************/
/* Signed 8 bit Generic Types.    */
/**********************************/

typedef signed8_T s7p0_T;
#define BPP_s7p0_T    0
#define SIGNED_s7p0_T TRUE

typedef signed8_T s6p1_T;
#define BPP_s6p1_T    1
#define SIGNED_s6p1_T TRUE

typedef signed8_T s5p2_T;
#define BPP_s5p2_T    2
#define SIGNED_s5p2_T TRUE

typedef signed8_T s4p3_T;
#define BPP_s4p3_T    3
#define SIGNED_s4p3_T TRUE

typedef signed8_T s3p4_T;
#define BPP_s3p4_T    4
#define SIGNED_s3p4_T TRUE

typedef signed8_T s2p5_T;
#define BPP_s2p5_T    5
#define SIGNED_s2p5_T TRUE

typedef signed8_T s1p6_T;
#define BPP_s1p6_T    6
#define SIGNED_s1p6_T TRUE

typedef signed8_T s0p7_T;
#define BPP_s0p7_T    7
#define SIGNED_s0p7_T TRUE

/**********************************/
/* Unsigned 16 bit Generic Types. */
/**********************************/

typedef unsigned16_T u16p0_T;
#define BPP_u16p0_T    0
#define SIGNED_u16p0_T FALSE

typedef unsigned16_T u15p1_T;
#define BPP_u15p1_T    1
#define SIGNED_u15p1_T FALSE

typedef unsigned16_T u14p2_T;
#define BPP_u14p2_T    2
#define SIGNED_u14p2_T FALSE

typedef unsigned16_T u13p3_T;
#define BPP_u13p3_T    3
#define SIGNED_u13p3_T FALSE

typedef unsigned16_T u12p4_T;
#define BPP_u12p4_T    4
#define SIGNED_u12p4_T FALSE

typedef unsigned16_T u11p5_T;
#define BPP_u11p5_T    5
#define SIGNED_u11p5_T FALSE

typedef unsigned16_T u10p6_T;
#define BPP_u10p6_T    6
#define SIGNED_u10p6_T FALSE

typedef unsigned16_T u9p7_T;
#define BPP_u9p7_T    7
#define SIGNED_u9p7_T FALSE

typedef unsigned16_T u8p8_T;
#define BPP_u8p8_T    8
#define SIGNED_u8p8_T FALSE

typedef unsigned16_T u7p9_T;
#define BPP_u7p9_T    9
#define SIGNED_u7p9_T FALSE

typedef unsigned16_T u6p10_T;
#define BPP_u6p10_T    10
#define SIGNED_u6p10_T FALSE

typedef unsigned16_T u5p11_T;
#define BPP_u5p11_T    11
#define SIGNED_u5p11_T FALSE

typedef unsigned16_T u4p12_T;
#define BPP_u4p12_T    12
#define SIGNED_u4p12_T FALSE

typedef unsigned16_T u3p13_T;
#define BPP_u3p13_T    13
#define SIGNED_u3p13_T FALSE

typedef unsigned16_T u2p14_T;
#define BPP_u2p14_T    14
#define SIGNED_u2p14_T FALSE

typedef unsigned16_T u1p15_T;
#define BPP_u1p15_T    15
#define SIGNED_u1p15_T FALSE

typedef unsigned16_T u0p16_T;
#define BPP_u0p16_T    16
#define SIGNED_u0p16_T FALSE

typedef unsigned16_T um2p18_T;
#define BPP_um2p18_T    18
#define SIGNED_um2p18_T FALSE

/**********************************/
/* Signed 16 bit Generic Types.   */
/**********************************/
typedef signed16_T s15p0_T;
#define BPP_s15p0_T    0
#define SIGNED_s15p0_T TRUE

typedef signed16_T s14p1_T;
#define BPP_s14p1_T    1
#define SIGNED_s14p1_T TRUE

typedef signed16_T s13p2_T;
#define BPP_s13p2_T    2
#define SIGNED_s13p2_T TRUE

typedef signed16_T s12p3_T;
#define BPP_s12p3_T    3
#define SIGNED_s12p3_T TRUE

typedef signed16_T s11p4_T;
#define BPP_s11p4_T    4
#define SIGNED_s11p4_T TRUE

typedef signed16_T s10p5_T;
#define BPP_s10p5_T    5
#define SIGNED_s10p5_T TRUE

typedef signed16_T s9p6_T;
#define BPP_s9p6_T    6
#define SIGNED_s9p6_T TRUE

typedef signed16_T s8p7_T;
#define BPP_s8p7_T    7
#define SIGNED_s8p7_T TRUE

typedef signed16_T s7p8_T;
#define BPP_s7p8_T    8
#define SIGNED_s7p8_T TRUE

typedef signed16_T s6p9_T;
#define BPP_s6p9_T    9
#define SIGNED_s6p9_T TRUE

typedef signed16_T s5p10_T;
#define BPP_s5p10_T    10
#define SIGNED_s5p10_T TRUE

typedef signed16_T s4p11_T;
#define BPP_s4p11_T    11
#define SIGNED_s4p11_T TRUE

typedef signed16_T s3p12_T;
#define BPP_s3p12_T    12
#define SIGNED_s3p12_T TRUE

typedef signed16_T s2p13_T;
#define BPP_s2p13_T    13
#define SIGNED_s2p13_T TRUE

typedef signed16_T s1p14_T;
#define BPP_s1p14_T    14
#define SIGNED_s1p14_T TRUE

typedef signed16_T s0p15_T;
#define BPP_s0p15_T    15
#define SIGNED_s0p15_T TRUE

/**********************************/
/* Unsigned 32 bit Generic Types. */
/**********************************/
typedef unsigned32_T u32p0_T;
#define BPP_u32p0_T    0
#define SIGNED_u32p0_T FALSE

typedef unsigned32_T u31p1_T;
#define BPP_u31p1_T    1
#define SIGNED_u31p1_T FALSE

typedef unsigned32_T u30p2_T;
#define BPP_u30p2_T    2
#define SIGNED_u30p2_T FALSE

typedef unsigned32_T u29p3_T;
#define BPP_u29p3_T    3
#define SIGNED_u29p3_T FALSE

typedef unsigned32_T u28p4_T;
#define BPP_u28p4_T    4
#define SIGNED_u28p4_T FALSE

typedef unsigned32_T u27p5_T;
#define BPP_u27p5_T    5
#define SIGNED_u27p5_T FALSE

typedef unsigned32_T u26p6_T;
#define BPP_u26p6_T    6
#define SIGNED_u26p6_T FALSE

typedef unsigned32_T u25p7_T;
#define BPP_u25p7_T    7
#define SIGNED_u25p7_T FALSE

typedef unsigned32_T u24p8_T;
#define BPP_u24p8_T    8
#define SIGNED_u24p8_T FALSE

typedef unsigned32_T u23p9_T;
#define BPP_u23p9_T    9
#define SIGNED_u23p9_T FALSE

typedef unsigned32_T u22p10_T;
#define BPP_u22p10_T    10
#define SIGNED_u22p10_T FALSE

typedef unsigned32_T u21p11_T;
#define BPP_u21p11_T    11
#define SIGNED_u21p11_T FALSE

typedef unsigned32_T u20p12_T;
#define BPP_u20p12_T    12
#define SIGNED_u20p12_T FALSE

typedef unsigned32_T u19p13_T;
#define BPP_u19p13_T    13
#define SIGNED_u19p13_T FALSE

typedef unsigned32_T u18p14_T;
#define BPP_u18p14_T    14
#define SIGNED_u18p14_T FALSE

typedef unsigned32_T u17p15_T;
#define BPP_u17p15_T    15
#define SIGNED_u17p15_T FALSE

typedef unsigned32_T u16p16_T;
#define BPP_u16p16_T    16
#define SIGNED_u16p16_T FALSE

typedef unsigned32_T u15p17_T;
#define BPP_u15p17_T    17
#define SIGNED_u15p17_T FALSE

typedef unsigned32_T u14p18_T;
#define BPP_u14p18_T    18
#define SIGNED_u14p18_T FALSE

typedef unsigned32_T u13p19_T;
#define BPP_u13p19_T    19
#define SIGNED_u13p19_T FALSE

typedef unsigned32_T u12p20_T;
#define BPP_u12p20_T    20
#define SIGNED_u12p20_T FALSE

typedef unsigned32_T u11p21_T;
#define BPP_u11p21_T    21
#define SIGNED_u11p21_T FALSE

typedef unsigned32_T u10p22_T;
#define BPP_u10p22_T    22
#define SIGNED_u10p22_T FALSE

typedef unsigned32_T u9p23_T;
#define BPP_u9p23_T    23
#define SIGNED_u9p23_T FALSE

typedef unsigned32_T u8p24_T;
#define BPP_u8p24_T    24
#define SIGNED_u8p24_T FALSE

typedef unsigned32_T u7p25_T;
#define BPP_u7p25_T    25
#define SIGNED_u7p25_T FALSE

typedef unsigned32_T u6p26_T;
#define BPP_u6p26_T    26
#define SIGNED_u6p26_T FALSE

typedef unsigned32_T u5p27_T;
#define BPP_u5p27_T    27
#define SIGNED_u5p27_T FALSE

typedef unsigned32_T u4p28_T;
#define BPP_u4p28_T    28
#define SIGNED_u4p28_T FALSE

typedef unsigned32_T u3p29_T;
#define BPP_u3p29_T    29
#define SIGNED_u3p29_T FALSE

typedef unsigned32_T u2p30_T;
#define BPP_u2p30_T    30
#define SIGNED_u2p30_T FALSE

typedef unsigned32_T u1p31_T;
#define BPP_u1p31_T    31
#define SIGNED_u1p31_T FALSE

typedef unsigned32_T u0p32_T;
#define BPP_u0p32_T    32
#define SIGNED_u0p32_T FALSE

/**********************************/
/* Signed 32 bit Generic Types.   */
/**********************************/
typedef signed32_T s31p0_T;
#define BPP_s31p0_T    0
#define SIGNED_s31p0_T TRUE

typedef signed32_T s30p1_T;
#define BPP_s30p1_T    1
#define SIGNED_s30p1_T TRUE

typedef signed32_T s29p2_T;
#define BPP_s29p2_T    2
#define SIGNED_s29p2_T TRUE

typedef signed32_T s28p3_T;
#define BPP_s28p3_T    3
#define SIGNED_s28p3_T TRUE

typedef signed32_T s27p4_T;
#define BPP_s27p4_T    4
#define SIGNED_s27p4_T TRUE

typedef signed32_T s26p5_T;
#define BPP_s26p5_T    5
#define SIGNED_s26p5_T TRUE

typedef signed32_T s25p6_T;
#define BPP_s25p6_T    6
#define SIGNED_s25p6_T TRUE

typedef signed32_T s24p7_T;
#define BPP_s24p7_T    7
#define SIGNED_s24p7_T TRUE

typedef signed32_T s23p8_T;
#define BPP_s23p8_T    8
#define SIGNED_s23p8_T TRUE

typedef signed32_T s22p9_T;
#define BPP_s22p9_T    9
#define SIGNED_s22p9_T TRUE

typedef signed32_T s21p10_T;
#define BPP_s21p10_T    10
#define SIGNED_s21p10_T TRUE

typedef signed32_T s20p11_T;
#define BPP_s20p11_T    11
#define SIGNED_s20p11_T TRUE

typedef signed32_T s19p12_T;
#define BPP_s19p12_T    12
#define SIGNED_s19p12_T TRUE

typedef signed32_T s18p13_T;
#define BPP_s18p13_T    13
#define SIGNED_s18p13_T TRUE

typedef signed32_T s17p14_T;
#define BPP_s17p14_T    14
#define SIGNED_s17p14_T TRUE

typedef signed32_T s16p15_T;
#define BPP_s16p15_T    15
#define SIGNED_s16p15_T TRUE

typedef signed32_T s15p16_T;
#define BPP_s15p16_T    16
#define SIGNED_s15p16_T TRUE

typedef signed32_T s14p17_T;
#define BPP_s14p17_T    17
#define SIGNED_s14p17_T TRUE

typedef signed32_T s13p18_T;
#define BPP_s13p18_T    18
#define SIGNED_s13p18_T TRUE

typedef signed32_T s12p19_T;
#define BPP_s12p19_T    19
#define SIGNED_s12p19_T TRUE

typedef signed32_T s11p20_T;
#define BPP_s11p20_T    20
#define SIGNED_s11p20_T TRUE

typedef signed32_T s10p21_T;
#define BPP_s10p21_T    21
#define SIGNED_s10p21_T TRUE

typedef signed32_T s9p22_T;
#define BPP_s9p22_T    22
#define SIGNED_s9p22_T TRUE

typedef signed32_T s8p23_T;
#define BPP_s8p23_T    23
#define SIGNED_s8p23_T TRUE

typedef signed32_T s7p24_T;
#define BPP_s7p24_T    24
#define SIGNED_s7p24_T TRUE

typedef signed32_T s6p25_T;
#define BPP_s6p25_T    25
#define SIGNED_s6p25_T TRUE

typedef signed32_T s5p26_T;
#define BPP_s5p26_T    26
#define SIGNED_s5p26_T TRUE

typedef signed32_T s4p27_T;
#define BPP_s4p27_T    27
#define SIGNED_s4p27_T TRUE

typedef signed32_T s3p28_T;
#define BPP_s3p28_T    28
#define SIGNED_s3p28_T TRUE

typedef signed32_T s2p29_T;
#define BPP_s2p29_T    29
#define SIGNED_s2p29_T TRUE

typedef signed32_T s1p30_T;
#define BPP_s1p30_T    30
#define SIGNED_s1p30_T TRUE

typedef signed32_T s0p31_T;
#define BPP_s0p31_T    31
#define SIGNED_s0p31_T TRUE

/**********************************/
/* Unsigned 40 bit Generic Types. */
/**********************************/
/* Range from (0p40) 9.094947017729282379150390625e-13 to (40p0)1099511627776 */
typedef unsigned40_T u40p0_T;
#define BPP_u40p0_T    0
#define SIGNED_u40p0_T FALSE

typedef unsigned40_T u39p1_T;
#define BPP_u39p1_T    1
#define SIGNED_u39p1_T FALSE

typedef unsigned40_T u38p2_T;
#define BPP_u38p2_T    2
#define SIGNED_u38p2_T FALSE

typedef unsigned40_T u37p3_T;
#define BPP_u37p3_T    3
#define SIGNED_u37p3_T FALSE

typedef unsigned40_T u36p4_T;
#define BPP_u36p4_T    4
#define SIGNED_u36p4_T FALSE

typedef unsigned40_T u35p5_T;
#define BPP_u35p5_T    5
#define SIGNED_u35p5_T FALSE

typedef unsigned40_T u34p6_T;
#define BPP_u34p6_T    6
#define SIGNED_u34p6_T FALSE

typedef unsigned40_T u33p7_T;
#define BPP_u33p7_T    7
#define SIGNED_u33p7_T FALSE

typedef unsigned40_T u32p8_T;
#define BPP_u32p8_T    8
#define SIGNED_u32p8_T FALSE

typedef unsigned40_T u31p9_T;
#define BPP_u31p9_T    9
#define SIGNED_u31p9_T FALSE

typedef unsigned40_T u30p10_T;
#define BPP_u30p10_T    10
#define SIGNED_u30p10_T FALSE

typedef unsigned40_T u29p11_T;
#define BPP_u29p11_T    11
#define SIGNED_u29p11_T FALSE

typedef unsigned40_T u28p12_T;
#define BPP_u28p12_T    12
#define SIGNED_u28p12_T FALSE

typedef unsigned40_T u27p13_T;
#define BPP_u27p13_T    13
#define SIGNED_u27p13_T FALSE

typedef unsigned40_T u26p14_T;
#define BPP_u26p14_T    14
#define SIGNED_u26p14_T FALSE

typedef unsigned40_T u25p15_T;
#define BPP_u25p15_T    15
#define SIGNED_u25p15_T FALSE

typedef unsigned40_T u24p16_T;
#define BPP_u24p16_T    16
#define SIGNED_u24p16_T FALSE

typedef unsigned40_T u23p17_T;
#define BPP_u23p17_T    17
#define SIGNED_u23p17_T FALSE

typedef unsigned40_T u22p18_T;
#define BPP_u22p18_T    18
#define SIGNED_u22p18_T FALSE

typedef unsigned40_T u21p19_T;
#define BPP_u21p19_T    19
#define SIGNED_u21p19_T FALSE

typedef unsigned40_T u20p20_T;
#define BPP_u20p20_T    20
#define SIGNED_u20p20_T FALSE

typedef unsigned40_T u19p21_T;
#define BPP_u19p21_T    21
#define SIGNED_u19p21_T FALSE

typedef unsigned40_T u18p22_T;
#define BPP_u18p22_T    22
#define SIGNED_u18p22_T FALSE

typedef unsigned40_T u17p23_T;
#define BPP_u17p23_T    23
#define SIGNED_u17p23_T FALSE

typedef unsigned40_T u16p24_T;
#define BPP_u16p24_T    24
#define SIGNED_u16p24_T FALSE

typedef unsigned40_T u15p25_T;
#define BPP_u15p25_T    25
#define SIGNED_u15p25_T FALSE

typedef unsigned40_T u14p26_T;
#define BPP_u14p26_T    26
#define SIGNED_u14p26_T FALSE

typedef unsigned40_T u13p27_T;
#define BPP_u13p27_T    27
#define SIGNED_u13p27_T FALSE

typedef unsigned40_T u12p28_T;
#define BPP_u12p28_T    28
#define SIGNED_u12p28_T FALSE

typedef unsigned40_T u11p29_T;
#define BPP_u11p29_T    29
#define SIGNED_u11p29_T FALSE

typedef unsigned40_T u10p30_T;
#define BPP_u10p30_T    30
#define SIGNED_u10p30_T FALSE

typedef unsigned40_T u9p31_T;
#define BPP_u9p31_T    31
#define SIGNED_u9p31_T FALSE

typedef unsigned40_T u8p32_T;
#define BPP_u8p32_T    32
#define SIGNED_u8p32_T FALSE

typedef unsigned40_T u7p33_T;
#define BPP_u7p33_T    33
#define SIGNED_u7p33_T FALSE

typedef unsigned40_T u6p34_T;
#define BPP_u6p34_T    34
#define SIGNED_u6p34_T FALSE

typedef unsigned40_T u5p35_T;
#define BPP_u5p35_T    35
#define SIGNED_u5p35_T FALSE

typedef unsigned40_T u4p36_T;
#define BPP_u4p36_T    36
#define SIGNED_u4p36_T FALSE

typedef unsigned40_T u3p37_T;
#define BPP_u3p37_T    37
#define SIGNED_u3p37_T FALSE

typedef unsigned40_T u2p38_T;
#define BPP_u2p38_T    38
#define SIGNED_u2p38_T FALSE

typedef unsigned40_T u1p39_T;
#define BPP_u1p39_T    39
#define SIGNED_u1p39_T FALSE

typedef unsigned40_T u0p40_T;
#define BPP_u0p40_T    40
#define SIGNED_u0p40_T FALSE

/**********************************/
/* Signed 40 bit Generic Types. */
/**********************************/

typedef signed40_T s39p0_T;
#define BPP_s39p0_T    0
#define SIGNED_s39p0_T TRUE

typedef signed40_T s38p1_T;
#define BPP_s38p1_T    1
#define SIGNED_s38p1_T TRUE

typedef signed40_T s37p2_T;
#define BPP_s37p2_T    2
#define SIGNED_s37p2_T TRUE

typedef signed40_T s36p3_T;
#define BPP_s36p3_T    3
#define SIGNED_s36p3_T TRUE

typedef signed40_T s35p4_T;
#define BPP_s35p4_T    4
#define SIGNED_s35p4_T TRUE

typedef signed40_T s34p5_T;
#define BPP_s34p5_T    5
#define SIGNED_s34p5_T TRUE

typedef signed40_T s33p6_T;
#define BPP_s33p6_T    6
#define SIGNED_s33p6_T TRUE

typedef signed40_T s32p7_T;
#define BPP_s32p7_T    7
#define SIGNED_s32p7_T TRUE

typedef signed40_T s31p8_T;
#define BPP_s31p8_T    8
#define SIGNED_s31p8_T TRUE

typedef signed40_T s30p9_T;
#define BPP_s30p9_T    9
#define SIGNED_s30p9_T TRUE

typedef signed40_T s29p10_T;
#define BPP_s29p10_T    10
#define SIGNED_s29p10_T TRUE

typedef signed40_T s28p11_T;
#define BPP_s28p11_T    11
#define SIGNED_s28p11_T TRUE

typedef signed40_T s27p12_T;
#define BPP_s27p12_T    12
#define SIGNED_s27p12_T TRUE

typedef signed40_T s26p13_T;
#define BPP_s26p13_T    13
#define SIGNED_s26p13_T TRUE

typedef signed40_T s25p14_T;
#define BPP_s25p14_T    14
#define SIGNED_s25p14_T TRUE

typedef signed40_T s24p15_T;
#define BPP_s24p15_T    15
#define SIGNED_s24p15_T TRUE

typedef signed40_T s23p16_T;
#define BPP_s23p16_T    16
#define SIGNED_s23p16_T TRUE

typedef signed40_T s22p17_T;
#define BPP_s22p17_T    17
#define SIGNED_s22p17_T TRUE

typedef signed40_T s21p18_T;
#define BPP_s21p18_T    18
#define SIGNED_s21p18_T TRUE

typedef signed40_T s20p19_T;
#define BPP_s20p19_T    19
#define SIGNED_s20p19_T TRUE

typedef signed40_T s19p20_T;
#define BPP_s19p20_T    20
#define SIGNED_s19p20_T TRUE

typedef signed40_T s18p21_T;
#define BPP_s18p21_T    21
#define SIGNED_s18p21_T TRUE

typedef signed40_T s17p22_T;
#define BPP_s17p22_T    22
#define SIGNED_s17p22_T TRUE

typedef signed40_T s16p23_T;
#define BPP_s16p23_T    23
#define SIGNED_s16p23_T TRUE

typedef signed40_T s15p24_T;
#define BPP_s15p24_T    24
#define SIGNED_s15p24_T TRUE

typedef signed40_T s14p25_T;
#define BPP_s14p25_T    25
#define SIGNED_s14p25_T TRUE

typedef signed40_T s13p26_T;
#define BPP_s13p26_T    26
#define SIGNED_s13p26_T TRUE

typedef signed40_T s12p27_T;
#define BPP_s12p27_T    27
#define SIGNED_s12p27_T TRUE

typedef signed40_T s11p28_T;
#define BPP_s11p28_T    28
#define SIGNED_s11p28_T TRUE

typedef signed40_T s10p29_T;
#define BPP_s10p29_T    29
#define SIGNED_s10p29_T TRUE

typedef signed40_T s9p30_T;
#define BPP_s9p30_T    30
#define SIGNED_s9p30_T TRUE

typedef signed40_T s8p31_T;
#define BPP_s8p31_T    31
#define SIGNED_s8p31_T TRUE

typedef signed40_T s7p32_T;
#define BPP_s7p32_T    32
#define SIGNED_s7p32_T TRUE

typedef signed40_T s6p33_T;
#define BPP_s6p33_T    33
#define SIGNED_s6p33_T TRUE

typedef signed40_T s5p34_T;
#define BPP_s5p34_T    34
#define SIGNED_s5p34_T TRUE

typedef signed40_T s4p35_T;
#define BPP_s4p35_T    35
#define SIGNED_s4p35_T TRUE

typedef signed40_T s3p36_T;
#define BPP_s3p36_T    36
#define SIGNED_s3p36_T TRUE

typedef signed40_T s2p37_T;
#define BPP_s2p37_T    37
#define SIGNED_s2p37_T TRUE

typedef signed40_T s1p38_T;
#define BPP_s1p38_T    38
#define SIGNED_s1p38_T TRUE

typedef signed40_T s0p39_T;
#define BPP_s0p39_T    39
#define SIGNED_s0p39_T TRUE

/**********************************/
/* Unsigned 64 bit Generic Types. */
/**********************************/
typedef unsigned64_T u64p0_T;
#define BPP_u64p0_T    0
#define SIGNED_u64p0_T FALSE

typedef unsigned64_T u63p1_T;
#define BPP_u63p1_T    1
#define SIGNED_u63p1_T FALSE

typedef unsigned64_T u62p2_T;
#define BPP_u62p2_T    2
#define SIGNED_u62p2_T FALSE

typedef unsigned64_T u61p3_T;
#define BPP_u61p3_T    3
#define SIGNED_u61p3_T FALSE

typedef unsigned64_T u60p4_T;
#define BPP_u60p4_T    4
#define SIGNED_u60p4_T FALSE

typedef unsigned64_T u59p5_T;
#define BPP_u59p5_T    5
#define SIGNED_u59p5_T FALSE

typedef unsigned64_T u58p6_T;
#define BPP_u58p6_T    6
#define SIGNED_u58p6_T FALSE

typedef unsigned64_T u57p7_T;
#define BPP_u57p7_T    7
#define SIGNED_u57p7_T FALSE

typedef unsigned64_T u56p8_T;
#define BPP_u56p8_T    8
#define SIGNED_u56p8_T FALSE

typedef unsigned64_T u55p9_T;
#define BPP_u55p9_T    9
#define SIGNED_u55p9_T FALSE

typedef unsigned64_T u54p10_T;
#define BPP_u54p10_T    10
#define SIGNED_u54p10_T FALSE

typedef unsigned64_T u53p11_T;
#define BPP_u53p11_T    11
#define SIGNED_u53p11_T FALSE

typedef unsigned64_T u52p12_T;
#define BPP_u52p12_T    12
#define SIGNED_u52p12_T FALSE

typedef unsigned64_T u51p13_T;
#define BPP_u51p13_T    13
#define SIGNED_u51p13_T FALSE

typedef unsigned64_T u50p14_T;
#define BPP_u50p14_T    14
#define SIGNED_u50p14_T FALSE

typedef unsigned64_T u49p15_T;
#define BPP_u49p15_T    15
#define SIGNED_u49p15_T FALSE

typedef unsigned64_T u48p16_T;
#define BPP_u48p16_T    16
#define SIGNED_u48p16_T FALSE

typedef unsigned64_T u47p17_T;
#define BPP_u47p17_T    17
#define SIGNED_u47p17_T FALSE

typedef unsigned64_T u46p18_T;
#define BPP_u46p18_T    18
#define SIGNED_u46p18_T FALSE

typedef unsigned64_T u45p19_T;
#define BPP_u45p19_T    19
#define SIGNED_u45p19_T FALSE

typedef unsigned64_T u44p20_T;
#define BPP_u44p20_T    20
#define SIGNED_u44p20_T FALSE

typedef unsigned64_T u43p21_T;
#define BPP_u43p21_T    21
#define SIGNED_u43p21_T FALSE

typedef unsigned64_T u42p22_T;
#define BPP_u42p22_T    22
#define SIGNED_u42p22_T FALSE

typedef unsigned64_T u41p23_T;
#define BPP_u41p23_T    23
#define SIGNED_u41p23_T FALSE

typedef unsigned64_T u40p24_T;
#define BPP_u40p24_T    24
#define SIGNED_u40p24_T FALSE

typedef unsigned64_T u39p25_T;
#define BPP_u39p25_T    25
#define SIGNED_u39p25_T FALSE

typedef unsigned64_T u38p26_T;
#define BPP_u38p26_T    26
#define SIGNED_u38p26_T FALSE

typedef unsigned64_T u37p27_T;
#define BPP_u37p27_T    27
#define SIGNED_u37p27_T FALSE

typedef unsigned64_T u36p28_T;
#define BPP_u36p28_T    28
#define SIGNED_u36p28_T FALSE

typedef unsigned64_T u35p29_T;
#define BPP_u35p29_T    29
#define SIGNED_u35p29_T FALSE

typedef unsigned64_T u34p30_T;
#define BPP_u34p30_T    30
#define SIGNED_u34p30_T FALSE

typedef unsigned64_T u33p31_T;
#define BPP_u33p31_T    31
#define SIGNED_u33p31_T FALSE

typedef unsigned64_T u32p32_T;
#define BPP_u32p32_T    32
#define SIGNED_u32p32_T FALSE

typedef unsigned64_T u31p33_T;
#define BPP_u31p33_T    33
#define SIGNED_u31p33_T FALSE

typedef unsigned64_T u30p34_T;
#define BPP_u30p34_T    34
#define SIGNED_u30p34_T FALSE

typedef unsigned64_T u29p35_T;
#define BPP_u29p35_T    35
#define SIGNED_u29p35_T FALSE

typedef unsigned64_T u28p36_T;
#define BPP_u28p36_T    36
#define SIGNED_u28p36_T FALSE

typedef unsigned64_T u27p37_T;
#define BPP_u27p37_T    37
#define SIGNED_u27p37_T FALSE

typedef unsigned64_T u26p38_T;
#define BPP_u26p38_T    38
#define SIGNED_u26p38_T FALSE

typedef unsigned64_T u25p39_T;
#define BPP_u25p39_T    39
#define SIGNED_u25p39_T FALSE

typedef unsigned64_T u24p40_T;
#define BPP_u24p40_T    40
#define SIGNED_u24p40_T FALSE

/**********************************/
/* Signed 64 bit Generic Types. */
/**********************************/

typedef signed64_T s63p0_T;
#define BPP_s63p0_T    0
#define SIGNED_s63p0_T TRUE

typedef signed64_T s62p1_T;
#define BPP_s62p1_T    1
#define SIGNED_s62p1_T TRUE

typedef signed64_T s61p2_T;
#define BPP_s61p2_T    2
#define SIGNED_s61p2_T TRUE

typedef signed64_T s60p3_T;
#define BPP_s60p3_T    3
#define SIGNED_s60p3_T TRUE

typedef signed64_T s59p4_T;
#define BPP_s59p4_T    4
#define SIGNED_s59p4_T TRUE

typedef signed64_T s58p5_T;
#define BPP_s58p5_T    5
#define SIGNED_s58p5_T TRUE

typedef signed64_T s57p6_T;
#define BPP_s57p6_T    6
#define SIGNED_s57p6_T TRUE

typedef signed64_T s56p7_T;
#define BPP_s56p7_T    7
#define SIGNED_s56p7_T TRUE

typedef signed64_T s55p8_T;
#define BPP_s55p8_T    8
#define SIGNED_s55p8_T TRUE

typedef signed64_T s54p9_T;
#define BPP_s54p9_T    9
#define SIGNED_s54p9_T TRUE

typedef signed64_T s53p10_T;
#define BPP_s53p10_T    10
#define SIGNED_s53p10_T TRUE

typedef signed64_T s52p11_T;
#define BPP_s52p11_T    11
#define SIGNED_s52p11_T TRUE

typedef signed64_T s51p12_T;
#define BPP_s51p12_T    12
#define SIGNED_s51p12_T TRUE

typedef signed64_T s50p13_T;
#define BPP_s50p13_T    13
#define SIGNED_s50p13_T TRUE

typedef signed64_T s49p14_T;
#define BPP_s49p14_T    14
#define SIGNED_s49p14_T TRUE

typedef signed64_T s48p15_T;
#define BPP_s48p15_T    15
#define SIGNED_s48p15_T TRUE

typedef signed64_T s47p16_T;
#define BPP_s47p16_T    16
#define SIGNED_s47p16_T TRUE

typedef signed64_T s46p17_T;
#define BPP_s46p17_T    17
#define SIGNED_s46p17_T TRUE

typedef signed64_T s45p18_T;
#define BPP_s45p18_T    18
#define SIGNED_s45p18_T TRUE

typedef signed64_T s44p19_T;
#define BPP_s44p19_T    19
#define SIGNED_s44p19_T TRUE

typedef signed64_T s43p20_T;
#define BPP_s43p20_T    20
#define SIGNED_s43p20_T TRUE

typedef signed64_T s42p21_T;
#define BPP_s42p21_T    21
#define SIGNED_s42p21_T TRUE

typedef signed64_T s41p22_T;
#define BPP_s41p22_T    22
#define SIGNED_s41p22_T TRUE

typedef signed64_T s40p23_T;
#define BPP_s40p23_T    23
#define SIGNED_s40p23_T TRUE

typedef signed64_T s39p24_T;
#define BPP_s39p24_T    24
#define SIGNED_s39p24_T TRUE

typedef signed64_T s38p25_T;
#define BPP_s38p25_T    25
#define SIGNED_s38p25_T TRUE

typedef signed64_T s37p26_T;
#define BPP_s37p26_T    26
#define SIGNED_s37p26_T TRUE

typedef signed64_T s36p27_T;
#define BPP_s36p27_T    27
#define SIGNED_s36p27_T TRUE

typedef signed64_T s35p28_T;
#define BPP_s35p28_T    28
#define SIGNED_s35p28_T TRUE

typedef signed64_T s34p29_T;
#define BPP_s34p29_T    29
#define SIGNED_s34p29_T TRUE

typedef signed64_T s33p30_T;
#define BPP_s33p30_T    30
#define SIGNED_s33p30_T TRUE

typedef signed64_T s32p31_T;
#define BPP_s32p31_T    31
#define SIGNED_s32p31_T TRUE

typedef signed64_T s31p32_T;
#define BPP_s31p32_T    32
#define SIGNED_s31p32_T TRUE

typedef signed64_T s30p33_T;
#define BPP_s30p33_T    33
#define SIGNED_s30p33_T TRUE

typedef signed64_T s29p34_T;
#define BPP_s29p34_T    34
#define SIGNED_s29p34_T TRUE

typedef signed64_T s28p35_T;
#define BPP_s28p35_T    35
#define SIGNED_s28p35_T TRUE

typedef signed64_T s27p36_T;
#define BPP_s27p36_T    36
#define SIGNED_s27p36_T TRUE

typedef signed64_T s26p37_T;
#define BPP_s26p37_T    37
#define SIGNED_s26p37_T TRUE

typedef signed64_T s25p38_T;
#define BPP_s25p38_T    38
#define SIGNED_s25p38_T TRUE

typedef signed64_T s24p39_T;
#define BPP_s24p39_T    39
#define SIGNED_s24p39_T TRUE

typedef signed64_T s23p40_T;
#define BPP_s23p40_T    40
#define SIGNED_s23p40_T TRUE

typedef signed64_T s22p41_T;
#define BPP_s22p41_T    41
#define SIGNED_s22p41_T TRUE

typedef signed64_T s21p42_T;
#define BPP_s21p42_T    42
#define SIGNED_s21p42_T TRUE

typedef signed64_T s20p43_T;
#define BPP_s20p43_T    43
#define SIGNED_s20p43_T TRUE

typedef signed64_T s19p44_T;
#define BPP_s19p44_T    44
#define SIGNED_s19p44_T TRUE

typedef signed64_T s18p45_T;
#define BPP_s18p45_T    45
#define SIGNED_s18p45_T TRUE

typedef signed64_T s17p46_T;
#define BPP_s17p46_T    46
#define SIGNED_s17p46_T TRUE

typedef signed64_T s16p47_T;
#define BPP_s16p47_T    47
#define SIGNED_s16p47_T TRUE

typedef signed64_T s15p48_T;
#define BPP_s15p48_T    48
#define SIGNED_s15p48_T TRUE

typedef signed64_T s14p49_T;
#define BPP_s14p49_T    49
#define SIGNED_s14p49_T TRUE

typedef signed64_T s13p50_T;
#define BPP_s13p50_T    50
#define SIGNED_s13p50_T TRUE

typedef signed64_T s12p51_T;
#define BPP_s12p51_T    51
#define SIGNED_s12p51_T TRUE

typedef signed64_T s11p52_T;
#define BPP_s11p52_T    52
#define SIGNED_s11p52_T TRUE

typedef signed64_T s10p53_T;
#define BPP_s10p53_T    53
#define SIGNED_s10p53_T TRUE

typedef signed64_T s9p54_T;
#define BPP_s9p54_T    54
#define SIGNED_s9p54_T TRUE

typedef signed64_T s8p55_T;
#define BPP_s8p55_T    55
#define SIGNED_s8p55_T TRUE

typedef signed64_T s7p56_T;
#define BPP_s7p56_T    56
#define SIGNED_s7p56_T TRUE

typedef signed64_T s6p57_T;
#define BPP_s6p57_T    57
#define SIGNED_s6p57_T TRUE

typedef signed64_T s5p58_T;
#define BPP_s5p58_T    58
#define SIGNED_s5p58_T TRUE

typedef signed64_T s4p59_T;
#define BPP_s4p59_T    59
#define SIGNED_s4p59_T TRUE

typedef signed64_T s3p60_T;
#define BPP_s3p60_T    60
#define SIGNED_s3p60_T TRUE

typedef signed64_T s2p61_T;
#define BPP_s2p61_T    61
#define SIGNED_s2p61_T TRUE

typedef signed64_T s1p62_T;
#define BPP_s1p62_T    62
#define SIGNED_s1p62_T TRUE

#ifdef PC_RESIMULATION
#define CALCONST
#else
#define CALCONST const
#endif
/*.***************************************************************************
 *. Name: Init_Fixed_Point    MACRO
 *.   Initialize a value, given in engineering units, to fixed point
 *.   representation.
 *.
 *.   The fixed point representation is limited to -31<=Binary
 *.   Position Point(BPP) <= 31. For example, it can respresnts u1p31_T,
 *.   but it can not represent u0p32_T.  Use Init_Fixed_Point_Ext if BBP is
 *.   outside ot the limit.
 *.
 *.   Warning: The parameters must be known at compile time to avoid
 *.   using floating point functions at runtime.
 *.   MUST use decimal point in any fraction argument to Init_Fixed_Point
 *.   or argument will be truncated to ZERO.  Eg.  1/68 will be turned into
 *.   0 and THEN passed to Init_Fixed_Point
 *.
 *.  This routine converts a floating point number to the nearest integer
 *.  that represents that float in the type specified. The cosmic compiler
 *.  rounds positive floating point numbers down to the nearest interger,
 *.  and rounds negative floating point number up (toward zero) to the nearest
 *.  integer. Therefore, the rounding bit (0.5) needs to be added for positive
 *.  values and subtracted for negative values.
 *.
 *. Shared Variables: none.
 *. Parameters: value: the input value or variable.
 *.             to_type: type to convert to.
 *.
 *. Return Value: The value in to_type form.
 *.
 *.
 *. Updated: 10/30/98 fgh
 *
 *  ACC3 SCRs: #007, #692, #3401
 ******************************************************************************/
#define Init_Fixed_Point(value, to_type) (((value) >= 0) ? (Init_Pos_Fixed_Point((value), to_type)) : (Init_Neg_Fixed_Point((value), to_type)))

#define Init_Neg_Fixed_Point(value, to_type) (((BPP_##to_type) > 0) ? ((to_type)((((float)(value)) * (float)(1ull << Max(BPP_##to_type, 0))) - 0.5)) : ((to_type)((((float)(value)) / (float)(1ull << Max(-1 * (BPP_##to_type), 0))) - 0.5)))

#define Init_Pos_Fixed_Point(value, to_type) (((BPP_##to_type) > 0) ? ((to_type)((((float)(value)) * (float)(1ull << Max(BPP_##to_type, 0))) + 0.5)) : ((to_type)((((float)(value)) / (float)(1ull << Max(-1 * (BPP_##to_type), 0))) + 0.5)))

/*.***************************************************************************
 *. Name: Init_Fixed_Point_Ext    MACRO
 *.   Initialize a value, given in engineering units, to fixed point
 *.   representation.
 *.   The fixed point representation is limited to 31<=Binary
 *.   Position Point(BPP) <= 62, and -62<= BBP <=-31. For example, it can
 *.   respresnts um30p62_T, but not um31p63_T
 *.
 *.   Warning: The parameters must be known at compile time to avoid
 *.   using floating point functions at runtime.
 *.   MUST use decimal point in any fraction argument to Init_Fixed_Point
 *.   or argument will be truncated to ZERO.  Eg.  1/68 will be turned into
 *.   0 and THEN passed to Init_Fixed_Point
 *.
 *.  This routine converts a floating point number to the nearest integer
 *.  that represents that float in the type specified. The cosmic compiler
 *.  rounds positive floating point numbers down to the nearest interger,
 *.  and rounds negative floating point number up (toward zero) to the nearest
 *.  integer. Therefore, the rounding bit (0.5) needs to be added for positive
 *.  values and subtracted for negative values.
 *.
 *. Shared Variables: none.
 *. Parameters: value: the input value or variable.
 *.             to_type: type to convert to.
 *.
 *. Return Value: The value in to_T form.
 *.
 *.
 *. Updated: 4/1/03 sd
 *
 *  ACC3 SCRs: #3401
 ******************************************************************************/
#define Init_Fixed_Point_Ext(value, to_T) (((value) >= 0) ? (Init_Pos_Fixed_Point_Ext((value), to_T)) : (Init_Neg_Fixed_Point_Ext((value), to_T)))

#define Init_Neg_Fixed_Point_Ext(value, to_T) (((BPP_##to_T) > 0) ? ((to_T)((value) * ((double)(1ull << 31)) * ((double)(1ull << (BPP_##to_T - 31))) - 0.5)) : ((to_T)((value) / ((double)(1ull << 31)) / ((double)(1ull << (-BPP_##to_T - 31))) - 0.5)))

#define Init_Pos_Fixed_Point_Ext(value, to_T) (((BPP_##to_T) > 0) ? ((to_T)((value) * ((double)(1ull << 31)) * ((double)(1ull << (BPP_##to_T - 31))) + 0.5)) : ((to_T)((value) / ((double)(1ull << 31)) / ((double)(1ull << (-BPP_##to_T - 31))) + 0.5)))

/* Reuse from CADS 2 program */
/*.***************************************************************************
 *. Name: Fix    MACRO
 *.   Fix one fixed point type to another. Returns new type's value.
 *.   If fixing from a signed to unsigned type and the value is negative,
 *.   the value returned is 0.
 *.
 *.   Warning: An incorrect result can easily occur if fixing from unsigned
 *.   to signed type.  (i.e. unsigned 255 = signed -1 ! ).
 *.
 *.   Does not check sizes. User must insure value will fit.
 *.
 *. Shared Variables: none.
 *. Parameters: value: the input value or variable.
 *.             from_type: type of inputed value or variable.
 *.             to_type: type to convert to.
 *.
 *. Return Value: The value in to_type form.
 *.
 *.
 ******************************************************************************/
#define Fix(value, from_type, to_type)                                                                                                                                         \
   (/* If from_type is signed and to_type is unsigned then call Signed_To_Unsigned_Fix */                                                                                      \
    ((SIGNED_##from_type) && (!SIGNED_##to_type)) ?                                                                                                                            \
                                                                                                                                                                               \
                                                  /* If the size of fromType is less than size of toType then Cast first */                                                    \
        ((sizeof(from_type) < sizeof(to_type)) ? Signed_To_Unsigned_Fix((to_type)(value), BPP_##from_type, BPP_##to_type) :                                                    \
                                                                                                                                                                               \
                                               /* Otherwise,  cast after. */                                                                                                   \
             (to_type)Signed_To_Unsigned_Fix((value), BPP_##from_type, BPP_##to_type))                                                                                         \
                                                  :                                                                                                                            \
                                                                                                                                                                               \
                                                  /* Else call unsigned Unsigned_To_Unsigned_Fix. */ /* If the size of fromType is less than size of toType then Cast first */ \
        ((sizeof(from_type) < sizeof(to_type)) ? Unsigned_To_Unsigned_Fix((to_type)(value), BPP_##from_type, BPP_##to_type) :                                                  \
                                                                                                                                                                               \
                                               /* Otherwise,  cast after. */                                                                                                   \
             (to_type)Unsigned_To_Unsigned_Fix((value), BPP_##from_type, BPP_##to_type)))

/* PRIVATE: Use Fix above. */
#define Unsigned_To_Unsigned_Fix(value, from_type_bpp, to_type_bpp) \
   (((from_type_bpp) > (to_type_bpp)) ? ((value) >> (Max(((from_type_bpp) - (to_type_bpp)), 0))) : ((value) << (Max(((to_type_bpp) - (from_type_bpp)), 0))))

/* PRIVATE: Use Fix above. */
#define Signed_To_Unsigned_Fix(value, from_type_bpp, to_type_bpp) \
   (((value) < 0) ? 0 : (Unsigned_To_Unsigned_Fix((value), from_type_bpp, to_type_bpp)))

/*======================================================================*
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*
*                       Inline functions
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*
;*======================================================================*/
/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         u8 x u8 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/
/*.===================================================================*\
*. FUNCTION: U8L_Mul_U8_U8
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  u8 * u8 = u8L  Returns lower 8 bits of the u16 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

/*
INLINE unsigned8_T U8L_Mul_U8_U8(unsigned8_T u8_1,unsigned8_T u8_2)
 { return ( (unsigned8_T)((unsigned32_T)u8_1 * u8_2 ) );}
*/

#define U8L_Mul_U8_U8(u8_1, u8_2) ((unsigned8_T)((unsigned32_T)(u8_1) * (u8_2)))

/*.===================================================================*\
*. FUNCTION: U8H_Mul_U8_U8
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*. u8 * u8 = u16   Returns high 8 bits of the u16 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

/*
INLINE unsigned8_T U8H_Mul_U8_U8(unsigned8_T u8_1,unsigned8_T u8_2)
{return((unsigned8_T)(((unsigned32_T)u8_1 * u8_2 )>>8 ));}
*/

#define U8H_Mul_U8_U8(u8_1, u8_2) ((unsigned8_T)(((unsigned32_T)(u8_1) * (u8_2)) >> 8))

/*.===================================================================*\
*. FUNCTION: U16_Mul_U8_U8
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * u8 = u16   Returns the u16 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

/*
INLINE unsigned16_T U16_Mul_U8_U8(unsigned8_T u8_1,unsigned8_T u8_2)
{ return( ((unsigned16_T)(unsigned32_T)u8_1 * u8_2 ));}
*/

#define U16_Mul_U8_U8(u8_1, u8_2) ((unsigned16_T)((unsigned32_T)(u8_1) * (u8_2)))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         u8 x s8 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION: S8H_Mul_U8_S8  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * s8 = s16   Returns high 8 bits of the s16 result.
*.   Variable sizes must match.
*.
\*===================================================================*/
/*
INLINE signed8_T S8H_Mul_U8_S8(unsigned8_T u8, signed8_T s8)
{ return( (signed8_T)( (u8 * (signed32_T)s8)>>8 ));}
*/

#define S8H_Mul_U8_S8(u8, s8) ((signed8_T)(((u8) * (signed32_T)(s8)) >> 8))

/*.===================================================================*\
*. FUNCTION: S8L_Mul_U8_S8  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * s8 = s16   Returns high 8 bits of the s16 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed8_T S8L_Mul_U8_S8(unsigned8_T u8, signed8_T s8)
{ return((signed8_T)(u8 * (signed32_T)s8));}
*/
#define S8L_Mul_U8_S8(u8, s8) ((signed8_T)((u8) * (signed32_T)(s8)))

/*.===================================================================*\
*. FUNCTION: S16_Mul_U8_S8  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * s8 = s16   Returns the s16 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16_Mul_U8_S8(unsigned8_T u8, signed8_T s8)
{ return( (signed16_T) (u8 * (signed32_T)s8) );}
*/

#define S16_Mul_U8_S8(u8, s8) ((signed16_T)((u8) * (signed32_T)(s8)))

#define S16_Mul_S16_U8(s16, u8) ((signed16_T)((u8) * (signed32_T)(s16)))
/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         s8 x s8 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION: S8H_Mul_S8_S8  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s8 * s8 = s16   Returns the high 8 bits of the s16 result.
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE signed8_T S8H_Mul_S8_S8(signed8_T s8_1, signed8_T s8_2)
{ return((signed8_T)(((signed32_T)s8_1 * s8_2)>>8) );}
*/

#define S8H_Mul_S8_S8(s8_1, s8_2) ((signed8_T)(((signed32_T)(s8_1) * (s8_2)) >> 8))

/*.===================================================================*\
*. FUNCTION: S8L_Mul_S8_S8  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s8 * s8 = s16   Returns the low 8 bits of the s16 result.
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE signed8_T S8L_Mul_S8_S8(signed8_T s8_1, signed8_T s8_2)
{ return((signed8_T)((signed32_T)s8_1 * s8_2) );}
*/
#define S8L_Mul_S8_S8(s8_1, s8_2) ((signed8_T)((signed32_T)(s8_1) * (s8_2)))

/*.===================================================================*\
*. FUNCTION: S16_Mul_S8_S8  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s8 * s8 = s16   Returns the s16 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16_Mul_S8_S8(signed8_T s8_1, signed8_T s8_2)
{ return((signed16_T)((signed32_T)s8_1 * s8_2) );}
*/

#define S16_Mul_S8_S8(s8_1, s8_2) ((signed16_T)((signed32_T)(s8_1) * (s8_2)))
#define S16_Mul_S8_U8(s8_1, u8_2) ((signed16_T)((signed32_T)(s8_1) * (u8_2)))
/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         u8 x u16 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION: U16L_Mul_U8_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  u8 * u16 = u24   Returns low 16 bits of the u24 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned16_T U16L_Mul_U8_U16(unsigned8_T u8, unsigned16_T u16)
{ return((unsigned16_T)((unsigned32_T)u8 * u16) );}
*/

#define U16L_Mul_U8_U16(u8, u16) ((unsigned16_T)((unsigned32_T)(u8) * (u16)))

/*.===================================================================*\
*. FUNCTION: U16H_Mul_U8_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  u8 * u16 = u24 Returns high 16 bits of the u24 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned16_T U16H_Mul_U8_U16(unsigned8_T u8, unsigned16_T u16)
{ return((unsigned16_T)(((unsigned32_T)u8 * u16)>>16) );}
*/

#define U16H_Mul_U8_U16(u8, u16) ((unsigned16_T)(((unsigned32_T)(u8) * (u16)) >> 16))

/*.===================================================================*\
*. FUNCTION: U32_Mul_U8_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  u8 * u16 = u24   Returns u32 type with 24 bit result. Hign 8 bits are 0.
*.   Variable sizes must match
\*===================================================================*/
/*
INLINE unsigned32_T U32_Mul_U8_U16(unsigned8_T u8, unsigned16_T u16)
{ return((unsigned32_T)((unsigned32_T)u8 * u16) );}
*/

#define U32_Mul_U8_U16(u8, u16) ((unsigned32_T)((unsigned32_T)(u8) * (u16)))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         s8 x u16 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION: S16L_Mul_S8_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s8 * u16 = s24   Returns low 16 bits of the s24 result.
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE signed16_T S16L_Mul_S8_U16(signed8_T s8, unsigned16_T u16)
{ return((signed16_T)((signed32_T)s8 * u16) );}
*/

#define S16L_Mul_S8_U16(s8, u16) ((signed16_T)((signed32_T)(s8) * (u16)))

/*.===================================================================*\
*. FUNCTION: S16H_Mul_S8_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s8 * u16 = s24   Returns high 16 bits of the s24 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16H_Mul_S8_U16(signed8_T s8, unsigned16_T u16)
{ return((signed16_T)(((signed32_T)s8 * u16)>>16));}
*/

#define S16H_Mul_S8_U16(s8, u16) ((signed16_T)(((signed32_T)(s8) * (u16)) >> 16))

/*.===================================================================*\
*. FUNCTION: S32_Mul_S8_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s8 * u16 = s24   Returns s32 type with s24 bit result. High 8 bits are
*.   sign extended. Variable sizes must match.
\*===================================================================*/
/*
INLINE signed32_T S32_Mul_S8_U16(signed8_T s8, unsigned16_T u16)
{ return(((signed32_T)s8 * u16) );}
*/

#define S32_Mul_S8_U16(s8, u16) (((signed32_T)(s8) * (u16)))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         u8 x s16 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION: S16L_Mul_U8_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * s16 = s24   Returns low 16 bits of the s24 result.
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE signed16_T S16L_Mul_U8_S16 (unsigned8_T u8, signed16_T s16)
{ return((signed16_T)(u8 *(signed32_T)s16) );}
*/

#define S16L_Mul_U8_S16 (u8, s16)((signed16_T)((u8) * (signed32_T)(s16)))

/*.===================================================================*\
*. FUNCTION: S16H_Mul_U8_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * s16 = s24   Returns high 16 bits of the s24 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16H_Mul_U8_S16(unsigned8_T u8, signed16_T s16)
{ return((signed16_T)((u8 *(signed32_T)s16)>>16) );}
*/

#define S16H_Mul_U8_S16(u8, s16) ((signed16_T)(((u8) * (signed32_T)(s16)) >> 16))

/*.===================================================================*\
*. FUNCTION: S32_Mul_U8_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u8 * s16 = s24   Returns s32 type with s24 bit result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed32_T S32_Mul_U8_S16(unsigned8_T u8, signed16_T s16)
{ return((u8 *(signed32_T)s16) );}
*/

#define S32_Mul_U8_S16(u8, s16) (((u8) * (signed32_T)(s16)))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         u16 x u16 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION: U16L_Mul_U16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u16 * u16 = u32   Returns the low 16 bits of the u32 result.
*.   Variable sizes must match
\*===================================================================*/
/*
INLINE unsigned16_T U16L_Mul_U16_U16(unsigned16_T u16_1, unsigned16_T u16_2)
{ return((unsigned16_T)((unsigned32_T)u16_1 *u16_2) );}
*/

#define U16L_Mul_U16_U16(u16_1, u16_2) ((unsigned16_T)((unsigned32_T)(u16_1) * (u16_2)))

/*.===================================================================*\
*. FUNCTION: U16M_Mul_U16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u16 * u16 = u32   Returns the middle 16 bits of the u32 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned16_T U16M_Mul_U16_U16 (unsigned16_T u16_1, unsigned16_T u16_2)
{ return((unsigned16_T)(((unsigned32_T)u16_1 *u16_2)>>8) );}
*/

#define U16M_Mul_U16_U16(u16_1, u16_2) ((unsigned16_T)(((unsigned32_T)(u16_1) * (u16_2)) >> 8))

/*.===================================================================*\
*. FUNCTION: U16H_Mul_U16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u16 * u16 = u32   Returns the high 16 bits of the u32 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned16_T U16H_Mul_U16_U16 (unsigned16_T u16_1, unsigned16_T u16_2)
{ return((unsigned16_T)(((unsigned32_T)u16_1 *u16_2)>>16) );}
*/

#define U16H_Mul_U16_U16(u16_1, u16_2) ((unsigned16_T)(((unsigned32_T)(u16_1) * (u16_2)) >> 16))

/*.===================================================================*\
*. FUNCTION:U32_Mul_U16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u16 * u16 = u32   Returns u32 type with 32 bit result.
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE unsigned32_T U32_Mul_U16_U16 (unsigned16_T u16_1, unsigned16_T u16_2)
{ return(((unsigned32_T)u16_1 *u16_2) );}
*/

#define U32_Mul_U16_U16(u16_1, u16_2) ((unsigned32_T)(u16_1) * (u16_2))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         s16 x u16 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION:S16L_Mul_S16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s16 * u16 = s32   Returns the low 16 bits of the s32 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16L_Mul_S16_U16 (signed16_T s16, unsigned16_T u16)
{ return((signed16_T)((signed32_T)s16 *u16) );}
*/

#define S16L_Mul_S16_U16(s16, u16) ((signed16_T)((signed32_T)(s16) * (u16)))

/*.===================================================================*\
*. FUNCTION:S16M_Mul_S16_U16  Inline
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s16 * u16 = s32   Returns the middle 16 bits of the s32 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16M_Mul_S16_U16 (signed16_T s16, unsigned16_T u16)
{ return((signed16_T)(((signed32_T)s16 *u16)>>8) );}
*/

#define S16M_Mul_S16_U16(s16, u16) ((signed16_T)(((signed32_T)(s16) * (u16)) >> 8))

/*.===================================================================*\
*. FUNCTION:S16H_Mul_S16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s16 * u16 = s32   Returns the high 16 bits of the s32 result.
*.   Variable sizes must match..
\*===================================================================*/
/*
INLINE signed16_T S16H_Mul_S16_U16 (signed16_T s16, unsigned16_T u16)
{ return((signed16_T)(((signed32_T)s16 *u16)>>16) );}
*/
#define S16H_Mul_S16_U16(s16, u16) ((signed16_T)(((signed32_T)(s16) * (u16)) >> 16))

/*.===================================================================*\
*. FUNCTION:S32_Mul_S16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s16 * u16 = s32   Returns s32 type with s32 bit result.
*.  Variable sizes must match
\*===================================================================*/
/*
INLINE signed32_T S32_Mul_S16_U16 (signed16_T s16, unsigned16_T u16)
{ return(((signed32_T)s16 *u16) );}
*/
#define S32_Mul_S16_U16(s16, u16) (((signed32_T)(s16) * (u16)))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         s16 x s16 Multiplies
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION:S32_Mul_S16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*. s16 * s16 = s16l   Returns the low 16 bits of the s32 result.
\*===================================================================*/
/*
INLINE signed16_T S16L_Mul_S16_S16(signed16_T s16_1, signed16_T s16_2)
{ return((signed16_T)((signed32_T)s16_1 * s16_2) );}
*/

#define S16L_Mul_S16_S16(s16_1, s16_2) ((signed16_T)((signed32_T)(s16_1) * (s16_2)))

/*.===================================================================*\
*. FUNCTION:S16M_Mul_S16_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*. s16 * s16 = s16h   Returns the middle 16 bits of the s32 result.
*.   Variable sizes must match
\*===================================================================*/
/*
INLINE signed16_T S16M_Mul_S16_S16(signed16_T s16_1, signed16_T s16_2)
{ return((signed16_T)(((signed32_T)s16_1 * s16_2)>>8) );}
*/

#define S16M_Mul_S16_S16(s16_1, s16_2) ((signed16_T)(((signed32_T)(s16_1) * (s16_2)) >> 8))

/*.===================================================================*\
*. FUNCTION:S16H_Mul_S16_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*. s16 * s16 = s16h   Returns the high 16 bits of the s32 result.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16H_Mul_S16_S16(signed16_T s16_1, signed16_T s16_2)
{ return((signed16_T)(((signed32_T)s16_1 * s16_2)>>16) );}
*/

#define S16H_Mul_S16_S16(s16_1, s16_2) ((signed16_T)(((signed32_T)(s16_1) * (s16_2)) >> 16))

/*.===================================================================*\
*. FUNCTION:S32_Mul_S16_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*. s16 * s16 = s32   Returns s32 type with s32 bit result.
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE signed32_T S32_Mul_S16_S16(signed16_T s16_1, signed16_T s16_2)
{ return( ((signed32_T)s16_1 * s16_2) );}
*/

#define S32_Mul_S16_S16(s16_1, s16_2) (((signed32_T)(s16_1) * (s16_2)))

/*.===================================================================*\
*. FUNCTION: U40L_Mul_U32_U32
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  u32 * u32 = u40L  Returns lower 40 bits of the u64 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

/*
INLINE unsigned40_T U40_Mul_U32_U32(unsigned32_T u32_1,unsigned32_T u32_2)
 { return ((unsigned40_T)u32_1 * u32_2 );}
*/

#define U40_Mul_U32_U32(u32_1, u32_2) ((unsigned40_T)(u32_1) * (u32_2))

#define U40_Mul_U32_U16(u32_1, u16_2) ((unsigned40_T)(u32_1) * (u16_2))

#define U32_Mul_U32_U16(u32_1, u16_2) ((unsigned32_T)(u32_1) * (u16_2))
#define S40_Mul_S40_U40(s40_1, u40_2) ((signed40_T)(s40_1) * (u40_2))
#define S40_Mul_S32_U16(s32_1, u16_2) ((signed40_T)(s32_1) * (u16_2))

#define S40_Mul_S32_U40(s32_1, u40_2) ((signed40_T)(s32_1) * (u40_2))

#define S40_Mul_S32_S40(s32_1, s40_2) ((signed40_T)(s32_1) * (s40_2))

#define S40_Mul_S32_S32(s32_1, s32_2) ((signed40_T)(s32_1) * (s32_2))
#define S40_Mul_S32_S16(s32_1, s16_2) ((signed40_T)(s32_1) * (s16_2))
#define S32_Mul_S32_U32(s32_1, u32_2) ((signed32_T)(s32_1) * (u32_2))

#define S32_Mul_S32_S32(s32_1, s32_2) (signed32_T)((signed32_T)(s32_1) * (s32_2))

#define S32_Mul_S32_S16(s32_1, s16_2) (signed32_T)((signed32_T)(s32_1) * (signed32_T)(s16_2))
#define S32_Mul_S32_U16(s32_1, u16_2) ((signed32_T)(s32_1) * (u16_2))

#define S40_Mul_S32_U32(s32_1, u32_2) ((signed40_T)(s32_1) * (u32_2))
#define S40_Mul_S32_S32(s32_1, s32_2) ((signed40_T)(s32_1) * (s32_2))
#define S40_Mul_S32_S8                (s32_1, s8_2)((signed40_T)(s32_1) * (s8_2))

#define S32_Mul_S16_U32(s16_1, u32_2) ((signed32_T)(s16_1) * (u32_2))

#define S32_Div_S40_S32(s40_1, s32_2)     (signed32_T)((signed40_T)(s40_1) / (s32_2))
#define S40_Div_S40_S32(s40_1, s32_2)     (signed40_T)((signed40_T)(s40_1) / (s32_2))
#define S16_Div_S40_S32(s40_1, s32_2)     (signed16_T)((signed40_T)(s40_1) / (s32_2))
#define U16_Div_U32_U32(u32_num, u32_div) ((unsigned16_T)((unsigned32_T)(u32_num) / (u32_div)))
#define U16_Div_U40_U32(u40_num, u32_div) ((unsigned16_T)((unsigned40_T)(u40_num) / (u32_div)))
#define U32_Div_U40_U16(u40_num, u16_div) ((unsigned32_T)((unsigned40_T)(u40_num) / (u16_div)))
#define U64_Mul_U32_U32(u32_1, u32_2)     ((unsigned64_T)(u32_1) * (u32_2))

#define U64_Mul_U32_U16(u32_1, u16_2) ((unsigned64_T)(u32_1) * (u16_2))
#define U64_Mul_U64_U32(u64_1, u32_2) ((unsigned64_T)(u64_1) * (u32_2))

#define U32_Mul_U32_U16(u32_1, u16_2) ((unsigned32_T)(u32_1) * (u16_2))
#define S64_Mul_S64_U64(S64_1, u64_2) ((signed64_T)(S64_1) * (u64_2))
#define S64_Mul_S32_U16(s32_1, u16_2) ((signed64_T)(s32_1) * (u16_2))

#define S64_Mul_S32_U64(s32_1, u64_2) ((signed64_T)(s32_1) * (u64_2))

#define S64_Mul_S64_S32(s64_1, s32_2) ((signed64_T)(s64_1) * (s32_2))

#define S64_Mul_S32_S32(s32_1, s32_2) ((signed64_T)(s32_1) * (s32_2))
#define S64_Mul_S32_S16(s32_1, s16_2) ((signed64_T)(s32_1) * (s16_2))
#define S32_Mul_S32_U32(s32_1, u32_2) ((signed32_T)(s32_1) * (u32_2))

#define S32_Mul_S32_S32(s32_1, s32_2) (signed32_T)((signed32_T)(s32_1) * (s32_2))

#define S32_Mul_S32_S16(s32_1, s16_2) (signed32_T)((signed32_T)(s32_1) * (signed32_T)(s16_2))
#define S32_Mul_S32_U16(s32_1, u16_2) ((signed32_T)(s32_1) * (u16_2))

#define S64_Mul_S32_U32(s32_1, u32_2) ((signed64_T)(s32_1) * (u32_2))
#define S64_Mul_S32_S8                (s32_1, s8_2)((signed64_T)(s32_1) * (s8_2))

#define S32_Mul_S16_U32(s16_1, u32_2) ((signed32_T)(s16_1) * (u32_2))

#define S32_Div_S64_S32(S64_1, s32_2) (signed32_T)((signed64_T)(S64_1) / (s32_2))
#define S32_Div_S64_S16(S64_1, s16_2) (signed32_T)((signed64_T)(S64_1) / (s16_2))

#define S64_Div_S64_S32(S64_1, s32_2)     (signed64_T)((signed64_T)(S64_1) / (s32_2))
#define U16_Div_U32_U32(u32_num, u32_div) ((unsigned16_T)((unsigned32_T)(u32_num) / (u32_div)))
#define U16_Div_U64_U32(u64_num, u32_div) ((unsigned16_T)((unsigned64_T)(u64_num) / (u32_div)))
#define U32_Div_U64_U16(u64_num, u16_div) ((unsigned32_T)((unsigned64_T)(u64_num) / (u16_div)))
#define U16_Mul_U16_U16(u16_1, u16_2)     (unsigned16_T)((u16_1) * (u16_2))
/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         Division
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

/*.===================================================================*\
*. FUNCTION:U16_Div_U16_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u16 / u16 = u16  Integer Divide.
*.   Returns the 16 bit quotient.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned16_T U16_Div_U16_U16(unsigned16_T u16_num, unsigned16_T u16_div)
{ return( (unsigned16_T)((unsigned32_T)u16_num / u16_div) );}
*/

#define U16_Div_U16_U16(u16_num, u16_div) ((unsigned16_T)((unsigned32_T)(u16_num) / (u16_div)))

/*.===================================================================*\
*. FUNCTION:S16_Div_S16_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s16 / s16 = s16  Integer Divide.
*.   Returns the 16 bit quotient.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16_Div_S16_S16(signed16_T s16_num, signed16_T s16_div)
{ return( (signed16_T)((signed32_T)s16_num / s16_div) );}
*/

#define S16_Div_S16_S16(s16_num, s16_div) ((signed16_T)((signed32_T)(s16_num) / (s16_div)))

/*.===================================================================*\
*. FUNCTION:U16_Div_U32_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.    u32 / u16 = u16  Integer Divide.
*.   Returns the 16 bit quotient.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned16_T U16_Div_U32_U16(unsigned32_T u32_num, unsigned16_T u16_div)
{ return( (unsigned16_T)((unsigned32_T)u32_num / u16_div) );}
*/

#define U16_Div_U32_U16(u32_num, u16_div) ((unsigned16_T)((unsigned32_T)(u32_num) / (u16_div)))

/*.===================================================================*\
*. FUNCTION:S16_Div_S32_S16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.    s32 / s16 = s16  Integer Divide.
*.   Returns the 16 bit quotient.
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed16_T S16_Div_S32_S16(signed32_T s32_num, signed16_T s16_div)
{ return( (signed16_T)((signed32_T)s32_num / s16_div) );}
*/

#define S16_Div_S32_S16(s32_num, s16_div) ((signed16_T)((signed32_T)(s32_num) / (s16_div)))
#define S16_Div_S32_U16(s32_num, u16_div) ((signed16_T)((signed32_T)(s32_num) / (u16_div)))
#define S16_Div_U32_S32(u32_num, s32_div) ((signed16_T)((signed32_T)(u32_num) / (s32_div)))
#define S16_Div_S32_S32(s32_num, s32_div) ((signed16_T)((signed32_T)(s32_num) / (s32_div)))
/*.===================================================================*\
*. FUNCTION:U32_Div_U32_U16  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u32 / u16 = u32   Interger Divide + Fractional Divide.
*.   BPPresult = BPPnumerator-BPPdenominator+16
*.   Variable sizes must match.
\*===================================================================*/

/*
INLINE unsigned32_T U32_Div_U32_U16(unsigned32_T u32_num, unsigned16_T u16_div)
{ return( (unsigned32_T)((unsigned32_T)u32_num / u16_div) );}
*/
#define U32_Div_U32_U16(u32_num, u16_div) ((unsigned32_T)((unsigned32_T)(u32_num) / (u16_div)))

/*.===================================================================*\
*. FUNCTION:U32_Div_U32_U32  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u32 / u32 = u32   Interger Divide + Fractional Divide.
*.   BPPresult = BPPnumerator-BPPdenominator+16
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE unsigned32_T U32_Div_U32_U32(unsigned32_T u32_num, unsigned32_T u32_div)
{ return( (unsigned32_T)((unsigned32_T)u32_num / u32_div) );}
*/

#define U32_Div_U32_U32(u32_num, u32_div) ((unsigned32_T)((unsigned32_T)(u32_num) / (u32_div)))

/* FIXIT RAGHU does this work? */
#define S32_Div_S64_S64(S64_1, s64_2) ((signed32_T)((signed64_T)(S64_1) / (s64_2)))

/* S32_Div_S64_S64 - This Macro does not work on ARM Simulator */
/*.===================================================================*\
*. FUNCTION:U32_Div_U40_U40
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u40 / u40 = u32   Interger Divide + Fractional Divide.
*.   BPPresult = BPPnumerator-BPPdenominator
*.   Variable sizes must match.
\*===================================================================*/
#define U32_Div_U64_U64(u64_num, u64_div) ((unsigned32_T)((u64_num) / (u64_div)))
#define U32_Div_U40_U40(u40_num, u40_div) ((unsigned32_T)((u40_num) / (u40_div)))

/*.===================================================================*\
*. FUNCTION:S32_Div_S32_S32  INLINE
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   s32 / s32 = s32   Interger Divide + Fractional Divide.
*.   BPPresult = BPPnumerator-BPPdenominator+16
*.   Variable sizes must match.
\*===================================================================*/
/*
INLINE signed32_T S32_Div_S32_S32( signed32_T s32_num, signed32_T s32_div)
{ return( (signed32_T)(( signed32_T)s32_num / s32_div) );}
*/

#define S32_Div_S32_S32(s32_num, s32_div) ((signed32_T)((signed32_T)(s32_num) / (s32_div)))

#define S64_Div_S64_U64(S64_num, u64_div) ((signed64_T)((signed64_T)(S64_num) / (u64_div)))

#define S64_Div_S64_S64(S64_num, S64_div) ((signed64_T)((S64_num) / (S64_div)))

#define S40_Div_S40_U40(s40_num, u40_div) ((signed40_T)((signed40_T)(s40_num) / (u40_div)))

#define S40_Div_S40_S40(s40_num, s40_div) ((signed40_T)((s40_num) / (s40_div)))

/*===========================================================================*\
 * U16_Fdiv_U16_U16  MACRO
 *   These MACROs return ratio of unsigned16_T numerator and divisor.
 *   Required condition numerator<divisor.
 *
 * Shared Variables: none.
 *
 * Parameters: u16_numerator, u16_divisor.
 *
 * Return Value for U16_Fdiv_U16_U16: unsigned16_T ratio.
 *                                    u16/0 = U16_MAX_RATIO
 *                                  u16/u16 = U16_MAX_RATIO ; if numerator >= divisor
 *                                    0/u16 = 0
 *                                    0/0   = 0
 *
 \*===========================================================================*/
#define U16_Fdiv_U16_U16(u16_numerator, u16_divisor) \
   (U16_Div_U32_U16((((unsigned32_T)(u16_numerator)) << 16), (u16_divisor)))

/*.===================================================================*\
*. FUNCTION: S32L_Mul_S32_S32
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s32 * s32 = s32L  Returns lower 32 bits of the 64 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

#define S32L_Mul_S32_S32(s32_1, s32_2) ((signed32_T)(s32_1) * (s32_2))
/*.===================================================================*\
*. FUNCTION: S32L_Mul_S32_U32
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s32 * s32 = s32L  Returns lower 32 bits of the 64 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

#define S32L_Mul_S32_U32(s32_1, u32_2) ((signed32_T)(s32_1) * (u32_2))

/*.===================================================================*\
*. FUNCTION: U32L_Mul_U32_U32
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s32 * s32 = s32L  Returns lower 32 bits of the 64 result.
*.   Variable sizes must match.
*.
\*===================================================================*/

#define U32L_Mul_U32_U32(u32_1, u32_2) ((unsigned32_T)(u32_1) * (u32_2))

/*.===================================================================*\
*. FUNCTION:U32H_Mul_U32_U16
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*. u32 * u16 = u32   Returns u32 type with u32 bit result.
*.   Variable sizes must match.
*.
\*===================================================================*/

#define U32H_Mul_U32_U16(u32, u16) \
   ((unsigned32_T)(((unsigned64_T)((unsigned64_T)(u32) * (unsigned64_T)(u16))) >> 16))

/*.===================================================================*\
*. FUNCTION: S32_Div_S32_S16
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s32 / s16 = s32
*.   Variable sizes must match.
*.
\*===================================================================*/

#define S32_Div_S32_S16(s32_num, s16_div) ((signed32_T)((signed32_T)(s32_num) / (s16_div)))

/*.===================================================================*\
*. FUNCTION: S32_Div_S32_U16
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.  s32/u16 = s32
*.   Variable sizes must match.
*.
\*===================================================================*/

#define S32_Div_S32_U16(s32_num, u16_div) ((signed32_T)((signed32_T)(s32_num) / (u16_div)))

/*.===================================================================*\
*. FUNCTION:U32_Div_U64_U32
*.=====================================================================
*. Return Value:
*.  None.
*.
*. Parameters:
*.  none
*.
*. External references:
*.  External variables:
*.    None
*.
*.  File scope variables:
*.   None
*.
*. Description:
*.   u64 / u32 = u32   Interger Divide .
*.   BPPresult = BPPnumerator-BPPdenominator
*.   Variable sizes must match.
\*===================================================================*/

#define U32_Div_U64_U32(u64_num, u32_div) \
   ((unsigned32_T)((u64_num) / (u32_div)))

/*===========================================================================*/
/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
 *                         Limit Routines prototypes
 *<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
/*===========================================================================*/

unsigned8_T U8_Add_Limit(unsigned8_T param1, unsigned8_T param2,
                         unsigned8_T limit_value);

signed8_T S8_Add_Limit(signed8_T param1, signed8_T param2,
                       signed8_T lower_limit, signed8_T upper_limit);

signed8_T S8_Sub_Limit(signed8_T param1, signed8_T param2,
                       signed8_T lower_limit, signed8_T upper_limit);

unsigned16_T U16_Add_Limit(unsigned16_T param1, unsigned16_T param2,
                           unsigned16_T limit_value);

signed16_T S16_Add_Limit(signed16_T param1, signed16_T param2,
                         signed16_T lower_limit, signed16_T upper_limit);

signed16_T S16_Sub_Limit(signed16_T param1, signed16_T param2,
                         signed16_T lower_limit, signed16_T upper_limit);

unsigned32_T U32_Add_Limit(unsigned32_T param1, unsigned32_T param2,
                           unsigned32_T limit_value);

signed32_T S32_Add_Limit(signed32_T param1, signed32_T param2,
                         signed32_T lower_limit, signed32_T upper_limit);

signed32_T S32_Sub_Limit(signed32_T param1, signed32_T param2,
                         signed32_T lower_limit, signed32_T upper_limit);

/*
INLINE unsigned16_T getH16bits(unsigned32_T u32)
{return ((unsigned16_T)(u32>>16));}
*/
#define getH16bits(u32) ((unsigned16_T)((u32) >> 16))

/*
INLINE unsigned16_T getL16bits(unsigned32_T u32)
{return ((unsigned16_T)(u32));}
*/
#define getL16bits(u32) ((unsigned16_T)(u32))

#ifndef Ceil_16Bit
#define Ceil_16Bit(value, to_type)                                   \
   ((to_type)(((0xFFFF >> (16 - BPP_##to_type)) & (value))           \
                  ? ((((value) >> BPP_##to_type) << BPP_##to_type) + \
                     (1ul << (BPP_##to_type)))                       \
                  : (value)))
#endif

#ifndef Ceil_32Bit
#define Ceil_32Bit(value, to_type)                                   \
   ((to_type)(((0xFFFFFFFF >> (32 - BPP_##to_type)) & (value))       \
                  ? ((((value) >> BPP_##to_type) << BPP_##to_type) + \
                     (1ul << (BPP_##to_type)))                       \
                  : (value)))
#endif

#ifndef Floor_32Bit
#define Floor_32Bit(value, to_type) \
   (((value) >> BPP_##to_type) << BPP_##to_type)
#endif

/* #ifndef Round_32Bit */
#define FIVE_p1 Init_Fixed_Point(0.5, u31p1_T)

#define Round_32Bit(value, to_type) \
   ((to_type)((0xFFFFFFFF >> (32 - BPP_##to_type)) & (value)) > (FIVE_p1 << (BPP_##to_type - 1)) ? (Ceil_32Bit((value), to_type)) : (Floor_32Bit((value), to_type)))

#define Round_32BitS(value, to_type) \
   ((to_type)((0xFFFFFFFF >> (31 - BPP_##to_type)) & (value)) > (FIVE_p1 << (BPP_##to_type - 1)) ? (Ceil_32Bit((value), to_type)) : (Floor_32Bit((value), to_type)))
/*#endif */

/*.***************************************************************************
 *. Name: FLP2FXP_NLC    MACRO
 *.   Initialize a value, given in engineering units, to fixed point
 *.   representation. NLC stands for No Limit Check. Use the macro FLP2FXP if
 *.   you want to do limit the engineering value before conversion.
 *.
 *. Shared Variables: none.
 *. Parameters: value: the input value or variable.
 *.             to_type: type to convert to.
 *.
 *. Return Value: The value in to_type form.
 *.
 *. Updated: 08/29/05 sunil
 *
 *  ACC3 SCRs: 5766
 ******************************************************************************/
#define FLP2FXP_NLC(value, to_type) \
   ((to_type)(FLP2FXP_NLC_Fcn(value, BPP_##to_type, SIGNED_##to_type)))

/*.***************************************************************************
 *. Name: FLP2FXP    MACRO
 *.   Initialize a value, given in engineering units, to fixed point
 *.   representation. Also the engineering value is limited to its given limits
 *.   before converting into the fixpoint form.
 *.
 *. Shared Variables: none.
 *. Parameters: value: the input value or variable.
 *.             to_type: type to convert to.
 *.             limit_low: Lower limit in engineering units .
 *.             limit_high: Upper limit in engineering units .
 *.
 *. Return Value: The value in to_type form.
 *.
 *. Updated: 08/29/05 sunil
 *
 *  ACC3 SCRs: 5766, 7740
 ******************************************************************************/
#define FLP2FXP(value, to_type, limit_low, limit_high)            \
   ((to_type)(FLP2FXP_Fcn(value, BPP_##to_type, SIGNED_##to_type, \
                          limit_low, limit_high)))
/*.***************************************************************************
 *. Name: FXP2FLP    MACRO
 *.   converts value from  fixed point form to engineering units
 *.
 *. Shared Variables: none.
 *. Parameters: value: the input value or variable in the fixpoint format.
 *.             to_type: type of the above variable..
 *.
 *. Return Value: The engineering value
 *.
 *. Updated: 08/29/05 sunil
 *
 *  ACC3 SCRs: 5766
 ******************************************************************************/
#define FXP2FLP(fix_point_value, from_type) \
   FXP2FLP_Fnc(fix_point_value, BPP_##from_type, SIGNED_##from_type)

/************************************************************************/
/* Prototype of Fixpt to float and float to Fix conversion functions    */
/* Theses functions are defined in fixmac.c                             */
/* Note: These functions are developed for using with the macros given  */
/*       below. Don't call theses functions directly, instead use the   */
/*       macros provided below.                                         */
/************************************************************************/
extern signed32_T FLP2FXP_Fcn(Float32_T value, signed32_T bpp_num, boolean_T signed_flag, Float32_T limit_low, Float32_T limit_high);

extern Float64_T FXP2FLP_Fnc_Ext(signed32_T value, signed32_T bpp_num, boolean_T signed_flag);

extern signed32_T FLP2FXP_NLC_Fcn(Float32_T value, signed32_T bpp_num, boolean_T signed_flag);

extern Float64_T FLP2FXP_NLC_Fcn_Ext(Float64_T value, signed32_T bpp_num);

extern Float32_T FXP2FLP_Fnc(signed32_T value, signed32_T bpp_num, boolean_T signed_flag);

extern Float32_T FXP2FLP_Fnc_Long(signed64_T value, signed32_T bpp_num, boolean_T signed_flag);

#endif

/*===========================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *===========================================================================
 *
 * Date        userid    (Description on following lines: SCR #, etc.)
 * ----------- --------
 * 03-June-2010 Divya - TCI
 * + SCR kok_css#11744 : Implement RSDS Tracker Version 19- Added 64bit datatypes.
 * 13/0/3/15 Priya kok_css2#25500 bool type disabled for PC_RESIM
 * 08/10/15    Anvesh  kok_css2#27907 added UNIT_TEST macro to handle Unit test
 * 12-Oct-2015 Arundhati kok_css2#29062 added UNIT_TEST macro to handle Unit test for audi
 * 06-Jan-2016 Aravindh Added U64_MUL macro
 * 11-APR-2016 Ananthesh RR_COMMON:kok_css2#32545:Fix compiler warnings
 * 15-MAY-2016 Ananthesh RR_COMMON:kok_css2#33421:Move fixmac.c from Z2_CORE to RR_COMMON and fix QAC warnings/errors
 * 18-MAY-2016 Ananthesh RR_COMMON:Fix issue in fixmac.h for converting floating to fixed point
 * 19-MAY-2016 Ananthesh RR_COMMON:kok_css2#33421:Fix issue in fixmac.h for converting fixed to float
 * 30-MAY-2016 Sankar    RR_COMMON:kok_css2#33747:Fix QAC and compiler warnings
 * 13-JUL-2016 GK & Ananthesh   kok_css2#34083:[AUDI]QAC for xcp_user_commands.c file
 * 06-NOV-2016 Ananthesh RR_COMMON:CR kok_css2#37665: Add new file Radar_Config.h
\*===========================================================================*/
