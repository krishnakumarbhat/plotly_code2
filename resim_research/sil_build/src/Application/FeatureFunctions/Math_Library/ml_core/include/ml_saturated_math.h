#ifndef ML_SATURATED_MATH_H
#define ML_SATURATED_MATH_H
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
* \defgroup saturated_math Saturated Math
* A collection of tool functions to handle math with saturating values.
A collection of tool functions to handle math with saturating values is provided.
* Each integer type has a minimum and a maximum value that can be expressed. If a
* mathematical operation exceeds these limits the most common result is that the
* least significant representable digits of the result are stored; the result is
* said to wrap around the maximum. This behaviour is what the C11 standard states.
*
* The result of the functions in this group saturates at the maximum/minimum value.
* E.g. for an uint8_t type the minimum is 0 and the maximum is 255. The operation
* 250 + 20 would wrap around to 5 while the saturated add operation would result
* in 255.
*
* Also see [Wikipedia](https:\\en.wikipedia.org/wiki/Integer_overflow) for further details.
*/

/**
*  Increases the unsigned-8 value that the pointer points to. Ensures that the resulting value does not exceed UINT8_MAX.
* \ingroup saturated_math
* \sdd{WI-13868}
*/
void Sat_Inc_Uint8(uint8_t *p_number /**< [in, out] pointer to a number to be increased*/);


/**
*  Increases the signed-8 value that the pointer points to. Ensures that the resulting value does not exceed INT8_MAX.
* \ingroup saturated_math
* \sdd{WI-13877}
*/
void Sat_Inc_Int8(int8_t *p_number /**< [in, out] pointer to a number to be increased*/);


/**
*  Increases the unsigned-16 value that the pointer points to. Ensures that the resulting value does not exceed UINT16_MAX.
* \ingroup saturated_math
* \sdd{WI-13867}
*/
void Sat_Inc_Uint16(uint16_t *p_number /**< [in, out] pointer to a number to be increased*/);


/**
*  Increases the signed-16 value that the pointer points to. Ensures that the resulting value does not exceed INT16_MAX.
* \ingroup saturated_math
* \sdd{WI-13876}
*/
void Sat_Inc_Int16(int16_t *p_number /**< [in, out] pointer to a number to be increased*/);


/**
*  Increases the unsigned-32 value that the pointer points to. Ensures that the resulting value does not exceed UINT32_MAX.
* \ingroup saturated_math
* \sdd{WI-13878}
*/
void Sat_Inc_Uint32(uint32_t *p_number /**< [in, out] pointer to a number to be increased*/);


/**
*  Increases the signed-32 value that the pointer points to. Ensures that the resulting value does not exceed INT32_MAX.
* \ingroup saturated_math
* \sdd{WI-13874}
*/
void Sat_Inc_Int32(int32_t *p_number /**< [in, out] pointer to a number to be increased*/);




/**
*  Decreases the unsigned-8 value that the pointer points to. Ensures that the resulting value does not exceed 0.
* \ingroup saturated_math
* \sdd{WI-13872}
*/
void Sat_Dec_Uint8(uint8_t *p_number /**< [in, out] pointer to a number to be decreased*/);


/**
*  Decreases the signed-8 value that the pointer points to. Ensures that the resulting value does not exceed INT8_MIN.
* \ingroup saturated_math
* \sdd{WI-13882}
*/
void Sat_Dec_Int8(int8_t *p_number /**< [in, out] pointer to a number to be decreased*/);


/**
*  Decreases the unsigned-16 value that the pointer points to. Ensures that the resulting value does not exceed 0.
* \ingroup saturated_math
* \sdd{WI-13870}
*/
void Sat_Dec_Uint16(uint16_t *p_number /**< [in, out] pointer to a number to be decreased*/);


/**
*  Decreases the signed-16 value that the pointer points to. Ensures that the resulting value does not exceed INT16_MIN.
* \ingroup saturated_math
* \sdd{WI-13887}
*/
void Sat_Dec_Int16(int16_t *p_number /**< [in, out] pointer to a number to be decreased*/);


/**
*  Decreases the unsigned-32 value that the pointer points to. Ensures that the resulting value does not exceed 0.
* \ingroup saturated_math
* \sdd{WI-13880}
*/
void Sat_Dec_Uint32(uint32_t *p_number /**< [in, out] pointer to a number to be decreased*/);

/**
*  Decreases the signed-32 value that the pointer points to. Ensures that the resulting value does not exceed INT32_MIN.
* \ingroup saturated_math
* \sdd{WI-13889}
*/
void Sat_Dec_Int32(int32_t *p_number /**< [in, out] pointer to a number to be decreased*/);




/**
*  Returns the unsigned-8 sum of two given unsigned-8 summand. Ensures that sum saturates at UINT8_MAX.
*
* \return         saturated sum of two uint8 values
* \ingroup saturated_math
* \sdd{WI-13879}
*/
uint8_t Sat_Add_Uint8(
   uint8_t summand1,     /**<[in, out] input first summand*/
   uint8_t summand2 /**<[in, out] input second summand*/);


