/** \file
   This file contains unit tests for content of f360_pseudo_position_estimation.cpp file
*/

#include "f360_pseudo_position_estimation.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_get_reference_point_para_side.h"
#include "f360_get_reference_point_orth_side.h"
#include "f360_math_func.h"
#include <CppUTest/TestHarness.h>

#include "f360_clear_object_track.h"

using namespace f360_variant_A;

/** \defgroup  f360_pseudo_position_estimation
 *  @{
 */

/** \brief
 * Testing of a function that estimates the position of an object based
 * on position of associated detections and corresponding weights.
 */

TEST_GROUP(f360_pseudo_position_estimation)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Calibrations_T calibrations = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   // Expected pseudo x and y position and corresponding variances
   float32_t exp_pseudo_pos_x = 0.0F;
   float32_t exp_pseudo_pos_x_var = 0.0F;
   float32_t exp_pseudo_pos_y = 0.0F;
   float32_t exp_pseudo_pos_y_var = 0.0F;

   float32_t exp_pseudo_pos_cov_xy = 0.0F; // Expected pseudo covariance between x- and y-position

   // Estimated value and variance of edge in para and orth direction
   float32_t para_est_mean = 0.0F;
   float32_t para_est_var = 0.0F;
   float32_t orth_est_mean = 0.0F;
   float32_t orth_est_var = 0.0F;

   // variances from previous iteration
   float32_t obj_var_para = 0.0F;
   float32_t obj_var_orth = 0.0F;

   Point assoc_dets_pos_tcs[MAX_DETS_IN_OBJ_TRK] = {};

   /** \setup
    * Initialize object with an arbitrary size, centroid in object center and VCS position to 0
    * Also, initialize the speed, movable prob and heading of the object
    * Set x pos and y pos part of meascov to zero
    * Set gains to same values as in source file
    * Initialze sensors with their normals and their mounting positions
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      globals.f_single_front_center_radar_only = false;
      
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);

      obj.vcs_position.x = 0.0F;
      obj.vcs_position.y = 0.0F;
      Point center = {0.0F,0.0F};
      obj.bbox.Set_Center(center);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;

      obj.Set_Bbox_Orientation(Angle{ 0.0F });

      obj.meascov[0][0] = 0.0F;
      obj.meascov[0][1] = 0.0F;
      obj.meascov[1][0] = 0.0F;
      obj.meascov[1][1] = 0.0F;

      obj.speed = 3.0F;
      obj.movable_prob = 0.6;
      obj.vcs_heading = Angle{ 0.0F };


      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[2].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[3].variable.look_id = F360_DET_LOOK_ID_0;

      // Set the normals for all 4 sensors
      sensors[0U].refined.left_fov_normal[0U] = 0.0524F;
      sensors[0U].refined.right_fov_normal[0U] = 0.7986F;
      sensors[0U].refined.left_fov_normal[1U] = 0.9986F;
      sensors[0U].refined.right_fov_normal[1U] = 0.6018F;
      sensors[0U].refined.left_fov_normal[2U] = 0.0524F;
      sensors[0U].refined.right_fov_normal[2U] = 0.7986F;
      sensors[0U].refined.left_fov_normal[3U] = 0.9986F;
      sensors[0U].refined.right_fov_normal[3U] = 0.6018F;

      sensors[1U].refined.left_fov_normal[0U] = 0.7986F;
      sensors[1U].refined.right_fov_normal[0U] = 0.0524F;
      sensors[1U].refined.left_fov_normal[1U] = -0.6018;
      sensors[1U].refined.right_fov_normal[1U] = -0.9986F;
      sensors[1U].refined.left_fov_normal[2U] = 0.7986F;
      sensors[1U].refined.right_fov_normal[2U] = 0.0524F;
      sensors[1U].refined.left_fov_normal[3U] = -0.6018;
      sensors[1U].refined.right_fov_normal[3U] = -0.9986F;

      sensors[2U].refined.left_fov_normal[0U] = -0.9397F;
      sensors[2U].refined.right_fov_normal[0U] = -0.3421F;
      sensors[2U].refined.left_fov_normal[1U] = 0.3421F;
      sensors[2U].refined.right_fov_normal[1U] = 0.9397;
      sensors[2U].refined.left_fov_normal[2U] = -0.9397F;
      sensors[2U].refined.right_fov_normal[2U] = -0.3421F;
      sensors[2U].refined.left_fov_normal[3U] = 0.3421F;
      sensors[2U].refined.right_fov_normal[3U] = 0.9397;

      sensors[3U].refined.left_fov_normal[0U] = -0.3421F;
      sensors[3U].refined.right_fov_normal[0U] = -0.9397F;
      sensors[3U].refined.left_fov_normal[1U] = -0.9397F;
      sensors[3U].refined.right_fov_normal[1U] = -0.3421F;
      sensors[3U].refined.left_fov_normal[2U] = -0.3421F;
      sensors[3U].refined.right_fov_normal[2U] = -0.9397F;
      sensors[3U].refined.left_fov_normal[3U] = -0.9397F;
      sensors[3U].refined.right_fov_normal[3U] = -0.3421F;

      // Make the first 4 sensors valid
      for (int32_t i = 0U; i < 4; i++)
      {
        sensors[i].variable.is_valid = true;
      }

      // Make all the rest sensors invalid
      for (int32_t i = 4; i < MAX_NUMBER_OF_SENSORS; i++)
      {
        sensors[i].variable.is_valid = false;
      }

      // Mounting positions for 4 sensors (VCS coordinates)
      // Front right
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 1.0F;

      // Front left
      sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;

      // Rear right
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -5.0F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 1.0F;

      // Rear left
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -5.0F;
      sensors[3].constant.mounting_position.vcs_position.lateral = -1.0F;

      // Set up the globals
      globals.four_corner_sensor_config_min_longitudinal_position = sensors[2].constant.mounting_position.vcs_position.longitudinal;
      globals.four_corner_sensor_config_max_longitudinal_position = sensors[0].constant.mounting_position.vcs_position.longitudinal;
      globals.f_four_corner_sensors_available = true;

      // Set up host to be stationary
      host.speed = 0.0F;
   }
};

/**
*\purpose  Verify that correct pseudo position is set when no edge is ok (i.e. reference point is CENTER)
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_No_Edge_Is_Ok)
{
   /** \precond
    * Set obj.ndets to 2
    * Set position variance for para and orth of first detection to 0, which will result in edge NOK.
    * Set detection tcs position to (x,y) = (3,2)
    * Set expected pseudo x and y pos to mean of detections
    * Set expected meascov matrix to what the test output (at this level of the function, we only test that it is changed - not that the values are necessarily correct)
    */
   obj.ndets = 2U; // 1
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 3.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = 2.0F;
   assoc_dets_pos_tcs[1].y = 1.0F;

   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }
   exp_pseudo_pos_x = 2.5F;
   exp_pseudo_pos_y = 1.5F;
   exp_pseudo_pos_x_var = 1.18656504F;
   exp_pseudo_pos_y_var = 1.07379186F;
   exp_pseudo_pos_cov_xy = 0.105724864F;

   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect pseudo position to be the same as before function call
    * Expect the variance from previous iteration to have been multiplied by k_gain_no_estimate
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, obj.pseudo_vcs_position.x, F360_EPSILON,
                      "The pseudo position in x direction did not match the expected value when mean calculation failed.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, obj.pseudo_vcs_position.y, F360_EPSILON,
                      "The pseudo position in y direction did not match the expected value when mean calculation failed.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x_var, obj.meascov[0][0], F360_EPSILON,
                      "The variance of the pseudo position in x direction did not match the expected value when mean calculation failed.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_var, obj.meascov[1][1], F360_EPSILON,
                      "The variance of the pseudo position in y direction did not match the expected value when mean calculation failed.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, obj.meascov[0][1], F360_EPSILON,
                      "The covariance between x component and y component of the pseudo position is not correct.");
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, obj.meascov[1][0], F360_EPSILON,
                      "The covariance between x component and y component of the pseudo position is not correct.");

}

/**
*\purpose  Verify that correct pseudo position is set when object is far away (i.e. treated as a point target).
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_Point_Target)
{
   /** \precond
    * Set object position to (x,y) = (70, 0)m
    * Set object reference point to REAR
    * Set obj.ndets to 2
    * Set detection tcs position to be close to host, on equal y distance from center
    */
   const float32_t test_thresh = 0.1F; // Needs to be quite big due to numerical errors
   obj.ndets = 2U; // 1
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.vcs_position.x = 70.0F;
   obj.Update_Bbox_Center();
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;

   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }
   exp_pseudo_pos_x = 70.0F;
   exp_pseudo_pos_y = 0.0F;

   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect pseudo position to be the same as before function call
    * Expect the variance from previous iteration to have been multiplied by k_gain_no_estimate
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, obj.pseudo_vcs_position.x, test_thresh,
                      "The pseudo position in x direction did not match the expected value when mean calculation failed.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, obj.pseudo_vcs_position.y, test_thresh,
                      "The pseudo position in y direction did not match the expected value when mean calculation failed.")
}


/**
*\purpose Test to to check that all properties are set to get high uncertainty for X in tcs, thus in vcs as well
* since the object has orientation of 0 rad.
* The properties that are set accordingly are:
* - Object is in blindzone
* - Object length is less than 1.5 m
* - Object speed is in relevant range (2 m/s to 4 m/s)
* - Host is stationary
* - Object is roughly ongoing or oncoming
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Big)
{
   /** \precond
    * Change object position and size so it is in Blindzone
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);
   obj.vcs_heading = Angle{ 0.0F };
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   const float32_t exp_vcs_x_pos_cov = calibrations.k_pseudo_pos_high_uncertainity;
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to the high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the low  movable probability.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Low_Movable_Prob)
{
   /** \precond
    * Change object position and size so it is in Blindzone
    * Change the movable probability to be less than 0.5
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);
   obj.movable_prob = 0.4;
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.Set_Bbox_Orientation(obj.vcs_heading);


   const float32_t exp_vcs_x_pos_cov = 1.232351; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of having vcs y > 5.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Big_VCS_Y_Position)
{
    /** \precond
     * Change object position and size so it is in no FOV for at least one sensor
     * Set the center of the object bbox accordingly
     * Set as rear left the reference point and min projection reference point
     * Set the expected pseudo x expected value
     * Set threshold for the test
     * Set detections just to not get error for no detections associated
     */
    obj.vcs_position.x = -2.0F;
    obj.vcs_position.y = 50.0F;
    obj.bbox.Set_Length(1.0F);
    obj.bbox.Set_Width(1.0F);
    obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.Set_Bbox_Orientation(obj.vcs_heading);


    const float32_t exp_vcs_x_pos_cov = 3.876699F; // Should be unchanged
    const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

    obj.ndets = 2U;
    obj.detids[0U] = 1U;
    obj.detids[1U] = 2U;
    assoc_dets_pos_tcs[0].x = 1.0F;
    assoc_dets_pos_tcs[0].y = 2.0F;
    assoc_dets_pos_tcs[1].x = -2.0F;
    assoc_dets_pos_tcs[1].y = -2.0F;
    for (uint32_t i = 0U; i < obj.ndets; i++)
    {
        Convert_TCS_Posn_To_VCS_Posn(
            assoc_dets_pos_tcs[i].x,
            assoc_dets_pos_tcs[i].y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            det_props[i].vcs_position.x,
            det_props[i].vcs_position.y);
    }


    /** \action
     * Call function
     */
    Pseudo_Position_Estimation(
        calibrations,
        host,
        det_props,
        sensors,
        globals,
        obj);

    /** \result
     * Expect covariance for x to be equal to not have high uncertainity value
     */
    DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov, obj.meascov[0][0], test_thresh,
        "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of beeing in the blindzone.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Not_Being_In_Blindzone)
{
    /** \precond
     * Change object position and size so it is in no FOV for at least one sensor
     * Set the center of the object bbox accordingly
     * Set as rear left the reference point and min projection reference point
     * Set the expected pseudo x expected value
     * Set threshold for the test
     * Set detections just to not get error for no detections associated
     */
    obj.vcs_position.x = -2.0F;
    obj.vcs_position.y = 4.9F;
    obj.bbox.Set_Length(1.0F);
    obj.bbox.Set_Width(1.0F);
    obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.Set_Bbox_Orientation(obj.vcs_heading);


    const float32_t exp_vcs_x_pos_cov = 1.142708F; // Should be unchanged
    const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

    obj.ndets = 2U;
    obj.detids[0U] = 1U;
    obj.detids[1U] = 2U;
    assoc_dets_pos_tcs[0].x = 1.0F;
    assoc_dets_pos_tcs[0].y = 2.0F;
    assoc_dets_pos_tcs[1].x = -2.0F;
    assoc_dets_pos_tcs[1].y = -2.0F;
    for (uint32_t i = 0U; i < obj.ndets; i++)
    {
        Convert_TCS_Posn_To_VCS_Posn(
            assoc_dets_pos_tcs[i].x,
            assoc_dets_pos_tcs[i].y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            det_props[i].vcs_position.x,
            det_props[i].vcs_position.y);
    }


    /** \action
     * Call function
     */
    Pseudo_Position_Estimation(
        calibrations,
        host,
        det_props,
        sensors,
        globals,
        obj);

    /** \result
     * Expect covariance for x to be equal to not have high uncertainity value
     */
    DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov, obj.meascov[0][0], test_thresh,
        "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the host not beeing stationary.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Moving_Host)
{
   /** \precond
    * Change object position and size so it is in Blindzone
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    * Set host speed equal to 5 m/s so it is not stationary
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);

   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   host.speed = 5.0F;

   const float32_t exp_vcs_x_pos_cov = 1.232351; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the length of the object.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Big_Length)
{
   /** \precond
    * Change object position and size so it is in Blindzone for all sensors
    * Change length of the object to be bigger than the threshold for length
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.6F);
   obj.bbox.Set_Width(1.0F);
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.Set_Bbox_Orientation(obj.vcs_heading);


   const float32_t exp_vcs_x_pos_cov = 1.235027; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the speed of the object.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Big_Object_Speed)
{
   /** \precond
    * Change object position and size so it is in Blindzone for all sensors
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Change speed of the object to be bigger than the threshold for speed
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);
   obj.speed = 10.0F;

   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT; 
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   const float32_t exp_vcs_x_pos_cov = 1.23235; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }

   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the speed of the object.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Low_Object_Speed)
{
   /** \precond
    * Change object position and size so it is in Blindzone for all sensors
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Change speed of the object to be smaller than the threshold for speed
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);
   obj.speed = 0.5F;

   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;  
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   const float32_t exp_vcs_x_pos_cov = 1.232351; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the heading of the object.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_Heading_Out_of_Thresholds)
{
   /** \precond
    * Change object position and size so it is in Blindzone for all sensors
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Change the object heading to be outside the thresholds
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);
   obj.vcs_heading = Angle{F360_DEG2RAD(40.0F)};
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   Point center = {-1.5F, 1.0F};
   obj.bbox.Set_Center(center);
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;  

   const float32_t exp_vcs_x_pos_cov = 1.124037; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for X in tcs except of the y position of the object,
* which is out of the threshold.
* Thus, the covariance in vcs X should not be changed.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_Due_To_y_Position_Out_of_Thresholds)
{
   /** \precond
    * Change object position and size so it is in Blindzone for all sensors
    * Change the object y position so its more than the threshold for y position
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Set the expected pseudo x expected value
    * Set threshold for the test
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 5.5F; // has to be bigger than 5.0
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);

   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;  
   obj.Set_Bbox_Orientation(obj.vcs_heading);


   const float32_t exp_vcs_x_pos_cov = 1.127903; // Should be unchanged
   const float32_t test_thresh = 0.01F; // Needs to be quite big due to numerical errors

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
    * Expect covariance for x to be equal to not have high uncertainity value
    */
   DOUBLES_EQUAL_TEXT(exp_vcs_x_pos_cov,  obj.meascov[0][0], test_thresh,
   "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to get high uncertainity for an oncoming object
* Thus, the covariance in vcs X should be set to a high value defined in the calibrations.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Big_But_Object_Is_Oncoming)
{
   /** \precond
    * Change object position and size so it is in Blindzone for all sensors
    * Set the center of the object bbox accordingly
    * Set as rear left the reference point and min projection reference point
    * Set the expected pseudo x expected value
    * Set detections just to not get error for no detections associated
    */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 0.5F;
   obj.bbox.Set_Length(1.0F);
   obj.bbox.Set_Width(1.0F);
   obj.vcs_heading = Angle{F360_DEG2RAD(180.0F)}; // Oncoming
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;  
   obj.Set_Bbox_Orientation(obj.vcs_heading);

   // The heading is neither 0 or 180 which means that the covariance will be shared between x and y in vcs.
   // Their sum will be equal to calibrations.k_pseudo_pos_high_uncertainity
   const float32_t exp_vcs_pos_cov_total = calibrations.k_pseudo_pos_high_uncertainity; 

   obj.ndets = 2U;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   assoc_dets_pos_tcs[0].x = 1.0F;
   assoc_dets_pos_tcs[0].y = 2.0F;
   assoc_dets_pos_tcs[1].x = -2.0F;
   assoc_dets_pos_tcs[1].y = -2.0F;
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      Convert_TCS_Posn_To_VCS_Posn(
         assoc_dets_pos_tcs[i].x,
         assoc_dets_pos_tcs[i].y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         det_props[i].vcs_position.x,
         det_props[i].vcs_position.y);
   }


   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensors,
      globals,
      obj);

   /** \result
   * Expect covariance for x to be equal to not have high uncertainity value
   */
   CHECK_TRUE_TEXT(obj.meascov[0][0] >= exp_vcs_pos_cov_total, "The pseudo position covariance in x direction did not match the expected value")
}

/**
*\purpose Test to to check that all conditions are set to not get high uncertainity because heading is outside the +/- 35 degree
* (applied for both ongoing and oncoming) since all the other conditions are met
* Thus, the sum of covariance in vcs X and vcs Y should be smaller than a high value defined in the calibrations.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_But_Object_Is_Ongoing)
{
    /** \precond
     * Change object position and size so it is in Blindzone for all sensors
     * Set the center of the object bbox accordingly
     * Set as rear left the reference point and min projection reference point
     * Set the expected pseudo x expected value
     * Set detections just to not get error for no detections associated
     */
    obj.vcs_position.x = -2.0F;
    obj.vcs_position.y = 0.5F;
    obj.bbox.Set_Length(1.0F);
    obj.bbox.Set_Width(1.0F);
    obj.vcs_heading = Angle{ F360_DEG2RAD(40.0F) }; // Ongoing
    obj.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(40.0F) });

    obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.Set_Bbox_Orientation(obj.vcs_heading);

    // The heading is neither 0 or 180 which means that the covariance will be shared between x and y in vcs.
    // Since the extra meascov is not expected to be addded, a small value should be expected
    const float32_t exp_vcs_pos_cov_total = 3.0F;

    obj.ndets = 2U;
    obj.detids[0U] = 1U;
    obj.detids[1U] = 2U;
    assoc_dets_pos_tcs[0].x = 1.0F;
    assoc_dets_pos_tcs[0].y = 2.0F;
    assoc_dets_pos_tcs[1].x = -2.0F;
    assoc_dets_pos_tcs[1].y = -2.0F;
    for (uint32_t i = 0U; i < obj.ndets; i++)
    {
        Convert_TCS_Posn_To_VCS_Posn(
            assoc_dets_pos_tcs[i].x,
            assoc_dets_pos_tcs[i].y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            det_props[i].vcs_position.x,
            det_props[i].vcs_position.y);
    }


    /** \action
     * Call function
     */
    Pseudo_Position_Estimation(
        calibrations,
        host,
        det_props,
        sensors,
        globals,
        obj);

    /** \result
     * Expect covariance for x to be equal to not have high uncertainity value
     */
    CHECK_TRUE_TEXT(obj.meascov[0][0] < exp_vcs_pos_cov_total, "The pseudo position covariance in x direction did not match the expected value")
    CHECK_TRUE_TEXT(obj.meascov[1][1] < exp_vcs_pos_cov_total, "The pseudo position covariance in y direction did not match the expected value")


}

/**
*\purpose Test to to check that all conditions are set to not get high uncertainity because heading is outside the +/- 35 degree
* (applied for both ongoing and oncoming) since all the other conditions are met
* Thus, the sum of covariance in vcs X and vcs Y should be smaller than a high value defined in the calibrations.
*\req    NA
*/
TEST(f360_pseudo_position_estimation, Pseudo_Position_Estimation_VCS_X_Pos_Cov_Is_Not_Big_But_Object_Is_OnComing)
{
    /** \precond
     * Change object position and size so it is in Blindzone for all sensors
     * Set the center of the object bbox accordingly
     * Set as rear left the reference point and min projection reference point
     * Set the expected pseudo x expected value
     * Set detections just to not get error for no detections associated
     */
    obj.vcs_position.x = -2.0F;
    obj.vcs_position.y = 0.5F;
    obj.bbox.Set_Length(1.0F);
    obj.bbox.Set_Width(1.0F);
    obj.vcs_heading = Angle{ F360_DEG2RAD(140.0F) }; // Oncoming
    obj.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(140.0F) });

    obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    obj.Set_Bbox_Orientation(obj.vcs_heading);

    // The heading is neither 0 or 180 which means that the covariance will be shared between x and y in vcs.
    // Their sum will be equal to calibrations.k_pseudo_pos_high_uncertainity
    const float32_t exp_vcs_pos_cov_total = calibrations.k_pseudo_pos_high_uncertainity;

    obj.ndets = 2U;
    obj.detids[0U] = 1U;
    obj.detids[1U] = 2U;
    assoc_dets_pos_tcs[0].x = 1.0F;
    assoc_dets_pos_tcs[0].y = 2.0F;
    assoc_dets_pos_tcs[1].x = -2.0F;
    assoc_dets_pos_tcs[1].y = -2.0F;
    for (uint32_t i = 0U; i < obj.ndets; i++)
    {
        Convert_TCS_Posn_To_VCS_Posn(
            assoc_dets_pos_tcs[i].x,
            assoc_dets_pos_tcs[i].y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            det_props[i].vcs_position.x,
            det_props[i].vcs_position.y);
    }


    /** \action
     * Call function
     */
    Pseudo_Position_Estimation(
        calibrations,
        host,
        det_props,
        sensors,
        globals,
        obj);

    /** \result
     * Expect covariance for x to be equal to not have high uncertainity value
     */
    CHECK_TRUE_TEXT((obj.meascov[0][0] + obj.meascov[1][1]) < exp_vcs_pos_cov_total,
        "The pseudo position covariance in x direction did not match the expected value")
}
/** @}*/


/** \defgroup  f360_pseudo_position_estimation_Compute_Pseudo_Pos_Mid_Point_Of_Detections
 *  @{
 */

/** \brief
 * Testing of a function that calculates the mid point of a set of detections associated to an object.
 */

TEST_GROUP(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Mid_Point_Of_Detections)
{
   // Common variables used in tests
   F360_Object_Track_T obj;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];

   Point exp_mid_point;

   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up 3 detections. 
    */
   TEST_SETUP()
   {
      obj.ndets = 3;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 2U;
      obj.detids[2U] = 3U;

      det_props[0U].vcs_position.x = 10.0F;
      det_props[0U].vcs_position.y = -3.0F;
      det_props[1U].vcs_position.x = 13.0F;
      det_props[1U].vcs_position.y = 1.0F;
      det_props[2U].vcs_position.x = 9.0F;
      det_props[2U].vcs_position.y = 2.0F;
   }
};

/** \purpose
 * Test that correct mid points of a set of associated detections is returned. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Mid_Point_Of_Detections, Compute_Pseudo_Pos_Mid_Point_Of_Detections)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    */
   exp_mid_point.x = 0.5F * (9.0F + 13.0F);
   exp_mid_point.y = 0.5F * ((-3.0F) + 2.0F);

   /** \action
    * Call Compute_Pseudo_Pos_Mid_Point_Of_Detections
    */
   Point mid_point = Compute_Pseudo_Pos_Mid_Point_Of_Detections(obj, det_props);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_mid_point.x, mid_point.x, test_threshold, "Incorrect mid x returned.")
   DOUBLES_EQUAL_TEXT(exp_mid_point.y, mid_point.y, test_threshold, "Incorrect mid y returned.")
}



/** \purpose
 * Test that correct mid points of a set of associated detections is returned when there are only two detections with the same position. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Mid_Point_Of_Detections, Compute_Pseudo_Pos_Mid_Point_Of_Detections_2_Identical_Dets)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * Change the number of associated detections to 2
    * Place the second detection on top of the first one
    * Expected mid point is the same as the first (and second) detection
    */
   obj.ndets = 2;
   det_props[1U].vcs_position.x = det_props[0U].vcs_position.x;
   det_props[1U].vcs_position.y = det_props[0U].vcs_position.y;
   exp_mid_point.x = det_props[0U].vcs_position.x;
   exp_mid_point.y = det_props[0U].vcs_position.y;

   /** \action
    * Call Compute_Pseudo_Pos_Mid_Point_Of_Detections
    */
   Point mid_point = Compute_Pseudo_Pos_Mid_Point_Of_Detections(obj, det_props);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_mid_point.x, mid_point.x, test_threshold, "Incorrect mid x returned.")
   DOUBLES_EQUAL_TEXT(exp_mid_point.y, mid_point.y, test_threshold, "Incorrect mid y returned.")
}


/** \defgroup  f360_min_max_pseudo_pos_estimation
 *  @{
 */

/** \brief
 * Testing of a function that estimates the position of an object based
 * on position of associated detections using the min/max or weighted average approach depending on object's visible side.
 */

TEST_GROUP(f360_min_max_pseudo_pos_estimation)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   float32_t assoc_dets_para_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float32_t assoc_dets_orth_pos_tcs[MAX_DETS_IN_OBJ_TRK];

   // Expected pseudo x and y position and corresponding variances
   float32_t exp_pseudo_pos_x = 0.0F;
   float32_t exp_pseudo_pos_y = 0.0F;

   Point pseudo_pos_tcs = {};

   /** \setup
    * Initialize object with an arbitrary size, centroid in object center and VCS position to 0
    * Set x pos and y pos part of meascov to zero
    * Set gains to same values as in source file
    */
   TEST_SETUP()
   {      
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);

      obj.vcs_position.x = 0.0F;
      obj.vcs_position.y = 0.0F;
      Point center = {0.0F,0.0F};
      obj.bbox.Set_Center(center);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;

      obj.Set_Bbox_Orientation(Angle{ 0.0F });
   }

};

