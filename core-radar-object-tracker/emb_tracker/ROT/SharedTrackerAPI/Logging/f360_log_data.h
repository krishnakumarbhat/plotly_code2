#ifndef F360_LOG_DATA_H
#define F360_LOG_DATA_H
/*===================================================================================*\
 * FILE:  f360_log_data.h
 *====================================================================================
 * Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "f360_reuse.h"

static const uint8_t f360_header_version = 2U;

typedef struct F360_Log_Header_Tag
{
   uint8_t version;
   uint8_t coordinate_system; // [0-3] 0: undefined, 1: SAE, 2: ISO
   uint8_t unused[2];
   uint32_t num_elements;
   uint64_t data_timstamp_us;
} F360_Log_Header_T;

typedef struct F360_Log_Data_Tag
{
   F360_Log_Header_T f360header;
   void *log_data_element;
} F360_Log_Data_T;

#endif
