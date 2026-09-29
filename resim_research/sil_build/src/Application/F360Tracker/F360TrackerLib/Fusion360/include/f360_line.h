/*===================================================================================*\
* FILE: f360_line.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains class declaration of class Line
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_LINE_H
#define F360_LINE_H

#include "f360_reuse.h"
#include "f360_point.h"

namespace f360_variant_A
{
   struct Lines_Intersection
   {
      Point coordinates;    // Point of intersection
      bool exists;    // Indicator of intersection existance. If set to false, lines are parallel or identical
   };

   class Line
   {
   public:
      Line():a(0.0F), b(0.0F), c(0.0F) {};
      Line(const Line& input_line) : a(input_line.a), b(input_line.b), c(input_line.c) {}; //copy constructor
      Line(const float32_t a_factor, const float32_t b_factor, const float32_t c_factor);    // Create line ax + by + c = 0
      Line(const Point &point_a, const Point &point_b);   // Create line which goes through two given points

      Line& operator=(const Line& input_line)   // copy assignment operator
      {
         this->a = input_line.a;
         this->b = input_line.b;
         this->c = input_line.c;
         return *this; 
      };   

      float32_t Get_a() const { return this->a; };
      float32_t Get_b() const { return this->b; };
      float32_t Get_c() const { return this->c; };

      Lines_Intersection Find_Intersection(const Line &other) const;    // Find point of intersection between two lines
      float32_t Signed_Distance_To(const Point &p) const;    // Calculate distance to point. Value will be postitive for all points which met condition a*p.x + b*p.y + c > 0.

      void Get_Lines_Equally_Distant_From_Original(const float32_t distance, Line& ub_line, Line& lb_line) const;
      bool Is_Point_Between_Parallel_Lines(const Point& p, const Line& line_1, const Line& line_2) const;
      Line Get_Line_Perpendicular_At_Point(const Point& p) const;

   private:
      void Normalize();    // Normalize parameters in way that identity sqrt(a^2 + b^2) == 1 is true

   private:
      float32_t a;    // a parameter of line equation ax + by + c = 0
      float32_t b;    // b parameter of line equation ax + by + c = 0
      float32_t c;    // c parameter of line equation ax + by + c = 0
   };
}
#endif
