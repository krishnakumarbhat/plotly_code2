/** \file
 * This file contains unit tests for content of f360_is_reflective_guardrail_track.cpp file
 */

#include "f360_is_reflective_guardrail_track.h"
#include "f360_static_env_polys_support_functions.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_is_reflective_guardrail_track
 *  @{
 */

 /** \brief
  * Test group of Is_Reflective_Guardrail_Track() function. Tests verify whether
  * objects are properly analysed whether they are reflected from guardrail
  */
TEST_GROUP(f360_is_reflective_guardrail_track)
{

   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   int32_t ghost_candidate_idx = 0;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T calib = {};
   F360_Host_T host = {};
   F360_Globals_T globals = {};

   F360_Object_Track_T& ghost_obj = object_tracks[0];
   F360_Object_Track_T& source_obj = object_tracks[1];
   F360_Object_Track_T& guardrail_obj1 = object_tracks[2];
   F360_Object_Track_T& guardrail_obj2 = object_tracks[3];
   F360_Object_Track_T& guardrail_obj3 = object_tracks[4];

   /** \setup
    * Initialize tracker calibrations
    * Setp up two SEP's
    * Set up source candidate object so that it is a valid source object 
    * that can yield a reflection in SEP id 1.
    * Set up ghost candidate so that it is a valid reflection of the
    * source object in SEP id 1.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      float32_t host_curvature_rear = 0.000001F;

      tracker_info.num_active_objs = 5;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
      tracker_info.active_obj_ids[3] = 4;
      tracker_info.active_obj_ids[4] = 5;

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

      ghost_obj.vcs_position.x = 10.0F;
      ghost_obj.vcs_position.y = -7.5F;
      ghost_obj.predicted_vcs_position.x = 10.0F;
      ghost_obj.predicted_vcs_position.y = -7.5F;
      ghost_obj.Update_Bbox_Size(1.0F, 1.0F);
      ghost_obj.Set_Bbox_Orientation(Angle{ 0.0F });
      ghost_obj.bbox.Set_Center(Point{ 10.0F, -7.5F });
      ghost_obj.f_moving = true;
      ghost_obj.movable_prob = 1.0F;
      ghost_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      ghost_obj.status = F360_OBJECT_STATUS_UPDATED;
      ghost_obj.reduced_id = 1;
      ghost_obj.speed = 1.0F;
      ghost_obj.id = 1;
      ghost_obj.vcs_heading = Angle{ 0.0F };

      source_obj.vcs_position.x = 10.0F;
      source_obj.vcs_position.y = -2.5F;
      source_obj.predicted_vcs_position.x = 10.0F;
      source_obj.predicted_vcs_position.y = -2.5F;
      source_obj.Update_Bbox_Size(1.0F, 1.0F);
      source_obj.Set_Bbox_Orientation(Angle{ 0.0F });
      source_obj.bbox.Set_Center(Point{ 10.0F, -2.5F });
      source_obj.f_moving = true;
      source_obj.movable_prob = 1.0F;
      source_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      source_obj.status = F360_OBJECT_STATUS_UPDATED;
      source_obj.reduced_id = 2;
      source_obj.speed = 1.0F;
      source_obj.id = 2;
      source_obj.vcs_heading = Angle{ 0.0F };

      // Guardrail objects (stationary CCA)
      guardrail_obj1.id = 3;
      guardrail_obj1.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      guardrail_obj1.movable_prob = 0.0F;
      guardrail_obj1.status = F360_OBJECT_STATUS_UPDATED;
      guardrail_obj1.otg_height = 1.0F;
      guardrail_obj1.vcs_position.x = 4.0F;
      guardrail_obj1.vcs_position.y = -5.0F;
      guardrail_obj1.predicted_vcs_position.x = 4.0F;
      guardrail_obj1.predicted_vcs_position.y = -5.0F;
      guardrail_obj1.bbox.Set_Center(Point{ 4.0F, -5.0F });
      guardrail_obj1.bbox.Set_Orientation(Angle{ 0.0F });

      guardrail_obj2.id = 4;
      guardrail_obj2.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      guardrail_obj2.movable_prob = 0.0F;
      guardrail_obj2.status = F360_OBJECT_STATUS_UPDATED;
      guardrail_obj2.otg_height = 1.0F;
      guardrail_obj2.vcs_position.x = 5.0F;
      guardrail_obj2.vcs_position.y = -5.0F;
      guardrail_obj2.predicted_vcs_position.x = 5.0F;
      guardrail_obj2.predicted_vcs_position.y = -5.0F;
      guardrail_obj2.bbox.Set_Center(Point{ 5.0F, -5.0F });
      guardrail_obj2.bbox.Set_Orientation(Angle{ 0.0F });

      guardrail_obj3.id = 5;
      guardrail_obj3.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      guardrail_obj3.movable_prob = 0.0F;
      guardrail_obj3.status = F360_OBJECT_STATUS_UPDATED;
      guardrail_obj3.otg_height = 1.0F;
      guardrail_obj3.vcs_position.x = 6.0F;
      guardrail_obj3.vcs_position.y = -5.0F;
      guardrail_obj3.predicted_vcs_position.x = 6.0F;
      guardrail_obj3.predicted_vcs_position.y = -5.0F;
      guardrail_obj3.bbox.Set_Center(Point{ 6.0F, -5.0F });
      guardrail_obj3.bbox.Set_Orientation(Angle{ 0.0F });

      Flag_Single_Object_Behind_SEP(sep, calib, globals, ghost_obj);
      Flag_Single_Object_Behind_SEP(sep, calib, globals, source_obj);
   }
};

/** \purpose
 * Purpose of this test is to verify that object is flagged as a
 * SEP mirror when all conditions are fulfilled
 * \req
 * NA.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Object_Is_Mirror)
{
   /** \precond
    * In test group data have been set up so that all conditions are fulfilled
    */

    /** \action
     * Call tested function
     */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Verify function returns false if ghost candidate is valid, source candidate is valid, but neither SEP nor no-SEP reflection is detected.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__No_SEP_No_NoSEP_Reflection)
{
   /** \precond
    * Ghost and source candidates are valid, but ghost_obj.behind_sep_id = F360_INVALID_UNSIGNED_ID,
    * and ghost_obj.mirror_prob <= 0.6F (so no-SEP check is skipped).
    */
   ghost_obj.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   ghost_obj.mirror_prob = 0.59F;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns true if SEP reflection is detected and mirror_prob/cntHostTurnForMirrorProb are updated.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__SEP_Reflection_Updates_MirrorProb)
{
   /** \precond
    * Ghost and source candidates are valid, ghost_obj.behind_sep_id is set,
    * and Is_Ghost_Reflected_By_SEP will return true (default setup).
    */
   ghost_obj.behind_sep_id = 1U;
   ghost_obj.mirror_prob = 0.0F;
   ghost_obj.cntHostTurnForMirrorProb = 42;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true and mirror_prob/cntHostTurnForMirrorProb are updated.
    */
   CHECK_TRUE(f_reflection);
   CHECK_EQUAL(1.0F, ghost_obj.mirror_prob);
   CHECK_EQUAL(0, ghost_obj.cntHostTurnForMirrorProb);
}

/** \purpose
 * Verify function returns true if no-SEP reflection is detected and mirror_prob/cntHostTurnForMirrorProb are updated.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__NoSEP_Reflection_Updates_MirrorProb)
{
   /** \precond
    * Ghost and source candidates are valid, ghost_obj.behind_sep_id = F360_INVALID_UNSIGNED_ID,
    * ghost_obj.mirror_prob > 0.6F, and Is_Ghost_Reflected_Without_SEP will return true.
    */
   ghost_obj.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   ghost_obj.mirror_prob = 0.7F;
   ghost_obj.cntHostTurnForMirrorProb = 42;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true and mirror_prob/cntHostTurnForMirrorProb are updated.
    */
   CHECK_TRUE(f_reflection);
   CHECK_EQUAL(1.0F, ghost_obj.mirror_prob);
   CHECK_EQUAL(0, ghost_obj.cntHostTurnForMirrorProb);
}

