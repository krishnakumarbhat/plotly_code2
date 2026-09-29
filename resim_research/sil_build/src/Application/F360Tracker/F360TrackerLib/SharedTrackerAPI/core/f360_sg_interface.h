#ifndef F360_SG_INTERFACE_VARIANT_A_H
#define F360_SG_INTERFACE_VARIANT_A_H
/*===================================================================================*\
* FILE: f360_sg_interface.h
*====================================================================================
* Copyright (C) 2024 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
\*===================================================================================*/

#include "f360_reuse.h"

#ifndef DISABLE_SG

#include "sg_output.h"

#else

// Mock the SG_Output_T
namespace sg
{
   struct SG_Output_T
   {
      uint8_t reserved;
   };
}

#endif

#endif
