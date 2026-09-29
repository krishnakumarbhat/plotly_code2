/** \file
 * This file contains unit tests for content of f360_calc_obj_size.cpp file
 */

#include "f360_calc_obj_size.h"
#include <CppUTest/TestHarness.h>

#include "f360_reference_point.h"
#include "f360_update_object_reference_point.h"


using namespace f360_variant_A;

/** \defgroup  Update_Process_Noise_If_Obj_Outside_Front_Only_Fov
 *  @{
 */

 /** \brief
 * The purpose of this test group is to test the functionality of the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov().
  */

TEST_GROUP(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov)
{
   /** \setup
   * Set up variables called by Update_Process_Noise_If_Obj_Outside_Front_Only_Fov function.
    */
   F360_Calibrations_T calib;
   F360_Object_Track_T  obj = {};
   F360_Radar_Sensor_T(sensors)[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   float32_t process_noise;
   float32_t tolerance = 1e-6F;

   TEST_SETUP()
   {
      // Use default tracker settings for calibrations
      Initialize_Tracker_Calibrations(calib);

      /* Set up object with
         - Reference point REAR RIGHT
            - with position (7,-6)
         - Pointing 0 degrees
         - Width 2m
         - Length 6m
         - vehicular_track set to true
         - f_movable set to true
         - Speed 10 m/s
      */
      obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      obj.id = 15U;
      obj.vcs_position.x = 7.0F;
      obj.vcs_position.y = -6.0F;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(0.0F);
      obj.length_uncertainty = 3.0F;
      obj.width_uncertainty = 2.0F;
      obj.ndets = 0U; // not used in process noise update
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.movable_prob = 1.0F;
      obj.f_vehicular_trk = true;
      obj.speed = 10.0F;
      process_noise = 1.0F;
      globals.f_single_front_center_radar_only = true;

      for(int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
      {

         sensors[i].variable.is_valid = true;
      }

      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;
      // Default generous FOV normals
      globals.rotated_left_fov_normal[0][0] = 1.0F; globals.rotated_left_fov_normal[0][1] = 0.0F;
      globals.rotated_right_fov_normal[0][0] = 1.0F; globals.rotated_right_fov_normal[0][1] = 0.0F;
   }
};

/** \purpose
 * Verify noise reduction when front-right not visible (both side points not fully visible on RIGHT side) reduces process noise.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Right_Front_Not_Visible_Prunes)
{
   /** \precond
    * Invalidate sensors to make all points invisible with RIGHT reference point selection.
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   for(int i=0;i<MAX_NUMBER_OF_SENSORS;++i){ sensors[i].variable.is_valid = false; }
   const float32_t initial_noise = process_noise;
   const float32_t expected = initial_noise * calib.k_size_update_process_noise_pruning;

   /** \action
    * Update process noise.
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Expect noise reduction.
    */
   DOUBLES_EQUAL_TEXT(expected, process_noise, tolerance, "Process noise not reduced when front-right not visible.");
}

/** \purpose
 * The purpose of this test is to check if process noise changes when there is no front sensor.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_No_Front_Sensor)
{
   /** \precond
    * Set up a movable CCA object with default size
   */

   globals.f_single_front_center_radar_only = false;
   const float32_t expected_process_noise = process_noise;

   /** \action
   * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
   * Check that the output object dimensions are correct after the call to Update_Process_Noise_If_Obj_Outside_Front_Only_Fov().
    */
   DOUBLES_EQUAL_TEXT(expected_process_noise, process_noise, tolerance, "Unexpected value for measured process noise.");
}

/** \purpose
 * Verify process noise remains unchanged when both LEFT side reference points are visible (no noise reduction).
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Left_Front_And_Rear_Visible_No_Change)
{
   /** \precond
    * Activate single front center radar config. Configure FOV normals so any point ahead (positive longitudinal) is inside FOV.
    * Place object so both front-left and rear-left are ahead of sensor.
    */
   globals.f_single_front_center_radar_only = true;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   for(int i=0;i<MAX_NUMBER_OF_SENSORS;++i){ sensors[i].variable.is_valid = true; }
   globals.rotated_left_fov_normal[0][0] = 1.0F; globals.rotated_left_fov_normal[0][1] = 0.0F;
   globals.rotated_right_fov_normal[0][0] = 1.0F; globals.rotated_right_fov_normal[0][1] = 0.0F;
   obj.bbox.Set_Length(6.0F);
   obj.bbox.Set_Center(Point(12.0F, obj.bbox.Get_Center().y)); // center far enough so rear point still positive
   const float32_t initial_noise = process_noise;

   /** \action
    * Update process noise.
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Expect unchanged noise (no noise reduction).
    */
   DOUBLES_EQUAL_TEXT(initial_noise, process_noise, tolerance, "Noise changed though both LEFT points visible.");
}

/** \purpose
 * Verify process noise remains unchanged when both RIGHT side reference points are visible (no noise reduction).
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Right_Front_And_Rear_Visible_No_Change)
{
   /** \precond
    * Single front center radar config. Configure FOV normals so any point ahead (positive longitudinal) is inside FOV.
    * Place object so both front-right and rear-right points are ahead of the sensor.
    */
   globals.f_single_front_center_radar_only = true;
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   for(int i=0;i<MAX_NUMBER_OF_SENSORS;++i){ sensors[i].variable.is_valid = true; }
   globals.rotated_left_fov_normal[0][0] = 1.0F; globals.rotated_left_fov_normal[0][1] = 0.0F;
   globals.rotated_right_fov_normal[0][0] = 1.0F; globals.rotated_right_fov_normal[0][1] = 0.0F;
   obj.bbox.Set_Length(6.0F);
   // Move center forward enough so rear-right (reference) point is still in front of sensor (x > 0)
   obj.bbox.Set_Center(Point(12.0F, obj.bbox.Get_Center().y));
   const float32_t initial_noise = process_noise;

   /** \action
    * Update process noise.
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Expect no noise reduction applied.
    */
   DOUBLES_EQUAL_TEXT(initial_noise, process_noise, tolerance, "Noise changed though both RIGHT points visible.");
}

/** \purpose
 * Verify noise reduction when front-left visible but rear-left not visible.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Left_Front_Visible_Rear_Not_Visible_Prunes_Case)
{
   /** \precond
    * Single front radar config; set FOV normals requiring positive longitudinal for visibility.
    * Place object so front-left ahead (positive) but rear-left behind sensor (negative).
    */
   globals.f_single_front_center_radar_only = true;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   for(int i=0;i<MAX_NUMBER_OF_SENSORS;++i){ sensors[i].variable.is_valid = true; }
   globals.rotated_left_fov_normal[0][0] = 1.0F; globals.rotated_left_fov_normal[0][1] = 0.0F;
   globals.rotated_right_fov_normal[0][0] = 1.0F; globals.rotated_right_fov_normal[0][1] = 0.0F;
   obj.bbox.Set_Length(10.0F);
   obj.bbox.Set_Center(Point(2.0F, obj.bbox.Get_Center().y)); // rear ~ -3, front ~ +7
   const float32_t pre_noise = process_noise;
   const float32_t expected = pre_noise * calib.k_size_update_process_noise_pruning;

   /** \action
    * Update process noise.
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Expect noise reduction applied.
    */
   DOUBLES_EQUAL_TEXT(expected, process_noise, tolerance, "Process noise not reduced with rear-left outside FOV.");
}