/**
*\purpose  Verify that correct pseudo position is set when front and right sides are visible.
*\req    NA
*/
TEST(f360_min_max_pseudo_pos_estimation, Pseudo_Position_Estimation_Pos_From_Front_Right_Sides)
{
   /** \precond
    * Set obj.ndets to 2
    * Set obj.reference_point to FRONT_RIGHT
    * Place a detection 0.3m away from edge in para and orth direction (in TCS)
    * Place a detection on object's reference position (in TCS)
    * Set expected pseudo x position to maximum x value of associated dets (in this case first detection)
    * Set expected pseudo y position to maximum y value of associated dets (in this case first detection)
    */
   obj.ndets = 2;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   obj.vcs_position = obj.bbox.Get_Corners().Front_Right();

   assoc_dets_para_pos_tcs[0] = 0.5F * obj.bbox.Get_Length() + 0.3F;
   assoc_dets_orth_pos_tcs[0] = 0.5F * obj.bbox.Get_Width() + 0.3F;
   assoc_dets_para_pos_tcs[1] = 2.0F;
   assoc_dets_orth_pos_tcs[1] = 1.0F;
   
   exp_pseudo_pos_x = assoc_dets_para_pos_tcs[0];
   exp_pseudo_pos_y = assoc_dets_orth_pos_tcs[0];

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      rear_front_side, 
      right_left_side, 
      assoc_dets_para_pos_tcs, 
      assoc_dets_orth_pos_tcs, 
      obj.ndets);

   /** \result
    * Expect pseudo position to have moved the centroid 0.3m in para and in orth direction respectively
    * Expect the pseudo position variance to be gain * 0.5 and gain * 0.2 in para and orth direction respectively
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, pseudo_pos_tcs.x, F360_EPSILON,
                      "The pseudo position in x direction did not match the expected value when front right side were visible.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, pseudo_pos_tcs.y, F360_EPSILON,
                      "The pseudo position in y direction did not match the expected value when front right side were visible.")
}

/**
*\purpose  Verify that correct pseudo position is set when rear and left sides are visible.
*\req    NA
*/
TEST(f360_min_max_pseudo_pos_estimation, Pseudo_Position_Estimation_Pos_From_Rear_Left_Sides)
{
   /** \precond
    * Set obj.ndets to 2
    * Set obj.reference_point to REAR_LEFT
    * Set first detection position to 0.1m away from edge in each direction.
    * Set second detection position to (-3, -1) m
    * Set weights in para and orth direction to 1.0F (this will also be normalized weight as ndets = 1)
    * Set expected pseudo x position to maximum x value of associated dets (in this case first detection)
    * Set expected pseudo y position to maximum y value of associated dets (in this case first detection)
    */
   obj.ndets = 2;
   obj.detids[0U] = 1U;
   obj.detids[1U] = 2U;
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   obj.vcs_position = obj.bbox.Get_Corners().Rear_Left();

   assoc_dets_para_pos_tcs[0] = -0.5F * obj.bbox.Get_Length() - 0.1F;
   assoc_dets_orth_pos_tcs[0] = -0.5F * obj.bbox.Get_Width() - 0.1F;
   assoc_dets_para_pos_tcs[1] = -3.0F;
   assoc_dets_orth_pos_tcs[1] = -1.0F;
   exp_pseudo_pos_x = -3.0F;
   exp_pseudo_pos_y = -1.1F;

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      rear_front_side, 
      right_left_side, 
      assoc_dets_para_pos_tcs, 
      assoc_dets_orth_pos_tcs, 
      obj.ndets);

   /** \result
    * Expect pseudo position to have moved the centroid 0.1m in para and in orth direction respectively
    * Expect the pseudo position variance to be gain * 0.1 and gain * 0.7 in para and orth direction respectively
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, pseudo_pos_tcs.x, F360_EPSILON,
                      "The pseudo position in x direction did not match the expected value when front right side were visible.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, pseudo_pos_tcs.y, F360_EPSILON,
                      "The pseudo position in y direction did not match the expected value when front right side were visible.")
}

/** \defgroup  f360_pseudo_position_estimation_Only_One_Side_Visible
 *  @{
 */

 /** \brief
  * Test setup to test that the correct pseudo position is calculated when only one edge of the object is seen.
  */
TEST_GROUP(f360_pseudo_position_estimation_Min_Max_Only_One_Side_Visible)
{
   F360_Object_Track_T obj = {};
   float32_t assoc_dets_para_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float32_t assoc_dets_orth_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   Point pseudo_pos_tcs = {};

   float32_t exp_pseudo_pos_x;
   float32_t exp_pseudo_pos_y;

   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up an object with LEFT as reference point, positioned at (-2,5).
    * Set up 3 associated detections positioned close to the object's left edge (close enough to each other to have Huber weights == 1)
    */
   TEST_SETUP()
   {
      obj.vcs_position.x = -2.0F;
      obj.vcs_position.y = 5.0F;
      obj.reference_point = F360_REFERENCE_POINT_LEFT;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(0.0F);
      obj.Update_Bbox_Center();
      obj.ndets = 3;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 2U;
      obj.detids[2U] = 3U;

      assoc_dets_para_pos_tcs[0] = 0.0F;
      assoc_dets_orth_pos_tcs[0] = -1.0F;
      assoc_dets_para_pos_tcs[1] = 2.0F;
      assoc_dets_orth_pos_tcs[1] = -1.1F;
      assoc_dets_para_pos_tcs[2] = -3.0F;
      assoc_dets_orth_pos_tcs[2] = -0.8F;
   }
};

/**
*\purpose  Verify that the correct pseudo position is calculated when object's reference point is LEFT
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Min_Max_Only_One_Side_Visible, Estimate_Para_Only_Left_Side_Visible)
{
   /** \precond
    * An object with ref point LEFT at (-2,5) is set up in the test group
    * Expected pseudo pos x is average of the detection xpos in TCS (in this example they're close enough to all have weight = 1)
    * Expected pseudo pos y is extreme value of detections ypos in TCS since right side of object is visible
   */
   exp_pseudo_pos_x = -7.0F / 3.0F;
   exp_pseudo_pos_y = 4.9F;
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      rear_front_side, 
      right_left_side, 
      assoc_dets_para_pos_tcs, 
      assoc_dets_orth_pos_tcs, 
      obj.ndets);

   Point pseudo_pos_vcs = {};
   Convert_TCS_Posn_To_VCS_Posn(
      pseudo_pos_tcs.x,
      pseudo_pos_tcs.y,
      obj.bbox.Get_Center().x,
      obj.bbox.Get_Center().y,
      obj.bbox.Get_Orientation(),
      pseudo_pos_vcs.x,
      pseudo_pos_vcs.y);

   /** \result
    * Check that calculated pseudo pos corresponds to expected one.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, pseudo_pos_vcs.x, test_threshold,"Pseudo pos x is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, pseudo_pos_vcs.y, test_threshold,"Pseudo pos y is incorrect.");
}

/**
*\purpose  Verify that the correct pseudo position is calculated when object's reference point is RIGHT
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Min_Max_Only_One_Side_Visible, Estimate_Para_Only_Right_Side_Visible)
{
   /** \precond
    * An object with ref point RIGHT at (0,-10) is set up
    * Place 3 detections along the right edge of the object
    * Expected pseudo pos x is average of the detection xpos in TCS (in this example they're close enough to all have weight = 1)
    * Expected pseudo pos y is extreme value of detections ypos in TCS since right side of object is visible
   */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   obj.vcs_position = {0.0F, -10.0F};
   obj.Update_Bbox_Center();

   assoc_dets_para_pos_tcs[0] = 1.0F;
   assoc_dets_orth_pos_tcs[0] = 1.1F;
   assoc_dets_para_pos_tcs[1] = -4.0F;
   assoc_dets_orth_pos_tcs[1] = 0.9F;
   assoc_dets_para_pos_tcs[2] = 0.0F;
   assoc_dets_orth_pos_tcs[2] = 1.0F;

   exp_pseudo_pos_x = -1.0F;
   exp_pseudo_pos_y = -9.9F;
   
   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      rear_front_side, 
      right_left_side, 
      assoc_dets_para_pos_tcs, 
      assoc_dets_orth_pos_tcs, 
      obj.ndets);

   Point pseudo_pos_vcs = {};
   Convert_TCS_Posn_To_VCS_Posn(
      pseudo_pos_tcs.x,
      pseudo_pos_tcs.y,
      obj.bbox.Get_Center().x,
      obj.bbox.Get_Center().y,
      obj.bbox.Get_Orientation(),
      pseudo_pos_vcs.x,
      pseudo_pos_vcs.y);

   /** \result
    * Check that calculated pseudo pos corresponds to expected one.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, pseudo_pos_vcs.x, test_threshold,"Pseudo pos x is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, pseudo_pos_vcs.y, test_threshold,"Pseudo pos y is incorrect.");
}

/**
*\purpose  Verify that the correct pseudo position is calculated when object's reference point is FRONT
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Min_Max_Only_One_Side_Visible, Estimate_Para_Only_Front_Side_Visible)
{
   /** \precond
    * Set object ref point to FRONT at position (-10,0)
    * Set up 3 associated detections around the objec't front edge
    * Expected pseudo pos y is average of the detection ypos in TCS (in this example they're close enough to all have weight = 1)
    * Expected pseudo pos x is extreme value of detections xpos in TCS since right side of object is visible
   */
   obj.vcs_position = {-10.0F, 0.0F};
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   obj.Update_Bbox_Center();

   assoc_dets_para_pos_tcs[0] = 2.0F;
   assoc_dets_orth_pos_tcs[0] = -1.0F;
   assoc_dets_para_pos_tcs[1] = 2.0F;
   assoc_dets_orth_pos_tcs[1] = 0.0F;
   assoc_dets_para_pos_tcs[2] = 2.0F;
   assoc_dets_orth_pos_tcs[2] = 2.0F;
   exp_pseudo_pos_x = -11.0F;
   exp_pseudo_pos_y = 0.33333F;
   
   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      rear_front_side, 
      right_left_side, 
      assoc_dets_para_pos_tcs, 
      assoc_dets_orth_pos_tcs, 
      obj.ndets);

   Point pseudo_pos_vcs = {};
   Convert_TCS_Posn_To_VCS_Posn(
      pseudo_pos_tcs.x,
      pseudo_pos_tcs.y,
      obj.bbox.Get_Center().x,
      obj.bbox.Get_Center().y,
      obj.bbox.Get_Orientation(),
      pseudo_pos_vcs.x,
      pseudo_pos_vcs.y);

   /** \result
    * Check that calculated pseudo pos corresponds to expected one.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, pseudo_pos_vcs.x, test_threshold,"Pseudo pos x is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, pseudo_pos_vcs.y, test_threshold,"Pseudo pos y is incorrect.");
}

/**
*\purpose  Verify that the correct pseudo position is calculated when object's reference point is REAR
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Min_Max_Only_One_Side_Visible, Estimate_Para_Only_Rear_Side_Visible)
{
   /** \precond
    * Set object ref point to REAR at position (10,0)
    * Set up 3 associated detections along the objec't rear edge
    * Expected pseudo pos y is average of the detection ypos in TCS (in this example they're close enough to all have weight = 1)
    * Expected pseudo pos x is extreme value of detections xpos in TCS since right side of object is visible
   */
   obj.vcs_position = {10.0F, 0.0F};
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   obj.Update_Bbox_Center();

   assoc_dets_para_pos_tcs[0] = -3.0F;
   assoc_dets_orth_pos_tcs[0] = -3.0F;
   assoc_dets_para_pos_tcs[1] = -3.0F;
   assoc_dets_orth_pos_tcs[1] = -2.0F;
   assoc_dets_para_pos_tcs[2] = -3.0F;
   assoc_dets_orth_pos_tcs[2] = -1.0F;
   exp_pseudo_pos_x = 10.0F;
   exp_pseudo_pos_y = -2.0F;
   
   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      rear_front_side, 
      right_left_side, 
      assoc_dets_para_pos_tcs, 
      assoc_dets_orth_pos_tcs, 
      obj.ndets);

   Point pseudo_pos_vcs = {};
   Convert_TCS_Posn_To_VCS_Posn(
      pseudo_pos_tcs.x,
      pseudo_pos_tcs.y,
      obj.bbox.Get_Center().x,
      obj.bbox.Get_Center().y,
      obj.bbox.Get_Orientation(),
      pseudo_pos_vcs.x,
      pseudo_pos_vcs.y);

   /** \result
    * Check that calculated pseudo pos corresponds to expected one.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_x, pseudo_pos_vcs.x, test_threshold,"Pseudo pos x is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y, pseudo_pos_vcs.y, test_threshold,"Pseudo pos y is incorrect.");
}
/** @}*/


/** \defgroup  f360_pseudo_position_estimation_check_Adjust_Pseudo_Cov_TCS_Wrt_Visibility_is_called
 *  @{
 */

/** \brief
 * Verify that Adjust_Pseudo_Cov_TCS_Wrt_Visibility() is called as expected in Pseudo_Position_Estimation()
 */

TEST_GROUP(f360_pseudo_position_estimation_check_Adjust_Pseudo_Cov_TCS_Wrt_Visibility_is_called)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Calibrations_T calibrations = {};
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensor[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set calibrations.k_pseudo_pos_high_uncertainity to 1337
    * Set obj.ndets to 1
    * Set obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT
    * Set obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT
    * Set all entrie in obj.meascov to 0
    */
   TEST_SETUP()
   {
      calibrations.k_pseudo_pos_high_uncertainity = 1337.0F;

      calibrations.k_pseudo_pos_cov_matrix_bias = 1.0F;
      calibrations.k_pseudo_pos_max_variance_threshold = 1.0F;
      
      obj.ndets = 1;
      obj.detids[0U] = 1U;
      obj.bbox.Set_Length(2.0F);
      obj.bbox.Set_Width(2.0F);
      
      obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
      obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;

      obj.meascov[0][0] = 0.0F;
      obj.meascov[0][1] = 0.0F;
      obj.meascov[1][0] = 0.0F;
      obj.meascov[1][1] = 0.0F;
   }
};

/**
*\purpose  Verify that Adjust_Pseudo_Cov_TCS_Wrt_Visibility is called for moveable objects
*\req    NA
*/
TEST(f360_pseudo_position_estimation_check_Adjust_Pseudo_Cov_TCS_Wrt_Visibility_is_called, Moveable_Object)
{
   /** \precond
    * Set obj.movable_prob to 1.0
    */
   obj.movable_prob = 1.0F;

   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensor,
      globals,
      obj);

   /** \result
    * Verify obj.meascov[0][0] and obj.meascov[1][1] is set to 1337
    * Verify obj.meascov[0][1] and obj.meascov[1][0] is set to 0
    */
   DOUBLES_EQUAL(1337.0F, obj.meascov[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, obj.meascov[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, obj.meascov[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, obj.meascov[1][1], F360_EPSILON)
}

/**
*\purpose  Verify that Adjust_Pseudo_Cov_TCS_Wrt_Visibility is not called for non-moveable objects
*\req    NA
*/
TEST(f360_pseudo_position_estimation_check_Adjust_Pseudo_Cov_TCS_Wrt_Visibility_is_called, Nonmoveable_Object)
{
   /** \precond
    * Set obj.movable_prob to 0.0
    */
   obj.movable_prob = 0.0F;

   /** \action
    * Call function
    */
   Pseudo_Position_Estimation(
      calibrations,
      host,
      det_props,
      sensor,
      globals,
      obj);

   /** \result
    * Verify obj.meascov[0][0] is set to 1
    * Verify obj.meascov[1][1] is set to 1
    * Verify obj.meascov[0][1] and obj.meascov[1][0] is set to 0
    */
   DOUBLES_EQUAL(1.0F, obj.meascov[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, obj.meascov[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, obj.meascov[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1.0F, obj.meascov[1][1], F360_EPSILON)
}


/** \defgroup  f360_pseudo_position_estimation_Raw_Pseudo_Pos_Cov
 *  @{
 */

 /** \brief
  * Test setup to test that the raw pseudo covariance for position in TCS is computed as expected.
  */
TEST_GROUP(f360_pseudo_position_estimation_Raw_Pseudo_Pos_Cov)
{
   F360_Object_Track_T obj = {};
   F360_Host_T host = {};
   F360_Calibrations_T calibs;
   float32_t pseudo_cov_tcs[2][2] = {};
   float32_t exp_pseudo_pos_cov_x;
   float32_t exp_pseudo_pos_cov_y;
   float32_t exp_pseudo_pos_cov_xy;

   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up an object with REAR LEFT as reference point, positioned at (10,5).
    * Pseudo position is placed slightly behind rear left corner at (9.8, 4.9)
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      host.dist_rear_axle_to_vcs_m = 3.0F;
      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 5.0F;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(0.0F);
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

      obj.pseudo_vcs_position.x = 9.8F;
      obj.pseudo_vcs_position.y = 4.9F;
   }
};

/**
*\purpose  Verify that the correct raw pseudo position cov is calculated when object's range is within max range
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Raw_Pseudo_Pos_Cov, Raw_Pseudo_Pos_Cov_Range_Below_Max)
{
   /** \precond
    * An object with ref point REAR LEFT at (10,5) is set up in the test group
    * Since range is below max range, no range saturation is needed
    * Exp data is set up to reflect that.
   */
   exp_pseudo_pos_cov_x = 0.2397F;
   exp_pseudo_pos_cov_y = 0.1952F;
   exp_pseudo_pos_cov_xy = 0.0238F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Compute_Raw_Pseudo_Pos_Cov_In_TCS(
      obj,
      host,
      pseudo_cov_tcs,
      calibs);

   /** \result
    * Check that calculated raw pseudo pos cov values correspond to expected ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the correct raw pseudo position cov is calculated when object's range is above max range
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Raw_Pseudo_Pos_Cov, Raw_Pseudo_Pos_Cov_Range_Above_Max)
{
   /** \precond
    * Set up an object with ref point REAR LEFT
    * Set position to (100, 60)
    * Set pseudo pos close to ref point at (99, 59)
    * Since range is above max range, range saturation is needed
    * Exp data is set up to reflect that.
   */
   obj.vcs_position.x = 100.0F;
   obj.vcs_position.y = 60.0F;
   obj.pseudo_vcs_position.x = 99.0F;
   obj.pseudo_vcs_position.y = 59.0F;
   exp_pseudo_pos_cov_x = 3.3090F;
   exp_pseudo_pos_cov_y = 9.1257F;
   exp_pseudo_pos_cov_xy = -5.2106F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Compute_Raw_Pseudo_Pos_Cov_In_TCS(
      obj,
      host,
      pseudo_cov_tcs,
      calibs);

   /** \result
    * Check that calculated raw pseudo pos cov values correspond to expected ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/** \defgroup  f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov
 *  @{
 */

 /** \brief
  * Test that the pseudo covariance for position in TCS is adjusted as expected.
  */
TEST_GROUP(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov)
{
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T obj = {};
   F360_Object_Sides_T rear_front_side = {};
   F360_Object_Sides_T right_left_side = {};
   Point pseudo_tcs_position = {};
   float32_t pseudo_cov_tcs[2][2] = {};

   float32_t exp_pseudo_pos_cov_x;
   float32_t exp_pseudo_pos_cov_y;
   float32_t exp_pseudo_pos_cov_xy;

   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up an object with REAR LEFT as reference point, positioned at (10,5)
    * Pseudo position is placed behind rear left corner at (9, 4), i.e. (-3, -2) in TCS
    * Set object filter type to CCA with slow speed and small time_since_initialization (to avoid adding extra covariance punishment for outlier pseudo pos).
    * Set object pseudo_pos_cov_outlier_count_orth to 0 (such that there is an effect from outlier covariance punishment if that logic is triggered)
    * Initialize elements of pseudo_cov_tcs to arbitrary non-zero values. 
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 5.0F;
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(0.0F);
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      rear_front_side = F360_OBJECT_SIDES_REAR;
      right_left_side = F360_OBJECT_SIDES_LEFT;
      pseudo_tcs_position = {-3.0F, -2.0F};
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      obj.speed = 0.0F;
      obj.time_since_initialization = 0.0F;
      obj.pseudo_pos_cov_outlier_count_orth = 0;

      pseudo_cov_tcs[0][0] = 2.0F;
      pseudo_cov_tcs[1][0] = 3.0F;
      pseudo_cov_tcs[0][1] = 3.0F;
      pseudo_cov_tcs[1][1] = 4.0F;
   }
};

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group
    *    - with pseudo pos close to ref point at (9, 4)
    *    - with object sides REAR and LEFT visible
   */
   exp_pseudo_pos_cov_x = 3.0F;
   exp_pseudo_pos_cov_y = 5.0F;
   exp_pseudo_pos_cov_xy = 3.0F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's pseudo position is over 20m away from the reference point
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_Oversaturation)
{
    /** \precond
     * An object with ref point at (50, 5)[vcs] 
     *    - with pseudo pos at (71, 7)[vcs]
    */
    exp_pseudo_pos_cov_x = 500.0F;
    exp_pseudo_pos_cov_y = 30.0F;
    exp_pseudo_pos_cov_xy = 0.0F;

    obj.vcs_position.x = 50.0F;
    obj.vcs_position.y = 5.0F;
    obj.bbox.Set_Length(20.0F);
    obj.bbox.Set_Width(3.0F);
    obj.bbox.Set_Orientation(0.0F);
    obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

    Point pseudo_vcs_position = { 71.0F, 7.0F };

    /** \action
     * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
     */
    Adjust_Pseudo_Cov_TCS(
        calibs,
        pseudo_vcs_position,
        obj,
        pseudo_cov_tcs);

    /** \result
     * Check that calculated adjusted pseudo pos cov values correspond to expected ones
     */
    DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
        DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
        DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
        DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the code can handle saturation of covariance = 0.0F
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Handle_Pseudo_Cov_Zero)
{
    /** \precond
     * set cov 00 and 11 to -1
     * expected cov is 0 for it
    */
    exp_pseudo_pos_cov_x = 0.0F;
    exp_pseudo_pos_cov_y = 0.0F;
    exp_pseudo_pos_cov_xy = 1.0F;

    pseudo_cov_tcs[0][0] = -1.0F;
    pseudo_cov_tcs[1][0] = 1.0F;
    pseudo_cov_tcs[0][1] = 1.0F;
    pseudo_cov_tcs[1][1] = -1.0F;

    /** \action
     * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
     */
    Adjust_Pseudo_Cov_TCS(
        calibs,
        pseudo_tcs_position,
        obj,
        pseudo_cov_tcs);

    /** \result
     * Check that calculated adjusted pseudo pos cov values correspond to expected ones
     */
    DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
        DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
        DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
        DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear)
{
   /** \precond
    * Set object ref point to REAR
    * Set object position at (10, 0)
    * Set pseudo pos close to ref point at (9, 0), i.e (-3, 0) in TCS
    * Object front/rear side is set to REAR visible in test group
    * Set object right/left side invalid
   */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   right_left_side = F360_OBJECT_SIDES_INVALID;
   obj.vcs_position = {10.0F, 0.0F};
   pseudo_tcs_position = {-1.0F, 0.0F};
   exp_pseudo_pos_cov_x = 3.0F;
   exp_pseudo_pos_cov_y = 8.0F;
   exp_pseudo_pos_cov_xy = 3.0F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated raw pseudo pos cov values correspond to exped ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Left)
{
   /** \precond
    * Set object ref point to LEFT
    * Set object position at (-2, 6)
    * Set pseudo pos close to ref point at (-2, 5), i.e (0, -2) in TCS
    * Set object front/rear side to invalid
    * Set object right/left side LEFT visible
   */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   rear_front_side = F360_OBJECT_SIDES_INVALID;
   right_left_side = F360_OBJECT_SIDES_LEFT;
   obj.vcs_position = {-1.0F, 6.0F};
   pseudo_tcs_position = {0.0F, -2.0F};
   exp_pseudo_pos_cov_x = 18.0F;
   exp_pseudo_pos_cov_y = 5.0F;
   exp_pseudo_pos_cov_xy = 3.0F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is CENTER
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Center)
{
   /** \precond
    * Set object ref point to CENTER
    * Set object position at (-2, 6)
    * Set pseudo pos close to ref point at (-1, 6), i.e (0, -2) in TCS
    * Set object front/rear side to invalid
    * Set object right/left side to invalid
   */
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   rear_front_side = F360_OBJECT_SIDES_INVALID;
   right_left_side = F360_OBJECT_SIDES_INVALID;
   obj.vcs_position = {-1.0F, 6.0F};
   pseudo_tcs_position = {0.0F, -1.0F};
   exp_pseudo_pos_cov_x = 3.0F;
   exp_pseudo_pos_cov_y = 5.0F;
   exp_pseudo_pos_cov_xy = 3.0F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT and pseudo pos cov
*          values are above maximum, such that they get saturated.
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_Saturation)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group
    * Set pseudo pos close to ref point at (9, 4)
    * Set object sides REAR and LEFT visible
    * Set input pseudo pos cov elements close to max threshold such that they exceed it when updated and get saturated.
   */
   pseudo_cov_tcs[0][0] = calibs.k_pseudo_pos_max_variance_threshold - 0.1F;
   pseudo_cov_tcs[1][1] = calibs.k_pseudo_pos_max_variance_threshold - 0.2F;
   pseudo_cov_tcs[1][0] = calibs.k_pseudo_pos_max_variance_threshold - 0.3F;
   pseudo_cov_tcs[0][1] = pseudo_cov_tcs[1][0];

   exp_pseudo_pos_cov_x = calibs.k_pseudo_pos_max_variance_threshold;
   exp_pseudo_pos_cov_y = calibs.k_pseudo_pos_max_variance_threshold;
   exp_pseudo_pos_cov_xy = 28.8817F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_x, pseudo_cov_tcs[0][0], test_threshold, "The pseudo position cov in para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[1][0], test_threshold, "The pseudo position cov in para-orth direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_xy, pseudo_cov_tcs[0][1], test_threshold, "The pseudo position cov in orth_para direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_cov_y, pseudo_cov_tcs[1][1], test_threshold, "The pseudo position cov in orth direction did not match the expected value.")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT and object is CTCA
*          such that extra covariance is added when object position and pseudo position differ (outlier handling).
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_CTCA_Outlier_Handling)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group with
    *    - Pseudo pos close to ref point at (9, 4), i.e. (-3, -2) in TCS
    *    - Object sides REAR and LEFT visible
    * Set object filter type to CTCA
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   float32_t pseudo_pos_cov_x_before_outlier_handling = 3.0F;
   float32_t pseudo_pos_cov_y_before_outlier_handling = 5.0F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   CHECK_FALSE_TEXT(pseudo_cov_tcs[0][0] == pseudo_pos_cov_x_before_outlier_handling, "No extra cov was addded to para pseudo pos cov")
   CHECK_FALSE_TEXT(pseudo_cov_tcs[1][1] == pseudo_pos_cov_y_before_outlier_handling, "No extra cov was addded to orth pseudo pos cov")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT and object is CCA with
*          large time_since_init such that extra covariance is added when object position and pseudo position differ (outlier handling).
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_CCA_Outlier_Handling_When_Large_Time_Since_Init)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group with
    *    - Pseudo pos close to ref point at (9, 4), i.e. (-3, -2) in TCS
    *    - Object sides REAR and LEFT visible
    * Set object filter type to CCA
    * Set time_since_initialization larger than calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca (calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca + 1e-3 is used in the test)
    * Set speed larger than calibs.fast_moving_thresh
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.time_since_initialization = calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca + 1e-6F;
   float32_t pseudo_pos_cov_x_before_outlier_handling = 3.0F;
   float32_t pseudo_pos_cov_y_before_outlier_handling = 5.0F;
   obj.speed = calibs.fast_moving_thresh + 0.1F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);
   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs[0][0] - pseudo_pos_cov_x_before_outlier_handling) > F360_EPSILON, "No extra cov was addded to para pseudo pos cov")
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs[1][1] - pseudo_pos_cov_y_before_outlier_handling) > F360_EPSILON, "No extra cov was addded to orth pseudo pos cov")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT and object is CCA with
*          small time_since_init such that no extra covariance is added when object position and pseudo position differ (outlier handling).
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_CCA_Outlier_Handling_When_Small_Time_Since_Init)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group with
    *    - Pseudo pos close to ref point at (9, 4), i.e. (-3, -2) in TCS
    *    - Object sides REAR and LEFT visible
    * Set object filter type to CCA
    * Set time_since_initialization smaller than calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca (calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca + 1e-3 is used in the test)
    * Set speed larger than calibs.fast_moving_thresh
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.time_since_initialization = calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca - 1e-6F;
   float32_t pseudo_pos_cov_x_before_outlier_handling = 3.0F;
   float32_t pseudo_pos_cov_y_before_outlier_handling = 5.0F;
   obj.speed = calibs.fast_moving_thresh + 0.1F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   CHECK_FALSE_TEXT(std::abs(pseudo_cov_tcs[0][0] - pseudo_pos_cov_x_before_outlier_handling) > F360_EPSILON, "Extra cov was unexpectedly addded to para pseudo pos cov")
   CHECK_FALSE_TEXT(std::abs(pseudo_cov_tcs[1][1] - pseudo_pos_cov_y_before_outlier_handling) > F360_EPSILON, "Extra cov was unexpectedly addded to para pseudo pos cov")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT and object is CCA with
