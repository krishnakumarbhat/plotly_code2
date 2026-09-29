/** \file
 * This file contains unit tests for content of f360_prioritize_clusters.cpp file
 */

#include "f360_prioritize_clusters.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_prioritize_clusters
 *  @{
 */

/** \brief
 * Add brief description of test group, i.e. describe what functionality is tested.
 * When using multiple test groups, make sure to write a brief description for each test group.
 * The description should be unique and describe the specific scenario that is tested in that group.
*/
TEST_GROUP(f360_prioritize_clusters)
{
   // Declare common variables used within all tests in this test group.
   F360_Calibrations_T calibrations{};
   F360_Host_T host{};
   F360_Tracker_Info_T tracker_info{};
   F360_Detection_Hist_T det_hist{};
   rspp_variant_A::RSPP_Detection_List_T raw_detections{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]{};
   int32_t prioritized_cluster_ids[NUMBER_OF_CLUSTERS]{};
   uint32_t num_clusters{};
   F360_Object_Track_T lowest_prio_dummy{};

   /** \setup
    * Initialize tracker calibration
    * set tracker variant
    * set the number of active clusters to 7
    * set maximum number of tracks to 10
    * set host properties - curvature (0.01F) and vcs_speed (3.0F)
    * set the priority of the lowest prio track currently in the tracker to 0.76F
    * prepare historical detections
    * prepare current detections
    * create clusters containing detections mentioned aboce
   */
   TEST_SETUP()
   {
      (void)memset(tracker_info.active_cluster_ids, 0, sizeof(tracker_info.active_cluster_ids));
      (void)memset(tracker_info.active_obj_ids, 0, sizeof(tracker_info.active_obj_ids));
      (void)memset(clusters, 0, sizeof(clusters));
      (void)memset(det_props, 0, sizeof(det_props));
      (void)memset(&det_hist, 0, sizeof(det_hist));
      (void)memset(&lowest_prio_dummy, 0, sizeof(lowest_prio_dummy));

      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);

      //number of active clusters set to 7
      tracker_info.num_active_clusters = 7;

      //maximum number of tracks forced to 10
      tracker_info.variant.num_tracks = 10;

      // Preparing host properties
      host.curvature_rear = 0.01F;
      host.vcs_speed = 3.0F;

      // object with priority 0.75F set to be the lowest priority in the tracker
      lowest_prio_dummy.priority = 0.76F;
      tracker_info.p_lowest_priority_track = &lowest_prio_dummy;

      // Preparing some historical detections
      for(int32_t i = 0; i< 12; i++)
      {
         det_hist.det_data[i].f_potential_angle_jump=false;
         raw_detections.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;
      }

      //current dets
      raw_detections.number_of_valid_detections = 13;
      for(int32_t i = 0; i< 13; i++){
         det_props[i].f_potential_angle_jump = false;
         if(i == 2 || i == 6)
         {
            det_props[i].f_angle_amb = true;
         }
         else
         {
            det_props[i].f_angle_amb = false;
         }
         raw_detections.detections[i].raw.det_id = static_cast<uint16_t>(i + 1);
         raw_detections.detections[i].raw.sensor_id = 1;
         raw_detections.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;
      }

      det_hist.n_occupied = 14;
      for (int32_t i = 0; i < det_hist.n_occupied; i++)
      {
         det_hist.f_idx_occupied[i] = true;
      }

      // Preparing some clusters
      tracker_info.active_cluster_ids[0] = 2;
      tracker_info.active_cluster_ids[1] = 4;
      tracker_info.active_cluster_ids[2] = 6;
      tracker_info.active_cluster_ids[3] = 7;
      tracker_info.active_cluster_ids[4] = 9;
      tracker_info.active_cluster_ids[5] = 12;
      tracker_info.active_cluster_ids[6] = 14;

      clusters[1].f_dealiased = true;
      clusters[1].vcs_position_x = 10.0F;
      clusters[1].vcs_position_y = 10.0F;
      clusters[1].id = 2;
      clusters[1].ndets = 0;
      clusters[1].num_old_dets = 2;
      clusters[1].old_det_idx[0] = 0;
      clusters[1].old_det_idx[1] = 1;

      clusters[3].f_dealiased = true;
      clusters[3].vcs_position_x = 11.0F;
      clusters[3].vcs_position_y = 11.0F;
      clusters[3].id = 4;
      clusters[3].ndets = 2;
      clusters[3].detids[0] = 1;
      clusters[3].detids[1] = 2;
      clusters[3].num_old_dets = 0;

      clusters[5].f_dealiased = true;
      clusters[5].vcs_position_x = 21.0F;
      clusters[5].vcs_position_y = 11.0F;
      clusters[5].id = 6;
      clusters[5].ndets = 2;
      clusters[5].detids[0] = 4;
      clusters[5].detids[1] = 5;
      clusters[5].num_old_dets = 2;
      clusters[5].old_det_idx[0] = 3;
      clusters[5].old_det_idx[1] = 4;

      clusters[6].f_dealiased = true;
      clusters[6].vcs_position_x = 11.1F;
      clusters[6].vcs_position_y = 11.0F;
      clusters[6].id = 7;
      clusters[6].ndets = 2;
      clusters[6].detids[0] = 6;
      clusters[6].detids[1] = 7;
      clusters[6].num_old_dets = 4;
      clusters[6].old_det_idx[0] = 5;
      clusters[6].old_det_idx[1] = 6;
      clusters[6].old_det_idx[2] = 7;
      clusters[6].old_det_idx[3] = 8;

      clusters[8].f_dealiased = true;
      clusters[8].vcs_position_x = 12.1F;
      clusters[8].vcs_position_y = 7.0F;
      clusters[8].id = 9;
      clusters[8].ndets = 1;
      clusters[8].detids[0] = 8;
      clusters[8].num_old_dets = 1;
      clusters[8].old_det_idx[0] = 9;

      clusters[11].f_dealiased = false;
      clusters[11].vcs_position_x = 12.1F;
      clusters[11].vcs_position_y = 7.0F;
      clusters[11].id = 12;
      clusters[11].ndets = 1;
      clusters[11].detids[0] = 9;
      clusters[11].num_old_dets = 1;
      clusters[11].old_det_idx[0] = 10;

      clusters[13].f_dealiased = true;
      clusters[13].vcs_position_x = 2.1F;
      clusters[13].vcs_position_y = 9.0F;
      clusters[13].id = 14;
      clusters[13].ndets = 3;
      clusters[13].detids[0] = 10;
      clusters[13].detids[1] = 11;
      clusters[13].detids[2] = 12;
      clusters[13].num_old_dets = 3;
      clusters[13].old_det_idx[0] = 11;
      clusters[13].old_det_idx[1] = 12;
      clusters[13].old_det_idx[2] = 13;
   }

   //checks if an array starting from index is empty
   bool array_empty_from_index(const uint32_t index, const int32_t (&array)[NUMBER_OF_CLUSTERS]){
      bool is_empty=true;
      for(uint32_t i = index; i< NUMBER_OF_CLUSTERS; i++){
         if(array[i] != 0)
         {
            is_empty = false;
         }
      }
      return is_empty;
   }

};

