/*===========================================================================*\
* FILE: f360_nearby_wheel_spins.cpp
*============================================================================
* Copyright (C) 2021-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This function contains definitions of functions that are used for 
*   marking wheel spins "nearby" type. 
*   Main function is Detect_Near_By_Wheel_Spins()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/


#include "f360_nearby_wheel_spins.h"
#include "f360_nearby_wheel_spins_support_functions.h"
#include "f360_math_func.h"
#include "f360_bounding_box.h"


namespace f360_variant_A {
   static void Mark_Internal_Wheel_Spins(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   );

   static void Mark_Detections_Above_Cluster(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   );

   static void Mark_Detections_Below_Cluster(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   );

   static void Try_To_Mark_Detection(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      F360_Detection_Props_T &det_to_check
   );

   /*===========================================================================*\
   * FUNCTION: Detect_Nearby_Wheel_Spins()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS: 
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   *  const F360_Calibrations_T &calibrations - tracker calibrations
   *  F360_Detection_Props_T (&det_properties) [MAX_NUMBER_OF_DETECTIONS] - detections properties
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
   * This fucntion detects wheel spins that are neighoburs of already discovered wheel spins.
   *
   * PRECONDITIONS:
   * Detections should be sorted longitudinally.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Detect_Nearby_Wheel_Spins(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const F360_Calibrations_T &calibrations,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      if (raw_detections.number_of_valid_detections > 0U)
      {
         Wheel_Spin_Internals_T wheel_spins_internals;
         uint16_t cluster_counter = 0U;

         while(cluster_counter < calibrations.k_nbws_max_num_clusters)
         {
            const BoundingBox area_of_interest{ Point{calibrations.k_nbws_min_long_pos, calibrations.k_nbws_min_lat_pos},  Point{calibrations.k_nbws_max_long_pos, calibrations.k_nbws_max_lat_pos} };
            const Wheel_Spin_Cluster_T new_cluster = Find_Next_Wheel_Spin_Cluster(calibrations, raw_detections, area_of_interest, det_properties, wheel_spins_internals);

            if (new_cluster.f_valid)
            {
               Mark_Internal_Wheel_Spins(new_cluster, calibrations, raw_detections, det_properties);
               Mark_Detections_Above_Cluster(new_cluster, calibrations, raw_detections, det_properties);
               Mark_Detections_Below_Cluster(new_cluster, calibrations, raw_detections, det_properties);
               cluster_counter++;
            }
            else
            {
               break;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Internal_Wheel_Spins()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const Wheel_Spin_Cluster_T &ws_cluster - wheel spin cluster
   *  const F360_Calibrations_T &calibrations - tracker calibrations
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   *  F360_Detection_Props_T (&det_properties) [MAX_NUMBER_OF_DETECTIONS] - detections properties
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
   * It marks internal detections as wheel spins. Internal detections are those 
   * which lay within wheel spin cluster (set of wheel spins detections)
   *
   * PRECONDITIONS:
   * Detections should be sorted longitudinally.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static void Mark_Internal_Wheel_Spins(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS])
   {
      if (ws_cluster.min_long_det_idx != ws_cluster.max_long_det_idx)
      {
         int32_t det_to_check_idx = raw_detections.detections[ws_cluster.min_long_det_idx].processed.next_sorted_idx;

         while(ws_cluster.max_long_det_idx != det_to_check_idx)
         {
            F360_Detection_Props_T &det_to_check = det_properties[det_to_check_idx];

            Try_To_Mark_Detection(ws_cluster, calibrations, det_to_check);

            det_to_check_idx = raw_detections.detections[det_to_check_idx].processed.next_sorted_idx;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Detections_Above_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const Wheel_Spin_Cluster_T &ws_cluster - wheel spin cluster
   *  const F360_Calibrations_T &calibrations - tracker calibrations
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detections
   *  F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS] - detections properties
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
   * This fucntion marks detections that are longitudinally above cluster and
   * meet conditions
   *
   * PRECONDITIONS:
   * Detections should be sorted longitudinally.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static void Mark_Detections_Above_Cluster(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      const rspp_variant_A::RSPP_Detection_T &boundary_cluster_raw_det = raw_detections.detections[ws_cluster.max_long_det_idx];
      const F360_Detection_Props_T &boundary_cluster_det_prop = det_properties[ws_cluster.max_long_det_idx];
      int32_t det_to_check_idx = boundary_cluster_raw_det.processed.next_sorted_idx;

      while (F360_INVALID_ID != det_to_check_idx)
      {
         F360_Detection_Props_T &det_to_check = det_properties[det_to_check_idx];
         const rspp_variant_A::RSPP_Detection_T &raw_det_to_check = raw_detections.detections[det_to_check_idx];

         if (std::abs(det_to_check.vcs_position.x - boundary_cluster_det_prop.vcs_position.x) < calibrations.k_nbws_long_marking_th)
         {
            Try_To_Mark_Detection(ws_cluster, calibrations, det_to_check);
         }
         else
         {
            break;
         }

         det_to_check_idx = raw_det_to_check.processed.next_sorted_idx;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Detections_Below_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const Wheel_Spin_Cluster_T &ws_cluster - wheel spin cluster
   *  const F360_Calibrations_T &calibrations - tracker calibrations
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   *  F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS] - detections properties
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
   * This fucntion marks detections that are longitudinally below cluster and
   * meet conditions
   *
   * PRECONDITIONS:
   * Detections should be sorted longitudinally.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static void Mark_Detections_Below_Cluster(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      const F360_Detection_Props_T &boundary_cluster_det = det_properties[ws_cluster.min_long_det_idx];
      const rspp_variant_A::RSPP_Detection_T &boundary_cluster_raw_det = raw_detections.detections[ws_cluster.min_long_det_idx];
      int32_t det_to_check_idx = boundary_cluster_raw_det.processed.prev_sorted_idx;

      while (F360_INVALID_ID != det_to_check_idx)
      {
         F360_Detection_Props_T &det_to_check = det_properties[det_to_check_idx];
         const rspp_variant_A::RSPP_Detection_T &raw_det_to_check = raw_detections.detections[det_to_check_idx];

         if (std::abs(det_to_check.vcs_position.x - boundary_cluster_det.vcs_position.x) < calibrations.k_nbws_long_marking_th)
         {
            Try_To_Mark_Detection(ws_cluster, calibrations, det_to_check);
         }
         else
         {
            break;
         }

         det_to_check_idx = raw_det_to_check.processed.prev_sorted_idx;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Try_To_Mark_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const Wheel_Spin_Cluster_T &ws_cluster - wheel spin cluster
   *  const F360_Calibrations_T &calibrations - tracker calibrations
   *  F360_Detection_Props_T &det_to_check - detections that algorithm tries to mark
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
   * This fucntion tries to mark detection if it meets conditions.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static void Try_To_Mark_Detection(
      const Wheel_Spin_Cluster_T &ws_cluster,
      const F360_Calibrations_T &calibrations,
      F360_Detection_Props_T &det_to_check
   )
   {
      const bool f_eligible_for_nearby_marking = (ws_cluster.ws_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT) || (!det_to_check.f_has_close_det_with_similar_rr);
      const bool f_is_within_lat_boundaries = ((ws_cluster.max_lat_pos - det_to_check.vcs_position.y) < calibrations.k_nbws_lat_marking_th) &&
                                               ((det_to_check.vcs_position.y - ws_cluster.min_lat_pos) < calibrations.k_nbws_lat_marking_th);

      if ((det_to_check.wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID) && (f_eligible_for_nearby_marking) && (f_is_within_lat_boundaries))
      {
         det_to_check.wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_NEARBY;
      }
   }  
}