*          small time_since_init such that extra covariance is added when object position and pseudo position differ (outlier handling).
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_CCA_No_Outlier_Handling_When_Small_Time_Since_Init)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group with
    *    - Pseudo pos close to ref point at (9, 4), i.e. (-3, -2) in TCS
    *    - Object sides REAR and LEFT visible
    * Set object filter type to CCA
    * Set time_since_initialization smaller than calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca (calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca - 1e-3 is used in the test)
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.time_since_initialization = calibs.k_time_since_init_th_to_enable_outlier_mitigation_cca - 1e-6F;
   float32_t pseudo_pos_cov_x_before_outlier_handling = 3.0F;
   float32_t pseudo_pos_cov_y_before_outlier_handling = 5.0F;
   
   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Check that calculated adjusted pseudo pos cov values correspond to expected ones
    */
   CHECK_FALSE_TEXT(std::abs(pseudo_cov_tcs[0][0] - pseudo_pos_cov_x_before_outlier_handling) > F360_EPSILON, "Extra cov was unexpectedly addded to para pseudo pos cov")
   CHECK_FALSE_TEXT(std::abs(pseudo_cov_tcs[1][1] - pseudo_pos_cov_y_before_outlier_handling) > F360_EPSILON, "Extra cov was unexpectedly addded to orth pseudo pos cov")
}

/**
*\purpose  Verify that the correct adjusted pseudo position cov is calculated when object's ref point is REAR LEFT and object is CTCA and its pseudo_pos_cov_outlier_count_orth is nonzero (extra covariance is added but decreased linearly when the counter is increasing)
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_CCA_Outlier_Handling_When_Increasing_pseudo_pos_cov_outlier_count_orth)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group with
    *    - Pseudo pos further away than calibs.k_pseudo_pos_dist_diff_thr (in [9, 4], i.e. [-3, -2] in TCS)
    * Set object filter type to CTCA (such that outlier mitigigation algo gets triggered)
    * Set calibs.k_max_num_consistent_outliers_orth to 3
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   calibs.k_max_num_consistent_outliers_orth = 3;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS() first time
    */
   float32_t pseudo_cov_tcs1[2][2] = {{pseudo_cov_tcs[0][0], pseudo_cov_tcs[0][1]}, {pseudo_cov_tcs[1][0], pseudo_cov_tcs[1][1]}};
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs1);
   const int32_t counter_first_time = obj.pseudo_pos_cov_outlier_count_orth;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS() second time
    */
   float32_t pseudo_cov_tcs2[2][2] = {{pseudo_cov_tcs[0][0], pseudo_cov_tcs[0][1]}, {pseudo_cov_tcs[1][0], pseudo_cov_tcs[1][1]}};
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs2);
   const int32_t counter_second_time = obj.pseudo_pos_cov_outlier_count_orth;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS() third time
    */
   float32_t pseudo_cov_tcs3[2][2] = {{pseudo_cov_tcs[0][0], pseudo_cov_tcs[0][1]}, {pseudo_cov_tcs[1][0], pseudo_cov_tcs[1][1]}};
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs3);
   const int32_t counter_third_time = obj.pseudo_pos_cov_outlier_count_orth;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS() fourth time
    */
   float32_t pseudo_cov_tcs4[2][2] = {{pseudo_cov_tcs[0][0], pseudo_cov_tcs[0][1]}, {pseudo_cov_tcs[1][0], pseudo_cov_tcs[1][1]}};
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs4);
   const int32_t counter_fourth_time = obj.pseudo_pos_cov_outlier_count_orth;

   /** \result
    * Verify that
    *  - the covariance in para direction is always the same after each call
    *  - the variance in orth is always smaller after each call up until call 3 and then saturates at same value
    *  - the counter is increasing after each call but saturated at calibs.k_max_num_consistent_outliers_orth
    */
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs1[0][0] - pseudo_cov_tcs2[0][0]) < F360_EPSILON, "Covaraiance in para direction is not same after first and second call")
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs2[0][0] - pseudo_cov_tcs2[0][0]) < F360_EPSILON, "Covaraiance in para direction is not same after second and third call")
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs3[0][0] - pseudo_cov_tcs4[0][0]) < F360_EPSILON, "Covaraiance in para direction is not same after third and fourth call")
   
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs1[1][1] - pseudo_cov_tcs2[1][1]) > F360_EPSILON, "Covaraiance in orth direction is not decreasing in second call")
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs2[1][1] - pseudo_cov_tcs3[1][1]) > F360_EPSILON, "Covaraiance in orth direction is not decreasing in third call")
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs3[1][1] - pseudo_cov_tcs4[1][1]) < F360_EPSILON, "Covaraiance in orth direction is not the same in third and fourth call")
   
   CHECK_TRUE_TEXT(counter_first_time == 1, "Counter was not increased in first call")
   CHECK_TRUE_TEXT(counter_second_time == 2, "Counter was not increased in second call")
   CHECK_TRUE_TEXT(counter_third_time == 3, "Counter was not increased in third call")
   CHECK_TRUE_TEXT(counter_fourth_time == 3, "Counter was not saturated at calibs.k_max_num_consistent_outliers_orth in fourth call")
}

/**
*\purpose  Verify that the protection against zero division inside algo for computing additional covariance punishment in case of an pseudo position outlier works as expected.
*          Expected is that there will be no additional punishment if calibs.k_max_num_consistent_outliers_orth <= 1
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Rear_Left_CCA_Outlier_Handling_When_Increasing_protection_zero_division)
{
   /** \precond
    * An object with ref point REAR LEFT at (10, 5) is set up in the test group with
    *    - Pseudo pos further away than calibs.k_pseudo_pos_dist_diff_thr (in [9, 4], i.e. [-3, -2] in TCS)
    * Set object filter type to CTCA (such that outlier mitigigation algo gets triggered)
    * Set calibs.k_max_num_consistent_outliers_orth to 1
   */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   calibs.k_max_num_consistent_outliers_orth = 1;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_tcs_position,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Verify that no additional covariance was added to orth direction but only to para direction
    */
   float32_t pseudo_pos_cov_x_before_outlier_handling = 3.0F;
   float32_t pseudo_pos_cov_y_before_outlier_handling = 5.0F;
   CHECK_TRUE_TEXT(pseudo_cov_tcs[0][0] - pseudo_pos_cov_x_before_outlier_handling > F360_EPSILON, "Additional para covariance was not added when pseudo pos was an outlier")
   CHECK_TRUE_TEXT(std::abs(pseudo_cov_tcs[1][1] - pseudo_pos_cov_y_before_outlier_handling) < F360_EPSILON, "Additional orth covariance was added due to pseudo pos being an outlier")
}

/**
*\purpose  Verify that when an object is not flagged for fast shrinkage and has reference point LEFT, pseudo position covariance is saturated correctly at the default value.
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_No_Fast_Shrinkage_Left_Ref_Point)
{
   /** \precond
    * An object with ref point LEFT at (-2, 5) is set up
   *   - Pseudo pos close to ref point at (-1.8, 5), i.e. (0.2, 0) in TCS
    *  - Object side LEFT visible
    * - Set object filter type to CCA and speed to 0 such that outlier handling is not triggered
    * - Set length to 8m such that TCS x pseudo position covariance before saturation is larger than max threshold
    * - Set f_shrink_fast to false
   */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 5.0F;
   obj.bbox.Set_Length(8.0F);
   obj.bbox.Set_Width(2.0F);
   obj.bbox.Set_Orientation(0.0F);
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   rear_front_side = F360_OBJECT_SIDES_INVALID;
   right_left_side = F360_OBJECT_SIDES_LEFT;
   Point pseudo_pos_vcs = {-1.8F, 5.0F};
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.speed = 0.0F;
   obj.time_since_initialization = 0.0F;
   obj.pseudo_pos_cov_outlier_count_orth = 0;
   obj.f_shrink_fast = false;

   pseudo_cov_tcs[0][0] = 2.0F;
   pseudo_cov_tcs[1][0] = 3.0F;
   pseudo_cov_tcs[0][1] = 3.0F;
   pseudo_cov_tcs[1][1] = 4.0F;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_pos_vcs,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Verify that pseudo position covariance for TCS x is saturated correctly at the default value.
    */
   DOUBLES_EQUAL_TEXT(pseudo_cov_tcs[0][0], calibs.k_pseudo_pos_max_variance_threshold, test_threshold,"Additional para covariance was not added when pseudo pos was an outlier")
}

/**
*\purpose  Verify that when an object is not flagged for fast shrinkage and has reference point RIGHT, pseudo position covariance is saturated correctly at the default value.
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_No_Fast_Shrinkage_Right_Ref_Point)
{
   /** \precond
    * An object with ref point RIGHT at (-2, -5) is set up
   *   - Pseudo pos close to ref point at (-2.2, -5), i.e. (-0.2, 0) in TCS
    *  - Object side RIGHT visible
    * - Set object filter type to CCA and speed to 0 such that outlier handling is not triggered
    * - Set length to 8m such that TCS x pseudo position covariance before saturation is larger than max threshold
    * - Set f_shrink_fast to false
   */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = -5.0F;
   obj.bbox.Set_Length(8.0F);
   obj.bbox.Set_Width(2.0F);
   obj.bbox.Set_Orientation(0.0F);
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   rear_front_side = F360_OBJECT_SIDES_INVALID;
   right_left_side = F360_OBJECT_SIDES_RIGHT;
   Point pseudo_pos_vcs = {-2.2F, -5.0F};
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.speed = 0.0F;
   obj.time_since_initialization = 0.0F;
   obj.pseudo_pos_cov_outlier_count_orth = 0;
   obj.f_shrink_fast = false;

   pseudo_cov_tcs[0][0] = 2.0F;
   pseudo_cov_tcs[1][0] = 3.0F;
   pseudo_cov_tcs[0][1] = 3.0F;
   pseudo_cov_tcs[1][1] = 4.0F;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_pos_vcs,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Verify that pseudo position covariance for TCS x is saturated correctly at the default value.
    */
   DOUBLES_EQUAL_TEXT(pseudo_cov_tcs[0][0], calibs.k_pseudo_pos_max_variance_threshold, test_threshold,"Additional para covariance was not added when pseudo pos was an outlier")
}

/**
*\purpose  Verify that when an object is flagged for fast shrinkage and has reference point LEFT, pseudo position covariance is saturated correctly at the lower value.
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Fast_Shrinkage_Left_Ref_Point)
{
   /** \precond
    * An object with ref point LEFT at (-2, 5) is set up
   *   - Pseudo pos close to ref point at (-1.8, 5), i.e. (0.2, 0) in TCS
    *  - Object side LEFT visible
    * - Set object filter type to CCA and speed to 0 such that outlier handling is not triggered
    * - Set length to 8m such that TCS x pseudo position covariance before saturation is larger than max threshold
    * - Set f_shrink_fast to true
   */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = 5.0F;
   obj.bbox.Set_Length(8.0F);
   obj.bbox.Set_Width(2.0F);
   obj.bbox.Set_Orientation(0.0F);
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   rear_front_side = F360_OBJECT_SIDES_INVALID;
   right_left_side = F360_OBJECT_SIDES_LEFT;
   Point pseudo_pos_vcs = {-1.8F, 5.0F};
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.speed = 0.0F;
   obj.time_since_initialization = 0.0F;
   obj.pseudo_pos_cov_outlier_count_orth = 0;
   obj.f_shrink_fast = true;

   pseudo_cov_tcs[0][0] = 2.0F;
   pseudo_cov_tcs[1][0] = 3.0F;
   pseudo_cov_tcs[0][1] = 3.0F;
   pseudo_cov_tcs[1][1] = 4.0F;

   float32_t expected_saturated_value = 10.0F;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_pos_vcs,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Verify that pseudo position covariance for TCS x is saturated correctly at the default value.
    */
   DOUBLES_EQUAL_TEXT(pseudo_cov_tcs[0][0], expected_saturated_value, test_threshold,"Additional para covariance was not added when pseudo pos was an outlier")
}

/**
*\purpose  Verify that when an object is flagged for fast shrinkage and has reference point RIGHT, pseudo position covariance is saturated correctly at the lower value.
*/
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Pos_Cov, Adjust_Pseudo_Cov_TCS_Fast_Shrinkage_Right_Ref_Point)
{
   /** \precond
    * An object with ref point RIGHT at (-2, -5) is set up
   *   - Pseudo pos close to ref point at (-2.2, -5), i.e. (-0.2, 0) in TCS
    *  - Object side RIGHT visible
    * - Set object filter type to CCA and speed to 0 such that outlier handling is not triggered
    * - Set length to 8m such that TCS x pseudo position covariance before saturation is larger than max threshold
    * - Set f_shrink_fast to true
   */
   obj.vcs_position.x = -2.0F;
   obj.vcs_position.y = -5.0F;
   obj.bbox.Set_Length(8.0F);
   obj.bbox.Set_Width(2.0F);
   obj.bbox.Set_Orientation(0.0F);
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   rear_front_side = F360_OBJECT_SIDES_INVALID;
   right_left_side = F360_OBJECT_SIDES_RIGHT;
   Point pseudo_pos_vcs = {-2.2F, -5.0F};
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.speed = 0.0F;
   obj.time_since_initialization = 0.0F;
   obj.pseudo_pos_cov_outlier_count_orth = 0;
   obj.f_shrink_fast = true;

   pseudo_cov_tcs[0][0] = 2.0F;
   pseudo_cov_tcs[1][0] = 3.0F;
   pseudo_cov_tcs[0][1] = 3.0F;
   pseudo_cov_tcs[1][1] = 4.0F;

   float32_t expected_saturated_value = 10.0F;

   /** \action
    * Call function Compute_Raw_Pseudo_Pos_Cov_In_TCS()
    */
   Adjust_Pseudo_Cov_TCS(
      calibs,
      pseudo_pos_vcs,
      obj,
      pseudo_cov_tcs);

   /** \result
    * Verify that pseudo position covariance for TCS x is saturated correctly at the default value.
    */
   DOUBLES_EQUAL_TEXT(pseudo_cov_tcs[0][0], expected_saturated_value, test_threshold,"Additional para covariance was not added when pseudo pos was an outlier")
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers
 *  @{
 */

 /** \brief
  * Test if the num of pseudo pos outliers is assigned correctly
  */
TEST_GROUP(f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers)
{
   float32_t diff_pos = 0.0F;
   int32_t pseudo_pos_cov_outlier_count = 0;
   int32_t k_max_num_consistent_outliers = 5;
   float32_t k_pseudo_pos_dist_diff_thr = 0.9F;
   int32_t result;
};

/** \purpose
 * Check that Count_Pseudo_Pos_Outliers() increase value of pseudo_pos_cov_outlier_count when pseudo_pos_cov_outlier_count is smaller than k_max_pseudo_pos_cov_outlier_count
 * and position diff have the same sign as before.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers, Count_Pseudo_Pos_Outliers__Outlier_Count_Increase)
{
   /** \precond
    * Set position difference to something bigger than threshold.
    * Specify expected output from function.
    */
   diff_pos = 0.99F;
   pseudo_pos_cov_outlier_count = 1;

   /** \action
    * Call Count_Pseudo_Pos_Outliers()
    */
   result = Count_Pseudo_Pos_Outliers(k_max_num_consistent_outliers, k_pseudo_pos_dist_diff_thr, diff_pos, pseudo_pos_cov_outlier_count);

   /** \result
    * Check that pseudo_pos_cov_outlier_count is computed as expected.
    */
   CHECK_EQUAL(2, result)
}

/** \purpose
 * Check that Count_Pseudo_Pos_Outliers() decrease value of pseudo_pos_cov_outlier_count when pseudo_pos_cov_outlier_count is smaller than k_max_pseudo_pos_cov_outlier_count
 * and position diff have the same sign as before.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers, Count_Pseudo_Pos_Outliers__Outlier_Count_Decrease)
{
   /** \precond
    * Set position difference to something bigger than threshold.
    * Specify expected output from function.
    */
   diff_pos = -0.99F;
   pseudo_pos_cov_outlier_count = -1;

   /** \action
    * Call Count_Pseudo_Pos_Outliers()
    */
   result = Count_Pseudo_Pos_Outliers(k_max_num_consistent_outliers, k_pseudo_pos_dist_diff_thr, diff_pos, pseudo_pos_cov_outlier_count);

   /** \result
    * Check that pseudo_pos_cov_outlier_count is computed as expected.
    */
   CHECK_EQUAL(-2, result)
}

/** \purpose
 * Check that Count_Pseudo_Pos_Outliers() assign correct value to pseudo_pos_cov_outlier_count when pseudo_pos_cov_outlier_count is positive
 * and position diff have the different sign as before.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers, Count_Pseudo_Pos_Outliers__Outlier_Count_Different_Sign)
{
   /** \precond
    * Set position difference to something bigger than threshold.
    * Specify expected output from function.
    */
   diff_pos = -0.99F;
   pseudo_pos_cov_outlier_count = 1;

   /** \action
    * Call Count_Pseudo_Pos_Outliers()
    */
   result = Count_Pseudo_Pos_Outliers(k_max_num_consistent_outliers, k_pseudo_pos_dist_diff_thr, diff_pos, pseudo_pos_cov_outlier_count);

   /** \result
    * Check that pseudo_pos_cov_outlier_count is computed as expected.
    */
   CHECK_EQUAL(-1, result)
}

/** \purpose
 * Check that Count_Pseudo_Pos_Outliers() assigns 0 to pseudo_pos_cov_outlier_count when object is not an outlier
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers, Count_Pseudo_Pos_Outliers__No_Outlier_Positive_Count)
{
   /** \precond
    * Set position difference to something smaller than threshold.
    * Specify expected output from function.
    */
   diff_pos = 0.0F;
   pseudo_pos_cov_outlier_count = 10;

   /** \action
    * Call Count_Pseudo_Pos_Outliers()
    */
   result = Count_Pseudo_Pos_Outliers(k_max_num_consistent_outliers, k_pseudo_pos_dist_diff_thr, diff_pos, pseudo_pos_cov_outlier_count);

   /** \result
    * Check that pseudo_pos_cov_outlier_count is computed as expected.
    */
   CHECK_EQUAL(0, result)
}

/** \purpose
 * Check that Count_Pseudo_Pos_Outliers() assigns 0 to pseudo_pos_cov_outlier_count when object is not an outlier
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Count_Pseudo_Pos_Outliers, Count_Pseudo_Pos_Outliers__No_Outlier_Negative_Count)
{
   /** \precond
    * Set position difference to something smaller than threshold.
    * Specify expected output from function.
    */
   diff_pos = 0.0F;
   pseudo_pos_cov_outlier_count = -10;

   /** \action
    * Call Count_Pseudo_Pos_Outliers()
    */
   result = Count_Pseudo_Pos_Outliers(k_max_num_consistent_outliers, k_pseudo_pos_dist_diff_thr, diff_pos, pseudo_pos_cov_outlier_count);

   /** \result
    * Check that pseudo_pos_cov_outlier_count is computed as expected.
    */
   CHECK_EQUAL(0, result)
}
/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Pseudo_Cov_Inc_TCS
 *  @{
 */

 /** \brief
  * Test that the pseudo covariance increase for position in TCS is computed as expected.
  */
TEST_GROUP(f360_pseudo_position_estimation_Pseudo_Cov_Inc_TCS)
{
   F360_Object_Track_T obj = {};
   F360_Calibrations_T calibrations = {};
   Point centroid_pseudo_pos_tcs = {};
   F360_Object_Sides_T rear_front_side = {};
   F360_Object_Sides_T right_left_side = {};
   Point obj_var_tcs = {};
   float32_t pseudo_cov_tcs[2][2] = {};

   float32_t diff_pos = 0.0F;
   float32_t cov_pos_inc = 0.0F;

   float32_t exp_pseudo_cov_tcs[2][2] = {};
   float32_t exp_cov_pos_inc = 0.0F;

   /** \setup
    * Set up input parameters to relevant functions Adjust_Pseudo_Cov_TCS() and Compute_Pos_Cov_Inc_In_TCS().
    * Values are chosen in such a way that no edge has been correctly estimated (rear_front_side = right_left_side = F360_OBJECT_SIDES_INVALID).
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      Initialize_Tracker_Calibrations(calibrations);

      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.bbox.Set_Length(5.0F);
      obj.bbox.Set_Width(2.0F);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.vcs_position = obj.bbox.Get_Center();

      centroid_pseudo_pos_tcs.x = -0.89F;
      centroid_pseudo_pos_tcs.y = -0.89F;

      rear_front_side = F360_OBJECT_SIDES_INVALID;
      right_left_side = F360_OBJECT_SIDES_INVALID;
      // Let pseudo_cov_tcs be initialized to zero when both rear_front_side and right_left_side are F360_OBJECT_SIDES_INVALID

      obj_var_tcs.x = 2.0F;
      obj_var_tcs.y = 1.0F;
   }
};

/** \purpose
 * Check that Compute_Pos_Cov_Inc_In_TCS() returns the expected value when the distance between pseudo position and time predicted position is smaller
 * than the threshold (positive value).
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Pseudo_Cov_Inc_TCS, Compute_Pos_Cov_Inc_In_TCS__Short_Dist_Positive)
{
   /** \precond
    * Set position difference to something smaller than threshold.
    * Specify expected output from function.
    */
   diff_pos = 0.89F;
   exp_cov_pos_inc = 0.0F;

   /** \action
    * Call Compute_Pos_Cov_Inc_In_TCS()
    */
   cov_pos_inc = Compute_Pos_Cov_Inc_In_TCS(calibrations.k_pseudo_pos_dist_diff_thr, calibrations.k_pseudo_pos_dist_diff_gain, diff_pos);

   /** \result
    * Check that covariance increase is computed as expected.
    */
   DOUBLES_EQUAL_TEXT(exp_cov_pos_inc, cov_pos_inc, F360_EPSILON, "Covariance increase was not computed as expected")
}

/** \purpose
 * Check that Compute_Pos_Cov_Inc_In_TCS() returns the expected value when the distance between pseudo position and time predicted position is smaller
 * than the threshold (negative value).
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Pseudo_Cov_Inc_TCS, Compute_Pos_Cov_Inc_In_TCS__Short_Dist_Negative)
{
   /** \precond
    * Set position difference to something negative and, in absolute value, smaller than threshold.
    * Specify expected output from function.
    */
   diff_pos = -0.89F;
   exp_cov_pos_inc = 0.0F;

   /** \action
    * Call Compute_Pos_Cov_Inc_In_TCS()
    */
   cov_pos_inc = Compute_Pos_Cov_Inc_In_TCS(calibrations.k_pseudo_pos_dist_diff_thr, calibrations.k_pseudo_pos_dist_diff_gain, diff_pos);

   /** \result
    * Check that covariance increase is computed as expected.
    */
   DOUBLES_EQUAL_TEXT(exp_cov_pos_inc, cov_pos_inc, F360_EPSILON, "Covariance increase was not computed as expected")
}

/** \purpose
 * Check that Compute_Pos_Cov_Inc_In_TCS() returns the expected value when the distance between pseudo position and time predicted position is larger
 * than the threshold.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Pseudo_Cov_Inc_TCS, Compute_Pos_Cov_Inc_In_TCS__Large_Dist_Positive)
{
   /** \precond
    * Set position difference to something larger than threshold.
    * Specify expected output from function.
    */
   diff_pos = 0.91F;
   exp_cov_pos_inc = 0.05025089F;

   /** \action
    * Call Compute_Pos_Cov_Inc_In_TCS()
    */
   cov_pos_inc = Compute_Pos_Cov_Inc_In_TCS(calibrations.k_pseudo_pos_dist_diff_thr, calibrations.k_pseudo_pos_dist_diff_gain, diff_pos);

   /** \result
    * Check that covariance increase is computed as expected.
    */
   DOUBLES_EQUAL_TEXT(exp_cov_pos_inc, cov_pos_inc, F360_EPSILON, "Covariance increase was not computed as expected")
}

/** \purpose
 * Check that Compute_Pos_Cov_Inc_In_TCS() returns the expected value when the distance between pseudo position and time predicted position is larger
 * than the threshold (negative value).
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Pseudo_Cov_Inc_TCS, Compute_Pos_Cov_Inc_In_TCS__Large_Dist_Negative)
{
   /** \precond
    * Set position difference to something negative and, in absolute value, larger than threshold.
    * Specify expected output from function.
    */
   diff_pos = -0.91F;
   exp_cov_pos_inc = 0.05025089F;

   /** \action
    * Call Compute_Pos_Cov_Inc_In_TCS()
    */
   cov_pos_inc = Compute_Pos_Cov_Inc_In_TCS(calibrations.k_pseudo_pos_dist_diff_thr, calibrations.k_pseudo_pos_dist_diff_gain, diff_pos);

   /** \result
    * Check that covariance increase is computed as expected.
    */
   DOUBLES_EQUAL_TEXT(exp_cov_pos_inc, cov_pos_inc, F360_EPSILON, "Covariance increase was not computed as expected")
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Para
 *  @{
 */

 /** \brief
  * Test that the simplified calculation of reference para pos is done correctly.
  */
TEST_GROUP(f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Para)
{
   uint32_t ndets;
   Point assoc_dets_pos_tcs[MAX_DETS_IN_OBJ_TRK] = {};
   float assoc_dets_para_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float assoc_dets_orth_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float32_t ref_orth_pos;

   float32_t simplified_para_pos;
   float32_t exp_para_pos;

   const float32_t k_huber_threshold = 0.4F;
   const float32_t test_thresh = 0.001F;
   /** \setup
    * Set up a default scenario with 3 detections
    * ref_orth_pos is the para position of the detection with the smallest para position
    */
   TEST_SETUP()
   {
      ndets = 3U;
      assoc_dets_pos_tcs[0] = {1.0F, 3.0F};
      assoc_dets_pos_tcs[1] = {2.0F, 3.3F};
      assoc_dets_pos_tcs[2] = {3.0F, 7.0F};

      for (uint32_t i = 0; i < ndets; i++)
      {
         assoc_dets_para_pos_tcs[i] = assoc_dets_pos_tcs[i].x;
         assoc_dets_orth_pos_tcs[i] = assoc_dets_pos_tcs[i].y;
      }

      ref_orth_pos = assoc_dets_pos_tcs[0].y;
   }
};

/** \purpose
 * Check that Calc_Weighted_Average_Pseudo_Pos() returns the expected value.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Para, Calc_Simplified_Pseudo_Pos_Para_3_Dets)
{
   /** \precond
    * 3 detections have been set up in TEST_GROUP
    * Set expected para pos to weighted mean of detections para positions
    */
   
   exp_para_pos = 1.5714F;

   /** \action
    * Call Calc_Weighted_Average_Pseudo_Pos()
    */
   simplified_para_pos = Calc_Weighted_Average_Pseudo_Pos(ndets, assoc_dets_para_pos_tcs, assoc_dets_orth_pos_tcs, ref_orth_pos, k_huber_threshold);

   /** \result
    * Check that reference point para estimation is computed as expected
    */
   DOUBLES_EQUAL_TEXT(exp_para_pos, simplified_para_pos, test_thresh, "Simplified para pos is incorrect")
}

