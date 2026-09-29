/** \file
 * This file contains unit tests for content of f360_mark_dets_as_close_target_and_farside.cpp file
 */

#include "f360_mark_dets_as_close_target_and_farside.h"
#include <CppUTest/TestHarness.h>
#include <string.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \brief
* This test group includes test of the function Mark_Detections_As_Close_Target() defined in
* f360_mark_dets_as_close_target_and_farside.cpp.
*/
TEST_GROUP(mark_detections_as_close_target)
{
   // Declare common variables used within all tests in this test group.
   uint32_t num_dets;
   F360_Object_Track_T object_track;
   BoundingBox ct_ext_box_on_farside;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];

   /** \setup
   * Set up a target with position (0,3) in VCS pointing in the same direction as host. 
   * Construct a reasonable zone for close target functionality (ct_ext_box_on_farside).
   */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      num_dets = 1U;
      object_track.vcs_position.x = 0.0F;
      object_track.vcs_position.y = 3.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });

      ct_ext_box_on_farside = BoundingBox{ Point{-1.75F, -1.5F}, Point{2.75F, 3.5F} }; //set bbox in TCS first for easier understanding
      ct_ext_box_on_farside.Transform_To_Relative_Coordinate_System(Point{ -object_track.vcs_position.x , -object_track.vcs_position.y }, -object_track.bbox.Get_Orientation()); // transform to VCS
   }
};

/** \purpose
* Check that Mark_Detections_As_Close_Target() works as expected when one detection is marked as f_close_target.
* \req
* NA.
*/
TEST(mark_detections_as_close_target, Mark_Detections_As_Close_Target_CT_True)
{
   /** \precond
   * Set up properties of a detection so it is relevant and placed inside farside zone. 
   */
   det_props[0].object_track_id = 0;
   det_props[0].f_close_target = false;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_props[0].f_double_bounce = false;

   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 3.0F;

   /** \action
   * Call Mark_Detections_As_Close_Target().
   */
   Mark_Detections_As_Close_Target(num_dets, ct_ext_box_on_farside, det_props);

   /** \result
   * Check that the detection is marked as expected.
   */
   CHECK_EQUAL_TEXT(true, det_props[0].f_close_target, "The detection's property was not set as expected.")
}

/** \purpose
* Check that Mark_Detections_As_Close_Target() works as expected when one detections is irrelevant for this functionality..
* \req
* NA.
*/
TEST(mark_detections_as_close_target, Mark_Detections_As_Close_Target_CT_False)
{
   /** \precond
   * Set up properties of a detection so it is considered as irrelevant.
   */
   det_props[0].object_track_id = 1;
   det_props[0].f_close_target = false;

   /** \action
   * Call Mark_Detections_As_Close_Target().
   */
   Mark_Detections_As_Close_Target(num_dets, ct_ext_box_on_farside, det_props);

   /** \result
   * Check that the detection is marked as expected.
   */
   CHECK_EQUAL_TEXT(false, det_props[0].f_close_target, "The detection's property was not set as expected.")
}
/** @}*/

/** \defgroup  farside_countermeasure
*  @{
*/

/** \brief
* This test group includes test of the function Farside_Countermeasure() defined in
* f360_mark_dets_as_close_target_and_farside.cpp.
*/
TEST_GROUP(farside_countermeasure)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object_track;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calibrations;
   BoundingBox fcm_ext_box_on_nearby_side;
   F360_Object_Sides_T closest_orth_edge;

   uint32_t exp_ndets = 0U;
   uint32_t exp_detids[MAX_DETS_IN_OBJ_TRK] = { };

   /** \setup
   * Initialize calibrations.
   * Set up a track with pointing in the same direction as host with left edge visible and 
   * coordinates (0, 3) in VCS. No detections associated with freezing logic activated for simplicity.
   * Construct a reasonable box for farside countermeasure which is extended on nearby side, i.e. left side. 
   */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      Initialize_Tracker_Calibrations(calibrations);

      closest_orth_edge = F360_OBJECT_SIDES_LEFT;

      object_track.ndets = 0;
      memset(object_track.detids, 0, sizeof(object_track.detids));
      object_track.vcs_position.x = 0.0F;
      object_track.vcs_position.y = 3.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });

      fcm_ext_box_on_nearby_side = BoundingBox{ Point{-2.0F, -2.5F}, Point{3.0F, 0.5F} }; //set bbox in TCS first for easier understanding
      fcm_ext_box_on_nearby_side.Transform_To_Relative_Coordinate_System(Point{ -object_track.vcs_position.x , -object_track.vcs_position.y }, -object_track.bbox.Get_Orientation()); // transform to VCS
   }
};

