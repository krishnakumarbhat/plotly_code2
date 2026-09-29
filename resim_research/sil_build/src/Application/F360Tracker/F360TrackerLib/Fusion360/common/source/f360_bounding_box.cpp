/*===================================================================================*\
* FILE: f360_bounding_box.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of BoundingBox class methods.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#include "f360_angle.h"
#include "f360_bounding_box.h"
#include "f360_line.h"
#include "f360_math_func.h"
#include "f360_object_track.h"
#include "f360_vector.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         BoundingBox::BoundingBox()
   *
   * Description    Default constructor.
   *
   * Parameters     None.
   *
   * Returns
   * BoundingBox - instance of bounding box
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   BoundingBox::BoundingBox()
      : center(Point(0.0F, 0.0F)),
      orientation(0.0F),
      length(0.0F),
      width(0.0F)
   {
   }

   /*=========================================================================
   * Method         BoundingBox::BoundingBox()
   *
   * Description    Constructor which creates bounding box based on given parameters.
   *
   * Parameters
   * const Point center_point - center point of bbox
   * const float32_t bbox_length - bbox length
   * const float32_t bbox_width - bbox width
   * const float32_t bbox_orientation - bbox orientation
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
   BoundingBox::BoundingBox(
      const Point & center_point,
      const float32_t bbox_length,
      const float32_t bbox_width,
      const Angle & bbox_orientation)
      : center(center_point),
      orientation(bbox_orientation),
      length(bbox_length),
      width(bbox_width)
   {
   }

   /*=========================================================================
   * Method         BoundingBox::BoundingBox()
   *
   * Description    Constructor which creates bounding box spreaded on given points and oriented by given angle.
   *
   * Parameters
   * const Point * const first_point - pointer to the first element of points array
   * const uint32_t points_count - number of points within array
   * const float32_t bbox_orientation - bounding box orientation
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
   BoundingBox::BoundingBox(const Point(&points)[MAX_DETS_IN_OBJ_TRK], const uint32_t points_count, const Angle& bbox_orientation)
      : orientation(bbox_orientation)
   {
      float32_t min_x = INFTY;
      float32_t min_y = INFTY;
      float32_t max_x = -INFTY;
      float32_t max_y = -INFTY;

      for (uint32_t point_index = 0U; point_index < points_count; point_index++)
      {
         Point rotated_point{ points[point_index] };
         rotated_point.Rotate_About_Origin(-this->orientation);
         const float32_t rotated_point_x = rotated_point.x;
         const float32_t rotated_point_y = rotated_point.y;

         min_x = rotated_point_x < min_x ? rotated_point_x : min_x;
         max_x = max_x < rotated_point_x ? rotated_point_x : max_x;
         min_y = rotated_point_y < min_y ? rotated_point_y : min_y;
         max_y = max_y < rotated_point_y ? rotated_point_y : max_y;
      }

      const float32_t bbox_len = max_x - min_x;
      const float32_t bbox_wid = max_y - min_y;
      const float32_t bbox_center_x = (max_x + min_x) * 0.5F;
      const float32_t bbox_center_y = (max_y + min_y) * 0.5F;

      this->center.Set_Position(bbox_center_x, bbox_center_y);
      this->center.Rotate_About_Origin(orientation);
      this->length = bbox_len;
      this->width = bbox_wid;
   }

   /*=========================================================================
   * Method         BoundingBox::BoundingBox()
   *
   * Description    Constructor which creates bounding box with no orientation
   *
   * Parameters
   * const Point& left_rear - rectangle's left rear point
   * const Point& right_front - rectangle's right front point
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   right_front point is further (in positive directions x and y)
   *                than left_rear point.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   BoundingBox::BoundingBox(const Point& left_rear, const Point& right_front)
      : orientation{0.0F, 0.0F, 1.0F}
   {
      const float32_t center_x = 0.5F * (right_front.x + left_rear.x);
      const float32_t center_y = 0.5F * (left_rear.y + right_front.y);
      this->length = std::abs(right_front.x - left_rear.x);
      this->width = std::abs(right_front.y - left_rear.y);
      this->center.Set_Position(center_x, center_y);
   }

   /*=========================================================================
   * Method         BoundingBox::Translate()
   *
   * Description    Move bounding box by given x,y vector.
   *
   * Parameters
   * const float32_t dx - step in longitudinal direction
   * const float32_t dy - step in lateral direction
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
   void BoundingBox::Translate(const float32_t dx, const float32_t dy)
   {
      this->center.Translate(dx, dy);
   }

   /*=========================================================================
   * Method         BoundingBox::Extend_Boundaries()
   *
   * Description    Extend bounding box boundaries into all four directions individually.
   *                A positive value shifts the respective boundary away from the bbox center, a negative value shifts it towards the center.
   *
   * Parameters
   * const float32_t right - right hand side boundary extension increment
   * const float32_t left -  left hand side boundary extension increment
   * const float32_t front - front boundary extension increment
   * const float32_t rear - rear boundary extension increment
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
   void BoundingBox::Extend_Boundaries(const float32_t left, const float32_t right, const float32_t rear, const float32_t front)
   {
      this->width = std::abs(width + right + left);
      this->length = std::abs(length + front + rear);

      Point expanded_bbox_center = Point(0.5F * (front - rear), 0.5F * (right - left));
      expanded_bbox_center.Rotate_About_Origin(this->orientation);
      this->center.Translate(expanded_bbox_center.x,expanded_bbox_center.y);
   }

   /*=========================================================================
   * Method         BoundingBox::Rotate_About_Origin()
   *
   * Description    Rotate_About_Origin bounding box by given angle.
   *
   * Parameters
   * const Angle& rot_angle - rotation angle
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
   void BoundingBox::Rotate_About_Origin(const Angle& rot_angle)
   {
      this->center.Rotate_About_Origin(rot_angle);
      this->orientation += rot_angle;
   }

   /*===========================================================================*\
   * FUNCTION: Flip_By_The_Line()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const Line& input_line
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
   * This function flips bounding box by the input_line.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void BoundingBox::Flip_By_The_Line(const Line& input_line)
   {
      //get rotation coefficient
      const Angle double_slope_angle(2.0F * F360_Atan2f(input_line.Get_b(), input_line.Get_a()));
      // Calculate the signed distance
      const float32_t distance = input_line.Signed_Distance_To(this->Get_Center());

      this->Set_Center(this->Get_Center().x - (2.0F * distance * input_line.Get_a()), (this->Get_Center().y - (2.0F * distance * input_line.Get_b())));
      const float32_t current_orientation = this->Get_Orientation().Value();
      this->Set_Orientation(F360_DEG2RAD(-180.0F) + double_slope_angle.Value() - current_orientation);
   }


   /*=========================================================================
   * Method         BoundingBox::Get_Corners()
   *
   * Description    Return 4 corners of bounding box.
   *
   * Parameters     None
   *
   * Returns
   * BboxCorners - corners of bounding box
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   BboxCorners BoundingBox::Get_Corners() const
   {
      const float32_t len_half = this->length * 0.5F;
      const float32_t wid_half = this->width * 0.5F;

      BboxCorners corners{
           Point(-len_half, -wid_half),
           Point(-len_half, wid_half),
           Point(len_half, wid_half),
           Point(len_half, -wid_half)
      };

      for (uint8_t corner_index = 0U; corner_index < NUM_CORNERS_IN_RECTANGLE; corner_index++)
      {
         Point& bbox_corner = corners.points[corner_index];
         bbox_corner.Rotate_About_Origin(this->orientation);
         bbox_corner.Translate(this->center.x, this->center.y);
      }

      return corners;
   }

   /*=========================================================================
   * Method         BoundingBox::Overlap_Area()
   *
   * Description    Return area of intersection between bounding boxes.
   *
   * Parameters
   * const BoundingBox& other_bbox - second bounding box
   *
   * Returns
   * float32_t     overlapping area
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t BoundingBox::Overlap_Area(const BoundingBox & other_bbox) const
   {
      float32_t intersection_area = 0.0F;
      const Polygon poly = this->Intersection_Polygon(other_bbox);
      if (poly.Get_Corners_Count() > 2U)
      {
         intersection_area = poly.Get_Area();
      }
      return intersection_area;
   }

   /*=========================================================================
   * Method         BoundingBox::Intersection_Polygon()
   *
   * Description    Return polygon which is created as intersection of two overlapping bounding boxes. If bounding boxes don't overlap
   *                attribute corners_count of returned Polygon object will be less than 3.
   *
   * Parameters
   * const BoundingBox& other_bbox - second bounding box
   *
   * Returns
   * Polygon        intersection polygon
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Polygon BoundingBox::Intersection_Polygon(const BoundingBox & other_bbox) const
   {
      Polygon poly(*this);
      const BboxCorners other_bbox_corners = other_bbox.Get_Corners();

      for (uint8_t corners_b_idx = 0U; corners_b_idx < NUM_CORNERS_IN_RECTANGLE; corners_b_idx++)
      {
         if (poly.Get_Corners_Count() <= 2U) {
            break;
         }

         const uint8_t corners_b_next_index = (corners_b_idx + 1U) % NUM_CORNERS_IN_RECTANGLE;
         const Line bbox_edge(other_bbox_corners.points[corners_b_idx], other_bbox_corners.points[corners_b_next_index]);
         poly.Slice(bbox_edge);
      }
      return poly;
   }

   /*=========================================================================
   * Method         BoundingBox::Get_Area()
   *
   * Description    Return area of bounding box.
   *
   * Parameters     None
   *
   * Returns
   * float32_t     area value
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t BoundingBox::Get_Area() const
   {
      return this->width * this->length;
   }

   /*=========================================================================
 * Method         BoundingBox::Get_Slope_Of_Diagonal()
 *
 * Description    Return slope of the diagonal of a  bounding box. It's enough to return the positive slope of the diagonal,
 *                the other slope of the diagonal is the negative of this slope. Slopes of diagonals of rectangle
 *                are m and -m.
 *
 * Parameters     None
 *
 * Returns
 * float32_t     slope of diagonal
 *
 * Externals:     None.
 *
 * Precondition   None.
 *
 * Postcondition  None.
 *
 * Note           None.
 *========================================================================*/
   float32_t BoundingBox::Get_Slope_Of_Diagonal() const
   {
      float32_t slope;

      if (this->length > F360_EPSILON)
      {
         slope = this->width / this->length;
      }
      else
      {
         slope = INFTY;
      }
      return slope;
   }

   /*=========================================================================
   * Method         BoundingBox::Distance_To_Closest_Corner()
   *
   * Description    Return distance to closest corner of given other bounding box.
   *
   * Parameters
   * const BoundingBox& other_bbox - bbox which distances to corners are evaluated
   *
   * Returns
   * float32_t - distance to closest corner
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t BoundingBox::Distance_To_Closest_Corner(const BoundingBox& other_bbox) const
   {
      BoundingBox other_bbox_relative{ other_bbox };
      other_bbox_relative.Transform_To_Relative_Coordinate_System(this->center, this->orientation);
      BboxCorners other_bbox_corners = other_bbox_relative.Get_Corners();

      const float32_t len_half = this->length * 0.5F;
      const float32_t wid_half = this->width * 0.5F;

      float32_t closest_corner_dist_squared = INFTY;
      for (uint8_t corner_index = 0U; corner_index < NUM_CORNERS_IN_RECTANGLE; corner_index++)
      {
        Point& bbox_corner = other_bbox_corners.points[corner_index];
         bbox_corner.Absolute();
         bbox_corner.Translate(-len_half, -wid_half);
         bbox_corner.Saturate_On_0();

         const float32_t dist_to_corner_squared = bbox_corner.Distance_To_Origin_Squared();
         if (dist_to_corner_squared < closest_corner_dist_squared)
         {
            closest_corner_dist_squared = dist_to_corner_squared;
         }
      }

      return F360_Sqrtf(closest_corner_dist_squared);
   }

   /*=========================================================================
   * Method         BoundingBox::Combined_Distance_To()
   *
   * Description    Return combination of two distances metrics between bounding boxes - closest possible distance between bboxes and lateral spread
   *                - which is distance to furhest corner of second bbox to opposite boundary (left or right) of this bbox.
   *
   * Parameters
   * const BoundingBox & other_box - other bounding box
   *
   * Returns
   * Distance_Between_Bboxes - POD struct containing closest distance and lateral spread
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Distance_Between_Bboxes BoundingBox::Combined_Distance_To(const BoundingBox & other_box) const
   {
      Distance_Between_Bboxes combined_distance = this->Get_Closest_Distance_And_Lateral_Spread_Between(other_box);

      // At this point closest distance from this bbox perspective to other bbox (A->B) is caluclated. If it's larger than 0, there is
      // need to calculate distance from other bbox perspective to this (B->A). Lower value is closest possible distance. Distance equal to 0 menas
      // that bboxes overlap, so there is no need to calculate B->A distance.

      if (F360_EPSILON < combined_distance.closest_distance)
      {
         const float32_t distance_other_to_this = other_box.Distance_To_Closest_Corner(*this);
         if (distance_other_to_this < combined_distance.closest_distance)
         {
            combined_distance.closest_distance = distance_other_to_this;
         }
      }

      return combined_distance;
   }

   /*=========================================================================
   * Method         BoundingBox::Closest_Distance_To()
   *
   * Description    Return closest possible distance (metal2metal) to other bbox.
   *
   * Parameters
   * const BoundingBox& other_bbox - target bounding box
   *
   * Returns
   * float32_t - closest possible distance
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t BoundingBox::Closest_Distance_To(const BoundingBox& other_bbox) const
   {
      float32_t min_distance;

      const float32_t dist_AB = this->Distance_To_Closest_Corner(other_bbox);
      if (dist_AB < F360_EPSILON)  // if first distance is 0.0F then it means that bboxes overlap - no need to calc second distance
      {
         min_distance = 0.0F;
      }
      else
      {
         const float32_t dist_BA = other_bbox.Distance_To_Closest_Corner(*this);
         min_distance = std::min(dist_AB, dist_BA);
      }

      return min_distance;
   }

   /*=========================================================================
   * Method         BoundingBox::Contains()
   *
   * Description    Check if the input point falls into the bbox.
   *
   * Parameters
   * const Point& position - input point's position
   *
   * Returns
   * bool - flag indicating if input point is in the bbox
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note
   * Tempporary solution for a mix of rectangular bbox and circle bbox.
   * To be refactored.
   *========================================================================*/
   bool BoundingBox::Contains(const Point& position) const
   {
      Point tmp{ position };
      tmp.Transform_To_Relative_Coordinate_System(center, this->orientation);

      const float32_t half_length = this->length * 0.5F;
      const float32_t half_width = this->width * 0.5F;

      return ((-half_length <= tmp.x) && (tmp.x <= half_length) &&
         (-half_width <= tmp.y) && (tmp.y <= half_width));
   }

   /*=========================================================================
   * Method         BoundingBox::Circle_Contains()
   *
   * Description    Check if a circle bounding box contains a point
   *
   * Parameters
   * const Point& position
   *
   * Returns
   * bool - flag indicating if input point is in the circle bbox
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note
   * Tempporary solution for a mix of rectangular bbox and circle bbox.
   * To be refactored
   *========================================================================*/
   bool BoundingBox::Circle_Contains(const Point& position) const
   {
      const float32_t radius = this->length * 0.5F;
      const float32_t det_long_pos_delta = position.x - center.x;
      const float32_t det_lat_pos_delta  = position.y - center.y;
      const float32_t delta_pos_sq = F360_Get_Hypotenuse_Squared(det_long_pos_delta, det_lat_pos_delta) ;
      const bool f_inside_circle = delta_pos_sq <= radius * radius;

      return f_inside_circle;
   }

   /*=========================================================================
   * Method         BoundingBox::Distance_To_Center()
   *
   * Description    calculate the distance from an input point to circle bounding box center
   *
   * Parameters
   * const Point& position
   *
   * Returns
   * float32_t - calculated distance
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t BoundingBox::Distance_To_Center(const Point& position) const
   {
      const float32_t det_long_pos_delta = position.x - center.x;
      const float32_t det_lat_pos_delta  = position.y - center.y;
      return F360_Get_Hypotenuse(det_long_pos_delta, det_lat_pos_delta);
   }

   /*=========================================================================
   * Method         BoundingBox::Collides()
   *
   * Description    Checks if two bounding box collides each other.
   *
   * Parameters     const BoundingBox & other_bbox - bounding box to check collision with.
   *
   * Returns
   * bool - true if bounding boxes collides each other, otherwise false.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           The algorithm is based on "Separating Axis Theorem".
   *========================================================================*/
   bool BoundingBox::Collides(const BoundingBox & other_bbox) const
   {
      const BboxAxes bbox_A_axes = this->Get_Axes();
      const BboxAxes bbox_B_axes = other_bbox.Get_Axes();
      const Vector_T axes[] = { bbox_A_axes.parallel, bbox_A_axes.orthogonal, bbox_B_axes.parallel, bbox_B_axes.orthogonal };

      const BboxCorners bbox_A_corners = this->Get_Corners();
      const BboxCorners bbox_B_corners = other_bbox.Get_Corners();

      bool f_collision = true;
      for(uint8_t axis_index = 0U; axis_index < 4U; axis_index++)
      {
         const Vector_T &axis = axes[axis_index];
         const Interval<float32_t> bbox_A_projected_scope = bbox_A_corners.Project_Shadow_On(axis);
         const Interval<float32_t> bbox_B_projected_scope = bbox_B_corners.Project_Shadow_On(axis);

         if ((bbox_B_projected_scope.upper < bbox_A_projected_scope.lower) ||
             (bbox_A_projected_scope.upper < bbox_B_projected_scope.lower))
         {
            f_collision = false;
            break;
         }
      }

      return f_collision;
   }

   /*=========================================================================
* Method         BoundingBox::Get_Slope_from_bbox_center_to_point()
*
* Description    calculate the slope from an input point to bounding box center
*
* Parameters
* const Point& position
*
* Returns
* float32_t - slope from bbox center to a point
*
* Externals:     None.
*
* Precondition   None.
*
* Postcondition  None.
*
* Note           None.
*========================================================================*/
   float32_t BoundingBox::Get_Slope_from_bbox_center_to_point(const Point& position) const
   {
      const float32_t delta_y = position.y - this->center.y;
      const float32_t delta_x = position.x - this->center.x;
      float32_t slope;

      /*Not infinite slope*/
      if (std::fabs(delta_x) > F360_EPSILON)
      {
         slope = delta_y / delta_x;

      }
      else
      {
         slope =  INFTY;
      }
      return slope;
   }

