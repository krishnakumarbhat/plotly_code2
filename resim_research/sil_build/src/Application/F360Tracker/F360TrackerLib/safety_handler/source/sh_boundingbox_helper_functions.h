#ifndef SH_BOUNDINGBOX_HELPER_FUNCTIONS_H
#define SH_BOUNDINGBOX_HELPER_FUNCTIONS_H
/*===========================================================================*/
/**
 * @file sh_boundingbox_helper_functions.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file declares geometric helper functions and data structures for
 *   bounding box operations in F360 radar plausibility checking. It defines
 *   point and bounding box structures, and provides APIs to compute rotated
 *   object bounding boxes, create axis-aligned extended tolerance regions,
 *   and perform point-in-rectangle function for detection validation.
 *
 * @section ABBR ABBREVIATIONS:
 *   - NA
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - NA
 *
 * Add Polarion Work Item Link to the intended line (if using Resource Link
 * for traceability) - NA to this file
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
 *
 * @defgroup template Provide API description and define/delete next line
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "f360_reuse.h"
#include "f360_rot_object_log.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
{
   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   typedef struct SH_Point_Tag
   {
      float32_t x; // a point longitudinal coordinate value
      float32_t y; // a point lateral coordinate value
   } SH_Point_T;

   typedef struct SH_BoundingBox_Tag
   {
      SH_Point_T corner_fl; // boundingbox front left corner point
      SH_Point_T corner_fr; // boundingbox front right corner point
      SH_Point_T corner_rl; // boundingbox rear left corner point
      SH_Point_T corner_rr; // boundingbox rear right corner point
   } SH_BoundingBox_T;

   enum ROT_Object_Reference_Point_T
   {
      ROT_OBJECT_REF_POINT_CENTER = 0,
      ROT_OBJECT_REF_POINT_FRONT_LEFT = 1,
      ROT_OBJECT_REF_POINT_FRONT_MID = 2,
      ROT_OBJECT_REF_POINT_FRONT_RIGHT = 3,
      ROT_OBJECT_REF_POINT_RIGHT_MID = 4,
      ROT_OBJECT_REF_POINT_REAR_RIGHT = 5,
      ROT_OBJECT_REF_POINT_REAR_MID = 6,
      ROT_OBJECT_REF_POINT_REAR_LEFT = 7,
      ROT_OBJECT_REF_POINT_LEFT_MID = 8,
      ROT_OBJECT_REF_POINT_INVALID = 9
   };

   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /******************************************************************************
   * Name:  Is_Point_Inside_Bounding_Box
   *   Check if a point is within a axis aligned bounding box.
   *
   * Shared Variables: none
   *
   * Parameters: det_p, input point to check
                 max_point, axis aligned bounding point with max x and y
                 min_point, axis aligned bounding point with min x and y
   *
   * Return Value:
   *    true - if point is inside the axis aligned bounding box.
   *    false - if point is outside the axis aligned bounding box.
   *
   * Design Information:
   *  - NA
   *
   * Change References:
   *  - NA
   *
   * Add Polarion Work Item Link to the intended line (if using Resource Link
   * for traceability) - refer to module header comment block.
   *
   ******************************************************************************/
   bool Is_Point_Inside_Bounding_Box(const SH_Point_T &det_p, const SH_Point_T &max_point, const SH_Point_T &min_point);

   /******************************************************************************
   * Name:  Get_Axis_Aligned_Rect_MinMax_Pt
   *   Create an axis aligned extended bounding box top right corner and
   *   bottom left corner with input bounding box and offsets.
   *
   * Shared Variables: none
   *
   * Parameters: box_in, the input bounding box
                 offset_x, offset to extend in x direction
                 offset_y, offset to extend in y direction
                 max_point_out, returned axis aligned bounding point with max x and y
                 min_point_out, returned axis aligned bounding point with min x and y
   *
   * Return Value: None
   *
   * Design Information:
   *  - NA
   *
   * Change References:
   *  - NA
   *
   * Add Polarion Work Item Link to the intended line (if using Resource Link
   * for traceability) - refer to module header comment block.
   *
   ******************************************************************************/
   void Get_Axis_Aligned_Rect_MinMax_Pt(const SH_BoundingBox_T &box_in, const float32_t offset_x,
                                        const float32_t offset_y, SH_Point_T &max_point_out, SH_Point_T &min_point_out);

   /******************************************************************************
   * Name:  Get_Object_Bounding_Box
   *    Create a bounding box for the object based on its properties.
   *
   * Shared Variables: none
   *
   * Parameters: object, input object properties
                 box_out, returned bounding box of the object
   *
   * Return Value: Center point of the bounding box
   *
   * Design Information:
   *  - NA
   *
   * Change References:
   *  - NA
   *
   * Add Polarion Work Item Link to the intended line (if using Resource Link
   * for traceability) - refer to module header comment block.
   *
   ******************************************************************************/
   SH_Point_T Get_Object_Bounding_Box(const ROT_Object_Output_T &object, SH_BoundingBox_T &box_out);

} // namespace f360_variant_A
/** @} doxygen end group */
#endif /*F360_BOUNDINGBOX_HELPER_FUNCTIONS_H */

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
