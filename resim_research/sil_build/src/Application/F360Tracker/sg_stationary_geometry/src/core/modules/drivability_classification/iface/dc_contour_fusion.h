/*===================================================================================*\
* FILE: dc_contour_fusion.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of contour fusion function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CONTOUR_FUSION_H
#define DC_CONTOUR_FUSION_H

#include "dc_contour_storage.h"
#include "dc_fused_contour_storage.h"
#include "sg_contour_storage.h"

namespace sg
{
   namespace dc
   {
      void contour_fusion(FusedContourStorage &fused_contour_storage,
                          const ContourStorage &sg_contour_storage,
                          const DCContourStorage &dc_contour_storage,
                          const float min_segment_length);
   }
}

#endif
