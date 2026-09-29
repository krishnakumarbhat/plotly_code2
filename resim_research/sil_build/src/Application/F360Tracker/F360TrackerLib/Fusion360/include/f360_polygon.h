/*===================================================================================*\
* FILE: f360_polygon.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Polygon class.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_POLYGON_H
#define F360_POLYGON_H

#include "f360_point.h"
#include "f360_constants.h"
#include "f360_math_func.h"
#include "f360_line.h"

namespace f360_variant_A
{
   class BoundingBox;

   class Polygon
   {
   public:
      // Constructors
      Polygon() = delete;    // Default constructor should not be used
      Polygon(const BoundingBox &bbox);    // Create polygon which covers given bbox

      // Modifiers
      void Add_Point(const Point &new_point);    // Add new corner to existing polygon
      void Slice(const Line &slicing_line);    // Perform slicing - modify polygon in way that only part on positive side of line is left

      // Getters
      inline uint8_t Get_Corners_Count() const { return corners_count; };    // Return number of polygon corners
      float32_t Get_Area() const;    // Return area of polygon

   private:
      Point corners[MAX_NUM_POLYGON_CORNERS];    // Polygon corners; only slots [0, corners_count) stores corners data
      uint8_t corners_count;    // Number of polygon corners
   };

}
#endif
