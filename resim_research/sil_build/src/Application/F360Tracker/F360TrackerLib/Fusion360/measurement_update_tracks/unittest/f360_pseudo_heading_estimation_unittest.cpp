/** \file
 * This file contains unit tests for content of f360_pseudo_heading_estimation.cpp file
 */

#include "f360_pseudo_heading_estimation.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_pseudo_heading_estimation
 *  @{
 */

 /** \brief
  * This group is used for testing pseudo heading estimation function.
  */
TEST_GROUP(f360_pseudo_heading_estimation)
{
    F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
    F360_Object_Track_T obj;

    /** \setup
    * Initialize detection properties and object
    */
    TEST_SETUP()
    {
        // Set up the object
        obj = {};
        obj.f_moving = true;
        obj.speed = 10.0F;
        obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
        obj.vcs_heading = Angle(0.0F);
        obj.vcs_position = Point(0.0F, 0.0F);
        obj.Set_Bbox_Orientation(obj.vcs_heading);
        obj.ndets = 2U;
        obj.detids[0] = 1U;
        obj.detids[1] = 2U;
        // Set up the detections properties
        for (uint32_t i = 0U; i < 6U; i++)
        {
            obj.pseudo_hdg_state_vec[i] = 0.0F;
        }

        // Set up the detections properties
        for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
        {
            det_props[det_idx] = {};
        }
        det_props[0].vcs_position = Point(5.0F, 0.0F);
        det_props[1].vcs_position = Point(7.0F, 2.0F);

    }
};

/** \purpose
 * Check if pseudo heading is computed correctly for two detections forming 45 degree angle, for a CCA object
 * and all the preconditions for pseudo heading estimation are met.
 */
TEST(f360_pseudo_heading_estimation, Compute_Pseudo_Heading_All_Preconditions_Met)
{
    /** \precond
    * Use default setup
    */

    /** \action
    * Call Pseudo_Heading_Estimation().
    */
    Pseudo_Heading_Estimation(det_props, obj);

    /** \result
    * Check that pseudo heading state vector and pseudo heading are computed correctly
    */
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[0], 1e-6F);
    DOUBLES_EQUAL(12.0F, obj.pseudo_hdg_state_vec[1], 1e-6F);
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[2], 1e-6F);
    DOUBLES_EQUAL(74.0F, obj.pseudo_hdg_state_vec[3], 1e-6F);
    DOUBLES_EQUAL(4.0F, obj.pseudo_hdg_state_vec[4], 1e-6F);
    DOUBLES_EQUAL(14.0F, obj.pseudo_hdg_state_vec[5], 1e-6F);
    DOUBLES_EQUAL(F360_DEG2RAD(45.0F), obj.pseudo_hdg, 1e-3F);
}

/** \purpose
 * Check if pseudo heading is computed correctly for two detections forming 45 degree angle, for a CCA object
 * and all the preconditions for pseudo heading estimation are met but the pseudo heading states are initialized
 * to no zero values.
 */
TEST(f360_pseudo_heading_estimation, Compute_Pseudo_Heading_All_Preconditions_Met_With_Initial_States_Equal_To_No_Zero)
{
    /** \precond
    * Set all pseudo heading states to 1 (non zero values)
    */
   for (uint32_t i = 0U; i < 6U; i++)
   {
      obj.pseudo_hdg_state_vec[i] = 1.0F;
   }


    /** \action
    * Call Pseudo_Heading_Estimation().
    */
    Pseudo_Heading_Estimation(det_props, obj);

    /** \result
    * Check that pseudo heading state vector and pseudo heading are computed correctly
    */
    DOUBLES_EQUAL(2.890899F, obj.pseudo_hdg_state_vec[0], 1e-6F);
    DOUBLES_EQUAL(12.8909F, obj.pseudo_hdg_state_vec[1], 1e-6F);
    DOUBLES_EQUAL(2.890899F, obj.pseudo_hdg_state_vec[2], 1e-6F);
    DOUBLES_EQUAL(74.8909F, obj.pseudo_hdg_state_vec[3], 1e-6F);
    DOUBLES_EQUAL(4.890899F, obj.pseudo_hdg_state_vec[4], 1e-6F);
    DOUBLES_EQUAL(14.8909F, obj.pseudo_hdg_state_vec[5], 1e-6F);
    DOUBLES_EQUAL(F360_DEG2RAD(7.0F), obj.pseudo_hdg, 1e-3F);
}

/** \purpose
 * Check if pseudo heading is set to 0 correctly for CTCA objects.
 * The psuedo heading state vector should still be updated.
 */
TEST(f360_pseudo_heading_estimation, Compute_Pseudo_Heading_For_CTCA_Objects)
{
    /** \precond
    * Switch object type to CTCA filter
    */
    obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

    /** \action
    * Call Pseudo_Heading_Estimation().
    */
    Pseudo_Heading_Estimation(det_props, obj);

    /** \result
    * Check that pseudo heading state vector are updated correctly and pseudo heading is set to 0
    */
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[0], 1e-6F);
    DOUBLES_EQUAL(12.0F, obj.pseudo_hdg_state_vec[1], 1e-6F);
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[2], 1e-6F);
    DOUBLES_EQUAL(74.0F, obj.pseudo_hdg_state_vec[3], 1e-6F);
    DOUBLES_EQUAL(4.0F, obj.pseudo_hdg_state_vec[4], 1e-6F);
    DOUBLES_EQUAL(14.0F, obj.pseudo_hdg_state_vec[5], 1e-6F);    
    DOUBLES_EQUAL(F360_DEG2RAD(0.0F), obj.pseudo_hdg, 1e-3F);
}

