/** \file
 * This file contains unit tests for content of f360_cond_deassoc_low_rr_dets.cpp file
 */

#include "f360_cond_deassoc_low_rr_dets.h"
#include <CppUTest/TestHarness.h>

 // Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_cond_deassoc_low_rr_dets
 *  @{
 */

 /** \brief
  * Tests using this test group will test the functionality of conditionally de-associating detections from objects based on
  * object speed, detection compensated range rate and detection position relative to object visible orth edge.
  */
TEST_GROUP(f360_cond_deassoc_low_rr_dets)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj = {};
   F360_Calibrations_T calibs = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /* Initialize tracker calibrations
    * Set up a default scenario with 3 detections associated to an object
    */

   TEST_SETUP()
   {
      /* Initialize tracker calibrations
       * * Set object:
       * - id to 1
       * - lateral position to 5
       * - wid1/2 to 0.5m
       * - visible edge to left edge
       * - speed to 3m/s
       * - ndets to 3
       * - detids to [1,2,3]
       * Create three detections and set
       * - object track id to 1
       * - longitudinal position to 0
       * - lateral position to be in object centroid
       * - range rate compensated to 0
       */
      Initialize_Tracker_Calibrations(calibs);
      obj.vcs_position.x = 0.0F;
      obj.vcs_position.y = 5.0F;
      Point center = {0.0F,5.0F};
      obj.bbox.Set_Center(center);
      obj.id = 1;
      obj.bbox.Set_Width(1.0F);
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.reference_point = F360_REFERENCE_POINT_LEFT;
      obj.speed = 3.0F;
      obj.ndets = 3;
      obj.detids[0] = 1U;
      obj.detids[1] = 2U;
      obj.detids[2] = 3U;

      det_props[0].object_track_id = obj.id;
      det_props[1].object_track_id = obj.id;
      det_props[2].object_track_id = obj.id;

      det_props[0].vcs_position.x = 0.0F;
      det_props[0].vcs_position.y = obj.vcs_position.y;
      det_props[1].vcs_position.x = 0.0F;
      det_props[1].vcs_position.y = obj.vcs_position.y;
      det_props[2].vcs_position.x = 0.0F;
      det_props[2].vcs_position.y = obj.vcs_position.y;

      det_props[0].range_rate_compensated = 0.0F;
      det_props[1].range_rate_compensated = 0.0F;
      det_props[2].range_rate_compensated = 0.0F;

   }

};

