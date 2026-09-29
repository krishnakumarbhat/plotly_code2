/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef ML_BIT_MANIPULATION_H
#define ML_BIT_MANIPULATION_H

#include "reuse.h"

/**
* \defgroup ml_bit_manipulation Bit Manipulation
* Accessory module for bit field manipulation.
* Allows to find positions of set bits, unset/set bits etc.
* \ingroup accessories
*/

/**
* returns the index of the highest set bit of given number
*
* \return         index of highest set bit, -1 if no bit is set
* \ingroup ml_bit_manipulation
* \sdd{WI-28402}
*/
int8_t Bitman_Get_Highest_Set_Bit_Index_BF32(bitfield32_t n /**< Number to find most significant bits index for*/);

/**
* returns the index of the lowest set bit of given number
*
* \return         index of lowest set bit, -1 if no bit is set
* \ingroup ml_bit_manipulation
* \sdd{WI-28401}
*/
int8_t Bitman_Get_Lowest_Set_Bit_Index_BF32(bitfield32_t n /**< Number to find least significant bits index for*/);

/**
* Returns a number where the bit at index is set to FALSE
* \return a number where the bit at index is set to FALSE
* \ingroup ml_bit_manipulation
* \sdd{WI-28396}
*/
bitfield32_t Bitman_Unset_Bit_BF32(
   bitfield32_t n /**< number to modify */,
   uint8_t      index /**< index to modify */);

/**
* Returns the bit at given index in given bitfield32_t n
* \return TRUE if requested bit is set.
* \ingroup ml_bit_manipulation
* \sdd{WI-28397}
*/
boolean_T Bitman_Get_Bit_BF32(
   bitfield32_t n /**< bit filed to read from */,
   uint8_t      index /**< index to read */);

/**
* Returns a number where the bit at index is set to TRUE
* \return a number where the bit at index is set to TRUE
* \ingroup ml_bit_manipulation
* \sdd{WI-28400}
*/
bitfield32_t Bitman_Set_Bit_BF32(
   bitfield32_t n /**< number to modify */,
   uint8_t      index /**< index to modify */);

/**
* returns the index of the nth lowest set bit of given number
* \return the index of the nth lowest set bit of given number
* \ingroup ml_bit_manipulation
* \sdd{WI-28404}
*/
int8_t Bitman_Get_Nth_Lowest_Set_Bit_Index_BF32(
   uint32_t number /**< Number to find least significant bits index for*/,
   uint32_t n /**< the least significant bits index of interest */);

/**
* Returns a number where the bit at index is inverted
* \return a number where the bit at index is inverted
* \ingroup ml_bit_manipulation
* \sdd{WI-28405}
*/
bitfield32_t Bitman_Invert_Bit_BF32(
   bitfield32_t n /**< number to modify */,
   uint8_t      index /**< index to modify */);

/**
* Sets the bit at index of p_n to TRUE
*
* \ingroup ml_bit_manipulation
* \sdd{WI-28406}
*/
void Bitman_Set_Bit_Pointer_BF32(
   uint32_t *p_n /**< number to modify */,
   uint8_t index /**< index to modify */);

/**
* Sets the bit at index of p_n to FALSE
*
* \ingroup ml_bit_manipulation
* \sdd{WI-28398}
*/
void Bitman_Unset_Bit_Pointer_BF32(
   uint32_t *p_n /**< number to modify */,
   uint8_t index /**< index to modify */);

/**
* Returns the number of set bits in given n.
* The implementation is fast for sparsely populated bit sets.
* \return the number of set bits in given n
* \ingroup ml_bit_manipulation
* \sdd{WI-28407}
*/
uint8_t Bitman_Count_Sparse_Set_Bits_BF32(uint32_t n /**< bit field to count bits in */);

#endif

