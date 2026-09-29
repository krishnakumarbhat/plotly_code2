/*===========================================================================*/
/**
 * @file sh_object_plausible_checks.cpp
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file implements plausibility checking algorithms for F360 radar object
 *   tracking. It validates that object tracks have associated detections within
 *   acceptable geometric bounds by comparing detection positions against extended
 *   bounding boxes derived from object poses. The primary function performs
 *   position plausibility verification to ensure tracked objects correspond to
 *   actual radar measurements within calibrated tolerance regions.
 *
 * @section ABBR ABBREVIATIONS:
 *   - NA
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_Safety_Handler
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *    - [C77];Compiler warning is generated if typedef enum type is used; No risk locally used with enum only.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <cmath>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "sh_object_plausible_checks.h"
#include "sh_boundingbox_helper_functions.h"
#include "sh_safety_handler_internal.h"

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
    * Name:  Is_Target_Params_Within_Scope
    *   Function verifies whether object track is an interested target to be evaluated.
    *
    * Shared Variables: none
    *
    * Parameters: const ROT_Object_Output_T& object
    *
    * Return Value:
    *    true - if object track is within interested scope
    *    false - if outside interested scope
    *
    * Design Information:
    *  - NA
    *
    *  Change References:
    *  - None
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   static bool Is_Target_Params_Within_Scope(const ROT_Object_Output_T &object)
   {
      bool f_obj_inscope = true;
      // Currently only check if object is valid and not coasted. object status 2 is coasted, 255 is invalid
      enum
      {
         OBJECT_STATUS_INVALID = 255U,
         OBJECT_STATUS_MEASURED = 0U,
         OBJECT_STATUS_NEWLY_CREATED = 1U,
         OBJECT_STATUS_COASTED = 2U
      };
      const bool f_measured_object = ((OBJECT_STATUS_COASTED != object.object_status)
                                     && (OBJECT_STATUS_INVALID != object.object_status) && (object.id > 0));
      const bool f_obj_infront_host = object.vcs_x_posn > 0.0F; // target reference point position shall be in front of host
      const bool f_obj_rel_spd_inscope = std::sqrt(object.iso_relative_x_vel * object.iso_relative_x_vel +
         object.iso_relative_y_vel * object.iso_relative_y_vel) <= (70.0F / 3.6F); // max object relative speed to host is 70kph
      f_obj_inscope = f_measured_object && f_obj_infront_host && f_obj_rel_spd_inscope;

      return f_obj_inscope;
   }

   bool Object_Plausible_Checks(const rspp_variant_A::RSPP_Detection_T (&processed_det_ref)[MAX_NUMBER_OF_DETECTIONS],
                                const F360_Detection_Log_T (&associated_detections)[MAX_NUMBER_OF_DETECTIONS],
                                const uint32_t number_of_valid_det,
                                const ROT_Object_Output_T (&radar_object_data)[NUMBER_OF_REDUCED_OBJECT_TRACKS])
   {
      bool f_plausible = true;
      if (number_of_valid_det > MAX_NUMBER_OF_DETECTIONS)
      {
         // Exceeded max number of detections set plausibility check failed
         f_plausible = false;
      }
      else
      {
         for (uint32_t obj_i = 0U; obj_i < NUMBER_OF_REDUCED_OBJECT_TRACKS; obj_i++)
         {
            const ROT_Object_Output_T &object = radar_object_data[obj_i];
            f_plausible = Object_Position_Plausible_Check(processed_det_ref, associated_detections,
                                                          number_of_valid_det, object);
            if (!f_plausible)
            {
               // if any object fails the plausibility check, set overall plausibility to false and break
               break;
            }
         }
      }

      return f_plausible;
   }

   bool Object_Position_Plausible_Check(const rspp_variant_A::RSPP_Detection_T (&processed_det_ref)[MAX_NUMBER_OF_DETECTIONS],
                                        const F360_Detection_Log_T (&associated_detections)[MAX_NUMBER_OF_DETECTIONS],
                                        const uint32_t number_of_valid_det,
                                        const ROT_Object_Output_T &object)
   {
      bool f_position_plausible = true;
      if (object.reference_point >= ROT_OBJECT_REF_POINT_INVALID)
      {
         // Invalid reference point value or not defined in enum, set plausibility check failed
         f_position_plausible = false;
      }
      else if (Is_Target_Params_Within_Scope(object)) // Check if the track is within allowed boundaries
      {
         f_position_plausible = false;
         // Object is valid target to check position plausibility, now create the bounding box for plausibility check
         typedef struct Ext_Bounding_Box_Calibrations_Tag
         {
            float32_t base_offset;            // minimum offset in both long and lat direction
            float32_t longitudinal_pos_offset; // extra offset in occluded direction (it will be added for both front and back)
            float32_t radial_range_coefficient;      // radial increase in range
            float32_t radial_range_based_saturation;    // radial max increase based on range
            float32_t longitudinal_pos_close_to_host_offset;   // separate handling for "dead zone" objects
            float32_t cross_radial_range_based_coefficient;    // cross radial increase based on range
            float32_t cross_radial_range_based_saturation;  // cross radial max increase based on range
         } Ext_Bounding_Box_Calibrations_T;
          constexpr Ext_Bounding_Box_Calibrations_T kConfig =
             {
               /* base_offset */ 0.4F,
               /* longitudinal_pos_offset */ 2.1F,
               /* radial_range_coefficient */ 0.1F,
               /* radial_range_based_saturation */ 1.0F,
               /* longitudinal_pos_close_to_host_offset */ 4.0F,
               /* cross_radial_range_based_coefficient */ 0.0555F,
               /* cross_radial_range_based_saturation */ 3.33F };
         const float32_t k_max_xpos_to_be_considered_close_to_host = 4.0F;
         SH_BoundingBox_T object_bounding_box;
         const SH_Point_T box_center = Get_Object_Bounding_Box(object, object_bounding_box);
         const float32_t host_to_target_range = std::sqrt(box_center.x * box_center.x +
         box_center.y * box_center.y);

         const float32_t range_based_increase = kConfig.base_offset + std::fminf(
            kConfig.radial_range_coefficient * host_to_target_range,kConfig.radial_range_based_saturation)
            + std::fminf(kConfig.cross_radial_range_based_coefficient * host_to_target_range,
            kConfig.cross_radial_range_based_saturation);

         const float32_t long_pos_gate_tcs = box_center.x < k_max_xpos_to_be_considered_close_to_host ?
         std::fmaxf(kConfig.longitudinal_pos_close_to_host_offset, range_based_increase + kConfig.longitudinal_pos_offset) :
         range_based_increase + kConfig.longitudinal_pos_offset;
         const float32_t lat_pos_gate_tcs = range_based_increase;
         const float32_t long_pos_gate = long_pos_gate_tcs * std::abs(std::cos(object.vcs_pointing)) +
         lat_pos_gate_tcs * std::abs(std::sin(object.vcs_pointing));
         const float32_t lat_pos_gate = long_pos_gate_tcs * std::abs(std::sin(object.vcs_pointing)) +
         lat_pos_gate_tcs * std::abs(std::cos(object.vcs_pointing));

         SH_Point_T matching_max_point;
         SH_Point_T matching_min_point;
         Get_Axis_Aligned_Rect_MinMax_Pt(object_bounding_box, long_pos_gate, lat_pos_gate,
                                         matching_max_point, matching_min_point);
         const uint32_t number_of_assoc_det = object.ndets;
         uint32_t num_det_checked = 0U;
         bool f_continue_checking = true;
         for (uint32_t idx = 0U; (f_continue_checking && (idx < number_of_valid_det)); idx++)
         {
            if (associated_detections[idx].objTrkID == static_cast<uint16_t>(object.id))
            {
               // Found an associated detection
               num_det_checked++;

               if (num_det_checked > number_of_assoc_det)
               {
                  // All associated detections checked
                  f_continue_checking = false;
               }
               else
               {
                  // Check if detection point is inside the extended bounding box
                  const SH_Point_T det_pt{processed_det_ref[idx].processed.vcs_position_x,
                                          processed_det_ref[idx].processed.vcs_position_y};
                  if (Is_Point_Inside_Bounding_Box(det_pt, matching_max_point, matching_min_point))
                  {
                     // Found at least one associated detection inside the bounding box
                     f_position_plausible = true;
                     f_continue_checking = false;
                  }
               }
            }
         }
      }
      else
      {
         // Do nothing,misra compliance
      }
      return f_position_plausible;
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
