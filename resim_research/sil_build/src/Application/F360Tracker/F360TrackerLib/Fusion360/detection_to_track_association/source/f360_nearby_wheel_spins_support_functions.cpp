/*===========================================================================*\
* FILE: f360_nearby_wheel_spins_support_functions.cpp
*============================================================================
* Copyright (C) 2021-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains definitions of supporting functions for 
*   f360_nearby_wheel_spins.cpp
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/


#include "f360_nearby_wheel_spins_support_functions.h"
#include "f360_math_func.h"
#include "f360_check_if_point_is_inside_box.h"
#include <algorithm>


namespace f360_variant_A 
{
   static void Find_Cluster_Starting_Point(
      const F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const Wheel_Spin_Internals_T &ws_internal,
      const BoundingBox &area_of_interest,
      Wheel_Spin_Cluster_T &cluster
   );

   static void Find_Detections_For_New_Cluster(
      const F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      const BoundingBox &area_of_interest,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      Wheel_Spin_Internals_T &ws_internal,
      Wheel_Spin_Cluster_T &cluster
   );

   static bool Try_To_Extend_Cluster(
      const F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      const int32_t suspect_det_idx,
      const F360_Calibrations_T &calibrations,
      Wheel_Spin_Internals_T &ws_internal,
      Wheel_Spin_Cluster_T &cluster
   );

   /*===========================================================================*\
   * FUNCTION: Is_Det_Vaild_For_WS_Clustering()
   *===========================================================================
   * RETURN VALUE:
   * bool - true if det is valid
   *
   * PARAMETERS:
   *   const F360_Detection_Props_T & det_prop - detection properties
   *   const BoundingBox &area_of_interest - area of interest within algorithm checks detections
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
   * Check if detections are valid for clustering.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   bool Is_Det_Vaild_For_WS_Clustering(
      const F360_Detection_Props_T &det_prop,
      const BoundingBox &area_of_interest 
   )
   {
      return ((det_prop.wheel_spin_type != F360_DETECTION_WHEELSPIN_TYPE_INVALID) && area_of_interest.Contains(det_prop.vcs_position));
   }

   /*===========================================================================*\
   * FUNCTION: Find_Next_Wheel_Spin_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * Wheel_Spin_Cluster_T - wheel spin cluster 
   *
   * PARAMETERS:
   *   const F360_Calibrations_T & calibrations - tracker calibrations
   *   const rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   *   const BoundingBox & - area of interest within algorithm checks detections
   *   F360_Detection_Props_T (& det_properties) [MAX_NUMBER_OF_DETECTIONS] - detections properties 
   *   Wheel_Spin_Internals_T & ws_internal - algorithm internal data
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
   * First it determines starting point. Then it goes though detections (via 
   * double-link list) and tries to create cluster on them. 
   * More details can be found in functions descriptions.
   *
   * PRECONDITIONS:
   * Detections should be sorted longitudinally.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   Wheel_Spin_Cluster_T Find_Next_Wheel_Spin_Cluster(
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const BoundingBox & area_of_interest,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      Wheel_Spin_Internals_T &ws_internal
   )
   {
      Wheel_Spin_Cluster_T new_cluster;

      if (ws_internal.f_first_call || (F360_INVALID_ID != ws_internal.next_starting_det_idx))
      {
         ws_internal.f_first_call = false;

         Find_Cluster_Starting_Point(det_properties, raw_detections, ws_internal, area_of_interest, new_cluster);

         if (F360_INVALID_ID != new_cluster.min_long_det_idx)
         {
            Find_Detections_For_New_Cluster(det_properties, area_of_interest, calibrations, raw_detections, ws_internal, new_cluster);

            new_cluster.max_long_det_idx = ws_internal.prev_ws_det_idx;
            new_cluster.f_valid = true;
         }
      }

      return new_cluster;
   }

   /*===========================================================================*\
   * FUNCTION: Find_Cluster_Starting_Point()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Detection_Props_T (& det_properties) [MAX_NUMBER_OF_DETECTIONS]
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   *  const Wheel_Spin_Internals_T & ws_internal - algorithm internal data
   *  const BoundingBox &area_of_interest - area of interest within algorithm checks detections
   *  Wheel_Spin_Cluster_T & cluster - output cluster
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
   * Find a cluster starting point by looking for first detection that meets conditions. 
   * Detection from this algoithm starts depends on last determined point that from which
   * it should start searching.
   * From this point cluster is created.
   *
   * PRECONDITIONS:
   * Detections should be sorted longitudinally.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static void Find_Cluster_Starting_Point(
      const F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const Wheel_Spin_Internals_T &ws_internal,
      const BoundingBox &area_of_interest,
      Wheel_Spin_Cluster_T &cluster
   )
   {
      int32_t ws_det_idx = (F360_INVALID_ID == ws_internal.next_starting_det_idx) ? raw_detections.vcslong_det_idx_min : ws_internal.next_starting_det_idx;

      while(F360_INVALID_ID != ws_det_idx)
      {
         const F360_Detection_Props_T &det_prop = det_properties[ws_det_idx];
         const rspp_variant_A::RSPP_Detection_T &raw_det = raw_detections.detections[ws_det_idx];

         if (Is_Det_Vaild_For_WS_Clustering(det_prop, area_of_interest))
         {
            cluster.min_long_det_idx = ws_det_idx;
            cluster.min_lat_pos = det_prop.vcs_position.y;
            cluster.max_lat_pos = det_prop.vcs_position.y;
            cluster.ws_type = det_prop.wheel_spin_type;
            break;
         }

         ws_det_idx = raw_det.processed.next_sorted_idx;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Cluster_Starting_Point()
   *===========================================================================
   * RETURN VALUE:
   * bool - returns information if there is still any det that has to be checked
   *
   * PARAMETERS:
   *  const F360_Detection_Props_T (& det_properties) [MAX_NUMBER_OF_DETECTIONS]
   *  const int32_t suspect_det_idx,
   *  const F360_Calibrations_T & calibrations - tracker calibrations
   *  const Wheel_Spin_Internals_T & ws_internal - algorithm internal data
   *  Wheel_Spin_Cluster_T & cluster - output cluster
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
   * Check if suspected detection is close enough to the last known wheel spin
   * detection. If yes then extend cluster parameters.
   *
   * PRECONDITIONS:
   * Suspect detection should be longitudinally higher than previous known
   * wheel spin detection
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static bool Try_To_Extend_Cluster(
      const F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      const int32_t suspect_det_idx,
      const F360_Calibrations_T &calibrations,
      Wheel_Spin_Internals_T &ws_internal,
      Wheel_Spin_Cluster_T &cluster)
   {
      const F360_Detection_Props_T &suspect_det_prop = det_properties[suspect_det_idx];
      const F360_Detection_Props_T &prev_ws_det_prop = det_properties[ws_internal.prev_ws_det_idx];
      bool f_still_det_to_check = true;

      if (suspect_det_prop.vcs_position.x - prev_ws_det_prop.vcs_position.x < calibrations.k_nbws_long_asc_th)
      {
         if ((std::abs(suspect_det_prop.vcs_position.y - prev_ws_det_prop.vcs_position.y) < calibrations.k_nbws_lat_asc_th))
         {
            ws_internal.prev_ws_det_idx = suspect_det_idx;

            cluster.min_lat_pos = std::min(cluster.min_lat_pos, suspect_det_prop.vcs_position.y);
            cluster.max_lat_pos = std::max(cluster.max_lat_pos, suspect_det_prop.vcs_position.y);

            // Only one detection in cluster needs to be marked as wheel spin type object to mark whole cluster as object type.
            if (suspect_det_prop.wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT)
            {
               cluster.ws_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
            }
         }
         else
         {
            if (F360_INVALID_ID == ws_internal.next_starting_det_idx)
            {
               ws_internal.next_starting_det_idx = suspect_det_idx;
            }
         }
      }
      else
      {
         if (F360_INVALID_ID == ws_internal.next_starting_det_idx)
         {
            ws_internal.next_starting_det_idx = suspect_det_idx;
         }

         f_still_det_to_check = false;
      }

      return f_still_det_to_check;
   }

   /*===========================================================================*\
   * FUNCTION: Find_Detections_For_New_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Detection_Props_T (& det_properties) [MAX_NUMBER_OF_DETECTIONS]
   *   const BoundingBox &area_of_interest - area of interest within algorithm checks detections
   *   const F360_Calibrations_T & calibrations - tracker calibrations
   *   const rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   *   Wheel_Spin_Internals_T & ws_internal - algorithm internal data
   *   Wheel_Spin_Cluster_T & cluster - output cluster
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
   * Cluster detections from the staring point. It moves longitudinally and checks
   * if suspected detection is within limits (with respect to the last known wheel
   * spin detection) and is marked as wheel spin. If yes then this detection
   * becomes "the last known" and we checks next detection on the list. 
   * If detection is longitudinally close but laterally far away then this detection
   * can become the starting point of new cluster.
   *
   * PRECONDITIONS:
   * 1. Detections should be sorted longitudinally.
   * 2. F360_INVALID_ID != cluster.min_long_det_idx
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   static void Find_Detections_For_New_Cluster(
      const F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      const BoundingBox &area_of_interest,
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      Wheel_Spin_Internals_T &ws_internal,
      Wheel_Spin_Cluster_T &cluster
   )
   {
      int32_t det_idx = raw_detections.detections[cluster.min_long_det_idx].processed.next_sorted_idx;
      bool f_still_dets_to_check = (F360_INVALID_ID != det_idx);

      ws_internal.prev_ws_det_idx = cluster.min_long_det_idx;
      ws_internal.next_starting_det_idx = F360_INVALID_ID;
 
      while(f_still_dets_to_check && (F360_INVALID_ID != det_idx))
      {
         const F360_Detection_Props_T &det_prop = det_properties[det_idx];
         const rspp_variant_A::RSPP_Detection_T &raw_det = raw_detections.detections[det_idx];

         if (Is_Det_Vaild_For_WS_Clustering(det_prop, area_of_interest))
         {
            f_still_dets_to_check = Try_To_Extend_Cluster(det_properties, det_idx, calibrations, ws_internal, cluster);
         }
         else
         {
            if (det_prop.vcs_position.x > calibrations.k_nbws_max_long_pos)
            {
               if (F360_INVALID_ID == ws_internal.next_starting_det_idx)
               {
                  ws_internal.next_starting_det_idx = det_idx;
               }

               f_still_dets_to_check = false;
            }
         }

         det_idx = raw_det.processed.next_sorted_idx;
      }
   }
}
