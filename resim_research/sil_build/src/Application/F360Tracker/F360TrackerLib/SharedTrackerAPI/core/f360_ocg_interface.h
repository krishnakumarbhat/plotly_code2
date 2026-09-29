#ifndef F360_OCG_INTERFACE_VARIANT_A_H
#define F360_OCG_INTERFACE_VARIANT_A_H
/*===================================================================================*\
* FILE: f360_ocg_interface.h
*====================================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
\*===================================================================================*/

#include "f360_reuse.h"

#ifndef DISABLE_OCG

#include "ocg_occupancy_grid_types.h"

#else

// Mock the OCG_Outputs_T type
namespace ocg
{
   typedef struct
   {
      uint8_t reserved;
   }OCG_Outputs_T;
}

#endif

#endif
