/*===================================================================================*\
* FILE: dc_contour_fusion.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains implementation of contour fusion function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_contour_fusion.h"

#include "dc_append_remaining_sg_contours.h"
#include "dc_consolidate_subsegment_drivability.h"
#include "dc_map_sg_covariance_to_dc_vertices.h"

namespace sg
{
   namespace dc
   {
      void contour_fusion(FusedContourStorage &fused_contour_storage,
                          const ContourStorage &sg_contour_storage,
                          const DCContourStorage &dc_contour_storage,
                          const float min_segment_length)
      {
         fused_contour_storage.clear();
         consolidate_subsegment_drivability(fused_contour_storage, sg_contour_storage, dc_contour_storage);
         map_sg_covariance_to_dc_vertices(fused_contour_storage, sg_contour_storage, min_segment_length);
         append_remaining_sg_contours(fused_contour_storage, sg_contour_storage);
      }
   }
}