/** \purpose
 * Verify function returns true if first source candidate fails, but second is a match.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Second_Source_Candidate_Matches)
{
   /** \precond
    * Add a second source candidate that is valid and will match, first is not valid.
    */
   F360_Object_Track_T& bad_source = object_tracks[1];
   F360_Object_Track_T& good_source = object_tracks[2];
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   // First source is not valid
   bad_source.f_moving = false;

   // Second source is valid and matches (copy from original source_obj)
   good_source = object_tracks[1];
   good_source.f_moving = true;
   good_source.id = 3;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true.
    */
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Purpose of this test is to verify that object is not flagged as a
 * SEP mirror when all conditions are fulfilled except that the ghost
 * object is not moving
 * \req
 * NA.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Object_Is_Not_Candidate_For_Check)
{
   /** \precond
    * In test group data have been set up so that all conditions are fulfilled
    * Set object moving flag to false
    */
   ghost_obj.f_moving = false;

    /** \action
     * Call tested function
     */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true
    */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns false if source candidate is behind SEP .
 * Ghost is behind SEP, but source is also behind SEP, so SEP reflection check should be skipped.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Source_Candidate_Behind_SEP)
{
   /** \precond
    * Ghost candidate is behind SEP (behind_sep_id = 1U),
    * Source candidate is also behind SEP (behind_sep_id = 1U).
    */
   ghost_obj.behind_sep_id = 1U;
   source_obj.behind_sep_id = 1U;  // Source is behind SEP - should prevent SEP reflection check
   ghost_obj.mirror_prob = 0.5F;  // Set low mirror_prob to prevent no-SEP check as well

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is false since source candidate is behind SEP.
    */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns false if source candidate is behind a different SEP than ghost candidate.
 * Both are behind SEPs, so SEP reflection check should be skipped.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Source_Behind_Different_SEP)
{
   /** \precond
    * Ghost candidate is behind SEP 1 (behind_sep_id = 1U),
    * Source candidate is behind SEP 2 (behind_sep_id = 2U).
    */
   ghost_obj.behind_sep_id = 1U;
   source_obj.behind_sep_id = 2U;  // Source is behind different SEP - should prevent SEP reflection check
   ghost_obj.mirror_prob = 0.5F;  // Set low mirror_prob to prevent no-SEP check

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is false since source candidate is behind a SEP.
    */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function proceeds to SEP check when ghost is behind SEP and source is NOT behind SEP.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Ghost_Behind_SEP_Source_Not_Behind_SEP)
{
   /** \precond
    * Ghost candidate is behind SEP (behind_sep_id = 1U),
    * Source candidate is NOT behind SEP (behind_sep_id = F360_INVALID_UNSIGNED_ID).
    */
   ghost_obj.behind_sep_id = 1U;
   source_obj.behind_sep_id = F360_INVALID_UNSIGNED_ID;  // Source is NOT behind SEP - SEP check should proceed
   ghost_obj.mirror_prob = 0.5F;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true since SEP reflection check proceeds and conditions are met.
    */
   CHECK_TRUE(f_reflection);
   CHECK_EQUAL(1.0F, ghost_obj.mirror_prob);
   CHECK_EQUAL(0, ghost_obj.cntHostTurnForMirrorProb);
}
/** @}*/
/** \purpose
 * Purpose of this test is to verify that object is flagged as a
 * mirror when all conditions are fulfilled and object is oncoming
 * \req
 * NA.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Object_Is_Oncoming_Mirror)
{
   /** \precond
    * In test group data have been set up so that all conditions are fulfilled
    */
   source_obj.f_oncoming = true;
   ghost_obj.f_oncoming = true;

    /** \action
     * Call tested function
     */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(f_reflection);
}
/** \purpose
 * Purpose of this test is to verify that object is not flagged as a
 * mirror when all conditions are not fulfilled and object is oncoming
 * \req
 * NA.
 */
TEST(f360_is_reflective_guardrail_track, Is_Reflective_Guardrail_Track__Object_Is_Not_Oncoming_Mirror)
{
   /** \precond
    * In test group data have been set up so that all conditions are fulfilled
    */
   source_obj.f_oncoming = true;
   ghost_obj.f_oncoming = true;
   source_obj.speed = 30.0F;


    /** \action
     * Call tested function
     */
   bool f_reflection = Is_Reflective_Guardrail_Track(tracker_info, ghost_candidate_idx, sep, calib, host, object_tracks);

   /** \result
    * Check whether returned value is true
    */
   CHECK_FALSE(f_reflection);
}
/** \defgroup  f360_Is_Hypot_Source_Pos_In_Source_Candidate_BBox
 *  @{
 */

 /** \brief
  * Test group related to tests of function f360_Is_Hypot_Source_Pos_In_Source_Candidate_BBox() function.
  * Data is set up in such a way that function should return true.
  */
