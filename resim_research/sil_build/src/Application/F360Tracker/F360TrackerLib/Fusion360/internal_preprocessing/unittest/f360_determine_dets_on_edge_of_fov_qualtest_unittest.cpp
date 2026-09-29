/** \file
   Tests the function that determine if a detections is outside the sensor FOV but inside another sensors FOV.
*/

#include "f360_determine_dets_on_edge_of_fov.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include "f360_math.h"

/** \defgroup  f360_determine_dets_on_edge_of_fov
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Test group that sets up two sensors with located in origo. One aligned with the x-axis and
*  one aligned with the y-axis. Their MR FOV is 90 degree so they overlap with 45 degrees while
*  their LR FOV is 45 degrees so they align with their respective borders. The test group also
*  contains helper functions to add detections to the respective sensors.
**/
TEST_GROUP(f360_determine_dets_on_edge_of_fov)
{
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Radar_Sensor_T sensors [MAX_NUMBER_OF_SENSORS] = {};
   F360_Detection_Props_T detction_props [MAX_NUMBER_OF_DETECTIONS] = {};

   const uint8_t ACTIVE_SENSOR_ID = 1U;
   const float32_t LR_FOV = F360_PI / 4.0F;
   const float32_t MR_FOV = F360_PI_2;

   typedef struct
   {
      float32_t x;
      float32_t y;
   } Vector_T;

   /** \setup
   * Creates two sensors in origo. One aligned with the x-axis and the other one with the y-axis.
   * Both have 90 and 45 degree FOV for the MR and LR looks respectively.
   **/
   TEST_SETUP()
   {
      Add_Sensor(ACTIVE_SENSOR_ID, 0.0f, LR_FOV, MR_FOV);
      Add_Sensor(2U, F360_PI_2, LR_FOV, MR_FOV);
   }


    // Run action to simplify the test actions and reduce points of dependencies to the funciton under test.
    // It simly run one iteration of the function.

   void Run_Action()
   {
      for (unsigned int det_idx = 0; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
      {
         const rspp_variant_A::RSPP_Detection_T &current_detection = raw_detection_list.detections[det_idx];
         F360_Detection_Props_T &current_detection_prop = detction_props[det_idx];
         Mark_Detection_On_Edge_Of_FOV(current_detection, sensors, current_detection_prop);
      }
   }

   // Helper function to add new detections with a specified range and azimuth to a sensor.
   F360_Detection_Props_T * Add_Detection(uint8_t sensor_id, float32_t range, float32_t azimuth)
   {
      uint8_t detection_idx = raw_detection_list.number_of_valid_detections++;
      rspp_variant_A::RSPP_Detection_T * raw_det = &raw_detection_list.detections[detection_idx];
      raw_det->raw.sensor_id = sensor_id;
      raw_det->raw.azimuth = azimuth;
      raw_det->processed.vcs_az = azimuth + sensors[sensor_id - 1].constant.mounting_position.vcs_boresight_azimuth_angle;

      Vector_T position = Calculate_Detection_Position(range, azimuth);

      F360_Detection_Props_T * det_props = &detction_props[detection_idx];
      det_props->vcs_position.x = position.x;
      det_props->vcs_position.y = position.y;

      return det_props;
   }
   // Helper function to add new sensors with specified settings.

   void Add_Sensor(uint8_t sensor_id, float32_t boresight, float32_t lr_fov, float32_t mr_fov)
   {
      F360_Radar_Sensor_T* sensor = &sensors[sensor_id - 1U];

      // Set sensor properties so that id match and sensor is valid */
      sensor->constant.id = sensor_id;
      sensor->variable.is_valid = true;

      // Set mounting properties with position in origo and specified boresight
      F360_Sensor_Mounting_Position_T mount_pos = {};
      mount_pos.vcs_boresight_azimuth_angle = boresight;
      sensor->constant.mounting_position = mount_pos;

      // Detection properties of sensor
      sensor->constant.range_limits[F360_DET_LOOK_ID_0] = 100.0F;
      sensor->constant.range_limits[F360_DET_LOOK_ID_1] = 100.0F;
      sensor->constant.range_limits[F360_DET_LOOK_ID_2] = 50.0F;
      sensor->constant.range_limits[F360_DET_LOOK_ID_3] = 50.0F;

      // Set up FOV limits and calculate normals for the FOV edges
      sensor->refined.interior_fov[F360_DET_LOOK_ID_0] = boresight - lr_fov;
      sensor->refined.interior_fov[F360_DET_LOOK_ID_1] = boresight + lr_fov;
      sensor->refined.interior_fov[F360_DET_LOOK_ID_2] = boresight - mr_fov;
      sensor->refined.interior_fov[F360_DET_LOOK_ID_3] = boresight + mr_fov;

      Vector_T normal = Calculate_Normal({0, 0}, sensor->refined.interior_fov[F360_DET_LOOK_ID_0], false);
      sensor->refined.left_fov_normal[F360_DET_LOOK_ID_0] = normal.x;
      sensor->refined.left_fov_normal[F360_DET_LOOK_ID_1] = normal.y;

      normal = Calculate_Normal({0, 0}, sensor->refined.interior_fov[F360_DET_LOOK_ID_1], true);
      sensor->refined.right_fov_normal[F360_DET_LOOK_ID_0] = normal.x;
      sensor->refined.right_fov_normal[F360_DET_LOOK_ID_1] = normal.y;

      normal = Calculate_Normal({0, 0}, sensor->refined.interior_fov[F360_DET_LOOK_ID_2], false);
      sensor->refined.left_fov_normal[F360_DET_LOOK_ID_2] = normal.x;
      sensor->refined.left_fov_normal[F360_DET_LOOK_ID_3] = normal.y;

      normal = Calculate_Normal({0, 0}, sensor->refined.interior_fov[F360_DET_LOOK_ID_3], true);
      sensor->refined.right_fov_normal[F360_DET_LOOK_ID_2] = normal.x;
      sensor->refined.right_fov_normal[F360_DET_LOOK_ID_3] = normal.y;
   }

    // Helper function to change the look index of a sensor.
   void Set_Sensor_Look_Id(uint8_t sensor_id, F360_Det_Look_ID_T look_id)
   {
      sensors[sensor_id - 1U].variable.look_id = look_id;
   }

    // Helper function to calculate a normal of line with starting point and an angle. Either clockwise or
    // anti clockwise.
   Vector_T Calculate_Normal(Vector_T start_of_line, float32_t angle_of_line, bool clockwise)
   {
      Vector_T line_vector = Rotate_Vector({1.0F, 0.0F}, angle_of_line);
      line_vector.x += start_of_line.x;
      line_vector.y += start_of_line.y;

      Vector_T normal_vector;
      if (clockwise)
      {
         normal_vector.x = line_vector.y;
         normal_vector.y = -line_vector.x;
      }
      else
      {
         normal_vector.x = -line_vector.y;
         normal_vector.y = line_vector.x;
      }

      return normal_vector;
   }

   // Helper function that rotates a point with a defined angle clockwise when using point in SAE
   // frame and counter clockwise if ISO frame is used.
   Vector_T Rotate_Vector(Vector_T vector, float32_t angle) {
      Vector_T rotated_vector;
      rotated_vector.x = vector.x*F360_Cosf(angle) - vector.y*F360_Sinf(angle);
      rotated_vector.y = vector.x*F360_Sinf(angle) + vector.y*F360_Cosf(angle);

      return rotated_vector;
   }

   // Helper funciton to calculate x and y position of an detection defined by range and azimuth
   Vector_T Calculate_Detection_Position(float32_t range, float32_t azimuth)
   {
      Vector_T position;
      position.x = range * F360_Cosf(azimuth);
      position.y = range * F360_Sinf(azimuth);

      return position;
   }
};


/**
*\purpose  Test different detection position inside/outside and border of FOV for the two sensors if the sensor
*\purpose  with detections have look id corresponding to MR look.
*\req    CPR-3856
*/
TEST(f360_determine_dets_on_edge_of_fov, mr_detections)
{
   /** \precond
   * Set the look id to MR on the sensor that should measure all detections together with a bunch of detections that will
   * test inside, outside and border conditions.
   **/
   Set_Sensor_Look_Id(ACTIVE_SENSOR_ID, F360_DET_LOOK_ID_2);

   // Set up different kinds of detections that will test all sides of the conditions
   F360_Detection_Props_T * det_inside_inside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, MR_FOV - F360_DEG2RAD(1.0F));
   F360_Detection_Props_T * det_inside_border_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, 0.0F);
   F360_Detection_Props_T * det_inside_outside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, -F360_DEG2RAD(1.0F));

   F360_Detection_Props_T * det_border_inside_mr = Add_Detection(ACTIVE_SENSOR_ID, 0.0F, MR_FOV);
   F360_Detection_Props_T * det_border_outside_mr = Add_Detection(ACTIVE_SENSOR_ID, 0.0F, -MR_FOV);

   F360_Detection_Props_T * det_outside_inside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, MR_FOV + F360_DEG2RAD(1.0F));
   F360_Detection_Props_T * det_outside_border_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, F360_PI_2 + MR_FOV);
   F360_Detection_Props_T * det_outside_outside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, -F360_PI * 3.0F / 4.0F);

   /** \action
   * Run the action defined by the test group.
   **/
   Run_Action();

   /** \result
   * Check that the flag of all detetions have correct value. Only the detection outside the owner sensors FOV
   * but inside the other sensors MR FOV should be true. All other includeing border cases should be false.
   **/
   CHECK_FALSE(det_inside_inside_mr->f_FOV_edge);
   CHECK_FALSE(det_inside_border_mr->f_FOV_edge);
   CHECK_FALSE(det_inside_outside_mr->f_FOV_edge);

   CHECK_FALSE(det_border_inside_mr->f_FOV_edge);
   CHECK_FALSE(det_border_outside_mr->f_FOV_edge);

   CHECK(det_outside_inside_mr->f_FOV_edge);
   CHECK_FALSE(det_outside_border_mr->f_FOV_edge);
   CHECK_FALSE(det_outside_outside_mr->f_FOV_edge);
}

