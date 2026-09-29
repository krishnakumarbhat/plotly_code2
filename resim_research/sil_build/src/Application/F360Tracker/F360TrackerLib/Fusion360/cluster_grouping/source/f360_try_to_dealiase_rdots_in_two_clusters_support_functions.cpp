/*===================================================================================*\
* FILE: f360_try_to_dealiase_rdots_in_two_clusters_support_functions.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains function definitions of
*     Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased()
*     Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased()
*     Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased()
*     Find_All_Rdot_Widths()
*     Calc_Rep_Rdot_Comp()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_math_func.h"
#include "f360_try_to_dealiase_rdots_in_two_clusters_support_functions.h"
#include "f360_is_two_look_type_ok_combine.h"
#include "f360_get_unique_rdot_interval_ids.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   /*===========================================================================*\
     * FUNCTION: Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
     * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
     * const F360_Detection_Hist_T &detections_hist,
     * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
     * const F360_Tracker_Info_T& tracker_info,
     * const F360_Globals_T& globals,
     * const F360_Calibrations_T &calib,
     * const F360_Cluster_T &cluster_older,
     * const F360_Cluster_T &cluster_newer,
     * float32_t& rngrate_interval_width_older,
     * float32_t& rngrate_interval_width_newer,
     * float32_t& interval_older,
     * float32_t& interval_newer,
     * bool& f_success)
     *
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Dealiasing clusters range rates with advantage of having none cluster dealiased
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T &detections_hist,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Globals_T& globals,
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &cluster_older,
      const F360_Cluster_T &cluster_newer,
      float32_t& rngrate_interval_width_older,
      float32_t& rngrate_interval_width_newer,
      float32_t& interval_older,
      float32_t& interval_newer,
      bool& f_success)
   {
      int32_t ndets_older = 0;
      int32_t ndets_newer = 0;
      float32_t det_rdot_width_older[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t det_rdot_width_newer[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      int32_t num_unique_rdot_intervals_older = 0;
      int32_t num_unique_rdot_intervals_newer = 0;
      int32_t rdot_interval_ids_older[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      int32_t rdot_interval_ids_newer[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      int32_t unique_rdot_interval_ids_older[MAX_NUM_UNIQUE_RDOT_INTERVAL];
      int32_t unique_rdot_interval_ids_newer[MAX_NUM_UNIQUE_RDOT_INTERVAL];
      
      Find_All_Rdot_Widths(cluster_older, raw_detection_list, sensors, detections_hist, ndets_older, det_rdot_width_older);
      Find_All_Rdot_Widths(cluster_newer, raw_detection_list, sensors, detections_hist, ndets_newer, det_rdot_width_newer);

      Get_Unique_Rdot_Interval_Ids(tracker_info.unique_rdot_interval_widths, tracker_info.num_unique_rdot_interval_widths, ndets_older, det_rdot_width_older, rdot_interval_ids_older, unique_rdot_interval_ids_older, num_unique_rdot_intervals_older);
      Get_Unique_Rdot_Interval_Ids(tracker_info.unique_rdot_interval_widths, tracker_info.num_unique_rdot_interval_widths, ndets_newer, det_rdot_width_newer, rdot_interval_ids_newer, unique_rdot_interval_ids_newer, num_unique_rdot_intervals_newer);

      for (int32_t index1 = 0; index1 < num_unique_rdot_intervals_older ; index1++)
      {
         const int32_t unique_rdot_interval_ids_older_idx = unique_rdot_interval_ids_older[index1] - 1;
         for (int32_t index2 = 0; index2 < num_unique_rdot_intervals_newer; index2++)
         {
            const int32_t unique_rdot_interval_ids_newer_idx = unique_rdot_interval_ids_newer[index2] - 1;
            if (tracker_info.rdot_interval_compatibility[unique_rdot_interval_ids_older_idx][unique_rdot_interval_ids_newer_idx])
            {
               rngrate_interval_width_older = tracker_info.unique_rdot_interval_widths[unique_rdot_interval_ids_older_idx];
               rngrate_interval_width_newer = tracker_info.unique_rdot_interval_widths[unique_rdot_interval_ids_newer_idx];

               const float32_t rep_rdot_comp1 = Calc_Rep_Rdot_Comp(cluster_older, det_props, detections_hist, unique_rdot_interval_ids_older, index1, rdot_interval_ids_older);
               const float32_t rep_rdot_comp2 = Calc_Rep_Rdot_Comp(cluster_newer, det_props, detections_hist, unique_rdot_interval_ids_newer, index2, rdot_interval_ids_newer);

               Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased(globals, calib, rep_rdot_comp1, rep_rdot_comp2, rngrate_interval_width_older,
                  rngrate_interval_width_newer, f_success, interval_older, interval_newer);

               if (f_success)
               {
                  break;
               }
            }
         }
         if (f_success)
         {
            break;
         }
      }
   }

   /*===========================================================================*\
     * FUNCTION: Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
     * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
     * const F360_Detection_Hist_T &detections_hist,
     * const F360_Tracker_Info_T& tracker_info,
     * const F360_Globals_T& globals,
     * const F360_Calibrations_T &calib,
     * const F360_Cluster_T &dealiased_cluster,
     * const F360_Cluster_T &cluster2,
     * float32_t& rngrate_interval_width,
     * float32_t& rngrate_interval,
     * bool& f_success)
     * 
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Dealiasing clusters range rates with advantage of having one cluster dealiased
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T &detections_hist,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Globals_T& globals,
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &dealiased_cluster,
      const F360_Cluster_T &cluster2,
      float32_t& rngrate_interval_width,
      float32_t& rngrate_interval,
      bool& f_success)
   {
      int32_t ndets_interval = 0;
      int32_t num_unique_rdot_intervals = 0;
      float32_t det_rdot_width[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      int32_t rdot_interval_ids[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      int32_t unique_rdot_interval_ids[MAX_NUM_UNIQUE_RDOT_INTERVAL];

      Find_All_Rdot_Widths(cluster2, raw_detection_list, sensors, detections_hist, ndets_interval, det_rdot_width);

      Get_Unique_Rdot_Interval_Ids(tracker_info.unique_rdot_interval_widths, tracker_info.num_unique_rdot_interval_widths, ndets_interval, det_rdot_width, rdot_interval_ids, unique_rdot_interval_ids, num_unique_rdot_intervals);

      for (int32_t index = 0; index < num_unique_rdot_intervals; index++)
      {
         rngrate_interval_width = tracker_info.unique_rdot_interval_widths[unique_rdot_interval_ids[index] - 1];

         const float32_t rep_rdot_comp = Calc_Rep_Rdot_Comp(cluster2, det_props, detections_hist, unique_rdot_interval_ids, index, rdot_interval_ids);

         Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased(globals, calib, dealiased_cluster.rep_rdotcomp, rep_rdot_comp, rngrate_interval_width,
             f_success, rngrate_interval);

         if (f_success)
         {
            break;
         }
      }
   }

   /*===========================================================================*\
     * FUNCTION: Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const F360_Calibrations_T &calib,
     * const F360_Cluster_T &cluster_older,
     * const F360_Cluster_T &cluster_newer,
     * bool& f_success
     * 
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Dealiasing clusters range rates with advantage of having both clusters dealiased
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased(
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &cluster_older,
      const F360_Cluster_T &cluster_newer,
      bool& f_success)
   {
      const float32_t rdot_diff = cluster_older.rep_rdotcomp - cluster_newer.rep_rdotcomp;
      Is_Two_Look_Type_Ok_Combine_For_Zero_Intervals(calib, rdot_diff, f_success);
   }

   /*===========================================================================*\
     * FUNCTION: Find_All_Rdot_Widths
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const F360_Cluster_T &cluster,
     * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
     * const F360_Detection_Hist_T &detections_hist,
     * int32_t &ndets_older,
     * float32_t(&rdot_width)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]
     *
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Fiding all range rates widths within cluster's detections
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Find_All_Rdot_Widths(
      const F360_Cluster_T &cluster,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T &detections_hist,
      int32_t &ndets_older,
      float32_t(&rdot_width)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]
   )
   {
      for (int32_t index = 0; index < cluster.ndets; index++)
      {
         const int16_t det_idx = cluster.detids[index] - 1;
         const int32_t sens_idx = raw_detection_list.detections[det_idx].raw.sensor_id - 1;
         const F360_Det_Look_ID_T look_id = sensors[sens_idx].variable.look_id;
         const float range_rate_interval_width = sensors[sens_idx].constant.v_wrapping[look_id];

         rdot_width[ndets_older] = range_rate_interval_width;
         ndets_older++;
      }
      for (int32_t index = 0; index < cluster.num_old_dets; index++)
      {
         rdot_width[ndets_older] = detections_hist.det_data[cluster.old_det_idx[index]].v_wrapping;
         ndets_older++;
      }
   }

   /*===========================================================================*\
     * FUNCTION: Calc_Rep_Rdot_Comp
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const F360_Cluster_T &cluster,
     * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
     * const F360_Detection_Hist_T &detections_hist,
     * const int32_t(&unique_rdot_interval_ids)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
     * const int32_t &unique_rdot_interval_index,
     * const int32_t (&rdot_interval_ids)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
     *
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Calculating mean range range from detections from cluster within one range rate interval width
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   float32_t Calc_Rep_Rdot_Comp(
      const F360_Cluster_T &cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T &detections_hist,
      const int32_t(&unique_rdot_interval_ids)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
      const int32_t &unique_rdot_interval_index,
      const int32_t (&rdot_interval_ids)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]
   )
   {
      float32_t rep_rdot_comp = 0.0F;
      float32_t n_used_dets = 0.0F;
      for (int32_t index = 0; index < cluster.ndets; index++)
      {
         if (rdot_interval_ids[index] == unique_rdot_interval_ids[unique_rdot_interval_index])
         {
            rep_rdot_comp += det_props[cluster.detids[index] - 1].range_rate_compensated;
            n_used_dets += 1.0F;
         }
      }

      for (int32_t index = 0; index < cluster.num_old_dets; index++)
      {
         if (rdot_interval_ids[cluster.ndets + index] == unique_rdot_interval_ids[unique_rdot_interval_index])
         {
            rep_rdot_comp += detections_hist.det_data[cluster.old_det_idx[index]].rdot_comp;
            n_used_dets += 1.0F;
         }
      }

      if (n_used_dets > 0.0F)
      {
         rep_rdot_comp /= n_used_dets;
      }
      return rep_rdot_comp;
   }
}
