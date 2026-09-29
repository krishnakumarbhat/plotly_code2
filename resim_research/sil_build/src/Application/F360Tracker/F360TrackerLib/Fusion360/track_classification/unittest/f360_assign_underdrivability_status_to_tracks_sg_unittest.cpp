/** \file
 * This file contains unit tests for content of f360_assign_underdrivability_status_to_tracks_sg.cpp file
 */

#include "f360_assign_underdrivability_status_to_tracks_sg.h"
#include <CppUTest/TestHarness.h>
#include "f360_sorted_tracks_mgmt.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_assign_underdrivability_status_to_tracks_sg
 *  @{
 */

/** \brief
 * The aim of this test group is to test the functionality of assigning drivibility status to objects based on
 * the classification of nearby SG segments.
 */
TEST_GROUP(f360_assign_underdrivability_status_to_tracks_sg)
{
   F360_Tracker_Info_T tracker_info;
   F360_Host_Props_T host_props;
   sg::SG_Output_T sg_output;
   uint8_t padding[sizeof(sg::SG_Contour_Out_T)*5];
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   float32_t host_dist_rear_axle_to_vcs_m;
   
   /** \setup
    * Set up a default scenario with
    * - One contour containing two vertices (i.e. one segment) on a straight longitudinal line
    * Three objects with two placed close to the segment
    */
   TEST_SETUP()
   {
      host_dist_rear_axle_to_vcs_m = 2.9F;

      tracker_info.num_active_objs = 3;
      tracker_info.active_obj_ids[0U] = 1U;
      tracker_info.active_obj_ids[1U] = 2U;
      tracker_info.active_obj_ids[2U] = 3U;

      object_tracks[0U].id = 1U;
      object_tracks[0U].movable_prob = 0.0F;
      object_tracks[0U].vcs_position.x = -50.0F;
      object_tracks[0U].vcs_position.y = -3.0F;
      object_tracks[0U].f_moving = false;
      object_tracks[0U].drivable_confidence_sg = 100U;
      object_tracks[0U].drivable_status_sg = sg::SG_Drivability_Class_T::OVERDRIVABLE;

      object_tracks[1U].id = 2U;
      object_tracks[1U].movable_prob = 0.0F;
      object_tracks[1U].vcs_position.x = 5.0F;
      object_tracks[1U].vcs_position.y = -3.0F;
      object_tracks[1U].f_moving = false;
      object_tracks[1U].drivable_confidence_sg = 50U;
      object_tracks[1U].drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;

      object_tracks[2U].id = 3U;
      object_tracks[2U].movable_prob = 0.0F;
      object_tracks[2U].vcs_position.x = 15.0F;
      object_tracks[2U].vcs_position.y = -3.0F;
      object_tracks[2U].f_moving = false;
      object_tracks[2U].drivable_confidence_sg = 50U;
      object_tracks[2U].drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;

      // Set up the list of longitudinally sorted objects
      tracker_info.vcslong_sorted_start = &object_tracks[0U];
      tracker_info.vcslong_sorted_next_track[0U] = &object_tracks[1U];
      tracker_info.vcslong_sorted_next_track[1U] = &object_tracks[2U];
      tracker_info.vcslong_sorted_next_track[2U] = NULL;
      tracker_info.vcslong_sorted_first_infront_of_host = &object_tracks[1U];

      // Set up the SG structures
      // (Note that vertex positions are in ISO, so they will be compensated with host dist to rear axle, and flipped y-axis)
      sg_output.num_contours = 1U;
      sg_output.contours[0U].num_vertices = 2U;

      sg_output.vertices[0U].drivability = sg::SG_Drivability_Class_T::NONDRIVABLE;
      sg_output.vertices[0U].drivability_confidence = 90U;
      sg_output.vertices[0U].position_x = 10.0F;
      sg_output.vertices[0U].position_y = 2.8F;

      sg_output.vertices[1U].drivability = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
      sg_output.vertices[1U].drivability_confidence = 95U;
      sg_output.vertices[1U].position_x = 7.0F;
      sg_output.vertices[1U].position_y = 2.8F;

      host_props.cos_delta_pointing = 1.0F;
      host_props.sin_delta_pointing = 0.0F;
   }
};

/** \purpose  
 * Check that the second object in the sorted list is associated and has its drivability status set to the corresponding SG status
 * and that the other objects are unclassified even when vcslong_sorted_first_infront_of_host is NULL.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Non_Drivable_SG_First_Obj_Infront_Null)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    * set vcslong_sorted_first_infront_of_host = NULL
    */
   tracker_info.vcslong_sorted_first_infront_of_host = NULL;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(90U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that all objects are unclassified if pointer to first sorted object is NULL and some part of the segment is behind host.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Non_Drivable_SG_First_Obj_Null)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    * set vcslong_sorted_start = NULL
    * Extend the segment behind host.
    */
   tracker_info.vcslong_sorted_start= NULL;
   sg_output.vertices[1U].position_x = -7.0F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that the second and third object in the sorted list is associated and have their drivability status set to the corresponding SG status
 * and that the other objects are unclassified. This also checks that all objects in the sorted list are processed when the object with largest
 * x position is closer than farthest vertex on the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Extended_Segment)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    * Extend segment from behind host to beyond last sorted object
    */
   sg_output.vertices[0U].position_x = 20.0F;
   sg_output.vertices[1U].position_x = -20.0F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(90U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(90U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that the second object in the sorted list is associated and has its drivability status set by the segment with the highest confidence
 * when there are more than one segment candidate at nearly equal distance. All other objects shall be unclassified.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Multiple_SG_Different_Conf)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - set up a second SG contour with a segment at nearly identical distance to the object as the first
   *   (ISO y 2.8 -> dist^2 0.04 m^2, ISO y 2.79 -> dist^2 0.0441 m^2, diff about 0.004 m^2 < 0.01 threshold)
    * - Set the status to OVERDRIVABLE with confidence 91%, higher than the first segment (NONDRIVABLE, 90%)
    * Expectation is that the object is assigned the status of the higher confidence segment (OVERDRIVABLE)
    */
   sg_output.num_contours = 2U;
   sg_output.contours[0U].num_vertices = 2U;
   sg_output.contours[1U].num_vertices = 2U;

   sg_output.vertices[0U].drivability = sg::SG_Drivability_Class_T::NONDRIVABLE;
   sg_output.vertices[0U].drivability_confidence = 90U;
   sg_output.vertices[0U].position_x = 10.0F;
   sg_output.vertices[0U].position_y = 2.8F;

   sg_output.vertices[1U].drivability = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
   sg_output.vertices[1U].drivability_confidence = 95U;
   sg_output.vertices[1U].position_x = 7.0F;
   sg_output.vertices[1U].position_y = 2.8F;

   sg_output.vertices[2U].drivability = sg::SG_Drivability_Class_T::OVERDRIVABLE;
   sg_output.vertices[2U].drivability_confidence = 91U;
   sg_output.vertices[2U].position_x = 10.0F;
   sg_output.vertices[2U].position_y = 2.79F;

   sg_output.vertices[3U].drivability = sg::SG_Drivability_Class_T::UNCLASSIFIED;
   sg_output.vertices[3U].drivability_confidence = 50U;
   sg_output.vertices[3U].position_x = 7.0F;
   sg_output.vertices[3U].position_y = 2.79F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::OVERDRIVABLE == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(91U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that the second object in the sorted list is left unclassified when it's moving
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Non_Drivable_SG_Obj_Moving)
{
   /** \precond
    * - set second object to moving
    * - set object drivable_status_sg to unclassified
    * - expecation is that it should be unclassified by SG
    */
   object_tracks[1U].f_moving = true;
   object_tracks[1U].drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
   object_tracks[1U].drivable_confidence_sg = 100U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(100U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence of moving object was reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when it's in the vicinity of the segment but not inside the classification gate.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Classification_Box)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted, such that the object can be inside the VCS zone it covers but outside the classification box
    * Place object in the vicinity of the segment but outside the classification box
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.x = 11.0F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its x pos is above the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_x_Above)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its x position is above the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.x = 11.6F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its x pos is below the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_x_Below)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its x position is below the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.x = 2.4F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its y pos is below the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_y_Below)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its y position is below the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.y = -6.6F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its y pos is above the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_y_Above)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its y position is above the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.y = -0.4F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its x and y pos is above the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_x_And_y_Above)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its a and y position is above the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.y = -1.4F;
   object_tracks[1U].vcs_position.x = 11.6F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its x pos is above and y pos below the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_x_Above_y_Below)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its a and y position is below and x pos above the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.y = -6.6F;
   object_tracks[1U].vcs_position.x = 11.6F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its x pos is below and y pos above the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_y_Above_x_Below)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its a and x position is below and y pos above the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.y = -1.4F;
   object_tracks[1U].vcs_position.x = 2.4F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose 
 * Check that the second object in the sorted list is left unclassified when its x and y pos are below the VCS vicinity zone of the segment.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Obj_Outisde_Vicinity_y_And_x_Below)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * Change the segment to be tilted
    * Place object such that its a and x and y position are below the VCS vicinity zone of the segment
    */
   sg_output.vertices[0U].position_x = 13.0F;
   sg_output.vertices[0U].position_y = 4.8F;

   object_tracks[1U].vcs_position.y = -6.6F;
   object_tracks[1U].vcs_position.x = 2.4F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that the all objects are assigned status unclassified, including the second object close to the SG segment, when the segment is
 * unclassified.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Unclassified_Drivable_SG)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - change drivable status of segment to UNCLASSIFIED (it's enough that only the first vertex is unclassified)
    * - Expectation is that all objects are UNCLASSIFIED after function call
    */
   sg_output.vertices[0U].drivability = sg::SG_Drivability_Class_T::UNCLASSIFIED;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that the all objects are assigned status unclassified, including the second object close to the SG segment, when the segment's
 * drivability_confidence is 0.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Zero_Drivability_Confidence)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - change drivability_confidence of segment to zero (it's enough that only the first vertex is unclassified)
    * - Expectation is that all objects are UNCLASSIFIED after function call
    */
   sg_output.vertices[0U].drivability_confidence = 0U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that all objects are unclassified.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}
