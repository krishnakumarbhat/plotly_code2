/*===================================================================================*\
* FILE: f360_calc_dist_to_edge.cpp
*====================================================================================
* Copyright © 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* full_filespec:
* version:
* derived_by:
* date_created:
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    Contains function definition of Calculate_Distance_To_Edge()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_calc_dist_to_edge.h"
#include "f360_math_func.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calculate_Distance_To_Edge()
   *===========================================================================
   * RETURN VALUE:
   * float32_t dist - Shortest distance from detection to edge. Returns -1 if end points coincides.
   *
   * PARAMETERS:
   * const Point p1 - TCS coordinates for the first end point of the edge
   * const Point p2 - TCS coordinates for the second end point of the edge
   * const Point det - TCS coordinates for the detection
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculates the shortest distance from a detection to the edge that is
   * spanned by the two passed end points. Shortest distance is either orthogonal
   * to the edge or the distance to one of the end points.
   * Theory from http://paulbourke.net/geometry/pointlineplane/
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   float32_t Calculate_Distance_To_Edge(
      const Point & p1,
      const Point & p2,
      const Point & det)
   {
      // Return variable
      float32_t dist;

      // Calculate length of edge
      const float32_t dx = p2.x - p1.x;
      const float32_t dy = p2.y - p1.y;
      const float32_t norm = dx*dx + dy*dy;

      // Edge points coincide and do not form an edge
      if (norm < F360_EPSILON)
      {
         dist = -1.0F;
      }
      else
      {
         float32_t min_dist_sq; // minimum distance squared

         // Evaluate if detection lies orthogonal to current edge or if it lies "below" or "above" edge limits
         const float32_t u = ((det.y - p1.y)*(p2.y - p1.y) + (det.x - p1.x)*(p2.x - p1.x)) / norm;

         if (u < 0.0F)
         {
            // Detection lies outside edges (shortest distance to edge is to end point p1)
            // Calculate distance from detection to the end point
            min_dist_sq = (p1.y - det.y)*(p1.y - det.y) + (p1.x - det.x)*(p1.x - det.x);
         }
         else if (u > 1.0F)
         {
            // Detection lies outside edges (shortest distance to edge is to end point p2)
            // Calculate distance from detection to the end point
            min_dist_sq = (p2.y - det.y)*(p2.y - det.y) + (p2.x - det.x)*(p2.x - det.x);

         }
         else
         {
            // Shortest distance from detection to edge is orthogonal to edge
            // Calculate distance between detection and intersection point on edge.
            const float32_t ix = p1.x + u * (p2.x - p1.x);
            const float32_t iy = p1.y + u * (p2.y - p1.y);
            min_dist_sq = (ix - det.x)*(ix - det.x) + (iy - det.y)*(iy - det.y);

         }
         dist = F360_Sqrtf(min_dist_sq);
      }
      return dist;
   }
}
