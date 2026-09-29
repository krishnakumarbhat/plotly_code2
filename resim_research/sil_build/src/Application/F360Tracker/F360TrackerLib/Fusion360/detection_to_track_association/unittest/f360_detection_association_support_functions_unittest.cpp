/** \file
 * This file contains unit tests for content of f360_detection_association_support_functions.cpp file
 */

#include "f360_detection_association_support_functions.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;


/** \defgroup  f360_Test_Is_Det_Allowed_To_Associate
*  @{
*/
/** \brief
*  Included tests related to calculation of detection is allowed to associate
*  based flags and properties
**/
TEST_GROUP(f360_Test_Is_Det_Allowed_To_Associate)
{

   F360_Detection_Props_T det_prop = {};
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_Object_Track_T obj_track = {};
   F360_Calibrations_T calib = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};

   /** \setup
   * Setup test so that the highest complexity branch passes.
   * Then tweak these parameters to reach full branch coverage in
   * each individual test
   **/
   TEST_SETUP()
   {
      // Detection data
      det_prop.f_ok_to_use = true;
      det_prop.on_sep_id = F360_INVALID_UNSIGNED_ID;
      det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_prop.vcs_position.x = 0.0F;
      det_prop.vcs_position.y = 7.0F;
      det_prop.f_nd_target = false;
      det_raw.raw.f_bistatic = false;
	  
      // Object data
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_track.f_moving = true;
      obj_track.mirror_prob = 0.0F;
      obj_track.behind_sep_id = F360_INVALID_UNSIGNED_ID;
      obj_track.vcs_heading = Angle{ 0.0F };
      obj_track.vcs_position.x = 0.0F;
      obj_track.vcs_position.y = 7.0F;
      Point center = obj_track.vcs_position;
      obj_track.bbox.Set_Center(center);
      obj_track.bbox.Set_Orientation(0.0F);
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;

      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].p2 = 0.0F;
      sep[0].p1 = 0.0F;
      sep[0].p0 = 8.0F;
      sep[0].upper_limit = 0.5F;
      sep[0].lower_limit = -0.5F;
	  
      // Init calibrations as defaults
      Initialize_Tracker_Calibrations(calib);
   }
};

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when all conditions
*          fulfilled in Is_Det_Allowed_To_Associate() and object is of CTCA type
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate)
{
   /** \precond
   * Object and detection properties set to fulfill all conditions of Is_Det_Allowed_To_Associate in TEST_SETUP.
   **/

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when all conditions
*          fulfilled in Is_Det_Allowed_To_Associate()  and object is of CCA type
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_CCA)
{
   /** \precond
   * Object and detection properties set to fulfill all conditions of Is_Det_Allowed_To_Associate in TEST_SETUP.
   **/

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/

   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when the object is behind a SEP and detection is not
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Not_Allowed_Due_To_SEP)
{
   /** \precond
   * Object and detection properties set to fulfill all conditions of Is_Det_Allowed_To_Associate in TEST_SETUP.
   * Set detection position is already set to in front of the SEP in TEST_SETUP.
   * Set object position to in behind the SEP.
   **/
   obj_track.vcs_position.y = 9.0F;
   obj_track.Update_Bbox_Center();

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when detection is flagged as
*          on guardrail and the object is a moving CCV track.
*\req    N/A
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_On_Guardrail_Obj_Moving_CCV)
{
   /** \precond
   * Set object to CCV
   * Set detections to be on guardrail
   **/
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   det_prop.on_sep_id = 1;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}


/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when detection is flagged as
*          on guardrail and the object is a fast moving CCA track.
*\req    N/A
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_On_Guardrail_Obj_Fast_Moving_CCA)
{
   /** \precond
   * Set object to CCA
   * Set object speed to just above calib.k_fast_moving_thres
   * Set detections to be on guardrail
   **/
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.speed = calib.fast_moving_thresh + 1e-3F;
   det_prop.on_sep_id = 1;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}


/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when detection is flagged as
*          on guardrail and the object is a slow moving CCA track.
*\req    N/A
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_On_Guardrail_Obj_Slow_Moving_CCA)
{
   /** \precond
   * Set object to CCA
   * Set object speed to just below calib.k_fast_moving_thres
   * Set detections to be on guardrail
   **/
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.speed = calib.fast_moving_thresh - 1e-3F;
   det_prop.on_sep_id = 1;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}


/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when the detection is flagged as
*          on guardrail but the object is not flagged as moving
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_On_Guardrail_Obj_Not_Moving)
{
   /** \precond
   * Set object to moving
   * Set detections to be on guardrail
   **/
   obj_track.f_moving = false;
   det_prop.on_sep_id = 1;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true due to object having a high mirror probability
*          but the detection motion status is moving which should allow association.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Mirror_Obj_Moving_Detection)
{
   /** \precond
   * Set object mirror probability high
   * Detection motion status set to moving in TEST_SETUP
   *
   **/
   obj_track.mirror_prob = 0.9F;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}


/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when object and detection are both behind the guardrail.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Object_And_Det_Behind_Of_Guardrail_Obj_Moving)
{
   /** \precond
   * Set object position behind guardrail
   * Set detection position behind guardrail
   * Object set to moving in TEST_SETUP
   **/
   obj_track.vcs_position.y = 9.0F;
   obj_track.Update_Bbox_Center();
   det_prop.vcs_position.y = 9.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when object is not moving. Detection and object
*          set to not behind guardrail.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Object_Not_Moving)
{
   /** \precond
   * Set object to not moving
   * Detection and object both set to not behind guardrail in TEST_SETUP
   **/
   obj_track.f_moving = false;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when object and detection are on opposite side of guardrail
*          while object is flagged as not moving. Here the object is behind guardrail while the detection is not.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Obj_Behind_Guardrail_But_Not_Moving)
{
   /** \precond
   * Set object behind guardrail
   * Detection set to not behind guardrail in TEST_SETUP
   * Object flagged as not moving.
   **/
   obj_track.vcs_position.y = 9.0F;
   obj_track.Update_Bbox_Center();
   obj_track.f_moving = false;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when object and detection are on opposite side of guardrail
*          while object is flagged as not moving. Here the detection is behind guardrail while the object is not.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_Behind_Guardrail_Obj_Not_Moving)
{
   /** \precond
   * Set detection behind guardrail
   * Object set to not behind guardrail in TEST_SETUP
   * Object flagged as not moving.
   **/
   det_prop.vcs_position.y = 9.0F;
   obj_track.Update_Bbox_Center();
   obj_track.f_moving = false;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true due to object and detection both being behind guardrail
*          while object is flagged as not moving.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_And_Obj_Behind_Guardrail_Obj_Not_Moving)
{
   /** \precond
   * Set detection behind guardrail
   * Set object behind guardrail
   * Object flagged as not moving.
   **/
   obj_track.vcs_position.y = 9.0F;
   obj_track.Update_Bbox_Center();
   det_prop.vcs_position.y = 9.0F;
   obj_track.f_moving = false;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  Purpose of this test is to verify whether bistatic dets are allowed to associate to CTCA objects.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_Is_Bistatic_CTCA_Object)
{
   /** \precond
   * Set required properties to make detection be allowed to associate
   * Set detection f_bistatic flag as true
   * Set object tracker filter type as CTCA
   **/
   det_prop.behind_sep_id = 1;
   obj_track.behind_sep_id = 1;
   obj_track.f_moving = false;

   det_raw.raw.f_bistatic = true;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  Purpose of this test is to verify whether bistatic dets are allowed to associate to CCA objects if they are fast moving (speed above calib.fast_moving_thresh).
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_Is_Bistatic_CCA_Object_Fast_Moving)
{
   /** \precond
   * Set required properties to make detection be allowed to associate
   * Set detection f_bistatic flag as true
   * Set object tracker filter type as CCA
   * Set object speed to just above calib.fast_moving_thresh
   **/
   det_prop.behind_sep_id = 1;
   obj_track.behind_sep_id = 1;
   obj_track.f_moving = false; // To prevent the detection on guardrail countermeasure to kick in so that test can focus only on bistatic countermeasure
   obj_track.speed = calib.fast_moving_thresh + 1e-3F;

   det_raw.raw.f_bistatic = true;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  Purpose of this test is to verify whether bistatic dets are not allowed to associate to CCA objects if they are slow moving (below above calib.fast_moving_thresh).
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_Is_Bistatic_CCA_Object_Slow_Moving)
{
   /** \precond
   * Set required properties to make detection be allowed to associate
   * Set detection f_bistatic flag as true
   * Set object tracker filter type as CCA
   * Set object speed to just below calib.fast_moving_thresh
   **/
   det_prop.behind_sep_id = 1;
   obj_track.behind_sep_id = 1;
   obj_track.f_moving = false; // To prevent the detection on guardrail countermeasure to kick in so that test can focus only on bistatic countermeasure
   obj_track.speed = calib.fast_moving_thresh - 1e-3F;

   det_raw.raw.f_bistatic = true;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  Purpose of this test is to verify whether low_az_conf detection not associate to object
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_Det_That_Is_Close_To_Host_And_Low_Az_Conf)
{
   /** \precond
   * Set required properties to make detection be not allowed to associate
   * Set detection position to (3.0, 1.0) and set f_low_az_conf_det as true
   * Set object position to (1.5, 2.2) and other properties
   * These properties need to be prepared to make function false due to function
   * Is_Low_Az_Conf_At_Boundaries_Association_Allowed()
   **/
   // Detection data
   det_prop.vcs_position.x = 3.0F;
   det_prop.vcs_position.y = 1.0F;
   det_prop.f_low_az_conf_det = true;

   // Object data
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj_track.f_moving = true;
   obj_track.vcs_position.x = 1.5F;
   obj_track.vcs_position.y = 2.2F;
   obj_track.bbox.Set_Orientation(0.0F);
   obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);
   obj_track.bbox.Set_Width(1.6F);
   obj_track.bbox.Set_Length(4.5F);
   obj_track.lat_buffer_zone_wid1 = 0.6F;
   obj_track.lat_buffer_zone_wid2 = 0.6F;
   obj_track.long_buffer_zone_len2 = 1.4F;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when object is young, and the detection to be evaluated has high elevation and low az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Young_object_High_elevation_Low_az_conf_Thus_Det_Rejected)
{
   /** \precond
   * Set young object less than 2 seconds
   * Set detection elevation greater than 11 degrees
   * Set the detection as a low conf det
   **/
   obj_track.time_since_initialization = 1.5F;

   det_raw.raw.elevation = F360_DEG2RAD(12.0F);
   det_prop.f_low_az_conf_det = true;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when moving object is mature
* and the detection to be evaluated has high elevation and low az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Mature_object_High_elevation_Low_az_conf_Thus_Det_Accepted)
{
   /** \precond
   * Set mature object greater than 2 seconds
   * Set detection elevation greater than 11 degrees
   * Set the detection as a low conf det
   **/
   obj_track.time_since_initialization = 2.5F;

   det_raw.raw.elevation = F360_DEG2RAD(12.0F);
   det_prop.f_low_az_conf_det = true;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when moving object is young
* and the detection to be evaluated has high elevation and high az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Young_object_High_elevation_High_az_conf_Thus_Det_Accepted)
{
   /** \precond
   * Set young object less than 2 seconds
   * Set detection elevation greater than 11 degrees
   * Set the detection as a high conf det
   **/
   obj_track.time_since_initialization = 1.5F;

   det_raw.raw.elevation = F360_DEG2RAD(12.0F);
   det_prop.f_low_az_conf_det = false;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when object is mature
* and the detection to be evaluated has high elevation and high az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Mature_object_High_elevation_High_az_conf_Thus_Det_Accepted)
{
   /** \precond
   * Set mature object greater than 2 seconds
   * Set detection elevation greater than 11 degrees
   * Set the detection as a high conf det
   **/
   obj_track.time_since_initialization = 2.5F;

   det_raw.raw.elevation = F360_DEG2RAD(12.0F);
   det_prop.f_low_az_conf_det = false;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when moving object is mature
* and the detection to be evaluated has low elevation and low az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Mature_object_Low_elevation_Low_az_conf_Thus_Det_Accepted)
{
   /** \precond
   * Set young object greater than 2 seconds
   * Set detection elevation lesser than 11 degrees
   * Set the detection as a low conf det
   **/
   obj_track.time_since_initialization = 2.5F;

   det_raw.raw.elevation = F360_DEG2RAD(10.0F);
   det_prop.f_low_az_conf_det = true;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}


/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when object is young and the detection to be evaluated has low elevation and low az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Young_object_Low_elevation_Low_az_conf_Thus_Det_Accepted)
{
   /** \precond
   * Set young object lesser than 2 seconds
   * Set detection elevation lesser than 11 degrees
   * Set the detection as a low conf det
   **/
   obj_track.time_since_initialization = 1.5F;

   det_raw.raw.elevation = F360_DEG2RAD(10.0F);
   det_prop.f_low_az_conf_det = true;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}


