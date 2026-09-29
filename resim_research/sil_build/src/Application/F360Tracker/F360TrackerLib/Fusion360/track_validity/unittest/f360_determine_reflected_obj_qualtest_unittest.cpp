/** \file
   This file contains qualification tests for content of f360_determine_reflected_obj.cpp
*/

#include "f360_determine_reflected_obj.h"
#include "f360_static_env_polys_support_functions.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_determine_reflected_obj
 *  @{
 */

 /** \brief
 *  Test group of f360_determine_reflected_obj
 **/
TEST_GROUP(f360_determine_reflected_obj)
{
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Globals_T globals = {};
   F360_Calibrations_T calib = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Trailer_Estimator_Output_T trailer_data = {};

   float32_t test_pass_thres = 0.0001F;
   bool f_marked_as_multipath;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 1.0F;
      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;
      sensors[2].variable.is_valid = true;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 1.0F;
      sensors[3].variable.is_valid = true;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[3].constant.mounting_position.vcs_position.lateral = -1.0F;

      globals.average_sensor_position_x = -2.0F;

      f_marked_as_multipath = false;

      host.f_trailer_presence_hardware = false;
      trailer_data.trailer_length[0] = 0.0F;
      trailer_data.trailer_length[1] = 0.0F;
   }
};

/**
*\purpose  Purpose of this test is to verify whether when track is on and behind guardrail and flag indicating that it should be hidden has its mirror prob set to 1
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_track_behind_guardrail_has_its_mirror_prob_set_to_one)
{

   /** \precond
   * Set parameters of selected track to be on and behind guardrail so it is qualified as mirror track
   * Host has no trailers attached.
   **/

   calib.hide_tracks_outside_guardrail = true;
   int32_t object_idx = 0;
   object_tracks[0].vcs_position.x = 0;
   object_tracks[0].vcs_position.y = -5.0F;
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -2.0F;
   object_tracks[0].mirror_prob = 0.0F;
   object_tracks[0].behind_sep_id = 1;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 1
   **/

   DOUBLES_EQUAL(1.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track is on and behind guardrail and flag indicating that it should be hidden has its mirror prob set to 0
*          Host has no trailers attached.
*\req
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_mirror_prop_with_wrong_sep_id)
{

   /** \precond
   * Set parameters of selected track to be on and behind guardrail so it is qualified as mirror track but with sep_id equal to 1
   **/

   calib.hide_tracks_outside_guardrail = true;
   int32_t object_idx = 0;
   object_tracks[0].vcs_position.x = 0;
   object_tracks[0].vcs_position.y = -5.0F;
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -2.0F;
   object_tracks[0].mirror_prob = 0.0F;
   object_tracks[0].behind_sep_id = 1;
   object_tracks[0].on_sep_id = 1;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 0
   **/

   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track that is not mirror and host speed is below threshold has its mirror prob reset
           Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_track_that_is_not_a_mirror_track_has_its_mirror_prob_reset)
{

   /** \precond
   * Set parameters of selected track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].mirror_prob = 0.11F;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 0.01
   **/

   DOUBLES_EQUAL(0.01F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track that is not mirror and host speed is below threshold and it is marked as multipath
*          Host has no trailers attached.
*\req
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_track_that_is_not_a_mirror_and_f_marked_as_multipath_true)
{

   /** \precond
   * Set parameters of selected track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].mirror_prob = 1.0F;
   f_marked_as_multipath = true;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 1
   **/

   DOUBLES_EQUAL(1.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track that is not mirror, and host speed is above threshold has its mirror prob reset
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_track_that_is_not_a_mirror_track_has_its_mirror_prob_reset_host_speed_threshold)
{

   /** \precond
   * Set parameters of selected track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].mirror_prob = 0.10F;
   host.vcs_speed = calib.k_reflective_guardrail_track_min_host_speed + test_pass_thres;

   F360_Object_Track_T& ghost_obj = object_tracks[0];
   F360_Object_Track_T& source_obj = object_tracks[1];
   float32_t host_curvature_rear = 0.000001F;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].p0 = -5.0F;
   sep[1].p0 = 5.0F;
   sep[0].p2 = 0.5F * host_curvature_rear;
   sep[1].p2 = 0.5F * host_curvature_rear;
   sep[0].poly_type = F360_STATIC_ENV_POLY_TYPE_CURVG;
   sep[1].poly_type = F360_STATIC_ENV_POLY_TYPE_CURVG;
   sep[0].lower_limit = -100.0F;
   sep[1].lower_limit = -100.0F;
   sep[0].upper_limit = 100.0F;
   sep[1].upper_limit = 100.0F;

   ghost_obj.reference_point = F360_REFERENCE_POINT_CENTER;
   ghost_obj.vcs_position.y = -7.5F;
   ghost_obj.vcs_position.x = 10.0F;
   ghost_obj.bbox.Set_Center(ghost_obj.vcs_position);
   ghost_obj.vcs_heading = Angle(0.0F);
   ghost_obj.hdg_ptng_disagmt = 0.0F;
   ghost_obj.bbox.Set_Orientation(ghost_obj.vcs_heading + ghost_obj.hdg_ptng_disagmt);
   ghost_obj.bbox.Set_Length(1.0F);
   ghost_obj.bbox.Set_Width(1.0F);
   ghost_obj.f_moving = true;
   ghost_obj.movable_prob = 1.0F;
   ghost_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   ghost_obj.status = F360_OBJECT_STATUS_UPDATED;
   ghost_obj.reduced_id = 1;
   ghost_obj.speed = 1.0F;
   ghost_obj.id = 1;

   source_obj.reference_point = F360_REFERENCE_POINT_CENTER;
   source_obj.vcs_position.y = -2.5F;
   source_obj.vcs_position.x = 10.0F;
   source_obj.bbox.Set_Center(source_obj.vcs_position);
   source_obj.vcs_heading = Angle(0.0F);
   source_obj.hdg_ptng_disagmt = 0.0F;
   source_obj.bbox.Set_Orientation(source_obj.vcs_heading + source_obj.hdg_ptng_disagmt);
   source_obj.bbox.Set_Length(1.0F);
   source_obj.bbox.Set_Width(1.0F);
   source_obj.f_moving = true;
   source_obj.movable_prob = 1.0F;
   source_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   source_obj.status = F360_OBJECT_STATUS_UPDATED;
   source_obj.reduced_id = 2;
   source_obj.speed = 1.0F;
   source_obj.id = 2;

   Flag_Single_Object_Behind_SEP(sep, calib, globals, ghost_obj);
   Flag_Single_Object_Behind_SEP(sep, calib, globals, source_obj);

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to calibration value
   **/

   DOUBLES_EQUAL(calib.k_reflected_object_max_mirror_probability, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track that is not mirror has its mirror prob lowered by calib value
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_track_that_is_not_a_mirror_track_has_its_mirror_prob_lowered_by_calib_value)
{

   /** \precond
   * Set parameters of selected track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].mirror_prob = 1.0F;
   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob reduced to 0.9
   **/

   CHECK_EQUAL(0.9F, object_tracks[object_idx].mirror_prob);
}

/**
*\purpose  Purpose of this test is to verify whether a host mirror track is not classified as mirror track when host speed is below threshold 0.2m/s.
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_mirror_track_has_its_mirror_prob_set_to_zero_stationary_host)
{
   /** \precond
   * Set parameters of selected track to be qualified as mirror track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position.x = 0;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_heading = Angle(0.0F);
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(3.0F);
   object_tracks[0].bbox.Set_Width(2.0F);
   object_tracks[0].f_moving = true;
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -2.0F;
   sep[0].p1 = 0.0F;
   sep[0].p2 = 0.0F;
   object_tracks[0].mirror_prob = 0.0F;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 0
   **/

   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether a host mirror track is classified as mirror track when host speed is above threshold 0.2m/s
*          but below k_reflective_guardrail_track_min_host_speed.
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_mirror_track_has_its_mirror_prob_set_to_one_low_host_speed)
{
   /** \precond
   * Set parameters of selected track to be qualified as mirror track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position.x = 0;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_heading = Angle(0.0F);
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(3.0F);
   object_tracks[0].bbox.Set_Width(2.0F);
   object_tracks[0].f_moving = true;
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -2.0F;
   sep[0].p1 = 0.0F;
   sep[0].p2 = 0.0F;
   object_tracks[0].mirror_prob = 0.0F;
   host.speed = 1.0F;
   host.vcs_speed = 1.0F;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 0.9F
   **/

   DOUBLES_EQUAL(0.9F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track is classified as mirror track and host speed is above k_reflective_guardrail_track_min_host_speed
*          threshold it has its mirror prob set to one
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_mirror_track_has_its_mirror_prob_set_to_one_host_speed_threshold)
{
   /** \precond
   * Set parameters of selected track to be qualified as mirror track
   **/
   float test_speed = calib.k_reflective_guardrail_track_min_host_speed + test_pass_thres;

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position.x = 0;
   object_tracks[0].vcs_position.y = -5.0F;
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_heading = Angle(0.0F);
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(3.0F);
   object_tracks[0].bbox.Set_Width(2.0F);
   object_tracks[0].f_moving = true;
   object_tracks[0].speed = test_speed;
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -2.0F;
   sep[0].p1 = 0.0F;
   sep[0].p2 = 0.0F;
   object_tracks[0].mirror_prob = 0.0F;
   host.vcs_speed = test_speed;
   host.speed = test_speed;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 1
   **/

   DOUBLES_EQUAL(1.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether the mirror prob remains as 1 due to that host is in dynamic maneuvre although the mirror condition is not fulfilled
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__mirror_prob_remain_due_to_host_curvature)
{
    /** \precond
    * Set parameters of selected track to be unqualified as mirror track for current scan
    * Set mirror_prob to 1 from previous scan
    * Set host curvature to be relatively large
    * Set a SEP between host and object
    **/

    calib.hide_tracks_outside_guardrail = false;
    int32_t object_idx = 0;
    object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
    object_tracks[0].mirror_prob = 1.0;
    object_tracks[0].cntHostTurnForMirrorProb = 0;
    object_tracks[0].behind_sep_id = 1U;
    object_tracks[0].vcs_position.x = 0;
    object_tracks[0].vcs_position.y = -5.0F;
    object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
    object_tracks[0].vcs_heading = Angle(11.0F);
    object_tracks[0].hdg_ptng_disagmt = 0.0F;
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
    object_tracks[0].bbox.Set_Length(3.0F);
    object_tracks[0].bbox.Set_Width(2.0F);
    sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
    sep[0].lower_limit = -10.0F;
    sep[0].upper_limit = 10.0F;
    sep[0].p0 = -2.0F;
    sep[0].p1 = 0.0F;
    sep[0].p2 = 0.0F;
    host.vcs_speed = calib.k_reflective_guardrail_track_min_host_speed + test_pass_thres;
    host.curvature_rear = 0.05F;

    /** \action
    * Call tested function
    **/

    Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

    /** \result
    * Check whether selected track had its mirror prob remain as 1.0 and there is no increment on cntHostTurnForMirrorProb
    **/

    DOUBLES_EQUAL(1.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
    DOUBLES_EQUAL(0, object_tracks[object_idx].cntHostTurnForMirrorProb, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether the mirror prob remains as 1 but the counter for decremental is rolling
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_mirror_prob_counter_rolling)
{
    /** \precond
    * Set parameters of selected track to be unqualified as mirror track for current scan
    * Set mirror_prob to 1 from previous scan
    * Set a SEP between host and object
    * Set host curvature to be small enough
    **/

    calib.hide_tracks_outside_guardrail = false;
    int32_t object_idx = 0;
    object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
    object_tracks[0].mirror_prob = 1.0;
    object_tracks[0].cntHostTurnForMirrorProb = 9;
    object_tracks[0].behind_sep_id = 1U;
    object_tracks[0].vcs_position.x = 0;
    object_tracks[0].vcs_position.y = -5.0F;
    object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
    object_tracks[0].vcs_heading = Angle(11.0F);
    object_tracks[0].hdg_ptng_disagmt = 0.0F;
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
    object_tracks[0].bbox.Set_Length(3.0F);
    object_tracks[0].bbox.Set_Width(2.0F);
    sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
    sep[0].lower_limit = -10.0F;
    sep[0].upper_limit = 10.0F;
    sep[0].p0 = -2.0F;
    sep[0].p1 = 0.0F;
    sep[0].p2 = 0.0F;
    host.vcs_speed = calib.k_reflective_guardrail_track_min_host_speed + test_pass_thres;
    host.curvature_rear = 0.001F;

    /** \action
    * Call tested function
    **/

    Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

    /** \result
    * Check whether selected track had its mirror prob remain as 1.0 and there is an increment on cntHostTurnForMirrorProb
    **/

    DOUBLES_EQUAL(1.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
    DOUBLES_EQUAL(10, object_tracks[object_idx].cntHostTurnForMirrorProb, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether the mirror prob ramps down after the counter cntHostTurnForMirrorProb reaches 10
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__mirror_prob_ramp_down_when_counter_exceeds_thres)
{
    /** \precond
    * Set parameters of selected track to be unqualified as mirror track for current scan
    * Set mirror_prob to 1 from previous scan
    * Set a SEP between host and object
    * Set cntHostTurnForMirrorProb to 10
    **/

    calib.hide_tracks_outside_guardrail = false;
    int32_t object_idx = 0;
    object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
    object_tracks[0].mirror_prob = 1.0;
    object_tracks[0].cntHostTurnForMirrorProb = 10;
    object_tracks[0].behind_sep_id = 1U;
    object_tracks[0].vcs_position.x = 0;
    object_tracks[0].vcs_position.y = -5.0F;
    object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
    object_tracks[0].vcs_heading = Angle(11.0F);
    object_tracks[0].hdg_ptng_disagmt = 0.0F;
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
    object_tracks[0].bbox.Set_Length(3.0F);
    object_tracks[0].bbox.Set_Width(2.0F);
    object_tracks[0].f_moving = true;
    object_tracks[0].time_since_initialization = 1.0F;
    sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
    sep[0].lower_limit = -10.0F;
    sep[0].upper_limit = 10.0F;
    sep[0].p0 = -2.0F;
    sep[0].p1 = 0.0F;
    sep[0].p2 = 0.0F;
    host.vcs_speed = calib.k_reflective_guardrail_track_min_host_speed + test_pass_thres;
    host.curvature_rear = 0.001F;

    /** \action
    * Call tested function
    **/

    Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

    /** \result
    * Check whether selected track had its mirror prob decrease to 0.9 and cntHostTurnForMirrorProb is reset to 0
    **/

    DOUBLES_EQUAL(0.9F, object_tracks[object_idx].mirror_prob, test_pass_thres);
    DOUBLES_EQUAL(0, object_tracks[object_idx].cntHostTurnForMirrorProb, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether the mirror prob ramps down once the guardrail between host and object disappears
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__mirror_prob_ramp_down_sep_disappear)
{
    /** \precond
    * Set parameters of selected track to be unqualified as mirror track for current scan
    * Set mirror_prob to 1 from previous scan
    * Set cntHostTurnForMirrorProb to 0
    * Set no SEP between host and object
    **/

    calib.hide_tracks_outside_guardrail = false;
    int32_t object_idx = 0;
    object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
    object_tracks[0].mirror_prob = 1.0;
    object_tracks[0].cntHostTurnForMirrorProb = 0;
    object_tracks[0].behind_sep_id = 0U;
    object_tracks[0].vcs_position.x = 0;
    object_tracks[0].vcs_position.y = -5.0F;
    object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
    object_tracks[0].vcs_heading = Angle(11.0F);
    object_tracks[0].hdg_ptng_disagmt = 0.0F;
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
    object_tracks[0].bbox.Set_Length(3.0F);
    object_tracks[0].bbox.Set_Width(2.0F);
    sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
    sep[0].lower_limit = -10.0F;
    sep[0].upper_limit = 10.0F;
    sep[0].p0 = -2.0F;
    sep[0].p1 = 0.0F;
    sep[0].p2 = 0.0F;
    host.vcs_speed = calib.k_reflective_guardrail_track_min_host_speed + test_pass_thres;
    host.curvature_rear = 0.001F;

    /** \action
    * Call tested function
    **/

    Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

    /** \result
    * Check whether selected track had its mirror prob decrease to 0.9 and cntHostTurnForMirrorProb remains as 0
    **/

    DOUBLES_EQUAL(0.9F, object_tracks[object_idx].mirror_prob, test_pass_thres);
    DOUBLES_EQUAL(0, object_tracks[object_idx].cntHostTurnForMirrorProb, test_pass_thres);
}

/**
   sep[0].p2 = 0.0F;
*\purpose  Purpose of this test is to verify whether when track is on guardrail, not behind it but is not a mirror track it has its mirror prob reset
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_valid_track_in_front_and_on_guardrail_has_its_mirror_prob_reset)
{
   /** \precond
   * Set parameters of selected track to be located in front and on guardrail
   * Set parameters of selected track to not be qualified as mirror track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[object_idx].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[object_idx].vcs_position.x = 0;
   object_tracks[object_idx].vcs_position.y = -4.9F;
   object_tracks[object_idx].bbox.Set_Center(object_tracks[object_idx].vcs_position);
   object_tracks[object_idx].vcs_heading = Angle(0.0F);
   object_tracks[object_idx].hdg_ptng_disagmt = 0.0F;
   object_tracks[object_idx].bbox.Set_Orientation(object_tracks[object_idx].vcs_heading + object_tracks[object_idx].hdg_ptng_disagmt);
   object_tracks[object_idx].bbox.Set_Length(5.0F);
   object_tracks[object_idx].bbox.Set_Width(2.0F);
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -5.0F;
   object_tracks[object_idx].mirror_prob = 0.12F;
   object_tracks[0].mirror_prob = 0.08F;
   object_tracks[object_idx].on_sep_id = 1;


   /** \action
   *describe actions
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 0
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose  Purpose of this test is to verify whether when track is not on guardrail, is behind it but is not a mirror track it has its mirror_prob reset
*          Host has no trailers attached.
*\req    CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj, Determine_Reflected_Obj__check_whether_valid_track_behind_guardrail_has_its_mirror_prob_reset)
{

   /** \precond
   * Set parameters of analysed track to not be treated as mirror reflection behind guardrail
   * Set parameters of track to make it not being qualified as mirror track
   **/

   calib.hide_tracks_outside_guardrail = false;
   int32_t object_idx = 0;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -8.0F;
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_heading = Angle{ 1.0F };
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(5.0F);
   object_tracks[0].bbox.Set_Width(2.0F);
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p0 = -5.0F;
   object_tracks[0].mirror_prob = 0.08F;

   /** \action
   * Call tested function
   **/

   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Check whether selected track had its mirror prob set to 0
   **/

   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/** \purpose
 * Check calibrations correctness
 * Host has no trailers attached.
 * \req
 * CPR-4874
 */
TEST(f360_determine_reflected_obj, check_calibrations)
{
    /** \precond
     * Same as setup (initialized calibrations)
     */

    /** \result
     * Calibrations set as expected
     */
    CHECK_FALSE(calib.hide_tracks_outside_guardrail);
    DOUBLES_EQUAL(1.0F, calib.k_reflected_object_max_mirror_probability, test_pass_thres);
    DOUBLES_EQUAL(1.0F, calib.k_reflective_guardrail_track_min_host_speed, test_pass_thres);
}

/** @}*/


/** \defgroup  f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail
 *  @{
 */

 /** \brief
 *  Test group of f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail
 *  It checks output by focusing on an object and a guardrail relation.
 **/
TEST_GROUP(f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail)
{
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Calibrations_T calib = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Trailer_Estimator_Output_T trailer_data = {};

   float32_t test_pass_thres = 0.0001F;
   bool f_marked_as_multipath;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      calib.hide_tracks_outside_guardrail = true;
      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 1.0F;
      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;
      sensors[2].variable.is_valid = true;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 1.0F;
      sensors[3].variable.is_valid = true;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[3].constant.mounting_position.vcs_position.lateral = -1.0F;

      f_marked_as_multipath = false;
      host.f_trailer_presence_hardware = false;
      trailer_data.trailer_length[0] = 0.0F;
      trailer_data.trailer_length[1] = 0.0F;
   }
};

/**
*\purpose   Checks if mirror probability is set to 0 when object is on guradrail but not behind guardrail and its current mirror probability is low
*           Host has no trailers attached.
*\req       CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail, prob_set_to_zero_because_track_is_on_but_not_behind_guardrail_and_low_initial_prob)
{
   /** \precond
   * Set object's mirror probability to low value
   * Object position close to a guardrail and on the left side of the guardrail
   **/
   const float32_t eps = 0.0001F;

   int32_t object_idx = 1;
   F360_Object_Track_T &obj = object_tracks[object_idx];

   obj.mirror_prob = 0.0001F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.vcs_position.x = 1.0F;
   obj.vcs_position.y = 1.0F;
   obj.bbox.Set_Center(obj.vcs_position);
   obj.vcs_heading = Angle(0.0F);
   obj.hdg_ptng_disagmt = 0.0F;
   obj.bbox.Set_Orientation(obj.vcs_heading + obj.hdg_ptng_disagmt);
   obj.bbox.Set_Length(5.0F);
   obj.bbox.Set_Width(2.0F);
   obj.on_sep_id = 1;

   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = obj.vcs_position.x - eps;
   sep[0].upper_limit = obj.vcs_position.x + eps;
   sep[0].p0 = obj.vcs_position.y + eps;

   /** \action
   * Call tested function
   **/
   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Mirror probability set to 0
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose   Checks if mirror probability is set to 1 when object is not on guradrail but it is behind guardrail
*           Host has no trailers attached.
*\req       CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail, prob_set_to_one_because_track_is_not_on_but_behind_guardrail)
{
   /** \precond
   * Object outside of a guardrail (i.e. its lateral position is much higher than guardrail lateral position).
   **/
   const float32_t eps = 0.0001F;

   int32_t object_idx = 1;
   F360_Object_Track_T &obj = object_tracks[object_idx];

   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.vcs_position.x = 1.0F;
   obj.vcs_position.y = 5.0F;
   obj.bbox.Set_Center(obj.vcs_position);
   obj.vcs_heading = Angle(0.0F);
   obj.hdg_ptng_disagmt = 0.0F;
   obj.bbox.Set_Orientation(obj.vcs_heading + obj.hdg_ptng_disagmt);
   obj.bbox.Set_Length(5.0F);
   obj.bbox.Set_Width(2.0F);
   obj.behind_sep_id = 1;

   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = obj.vcs_position.x - eps;
   sep[0].upper_limit = obj.vcs_position.x + eps;
   sep[0].p0 = 1.0F;

   /** \action
   * Call tested function
   **/
   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Mirror probability set to 1
   **/
   DOUBLES_EQUAL(1.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose   Checks if mirror probability is set to 0 when object is on guradrail and it is behind guardrail and object's current mirror probability is low
*           Host has no trailers attached.
*\req       CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail, prob_set_to_zero_because_track_is_on_and_behind_guardrail_and_low_initial_prob)
{
   /** \precond
   * Object's mirror probability set to low value
   * Object position close to a guardrail and on the right side (outside) of the guardrail
   * Host speed set to 10
   **/
   const float32_t eps = 0.0001F;

   int32_t object_idx = 1;
   F360_Object_Track_T &obj = object_tracks[object_idx];

   obj.mirror_prob = 0.0001F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.vcs_position.x = 1.0F;
   obj.vcs_position.y = 1.0F;
   obj.bbox.Set_Center(obj.vcs_position);
   obj.vcs_heading = Angle(0.0F);
   obj.hdg_ptng_disagmt = 0.0F;
   obj.bbox.Set_Orientation(obj.vcs_heading + obj.hdg_ptng_disagmt);
   obj.bbox.Set_Length(5.0F);
   obj.bbox.Set_Width(2.0F);

   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = obj.vcs_position.x - eps;
   sep[0].upper_limit = obj.vcs_position.x + eps;
   sep[0].p0 = obj.vcs_position.y - eps;

   host.speed = 10.0F;

   /** \action
   * Call tested function
   **/
   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Mirror probability set to 1
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/**
*\purpose   Checks if mirror probability is set to 1 when object is not on guradrail and not behind guardrail and object's current mirror probability is low
*           Host has no trailers attached.
*\req       CPR-4874 CPR-4875
*/
TEST(f360_determine_reflected_obj_with_turned_hide_tracks_outside_guardrail, prob_set_to_one_because_track_is_not_on_and_not_behind_guardrail_and_low_init_prob)
{
   /** \precond
   * Object's mirror probability set to low value
   * Object position far away from a guardrail and on the left side (inside) of the guardrail
   **/
   const float32_t eps = 0.0001F;

   int32_t object_idx = 1;
   F360_Object_Track_T &obj = object_tracks[object_idx];

   obj.mirror_prob = 0.001F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.vcs_position.x = 1.0F;
   obj.vcs_position.y = 1.0F;
   obj.bbox.Set_Center(obj.vcs_position);
   obj.vcs_heading = Angle(0.0F);
   obj.hdg_ptng_disagmt = 0.0F;
   obj.bbox.Set_Orientation(obj.vcs_heading + obj.hdg_ptng_disagmt);
   obj.bbox.Set_Length(5.0F);
   obj.bbox.Set_Width(2.0F);

   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = obj.vcs_position.x - eps;
   sep[0].upper_limit = obj.vcs_position.x + eps;
   sep[0].p0 = 10.0F;

   /** \action
   * Call tested function
   **/
   Determine_Reflected_Obj(tracker_info, object_idx, host, sep, calib, sensors, trailer_data, f_marked_as_multipath, object_tracks);

   /** \result
   * Mirror probability set to 0
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[object_idx].mirror_prob, test_pass_thres);
}

/** @}*/
