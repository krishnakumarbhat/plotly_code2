#ifndef OCCLUSION_TYPES_H
#define OCCLUSION_TYPES_H
/*===================================================================================*\
* FILE:  f360_occlusion_types.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*/

#include "f360_reuse.h"

namespace f360_variant_A
{
   const int16_t num_occlusion_sectors = 150; // 150 sectors: 0.8 degrees per sector, at 70m a sector is 1m wide.
   typedef struct F360_Occlusion_Data_Tag
   {
      float32_t range[num_occlusion_sectors];
      int16_t occluding_id[num_occlusion_sectors];
      float32_t sector_size;
      float32_t min_vcs_az;
      float32_t max_range;
   }F360_Occlusion_Data_T;

   enum F360_Occlusion_Status_T : uint8_t
   {
      OCCLUSION_STATUS_UNDEFINED = 0,
      OCCLUSION_STATUS_OCCLUDED = 1,
      OCCLUSION_STATUS_ON_EDGE = 2,
      OCCLUSION_STATUS_VISIBLE = 3
   };
}
#endif
