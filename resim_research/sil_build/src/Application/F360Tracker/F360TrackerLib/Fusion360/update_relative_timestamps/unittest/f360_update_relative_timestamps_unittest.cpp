/** \file
   This file contains unit tests for function Update_Relative_Timestamps
*/

#include "f360_update_relative_timestamps.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>


/** \defgroup  sensor_preprocessing
 *  @{
 */
using namespace f360_variant_A;

   /** \brief
   *  provides data/variables needed as input for function Update_Relative_Timestamps
   **/

   TEST_GROUP(update_relative_timestamps)
   {
      float32_t elapsed_time_s=0.05;
      F360_Tracker_Info_T tracker_info = {};
      F360_Object_Track_T obj_trks[NUMBER_OF_OBJECT_TRACKS] = {};
      F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
      F360_Detection_Hist_T det_hist = {};
      float32_t tolerance = 1e-3;
      TEST_SETUP()
      {
         Set_Tracker_Variant(tracker_info.variant);
      }
   };

   /**
   *\purpose  check if relative timestamp since updated is updated correctly for objects
   *\req    put in requirement tag if any otherwise set to NA
   */
   TEST(update_relative_timestamps, Check_Relative_Timestamps_Object)
   {
      /** \precond
       * create a test object and set time since last update to a known value
       **/
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      F360_Object_Track_T *p_test_obj_trk = &obj_trks[0];

      float32_t known_time = 3.8;
      p_test_obj_trk->time_since_cluster_created = known_time;
      p_test_obj_trk->time_since_track_updated = known_time;
      p_test_obj_trk->time_since_stage_start = known_time;
      p_test_obj_trk->time_since_initialization = known_time;

       /** \action
       * call FUT
       **/
      Update_Relative_Timestamps(elapsed_time_s, tracker_info, obj_trks, clusters, det_hist);

      /** \result
      * time since last update shall be known time + elapsed time
      **/
      DOUBLES_EQUAL(p_test_obj_trk->time_since_cluster_created, known_time+elapsed_time_s, tolerance);
      DOUBLES_EQUAL(p_test_obj_trk->time_since_track_updated, known_time + elapsed_time_s, tolerance);
      DOUBLES_EQUAL(p_test_obj_trk->time_since_stage_start, known_time + elapsed_time_s, tolerance);
      DOUBLES_EQUAL(p_test_obj_trk->time_since_initialization, known_time + elapsed_time_s, tolerance);
   }

   /**
   *\purpose  check if relative timestamp since updated is updated correctly for objects with negative time
   *\req    put in requirement tag if any otherwise set to NA
   */
   TEST(update_relative_timestamps, Check_Relative_Timestamps_Object_Negative)
   {
      /** \precond
      * create a test object and set time since last update to a known negative value
      **/
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      F360_Object_Track_T *p_test_obj_trk = &obj_trks[0];

      float32_t known_time = -3.8;
      p_test_obj_trk->time_since_cluster_created = known_time;
      p_test_obj_trk->time_since_track_updated = known_time;
      p_test_obj_trk->time_since_stage_start = known_time;
      p_test_obj_trk->time_since_initialization = known_time;

      /** \action
      * call FUT
      **/
      Update_Relative_Timestamps(elapsed_time_s, tracker_info, obj_trks, clusters, det_hist);

      /** \result
      * time since last update shall be stay with known time before
      **/
      CHECK_EQUAL(p_test_obj_trk->time_since_cluster_created, known_time);
      CHECK_EQUAL(p_test_obj_trk->time_since_track_updated, known_time);
      CHECK_EQUAL(p_test_obj_trk->time_since_stage_start, known_time);
      CHECK_EQUAL(p_test_obj_trk->time_since_initialization, known_time);
   }

   /**
   *\purpose  check if relative timestamp since updated is updated correctly for clusters
   *\req    put in requirement tag if any otherwise set to NA
   */
   TEST(update_relative_timestamps, Check_Relative_Timestamps_Cluster)
   {
      /** \precond
      * create a test cluster and set time since last update to a known value
      **/
      tracker_info.num_active_clusters = 1;
      tracker_info.active_cluster_ids[0] = 1;
      F360_Cluster_T * p_test_cluster = &clusters[0];

      float32_t known_time = 3.8;
      p_test_cluster->time_since_cluster_updated = known_time;
      p_test_cluster->time_since_measurement = known_time;

      /** \action
      * call FUT
      **/
      Update_Relative_Timestamps(elapsed_time_s, tracker_info, obj_trks, clusters, det_hist);

      /** \result
      * time since last update shall be known time + elapsed time
      **/
      DOUBLES_EQUAL(p_test_cluster->time_since_cluster_updated, known_time + elapsed_time_s, tolerance);
      DOUBLES_EQUAL(p_test_cluster->time_since_measurement, known_time + elapsed_time_s, tolerance);
   }

   /**
   *\purpose  check if relative timestamp since updated is updated correctly for clusters with negative timestamp
   *\req    put in requirement tag if any otherwise set to NA
   */
   TEST(update_relative_timestamps, Check_Relative_Timestamps_Cluster_Negative)
   {
      /** \precond
      * create a test cluster and set time since last update to a known negative value
      **/
      tracker_info.num_active_clusters = 1;
      tracker_info.active_cluster_ids[0] = 1;
      F360_Cluster_T * p_test_cluster = &clusters[0];

      float32_t known_time = -3.8;
      p_test_cluster->time_since_cluster_updated = known_time;
      p_test_cluster->time_since_measurement = known_time;

      /** \action
      * call FUT
      **/
      Update_Relative_Timestamps(elapsed_time_s, tracker_info, obj_trks, clusters, det_hist);

      /** \result
      * time since last update shall be stay with known time before
      **/
      CHECK_EQUAL(p_test_cluster->time_since_cluster_updated, known_time);
      CHECK_EQUAL(p_test_cluster->time_since_measurement, known_time);
   }


   /**
   *\purpose  check if relative timestamp since measured is updated correctly for det hist
   *\req    put in requirement tag if any otherwise set to NA
   */
   TEST(update_relative_timestamps, Check_Relative_Timestamps_Det_Hist)
   {
      /** \precond
      * fill some det hist entries and set time since last measured to a known value
      **/
      det_hist.f_idx_occupied[0] = true;
      det_hist.f_idx_occupied[1] = false;

      float32_t known_time = 3.8;
      det_hist.det_data[0].time_since_meas = known_time;
      det_hist.det_data[1].time_since_meas = known_time;

      /** \action
      * call FUT
      **/
      Update_Relative_Timestamps(elapsed_time_s, tracker_info, obj_trks, clusters, det_hist);

      /** \result
      * time since measured shall be known time + elapsed time for occupied indexes
      **/
      DOUBLES_EQUAL(det_hist.det_data[0].time_since_meas, known_time + elapsed_time_s, tolerance);
      /** \result
      * no update for unoccupied indexes
      **/
      CHECK_EQUAL(det_hist.det_data[1].time_since_meas, known_time);
   }

   /**
   *\purpose  check if relative timestamp since measured is updated correctly for det hist with negative timestamp
   *\req    put in requirement tag if any otherwise set to NA
   */
   TEST(update_relative_timestamps, Check_Relative_Timestamps_Det_Hist_Negative)
   {
      /** \precond
      * fill some det hist entries and set time since last measured to a known negative value
      **/
      det_hist.f_idx_occupied[0] = true;
      det_hist.f_idx_occupied[1] = false;

      float32_t known_time = -3.8;
      det_hist.det_data[0].time_since_meas = known_time;
      det_hist.det_data[1].time_since_meas = known_time;

      /** \action
      * call FUT
      **/
      Update_Relative_Timestamps(elapsed_time_s, tracker_info, obj_trks, clusters, det_hist);

      /** \result
      * time since last update shall be stay with known time before
      **/
      CHECK_EQUAL(det_hist.det_data[0].time_since_meas, known_time );
      CHECK_EQUAL(det_hist.det_data[1].time_since_meas, known_time);
   }

/** @}*/