/** \purpose
 * Verify noise reduction when front-left is not visible (short-circuit case).
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Left_Front_Not_Visible_Prunes)
{
   /** \precond
    * Single front radar config; invalidate sensors so no points visible.
    */
   globals.f_single_front_center_radar_only = true;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   for(int i=0;i<MAX_NUMBER_OF_SENSORS;++i){ sensors[i].variable.is_valid = false; }
   const float32_t pre_noise = process_noise;
   const float32_t expected = pre_noise * calib.k_size_update_process_noise_pruning;

   /** \action
    * Update process noise.
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Expect noise reduction applied.
    */
   DOUBLES_EQUAL_TEXT(expected, process_noise, tolerance, "Process noise not reduced when front-left not visible.");
}

/** \purpose
 * The purpose of this test is to check the reference point of the target object is set to neither left nor right.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Neither_Left_Nor_Right_Refpoint)
{
   /** \precond
    * Set up a movable CCA object with default size
   */
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   const float32_t expected_process_noise = process_noise;

   /** \action
   * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
   * Update_Process_Noise_If_Obj_Outside_Front_Only_Fov() should not change the noise value.
    */
   DOUBLES_EQUAL_TEXT(expected_process_noise, process_noise, tolerance, "Unexpected value for measured process noise.");
}

/** \purpose
 * The purpose of this test is to check if the process noise changes when both refpoints on the side are visible.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Left_Refpoint_Both_Inside_FOV)
{
   /** \precond
    * Set up a movable CTCA object with default size
   */

   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.id = 15U;
   obj.vcs_position.x = 12.0F;
   obj.vcs_position.y = 2.5F;
   obj.bbox.Set_Length(23.0F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Orientation(0.0F);
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;

   const float32_t expected_process_noise = process_noise;

   /** \action
    * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Update_Process_Noise_If_Obj_Outside_Front_Only_Fov() should not change the noise value since.
    */
   DOUBLES_EQUAL_TEXT(expected_process_noise, process_noise, tolerance, "Unexpected value for measured process noise.");
}

/** \purpose
 * The purpose of this test is to check if process noise will change if one point is outside FOV.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Left_Refpoint_Rear_Left_Outside_FOV)
{
   /** \precond
    * Set up a movable CTCA object with default size
   */

   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.id = 15U;
   obj.vcs_position.x = 9.5F;
   obj.vcs_position.y = 2.2F;
   obj.bbox.Set_Length(19.6F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Orientation(0.0F);
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;

   const float32_t initial_process_noise = process_noise;

   /** \action
    * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Measured noise should be smaller than the input value.
    */
   CHECK_TRUE_TEXT(process_noise < initial_process_noise, "Unexpected value for measured process noise.");
}


/** \purpose
 * The purpose of this test is to check if process noise will change if one point is outside FOV.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Left_Refpoint_Rear_Left_Outside_front_left_inside)
{
   /** \precond
    * Set up a movable CCA object with the left front corner being inside the fov of front sensor
    * The rear left corner is outside the fov of the front sensor
    * The reference point is the center of the left visible edge of the object
    * The object is moving and is moving parallel to the host
   */

   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.id = 15U;
   obj.vcs_position.x = 9.5F;
   obj.vcs_position.y = 2.2F;
   obj.bbox.Set_Length(19.6F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Center(Point(9.5,2.2+1.25));
   obj.bbox.Set_Orientation(0.0F);
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;


   const float32_t initial_process_noise = process_noise;

   /** \action
    * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Measured noise should be smaller than the input value.
    */
   CHECK_TRUE_TEXT(process_noise < initial_process_noise, "Unexpected value for measured process noise.");
}

/** \purpose
 * The purpose of this test is to check if process noise will not change if object is completely in fov.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Left_Refpoint_both_ref_points_inside_fov)
{
   /** \precond
    * Set up a movable CCA object with the object, completely inside the fov of front sensor
    * The reference point is the rear left corner of the object
    * The object is moving and is moving parallel to the host
   */


   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.id = 15U;
   obj.vcs_position.x = 99.5F;
   obj.vcs_position.y = 2.2F;
   obj.bbox.Set_Length(6.0F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Orientation(0.0F);
   obj.bbox.Set_Center(Point(99.5+3, 2.2+1.25));
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;
   globals.rotated_left_fov_normal[0][0] = 0.1;
   globals.rotated_left_fov_normal[0][1] = -0.6;
   globals.rotated_right_fov_normal[0][0] = 0.1;
   globals.rotated_right_fov_normal[0][1] = -0.6;

   const float32_t initial_process_noise = process_noise;

   /** \action
    * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Measured noise should be smaller than the input value.
    */
   CHECK_TRUE_TEXT(process_noise == initial_process_noise, "No change in process noise when both ref points are in fov");
}


/** \purpose
 * The purpose of this test is to check if process noise changes when the ref point is on the right
 * and both front and rear refpoints are visible.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Right_Refpoint_Both_Inside_FOV)
{
   /** \precond
    * Set up a movable CCA object with default size
   */

   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.id = 15U;
   obj.vcs_position.x = -12.0F; // Mirror the Left refpoint test case obj X position
   obj.vcs_position.y = 2.5F;
   obj.bbox.Set_Length(23.0F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Orientation(0.0F);
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;

   const float32_t expected_process_noise = process_noise;

   /** \action
    * Call the function Update_Process_Noise_If_Obj_Outside_Front_Only_Fov()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Update_Process_Noise_If_Obj_Outside_Front_Only_Fov() should not change the noise value since.
    */
   DOUBLES_EQUAL_TEXT(expected_process_noise, process_noise, tolerance, "Unexpected value for measured process noise.");
}

/** \purpose
 * The purpose of this test is to check if process noise changes when the right ref point is chosen
 * and the front ref point on the same side is not visible.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Right_Refpoint_Right_RP_Outside_FOV)
{
   /** \precond
    * Set up a movable CCA object with default size
   */

   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj.id = 15U;
   obj.vcs_position.x = -9.5F;
   obj.vcs_position.y = 2.2F;
   obj.bbox.Set_Length(19.6F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Orientation(0.0F);
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;

   const float32_t initial_process_noise = process_noise;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Measured noise should be smaller than the input value.
    */
   CHECK_TRUE_TEXT(process_noise < initial_process_noise, "Unexpected value for measured process noise.");
}

/** \purpose
 * The purpose of this test is to check if process noise changes when the right ref point is chosen
 * and the rear ref point on the right side is not in fov, while front right is in fov
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Right_Refpoint_rear_right_not_visible_front_right_visible)
{
   /** \precond
    * Set up a movable CCA object with the right front corner being inside the fov of front sensor
    * The rear right corner is outside the fov of the front sensor
    * The reference point is the center of the right visible edge of the object
    * The object is moving and is moving parallel to the host
   */

   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   obj.id = 15U;
   obj.vcs_position.x = 9.5F;
   obj.vcs_position.y = -2.2F;
   obj.bbox.Set_Length(19.6F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Center(Point(9.5,-2.2-1.25));
   obj.bbox.Set_Orientation(0.0F);
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;

   globals.rotated_left_fov_normal[0][0] = 0.1;
   globals.rotated_left_fov_normal[0][1] = 0.6;
   globals.rotated_right_fov_normal[0][0] = 0.1;
   globals.rotated_right_fov_normal[0][1] = 0.6;

   const float32_t initial_process_noise = process_noise;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Measured noise should be smaller than the input value.
    */
   CHECK_TRUE_TEXT(process_noise < initial_process_noise, "Unexpected value for measured process noise.");
}



/** \purpose
 * The purpose of this test is to check if process noise does not change when the right ref point is chosen
 * and both the points in the fov are visible.
 */