/** \purpose
 * Check that Calc_Weighted_Average_Pseudo_Pos() returns the expected value when detection's orth pos is very far from the reference point.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Para, Calc_Simplified_Pseudo_Pos_Para_1_Det)
{
   /** \precond
    * Set detection's orth pos extremely far away from the reference point to trigger a very small weight sum
    * Set expected output to the position of the detections TCS para position
    */
   ndets = 1;
   assoc_dets_orth_pos_tcs[0] = INFTY;
   assoc_dets_para_pos_tcs[0] = 10.0F;
   ref_orth_pos = -INFTY;
   exp_para_pos = assoc_dets_para_pos_tcs[0];

   /** \action
    * Call Calc_Weighted_Average_Pseudo_Pos()
    */
   simplified_para_pos = Calc_Weighted_Average_Pseudo_Pos(ndets, assoc_dets_para_pos_tcs, assoc_dets_orth_pos_tcs, ref_orth_pos, k_huber_threshold);

   /** \result
    * Check that reference point para estimation is computed as expected
    */
   DOUBLES_EQUAL_TEXT(exp_para_pos, simplified_para_pos, test_thresh, "Simplified para pos is incorrect")
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Orth
 *  @{
 */

 /** \brief
  * Test that the simplified calculation of reference orth pos is done correctly.
  */
TEST_GROUP(f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Orth)
{
   uint32_t ndets;
   Point assoc_dets_pos_tcs[MAX_DETS_IN_OBJ_TRK] = {};
   float assoc_dets_para_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float assoc_dets_orth_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float32_t ref_para_pos;

   float32_t simplified_orth_pos;
   float32_t exp_orth_pos;

   const float32_t k_huber_threshold = 0.4F;
   const float32_t test_thresh = 0.001F;
   /** \setup
    * Set up a default scenario with 3 detections
    * ref_orth_pos is the para position of the detection with the smallest para position
    */
   TEST_SETUP()
   {
      ndets = 3U;
      assoc_dets_pos_tcs[0] = {5.0F, -1.0F};
      assoc_dets_pos_tcs[1] = {8.0F, 2.0F};
      assoc_dets_pos_tcs[2] = {5.1F, 3.0F};
      for (uint32_t i = 0; i < ndets; i++)
      {
         assoc_dets_para_pos_tcs[i] = assoc_dets_pos_tcs[i].x;
         assoc_dets_orth_pos_tcs[i] = assoc_dets_pos_tcs[i].y;
      }
      ref_para_pos = assoc_dets_pos_tcs[0].x;
   }
};

/** \purpose
 * Check that Calc_Simplified_Pseudo_Pos_Para() returns the expected value.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calc_Simplified_Pseudo_Pos_Orth, Calc_Simplified_Pseudo_Pos_Orth_3_Dets)
{
   /** \precond
    * 3 detections have been set up in TEST_GROUP
    * Set expected orth pos to weighted mean of detections orth positions
    */
   
   exp_orth_pos = 1.0624F;

   /** \action
    * Call Calc_Simplified_Pseudo_Pos_Para()
    */
   simplified_orth_pos = Calc_Weighted_Average_Pseudo_Pos(ndets, assoc_dets_orth_pos_tcs, assoc_dets_para_pos_tcs, ref_para_pos, k_huber_threshold);

   /** \result
    * Check that reference point orth estimation is computed as expected
    */
   DOUBLES_EQUAL_TEXT(exp_orth_pos, simplified_orth_pos, test_thresh, "Simplified orth pos is incorrect")
}
/** @}*/


/** \defgroup  f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility
 *  @{
 */

 /** \brief
  * Test that the pseudo pos covariance is increased as expected depending on the
  * visibility of the object.
  */
TEST_GROUP(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility)
{
   F360_Object_Track_T obj = {};
   F360_Radar_Sensor_T sensor[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibrations = {};
   F360_Globals_T globals = {};
   float32_t pseudo_pos_cov_tcs[2][2] = {};

   /** \setup
    * Reset the object by calling Clear_Object_Track(obj)
    * 
    */
   TEST_SETUP()
   {
      Clear_Object_Track(obj);

      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(4.0F);

      globals.f_single_front_center_radar_only = true;

      calibrations.k_pseudo_pos_high_uncertainity = 1337.0F;

      sensor[0].variable.is_valid = true;
      
      globals.rotated_left_fov_normal[0][0] = 1.0F; // unit vector pointing 0 deg vcs
      globals.rotated_left_fov_normal[0][1] = 0.0F;
      
      globals.rotated_right_fov_normal[0][0] = 1.0F; // unit vector pointing 0 deg vcs
      globals.rotated_right_fov_normal[0][1] = 0.0F;

      pseudo_pos_cov_tcs[0][0] = 0.0F;
      pseudo_pos_cov_tcs[0][1] = 0.0F;
      pseudo_pos_cov_tcs[1][0] = 0.0F;
      pseudo_pos_cov_tcs[1][1] = 0.0F;
   }
};

/** \purpose
 * Verify pseudo pos cov is not increased when reference point is left and both left front and left rear are inside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Left__Both_Left_Corners_Visible)
{
   /** \precond
    * Set reference point to left
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 0 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(0.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is left and rear left corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Left__Rear_Left_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to left
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to -45 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(-45.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is left and front left corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Left__Front_Left_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to left
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to -135 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(-135.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when reference point is right and both front right and rear right corners are inside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Right__Both_Right_Corners_Visible)
{
   /** \precond
    * Set reference point to rear
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 0 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(0.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is right and right rear corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Right__Rear_Right_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to right
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 45 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(45.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is right and front right corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Right__Front_Right_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to right
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 135 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(135.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when reference point is rear and both rear right and rear left corners are inside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear__Both_Rear_Corners_Visible)
{
   /** \precond
    * Set reference point to rear
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 0 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(0.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is rear and rear right corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear__Rear_Right_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to rear
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 45 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(45.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1s] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is rear and rear left corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear__Rear_Left_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to rear
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to -45 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(-45.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1s] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when reference point is front and both front right and front left corners are inside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front__Both_Front_Corners_Visible)
{
   /** \precond
    * Set reference point to front
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 0 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(0.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is front and front right corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front__Front_Right_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to front
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to 135 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(135.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when reference point is front and front left corner is outside sensor fov
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front__Front_Left_Corner_Not_Visible)
{
   /** \precond
    * Set reference point to front
    * Set object center vcs position to x = 2.1, y = 0
    * Set object orientation to -135 deg vcs
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.bbox.Set_Center(Point{2.1F,0.0F});
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(-135.0F)});

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when "reference_point" and "min_projection_reference_point" are the same
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Left__No_Delta)
{
   /** \precond
    * Set reference point to front left
    * Set min projection reference point to front left
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs was changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Left__Delta_In_X_Dir)
{
   /** \precond
    * Set reference point to front left
    * Set min projection reference point to rear left
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Left__Delta_In_Y_Dir)
{
   /** \precond
    * Set reference point to front left
    * Set min projection reference point to front right
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x and y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Left__Delta_In_X_Y_Dir)
{
   /** \precond
    * Set reference point to front left
    * Set min projection reference point to rear right
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when "reference_point" and "min_projection_reference_point" are the same
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Left__No_Delta)
{
   /** \precond
    * Set reference point to rear left
    * Set min projection reference point to rear left
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs was changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Left__Delta_In_X_Dir)
{
   /** \precond
    * Set reference point to rear left
    * Set min projection reference point to front left
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Left__Delta_In_Y_Dir)
{
   /** \precond
    * Set reference point to rear left
    * Set min projection reference point to rear right
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x and y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Left__Delta_In_X_Y_Dir)
{
   /** \precond
    * Set reference point to rear left
    * Set min projection reference point to front right
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when "reference_point" and "min_projection_reference_point" are the same
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Right__No_Delta)
{
   /** \precond
    * Set reference point to front right
    * Set min projection reference point to front right
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs was changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Right__Delta_In_X_Dir)
{
   /** \precond
    * Set reference point to front right
    * Set min projection reference point to rear right
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Right__Delta_In_Y_Dir)
{
   /** \precond
    * Set reference point to front right
    * Set min projection reference point to front left
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x and y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Front_Right__Delta_In_X_Y_Dir)
{
   /** \precond
    * Set reference point to front right
    * Set min projection reference point to rear left
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is not increased when "reference_point" and "min_projection_reference_point" are the same
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Right__No_Delta)
{
   /** \precond
    * Set reference point to rear right
    * Set min projection reference point to rear left
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify no entry in pseudo_pos_cov_tcs was changed from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Right__Delta_In_X_Dir)
{
   /** \precond
    * Set reference point to rear right
    * Set min projection reference point to front right
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Right__Delta_In_Y_Dir)
{
   /** \precond
    * Set reference point to rear right
    * Set min projection reference point to rear left
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is increased when "reference_point" and "min_projection_reference_point" differ in tcs x and y direction
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Rear_Right__Delta_In_X_Y_Dir)
{
   /** \precond
    * Set reference point to rear right
    * Set min projection reference point to front right
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify pseudo_pos_cov_tcs[0][0] was set to 1337
    * Verify pseudo_pos_cov_tcs[1][1] was set to 1337
    * Verify the remaining entries in pseudo_pos_cov_tcs are unchanged from 0
    */
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(1337.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** \purpose
 * Verify pseudo pos cov is unchanged when reference point is center
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Adjust_Pseudo_Cov_TCS_Wrt_Visibility, Ref_Pnt_Center)
{
   /** \precond
    * Set reference point to center
    * Set min projection reference point to front right
    */
   pseudo_pos_cov_tcs[0][0] = 1.0F;
   pseudo_pos_cov_tcs[0][1] = 2.0F;
   pseudo_pos_cov_tcs[1][0] = 3.0F;
   pseudo_pos_cov_tcs[1][1] = 4.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;

   /** \action
    * Call Adjust_Pseudo_Cov_TCS_Wrt_Visibility()
    */
   Adjust_Pseudo_Cov_TCS_Wrt_Visibility(obj, sensor, calibrations, globals, pseudo_pos_cov_tcs);
   
   /** \result
    * Verify that all entries in pseudo_pos_cov_tcs are unchanged
    */
   DOUBLES_EQUAL(1.0F, pseudo_pos_cov_tcs[0][0], F360_EPSILON)
   DOUBLES_EQUAL(2.0F, pseudo_pos_cov_tcs[0][1], F360_EPSILON)
   DOUBLES_EQUAL(3.0F, pseudo_pos_cov_tcs[1][0], F360_EPSILON)
   DOUBLES_EQUAL(4.0F, pseudo_pos_cov_tcs[1][1], F360_EPSILON)
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Create_Position_Grid
 *  @{
 */

/** \brief
 * Testing of a function that estimates the position of an object based
 * on position of associated detections using the grid search approach.
 */

TEST_GROUP(f360_pseudo_position_estimation_Create_Position_Grid)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Calibrations_T calibrations = {};
   float32_t assoc_dets_para_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   float32_t assoc_dets_orth_pos_tcs[MAX_DETS_IN_OBJ_TRK];

   float32_t pos_grid[MAX_DETS_IN_OBJ_TRK] = {};
   uint32_t num_pos_points;

   float32_t exp_pos_grid[MAX_DETS_IN_OBJ_TRK] = {};
   uint32_t exp_num_pos_points;

   float32_t test_threshold = 0.0001F;
   /** \setup
    * Set up an object with
    *    - Position (20,5)
    *    - Reference point REAR_LEFT
    *    - Orientation 0
    *    - [length, width] = [4, 2]
    * Set up 4 detections placed close to the object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.vcs_position.x = 20.0F;
      obj.vcs_position.y = 5.0F;
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.ndets = 4;

      assoc_dets_para_pos_tcs[3] = 21.0F;
      assoc_dets_orth_pos_tcs[3] = 4.5F;
      assoc_dets_para_pos_tcs[1] = 22.0F;
      assoc_dets_orth_pos_tcs[1] = 5.5F;
      assoc_dets_para_pos_tcs[2] = 23.0F;
      assoc_dets_orth_pos_tcs[2] = 5.7F;
      assoc_dets_para_pos_tcs[0] = 22.0F;
      assoc_dets_orth_pos_tcs[0] = 6.0F;
   }

};

/** \purpose
 * Test that the correct grid points for the orthogonal grid are generated for an object with 4 assigned detections and reference point in REAR LEFT
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Create_Position_Grid, Orthogonal_Grid_Ref_Pnt_Rear_Left)
{
   /** \precond
    * An object and 4 detections have been set up in the TEST_GROUP
    * Object position = [20,5]
    * Reference point = REAR_LEFT
    * Expected grid points are the detection positions, shifted by the objects half width
    */
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);

   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      exp_pos_grid[i] = assoc_dets_orth_pos_tcs[i] + obj.bbox.Get_Width() * 0.5F;
   }
   exp_num_pos_points = obj.ndets;


   /** \action
    * Call Create_Position_Grid
    */
   Create_Position_Grid(assoc_dets_orth_pos_tcs, obj.ndets, right_left_side, obj.bbox.Get_Width(), pos_grid, num_pos_points);
   
   /** \result
    * Check that the generated grid points correspond to the expected values
    */
   CHECK_EQUAL_TEXT(exp_num_pos_points, num_pos_points, "Number of grid points is incorrect.")
   for (uint32_t i = 0U; i < num_pos_points; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_pos_grid[i], pos_grid[i], test_threshold, "Position grid point is incorrect.")
   }
}

/** \purpose
 * Test that the correct grid points for the orthogonal grid are generated for an object with 4 assigned detections and reference point in REAR
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Create_Position_Grid, Orthogonal_Grid_Ref_Pnt_Rear)
{
   /** \precond
    * An object and 4 detections have been set up in the TEST_GROUP
    * Object position = [20,5]
    * Reference point = REAR
    * Expected grid points are the detection positions, shifted by the objects half width
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);

   // Generate extected grid point: 11 points on the left of the median and 5 on the right. Increase by 0.1 from left-most detection for each step.
   exp_num_pos_points = 17U;
   for (uint32_t i = 0; i < exp_num_pos_points; i++)
   {
      exp_pos_grid[i] = assoc_dets_orth_pos_tcs[3] + static_cast<float32_t>(i) * 0.1F;
   }

   /** \action
    * Call Create_Position_Grid
    */
   Create_Position_Grid(assoc_dets_orth_pos_tcs, obj.ndets, right_left_side, obj.bbox.Get_Width(), pos_grid, num_pos_points);
   
   /** \result
    * Check that the generated grid points correspond to the expected values
    */
   CHECK_EQUAL_TEXT(exp_num_pos_points, num_pos_points, "Number of grid points is incorrect.")
   for (uint32_t i = 0U; i < num_pos_points; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_pos_grid[i], pos_grid[i], test_threshold, "Position grid point is incorrect.")
   }
}

/** \purpose
 * Test that the number of grid points for the orthogonal grid is correctly saturated when the detection spread is very big and reference point in REAR
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Create_Position_Grid, Orthogonal_Grid_Ref_Pnt_Rear_Num_Points_Saturated)
{
   /** \precond
    * An object and 4 detections have been set up in the TEST_GROUP
    * Make distance between smallest and largest orth pos of detections big.
    * Object position = [20,5]
    * Reference point = REAR
    * Expected grid points are the detection positions, shifted by the objects half width
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   const F360_Object_Sides_T right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   assoc_dets_orth_pos_tcs[3] = -1.0F;
   assoc_dets_orth_pos_tcs[0] = 16.0F;

   // Generate extected grid point: saturated to 79 points with the median in the middle and 39 steps in each direction with interval 0.1.
   exp_num_pos_points = 79U;
   float32_t median_pos = 5.6F;
   for (uint32_t i = 0; i < 40U; i++)
   {
      exp_pos_grid[39U - i] = median_pos - static_cast<float32_t>(i) * 0.1F;
   }
   for (uint32_t i = 0; i < 40U; i++)
   {
      exp_pos_grid[39U + i] = median_pos + static_cast<float32_t>(i) * 0.1F;
   }

   /** \action
    * Call Create_Position_Grid
    */
   Create_Position_Grid(assoc_dets_orth_pos_tcs, obj.ndets, right_left_side, obj.bbox.Get_Width(), pos_grid, num_pos_points);
   
   /** \result
    * Check that the generated grid points correspond to the expected values after saturation
    */
   CHECK_EQUAL_TEXT(exp_num_pos_points, num_pos_points, "Number of grid points is incorrect.")
   for (uint32_t i = 0U; i < num_pos_points; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_pos_grid[i], pos_grid[i], test_threshold, "Position grid point is incorrect.")
   }
}


/** \purpose
 * Test that the correct grid points for the paralell grid are generated for an object with 4 assigned detections and reference point in REAR LEFT
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Create_Position_Grid, Paralell_Grid_Ref_Pnt_Rear_Left)
{
   /** \precond
    * An object and 4 detections have been set up in the TEST_GROUP
    * Object position = [20,5]
    * Reference point = REAR_LEFT
    * Expected grid points are the detection positions, shifted by the objects half width
    */
   const F360_Object_Sides_T rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);

   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      exp_pos_grid[i] = assoc_dets_orth_pos_tcs[i] + obj.bbox.Get_Length() * 0.5F;
   }
   exp_num_pos_points = obj.ndets;


   /** \action
    * Call Create_Position_Grid
    */
   Create_Position_Grid(assoc_dets_orth_pos_tcs, obj.ndets, rear_front_side, obj.bbox.Get_Length(), pos_grid, num_pos_points);
   
   /** \result
    * Check that the generated grid points correspond to the expected values
    */
   CHECK_EQUAL_TEXT(exp_num_pos_points, num_pos_points, "Number of grid points is incorrect.")
   for (uint32_t i = 0U; i < num_pos_points; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_pos_grid[i], pos_grid[i], test_threshold, "Position grid point is incorrect.")
   }
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Compute_Detection_Score
 *  @{
 */

/** \brief
 * Testing of a function that computes the score of detections based on their positions in given object's TCS the object's extension.
 */

TEST_GROUP(f360_pseudo_position_estimation_Compute_Detection_Score)
{
   // Common variables used in tests
   float32_t det_coordinate_tcs;
   F360_Object_Sides_T visible_side;
   float32_t object_dimension_size;

   float32_t detection_score;
   float32_t exp_det_score;
   const float32_t test_threshold = 0.0001F;
   /** \setup
    * A detection coordinate in (modified) TCS is set to 0.5 
    *    - this can be either the para or orth coordinate, depending on the context. Here it doesn't matter.
    * Object visible side is set to INVALID, i.e. the side that gives the best information about the edge position is not visible.
    * Object dimension is set to 2
    *    - this can be either the para or orth coordinate, depending on the context. Here it doesn't matter.
    */
   TEST_SETUP()
   {      
      det_coordinate_tcs = 0.5F;
      visible_side = F360_OBJECT_SIDES_INVALID;
      object_dimension_size = 2.0F;
   }

};

/** \purpose
 * Test that the detection gets the maximum score when visible side is invalid and detection is inside the bounding box
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Invalid_Det_Inside)
{
   /** \precond
    * A detection coordinate, visible side and object dimension have been set up in TEST_GROUP such that
    * the detection is inside the object bounding box
    * Expected detection score is the maximum, i.e 1 since it's inside the bounding box.
    */
   exp_det_score = 1.0F;

   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that the correct detection score is calculated when visible side is invalid and detection is outside the bounding box
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Invalid_Det_Outside)
{
   /** \precond
    * A detection coordinate, visible side and object dimension have been set up in TEST_GROUP
    * Set detection position to be outside the the bounding box
    * Expected detection score 0.9216, below 1 since it's outside the bounding box.
    */
   det_coordinate_tcs = object_dimension_size * 0.5F + 1.0F;
   exp_det_score = 0.9216F;

   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that the score is zero when visible side is invalid and detection is far outside the bounding box
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Invalid_Det_Far_Outside)
{
   /** \precond
    * A detection coordinate, visible side and object dimension have been set up in TEST_GROUP
    * Set detection position to be far outside the the bounding box
    * Expected detection score 0.
    */
   det_coordinate_tcs = 6.0F;
   exp_det_score = 0.0F;

   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that the correct score is computed when the visible side is left and the detection is close to the edge, inside the box.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Inside_Closte_To_Edge)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place detection inside the bounding box, close to the edge
    */
   visible_side = F360_OBJECT_SIDES_LEFT;
   det_coordinate_tcs = 0.1F;
   exp_det_score = 0.992249F;

   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that the correct score is computed when the visible side is left and the detection is close to the edge,
 * inside the box and the bisquare is not saturated.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Inside_Closte_To_Edge_Bisquare_Not_Saturated)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place detection inside the bounding box, close to the edge
    * Set object dimension to something small, such that bisquare needs to be saturated.
    */
   object_dimension_size = 0.15F;
   visible_side = F360_OBJECT_SIDES_LEFT;
   det_coordinate_tcs = 0.1F;
   exp_det_score = 0.827160478F;

   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that for two detections inside the bounding box, the score is lower for the one farther from the edge.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Inside_Far_From_Edge)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place detection inside the bounding box, close to the edge
    */
   visible_side = F360_OBJECT_SIDES_LEFT;
   const float32_t det_coordinate_tcs_close = 0.1F;
   const float32_t det_coordinate_tcs_not_close = 1.9F;

   /** \action
    * Call Compute_Detection_Score
    */
   const float32_t detection_score_close_to_edge = Compute_Detection_Score(det_coordinate_tcs_close, visible_side, object_dimension_size);
   const float32_t detection_score_far_from_edge = Compute_Detection_Score(det_coordinate_tcs_not_close, visible_side, object_dimension_size);
   
   /** \result
    * Check that score for the detection farther from the edge is smaller than that of the closer detection.
    * Check that the score is not below k_min_score_inside_bbox (0.75)
    */
   CHECK_TRUE_TEXT(detection_score_far_from_edge < detection_score_close_to_edge, "Close detection score is smaller.")
   CHECK_TRUE_TEXT(detection_score_far_from_edge >= 0.75F, "Detection score for a detection inside the box is too small.");
}

/** \purpose
 * Test that the correct score is computed when the visible side is left and the detection is outisde the box, on the side between
 * the object and host (i.e. not ouside the far edge).
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Outside_Close_Edge)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place detection outside the box, between host and object
    */
   visible_side = F360_OBJECT_SIDES_LEFT;
   det_coordinate_tcs = -0.05F;
   exp_det_score = 0.5625F;
   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that detection score is higher outside the non-visible side than outside the visible side.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Compare_Outside_Obj_Different_Sides)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place one detection outside the box, between host and object
    * Place another detection outisde the box on the far side, with same distance from that edge as the first detection
    */
   visible_side = F360_OBJECT_SIDES_LEFT;
   const float32_t dist_from_edge = 0.05F;
   const float32_t det_coordinate_tcs_vis_side = -dist_from_edge;
   const float32_t det_coordinate_tcs_non_vis_side = object_dimension_size + dist_from_edge;

   /** \action
    * Call Compute_Detection_Score
    */
   const float32_t detection_score_vis_side = Compute_Detection_Score(det_coordinate_tcs_vis_side, visible_side, object_dimension_size);
   const float32_t detection_score_non_vis_side = Compute_Detection_Score(det_coordinate_tcs_non_vis_side, visible_side, object_dimension_size);
   
   /** \result
    * Check that the detection score outside the non-visible edge is higher than that of the detection outside the visible edge
    */
   CHECK_TRUE_TEXT(detection_score_non_vis_side > detection_score_vis_side, "Detection score incorrectly higher outisde non-visible edge.");
}