/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when object is mature and the detection to be evaluated has low elevation and low az conf.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_When_Mature_object_Low_elevation_High_az_conf_Thus_Det_Accepted)
{
   /** \precond
   * Set young object lesser than 2 seconds
   * Set detection elevation lesser than 11 degrees
   * Set the detection as a low conf det
   **/
   obj_track.time_since_initialization = 1.5F;

   det_raw.raw.elevation = F360_DEG2RAD(10.0F);
   det_prop.f_low_az_conf_det = false;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when detection is angle jump but object is not mature enough.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_AJ_Det_Obj_Not_Mature)
{
   /** \precond
   * Set young object lesser than 4 seconds
   * Set the detection as a object based angle jump
   * Set the detection as not f_ok_to_use
   **/
   det_prop.f_ok_to_use = false;
   det_prop.f_object_based_angle_jump = true;
   obj_track.time_since_initialization = 3.9F;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when detection is angle jump and object is mature with some non zero mirror probability.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_AJ_Det_Obj_Mature_And_High_Mirror_Prob)
{
   /** \precond
   * Set the detection as a object based angle jump
   * Set the detection as not f_ok_to_use
   * Set object's lifespan to more than 4 seconds
   * Set object's mirror probability to 0.1
   **/
   det_prop.f_ok_to_use = false;
   det_prop.f_object_based_angle_jump = true;
   obj_track.time_since_initialization = 4.1F;
   obj_track.mirror_prob = 0.1F;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when detection is angle jump and object is mature with zero mirror probability.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_AJ_Det_Obj_Mature_And_Zero_Mirror_Prob)
{
   /** \precond
   * Set the detection as a object based angle jump
   * Set the detection as not f_okay_to_use
   * Set object's lifespan to more than 4 seconds
   * Set object's mirror probability to 0
   **/
   det_prop.f_ok_to_use = false;
   det_prop.f_object_based_angle_jump = true;
   obj_track.time_since_initialization = 4.1F;
   obj_track.mirror_prob = 0.0F;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when detection is double bounce and has f_ok_to_use flag set to false.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_DB_Det_Not_Ok_To_Use)
{
   /** \precond
   * Set the detection as a double bounce
   * Set the detection as not f_okay_to_use
   **/
   det_prop.f_ok_to_use = false;
   det_prop.f_double_bounce = true;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns true when detection is nd_target and object is mature 0 mirror prob.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_ND_Target_Stable_Object)
{
   /** \precond
   * Set the detection as a nd_target
   * Set the object time since initialization to more than 2
   * Set object's mirror probability to 0
   **/
   det_prop.f_nd_target = true;
   obj_track.time_since_initialization = 2.1F;
   obj_track.mirror_prob = 0.0F;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is true
   **/
   CHECK_TRUE_TEXT(f_allowed_to_associate, "Detection should be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when detection is nd_target but object is not mature enough.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_Associate_ND_Target_Det_Obj_Not_Mature)
{
   /** \precond
   * Set young object lesser than 2 seconds
   * Set the detection as a nd_target
   **/
   det_prop.f_nd_target = true;
   obj_track.time_since_initialization = 1.9F;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false when detection is nd_target and object is mature with some non zero mirror probability.
*\req    NA
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate, Test_Is_Det_Allowed_To_ND_Target_Det_Obj_Mature_And_High_Mirror_Prob)
{
   /** \precond
   * Set the detection as a nd_target
   * Set object's lifespan to more than 2 seconds
   * Set object's mirror probability to 0.1
   **/
   det_prop.f_nd_target = true;
   obj_track.time_since_initialization = 2.1F;
   obj_track.mirror_prob = 0.1F;


   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}
/** @}*/

/** \defgroup  f360_Test_Calc_Range_Rate_Threshold
*  @{
*/
/** \brief
*  Included tests related to calculation of range rate threshold for detection association for moveable objects.
**/
TEST_GROUP(f360_Test_Calc_Range_Rate_Threshold)
{

   F360_Object_Track_T obj_track;
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_Detection_Props_T det_prop;
   F360_Calibrations_T calib = {};
   F360_Radar_Sensor_T sensor = {};
   float32_t range_rate_score_threshold;
   float32_t host_vcs_speed;

   float32_t exp_fov_edge_result;
   float32_t exp_far_coasted_result;
   float32_t exp_general_result;

   /** \setup
   * Setup test so that the highest complexity branch passes.
   * Then tweak these parameters to reach full branch coverage in
   * each individual test
   **/
   TEST_SETUP()
   {
      obj_track.vcs_position.x = 50.0F;
      obj_track.vcs_position.y = 10.0F;
      obj_track.status = F360_OBJECT_STATUS_COASTED;
      obj_track.speed = 10.0F;
      obj_track.movable_prob = 1.0F;
      obj_track.time_since_stage_start = 1.0F;
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = obj_track.vcs_position;
      obj_track.bbox.Set_Center(center);
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_track.heading_rate = 0.3F; // Set to something non-zero such that we can verify that the CTCA association gates are not impacted by the CCA gate extension for yawing objects
      obj_track.assoc_dets_pct_filtered = 1.0F;

      det_prop.f_FOV_edge = false;
      det_prop.vcs_position.x = 51.0F; // Set different compared to object position such that there is a leverage arm for the yaw rate component of object velocity
      det_prop.vcs_position.y = 11.0F; // Set different compared to object position such that there is a leverage arm for the yaw rate component of object velocity
      det_raw.processed.cos_vcs_az = 0.9775F;
      det_raw.processed.sin_vcs_az = 0.2108F;

      Initialize_Tracker_Calibrations(calib);

      // This is constant for all tests
      range_rate_score_threshold = 2.0F;
      host_vcs_speed = 5.0F;

      // Three possible return values
      exp_fov_edge_result = range_rate_score_threshold * calib.k_rr_thr_factor_fov_edge;
      exp_far_coasted_result = range_rate_score_threshold * calib.k_rr_thr_factor_far_away_coasted;
      exp_general_result = calib.k_range_rate_score_threshold;
   }

};

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "far away coasting track" range rate threshold is expected
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_A)
{
   /** \action
   *Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   * The expected value depends on currently hardcoded parameters in function
   **/
   DOUBLES_EQUAL_TEXT(-exp_far_coasted_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(exp_far_coasted_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object speed too low.
*          It will test that the association gates are not extended but are kept the the "general value" for a slow moving CCA object.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_B_Slow_CCA)
{
   /** \precond
   * Set object speed beneath threshold
   * Set track filter type to CCA
   **/
   obj_track.speed = 1.0F;
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
   *Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object not too far away
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_C_CTCA)
{
   /** \precond
   * Set object position close
   **/
   obj_track.vcs_position.x = 10.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object not too far away
*          The test will verify that the gates are extended in the positive direction for a fast moving CCA object but that the lower 
*          threshold is set to the "general value".
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_C_CCA_Extend_Upper_Th)
{
   /** \precond
   * Set object position close
   * Set object track filter type to CCA (speed is fast moving from test group)
   * Set the detection position close to object position but sligtly different
   * such that there is a leverage arm for the yaw rate compnent of object velocity
   * Set detection azimuth to correspond to detection position,
   **/
   obj_track.vcs_position.x = 10.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   det_prop.vcs_position.x = 9.0F;
   det_raw.processed.cos_vcs_az = 0.6332F;
   det_raw.processed.sin_vcs_az = 0.7740F;

   /** \action
   *Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   float32_t expected_extended_result_upper = exp_general_result + 0.4221602F;
   float32_t expected_extended_result_lower = -exp_general_result;
   DOUBLES_EQUAL_TEXT(expected_extended_result_lower, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(expected_extended_result_upper, rdot_thres_upper, 1e-7F, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object not too far away
*          The test will verify that the gates are extended in the positive direction for a fast moving CCA object but that the lower 
*          threshold is set to the "general value". The test will check that the upper threshold is saturated at the maximum allowed value
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_C_CCA_Extend_Upper_Th_Saturated)
{
   /** \precond
   * Set object position close
   * Set object track filter type to CCA (speed is fast moving from test group)
   * Increase object yaw rate such that the extention becomes too large
   * Set the detection position close to object position but sligtly different
   * such that there is a leverage arm for the yaw rate compnent of object velocity
   * Set detection azimuth to correspond to detection position,
   **/
   obj_track.vcs_position.x = 10.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.heading_rate = 1.8F;
   det_prop.vcs_position.x = 9.0F;
   det_raw.processed.cos_vcs_az = 0.6332F;
   det_raw.processed.sin_vcs_az = 0.7740F;

   /** \action
   *Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   float32_t expected_extended_result_upper = 4.0F;
   float32_t expected_extended_result_lower = -exp_general_result;
   DOUBLES_EQUAL_TEXT(expected_extended_result_lower, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(expected_extended_result_upper, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object not too far away
*          The test will verify that the gates are extended in the negative direction for a fast moving CCA object but that the upper 
*          threshold is set to the "general value".
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_C_CCA_Extend_Lower_Th)
{
   /** \precond
   * Set object position close
   * Set object track filter type to CCA (speed is fast moving from test group)
   * Negate the object yaw rate such that the extension is in the negative direction instead of in the positive
   * Set the detection position close to object position but slightly different
   * such that there is a leverage arm for the yaw rate component of object velocity
   * Set detection azimuth to correspond to detection position,
   **/
   obj_track.vcs_position.x = 10.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.heading_rate = -0.3F;
   det_prop.vcs_position.x = 9.0F;
   det_raw.processed.cos_vcs_az = 0.6332F;
   det_raw.processed.sin_vcs_az = 0.7740F;

   /** \action
   *Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   float32_t expected_extended_result_upper = exp_general_result;
   float32_t expected_extended_result_lower = -exp_general_result - 0.4221602F;
   DOUBLES_EQUAL_TEXT(expected_extended_result_lower, rdot_thres_lower, 1e-7F, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(expected_extended_result_upper, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object not too far away
*          The test will verify that the gates are extended in the negative direction for a fast moving CCA object but that the upper 
*          threshold is set to the "general value". The test will check that the lower threshold is saturated at the mimimum allowed value
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_C_CCA_Extend_Lower_Th_Saturated)
{
   /** \precond
   * Set object position close
   * Set object track filter type to CCA (speed is fast moving from test group)
   * Negare and decrease object yaw rate such that the extention becomes too large
   * Set the detection position close to object position but sligtly different
   * such that there is a leverage arm for the yaw rate compnent of object velocity
   * Set detection azimuth to correspond to detection position,
   **/
   obj_track.vcs_position.x = 10.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.heading_rate = -1.8F;
   det_prop.vcs_position.x = 9.0F;
   det_raw.processed.cos_vcs_az = 0.6332F;
   det_raw.processed.sin_vcs_az = 0.7740F;

   /** \action
   *Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   float32_t expected_extended_result_upper = exp_general_result;
   float32_t expected_extended_result_lower = -4.0F;
   DOUBLES_EQUAL_TEXT(expected_extended_result_lower, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(expected_extended_result_upper, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object stage age too young
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_D)
{
   /** \precond
   * Set time since stage start to small
   **/
   obj_track.time_since_stage_start = 0.0F;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "general" range rate threshold is expected due to object is updated
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_E)
{
   /** \precond
   * Set object position close
   **/
   obj_track.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where "FOV edge" range rate threshold is expected due to detection
*          is a "field of view edge" detection
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_F)
{
   /** \precond
   * Set detections flag to true
   **/
   det_prop.f_FOV_edge = true;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_fov_edge_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold")
   DOUBLES_EQUAL_TEXT(exp_fov_edge_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold")
}

/**
*\purpose  This test will test if proper value is returned from Calc_Range_Rate_Threshold()
           when detection has f_elevation_unreliable flag is set.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_For_Unreliable_Elevation)
{
   /** \precond
   * Set detections flag to true
   **/
   det_raw.raw.elevation = F360_DEG2RAD(6.0F) + 0.1F;
   sensor.constant.sensor_type = F360_SENSOR_TYPE_MRR360_RADAR;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_fov_edge_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold");
   DOUBLES_EQUAL_TEXT(exp_fov_edge_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold");
}

/**
*\purpose  This test will test if proper value is returned from Calc_Range_Rate_Threshold()
           when object is movable, has low percentage of associated detections and 
           is moving for over 1s.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moveable_Object_Small_Pct_Of_Assoc_Dets)
{
   /** \precond
   * Set object's:
   * - percentage of associated detections slightly below 0.5
   * - time since started to move to slightly above 1.0
   * - status to updated
   **/
   obj_track.assoc_dets_pct_filtered = 0.49F;
   obj_track.time_since_started_move = 1.01F;
   obj_track.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches expected value.
   **/
   constexpr float32_t exp_noise_rdot_thres = 1.5F;
   DOUBLES_EQUAL_TEXT(-exp_noise_rdot_thres, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match expected threshold");
   DOUBLES_EQUAL_TEXT(exp_noise_rdot_thres, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match expected threshold");
}

/**
*\purpose  This test will test if proper value is returned from Calc_Range_Rate_Threshold()
           when detection has f_low_az_conf_det=true, high elevation (>20 degrees), and close range (<4.0F).
           This combination should trigger the FOV edge threshold.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Low_Az_Conf_High_Elev_Close_Range)
{
   /** \precond
   * Set detection properties:
   * - f_low_az_conf_det = true
   * - elevation > abs(20 degrees) 
   * - range < 4.0F
   * All conditions met for FOV edge threshold
   **/
   det_prop.f_low_az_conf_det = true;
   det_raw.raw.elevation = F360_DEG2RAD(25.0F); // Greater than 20 degrees
   det_raw.raw.range = 3.0F; // Less than 4.0F

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches FOV edge expected value.
   **/
   DOUBLES_EQUAL_TEXT(-exp_fov_edge_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match FOV edge threshold");
   DOUBLES_EQUAL_TEXT(exp_fov_edge_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match FOV edge threshold");
}

/**
*\purpose  This test will test if proper value is returned from Calc_Range_Rate_Threshold()
           when detection has f_low_az_conf_det=true but low elevation (<=20 degrees).
           This should NOT trigger the FOV edge threshold.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Low_Az_Conf_Low_Elev)
{
   /** \precond
   * Set detection properties:
   * - f_low_az_conf_det = true
   * - elevation <= abs(20 degrees) 
   * - range < 4.0F
   * Elevation condition NOT met for FOV edge threshold
   **/
   det_prop.f_low_az_conf_det = true;
   det_raw.raw.elevation = F360_DEG2RAD(15.0F); // Less than or equal to 20 degrees
   det_raw.raw.range = 3.0F; // Less than 4.0F
   obj_track.vcs_position.x = 10.0F; // Set object position close to avoid far coasted threshold
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches general expected value, not FOV edge.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match general threshold");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match general threshold");
}

/**
*\purpose  This test will test if proper value is returned from Calc_Range_Rate_Threshold()
           when detection has f_low_az_conf_det=true, high elevation (>20 degrees), but far range (>=4.0F).
           This should NOT trigger the FOV edge threshold.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Low_Az_Conf_Far_Range)
{
   /** \precond
   * Set detection properties:
   * - f_low_az_conf_det = true
   * - elevation > abs(20 degrees) 
   * - range >= 4.0F
   * Range condition NOT met for FOV edge threshold
   **/
   det_prop.f_low_az_conf_det = true;
   det_raw.raw.elevation = F360_DEG2RAD(25.0F); // Greater than 20 degrees
   det_raw.raw.range = 5.0F; // Greater than or equal to 4.0F
   obj_track.vcs_position.x = 10.0F; // Set object position close to avoid far coasted threshold
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches general expected value, not FOV edge.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match general threshold");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match general threshold");
}

/**
*\purpose  This test will test if proper value is returned from Calc_Range_Rate_Threshold()
           when detection has f_low_az_conf_det=false even with high elevation and close range.
           This should NOT trigger the FOV edge threshold.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Normal_Az_Conf_High_Elev_Close_Range)
{
   /** \precond
   * Set detection properties:
   * - f_low_az_conf_det = false
   * - elevation > abs(20 degrees) 
   * - range < 4.0F
   * f_low_az_conf_det condition NOT met for FOV edge threshold
   **/
   det_prop.f_low_az_conf_det = false;
   det_raw.raw.elevation = F360_DEG2RAD(25.0F); // Greater than 20 degrees
   det_raw.raw.range = 3.0F; // Less than 4.0F
   obj_track.vcs_position.x = 10.0F; // Set object position close to avoid far coasted threshold
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches general expected value, not FOV edge.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold do not match general threshold");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold do not match general threshold");
}