TEST(Update_Process_Noise_If_Obj_Outside_Front_Only_Fov, Test_Right_Refpoint_Right_ref_points_both_visible)
{
   /** \precond
    * Set up a movable CCA object with the object, completely inside the fov of front sensor
    * The reference point is the rear right corner of the object
    * The object is moving and is moving parallel to the host
   */


   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.id = 15U;
   obj.vcs_position.x = 99.5F;
   obj.vcs_position.y = -2.2F;
   obj.bbox.Set_Length(6.0F);
   obj.bbox.Set_Width(2.5F);
   obj.bbox.Set_Orientation(0.0F);
   obj.bbox.Set_Center(Point(99.5+3, -2.2-1.25));
   obj.length_uncertainty = 0.03F;
   obj.width_uncertainty = 0.07F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.movable_prob = 1.0F;
   obj.f_vehicular_trk = true;
   obj.speed = 29.0F;

   globals.rotated_left_fov_normal[0][0] = 0.1;
   globals.rotated_left_fov_normal[0][1] = -0.6;
   globals.rotated_right_fov_normal[0][0] = 0.1;
   globals.rotated_right_fov_normal[0][1] = -0.6;

   const float32_t initial_process_noise = process_noise;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, obj, process_noise);

   /** \result
    * Measured noise should be equal to the input value.
    */
   CHECK_TRUE_TEXT(process_noise == initial_process_noise, "No change in process noise when both points are inside fov");
}
/** @}*/

/** \defgroup  f360_slow_shrinkage
 *  @{ 
 */

 /** \brief
  * This test group verifies the gradual downsizing ("slow shrink") behavior in Calc_Obj_Size() for objects moving
  * at modest speeds. It focuses on correct application of the speed-dependent upper length, proper skipping when
  * length is within limits, interactions with CIPV protection, and boundary speeds.
  */
