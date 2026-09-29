/*===================================================================================*\
* FILE: f360_polygon.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of Polygon methods.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_polygon.h"
#include <algorithm>
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Polygon
   *
   * Description    Construct polygon on bounding box corners.
   *
   * Parameters     const BoundingBox &bbox - bounding box
   *
   * Returns        Polygon
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Polygon::Polygon(const BoundingBox &bbox)
   {
      const BboxCorners corners_to_add = bbox.Get_Corners();
      this->corners_count = 0U;
      for (uint8_t i = 0U; i < NUM_CORNERS_IN_RECTANGLE; i++)
      {
         this->Add_Point(corners_to_add.points[i]);
      }
   }

   /*=========================================================================
   * Method         Add_Point
   *
   * Description    Add new point to polygon. 
   *                Checking if new is not duplicate of existing ones is done.
   *                Checking if new edge will cross existing ones is NOT done. 
   *
   * Parameters     const Point & new_point - point to add
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Polygon::Add_Point(const Point & new_point)
   {
      // Make sure we are not adding a duplicated point
      const Point* const begin = cmn::begin(this->corners);
      const Point* const end = &this->corners[this->corners_count];
      const Point* const result = std::find(begin, end, new_point);

      const bool f_no_duplicate_found = (result == end);

      if (f_no_duplicate_found && (this->corners_count < MAX_NUM_POLYGON_CORNERS))
      {
         this->corners[this->corners_count] = new_point;
         this->corners_count++;
      }
   }

   /*=========================================================================
   * Method         Slice
   *
   * Description    Perform slicing operation - modify polygon in way that only part on positive side of line is left. 
   *                In case that line is crossing polygon - intersection points will be found and polygon will be modified.
   *                In case that whole polygon is on positive side of line - polygon will stay unchanged.
   *                In case that whole polygon is on negative side of line - corners_count will be set to 0 and polygon will be invalid.
   *
   * Parameters     const Line &slicing_line - line used in slicing operation
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Polygon::Slice(const Line &slicing_line)
   {
      // Calculate distances from all polygon corners to line
      float32_t corners_to_line_distances[MAX_NUM_POLYGON_CORNERS] = {};
      for (uint8_t polygon_corner_idx = 0U; polygon_corner_idx < this->corners_count; polygon_corner_idx++)
      {
         const Point polygon_corner = this->corners[polygon_corner_idx];
         corners_to_line_distances[polygon_corner_idx] = slicing_line.Signed_Distance_To(polygon_corner);
      }

      Point updated_polygon_corners[MAX_NUM_POLYGON_CORNERS];  // points array of sliced polygon
      uint8_t updated_polygon_corners_count = 0U;

      for (uint8_t polygon_corner_idx = 0U; polygon_corner_idx < this->corners_count; polygon_corner_idx++)
      {
         uint8_t polygon_corner_next_idx = polygon_corner_idx + 1U;
         if (polygon_corner_next_idx == this->corners_count)  // handle last-first corners segment case
         {
            polygon_corner_next_idx = 0U;
         }

         const Point corner = this->corners[polygon_corner_idx];
         const float32_t corner_distance = corners_to_line_distances[polygon_corner_idx];
         const float32_t next_corner_distance = corners_to_line_distances[polygon_corner_next_idx];

         if (corner_distance >= 0.0F)  // point lays 'over' the line - it should be added to new polygon
         {
            updated_polygon_corners[updated_polygon_corners_count] = corner;
            updated_polygon_corners_count++;
         }

         const bool corners_on_two_sides_of_line = (corner_distance * next_corner_distance) < 0.0F;
         if (corners_on_two_sides_of_line)  // neighbor corners lays on two sides of line - intersection needs to be found
         {
            const Point corner_next = this->corners[polygon_corner_next_idx];
            const Lines_Intersection intersect = slicing_line.Find_Intersection(Line(corner, corner_next));
            if (intersect.exists)
            {
               updated_polygon_corners[updated_polygon_corners_count] = intersect.coordinates;
               updated_polygon_corners_count++;
            }
         }

      }

      // Update this polygon instance with new, sliced one.
      if (updated_polygon_corners_count > 0U)
      {
         (void)std::copy(cmn::begin(updated_polygon_corners), &updated_polygon_corners[updated_polygon_corners_count], cmn::begin(this->corners));
      }
      this->corners_count = updated_polygon_corners_count;
   }

   /*=========================================================================
   * Method         Get_Area
   *
   * Description    Return area of polygon
   *
   * Parameters     None.
   *
   * Returns        float32_t polygon area
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Polygon::Get_Area() const
   {
      float32_t sum = 0.0F;
      for (uint8_t corner_index = 0U; corner_index < this->corners_count; corner_index++)
      {
         const Point corner = this->corners[corner_index];
         const Point corner_next = this->corners[(corner_index + 1U) % this->corners_count];
         sum += (corner.x * corner_next.y) - (corner.y * corner_next.x);
      }
      return std::abs(sum) * 0.5F;
   }

}