/**
*\purpose  This test will test Calc_Range_Rate_Threshold() where reduced range rate threshold is expected
*          for a slow-moving object (3 m/s) behind SEP and a detection ambiguous motion status.
*          Under these specific conditions, the function should return 0.75 times the general threshold.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Slow_Moving_Obj_Ambiguous_Detection)
{
   /** \precond
   * Set specific conditions to trigger the reduced threshold path:
   * - Object speed 3 m/s (within slow-moving range 2-7 m/s, too low for coasted threshold)
   * - Detection motion status ambiguous
   * - Object behind_sep_id = 1 (behind SEP)
   **/
   obj_track.speed = 3.0F; // Slow-moving object speed
   obj_track.behind_sep_id = 1; // Behind SEP
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS; // Ambiguous motion

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches the reduced expected value.
   * With ambiguous motion status and behind_sep_id = 1, the function should return
   * 0.75 times the general threshold for slow-moving objects behind SEP.
   **/
   const float32_t expected_reduced_threshold = exp_general_result * 0.75F;
   DOUBLES_EQUAL_TEXT(-expected_reduced_threshold, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold should match reduced threshold with ambiguous motion and valid sep_id");
   DOUBLES_EQUAL_TEXT(expected_reduced_threshold, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold should match reduced threshold with ambiguous motion and valid sep_id");
}

/**
*\purpose  This test verifies that speed just below the lower threshold (2.0 m/s) returns general threshold
*          even with ambiguous motion and valid SEP ID.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Speed_Below_Lower_Threshold)
{
   /** \precond
   * Set speed just below 2.0 m/s threshold with other conditions that would trigger reduced threshold
   **/
   obj_track.speed = 1.9F; // Just below lower speed threshold
   obj_track.behind_sep_id = 1; // Behind SEP
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS; // Ambiguous motion

   /** \action **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(obj_track, det_raw, det_prop, sensor, calib, host_vcs_speed, rdot_thres_lower, rdot_thres_upper);

   /** \result **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower threshold should be general when speed below 2.0 m/s");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper threshold should be general when speed below 2.0 m/s");
}

/**
*\purpose  This test verifies that speed just above the upper threshold (5.0 m/s) returns general threshold
*          even with ambiguous motion and valid SEP ID.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Speed_Above_Upper_Threshold)
{
   /** \precond
   * Set speed just above 5.0 m/s threshold with other conditions that would trigger reduced threshold
   **/
   obj_track.speed = 5.1F; // Just above upper speed threshold
   obj_track.behind_sep_id = 1; // Behind SEP
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS; // Ambiguous motion

   /** \action **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(obj_track, det_raw, det_prop, sensor, calib, host_vcs_speed, rdot_thres_lower, rdot_thres_upper);

   /** \result **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower threshold should be general when speed above 5.0 m/s");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper threshold should be general when speed above 5.0 m/s");
}

/**
*\purpose  This test verifies that for a detection with moving (non-ambiguous) motion status returns general threshold
*          even with valid speed range and valid SEP ID.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Moving_Motion_Status)
{
   /** \precond
   * Set moving motion status with other conditions that would trigger reduced threshold
   **/
   obj_track.speed = 3.0F; // Within speed range
   obj_track.behind_sep_id = 1; // Behind SEP
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING; // Moving (not ambiguous)

   /** \action **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(obj_track, det_raw, det_prop, sensor, calib, host_vcs_speed, rdot_thres_lower, rdot_thres_upper);

   /** \result **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower threshold should be general with moving motion status");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper threshold should be general with moving motion status");
}

/**
*\purpose  This test verifies that invalid SEP ID returns general threshold
*          even with valid speed range and ambiguous motion status.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Invalid_Sep_Id)
{
   /** \precond
   * Set invalid SEP ID with other conditions that would trigger reduced threshold
   **/
   obj_track.speed = 3.0F; // Within speed range
   obj_track.behind_sep_id = F360_INVALID_UNSIGNED_ID; // Not behind SEP
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS; // Ambiguous motion

   /** \action **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(obj_track, det_raw, det_prop, sensor, calib, host_vcs_speed, rdot_thres_lower, rdot_thres_upper);

   /** \result **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Lower threshold should be general with invalid SEP ID");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Upper threshold should be general with invalid SEP ID");
}



/** \defgroup  Assign_Association_Hypothesis
*  @{
*/
/** \brief
* This test group includes test of the function Assign_Association_Hypothesis() defined in
* f360_detection_association_support_functions.cpp.
**/
TEST_GROUP(f360_Test_Assign_Association_Hypothesis)
{
   // Declare common variables used within all tests in this test group.
   uint32_t number_of_valid_detections;
   float32_t det_rdot_comp_array[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   /** \setup
   * Set up a default scenario where a detection is associated to an object successfully.
   */
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      number_of_valid_detections = 1U;
      det_rdot_comp_array[0] = 1.0F;

      object_tracks[0].ndets = 0;
      object_tracks[0].id = 1;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = object_tracks[0].id;


      detection_props[0].object_track_id = object_tracks[0].id;

   }

   void Create_Detection_On_Obj_Edge_And_Assoc_To_Obj(
         F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
         const F360_Object_Track_T & object_track,
         const uint32_t det_idx)
   {
      detection_props[det_idx].vcs_position.x = object_track.bbox.Get_Center().x - object_track.bbox.Get_Length() * 0.5F;
      detection_props[det_idx].vcs_position.y = object_track.bbox.Get_Center().y - object_track.bbox.Get_Width() * 0.5F;
      detection_props[det_idx].object_track_id = object_track.id;
   }
};

/**
*\purpose Check that Assign_Association_Hypothesis() works as intended when a detection has been associated to an object correctly
*\req NA
*/
TEST(f360_Test_Assign_Association_Hypothesis, AssignAssociationHypothesis_AssocDet)
{
   /** \precond
   * Nothing needs to change from the setup in this test.
   */

   /** \action
   * Call Assign_Association_Hypothesis()
   **/
   Assign_Association_Hypothesis(number_of_valid_detections, det_rdot_comp_array, tracker_info, detection_props, object_tracks);

   /** \result
   * Check that the corresponding detection properties have been modified as expected.
   **/
   DOUBLES_EQUAL_TEXT(det_rdot_comp_array[0], detection_props[0].range_rate_compensated, F360_EPSILON, "Detection property range_rate_compensated was not set as expected.")
   CHECK_EQUAL_TEXT(true, detection_props[0].f_dealiased, "Detection property f_inlier was not set as expected.")
}

/**
*\purpose Check that Assign_Association_Hypothesis() works as intended when the slots for detection association for a track are full.
*\req NA
*/
TEST(f360_Test_Assign_Association_Hypothesis, AssignAssociationHypothesis_MaxNumDetsForTrack)
{
   /** \precond
   * Set number of valid detections to MAX_DETS_IN_OBJ_TRK + 1
   * create MAX_DETS_IN_OBJ_TRK detections on object edge that all want to associate to the obj
   * Create a detection that wants to associate to object but 1m from edge.
   * Fill range rate compensated array for the detections to 1
   * Set expected range rate compensated to 1 (for MAX_DETS_IN_OBJ_TRK best detections)
   */
   number_of_valid_detections = MAX_DETS_IN_OBJ_TRK + 1U;
   float32_t expected_rr_comp = 1.0F;

   for (uint32_t det_idx = 0U; det_idx < MAX_DETS_IN_OBJ_TRK + 1U; det_idx++)
   {
      Create_Detection_On_Obj_Edge_And_Assoc_To_Obj(detection_props, object_tracks[0], det_idx);
      det_rdot_comp_array[det_idx] = 1.0F;
   }
   detection_props[MAX_DETS_IN_OBJ_TRK].vcs_position.x += 1.0F;

   /** \action
   * Call Assign_Association_Hypothesis()
   **/
   Assign_Association_Hypothesis(number_of_valid_detections, det_rdot_comp_array, tracker_info, detection_props, object_tracks);

   /** \result
   * Check that the corresponding detection properties have been modified as expected.
   **/
   for (uint32_t det_idx = 0U; det_idx < MAX_DETS_IN_OBJ_TRK; det_idx++)
   {
      DOUBLES_EQUAL_TEXT(expected_rr_comp, detection_props[det_idx].range_rate_compensated, F360_EPSILON, "Detection property range_rate_compensated was not set as expected.")
      CHECK_TRUE_TEXT(detection_props[det_idx].f_dealiased, "Detection property f_inlier was not set as expected.")
      CHECK_EQUAL_TEXT(1, detection_props[det_idx].object_track_id, "Detection property object_track_id is not correct.")

   }
   DOUBLES_EQUAL_TEXT(0.0F, detection_props[MAX_DETS_IN_OBJ_TRK].range_rate_compensated, F360_EPSILON, "Detection property range_rate_compensated was not set as expected.")
   CHECK_FALSE_TEXT(detection_props[MAX_DETS_IN_OBJ_TRK].f_dealiased, "Detection property f_inlier was not set as expected.")
   CHECK_EQUAL_TEXT(0, detection_props[MAX_DETS_IN_OBJ_TRK].object_track_id, "Detection property object_track_id is not correct.")
}

/**
*\purpose Check that Assign_Association_Hypothesis() works as intended when the detection has not been associated to any object,
* so it's object_track_id is equal to 0.
*\req NA
*/
TEST(f360_Test_Assign_Association_Hypothesis, AssignAssociationHypothesis_NotAssocDet)
{
   /** \precond
   * Set field object_track_id to 0 for the detection.
   */
   detection_props[0].object_track_id = 0;

   /** \action
   * Call Assign_Association_Hypothesis()
   **/
   Assign_Association_Hypothesis(number_of_valid_detections, det_rdot_comp_array, tracker_info, detection_props, object_tracks);

   /** \result
   * Check that the corresponding detection properties have been modified as expected.
   **/
   DOUBLES_EQUAL_TEXT(0.0F, detection_props[0].range_rate_compensated, F360_EPSILON, "Detection property range_rate_compensated was not set as expected.")
   CHECK_EQUAL_TEXT(false, detection_props[0].f_dealiased, "Detection property f_inlier was not set as expected.")
}

/**
*\purpose  This test will test if Calc_Range_Rate_Threshold() returns the tightened range rate threshold
*          when object meets all criteria for tightened gates (slow moving object, 2-7 m/s, in front of host).
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Tightened_Gates_For_Slow_Moving_Object)
{
   /** \precond
   * Set object properties to meet all tightened gate criteria:
   * - movable_prob > 0.5
   * - speed between 2-7 m/s
   * - position in front of host (x > 0)
   * - lateral position within limits (abs(y) < 15 m)
   * - vcs_heading within limits (abs < 60 degrees)
   * - bbox orientation within limits (abs < 60 degrees)
   * - time_since_started_move > 0.1s
   * - low tangential acceleration (abs < 1.0 m/s^2)
   * - low range rate error mean (< 1.0 m/s)
   * - status set to updated (not coasted)
   **/
   obj_track.movable_prob = 0.6F;
   obj_track.speed = 5.0F;
   obj_track.vcs_position.x = 10.0F;
   obj_track.vcs_position.y = 5.0F;
   obj_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   obj_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(20.0F)));
   obj_track.time_since_started_move = 0.2F;
   obj_track.tang_accel = 0.5F;
   obj_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   obj_track.status = F360_OBJECT_STATUS_UPDATED;
   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   float32_t tightened_range_rate_threshold = Calculate_Tightened_Range_Rate_Threshold(obj_track, range_rate_score_threshold);

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that the range rate threshold matches the tightened threshold value.
   **/
   DOUBLES_EQUAL_TEXT(-tightened_range_rate_threshold, rdot_thres_lower, F360_EPSILON, "Lower range rate threshold should match tightened threshold");
   DOUBLES_EQUAL_TEXT(tightened_range_rate_threshold, rdot_thres_upper, F360_EPSILON, "Upper range rate threshold should match tightened threshold");
}