/** \purpose
 * Test that a detection that is outside the visible edge with enough margin get a score of zero.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Outside_Close_Edge_With_Margin)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place detection outside the box, between host and object far enough from the edge for 0 score
    * Expected score is 0.
    */
   visible_side = F360_OBJECT_SIDES_LEFT;
   det_coordinate_tcs = -0.5F;
   exp_det_score = 0.0F;
   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is 0.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that a detection that is outside the non-visible edge with enough margin get a score of zero when visible side is left.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Left_Outside_Far_Edge_With_Margin)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to LEFT
    * Place detection outside the box, between host and object far enough from the edge for 0 score
    * Expected score is 0.
    */
   visible_side = F360_OBJECT_SIDES_LEFT;
   det_coordinate_tcs = object_dimension_size  + 1.1F;
   exp_det_score = 0.0F;
   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is 0.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that the correct detection score is calculated when visible side is rear and the detection is outside the far edge (front) with a small margin.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Rear_Outside_Far_Edge)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to REAR
    * Place detection outside the box, between host and object far enough from the edge for 0 score
    * Expected score is 0.
    */
   visible_side = F360_OBJECT_SIDES_REAR;
   det_coordinate_tcs = object_dimension_size  + 0.5F;
   exp_det_score = 0.735074997F;
   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** \purpose
 * Test that a detection that is outside the non-visible edge with enough margin get a score of zero when visible side is rear.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Detection_Score, Compute_Detection_Score_Vis_Side_Rear_Outside_Far_Edge_With_Margin)
{
   /** \precond
    * Object dimension has been set up in TEST_GROUP
    * Set visible side to REAR
    * Place detection outside the box, between host and object far enough from the edge for 0 score
    * Expected score is 0.
    */
   visible_side = F360_OBJECT_SIDES_REAR;
   det_coordinate_tcs = object_dimension_size  + 5.1F;
   exp_det_score = 0.0F;
   /** \action
    * Call Compute_Detection_Score
    */
   detection_score = Compute_Detection_Score(det_coordinate_tcs, visible_side, object_dimension_size);
   
   /** \result
    * Check that the computed detection score is 0.
    */
   DOUBLES_EQUAL_TEXT(exp_det_score, detection_score, test_threshold, "Detection score is incorrect.");
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Compute_Bisquare_Score
 *  @{
 */

/** \brief
 * Testing of a specific scoring function - The Bisquare Score
 */

TEST_GROUP(f360_pseudo_position_estimation_Compute_Bisquare_Score)
{
   // Common variables used in tests
   float32_t input;
   float32_t bisquare;

   float32_t score;
   float32_t exp_score;
   float32_t test_threshold = 0.0001F;
   /** \setup
    * Set up a bisquare parameter and an input for the bisquare function that's smaller than the bisquare.
    */
   TEST_SETUP()
   {      
      input = 0.5F;
      bisquare = 0.8F;
   }
};

/** \purpose
 * Test that the bisquare scoring function returns the correct value when the absolute value of the input is smaller than the bisquare.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Bisquare_Score, Compute_Bisquare_Score_Input_Below_Bisqaure)
{
   /** \precond
    * Using the default test setup from TEST_GROUP.
    */
   exp_score = 0.371338F;

   /** \action
    * Call Compute_Bisquare_Score
    */
   score = Compute_Bisquare_Score(input, bisquare);
   
   /** \result
    * Check that the computed detection score is 0.
    */
   DOUBLES_EQUAL_TEXT(exp_score, score, test_threshold, "Computed score doesn't match the expected value.");
}

/** \purpose
 * Test that the bisquare scoring function returns 0 when the absolute of the input is larger than the bisquare.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Bisquare_Score, Compute_Bisquare_Score_Input_Above_Bisqaure)
{
   /** \precond
    * Set input value above bisquare parameter.
    * Expected output is 0.
    */
   input = bisquare + F360_EPSILON;
   exp_score = 0.0F;
   
   /** \action
    * Call Compute_Bisquare_Score
    */
   score = Compute_Bisquare_Score(input, bisquare);
   
   /** \result
    * Check that the computed detection score is 0.
    */
   DOUBLES_EQUAL_TEXT(exp_score, score, test_threshold, "Computed score doesn't match the expected value.");
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Compute_Detection_Score
 *  @{
 */

/** \brief
 * Testing of a function that computes the score of detections based on their positions in given object's TCS the object's extension.
 */

TEST_GROUP(f360_pseudo_position_estimation_Iterate_Over_Grid)
{
   // Common variables used in tests
   float32_t pos_grid[MAX_DETS_IN_OBJ_TRK];
   uint32_t num_grid_points;
   float32_t assoc_dets_pos_tcs[MAX_DETS_IN_OBJ_TRK];
   uint32_t ndets;
   F360_Object_Sides_T obj_visible_side;
   float32_t obj_dimension;

   float32_t pseudo_pos_coordinate_tcs;
   float32_t exp_pseudo_pos_coordinate_tcs;
   float32_t test_threshold = 0.0001F;

   float32_t best_score;
   
   /** \setup
    * Assume an object with reference point rear right at (20, -5) and the orth pos is being estimated, i.e.
    * - obj_visible_side = RIGHT
    * - obj_dimension = object width = 2m
    * Set up 3 detections with
    * - two inside the objects bounding box, close to the edge
    * - and one outisde the box, between host and the object
    * Set up a simple grid with 2 positions only:
    * - One point that is aligned with the actual object position
    * - One point such that a hypothetical object would be aligned with the right-most detection
    */
   TEST_SETUP()
   {      
      ndets = 3U;
      assoc_dets_pos_tcs[0U] = 0.9F;
      assoc_dets_pos_tcs[1U] = 0.95F;
      assoc_dets_pos_tcs[2U] = 1.5F;

      num_grid_points = 2U;
      pos_grid[0U] = 0.0F;
      pos_grid[1U] = 1.0F;

      obj_visible_side = F360_OBJECT_SIDES_RIGHT;
      obj_dimension = 2.0F;

      best_score = -1e7F;
   }
};

/** \purpose
 * Test that the correct grid point is chosen when two grid points have the same score and object side is invalid. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Invalid_Same_Score)
{
   /** \precond
    * Set the two grid points to be equal, to generate same socre
    * Set object side to INVALID
    * Set expected pseudo pos to the same position as the grid point
    */
   pos_grid[0U] = pos_grid[1U];
   obj_visible_side = F360_OBJECT_SIDES_INVALID;
   exp_pseudo_pos_coordinate_tcs = pos_grid[1U];

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the correct grid point is chosen when two points have the same score and object side is invalid and there's one detection
 * in the middle with positive x position.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Invalid_Same_Score_One_Det)
{
   /** \precond
    * Set number of detecitons to one and place it at 0.5 m.
    * Set object side to INVALID
    * Set expected pseudo pos to the average of the grid points
    */
   ndets = 1U;
   assoc_dets_pos_tcs[0U] = 0.5F;
   obj_visible_side = F360_OBJECT_SIDES_INVALID;
   exp_pseudo_pos_coordinate_tcs = 0.5F * (pos_grid[0U] + pos_grid[1U]);

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the correct grid point is chosen when two points have the same score and object side is invalid and there's one detection
 * in the middle with negative x position.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Invalid_Same_Score_One_Det_2)
{
   /** \precond
    * Set number of detecitons to one and place it at -0.5 m.
    * Set object side to INVALID
    * Set expected pseudo pos to the average of the grid points
    */
   ndets = 1U;
   assoc_dets_pos_tcs[0U] = -0.5F;
   obj_visible_side = F360_OBJECT_SIDES_INVALID;
   pos_grid[0U] = 0.0F;
   pos_grid[1U] = -1.0F;
   exp_pseudo_pos_coordinate_tcs = 0.5F * (pos_grid[0U] + pos_grid[1U]);

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the outlier detection determines the pseudo position when there are only 3 detections and 2 grid points. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Right_3_Dets_1_Outlier)
{
   /** \precond
    * An orth pos grid, 3 detections and object properties have been set up in TEST_GROUP
    * Since the right-most grid point is aligned with the outlier detection (at tcs y 1.5) and the other two detection are inside the
    * bounding box, there's not enough support to reject the outlier. Thus expected orth pseudo pos is 1.
    */
   exp_pseudo_pos_coordinate_tcs = pos_grid[1U];

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the outlier detection's impact on the pseudo position is limited when there are enough detections (5 in this case) to suspect there's an actual edge. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Right_4_Dets_1_Outlier)
{
   /** \precond
    * An orth pos grid, 3 detections and object properties have been set up in TEST_GROUP
    * Two extra detection are added close to the initial two close to the object edge.
    * Expected pseudo pos is now 0, i.e. the grid point that matches the actual object and the majority of detections.
    */
   ndets = 5U;
   assoc_dets_pos_tcs[3U] = 0.93F;
   assoc_dets_pos_tcs[4U] = 0.99F;
   exp_pseudo_pos_coordinate_tcs = pos_grid[0U];

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the computed pseudo pos is between the two extreme default grid points (0 and 1) when more grid points are added between.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Right_3_Dets_1_Outlier_More_Grid_Points)
{
   /** \precond
    * An orth pos grid, 3 detections and object properties have been set up in TEST_GROUP
    * 3 extra grid points are added between 0 and 1.
    * Expected output is 0.5.
    */
   num_grid_points = 5U;
   pos_grid[2U] = 0.25F;
   pos_grid[3U] = 0.5F;
   pos_grid[4U] = 0.75F;
   exp_pseudo_pos_coordinate_tcs = pos_grid[3U];
   

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the outlier detection determines the pseudo position when there are only 3 detections and 2 grid points. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Right_Similar_Score)
{
   /** \precond
    * An orth pos grid, 3 detections and object properties have been set up in TEST_GROUP
    * Since the right-most grid point is aligned with the outlier detection (at tcs y 1.5) and the other two detection are inside the
    * bounding box, there's not enough support to reject the outlier. Thus expected orth pseudo pos is 1.
    */
   pos_grid[1U] = 0.0000000001F;
   exp_pseudo_pos_coordinate_tcs = pos_grid[1U];

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the outlier detection determines the pseudo position when there are only 3 detections and 2 grid points and visible side is left. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Iterate_Over_Grid, Iterate_Over_Orth_Grid_Vis_Side_Left_3_Dets_1_Outlier)
{
   /** \precond
    * An orth pos grid, 3 detections and object properties have been set up in TEST_GROUP
    * Set object visible side to LEFT
    * Mirror detection coordinates and grid points
    * Since the right-most grid point is aligned with the outlier detection (at tcs y 1.5) and the other two detection are inside the
    * bounding box, there's not enough support to reject the outlier. Thus expected orth pseudo pos is -1.
    */
   obj_visible_side = F360_OBJECT_SIDES_LEFT;
   pos_grid[0U] = - pos_grid[0U];
   pos_grid[1U] = - pos_grid[1U];
   assoc_dets_pos_tcs[0U] = - assoc_dets_pos_tcs[0U];
   assoc_dets_pos_tcs[1U] = - assoc_dets_pos_tcs[1U];
   assoc_dets_pos_tcs[2U] = - assoc_dets_pos_tcs[2U];
   exp_pseudo_pos_coordinate_tcs = pos_grid[1U];

   /** \action
    * Call Iterate_Over_Grid
    */
   pseudo_pos_coordinate_tcs = Iterate_Over_Grid(
      pos_grid,
      num_grid_points,
      assoc_dets_pos_tcs,
      ndets,
      obj_visible_side,
      obj_dimension,
      best_score);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_coordinate_tcs, pseudo_pos_coordinate_tcs, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Weights
 *  @{
 */

/** \brief
 * Testing of a function that calculates weights for a pseudo position measurements based on its distances to the object's reference point.
 */

TEST_GROUP(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Weights)
{
   // Common variables used in tests
   Point pseudo_pos_tcs;
   Point ref_point_tcs;
   float32_t weights[2];

   float32_t exp_weights[2];

   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set an object reference point in TCS at (-2, 1).
    * Set up a pseudo position in TCS at (-2.01, 1.005) 
    */
   TEST_SETUP()
   {
      pseudo_pos_tcs.x = -2.0F;
      pseudo_pos_tcs.y = 1.0F;

      ref_point_tcs.x = -2.01F;
      ref_point_tcs.y = 1.005F;
   }
};

/** \purpose
 * Test that when the estimated pseudo position is very close to the reference point the calculated weights are maximized, i.e. 1. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Weights, Calculate_Pseudo_Pos_Weights_Pseudo_Pos_Very_Close_To_Ref_Point)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * Expected weights are [1,1] since the distance betwen pseudo point and reference point is below distance for maximum weights.
    */
   exp_weights[0U] = 1.0F;
   exp_weights[1U] = 1.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Weights
    */
   Calculate_Pseudo_Pos_Weights(pseudo_pos_tcs, ref_point_tcs, weights);
      
   /** \result
    * Check that the computed weights are correct.
    */
   DOUBLES_EQUAL_TEXT(exp_weights[0U], weights[0U], test_threshold, "Incorrect weight for para pos returned.")
   DOUBLES_EQUAL_TEXT(exp_weights[1U], weights[1U], test_threshold, "Incorrect weight for orth pos returned.")
}

/** \purpose
 * Test that when the estimated pseudo position is close to the reference point but outside range for maximum weight, the calculated weights computed correctly. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Weights, Calculate_Pseudo_Pos_Weights_Pseudo_Pos_Close_To_Ref_Point)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * Slightly change the position of the pseudo position away from the reference point.
    * Expected weights are [0.9801986, 0.9750052]
    */
   pseudo_pos_tcs.x = pseudo_pos_tcs.x - 0.2F;
   pseudo_pos_tcs.y = pseudo_pos_tcs.y + 0.2F;
   exp_weights[0U] = 0.9801986F;
   exp_weights[1U] = 0.9750052F;

   /** \action
    * Call Calculate_Pseudo_Pos_Weights
    */
   Calculate_Pseudo_Pos_Weights(pseudo_pos_tcs, ref_point_tcs, weights);
      
   /** \result
    * Check that the computed weights are correct.
    */
   DOUBLES_EQUAL_TEXT(exp_weights[0U], weights[0U], test_threshold, "Incorrect weight for para pos returned.")
   DOUBLES_EQUAL_TEXT(exp_weights[1U], weights[1U], test_threshold, "Incorrect weight for orth pos returned.")
}

/** \purpose
 * Test that when the estimated pseudo position is far from the reference point, the calculated weight is non-zero. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Weights, Calculate_Pseudo_Pos_Weights_Pseudo_Pos_Far_From_Ref_Point)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * Change the position of the pseudo measurement to be far away enough from reference point such that the weight would be 0 without saturation.
    * Expectecation is that the output weights will be saturated to non-zero values.
    */
   pseudo_pos_tcs.x = pseudo_pos_tcs.x - 12.0F;
   pseudo_pos_tcs.y = pseudo_pos_tcs.y + 15.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Weights
    */
   Calculate_Pseudo_Pos_Weights(pseudo_pos_tcs, ref_point_tcs, weights);
      
   /** \result
    * Check that the computed weights are correct.
    */
   CHECK_TRUE_TEXT(weights[0U] > 0.0F, "Weight for para pos not saturated correctly.")
   CHECK_TRUE_TEXT(weights[1U] > 0.0F, "Weight for orth pos not saturated correctly.")
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Compute_Combined_Weighted_Pseudo_Position
 *  @{
 */

/** \brief
 * Testing of a function that calculates weights for a pseudo position measurements based on its distances to the object's reference point.
 */

TEST_GROUP(f360_pseudo_position_estimation_Compute_Combined_Weighted_Pseudo_Position)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   Point pseudo_pos_tcs_min_max;
   Point pseudo_pos_tcs_grid_search;
   Point combined_pseudo_pos;
   
   Point exp_combined_pseudo_pos;

   float32_t test_threshold = 0.0001F;

   /** \setup
    * Place object with rear reference point at (10,0) and dimensions length, width = [4, 2]
    * Place the two pseudopositions on opposite sides of reference point on equal distance
    */
   TEST_SETUP()
   {
      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 0.0F;
      obj.reference_point = F360_REFERENCE_POINT_REAR;
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);

      const float32_t orth_pos_offset = 0.3F;
      pseudo_pos_tcs_min_max.x = obj.vcs_position.x - obj.bbox.Get_Length() * 0.5F;
      pseudo_pos_tcs_min_max.y = obj.vcs_position.y + orth_pos_offset;

      pseudo_pos_tcs_grid_search.x = obj.vcs_position.x - obj.bbox.Get_Length() * 0.5F;
      pseudo_pos_tcs_grid_search.y = obj.vcs_position.y - orth_pos_offset;
   }
};

/** \purpose
 * Test that when two pseudo positions are on euqal distance from the reference point, the combined positon is in the middle. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Combined_Weighted_Pseudo_Position, Compute_Combined_Weighted_Pseudo_Position_Same_Distance)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * Expected combined position is betweeen the two pseudo positions.
    */
   exp_combined_pseudo_pos.x = (pseudo_pos_tcs_grid_search.x + pseudo_pos_tcs_min_max.x) * 0.5F;
   exp_combined_pseudo_pos.y = (pseudo_pos_tcs_grid_search.y + pseudo_pos_tcs_min_max.y) * 0.5F;

   /** \action
    * Call Compute_Combined_Weighted_Pseudo_Position
    */
   Compute_Combined_Weighted_Pseudo_Position(obj, pseudo_pos_tcs_min_max, pseudo_pos_tcs_grid_search, combined_pseudo_pos);
      
   /** \result
    * Check that the computed weights are correct.
    */
   DOUBLES_EQUAL_TEXT(exp_combined_pseudo_pos.x, combined_pseudo_pos.x, test_threshold, "Incorrect pseudo para pos returned.")
   DOUBLES_EQUAL_TEXT(exp_combined_pseudo_pos.y, combined_pseudo_pos.y, test_threshold, "Incorrect pseudo orth pos returned.")
}

/** \purpose
 * Test that when one pseudo position is much farther away than the other, it has almost zero impact on the combined pseudo position. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Combined_Weighted_Pseudo_Position, Compute_Combined_Weighted_Pseudo_Position_One_Far_Away)
{
   /** \precond
    * Move one pseudo position far away from the reference point.
    * The expected output orth position is very close the other pseudo position.
    */
   pseudo_pos_tcs_min_max.y = 15.0F;
   exp_combined_pseudo_pos.x = (pseudo_pos_tcs_grid_search.x + pseudo_pos_tcs_min_max.x) * 0.5F;
   exp_combined_pseudo_pos.y = pseudo_pos_tcs_grid_search.y;

   /** \action
    * Call Compute_Combined_Weighted_Pseudo_Position
    */
   Compute_Combined_Weighted_Pseudo_Position(obj, pseudo_pos_tcs_min_max, pseudo_pos_tcs_grid_search, combined_pseudo_pos);
      
   /** \result
    * Check that the computed weights are correct.
    */
   DOUBLES_EQUAL_TEXT(exp_combined_pseudo_pos.x, combined_pseudo_pos.x, test_threshold, "Incorrect pseudo para pos returned.")
   DOUBLES_EQUAL_TEXT(exp_combined_pseudo_pos.y, combined_pseudo_pos.y, test_threshold, "Incorrect pseudo orth pos returned.")
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Get_Vectors_And_Distances_To_Obj_Corners
 *  @{
 */

/** \brief
 * Testing of a function that computes vectors (and vector lengths) from host center to an object's corners.
 */

TEST_GROUP(f360_pseudo_position_estimation_Get_Vectors_And_Distances_To_Obj_Corners)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Host_T host = {};
   
   float32_t vec_from_host_center_to_obj_corners[4][2];
   float32_t dist_from_host_center_to_obj_corners[4];
   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up an object with
    * - reference point rear left at [6, 4] m
    * - orientation 0 degrees
    * - [length, width] = [4, 2] m
    * Set host distance to rear axle to 3.33, such that host center is [-2, 0]
    */
   TEST_SETUP()
   {      
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      obj.vcs_position.x = 6.0F;
      obj.vcs_position.y = 4.0F;
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.Update_Bbox_Center();

      host.dist_rear_axle_to_vcs_m = 3.333333333F;
   }
};

/** \purpose
 * Test that the vectors and their lengths are correctly calculated.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Get_Vectors_And_Distances_To_Obj_Corners, Get_Vectors_And_Distances_To_Obj_Corners)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    */
   float32_t exp_vec_from_host_center_to_obj_corners[4][2];
   exp_vec_from_host_center_to_obj_corners[3][0] = 8.0F;
   exp_vec_from_host_center_to_obj_corners[3][1] = 4.0F;
   exp_vec_from_host_center_to_obj_corners[2][0] = 8.0F;
   exp_vec_from_host_center_to_obj_corners[2][1] = 6.0F;
   exp_vec_from_host_center_to_obj_corners[0][0] = 12.0F;
   exp_vec_from_host_center_to_obj_corners[0][1] = 4.0F;
   exp_vec_from_host_center_to_obj_corners[1][0] = 12.0F;
   exp_vec_from_host_center_to_obj_corners[1][1] = 6.0F;

   float32_t exp_dist_from_host_center_to_obj_corners[4];
   exp_dist_from_host_center_to_obj_corners[3] = std::sqrt(80.0F);
   exp_dist_from_host_center_to_obj_corners[2] = std::sqrt(100.0F);
   exp_dist_from_host_center_to_obj_corners[0] = std::sqrt(160.0F);
   exp_dist_from_host_center_to_obj_corners[1] = std::sqrt(180.0F);
   
   /** \action
    * Call Get_Vectors_And_Distances_To_Obj_Corners
    */
   Get_Vectors_And_Distances_To_Obj_Corners(host, obj, vec_from_host_center_to_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the calculated vectors and their lengths are correct.
    */
   
   for (uint32_t i = 0U; i < 4U; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_dist_from_host_center_to_obj_corners[i], dist_from_host_center_to_obj_corners[i], test_threshold, "Incorrect distance to object corner.");
      for (uint32_t j = 0U; j < 2U; j++)
      {
         DOUBLES_EQUAL_TEXT(exp_vec_from_host_center_to_obj_corners[i][j], vec_from_host_center_to_obj_corners[i][j], test_threshold, "Incorrect element of vector to object corner.");
      } 
   }
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid
 *  @{
 */

/** \brief
 * Testing of a function that computes the cos of the maximum azimuth spread of an object to deremine if it should be considered a point target or not.
 */

TEST_GROUP(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid)
{
   // Common variables used in tests
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T obj = {};
   float32_t dist_from_host_center_to_obj_corners[4];
   float32_t vec_obj_corners[4][2];

   /** \setup
    * Set up vectors to object corner for an object assumed with
    * - reference point rear at [10, 4] m
    * - orientation 20 degrees
    * - [length, width] = [4, 2] m
    * - and host center in [-2, 0] m
    */
   TEST_SETUP()
   {
      vec_obj_corners[0][0] = 13.41675F;
      vec_obj_corners[0][1] = 6.30777264F; // FL
      vec_obj_corners[1][0] = 14.10079F;
      vec_obj_corners[1][1] = 4.42838764F; // FR
      vec_obj_corners[2][0] = 10.34202F;
      vec_obj_corners[2][1] = 3.06030726F; // RR
      vec_obj_corners[3][0] = 9.65797997F;
      vec_obj_corners[3][1] = 4.9396925F; // RL

      for (uint32_t i = 0U; i < 4; i++)
      {
         dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
      }
      
   }

};

/** \purpose
 * Test that a nearby object is correclty considered as an extended target.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_Obj_Close)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    */
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Point assumption not valid");
}

/** \purpose
 * Test that a far-away object is correclty considered as a point target.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_Obj_Far_Away)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Shift all corner points 65m longitudinally such that maximum azimuth spread is small enough for the point target assumption 
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      vec_obj_corners[i][0] += 65.0F;
   }

   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
   }
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_TRUE_TEXT(f_result, "Point assumption is valid");
}

/** \purpose
 * Test that a close object with all its distance to host center set to 0 is not considered as a point target.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_Obj_Close_Zero_Div_Protection)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = 0.0F;
   }
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Point assumption should not be valid");
}

/** \purpose
 * Test that if distance between host center and object front left corner is below 0 m function returns false.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Invalid_Negative_Dist_Front_Left)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set distance to front left corner to -1 m.
    */
   dist_from_host_center_to_obj_corners[0] = 1.0F;
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Point assumption not valid");
}

/** \purpose
 * Test that if distance between host center and object front right corner is below 0 m function returns false.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Invalid_Negative_Dist_Front_Right)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set distance to front right corner to -1 m.
    */
   dist_from_host_center_to_obj_corners[1] = 1.0F;
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Point assumption not valid");
}

/** \purpose
 * Test that if distance between host center and object rear right corner is below 0 m function returns false.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Invalid_Negative_Dist_Rear_Right)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set distance to rear right corner to -1 m.
    */
   dist_from_host_center_to_obj_corners[2] = 1.0F;
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Point assumption not valid");
}

/** \purpose
 * Test that if distance between host center and object rear left corner is below 0 m function returns false.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Invalid_Negative_Dist_Rear_Left)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set distance to rear left corner to -1 m.
    */
   dist_from_host_center_to_obj_corners[3] = 1.0F;
   
   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Point assumption not valid");
}

/** \purpose
 * Test that the object which otherwise would be considered a point-target & has a significant detection spread will be considered as extended target.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Invalid_High_Detection_Spread)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set vec_obj_corners in a way which would make the target be considered point-based
    * Set object 80m + away from the host.
    * Associate at least 5 detections with significant position spread
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      vec_obj_corners[i][0] += 100.0F;
   }

   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
   }

   obj.vcs_position.x = 100.0F;
   obj.vcs_position.y = 0.0F;
   obj.ndets = 6U;

   for(uint8_t i = 0U; i < obj.ndets; i++)
   {
      obj.detids[i] = i + 1U;
      det_props[i].vcs_position.x = obj.vcs_position.x + i * 3.0F;
      det_props[i].vcs_position.y = obj.vcs_position.y;
   }

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Object is not considered as extended taret");
}

/** \purpose
 * Test that the object will be considered a point-target when it does not have a significant detection spread.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_Low_Detection_Spread)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set vec_obj_corners in a way which would make the target be considered point-based
    * Set object 80m + away from the host.
    * Associate at least 5 detections with low position spread
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      vec_obj_corners[i][0] += 100.0F;
   }

   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
   }

   obj.vcs_position.x = 100.0F;
   obj.vcs_position.y = 0.0F;
   obj.ndets = 6U;

   for(uint8_t i = 0U; i < obj.ndets; i++)
   {
      obj.detids[i] = i + 1U;
      det_props[i].vcs_position.x = obj.vcs_position.x + i * 1.0F;
      det_props[i].vcs_position.y = obj.vcs_position.y;
   }

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is valid.
    */
   
   CHECK_TRUE_TEXT(f_result, "Object is considered as extended target");
}

/** \purpose
 * Test that the object will be considered a point-target when it does not have any detections.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_No_Detections)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set vec_obj_corners in a way which would make the target be considered point-based
    * Set object 80m + away from the host.
    * Dont associate any detections
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      vec_obj_corners[i][0] += 100.0F;
   }

   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
   }

   obj.vcs_position.x = 100.0F;
   obj.vcs_position.y = 0.0F;

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is valid.
    */
   
   CHECK_TRUE_TEXT(f_result, "Object is considered as extended target");
}

/** \purpose
 * Test that the object will not be considered a point-target when it does not have any detections but is close enough to be considered extended target.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_No_Detections_Target_Is_Close)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set vec_obj_corners in a way which would not make the target be considered point-based
    * Set object closer than 80m away from the host.
    * Dont associate any detections
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      vec_obj_corners[i][0] += 40.0F;
   }

   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
   }

   obj.vcs_position.x = 40.0F;
   obj.vcs_position.y = 0.0F;

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Object is considered as point target");
}

/** \purpose
 * Test that the object will not be considered a point-target when its close and it does have detections.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Is_Point_Object_Assumption_Valid_Detections_Available_Target_Is_Close)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set vec_obj_corners in a way which would not make the target be considered point-based
    * Set object closer than 80m away from the host.
    * Associate at least 6 detections
    */
   for (uint32_t i = 0U; i < 4; i++)
   {
      vec_obj_corners[i][0] += 40.0F;
   }

   for (uint32_t i = 0U; i < 4; i++)
   {
      dist_from_host_center_to_obj_corners[i] = std::sqrt(vec_obj_corners[i][0]*vec_obj_corners[i][0] + vec_obj_corners[i][1]*vec_obj_corners[i][1]);
   }

   obj.vcs_position.x = 40.0F;
   obj.vcs_position.y = 0.0F;
   obj.ndets = 6U;

   for(uint8_t i = 0U; i < obj.ndets; i++)
   {
      obj.detids[i] = i + 1U;
      det_props[i].vcs_position.x = obj.vcs_position.x + i * 1.0F;
      det_props[i].vcs_position.y = obj.vcs_position.y;
   }

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const bool f_result = Is_Point_Object_Assumption_Valid(det_props, obj, vec_obj_corners, dist_from_host_center_to_obj_corners);

   /** \result
    * Check that the point target assumption is not valid.
    */
   
   CHECK_FALSE_TEXT(f_result, "Object is considered as point target");
}

/** \purpose
 * Test that there is an appropriate division by zero protection when calling Calculate_Distance_Std_Dev_Of_Associated_Dets with no detections associated.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Calc_Std_Dev_Call_No_Dets)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Do not associate any detections to an object
    */
   obj.ndets = 0U;

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const float32_t distance_deviation = Calculate_Distance_Std_Dev_Of_Associated_Dets(det_props, obj);

   /** \result
    * Check that the function properly returns -1.0F without dets
    */
   
   CHECK_EQUAL_TEXT(-1.0F, distance_deviation, "Function returned wrong distance deviation");
}

/** \purpose
 * Test that the Calculate_Distance_Std_Dev_Of_Associated_Dets return correct value when detections are available.  
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Is_Point_Object_Assumption_Valid, Calc_Std_Dev_Call_Dets)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Associate at least 6 detections to an object with arbitary positions
    */
   obj.ndets = 6U;

   for(uint8_t i = 0U; i < obj.ndets; i++)
   {
      obj.detids[i] = i + 1U;
      det_props[i].vcs_position.x = obj.vcs_position.x + i * 1.0F;
      det_props[i].vcs_position.y = obj.vcs_position.y;
   }

   /** \action
    * Call Is_Point_Object_Assumption_Valid
    */
   const float32_t distance_deviation = Calculate_Distance_Std_Dev_Of_Associated_Dets(det_props, obj);

   /** \result
    * Check that the function returns correct value
    */
   
   DOUBLES_EQUAL(1.70782518F, distance_deviation, F360_EPSILON);
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Calculate_SCS_Min_Range
 *  @{
 */

/** \brief
 * Testing of a function that computes the min range of detections in a pseudo sensor coordinate system (SCS).
 */

TEST_GROUP(f360_pseudo_position_estimation_Calculate_SCS_Min_Range)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Point host_center_vcs = {};

   const float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up an object with
    * - reference point rear at [10, 4] m
    * - orientation 20 degrees
    * - [length, width] = [4, 2] m
    * Get corners of the object in VCS
    * Set host center to [-2, 0] m
    */
   TEST_SETUP()
   {      
      obj.reference_point = F360_REFERENCE_POINT_REAR;
      obj.vcs_position.x = 100.0F;
      obj.vcs_position.y = 0.0F;
      obj.bbox.Set_Orientation(F360_DEG2RAD(0.0F));
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.Update_Bbox_Center();
      obj.ndets = 3;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 2U;
      obj.detids[2U] = 3U;

      det_props[0U].vcs_position.x = 99.8F;
      det_props[0U].vcs_position.y = -1.5F;

      det_props[1U].vcs_position.x = 102.0F;
      det_props[1U].vcs_position.y = -1.7F;
      
      det_props[2U].vcs_position.x = 101.5F;
      det_props[2U].vcs_position.y = 1.9F;

      host_center_vcs.x = -2.0F;
      host_center_vcs.y = 0.0F;
   }

};

