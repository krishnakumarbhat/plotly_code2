/*===========================================================================*\
* FILE: f360_rectangle.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Rectangle_T class()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_RECTANGLE_H
#define F360_RECTANGLE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   class Rectangle_T
   {
   public:
      Rectangle_T(
         const float32_t min_long_in,
         const float32_t max_long_in,
         const float32_t min_lat_in,
         const float32_t max_lat_in);

      ~Rectangle_T();

      bool Contains(
         const float32_t long_posn,
         const float32_t lat_posn) const;

   private:
      const float32_t min_long;
      const float32_t max_long;
      const float32_t min_lat;
      const float32_t max_lat;
   };
}
#endif
