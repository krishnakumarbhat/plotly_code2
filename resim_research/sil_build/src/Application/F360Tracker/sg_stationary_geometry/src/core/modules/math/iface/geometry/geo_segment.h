/*=============================================================================================*\
* FILE: geo_segment.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry segment type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_SEGMENT_H
#define GEO_SEGMENT_H

#include <utility>

#include "geo_point.h"

namespace sg
{
   namespace geometry
   {
      using Segment2D_T = std::pair<Point2D_T, Point2D_T>;
   }
}
#endif
