/*===================================================================================*\
* FILE: dc_amend_segment.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration amend_segment function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_AMEND_SEGMENT_H
#define DC_AMEND_SEGMENT_H

#include <bitset>

#include "dc_common.h"
#include "dc_critical_region.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief    Interpolate segment at the front and back using projected segment to create new DC contour.
       *
       * @param    dc_contour - new DC_Contour.
       * @param    f_leftover - mask indicating which vertices were added from projected_segment.
       * @param    projected_segment - vertices to be added from old DC_Contour.
       * @param    end_state - last vertex in DC_Contour.
       * @param    critical_region - critical region object.
       * @param    f_end_vertex_in_region - is end vertex in critical region.
       * @param    subsegment_length - calibration parameter
       *
       **/
      void amend_segment(DC_Contour_T &dc_contour,
                         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_leftover,
                         const UpdatedSegment &projected_segment,
                         const geometry::Point2D_T &end_state,
                         const CriticalRegion &critical_region,
                         const bool f_end_vertex_in_region,
                         const float subsegment_length);
   }
}

#endif
