/*===================================================================================*\
* FILE:  f360_detection_association_evaluation.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the main function for detection to track association where a 
* hypothesis is formed and assigned concerning possible object association.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_detection_association_evaluation.h"
#include <algorithm>
#include "f360_iterator.h"
#include "f360_detection_association_support_functions.h"
#include "f360_dets_inside_bbox.h"
#include "f360_determine_association_hypothesis.h"
#include "f360_define_dead_zone.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Detection_Association_Evaluation()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Tracker_Info_T & tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
   * const F360_Calibrations_T & calibrations
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function evaluates what detection to track associations that are most 
   * probable in four steps. 
   * Firstly, all associations from previous scan are cleared (ndets, 
   * num_types_of_dets and detids). 
   * Secondly, the tracks' bounding box extents are updated. 
   * Thirdly, detections which are close enough to the track are marked as inside
   * (based on the dimensions of the bounding box).
   * Lastly, association is executed (by calculating a score based on the dynamic
   * match between detection and track).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Detection_Association_Evaluation(
      const F360_Host_T & host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T & tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      // Initialize arrays for detection scores and compensated range rates
      float32_t det_score_array[MAX_NUMBER_OF_DETECTIONS] = {};
      std::fill(cmn::begin(det_score_array), cmn::end(det_score_array), INFTY);

      float32_t det_rdot_comp_array[MAX_NUMBER_OF_DETECTIONS] = {};

      const Dead_Zone_T dead_zone = Define_Dead_Zone(sensors, calibrations.k_dead_zone_long_limit_extension);

      for (uint32_t i = 0U; i < static_cast<uint32_t>(tracker_info.num_active_objs); i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         if (obj_idx >= 0) 
         {
            object_tracks[obj_idx].ndets = 0U;

            std::fill(cmn::begin(object_tracks[obj_idx].num_types_of_dets), cmn::end(object_tracks[obj_idx].num_types_of_dets), 0);
            std::fill(cmn::begin(object_tracks[obj_idx].detids), cmn::end(object_tracks[obj_idx].detids), 0U);

            uint32_t det_idx_in_extbox[MAX_NUMBER_OF_DETECTIONS] = {};
            uint32_t num_dets_in_extbox = 0U;
            if (object_tracks[obj_idx].movable_prob > 0.5F)
            {
               // If object is moveable, check if detection is inside a gate (rectangular box)
               Calculate_Ext_Bbox_And_Find_Dets_Inside(
                  calibrations,
                  host.dist_rear_axle_to_vcs_m, 
                  host.speed,
                  raw_detection_list,
                  dead_zone,
                  detection_props, 
                  object_tracks[obj_idx],
                  det_idx_in_extbox, 
                  num_dets_in_extbox);
            }
            else
            {
               // If object is NOT moveable, do simple check based on radial distance to determine if detection is inside a gate (circular area)
               // Note: moveable is a condition for CTCA. This means that only non-moveable CCA objects can enter this else statement
               Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside(
                  calibrations,
                  raw_detection_list,
                  host.dist_rear_axle_to_vcs_m,
                  object_tracks[obj_idx],
                  detection_props,
                  det_idx_in_extbox, 
                  num_dets_in_extbox);
            }
            object_tracks[obj_idx].num_dets_in_ext_bbox = static_cast<uint16_t>(num_dets_in_extbox);

            Determine_Association_Hypothesis(
               calibrations,
               sensors,
               host.vcs_speed,
               det_idx_in_extbox,
               num_dets_in_extbox,
               obj_idx,
               raw_detection_list.detections,
               static_env_polys,
               detection_props,
               object_tracks,
               det_score_array,
               det_rdot_comp_array);
         }
      }

      // Assign current hypotesis
      Assign_Association_Hypothesis(
         raw_detection_list.number_of_valid_detections,
         det_rdot_comp_array,
         tracker_info,
         detection_props, 
         object_tracks);
   }
}