/** \purpose
* Check that Farside_Countermeasure() works as expected when there are no associated detections to the track. 
* \req
* NA
*/
TEST(farside_countermeasure, Farside_Countermeasure_No_Detections)
{
   /** \precond
   * Nothing from set up needs to be changed here. 
   */

   /** \action
   * Call Farside_Countermeasure().
   */
   Farside_Countermeasure(object_track, det_props, fcm_ext_box_on_nearby_side);

   /** \result
   * Check that no relevant object properties have changed when no detections are associated. 
   */
   CHECK_EQUAL_TEXT(0, object_track.ndets, "Number of associated detections was not returned as expected.")
   for (uint32_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      CHECK_EQUAL_TEXT(0, object_track.detids[i], "The target's associated detections' IDs were not returned as expected.")
   }
}

/** \purpose
* Check that Farside_Countermeasure() works as expected when there is one ok detection associated to the target.
* \req
* NA
*/
TEST(farside_countermeasure, Farside_Countermeasure_One_Detection_OK)
{
   /** \precond
   * Add one associated detections which is placed inside the zone fcm_ext_box_on_nearby_side. 
   * Therefore it should be considered as ok and still be associated to the track. 
   */
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 3.0F;
   det_props[0].f_close_target = false;
   det_props[0].f_ok_to_use = true;
   det_props[0].object_track_id = 5;

   /** \action
   * Call Farside_Countermeasure().
   */
   Farside_Countermeasure(object_track, det_props, fcm_ext_box_on_nearby_side);

   /** \result
   * Check that the detection is still associated to the target and that none of its or the target's properties have changed.
   */
   exp_ndets = 1U;
   exp_detids[0] = 1;

   CHECK_EQUAL_TEXT(exp_ndets, object_track.ndets, "Number of associated detections was not returned as expected.")
   for (uint32_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      CHECK_EQUAL_TEXT(exp_detids[i], object_track.detids[i], "The target's associated detections' IDs were not returned as expected.")
   }

   CHECK_EQUAL_TEXT(false, det_props[0].f_close_target, "The detection's property f_close_target was not returned as expected.")
   CHECK_EQUAL_TEXT(true, det_props[0].f_ok_to_use, "The detection's property f_ok_to_use was not returned as expected.")
   CHECK_EQUAL_TEXT(5, det_props[0].object_track_id, "The detection's property object_track_id was not returned as expected.")
}

/** \purpose
* Check that Farside_Countermeasure() works as expected when there is one not ok detection associated to the target.
* \req
* NA
*/
TEST(farside_countermeasure, Farside_Countermeasure_One_Detection_Not_OK)
{
   /** \precond
   * Add one associated detections which is placed outside the zone fcm_ext_box_on_nearby_side.
   * Therefore it should be considered as not ok and be unassociated to the track.
   */
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   object_track.num_types_of_dets[0] = 1;
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 4.6F;
   det_props[0].f_close_target = false;
   det_props[0].f_ok_to_use = true;
   det_props[0].object_track_id = 5;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
   * Call Farside_Countermeasure().
   */
   Farside_Countermeasure(object_track, det_props, fcm_ext_box_on_nearby_side);

   /** \result
   * Check that the detection is unassociated to the target and that the properties have changed accordingly.
   */
   exp_ndets = 0U;
   exp_detids[0] = 0;

   CHECK_EQUAL_TEXT(exp_ndets, object_track.ndets, "Number of associated detections was not returned as expected.")
   for (uint32_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      CHECK_EQUAL_TEXT(exp_detids[i], object_track.detids[i], "The target's associated detections' IDs were not returned as expected.")
   }

   CHECK_EQUAL_TEXT(true, det_props[0].f_close_target, "The detection's property f_close_target was not returned as expected.")
   CHECK_EQUAL_TEXT(false, det_props[0].f_ok_to_use, "The detection's property f_ok_to_use was not returned as expected.")
   CHECK_EQUAL_TEXT(0, det_props[0].object_track_id, "The detection's property object_track_id was not returned as expected.")

   CHECK_EQUAL_TEXT(0, object_track.num_types_of_dets[0], "Number of associated moving detections does not match expected value")
}

