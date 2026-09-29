/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_serialization.h"
#include <assert.h>
#include <string.h> /* for memcpy>*/
#include "ml_macros.h"

/**
* Union used to convert data read from the buffer to different types.
* Direct casting is forbidden by QAC. This implementation seems to be
* safer than direct casting.
* \ingroup trigonometric_functions_serialization
*/
typedef union Four_Byte_Types_Tag
{
   uint32_t u32; /**< for conversion to/from uint32_t */
   float f; /**< for conversion to/from float */ /* PRQA S 3629 */ /* Union is used to convert from u32 into float */
}Four_Byte_Types_T;



/**
* Number of bits in a byte
* \ingroup trigonometric_functions_serialization
*/
#define ST_SRIO_BITS_IN_BYTE    (8)

/**
* number of bits to shift a full byte
* \ingroup trigonometric_functions_serialization
*/
#define ST_SRIO_SHIFT_ONE_BYTE (ST_SRIO_BITS_IN_BYTE)

/**
* number of bits to shift two full bytes
* \ingroup trigonometric_functions_serialization
*/
#define ST_SRIO_SHIFT_TWO_BYTES (2 * ST_SRIO_BITS_IN_BYTE)

/**
*number of bits to shift three full bytes
* \ingroup trigonometric_functions_serialization
*/
#define ST_SRIO_SHIFT_THREE_BYTES (3 * ST_SRIO_BITS_IN_BYTE)

uint32_t Serialize_Read_BE(char* p_data)
{
   size_t byte_offset = 0;
   uint8_t *p_byte_to_read;
   uint32_t  data_to_read = 0;

   assert(NULL != p_data);

   p_byte_to_read = (uint8_t*)p_data;/* PRQA S 0310 */ /* Cast intended */

   data_to_read = ((uint32_t)p_byte_to_read[byte_offset]) << ST_SRIO_SHIFT_THREE_BYTES;
   byte_offset++;
   data_to_read += ((uint32_t)p_byte_to_read[byte_offset]) << ST_SRIO_SHIFT_TWO_BYTES;
   byte_offset++;
   data_to_read += ((uint32_t)p_byte_to_read[byte_offset]) << ST_SRIO_SHIFT_ONE_BYTE;
   byte_offset++;
   data_to_read += (uint32_t)p_byte_to_read[byte_offset];

   return data_to_read;
}

uint32_t Serialize_Read_LE(char* p_data)
{
   size_t byte_offset = 0;
   uint8_t *p_byte_to_read;
   uint32_t  data_to_read = 0;

   assert(NULL != p_data);

   p_byte_to_read = (uint8_t*)p_data;/* PRQA S 0310 */ /* Cast intended */

   data_to_read = (uint32_t)p_byte_to_read[byte_offset];
   byte_offset++;
   data_to_read += ((uint32_t)p_byte_to_read[byte_offset]) << ST_SRIO_SHIFT_ONE_BYTE;
   byte_offset++;
   data_to_read += ((uint32_t)p_byte_to_read[byte_offset]) << ST_SRIO_SHIFT_TWO_BYTES;
   byte_offset++;
   data_to_read += ((uint32_t)p_byte_to_read[byte_offset]) << ST_SRIO_SHIFT_THREE_BYTES;

   return data_to_read;
}

float32_T Serialize_Read_LE_Float(char* p_data)
{
   Four_Byte_Types_T data;

   assert(NULL != p_data);

   data.u32 = Serialize_Read_LE(p_data);
   return data.f;
}

float32_T Serialize_Read_BE_Float(char* p_data)
{
   Four_Byte_Types_T data;

   assert(NULL != p_data);

   data.u32 = Serialize_Read_BE(p_data);
   return data.f;
}

size_t Deserialize_Float_Array_Le(
   char *p_data,
   size_t length,
   float *p_array
   )
{
   size_t i;
   size_t num_read_bytes = 0;

   assert(NULL != p_data);
   assert(NULL != p_array);

   for (i = 0; i < length; i++)
   {
      p_array[i] = Serialize_Read_LE_Float(&p_data[i * sizeof(float)]);
      num_read_bytes += sizeof(float);
   }

   return num_read_bytes;
}

size_t Deserialize_Float_Array_Be(
   char *p_data,
   size_t length,
   float *p_array
)
{
   size_t i;
   size_t num_read_bytes = 0;

   assert(NULL != p_data);
   assert(NULL != p_array);

   for (i = 0; i < length; i++)
   {
      p_array[i] = Serialize_Read_BE_Float(&p_data[i * sizeof(float)]);
      num_read_bytes += sizeof(float);
   }

   return num_read_bytes;
}

size_t Serialize_Write_Uint32(
   char* p_stream,
   uint32_t data)
{
   assert(NULL != p_stream);

   (void)memcpy(p_stream, &data, sizeof(uint32_t)); /* NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling) */
   return sizeof(uint32_t);
}


size_t Serialize_Write_Float_Array(
   char* p_stream,
   const float *p_data,
   size_t length)
{
   assert(NULL != p_stream);
   assert(NULL != p_data);
   assert(length > 0);

   (void)memcpy(p_stream, p_data, sizeof(float) * length); /* NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling) */

   return (sizeof(float) * length);
}

