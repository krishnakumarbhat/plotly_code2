/** \file
 * This file contains qual tests and unit tests for content of f360_update_object_orientation_uncertainty.cpp file
 */

#include "f360_update_object_orientation_uncertainty.h"
#include "f360_clear_object_track.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_update_object_orientation_uncertainty
 *  @{
 */

/** \brief
 * The purpose of this test group is to test the business logic defined in Update_Object_Orientation_Uncertainty()
 */
TEST_GROUP(f360_update_object_orientation_uncertainty)
{	
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   
   /** \setup
    * define two active objects
    * The first object is defined as a non-moveable CCA.
    * The second object is defined as a stopped CTCA.
    */
   TEST_SETUP()
   {
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;

      F360_Object_Track_T cca_obj = {};
      F360_Object_Track_T ctca_obj = {};
      cca_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      cca_obj.f_moving = false;
      cca_obj.movable_prob = 0.0F;
      ctca_obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      ctca_obj.f_moving = false;
      ctca_obj.movable_prob = 1.0F;
      object_tracks[0] = cca_obj;
      object_tracks[1] = ctca_obj;
   }
};

/** \purpose  
 * The purpose of the test is to ensure all non-moveable stationary objects are assigned with expected orientation std
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, non_moveable_object)
{
   /** \precond
    * Default setting where object[0] is a non-moveable CCA object
    */

   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches expected data.
    */
   DOUBLES_EQUAL(object_tracks[0].orientation_std, F360_PI, F360_EPSILON);
}

/** \purpose  
 * The purpose of the test is to ensure stopped objects, regardless of CCA or CTCA filter types, 
 * are assigned with expected orientation std in nomimnal scenarios
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, stopped_objects)
{
   /** \precond
    * CCA object[0] is configured with moving flag set to false and movable_prob set to 1.0F
    * CTCA object[1] is configured with moving flag set to false and movable_prob set to 1.0F
    */
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].orientation_std = F360_DEG2RAD(5.0F);

   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches expected data.
    */
   DOUBLES_EQUAL(object_tracks[0].orientation_std, F360_PI, F360_EPSILON);
   DOUBLES_EQUAL(object_tracks[1].orientation_std, F360_DEG2RAD(7.0F), F360_EPSILON);
}

/** \purpose  
 * The purpose of the test is to ensure stopped CTCA objects to have  
 * orientation std properly capped
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, stopped_objects_ctca)
{
   /** \precond
    * moving flag is set to false and movable_prob set to 1.0F
    * orientation std from the previous cycle is 29 deg
    */
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].orientation_std = F360_DEG2RAD(29.0F);

   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches ceilling threshold.
    */
   DOUBLES_EQUAL(object_tracks[1].orientation_std, F360_DEG2RAD(30.0F), F360_EPSILON);
}

/** \purpose  
 * The purpose of the test is to check if a moving CCA object's orientation std performs as expected
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, moving_cca)
{
   /** \precond
    * CCA object0's moving flag is set to true and movable_prob set to 1.0F
    * CCA object0's cca_pnt_filter_cov[0][0] is set to 16 deg
    * CCA object1's moving flag is set to true and movable_prob set to 1.0F
    * CCA object0's cca_pnt_filter_cov[0][0] is set to 14 deg
    */
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].f_moving = true;
   object_tracks[0].cca_pnt_filter_cov[0][0] = F360_DEG2RAD(16.0F);
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[1].f_moving = true;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].cca_pnt_filter_cov[0][0] = F360_DEG2RAD(14.0F);

   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches the expected value.
    */
   DOUBLES_EQUAL(object_tracks[0].orientation_std, F360_DEG2RAD(16.0F), F360_EPSILON);
   DOUBLES_EQUAL(object_tracks[1].orientation_std, F360_DEG2RAD(15.0F), F360_EPSILON);
}