\
/** \purpose  
 * Check that the first object in the sorted list is associated and has its drivability status set to the corresponding SG status
 * and that the other objects are unclassified when the SG segment is behind host.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Behind_Host)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - Move SG segment behind host, close to the first sorted object.
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    */
   sg_output.vertices[0U].position_x = -30.0F;
   sg_output.vertices[1U].position_x = -70.0F;


   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(90U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that contour array is not accessed outside of bounds
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Num_Contours_Out_Of_Bounds)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    */

    //Clear number of vertices
    for(int i = 0; i < SG_MAX_NUM_OUTPUT_CONTOURS; i++)
    {
       sg_output.contours[i].num_vertices = 0U;
    }
    
    //Set number of contours to out of bounds value
    sg_output.num_contours = SG_MAX_NUM_OUTPUT_CONTOURS + 1;

    //Set last contour to have two vertices
    sg::SG_Contour_Out_T& last_contour = sg_output.contours[SG_MAX_NUM_OUTPUT_CONTOURS]; // Out of bounds
    last_contour.num_vertices = 2U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that no object has been classified
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that vertices array is not accessed outside of bounds
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Num_Vertices_Out_Of_Bounds)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    */

    // Reset outputs to a known baseline (setup-only change)
    for (uint32_t i = 0U; i < 3U; i++)
    {
       object_tracks[i].drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
       object_tracks[i].drivable_confidence_sg = 0U;
    }

    // Make all vertices unclassified with zero confidence to prevent any assignment
    for (uint32_t i = 0U; i < SG_MAX_NUM_OUTPUT_VERTICES; i++)
    {
       sg_output.vertices[i].drivability = sg::SG_Drivability_Class_T::UNCLASSIFIED;
       sg_output.vertices[i].drivability_confidence = 0U;
    }

    sg_output.num_contours = 1U;
    //Set first contour to have out of bounds number of vertices
    sg_output.contours[0U].num_vertices = SG_MAX_NUM_OUTPUT_VERTICES + 2U;;


   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that no object has been classified
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}