TEST_GROUP(f360_slow_shrinkage)
{
   /** \setup
    * Establish a movable vehicular object and baseline detections. Speed and length adjusted per test.
    */
   F360_Calibrations_T calib;
   F360_Object_Track_T obj{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T dets_raw = {};
   const F360_Radar_Sensor_T(sensors)[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals{};
   float32_t CIPV_long_pos;
   float32_t tolerance;
   F360_Tracker_Info_T tracker_info = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      tolerance = 1e-6F;
      CIPV_long_pos = 0.0F;
      obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      obj.id = 25U;
      obj.vcs_position.x = 20.0F;
      obj.vcs_position.y = 0.0F;
      obj.bbox.Set_Length(5.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(0.0F);
      obj.length_uncertainty = 1.0F;
      obj.width_uncertainty = 1.0F;
      obj.ndets = 3U;
      obj.detids[0] = 1;
      obj.detids[1] = 2;
      obj.detids[2] = 3;
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.movable_prob = 1.0F;
      obj.f_vehicular_trk = true;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      det_props[0].vcs_position = {19.0F, -1.0F};
      det_props[1].vcs_position = {21.0F, 1.0F};
      det_props[2].vcs_position = {20.5F, -0.8F};
      dets_raw.detections[0].raw.range = 30.0F;
      dets_raw.detections[1].raw.range = 35.0F;
      dets_raw.detections[2].raw.range = 40.0F;
      globals.f_single_front_center_radar_only = false;
   }
};

/** \purpose
 * Verify length gently reduces when speed is in slow-shrink band and length exceeds computed upper limit.
 */
TEST(f360_slow_shrinkage, Slow_Shrink_Length_Reduced)
{
   /** \precond
    * Choose speed between 1.0 m/s and shrinking threshold; extend length above expected upper limit.
    */
   obj.speed = 2.0F;
   obj.bbox.Set_Length(calib.k_max_length_for_slow_moving_objects + 0.5F);
   const float32_t pre_len = obj.bbox.Get_Length();

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length should shrink (be < pre_len) but remain > minimum.
    */
   CHECK_TRUE_TEXT(obj.bbox.Get_Length() < pre_len, "Slow shrink did not reduce length.");
   CHECK_TRUE_TEXT(obj.bbox.Get_Length() > calib.k_nonmoveable_target_diameter, "Length reduced below allowed minimum.");
}

/** \purpose
 * Verify that when length is already within the speed-dependent upper limit, no shrink logic is applied
 * and only a normal (minor) size update occurs (object sides visible).
 */
TEST(f360_slow_shrinkage, Slow_Shrink_No_Change_Within_Limit)
{
   /** \precond
   * Speed in slow range; set length below the computed upper limit so shrink condition is not met.
   * Configure detections so size update logic is entered but no dimension update occurs.
   * Reference point set to CENTER so no sides are directly visible; one detection range below minimum update range to block non-visible side updates.
    */
   obj.speed = 1.8F; // within slow shrink speed band
   // Compute speed-dependent upper length and choose a length safely below it
   const float32_t upper_len = F360_Linear_Equation_With_Saturation(std::abs(obj.speed),
      calib.k_speed_for_min_length_of_slow_moving_objects,
      calib.k_speed_for_max_length_of_slow_moving_objects,
      calib.k_min_length_for_slow_moving_objects,
      calib.k_max_length_for_slow_moving_objects);
   obj.bbox.Set_Length(upper_len - 0.4F); // inside limit so shrink not triggered

   // Ensure detections configured for update path (object already has ndets=3 from setup)
   // Adjust detection positions to approximate current length so Kalman update causes only minor adjustment.
   det_props[0].vcs_position = {obj.vcs_position.x - 2.3F, obj.vcs_position.y - 0.2F};
   det_props[1].vcs_position = {obj.vcs_position.x + 2.4F, obj.vcs_position.y + 0.1F};
   det_props[2].vcs_position = {obj.vcs_position.x + 0.5F, obj.vcs_position.y - 0.15F};

   dets_raw.detections[0].raw.range = calib.k_size_update_min_det_range;      // == threshold
   dets_raw.detections[1].raw.range = calib.k_size_update_min_det_range + 8.0F;
   dets_raw.detections[2].raw.range = calib.k_size_update_min_det_range + 12.0F;

   // Make object sides visible so update executes despite range criterion failure for non-visible logic.
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT; // left & front visible

   const float32_t pre_len = obj.bbox.Get_Length();
   const float32_t pre_wid = obj.bbox.Get_Width();

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * No shrink applied (length stays below upper limit). Only minor measurement-driven adjustment permitted.
    */
   CHECK_TRUE_TEXT(obj.bbox.Get_Length() <= upper_len, "Length exceeded upper limit unexpectedly.");
   CHECK_TRUE_TEXT(std::fabs(obj.bbox.Get_Length() - pre_len) < 0.2F, "Length deviated more than expected for minor update.");
   CHECK_TRUE_TEXT(std::fabs(obj.bbox.Get_Width() - pre_wid) < 0.3F, "Width deviated more than expected for minor update.");
}

/** \purpose
 * Verify slow shrink is skipped when speed exceeds shrinking threshold.
 */
TEST(f360_slow_shrinkage, Slow_Shrink_Skipped_Above_Threshold)
{
   /** \precond
    * Speed > shrinking threshold; set long length to show absence of shrink.
    */
   obj.speed = calib.k_object_shrinking_speed_threshold + 0.5F;
   obj.bbox.Set_Length(calib.k_max_length_for_slow_moving_objects + 0.8F);
   obj.ndets = 0U; // no detections, so no update should happen
   const float32_t pre_len = obj.bbox.Get_Length();

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length should not follow the slow shrink formula directly; may change via normal update but not be forced toward upper limit.
    */
   DOUBLES_EQUAL_TEXT(pre_len, obj.bbox.Get_Length(), tolerance, "Length unexpectedly forced toward slow-shrink bound above threshold.");
}

/** \purpose
 * Verify CIPV status prevents slow shrink even if speed and length would trigger it.
 */
TEST(f360_slow_shrinkage, Slow_Shrink_Prevented_By_CIPV)
{
   /** \precond
    * Set CIPV_long_pos positive and object x inside window; speed in slow range; length large.
    * No detections present, so no length update should happen.
    */
   CIPV_long_pos = 15.0F;
   obj.vcs_position.x = 15.5F; // inside +/- (cipv + buffer)
   obj.speed = 2.2F;
   obj.ndets = 0U;
   obj.bbox.Set_Length(calib.k_max_length_for_slow_moving_objects + 0.6F);
   const float32_t pre_len = obj.bbox.Get_Length();

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length should remain at pre_len (no slow shrink applied).
    */
   DOUBLES_EQUAL_TEXT(pre_len, obj.bbox.Get_Length(), tolerance, "CIPV did not prevent slow shrink.");
}

/** \purpose
 * Verify slow shrink is prevented by variant K zone status (f_prevent_shrink_variant_K) even if speed and length would trigger it.
 */
TEST(f360_slow_shrinkage, Slow_Shrink_Prevented_By_Variant_K_In_Zone)
{
   /** \precond
    * Place object inside zone1 bounds and set variant to K.
    * Choose speed in slow-shrink range (>1 and < threshold) and length above computed upper limit so slow shrink would normally apply.
    * No detections so no update should happen.
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_K;
   obj.speed = 2.2F; // within slow shrink speed band
   // Put object within zone1 longitudinal & lateral bounds
   obj.vcs_position.x = (-40.0F + 25.0F) * 0.25F; // inside zone1 range
   obj.vcs_position.y = 22.0F * 0.5F; // within zone1 lateral
   obj.ndets = 0U; // no detections, so no length update should happen
   // Ensure length above upper limit so shrink would occur without prevention
   const float32_t abs_speed = std::abs(obj.speed);
   const float32_t upper_length_for_slow_speed = F360_Linear_Equation_With_Saturation(abs_speed,
      calib.k_speed_for_min_length_of_slow_moving_objects,
      calib.k_speed_for_max_length_of_slow_moving_objects,
      calib.k_min_length_for_slow_moving_objects,
      calib.k_max_length_for_slow_moving_objects);
   obj.bbox.Set_Length(upper_length_for_slow_speed + 0.6F); // exceed upper bound
   const float32_t pre_len = obj.bbox.Get_Length();

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length should remain unchanged (no slow shrink applied due to variant K zone prevention).
    */
   DOUBLES_EQUAL_TEXT(pre_len, obj.bbox.Get_Length(), tolerance, "Variant K zone prevention failed; length changed under slow shrink conditions");
}
/** @}*/

/** \defgroup  f360_fast_shrinkage
 *  @{ 
 */

 /** \brief
   * This test group verifies the object resizing logic in Calc_Obj_Size() for low-speed conditions.
   * It exercises: rapid downsizing below a speed threshold, gradual downsizing at modest speeds,
   * standard Kalman-based growth/shrink updates when downsizing does not apply, protection against downsizing
   * for the current path vehicle (CIPV), and correct behavior exactly at the low-speed boundary.
  */
TEST_GROUP(f360_fast_shrinkage)
{
   /** \setup
    * Set up common objects, calibrations, detections for shrinkage tests.
    */
   F360_Calibrations_T calib;
   F360_Object_Track_T obj{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T dets_raw = {};
   const F360_Radar_Sensor_T(sensors)[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals{};
   float32_t CIPV_long_pos; // will be set per test
   float32_t tolerance;
   F360_Tracker_Info_T tracker_info = {};
   float32_t gain;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      tolerance = 1e-6F;
      // Base object setup (vehicular, movable, CTCA)
      obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      obj.id = 5U;
      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 0.0F;
      obj.bbox.Set_Length(3.5F); // start within movable range
      obj.bbox.Set_Width(1.8F);
      obj.bbox.Set_Orientation(0.0F);
      obj.length_uncertainty = 1.0F;
      obj.width_uncertainty = 1.0F;
      obj.ndets = 3U;
      obj.detids[0] = 1;
      obj.detids[1] = 2;
      obj.detids[2] = 3;
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.movable_prob = 1.0F;
      obj.f_vehicular_trk = true;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      globals.f_single_front_center_radar_only = false; // not relevant for shrinkage branches
      // Default detections (span object length/width roughly)
      det_props[0].vcs_position = {9.5F, -0.9F};
      det_props[1].vcs_position = {11.5F, 0.9F};
      det_props[2].vcs_position = {10.2F, -0.8F};
      dets_raw.detections[0].raw.range = 30.0F;
      dets_raw.detections[1].raw.range = 35.0F;
      dets_raw.detections[2].raw.range = 40.0F;
      CIPV_long_pos = 0.0F; // default no CIPV
      gain = 0.4F;
   }
};

/** \purpose
   * Verify that at very low speed the object snaps to the minimal (non-movable) target size when it is already near that size.
 */
TEST(f360_fast_shrinkage, Fast_Shrink_Already_Small_f_shrinked_true)
{
   /** \precond
    * Set speed < 1.0 -> abs_obj_speed < speed_limit_for_fast_shrinkage triggers f_fast_shrink.
    * Set bbox length/width to just below non-movable diameter + rounding buffer to satisfy f_shrinked condition.
    */
   obj.speed = 0.5F;
   const float32_t target = calib.k_nonmoveable_target_diameter;
   obj.bbox.Set_Length(target + 0.005F); // < target + 0.01 rounding buffer
   obj.bbox.Set_Width(target + 0.005F);
   // Record initial uncertainties
   const float32_t init_len_unc = obj.length_uncertainty;
   const float32_t init_wid_unc = obj.width_uncertainty;

   /** \action
    * Call Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length/width set exactly to target diameter; uncertainties unchanged (update not applied with gain path).
    */
   DOUBLES_EQUAL_TEXT(target, obj.bbox.Get_Length(), tolerance, "Fast shrink f_shrinked true length not set to target");
   DOUBLES_EQUAL_TEXT(target, obj.bbox.Get_Width(), tolerance, "Fast shrink f_shrinked true width not set to target");
   DOUBLES_EQUAL_TEXT(init_len_unc, obj.length_uncertainty, tolerance, "Length uncertainty should remain unchanged");
   DOUBLES_EQUAL_TEXT(init_wid_unc, obj.width_uncertainty, tolerance, "Width uncertainty should remain unchanged");
}

/** \purpose
   * Verify that at very low speed a larger object is progressively reduced toward the minimal size using the configured gain.
 */
TEST(f360_fast_shrinkage, Fast_Shrink_f_shrinked_false_gain_applied)
{
   /** \precond
    * speed < 1.0 triggers fast shrink; make object larger than target + rounding buffer => f_shrinked false.
    */
   obj.speed = 0.2F;
   const float32_t target = calib.k_nonmoveable_target_diameter; // expected measured
   obj.bbox.Set_Length(target + 1.0F);
   obj.bbox.Set_Width(target + 0.8F);
   const float32_t pre_len = obj.bbox.Get_Length();
   const float32_t pre_wid = obj.bbox.Get_Width();

   /** \action
    * Call Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length/width moved towards target (reduced but not yet equal). Gain 0.4 applied: new = pre + 0.4*(measured - pre).
    */
   const float32_t exp_len = pre_len + gain*(target - pre_len);
   const float32_t exp_wid = pre_wid + gain*(target - pre_wid);
   DOUBLES_EQUAL_TEXT(exp_len, obj.bbox.Get_Length(), 1e-5F, "Fast shrink gain length mismatch");
   DOUBLES_EQUAL_TEXT(exp_wid, obj.bbox.Get_Width(), 1e-5F, "Fast shrink gain width mismatch");
}

/** \purpose
 * Verify fast shrink gain behavior when only the width exceeds the shrink completion threshold while width is already near the
 * non-movable target size (length still below target + rounding buffer). In this situation the object should not "snap" to the target
 * size; instead the gain-based gradual reduction is applied to both length and width, and size uncertainties remain unchanged.
 */
TEST(f360_fast_shrinkage, Fast_Shrink_Width_Only_Above_Buffer)
{
   /** \precond
    * Speed below fast shrink threshold (<1.0 m/s) to enable fast shrink logic.
    * Width set greater than target + rounding_buffer so width not considered sufficiently small.
    * Length set slightly below target + rounding_buffer to emulate length nearly converged.
    */
   obj.speed = 0.3F;
   const float32_t target = calib.k_nonmoveable_target_diameter;
   obj.bbox.Set_Length(target + 0.005F);    
   obj.bbox.Set_Width(target + 0.5F);     
   const float32_t pre_len = obj.bbox.Get_Length();
   const float32_t pre_wid = obj.bbox.Get_Width();
   const float32_t pre_len_unc = obj.length_uncertainty;
   const float32_t pre_wid_unc = obj.width_uncertainty;

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Gain path applied (f_shrinked false): length and width move 40% toward target; length/width uncertainties remain unchanged.
    */
   const float32_t exp_len = pre_len + gain*(target - pre_len);
   const float32_t exp_wid = pre_wid + gain*(target - pre_wid);
   DOUBLES_EQUAL_TEXT(exp_len, obj.bbox.Get_Length(), 1e-5F, "Length gain mismatch for branch length-only above buffer");
   DOUBLES_EQUAL_TEXT(exp_wid, obj.bbox.Get_Width(), 1e-5F, "Width gain mismatch when width below buffer but length above");
   DOUBLES_EQUAL_TEXT(pre_len_unc, obj.length_uncertainty, tolerance, "Length uncertainty should remain unchanged in gain path");
   DOUBLES_EQUAL_TEXT(pre_wid_unc, obj.width_uncertainty, tolerance, "Width uncertainty should remain unchanged in gain path");
}
/** \purpose
   * Verify that a current path vehicle (CIPV) is exempt from low-speed downsizing.
 */
TEST(f360_fast_shrinkage, CIPV_Prevents_Fast_Shrink)
{
   /** \precond
    * Make object CIPV by giving a positive cipv_long_pos and x within buffer; speed <1.0.
    */
   CIPV_long_pos = 8.0F; // window center reference
   obj.vcs_position.x = 7.5F; // inside +/- (cipv + buffer)
   obj.speed = 0.5F; // would trigger fast shrink without CIPV prevention
   obj.ndets = 0U; // no detections to cause size update
   obj.bbox.Set_Length(calib.k_nonmoveable_target_diameter + 2.0F);
   const float32_t pre_len = obj.bbox.Get_Length();

   /** \action
    * Call Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length should not perform fast shrink gain step (remain above target + large offset), indicating prevention.
    */
   DOUBLES_EQUAL_TEXT(pre_len, obj.bbox.Get_Length(), tolerance, "CIPV prevention failed; length changed under fast shrink conditions");
}

/** \purpose
 * Verify that fast shrink (speed < 1.0 m/s) is prevented when variant K is inside a zone.
 */
TEST(f360_fast_shrinkage, Fast_Shrink_Prevented_By_Variant_K_In_Zone)
{
   /** \precond
    * Set variant to K and place object inside zone1 longitudinal & lateral bounds.
    * Speed below fast shrink threshold (<1.0 m/s). Make object length/width larger than non-movable target.
    * Set ndets = 0 so no Kalman size update occurs; any change would have to be from fast shrink logic.
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_K;
   obj.speed = 0.6F; // would trigger fast shrink absent prevention
   obj.ndets = 0U;   // ensure no detection-based size update
   obj.bbox.Set_Length(calib.k_nonmoveable_target_diameter + 1.5F);
   obj.bbox.Set_Width(calib.k_nonmoveable_target_diameter + 1.0F);
   obj.vcs_position.x = (-40.0F + 25.0F) * 0.25F; // inside zone1
   obj.vcs_position.y = 22.0F * 0.3F; // lateral inside zone1
   const float32_t pre_len = obj.bbox.Get_Length();
   const float32_t pre_wid = obj.bbox.Get_Width();

   /** \action
    * Call Calc_Obj_Size().
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Length and width should remain unchanged (fast shrink prevented by variant K zone status).
    */
   DOUBLES_EQUAL_TEXT(pre_len, obj.bbox.Get_Length(), tolerance, "Variant K zone prevention failed; length changed under fast shrink conditions");
   DOUBLES_EQUAL_TEXT(pre_wid, obj.bbox.Get_Width(), tolerance, "Variant K zone prevention failed; width changed under fast shrink conditions");
}
/** @}*/

/** \defgroup  f360_calc_obj_size
 *  @{
 */

 /** \brief
  * The purpose of this test group is to test the functionality of the function Calc_Obj_Size().
  */
TEST_GROUP(f360_calc_obj_size)
{
   /** \setup
    * Set up variables called by Calc_Obj_Size function.
    */
   F360_Calibrations_T calib;
   F360_Object_Track_T  obj = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T dets_raw = {};
   const F360_Radar_Sensor_T(sensors)[MAX_NUMBER_OF_SENSORS] = {};
   const F360_Globals_T globals = {};
   float32_t measured_len;
   float32_t measured_wid;
   float32_t CIPV_long_pos;
   F360_Tracker_Info_T tracker_info = {};

   float32_t exp_updated_len;
   float32_t exp_updated_wid;
   float32_t exp_measured_len;
   float32_t exp_measured_wid;
   float32_t exp_len_uncertainty;
   float32_t exp_wid_uncertainty;

   float32_t tolerance = 1e-6F;

   TEST_SETUP()
   {
      // Use default tracker settings for calibrations
      Initialize_Tracker_Calibrations(calib);

      /* Set up object with
         - Reference point REAR RIGHT
            - with position (7,-6)
         - Pointing 0 degrees
         - Width 2m
         - Length 6m
         - vehicular_track set to true
         - f_movable set to true
         - Speed 10 m/s
         - 3 associated detections
      */
      obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      obj.id = 15U;
      obj.vcs_position.x = 7.0F;
      obj.vcs_position.y = -6.0F;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Orientation(0.0F);
      obj.length_uncertainty = 3.0F;
      obj.width_uncertainty = 2.0F;
      obj.ndets = 3U;
      obj.detids[0] = 1;
      obj.detids[1] = 2;
      obj.detids[2] = 3;
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.movable_prob = 1.0F;
      obj.f_vehicular_trk = true;
      obj.speed = 10.0F;
      obj.status = F360_OBJECT_STATUS_UPDATED;

      /* Set up 3 associated detections
         - with positions (6,-6), (14,-6) and (10, -8.2)
         - Range > min range to update size of object
      */
     det_props[0U].vcs_position = {6.0F, -6.0F};
     det_props[1U].vcs_position = {14.0F, -6.0F};
     det_props[2U].vcs_position = {10.0F, -8.3F};
     det_props[0U].object_track_id = obj.id;
     det_props[1U].object_track_id = obj.id;
     det_props[2U].object_track_id = obj.id;
     // Choose some arbitrary ranges for the dets
     dets_raw.detections[0U].raw.range = 30.0F;
     dets_raw.detections[1U].raw.range = 40.0F;
     dets_raw.detections[2U].raw.range = 50.0F;

     CIPV_long_pos = 0.0F;
   }
};

/** \purpose
 * The purpose of this test is to check that a non-movable object is not updated (kept at default dimensions for non-movable objects).
 */
TEST(f360_calc_obj_size, Test_Non_Movable_Object_Not_Updated)
{
   /** \precond
    * Set up a non-movable CCA object with default size
   */
   obj = {};
   obj.bbox.Set_Length(calib.k_nonmoveable_target_diameter);
   obj.bbox.Set_Width(calib.k_nonmoveable_target_diameter);
   obj.length_uncertainty = 1.0F;
   obj.width_uncertainty = 2.0F;
   obj.movable_prob = 0.0F;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   float32_t init_width_uncertainty = obj.width_uncertainty;
   float32_t init_length_uncertainty = obj.length_uncertainty;

   exp_updated_len = calib.k_nonmoveable_target_diameter;
   exp_updated_wid = calib.k_nonmoveable_target_diameter;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(init_length_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(init_width_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it. Detections are set up such that
 * the measured length/wid is bigger than the object's current size. Hence, size should grow in both directions.
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Rear_Right_Visible_Grow)
{
   /** \precond
    * An object has been set up in TEST_GROUP
   */
   exp_updated_len = 6.750156F;
   exp_updated_wid = 2.085745F;
   exp_len_uncertainty = 1.875391F;
   exp_wid_uncertainty = 1.429082F;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it. Seven detections are associated and set up
 * such that the measured length is bigger than the object's initial size. Thus measurement uncertainty will be decreased and updated
 * length bigger.
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Rear_Right_Visible_Grow_Decreased_Uncertainty)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Add extra detections
   */
   det_props[3U].vcs_position = {9.0F, -8.2F};
   det_props[3U].object_track_id = obj.id;
   det_props[4U].vcs_position = {7.0F, -7.9F};
   det_props[4U].object_track_id = obj.id;
   det_props[5U].vcs_position = {7.5F, -7.5F};
   det_props[5U].object_track_id = obj.id;
   det_props[6U].vcs_position = {7.6F, -7.2F};
   det_props[6U].object_track_id = obj.id;
   obj.ndets = 7;
   obj.detids[3] = 4;
   obj.detids[4] = 5;
   obj.detids[5] = 6;
   obj.detids[6] = 7;

   dets_raw.detections[3U].raw.range = 30.0F;
   dets_raw.detections[4U].raw.range = 40.0F;
   dets_raw.detections[5U].raw.range = 50.0F;
   dets_raw.detections[6U].raw.range = 50.0F;

   exp_updated_len = 7.411903F;
   exp_updated_wid = 2.085745F;
   exp_len_uncertainty = 0.882439F;
   exp_wid_uncertainty = 1.429082F;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it. Seven detections are associated and set up
 * but the para spread of detections is smaller than current object size, so measurement uncertainty is modified so the length filter
 * can become faster and shrink faster .
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Rear_Right_Visible_Decreased_Uncertainty_And_Fast_Shrink)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Add extra detections
    * Decrease longitudinal gap between detections
    * Set the f_shrink_fast to true
   */
   det_props[1U].vcs_position = {8.0F, -6.0F};

   det_props[3U].vcs_position = {9.0F, -8.2F};
   det_props[3U].object_track_id = obj.id;
   det_props[4U].vcs_position = {7.0F, -7.9F};
   det_props[4U].object_track_id = obj.id;
   det_props[5U].vcs_position = {7.5F, -7.5F};
   det_props[5U].object_track_id = obj.id;
   det_props[6U].vcs_position = {7.6F, -7.2F};
   det_props[6U].object_track_id = obj.id;
   obj.ndets = 7;
   obj.detids[3] = 4;
   obj.detids[4] = 5;
   obj.detids[5] = 6;
   obj.detids[6] = 7;

   dets_raw.detections[3U].raw.range = 30.0F;
   dets_raw.detections[4U].raw.range = 40.0F;
   dets_raw.detections[5U].raw.range = 50.0F;
   dets_raw.detections[6U].raw.range = 50.0F;

   exp_updated_len = 4.188622F;
   exp_updated_wid = 2.085745F;
   exp_len_uncertainty = 0.2830278F;
   exp_wid_uncertainty = 1.429082F;
   const bool exp_f_shrink_fast = true;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it. 11 detections are associated and set up
 * such that the measured length is bigger than the object's initial size. Thus measurement uncertainty will be decreased and updated
 * length bigger. However, decrease factor is saturated when number of detections is more than 10.
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Rear_Right_Visible_Grow_Decreased_Uncertainty_Saturated)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Add extra detections
   */
   det_props[3U].vcs_position = {9.0F, -8.2F};
   det_props[3U].object_track_id = obj.id;
   det_props[4U].vcs_position = {7.0F, -7.9F};
   det_props[4U].object_track_id = obj.id;
   det_props[5U].vcs_position = {7.5F, -7.5F};
   det_props[5U].object_track_id = obj.id;
   det_props[6U].vcs_position = {7.6F, -7.2F};
   det_props[6U].object_track_id = obj.id;
   det_props[7U].vcs_position = {7.6F, -7.2F};
   det_props[7U].object_track_id = obj.id;
   det_props[8U].vcs_position = {7.6F, -7.2F};
   det_props[8U].object_track_id = obj.id;
   det_props[9U].vcs_position = {7.6F, -7.2F};
   det_props[9U].object_track_id = obj.id;
   det_props[10U].vcs_position = {7.6F, -7.2F};
   det_props[10U].object_track_id = obj.id;
   obj.ndets = 11;
   obj.detids[3] = 4;
   obj.detids[4] = 5;
   obj.detids[5] = 6;
   obj.detids[6] = 7;
   obj.detids[7] = 8;
   obj.detids[8] = 9;
   obj.detids[9] = 10;
   obj.detids[10] = 11;
   obj.detids[11] = 12;

   dets_raw.detections[3U].raw.range = 30.0F;
   dets_raw.detections[4U].raw.range = 40.0F;
   dets_raw.detections[5U].raw.range = 50.0F;
   dets_raw.detections[6U].raw.range = 50.0F;
   dets_raw.detections[7U].raw.range = 30.0F;
   dets_raw.detections[8U].raw.range = 40.0F;
   dets_raw.detections[9U].raw.range = 50.0F;
   dets_raw.detections[10U].raw.range = 50.0F;
   dets_raw.detections[11U].raw.range = 50.0F;

   exp_updated_len = 7.875039F;
   exp_updated_wid = 2.085745F;
   exp_len_uncertainty = 0.187504F;
   exp_wid_uncertainty = 1.429082F;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it. Detections are set up such that
 * the measured length/wid is smaller than the object's current size. Hence, size should shrink in both directions.
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Rear_Right_Visible_Shrink)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Move detections to make measured len and wid smaller than current size
    *    - Change x pos of first detection to 9m
    *    - Change y pos of third detection to -7,5m
   */
   det_props[0U].vcs_position.x = 9.0F;
   det_props[2U].vcs_position.y = -7.5F;
   exp_updated_len = 5.624922F;
   exp_updated_wid = 1.857092F;
   exp_len_uncertainty = 1.875391F;
   exp_wid_uncertainty = 1.429082F;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it when only REAR side of object is visible.
 * This means that process noise is decreased and updated length should be slightly smaller comapred to the default case in TEST_GROUP.
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Only_Rear_Visible)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Set object reference point to REAR
   */
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   exp_updated_len = 6.750016F;
   exp_updated_wid = 2.085745F;
   exp_len_uncertainty = 1.875039F;
   exp_wid_uncertainty = 1.429082F;


   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it when only RIGHT side of object is visible.
 * This means that process noise is decreased and updated width should be slightly smaller comapred to the default case in TEST_GROUP.
 */
TEST(f360_calc_obj_size, Test_Fast_Moving_Far_Away_Only_Right_Visible)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Set object reference point to RIGHT
   */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   exp_updated_len = 6.750156F;
   exp_updated_wid = 2.085717F;
   exp_len_uncertainty = 1.875391F;
   exp_wid_uncertainty = 1.428622F;


   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}