/**
*\purpose  This test verifies that Calc_Range_Rate_Threshold() does NOT apply tightened gates
*          when object speed is too high (>= 7.0 m/s) even if other criteria are met.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Speed_Too_High_For_Tightened_Gates)
{
   /** \precond
   * Set object properties to meet all criteria except speed is at the upper limit (7.0 m/s)
   **/
   obj_track.movable_prob = 0.6F;
   obj_track.speed = 7.0F;
   obj_track.vcs_position.x = 10.0F;
   obj_track.vcs_position.y = 5.0F;
   obj_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   obj_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(20.0F)));
   obj_track.time_since_started_move = 0.2F;
   obj_track.tang_accel = 0.5F;
   obj_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   obj_track.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that general threshold is used, not tightened threshold.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Should use general threshold when speed too high");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Should use general threshold when speed too high");
}

/**
*\purpose  This test verifies that Calc_Range_Rate_Threshold() does NOT apply tightened gates
*          when object speed is too low (<= 2.0 m/s) even if other criteria are met.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Speed_Too_Low_For_Tightened_Gates)
{
   /** \precond
   * Set object properties to meet all criteria except speed is at the lower limit (2.0 m/s)
   **/
   obj_track.movable_prob = 0.6F;
   obj_track.speed = 2.0F;
   obj_track.vcs_position.x = 10.0F;
   obj_track.vcs_position.y = 5.0F;
   obj_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   obj_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(20.0F)));
   obj_track.time_since_started_move = 0.2F;
   obj_track.tang_accel = 0.5F;
   obj_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   obj_track.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Check that general threshold is used, not tightened threshold.
   **/
   DOUBLES_EQUAL_TEXT(-exp_general_result, rdot_thres_lower, F360_EPSILON, "Should use general threshold when speed too low");
   DOUBLES_EQUAL_TEXT(exp_general_result, rdot_thres_upper, F360_EPSILON, "Should use general threshold when speed too low");
}

/**
*\purpose  This test verifies that tightened gates take priority over chaotic environment reduction
*          when object meets tightened gate criteria.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_Tightened_Gates_Priority_Over_Chaotic_Env)
{
   /** \precond
   * Set object properties to meet both tightened gate criteria and chaotic environment criteria
   **/
   obj_track.movable_prob = 0.6F;
   obj_track.speed = 5.0F;
   obj_track.vcs_position.x = 10.0F;
   obj_track.vcs_position.y = 5.0F;
   obj_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   obj_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(20.0F)));
   obj_track.time_since_started_move = 1.5F;
   obj_track.tang_accel = 0.5F;
   obj_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   obj_track.status = F360_OBJECT_STATUS_UPDATED;
   obj_track.assoc_dets_pct_filtered = 0.3F;

   float32_t tightened_range_rate_threshold = Calculate_Tightened_Range_Rate_Threshold(obj_track, range_rate_score_threshold);

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * Tightened gate criteria is checked before chaotic environment, so tightened threshold should be used.
   **/
   DOUBLES_EQUAL_TEXT(-tightened_range_rate_threshold, rdot_thres_lower, F360_EPSILON, "Tightened threshold should take priority");
   DOUBLES_EQUAL_TEXT(tightened_range_rate_threshold, rdot_thres_upper, F360_EPSILON, "Tightened threshold should take priority");
}

/**
*\purpose  This test verifies that FOV edge reduction takes priority over tightened gates
*          when detection is at FOV edge, even if object meets tightened gate criteria.
*\req    NA
*/
TEST(f360_Test_Calc_Range_Rate_Threshold, Test_Calc_Range_Rate_Threshold_FOV_Edge_Priority_Over_Tightened_Gates)
{
   /** \precond
   * Set object properties to meet tightened gate criteria
   * Set detection at FOV edge
   **/
   obj_track.movable_prob = 0.6F;
   obj_track.speed = 5.0F;
   obj_track.vcs_position.x = 10.0F;
   obj_track.vcs_position.y = 5.0F;
   obj_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   obj_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(20.0F)));
   obj_track.time_since_started_move = 0.2F;
   obj_track.tang_accel = 0.5F;
   obj_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   obj_track.status = F360_OBJECT_STATUS_UPDATED;

   det_prop.f_FOV_edge = true;

   /** \action
   * Call Calc_Range_Rate_Threshold
   **/
   float32_t rdot_thres_lower = 0.0F;
   float32_t rdot_thres_upper = 0.0F;
   Calc_Range_Rate_Threshold(
      obj_track,
      det_raw,
      det_prop,
      sensor,
      calib,
      host_vcs_speed,
      rdot_thres_lower,
      rdot_thres_upper);

   /** \result
   * FOV edge condition is checked first in the if-else chain, so it takes priority.
   * The FOV edge threshold should be used, not the tightened threshold.
   **/
   DOUBLES_EQUAL_TEXT(-exp_fov_edge_result, rdot_thres_lower, F360_EPSILON, "FOV edge threshold should take priority");
   DOUBLES_EQUAL_TEXT(exp_fov_edge_result, rdot_thres_upper, F360_EPSILON, "FOV edge threshold should take priority");
}

/** @}*/


/** \defgroup  f360_calculate_detection_association_cost_rear_left_corner_moveable_obj
*  @{
*/
/** \brief
* This test group includes test of the function Calculate_Detection_Association_Cost(). The tests will be based on a detection
* on rear left corner of a moveable object.
**/
TEST_GROUP(f360_calculate_detection_association_cost_rear_left_corner_moveable_obj)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Props_T det_prop = {};
   F360_Object_Track_T object_track = {};
   float32_t test_pass_threshold = 1e-5F;
   /** \setup
    * Place a moveable object with size 4x2m at (10, 10) in VCS with VCS pointing angle of 0
    * Set up a default scenario where a detection is placed on rear left object corner.
    */
   TEST_SETUP()
   {
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 10.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = object_track.vcs_position;
      object_track.bbox.Set_Center(center);
      object_track.bbox.Set_Length(4.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.movable_prob = 1.0F;


      Create_Detection_On_Obj_Edge(det_prop, object_track);

   }

   void Create_Detection_On_Obj_Edge(
         F360_Detection_Props_T & det_prop,
         const F360_Object_Track_T & object_track)
   {
      det_prop.vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length() * 0.5F;
      det_prop.vcs_position.y = object_track.vcs_position.y - object_track.bbox.Get_Width() * 0.5F;
   }
};

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended when a detection is placed on the rear left object edge.
*\req NA
*/
TEST(f360_calculate_detection_association_cost_rear_left_corner_moveable_obj, Calculate_Detection_Association_Cost_Zero_Cost)
{
   /** \precond
   * Nothing needs to change from the setup in this test.
   */
   float32_t expected_det_cost = 0.0F;

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")

}

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended when a detection is placed outside object.
*\req NA
*/
TEST(f360_calculate_detection_association_cost_rear_left_corner_moveable_obj, Calculate_Detection_Association_Cost_Detection_Orthogonal_To_Obj_Rear)
{
   /** \precond
   * A detection has been set up in test group on rear left corner of the object.
   * Move the detection 1m towards host in longitudinal direction
   */
   det_prop.vcs_position.x -= 1.0F;
   float32_t expected_det_cost = 1.0F;

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")

}

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended when a detection is placed outside object in
*         both longitudinal and lateral direction.
*\req NA
*/
TEST(f360_calculate_detection_association_cost_rear_left_corner_moveable_obj, Calculate_Detection_Association_Cost_Detection_Outside_Rear_Left_Corner)
{
   /** \precond
   * A detection has been set up in test group on rear left corner of the object.
   * Move the detection 1m towards host in both longitudinal and lateral direction
   */
   det_prop.vcs_position.x -= 1.0F;
   det_prop.vcs_position.y -= 1.0F;
   float32_t expected_det_cost = 1.41421356F;

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")

}
/** @}*/

/** \defgroup  f360_calculate_detection_association_cost_front_right_corner_moveable_obj
*  @{
*/
/** \brief
* This test group includes test of the function Calculate_Detection_Association_Cost(). The tests will be based on a detection
* on front right corner of a moveable object.
**/
TEST_GROUP(f360_calculate_detection_association_cost_front_right_corner_moveable_obj)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Props_T det_prop = {};
   F360_Object_Track_T object_track = {};
   float32_t test_pass_threshold = 1e-5F;
   /** \setup
    * Place a moveable object with size 4x2m at (10, 10) in VCS with VCS pointing angle of 0
    * Set up a default scenario where a detection is placed on front right object corner.
    */
   TEST_SETUP()
   {
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 10.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      Point center = object_track.vcs_position;
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.bbox.Set_Center(center);
      object_track.bbox.Set_Length(4.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.movable_prob = 1.0F;

      Create_Detection_On_Obj_Edge(det_prop, object_track);

   }

   void Create_Detection_On_Obj_Edge(
         F360_Detection_Props_T & det_prop,
         const F360_Object_Track_T & object_track)
   {
      det_prop.vcs_position.x = object_track.vcs_position.x + object_track.bbox.Get_Length() * 0.5F;
      det_prop.vcs_position.y = object_track.vcs_position.y + object_track.bbox.Get_Width() * 0.5F;
   }
};

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended when a detection is placed on rear right corner of object.
*\req NA
*/
TEST(f360_calculate_detection_association_cost_front_right_corner_moveable_obj, Calculate_Detection_Association_Cost_Zero_Cost)
{
   /** \precond
   * A detection has been set up in test group on front right corner of the object.
   */
   float32_t expected_det_cost = 0.0F;

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")

}

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended when a detection is placed inside the object bounding box.
*\req NA
*/
TEST(f360_calculate_detection_association_cost_front_right_corner_moveable_obj, Calculate_Detection_Association_Cost_Detection_On_Front_Edge)
{
   /** \precond
   * A detection has been set up in test group on front right corner of the object.
   * Move the detection 1m inside bounding box in the lateral direction
   */
   det_prop.vcs_position.y -= 1.0F;
   float32_t expected_det_cost = 0.0F; // Detection is on front edge, cost should be 0.

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")

}

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended when a detection is placed inside the object bounding box.
*\req NA
*/
TEST(f360_calculate_detection_association_cost_front_right_corner_moveable_obj, Calculate_Detection_Association_Cost_Detection_Inside_Object)
{
   /** \precond
   * A detection has been set up in test group on front right corner of the object.
   * Move the detection 1m inside bounding box in both longitudinal and lateral direction
   */
   det_prop.vcs_position.x -= 1.0F;
   det_prop.vcs_position.y -= 0.5F;
   float32_t expected_det_cost = 0.5F; // Distance to closest edge is 0.5m

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")

}
/** @}*/


/** \defgroup  f360_calculate_detection_association_non_moveable_obj
*  @{
*/
/** \brief
* This test group includes test of the function Calculate_Detection_Association_Cost(). The tests are testing the fucntionality
* for a non-moveable object.
**/
TEST_GROUP(f360_calculate_detection_association_non_moveable_obj)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Props_T det_prop = {};
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibs = {};
   const float32_t test_pass_threshold = 1e-5F;

   /** \setup
    * Place a non-moveable object at (10, 10) in VCS.
    * Set up a default scenario where a detection is placed at position (10-1, 10+0.5) in VCS
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 10.0F;
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.bbox.Set_Center(object_track.vcs_position);
      object_track.bbox.Set_Length(calibs.k_nonmoveable_target_diameter);
      object_track.bbox.Set_Width(calibs.k_nonmoveable_target_diameter);
      object_track.bbox.Set_Orientation(0.0F);
      object_track.movable_prob = 0.0F;

      det_prop.vcs_position.x = object_track.vcs_position.x - 1.0F;
      det_prop.vcs_position.y = object_track.vcs_position.y + 0.5F;

   }
};

/**
*\purpose Check that Calculate_Detection_Association_Cost() works as intended for a non-moveable object.
*\req NA
*/
TEST(f360_calculate_detection_association_non_moveable_obj, Calculate_Detection_Association_Cost)
{
   /** \precond
   * Test setup from test group can be used
   */

   /** \action
   * Call Calculate_Detection_Association_Cost()
   **/
   float32_t det_cost = Calculate_Detection_Association_Cost(det_prop, object_track);

   /** \result
   * Check that association cost for the detection matches the expected data.
   **/
   const  float32_t expected_det_cost = F360_Get_Hypotenuse(1.0F, 0.5F);
   DOUBLES_EQUAL_TEXT(expected_det_cost, det_cost, test_pass_threshold, "The association cost for the detection did not match the expected data.")
}
/** @}*/


/** \defgroup  f360_Test_Calc_Det_Score_Moveable_Object
*  @{
*/
/** \brief
*  Test group for testing if correct algorithm is used for score calculation and correct value is returned
**/
TEST_GROUP(f360_Test_Calc_Det_Score_Moveable_Object)
{
   F360_Detection_Props_T det_p = {};
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibs;
   float32_t range_rate_diff = 0.0F;

   /** \setup
   * One object setup - base parameters like size and position
   * One detection setup
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.speed = 6.0F;
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center(object_track.vcs_position);
      object_track.bbox.Set_Center(center);
      object_track.bbox.Set_Length(3.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track.long_buffer_zone_len2 = 1.2F;
      object_track.lat_buffer_zone_wid2 = 0.8F;
      object_track.movable_prob = 1.0F;
   }

   // Helper function to add detections on object corners
   void Add_Detection_On_Front_Right_Corner_Of_Object()
   {
      Convert_TCS_Posn_To_VCS_Posn(object_track.bbox.Get_Length() * 0.5F,
         object_track.bbox.Get_Width() * 0.5F,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         det_p.vcs_position.x,
         det_p.vcs_position.y);
   }

   void Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(float32_t offset_x, float32_t offset_y)
   {
      Convert_TCS_Posn_To_VCS_Posn(object_track.bbox.Get_Length() * 0.5F + offset_x,
         object_track.bbox.Get_Width() * 0.5F + offset_y,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         det_p.vcs_position.x,
         det_p.vcs_position.y);
   }
};

/**
*\purpose  This test will test that correct score is returned if detection is inside solid bbox
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Moveable_Object, f360_Test_Calc_Det_Score_Moveable_Object_In_Solid_BBox)
{
   /** \precond
   * Set detection's position to the front right corner of solid bounding box
   */
   Add_Detection_On_Front_Right_Corner_Of_Object();

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det_p.vcs_position, det_p.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(0.0F, score, F360_EPSILON, "Unexpected score returned")
}