/** \purpose
 * Check if pseudo heading is set to 0 correctly for stationary objects.
 * The psuedo heading state vector should still be updated.
 */
TEST(f360_pseudo_heading_estimation, Compute_Pseudo_Heading_For_Stationary_Objects)
{
    /** \precond
    * Switch moving flag and speed of the object to indicate stationary object
    */
    obj.f_moving = false;
    obj.speed = 0.0F;

    /** \action
    * Call Pseudo_Heading_Estimation().
    */
    Pseudo_Heading_Estimation(det_props, obj);

    /** \result
    * Check that pseudo heading state vector are updated correctly and pseudo heading is set to 0
    */
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[0], 1e-6F);
    DOUBLES_EQUAL(12.0F, obj.pseudo_hdg_state_vec[1], 1e-6F);
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[2], 1e-6F);
    DOUBLES_EQUAL(74.0F, obj.pseudo_hdg_state_vec[3], 1e-6F);
    DOUBLES_EQUAL(4.0F, obj.pseudo_hdg_state_vec[4], 1e-6F);
    DOUBLES_EQUAL(14.0F, obj.pseudo_hdg_state_vec[5], 1e-6F);
    DOUBLES_EQUAL(F360_DEG2RAD(0.0F), obj.pseudo_hdg, 1e-3F);
}

/** \purpose
 *  Check if pseudo heading is set to 0 for stationary objects and the pseudo heading state vector is updated correctly
 * when the initial pseudo heading states are non zero values.
 */
TEST(f360_pseudo_heading_estimation, Compute_Pseudo_Heading_For_Stationary_Objects_With_Initial_States_Equal_To_No_Zero)
{
    /** \precond
    * Switch moving flag and speed of the object to indicate stationary object
    * Set all pseudo heading states to 1 (non zero values)
    */
    obj.f_moving = false;
    obj.speed = 0.0F;
    for (uint32_t i = 0U; i < 6U; i++)
    {
      obj.pseudo_hdg_state_vec[i] = 1.0F;
    }


    /** \action
    * Call Pseudo_Heading_Estimation().
    */
    Pseudo_Heading_Estimation(det_props, obj);

    /** \result
    * Check that pseudo heading state vector are updated correctly and pseudo heading is set to 0
    */
    DOUBLES_EQUAL(2.93F, obj.pseudo_hdg_state_vec[0], 1e-6F);
    DOUBLES_EQUAL(12.93F, obj.pseudo_hdg_state_vec[1], 1e-6F);
    DOUBLES_EQUAL(2.93, obj.pseudo_hdg_state_vec[2], 1e-6F);
    DOUBLES_EQUAL(74.93, obj.pseudo_hdg_state_vec[3], 1e-6F);
    DOUBLES_EQUAL(4.93F, obj.pseudo_hdg_state_vec[4], 1e-6F);
    DOUBLES_EQUAL(14.93F, obj.pseudo_hdg_state_vec[5], 1e-6F);
    DOUBLES_EQUAL(F360_DEG2RAD(0.0F), obj.pseudo_hdg, 1e-3F);
}

/**
 * Check if pseudo heading computation sweeps angles beyond 3pi/4 so the second condition in f_crossing goes false
 * \req
 * NA.
 */
TEST(f360_pseudo_heading_estimation, Computes_Heading_When_Sweep_Exceeds_ThreeQuarter_PI)
{
    /** \precond
    * change heading to 100 degrees so grid search covers angles where |pseudo_angle| < 3pi/4
    */
    obj.vcs_heading = Angle(F360_DEG2RAD(100.0F));
    obj.Set_Bbox_Orientation(obj.vcs_heading);

    /** \action
    * Call Pseudo_Heading_Estimation().
    */
    Pseudo_Heading_Estimation(det_props, obj);

    /** \result
    * Check that pseudo heading state vector and pseudo heading are computed correctly
    */
    // State vector still counts detections; best heading should remain near the true line (~45 deg)
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[0], 1e-6F);
    DOUBLES_EQUAL(12.0F, obj.pseudo_hdg_state_vec[1], 1e-6F);
    DOUBLES_EQUAL(2.0F, obj.pseudo_hdg_state_vec[2], 1e-6F);
    DOUBLES_EQUAL(74.0F, obj.pseudo_hdg_state_vec[3], 1e-6F);
    DOUBLES_EQUAL(4.0F, obj.pseudo_hdg_state_vec[4], 1e-6F);
    DOUBLES_EQUAL(14.0F, obj.pseudo_hdg_state_vec[5], 1e-6F);
    DOUBLES_EQUAL(F360_DEG2RAD(45.0F), obj.pseudo_hdg, 1e-3F);
}

/** @}*/