TEST_GROUP(f360_Is_Hypot_Source_Pos_In_Source_Candidate_BBox)
{
   Point hypothetic_source_pos = {};
   F360_Object_Track_T source_candidate = { };
   F360_Object_Track_T ghost_candidate = { };
   F360_Calibrations_T calibs = { };

   /** \setup
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      hypothetic_source_pos.x = 0.0F;
      hypothetic_source_pos.y = 0.0F;

      ghost_candidate.vcs_position.y = 5.0;
      ghost_candidate.vcs_position.x = 0.0F;
      ghost_candidate.Set_Bbox_Orientation(Angle{ 0.0F });
      ghost_candidate.Update_Bbox_Size(1.0F, 1.0F);

      source_candidate.vcs_position.y = 0.0;
      source_candidate.vcs_position.x = 0.0F;
      source_candidate.Set_Bbox_Orientation(Angle{ 0.0F });
      source_candidate.Update_Bbox_Size(1.0F, 1.0F);
   }
};

/** \purpose
 * Purpose of this test is to verify that function returns true when hypothetic source position
 * matches source candidate position.
 * \req
 * NA.
 */
TEST(f360_Is_Hypot_Source_Pos_In_Source_Candidate_BBox, Is_Hypot_Source_Pos_In_Source_Candidate_BBox__Hypothetic_Source_Pos_Matches_Source_Candidate)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    */

    /** \action
     * Call function Is_Hypot_Source_Pos_In_Source_Candidate_BBox()
     */
   const bool f_matching = Is_Hypot_Source_Pos_In_Source_Candidate_BBox(
      hypothetic_source_pos,
      source_candidate,
      ghost_candidate,
      calibs);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns true when hypothetic source position
 * not matches source candidate position.
 * \req
 * NA.
 */
TEST(f360_Is_Hypot_Source_Pos_In_Source_Candidate_BBox, Is_Hypot_Source_Pos_In_Source_Candidate_BBox__Hypothetic_Source_Pos_Not_Matches_Source_Candidate)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set hypothetic source position far away from source candidate position
    */
   hypothetic_source_pos.x = 10.0F;
   hypothetic_source_pos.y = 10.0F;

    /** \action
     * Call function Is_Hypot_Source_Pos_In_Source_Candidate_BBox()
     */
   const bool f_matching = Is_Hypot_Source_Pos_In_Source_Candidate_BBox(
      hypothetic_source_pos,
      source_candidate,
      ghost_candidate,
      calibs);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(f_matching);
}
/** @}*/

/** \defgroup  f360_Is_Source_Candidate_Similar_To_Ghost_Candidate
 *  @{
 */

 /** \brief
  * Test group related to tests of function Is_Source_Candidate_Similar_To_Ghost_Candidate() function.
  * Data is set up in such a way that function should return true.
  */
TEST_GROUP(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate)
{
   Point hypothetic_source_pos = {};
   F360_Object_Track_T source_candidate = { };
   F360_Object_Track_T ghost_candidate = { };
   F360_Calibrations_T calibs = { };

   /** \setup
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      hypothetic_source_pos.x = 0.0F;
      hypothetic_source_pos.y = 0.0F;

      ghost_candidate.vcs_position.y = 5.0F;
      ghost_candidate.vcs_position.x = 0.0F;
      ghost_candidate.Set_Bbox_Orientation(Angle{ 0.0F });
      ghost_candidate.speed = 10.0F;
      ghost_candidate.vcs_heading = Angle{ 0.0F };
      ghost_candidate.Update_Bbox_Size(1.0F, 1.0F);

      source_candidate.vcs_position.y = 0.0;
      source_candidate.vcs_position.x = 0.0F;
      source_candidate.Set_Bbox_Orientation(Angle{ 0.0F });
      source_candidate.speed = 10.0F;
      source_candidate.vcs_heading = Angle{ 0.0F };
      source_candidate.Update_Bbox_Size(1.0F, 1.0F);
   }
};

/** \purpose
 * Purpose of this test is to verify that function returns true when hypothetic source position
 * matches source candidate position.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Matching)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    */

    /** \action
     * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
     */
   const float32_t sep_tangent_angle = 0.0F;
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns false when hypothetic source position
 * matches source candidate position but source object speed is too high.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_Source_Speed_Too_High)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    * 
    * Set source object speed different
    */
   source_candidate.speed = 100.0F;

    /** \action
     * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
     */
   const float32_t sep_tangent_angle = 0.0F;
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is true
    */
   CHECK_FALSE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns false when hypothetic source position
 * matches source candidate position but source object speed is too low
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_Source_Speed_Too_Low)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    *
    * Set source object speed low
    */
   source_candidate.speed = 0.0F;

   /** \action
    * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
    */
   const float32_t sep_tangent_angle = 0.0F;
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is true
    */
   CHECK_FALSE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns true when hypothetic source position
 * matches source candidate position. Heading of source object is large so a different threshold for
 * position is used.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_Source_Heading_Large)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    *
    * Set source object heading large
    * Set ghost object heading large
    */
   source_candidate.vcs_heading = Angle{ F360_DEG2RAD(90.0F) };
   ghost_candidate.vcs_heading = Angle{ F360_DEG2RAD(90.0F) };

   /** \action
    * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
    */
   // SEP tangent aligned with both headings -> mirror reflection condition is satisfied (source + ghost in tangent frame sum to 0)
   const float32_t sep_tangent_angle = F360_DEG2RAD(90.0F);
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns false when hypothetic source position
 * not matches source candidate position in lateral direction.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_Source_Lat_Pos_Not_Matching)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    *
    * Set source object lateral position different
    */
   source_candidate.vcs_position.y = -10.0F;
   source_candidate.Update_Bbox_Center();

   /** \action
    * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
    */
   const float32_t sep_tangent_angle = 0.0F;
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns false when hypothetic source position
 * not matches source candidate in heading.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_Source_Heading_Not_Matching)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    *
    * Set source object heading different
    */
   source_candidate.vcs_heading = Angle{ F360_DEG2RAD(90.0F) };

   /** \action
    * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
    */
   const float32_t sep_tangent_angle = 0.0F;
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns false when hypothetic source position
 * not matches source candidate in longitudinal position.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_Source_Long_Pos_Not_Matching)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    * Set up valid speed and vcs heading for both source and ghost object
    *
    * Set source object long position different
    */
   source_candidate.vcs_position.x = 50.0F;
   source_candidate.Update_Bbox_Center();

   /** \action
    * Call function Is_Hypot_Source_Pos_In_Sourc_Candidate_BBox()
    */
   const float32_t sep_tangent_angle = 0.0F;
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(f_matching);
}

/** \purpose
 * Purpose of this test is to verify that function returns false when source and ghost
 * candidates pass all legacy similarity checks but their headings are not consistent
 * with a mirror reflection about the SEP tangent line at the reflection point.
 * \req
 * NA.
 */
TEST(f360_Is_Source_Candidate_Similar_To_Ghost_Candidate, Is_Source_Candidate_Similar_To_Ghost_Candidate__Source_And_Ghost_Not_Matching_Due_To_SEP_Reflection_Heading_Inconsistent)
{
   /** \precond
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up valid dimension, orientation, speed and heading for both source and ghost object
    *
    * Set SEP tangent angle to 45 deg so that reflected ghost heading should be -90 deg
    * but actual ghost heading is 0 deg, breaking the SEP reflection heading consistency check
    */
   const float32_t sep_tangent_angle = F360_DEG2RAD(45.0F);

   /** \action
    * Call function Is_Source_Candidate_Similar_To_Ghost_Candidate()
    */
   const bool f_matching = Is_Source_Candidate_Similar_To_Ghost_Candidate(
      source_candidate,
      ghost_candidate,
      hypothetic_source_pos,
      sep_tangent_angle,
      calibs);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(f_matching);
}
/** @}*/

