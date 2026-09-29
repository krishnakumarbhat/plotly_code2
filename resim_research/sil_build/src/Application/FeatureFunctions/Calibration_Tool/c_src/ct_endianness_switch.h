#ifndef CT_ENDIANNESS_SWITCH_H
#define CT_ENDIANNESS_SWITCH_H


/**
 * @file ct_endianness_switch.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Declaration of array reversing of SFL calibration tool.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "reuse.h"

/*===========================================================================*\
 * Typedefs
\*===========================================================================*/

typedef enum
{
   CT_ONE_BYTE  = 1,
   CT_TWO_BYTE  = 2,
   CT_FOUR_BYTE = 4
} CT_DATATYPE_NUM_BYTE_T;

/*===========================================================================*\
 * Function declaration
\*===========================================================================*/

void Ct_Reverse_Array(uint8_t *p_array_addr, size_t array_size_in_bytes, const CT_DATATYPE_NUM_BYTE_T number_of_bytes);

#endif /* CT_ENDIANNESS_SWITCH_H*/
