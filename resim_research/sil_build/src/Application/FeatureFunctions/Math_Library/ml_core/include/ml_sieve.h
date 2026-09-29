#ifndef ML_SIEVE_H
#define ML_SIEVE_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
* \defgroup Sieve Sieve
* \brief Sieve out extreme values from data.
*
* The Sieve module is built to sieve out extreme values. The usage is like this:
* Initialize a sieve
* - Call the sieve function with all the raw data values
* - For each raw data value you want to use check if it is a non sieved valid value (Is_Sieved_Value_Valid())
*
* - The sieves keep track of the known max and min values. A new value sent into the sieve may replace the current known max/min value.
* - In this case the return value is the old max/min value.
* - This means that the returned values are not in the same sequence like the input values.
*
* Example:
* \code
* Init_Max_Sieve(&sieve);
* value = Max_Sieve(&sieve, 6); // The known max value now is 6, the returned value is -INFINITY
* if (Is_Sieved_Value_Valid(value))
* {
*      // this is NOT hit
* }
* value = Max_Sieve(&sieve, 5); // Since the current max value is 6 this returns the given 5
* if (Is_Sieved_Value_Valid(value))
* {
*      // this is hit
* }
* value = Max_Sieve(&sieve, 7); // Since the current max value is 6 the max value is replaced with 7 and the function returns the old max value 6
* if (Is_Sieved_Value_Valid(value))
* {
*      // this is hit
* }
* \endcode
*/

#include "ml_max_sieve_t.h"
#include "ml_min_max_sieve_t.h"
#include "ml_min_sieve_t.h"

/**
* Initializes a given Max_Sieve_T
* \ingroup Sieve
* \sdd{WI-13577}
*/
void Init_Max_Sieve(Max_Sieve_T * const p_sieve /**< [in] pointer to a max sieve to be initialized */);

/**
* Sieves out a max value
* \return           float32_T Value that has been sieved out
* \ingroup Sieve
* \sdd{WI-13577}
*/
float32_T Max_Sieve(
   Max_Sieve_T * const p_sieve, /**< [in] Pointer to a Max_Sieve_T */
   float32_T    value /**< [in] Value to be sieved */);

/**
* Initializes an array of Max_Sieve_T
* \ingroup Sieve
* \sdd{WI-13582}
*/
void Init_Max_Sieve_Set(
   Max_Sieve_T * const p_sieve, /**< [in, out] Pointer to an array of Max_Sieve_T */
   const uint8_t      sieve_set_size /**< [in] size of given p_sieve */);

/**
* Sieves out max values using an array of Max_Sieve_T
* \return           float32_T Value that has been sieved out
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Max_Sieve_Set(
   Max_Sieve_T * const p_sieve, /**< [in] Pointer to an array of Max_Sieve_T */
   const uint8_t      sieve_set_size, /**< [in] size of given p_sieve */
   float32_T    value /**< [in] Value to be sieved */);

/**
* Returns the value stored in the given Max_Sieve_T
* \return           float32_T the value stored in the given Max_Sieve_T
* \ingroup Sieve
* \sdd{WI-13577}
*/
float32_T Get_Max_Sieve_Content(const Max_Sieve_T * const p_sieve /**< [in] Max_Sieve_T to get the content of */);

/**
* Initializes a given Min_Sieve_T
* \ingroup Sieve
* \sdd{WI-13571}
*/
void Init_Min_Sieve(Min_Sieve_T * const p_sieve/**<[out] Min_Sieve_T to be initialized */);

/**
* Sieves out a min value
* \return float32_T Value that has been sieved out
* \ingroup Sieve
* \sdd{WI-13571}
*/
float32_T Min_Sieve(
   Min_Sieve_T * const p_sieve, /**< [in, out] Sieve to apply on value */
   float32_T    value /**< [in] Value to be sieved */);

/**
* Initializes an array of Min_Sieve_T
* \ingroup Sieve
* \sdd{WI-13582}
*/
void Init_Min_Sieve_Set(
   Min_Sieve_T *p_sieve, /**< [out] pointer to an array of Min_Sieve_T to be initialized */
   uint8_t      sieve_set_size /**< [in] Size of the given array */);

/**
* Sieves out min values using an array of Min_Sieve_T
* \return            float32_T Value that has been sieved out
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Min_Sieve_Set(
   Min_Sieve_T *p_sieve, /**< [in, out] pointer to an array of Min_Sieve_T */
   uint8_t      sieve_set_size, /**< [in] Size of given array p_sieve points to */
   float32_T    value /**< Value to be sieved */);