/** \defgroup  f360_Is_Heading_Consistent_With_SEP_Reflection
 *  @{
 */

 /** \brief
  * Test group related to tests of function Is_Heading_Consistent_With_SEP_Reflection().
  * Verifies that the function correctly determines whether source and ghost headings
  * are consistent with a mirror reflection about the SEP tangent line.
  */
TEST_GROUP(f360_Is_Heading_Consistent_With_SEP_Reflection)
{
   F360_Calibrations_T calib = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Verify that headings perfectly mirrored about a tangent aligned with the VCS x-axis
 * are reported as consistent.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Perfect_Mirror_Tangent_Zero)
{
   const float32_t source_heading = F360_DEG2RAD(30.0F);
   const float32_t ghost_heading  = F360_DEG2RAD(-30.0F);
   const float32_t tangent_angle  = 0.0F;
   const float32_t tolerance      = F360_DEG2RAD(5.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_TRUE(f_consistent);
}

/** \purpose
 * Verify that headings perfectly mirrored about an arbitrary, non-zero tangent angle
 * are reported as consistent.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Perfect_Mirror_Tangent_NonZero)
{
   // Tangent at 45 deg, source at 60 deg -> source_local = 15 deg
   // Mirror requires ghost_local = -15 deg -> ghost = 30 deg
   const float32_t source_heading = F360_DEG2RAD(60.0F);
   const float32_t ghost_heading  = F360_DEG2RAD(30.0F);
   const float32_t tangent_angle  = F360_DEG2RAD(45.0F);
   const float32_t tolerance      = F360_DEG2RAD(1.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_TRUE(f_consistent);
}

/** \purpose
 * Verify that the function returns false when the deviation from a perfect mirror
 * exceeds the supplied tolerance.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Heading_Deviation_Above_Tolerance)
{
   // Source at 30 deg, mirrored ghost should be at -30 deg, but ghost is set to 0 deg
   const float32_t source_heading = F360_DEG2RAD(30.0F);
   const float32_t ghost_heading  = 0.0F;
   const float32_t tangent_angle  = 0.0F;
   const float32_t tolerance      = F360_DEG2RAD(10.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_FALSE(f_consistent);
}

/** \purpose
 * Verify that a heading deviation right at the tolerance boundary is reported as consistent.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Heading_At_Tolerance_Boundary)
{
   // |theta_ghost_local + theta_source_local| = 5 deg, tolerance = 5 deg -> consistent
   const float32_t source_heading = F360_DEG2RAD(20.0F);
   const float32_t ghost_heading  = F360_DEG2RAD(-15.0F);
   const float32_t tangent_angle  = 0.0F;
   const float32_t tolerance      = F360_DEG2RAD(5.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_TRUE(f_consistent);
}

/** \purpose
 * Verify that the function correctly handles the wrap-around at +/-pi by passing
 * angles whose raw sum is outside [-pi, pi] but whose normalized sum is within tolerance.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Wraparound_Normalization)
{
   // source_local = 170 deg, ghost_local = -170 deg -> raw sum = 0 -> consistent
   const float32_t source_heading = F360_DEG2RAD(170.0F);
   const float32_t ghost_heading  = F360_DEG2RAD(-170.0F);
   const float32_t tangent_angle  = 0.0F;
   const float32_t tolerance      = F360_DEG2RAD(5.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_TRUE(f_consistent);
}

/** \purpose
 * Verify that two parallel (non-mirrored) headings about a non-aligned tangent line
 * are reported as inconsistent.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Parallel_Headings_Off_Tangent)
{
   // Both headings at 0 deg, tangent at 30 deg -> source_local + ghost_local = -60 deg
   const float32_t source_heading = 0.0F;
   const float32_t ghost_heading  = 0.0F;
   const float32_t tangent_angle  = F360_DEG2RAD(30.0F);
   const float32_t tolerance      = F360_DEG2RAD(10.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_FALSE(f_consistent);
}

/** \purpose
 * Verify that two parallel headings whose direction is aligned with the tangent
 * (so both local headings are 0) are reported as consistent.
 * \req
 * NA.
 */
TEST(f360_Is_Heading_Consistent_With_SEP_Reflection, Is_Heading_Consistent_With_SEP_Reflection__Headings_Along_Tangent)
{
   const float32_t source_heading = F360_DEG2RAD(45.0F);
   const float32_t ghost_heading  = F360_DEG2RAD(45.0F);
   const float32_t tangent_angle  = F360_DEG2RAD(45.0F);
   const float32_t tolerance      = F360_DEG2RAD(1.0F);

   const bool f_consistent = Is_Heading_Consistent_With_SEP_Reflection(
      source_heading, ghost_heading, tangent_angle, tolerance);

   CHECK_TRUE(f_consistent);
}
/** @}*/

/** \defgroup  f360_Calculate_Center_Of_Symmetry_LSC
 *  @{
 */

 /** \brief
  * Test group related to tests of function Calculate_Center_Of_Symmetry_LSC() function.
  * Data is set up in such a way that function should return true.
  */
TEST_GROUP(f360_Calculate_Center_Of_Symmetry_LSC)
{
   Point ghost_cand_pos = {};
   Point point_of_reflection = {};
   Static_Env_Poly_T sep = {};

   float32_t test_pass_thres = 0.0001F;

   /** \setup
    * Initialize tracker calibrations
    * Set up hypothetic source position equal to source candidate object position
    * Set up arbitrary valid dimension and orientation of both source and ghost object
    */
   TEST_SETUP()
   {
      ghost_cand_pos.x = 0.0F;
      ghost_cand_pos.y = 4.0F;

      point_of_reflection.x = 0.0F;
      point_of_reflection.y = 2.0F;

      sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep.p0 = 2.0F;
      sep.p1 = 0.0F;
      sep.p2 = 0.0F;
      sep.lower_limit = -20.0F;
      sep.upper_limit = 20.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify that function returns correct center of symmetry
 * when SEP slope is zero
 * \req
 * NA.
 */
TEST(f360_Calculate_Center_Of_Symmetry_LSC, Calculate_Center_Of_Symmetry_LSC__Tangent_Slope_Zero)
{
   /** \precond
    * Ghost candidate VCS position is (0,4)
    * Point of reflection VCS position is (0,2)
    * SEP is a straight line at lateral VCS position 2
    */

   /** \action
   * Call function Calculate_Center_Of_Symmetry_LSC()
   */
   Point vcs_pos = Calculate_Center_Of_Symmetry_SEP(
      ghost_cand_pos,
      point_of_reflection,
      sep);

   /** \result
    * Check that center of symmetry matches expectation
    */
   DOUBLES_EQUAL(0.0F, vcs_pos.x, test_pass_thres);
   DOUBLES_EQUAL(2.0F, vcs_pos.y, test_pass_thres);
}

/** \purpose
 * Purpose of this test is to verify that function returns correct center of symmetry
 * when SEP slope is not zero
 * \req
 * NA.
 */
TEST(f360_Calculate_Center_Of_Symmetry_LSC, Calculate_Center_Of_Symmetry_LSC__Tangent_Slope_Not_Zero)
{
   /** \precond
    * Ghost candidate VCS position is (10,4)
    * Point of reflection VCS position is (8,2)
    * SEP is a curved line
    */
   ghost_cand_pos.x = 10.0F;
   ghost_cand_pos.y = 4.0F;

   point_of_reflection.x = 8.0F;
   point_of_reflection.y = 2.0F;

   sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep.p0 = 3.0F;
   sep.p1 = 0.0F;
   sep.p2 = 0.5F;
   sep.lower_limit = -20.0F;
   sep.upper_limit = 20.0F;

    /** \action
    * Call function Calculate_Center_Of_Symmetry_LSC()
    */
   Point vcs_pos = Calculate_Center_Of_Symmetry_SEP(
      ghost_cand_pos,
      point_of_reflection,
      sep);

   /** \result
    * Check that center of symmetry matches expectation
    */
   DOUBLES_EQUAL(4.21538448F, vcs_pos.x, test_pass_thres);
   DOUBLES_EQUAL(4.72307682F, vcs_pos.y, test_pass_thres);
}
/** @}*/

/** \defgroup  f360_Is_Ghost_Reflected_By_SEP
 *  @{
 */

 /** \brief
  * Test group of Is_Ghost_Reflected_By_SEP() function. 
  * Data is set up in such a way that the ghost candidate should be
  * flagged a reflection in the SEP
  */
TEST_GROUP(f360_Is_Ghost_Reflected_By_SEP)
{

   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T ghost_obj = {};
   F360_Object_Track_T source_obj = {};
   F360_Globals_T globals = {};

   Point ghost_cand_pos = {};


   /** \setup
    * Initialize tracker calibrations
    * Setp up two SEP's
    * Set up base object properties
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      float32_t host_curvature_rear = 0.000001F;

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

      ghost_obj.vcs_position.y = -7.5F;
      ghost_obj.vcs_position.x = 10.0F;
      ghost_obj.Set_Bbox_Orientation(Angle{ 0.0F });
      ghost_obj.f_moving = true;
      ghost_obj.movable_prob = 1.0F;
      ghost_obj.Update_Bbox_Size(1.0F, 1.0F);
      ghost_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      ghost_obj.status = F360_OBJECT_STATUS_UPDATED;
      ghost_obj.reduced_id = 1;
      ghost_obj.speed = 1.0F;

      ghost_cand_pos = ghost_obj.bbox.Get_Center();

      source_obj.vcs_position.y = -2.5F;
      source_obj.vcs_position.x = 10.0F;
      source_obj.Set_Bbox_Orientation(Angle{ 0.0F });
      source_obj.f_moving = true;
      source_obj.movable_prob = 1.0F;
      source_obj.Update_Bbox_Size(1.0F, 1.0F);
      source_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      source_obj.status = F360_OBJECT_STATUS_UPDATED;
      source_obj.reduced_id = 2;
      source_obj.speed = 1.0F;

      Flag_Single_Object_Behind_SEP(sep, calib, globals, ghost_obj);
      Flag_Single_Object_Behind_SEP(sep, calib, globals, source_obj);
   }
};

/** \purpose
 * Purpose of this test is to verify that ghost object is flagged as a
 * SEP mirror given that all conditions are fulfilled as is set up in the test group
 * (i.e. source cantidate is a CTCA object)
 * \req
 * NA.
 */
TEST(f360_Is_Ghost_Reflected_By_SEP, Is_Ghost_Reflected_By_SEP__Candidate_Is_SEP_Mirror__CTCA_Souce_Canditiate)
{
   /** \precond
   * Data have been set up in test group so
   * that ghost object is expected to be flagged
   * as a SEP mirror
   */

   /** \action
   * Call tested function
   */
   bool f_reflection = Is_Ghost_Reflected_By_SEP(
      sep[0],
      ghost_obj,
      ghost_cand_pos,
      source_obj,
      calib);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Purpose of this test is to verify that ghost object is not flagged as a
 * SEP mirror since ghost candidate intersection with SEP is below valid
 * calibration interval to activate countermeasure
 * \req
 * NA.
 */
TEST(f360_Is_Ghost_Reflected_By_SEP, Is_Ghost_Reflected_By_SEP__Candidate_Is_Not_SEP_Mirror_Due_To_Ghost_SEP_Intersection_Too_Low)
{
   /** \precond
   * Data have been set up in test group so
   * that ghost object is expected to be flagged
   * as a SEP mirror
   * Set ghost object longitudinal SEP intersection below valid calibration
   */
   ghost_obj.sep_intersection_point.x = calib.k_tv_refl_gr_trk_min_sep_lon_pos - 1.0F;

   /** \action
   * Call tested function
   */
   bool f_reflection = Is_Ghost_Reflected_By_SEP(
      sep[0],
      ghost_obj,
      ghost_cand_pos,
      source_obj,
      calib);

   /** \result
   * Check whether returned value is true
   */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Purpose of this test is to verify that ghost object is not flagged as a
 * SEP mirror since ghost candidate intersection with SEP is above valid
 * calibration interval to activate countermeasure
 * \req
 * NA.
 */
TEST(f360_Is_Ghost_Reflected_By_SEP, Is_Ghost_Reflected_By_SEP__Candidate_Is_Not_SEP_Mirror_Due_To_Ghost_SEP_Intersection_Too_High)
{
   /** \precond
   * Data have been set up in test group so
   * that ghost object is expected to be flagged
   * as a SEP mirror
   * Set ghost object longitudinal SEP intersection above valid calibration
   */
   ghost_obj.sep_intersection_point.x = calib.k_tv_refl_gr_trk_max_sep_lon_pos + 1.0F;

   /** \action
   * Call tested function
   */
   bool f_reflection = Is_Ghost_Reflected_By_SEP(
      sep[0],
      ghost_obj,
      ghost_cand_pos,
      source_obj,
      calib);

   /** \result
   * Check whether returned value is true
   */
   CHECK_FALSE(f_reflection);
}
/** @}*/

/** \defgroup  f360_Is_Ghost_Obj_Valid_For_Reflection_Check
 *  @{
 */

 /** \brief
  * Test group for Is_Ghost_Obj_Valid_For_Reflection_Check() function.
  * Verifies if ghost candidate is valid for guardrail reflection check.
  */
TEST_GROUP(f360_Is_Ghost_Obj_Valid_For_Reflection_Check)
{
   F360_Calibrations_T calib;
   F360_Object_Track_T ghost_candidate = {};

   /** \setup
    * Initialize tracker calibrations and set up ghost candidate defaults.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      ghost_candidate.speed = 1.0F;
      ghost_candidate.f_moving = true;
   }
};

/** \purpose
 * Verify function returns true for CTCA type and moving ghost candidate.
 */
TEST(f360_Is_Ghost_Obj_Valid_For_Reflection_Check, Is_Ghost_Obj_Valid_For_Reflection_Check__CTCA_Moving)
{
   /** \precond
    * CTCA filter type, moving, speed above zero.
    */
   ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   ghost_candidate.speed = 1.0F;
   ghost_candidate.f_moving = true;

   /** \action */
   bool f_valid = Is_Ghost_Obj_Valid_For_Reflection_Check(ghost_candidate, calib);

   /** \result */
   CHECK_TRUE(f_valid);
}

/** \purpose
 * Verify function returns true for fast moving CCA ghost candidate.
 */
TEST(f360_Is_Ghost_Obj_Valid_For_Reflection_Check, Is_Ghost_Obj_Valid_For_Reflection_Check__Fast_Moving_CCA)
{
   /** \precond
    * CCA filter type, moving, speed above fast_moving_thresh.
    */
   ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   ghost_candidate.speed = calib.fast_moving_thresh + 1e-3F;
   ghost_candidate.f_moving = true;

   /** \action */
   bool f_valid = Is_Ghost_Obj_Valid_For_Reflection_Check(ghost_candidate, calib);

   /** \result */
   CHECK_TRUE(f_valid);
}

/** \purpose
 * Verify function returns false for slow moving CCA ghost candidate.
 */
TEST(f360_Is_Ghost_Obj_Valid_For_Reflection_Check, Is_Ghost_Obj_Valid_For_Reflection_Check__Slow_Moving_CCA)
{
   /** \precond
    * CCA filter type, moving, speed below fast_moving_thresh.
    */
   ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   ghost_candidate.speed = calib.fast_moving_thresh - 1e-3F;
   ghost_candidate.f_moving = true;

   /** \action */
   bool f_valid = Is_Ghost_Obj_Valid_For_Reflection_Check(ghost_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}

/** \purpose
 * Verify function returns false if ghost candidate is not moving.
 */
TEST(f360_Is_Ghost_Obj_Valid_For_Reflection_Check, Is_Ghost_Obj_Valid_For_Reflection_Check__Not_Moving)
{
   /** \precond
    * CTCA filter type, not moving.
    */
   ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   ghost_candidate.speed = 1.0F;
   ghost_candidate.f_moving = false;

   /** \action */
   bool f_valid = Is_Ghost_Obj_Valid_For_Reflection_Check(ghost_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}
/** @} */

/** \defgroup  f360_Is_Source_Obj_Valid_For_Reflection_Check
 *  @{
 */

 /** \brief
  * Test group for Is_Source_Obj_Valid_For_Reflection_Check() function.
  * Verifies if source candidate is valid for guardrail reflection check.
  */
TEST_GROUP(f360_Is_Source_Obj_Valid_For_Reflection_Check)
{
   F360_Calibrations_T calib;
   F360_Object_Track_T ghost_candidate = {};
   F360_Object_Track_T source_candidate = {};

   /** \setup
    * Initialize tracker calibrations and set up default ghost/source candidates.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      ghost_candidate.id = 1;
      ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      ghost_candidate.speed = 1.0F;
      ghost_candidate.f_moving = true;

      source_candidate.id = 2;
      source_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      source_candidate.speed = 1.0F;
      source_candidate.f_moving = true;
      source_candidate.on_sep_id = F360_INVALID_UNSIGNED_ID;
      source_candidate.status = F360_OBJECT_STATUS_UPDATED;
      source_candidate.reduced_id = 1;
      source_candidate.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   }
};

/** \purpose
 * Verify function returns true for valid CTCA source candidate.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__CTCA_Valid)
{
   /** \precond
    * All conditions for valid CTCA source candidate.
    */
   source_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   source_candidate.speed = 1.0F;
   source_candidate.f_moving = true;
   source_candidate.on_sep_id = F360_INVALID_UNSIGNED_ID;
   source_candidate.status = F360_OBJECT_STATUS_UPDATED;
   source_candidate.reduced_id = 1;
   source_candidate.behind_sep_id = F360_INVALID_UNSIGNED_ID;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_TRUE(f_valid);
}

/** \purpose
 * Verify function returns true for fast moving CCA source candidate.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__Fast_Moving_CCA)
{
   /** \precond
    * CCA filter type, speed above fast_moving_thresh, all other conditions valid.
    */
   source_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   source_candidate.speed = calib.fast_moving_thresh + 1e-3F;
   source_candidate.f_moving = true;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_TRUE(f_valid);
}

/** \purpose
 * Verify function returns false if source candidate is same as ghost candidate.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__Same_Id)
{
   /** \precond
    * Source candidate id matches ghost candidate id.
    */
   source_candidate.id = ghost_candidate.id;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}

/** \purpose
 * Verify function returns false for slow moving CCA source candidate.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__Slow_Moving_CCA)
{
   /** \precond
    * CCA filter type, speed below fast_moving_thresh.
    */
   source_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   source_candidate.speed = calib.fast_moving_thresh - 1e-3F;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}

/** \purpose
 * Verify function returns false if source candidate is on SEP.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__On_SEP)
{
   /** \precond
    * Source candidate is flagged as on SEP.
    */
   source_candidate.on_sep_id = 1U;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}

/** \purpose
 * Verify function returns false if source candidate is not moving.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__Not_Moving)
{
   /** \precond
    * Source candidate is not moving.
    */
   source_candidate.f_moving = false;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}

/** \purpose
 * Verify function returns false if source candidate status is new or new coasted.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__Status_New)
{
   /** \precond
    * Source candidate status is new updated.
    */
   source_candidate.status = F360_OBJECT_STATUS_NEW_UPDATED;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}

/** \purpose
 * Verify function returns false if source candidate reduced_id is not valid.
 */
TEST(f360_Is_Source_Obj_Valid_For_Reflection_Check, Is_Source_Obj_Valid_For_Reflection_Check__Reduced_Id_Invalid)
{
   /** \precond
    * Source candidate reduced_id is zero.
    */
   source_candidate.reduced_id = 0U;

   /** \action */
   bool f_valid = Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, calib);

   /** \result */
   CHECK_FALSE(f_valid);
}
/** @} */

/** \defgroup  f360_Is_Ghost_Reflected_Without_SEP
 *  @{
 */

 /** \brief
  * Test group for Is_Ghost_Reflected_Without_SEP() function.
  * Verifies if ghost candidate is flagged as a reflection without SEP.
  */
TEST_GROUP(f360_Is_Ghost_Reflected_Without_SEP)
{
   F360_Calibrations_T calib;
   F360_Host_T host;
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Object_Track_T& ghost_candidate = object_tracks[0];
   F360_Object_Track_T& source_candidate = object_tracks[1];
   F360_Object_Track_T& guardrail_obj1 = object_tracks[2];
   F360_Object_Track_T& guardrail_obj2 = object_tracks[3];
   F360_Object_Track_T& guardrail_obj3 = object_tracks[4];

   /** \setup
    * Initialize tracker calibrations, host, and set up ghost/source/guardrail objects.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      host.dist_rear_axle_to_vcs_m = 4.1667F; //corresponds to host center at 2.5m longitudinally

      tracker_info.num_active_objs = 5;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
      tracker_info.active_obj_ids[3] = 4;
      tracker_info.active_obj_ids[4] = 5;

      // Ghost candidate setup
      ghost_candidate.id = 1;
      ghost_candidate.speed = 10.0F;
      ghost_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      ghost_candidate.vcs_position.x = 10.0F;
      ghost_candidate.vcs_position.y = 5.0F;
      ghost_candidate.predicted_vcs_position.x = 10.0F;
      ghost_candidate.predicted_vcs_position.y = 5.0F;
      ghost_candidate.vcs_heading = Angle{ 0.0F };
      ghost_candidate.bbox.Set_Center(Point{ 10.0F, 5.0F });
      ghost_candidate.bbox.Set_Orientation(Angle{ 0.0F });

      // Source candidate setup
      source_candidate.id = 2;
      source_candidate.speed = 10.0F;
      source_candidate.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      source_candidate.vcs_position.x = 10.0F;
      source_candidate.vcs_position.y = 2.0F;
      source_candidate.predicted_vcs_position.x = 10.0F;
      source_candidate.predicted_vcs_position.y = 2.0F;
      source_candidate.vcs_heading = Angle{ 0.0F };
      source_candidate.bbox.Set_Center(Point{ 10.0F, 2.0F });
      source_candidate.bbox.Set_Orientation(Angle{ 0.0F });

      // Guardrail objects (stationary CCA)
      guardrail_obj1.id = 3;
      guardrail_obj1.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      guardrail_obj1.movable_prob = 0.0F;
      guardrail_obj1.status = F360_OBJECT_STATUS_UPDATED;
      guardrail_obj1.otg_height = 1.0F;
      guardrail_obj1.vcs_position.x = 4.0F;
      guardrail_obj1.vcs_position.y = 4.0F;
      guardrail_obj1.predicted_vcs_position.x = 4.0F;
      guardrail_obj1.predicted_vcs_position.y = 4.0F;
      guardrail_obj1.bbox.Set_Center(Point{ 4.0F, 4.0F });
      guardrail_obj1.bbox.Set_Orientation(Angle{ 0.0F });

      guardrail_obj2.id = 4;
      guardrail_obj2.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      guardrail_obj2.movable_prob = 0.0F;
      guardrail_obj2.status = F360_OBJECT_STATUS_UPDATED;
      guardrail_obj2.otg_height = 1.0F;
      guardrail_obj2.vcs_position.x = 5.0F;
      guardrail_obj2.vcs_position.y = 4.0F;
      guardrail_obj2.predicted_vcs_position.x = 5.0F;
      guardrail_obj2.predicted_vcs_position.y = 4.0F;
      guardrail_obj2.bbox.Set_Center(Point{ 5.0F, 4.0F });
      guardrail_obj2.bbox.Set_Orientation(Angle{ 0.0F });

      guardrail_obj3.id = 5;
      guardrail_obj3.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      guardrail_obj3.movable_prob = 0.0F;
      guardrail_obj3.status = F360_OBJECT_STATUS_UPDATED;
      guardrail_obj3.otg_height = 1.0F;
      guardrail_obj3.vcs_position.x = 6.0F;
      guardrail_obj3.vcs_position.y = 4.0F;
      guardrail_obj3.predicted_vcs_position.x = 6.0F;
      guardrail_obj3.predicted_vcs_position.y = 4.0F;
      guardrail_obj3.bbox.Set_Center(Point{ 6.0F, 4.0F });
      guardrail_obj3.bbox.Set_Orientation(Angle{ 0.0F });
   }
};

/** \purpose
 * Verify function returns true when all conditions for reflection are met.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Reflection_Confirmed)
{
   /** \precond
    * Ghost and source speeds are similar, source is closer to host laterally,
    * and at least 3 stationary CCA objects are present near predicted reflection point.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate,
      source_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result */
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Verify function returns false if speeds are not similar.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Speed_Not_Similar)
{
   /** \precond
    * Source candidate speed is very different from ghost candidate.
    */
   source_candidate.speed = 100.0F;

   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate,
      source_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns false if source is not closer to host laterally.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Source_Not_Closer_Laterally)
{
   /** \precond
    * Source candidate is not closer to host laterally than ghost candidate.
    */
   source_candidate.vcs_position.y = 10.0F;

   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate,
      source_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns false if not enough stationary CCA objects near predicted reflection point.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Not_Enough_Stationary_CCA)
{
   /** \precond
    * Only 2 stationary CCA objects present near predicted reflection point.
    */
   F360_Object_Track_T object_tracks_local[NUMBER_OF_OBJECT_TRACKS] = {};
   object_tracks_local[0] = object_tracks[0];
   object_tracks_local[1] = object_tracks[1];
   object_tracks_local[2] = object_tracks[2];

   F360_Tracker_Info_T tracker_info_local = {};
   tracker_info_local.num_active_objs = 3;
   tracker_info_local.active_obj_ids[0] = 1;
   tracker_info_local.active_obj_ids[1] = 2;
   tracker_info_local.active_obj_ids[2] = 3;

   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate,
      source_candidate,
      tracker_info_local,
      object_tracks_local,
      host);

   /** \result */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns true if source_candidate.speed is zero (division by zero protection).
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Source_Speed_Zero)
{
   /** \precond
    * Set source_candidate.speed to 0 to check division by zero protection.
    * All other conditions for a positive reflection are fulfilled.
    */
   source_candidate.speed = 0.0F;
   ghost_candidate.speed = 0.1F;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is true.
    */
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Verify function returns true if only the second angle check passes.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Second_Angle_Check_Passes)
{
   /** \precond
    * Set ghost_candidate.vcs_heading = 0.0F, source_candidate.vcs_heading = PI.
    * All other conditions for a positive reflection are fulfilled.
    */
   ghost_candidate.vcs_heading = Angle{ F360_PI };
   source_candidate.vcs_heading = Angle{ F360_PI };

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is true.
    */
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Verify function returns false if a guardrail candidate is the ghost candidate.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Is_Ghost)
{
   /** \precond
    * Set object_tracks[2].id = ghost_candidate.id.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].id = ghost_candidate.id;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].id = 3; // restore
}

/** \purpose
 * Verify function returns false if a guardrail candidate is the source candidate.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Is_Source)
{
   /** \precond
    * Set object_tracks[2].id = source_candidate.id.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].id = source_candidate.id;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].id = 3; // restore
}

/** \purpose
 * Verify function returns false if a guardrail candidate is not CCA type.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Not_CCA)
{
   /** \precond
    * Set object_tracks[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA; // restore
}

/** \purpose
 * Verify function returns false if a guardrail candidate is movable.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Movable)
{
   /** \precond
    * Set object_tracks[2].movable_prob = 0.6F.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].movable_prob = 0.6F;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].movable_prob = 0.0F; // restore
}

/** \purpose
 * Verify function returns false if a guardrail candidate status is new.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Status_New)
{
   /** \precond
    * Set object_tracks[2].status = F360_OBJECT_STATUS_NEW_UPDATED.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].status = F360_OBJECT_STATUS_NEW_UPDATED;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].status = F360_OBJECT_STATUS_UPDATED; // restore
}

/** \purpose
 * Verify function returns false if a guardrail candidate otg_height >= 5.0F.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Otg_Height)
{
   /** \precond
    * Set object_tracks[2].otg_height = 5.0F.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].otg_height = 5.0F;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].otg_height = 1.0F; // restore
}

/** \purpose
 * Verify function returns false if a guardrail candidate is not within predicted zone.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Guardrail_Candidate_Not_Within_Pred_Zone)
{
   /** \precond
    * Set object_tracks[2] far from intersection.
    * Only 2 valid guardrail objects remain.
    */
   object_tracks[2].bbox.Set_Center(Point{ 100.0F, 100.0F });

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   object_tracks[2].bbox.Set_Center(Point{ 4.0F, 4.0F }); // restore
}

/** \purpose
 * Verify function returns false if there are no objects.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__No_Objects)
{
   /** \precond
    * Set tracker_info.num_active_objs = 0.
    */
   F360_Tracker_Info_T tracker_info_local = {};
   tracker_info_local.num_active_objs = 0;

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info_local, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns false if both angle checks fail.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Angle_Check_Fails)
{
   /** \precond
    * Set ghost_candidate.vcs_heading = 0.0F, source_candidate.vcs_heading = 0.0F.
    * Move predicted positions to force guardrail line angle far from expected.
    */
   ghost_candidate.vcs_heading = Angle{ 0.0F };
   source_candidate.vcs_heading = Angle{ 0.0F };
   ghost_candidate.predicted_vcs_position.y = 100.0F;
   source_candidate.predicted_vcs_position.y = -100.0F;
   ghost_candidate.bbox.Set_Center(Point{ 10.0F, 100.0F });
   source_candidate.bbox.Set_Center(Point{ 10.0F, -100.0F });

   /** \action
    * Call tested function.
    */
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Check whether returned value is false.
    */
   CHECK_FALSE(f_reflection);

   ghost_candidate.predicted_vcs_position.y = 8.0F;
   source_candidate.predicted_vcs_position.y = 0.0F;
   ghost_candidate.bbox.Set_Center(Point{ 10.0F, 8.0F });
   source_candidate.bbox.Set_Center(Point{ 10.0F, 0.0F });
}