/** \purpose  
 * The purpose of the test is to check if a moving CTCA object's orientation std is properly updated
 * after merge
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, moving_ctca_after_merge)
{
   /** \precond
    * CTCA object0's moving flag is set to true and movable_prob set to 1.0F
    * CTCA object0's f_prevent_orientation_std_decrease is set to true
    * CTCA object0's heading rate is set to 0.02
    * CTCA object0's status is set to F360_OBJECT_STATUS_UPDATED
    * CTCA object1's moving flag is set to true and movable_prob set to 1.0F
    * CTCA object1's f_prevent_orientation_std_decrease is set to true
    * CTCA object1's heading rate is set to 0.04
    * CTCA object1's status is set to F360_OBJECT_STATUS_UPDATED
    * Set both objects' orientation std to 20 DEG
    */
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].f_moving = true;
   object_tracks[0].f_prevent_orientation_std_decrease = true;
   object_tracks[0].heading_rate = 0.02;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].orientation_std = F360_DEG2RAD(20.0F);
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[1].f_moving = true;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].f_prevent_orientation_std_decrease = true;
   object_tracks[1].heading_rate = 0.04;
   object_tracks[1].orientation_std = F360_DEG2RAD(20.0F);
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;


   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches the expected value.
    */
   DOUBLES_EQUAL(object_tracks[0].orientation_std, F360_DEG2RAD(18.0F), F360_EPSILON);
   DOUBLES_EQUAL(object_tracks[1].orientation_std, F360_DEG2RAD(20.0F), F360_EPSILON);
}

/** \purpose  
 * The purpose of the test is to check if a moving CTCA object's orientation std shrink condition 
 * is determined as expected
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, moving_ctca_orientation_std_nominal_shrink)
{
   /** \precond
    * Set both objects' moving flag to true and movable_prob to 1.0F
    * Set both objects' f_prevent_orientation_std_decrease to false
    * Set both objects' status to F360_OBJECT_STATUS_UPDATED
    * Set object 0' orientation std to 4 DEG
    * Set object 1' orientation std to 19 DEG
    */
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].f_moving = true;
   object_tracks[0].f_prevent_orientation_std_decrease = false;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].orientation_std = F360_DEG2RAD(4.0F);
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[1].f_moving = true;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].f_prevent_orientation_std_decrease = false;
   object_tracks[1].orientation_std = F360_DEG2RAD(19.0F);
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;


   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches the expected value.
    */
   DOUBLES_EQUAL(object_tracks[0].orientation_std, F360_DEG2RAD(3.0F), F360_EPSILON);
   DOUBLES_EQUAL(object_tracks[1].orientation_std, F360_DEG2RAD(17.0F), F360_EPSILON);
}

/** \purpose  
 * The purpose of the test is to check if a moving CTCA object's orientation std drift condition 
 * is determined as expected
 * \req
 * CPR-6674
 */
TEST(f360_update_object_orientation_uncertainty, moving_ctca_orientation_std_nominal_drift)
{
   /** \precond
    * Set both objects' moving flag to true and movable_prob to 1.0F
    * Set both objects' f_prevent_orientation_std_decrease to false
    * Set both objects' status to F360_OBJECT_STATUS_COASTED
    * Set object 0's orientation std to 19 deg
    * Set object 1's orientation std to 4 deg
    */
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].f_moving = true;
   object_tracks[0].f_prevent_orientation_std_decrease = false;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].orientation_std = F360_DEG2RAD(19.0F);
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].f_moving = true;
   object_tracks[1].f_prevent_orientation_std_decrease = false;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].orientation_std = F360_DEG2RAD(4.0F);

   /** \action
    * call Update_Object_Orientation_Uncertainty().
    */
   Update_Object_Orientation_Uncertainty(tracker_info, object_tracks);

   /** \result
    * Check object's orientation standard matches the expected value.
    */
   DOUBLES_EQUAL(object_tracks[0].orientation_std, F360_DEG2RAD(20.0F), F360_EPSILON);
   DOUBLES_EQUAL(object_tracks[1].orientation_std, F360_DEG2RAD(6.0F), F360_EPSILON);
}
/** @}*/