/** \purpose
 * Test that the min detection range in SCS is calculated correctly for the case where the object is right in front of host, far away. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_SCS_Min_Range, Calculate_SCS_Min_Range_Obj_In_Front)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set expected min range to be the distance to the detection closest to host
    */
   const float32_t exp_min_range_pseudo_SCS = F360_Sqrtf((det_props[0U].vcs_position.x - host_center_vcs.x)*(det_props[0U].vcs_position.x - host_center_vcs.x)
      + (det_props[0U].vcs_position.y - host_center_vcs.y) * (det_props[0U].vcs_position.y - host_center_vcs.y));
   
   /** \action
    * Call Calculate_SCS_Min_Range
    */
   const float32_t min_range_pseudo_SCS = Calculate_SCS_Min_Range(obj, det_props, host_center_vcs);

   /** \result
    * Check that the calculated distance to the closest detection is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_min_range_pseudo_SCS, min_range_pseudo_SCS, test_threshold, "Incorrect min pseudo SCS detection range.");
}

/** \purpose
 * Test that the min detection range in SCS is calculated correctly for the case where the object is to the left and rear of host. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_SCS_Min_Range, Calculate_SCS_Min_Range_Obj_In_Rear_Left)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Move the object ot the rear left of host
    * Update detection positions to be close to the object's new position.
    * Set expected min range to be the distance to the detection closest to host
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj.vcs_position.x = - 20.0F;
   obj.vcs_position.y = - 5.0F;
   obj.Update_Bbox_Center();

   det_props[0U].vcs_position.x = -19.0F;
   det_props[0U].vcs_position.y = -6.0F;

   det_props[1U].vcs_position.x = -18.0F;
   det_props[1U].vcs_position.y = -3.0F;
   
   det_props[2U].vcs_position.x = -22.0F;
   det_props[2U].vcs_position.y = -3.0F;

   const float32_t exp_min_range_pseudo_SCS = 16.2788F;

   /** \action
    * Call Calculate_SCS_Min_Range
    */
   const float32_t min_range_pseudo_SCS = Calculate_SCS_Min_Range(obj, det_props, host_center_vcs);

   /** \result
    * Check that the calculated distance to the closest detection is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_min_range_pseudo_SCS, min_range_pseudo_SCS, test_threshold, "Incorrect min pseudo SCS detection range.");
}

/** \purpose
 * Test that the min detection range in SCS is calculated correctly for the case where the object is straight behind host. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_SCS_Min_Range, Calculate_SCS_Min_Range_Obj_In_Left)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Change object position to be straight behind host
    * Change detection positions to be close to the object's new position
    * Set expected min range to be the distance to the detection closest to host
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.vcs_position.x = - 20.0F;
   obj.vcs_position.y = 0.0F;
   obj.Update_Bbox_Center();

   det_props[0U].vcs_position.x = -20.0F;
   det_props[0U].vcs_position.y = 1.0F;

   det_props[1U].vcs_position.x = -22.0F;
   det_props[1U].vcs_position.y = 0.0F;
   
   det_props[2U].vcs_position.x = -20.0F;
   det_props[2U].vcs_position.y = -0.5F;

   const float32_t exp_min_range_pseudo_SCS = 18.0069F;
   
   /** \action
    * Call Calculate_SCS_Min_Range
    */
   const float32_t min_range_pseudo_SCS = Calculate_SCS_Min_Range(obj, det_props, host_center_vcs);

   /** \result
    * Check that the calculated distance to the closest detection is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_min_range_pseudo_SCS, min_range_pseudo_SCS, test_threshold, "Incorrect min pseudo SCS detection range.");
}

/** \purpose
 * Test that the min detection range in SCS is calculated correctly for the case where the object is in front to the right of host. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_SCS_Min_Range, Calculate_SCS_Min_Range_Obj_In_Front_Right)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Change object position to be (3,4) and have reference point REAR RIGHT
    * Change orientation of the object by 90 degrees
    * Change detection positions to be close to the object's new position
    * Set expected min range to be the distance to the detection closest to host
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.bbox.Set_Orientation(F360_DEG2RAD(90.0F));
   obj.vcs_position.x = 3.0F;
   obj.vcs_position.y = 4.0F;
   obj.Update_Bbox_Center();
   obj.ndets = 2;

   det_props[0U].vcs_position.x = 4.0F;
   det_props[0U].vcs_position.y = 4.0F;

   det_props[1U].vcs_position.x = 5.0F;
   det_props[1U].vcs_position.y = 7.0F;

   const float32_t exp_min_range_pseudo_SCS = 7.2111F;
   
   /** \action
    * Call Calculate_SCS_Min_Range
    */
   const float32_t min_range_pseudo_SCS = Calculate_SCS_Min_Range(obj, det_props, host_center_vcs);

   /** \result
    * Check that the calculated distance to the closest detection is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_min_range_pseudo_SCS, min_range_pseudo_SCS, test_threshold, "Incorrect min pseudo SCS detection range.");
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Calculate_Mean_SCS_Azimuth
 *  @{
 */

/** \brief
 * Testing of a function that computes the average azimuth of detections in a pseudo sensor coordinate system (SCS).
 */

TEST_GROUP(f360_pseudo_position_estimation_Calculate_Mean_SCS_Azimuth)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Point host_center_vcs = {};
   Angle angle_host_center_to_obj_center_vcs = {};

   const float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up an object with
    * - reference point rear at [100, 0] m
    * - orientation 20 degrees
    * - [length, width] = [4, 2] m
    * Get corners of the object in VCS
    * Set host center to [-2, 0] m
    */
   TEST_SETUP()
   {      
      obj.reference_point = F360_REFERENCE_POINT_REAR;
      obj.vcs_position.x = 100.0F;
      obj.vcs_position.y = 0.0F;
      obj.bbox.Set_Orientation(F360_DEG2RAD(0.0F));
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.Update_Bbox_Center();
      obj.ndets = 3;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 2U;
      obj.detids[2U] = 3U;

      det_props[0U].vcs_position.x = 99.8F;
      det_props[0U].vcs_position.y = -1.5F;

      det_props[1U].vcs_position.x = 102.0F;
      det_props[1U].vcs_position.y = -1.7F;
      
      det_props[2U].vcs_position.x = 101.5F;
      det_props[2U].vcs_position.y = 1.9F;

      host_center_vcs.x = -2.0F;
      host_center_vcs.y = 0.0F;

      angle_host_center_to_obj_center_vcs.Value(F360_Atan2f(obj.bbox.Get_Center().y -  host_center_vcs.y, obj.bbox.Get_Center().x - host_center_vcs.x));
   }

};

/** \purpose
 * Test that the average detection azimuth in SCS is calculated correctly for the case where the object is right in front of host, far away. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Mean_SCS_Azimuth, Calculate_Mean_SCS_Azimuth_Obj_In_Front)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Set expected average azimuth
    */
   const float32_t exp_avg_az_pseudo_SCS = -0.0042F;
   
   /** \action
    * Call Calculate_Mean_SCS_Azimuth
    */
   const Angle avg_az_pseudo_SCS = Calculate_Mean_SCS_Azimuth(obj, det_props, host_center_vcs, angle_host_center_to_obj_center_vcs);

   /** \result
    * Check that the calculated mean detection azimuth is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_avg_az_pseudo_SCS, avg_az_pseudo_SCS.Value(), test_threshold, "Average det SCS azimuth is incorrect.");
}

/** \purpose
 * Test that the average detection azimuth in SCS is calculated correctly for the case where the object is to the left and rear of host. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Mean_SCS_Azimuth, Calculate_Mean_SCS_Azimuth_Obj_In_Rear_Left)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Move the object ot the rear left of host
    * Update detection positions to be close to the object's new position.
    * Set expected average azimuth
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj.vcs_position.x = - 20.0F;
   obj.vcs_position.y = - 5.0F;
   obj.Update_Bbox_Center();
   angle_host_center_to_obj_center_vcs.Value(F360_Atan2f(obj.bbox.Get_Center().y -  host_center_vcs.y, obj.bbox.Get_Center().x - host_center_vcs.x));

   det_props[0U].vcs_position.x = -19.0F;
   det_props[0U].vcs_position.y = -6.0F;

   det_props[1U].vcs_position.x = -18.0F;
   det_props[1U].vcs_position.y = -3.0F;
   
   det_props[2U].vcs_position.x = -22.0F;
   det_props[2U].vcs_position.y = -3.0F;

   const float32_t exp_avg_az_pseudo_SCS = -0.0669F;
   
   /** \action
    * Call Calculate_Mean_SCS_Azimuth
    */
   const Angle avg_az_pseudo_SCS = Calculate_Mean_SCS_Azimuth(obj, det_props, host_center_vcs, angle_host_center_to_obj_center_vcs);

   /** \result
    * Check that the calculated mean detection azimuth is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_avg_az_pseudo_SCS, avg_az_pseudo_SCS.Value(), test_threshold, "Average det SCS azimuth is incorrect.");
}

/** \purpose
 * Test that the average detection azimuth in SCS is calculated correctly for the case where the object is straight behind host. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Mean_SCS_Azimuth, Calculate_Mean_SCS_Azimuth_Obj_In_Left)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Change object position to be straight behind host
    * Change detection positions to be close to the object's new position
    * Set expected min range to be the distance to the detection closest to host
    * Set expected average azimuth
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.vcs_position.x = - 20.0F;
   obj.vcs_position.y = 0.0F;
   obj.Update_Bbox_Center();
   angle_host_center_to_obj_center_vcs.Value(F360_Atan2f(obj.bbox.Get_Center().y -  host_center_vcs.y, obj.bbox.Get_Center().x - host_center_vcs.x));

   det_props[0U].vcs_position.x = -20.0F;
   det_props[0U].vcs_position.y = 1.0F;

   det_props[1U].vcs_position.x = -22.0F;
   det_props[1U].vcs_position.y = 0.0F;
   
   det_props[2U].vcs_position.x = -20.0F;
   det_props[2U].vcs_position.y = -0.5F;

   const float32_t exp_avg_az_pseudo_SCS = -0.0092F;
   
   /** \action
    * Call Calculate_Mean_SCS_Azimuth
    */
   const Angle avg_az_pseudo_SCS = Calculate_Mean_SCS_Azimuth(obj, det_props, host_center_vcs, angle_host_center_to_obj_center_vcs);

   /** \result
    * Check that the calculated mean detection azimuth is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_avg_az_pseudo_SCS, avg_az_pseudo_SCS.Value(), test_threshold, "Average det SCS azimuth is incorrect.");
}

/** \purpose
 * Test that the min detection range and average detection azimuth in SCS is calculated correctly for the case where the object is in front to the right of host. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Mean_SCS_Azimuth, Calculate_Mean_SCS_Azimuth_Obj_In_Front_Right)
{
   /** \precond
    * A default test has been set up in the TEST_GROUP.
    * Change object position to be (3,4) and have reference point REAR RIGHT
    * Change orientation of the object by 90 degrees
    * Change detection positions to be close to the object's new position
    * Set expected min range to be the distance to the detection closest to host
    * Set expected average azimuth
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.bbox.Set_Orientation(F360_DEG2RAD(90.0F));
   obj.vcs_position.x = 3.0F;
   obj.vcs_position.y = 4.0F;
   obj.Update_Bbox_Center();
   obj.ndets = 2;
   angle_host_center_to_obj_center_vcs.Value(F360_Atan2f(obj.bbox.Get_Center().y -  host_center_vcs.y, obj.bbox.Get_Center().x - host_center_vcs.x));

   det_props[0U].vcs_position.x = 4.0F;
   det_props[0U].vcs_position.y = 4.0F;

   det_props[1U].vcs_position.x = 5.0F;
   det_props[1U].vcs_position.y = 7.0F;

   const float32_t exp_avg_az_pseudo_SCS = -0.0987F;
   
   /** \action
    * Call Calculate_Mean_SCS_Azimuth
    */
   const Angle avg_az_pseudo_SCS = Calculate_Mean_SCS_Azimuth(obj, det_props, host_center_vcs, angle_host_center_to_obj_center_vcs);

   /** \result
    * Check that the calculated mean detection azimuth is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_avg_az_pseudo_SCS, avg_az_pseudo_SCS.Value(), test_threshold, "Average det SCS azimuth is incorrect.");
}

/** @}*/


/** \defgroup  f360_pseudo_position_estimation_Compute_Pseudo_Pos_Extended_Obj_Assumption
 *  @{
 */

/** \brief
 * Testing of a function that computes the score of detections based on their positions in given object's TCS the object's extension.
 */

TEST_GROUP(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Extended_Obj_Assumption)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   float32_t exp_det_score;
   float32_t test_threshold = 0.0001F;

   Point pseudo_pos_vcs;

   /** \setup
    * Set up an object with
    *    - Reference point REAR at [20,0]
    *    - [Length, width] = [4,2]m
    * Place some detections close to the object and one obvious outlier
    */
   TEST_SETUP()
   {  
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.vcs_position.x = 20.0F;
      obj.vcs_position.y = 4.0F;
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      obj.bbox.Set_Orientation(0.0F);
      obj.Update_Bbox_Center();

      obj.ndets = 6;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 2U;
      obj.detids[2U] = 3U;
      obj.detids[3U] = 4U;
      obj.detids[4U] = 5U;
      obj.detids[5U] = 6U;
      
      // First detection is 1m from left edge
      det_props[0U].vcs_position.x = 22.0F;
      det_props[0U].vcs_position.y = 3.0F;

      // The rest of the detections are close to the left and rear edges
      det_props[1U].vcs_position.x = 20.0F;
      det_props[1U].vcs_position.y = 4.5F;

      det_props[2U].vcs_position.x = 21.0F;
      det_props[2U].vcs_position.y = 4.0F;

      det_props[3U].vcs_position.x = 20.0F;
      det_props[3U].vcs_position.y = 6.0F;

      det_props[4U].vcs_position.x = 23.0F;
      det_props[4U].vcs_position.y = 4.0F;

      det_props[5U].vcs_position.x = 22.0F;
      det_props[5U].vcs_position.y = 4.0F;
   }

};

/** \purpose
 * Test that when there are many detections close to the object edges and one outlier, the computed pseudo pos is close to the median of the detections.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Extended_Obj_Assumption, Compute_Pseudo_Pos_Extended_Obj_Assumption_Many_Dets_One_Outlier)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * The pseudo position is expected to have
    * - longitudinal position close to the detection with the minimal longitudinal position
    * - lateral position close to the detections on the, in this case close to the median lateral position
    * I.e. in the lateral position, the grid search scheme will have a greater impact on the final pseudo position.
    */
   constexpr int32_t ndets = 6;
   float32_t dets_x[ndets] = {};
   float32_t dets_y[ndets] = {};
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      dets_x[i] = det_props[i].vcs_position.x;
      dets_y[i] = det_props[i].vcs_position.y;
   }

   uint32_t sort_idx[ndets] = {};
   (void)F360_Sort(static_cast<uint32_t>(obj.ndets), true, dets_y, sort_idx);
   const float32_t median_y = 0.5F * (dets_y[2] + dets_y[3]);

   const float32_t exp_x_pos = F360_Min_Element(dets_x);


   /** \action
    * Call Compute_Pseudo_Pos_Extended_Obj_Assumption
    */
   const Point pseudo_pos_vcs = Compute_Pseudo_Pos_Extended_Obj_Assumption(det_props, obj);

   /** \result
    * Check that the generated grid points correspond to the expected values
    */
   const float32_t y_dist = std::abs(pseudo_pos_vcs.y - median_y);
   const float32_t x_dist = std::abs(pseudo_pos_vcs.x - exp_x_pos);
   const float32_t test_threshold = 0.01F;

   CHECK_TRUE_TEXT(y_dist < test_threshold, "Pseudo y pos too far from expected value.");
   CHECK_TRUE_TEXT(x_dist < test_threshold, "Pseudo x pos too far from expected value.");
}

/** \purpose
 * Test that when there are only two detections, the min max scheme to calculate the pseudo position is used. As a result, the minimum x and y positions of detections will be selected.
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Extended_Obj_Assumption, Compute_Pseudo_Pos_Extended_Obj_Assumption_Two_Dets_One_Outlier)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP.
    * Change to 2 associated detections
    * - One close to the reference point at (20, 4.5)
    * - One lateral outlier at (22, 3)
    * The pseudo position is expected to have
    * - longitudinal position at the detection with the smallest longitudinal position
    * - lateral position at the detection with the smallest lateral position (i.e. the outlier).
    * I.e. in the lateral position, the min max scheme will have a greater impact on the final pseudo position.
    */
   obj.ndets = 2U;
   constexpr uint32_t ndets = 2U;
   float32_t dets_x[ndets] = {};
   float32_t dets_y[ndets] = {};
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      dets_x[i] = det_props[i].vcs_position.x;
      dets_y[i] = det_props[i].vcs_position.y;
   }

   const float32_t exp_y_pos = F360_Min_Element(dets_y);
   const float32_t exp_x_pos = F360_Min_Element(dets_x);


   /** \action
    * Call Compute_Pseudo_Pos_Extended_Obj_Assumption
    */
   const Point pseudo_pos_vcs = Compute_Pseudo_Pos_Extended_Obj_Assumption(det_props, obj);

   /** \result
    * Check that the generated grid points correspond to the expected values
    */
   const float32_t y_dist = std::abs(pseudo_pos_vcs.y - exp_y_pos);
   const float32_t x_dist = std::abs(pseudo_pos_vcs.x - exp_x_pos);
   const float32_t test_threshold = 0.0001F;

   CHECK_TRUE_TEXT(y_dist < test_threshold, "Pseudo y pos too far from expected value.");
   CHECK_TRUE_TEXT(x_dist < test_threshold, "Pseudo x pos too far from expected value.");
}

/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Compute_Pseudo_Pos_Object_Point_Assumption
 *  @{
 */

/** \brief
 * Testing of a function that computes the pseudo position of an object given that the object is assumed to be a point target.
 */

TEST_GROUP(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Object_Point_Assumption)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Host_T host = {};

   /** \setup
    * Setup dist_rear_axle_to_vcs_m to 0.0
    * Set up an object with properties center vcs point & ndets & detids
    * - Center point [-2, -85]
    * - 4 associated detection
    * Set detection positions such that object's pseudo position is clearly updated.
    */
   TEST_SETUP()
   {  
      // Host center -> [0, 0]
      host.dist_rear_axle_to_vcs_m = 0.0F;

      obj.vcs_position.x = -2.0F;
      obj.vcs_position.y = -85.0F;
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.bbox.Set_Orientation(0.0F);
      obj.Update_Bbox_Center();

      obj.ndets = 4;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 2U;
      obj.detids[2U] = 3U;
      obj.detids[3U] = 4U;
      
      det_props[0U].vcs_position.x = -11.0F;
      det_props[0U].vcs_position.y = -86.0F;
      
      det_props[1U].vcs_position.x = -10.0F;
      det_props[1U].vcs_position.y = -86.0F;
      
      det_props[2U].vcs_position.x = -2.0F;
      det_props[2U].vcs_position.y = -84.0F;

      det_props[3U].vcs_position.x = -10.5F;
      det_props[3U].vcs_position.y = -86.0F;
   }

};

/** \purpose
 * Test that the calculated pseudo position is as expected. A script to compute expected pseudopos manually can be found in DFD-2294. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Compute_Pseudo_Pos_Object_Point_Assumption, Compute_Pseudo_Pos_Object_Point_Assumption)
{
   /** \precond
    * A default case has been set up in the TEST GROUP.
    * Set expected position as calculated in DFD-2294
    */
   const float32_t exp_x_pos_vcs = -8.143F;
   const float32_t exp_y_pos_vcs = -83.628F;

   /** \action
    * Call Compute_Pseudo_Pos_Object_Point_Assumption
    */
   const Point pseudo_pos_vcs = Compute_Pseudo_Pos_Object_Point_Assumption(det_props, host, obj);

   /** \result
    * Check that the calculated pseudo position is within position bounds.
    */
   DOUBLES_EQUAL(pseudo_pos_vcs.x, exp_x_pos_vcs, 0.001F);
   DOUBLES_EQUAL(pseudo_pos_vcs.y, exp_y_pos_vcs, 0.001F);
}
/** @}*/


/** \defgroup  f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS
 *  @{
 */

/** \brief
 * This test group tests the functionality of Transform_Pseudo_SCS_Pos_To_VCS
 */