/** \purpose
 * Verify function returns true when relaxed conditions are used (high curvature and previously flagged mirror).
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Relaxed_Conditions_High_Curvature)
{
   /** \precond
    * host.curvature_rear > 0.04F, ghost_candidate.mirror_prob > 0.9F, speeds within relaxed threshold.
    */
   host.curvature_rear = 0.05F;
   ghost_candidate.mirror_prob = 0.95F;
   ghost_candidate.speed = 8.6F;
   source_candidate.speed = 10.0F; // within relaxed threshold (0.15F relative diff)
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Verify function returns true when host curvature is high but relaxed conditions are not used.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__High_Curvature_Not_Relaxed)
{
   /** \precond
    * host.curvature_rear > 0.04F, ghost_candidate.mirror_prob <= 0.9F, strict threshold applies.
    */
   host.curvature_rear = 0.05F;
   ghost_candidate.mirror_prob = 0.5F;
   ghost_candidate.speed = 10.0F;
   source_candidate.speed = 9.1F; // within strict threshold (0.1F relative diff)
   source_candidate.vcs_position.y = 2.0F;
   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);
   CHECK_TRUE(f_reflection);
}

/** \purpose
 * Verify function returns false when angle difference is above strict threshold but below relaxed threshold.
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Angle_Above_Strict_Below_Relaxed)
{
   /** \precond
    * f_relaxed_conditions is false (host.curvature_rear <= 0.04F or mirror_prob <= 0.9F).
    * Set angle difference to just above 15 degrees (strict threshold), but below 25 degrees (relaxed threshold).
    */
   host.curvature_rear = 0.01F; // Not high curvature
   ghost_candidate.mirror_prob = 0.5F; // Not previously flagged mirror

   // Set headings so that angle difference is just above 15 degrees (strict threshold)
   // F360_DEG2RAD(16.0F) > 15 degrees but < 25 degrees
   ghost_candidate.vcs_heading = Angle{ 0.0F };
   source_candidate.vcs_heading = Angle{ F360_DEG2RAD(32.0F) }; // average is 16 degrees

   // Move predicted positions so the guardrail line angle matches expected_guardrail_angle
   ghost_candidate.predicted_vcs_position.x = 10.0F;
   ghost_candidate.predicted_vcs_position.y = 5.0F;
   source_candidate.predicted_vcs_position.x = 10.0F;
   source_candidate.predicted_vcs_position.y = 2.0F;

   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Should be false, since strict threshold is 15 degrees and angle difference is just above.
    */
   CHECK_FALSE(f_reflection);
}