/** @}*/

/** \defgroup  f360_assign_underdrivability_status_to_tracks_sg_movable
 *  @{
 */

/** \brief
 * The aim of this test group is to test the functionality of assigning drivibility status to non-moving but movable objects based on
 * the classification of nearby SG segments.
 */
TEST_GROUP(f360_assign_underdrivability_status_to_tracks_sg_movable)
{
   F360_Tracker_Info_T tracker_info;
   F360_Host_Props_T host_props;
   sg::SG_Output_T sg_output;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   float32_t host_dist_rear_axle_to_vcs_m;
   
   /** \setup
    * Set up a default scenario with
    * - One contour containing two vertices (i.e. one segment) on a straight longitudinal line
    * Three objects with two placed close to the segment
    */
   TEST_SETUP()
   {
      host_dist_rear_axle_to_vcs_m = 2.9F;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0U] = 1U;

      object_tracks[0U].id = 1U;
      object_tracks[0U].movable_prob = 1.0F;
      object_tracks[0U].vcs_position.x = 5.0F;
      object_tracks[0U].vcs_position.y = -3.0F;
      object_tracks[0U].f_moving = false;
      object_tracks[0U].drivable_confidence_sg = 100U;
      object_tracks[0U].drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;

      // Set up the list of longitudinally sorted objects
      tracker_info.vcslong_sorted_start = &object_tracks[0U];
      tracker_info.vcslong_sorted_next_track[0U] = NULL;
      tracker_info.vcslong_sorted_first_infront_of_host = &object_tracks[0U];

      // Set up the SG structures
      // (Note that vertex positions are in ISO, so they will be compensated with host dist to rear axle, and flipped y-axis)
      sg_output.num_contours = 1U;
      sg_output.contours[0U].num_vertices = 2U;

      sg_output.vertices[0U].drivability = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
      sg_output.vertices[0U].drivability_confidence = 74U;
      sg_output.vertices[0U].position_x = 10.0F;
      sg_output.vertices[0U].position_y = 2.8F;

      sg_output.vertices[1U].drivability = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
      sg_output.vertices[1U].drivability_confidence = 74U;
      sg_output.vertices[1U].position_x = 7.0F;
      sg_output.vertices[1U].position_y = 2.8F;
   }
};

