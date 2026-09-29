/*===================================================================================*\
* FILE:  f360_check_if_point_is_inside_box.cpp
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functions that check if a point is inside a defined box.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_reuse.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"

namespace f360_variant_A
{
   static void Determine_Extended_BB_For_Water_Sprays(
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T &calib,
      BoundingBox &box
   );

   /*===========================================================================*\
   * FUNCTION: Check_If_Point_Is_Inside_Box_In_Same_CS()
   *===========================================================================f
   * RETURN VALUE:
   * bool f_is_inside_box
   *
   * PARAMETERS:
   * const float32_t x_point - X coordinate of point
   * const float32_t y_point - Y coordinate of point
   * float32_t const (&box)[2][2] - dimensions of box from a predefined center: bottom, top, left, right
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
   * This function determines if a point is inside a specified box where both the
   * point and box must be defined in the same coordinate system (CS). 
   *
   * PRECONDITIONS:
   * The box and its center must be aligned with the same coordinate system as for 
   * the point (x_point, y_point). 
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_If_Point_Is_Inside_Box_In_Same_CS(
      const float32_t x_point,
      const float32_t y_point,
      const float32_t (&box)[2][2])
   {
      const bool f_is_inside_box = ((box[0][0] <= x_point) &&
              (x_point <= box[0][1]) && 
              (box[1][0] <= y_point) &&
              (y_point <= box[1][1]));

      return f_is_inside_box;
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Vcs_Point_Is_Inside_Bounding_Box()
   *===========================================================================
   * RETURN VALUE:
   * bool f_inside_bounding_box
   *
   * PARAMETERS:
   * const float32_t x_point_vcs      - X coordinate of point in VCS (Vehicle Coordinate System)
   * const float32_t y_point_vcs      - Y coordinate of point in VCS
   * const F360_Object_Track_T & obj   - Struct with object properties
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
   * This function determines if a point is inside the bounding box of the specified object.
   *
   * PRECONDITIONS:
   * The point to check shall be given in VCS (Vehicle Coordinate System)
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Check_If_Vcs_Point_Is_Inside_Bounding_Box(
      const float32_t x_point_vcs,
      const float32_t y_point_vcs,
      const F360_Object_Track_T& obj)
   {
      float32_t x_point_tcs;
      float32_t y_point_tcs;

      Convert_VCS_Posn_To_TCS_Posn(
         x_point_vcs, 
         y_point_vcs, 
         obj.bbox.Get_Center().x, 
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         x_point_tcs, 
         y_point_tcs);

      const float32_t half_length = 0.5F * obj.bbox.Get_Length();
      const float32_t half_width = 0.5F * obj.bbox.Get_Width();

      float32_t box[2][2] = {};
      box[0][0] = -half_length;
      box[0][1] = half_length;
      box[1][0] = -half_width;
      box[1][1] = half_width;

      const bool f_inside_bounding_box = Check_If_Point_Is_Inside_Box_In_Same_CS(
         x_point_tcs,
         y_point_tcs,
         box);

      return f_inside_bounding_box;
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Point_Is_Inside_Extended_Bounding_Box()
   *===========================================================================
   * RETURN VALUE:
   * bool f_inside_extended_bounding_box
   *
   * PARAMETERS:
   * const Point & point_vcs           - Point in VCS coordinates
   * const F360_Object_Track_T & obj   - Object structure
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
   * This function determines if a point is inside the extended bounding box of the specified object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Check_If_Point_Is_Inside_Extended_Bounding_Box(
      const Point & point_vcs,
      const F360_Object_Track_T & obj)
   {
      BoundingBox box{ obj.bbox };
      Determine_Extended_Bounding_Box(obj, box);

      const bool f_inside_extended_bounding_box = box.Contains(point_vcs);

      return f_inside_extended_bounding_box;
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond()
   *===========================================================================
   * RETURN VALUE:
   * true if detection is inside extended conditional bbox
   *
   * PARAMETERS:
   * const Point &point_vcs,
   * const F360_Object_Track_T &obj,
   * const bool f_water_spray,ection_prop
   * const F360_Calibrations_T &calib
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
   * This function determines if a point is inside the extended conditional 
   * bounding box of the specified object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(
      const Point &point_vcs,
      const F360_Object_Track_T &obj,
      const bool f_water_spray,
      const F360_Calibrations_T &calib
   )
   {
      BoundingBox box{ obj.bbox };
      if (f_water_spray)
      {
         Determine_Extended_BB_For_Water_Sprays(obj, calib, box);
      }
      else
      {
         Determine_Extended_Bounding_Box(obj, box);
      }

      const bool f_inside_extended_conditional_bbox = box.Contains(point_vcs);

      return f_inside_extended_conditional_bbox;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Extended_Bounding_Box()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & obj
   * BoundingBox &box
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
   * Determines standard extended bounding box based on longitudinal and lateral
   * extension length
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Determine_Extended_Bounding_Box(
      const F360_Object_Track_T & obj, 
      BoundingBox &box
   )
   {
      box.Extend_Boundaries(
         obj.lat_buffer_zone_wid1,
         obj.lat_buffer_zone_wid2,
         obj.long_buffer_zone_len1,
         obj.long_buffer_zone_len2);
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Extended_BB_For_Water_Sprays()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Object_Track_T & obj,
   *  const F360_Calibrations_T &calib,
   *  BoundingBox &box
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
   * Determines extended bounding box based on longitudinal and lateral
   * extension length for water sprays detections
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Determine_Extended_BB_For_Water_Sprays(
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T &calib,
      BoundingBox &box
   )
   {
      box.Extend_Boundaries(
         obj.lat_buffer_zone_wid1 * calib.k_ws_bbox_wid_extension_factor,
         obj.lat_buffer_zone_wid2 * calib.k_ws_bbox_wid_extension_factor,
         obj.long_buffer_zone_len1 * calib.k_ws_bbox_len_extension_factor,
         obj.long_buffer_zone_len2 * calib.k_ws_bbox_len_extension_factor);
   }
}