/** \purpose
* Check that Farside_Countermeasure() works as expected when there are three associated detections 
* and the second one is not ok (and the other two are ok).
* \req
* NA
*/
TEST(farside_countermeasure, Farside_Countermeasure_Three_Detections_OK_NOK_OK)
{
   /** \precond
   * Add three associated detections where the second one is placed outside the zone fcm_ext_box_on_nearby_side.
   * Therefore it should be considered as not ok and be unassociated to the track.
   * The other two are placed inside and should be considered as ok and stay associated. 
   */
   object_track.ndets = 3;
   object_track.detids[0] = 1;
   object_track.detids[1] = 2;
   object_track.detids[2] = 3;
   object_track.num_types_of_dets[1] = 3;

   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 2.0F;
   det_props[0].f_close_target = false;
   det_props[0].f_ok_to_use = true;
   det_props[0].object_track_id = 5;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   det_props[1].vcs_position.x = 0.0F;
   det_props[1].vcs_position.y = 4.6F;
   det_props[1].f_close_target = false;
   det_props[1].f_ok_to_use = true;
   det_props[1].object_track_id = 5;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = 3.4F;
   det_props[2].f_close_target = false;
   det_props[2].f_ok_to_use = true;
   det_props[2].object_track_id = 5;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   /** \action
   * Call Farside_Countermeasure().
   */
   Farside_Countermeasure(object_track, det_props, fcm_ext_box_on_nearby_side);

   /** \result
   * Check that the detection is unassociated to the target and that the properties have changed accordingly.
   */
   exp_ndets = 2U;
   exp_detids[0] = 1;
   exp_detids[1] = 3;
   exp_detids[2] = 0;

   CHECK_EQUAL_TEXT(exp_ndets, object_track.ndets, "Number of associated detections was not returned as expected.")
   for (uint32_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      CHECK_EQUAL_TEXT(exp_detids[i], object_track.detids[i], "The target's associated detections' IDs were not returned as expected.")
   }

   CHECK_EQUAL_TEXT(false, det_props[0].f_close_target, "The detection's property f_close_target was not returned as expected.")
   CHECK_EQUAL_TEXT(true, det_props[0].f_ok_to_use, "The detection's property f_ok_to_use was not returned as expected.")
   CHECK_EQUAL_TEXT(5, det_props[0].object_track_id, "The detection's property object_track_id was not returned as expected.")

   CHECK_EQUAL_TEXT(true, det_props[1].f_close_target, "The detection's property f_close_target was not returned as expected.")
   CHECK_EQUAL_TEXT(false, det_props[1].f_ok_to_use, "The detection's property f_ok_to_use was not returned as expected.")
   CHECK_EQUAL_TEXT(0, det_props[1].object_track_id, "The detection's property object_track_id was not returned as expected.")

   CHECK_EQUAL_TEXT(false, det_props[2].f_close_target, "The detection's property f_close_target was not returned as expected.")
   CHECK_EQUAL_TEXT(true, det_props[2].f_ok_to_use, "The detection's property f_ok_to_use was not returned as expected.")
   CHECK_EQUAL_TEXT(5, det_props[2].object_track_id, "The detection's property object_track_id was not returned as expected.")

   CHECK_EQUAL_TEXT(2, object_track.num_types_of_dets[1], "Number of associated ambiguous detections does not match expected value")

}
/** @}*/