/** \purpose
 * Tests whether clusters are prioritized correctly and puts them into the prioritized_cluster_ids (they fit into the available slots for initialization)

 * \req
 *NA
 */
TEST(f360_prioritize_clusters, Prioritize_clusters_check)
{
   /** \precond
    * Number of objects available to initialize must be lower than the amount of clusters valid to initialize.
   */
   tracker_info.num_active_objs = 6;

   /** \action
    * Call Prioritize_Clusters
   */
   Prioritize_Clusters(calibrations, host, tracker_info, det_hist, raw_detections, det_props, clusters, prioritized_cluster_ids, num_clusters);

   /** \result
    * Only two clusters in the prioritized_cluster_ids (third id == 0) in order 6, 14.
   */
   CHECK_TRUE(prioritized_cluster_ids[0] == 6 && prioritized_cluster_ids[1] == 14 && prioritized_cluster_ids[2] == 0)
}


/** \purpose
 * Tests whether clusters are prioritized correctly and puts them into the prioritized_cluster_ids (they dont fit into the available slots for initialization-
 * there is only one free slot)
 *
 * \req
 * NA
 */
TEST(f360_prioritize_clusters, Prioritize_clusters_and_truncate_to_fit_max_active_obj)
{
   /** \precond
    * Number of objects available to initialize must be lower than the clusters valid to initialize.
   */

   tracker_info.num_active_objs = 9;

   /** \action
    * Call Prioritize_Clusters
   */
   Prioritize_Clusters(calibrations, host, tracker_info, det_hist, raw_detections, det_props, clusters, prioritized_cluster_ids, num_clusters);

   /** \result
    * prioritized_cluster_ids has to contain 1 nonzero element (14)
   */
   CHECK_TRUE(prioritized_cluster_ids[0] == 14 && array_empty_from_index(1, prioritized_cluster_ids))
}

/** \purpose
 * Check if one cluster gets further passed to initialization when there is only one available slot in the tracker
 * and neither of the active clusters priority is lower than the lowest track priority currently existing in the tracker.
 *
 * \req
 * NA
 */
TEST(f360_prioritize_clusters, Fewer_Clusters_To_Initialize_Than_Available_Slots_And_Lower_Prio_Than_Lowest_Track)
{
   /** \precond
    * Fewer clusters for initialization than available slots in the tracker and neither of them has higher priority than
    * the lowest priority object currently in the tracker
    * lowest priority of the object currently in the tracker set to 0.9F
    * three clusters currently active in the tracker: 4, 6, 14
    * number of active clusters set to 3
    * number of active objects set to 9 (only 1 available)
   */

  F360_Object_Track_T lowest_prio_obj_trk{};
  lowest_prio_obj_trk.priority = 0.9F;
  tracker_info.p_lowest_priority_track = &lowest_prio_obj_trk;

   tracker_info.active_cluster_ids[0] = 4;
   tracker_info.active_cluster_ids[1] = 6;
   tracker_info.active_cluster_ids[2] = 14;

   tracker_info.num_active_clusters = 3;
   tracker_info.num_active_objs = 9;

   /** \action
    * Call Prioritize_Clusters
   */
   Prioritize_Clusters(calibrations, host, tracker_info, det_hist, raw_detections, det_props, clusters, prioritized_cluster_ids, num_clusters);

   /** \result
    * prioritized_cluster_ids contain only one cluster with id 14
   */
   CHECK_TRUE(prioritized_cluster_ids[0] == 14 && array_empty_from_index(1, prioritized_cluster_ids))
}
/** @}*/