TEST_GROUP(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS)
{
   // Common variables used in tests
   const float32_t test_threshold = 0.0001F;

   Point host_center_vcs;

   /** \setup
    * Set host_center_vcs to x=-5, y=0
    */
   TEST_SETUP()
   {  
      host_center_vcs = Point(-5, 0);
   }
};

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is in front right of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_front_right_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=0,y=5
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to 45 deg
    */
   const Point exp_pos_vcs = Point(0.0F, 5.0F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(45.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is in front of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_front_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=sqrt(50)-5,y=0
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to 0 deg
    */
   const Point exp_pos_vcs = Point(2.0710678F, 0.0F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(0.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is in front left of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_front_left_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=0,y=-5
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to 0 deg
    */
   const Point exp_pos_vcs = Point(0.0F, -5.0F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(-45.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is to the left of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_left_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=-5,y=-sqrt(50)
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to -90 deg
    */
   const Point exp_pos_vcs = Point(-5.0F, -7.0710678F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(-90.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is to the down left of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_down_left_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=-10,y=-5
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to -135 deg
    */
   const Point exp_pos_vcs = Point(-10.0F, -5.0F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(-135.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is behind of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_behind_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=-5-sqrt(50),y=0
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to -180 deg
    */
   const Point exp_pos_vcs = Point(-12.0710678F, 0.0F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(-180.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is to the down right of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_down_right_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=-10,y=5
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to 135 deg
    */
   const Point exp_pos_vcs = Point(-10.0F, 5.0F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(135.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** \purpose
 * Test that Transform_Pseudo_SCS_Pos_To_VCS transforms a point defined in the pseudo sensor coordinate system (SCS) 
 * converts correctly to a point in the vehicle coordinate system (VCS) when the point is to the right of host
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Pseudo_SCS_Pos_To_VCS, Transform_Pseudo_SCS_Pos_To_VCS_right_of_host)
{
   /** \precond
    * Set exp_pos_vcs to x=-5,y=sqrt(50)
    * Set pos_pseudo_SCS to x=sqrt(50),y=0
    * Set pseudo_scs_rotation_in_vcs to 90 deg
    */
   const Point exp_pos_vcs = Point(-5.0F, 7.0710678F);

   const Point pos_pseudo_SCS = Point(7.0710678F, 0.0F); //sqrt(50)
   const Angle pseudo_scs_rotation_in_vcs = Angle(90.0F * 0.0174532925F);

   /** \action
    * Call Transform_Pseudo_SCS_Pos_To_VCS
    */
   const Point pos_vcs = Transform_Pseudo_SCS_Pos_To_VCS(pos_pseudo_SCS, pseudo_scs_rotation_in_vcs, host_center_vcs);

   /** \result
    * Check that the calculated pseudo position is correct.
    */
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.x, pos_vcs.x, test_threshold, "Pseudo position x coordinate in vcs is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_vcs.y, pos_vcs.y, test_threshold, "Pseudo position y coordinate in vcs is incorrect.");
}

/** @}*/


/** \defgroup  f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
 *  @{
 */

/** \brief
 * This test group tests the functionality of Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
 */
TEST_GROUP(f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension)
{
   // Common variables used in tests
   const float32_t test_threshold = 0.0001F;
   const float32_t obj_length = 10.0F;
   const float32_t obj_width = 4.0F;

   const float32_t center_point_tcs_dimension = 2.5F;
};

/** \purpose
 * Test that this function translates a position corresponding to an object's center
 * point to instead correspond to the reference point in tcs x direction when the reference point is front
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension, Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension_front)
{
   /** \precond
    * Set seen_side to front
    */
   const F360_Object_Sides_T seen_side = F360_OBJECT_SIDES_FRONT; 

   const float32_t expected_reference_point_x = 7.5F;

   /** \action
    * Call Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
    */
   const float32_t reference_point_x = Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension(seen_side, obj_length, center_point_tcs_dimension);
  
   /** \result
    * Check that the calculated reference point position is correct.
    */
   DOUBLES_EQUAL_TEXT(expected_reference_point_x, reference_point_x, test_threshold, "Reference point position x coordinate is incorrect.");
}

/** \purpose
 * Test that this function translates a position corresponding to an object's center
 * point to instead correspond to the reference point in tcs x direction when the reference point is rear
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension, Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension_rear)
{
   /** \precond
    * Set seen_side to front
    */
   const F360_Object_Sides_T seen_side = F360_OBJECT_SIDES_REAR; 

   const float32_t expected_reference_point_x = -2.5F;

   /** \action
    * Call Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
    */
   const float32_t reference_point_x = Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension(seen_side, obj_length, center_point_tcs_dimension);
  
   /** \result
    * Check that the calculated reference point position is correct.
    */
   DOUBLES_EQUAL_TEXT(expected_reference_point_x, reference_point_x, test_threshold, "Reference point position x coordinate is incorrect.");
}

/** \purpose
 * Test that this function translates a position corresponding to an object's center
 * point to instead correspond to the reference point in tcs y direction when the reference point is right
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension, Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension_right)
{
   /** \precond
    * Set seen_side to right
    */
   const F360_Object_Sides_T seen_side = F360_OBJECT_SIDES_RIGHT; 

   const float32_t expected_reference_point_y = 4.5F;

   /** \action
    * Call Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
    */
   const float32_t reference_point_y = Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension(seen_side, obj_width, center_point_tcs_dimension);
  
   /** \result
    * Check that the calculated reference point position is correct.
    */
   DOUBLES_EQUAL_TEXT(expected_reference_point_y, reference_point_y, test_threshold, "Reference point position y coordinate is incorrect.");
}

/** \purpose
 * Test that this function translates a position corresponding to an object's center
 * point to instead correspond to the reference point in tcs y direction when the reference point is left
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension, Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension_left)
{
   /** \precond
    * Set seen_side to left
    */
   const F360_Object_Sides_T seen_side = F360_OBJECT_SIDES_LEFT; 

   const float32_t expected_reference_point_y = 0.5F;

   /** \action
    * Call Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
    */
   const float32_t reference_point_y = Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension(seen_side, obj_width, center_point_tcs_dimension);
  
   /** \result
    * Check that the calculated reference point position is correct.
    */
   DOUBLES_EQUAL_TEXT(expected_reference_point_y, reference_point_y, test_threshold, "Reference point position y coordinate is incorrect.");
}

/** \purpose
 * Test that this function translates a position corresponding to an object's center
 * point to instead correspond to the reference point when the reference point is center (-> seen side = invalid)
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension, Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension_invalid)
{
   /** \precond
    * Set seen_side to invalid
    */
   const F360_Object_Sides_T seen_side = F360_OBJECT_SIDES_INVALID; 

   const float32_t expected_reference_point = 2.5F;

   /** \action
    * Call Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension
    */
   const float32_t reference_point = Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension(seen_side, obj_width, center_point_tcs_dimension);
  
   /** \result
    * Check that the calculated reference point position is correct.
    */
   DOUBLES_EQUAL_TEXT(expected_reference_point, reference_point, test_threshold, "Reference point position is incorrect.");
}

/** @}*/

/** \defgroup  f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search
 *  @{
 */

/** \brief
 * Testing of a function that estimates the pseudo position of a long CTCA object based
 * on the position of associated detections using the grid position and pointing search method
 */

TEST_GROUP(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   float32_t assoc_dets_tcs_x_pos[MAX_DETS_IN_OBJ_TRK];
   float32_t assoc_dets_tcs_y_pos[MAX_DETS_IN_OBJ_TRK];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Sides_T rear_front_side;
   F360_Object_Sides_T right_left_side;
   Angle det_orientation;

   Point pseudo_pos_tcs = {};

   /** \setup
    * Initialize object with the following properties
    * Length greater than 12m (Width can be any reasonable arbitarary value)
    * Number of associated detections is greater than 11
    * Object's longitudinal position can be within -20 to 70m
    * Object must be of CTCA filter type
    */
   TEST_SETUP()
   {
      // Set Object bbox dimensions
      obj.bbox.Set_Length(14.0F);
      obj.bbox.Set_Width(2.5F);

      // Set center and reference point in vcs
      Point vcs_center = {40.0F,4.0F};
      // Get the VCS position of the rear left corner based on the center position
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      obj.vcs_position.x = (-obj.bbox.Get_Length() * 0.5F) + vcs_center.x;
      obj.vcs_position.y = (-obj.bbox.Get_Width() * 0.5F) + vcs_center.y;
      obj.bbox.Set_Center(vcs_center);

      // Set bbox orientation and filter type
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

      // Set visible sides
      rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
      right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   }

   void Generate_Dets_Based_On_Long_Object_Props(
      const float32_t det_orientation,
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t(&assoc_dets_tcs_x_pos)[MAX_DETS_IN_OBJ_TRK],
      float32_t(&assoc_dets_tcs_y_pos)[MAX_DETS_IN_OBJ_TRK])
   {
      // This function assumes that the object's reference point is rear left
      const float32_t half_object_length = obj.bbox.Get_Length() * 0.5F;
      const float32_t half_obj_width = obj.bbox.Get_Width() * 0.5F;
      uint32_t ndets = 0U;

      // Create det points along the left edge of the object bbox
      // The distance between det points is 1m
      for (float32_t i = -half_object_length; i <= half_object_length; i=i+1.0F)
      {
         det_props[ndets].vcs_position.x = i;
         det_props[ndets].vcs_position.y = -half_obj_width;
         obj.detids[ndets] = ndets + 1U;
         ndets++;
      }

      // Create det points along the rear edge of the object bbox
      // The distance between det points is 0.5m
      for (float32_t j = -half_obj_width; j <= half_obj_width; j=j+0.5F)
      {
         det_props[ndets].vcs_position.y = j;
         det_props[ndets].vcs_position.x = -half_object_length;
         obj.detids[ndets] = ndets + 1U;
         ndets++;
      }
      obj.ndets = ndets;

      // Convert generated detection positions from TCS to VCS based on the detection spread orientation
      for (uint32_t det_i = 0U; det_i < ndets; det_i++)
      {
         const float32_t rot_vect_x = det_props[det_i].vcs_position.x * F360_Cosf(det_orientation) - det_props[det_i].vcs_position.y * F360_Sinf(det_orientation);
         const float32_t rot_vect_y = det_props[det_i].vcs_position.x * F360_Sinf(det_orientation) + det_props[det_i].vcs_position.y * F360_Cosf(det_orientation);

         det_props[det_i].vcs_position.x = rot_vect_x + obj.bbox.Get_Center().x;
         det_props[det_i].vcs_position.y = rot_vect_y + obj.bbox.Get_Center().y;
      }

      // Convert detection positions from VCS to TCS for object bbox pointing angle
      for (uint32_t det_i = 0U; det_i < ndets; det_i++)
      {
         Convert_VCS_Posn_To_TCS_Posn(
            det_props[det_i].vcs_position.x,
            det_props[det_i].vcs_position.y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            assoc_dets_tcs_x_pos[det_i],
            assoc_dets_tcs_y_pos[det_i]);
      }
   }

   Point Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(
      const Point& obj_vcs_center,
      const Angle& obj_pointing,
      const Angle& det_orientation,
      const Point& in_tcs_pos)
   {
      Point out_tcs_pos = {};
      float32_t temp_pseudo_pos_vcs_x = 0.0F;
      float32_t temp_pseudo_pos_vcs_y = 0.0F;
      
      // Transform to VCS (using detection spread orientation)
      Convert_TCS_Posn_To_VCS_Posn(
            in_tcs_pos.x,
            in_tcs_pos.y,
            obj_vcs_center.x,
            obj_vcs_center.y,
            det_orientation,
            temp_pseudo_pos_vcs_x,
            temp_pseudo_pos_vcs_y);

      // Transform to TCS (with rotation given by object and not by the pointing grid)
      Convert_VCS_Posn_To_TCS_Posn(
            temp_pseudo_pos_vcs_x,
            temp_pseudo_pos_vcs_y,
            obj_vcs_center.x,
            obj_vcs_center.y,
            obj_pointing,
            out_tcs_pos.x,
            out_tcs_pos.y);

      return out_tcs_pos;
   }
};

/**
*\purpose  This test verifies that, for a long object, the pseudo position estimate is correct, even when the object's placement does not match the detection spread
* The intention of this test is also to verify that the pointing search leads to the correct pseudo position estimate
*\req    NA
*/
TEST(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search, Grid_Search_Pseudo_Position_Estimation_When_Long_Obj_Does_Not_Match_Dets)
{
   /** \precond
    * Set detection spread orientation (-3) i.e the true orientation of the object
    * Generate the detections in VCS, based on the above mentioned detection orientation
    * Generate the expected TCS Pseudo position, assuming that the reference point is rear left
    * Set previous obj.average_grid_search_tcs_position to be same as exp_tcs_pseudo_pos
    */
   (void)det_orientation.Value(F360_DEG2RAD(-3.0F));
   Generate_Dets_Based_On_Long_Object_Props(det_orientation.Value(), obj, det_props, assoc_dets_tcs_x_pos, assoc_dets_tcs_y_pos);
   
   Point temp_tcs_pos;
   temp_tcs_pos.x = -obj.bbox.Get_Length() * 0.5F;
   temp_tcs_pos.y = -obj.bbox.Get_Width() * 0.5F;
   Point exp_tcs_pseudo_pos = Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(obj.bbox.Get_Center(), obj.bbox.Get_Orientation(), det_orientation, temp_tcs_pos);

   obj.average_grid_search_tcs_position.x = exp_tcs_pseudo_pos.x;
   obj.average_grid_search_tcs_position.y = exp_tcs_pseudo_pos.y;

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      obj,
      rear_front_side, 
      right_left_side, 
      assoc_dets_tcs_x_pos, 
      assoc_dets_tcs_y_pos, 
      det_props);

   /** \result
    * Expect pseudo position to be at the rear left corner of the bbox, when rotated by -3 degrees
    */
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.x, pseudo_pos_tcs.x, F360_EPSILON,
                       "The pseudo position in x direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.y, pseudo_pos_tcs.y, F360_EPSILON,
                       "The pseudo position in y direction did not match the expected value.")
}

/**
*\purpose  This test verifies that, for a long object, the pseudo position estimate is correct, when the object's placement matches the detection spread
* The intention of this test is also to verify that the pointing search leads to the correct pseudo position estimate
*\req    NA
*/
TEST(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search, Grid_Search_Pseudo_Position_Estimation_When_Long_Obj_Does_Match_Dets)
{
   /** \precond
    * Set detection spread orientation (0) i.e the true orientation of the object
    * Generate the detections in VCS, based on the above mentioned detection orientation
    * Generate the expected TCS Pseudo position, assuming that the reference point is rear left
    * Set previous obj.average_grid_search_tcs_position to INFTY, this implies that there is no previous mean
    */
   (void)det_orientation.Value(F360_DEG2RAD(0.0F));
   Generate_Dets_Based_On_Long_Object_Props(det_orientation.Value(), obj, det_props, assoc_dets_tcs_x_pos, assoc_dets_tcs_y_pos);
   
   Point temp_tcs_pos;
   temp_tcs_pos.x = -obj.bbox.Get_Length() * 0.5F;
   temp_tcs_pos.y = -obj.bbox.Get_Width() * 0.5F;
   Point exp_tcs_pseudo_pos = Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(obj.bbox.Get_Center(), obj.bbox.Get_Orientation(), det_orientation, temp_tcs_pos);

   obj.average_grid_search_tcs_position.x = INFTY;
   obj.average_grid_search_tcs_position.y = INFTY;

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      obj,
      rear_front_side, 
      right_left_side, 
      assoc_dets_tcs_x_pos, 
      assoc_dets_tcs_y_pos, 
      det_props);

   /** \result
    * Expect pseudo position to be at the rear left corner of the bbox
    */
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.x, pseudo_pos_tcs.x, F360_EPSILON,
                       "The pseudo position in x direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.y, pseudo_pos_tcs.y, F360_EPSILON,
                       "The pseudo position in y direction did not match the expected value.")
}

/** @}*/

/** \defgroup  f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search_without_pointing_grid
 *  @{
 */

/** \brief
 * Testing of a function that estimates the pseudo position of a long object based
 * on the position of associated detections using the grid position (without pointing grid) search method
 */

TEST_GROUP(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search_without_pointing_grid)
{
   // Common variables used in tests
   F360_Object_Track_T obj = {};
   float32_t assoc_dets_tcs_x_pos[MAX_DETS_IN_OBJ_TRK];
   float32_t assoc_dets_tcs_y_pos[MAX_DETS_IN_OBJ_TRK];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Sides_T rear_front_side;
   F360_Object_Sides_T right_left_side;
   Angle det_orientation;

   Point pseudo_pos_tcs = {};

   /** \setup
    * Initialize object with the following properties
    * Length and Width can be any reasonable arbitarary value
    * Number of associated detections is greater than 11
    * Object's longitudinal position should be outside -20 to 70m range
    */
   TEST_SETUP()
   {
      // Set Object bbox dimensions
      obj.bbox.Set_Length(14.0F);
      obj.bbox.Set_Width(2.5F);

      // Set center and reference point in vcs
      Point vcs_center = {90.0F,4.0F};
      // Get the VCS position of the rear left corner based on the center position
      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      obj.vcs_position.x = (-obj.bbox.Get_Length() * 0.5F) + vcs_center.x;
      obj.vcs_position.y = (-obj.bbox.Get_Width() * 0.5F) + vcs_center.y;
      obj.bbox.Set_Center(vcs_center);

      // Set bbox orientation and filter type
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

      // Set visible sides
      rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
      right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);
   }

   void Generate_Dets_Based_On_Long_Object_Props(
      const float32_t det_orientation,
      const float32_t para_det_diff,
      const float32_t orth_det_diff,
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t(&assoc_dets_tcs_x_pos)[MAX_DETS_IN_OBJ_TRK],
      float32_t(&assoc_dets_tcs_y_pos)[MAX_DETS_IN_OBJ_TRK])
   {
      // This function assumes that the object's reference point is rear left
      const float32_t half_object_length = obj.bbox.Get_Length() * 0.5F;
      const float32_t half_obj_width = obj.bbox.Get_Width() * 0.5F;
      uint32_t ndets = 0U;

      // Create det points along the left edge of the object bbox
      // The distance between det points is 1m
      for (float32_t i = -half_object_length; i <= (half_object_length - para_det_diff); i=i+1.0F)
      {
         det_props[ndets].vcs_position.x = i;
         det_props[ndets].vcs_position.y = -half_obj_width;
         obj.detids[ndets] = ndets + 1U;
         ndets++;
      }

      // Create det points along the rear edge of the object bbox
      // The distance between det points is 0.5m
      for (float32_t j = -half_obj_width; j <= (half_obj_width - orth_det_diff); j=j+0.5F)
      {
         det_props[ndets].vcs_position.y = j;
         if (obj.reference_point == F360_REFERENCE_POINT_REAR_LEFT)
         {
            det_props[ndets].vcs_position.x = -half_object_length;
         }
         else if (obj.reference_point == F360_REFERENCE_POINT_FRONT_LEFT)
         {
            det_props[ndets].vcs_position.x = half_object_length;
         }
         else
         {
            // Do Nothing. This function is only for rear and front left ref point
         }
         obj.detids[ndets] = ndets + 1U;
         ndets++;
      }
      obj.ndets = ndets;

      // Convert generated detection positions from TCS to VCS based on the detection spread orientation
      for (uint32_t det_i = 0U; det_i < ndets; det_i++)
      {
         const float32_t rot_vect_x = det_props[det_i].vcs_position.x * F360_Cosf(det_orientation) - det_props[det_i].vcs_position.y * F360_Sinf(det_orientation);
         const float32_t rot_vect_y = det_props[det_i].vcs_position.x * F360_Sinf(det_orientation) + det_props[det_i].vcs_position.y * F360_Cosf(det_orientation);

         det_props[det_i].vcs_position.x = rot_vect_x + obj.bbox.Get_Center().x;
         det_props[det_i].vcs_position.y = rot_vect_y + obj.bbox.Get_Center().y;
      }

      // Convert detection positions from VCS to TCS for object bbox pointing angle
      for (uint32_t det_i = 0U; det_i < ndets; det_i++)
      {
         Convert_VCS_Posn_To_TCS_Posn(
            det_props[det_i].vcs_position.x,
            det_props[det_i].vcs_position.y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            assoc_dets_tcs_x_pos[det_i],
            assoc_dets_tcs_y_pos[det_i]);
      }
   }

   Point Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(
      const Point& obj_vcs_center,
      const Angle& obj_pointing,
      const Angle& det_orientation,
      const Point& in_tcs_pos)
   {
      Point out_tcs_pos = {};
      float32_t temp_pseudo_pos_vcs_x = 0.0F;
      float32_t temp_pseudo_pos_vcs_y = 0.0F;
      
      // Transform to VCS (using detection spread orientation)
      Convert_TCS_Posn_To_VCS_Posn(
            in_tcs_pos.x,
            in_tcs_pos.y,
            obj_vcs_center.x,
            obj_vcs_center.y,
            det_orientation,
            temp_pseudo_pos_vcs_x,
            temp_pseudo_pos_vcs_y);

      // Transform to TCS (with rotation given by object and not by the pointing grid)
      Convert_VCS_Posn_To_TCS_Posn(
            temp_pseudo_pos_vcs_x,
            temp_pseudo_pos_vcs_y,
            obj_vcs_center.x,
            obj_vcs_center.y,
            obj_pointing,
            out_tcs_pos.x,
            out_tcs_pos.y);

      return out_tcs_pos;
   }
};

/**
*\purpose  This test verifies that, for a long object, the pseudo position estimate is correct, when the object's placement matches the detection spread
* The intention of this test is also to verify that without the pointing search, the pseudo position estimate is correct
* In this test, the object is not valid for pointing search because its longitudinal position is above -70m
*\req    NA
*/
TEST(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search_without_pointing_grid, Grid_Search_Pseudo_Position_Estimation_When_Long_Obj_Position_Is_Above_Pointing_Search_Limit)
{
   /** \precond
    * Set detection spread orientation (0) i.e the true orientation of the object
    * Generate the detections in VCS, based on the above mentioned detection orientation
    * Generate the expected TCS Pseudo position, assuming that the reference point is rear left
    */
   (void)det_orientation.Value(F360_DEG2RAD(0.0F));
   Generate_Dets_Based_On_Long_Object_Props(det_orientation.Value(), 0.0F, 0.0F, obj, det_props, assoc_dets_tcs_x_pos, assoc_dets_tcs_y_pos);
   
   Point temp_tcs_pos;
   temp_tcs_pos.x = -obj.bbox.Get_Length() * 0.5F;
   temp_tcs_pos.y = -obj.bbox.Get_Width() * 0.5F;
   Point exp_tcs_pseudo_pos = Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(obj.bbox.Get_Center(), obj.bbox.Get_Orientation(), det_orientation, temp_tcs_pos);

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      obj,
      rear_front_side, 
      right_left_side, 
      assoc_dets_tcs_x_pos, 
      assoc_dets_tcs_y_pos, 
      det_props);

   /** \result
    * Expect pseudo position to be at the rear left corner of the bbox
    */
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.x, pseudo_pos_tcs.x, F360_EPSILON,
                       "The pseudo position in x direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.y, pseudo_pos_tcs.y, F360_EPSILON,
                       "The pseudo position in y direction did not match the expected value.")
}

/**
*\purpose  This test verifies that, for a long object, the pseudo position estimate is correct, when the object's placement matches the detection spread
* The intention of this test is also to verify that without the pointing search, the pseudo position estimate is correct
* In this test, the object is not valid for pointing search because its filter type is CCA
*\req    NA
*/
TEST(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search_without_pointing_grid, Grid_Search_Pseudo_Position_Estimation_When_Long_Obj_Is_CCA)
{
   /** \precond
    * Set object filter type to CCA
    * Set object long pos to be within 0 to 70m
    * Set detection spread orientation (0) i.e the true orientation of the object
    * Generate the detections in VCS, based on the above mentioned detection orientation
    * Generate the expected TCS Pseudo position, assuming that the reference point is rear left
    */
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   // Set center and reference point in vcs
   Point vcs_center = {40.0F,4.0F};
   // Get the VCS position of the rear left corner based on the center position
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.vcs_position.x = (-obj.bbox.Get_Length() * 0.5F) + vcs_center.x;
   obj.vcs_position.y = (-obj.bbox.Get_Width() * 0.5F) + vcs_center.y;
   obj.bbox.Set_Center(vcs_center);
   
   (void)det_orientation.Value(F360_DEG2RAD(0.0F));
   Generate_Dets_Based_On_Long_Object_Props(det_orientation.Value(), 0.0F, 0.0F, obj, det_props, assoc_dets_tcs_x_pos, assoc_dets_tcs_y_pos);
   
   Point temp_tcs_pos;
   temp_tcs_pos.x = -obj.bbox.Get_Length() * 0.5F;
   temp_tcs_pos.y = -obj.bbox.Get_Width() * 0.5F;
   Point exp_tcs_pseudo_pos = Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(obj.bbox.Get_Center(), obj.bbox.Get_Orientation(), det_orientation, temp_tcs_pos);

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      obj,
      rear_front_side, 
      right_left_side, 
      assoc_dets_tcs_x_pos, 
      assoc_dets_tcs_y_pos, 
      det_props);

   /** \result
    * Expect pseudo position to be at the rear left corner of the bbox
    */
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.x, pseudo_pos_tcs.x, F360_EPSILON,
                       "The pseudo position in x direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.y, pseudo_pos_tcs.y, F360_EPSILON,
                       "The pseudo position in y direction did not match the expected value.")
}

/**
*\purpose  This test verifies that, for a long object, the pseudo position estimate is correct, when the object's placement matches the detection spread
* The intention of this test is also to verify that without the pointing search, the pseudo position estimate is correct
* In this test, the object is not valid for pointing search because there are not enough associated detections
*\req    NA
*/
TEST(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search_without_pointing_grid, Grid_Search_Pseudo_Position_Estimation_When_Too_Few_Ndets)
{
   /** \precond
    * Set object long pos to be within 0 to 70m
    * Set detection spread orientation (0) i.e the true orientation of the object
    * Generate the detections in VCS, based on the above mentioned detection orientation
    * There should be 11 or less ndets
    * Generate the expected TCS Pseudo position, assuming that the reference point is rear left
    */

   // Set center and reference point in vcs
   Point vcs_center = {40.0F,4.0F};
   // Get the VCS position of the rear left corner based on the center position
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.vcs_position.x = (-obj.bbox.Get_Length() * 0.5F) + vcs_center.x;
   obj.vcs_position.y = (-obj.bbox.Get_Width() * 0.5F) + vcs_center.y;
   obj.bbox.Set_Center(vcs_center);
   
   (void)det_orientation.Value(F360_DEG2RAD(0.0F));
   Generate_Dets_Based_On_Long_Object_Props(det_orientation.Value(), 9.0F, 0.5F, obj, det_props, assoc_dets_tcs_x_pos, assoc_dets_tcs_y_pos);
   
   Point temp_tcs_pos;
   temp_tcs_pos.x = -obj.bbox.Get_Length() * 0.5F;
   temp_tcs_pos.y = -obj.bbox.Get_Width() * 0.5F;
   Point exp_tcs_pseudo_pos = Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(obj.bbox.Get_Center(), obj.bbox.Get_Orientation(), det_orientation, temp_tcs_pos);

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      obj,
      rear_front_side, 
      right_left_side, 
      assoc_dets_tcs_x_pos, 
      assoc_dets_tcs_y_pos, 
      det_props);

   /** \result
    * Expect pseudo position to be at the rear left corner of the bbox
    */
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.x, pseudo_pos_tcs.x, F360_EPSILON,
                       "The pseudo position in x direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.y, pseudo_pos_tcs.y, F360_EPSILON,
                       "The pseudo position in y direction did not match the expected value.")
}

/**
*\purpose  This test verifies that, for a long object, the pseudo position estimate is correct, when the object's placement matches the detection spread
* The intention of this test is also to verify that without the pointing search, the pseudo position estimate is correct
* In this test, the object is not valid for pointing search because its longitudinal position is below -20m
*\req    NA
*/
TEST(f360_compute_pseudo_pos_tcs_for_extended_object_case_grid_search_without_pointing_grid, Grid_Search_Pseudo_Position_Estimation_When_Long_Obj_Position_Is_Below_Pointing_Search_Limit)
{
   /** \precond
    * Set object long pos to be below -20m
    * Set object ref point to Front Left
    * Set detection spread orientation (0) i.e the true orientation of the object
    * Generate the detections in VCS, based on the above mentioned detection orientation
    * Generate the expected TCS Pseudo position, assuming that the reference point is rear left
    */

   // Set center and reference point in vcs
   Point vcs_center = {-40.0F,4.0F};
   // Get the VCS position of the rear left corner based on the center position
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.vcs_position.x = (obj.bbox.Get_Length() * 0.5F) + vcs_center.x;
   obj.vcs_position.y = (-obj.bbox.Get_Width() * 0.5F) + vcs_center.y;
   obj.bbox.Set_Center(vcs_center);
   // Set visible sides
   rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
   right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);

   (void)det_orientation.Value(F360_DEG2RAD(0.0F));
   Generate_Dets_Based_On_Long_Object_Props(det_orientation.Value(), 0.0F, 0.0F, obj, det_props, assoc_dets_tcs_x_pos, assoc_dets_tcs_y_pos);
   
   Point temp_tcs_pos;
   temp_tcs_pos.x = obj.bbox.Get_Length() * 0.5F;
   temp_tcs_pos.y = -obj.bbox.Get_Width() * 0.5F;
   Point exp_tcs_pseudo_pos = Transform_TCS_POS_wrt_Det_Pointing_To_TCS_POS_wrt_Object_Pointing(obj.bbox.Get_Center(), obj.bbox.Get_Orientation(), det_orientation, temp_tcs_pos);

   /** \action
    * Call function
    */
   pseudo_pos_tcs = Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      obj,
      rear_front_side, 
      right_left_side, 
      assoc_dets_tcs_x_pos, 
      assoc_dets_tcs_y_pos, 
      det_props);

   /** \result
    * Expect pseudo position to be at the front left corner of the bbox
    */
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.x, pseudo_pos_tcs.x, F360_EPSILON,
                       "The pseudo position in x direction did not match the expected value.")
   DOUBLES_EQUAL_TEXT(exp_tcs_pseudo_pos.y, pseudo_pos_tcs.y, F360_EPSILON,
                       "The pseudo position in y direction did not match the expected value.")
}
/** @}*/

/** \defgroup  f360_pseudo_position_estimation_Get_Final_TCS_Pseudo_Pos
 *  @{
 */

/** \brief
 * Testing of a function that computes the final grid search based pseudo position
 */

TEST_GROUP(f360_pseudo_position_estimation_Get_Final_TCS_Pseudo_Pos)
{
   // Common variables used in tests
   bool f_valid_pseudo_pnt = true;
   Point obj_average_grid_search_tcs_position;
   Point unfiltered_pseudo_pos_tcs;
   Point final_pseudo_pos_tcs;
   const float32_t test_threshold = 0.00001F;

   /** \setup
    * Set up a scenario where the object's average grid search position valid (< INF in x and y)
    * Set unfiltered pseudo position estimate to an arbitrary value
    * Set object valid for pseudo pointing
    */
   TEST_SETUP()
   {      
      obj_average_grid_search_tcs_position.x = 10.0F;
      obj_average_grid_search_tcs_position.y = 5.0F;
      unfiltered_pseudo_pos_tcs.x = 11.0F;
      unfiltered_pseudo_pos_tcs.y = 4.0F;
   }
};