/** \defgroup is_obj_relevant_for_close_target_and_farside
*  @{
*/

/** \brief
* This test group includes test of the function Is_Obj_Relevant_For_Close_Target_And_Farside() defined in
* f360_mark_dets_as_close_target_and_farside.cpp.
*/
TEST_GROUP(is_obj_relevant_for_close_target_and_farside)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object_track;
   F360_Calibrations_T calibs;
   F360_Object_Sides_T closest_orth_edge;
   /** \setup
   * Initialize calibration structure.
   */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      Initialize_Tracker_Calibrations(calibs);
   }
};

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (small heading, left edge).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Small_Heading_Left_Edge)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   closest_orth_edge = F360_OBJECT_SIDES_LEFT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (large heading, left edge).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Large_Heading_Left_Edge)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just above 150 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(150.1F) };
   closest_orth_edge = F360_OBJECT_SIDES_LEFT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (small heading, right edge).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Small_Heading_Right_Edge)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (large heading, right edge).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Large_Heading_Right_Edge)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just above 150 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(150.1F) };
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when some conditions
* are met, but not accepted edge.
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_NOK_Invalid_Edge)
{
   /** \precond
   * Set fields so that some conditions are met for object to be considered relevant, except edge.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   closest_orth_edge = F360_OBJECT_SIDES_INVALID;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when some conditions
* are met, but not accepted heading.
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_NOK_Heading)
{
   /** \precond
   * Set fields so that some conditions are met for object to be considered relevant, except heading.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just above 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(30.1F) };

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when some conditions
* are met, but not position.
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside, Is_Obj_Relevant_For_Close_Target_And_Farside_NOK_Position)
{
   /** \precond
   * Set fields so that some conditions are met for object to be considered relevant, except position.
   * Position is set so Euclidean distance squared is just above calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.92F;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}
/** @}*/


/** \defgroup is_obj_relevant_for_close_target_and_farside_CCA
*  @{
*/

/** \brief
* This test group includes test of the function Is_Obj_Relevant_For_Close_Target_And_Farside() defined in
* f360_mark_dets_as_close_target_and_farside.cpp for CCA objects
*/
TEST_GROUP(is_obj_relevant_for_close_target_and_farside_CCA)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object_track;
   F360_Calibrations_T calibs;
   F360_Object_Sides_T closest_orth_edge;
   /** \setup
   * Initialize calibration structure.
   */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      Initialize_Tracker_Calibrations(calibs);
   }
};


/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when all conditions
* are met (small heading, left edge) except for object being fast moving
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Small_Heading_Left_Edge_slow_moving_CCA)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   object_track.speed = calibs.fast_moving_thresh - 0.1F;
   closest_orth_edge = F360_OBJECT_SIDES_LEFT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_FALSE_TEXT(f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}


