#ifndef ML_CHECKSUM_H
#define ML_CHECKSUM_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

/**
* \defgroup shared_toolbox_checksum Check Sum Calculation
* To ensure that a memory block contains valid data a checksum can be used.
* This module offers functions to calculate various checksums.
* \section shared_toolbox_checksum_demo Demo checksum
* This demonstration shows how to use the Calc_Checksum_U*() function.
* The code shown here can be found in the unit test file "st_checksum_test.cpp"
* in the test
* \code{.cpp}
* TEST(ChecksumTestFixture, demo_u16)
* \endcode
* \subsection shared_toolbox_checksum_demo_steps Steps
* The demonstration shows the following steps:
* - Create a data structure that holds a checksum member of the desired size.
*   - Be careful that the whole structure is initialized. Either use memset to also initialize padding bytes
*     or ensure that the structure does not have any padding bytes. The easiest way to achieve this is to sort
*     the members by memory size, biggest to smallest.
*
* \snippet st_checksum_test.cpp data_structure
*
* - Fill the structure with arbitrary data, make sure the checksum is set to zero:
*
* \snippet st_checksum_test.cpp data_content
*
* - Set the checksum member to the twos complement of the calculated checksum + 1
*
* \snippet st_checksum_test.cpp set_cs
*
* - The recalculated checksum of the structure now is zero
*
* \snippet st_checksum_test.cpp check_cs1
*
* - If the data now is changed a little
*
* \snippet st_checksum_test.cpp change_data
*
* - The calculated checksum will no longer be zero
*
* \snippet st_checksum_test.cpp check_cs2
*
* In real live functions tailored to the data structure for checking and resetting the checksum would be written.
*/

/**
* Calculates an 8 bit checksum over given data.
* \return the computed checksum
* \ingroup shared_toolbox_checksum
* \sdd{WI-14651}
*/
uint8_t Calc_Checksum_U8(
   const void *data, /**< [in] pointer to the data the checksum is calculated for */
   size_t size /**< [in] size [bytes] of the data the checksum is calculated for */
);

/**
* Calculates an 16 bit checksum over given data.
* \return the computed checksum
* \ingroup shared_toolbox_checksum
* \sdd{WI-14653}
*/
uint16_t Calc_Checksum_U16(
   const void *data, /**< [in] pointer to the data the checksum is calculated for */
   size_t size /**< [in] size [bytes] of the data the checksum is calculated for. Must be a multiple of 2. */
);

/**
* Calculates an 32 bit checksum over given data.
* \return the computed checksum
* \ingroup shared_toolbox_checksum
* \sdd{WI-14652}
*/
uint32_t Calc_Checksum_U32(
   const void *data, /**< [in] pointer to the data the checksum is calculated for */
   size_t size /**< [in] size [bytes] of the data the checksum is calculated for. Must be a multiple of 4. */
);

#ifdef __cplusplus
}
#endif
#endif
