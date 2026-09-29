/** \file
 * This file contains unit tests for content of f360_cancel_new_updated_trk_overlapping_confirmed_trks.cpp file
 */

#include "f360_cancel_new_updated_trk_overlapping_confirmed_trks.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_cancel_new_updated_trk_overlapping_confirmed_trks
 *  @{
 */

/** \brief
 * test group for testing Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks function
 */
TEST_GROUP(f360_cancel_new_updated_trk_overlapping_confirmed_trks)
{	
   F360_Calibrations_T calib;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);
      
      // Initialize active objects parameters
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      
      object_tracks[0].id = 1;
      object_tracks[0].f_moving = true;
      object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;

      object_tracks[1].id = 2;
      object_tracks[1].f_moving = true;
      object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   }
};

/** \purpose  
 *  Check whether number of active object has not been changed when one of two objects is not moving.
 * \req  NA.
 */
TEST(f360_cancel_new_updated_trk_overlapping_confirmed_trks, Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks_No_Moving_Object)
{
   /** \precond
    * Set up needed variables.
    */
   object_tracks[0].f_moving = false;
   
   int32_t exp_num_of_active_obj = tracker_info.num_active_objs;

   /** \action
    * call Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks().
    */
   Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(calib, object_tracks, tracker_info, det_props, timing_info);
   
   /** \result
    *  Check if number of active objects have not been changed.
    */	
   CHECK_EQUAL(exp_num_of_active_obj, tracker_info.num_active_objs);
}

/** \purpose
 *  Check whether number of active object has not been changed when one of two objects has invalid status.
 * \req  NA.
 */
TEST(f360_cancel_new_updated_trk_overlapping_confirmed_trks, Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks_Invalid_Status)
{
   /** \precond
    * Set up needed variables.
    */
   object_tracks[0].status = F360_OBJECT_STATUS_INVALID;
   
   int32_t exp_num_of_active_obj = tracker_info.num_active_objs;

   /** \action
    * call Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks().
    */
   Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(calib, object_tracks, tracker_info, det_props, timing_info);

   /** \result
    *  Check if number of active objects have not been changed.
    */
   CHECK_EQUAL(exp_num_of_active_obj, tracker_info.num_active_objs);

}

/** \purpose
 *  Check whether number of active object has not been changed when second active objects has confidence below threshold.
 * \req  NA.
 */
TEST(f360_cancel_new_updated_trk_overlapping_confirmed_trks, Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks_Object_Do_Not_Overlap)
{
   /** \precond
    * Set up needed variables.
    */
   object_tracks[1].confidenceLevel = calib.k_puta_min_object_confidence - 0.1F;

   int32_t exp_num_of_active_obj = tracker_info.num_active_objs;

   /** \action
    * call Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks().
    */
   Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(calib, object_tracks, tracker_info, det_props, timing_info);

   /** \result
    *  Check if number of active objects have not been changed.
    */
   CHECK_EQUAL(exp_num_of_active_obj, tracker_info.num_active_objs);
}

/** @}*/