/*=========================================================================
* Method         BoundingBox::Get_Closest_Corner_To_Point()
*
* Description    Finds the closest boundin box corner to a point and returns it
*
* Parameters
* const Point& position
*
* Returns
* Point - Bounding Box corner/vertex instance
*
* Externals:     None.
*
* Precondition   None.
*
* Postcondition  None.
*
* Note           None.
*========================================================================*/

   Point BoundingBox::Get_Closest_Corner_To_Point(const Point& position) const
   {
      BboxCorners corners = this->Get_Corners();
      float32_t distance[4] = { 0.0F };
      Point corner (0.0F,0.0F);

      distance[corners.CORNER_POSITION::FRONT_LEFT_INDEX] = Point::get_distance_between_points(position, corners.Front_Left());
      distance[corners.CORNER_POSITION::FRONT_RIGHT_INDEX] = Point::get_distance_between_points(position, corners.Front_Right());
      distance[corners.CORNER_POSITION::REAR_LEFT_INDEX] = Point::get_distance_between_points(position, corners.Rear_Left());
      distance[corners.CORNER_POSITION::REAR_RIGHT_INDEX] = Point::get_distance_between_points(position, corners.Rear_Right());

      // Find the index of the minimum distance
      uint32_t minDistanceIndex = 0U;
      for (uint8_t i = 1U; i < 4U; ++i)
      {
         if (distance[i] < distance[minDistanceIndex])
         {
            minDistanceIndex = i;
         }
      }

      /* Return the closest corner */
      switch (minDistanceIndex)
      {
      case BboxCorners::CORNER_POSITION::FRONT_LEFT_INDEX:
      {
         corner = corners.Front_Left();
         break;
      }
      case BboxCorners::CORNER_POSITION::FRONT_RIGHT_INDEX:
      {
         corner = corners.Front_Right();
         break;
      }
      case BboxCorners::CORNER_POSITION::REAR_LEFT_INDEX:
      {
         corner = corners.Rear_Left();
         break;
      }
      case BboxCorners::CORNER_POSITION::REAR_RIGHT_INDEX:
      {
         corner = corners.Rear_Right();
         break;
      }
      default:
         /* Handle unexpected case, return center */
         corner = this->center;
         break;
      }

      return corner;
   }

   /*=========================================================================
   * Method         BoundingBox::Transform_To_Relative_Coordinate_System()
   *
   * Description    Transform bounding box to other coordinate system by translation and rotation.
   *
   * Parameters
   * const Point & cs_center - CS system center
   * const Angle & cs_orientation - CS system orientation
   *
   * Returns        None
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void BoundingBox::Transform_To_Relative_Coordinate_System(const Point & cs_center, const Angle& cs_orientation)
   {
      this->Translate(-cs_center.x, -cs_center.y);
      this->Rotate_About_Origin(-cs_orientation);
   }

   /*=========================================================================
   * Method         BoundingBox::Lateral_Spread_Between()
   *
   * Description    Get lateral spread between this and other bboxes. It is considered as largest possible lateral distance between corners in this bbox coordinate system.
   *
   * Parameters
   * const BoundingBox & other_bbox - bbox for which lateral spread is evaluated
   *
   * Returns
   * float32_t - lateral spread
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t BoundingBox::Lateral_Spread_Between(const BoundingBox & other_bbox) const
   {
      BoundingBox other_bbox_relative = BoundingBox(other_bbox);
      other_bbox_relative.Transform_To_Relative_Coordinate_System(this->center, this->orientation);

      BboxCorners other_corners = other_bbox_relative.Get_Corners();
      for(uint8_t corner_index = 0U; corner_index < NUM_CORNERS_IN_RECTANGLE; corner_index++)
      {
         Point& bbox_corner = other_corners.points[corner_index];
         bbox_corner.Absolute();
      }

      const Point max_lat_point = *std::max_element(cmn::begin(other_corners.points), cmn::end(other_corners.points), [](const Point &first, const Point &second) { return (first.y < second.y); });
      return this->width * 0.5F + max_lat_point.y;
   }

   /*=========================================================================
   * Method         BoundingBox::Get_Closest_Distance_And_Lateral_Spread_Between()
   *
   * Description    Return combination of two distances metrics between bounding boxes - distance to closest corner of other_bbox and lateral spread
   *                - which is distance to farthest corner of second bbox to opposite boundary (left or right) of this bbox.
   *
   * Parameters
   * const BoundingBox & other_bbox - other bounding box
   *
   * Returns
   * Distance_Between_Bboxes - POD struct containing closest distance and lateral spread
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Distance_Between_Bboxes BoundingBox::Get_Closest_Distance_And_Lateral_Spread_Between(const BoundingBox & other_bbox) const
   {
      BoundingBox other_bbox_relative{ other_bbox };
      other_bbox_relative.Transform_To_Relative_Coordinate_System(this->center, this->orientation);
      BboxCorners corners_other = other_bbox_relative.Get_Corners();

      const float32_t len_half = this->length * 0.5F;
      const float32_t wid_half = this->width * 0.5F;

      float32_t closest_corner_dist_squared = INFTY;
      float32_t lateral_max = -INFTY;
      for (uint8_t corner_index = 0U; corner_index < NUM_CORNERS_IN_RECTANGLE; corner_index++)
      {
         Point& corner = corners_other.points[corner_index];
         corner.Absolute();
         const float32_t pos_lat = corner.y;
         if (lateral_max < pos_lat)
         {
            lateral_max = pos_lat;
         }

         corner.Translate(-len_half, -wid_half);
         corner.Saturate_On_0();
         const float32_t dist_to_origin_squared = corner.Distance_To_Origin_Squared();
         if (dist_to_origin_squared < closest_corner_dist_squared)
         {
            closest_corner_dist_squared = dist_to_origin_squared;
         }
      }
      Distance_Between_Bboxes combined_distance = {};
      combined_distance.closest_distance = F360_Sqrtf(closest_corner_dist_squared);
      combined_distance.lateral_spread = lateral_max + wid_half;
      return combined_distance;
   }

   /*=========================================================================
   * Method         BoundingBox::Get_Axes()
   *
   * Description    Returns axes as unit vector. Those axes are parallel and orthogonal to
   *                the bounding box orientation.
   *
   * Parameters     None
   *
   * Returns
   * BboxAxes - bounding box axes (parallel and orthogonal to the orientation) as unit vector
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   BboxAxes BoundingBox::Get_Axes() const
   {
      const Vector_T axis_parallel{ this->orientation.Cos(), this->orientation.Sin() };
      const Vector_T axis_orthogonal{ -this->orientation.Sin(), this->orientation.Cos() };
      const BboxAxes axes{ axis_parallel, axis_orthogonal };

      return axes;
   }

   /*=========================================================================
   * Method         BboxCorners::Project_Shadow_On()
   *
   * Description    Estimates scope of bounding box (corners) that is projected on axis (specified as vector)
   *
   * Parameters     axis_unit_vect -> axis as unit vector
   *
   * Returns
   * Interval<float32_t> - interval defined as minimum and maximum value.
   *
   * Externals:     None.
   *
   * Precondition   axis_unit_vect has to be unit vector.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Interval<float32_t> BboxCorners::Project_Shadow_On(const Vector_T & axis_unit_vect) const
   {
      Interval<float32_t> projected_interval;
      projected_interval.lower = INFTY;
      projected_interval.upper = -INFTY;

      for (uint8_t corner_index = 0U; corner_index < NUM_CORNERS_IN_RECTANGLE; corner_index++)
      {
         const Point& corner = points[corner_index];
         const Vector_T corner_vect{ corner.x, corner.y };
         const float32_t projected_corner = corner_vect * axis_unit_vect;

         if (projected_corner < projected_interval.lower)
         {
            projected_interval.lower = projected_corner;
         }

         if (projected_interval.upper < projected_corner)
         {
            projected_interval.upper = projected_corner;
         }
      }

      return projected_interval;
   }
}