/**
*\purpose  This test will test that correct score is returned if detection is only inside extended bbox
*          and is water spray
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Moveable_Object, f360_Test_Calc_Det_Score_Moveable_Object_Only_In_Extended_BBox_And_Watter_Spray)
{
   /** \precond
   * Set detection's position close to the front right corner of extended bounding box
   * Mark detection as water spray
   */
   Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(
      object_track.long_buffer_zone_len2*calibs.k_ws_bbox_len_extension_factor -0.0001F,
      object_track.lat_buffer_zone_wid2*calibs.k_ws_bbox_wid_extension_factor - 0.0001F);
   det_p.f_water_spray = true;

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det_p.vcs_position, det_p.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(0.209F, score, 0.001F, "Unexpected score returned")
}

/**
*\purpose  This test will test that correct score is returned if detection is only inside extended bbox
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Moveable_Object, f360_Test_Calc_Det_Score_Moveable_Object_Only_In_Extended_BBox)
{
   /** \precond
   * Set detection's position close to the front right corner of extended bounding box
   */
   Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(
      object_track.long_buffer_zone_len2 - 0.0001F,
      object_track.lat_buffer_zone_wid2 - 0.0001F);
   det_p.f_water_spray = false;

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det_p.vcs_position, det_p.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(0.7F, score, 0.001F, "Unexpected score returned")
}

/**
*\purpose  This test will test that correct score is returned if detection is out of extended bbox
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Moveable_Object, f360_Test_Calc_Det_Score_Moveable_Object_out_of_extended_bbox)
{
   /** \precond
   * Set detection's position to be out the extended bounding box
   */
   Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(
      object_track.long_buffer_zone_len2 + 0.0001F,
      object_track.lat_buffer_zone_wid2);

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det_p.vcs_position, det_p.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(calibs.k_score_outside_ext_bbox, score, F360_EPSILON, "Unexpected score returned")
}


/** \defgroup  f360_Test_Calc_Det_Score_Non_Moveable_Object
*  @{
*/
/** \brief
*  Test group for testing if correct algorithm is used for score calculation and correct value is returned
**/
TEST_GROUP(f360_Test_Calc_Det_Score_Non_Moveable_Object)
{
   F360_Detection_Props_T det = {};
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibs = {};
   const float32_t range_rate_diff = 0.4F;

   const float32_t test_pass_th = 1e-6F;

   /** \setup
   * Object setup:
   *    - Peference point: Center
   *    - Position: [10, 0]
   *    - BBox center: Same as object position
   *    - BBox length and with: Default non-moveable target diameter (calibs.k_nonmoveable_target_diameter)
   *    - BBox orientation: 0deg (value is not improtnt though but we want to add it anyways to fully define the object bbox)
   *    - Speed: 0.1F
   *    - movable_prob: 0.0
   *    - time_since_initialization: 1.1 (set time so that no extra score is added)
   *    - long_buffer_zone_len1, long_buffer_zone_len2, lat_buffer_zone_wid1, lat_buffer_zone_wid2: Default maximum possible asociation gate extension for non-moveable objects (taken from calibs.k_max_assoc_gate_extension_non_moveable)
   * One detection setup:
   * Position: Inside object bbox = Object position + [calibs.k_nonmoveable_target_diameter * 0.25F, 0]. (I.e straight in front of object center half the object radius distance away from the object center)
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.bbox.Set_Center(object_track.vcs_position);
      object_track.bbox.Set_Length(calibs.k_nonmoveable_target_diameter);
      object_track.bbox.Set_Width(calibs.k_nonmoveable_target_diameter);
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.speed = 0.1F;
      object_track.movable_prob = 0.0F;
      object_track.time_since_initialization = 1.1F;

      object_track.long_buffer_zone_len2 = calibs.k_max_assoc_gate_extension_non_moveable;
      object_track.lat_buffer_zone_wid2 = calibs.k_max_assoc_gate_extension_non_moveable;

      const Point det_pos_offset(calibs.k_nonmoveable_target_diameter * 0.25F, 0.0F);
      det.vcs_position.x = object_track.bbox.Get_Center().x - det_pos_offset.x;
      det.vcs_position.y = object_track.bbox.Get_Center().y - det_pos_offset.y;
   }
};

/**
*\purpose  This test will test that correct score is returned if detection is inside solid bbox
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Non_Moveable_Object, f360_Test_Calc_Det_Score_Non_Moveable_Object_In_Solid_BBox)
{
   /** \precond
   * Test setup from test group can be used (one detection inside the solid bbox of a non-moveable object)
   */

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det.vcs_position, det.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(0.2525F, score, test_pass_th, "Unexpected score returned")
}

/**
*\purpose  This test will test that correct score is returned if detection is only inside extended bbox
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Non_Moveable_Object,f360_Test_Calc_Det_Score_Non_Moveable_Object_Only_In_Extended_BBox)
{
   /** \precond
   * Use setup from test group but change the detection position to be outside of the solid bounding box but inside the extended bounding box.
   * Set detection position to object position - [0.0F, 0.8F * calibs.k_max_assoc_gate_extension_non_moveable];
   */
   const Point det_pos_offset(0.0F, 0.8F * (calibs.k_max_assoc_gate_extension_non_moveable + object_track.bbox.Get_Length()*0.5F));
   object_track.long_buffer_zone_len1 = calibs.k_max_assoc_gate_extension_non_moveable;
   det.vcs_position.x = object_track.bbox.Get_Center().x - det_pos_offset.x;
   det.vcs_position.y = object_track.bbox.Get_Center().y - det_pos_offset.y;

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det.vcs_position, det.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(0.6F, score, test_pass_th, "Unexpected score returned")
}

/**
*\purpose  This test will test that the score is correctly increased for newly created objects
*\req    NA
*/
TEST(f360_Test_Calc_Det_Score_Non_Moveable_Object,f360_Test_Calc_Det_Score_Non_Moveable_New_Object_Only_In_Extended_BBox)
{
   /** \precond
   * Use setup from test group but change the detection position to be outside of the solid bounding box but inside the extended bounding box.
   * Set detection position to object position - [0.0F, 0.8F * calibs.k_max_assoc_gate_extension_non_moveable];
   * Set object's time_since_initialization to 0.1
   */
   const Point det_pos_offset(0.0F, 0.8F * (calibs.k_max_assoc_gate_extension_non_moveable + object_track.bbox.Get_Length()*0.5F));
   object_track.long_buffer_zone_len1 = calibs.k_max_assoc_gate_extension_non_moveable;
   det.vcs_position.x = object_track.bbox.Get_Center().x - det_pos_offset.x;
   det.vcs_position.y = object_track.bbox.Get_Center().y - det_pos_offset.y;
   object_track.time_since_initialization = 0.1F;

   /** \action
   *Call Calc_Det_Score
   **/
   float32_t score = Calc_Det_Score(object_track, calibs, range_rate_diff, det.vcs_position, det.f_water_spray);

   /** \result
   * Check that correct parameters are returned
   **/
   DOUBLES_EQUAL_TEXT(1.14F, score, test_pass_th, "Unexpected score returned")
}

/** \defgroup  f360_Test_Is_Association_Wrt_SEP_Allowed
*  @{
*/
/** \brief
*  Included tests for Is_Association_Wrt_SEP_Allowed.
**/
TEST_GROUP(f360_Test_Is_Association_Wrt_SEP_Allowed)
{

   F360_Detection_Props_T det_prop = {};
   F360_Object_Track_T obj_track = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};

   /** \setup
   * Create a SEP as a straight line y = 8;
   * Set detection position to be not behind a SEP ([0, 7])
   * Create a moving CTCA object with position behind SEP ([2, 9])
   * and 0 bbox orientation.
   **/
   TEST_SETUP()
   {
      // Detection data
      det_prop.vcs_position.x = 0.0F;
      det_prop.vcs_position.y = 7.0F;
	  
      // Object data
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_track.f_moving = true;
      obj_track.f_behind_sep_ambiguous = false;
      obj_track.vcs_position.x = 2.0F;
      obj_track.vcs_position.y = 9.0F;
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = obj_track.vcs_position;
      obj_track.bbox.Set_Center(center);
      obj_track.bbox.Set_Length(6.0F);
      obj_track.bbox.Set_Width(2.0F);
      obj_track.bbox.Set_Orientation(0.0F);
	  
      // SEP
      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].p2 = 0.0F;
      sep[0].p1 = 0.0F;
      sep[0].p0 = 8.0F;
      sep[0].upper_limit = 3.0F;
      sep[0].lower_limit = -3.0F;
   }
};

/**
*\purpose  Tests that a moving CTCA object behind a SEP is not allowed
*          to associate with a detection on opposite side of the SEP
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed, Is_Association_Wrt_SEP_Allowed_Not_Allowed_Obj_Behind)
{
   /** \precond
   * A SEP has been set up
   * An object behind SEP has been set up
   * The object is moving and CTCA
   * A detection has been set up, not behind the SEP
   */

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_FALSE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should not be allowed.")
}

/**
*\purpose  Tests that a moving CTCA object not behind a SEP
*          to associate with a detection behind the SEP
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed, Is_Association_Wrt_SEP_Allowed_Not_Allowed_Det_Behind)
{
   /** \precond
   * A SEP has been set up
   * The object is moving and CTCA
   * Set object position to be in front of SEP
   * Set detection position to be behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   det_prop.vcs_position.y = 9.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_FALSE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should not be allowed.")
}

/**
*\purpose  Tests that a CTCA object not behind a SEP is allowed
*          to associate with a detection behind the SEP when the object is not flagged as moving.
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed, Is_Association_Wrt_SEP_Allowed_Obj_Not_Moving)
{
   /** \precond
   * A SEP has been set up
   * The object is CTCA
   * Set the object position to in front of SEP
   * Set object's f_moving flag is set to false.
   * Set detection position to behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.f_moving = false;
   det_prop.vcs_position.y = 6.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/**
*\purpose  Tests that a moving CTCA object not behind a SEP is allowed to
*          to associate with a detection behind the SEP when the object front is behind of SEP
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed, Is_Association_Wrt_SEP_Allowed_Obj_Ambiguous_Front_Behind)
{
   /** \precond
   * A SEP has been set up
   * Set the object position to in front of SEP
   * The object is CTCA and moving
   * Set detection position behind the SEP
   * Rotate object such that its front is behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.f_behind_sep_ambiguous = true;
   det_prop.vcs_position.y = 9.0F;
   obj_track.bbox.Set_Orientation(-F360_PI_2);

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/**
*\purpose  Tests that a moving CTCA object not behind a SEP is allowed to
*          to associate with a detection behind the SEP when the object rear is behind of SEP
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed, Is_Association_Wrt_SEP_Allowed_Obj_Ambiguous_Rear_Behind)
{
   /** \precond
   * A SEP has been set up
   * Set the object position to in front of SEP
   * The object is CTCA and moving
   * Set detection position behind the SEP
   * Rotate object such that its rear is behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.bbox.Set_Orientation(F360_PI_2);
   obj_track.f_behind_sep_ambiguous = true;
   det_prop.vcs_position.y = 9.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/**
*\purpose  Tests that a moving CTCA object behind a SEP is allowed
*          to associate with a detection behind the SEP.
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed, Is_Association_Wrt_SEP_Allowed_Obj_And_Det_Behind_SEP)
{
   /** \precond
   * A SEP has been set up
   * An object has been set up and its position is set to behind the SEP
   * The object is CTCA and moving
   * Set the detection position to behind the SEP
   */
   det_prop.vcs_position.y = 9.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/** @}*/



//----------------------------- CCA ------------------------------------


/** \defgroup  f360_Test_Is_Association_Wrt_SEP_Allowed_CCA
*  @{
*/
/** \brief
*  Included tests for Is_Association_Wrt_SEP_Allowed_CCA.
**/
TEST_GROUP(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA)
{

   F360_Detection_Props_T det_prop = {};
   F360_Object_Track_T obj_track = {};
  Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};

   /** \setup
   * Set detection not behind a SEP
   * Create a moving CCA object behind a SEP
   * Set object position to (2, 8)
   * Create a sep in front of object but behind the detection as a straight line y = 7 and x limit -3m to 3m
   **/
   TEST_SETUP()
   {
      // Detection data
      det_prop.behind_sep_id = F360_INVALID_UNSIGNED_ID;
      det_prop.vcs_position.x = 0.0F;
      det_prop.vcs_position.y = 6.0F;
	  
      // Object data
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      obj_track.f_moving = true;
      obj_track.behind_sep_id = 1U;
      obj_track.f_behind_sep_ambiguous = false;
      obj_track.vcs_position.x = 2.0F;
      obj_track.vcs_position.y = 8.0F;
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = obj_track.vcs_position;
      obj_track.bbox.Set_Center(center);
      obj_track.bbox.Set_Length(6.0F);
      obj_track.bbox.Set_Width(2.0F);
      obj_track.bbox.Set_Orientation(0.0F);
	  
      // SEP association box data
      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].p2 = 0.0F;
      sep[0].p1 = 0.0F;
      sep[0].p0 = 7.0F;
      sep[0].upper_limit = 3.0F;
      sep[0].lower_limit = -3.0F;
   }
};

/**
*\purpose  Tests that a moving CCA object behind a SEP and within its SEP bounding box is not allowed
*          to associate with a detection on opposite side of the SEP
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA, Is_Association_Wrt_SEP_Allowed_Not_Allowed_Obj_Behind)
{
   /** \precond
   * A SEP association has been set up
   * An object has been set up and its position is set up to be behind the SEP
   * The object is moving and CCA 
   * A detection has been set up with position in front of the SEP
   */

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_FALSE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should not be allowed.")
}