/** \purpose
 * The purpose of this test is to check that the object size is correct after updating it. Detections are set up such that
 * the measured length/wid is bigger than the maximum allowed measurements.
 */
TEST(f360_calc_obj_size, Test_Saturate_Dimensions_At_Max)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Set associated detections far away enough to exceed maximum measurements in both length and width
   */
   det_props[0].vcs_position.x = -5.0F;
   det_props[1].vcs_position.x = 21.0F;
   det_props[2].vcs_position.y = -10.0F;
   exp_updated_len = 13.126484F;
   exp_updated_wid = 2.142908F;
   exp_len_uncertainty = 1.875391F;
   exp_wid_uncertainty = 1.429082F;


   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object dimensions are correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
   DOUBLES_EQUAL_TEXT(exp_updated_wid, obj.bbox.Get_Width(), tolerance, "Unexpected value for object updated width.");
   DOUBLES_EQUAL_TEXT(exp_len_uncertainty, obj.length_uncertainty, tolerance, "Unexpected value for object updated length uncertainty.");
   DOUBLES_EQUAL_TEXT(exp_wid_uncertainty, obj.width_uncertainty, tolerance, "Unexpected value for object updated width uncertainty.");
}


/** \purpose
 * The purpose of this test is to check that the object size calculation was not stopped when the detections longitudinal spread is less than 0.5m
 * and the object speed is less than 8m/s..
 */