/** \purpose  
 * Check that an object that is stationary but movable is assigned default status NONDRIVABLE if there is no associated SG segment with high enough confidence.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_movable, Assign_Underdrivability_Status_To_Stationary_Object_SG_Stationary_Movable_Obj_Default_Status)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - movable prob > 0.5
    * - No SG segments with enough confidence to classify movable objects.
    */


   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(75U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that an object that is stationary but movable is assigned the status of the associated SG segment when confidence is high enough.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_movable, Assign_Underdrivability_Status_To_Stationary_Object_SG_Stationary_Movable_Obj_New_Status)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - movable prob > 0.5
    * Set confidence of SG segment above 75%.
    */
   sg_output.vertices[0U].drivability_confidence = 75U;
   const sg::SG_Drivability_Class_T expected_drivable_status = sg::SG_Drivability_Class_T::NONDRIVABLE;
   const uint8_t expected_drivable_confidence = sg_output.vertices[0U].drivability_confidence;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, sg_output, host_dist_rear_axle_to_vcs_m, object_tracks);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(expected_drivable_status == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(expected_drivable_confidence, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** @}*/

/** \defgroup  f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment
 *  @{
 */

/** \brief
 * The purpose of this test group is to test the function that sets the object's drivable status from the stationary geometries vertex.
 */
TEST_GROUP(f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment)
{	
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object = {};
   sg::SG_Drivability_Class_T segment_drivability_status;
   uint8_t segment_drivability_confidence;
   float32_t segment_dist_sq;
   
   /** \setup
    * Set up a default scenario where an object has not been classified in the current tracker iteration yet, i.e.
    * - drivable status is UNCLASSIFIED
    * - drivable status confidence is 0
    * - drivable dist to segment is INFTY (as reset at the start of each tracker iteration)
    * An SG segment is associated with vertex status UNDERDRIVABLE, confidence 80%, dist 1.0 m^2
    */
   TEST_SETUP()
   {
      object.movable_prob = 0.0F;
      object.drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
      object.drivable_confidence_sg = 0U;
      object.drivable_sg_dist_to_segment_sq = INFTY;

      segment_drivability_status = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
      segment_drivability_confidence = 80U;
      segment_dist_sq = 1.0F;
   }
};

/** \purpose  
 * Test that the object is assigned the drivable status of the segment when the new segment is significantly closer than the previously assigned one.
 * \req
 * NA
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment, Assign_Object_Drivable_Status_And_Conf_From_SG_Segment_Assign_New_Status)
{
   /** \precond
    * An object and a segment vertex has been set up in the TEST_GROUP.
   * Object's dist to segment is INFTY, new segment is at 1.0 m^2 - significantly closer.
    * Expected outcome is that object is assigned the drivable status and confidence of the segment.
    */
   const sg::SG_Drivability_Class_T expected_drivable_status = segment_drivability_status;
   const uint8_t expected_drivable_confidence = segment_drivability_confidence;
	
   /** \action
    * Call Assign_Object_Drivable_Status_And_Conf_From_SG_Segment().
    */
   Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(segment_drivability_status, segment_drivability_confidence, segment_dist_sq, object);

   /** \result
    * Check that object is classified as UNDERDRIVABLE with confidence 80%.
    */
   CHECK_TRUE_TEXT(expected_drivable_status == object.drivable_status_sg, "Incorrect drivable status assined to object.");
   CHECK_EQUAL_TEXT(expected_drivable_confidence, object.drivable_confidence_sg, "Incorrect drivable confidence assined to object.")
}

