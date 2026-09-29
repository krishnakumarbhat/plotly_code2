#ifndef F360_LOG_DATA_H
#define F360_LOG_DATA_H
/*===================================================================================*\
* FILE:  f360_log_data.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
*/

#include <f360_reuse.h>

static const uint8_t f360_header_version = 1U;

typedef struct F360_Log_Header_Tag
{
   uint8_t version;
   uint8_t padding[3];
   uint32_t num_elements;
   uint64_t data_timstamp_us;
}F360_Log_Header_T;

typedef struct F360_Log_Data_Tag
{
   F360_Log_Header_T f360header;
   void* log_data_element;
} F360_Log_Data_T;

#endif 