/**
* Returns the value stored in the given Min_Sieve_T
* \return           float32_T the value stored in the given Min_Sieve_T
* \ingroup Sieve
* \sdd{WI-13571}
*/
float32_T Get_Min_Sieve_Content(const Min_Sieve_T *p_sieve /**< [in] Sieve to get the content from */);

/**
* Returns the max value of a max sieve set
* \return           float32_T the max value of a max sieve set
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Get_Max_From_Max_Sieve_Set(const Max_Sieve_T *p_sieve /**< [in] pointer to a Max_Sieve_T set */);

/**
* Returns the value of a max sieve set at index
* \return           the value of a max sieve set at index
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Get_Max_Sieve_Set_Content_At_Index(
   const Max_Sieve_T *p_sieve, /**< [in] pointer to a Max_Sieve_T set */
   uint8_t      index /**< [in] index to be read */);

/**
* Returns the sieve content at given index
* \return           Sieve content at given index
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Get_Min_Sieve_Set_Content_At_Index(
   const Min_Sieve_T *p_sieve, /**< [in] Sieve to read content from */
   uint8_t      index /**< [in] Index to be read */);

/**
* Returns the min value of a min sieve set
* \return           the min value of a min sieve set
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Get_Min_From_Min_Sieve_Set(const Min_Sieve_T *p_sieve /**< [in] pointer to a min sieve set */);

/**
* Initializes a given Min_Max_Sieve_T
* \ingroup Sieve
* \sdd{WI-13573}
*/
void Init_Min_Max_Sieve(Min_Max_Sieve_T * const p_sieve /**< [in] pointer to a Min_Max_Sieve_T to be initialized */);

/**
* Sieves out a min and a max value
* \return sieved out content
* \ingroup Sieve
* \sdd{WI-13573}
*/
float32_T Min_Max_Sieve(
   Min_Max_Sieve_T * const p_sieve, /**< [in] Min_Max_Sieve_T to be applied on value */
   float32_T        value /**< [in] Value to be sieved */);

/**
* Initializes an array of Min_Max_Sieve_T
* \ingroup Sieve
* \sdd{WI-13582}
*/
void Init_Min_Max_Sieve_Set(
   Min_Max_Sieve_T * const p_sieve, /**< [in] Pointer to an array of Min_Max_Sieve_T */
   const uint8_t          sieve_set_size /**< [in] Size of given array */);

/**
* Sieves out max values using an array of Min_Max_Sieve_T
* \return           sieved out content
* \ingroup Sieve
* \sdd{WI-13582}
*/
float32_T Min_Max_Sieve_Set(
   Min_Max_Sieve_T * const p_sieve, /**< [in] Pointer to an array of Min_Max_Sieve_T */
   const uint8_t          sieve_set_size, /**< [in] Size of given array */
   float32_T        value /**< [in] Value to be sieved */);

/**
* Returns the min value stored in the given Min_Max_Sieve_T at index
* \return           min value stored in the given Min_Max_Sieve_T at index
* \ingroup Sieve
* \sdd{WI-13573}
*/
float32_T Get_Min_Max_Sieve_Min_Content(
   const Min_Max_Sieve_T * const p_sieve, /**< [in] Pointer to an array of Min_Max_Sieve_T */
   const uint8_t          index /**< [in] index in given array to be read */);

/**
* Returns the max value stored in the given Min_Max_Sieve_T at index
* \return           the max value stored in the given Min_Max_Sieve_T at index
* \ingroup Sieve
* \sdd{WI-13573}
*/
float32_T Get_Min_Max_Sieve_Max_Content(
   const Min_Max_Sieve_T * const p_sieve, /**< [in] Pointer to an array of Min_Max_Sieve_T */
   const uint8_t          index/**< [in] index in given array to be read */);

/**
* Returns TRUE if the given value is valid (not sieved out)
* \return           TRUE if the given value is valid (not sieved out)
* \ingroup Sieve
* \sdd{WI-13577}
* \sdd{WI-13582}
* \sdd{WI-13573}
*/
boolean_T Is_Sieved_Value_Valid(const float32_T value /**< [in] value to be checked */);


#ifdef __cplusplus
}
#endif
#endif