/** \purpose  
 * Test that the object is not assigned the drivable status of the segment when distances are similar and
 * the new segment's confidence is lower than the object's current drivability confidence.
 * \req
 * NA
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment, Assign_Object_Drivable_Status_And_Conf_From_SG_Segment_Dont_Assign_New_Status)
{
   /** \precond
    * An object and a segment vertex has been set up in the TEST_GROUP.
    * Set object's current drivability confidence to 90% and squared dist to segment equal to segment_dist_sq,
    * so that distances are similar and the confidence tiebreaker is applied.
    * Expected outcome is that object is not assigned the drivable status and confidence of the segment
    * since segment confidence (80%) is lower than the object's current confidence (90%).
    */
   object.drivable_confidence_sg = 90U;
   object.drivable_sg_dist_to_segment_sq = segment_dist_sq;
   const sg::SG_Drivability_Class_T expected_drivable_status = object.drivable_status_sg;
   const uint8_t expected_drivable_confidence = object.drivable_confidence_sg;
	
   /** \action
    * Call Assign_Object_Drivable_Status_And_Conf_From_SG_Segment().
    */
   Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(segment_drivability_status, segment_drivability_confidence, segment_dist_sq, object);

   /** \result
    * Check that object is classified as UNCLASSIFIED with confidence 90%.
    */
   CHECK_TRUE_TEXT(expected_drivable_status == object.drivable_status_sg, "Incorrect drivable status assined to object.");
   CHECK_EQUAL_TEXT(expected_drivable_confidence, object.drivable_confidence_sg, "Incorrect drivable confidence assined to object.")
}

