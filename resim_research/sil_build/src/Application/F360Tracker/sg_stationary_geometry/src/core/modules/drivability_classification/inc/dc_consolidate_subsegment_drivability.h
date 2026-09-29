/*===================================================================================*\
* FILE: dc_consolidate_subsegment_drivability.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of consolidate_subsegment_drivability function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CONSOLIDATE_SUBSEGMENT_DRIVABILITY
#define DC_CONSOLIDATE_SUBSEGMENT_DRIVABILITY

#include "dc_contour_storage.h"
#include "dc_fused_contour_storage.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief            This function creates a list of fused SG and DC contours and
       * @brief            fills it with consolidated dc_contours. New vertices are created where
       * @brief            the subsegments change in drivability. The new drivability is then
       * @brief            assigned to this new segment. Subsegments are then removed entirely since
       * @brief            the change in drivability has been accounted for by the added vertices.
       *
       * @param[out]       fused_contour_storage - reference to list of fused SG and DC contours.
       * @param[in]        sg_contour_storage - reference to list of SG contours.
       * @param[in]        dc_contour_storage - reference to list of DC contours.
       **/
      void consolidate_subsegment_drivability(FusedContourStorage &fused_contour_storage,
                                              const ContourStorage &sg_contour_storage,
                                              const DCContourStorage &dc_contour_storage);
   }
}

#endif