/** \purpose
 * Verify function returns true when angle difference is above strict threshold but within relaxed threshold (relaxed branch).
 */
TEST(f360_Is_Ghost_Reflected_Without_SEP, Is_Ghost_Reflected_Without_SEP__Angle_Above_Strict_Within_Relaxed)
{
   /** \precond
    * f_relaxed_conditions is true (host.curvature_rear > 0.04F and mirror_prob > 0.9F).
    * Set angle difference to just above 15 degrees (strict threshold), but below 25 degrees (relaxed threshold).
    */
   host.curvature_rear = 0.05F; // High curvature
   ghost_candidate.mirror_prob = 0.95F; // Previously flagged mirror

   // Set headings so that angle difference is just above 15 degrees (strict threshold)
   // F360_DEG2RAD(16.0F) > 15 degrees but < 25 degrees
   ghost_candidate.vcs_heading = Angle{ 0.0F };
   source_candidate.vcs_heading = Angle{ F360_DEG2RAD(32.0F) }; // average is 16 degrees

   // Move predicted positions so the guardrail line angle matches expected_guardrail_angle
   ghost_candidate.predicted_vcs_position.x = 10.0F;
   ghost_candidate.predicted_vcs_position.y = 5.0F;
   source_candidate.predicted_vcs_position.x = 10.0F;
   source_candidate.predicted_vcs_position.y = 2.0F;

   bool f_reflection = Is_Ghost_Reflected_Without_SEP(
      ghost_candidate, source_candidate, tracker_info, object_tracks, host);

   /** \result
    * Should be true, since relaxed threshold is 25 degrees and angle difference is just above strict but within relaxed.
    */
   CHECK_TRUE(f_reflection);
}
/** @} */