/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (small heading, left edge, fast moving).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Small_Heading_Left_Edge_Fast_Moving_CCA)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_LEFT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (large heading, left edge, fast moving).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Large_Heading_Left_Edge_Fast_Moving_CCA)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just above 150 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(150.1F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_LEFT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (small heading, right edge, fast moving).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Small_Heading_Right_Edge_Fast_Moving_CCA)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns true when all conditions
* are met (large heading, right edge, fast moving).
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_OK_Large_Heading_Right_Edge_Fast_Moving_CCA)
{
   /** \precond
   * Set fields to all conditions are met for object to be considered relevant.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just above 150 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(150.1F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when some conditions
* are met, but not accepted edge.
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_NOK_Invalid_Edge_Fast_Moving_CCA)
{
   /** \precond
   * Set fields so that some conditions are met for object to be considered relevant, except edge.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just below 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_INVALID;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when some conditions
* are met, but not accepted heading.
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_NOK_Heading_Fast_Moving_CCA)
{
   /** \precond
   * Set fields so that some conditions are met for object to be considered relevant, except heading.
   * Position is set so Euclidean distance squared is just below calibration parameter.
   * Heading is set so it is just above 30 degrees as in calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.91F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(30.1F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Obj_Relevant_For_Close_Target_And_Farside() returns false when some conditions
* are met, but not position.
* \req
* NA
*/
TEST(is_obj_relevant_for_close_target_and_farside_CCA, Is_Obj_Relevant_For_Close_Target_And_Farside_NOK_Position_Fast_Moving_CCA)
{
   /** \precond
   * Set fields so that some conditions are met for object to be considered relevant, except position.
   * Position is set so Euclidean distance squared is just above calibration parameter.
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 5.92F;
   object_track.vcs_heading = Angle{ F360_DEG2RAD(29.9F) };
   object_track.speed = calibs.fast_moving_thresh + 1.0F;
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call Is_Obj_Relevant_For_Close_Target_And_Farside().
   */
   const bool f_relevant = Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibs, closest_orth_edge);

   /** \result
   * Check that Is_Obj_Relevant_For_Close_Target_And_Farside() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Obj_Relevant_For_Close_Target_And_Farside() did not return the expected output.")
}

/** @}*/


/** \defgroup  compute_ext_bounding_boxes
*  @{
*/

/** \brief
* This test group includes test of the function Compute_Ext_Bounding_Boxes() defined in
* f360_mark_dets_as_close_target_and_farside.cpp.
*/
TEST_GROUP(compute_ext_bounding_boxes)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object_track;
   F360_Calibrations_T calibs;
   F360_Object_Sides_T closest_orth_edge;
   /** \setup
   * Initialize calibration parameters. Set up dimensions of target that is driving in parallel with host (vcs_pointing = 0 degrees).
   */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      Initialize_Tracker_Calibrations(calibs);

      object_track.long_buffer_zone_len1 = 1.0F;
      object_track.long_buffer_zone_len2 = 1.0F;
      object_track.lat_buffer_zone_wid1 = 1.0F;
      object_track.lat_buffer_zone_wid2 = 1.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.Update_Bbox_Size(3.0F, 2.0F);
   }
};

/** \purpose
* Check that the function Compute_Ext_Bounding_Boxes() works as intended when left edge is closest.
* \req
* NA
*/
TEST(compute_ext_bounding_boxes, Compute_Ext_Bounding_Boxes_Left)
{
   /** \precond
   * Set up position and orth edge for target.
   */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 3.0F;
   object_track.Update_Bbox_Center();

   closest_orth_edge = F360_OBJECT_SIDES_LEFT;
   BoundingBox fcm_ext_box_on_nearby_side{ object_track.bbox };
   BoundingBox ct_ext_box_on_farside{ object_track.bbox };

   /** \action
   * Call Compute_Ext_Bounding_Boxes().
   */
   Compute_Ext_Bounding_Boxes(object_track, calibs, closest_orth_edge, fcm_ext_box_on_nearby_side, ct_ext_box_on_farside);

   /** \result
   * Check that the extended bounding boxes were computed as expected in Compute_Ext_Bounding_Boxes()
   */

   // Define expected output
   const BoundingBox exp_fcm_ext_box_on_nearby_side{ Point{-2.5F, 1.0F}, Point{2.5F, 4.0F} };
   const BoundingBox exp_ct_ext_box_on_farside{ Point{-2.5F, 2.0F}, Point{2.5F, 8.0F} };

   for (uint32_t i = 0U; i < 4U; i++)
   {
      CHECK_TRUE_TEXT(exp_fcm_ext_box_on_nearby_side.Get_Corners().points[i] == fcm_ext_box_on_nearby_side.Get_Corners().points[i], "Extended box for farside countermeasure was not computed correctly.")
      CHECK_TRUE_TEXT(exp_ct_ext_box_on_farside.Get_Corners().points[i] == ct_ext_box_on_farside.Get_Corners().points[i], "Extended box for farside countermeasure was not computed correctly.")
   }
}

