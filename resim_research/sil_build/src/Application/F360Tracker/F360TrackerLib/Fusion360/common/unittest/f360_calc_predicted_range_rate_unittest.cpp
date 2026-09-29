/** \file
    This file contains unit tests for content of f360_calc_predicted_range_rate.cpp file
*/

#include "f360_calc_predicted_range_rate.h"
#include <CppUTest/TestHarness.h>
#include "f360_math.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_calc_predicted_range_rate
 *  @{
 */
/** \brief
 *  Test group containing common data for tests related to calculation of predicted range rate
 */
TEST_GROUP(f360_calc_predicted_range_rate)
{

   F360_Detection_Props_T det_p;
   rspp_variant_A::RSPP_Detection_T det;
   F360_Object_Track_T obj;
   F360_Radar_Sensor_T sens;
   float32_t exp_rdot;

   /** \setup
   * Setup one object with
   *    - vcs position along the x axis (i.e. y position is 0). In this setup we use [10, 0]m
   *    - reference point in center rear
   *    - object bbox dimensions 6 x 2m (length x width) 
   *    - heading and pointing of 0 degrees in VCS
   *    - speed 10 m/s 
   *    - non-zero curvature. In this test we use 0.1m^-1
   * Setup one detection with:
   *    - azimuth 0 degrees
   *    - vcs position [9, 0] (i.e 1 m behind object rear center)
   * Setup sensor with:
   *    - origin in VCS
   *    - orientation such that boresight is aligned with VCS x-axis
   * 
   * Given this data defined in the test group also set expected predicted range rate for the detection (which will be the same
   * for both CCA and CTCA tracks with this setup)
   */
   TEST_SETUP()
   {
      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 0.0F;
      obj.reference_point = F360_REFERENCE_POINT_REAR;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);;
      obj.vcs_heading.Value(0.0F);
      obj.bbox.Set_Orientation(obj.vcs_heading);
      obj.Update_Bbox_Center();
      obj.speed = 10.0F;
      obj.vcs_velocity.longitudinal = obj.speed * obj.vcs_heading.Cos();
      obj.vcs_velocity.lateral = obj.speed * obj.vcs_heading.Sin();
      obj.curvature = 0.1F;

      sens.variable.vcs_velocity.longitudinal = 2.0F;
      sens.variable.vcs_velocity.lateral = 0.0F;
      
      float32_t azimuth = 0.0F;
      det.processed.cos_vcs_az = F360_Cosf(azimuth);
      det.processed.sin_vcs_az = F360_Sinf(azimuth);
      det_p.vcs_position.x = 9.0F;
      det_p.vcs_position.y = 0.0F;

      exp_rdot = 8.0F;
   }
};

/**
*\purpose  Verify that correct range rate is returned when object is CCV type
*\req    NA
*/
TEST(f360_calc_predicted_range_rate, Test_Calc_Predicted_Range_Rate_CCV)
{

   /** \precond
    * Set filter type to CCV
    */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;

   /** \action
    * Call function
    */
   float32_t rdot_pred = Calc_Predicted_Range_Rate(
      det_p,
      det,
      obj,
      sens);

   /** \result
    * Verify result matches expected range rate
    */
   DOUBLES_EQUAL_TEXT(exp_rdot, rdot_pred, F360_EPSILON, "Predicted range rate does not match expected data")
}

/**
*\purpose  Verify that correct range rate is returned when object is CCA type
*\req    NA
*/
TEST(f360_calc_predicted_range_rate, Test_Calc_Predicted_Range_Rate_CCA)
{

   /** \precond
    * Set filter type to CCA
    */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call function
    */
   float32_t rdot_pred = Calc_Predicted_Range_Rate(
      det_p,
      det,
      obj,
      sens);

   /** \result
    * Verify result matches expected range rate
    */
   DOUBLES_EQUAL_TEXT(exp_rdot, rdot_pred, F360_EPSILON, "Predicted range rate does not match expected data")
}

/**
*\purpose  Verify that correct range rate is returned when object is CTCA type and have reference point in rear center
*\req    NA
*/
TEST(f360_calc_predicted_range_rate, Test_Calc_Predicted_Range_Rate_CTCA_Ref_Pnt_Rear_Center)
{

   /** \precond
   * Set filter type to CTCA
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
   * Call function
   */
   float32_t rdot_pred = Calc_Predicted_Range_Rate(
      det_p,
      det,
      obj,
      sens);

   /** \result
   * Verify result matches expected range rate
   */
   DOUBLES_EQUAL_TEXT(exp_rdot, rdot_pred, F360_EPSILON, "Predicted range rate does not match expected data")
}

