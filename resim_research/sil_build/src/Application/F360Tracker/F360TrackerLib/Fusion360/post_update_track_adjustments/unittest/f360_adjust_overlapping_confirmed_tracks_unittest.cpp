/** \file
   This file contains unit tests for content of f360_adjust_overlapping_confirmed_tracks.cpp
*/

#include "f360_adjust_overlapping_confirmed_tracks.h"

#include <CppUTest/TestHarness.h>
#include <cfloat>


/** \defgroup  f360_adjust_overlapping_confirmed_tracks
 *  @{
 */
using namespace f360_variant_A;

/** \brief
*  Test group of Adjust_Overlapping_Confirmed_Tracks
**/

TEST_GROUP(f360_adjust_overlapping_confirmed_tracks)
{
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};


/**
*\purpose  Purpose of this test is to verify whether 2 tracks that are not qualified as overlaping because of too low confidence level
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_confidence_level_is_too_low_objects_are_not_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Update_Bbox_Center();

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = 5.0F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Center();

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];


   object_tracks[1].confidenceLevel = 0;
   object_tracks[0].confidenceLevel = 0;

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlapping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks are not marked as overlaping if one of them is not movable.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_second_track_is_not_movable_are_not_marked)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   * and set f_movable of one of tracks as false
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 0.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of both tracks should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether overlaping objects that are not movable are not marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_tracks_are_not_movable_objects_are_not_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1;
   object_tracks[0].movable_prob = 0.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = 5.0F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 0.0F;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlapping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether 2 not overlaping tracks are not marked as overlaping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_not_overlaping_tracks_are_not_marked_as_overlapping)
{
   /** \precond
    * Set two tracks parameters to make them not overlaping
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1;
   object_tracks[0].movable_prob = 1.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = 5.0F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether track with lower confidence level is marked as overlaping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_track_with_lower_confidence_level_is_marked_as_overlapping)
{
   /** \precond
   * Set three objects to overlap. Their confidence level should be different
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = 0.35F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });


   object_tracks[2].vcs_position.x = 0.0F;
   object_tracks[2].vcs_position.y = -4.9F;
   object_tracks[2].f_overlapping_with_object = false;
   object_tracks[2].id = 3;
   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].confidenceLevel = 0.25;
   object_tracks[2].movable_prob = 1.0F;
   object_tracks[2].Update_Bbox_Size(0.5F, 0.5F);
   object_tracks[2].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
   tracker_info.vcslong_sorted_next_track[1] = NULL;

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * First and third track should be marked as overlaping with object
   **/
   CHECK_TRUE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[2].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether when vcslong_sorted_next_track points to NULL, overlaping tracks are not marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_vcslong_sorted_next_track_does_not_point_to_valid_track_are_not_marked_as_overlapping)
{
   /** \precond
   * Set three objects to overlap. Their confidence level should be different.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = 0.35F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   object_tracks[2].vcs_position.x = 0.0F;
   object_tracks[2].vcs_position.y = -4.9F;
   object_tracks[2].f_overlapping_with_object = false;
   object_tracks[2].id = 3;
   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].confidenceLevel = 0.25;
   object_tracks[2].movable_prob = 1.0F;
   object_tracks[2].Update_Bbox_Size(0.5F, 0.5F);

   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   // Set vcslong_sorted_next_track pointers to NULL
   tracker_info.vcslong_sorted_next_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[1] = NULL;

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[2].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks with high enough vcs_heading difference are marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, chech_whether_overlaping_tracks_with_high_enough_vcs_heading_difference_are_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   * and to have high enough vcs_heading difference
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.1F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of first track should be true
   * f_overlaping_with_object of second track should be false
   **/
   CHECK_TRUE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks are not marked as overlaping if one of them has too high confidence level
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, chech_whether_when_second_track_has_too_high_confidence_level_are_not_marked)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of both tracks should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks with too high vcs_heading difference are not marked as overlaping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, chech_whether_overlaping_tracks_with_too_big_vcs_heading_diff_are_not_marked)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   * and have big vcs_heading difference.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.05F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].vcs_heading = Angle{ -3.14F };
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of both tracks should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks with high enough speed difference are marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, chech_whether_overlaping_tracks_with_high_enough_speed_difference_are_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   * have high enough speed difference.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.1F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].speed = 20.0F;
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of first track should be true
   **/
   CHECK_TRUE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks that have high enough speed difference but low confidence level