/** \purpose
 * Test that the new segment overrides the currently assigned status when it is significantly closer,
 * even if its confidence is lower than the previously assigned segment's confidence.
 * \req
 * NA
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment, Assign_Object_Drivable_Status_And_Conf_From_SG_Segment_New_Segment_Closer_Overrides_Confidence)
{
   /** \precond
    * An object and a segment vertex has been set up in the TEST_GROUP.
    * Set object's current drivability status to NONDRIVABLE with confidence 90% at dist 1.5 m^2.
   * New segment is at 0.2 m^2 - significantly closer (diff = 1.3 m^2 > 0.01 m^2 threshold).
    * New segment has lower confidence (80%) than the currently assigned one.
    */
   object.drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
   object.drivable_confidence_sg = 90U;
   object.drivable_sg_dist_to_segment_sq = 1.5F;
   const float32_t closer_dist_sq = 0.2F;

   /** \action
    * Call Assign_Object_Drivable_Status_And_Conf_From_SG_Segment().
    */
   Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(segment_drivability_status, segment_drivability_confidence, closer_dist_sq, object);

   /** \result
    * Check that object is overridden to UNDERDRIVABLE with confidence 80% since the new segment is significantly closer.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNDERDRIVABLE == object.drivable_status_sg, "Incorrect drivable status assined to object.");
   CHECK_EQUAL_TEXT(80U, object.drivable_confidence_sg, "Incorrect drivable confidence assined to object.")
   DOUBLES_EQUAL(0.2F, object.drivable_sg_dist_to_segment_sq, 1e-6F);
}

/** \purpose
 * Test that the currently assigned status is kept when the previously associated segment is significantly
 * closer than the new one, even if the new segment's confidence is higher.
 * \req
 * NA
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment, Assign_Object_Drivable_Status_And_Conf_From_SG_Segment_Previous_Segment_Closer_Keeps_Status)
{
   /** \precond
    * An object and a segment vertex has been set up in the TEST_GROUP.
    * Set object's current drivability status to NONDRIVABLE with confidence 70% at dist 0.2 m^2.
   * New segment is at 1.5 m^2 - significantly farther (diff = -1.3 m^2 < -0.01 m^2 threshold).
    * New segment has higher confidence (80%) than the currently assigned one.
    */
   object.drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
   object.drivable_confidence_sg = 70U;
   object.drivable_sg_dist_to_segment_sq = 0.2F;
   const float32_t farther_dist_sq = 1.5F;

   /** \action
    * Call Assign_Object_Drivable_Status_And_Conf_From_SG_Segment().
    */
   Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(segment_drivability_status, segment_drivability_confidence, farther_dist_sq, object);

   /** \result
    * Check that object retains NONDRIVABLE with confidence 70% since the previously assigned segment is significantly closer.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object.drivable_status_sg, "Incorrect drivable status assined to object.");
   CHECK_EQUAL_TEXT(70U, object.drivable_confidence_sg, "Incorrect drivable confidence assined to object.")
   DOUBLES_EQUAL(0.2F, object.drivable_sg_dist_to_segment_sq, 1e-6F);
}

/** \purpose
 * Test that the new segment status is assigned when confidences are equal and the new segment is closer,
 * verifying that equal-confidence tie is resolved by picking the closest segment.
 * \req
 * NA
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Assign_Status_From_SG_Segment, Assign_Object_Drivable_Status_And_Conf_From_SG_Segment_Equal_Confidence_Closer_Assigns)
{
   /** \precond
    * An object and a segment vertex has been set up in the TEST_GROUP.
    * Set object's current drivability status to NONDRIVABLE with confidence 80% at dist 1.005 m^2.
    * New segment has the same confidence (80%) and distance 1.0 m^2.
   * Distances are similar (diff = 0.005 m^2, within +/-0.01 threshold), so equal-confidence tie
    * shall be resolved by selecting the strictly closer segment.
    */
   object.drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
   object.drivable_confidence_sg = 80U;
   object.drivable_sg_dist_to_segment_sq = 1.005F;

   /** \action
    * Call Assign_Object_Drivable_Status_And_Conf_From_SG_Segment().
    */
   Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(segment_drivability_status, segment_drivability_confidence, segment_dist_sq, object);

   /** \result
   * Check that object is assigned UNDERDRIVABLE with confidence 80% since equal-confidence tie picks closest segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNDERDRIVABLE == object.drivable_status_sg, "Incorrect drivable status assined to object.");
   CHECK_EQUAL_TEXT(80U, object.drivable_confidence_sg, "Incorrect drivable confidence assined to object.")
}

/** @}*/