/** \purpose
 * Test that the final pseudo position is the average of the unfiltered estimate and the previous average object pseudo position
 * when the average object pseudo position is within bounds. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Get_Final_TCS_Pseudo_Pos, Get_Final_TCS_Pseudo_Pos_Valid_Avg_Pos)
{
   /** \precond
    * A default scenario with valid average obj pseudo pos is set up in TEST_GROUP.
    */
   const Point exp_final_pseudo_pos = {10.2F, 4.8F};

   /** \action
    * Call Iterate_Over_Grid
    */
   final_pseudo_pos_tcs = Get_Final_TCS_Pseudo_Pos(f_valid_pseudo_pnt, obj_average_grid_search_tcs_position, unfiltered_pseudo_pos_tcs);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_final_pseudo_pos.x, final_pseudo_pos_tcs.x, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the final pseudo position is the the unfiltered estimate when the average object pseudo x position is not within bounds. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Get_Final_TCS_Pseudo_Pos, Get_Final_TCS_Pseudo_Pos_Avg_Pos_X_Too_Big)
{
   /** \precond
    * A default scenario with valid average obj pseudo pos is set up in TEST_GROUP.
    * Set object's average grid search x position to INFTY.
    * Expectation is that the unfiltered pseudo pos is used.
    */
   obj_average_grid_search_tcs_position.x = INFTY;
   Point exp_final_pseudo_pos;
   exp_final_pseudo_pos.x = unfiltered_pseudo_pos_tcs.x;
   exp_final_pseudo_pos.y = unfiltered_pseudo_pos_tcs.y;

   /** \action
    * Call Iterate_Over_Grid
    */
   final_pseudo_pos_tcs = Get_Final_TCS_Pseudo_Pos(f_valid_pseudo_pnt, obj_average_grid_search_tcs_position, unfiltered_pseudo_pos_tcs);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_final_pseudo_pos.x, final_pseudo_pos_tcs.x, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the final pseudo position is the the unfiltered estimate when the average object pseudo y position is not within bounds. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Get_Final_TCS_Pseudo_Pos, Get_Final_TCS_Pseudo_Pos_Avg_Pos_Y_Too_Big)
{
   /** \precond
    * A default scenario with valid average obj pseudo pos is set up in TEST_GROUP.
    * Set object's average grid search y position to INFTY.
    * Expectation is that the unfiltered pseudo pos is used.
    */
   obj_average_grid_search_tcs_position.y = INFTY;
   Point exp_final_pseudo_pos;
   exp_final_pseudo_pos.x = unfiltered_pseudo_pos_tcs.x;
   exp_final_pseudo_pos.y = unfiltered_pseudo_pos_tcs.y;

   /** \action
    * Call Iterate_Over_Grid
    */
   final_pseudo_pos_tcs = Get_Final_TCS_Pseudo_Pos(f_valid_pseudo_pnt, obj_average_grid_search_tcs_position, unfiltered_pseudo_pos_tcs);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_final_pseudo_pos.x, final_pseudo_pos_tcs.x, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \purpose
 * Test that the final pseudo position is the the unfiltered estimate when the average object pseudo x and y position is not within bounds. 
 * \req
 * NA
 */
TEST(f360_pseudo_position_estimation_Get_Final_TCS_Pseudo_Pos, Get_Final_TCS_Pseudo_Pos_Avg_Pos_X_And_Y_Too_Big)
{
   /** \precond
    * A default scenario with valid average obj pseudo pos is set up in TEST_GROUP.
    * Set object's average grid search y position to INFTY.
    * Expectation is that the unfiltered pseudo pos is used.
    */
   obj_average_grid_search_tcs_position.x = INFTY;
   obj_average_grid_search_tcs_position.y = INFTY;
   Point exp_final_pseudo_pos;
   exp_final_pseudo_pos.x = unfiltered_pseudo_pos_tcs.x;
   exp_final_pseudo_pos.y = unfiltered_pseudo_pos_tcs.y;

   /** \action
    * Call Iterate_Over_Grid
    */
   final_pseudo_pos_tcs = Get_Final_TCS_Pseudo_Pos(f_valid_pseudo_pnt, obj_average_grid_search_tcs_position, unfiltered_pseudo_pos_tcs);
      
   /** \result
    * Check that the computed pseudo pos coordinate matches the expected output.
    */
   DOUBLES_EQUAL_TEXT(exp_final_pseudo_pos.x, final_pseudo_pos_tcs.x, test_threshold, "Computed pseudo position coordinate doesn't match the expected value.")
}

/** \defgroup  f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
 *  @{
 */

/** \brief
 * Testing of Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner function that calculates a lateral
 * adjustment for the pseudo position to prevent object front corners from protruding into the host
 * vehicle path due to incorrect heading or reference point estimation.
 */

TEST_GROUP(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner)
{
   F360_Object_Track_T obj = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Host_T host = {};
   
   float32_t pseudo_pos_y_adjustment = 0.0F;
   float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up a scenario with an oncoming object that has protruding corners
    * based on the specified test parameters:
    * - Host curvature = -0.001 (slight left curve)
    * - Object dimensions: Length 5.000m, Width 2.000m, Speed 5.000m/s
    * - Reference Point Position (VCS): (29.500, -3.000) m
    * - Pseudo Position (VCS): (30.000, -2.800) m
    * - Pointing Angle: 180.0 degrees
    * - Reference Point: FRONT
    * - 4 detections at specified positions
    */
   TEST_SETUP()
   {
      host.curvature_rear = -0.001F;

      obj.bbox.Set_Length(5.0F);
      obj.bbox.Set_Width(2.0F);
      obj.speed = 5.0F;      
      obj.pseudo_vcs_position.x = 30.0F;
      obj.pseudo_vcs_position.y = -2.8F;      
      obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(165)});
      obj.vcs_heading = Angle{F360_DEG2RAD(165)};
      obj.reference_point = F360_REFERENCE_POINT_FRONT; 
      obj.vcs_position.x = 29.5F;
      obj.vcs_position.y = -3.0F;
      obj.vcs_velocity.longitudinal = -obj.speed;
      obj.Update_Bbox_Center();
      obj.time_since_initialization = 2.0F;      
      obj.ndets = 4;
      obj.detids[0] = 1;
      obj.detids[1] = 2;  
      obj.detids[2] = 3;
      obj.detids[3] = 4;
      obj.id = 1U;
      
      det_props[0].vcs_position.x = 30.0F;
      det_props[0].vcs_position.y = -3.0F;
      det_props[0].object_track_id = obj.id;
      
      det_props[1].vcs_position.x = 31.0F;
      det_props[1].vcs_position.y = -3.2F;
      det_props[1].object_track_id = obj.id;
      
      det_props[2].vcs_position.x = 32.0F;
      det_props[2].vcs_position.y = -2.9F;
      det_props[2].object_track_id = obj.id;
      
      det_props[3].vcs_position.x = 33.0F;
      det_props[3].vcs_position.y = -2.7F;
      det_props[3].object_track_id = obj.id;
   }
};

/**
 * \purpose 
 * Test the basic functionality of Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner with the specified scenario where
 * an oncoming object has its front left corner protruding beyond the detection spread toward the host path. The expectation
 * is that pseudo position is adjusted away from host path.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, Calculate_Adjustment_For_Protruding_Corner_Basic_Scenario)
{
   /** \precond
    * Test scenario has been set up in TEST_SETUP with:
    * - Oncoming object (165 deg pointing angle) 
    * - Object positioned on left side of host path
    * - Pseudo position offset from actual position
    * - 4 detections spread around the object
    * - Host curvature = 0 (straight path)
    */
   const float32_t exp_pseudo_pos_y_adjustment = -0.865926F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that an adjustment is calculated since the object's front corner
    * would protrude beyond the detection spread toward the host path.
    * The adjustment should be non-zero and move the pseudo position to align
    * the critical corner with the most extreme detection.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "The calculated pseudo position y adjustment did not match the expected value.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object heading is just outside the oncoming threshold (too far clockwise).
 * Changes object pointing angle to 154 deg which is just outside the 25 deg tolerance threshold.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Heading_Outside_Threshold_Clockwise)
{
   /** \precond
    * Modify object heading to be just outside the oncoming threshold (clockwise)
    */
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(154.0F)});  // 180 deg - 25 deg - 1 deg = 154 deg
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since the object is not considered oncoming
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object heading is outside oncoming threshold (clockwise).");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object heading is just outside the oncoming threshold (too far counter-clockwise).
 * Changes object pointing angle to -154 deg which is just outside the 25 deg tolerance threshold.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Heading_Outside_Threshold_CounterClockwise)
{
   /** \precond
    * Modify object heading to be just outside the oncoming threshold (counter-clockwise)
    */
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(-154.0F)});
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since the object is not considered oncoming
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object heading is outside oncoming threshold (counter-clockwise).");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object X position is beyond the relevant range (too far).
 * Changes object X position to 41m which exceeds the 40m threshold for position relevance.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_X_Position_Too_Far)
{
   /** \precond
    * Modify object and detection positions to be beyond 40m threshold
    */
   const float32_t x_offset = 11.5F; // Move from 29.5m to 41.0m
   obj.vcs_position.x += x_offset;
   obj.pseudo_vcs_position.x += x_offset;
   obj.Update_Bbox_Center();
   
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      det_props[i].vcs_position.x += x_offset;
   }
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since object X position is not relevant (> 40m)
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object X position is beyond relevant range.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object X position is negative (behind host).
 * Changes object X position to -5m which is less than 0m threshold for position relevance.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_X_Position_Behind_Host)
{
   /** \precond
    * Modify object and detection positions to be behind host vehicle
    */
   const float32_t x_offset = -34.5F; // Move from 29.5m to -5.0m
   obj.vcs_position.x += x_offset;
   obj.pseudo_vcs_position.x += x_offset;
   obj.Update_Bbox_Center();
   
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      det_props[i].vcs_position.x += x_offset;
   }
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since object X position is not relevant (< 0m)
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object X position is behind host vehicle.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object is not mature enough.
 * Changes time_since_initialization to 0.5s which is less than 1.0s threshold for object maturity.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Object_Not_Mature)
{
   /** \precond
    * Modify object to be not mature enough (< 1.0s since initialization)
    */
   obj.time_since_initialization = 0.5F;
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since object is not mature enough
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object is not mature enough.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when host curvature is too large.
 * Changes host curvature to 0.06 rad/m which exceeds the 0.05 rad/m threshold.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Host_Curvature_Too_Large)
{
   /** \precond
    * Modify host curvature to exceed threshold (> 0.05 rad/m)
    */
   host.curvature_rear = 0.06F;
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since host curvature is too large
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when host curvature is too large.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object corners are on different sides of host path.
 * Translates object and all detections 3m to the right so front corners span across host path,
 * preventing determination of a critical corner.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Corners_On_Different_Sides_Of_Host_Path)
{
   /** \precond
    * Translate object and detections 3m to the right to put corners on different sides of host path
    */
   const float32_t y_offset = 3.0F;
   obj.vcs_position.y += y_offset;
   obj.pseudo_vcs_position.y += y_offset;
   obj.Update_Bbox_Center();
   
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      det_props[i].vcs_position.y += y_offset;
   }
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since critical corner cannot be determined
    * when corners are on different sides of host path
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object corners span across host path.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when object is flagged as being in direct host path.
 * Moves object and all detections 1.3m to the right so its lateral position is close to host centerline,
 * making it be considered as in the direct host path.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Object_In_Direct_Host_Path)
{
   /** \precond
    * Move object and detections closer to host path centerline to be flagged as in direct path
    * but keep both corners on the same side of host path
    */
   const float32_t y_offset = 1.3F;
   obj.vcs_position.y += y_offset;
   obj.pseudo_vcs_position.y += y_offset;
   obj.Update_Bbox_Center();
   
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      det_props[i].vcs_position.y += y_offset;
   }
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since object is considered to be in direct host path
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when object is in direct host path.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when critical corner is too far from host path.
 * Moves object and detections further from host path so critical corner distance exceeds 3m threshold.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Corner_Too_Far_From_Host_Path)
{
   /** \precond
    * Move object and detections further from host path so corner distance exceeds threshold
    */
   const float32_t y_offset = -2.5F; // Move from -3.0m to -5.5m (corner will be > 3m from host path)
   obj.vcs_position.y += y_offset;
   obj.pseudo_vcs_position.y += y_offset;
   obj.Update_Bbox_Center();
   
   for (uint32_t i = 0U; i < obj.ndets; i++)
   {
      det_props[i].vcs_position.y += y_offset;
   }
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since critical corner is too far from host path
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when critical corner is too far from host path.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when most extreme detection is more extreme than critical corner.
 * Moves one detection closer to host path (more extreme) than the critical object corner, so the corner
 * is not the most protruding element toward the host path.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Detection_More_Extreme_Than_Corner)
{
   /** \precond
    * Keep detections in original positions but move object 1m to the left (more negative Y)
    * This makes the detections relatively more extreme than the critical corner
    */
   const float32_t y_offset = -1.0F; // Move object from -3.0m to -4.0m
   obj.vcs_position.y += y_offset;
   obj.pseudo_vcs_position.y += y_offset;
   obj.Update_Bbox_Center();
   
   // Keep detections at original positions: [-3.0, -3.2, -2.9, -2.7]
   // These will now be more extreme (closer to host) than the object's critical corner
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since detection is more extreme than critical corner
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when most extreme detection is more extreme than critical corner.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when most extreme detection is too close to host path.
 * Uses a modified setup with wider object and front right reference point to create a scenario where
 * the critical corner is more extreme than detections but one detection is within 1.8m threshold.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Most_Extreme_Detection_Too_Close_To_Host_Path)
{
   /** \precond
    * Modify object setup: 180 deg orientation, 2.5m width, front right reference point
    * Then move one detection to be just closer than 1.8m from host path
    */
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(180.0F)});  // Straight oncoming
   obj.bbox.Set_Width(2.5F);  // Wider object
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;  // Front right reference
   obj.Update_Bbox_Center();
   
   // Move one detection to be just within 1.8m threshold
   det_props[3].vcs_position.y = -1.7F; // Move from -2.7m to -1.7m (< 1.8m threshold)
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since most extreme detection is too close to host path
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when most extreme detection is too close to host path.");
}

/**
 * \purpose 
 * Test the basic functionality with object on the right side of host path (mirrored scenario).
 * This mirrors the basic scenario but places the object on the positive Y side of the host path
 * to test that the algorithm works correctly for objects on either side.
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, Calculate_Adjustment_For_Protruding_Corner_Basic_Scenario_Right_Side)
{
   /** \precond
    * Mirror the entire scenario: host curvature, object position, orientation, and detections
    */
   // Mirror host curvature from left curve to right curve
   host.curvature_rear = +0.001F;
   
   // Mirror object Y positions
   obj.vcs_position.y = +3.0F;  // Mirror from -3.0F
   obj.pseudo_vcs_position.y = +2.8F;  // Mirror from -2.8F
   
   // Mirror object orientation: 165 deg -> 195 deg (mirror around 180 deg) 
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(195)});
   obj.vcs_heading = Angle{F360_DEG2RAD(195)};
   
   obj.Update_Bbox_Center();
   
   // Mirror detection Y positions
   det_props[0].vcs_position.y = +3.0F;  // Mirror from -3.0F
   det_props[1].vcs_position.y = +3.2F;  // Mirror from -3.2F
   det_props[2].vcs_position.y = +2.9F;  // Mirror from -2.9F
   det_props[3].vcs_position.y = +2.7F;  // Mirror from -2.7F
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.865926F; // Positive adjustment (toward host path from right side)

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that an adjustment is calculated for right-side object.
    * The adjustment should be positive, moving the pseudo position toward the host path.
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "The calculated pseudo position y adjustment did not match the expected value for right-side object.");
}

/**
 * \purpose 
 * Test that no adjustment is calculated when most extreme detection is more extreme than critical corner
 * for an object on the right side of host path (mirrored scenario).
 * \req NA
 */
TEST(f360_pseudo_position_estimation_Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner, No_Adjustment_When_Detection_More_Extreme_Than_Corner_Right_Side)
{
   /** \precond
    * Mirror the scenario and make detections more extreme than corner
    */
   // Mirror host curvature from left curve to right curve
   host.curvature_rear = +0.001F;
   
   // Mirror object Y positions and move object further from host path
   const float32_t y_object_offset = 1.0F; // Move object further away so detections are more extreme
   obj.vcs_position.y = +3.0F + y_object_offset;  // Mirror from -3.0F to +4.0F
   obj.pseudo_vcs_position.y = +2.8F + y_object_offset;  // Mirror from -2.8F to +3.8F
   
   // Mirror object orientation: 165 deg -> 195 deg (mirror around 180 deg)
   obj.bbox.Set_Orientation(Angle{F360_DEG2RAD(195)});
   
   obj.Update_Bbox_Center();
   
   // Keep detections at mirrored original positions (more extreme than object corner)
   det_props[0].vcs_position.y = +3.0F;  // Mirror from -3.0F
   det_props[1].vcs_position.y = +3.2F;  // Mirror from -3.2F  
   det_props[2].vcs_position.y = +2.9F;  // Mirror from -2.9F
   det_props[3].vcs_position.y = +2.7F;  // Mirror from -2.7F
   
   const float32_t exp_pseudo_pos_y_adjustment = 0.0F;

   /** \action
    * Call Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner
    */
   pseudo_pos_y_adjustment = Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(det_props, host, obj);

   /** \result
    * Verify that no adjustment is calculated since detection is more extreme than critical corner
    */
   DOUBLES_EQUAL_TEXT(exp_pseudo_pos_y_adjustment, pseudo_pos_y_adjustment, test_threshold,
                       "No adjustment should be calculated when most extreme detection is more extreme than critical corner (right-side object).");
}

/** \defgroup  f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos
 *  @{
 */

 /** \brief
  * Unit tests for Get_Most_Critical_Front_Corner_Ypos()
  */
TEST_GROUP(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos)
{
   F360_Object_Track_T obj = {};
   F360_Host_T host = {};
   float32_t critical_corner_y_pos = 0.0F;
   const float32_t test_threshold = 0.0001F;

   /** \setup
    * Set up a default object with length 4, width 2, orientation 180, center at (10,0)
    * Host at default (no curvature)
    */
   TEST_SETUP()
   {
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(F360_DEG2RAD(180.0F));
      obj.bbox.Set_Center(Point{ 10.0F, 0.0F });
      obj.vcs_velocity.longitudinal = -5.0F; // Oncoming at 5 m/s
      host.curvature_rear = 0.0F;
   }
};

/**
 * \purpose  Both corners to the left of host path: should return true and y of front left
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Both_Corners_Left)
{
   /** \precond
    * Place object to the left of host path (negative y)
    */
   obj.bbox.Set_Center(Point{ 10.0F, -5.0F });

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_TRUE_TEXT(found, "Should find a critical corner when both corners are left of host path.");
   // For oncoming objects (180 deg) with both corners left: front left is most critical
   // Expected front left corner y = center_y + width/2 = -5.0 + 1.0 = -4.0
   DOUBLES_EQUAL_TEXT(-4.0F, critical_corner_y_pos, test_threshold, "Critical corner should be front left corner.");
}

/**
 * \purpose  Both corners to the right of host path: should return true and y of front right
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Both_Corners_Right)
{
   /** \precond
    * Place object to the right of host path (positive y)
    */
   obj.bbox.Set_Center(Point{ 10.0F, 5.0F });

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_TRUE_TEXT(found, "Should find a critical corner when both corners are right of host path.");
   // For oncoming objects (180 deg) with both corners right: front right is most critical  
   // Expected front right corner y = center_y - width/2 = 5.0 - 1.0 = 4.0
   DOUBLES_EQUAL_TEXT(4.0F, critical_corner_y_pos, test_threshold, "Critical corner should be front right corner.");
}

/**
 * \purpose  Corners on different sides of host path: should return false and not set y
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Corners_On_Different_Sides)
{
   /** \precond
    * Place object so that one corner is left and one is right of host path
    * Set up object directly in line with host but 10m ahead longitudinally (i.e. already done in TEST_SETUP)
    */
   critical_corner_y_pos = 100.0F; // placeholder to check that value is not overwritten

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_FALSE_TEXT(found, "Should not find a critical corner when corners are on different sides.");
   // Value should remain unchanged
   DOUBLES_EQUAL_TEXT(100.0F, critical_corner_y_pos, test_threshold, "critical_corner_y_pos should not be set.");
}

/**
 * \purpose  Both corners exactly on host path (y=0): should return true and y of front right
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Both_Corners_On_Host_Path)
{
   /** \precond
    * Place object so both corners are at y=0
    */
   obj.bbox.Set_Center(Point{ 10.0F, -2.0F });
   obj.bbox.Set_Orientation(F360_DEG2RAD(90.0F));

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_TRUE_TEXT(found, "Should find a critical corner when both corners are on host path.");
   DOUBLES_EQUAL_TEXT(0.0F, critical_corner_y_pos, test_threshold, "Critical corner y should be zero.");
}

/**
 * \purpose  Front corners on opposite sides: front left negative, front right positive. Note that this test
 * requires a 0 degree orientation to achieve this configuration which is set up here even though the function
 * is to be called for oncoming objects, i.e. with orientations near 180 degrees.
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Front_Left_Negative_Front_Right_Positive)
{
   /** \precond
    * Place object with 0 degree orientation so front left < 0 and front right >= 0
    */
   obj.bbox.Set_Center(Point{ 10.0F, 0.0F });
   obj.bbox.Set_Orientation(F360_DEG2RAD(0.0F));  // Object pointing forward
   critical_corner_y_pos = 100.0F; // placeholder to check that value is not overwritten

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_FALSE_TEXT(found, "Should not find a critical corner when front corners are on different sides.");
   // Value should remain unchanged
   DOUBLES_EQUAL_TEXT(100.0F, critical_corner_y_pos, test_threshold, "critical_corner_y_pos should not be set.");
}

/**
 * \purpose  Both corners to the left of host path with positive velocity: should return true and y of front right
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Both_Corners_Left_Ongoing)
{
   /** \precond
    * Place object to the left of host path (negative y) with positive longitudinal velocity (ongoing)
    */
   obj.bbox.Set_Center(Point{ 10.0F, -5.0F });
   obj.bbox.Set_Orientation(F360_DEG2RAD(0.0F)); // Ongoing object orientation
   obj.vcs_velocity.longitudinal = 5.0F; // Ongoing object (positive velocity)

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_TRUE_TEXT(found, "Should find a critical corner when both corners are left of host path (ongoing).");
   // For ongoing objects with both corners left: front right is most critical
   // Expected front right corner y = center_y + width/2 = -5.0 + 1.0 = -4.0
   DOUBLES_EQUAL_TEXT(-4.0F, critical_corner_y_pos, test_threshold, "Critical corner should be front right corner.");
}

/**
 * \purpose  Both corners to the right of host path with positive velocity: should return true and y of front left
 */
TEST(f360_pseudo_position_estimation_Get_Most_Critical_Front_Corner_Ypos, Both_Corners_Right_Ongoing)
{
   /** \precond
    * Place object to the right of host path (positive y) with positive longitudinal velocity (ongoing)
    */
   obj.bbox.Set_Center(Point{ 10.0F, 5.0F });
   obj.bbox.Set_Orientation(F360_DEG2RAD(0.0F)); // Ongoing object orientation
   obj.vcs_velocity.longitudinal = 5.0F; // Ongoing object (positive velocity)

   /** \action */
   bool found = Get_Most_Critical_Front_Corner_Ypos(obj, host, critical_corner_y_pos);

   /** \result */
   CHECK_TRUE_TEXT(found, "Should find a critical corner when both corners are right of host path (ongoing).");
   // For ongoing objects with both corners right: front left is most critical
   // Expected front left corner y = center_y - width/2 = 5.0 - 1.0 = 4.0
   DOUBLES_EQUAL_TEXT(4.0F, critical_corner_y_pos, test_threshold, "Critical corner should be front left corner.");
}

/** \defgroup  f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path
 *  @{
 */

 /** \brief
  * Unit tests for Is_Object_Not_In_Direct_Host_Path()
  */
TEST_GROUP(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path)
{
   F360_Object_Track_T obj;
   float32_t obj_curvi_lat_pos;
   bool result;

   /** \setup
    * Set up a default object with length 4, width 2
    */
   TEST_SETUP()
   {
      obj = {};
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj_curvi_lat_pos = 0.0F;
      result = false;
   }
};

/**
*\purpose  Verify that function returns true when corner reference point is visible and curvi_lat_pos is above threshold (positive)
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Corner_Visible_Above_Threshold_Positive)
{
   /** \precond
    * Set reference point to FRONT_LEFT corner
    * Set curvi_lat_pos to 0.6 (above 0.5 threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj_curvi_lat_pos = 0.6F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return true
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Verify that function returns true when corner reference point is visible and curvi_lat_pos is above threshold (negative)
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Corner_Visible_Above_Threshold_Negative)
{
   /** \precond
    * Set reference point to REAR_LEFT corner
    * Set curvi_lat_pos to -0.6 (above 0.5 threshold in absolute value)
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj_curvi_lat_pos = -0.6F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return true
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Verify that function returns false when corner reference point is visible and curvi_lat_pos equals threshold
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Corner_Visible_At_Threshold)
{
   /** \precond
    * Set reference point to REAR_RIGHT corner
    * Set curvi_lat_pos to 0.5 (at threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj_curvi_lat_pos = 0.5F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns false when corner reference point is visible and curvi_lat_pos is below threshold
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Corner_Visible_Below_Threshold)
{
   /** \precond
    * Set reference point to FRONT_RIGHT corner
    * Set curvi_lat_pos to 0.1 (below 0.5 threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj_curvi_lat_pos = 0.1F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns true when long side (LEFT) is visible and curvi_lat_pos is above threshold + half length
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Long_Side_Left_Above_Threshold)
{
   /** \precond
    * Set reference point to LEFT
    * Object length is 4.0m (threshold = 0.5 + 2.0 = 2.5)
    * Set curvi_lat_pos to 2.6 (above threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj_curvi_lat_pos = 2.6F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return true
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Verify that function returns true when long side (RIGHT) is visible and curvi_lat_pos is above threshold + half length (negative)
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Long_Side_Right_Above_Threshold_Negative)
{
   /** \precond
    * Set reference point to RIGHT
    * Object length is 4.0m (threshold = 0.5 + 2.0 = 2.5)
    * Set curvi_lat_pos to -2.6 (above threshold in absolute value)
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj_curvi_lat_pos = -2.6F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return true
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Verify that function returns false when long side is visible and curvi_lat_pos equals threshold + half length
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Long_Side_At_Threshold)
{
   /** \precond
    * Set reference point to LEFT
    * Object length is 4.0m (threshold = 0.5 + 2.0 = 2.5)
    * Set curvi_lat_pos to 2.5 (at threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj_curvi_lat_pos = 2.5F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns false when long side is visible and curvi_lat_pos is below threshold + half length
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Long_Side_Below_Threshold)
{
   /** \precond
    * Set reference point to RIGHT
    * Object length is 4.0m (threshold = 0.5 + 2.0 = 2.5)
    * Set curvi_lat_pos to 2.4 (below threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj_curvi_lat_pos = 2.4F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns true when short side (FRONT) is visible and curvi_lat_pos is above threshold + half width
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Short_Side_Front_Above_Threshold)
{
   /** \precond
    * Set reference point to FRONT
    * Set object width to 2.0m (threshold = 0.5 + 1.0 = 1.5)
    * Set curvi_lat_pos to 1.6 (above threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.bbox.Set_Width(2.0F);
   obj_curvi_lat_pos = 1.6F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return true
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Verify that function returns true when short side (REAR) is visible and curvi_lat_pos is above threshold + half width (negative)
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Short_Side_Rear_Above_Threshold_Negative)
{
   /** \precond
    * Set reference point to REAR
    * Set object width to 2.0m (threshold = 0.5 + 1.0 = 1.5)
    * Set curvi_lat_pos to -1.6 (above threshold in absolute value)
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.bbox.Set_Width(2.0F);
   obj_curvi_lat_pos = -1.6F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return true
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Verify that function returns false when short side is visible and curvi_lat_pos equals threshold + half width
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Short_Side_At_Threshold)
{
   /** \precond
    * Set reference point to FRONT
    * Set object width to 2.0m (threshold = 0.5 + 1.0 = 1.5)
    * Set curvi_lat_pos to 1.5 (at threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.bbox.Set_Width(2.0F);
   obj_curvi_lat_pos = 1.5F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns false when short side is visible and curvi_lat_pos is below threshold + half width
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Short_Side_Below_Threshold)
{
   /** \precond
    * Set reference point to REAR
    * Set object width to 2.0m (threshold = 0.5 + 1.0 = 1.5)
    * Set curvi_lat_pos to 1.4 (below threshold)
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.bbox.Set_Width(2.0F);
   obj_curvi_lat_pos = 1.4F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns false when reference point is CENTER (default case)
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Default_Case_Center_Large_Positive)
{
   /** \precond
    * Set reference point to CENTER
    * Set curvi_lat_pos to a large positive value
    */
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj_curvi_lat_pos = 100.0F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false (default case)
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Verify that function returns false when reference point is CENTER (default case) with large negative value
*\req    NA
*/
TEST(f360_pseudo_position_estimation_Is_Object_Not_In_Direct_Host_Path, Default_Case_Center_Large_Negative)
{
   /** \precond
    * Set reference point to CENTER
    * Set curvi_lat_pos to a large negative value
    */
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj_curvi_lat_pos = -100.0F;

   /** \action
    * Call Is_Object_Not_In_Direct_Host_Path
    */
   result = Is_Object_Not_In_Direct_Host_Path(obj, obj_curvi_lat_pos);

   /** \result
    * Expect function to return false (default case)
    */
   CHECK_FALSE(result);
}

/** @}*/