/**
*\purpose  Tests that a moving CCA object not behind a SEP is not allowed
*          to associate with a detection behind the SEP
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA, Is_Association_Wrt_SEP_Allowed_Not_Allowed_Det_Behind)
{
   /** \precond
   * A SEP association box has been set up
   * Set object position to be in front of the SEP
   * The object is moving and CCA
   * Set the detection position to behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   det_prop.vcs_position.y = 8.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_FALSE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should not be allowed.")
}

/**
*\purpose  Tests that a CCA object not behind a SEP and within its SEP bounding box is allowed
*          to associate with a detection behind the SEP when the object is not flagged as moving.
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA, Is_Association_Wrt_SEP_Allowed_Obj_Not_Moving)
{
   /** \precond
   * A SEP association has been set up
   * Set object position to in front of the SEP
   * Set detection position to behind the SEP
   * Object's f_moving flag is set to false.
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.f_moving = false;
   det_prop.vcs_position.y = 8.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/**
*\purpose  Tests that a moving CCA object not behind a SEP is allowed
*          to associate with a detection behind the SEP when the object
*          front is behind the SEP.
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA, Is_Association_Wrt_SEP_Allowed_Obj_Front_Behind)
{
   /** \precond
   * A SEP has been set up
   * Set object position to in front of the SEP
   * The object is CCA and moving
   * Set detection position to behind the SEP
   * Rotate object bounding box such that its front is behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.bbox.Set_Orientation(F360_PI_2);
   det_prop.vcs_position.y = 8.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/**
*\purpose  Tests that a moving CCA object not behind a SEP is allowed
*          to associate with a detection behind the SEP when the object
*          rear is behind the SEP.
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA, Is_Association_Wrt_SEP_Allowed_Obj_Rear_Behind)
{
   /** \precond
   * A SEP has been set up
   * Set object position to in front of the SEP
   * The object is CCA and moving
   * Set detection position to behind the SEP
   * Rotate object bounding box such that its rear is behind the SEP
   */
   obj_track.vcs_position.y = 6.0F;
   obj_track.Update_Bbox_Center();
   obj_track.bbox.Set_Orientation(-F360_PI_2);
   det_prop.vcs_position.y = 8.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/**
*\purpose  Tests that a moving CCA object behind a SEP is allowed
*          to associate with a detection behind the SEP.
*\req    NA
*/
TEST(f360_Test_Is_Association_Wrt_SEP_Allowed_CCA, Is_Association_Wrt_SEP_Allowed_Obj_And_Det_Behind_SEP)
{
   /** \precond
   * A SEP has been set up
   * An object has been set up and its position is set up behind the SEP
   * The object is CCA and moving
   * Set detection position to be behind the SEP
   */
   det_prop.vcs_position.y = 8.0F;

   /** \action
   *Call Is_Association_Wrt_SEP_Allowed
   **/
   const bool f_association_allowed_wrt_SEP = Is_Association_Wrt_SEP_Allowed(det_prop, obj_track, sep);

   /** \result
   * Check that f_association_allowed_wrt_SEP is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_association_allowed_wrt_SEP, "Association w.r.t. SEP should be allowed.")
}

/** @}*/

/** \defgroup  f360_Compare_Against_Stationary_Hypothesis
*  @{
*/
/** \brief
*  Testing a dealiasing strategy based on comparing a moving object and
*  an imaginary stationary object in its place.
**/
TEST_GROUP(f360_Compare_Against_Stationary_Hypothesis)
{

   F360_Detection_Props_T det_prop = {};
   rspp_variant_A::RSPP_Detection_T detection = {};
   F360_Object_Track_T obj_track = {};
   F360_Calibrations_T calib = {};
   F360_Radar_Sensor_T sensor = {};

   float32_t range_rate_threshold_lower = -2.0F;
   float32_t range_rate_threshold_upper = 2.0F;
   
   /** \setup
   * The host is traveling straight ahead with rr = 30 m/s
   * The object in front of it travels with rr = 30.5 m/s or
   * is stationary.
   *
   * Dealiasing interval: 30 m/s
   * Minimun of the dealiasing range: -23 m/s
   *
   * Measured rr = 0.4 m/s
   *
   * Imaginary stationary object:
   * Predicted rr = -30 m/s
   * De-aliased rr = -29.6 m/s
   * RR diff = 0.4 m/s
   **/
   TEST_SETUP()
   {
      // Detection data
      detection.processed.cos_vcs_az = 1;
      detection.processed.sin_vcs_az = 0;
      // measured range rate
      detection.raw.range_rate = 0.4F;

      // sensor data
      sensor.variable.look_id = F360_DET_LOOK_ID_0;
      sensor.constant.v_wrapping[sensor.variable.look_id] = 30.0F;
      sensor.constant.min_aliaised_range_rate[sensor.variable.look_id] = -23.0F;
      // host parameters
      sensor.variable.vcs_velocity.longitudinal = 30.0F;
      sensor.variable.vcs_velocity.lateral = 0.0F;

      Initialize_Tracker_Calibrations(calib);

   }
};

/**
*\purpose   Test that the function returns true when the moving object's range rate fits the de-aliased range rate
*           significantly better than that of the hypothetical stationary object.
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Moving_Selected)
{
   /** \precond
    *
    * Moving object:
    * Predicted rr = 0.5 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 0.1 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 0.4 m/s
   */

   const float32_t predicted_range_rate = 0.5F;
   const float32_t dealiased_range_rate = 0.4F;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "True".
   * In other words the moving object has smaller rr error compared to the stationary object and the detection
   * is associated to the former.
   **/
   CHECK_TRUE(f_moving_hypothesis_significantly_better_than_stat);
}

/**
*\purpose   Test the case when the stationary object has a smaller rr error. The function should returns False
*           since moving hypothesis has higher range rate error and the detection should not be assigned to the
*           moving object.
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Stationary_Selected)
{
   /** \precond
    *
    * The object in front of it is stationary
    *
    * Moving object:
    * Predicted rr = 1.5 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 1.1 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 0.4 m/s
   */

   const float32_t predicted_range_rate = 1.5F;
   const float32_t dealiased_range_rate = 0.4F;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "False".
   * In other words the stationary object has smaller rr error compared to the moving object and the detection
   * is associated to the former.
   **/
   CHECK_FALSE(f_moving_hypothesis_significantly_better_than_stat);
}

/**
*\purpose   Test the case when the difference between the stationary and moving objects is smaller than
*           threshold defined in calibrations. The stationary hypothesis needs to be selected as default.
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Small_Diff_Select_Default)
{
   /** \precond
    *
    * The host is traveling straight ahead with rr = 30 m/s
    * The object in front of it is stationary
    *
    * Moving object:
    * Predicted rr = 0.7 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 0.3 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 0.4 m/s
   */

   const float32_t predicted_range_rate = 0.7F;
   const float32_t dealiased_range_rate = 0.4F;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "False".
   **/
   CHECK_FALSE(f_moving_hypothesis_significantly_better_than_stat);
}

/**
*\purpose   This test checks that the function returns false when the moving objects range rate diff is smaller than the stationary,
            but not significantly smaller, i.e. the difference is below the threhsold
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Fail_Moving_Better_And_Diff)
{
   /** \precond
    *
    * The host is traveling straight ahead with rr = 30.6 m/s
    * The object in front of it is stationary
    *
    * Moving object:
    * Predicted rr = 1.5 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 1.1 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30.6 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 1.0 m/s
   */

   const float32_t predicted_range_rate = 1.5F;
   const float32_t dealiased_range_rate = 0.4F;

   sensor.variable.vcs_velocity.longitudinal = 30.6F;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "False".
   * Both
   **/
   CHECK_FALSE(f_moving_hypothesis_significantly_better_than_stat);
}

/**
*\purpose   This test checks that the function returns true when the moving objects range rate diff is smaller than the stationary,
            but not significantly smaller, i.e. the difference is below the threhsold and the detections is inside moving gate.
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Pass_Moving_Better_And_Diff_Within_Gate)
{
   /** \precond
    *
    * The host is traveling straight ahead with rr = 30.6 m/s
    * The object in front of it is stationary
    * Detection is inside the reduced extension gate of the moving object
    *
    * Moving object:
    * Predicted rr = 0.9 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 1.1 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30.6 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 1.0 m/s
    */

   const float32_t predicted_range_rate = 0.9F;
   const float32_t dealiased_range_rate = 0.4F;
   det_prop.f_inside_mov_gate = true;
   sensor.variable.vcs_velocity.longitudinal = 30.6F;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "True".
   *
   **/
   CHECK_TRUE(f_moving_hypothesis_significantly_better_than_stat);
}

/**
*\purpose   This tests that the function returns true when the range rate difference
*           for the stationary objects is outside of the range rate threshold, i.e. it doesn't fit well enough to be considered
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Fail_Inside_Gate)
{
   /** \precond
    *
    * Moving object:
    * Predicted rr = 0.5 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 0.1 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 0.4 m/s
   */

   const float32_t predicted_range_rate = 0.5F;
   const float32_t dealiased_range_rate = 0.4F;

   //The range rate threshold is small enough that the stationary object is not inside the range rate gate
   range_rate_threshold_lower = -0.2F;
   range_rate_threshold_upper = 0.2F;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "True".
   *
   **/
   CHECK_TRUE(f_moving_hypothesis_significantly_better_than_stat);
}


/**
*\purpose   Test that the function returns true when the moving object's range rate fits the de-aliased range rate
*           significantly better than that of the hypothetical stationary object.
*\req    NA
*/
TEST(f360_Compare_Against_Stationary_Hypothesis, Save_Coverage)
{
   /** \precond
    *
    * Moving object:
    * Predicted rr = 0.5 m/s
    * De-aliased rr = 0.4 m/s
    * RR diff = 0.1 m/s
    *
    * Imaginary stationary object:
    * Predicted rr = -30 m/s
    * De-aliased rr = -29.6 m/s
    * RR diff = 0.4 m/s
   */

   const float32_t predicted_range_rate = 0.5F;
   const float32_t dealiased_range_rate = 0.15F;
   det_prop.f_inside_mov_gate = true;

   /** \action
   *
   *  Call Compare_Against_Stationary_Hypothesis
   *
   **/
   const bool f_moving_hypothesis_significantly_better_than_stat = Compare_Against_Stationary_Hypothesis(
      calib,
      sensor,
      detection,
      range_rate_threshold_lower,
      range_rate_threshold_upper,
      predicted_range_rate,
      dealiased_range_rate,
      det_prop
   );

   /** \result
   * Check that f_moving_hypothesis_significantly_better_than_stat is set to the correct value i.e. "True".
   * In other words the moving object has smaller rr error compared to the stationary object and the detection
   * is associated to the former.
   **/
   CHECK_TRUE(f_moving_hypothesis_significantly_better_than_stat);
   CHECK_TRUE(det_prop.f_rr_ambiguity);
}

/** @}*/

/** \defgroup  DetectionPositionScoreTests
 *  @{
 */

 /** \brief
  * Test group for testing association score for various positions of detections inside the bbox and outside the bbox
  */

TEST_GROUP(DetectionPositionScoreTests)
{
    F360_Object_Track_T object_track;
    F360_Calibrations_T calib;
    float32_t length;
    float32_t width;
    float long_buffer_zone_len1;
    float long_buffer_zone_len2;
    float lat_buffer_zone_wid1;
    float lat_buffer_zone_wid2;
   // Set up calibration parameters


    TEST_SETUP() {
        // Setup a default object_track and calibration settings
        Point center = Point(0, 0);
        length = 4.0F;
        width = 2.0F;
        long_buffer_zone_len1 = 2;
        long_buffer_zone_len2 = 3;
        lat_buffer_zone_wid1 = 1;
        lat_buffer_zone_wid2 = 2;

        Angle orientation = Angle(0.785398);
        object_track.long_buffer_zone_len1 = long_buffer_zone_len1;
        object_track.long_buffer_zone_len2 = long_buffer_zone_len2;
        object_track.lat_buffer_zone_wid1 = lat_buffer_zone_wid1;
        object_track.lat_buffer_zone_wid2 = lat_buffer_zone_wid2;

        object_track.bbox = BoundingBox(center, length, width, orientation);

        calib.k_base_score_bbox_center = 0.75F;


    }

    void teardown() {
        // Teardown actions if necessary
    }
};

