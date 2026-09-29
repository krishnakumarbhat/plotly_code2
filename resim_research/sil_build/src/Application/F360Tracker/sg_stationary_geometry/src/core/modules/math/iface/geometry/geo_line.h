/*=============================================================================================*\
* FILE: geo_line.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry line type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_LINE_H
#define GEO_LINE_H

#include "geometry/geo_point.h"

namespace sg
{
   namespace geometry
   {
      struct Line_T
      {
         // general line equation coefficients
         // ax + by + c = 0
         float a;
         float b;
         float c;

         Line_T(float a_in, float b_in, float c_in) : a{a_in}, b{b_in}, c{c_in} {};
         Line_T(const geometry::Point2D_T &point1, const geometry::Point2D_T &point2);

         /*=============================================================================================*\
          * Function:      sg::geometry::Point2D_T intersection(const Line_T& other_line) const
          *
          * Description:   Find intersection of self with line 'other_line'.
          *
          * Parameters:    const Line_T& other_line -  line with which to find the intersection
          *
          * Returns:       sg::geometry::Point2D_T
          *=============================================================================================*/
         sg::geometry::Point2D_T intersection(const Line_T &other_line) const;

         /*=============================================================================================*\
          * Function:      geometry::Point2D_T orthogonal_projection(const geometry::Point2D_T& to_project) const
          *
          * Description:   Find orthogonal projection of point 'to_project' to self.
          *
          * Parameters:    const geometry::Point2D_T& to_project -  point to be orthogonally projected
          *
          * Returns:       sg::geometry::Point2D_T
          *=============================================================================================*/
         geometry::Point2D_T orthogonal_projection(const geometry::Point2D_T &to_project) const;
      };
   }
}
#endif