/*===========================================================================*\
* FILE: f360_internal_reflection_buffer_slot.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Internal_Reflection_Buffer_Slot()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_INTERNAL_REFLECTION_BUFFER_SLOT_H
#define F360_INTERNAL_REFLECTION_BUFFER_SLOT_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   struct Internal_Reflection_Buffer_Slot {
      float32_t range;                                 // [m] range of detection
      float32_t azimuth;                               // [rad] sensor azimuth of detection
      float32_t rcs;                             // [dB] rcs of detection
      uint32_t occurrence_count;                    // Counts how many tracker iterations a matching detection has been found
      uint32_t age;                                 // Counts how many tracker iterations this buffer slot has been storing a detection
      bool f_updated_this_cycle;                // TRUE if a matching detection has been found in the current cycle
      bool f_classified_as_internal_reflection; // TRUE if this buffer entry has occurred sufficiently often to consider it an internal reflection
      uint8_t padding[2];                           // Padding
   };

   static_assert(sizeof(Internal_Reflection_Buffer_Slot) == 24, "Internal_Reflection_Buffer_Slot: Wrong size.");
}


#endif
