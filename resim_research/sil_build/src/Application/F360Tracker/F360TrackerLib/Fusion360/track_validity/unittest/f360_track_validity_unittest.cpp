/** \file
* This file contains unit tests for content of f360_track_validity.cpp
*/

#include "f360_track_validity.h"
#include "f360_detect_multipath.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include "f360_sorted_tracks_mgmt.h"
#include "f360_occlusion.h"

using namespace f360_variant_A;

/** \defgroup  f360_track_validity
 *  @{
 */

/** \brief
*  Test group of f360_track_validity module
**/
TEST_GROUP(f360_track_validity)
{
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   rspp_variant_A::RSPP_Detection_List_T detection_list = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Trailer_Estimator_Output_T trailer_data = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS]{};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      host.f_trailer_presence_hardware = false;
      trailer_data.trailer_length[0] = 0.0F;
      trailer_data.trailer_length[1] = 0.0F;

      // Reset sorted vcslong list
      tracker_info.vcslong_sorted_start = NULL;
      for(uint32_t i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = NULL;
         tracker_info.vcslong_sorted_prev_track[i] = NULL;
      }
   }
};

/**
*\purpose  Purpose of this test is to verify whether invalid objects dont have their confidence parameters changed
*\req    NA
*/
TEST(f360_track_validity, Track_Validity__check_whether_invalid_objects_do_not_have_their_confidence_parameters_changed)
{
   /** \precond
   * Set selected params of one selected object
   * Chcek whether they are not changed
   **/
   tracker_info.num_active_objs = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_INVALID;
   object_tracks[0].vcs_heading = Angle{ 1.0F };
   object_tracks[0].id = 1;
   F360_Object_Track_T copy_of_selected_track;
   copy_of_selected_track = object_tracks[0];
   tracker_info.active_obj_ids[0] = 1;

   Sorted_Tracks_Insert(tracker_info, &(object_tracks[0]));

   /** \action
   * Call tested function
   **/
   Track_Validity(host, tracker_info, calibrations, detection_list, sensors, static_env_polys, occlusion_data, det_props, trailer_data, object_tracks, timing_info);

   /** \result
   * Check whether input and output track has same parameters
   **/
   CHECK(object_tracks[0].confidenceLevel == copy_of_selected_track.confidenceLevel);
   CHECK(object_tracks[0].raw_confidence_level == copy_of_selected_track.raw_confidence_level);
}

/** @}*/
