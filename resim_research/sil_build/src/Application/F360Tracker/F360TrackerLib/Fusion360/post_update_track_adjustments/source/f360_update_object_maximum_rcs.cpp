/*===========================================================================*\
* FILE: f360_update_object_maximum_rcs.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*==========================================================================================*/

#include "f360_update_object_maximum_rcs.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Object_Maximum_Rcs()
   * ===========================================================================
   * DESCRIPTION:
   * This function filters maximum RCS of associated detections
   \*===========================================================================*/
   
   void Update_Object_Maximum_Rcs(
      const rspp_variant_A::RSPP_Detection_T(&raw_dets)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object)
   {
      float32_t max_rcs = -1000.0F;
      for (uint32_t i = 0U; i < object.ndets; i++)
      {
         const uint32_t det_idx = object.detids[i] - 1U;
         max_rcs = fmaxf(max_rcs, raw_dets[det_idx].raw.rcs);
      }

      if (max_rcs > object.maximum_rcs)
      {
         object.maximum_rcs = max_rcs;
      }
      else
      {
         if (object.status == F360_OBJECT_STATUS_UPDATED)
         {
            object.maximum_rcs = object.maximum_rcs * 0.99F + max_rcs * 0.01F;
         }
         else
         {
            object.maximum_rcs -= 0.7F;
            object.maximum_rcs = fmaxf(object.maximum_rcs, -50.0F);
         }
      }
   }
}