/**
*\purpose  Verify that correct range rate is returned when object is CTCA type and reference point is not in rear center
*          (for this test we choose center of left side)
*\req    NA
*/
TEST(f360_calc_predicted_range_rate, Test_Calc_Predicted_Range_Rate_CTCA_Ref_Pnt_Left_Center)
{

   /** \precond
   * - Set filter type to CTCA
   * - Set object reference point to center of left side and update object vcs position such that center rear of object
   *   are still in same position as in default test setup (by doing so we still expect the same predicted range rate for the detection)
   * 
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.vcs_position.x = obj.vcs_position.x + 0.5F * obj.bbox.Get_Length();
   obj.vcs_position.y = obj.vcs_position.y - 0.5F * obj.bbox.Get_Width();

   /** \action
   * Call function
   */
   float32_t rdot_pred = Calc_Predicted_Range_Rate(
      det_p,
      det,
      obj,
      sens);

   /** \result
   * Verify result matches expected range rate
   */
   DOUBLES_EQUAL_TEXT(exp_rdot, rdot_pred, F360_EPSILON, "Predicted range rate does not match expected data")
}

/**
*\purpose  Verify that correct range rate is returned when object is CTCA type and reference point is not in rear center and the object pointing is non-zero
*          (for this test we choose center of left side as reference point and a -90degree pointing)
*\req    NA
*/
TEST(f360_calc_predicted_range_rate, Test_Calc_Predicted_Range_Rate_CTCA_Ref_Pnt_Left_Center_Pointing_Minus90deg)
{

   /** \precond
   * - Set filter type to CTCA
   * - Set object reference point to center of left side object pointing to -90 degrees and update object vcs position such that center rear of object
   *   are still in same position as in default test setup (by doing so we still expect the same predicted range rate for the detection)
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.bbox.Set_Orientation(F360_DEG2RAD(-90.0F));
   obj.vcs_position.x = obj.vcs_position.x - 0.5F * obj.bbox.Get_Width();
   obj.vcs_position.y = obj.vcs_position.y - 0.5F * obj.bbox.Get_Length();
   obj.Update_Bbox_Center();

   /** \action
   * Call function
   */
   float32_t rdot_pred = Calc_Predicted_Range_Rate(
      det_p,
      det,
      obj,
      sens);

   /** \result
   * Verify result matches expected range rate
   */
   DOUBLES_EQUAL_TEXT(exp_rdot, rdot_pred, F360_EPSILON, "Predicted range rate does not match expected data")
}

/**
*\purpose  Verify that correct range rate is returned when
              - object is CTCA type and reference point is not in rear center and the object pointing is non-zero
*               (for this test we choose center of left side as reference point and a -90degree pointing)
              - detection position and azimuth are laterally (as well as longitudinally) compared to the position of the object rear center
*\req    NA
*/
TEST(f360_calc_predicted_range_rate, Test_Calc_Predicted_Range_Rate_CTCA_Ref_Pnt_Left_Center_Pointing_Minus90deg_Det_Nonzero_Az)
{

   /** \precond
   * - Set filter type to CTCA
   * - Set object reference point to center of left side object pointing to -90 degrees and update object vcs position such that center rear of object
   *   are still in same position as in default test setup
   * - Set detection position position to be also laterally offsetted compared to object rear center (as well as longitudinally from the default test setup).
   *   By doing so we ensure the that yaw rate component of the object is visible in te prediction (and hence we have to update expected predicted range rate
   *   compared to other testst)
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.bbox.Set_Orientation(F360_DEG2RAD(-90.0F));
   obj.vcs_position.x = obj.vcs_position.x - 0.5F * obj.bbox.Get_Width();
   obj.vcs_position.y = obj.vcs_position.y - 0.5F * obj.bbox.Get_Length();
   obj.Update_Bbox_Center();

   det_p.vcs_position.y = -2.0F;
   const float32_t det_vcs_az = F360_Atan2f(det_p.vcs_position.y, det_p.vcs_position.x);
   det.processed.cos_vcs_az = F360_Cosf(det_vcs_az);
   det.processed.sin_vcs_az = F360_Sinf(det_vcs_az);

   exp_rdot = 9.978801059658183F;

   /** \action
   * Call function
   */
   float32_t rdot_pred = Calc_Predicted_Range_Rate(
      det_p,
      det,
      obj,
      sens);

   /** \result
   * Verify result matches expected range rate
   */
   DOUBLES_EQUAL_TEXT(exp_rdot, rdot_pred, F360_EPSILON, "Predicted range rate does not match expected data")
}
/** @}*/