/** \purpose
 * Verify that no detections are deassociated when detections are located in object centroid even though compensated range rate is 0.
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_All_Dets_In_Centroid_And_Low_RR)
{
   /** \precond
    * Tracker calibrations have been initialized
    * An object has been created to the right of host with left edge visible
    * Object speed has been set to 3m/s
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - detections have been placed in object centroid
    * - compensated range rate for all detections has been set to 0
    */

    /** \action
     * Call Cond_Deassoc_Low_RR_Dets
     */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object is 3
    * Check that object detids was not modified
    * Check that object track id for the detections was not cleared
    */
   CHECK_EQUAL_TEXT(3, obj.ndets, "Number of associated detections was reduced when it should not have been.");

   CHECK_EQUAL_TEXT(1, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(2, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[2], "Detection id for associated detections was modified when it should not have been.");

   CHECK_EQUAL_TEXT(obj.id, det_props[0].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the detection that is close to the objects furthest orth edge is successfully deassociated from the object when
 * the compensated range rate is slightly below max range rate threshold
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Inside_Zone_And_Low_RR)
{
   /** \precond
    * Tracker calibrations have been initialized
    * An object has been created to the right of host with left edge visible
    * Object speed has been set to 3m/s
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - The last two detections have been placed in object centroid
    * - compensated range rate for the last two detections has been set to 0
    * Set lateral position of the first detection to be inside deassociation zone
    * Set compensated range rate for first detection to be slightly below max range rate threshold
    */
   det_props[0].vcs_position.y = obj.vcs_position.y + calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width() + 0.01F;
   det_props[0].range_rate_compensated = calibs.k_cond_deassoc_det_comp_rr_max - 0.01F;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object was reduced to 2
    * Check that object detids array was modified correctly
    * Check that object track id for the deassociated detection was set to 0
    * Check that object track id for the detections that were not deassociated was not modified.
    */
   CHECK_EQUAL_TEXT(2, obj.ndets, "Number of associated detections was not reduced when it should have been.");

   CHECK_EQUAL_TEXT(2, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(0, obj.detids[2], "Detection id was not reset to 0 when a detection was deassociated.");

   CHECK_EQUAL_TEXT(0, det_props[0].object_track_id, "Detection object track id was not set to 0.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the detection that is close to the objects furthest orth edge is not deassociated from the object when
 * the compensated range rate is slightly above max range rate threshold
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Inside_Zone_But_RR_Above_Threshold)
{
   /** \precond
    * Tracker calibrations have been initialized
    * An object has been created to the right of host with left edge visible
    * Object speed has been set to 3m/s
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - detections have been placed in object centroid
    * - compensated range rate for all detections has been set to 0
    * Set lateral position of the first detection to be inside deassociation zone
    * Set compensated range rate for first detection to be slightly above max range rate threshold
    */
   det_props[0].vcs_position.y = obj.vcs_position.y + calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width() + 0.01F;
   det_props[0].range_rate_compensated = calibs.k_cond_deassoc_det_comp_rr_max + 0.01F;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object is 3
    * Check that object detids was not modified
    * Check that object track id for the detections was not cleared
    */
   CHECK_EQUAL_TEXT(3, obj.ndets, "Number of associated detections was reduced when it should not have been.");

   CHECK_EQUAL_TEXT(1, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(2, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[2], "Detection id for associated detections was modified when it should not have been.");

   CHECK_EQUAL_TEXT(obj.id, det_props[0].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the detection that is close to the objects furthest orth edge and with low compensated range rate
 * is not deassociated when object speed is below threshold.
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Inside_Zone_And_Low_RR_But_Obj_Spd_To_Low)
{
   /** \precond
    * Tracker calibrations have been initialized
    * An object has been created to the right of host with left edge visible
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - detections have been placed in object centroid
    * - compensated range rate for all detections has been set to 0
    * Set lateral position of the first detection to be inside deassociation zone
    * Set compensated range rate for first detection to be slightly below max range rate threshold
    * Set object speed to below threshold
    */
   det_props[0].vcs_position.y = obj.vcs_position.y + calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width() + 0.01F;
   det_props[0].range_rate_compensated = calibs.k_cond_deassoc_det_comp_rr_max - 0.01F;

   obj.speed = calibs.k_cond_deassoc_min_obj_spd_for_deassoc - 0.01F;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object is 3
    * Check that object detids was not modified
    * Check that object track id for the detections was not cleared
    */
   CHECK_EQUAL_TEXT(3, obj.ndets, "Number of associated detections was reduced when it should not have been.");

   CHECK_EQUAL_TEXT(1, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(2, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[2], "Detection id for associated detections was modified when it should not have been.");

   CHECK_EQUAL_TEXT(obj.id, det_props[0].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the detection that is close to the objects furthest orth edge and with low compensated range rate
 * is successfully deassociated when right side of object is visible.
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Inside_Zone_And_Low_RR_Object_To_The_Left_Of_Host)
{
   /** \precond
    * Tracker calibrations have been initialized
    * Object width has been set to 1m with centroid in the middle
    * Object speed has been set to 3m/s
    * Three detections have been associated to the object
    * - compensated range rate for all detections has been set to 0
    * Set compensated range rate for first detection to be slightly below max range rate threshold
    * Set object position to the left of host
    * Set object visible edge to right edge
    * Set lateral position of first detection to be inside deassociation zone
    * Set lateral position for second and third detection to be in object centroid
    */

   det_props[0].range_rate_compensated = calibs.k_cond_deassoc_det_comp_rr_max - 0.01F;
   obj.vcs_position.x = 0.0F;
   obj.vcs_position.y = -1.5F;
   Point center = {0.0F,-2.0F};
   obj.bbox.Set_Center(center);
   obj.bbox.Set_Width(1.0F);
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   det_props[0].vcs_position.y = obj.vcs_position.y - calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width() - 0.01F;
   det_props[1].vcs_position.y = obj.vcs_position.y;
   det_props[2].vcs_position.y = obj.vcs_position.y;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object was reduced to 2
    * Check that object detids array was modified correctly
    * Check that object track id for the deassociated detection was set to 0
    * Check that object track id for the detections that were not deassociated was not modified.
    */
   CHECK_EQUAL_TEXT(2, obj.ndets, "Number of associated detections was not reduced when it should have been.");

   CHECK_EQUAL_TEXT(2, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(0, obj.detids[2], "Detection id was not reset to 0 when a detection was deassociated.");

   CHECK_EQUAL_TEXT(0, det_props[0].object_track_id, "Detection object track id was not set to 0.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the no detections are deassociated from the object when neither of the orth edges are visible.
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Rear_Visible)
{
   /** \precond
    * Tracker calibrations have been initialized
    * Object speed has been set to 3m/s
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - compensated range rate for all detections has been set to 0
    * Set object position to be straight in front of host
    * Set object visible edges to rear
    * Set detection position on rear left, rear center and rear right.
    */
   obj.vcs_position.x = 2.0F;
   obj.vcs_position.y = 0.0F;
   Point center = {5.0F,0.0F};
   obj.bbox.Set_Center(center);
   obj.bbox.Set_Length(6.0F);
   obj.reference_point = F360_REFERENCE_POINT_REAR;

   det_props[0].vcs_position.y = obj.vcs_position.y - obj.bbox.Get_Width()/2;
   det_props[1].vcs_position.y = obj.vcs_position.y;
   det_props[2].vcs_position.y = obj.vcs_position.y + obj.bbox.Get_Width()/2;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object is 3
    * Check that object detids was not modified
    * Check that object track id for the detections was not cleared
    */
   CHECK_EQUAL_TEXT(3, obj.ndets, "Number of associated detections was reduced when it should not have been.");

   CHECK_EQUAL_TEXT(1, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(2, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[2], "Detection id for associated detections was modified when it should not have been.");

   CHECK_EQUAL_TEXT(obj.id, det_props[0].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the detection that is close to the objects furthest orth edge is successfully deassociated from the object when
 * the compensated range rate is slightly below max range rate threshold and that num types of dets in object track is modified correctly 
 * when detection motion status is moving.
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Inside_Zone_And_Low_RR_Moving_Det)
{
   /** \precond
    * Tracker calibrations have been initialized
    * An object has been created to the right of host with left edge visible
    * Object speed has been set to 3m/s
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - detections have been placed in object centroid
    * - compensated range rate for all detections has been set to 0
    * Set lateral position of the first detection to be inside deassociation zone
    * Set compensated range rate for first detection to be slightly below max range rate threshold
    * Set first element of object num types of dets to 1
    * Set motion status for the first detection to moving
    */
   det_props[0].vcs_position.y = obj.vcs_position.y + calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width() + 0.01F;
   det_props[0].range_rate_compensated = calibs.k_cond_deassoc_det_comp_rr_max - 0.01F;

   obj.num_types_of_dets[0] = 1;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object was reduced to 2
    * Check that object detids array was modified correctly
    * Check that object track id for the deassociated detection was set to 0
    * Check that object track id for the detections that were not deassociated was not modified.
    */
   CHECK_EQUAL_TEXT(2, obj.ndets, "Number of associated detections was not reduced when it should have been.");
   CHECK_EQUAL_TEXT(0, obj.num_types_of_dets[0], "The number of moving detections was not reduced when the moving detection was deassociated.");

   CHECK_EQUAL_TEXT(2, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(0, obj.detids[2], "Detection id was not reset to 0 when a detection was deassociated.");

   CHECK_EQUAL_TEXT(0, det_props[0].object_track_id, "Detection object track id was not set to 0.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** \purpose
 * Verify that the detection that is close to the objects furthest orth edge is successfully deassociated from the object when
 * the compensated range rate is slightly below max range rate threshold and that num types of dets in object track is modified correctly 
 * when detection motion status is ambiguous.
 * \req
 * NA.
 */
TEST(f360_cond_deassoc_low_rr_dets, Cond_Deassoc_Low_RR_Dets_Detection_Inside_Zone_And_Low_RR_Ambiguous_Det)
{
   /** \precond
    * Tracker calibrations have been initialized
    * An object has been created to the right of host with left edge visible
    * Object speed has been set to 3m/s
    * Object width has been set to 1m with centroid in the middle
    * Three detections have been associated to the object
    * - detections have been placed in object centroid
    * - compensated range rate for all detections has been set to 0
    * Set lateral position of the first detection to be inside deassociation zone
    * Set compensated range rate for first detection to be slightly below max range rate threshold
    * Set second element of object num types of dets to 2
    * Set motion status for the first detection to ambiguous
    */
   det_props[0].vcs_position.y = obj.vcs_position.y + calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width() + 0.01F;
   det_props[0].range_rate_compensated = calibs.k_cond_deassoc_det_comp_rr_max - 0.01F;

   obj.num_types_of_dets[1] = 2;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   /** \action
    * Call Cond_Deassoc_Low_RR_Dets
    */
   Cond_Deassoc_Low_RR_Dets(calibs, det_props, obj);

   /** \result
    * Check that number of detections associated to the object was reduced to 2
    * Check that object detids array was modified correctly
    * Check that object track id for the deassociated detection was set to 0
    * Check that object track id for the detections that were not deassociated was not modified.
    */
   CHECK_EQUAL_TEXT(2, obj.ndets, "Number of associated detections was not reduced when it should have been.");
   CHECK_EQUAL_TEXT(1, obj.num_types_of_dets[1], "The number of ambiguous detections was not reduced when the moving detection was deassociated.");

   CHECK_EQUAL_TEXT(2, obj.detids[0], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(3, obj.detids[1], "Detection id for associated detections was modified when it should not have been.");
   CHECK_EQUAL_TEXT(0, obj.detids[2], "Detection id was not reset to 0 when a detection was deassociated.");

   CHECK_EQUAL_TEXT(0, det_props[0].object_track_id, "Detection object track id was not set to 0.");
   CHECK_EQUAL_TEXT(obj.id, det_props[1].object_track_id, "Detection object track id was modified when it should not have been.");
   CHECK_EQUAL_TEXT(obj.id, det_props[2].object_track_id, "Detection object track id was modified when it should not have been.");
}

/** @}*/
