#ifndef ML_SERIALIZATION_H
#define ML_SERIALIZATION_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <stddef.h>
#include "reuse.h"

/**
 * \defgroup shared_toolbox_serialization Serialization
 * \ingroup shared_toolbox_private
 * Reading and writing needed data types to a serialized data stream
 */
/**
* Reads 4 bytes at given p_data in big endian.
* \return an uint32_t representing the read bytes.
* \ingroup shared_toolbox_serialization
*/
uint32_t Serialize_Read_BE(char* p_data /**< Address to read from */);

/**
* Reads 4 bytes at given p_data in little endian.
* \return an uint32_t representing the read bytes.
* \ingroup shared_toolbox_serialization
*/
uint32_t Serialize_Read_LE(char* p_data/**< Address to read from */);

/**
* Writes given data to given p_stream in host endianess.
* The caller has to ensure that the given p_stream is big enough to hold given data.
* \return Number of written bytes
* \ingroup shared_toolbox_serialization
*/
size_t Serialize_Write_Uint32(
   char* p_stream, /**< Stream to write to */
   uint32_t data/**< Data to be written */
);

/**
* Writes given data to given p_stream in host endianess.
* The caller has to ensure that the given p_stream is big enough to hold given data.
* \return Number of written bytes
* \ingroup shared_toolbox_serialization
*/
size_t Serialize_Write_Float_Array(
   char* p_stream, /**< Stream to write to */
   const float *p_data, /**< Pointer to data to be written */
   size_t length  /**< Length of data array */
);

/**
* Reads a float at given p_data in big endian.
* \return an float32_T representing the read data.
* \ingroup shared_toolbox_serialization
*/
float32_T Serialize_Read_BE_Float(char* p_data/**< Address to read from */);

/**
* Reads a float at given p_data in little endian.
* \return an float32_T representing the read data.
* \ingroup shared_toolbox_serialization
*/
float32_T Serialize_Read_LE_Float(char* p_data/**< Address to read from */);

/**
* Reads an array of floats from given p_data in little endian.
* \return number of read bytes
* \ingroup shared_toolbox_serialization
*/
size_t Deserialize_Float_Array_Le(
   char *p_data,  /**< data stream to read from */
   size_t length, /**< Size of the array to deserialize */
   float *p_array /**< Pointer to the array to deserialize */
);

/**
* Reads an array of floats from given p_data in big endian.
* \return number of read bytes
* \ingroup shared_toolbox_serialization
*/
size_t Deserialize_Float_Array_Be(
   char *p_data,  /**< data stream to read from */
   size_t length, /**< Size of the array to deserialize */
   float *p_array /**< Pointer to the array to deserialize */
);
#endif