/**
*  Returns the signed-8 sum of two given signed-8 summand. Ensures that sum saturates at INT8_MAX.
*
* \return         saturated sum of two int8 values
* \ingroup saturated_math
* \sdd{WI-13875}
*/
int8_t Sat_Add_Int8(
   int8_t summand1,     /**<[in, out] input first summand*/
   int8_t summand2 /**<[in, out] input second summand*/);


/**
*  Returns the unsigned-16 sum of two given unsigned-16 summand. Ensures that sum saturates at UINT16_MAX.
*
* \return         saturated sum of two uint16 values
* \ingroup saturated_math
* \sdd{WI-13885}
*/
uint16_t Sat_Add_Uint16(
   uint16_t summand1,     /**<[in, out] input first summand*/
   uint16_t summand2 /**<[in, out] input second summand*/);


/**
*  Returns the signed-16 sum of two given signed-16 summand. Ensures that sum saturates at INT16_MAX.
*
* \return         saturated sum of two int16 values
* \ingroup saturated_math
* \sdd{WI-13869}
*/
int16_t Sat_Add_Int16(
   int16_t summand1,     /**<[in, out] input first summand*/
   int16_t summand2 /**<[in, out] input second summand*/);


/**
* Saturating Add for unsigned 32bit integer values.
* Returns the sum of two given unsigned 32bit summand, if it can be represented by an unsigned 32bit integer.
* In case the result would cause an integer overflow, UINT32_MAX is returned instead.
* This function exploits that a integer overflow of a + b can be expressed as sum = a + b - (UINT32_MAX + 1).
* As both 0 <= a <= UINT32_MAX and 0 <= b <= UINT32_MAX, it holds that sum <= a + UINT32_MAX - UINT32_MAX - 1,
* so sum <= a - 1. Only one check is required as a and b may be exchanged in above inequation.
*
* \return         saturated sum of two uint32 values
* \ingroup saturated_math
* \sdd{WI-13873}
*/
uint32_t Sat_Add_Uint32(
   uint32_t summand1,     /**<[in, out] input first summand*/
   uint32_t summand2 /**<[in, out] input second summand*/);


/**
*  Returns the signed-32 sum of two given signed-32 summand. Ensures that sum saturates at INT32_MIN and INT32_MAX.
*  In case the actual sum is bigger than INT32_MAX this function returns INT32_MAX. In case the actual sum is smaller
*  than INT32_MIN it returns INT32_MIN.
* \return         saturated sum of two int32 values
* \ingroup saturated_math
* \sdd{WI-13871}
*/
int32_t Sat_Add_Int32(
   int32_t summand1,     /**<[in, out] input first summand*/
   int32_t summand2 /**<[in, out] input second summand*/);




/**
*  Returns the unsigned-8 difference of a given unsigned-8 minuend and an unsigned-8 subtrahend. Ensures that sum saturates at 0.
*
* \return         saturated difference of two uint8 values
* \ingroup saturated_math
* \sdd{WI-13884}
*/
uint8_t Sat_Sub_Uint8(
   uint8_t minuend,        /**<[in, out] input minuend*/
   uint8_t subtrahend /**<[in, out] input subtrahend*/);

/**
*  Returns the signed-8 difference of a given signed-8 minuend and an signed-8 subtrahend. Ensures that sum saturates at INT8_MIN.
*
* \return         saturated difference of two int8 values
* \ingroup saturated_math
* \sdd{WI-13888}
*/
int8_t Sat_Sub_Int8(
   int8_t minuend,        /**<[in, out] input minuend*/
   int8_t subtrahend /**<[in, out] input subtrahend*/);


/**
*  Returns the unsigned-16 difference of a given unsigned-16 minuend and an unsigned-16 subtrahend. Ensures that sum saturates at 0.
*
* \return         saturated difference of two uint16 values
* \ingroup saturated_math
* \sdd{WI-13881}
*/
uint16_t Sat_Sub_Uint16(
   uint16_t minuend,        /**<[in, out] input minuend*/
   uint16_t subtrahend /**<[in, out] input subtrahend*/);


/**
*  Returns the signed-16 difference of a given signed-16 minuend and an signed-16 subtrahend. Ensures that sum saturates at INT16_MIN.
*
* \return         saturated difference of two int16 values
* \ingroup saturated_math
* \sdd{WI-13890}
*/
int16_t Sat_Sub_Int16(
   int16_t minuend,        /**<[in, out] input minuend*/
   int16_t subtrahend /**<[in, out] input subtrahend*/);


/**
*  Returns the unsigned-32 difference of a given unsigned-32 minuend and an unsigned-32 subtrahend. Ensures that sum saturates at 0.
*
* \return         saturated difference of two uint32 values
* \ingroup saturated_math
* \sdd{WI-13883}
*/
uint32_t Sat_Sub_Uint32(
   uint32_t minuend,        /**<[in, out] input minuend*/
   uint32_t subtrahend /**<[in, out] input subtrahend*/);


/**
*  Returns the signed-32 difference of a given signed-32 minuend and an signed-32 subtrahend. Ensures that sum saturates at INT32_MIN.
*
* \return         saturated difference of two int32 values
* \ingroup saturated_math
* \sdd{WI-13886}
*/
int32_t Sat_Sub_Int32(
   int32_t minuend,        /**<[in, out] input minuend*/
   int32_t subtrahend /**<[in, out] input subtrahend*/);

#ifdef __cplusplus
}
#endif
#endif
