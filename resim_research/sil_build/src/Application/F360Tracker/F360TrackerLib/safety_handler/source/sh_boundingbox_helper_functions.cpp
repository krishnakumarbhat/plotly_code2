/*===========================================================================*/
/**
 * @file sh_boundingbox_helper_functions.cpp
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file implements geometric helper functions for bounding box calculations
 *   used in F360 radar object plausibility checking. It provides utilities to
 *   compute axis-aligned rectangular bounds from rotated object bounding boxes,
 *   determine min/max coordinates, and perform point-in-rectangle tests. These
 *   functions support position plausibility validation by creating extended
 *   tolerance regions around tracked objects for detection association verification.
 *
 * @section ABBR ABBREVIATIONS:
 *   - NA
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - NA
 *
 *   - Requirements Document(s):
 *     - NA
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <cmath>
#include "f360_reuse.h"

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "sh_boundingbox_helper_functions.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
{

   /*===========================================================================*
    * Local Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Static Variable Definitions
    *===========================================================================*/

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/

   /******************************************************************************
    * Name:  Max_X_In_Box
    *   Find the maximum X coordinate in the bounding box.
    *
    * Shared Variables: none
    *
    * Parameters: box - input bounding box
    *
    * Return Value:
    *    max_x - Maximum longitudinal coordinate in the bounding box.
    *
    * Design Information:
    *  - NA - helpter functions
    *
    *  Change References:
    *  - None
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   static inline float32_t Max_X_In_Box(const SH_BoundingBox_T &box)
   {
      return fmaxf(fmaxf(box.corner_fl.x, box.corner_fr.x),
                   fmaxf(box.corner_rl.x, box.corner_rr.x));
   }

   /******************************************************************************
    * Name:  Min_X_In_Box
    *   Find the minimum X coordinate in the bounding box.
    *
    * Shared Variables: none
    *
    * Parameters: box - input bounding box
    *
    * Return Value:
    *    min_x - Minimum longitudinal coordinate in the bounding box.
    *
    * Design Information:
    *  - NA - helper functions
    *
    *  Change References:
    *  - None
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   static inline float32_t Min_X_In_Box(const SH_BoundingBox_T &box)
   {
      return fminf(fminf(box.corner_fl.x, box.corner_fr.x),
                   fminf(box.corner_rl.x, box.corner_rr.x));
   }

   /******************************************************************************
    * Name:  Max_Y_In_Box
    *   Find the maximum Y coordinate in the bounding box.
    *
    * Shared Variables: none
    *
    * Parameters: box - input bounding box
    *
    * Return Value:
    *    max_y - Maximum lateral coordinate in the bounding box.
    *
    * Design Information:
    *  - NA - helper functions
    *
    *  Change References:
    *  - None
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   static inline float32_t Max_Y_In_Box(const SH_BoundingBox_T &box)
   {
      return fmaxf(fmaxf(box.corner_fl.y, box.corner_fr.y),
                   fmaxf(box.corner_rl.y, box.corner_rr.y));
   }

   /******************************************************************************
    * Name:  Min_Y_In_Box
    *   Find the minimum Y coordinate in the bounding box.
    *
    * Shared Variables: none
    *
    * Parameters: box - input bounding box
    *
    * Return Value:
    *    min_y - Minimum lateral coordinate in the bounding box.
    *
    * Design Information:
    *  - NA - helper functions
    *
    *  Change References:
    *  - None
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   static inline float32_t Min_Y_In_Box(const SH_BoundingBox_T &box)
   {
      return fminf(fminf(box.corner_fl.y, box.corner_fr.y),
                   fminf(box.corner_rl.y, box.corner_rr.y));
   }

   bool Is_Point_Inside_Bounding_Box(const SH_Point_T &det_p, const SH_Point_T &max_point,
                                     const SH_Point_T &min_point)
   {
      // Initialize return value
      bool pt_inside = false;

      // Point is inside if both min max conditions are met
      if ((det_p.x >= min_point.x) && (det_p.x <= max_point.x) &&
          (det_p.y >= min_point.y) && (det_p.y <= max_point.y))
      {
         pt_inside = true;
      }
      return pt_inside;
   }

   void Get_Axis_Aligned_Rect_MinMax_Pt(const SH_BoundingBox_T &box_in, const float32_t offset_x,
                                        const float32_t offset_y, SH_Point_T &max_point_out,
                                        SH_Point_T &min_point_out)
   {
      const float32_t min_x = Min_X_In_Box(box_in);
      const float32_t max_x = Max_X_In_Box(box_in);
      const float32_t min_y = Min_Y_In_Box(box_in);
      const float32_t max_y = Max_Y_In_Box(box_in);

      max_point_out.x = max_x + offset_x;
      max_point_out.y = max_y + offset_y;
      min_point_out.x = min_x - offset_x;
      min_point_out.y = min_y - offset_y;
   }

   SH_Point_T Get_Object_Bounding_Box(const ROT_Object_Output_T &object, SH_BoundingBox_T &box_out)
   {
      // Calculate the bounding box center poisition base on object reference point
      const uint8_t reference_pt = object.reference_point;
      SH_Point_T box_center;
      const float32_t point_angle = object.vcs_pointing;
      const float32_t half_length = object.length * 0.5F;
      const float32_t half_width = object.width * 0.5F;
      const float32_t cos_angle = cosf(point_angle);
      const float32_t sin_angle = sinf(point_angle);

      if (reference_pt == ROT_OBJECT_REF_POINT_CENTER)
      {
         box_center.x = object.vcs_x_posn;
         box_center.y = object.vcs_y_posn;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_FRONT_LEFT)
      {
         box_center.x = object.vcs_x_posn - cos_angle * half_length - sin_angle * half_width;
         box_center.y = object.vcs_y_posn - sin_angle * half_length + cos_angle * half_width;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_FRONT_MID)
      {
         box_center.x = object.vcs_x_posn - cos_angle * half_length;
         box_center.y = object.vcs_y_posn - sin_angle * half_length;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_FRONT_RIGHT)
      {
         box_center.x = object.vcs_x_posn - cos_angle * half_length + sin_angle * half_width;
         box_center.y = object.vcs_y_posn - sin_angle * half_length - cos_angle * half_width;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_RIGHT_MID)
      {
         box_center.x = object.vcs_x_posn + sin_angle * half_width;
         box_center.y = object.vcs_y_posn - cos_angle * half_width;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_REAR_RIGHT)
      {
         box_center.x = object.vcs_x_posn + cos_angle * half_length + sin_angle * half_width;
         box_center.y = object.vcs_y_posn + sin_angle * half_length - cos_angle * half_width;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_REAR_MID)
      {
         box_center.x = object.vcs_x_posn + cos_angle * half_length;
         box_center.y = object.vcs_y_posn + sin_angle * half_length;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_REAR_LEFT)
      {
         box_center.x = object.vcs_x_posn + cos_angle * half_length - sin_angle * half_width;
         box_center.y = object.vcs_y_posn + sin_angle * half_length + cos_angle * half_width;
      }
      else if (reference_pt == ROT_OBJECT_REF_POINT_LEFT_MID)
      {
         box_center.x = object.vcs_x_posn - sin_angle * half_width;
         box_center.y = object.vcs_y_posn + cos_angle * half_width;
      }
      else
      {
         // Default to center if unknown reference point
         box_center.x = object.vcs_x_posn;
         box_center.y = object.vcs_y_posn;
      }

      // Define corners relative to object center
      box_out.corner_fl.x = box_center.x + cos_angle * half_length + sin_angle * half_width; // front left corner
      box_out.corner_fl.y = box_center.y + sin_angle * half_length - cos_angle * half_width; // front left corner

      box_out.corner_fr.x = box_center.x + cos_angle * half_length - sin_angle * half_width; // front right corner
      box_out.corner_fr.y = box_center.y + sin_angle * half_length + cos_angle * half_width; // front right corner

      box_out.corner_rl.x = box_center.x - cos_angle * half_length + sin_angle * half_width; // rear left corner
      box_out.corner_rl.y = box_center.y - sin_angle * half_length - cos_angle * half_width; // rear left corner

      box_out.corner_rr.x = box_center.x - cos_angle * half_length - sin_angle * half_width; // rear right corner
      box_out.corner_rr.y = box_center.y - sin_angle * half_length + cos_angle * half_width; // rear right corner

      return box_center;
   }

} // namespace f360_variant_A

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  fjzzjn      Wenbo Xu
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 File history can be traced by URL:
 "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"

\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