/**
*\purpose  Test different detection position inside/outside and border of FOV for the two sensors if the sensor
*\purpose  with detections have look id corresponding to LR look.
*\req    CPR-3856
*/
TEST(f360_determine_dets_on_edge_of_fov, lr_detections)
{
   /** \precond
   * Set the look id to LR on the sensor that should measure all detections together with a bunch of detections that will
   * test inside, outside and border conditions.
   **/
   Set_Sensor_Look_Id(ACTIVE_SENSOR_ID, F360_DET_LOOK_ID_0);

   // Set up different kinds of detections that will test all sides of the conditions
   F360_Detection_Props_T * det_inside_inside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, LR_FOV - F360_DEG2RAD(1.0F));
   F360_Detection_Props_T * det_inside_border_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, 0.0F);
   F360_Detection_Props_T * det_inside_outside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, -F360_DEG2RAD(1.0F));

   F360_Detection_Props_T * det_border_border_mr = Add_Detection(ACTIVE_SENSOR_ID, 0.0F, LR_FOV);
   F360_Detection_Props_T * det_border_outside_mr = Add_Detection(ACTIVE_SENSOR_ID, 0.0F, -LR_FOV);

   F360_Detection_Props_T * det_outside_inside_lrmr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, LR_FOV + F360_DEG2RAD(1.0F));
   F360_Detection_Props_T * det_outside_border_lr_inside_mr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, F360_PI_2 + LR_FOV);
   F360_Detection_Props_T * det_outside_outside_lrmr = Add_Detection(ACTIVE_SENSOR_ID, 1.0F, -F360_PI * 3.0F / 4.0F);

   /** \action
   * Run the action defined by the test group.
   **/
   Run_Action();

   /** \result
   * Check that the flag of all detections have correct value. The detection outside owner FOV and inside the
   * LR or MR FOV of the other sensor should be true.
   **/
   CHECK_FALSE(det_inside_inside_mr->f_FOV_edge);
   CHECK_FALSE(det_inside_border_mr->f_FOV_edge);
   CHECK_FALSE(det_inside_outside_mr->f_FOV_edge);

   CHECK_FALSE(det_border_border_mr->f_FOV_edge);
   CHECK_FALSE(det_border_outside_mr->f_FOV_edge);

   CHECK(det_outside_inside_lrmr->f_FOV_edge);
   CHECK(det_outside_border_lr_inside_mr->f_FOV_edge);
   CHECK_FALSE(det_outside_outside_lrmr->f_FOV_edge);
}