/** \purpose
* Check that the function Compute_Ext_Bounding_Boxes() works as intended when right edge is closest.
* \req
* NA
*/
TEST(compute_ext_bounding_boxes, Compute_Ext_Bounding_Boxes_Right)
{
   /** \precond
   * Set up position and orth edge for target.
   */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = -3.0F;
   object_track.Update_Bbox_Center();
   closest_orth_edge = F360_OBJECT_SIDES_RIGHT;
   BoundingBox fcm_ext_box_on_nearby_side{ object_track.bbox };
   BoundingBox ct_ext_box_on_farside{ object_track.bbox };

   /** \action
   * Call Compute_Ext_Bounding_Boxes().
   */
   Compute_Ext_Bounding_Boxes(object_track, calibs, closest_orth_edge, fcm_ext_box_on_nearby_side, ct_ext_box_on_farside);

   /** \result
   * Check that the extended bounding boxes were computed as expected in Compute_Ext_Bounding_Boxes()
   */

   // Define expected output
   const BoundingBox exp_fcm_ext_box_on_nearby_side{ Point{-2.5F, -4.0F}, Point{2.5F, -1.0F} };
   const BoundingBox exp_ct_ext_box_on_farside{ Point{-2.5F, -8.0F}, Point{2.5F, -2.0F} };

   for (uint32_t i = 0U; i < 4U; i++)
   {
      CHECK_TRUE_TEXT(exp_fcm_ext_box_on_nearby_side.Get_Corners().points[i] == fcm_ext_box_on_nearby_side.Get_Corners().points[i], "Extended box for farside countermeasure was not computed correctly.")
      CHECK_TRUE_TEXT(exp_ct_ext_box_on_farside.Get_Corners().points[i] == ct_ext_box_on_farside.Get_Corners().points[i], "Extended box for farside countermeasure was not computed correctly.")
   }
}

/** \purpose
* Check that the function Compute_Ext_Bounding_Boxes() works as intended when there is no valid orth edge.
* \req
* NA
*/
TEST(compute_ext_bounding_boxes, Compute_Ext_Bounding_Boxes_Invalid_Edge)
{
   /** \precond
   * Set up position and orth edge for target.
   */
   object_track.vcs_position.x = 3.0F;
   object_track.vcs_position.y = 0.0F;
   object_track.Update_Bbox_Center();
   closest_orth_edge = F360_OBJECT_SIDES_INVALID;
   BoundingBox fcm_ext_box_on_nearby_side{ object_track.bbox };
   BoundingBox ct_ext_box_on_farside{ object_track.bbox };

   /** \action
   * Call Compute_Ext_Bounding_Boxes().
   */
   Compute_Ext_Bounding_Boxes(object_track, calibs, closest_orth_edge, fcm_ext_box_on_nearby_side, ct_ext_box_on_farside);

   /** \result
   * Check that the extended bounding boxes were computed as expected in Compute_Ext_Bounding_Boxes()
   */

   // Define expected output
   const BoundingBox exp_fcm_ext_box_on_nearby_side{ Point{0.5F, -2.0F}, Point{5.5F, 2.0F} };
   const BoundingBox exp_ct_ext_box_on_farside{ Point{1.5F, -1.0F}, Point{4.5F, 1.0F} };

   for (uint32_t i = 0U; i < 4U; i++)
   {
      CHECK_TRUE_TEXT(exp_fcm_ext_box_on_nearby_side.Get_Corners().points[i] == fcm_ext_box_on_nearby_side.Get_Corners().points[i], "Extended box for farside countermeasure was not computed correctly.")
      CHECK_TRUE_TEXT(exp_ct_ext_box_on_farside.Get_Corners().points[i] == ct_ext_box_on_farside.Get_Corners().points[i], "Extended box for farside countermeasure was not computed correctly.")
   }
}

/** @}*/

/** \defgroup  is_det_relevant_for_close_target
*  @{
*/

/** \brief
* This test group includes test of the function Is_Det_Relevant_For_Close_Target() defined in
* f360_mark_dets_as_close_target_and_farside.cpp.
*/
TEST_GROUP(is_det_relevant_for_close_target)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Props_T det_prop;
};

