/*===========================================================================*\
* FILE: f360_static_env_polys_support_functions.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function definitions of support functions to Static Environment Polynomials
*   It contains functions to flag detections/objects as "on" or "behind" the active polynomials.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_static_env_polys_support_functions.h" 
#include "f360_constants.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Check_If_Point_Is_Behind_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_behind_sep
   *
   * PARAMETERS:
   *  const float32_t point_x_start,
   *  const float32_t point_x_end,
   *  const float32_t point_y_start,
   *  const float32_t point_y_end,
   *  const float32_t k, Linear coefficient of vector start to end expressed as a linear equation
   *  const float32_t m, Constant of vector start to end expressed as a linear equation
   *  const Static_Env_Poly_T& sep,
   *  const F360_Calibrations_T& calibs,
   *  Point& output_intersection
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
   * Evaluates if a point is behind a longi stat curve. A point is considered as
   * behind if an intersection with a SEP exist in the SEP's valid interval from
   * a start point (sensor for detections / VCS origin for objects). The intersection
   * must also be well within the valid interval of the SEP, see calibration
   * "k_lsc_hysterisis_bias". This to protect from the ambiguity that arises
   * when we consider objects as points while in reality they are boxes that can
   * be partially behind a curve.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_If_Point_Is_Behind_SEP(
      const float32_t point_x_start,
      const float32_t point_x_end,
      const float32_t point_y_start,
      const float32_t point_y_end,
      const float32_t k,
      const float32_t m,
      const Static_Env_Poly_T &sep,
      const F360_Calibrations_T &calibs,
      Point &output_intersection)
   {

      bool f_behind_sep;
      if (std::abs(k) > calibs.k_sep_max_k_coeff_for_lateral_line)
      {
         /* Start and end x-points are almost equal which means k value is very large
            Calculating an intersection in this case may be incorrect due to floating point errors
            Since start and end x-point is almost equal we approximate the intersection to be
            equal to the end x-point.
         */

         if ((sep.lower_limit < point_x_end) && (point_x_end < sep.upper_limit))
         {
            const float32_t y_int = sep.SEP_Lateral_Pos_At(point_x_end);

            if (F360_Sign(point_y_end) == F360_Sign(y_int))
            {
               // Point is on the same side of host as the intersection
               if (std::abs(y_int) < std::abs(point_y_end))
               {
                  f_behind_sep = true;
                  output_intersection.x = point_x_end;
                  output_intersection.y = y_int;
               }
               else
               {
                  f_behind_sep = false;
                  output_intersection.x = INFTY;
                  output_intersection.y = INFTY;
               }
            }
            else
            {
               // Point is on opposite side of host as the intersection
               f_behind_sep = false;
               output_intersection.x = INFTY;
               output_intersection.y = INFTY;
            }
         }
         else
         {
            // Intersection does not exist in valid SEP interval
            f_behind_sep = false;
            output_intersection.x = INFTY;
            output_intersection.y = INFTY;
         }
      }
      else
      {
         Point intersection_1;
         Point intersection_2;
         const bool f_intersects = Calc_Intersection_With_SEP(k, m, sep, calibs, intersection_1, intersection_2);


         if (f_intersects)
         {
            // Check that intersection exists on the finite vector
            // Check that intersection lies on valid curve interval
            // Check that the intersection point have the same lateral sign as the point the end point of the finite vector
            const float32_t x_min = std::min(point_x_start, point_x_end);
            const float32_t x_max = std::max(point_x_start, point_x_end);
            const float32_t y_min = std::min(point_y_start, point_y_end);
            const float32_t y_max = std::max(point_y_start, point_y_end);

            const bool f_behind_sep_1 = (
               (intersection_1.x >= x_min) &&
               (intersection_1.x <= x_max) &&
               (intersection_1.x > sep.lower_limit) &&
               (intersection_1.x < sep.upper_limit) &&
               (y_min <= intersection_1.y) && (intersection_1.y <= y_max));

            const bool f_behind_sep_2 = (
               (intersection_2.x >= x_min) &&
               (intersection_2.x <= x_max) &&
               (intersection_2.x > sep.lower_limit) &&
               (intersection_2.x < sep.upper_limit) &&
               (y_min <= intersection_2.y) && (intersection_2.y <= y_max));

            if (f_behind_sep_1 && f_behind_sep_2)
            {
               // Two valid intersections found. Pick the closest solution as the interesting one
               f_behind_sep = true;
               if (std::abs(intersection_1.x) < std::abs(intersection_2.x))
               {
                  output_intersection = intersection_1;
               }
               else
               {
                  output_intersection = intersection_2;
               }
            }
            else if (f_behind_sep_1)
            {
               f_behind_sep = true;
               output_intersection = intersection_1;
            }
            else if (f_behind_sep_2)
            {
               f_behind_sep = true;
               output_intersection = intersection_2;
            }
            else
            {
               // No valid intersection exists, point is not behind curve
               f_behind_sep = false;
               output_intersection.x = INFTY;
               output_intersection.y = INFTY;
            }
         }
         else
         {
            // No intersection exists, point is not behind curve
            f_behind_sep = false;
            output_intersection.x = INFTY;
            output_intersection.y = INFTY;
         }
      }

      return f_behind_sep;
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Point_Is_On_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_on_sep
   *
   * PARAMETERS:
   *  const float32_t point_x
   *  const float32_t point_y
   *  const Static_Env_Poly_T& sep
   *  const float32_t on_curve_threshold
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
   * This function evaluates if a point is classified as "on" a longi stat curve
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_If_Point_Is_On_SEP(
      const float32_t point_x,
      const float32_t point_y,
      const Static_Env_Poly_T &sep,
      const float32_t on_curve_threshold)
   {
      bool f_on_sep;

      if ((point_x > sep.lower_limit) && (point_x < sep.upper_limit))
      {
         const float32_t y_curv = sep.SEP_Lateral_Pos_At(point_x);

         const float32_t delta = point_y - y_curv;

         if (std::abs(delta) < on_curve_threshold)
         {
            f_on_sep = true;
         }
         else
         {
            f_on_sep = false;
         }
      }
      else
      {
         f_on_sep = false;
      }

      return f_on_sep;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Intersection_With_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_intersect_found
   *
   * PARAMETERS:
   *  const float32_t k, Linear coefficient of vector start to end expressed as a linear equation
   *  const float32_t m, Constant of vector start to end expressed as a linear equation
   *  const Static_Env_Poly_T& sep
   *  Point& intersection_1 - output for first of two found intersection points
   *  Point& intersection_2 - output for second of two found intersection points
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
   * Calculates and returns the intersection points of a line with a sep
   * in VCS coordinates. Both solutions are returned in x1 and x2 respectively
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Calc_Intersection_With_SEP(
      const float32_t k,
      const float32_t m,
      const Static_Env_Poly_T &sep,
      const F360_Calibrations_T &calibs,
      Point &intersection_1,
      Point &intersection_2)
   {
      bool f_intersection_found;
      if (std::abs(sep.p2) < calibs.k_sep_p2_coeff_poly_linear_thr)
      {
         // "p2" coefficient is very small, we may approximate the polynomial as a line
         f_intersection_found = Find_X_Intersect_Between_Two_Lines(sep.p1, sep.p0, k, m, intersection_1.x);
         if (f_intersection_found)
         {
            intersection_1.y = k * intersection_1.x + m;
         }
         intersection_2.x = INFTY;
         intersection_2.y = INFTY;
      }
      else
      {
         // "p2" coefficient is large, we need to solve intersection for the full polynomial
         f_intersection_found = Find_X_Intersect_Between_Line_And_2_Deg_Poly(sep.p2, sep.p1, sep.p0, k, m, intersection_1.x, intersection_2.x);
         if (f_intersection_found)
         {
            intersection_1.y = sep.SEP_Lateral_Pos_At(intersection_1.x);
            intersection_2.y = sep.SEP_Lateral_Pos_At(intersection_2.x);
         }
      }

      return f_intersection_found;
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Objects_On_And_Behind_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T &tracker_info,
   *  const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   *  const F360_Calibrations_T &calibs,
   *  const F360_Globals_T &globals,
   *  F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function flags objects on and behind Static Environment Polynomials. 
   * The SEP array is divided into two parts, first a number of LSC curves, then a number of CWD curves. 
   * Each SEP will be considered in the order that they are located in the array. If an object is 
   * behind multiple SEPs it will be flagged as behind the one that is located first in the array.
   * The LSC curves are ordered by their priority, with the most prioritized curve located first.
   *
   * Only CTCA objects are evaluated for "behind curve" status.
   * Objects that have been flagged as "on" will never be flagged as "behind"
   *
   \*===========================================================================*/
   void Flag_Objects_On_And_Behind_SEP(
      const F360_Tracker_Info_T &tracker_info,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      Flag_Objects_On_SEP(tracker_info, sep, calibs, object_tracks);
      Flag_Objects_Behind_SEP(tracker_info, sep, calibs, globals, object_tracks);
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Single_Object_On_And_Behind_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   *  const F360_Calibrations_T &calibs,
   *  const F360_Globals_T &globals,
   *  F360_Object_Track_T &obj
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function flags a single object on and behind Static Environment Polynomials.
   * The SEP array is divided into two parts, first a number of LSC curves, then a number of CWD curves.
   * Each SEP will be considered in the order that they are located in the array. If an object is
   * behind multiple SEPs it will be flagged as behind the one that is located first in the array.
   * The LSC curves are ordered by their priority, with the most prioritized curve located first.
   *
   * Only CTCA objects are evaluated for "behind curve" status.
   * Objects that have been flagged as "on" will never be flagged as "behind"
   *
   \*===========================================================================*/
   void Flag_Single_Object_On_And_Behind_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T &obj)
   {
      Flag_Single_Object_On_SEP(sep, calibs, obj);
      Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Objects_Behind_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T &tracker_info,
   *  const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   *  const F360_Calibrations_T &calibs,
   *  const F360_Globals_T &globals,
   *  F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function flags objects behind Static Environment Polynomials. The curve array
   * is sorted in prioritized order, thus if the object is behind several
   * curves it will only be flagged as behind the most prioritized curve.
   *
   * Only CTCA objects are evaluated for "behind curve" status.
   * Objects that have been flagged as "on" will never be flagged as "behind"
   *
   * PRECONDITIONS:
   * Call Flag_Objects_On_Longi_Stat_Curves() before call to this function.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Flag_Objects_Behind_SEP(
      const F360_Tracker_Info_T &tracker_info,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;

         Flag_Single_Object_Behind_SEP(
            sep,
            calibs,
            globals,
            object_tracks[obj_idx]);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Single_Object_Behind_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   *  const F360_Calibrations_T &calibs,
   *  const F360_Globals_T &globals,
   *  F360_Object_Track_T &obj
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
   * This function flags a single object behind Static Environment Polynomials. The curve array
   * is sorted in prioritized order, thus if the object is behind several
   * curves it will only be flagged as behind the most prioritized curve.
   *
   * Only CTCA and CCA objects (i.e. fast moving objects) are evaluated for "behind curve" status.
   * Objects that have been flagged as "on" will never be flagged as "behind"
   *
   * PRECONDITIONS:
   * Call Flag_Objects_On_Longi_Stat_Curves() before call to this function.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Flag_Single_Object_Behind_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T &obj)
   {
      // Clear objects previous info
      obj.behind_sep_id = F360_INVALID_UNSIGNED_ID;
      obj.sep_intersection_point.x = INFTY;
      obj.sep_intersection_point.y = INFTY;
      obj.f_behind_sep_ambiguous = false;

      if ((obj.movable_prob > 0.5F) && (F360_INVALID_UNSIGNED_ID == obj.on_sep_id))
      {
         // Get object rear and front VCS positions
         Point obj_rear_vcs;
         Convert_TCS_Posn_To_VCS_Posn(
            -obj.bbox.Get_Length() * 0.5F,
            0.0F,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            obj_rear_vcs.x,
            obj_rear_vcs.y);

         Point obj_front_vcs;
         Convert_TCS_Posn_To_VCS_Posn(
            obj.bbox.Get_Length() * 0.5F,
            0.0F,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            obj_front_vcs.x,
            obj_front_vcs.y);
         
         const float32_t sensor_avg_longi_pos = globals.average_sensor_position_x;

         // Express object position as linear equation from host center to object position
         float32_t obj_center_line_slope;
         float32_t obj_center_line_offset;
         Get_Vector_As_Linear_Equation(
            sensor_avg_longi_pos,
            0.0F,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj_center_line_slope,
            obj_center_line_offset);

         // Express object rear position as linear equation from host center to object position
         float32_t obj_rear_line_slope;
         float32_t obj_rear_line_offset;
         Get_Vector_As_Linear_Equation(
            sensor_avg_longi_pos,
            0.0F,
            obj_rear_vcs.x,
            obj_rear_vcs.y,
            obj_rear_line_slope,
            obj_rear_line_offset);

         // Express object front position as linear equation from host center to object position
         float32_t obj_front_line_slope;
         float32_t obj_front_line_offset;
         Get_Vector_As_Linear_Equation(
            sensor_avg_longi_pos,
            0.0F,
            obj_front_vcs.x,
            obj_front_vcs.y,
            obj_front_line_slope,
            obj_front_line_offset);

         Point intersection_point_center = {};
         for (uint8_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
         {
            if (F360_STATIC_ENV_POLY_STATUS_INVALID != sep[sep_idx].status)
            {
               // Check if object's center is behind SEP with host center as reference
               const bool f_behind_sep = Check_If_Point_Is_Behind_SEP(
                  sensor_avg_longi_pos,
                  obj.bbox.Get_Center().x,
                  0.0F,
                  obj.bbox.Get_Center().y,
                  obj_center_line_slope,
                  obj_center_line_offset,
                  sep[sep_idx],
                  calibs,
                  intersection_point_center);

               // Check if object's rear end is behind SEP with host center as reference
               Point intersection_point_rear = {};
               const bool f_behind_sep_rear = Check_If_Point_Is_Behind_SEP(
                  sensor_avg_longi_pos,
                  obj_rear_vcs.x,
                  0.0F,
                  obj_rear_vcs.y,
                  obj_rear_line_slope,
                  obj_rear_line_offset,
                  sep[sep_idx],
                  calibs,
                  intersection_point_rear);

               // Check if object's front is behind SEP with host center as reference
               Point intersection_point_front = {};
               const bool f_behind_sep_front = Check_If_Point_Is_Behind_SEP(
                  sensor_avg_longi_pos,
                  obj_front_vcs.x,
                  0.0F,
                  obj_front_vcs.y,
                  obj_front_line_slope,
                  obj_front_line_offset,
                  sep[sep_idx],
                  calibs,
                  intersection_point_front);

               if (f_behind_sep)
               {
                  obj.behind_sep_id = sep_idx + 1U;
                  obj.f_behind_sep_ambiguous = (f_behind_sep_front != f_behind_sep_rear);
                  break;
               }
               else
               {
                  obj.f_behind_sep_ambiguous = (obj.f_behind_sep_ambiguous) || (f_behind_sep_front != f_behind_sep_rear);
               }
            }
         }
         obj.sep_intersection_point.x = intersection_point_center.x;
         obj.sep_intersection_point.y = intersection_point_center.y;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Objects_On_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T &tracker_info,
   *  const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   *  const F360_Calibrations_T &calibs,
   *  F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function flags objects on Static Environment Polynomials.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Flag_Objects_On_SEP(
      const F360_Tracker_Info_T &tracker_info,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;

         Flag_Single_Object_On_SEP(
            sep,
            calibs,
            object_tracks[obj_idx]);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Single_Object_On_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   *  const F360_Calibrations_T &calibs,
   *  F360_Object_Track_T &obj
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
   * This function flags single object on Static Environment Polynomials.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Flag_Single_Object_On_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Object_Track_T &obj)
   {
      // Clear objects previous info
      obj.on_sep_id = F360_INVALID_UNSIGNED_ID;

      for (uint8_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
      {
         if (F360_STATIC_ENV_POLY_STATUS_INVALID != sep[sep_idx].status)
         {
            const bool f_on_sep = Check_If_Point_Is_On_SEP(
               obj.bbox.Get_Center().x,
               obj.bbox.Get_Center().y,
               sep[sep_idx],
               calibs.k_sep_obj_on_poly_thr);

            if (f_on_sep)
            {
               obj.on_sep_id = sep_idx + 1U;
               break;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Detection_Behind_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T &sensor,
   * const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   * const F360_Calibrations_T &calibs,
   * F360_Detection_Props_T &detection_prop
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
   * This function flags detection behind Static Environment Polynomials. The curve array
   * is sorted in prioritized order, thus if the detection is behind several
   * curves it will only be flagged as behind the most prioritized curve.
   * Detection that has been flagged as "on" will never be flagged as "behind"
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Detection_Behind_SEP(
      const F360_Radar_Sensor_T &sensor,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T &detection_prop)
   {
      if ((detection_prop.f_ok_to_use) && (F360_INVALID_UNSIGNED_ID == detection_prop.on_sep_id))
      {

         // Express detection position as linear equation from sensor position to detection position
         float32_t det_line_slope;
         float32_t det_line_offset;
         Get_Vector_As_Linear_Equation(
            sensor.constant.mounting_position.vcs_position.longitudinal,
            sensor.constant.mounting_position.vcs_position.lateral,
            detection_prop.vcs_position.x,
            detection_prop.vcs_position.y,
            det_line_slope,
            det_line_offset);

         for (uint8_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
         {
            if (F360_STATIC_ENV_POLY_STATUS_INVALID != sep[sep_idx].status)
            {
               Point intersection_point = {};
               const bool f_behind_sep = Check_If_Point_Is_Behind_SEP(
                  sensor.constant.mounting_position.vcs_position.longitudinal,
                  detection_prop.vcs_position.x,
                  sensor.constant.mounting_position.vcs_position.lateral,
                  detection_prop.vcs_position.y,
                  det_line_slope,
                  det_line_offset,
                  sep[sep_idx],
                  calibs,
                  intersection_point);

               if (f_behind_sep)
               {
                  detection_prop.behind_sep_id = sep_idx + 1U;
                  break;
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Detection_On_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
   * const F360_Calibrations_T &calibs,
   * F360_Detection_Props_T &detection_prop
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
   * This function flags detection on Static Environment Polynomials.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Detection_On_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T &detection_prop)
   {
      if (detection_prop.f_ok_to_use)
      {
         for (uint8_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
         {
            if (F360_STATIC_ENV_POLY_STATUS_INVALID != sep[sep_idx].status)
            {
               const bool f_on_sep = Check_If_Point_Is_On_SEP(
                  detection_prop.vcs_position.x,
                  detection_prop.vcs_position.y,
                  sep[sep_idx],
                  calibs.k_sep_det_on_poly_thr);

               if (f_on_sep)
               {
                  detection_prop.on_sep_id = sep_idx + 1U;
                  break;
               }
            }
         }
      }
   }

}