/** \purpose
 * Test the score calculation when detection is inside the bbox, positioned 
 * below the center of the bbox and its score is based on its
 * distance along TCS X to the bottom edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Below_BBox_Center) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection below the center of the bbox in VCS.
     */
     Point detectionPosition_VCS(-1.131, -1.414);
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(detectionPosition_VCS, object_track, calib);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X to the bottom edge of the bbox
     */
    Point detectionPosition_TCS(-1.8, -0.2);
    float32_t half_length = 0.5*length;
    float32_t dist_to_center = std::abs(detectionPosition_TCS.x);
    float32_t expectedScore = calib.k_base_score_bbox_center*(1 - (dist_to_center/(half_length)));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is inside the bbox, positioned 
 * above the center of the bbox and its score is based on its
 * distance along TCS X to the top edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Above_BBox_Center) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection above the center of the bbox in VCS.
     */
    Point detectionPosition_VCS(1.414, 1.131);

    /** \action
     * Call Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(detectionPosition_VCS, object_track, calib);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X to the top edge of the bbox
     */
    Point detectionPosition_TCS(1.8, -0.2);
    float32_t half_length = 0.5*length;
    float32_t dist_to_center = std::abs(detectionPosition_TCS.x);
    float32_t expectedScore = calib.k_base_score_bbox_center*(1 - (dist_to_center/(half_length)));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is inside the bbox, positioned 
 * to the leftt of the center of the bbox and its score is based on its
 * distance along TCS Y to the left edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Left_of_BBox_Center) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection to the left of the center of the bbox in VCS.
     */
    Point detectionPosition_VCS(-0.0707, -1.202);


    /** \action
     * Call Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(detectionPosition_VCS, object_track, calib);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y to the closest edge parallel to TCS X
     */
    Point detectionPosition_TCS(-0.9, -0.8);
    float32_t dist_to_center = std::abs(detectionPosition_TCS.y); // this is 0.8
    float32_t expectedScore = calib.k_base_score_bbox_center*(1 - (dist_to_center/(0.5*width)));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is inside the bbox, positioned 
 * to the right of the center of the bbox and its score is based on its
 * distance along TCS Y to the right edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Right_of_BBox_Center) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection to the right of the center of the bbox in VCS.
     */
    Point detectionPosition_VCS(-1.202, -0.0707);
    Point detectionPosition_TCS(-0.9, 0.8);
    float32_t dist_to_center = std::abs(detectionPosition_TCS.y); // this is 0.8

    /** \action
     * Call Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(detectionPosition_VCS, object_track, calib);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y to the right edge of the bbox
     */
    float32_t expectedScore = calib.k_base_score_bbox_center*(1 - (dist_to_center/(0.5*width)));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned below the bottom edge of the 
 * bbox and its score is based on its distance along TCS X to the
 * bottom edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Below_BBox_Edge) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection below the bottom edge of the bbox.
     */
    Point detectionPosition_VCS(-1.9798, -2.26274);
    Point detectionPosition_TCS(-3, -0.2);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.x)-0.5*length; // this is 3-2=1
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X from the bottom edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(long_buffer_zone_len1));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}


/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned above the top edge of the 
 * bbox and its score is based on its distance along TCS X to the
 * top edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Above_BBox_Edge) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection above the top edge of the bbox.
     */
    Point detectionPosition_VCS(2.2627, 1.9798);
    Point detectionPosition_TCS(3, -0.2);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.x)-0.5*length; // this is 3-2=1

    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X from the top edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(long_buffer_zone_len2));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned to the left of the left edge of the 
 * bbox and its score is based on its distance along TCS Y to the
 * left edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Left_Of_BBox_Edge) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection left of the left edge of the bbox.
     */
    Point detectionPosition_VCS(1.697, 0);
    Point detectionPosition_TCS(1.2, -1.2);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.y)-0.5*width; // this is 1.2-1=0.2
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y from the left edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(lat_buffer_zone_wid1));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned to the right of the right edge of the 
 * bbox and its score is based on its distance along TCS Y to the
 * right edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Right_Of_BBox_Edge) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection right of the right edge of the bbox.
     */
    Point detectionPosition_VCS(0, 1.697);
    Point detectionPosition_TCS(1.2, 1.2);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.y)-0.5*width; // this is 1.2-1=0.2
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

    /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y from the right edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(lat_buffer_zone_wid2));
    DOUBLES_EQUAL(expectedScore, score, 0.0001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the bottom left vertex
 * of the bbox and its score is based on its distance along TCS Y to the
 * left edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Bottom_Left_of_BBox_Vertex_normalized_along_width) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the bottom left vertex of bbox.
     */
    Point detectionPosition_VCS(-0.494, -2.616);
    Point detectionPosition_TCS(-2.2, -1.5);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.y)-0.5*width; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y from the left edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(lat_buffer_zone_wid1));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the bottom left vertex
 * of the bbox and its score is based on its distance along TCS X to the
 * bottom edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Bottom_Left_of_BBox_Vertex_normalized_along_length) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the bottom left vertex of bbox.
     */
    Point detectionPosition_VCS(-1.0606, -2.7577);
    Point detectionPosition_TCS(-2.7, -1.2);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.x)-0.5*length; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);
     
     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X from the bottom edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(long_buffer_zone_len1));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the top left vertex
 * of the bbox and its score is based on its distance along TCS Y to the
 * left edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Top_Left_of_BBox_Vertex_normalized_along_width) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the top left vertex of bbox.
     */
    Point detectionPosition_VCS(2.616, 0.494);
    Point detectionPosition_TCS(2.2, -1.5);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.y)-0.5*width; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y from the left edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(lat_buffer_zone_wid1));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the top left vertex
 * of the bbox and its score is based on its distance along TCS X to the
 * top edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Top_Left_of_BBox_Vertex_normalized_along_length) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the top left vertex of bbox.
     */
    Point detectionPosition_VCS(2.7577, 1.0606);
    Point detectionPosition_TCS(2.7, -1.2);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.x)-0.5*length; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X from the top edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(long_buffer_zone_len2));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the bottom right vertex
 * of the bbox and its score is based on its distance along TCS Y to the
 * right edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Bottom_Right_of_BBox_Vertex_normalized_along_width) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the bottom right vertex of bbox.
     */
    Point detectionPosition_VCS(-3.32340, 0.212);
    Point detectionPosition_TCS(-2.2, 2.5);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.y)-0.5*width; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y from the right edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(lat_buffer_zone_wid2));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the bottom right vertex
 * of the bbox and its score is based on its distance along TCS X to the
 * bottom edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Bottom_Right_of_BBox_Vertex_normalized_along_length) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the bottom right vertex of bbox.
     */
    Point detectionPosition_VCS(-3.04, -0.777);
    Point detectionPosition_TCS(-2.7, 1.6);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.x)-0.5*length; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);

     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X from the bottom edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(long_buffer_zone_len1));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the top right vertex
 * of the bbox and its score is based on its distance along TCS Y to the
 * right edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Top_Right_of_BBox_Vertex_normalized_along_width) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the top right vertex of bbox.
     */
    Point detectionPosition_VCS(-0.2121, 3.323);
    Point detectionPosition_TCS(2.2, 2.5);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.y)-0.5*width; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);
     
     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS Y from the right edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(lat_buffer_zone_wid2));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}

/** \purpose
 * Test the score calculation when detection is outside the bbox and
 * inside the extended bbox, positioned diagonal to the top right vertex
 * of the bbox and its score is based on its distance along TCS X to the
 * bottom edge of the bbox
 * \req
 * NA.
 */
TEST(DetectionPositionScoreTests, DetectionAlong_Top_Right_of_BBox_Vertex_normalized_along_length) {
    /** \precond
     * Bounding box oriented 45 degrees to the VCS X axis and calibrations set in setup.
     * Position detection diaginal to the top right vertex of bbox.
     */
    Point detectionPosition_VCS(0.989,2.828);
    Point detectionPosition_TCS(2.7, 1.3);
    float32_t closest_dist_to_edge = abs(detectionPosition_TCS.x)-0.5*length; // this
    /** \action
     * Call Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox function and save result.
     */
    float32_t score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(detectionPosition_VCS, object_track);
     
     /** \result
     * Expect a score based on the distance of the detection 
     * measured along TCS X from the bottom edge of the bbox
     */
    float32_t expectedScore = (closest_dist_to_edge/(long_buffer_zone_len2));
    DOUBLES_EQUAL(expectedScore, score, 0.001);
}
/** @}*/

/** \defgroup  f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed
*  @{
*/
/** \brief
*  Included tests for Is_Low_Az_Conf_At_Boundaries_Association_Allowed.
**/
TEST_GROUP(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed)
{
 
   F360_Detection_Props_T det_prop = {};
   F360_Object_Track_T obj_track = {};
   F360_Calibrations_T calibs = {};
 
   /** \setup
   * Set Object position close to the host for Is_Low_Az_Conf_At_Boundaries_Association_Allowed function. 
   * Set object position to (1.5, 2.2).
   * Detection need to be f_low_az_conf and position of detection need to be in selected zone of object 
   * for Is_Low_Az_Conf_At_Boundaries_Association_Allowed().
   * Set detection position to (3.0, 1.0).
   **/
   TEST_SETUP()
   {
      // Detection data
      det_prop.vcs_position.x = 3.0F;
      det_prop.vcs_position.y = 1.0F;
      det_prop.f_low_az_conf_det = true;
 
      // Object data
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_track.f_moving = true;
      obj_track.vcs_position.x = 1.5F;
      obj_track.vcs_position.y = 2.2F;
      obj_track.vcs_heading = Angle{ 0.0F };
      obj_track.bbox.Set_Orientation(0.0F);
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = obj_track.vcs_position;
      obj_track.bbox.Set_Center(center);
      obj_track.bbox.Set_Width(1.6F);
      obj_track.bbox.Set_Length(4.5F);
      obj_track.lat_buffer_zone_wid1 = 0.6F;
      obj_track.lat_buffer_zone_wid2 = 0.6F;
      obj_track.long_buffer_zone_len2 = 1.4F;

      // Calibrations
      Initialize_Tracker_Calibrations(calibs);
   }
};
 
/**
*\purpose  Tests that an object does not associate detection with low_az_conf close to the host from right side
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Low_Az_Conf_Detection_Allowed_Close_To_Host_Right)
{
   /** \precond
   * as in test setup
   */
 
   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_FALSE_TEXT(f_low_az_conf_for_close_obj, "Association to low az conf detection should not be allowed.")
}
 
/**
*\purpose  Tests that an object not associate detection with low_az_conf close to the host from left side
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Low_Az_Conf_Detection_Allowed_Close_To_Host_Left)
{
   /** \precond
   * as in test setup but change position of detection and object to be at left of host
   */
   det_prop.vcs_position.y = -1.0F;
   obj_track.vcs_position.y = -2.2F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_FALSE_TEXT(f_low_az_conf_for_close_obj, "Association to low az conf detection should not be allowed.")
}

/**
*\purpose  Tests that an object associate detection with no low_az_conf close to the host, detection is inside of excluded zone.
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_No_Low_Az_Conf_Detection_Allowed_Close_To_Host_Right)
{
   /** \precond
   * as in test setup but set the f_low_az_conf_det to false
   */
   det_prop.f_low_az_conf_det = false;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Association to not low az conf detection should be allowed.")
}

/**
*\purpose  Tests that an object is too far away from host in longi
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Object_Too_Far_Away)
{
   /** \precond
   * as in test setup but set x position of object far from host in longi further than 3m
   * Max longitudal threshold is 3.0m
   */
   obj_track.vcs_position.x = 5.0F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Object should not try to associate, but the detection should be allowed")
}

/**
*\purpose  Tests that a function not allow objects with too big heading angle disassociate
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Object_With_Too_Big_Heading_Angle_Allowed)
{
   /** \precond
   * as in test setup but set the angle of heading to more than in calibs threshold, more than 0.1 rad
   */
   obj_track.vcs_heading = Angle{ 0.8F };

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Association should be allowed.")
}

/**
*\purpose  Tests that a detection under selected zone will be associated
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Detections_Under_Selected_Zone_Associated)
{
   /** \precond
   * as in test setup but set x position of detection below selected zone 
   */
   det_prop.vcs_position.x = -3.0F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Association should be allowed.")
}

/**
*\purpose  Tests that a detection outside selected zone will be associated
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Detections_Outside_Selected_Zone_Associated)
{
   /** \precond
   * as in test setup but set x position of detection below selected zone 
   */
   det_prop.vcs_position.x = -1.0F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Association should be allowed.")
}

/**
*\purpose  Tests that a detection outside selected zone will be associated
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Detections_Outside_Selected_Zone_Associated_2)
{
   /** \precond
   * as in test setup but set x position of detection below selected zone 
   */
   det_prop.vcs_position.x = 6.0F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Association should be allowed.")
}

/**
*\purpose  Tests that a detection outside selected zone
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Detections_Outside_Selected_Zone_Associated_3)
{
   /** \precond
   * as in test setup but set y position of detection below selected zone 
   */
   det_prop.vcs_position.y = 0.0F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Detection is outside of association gates.")
}

/**
*\purpose  Tests that a detection outside selected zone
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Detections_Outside_Selected_Zone_Associated_4)
{
   /** \precond
   * as in test setup but set y position of detection below selected zone 
   */
   det_prop.vcs_position.y = 3.0F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Detection is outside of association gates.")
}

/**
*\purpose  Tests that a detection outside selected zone will be associated
*\req    NA
*/
TEST(f360_Test_Is_Low_Az_Conf_At_Boundaries_Association_Allowed, Is_Detections_Outside_Selected_Zone_Associated_5)
{
   /** \precond
   * as in test setup but set y position of detection below selected zone 
   */
   det_prop.vcs_position.y = 2.4F;

   /** \action
   *Call Is_Low_Az_Conf_At_Boundaries_Association_Allowed
   **/
   const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_prop, obj_track, calibs.rp_max_abs_pointing_disagreement);
 
   /** \result
   * Check that f_low_az_conf_for_close_obj is set to the correct value.
   **/
   CHECK_TRUE_TEXT(f_low_az_conf_for_close_obj, "Detection is outside of excluded zone.")
}

/** @}*/

/** \defgroup  f360_Test_Deassociate_Detection
*  @{
*/
/** \brief
*  Test group for testing the Deassociate_Detection function
**/
TEST_GROUP(f360_Test_Deassociate_Detection)
{
   F360_Detection_Props_T det_prop = {};
   rspp_variant_A::RSPP_Detection_T detection = {};
   F360_Radar_Sensor_T sensor = {};
   const float32_t expected_vcs_pos_x = 15.0F;
   const float32_t expected_vcs_pos_y = 7.5F;
   const float32_t expected_range_rate_compensated = 20.0F;

   /** \setup
   * Setup test with a detection that is associated to an object
   **/
   TEST_SETUP()
   {
      // Initialize detection properties with associated state
      det_prop.object_track_id = 5;  // Associated to object track 5
      det_prop.f_dealiased = true;   // Detection is dealiased
      det_prop.vcs_position.x = 10.0F;
      det_prop.vcs_position.y = 5.0F;
      det_prop.range_dealiased = 20.0F;
      det_prop.range_rate_dealiased = 2.5F;
      det_prop.range_rate_compensated = 2.0F;

      detection.processed.cos_vcs_az = 0.15F;
      detection.processed.sin_vcs_az = 0.075F;

      // Raw sensor properties (before dealiasing)
      detection.raw.range = 100.0F;
      detection.raw.range_rate = 20.0F;
      detection.processed.vcs_position_x = expected_vcs_pos_x;
      detection.processed.vcs_position_y = expected_vcs_pos_y;

      // Set properties so that they will be reset to the raw values after deassociation
      sensor.variable.vcs_velocity.lateral = 0.01F; // Some small lateral velocity
      const float32_t expected_rdot_pred = detection.raw.range_rate - expected_range_rate_compensated;
      sensor.variable.vcs_velocity.longitudinal =
          -(expected_rdot_pred + sensor.variable.vcs_velocity.lateral * detection.processed.sin_vcs_az) /
          detection.processed.cos_vcs_az;
   }
};

