/*=============================================================================================*\
* FILE: geo_projection.cpp
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of geometry projection functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "geometry/geo_projection.h"

#include "sg_math.h"

namespace sg
{
   namespace geometry
   {
      Footpoint_T make_projection(const geometry::Segment2D_T &segment, const geometry::Point2D_T &point, const bool f_limit_footpoint)
      {
         Footpoint_T foot_point{};

         const geometry::Point2D_T A{segment.first.x, segment.first.y};
         const geometry::Point2D_T B{segment.second.x, segment.second.y};
         const geometry::Point2D_T C{point};

         const geometry::Point2D_T vector_AB = B - A;
         const geometry::Point2D_T vector_BC = C - B;
         const geometry::Point2D_T vector_AC = C - A;

         const auto product_AB_BC = vector_AB * vector_BC;
         const auto product_AB_AC = vector_AB * vector_AC;
         const auto product_AB_AB = vector_AB * vector_AB;
         const auto dot_products  = product_AB_AC / product_AB_AB;

         /*
         Case 1, C beyond A
          (C)
           \
            \
            (A)------(B)
          */
         if ((product_AB_AC < 0.0F) && f_limit_footpoint)
         {
            foot_point.point = A;
         }
         /*
         Case 2, C beyond B
                       (C)
                       /
                      /
            (A)------(B)
          */
         else if ((product_AB_BC > 0.0F) && f_limit_footpoint)
         {
            foot_point.point = B;
         }
         /*
         Case 3, C between A and B
              (C)
               |
               |
          (A)------(B)
          */
         else
         {
            foot_point.point = C + dot_products * vector_AB - vector_AC;
         }

         // s parameter
         if (f_limit_footpoint)
         {
            foot_point.s = clamp(dot_products, 0.0F, 1.0F);
         }
         else
         {
            foot_point.s = dot_products;
         }

         (void) product_AB_BC; // MISRA

         return foot_point;
      }
   }
}