/**
*\purpose  Test that the range limits of the look type its used to exclude matches correctly.
*\req    CPR-3856
*/
TEST(f360_determine_dets_on_edge_of_fov, range_limits)
{
   /** \precond
   * Set the look id of the sensor to LR so that only the more narrow FOV is valid. Define two angles of
   * detections that will be outside the ownser FOV but inside either both look types or the MR look type
   * of the other sensor. Then setup detections with these two angles that also test inside, outside and
   * border of that look types range limit.
   **/
   Set_Sensor_Look_Id(ACTIVE_SENSOR_ID, F360_DET_LOOK_ID_0);
   const float32_t AZIMUTH_IN_MR_LR_FOV = LR_FOV + F360_DEG2RAD(1.0F);
   const float32_t AZIMUTH_IN_MR_FOV = F360_PI_2 + LR_FOV + F360_DEG2RAD(1.0F);

   F360_Detection_Props_T * det_lr_high_range = Add_Detection(ACTIVE_SENSOR_ID, 101.0F, AZIMUTH_IN_MR_LR_FOV);
   F360_Detection_Props_T * det_lr_border_range = Add_Detection(ACTIVE_SENSOR_ID, 100.0F, AZIMUTH_IN_MR_LR_FOV);
   F360_Detection_Props_T * det_lr_low_range = Add_Detection(ACTIVE_SENSOR_ID, 99.0F, AZIMUTH_IN_MR_LR_FOV);

   F360_Detection_Props_T * det_mr_high_range = Add_Detection(ACTIVE_SENSOR_ID, 51.0F, AZIMUTH_IN_MR_FOV);
   F360_Detection_Props_T * det_mr_border_range = Add_Detection(ACTIVE_SENSOR_ID, 50.0F, AZIMUTH_IN_MR_FOV);
   F360_Detection_Props_T * det_mr_low_range = Add_Detection(ACTIVE_SENSOR_ID, 49.0F, AZIMUTH_IN_MR_FOV);


   /** \action
   * Run the action defined by the test group.
   **/
   Run_Action();

   /** \result
   * Check that the correct flag have been set. Only reanges inside or on the border of the range limits
   * should be true.
   **/
   CHECK_FALSE(det_lr_high_range->f_FOV_edge);
   CHECK(det_lr_border_range->f_FOV_edge);
   CHECK(det_lr_low_range->f_FOV_edge);

   CHECK_FALSE(det_mr_high_range->f_FOV_edge);
   CHECK(det_mr_border_range->f_FOV_edge);
   CHECK(det_mr_low_range->f_FOV_edge);
}

/** @}*/