/** \purpose
* Check that the function Is_Det_Relevant_For_Close_Target() returns true when all conditions
* are met.
* \req
* NA
*/
TEST(is_det_relevant_for_close_target, Is_Det_Relevant_For_Close_Target_All_Cond_Met)
{
   /** \precond
   * Set fields of detection properties so that all conditions in function is met.
   */
   det_prop.object_track_id = 0;
   det_prop.f_close_target = false;
   det_prop.wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_prop.f_double_bounce = false;

   /** \action
   * Call Is_Det_Relevant_For_Close_Target().
   */
   bool f_relevant = Is_Det_Relevant_For_Close_Target(det_prop);

   /** \result
   * Check that Is_Det_Relevant_For_Close_Target() returns the expected output
   */
   CHECK_EQUAL_TEXT(true, f_relevant, "Function Is_Det_Relevant_For_Close_Target() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Det_Relevant_For_Close_Target() returns true when some conditions
* are met but not f_double_bounce
* \req
* NA
*/
TEST(is_det_relevant_for_close_target, Is_Det_Relevant_For_Close_Target_Double_Bounce)
{
   /** \precond
   * Set fields of detection properties so that some conditions in function is met but not f_double_bounce.
   */
   det_prop.object_track_id = 0;
   det_prop.f_close_target = false;
   det_prop.wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_prop.f_double_bounce = true;

   /** \action
   * Call Is_Det_Relevant_For_Close_Target().
   */
   bool f_relevant = Is_Det_Relevant_For_Close_Target(det_prop);

   /** \result
   * Check that Is_Det_Relevant_For_Close_Target() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Det_Relevant_For_Close_Target() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Det_Relevant_For_Close_Target() returns true when some conditions
* are met but not wheel_spin_type
* \req
* NA
*/
TEST(is_det_relevant_for_close_target, Is_Det_Relevant_For_Close_Target_Wheel_Spin)
{
   /** \precond
   * Set fields of detection properties so that some conditions in function is met but not wheel_spin_type.
   */
   det_prop.object_track_id = 0;
   det_prop.f_close_target = false;
   det_prop.wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;

   /** \action
   * Call Is_Det_Relevant_For_Close_Target().
   */
   bool f_relevant = Is_Det_Relevant_For_Close_Target(det_prop);

   /** \result
   * Check that Is_Det_Relevant_For_Close_Target() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Det_Relevant_For_Close_Target() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Det_Relevant_For_Close_Target() returns true when some conditions
* are met but not f_close_target
* \req
* NA
*/
TEST(is_det_relevant_for_close_target, Is_Det_Relevant_For_Close_Target_Close_Target)
{
   /** \precond
   * Set fields of detection properties so that some conditions in function is met but not f_close_target.
   */
   det_prop.object_track_id = 0;
   det_prop.f_close_target = true;

   /** \action
   * Call Is_Det_Relevant_For_Close_Target().
   */
   bool f_relevant = Is_Det_Relevant_For_Close_Target(det_prop);

   /** \result
   * Check that Is_Det_Relevant_For_Close_Target() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Det_Relevant_For_Close_Target() did not return the expected output.")
}

/** \purpose
* Check that the function Is_Det_Relevant_For_Close_Target() returns true when some conditions
* are met but not object track id
* \req
* NA
*/
TEST(is_det_relevant_for_close_target, Is_Det_Relevant_For_Close_Target_Obj_ID)
{
   /** \precond
   * Set fields of detection properties so that some conditions in function is met but not object track id.
   */
   det_prop.object_track_id = 1;

   /** \action
   * Call Is_Det_Relevant_For_Close_Target().
   */
   bool f_relevant = Is_Det_Relevant_For_Close_Target(det_prop);

   /** \result
   * Check that Is_Det_Relevant_For_Close_Target() returns the expected output
   */
   CHECK_EQUAL_TEXT(false, f_relevant, "Function Is_Det_Relevant_For_Close_Target() did not return the expected output.")
}
/** @}*/