/** \defgroup  f360_assign_underdrivability_status_to_tracks_sg_Create_Classification_Gate_From_Segment
 *  @{
 */

/** \brief
 * This test group tests the function that creates an association bounding box from a segment.
 */
TEST_GROUP(f360_assign_underdrivability_status_to_tracks_sg_Create_Classification_Gate_From_Segment)
{	
   // Declare common variables used within all tests in this test group.
   SG_Vertex_VCS prev_vertex = {};
   SG_Vertex_VCS curr_vertex = {};
   float32_t classification_gate_width;
   const float32_t test_thresh = 0.00001F;

   /** \setup
    * Set up two vertices that form a segment such that the segment with 0 orientation.
    */
   TEST_SETUP()
   {
      prev_vertex.vcs_position_x = 2.0F;
      prev_vertex.vcs_position_y = 1.0F;

      curr_vertex.vcs_position_x = 10.0F;
      curr_vertex.vcs_position_y = 1.0F;

      classification_gate_width = 2.0F;
   }
};

/** \purpose  
 * Test that the expected bounding box is created for a segment with 0 degrees orientation.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Create_Classification_Gate_From_Segment, Create_Classification_Gate_From_Segment_0_Orientation)
{
   /** \precond
    * Vertices have been set up in the TEST_GROUP.
    */
   const float32_t exp_bbox_center_x = 6.0F;
   const float32_t exp_bbox_center_y = 1.0F;
   const float32_t exp_bbox_width = 2.0F;
   const float32_t exp_bbox_length = 8.0F+2.0F; // 8m segment distance + 2m buffer
   const float32_t exp_bbox_orientation = 0.0F;
	
   /** \action
    * Call Create_Classification_Gate_From_Segment().
    */
   BoundingBox output_bbox = Create_Classification_Gate_From_Segment(prev_vertex, curr_vertex, classification_gate_width);

   /** \result
    * Check that the segment's bounding box has the expected properties.
    */
   DOUBLES_EQUAL_TEXT(exp_bbox_center_x, output_bbox.Get_Center().x, test_thresh, "Bounding box's x position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_center_y, output_bbox.Get_Center().y, test_thresh, "Bounding box's y position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_width, output_bbox.Get_Width(), test_thresh, "Bounding box's x position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_length, output_bbox.Get_Length(), test_thresh, "Bounding box's x position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_orientation, output_bbox.Get_Orientation().Value(), test_thresh, "Bounding box's x position is incorrect");
}

/** \purpose  
 * Test that the created bounding box has an extended length when the segment is short enough.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Create_Classification_Gate_From_Segment, Create_Classification_Gate_From_Segment_Ext_Bbox_Short_Segment)
{
   /** \precond
    * Vertices have been set up in the TEST_GROUP.
    * Change placement of vertex such that the length of segment to make it short enough to extend the box's length.
    */
   prev_vertex.vcs_position_x = 9.6F;

   const float32_t exp_bbox_center_x = 9.8F;
   const float32_t exp_bbox_center_y = 1.0F;
   const float32_t exp_bbox_width = 2.0F;
   const float32_t exp_bbox_length = 0.4F+2.0F; // 0.4m segment distance + 2m buffer
   const float32_t exp_bbox_orientation = 0.0F;
	
   /** \action
    * Call Create_Classification_Gate_From_Segment().
    */
   BoundingBox output_bbox = Create_Classification_Gate_From_Segment(prev_vertex, curr_vertex, classification_gate_width);

   /** \result
    * Check that the segment's bounding box has the expected properties.
    */
   DOUBLES_EQUAL_TEXT(exp_bbox_center_x, output_bbox.Get_Center().x, test_thresh, "Bounding box's x position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_center_y, output_bbox.Get_Center().y, test_thresh, "Bounding box's y position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_width, output_bbox.Get_Width(), test_thresh, "Bounding box's wdith is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_length, output_bbox.Get_Length(), test_thresh, "Bounding box's length is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_orientation, output_bbox.Get_Orientation().Value(), test_thresh, "Bounding box's orientation is incorrect");
}

