/*===================================================================================*\
* FILE: dc_interpolate_segment.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of interpolate segment method.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_INTERPOLATE_SEGMENT_H
#define DC_INTERPOLATE_SEGMENT_H

#include <bitset>

#include "dc_contour.h"
#include "dc_critical_region.h"
#include "geometry/geo_point.h"
#include "sg_constants.h"
#include "sg_contour.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief             This function performs subsegments and leftovers [dc_contour.subsegments, leftover].
       *
       * @param[out]        dc_contour - new DC contour to be filled with data
       * @param[out]        leftovers_mask - mask indicating leftovers
       * @param[in]         start_point - start point of interpolation
       * @param[in]         end_point - end point of interpolation
       * @param[in]         subsegment_length - length of subsegment
       * @param[in]         critical_region - polygon that needs to be checked if the point is inside
       *
       **/
      void interpolate_segment(DC_Contour_T &dc_contour,
                               std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &leftovers_mask,
                               const geometry::Point2D_T &start_point,
                               const geometry::Point2D_T &end_point,
                               const float subsegment_length,
                               const CriticalRegion &critical_region);
   }
}
#endif
