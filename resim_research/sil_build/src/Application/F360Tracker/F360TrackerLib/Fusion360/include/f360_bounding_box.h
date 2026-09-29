/*===================================================================================*\
* FILE: f360_bounding_box.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of BoundingBox class.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_BOUNDING_BOX_H
#define F360_BOUNDING_BOX_H

#include "f360_reuse.h"
#include "f360_point.h"
#include "f360_vector.h"
#include "f360_angle.h"
#include "f360_polygon.h"
#include "f360_interval.h"

namespace f360_variant_A
{

   struct BboxAxes
   {
      Vector_T parallel;   // [-]
      Vector_T orthogonal; // [-]
   };

   struct BboxCorners
   {
      // Enum representing position of given corner in points array 
      enum CORNER_POSITION : uint8_t
      {
         REAR_LEFT_INDEX,
         REAR_RIGHT_INDEX,
         FRONT_RIGHT_INDEX,
         FRONT_LEFT_INDEX,
      };

      // Member array containing points which represents 4 corners
      Point points[NUM_CORNERS_IN_RECTANGLE];

      // Constructors
      BboxCorners(): points() {};
      BboxCorners(const Point &rear_left_corner, const Point &rear_right_corner, const Point &front_right_corner, const Point &front_left_corner)
      {
         this->points[CORNER_POSITION::REAR_LEFT_INDEX] = rear_left_corner;
         this->points[CORNER_POSITION::REAR_RIGHT_INDEX] = rear_right_corner;
         this->points[CORNER_POSITION::FRONT_RIGHT_INDEX] = front_right_corner;
         this->points[CORNER_POSITION::FRONT_LEFT_INDEX] = front_left_corner;
      }

      // Other methods
      Point& Rear_Left()   { return this->points[CORNER_POSITION::REAR_LEFT_INDEX]; } ;
      Point& Rear_Right()  { return this->points[CORNER_POSITION::REAR_RIGHT_INDEX]; };
      Point& Front_Right() { return this->points[CORNER_POSITION::FRONT_RIGHT_INDEX]; };
      Point& Front_Left()  { return this->points[CORNER_POSITION::FRONT_LEFT_INDEX]; };

      const Point& Rear_Left() const { return this->points[CORNER_POSITION::REAR_LEFT_INDEX]; };
      const Point& Rear_Right() const { return this->points[CORNER_POSITION::REAR_RIGHT_INDEX]; };
      const Point& Front_Right() const { return this->points[CORNER_POSITION::FRONT_RIGHT_INDEX]; };
      const Point& Front_Left() const { return this->points[CORNER_POSITION::FRONT_LEFT_INDEX]; };

      Interval<float32_t> Project_Shadow_On(const Vector_T & axis_unit_vect) const;
   };

   struct Distance_Between_Bboxes
   {
      float32_t closest_distance;
      float32_t lateral_spread;
   };

   class BoundingBox
   {
   public:
      // Constructors
      BoundingBox();
      BoundingBox(
         const Point & center_point,
         const float32_t bbox_length,
         const float32_t bbox_width,
         const Angle & bbox_orientation
      );
      BoundingBox(const Point (&points)[MAX_DETS_IN_OBJ_TRK], const uint32_t points_count, const Angle& bbox_orientation);
      BoundingBox(const Point& left_rear, const Point& right_front);

      // Setters
      inline void Set_Center(const Point & cent) { this->center = cent; }
      inline void Set_Center(const float32_t xpos, const float32_t ypos) { this->center.Set_Position(xpos, ypos); }
      inline void Set_Length(const float32_t len) { this->length = len; };
      inline void Set_Width(const float32_t wid) { this->width = wid; };
      inline void Set_Orientation(const float32_t ori) { (void)this->orientation.Value(ori); (void)this->orientation.Normalize(); };
      inline void Set_Orientation(const Angle & ori) { this->orientation = ori; (void)this->orientation.Normalize(); };

      // Getters
      inline Point Get_Center() const { return this->center; };
      inline float32_t Get_Length() const { return this->length; };
      inline float32_t Get_Width() const { return this->width; };
      inline Angle Get_Orientation() const { return this->orientation; };
      BboxCorners Get_Corners() const;    // Return BboxCorners that is corners container
      float32_t Get_Area() const;    // Returns area of bbox
      float32_t Get_Slope_Of_Diagonal() const;

      // Transformations
      void Transform_To_Relative_Coordinate_System(const Point& cs_center, const Angle& cs_orientation);    // Transform bounding box to other coordinate system
      void Translate(const float32_t dx, const float32_t dy);    // Move bounding box by given x,y vector
      void Rotate_About_Origin(const Angle& rot_angle);    // Rotate bounding box by given angle, center of rotation is placed in (0, 0)
      void Extend_Boundaries(const float32_t left, const float32_t right, const float32_t rear, const float32_t front);    // Extend boundaries of the bounding box. Extension can be both positive (outwards) and negative (inwards)
      void Flip_By_The_Line(const Line& input_line); // Flip bbox by input_line

      Distance_Between_Bboxes Combined_Distance_To(const BoundingBox& other_box) const;    // Return combination of two distances metrics between bboxes
      float32_t Closest_Distance_To(const BoundingBox& other_bbox) const;   // Return closest possible distance (metal2metal) to other bbox.
      float32_t Lateral_Spread_Between(const BoundingBox& other_bbox) const;    // Return lateral spread between bboxes
      float32_t Distance_To_Closest_Corner(const BoundingBox& other_bbox) const;    // Return distance to closest corner of given other bounding box
      float32_t Overlap_Area(const BoundingBox& other_bbox) const;    // Return area of overlap between two bboxes
      bool Contains(const Point& position) const;    // Check if given point is inside bbox
      float32_t Distance_To_Center(const Point& position) const; //distance to circle center
      bool Circle_Contains(const Point& position) const; // Check if given point is inside bbox
      bool Collides(const BoundingBox& other_bbox) const;    // Check if two bboxes collide with each other
      float32_t Get_Slope_from_bbox_center_to_point(const Point& position) const;
      Point Get_Closest_Corner_To_Point(const Point& position) const; // Returns the closest corner to a point. If multiple corners are close, returns only one
      BboxAxes Get_Axes() const;    // Return bbox axes as unit vectors

   private:
      Distance_Between_Bboxes Get_Closest_Distance_And_Lateral_Spread_Between(const BoundingBox& other_bbox) const;    // Return combination of two distances metrics between bboxes 
      Polygon Intersection_Polygon(const BoundingBox& other_bbox) const;   // Return polygon of overlapping area

      Point center;    // Position of bbox center
      Angle orientation;    // Angle of bbox orientation
      float32_t length;    // Length of bbox
      float32_t width;    // Width of bbox
   };
}
#endif