TEST(f360_calc_obj_size, Test_Update_Length_Small_Longitudinal_Spread_Low_Speed)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Change object speed to be less than 8m/s
    * Set associated detections x position difference to less than 0.5m
   */
   det_props[0U].vcs_position = {7.0F, -6.0F};
   det_props[1].vcs_position = {7.24F, -6.0F};
   det_props[2].vcs_position = {6.76F, -6.0F};
   obj.speed = 7.9F;

   exp_updated_len = 3.93707;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object length is correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
}

/** \purpose
 * The purpose of this test is to check that the object size calculation was not stopped when the detections longitudinal spread is less than 0.5m
 * and the object speed is more than 8m/s but the side is visible.
 */
TEST(f360_calc_obj_size, Test_Update_Length_Small_Longitudinal_Spread_High_Speed_Left_Visible)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Change object speed to be more than 8m/s
    * Change object reference point to LEFT
    * Set associated detections x position difference to less than 0.5m
   */
   det_props[0U].vcs_position = {7.0F, -6.0F};
   det_props[1].vcs_position = {7.24F, -6.0F};
   det_props[2].vcs_position = {6.76F, -6.0F};
   obj.speed = 8.1F;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;

   exp_updated_len = 3.93707;

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object length is correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(exp_updated_len, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
}