* are not marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, chech_whether_overlaping_tracks_with_high_enough_speed_difference_but_too_low_confidence_are_not_marked)
{
   /** \precond
   * Set two tracks parameters to make them overlaping
   * have different speed
   * have low confidence level
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.05F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].speed = 20.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 0.6F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of both tracks should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks that have similar vcs_heading and speed are marked properly
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_normalized_vcs_heading_diff_is_small_enough_track_is_marked_properly)
{
   /** \precond
   * Set two tracks parameters to make them overlap
   * have similar vcs_heading
   * and speed
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = 1;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].vcs_heading = Angle{ -0.45F };
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ -0.45F });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = false;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 0.8F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of second track should be true
   * f_overlaping_with_object of first track should be false
   **/
   CHECK_TRUE(object_tracks[1].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 tracks have similar vcs_heading but their speed difference is too big are not marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_normalized_vcs_heading_diff_is_small_enough_but_tracks_speed_differs_too_much_track_is_not_marked)
{
   /** \precond
   * Set two tracks parameters to make them overlap
   * have similar vcs_heading
   * and big speed difference
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = 1;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].vcs_heading = Angle{ -0.45F };
   object_tracks[0].speed = 20.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 0.8F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
}

/**
*\purpose  Purpose of this test is to verify whether 2 tracks which overlap each other and match all conditions to be marked as overlapping but 
* confidence level of larger track is smaller than smaller one, tracks are not marked as overlaping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_when_normalized_vcs_heading_diff_is_small_enough_but_larger_track_has_higher_confidence_are_not_marked)
{
   /** \precond
   * Set two tracks parameters to make them overlap
   * Set confidence level of bigger track to be lower than second one
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = 1;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(1.0F, 1.0F);
   object_tracks[0].vcs_heading = Angle{ -0.45F };

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 0.8F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(2.0F, 2.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of second track (idx = 1) should be reseted
   **/
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 tracks with too big longitudinal position difference are not marked as overlapping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_tracks_with_too_big_longitudinal_position_difference_are_not_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to have similar lateral postion but different longitudinal postion.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.1F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };

   object_tracks[1].vcs_position.x = 100.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 tracks that have very large lateral position difference are not marked as overlapping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_tracks_with_very_large_lateral_position_difference_are_not_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to have similar longitudinal position but very different lateral position.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.01F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -50.0F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}