/** \purpose  
 * Test that the expected bounding box is created for a segment with 180 degrees orientation.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Create_Classification_Gate_From_Segment, Create_Classification_Gate_From_Segment_180_Orientation)
{
   /** \precond
    * Vertices have been set up in the TEST_GROUP.
    * Switch positions for prev and curr vertices to make box 180 deg oriented.
    */
   prev_vertex.vcs_position_x = 10.0F;
   curr_vertex.vcs_position_x = 2.0F;

   const float32_t exp_bbox_center_x = 6.0F;
   const float32_t exp_bbox_center_y = 1.0F;
   const float32_t exp_bbox_width = 2.0F;
   const float32_t exp_bbox_length = 8.0F + 2.0F; // 8m segment distance + 2m buffer
   const float32_t exp_bbox_orientation = F360_DEG2RAD(180.0F);
	
   /** \action
    * Call Create_Classification_Gate_From_Segment().
    */
   BoundingBox output_bbox = Create_Classification_Gate_From_Segment(prev_vertex, curr_vertex, classification_gate_width);

   /** \result
    * Check that the segment's bounding box has the expected properties.
    */
   DOUBLES_EQUAL_TEXT(exp_bbox_center_x, output_bbox.Get_Center().x, test_thresh, "Bounding box's x position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_center_y, output_bbox.Get_Center().y, test_thresh, "Bounding box's y position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_width, output_bbox.Get_Width(), test_thresh, "Bounding box's width is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_length, output_bbox.Get_Length(), test_thresh, "Bounding box's length is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_orientation, output_bbox.Get_Orientation().Value(), test_thresh, "Bounding box's orientation is incorrect");
}

/** \purpose  
 * Test that the expected bounding box is created for a segment with 45 degrees orientation.
 * \req
 * NA.
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg_Create_Classification_Gate_From_Segment, Create_Classification_Gate_From_Segment_45_Orientation)
{
   /** \precond
    * Vertices have been set up in the TEST_GROUP.
    * Change positions of the vertices to create a segment with 45 degree orientation.
    */
   prev_vertex.vcs_position_x = 1.0F;
   prev_vertex.vcs_position_y = 1.0F;
   curr_vertex.vcs_position_x = 3.0F;
   curr_vertex.vcs_position_y = 3.0F;

   const float32_t exp_bbox_center_x = 2.0F;
   const float32_t exp_bbox_center_y = 2.0F;
   const float32_t exp_bbox_width = 2.0F;
   const float32_t exp_bbox_length = F360_Sqrtf(8.0F) + 2.0F; // sqrt(8) segment distance + 2m buffer
   const float32_t exp_bbox_orientation = F360_DEG2RAD(45.0F);
	
   /** \action
    * Call Create_Classification_Gate_From_Segment().
    */
   BoundingBox output_bbox = Create_Classification_Gate_From_Segment(prev_vertex, curr_vertex, classification_gate_width);

   /** \result
    * Check that the segment's bounding box has the expected properties.
    */
   DOUBLES_EQUAL_TEXT(exp_bbox_center_x, output_bbox.Get_Center().x, test_thresh, "Bounding box's x position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_center_y, output_bbox.Get_Center().y, test_thresh, "Bounding box's y position is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_width, output_bbox.Get_Width(), test_thresh, "Bounding box's width is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_length, output_bbox.Get_Length(), test_thresh, "Bounding box's length is incorrect");
   DOUBLES_EQUAL_TEXT(exp_bbox_orientation, output_bbox.Get_Orientation().Value(), test_thresh, "Bounding box's orientation is incorrect");
}

/** @}*/