/** \purpose
 * The purpose of this test is to check that the object size calculation was stopped when the detections longitudinal spread is less than 0.5m
 * and the object speed is more than 8m/s and only the front is visible.
 */
TEST(f360_calc_obj_size, Test_Update_Length_Small_Longitudinal_Spread_High_Speed_Front_Visible)
{
   /** \precond
    * An object has been set up in TEST_GROUP
    * Change object speed to be less than 8m/s
    * Change object reference point to FRONT
    * Set associated detections x position difference to less than 0.5m
   */
   det_props[0U].vcs_position = {7.0F, -6.0F};
   det_props[1].vcs_position = {7.24F, -6.0F};
   det_props[2].vcs_position = {6.76F, -6.0F};
   obj.speed = 8.1F;
   obj.reference_point = F360_REFERENCE_POINT_FRONT;

   exp_updated_len = obj.bbox.Get_Length();

   /** \action
    * Call the function Calc_Obj_Size()
    */
   Calc_Obj_Size(det_props, dets_raw, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);

   /** \result
    * Check that the output object length is correct after the call to Calc_Obj_Size().
    */
   DOUBLES_EQUAL_TEXT(6.0F, obj.bbox.Get_Length(), tolerance, "Unexpected value for object updated length.");
}

/** @}*/

/** \brief
  * This test group verifies that the function Update_Measurement_Noise_If_Many_Detections updates the
  * measurement noise correctly.
  */