/**
*\purpose  Purpose of this test is to verify whether one of overlaping coasted tracks are marked as overlaping
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, chceck_whether_one_of_overlaping_coasted_tracks_is_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to be overlaping
   * make them coasted
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.1F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].Update_Bbox_Size(4.0F, 4.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Update_Bbox_Size(6.0F, 6.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object of first track should be true
   * f_overlaping_with_object of second track should be false
   **/
   CHECK_TRUE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks are not marked as overlaping when first is invalid
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_two_overlaping_tracks_are_not_marked_as_overlapping_when_first_is_invalid)
{
   /** \precond
   * Set two tracks parameters to be overlaping
   * make one of them as invalid.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_INVALID;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(4.0F, 4.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].confidenceLevel = 1.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(6.0F, 6.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks are not marked as overlaping when second is invalid
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_two_overlaping_tracks_are_not_marked_as_overlapping_when_second_is_invalid)
{
   /** \precond
   * Set two tracks parameters to be overlaping
   * make one of them as invalid.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(4.0F, 4.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(6.0F, 6.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 overlaping tracks are not marked as overlaping when one has low confidence level.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_two_overlaping_tracks_are_not_marked_as_overlapping_when_one_has_zero_confidence_level)
{
   /** \precond
   * Set two tracks parameters to be overlaping
   * set confidence level of one track to zero.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.05F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(4.0F, 4.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 1;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(6.0F, 6.0F);
   object_tracks[1].confidenceLevel = 0.0F;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 tracks that longitudinal position difference is so high that track length would be too big
* are not marked as overlaping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_two_overlaping_tracks_are_not_marked_as_overlapping_when_combined_length_would_be_too_big)
{
   /** \precond
   * Set two tracks parameters to be overlaping
   * make them have high difference in longitudinal position
   * track length sum should be smaller than longituidnal position difference
   **/
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(4.0F, 4.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].confidenceLevel = 1.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(6.0F, 6.0F);
   object_tracks[1].confidenceLevel = 1.0F;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify whether 2 tracks that lateral position difference is so high that track width would be too big
* are not marked as overlaping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_whether_two_overlaping_tracks_are_not_marked_as_overlapping_when_combined_width_would_be_too_big)
{
   /** \precond
   * Set two tracks parameters to be overlaping
   * make them have high difference in lateral position
   * track width sum should be smaller than lateral position difference
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -25.0F;
   object_tracks[0].f_overlapping_with_object = false;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[0].Update_Bbox_Size(4.0F, 4.0F);
   object_tracks[0].vcs_heading = Angle{ -1.0F };
   object_tracks[0].confidenceLevel = 1.0F;

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = -5.1F;
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
   object_tracks[1].Update_Bbox_Size(6.0F, 6.0F);
   object_tracks[1].confidenceLevel = 1.0F;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify that 2 tracks that are very close to each other but not overlapping are not marked as overlapping.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_that_tracks_close_but_not_overlapping_are_not_marked_as_overlapping)
{
   /** \precond
   * Set two tracks parameters to be very close to each other but not overlapping (same longitudinal position but lateral positions so that object bounding boxes are not overlapping).
   * Set other parameters of objects so that objects would have been flagged as overlapping if not for the lateral difference.
   * Set the overlapping flag of objects to true so that it can be checked that these flags have been reset.
   ***/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh + 0.01F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].speed = 10.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[0].vcs_heading = Angle { 0.0F };
   object_tracks[0].Update_Bbox_Size(2.0F, 2.0F);

   object_tracks[1].vcs_position.x = 0.0F;
   object_tracks[1].vcs_position.y = 1.51F; // Just slightly larger than the sum of wid2 of object 1 and wid1 of object 2 so that bounding boxes are just not overlapping
   object_tracks[1].f_overlapping_with_object = true;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].speed = 20.0F; // Larger than speed of object 1 so that there is a large speed disagreement between the objects
   object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[1].vcs_heading = Angle { 0.0F };
   object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
   CHECK_FALSE(object_tracks[1].f_overlapping_with_object);
}


/**
*\purpose  Purpose of this test is to verify that an object should not be marked as overlapping when there is only one object present.
*\req    NA
*/
TEST(f360_adjust_overlapping_confirmed_tracks, check_one_single_object_is_not_marked_as_overlapping)
{
   /** \precond
   * Create one single active object.
   * Make it have properties such that it is considered as "confirmed" by the function (i.e. it would be possible to set the overlapping flag if there were another active object as well)
   * Set its overlapping flag to true so that it can later be checked that this flag has been reset.
   **/
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].f_overlapping_with_object = true;
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].confidenceLevel = 1.0F;

   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = NULL;

   /** \action
   * Call tested function
   **/
   Adjust_Overlapping_Confirmed_Tracks(calib, tracker_info, object_tracks, timing_info);

   /** \result
   * f_overlaping_with_object should be false
   **/
   CHECK_FALSE(object_tracks[0].f_overlapping_with_object);
}

/** @}*/
