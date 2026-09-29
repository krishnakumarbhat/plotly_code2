#ifndef F360_SORT_DATA_TYPE_H
#define F360_SORT_DATA_TYPE_H
/*===========================================================================*\
* FILE:  f360_sort_data_type.h
*============================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   static const uint32_t SORT_ARRAY_SIZE = (NUMBER_OF_CLUSTERS > MAX_NUMBER_OF_DETECTIONS) ? NUMBER_OF_CLUSTERS : MAX_NUMBER_OF_DETECTIONS;
   typedef struct F360_Sort_Data_Tag
   {
      float32_t data;
      uint32_t index;
   }F360_Sort_Data_T;

   typedef struct F360_Sort_Working_Data_Tag
   {
      F360_Sort_Data_T work_data[SORT_ARRAY_SIZE];
      F360_Sort_Data_T work_buffer[SORT_ARRAY_SIZE];
   }F360_Sort_Working_Data_T;
}
#endif
