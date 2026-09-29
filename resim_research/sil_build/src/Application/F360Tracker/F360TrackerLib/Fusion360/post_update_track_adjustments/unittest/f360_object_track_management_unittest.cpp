/** \file
 This file contains unit tests that verifies the content of f360_object_track_management.cpp
 */

#include "f360_object_track_management.h"
#include "f360_object_track_equal_operator.h"
#include "f360_clear_detections_props.h"
#include "f360_clear_object_track.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_object_track_management
 *  @{
 **/

/** \brief
 *  Includes tests that will test the behavior of functions implemented in f360_object_track_management.cpp.
 **/
TEST_GROUP(f360_object_track_management)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Track_T object_track[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   F360_TRKR_TIMING_INFO_T timing_info;
   float32_t time_since_stage_start_bef;
   float32_t time_since_track_updated_bef;
   F360_Calibrations_T calibs;

   /** \setup
    * Setting up common variables for test group f360_object_track_management.
    **/
   TEST_SETUP()
   {
      calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C = 50.0F;
      calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C = 30.0F;

      Clear_Detections_Props(det_props);
      for (uint32_t index = 0U; index < NUMBER_OF_OBJECT_TRACKS; index++)
      {
         Clear_Object_Track(object_track[index]);
      }
      tracker_info = { };
      timing_info = { };

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;

      object_track[0].detids[0] = 1;
      object_track[0].ndets = 1;
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {
   }

};

/**
 *\purpose  This test will test Obj_Trk_Status_Book_Keeping() when the status is Obj_Trk_Status_Book_Keeping_COASTED.
 *\req    NA
 */
TEST(f360_object_track_management, Obj_Trk_Status_Book_Keeping_COASTED)
{
   /** \step{1}
    * Check that a COASTED object is set to UPDATED when enough approved detections have been associated,
    * i.e. when there are enough num_rr_inlier_dets (>0 and > #wheelspin detections).
    * This variant utilizes f_rr_inlier = true and wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID.
    **/

   /** \precond
    * Create initialized input. Set parameters so we will be able to enter wanted cases.
    **/
   object_track[0].status = F360_OBJECT_STATUS_COASTED;
   object_track[0].time_since_initialization = 1.00F;
   object_track[0].time_since_stage_start = 0.20F;
   object_track[0].time_since_track_updated = 0.25F;
   object_track[0].num_rr_inlier_dets = 1;
   object_track[0].f_ghost_NU_2_C = true;

   det_props[0].f_rr_inlier = true;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   /** \action
    * Call Obj_Trk_Status_Book_Keeping() which affects time_since_stage_start, time_since_track_updated, status and f_ghost_NU_2_C.
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * The status is expected to be set as UPDATED, time_since_track_updated = 0, time_since_stage_start = 0 and f_ghost_NU_2_C to be false.
    **/
   CHECK_EQUAL(object_track[0].status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track[0].time_since_track_updated, 0.0F, F360_EPSILON);
   DOUBLES_EQUAL(object_track[0].time_since_stage_start, 0.0F, F360_EPSILON);
   CHECK(!object_track[0].f_ghost_NU_2_C);

   /** \step{2}
    * Check that a COASTED object stays COASTED when there are no satisfying detections associated.
    **/

   /** \precond
    * Create initialized input. Set parameters so we will be able to enter wanted cases.
    **/
   object_track[0].status = F360_OBJECT_STATUS_COASTED;
   object_track[0].time_since_initialization = 1.00F;
   object_track[0].time_since_stage_start = 0.20F;
   time_since_stage_start_bef = object_track[0].time_since_stage_start;
   object_track[0].time_since_track_updated = 0.25F;
   time_since_track_updated_bef = object_track[0].time_since_track_updated;

   object_track[0].num_rr_inlier_dets = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;

   /** \action
    * Call Obj_Trk_Status_Book_Keeping() which affects time_since_stage_start, time_since_track_updated, status and f_ghost_NU_2_C.
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * The status is expected to stay as COASTED, time_since_track_updated and time_since_stage_start to be constant and f_ghost_NU_2_C to be false.
    **/
   CHECK_EQUAL(object_track[0].status, F360_OBJECT_STATUS_COASTED);
   DOUBLES_EQUAL(object_track[0].time_since_track_updated, time_since_track_updated_bef, F360_EPSILON);
   DOUBLES_EQUAL(object_track[0].time_since_stage_start, time_since_stage_start_bef, F360_EPSILON);
   CHECK(!object_track[0].f_ghost_NU_2_C);
}


/**
 *\purpose  This test will test Obj_Trk_Status_Book_Keeping() when the status is F360_OBJECT_STATUS_INVALID.
 *\req    NA
 */
TEST(f360_object_track_management, Obj_Trk_Status_Book_Keeping__INVALID_object_makes_no_change)
{
    /** \precond
     **/
   object_track[0].status = F360_OBJECT_STATUS_INVALID;
   F360_Object_Track_T expected = object_track[0];

   /** \action
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * Not changed object
    **/
   CHECK_TRUE(object_track[0] == expected);
}

/**
 *\purpose  This test will test Obj_Trk_Status_Book_Keeping() when the status time since init is zero.
 *\req    NA
 */
TEST(f360_object_track_management, Obj_Trk_Status_Book_Keeping__zero_time_since_init_makes_no_change)
{
    /** \precond
     **/
   object_track[0].status = F360_OBJECT_STATUS_UPDATED;
   object_track[0].time_since_initialization = 0.0F;
   F360_Object_Track_T expected = object_track[0];
   
   /** \action
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * Not changed object
    **/
   CHECK_TRUE(object_track[0] == expected);
}

/** @}*/