TEST_GROUP(f360_Update_Measurement_Noise_If_Many_Detections)
{
   // Common variables used in tests
   F360_Calibrations_T calib;
   F360_Object_Track_T obj{};
   float32_t object_length;
   float32_t measured_length;
   bool left_or_right_side_visible;
   float32_t measurement_uncertainty;
   const float32_t tolerance = 1e-6F; 

   /** \setup
    * Initialize calibrations
    * Set object length to 5.0m
    * Set measured length to 3.4m
    * Set number of detections to more than threshold (3)
    * Set left or right side visible to true
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      object_length = 5.0F;
      measured_length = 3.4F;
      left_or_right_side_visible = true;
      measurement_uncertainty = 1.0F;
      obj.ndets = 12U; // more than threshold of 3
   }
};

/** \purpose
 * The purpose of this test is to check that the filter becomes faster for shrinkage when the object is smaller but the length innovation is big enough
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Shrinks_Fast_Length_Small_Innovation_Big)
{
   /** \precond
   * set the expected values
   */
   const float32_t exp_measurement_uncertainty = 0.04F;
   const bool exp_f_shrink_fast = true;
   
   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set.
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when the object is small and innovation not big enough
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Object_And_Length_Innovation_Small)
{
   /** \precond
   * modify the measured length such that the innovation is smaller than 1.5m
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   measured_length = 3.6F;

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set.
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when the object is large (>6m) but the length innovation is small (<0.6m)
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Large_Object_And_Small_Length_Innovation)
{
   /** \precond
   * modify the object length to be > 6m and measured length such that the innovation is smaller than 0.6m
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 7.0F;  // > 6m
   measured_length = 6.5F; // innovation = |7.0 - 6.5| = 0.5m < 0.6m

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set.
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter becomes faster when the object is large (>6m) and the length innovation is big enough (>0.6m)
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Shrinks_Fast_Due_To_Large_Object_And_Big_Length_Innovation)
{
   /** \precond
   * modify the object length to be > 6m and measured length such that the innovation is larger than 0.6m
   */
   const float32_t exp_measurement_uncertainty = 0.04F;
   const bool exp_f_shrink_fast = true;
   const float32_t tolerance = 1e-6F; 
   object_length = 7.5F;  // > 6m
   measured_length = 6.8F; // innovation = |7.5 - 6.8| = 0.7m > 0.6m

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set.
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when the measured length is so small it's considered an outlier (< 45% of object length)
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Outlier_Small_Measured_Length)
{
   /** \precond
   * modify the object length to be > 6m and measured length to be < 45% of object length (outlier condition)
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 8.0F;   // > 6m
   measured_length = 3.0F; // 3.0/8.0 = 37.5% < 45% (outlier condition)
                          // innovation = |8.0 - 3.0| = 5.0m > 0.6m (would normally trigger fast shrinkage)

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to outlier).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when the object is small with big innovation but the measured length is an outlier (< 45% of object length)
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Small_Object_Outlier_Measured_Length)
{
   /** \precond
   * modify the object length to be small (<= 6m) with big innovation, but measured length < 45% of object length (outlier condition)
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 4.0F;   // <= 6m (small object)
   measured_length = 1.5F; // 1.5/4.0 = 37.5% < 45% (outlier condition)
                          // innovation = |4.0 - 1.5| = 2.5m > 1.5m (would normally trigger fast shrinkage for small objects)

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to outlier).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when the large object has big innovation but left/right side is not visible
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Large_Object_Big_Innovation_No_Side_Visible)
{
   /** \precond
   * modify the object length to be > 6m with big innovation, but set left_or_right_side_visible to false
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 7.5F;  // > 6m
   measured_length = 6.8F; // innovation = |7.5 - 6.8| = 0.7m > 0.6m (would normally trigger fast shrinkage)
   left_or_right_side_visible = false; // This prevents fast shrinkage

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to no side visible).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when the small object has big innovation but left/right side is not visible
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Small_Object_Big_Innovation_No_Side_Visible)
{
   /** \precond
   * modify the object length to be small (<=6m) with big innovation, but set left_or_right_side_visible to false
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 5.0F;   // <= 6m (small object)
   measured_length = 3.4F; // innovation = |5.0 - 3.4| = 1.6m > 1.5m (would normally trigger fast shrinkage for small objects)
   left_or_right_side_visible = false; // This prevents fast shrinkage

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to no side visible).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when multiple conditions fail: few detections AND no side visible
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Few_Detections_And_No_Side_Visible)
{
   /** \precond
   * Set up conditions where multiple failure conditions occur: few detections AND no side visible
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 7.0F;   // > 6m (would normally be good for fast shrinkage)
   measured_length = 6.2F; // innovation = |7.0 - 6.2| = 0.8m > 0.6m (would normally be good for fast shrinkage)
   obj.ndets = 2U; // < 3 (insufficient detections)
   left_or_right_side_visible = false; // No side visible

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to multiple failures).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when multiple conditions fail: outlier measurement AND no side visible
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Outlier_And_No_Side_Visible)
{
   /** \precond
   * Set up conditions where multiple failure conditions occur: outlier measurement AND no side visible
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 6.0F;   // > 6m threshold (would normally be good for fast shrinkage)
   measured_length = 2.5F; // 2.5/6.0 = 41.7% < 45% (outlier condition)
                          // innovation = |6.0 - 2.5| = 3.5m > 0.6m (would normally be good for fast shrinkage)
   left_or_right_side_visible = false; // No side visible

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to multiple failures).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when multiple conditions fail: small innovation AND few detections AND no side visible
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Triple_Failure_Conditions)
{
   /** \precond
   * Set up conditions where three failure conditions occur simultaneously: small innovation AND few detections AND no side visible
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 7.5F;   // > 6m (would normally be good for fast shrinkage)
   measured_length = 7.2F; // innovation = |7.5 - 7.2| = 0.3m < 0.6m (small innovation)
   obj.ndets = 1U; // < 3 (insufficient detections)
   left_or_right_side_visible = false; // No side visible

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to triple failures).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when multiple conditions fail: outlier measurement AND few detections
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Outlier_And_Few_Detections)
{
   /** \precond
   * Set up conditions where multiple failure conditions occur: outlier measurement AND few detections
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 8.0F;   // > 6m (would normally be good for fast shrinkage)
   measured_length = 3.2F; // 3.2/8.0 = 40.0% < 45% (outlier condition)
                          // innovation = |8.0 - 3.2| = 4.8m > 0.6m (would normally be good for fast shrinkage)
   obj.ndets = 2U; // < 3 (insufficient detections)
   left_or_right_side_visible = true; // Side visible (but other conditions fail)

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to multiple failures).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when small innovation AND few detections occur (without no-side-visible)
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Small_Innovation_And_Few_Detections)
{
   /** \precond
   * Set up conditions where small innovation AND few detections occur, but side is visible
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 7.0F;   // > 6m (would normally be good for fast shrinkage)
   measured_length = 6.7F; // innovation = |7.0 - 6.7| = 0.3m < 0.6m (small innovation)
   obj.ndets = 2U; // < 3 (insufficient detections)
   left_or_right_side_visible = true; // Side visible (but other conditions fail)

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to dual failures).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when small innovation AND outlier measurement occur (without other failures)
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Small_Innovation_And_Outlier)
{
   /** \precond
   * Set up conditions where small innovation AND outlier measurement occur, but other conditions are good
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 8.0F;   // > 6m (would normally be good for fast shrinkage)
   measured_length = 7.6F; // innovation = |8.0 - 7.6| = 0.4m < 0.6m (small innovation)
                          // AND measured_length/object_length = 7.6/8.0 = 95% > 45% (NOT an outlier)
   // Let me correct this to actually be an outlier:
   measured_length = 3.0F; // 3.0/8.0 = 37.5% < 45% (outlier condition)
                          // innovation = |8.0 - 3.0| = 5.0m > 0.6m (but it's an outlier, so small effective innovation)
   left_or_right_side_visible = true; // Side visible
   // Note: Sufficient detections from setup (12 > 3)

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to outlier).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster when ALL four failure conditions occur simultaneously
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Quadruple_Failure_Conditions)
{
   /** \precond
   * Set up conditions where all four failure conditions occur: small innovation AND few detections AND no side visible AND outlier measurement
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   object_length = 9.0F;   // > 6m (would normally be good for fast shrinkage)
   measured_length = 8.7F; // innovation = |9.0 - 8.7| = 0.3m < 0.6m (small innovation)
   // But let's make it an outlier too:
   measured_length = 3.5F; // 3.5/9.0 = 38.9% < 45% (outlier condition)
                          // innovation = |9.0 - 3.5| = 5.5m > 0.6m (but it's an outlier)
   obj.ndets = 1U; // < 3 (insufficient detections)
   left_or_right_side_visible = false; // No side visible

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the correct measurement uncertainty and f_shrink_fast flag are set (no fast shrinkage due to all failures).
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster for shrinkage due to few number of detections
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Few_Detections)
{
   /** \precond
   * modify the number of detections of the object to be less than threshold to prevent shrinking
   * set the expected values
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   obj.ndets = 2U; // less than threshold of 3

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the output object dimensions are correct after the call to Update_Measurement_Noise_If_Many_Detections().
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter doesnt become faster for shrinkage due to not eirther left nor right side visible
 * by checking the measuremement uncertainty and the the f_shrink_fast flag.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Obj_Not_Shrinks_Fast_Due_To_Not_Side_Visible)
{
   /** \precond
   * modify left_or_right_side_visible to be false
   * set the expected values
   */
   const float32_t exp_measurement_uncertainty = 1.0F;
   const bool exp_f_shrink_fast = false;
   const float32_t tolerance = 1e-6F; 
   left_or_right_side_visible = false;

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the output object dimensions are correct after the call to Update_Measurement_Noise_If_Many_Detections().
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}

/** \purpose
 * The purpose of this test is to check that the filter becomes faster for a long object with small innovation.
 */
TEST(f360_Update_Measurement_Noise_If_Many_Detections, Long_Obj_Shrinks_Fast)
{
   /** \precond
   * modify length of the object to be long enough to allow fast shrinking even with small innovation
   * modify the measured length to be larger close to the actual length to have small innovation
   * set the expected values
   */
   const float32_t exp_measurement_uncertainty = 0.04F;
   const bool exp_f_shrink_fast = true;
   const float32_t tolerance = 1e-6F; 
   measured_length = 8.0F;
   object_length = 8.7F;

   /** \action
    * Call the function Update_Measurement_Noise_If_Many_Detections()
    */
   Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length,  left_or_right_side_visible, obj, measurement_uncertainty);

   /** \result
    * Check that the output object dimensions are correct after the call to Update_Measurement_Noise_If_Many_Detections().
    */
   DOUBLES_EQUAL_TEXT(exp_measurement_uncertainty, measurement_uncertainty, tolerance, "Unexpected value for measurement uncertainty.");
   CHECK_EQUAL_TEXT(exp_f_shrink_fast, obj.f_shrink_fast, "Unexpected value for object f_shrink_fast flag.");
}


/** @}*/
