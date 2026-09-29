#ifndef F360_CWD_DATA_H
#define F360_CWD_DATA_H
/*===================================================================================*\
* FILE: f360_cwd_data.h
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
\*===================================================================================*/

#include "f360_reuse.h"
#include "f360_mounting_location.h"

namespace f360_variant_A
{
   const int8_t cwd_buffer_size = 5;
   typedef struct CWD_Data_Tag
   {
      float32_t circular_buffer[MAX_NUMBER_OF_SENSORS][cwd_buffer_size];
      int8_t buffer_index[MAX_NUMBER_OF_SENSORS];
      F360_Mounting_Location_T mount_loc[MAX_NUMBER_OF_SENSORS];
   }CWD_Data_T;
}
#endif