/**
*\purpose  This test verifies that Deassociate_Detection() correctly resets the object_track_id to 0
*          and sets f_dealiased to false for an associated detection.
*\req    NA
*/
TEST(f360_Test_Deassociate_Detection, Test_Deassociate_Detection_Associated_Detection)
{
   /** \precond
   * Detection is associated to object track 5, is dealiased and have range, range rate set
   */
   CHECK_EQUAL(5, det_prop.object_track_id);
   CHECK_TRUE(det_prop.f_dealiased);
   CHECK_EQUAL(20.0, det_prop.range_dealiased);
   CHECK_EQUAL(2.5, det_prop.range_rate_dealiased);
   CHECK_EQUAL(2.0, det_prop.range_rate_compensated);
   CHECK_EQUAL(10.0F, det_prop.vcs_position.x);
   CHECK_EQUAL(5.0F, det_prop.vcs_position.y);

   /** \action
   * Call Deassociate_Detection
   **/
   Deassociate_Detection(detection, sensor, det_prop);

   /** \result
   * Check that object_track_id is reset to 0, f_dealiased is set to false and range, range rate is reset
   **/
   CHECK_EQUAL(0, det_prop.object_track_id);
   CHECK_FALSE(det_prop.f_dealiased);
   DOUBLES_EQUAL(detection.raw.range, det_prop.range_dealiased, 1e-6);
   DOUBLES_EQUAL(detection.raw.range_rate, det_prop.range_rate_dealiased, 1e-6);
   DOUBLES_EQUAL(expected_range_rate_compensated, det_prop.range_rate_compensated, 1e-6);
   DOUBLES_EQUAL(expected_vcs_pos_x, det_prop.vcs_position.x, 1e-6);
   DOUBLES_EQUAL(expected_vcs_pos_y, det_prop.vcs_position.y, 1e-6);
}

/** @}*/

/** \defgroup  f360_Test_Is_Object_Valid_For_Tightened_Gates
*  @{
*/
/** \brief
*  Included tests related to validation of slow moving objects for tightened association gates
**/
TEST_GROUP(f360_Test_Is_Object_Valid_For_Tightened_Gates)
{
   F360_Object_Track_T object_track = {};

   /** \setup
   * Setup test with default object properties that would pass the tightened gate criteria
   **/
   TEST_SETUP()
   {
      object_track.movable_prob = 0.6F;
      object_track.speed = 5.0F;
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 5.0F;
      object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
      object_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(20.0F)));
      object_track.time_since_started_move = 0.2F;
      object_track.tang_accel = 0.5F;
      object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   }
};

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns true
*          when all criteria are met for a valid slow-moving object.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Valid_Slow_Moving_Object)
{
   /** \precond
   * Object has movable_prob > 0.5, speed between 2-7 m/s, position and heading within limits,
   * time_since_started_move > 0.1s, low tangential acceleration and low range rate error mean
   */

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return true
   **/
   CHECK_TRUE_TEXT(is_valid, "Object should be valid for tightened gates");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when movable_prob is too low.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Low_Movable_Prob)
{
   /** \precond
   * Set movable_prob to 0.5 (not greater than 0.5)
   */
   object_track.movable_prob = 0.5F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with movable_prob = 0.5 should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object speed is too high.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Speed_Too_High)
{
   /** \precond
   * Set speed to 7.0 m/s (equal to upper limit, should fail)
   */
   object_track.speed = 7.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with speed 7.0 m/s should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object speed is too low.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Speed_Too_Low)
{
   /** \precond
   * Set speed to 2.0 m/s (equal to lower limit, should fail)
   */
   object_track.speed = 2.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with speed 2.0 m/s should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object lateral position is too large positive.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Lateral_Position_Too_Large_Positive)
{
   /** \precond
   * Set lateral position to 15.0 m (equal to limit, should fail)
   */
   object_track.vcs_position.y = 15.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with lateral position 15.0 m should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object lateral position is too large negative.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Lateral_Position_Too_Large_Negative)
{
   /** \precond
   * Set lateral position to -15.0 m (equal to limit, should fail)
   */
   object_track.vcs_position.y = -15.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with lateral position -15.0 m should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object VCS heading is too large positive.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_VCS_Heading_Too_Large_Positive)
{
   /** \precond
   * Set VCS heading to 60 degrees (equal to limit, should fail)
   */
   object_track.vcs_heading = Angle(F360_DEG2RAD(60.0F));

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with VCS heading 60 degrees should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object VCS heading is too large negative.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_VCS_Heading_Too_Large_Negative)
{
   /** \precond
   * Set VCS heading to -60 degrees (equal to limit, should fail)
   */
   object_track.vcs_heading = Angle(F360_DEG2RAD(-60.0F));

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with VCS heading -60 degrees should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object bounding box orientation is too large positive.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Bbox_Orientation_Too_Large_Positive)
{
   /** \precond
   * Set bbox orientation to 60 degrees (equal to limit, should fail)
   */
   object_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(60.0F)));

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with bbox orientation 60 degrees should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object bounding box orientation is too large negative.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Bbox_Orientation_Too_Large_Negative)
{
   /** \precond
   * Set bbox orientation to -60 degrees (equal to limit, should fail)
   */
   object_track.bbox.Set_Orientation(Angle(F360_DEG2RAD(-60.0F)));

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with bbox orientation -60 degrees should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when object is behind the host vehicle (negative x position).
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Object_Behind_Host)
{
   /** \precond
   * Set longitudinal position to negative value
   */
   object_track.vcs_position.x = -0.1F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object behind host should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when time_since_started_move is too low.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Time_Since_Started_Move_Too_Low)
{
   /** \precond
   * Set time_since_started_move to 0.1s (equal to minimum, should fail)
   */
   object_track.time_since_started_move = 0.1F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with time_since_started_move = 0.1s should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when tangential acceleration is too high positive.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Tang_Accel_Too_High_Positive)
{
   /** \precond
   * Set tangential acceleration to 1.0 m/s^2 (equal to limit, should fail)
   */
   object_track.tang_accel = 1.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with tang_accel = 1.0 m/s^2 should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when tangential acceleration is too high negative.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Tang_Accel_Too_High_Negative)
{
   /** \precond
   * Set tangential acceleration to -1.0 m/s^2 (equal to limit, should fail)
   */
   object_track.tang_accel = -1.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with tang_accel = -1.0 m/s^2 should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns false
*          when filtered_hist_assoc_det_rr_err_mean is too high.
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Range_Rate_Error_Mean_Too_High)
{
   /** \precond
   * Set filtered_hist_assoc_det_rr_err_mean to 1.0 m/s (equal to limit, should fail)
   */
   object_track.filtered_hist_assoc_det_rr_err_mean = 1.0F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return false
   **/
   CHECK_FALSE_TEXT(is_valid, "Object with rr_err_mean = 1.0 m/s should not be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns true
*          at the edge of valid speed range (just above lower limit).
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Speed_Lower_Boundary)
{
   /** \precond
   * Set speed to 2.1 m/s (just above lower limit)
   */
   object_track.speed = 2.1F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return true
   **/
   CHECK_TRUE_TEXT(is_valid, "Object with speed 2.1 m/s should be valid");
}

/**
*\purpose  This test verifies that Is_Object_Valid_For_Tightened_Gates returns true
*          at the edge of valid speed range (just below upper limit).
*\req    NA
*/
TEST(f360_Test_Is_Object_Valid_For_Tightened_Gates, Test_Speed_Upper_Boundary)
{
   /** \precond
   * Set speed to 6.9 m/s (just below upper limit)
   */
   object_track.speed = 6.9F;

   /** \action
   * Call Is_Object_Valid_For_Tightened_Gates
   **/
   bool is_valid = Is_Object_Valid_For_Tightened_Gates(object_track);

   /** \result
   * Function should return true
   **/
   CHECK_TRUE_TEXT(is_valid, "Object with speed 6.9 m/s should be valid");
}

/** @}*/

/** \defgroup  f360_Test_Calculate_Tightened_Range_Rate_Threshold
*  @{
*/
/** \brief
*  Included tests related to calculation of tightened range rate threshold
*  for slow moving objects
**/
TEST_GROUP(f360_Test_Calculate_Tightened_Range_Rate_Threshold)
{
   F360_Object_Track_T object_track = {};
   float32_t max_range_rate_threshold = 2.0F;

   /** \setup
   * Setup test with default object properties
   **/
   TEST_SETUP()
   {
      object_track.speed = 5.0F;
      object_track.heading_rate = 0.0F;
   }
};

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold returns
*          the minimum threshold (0.7 m/s) when speed is at minimum and heading_rate is zero.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Minimum_Threshold)
{
   /** \precond
   * Set speed to 5.0 m/s (minimum for speed component) and heading_rate to 0.0 rad/s
   */
   object_track.speed = 5.0F;
   object_track.heading_rate = 0.0F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Threshold should be 0.7 m/s (MIN_RR_THOLD + 0.0 + 0.0)
   **/
   DOUBLES_EQUAL(0.7F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold increases
*          threshold when speed increases from minimum.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Speed_Component_Increase)
{
   /** \precond
   * Set speed to 7.0 m/s (maximum for speed component) and heading_rate to 0.0 rad/s
   */
   object_track.speed = 7.0F;
   object_track.heading_rate = 0.0F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Threshold should be 0.7 + 2.0 = 2.7 m/s, but saturated at max_range_rate_threshold = 2.0 m/s
   **/
   DOUBLES_EQUAL(2.0F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold increases
*          threshold when heading_rate increases.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Heading_Rate_Component_Increase)
{
   /** \precond
   * Set speed to 5.0 m/s (minimum) and heading_rate to 0.5 rad/s (maximum for yaw component)
   */
   object_track.speed = 5.0F;
   object_track.heading_rate = 0.5F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Threshold should be 0.7 + 0.0 + 0.8 = 1.5 m/s
   **/
   DOUBLES_EQUAL(1.5F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold handles
*          negative heading_rate correctly (uses absolute value).
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Negative_Heading_Rate)
{
   /** \precond
   * Set speed to 5.0 m/s and heading_rate to -0.5 rad/s
   */
   object_track.speed = 5.0F;
   object_track.heading_rate = -0.5F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Threshold should be 1.5 m/s (same as positive heading_rate)
   **/
   DOUBLES_EQUAL(1.5F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold correctly
*          combines both speed and heading_rate components.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Combined_Components)
{
   /** \precond
   * Set speed to 6.0 m/s (midpoint) and heading_rate to 0.25 rad/s (midpoint)
   */
   object_track.speed = 6.0F;
   object_track.heading_rate = 0.25F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Speed component: (6.0 - 5.0) / (7.0 - 5.0) * 2.0 = 1.0
   * Yaw component: (0.25 - 0.0) / (0.5 - 0.0) * 0.8 = 0.4
   * Total: 0.7 + 1.0 + 0.4 = 2.1, saturated at 2.0 m/s
   **/
   DOUBLES_EQUAL(2.0F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold saturates
*          at max_range_rate_threshold when components sum to a higher value.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Saturation_At_Max)
{
   /** \precond
   * Set speed to 7.0 m/s and heading_rate to 0.5 rad/s (both at max)
   */
   object_track.speed = 7.0F;
   object_track.heading_rate = 0.5F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Combined would be 0.7 + 2.0 + 0.8 = 3.5 m/s, but should saturate at 2.0 m/s
   **/
   DOUBLES_EQUAL(2.0F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold handles
*          mid-range heading_rate values correctly.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Mid_Range_Heading_Rate)
{
   /** \precond
   * Set speed to 5.0 m/s (minimum) and heading_rate to 0.1 rad/s
   */
   object_track.speed = 5.0F;
   object_track.heading_rate = 0.1F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Speed component: 0.0
   * Yaw component: 0.1 / 0.5 * 0.8 = 0.16
   * Total: 0.7 + 0.0 + 0.16 = 0.86 m/s
   **/
   DOUBLES_EQUAL(0.86F, threshold, 0.01F);
}

/**
*\purpose  This test verifies that Calculate_Tightened_Range_Rate_Threshold handles
*          mid-range speed values correctly.
*\req    NA
*/
TEST(f360_Test_Calculate_Tightened_Range_Rate_Threshold, Test_Mid_Range_Speed)
{
   /** \precond
   * Set speed to 6.0 m/s (midpoint between 5.0 and 7.0) and heading_rate to 0.0 rad/s
   */
   object_track.speed = 6.0F;
   object_track.heading_rate = 0.0F;

   /** \action
   * Call Calculate_Tightened_Range_Rate_Threshold
   **/
   float32_t threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, max_range_rate_threshold);

   /** \result
   * Speed component: (6.0 - 5.0) / (7.0 - 5.0) * 2.0 = 1.0
   * Yaw component: 0.0
   * Total: 0.7 + 1.0 + 0.0 = 1.7 m/s
   **/
   DOUBLES_EQUAL(1.7F, threshold, 0.01F);
}

/** @}*/
