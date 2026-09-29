/**
 * @file
 * Unit tests for functionality implemented in f360_calculate_obstacle_prob.cpp.
 */

#include "f360_calculate_obstacle_prob.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/**
 * @defgroup f360_calculate_obstacle_prob
 * @{
 */

/**
 * @brief
 * Tests are designed to check if temp_factor is calculated as expected
 * It value is calculated using object range that is calculated from object bounding box center
 *
 */
TEST_GROUP(f360_Calculate_Properties_For_Obstacle_Prob)
{
   // Common fixtures for the group
   /**
    * @setup
    * Prepare detections, sensors, and an object under test.
    * - All detections share: sensor_id=1, confid_azimuth=0, confid_elevation=0
    * - Detection z-positions (vcs_position_z) are taken from det_Z_pos[]
    * - The object by default has 10 detections (ids 1..10)
    * - Sensors list is empty (default-initialized)
    */
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T obj{};
   float32_t det_Z_pos[10] = { 1.7F, 1.1F, -2.1F, -3.0F, 3.0F, -3.0F, 3.0F, -3.0F, 3.0F, -3.0F };
   float32_t fourDetectionMean = 0.0F;
   float32_t tenDetectionMean = 0.0F;

   TEST_SETUP()
   {
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.ndets = 10;
      for (uint8_t idx = 0; idx < obj.ndets; ++idx) {
         raw_detect_list.detections[idx].raw.sensor_id = 1;
         raw_detect_list.detections[idx].raw.confid_azimuth = 0;
         raw_detect_list.detections[idx].raw.confid_elevation = 0;
         raw_detect_list.detections[idx].processed.vcs_position_z = det_Z_pos[idx];
         obj.detids[idx] = static_cast<uint32_t>(idx + 1U);
         tenDetectionMean += det_Z_pos[idx]/obj.ndets;
      }

      for(uint8_t idx = 0; idx < 4; ++idx){
         fourDetectionMean += det_Z_pos[idx]/4.0F;
      }
      Initialize_Tracker_Calibrations(calib);
   }

   /**
    * @teardown
    * Perform any required cleanup after each test (none needed here).
    */
   TEST_TEARDOWN()
   {
      // e.g., mock().clear();
   }
};

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given stationary object with low detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, FarStationaryObjectWithLowDetectionCount)
{
   /** \precond
   Object has:
   bounding box set to 50.1 0
   one associated detection
   and is stationary
   */
   obj.ndets = 1;
   obj.bbox.Set_Center({50.1F, 0.0F});
   obj.movable_prob = 0.0F;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

    /** \result
   we expect that bbox_center_otg_altitude will be inverted det_Z_pos from our detection
   and bbox_height to be result of F360_Low_Pass_Filter_First_Order for
   new_input 0.5
   prev_filt 0
   and filter_coef 0.2
   result is 0.1
   */
   DOUBLES_EQUAL(-det_Z_pos[0], obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(0.1F, obj.bbox_height, F360_EPSILON);
}

TEST(f360_Calculate_Properties_For_Obstacle_Prob, FurtherStationaryObjectWithLowDetectionCount)
{
   /** \precond
   Object has:
   bounding box set to 49.9 0
   one associated detection
   and is stationary
   */
   obj.ndets = 1;
   obj.bbox.Set_Center({49.9F, 0.0F});
   obj.movable_prob = 0.0F;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

    /** \result
   we expect that bbox_center_otg_altitude will be inverted det_Z_pos from our detection
   and bbox_height to be result of F360_Low_Pass_Filter_First_Order for
   new_input 0.5
   prev_filt 0
   and filter_coef 0.2
   result is 0.1
   */
   DOUBLES_EQUAL(-det_Z_pos[0], obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(0.1F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given object with no detection.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, NoDetections)
{
   /** \precond
   Object has:
   no associated detection
   */
   obj.ndets = 0;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    * we expect that bbox_height will not change
    */
   DOUBLES_EQUAL(0.0F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given far moving object with low detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, MovingObjectWithFewDetectionCountAndFurtherCenter)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 100.1 0
   */
   obj.ndets = 4;
   obj.bbox.Set_Center({100.1F, 0.0F});

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
   we expect that bbox_center_otg_altitude will be inverted det_Z_pos from our detection
   and bbox_height to be saturated between 1 and 3 witch is 2
   */
   DOUBLES_EQUAL(-fourDetectionMean, obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(2.0F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given far moving object with low detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, MovingObjectWithFewDetectionCountAndFarCenter)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 99.9 0
   */
   obj.ndets = 4;
   obj.bbox.Set_Center({99.9F, 0.0F});

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
   we expect that bbox_center_otg_altitude will be inverted det_Z_pos from our detection
   and bbox_height to be saturated between 1 and 3 witch is 2
   */
   DOUBLES_EQUAL(-fourDetectionMean, obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(2.0F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given moving object with low detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, MovingObjectWithFewDetectionCount)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 50.1 50.1
   */
   obj.ndets = 4;
   obj.bbox.Set_Center({50.1F, 50.1F});

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
   we expect that bbox_center_otg_altitude will be inverted det_Z_pos from our detection
   and bbox_height to be saturated between 1 and 3 witch is 2
   */
   DOUBLES_EQUAL(-fourDetectionMean, obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(2.0F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given young stationary object with ten detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, StationaryYoungObjectWithTenDetectionCount)
{
   /** \precond
   Object has:
   ten associated detection
   bounding box set to 10 50
   one update since init
   and is stationary
   */
   obj.movable_prob = 0.0F;
   obj.time_since_initialization = 0.0F;
   obj.num_updates_since_init = 1U;
   obj.bbox.Set_Center({10.0F, 50.0F});

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    we expect that bbox_center_otg_altitude will be inverted mean of det_Z_pos from our detections
    and bbox_height to be 0.4 calculated as a result of object having more than 6 dets in close sigma
    and is in range shorter than 70 so it overrides the F360_Saturate with F360_Low_Pass_Filter_First_Order
   */
   DOUBLES_EQUAL(-tenDetectionMean, obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(0.4F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given old moving object with ten detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, MovingOldObjectWithTenDetectionCount)
{
   /** \precond
   Object has:
   ten associated detection
   bounding box set to 10 50
   time since init 10
   and is moving
   */
   obj.movable_prob = 1.0F;
   obj.time_since_initialization = 10.0F;
   obj.bbox.Set_Center({10.0F, 50.0F});

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    we expect that bbox_center_otg_altitude will be 0.023 as a result from :
    F360_Low_Pass_Filter_First_Order(measured_otg_altitude, obj.bbox_center_otg_altitude, total_gain)
    where measured_otg_altitude is inverted mean of det_Z_pos from our detections
    obj.bbox_center_otg_altitude is 0
    and bbox_height to be 0.3 calculated as a result of object having more than 6 dets in close sigma
    and is in range shorter than 70 so it overrides the F360_Saturate with F360_Low_Pass_Filter_First_Order
   */
   DOUBLES_EQUAL(0.023F, obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(0.3F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given old moving object with two detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, MovingOldObjectWithTwoDetectionCount)
{
   /** \precond
   Object has:
   two associated detection
   bounding box set to 10 50
   bbox_center_otg_altitude  is 10
   time since init 10
   and is moving
   */
   obj.ndets = 2;
   obj.movable_prob = 1.0F;
   obj.time_since_initialization = 10.0F;
   obj.bbox.Set_Center({10.0F, 50.0F});
   obj.bbox_center_otg_altitude = 10.0F;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    we expect that bbox_center_otg_altitude will be 8.86 as a result from :
    F360_Low_Pass_Filter_First_Order(measured_otg_altitude, obj.bbox_center_otg_altitude, total_gain)
    where measured_otg_altitude is inverted mean of det_Z_pos from our detections
    and bbox_height to be 0.1 as a result of object having more than 6 dets in close sigma
    and is in range shorter than 70 so it overrides the F360_Saturate with F360_Low_Pass_Filter_First_Order
   */
   DOUBLES_EQUAL(8.86F, obj.bbox_center_otg_altitude, F360_EPSILON);
   DOUBLES_EQUAL(0.1F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given far object with low detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, FarAwayObject)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 100.1 0
   sensor mounting_location is set to F360_MOUNTING_LOCATION_CENTER_FORWARD
   */
   obj.bbox.Set_Center({100.1F, 0.0F});
   obj.ndets = 4;

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    Will result with bbox_height to be 2.0 as a result from :
    F360_Saturate(bbox_height, height_limit_min, height_limit_max)
    for 0, 1, 3 is 2
   */
   DOUBLES_EQUAL(2.0F, obj.bbox_height, F360_EPSILON);
}

TEST(f360_Calculate_Properties_For_Obstacle_Prob, AwayObject)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 99.9 0
   sensor mounting_location is set to F360_MOUNTING_LOCATION_CENTER_FORWARD
   */
   obj.bbox.Set_Center({99.9F, 0.0F});
   obj.ndets = 4;

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    Will result with bbox_height to be 2.0 as a result from :
    F360_Saturate(bbox_height, height_limit_min, height_limit_max)
    for 0, 1, 3 is 2
   */
   DOUBLES_EQUAL(2.0F, obj.bbox_height, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Properties_For_Obstacle_Prob() works as intended when given old stationary object with low detection count.
*/
TEST(f360_Calculate_Properties_For_Obstacle_Prob, StationaryOldObject)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 49.9 0
   time since initialization is 100
   and is stationary
   sensor mounting_location is set to F360_MOUNTING_LOCATION_CENTER_FORWARD
   */
   obj.bbox.Set_Center({49.9F, 0.0F});
   obj.time_since_initialization = 100;
   obj.movable_prob = 0;
   obj.ndets = 4;

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    Will result with bbox_height to be 0.4
    as a result from F360_Low_Pass_Filter_First_Order(measured_height, obj.bbox_height, total_gain);
   */
   DOUBLES_EQUAL(0.4F, obj.bbox_height, F360_EPSILON);
}

TEST(f360_Calculate_Properties_For_Obstacle_Prob, StationaryOldObjectHFurther)
{
   /** \precond
   Object has:
   four associated detection
   bounding box set to 50.1 0
   time since initialization is 100
   and is stationary
   sensor mounting_location is set to F360_MOUNTING_LOCATION_CENTER_FORWARD
   */
   obj.bbox.Set_Center({50.1F, 0.0F});
   obj.time_since_initialization = 100;
   obj.movable_prob = 0;
   obj.ndets = 4;

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;

   /** \action
   Call Calculate_Properties_For_Obstacle_Prob()
   */
   Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);

   /** \result
    Will result with bbox_height to be 0.4
    as a result from F360_Low_Pass_Filter_First_Order(measured_height, obj.bbox_height, total_gain);
   */
   DOUBLES_EQUAL(0.4F, obj.bbox_height, F360_EPSILON);
}

/**
 * @brief
 * Tests for Calculate_Instantaneous_Obstacle_Prob.
 */
TEST_GROUP(f360_Calculate_Instantaneous_Obstacle_Prob)
{
   // Common fixtures for the group
   /**
    * @setup
    * Prepare an object under test.
    * - The object by default has 10 detections
    * - Object Status is Updated
    * - Object is moving
    * - Object bbox height is 1
    */

   F360_Object_Track_T obj{};

   TEST_SETUP()
   {
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.ndets = 10;
      for (uint8_t i = 0; i < obj.ndets; ++i) {
         obj.detids[i] = static_cast<uint32_t>(i + 1U);
      }
      obj.movable_prob = 1.0F;
      obj.bbox_height  = 1.0F;
   }

   TEST_TEARDOWN()
   {
      // e.g., mock().clear();
   }
};

 /** \purpose
This aims to verify that Calculate_Instantaneous_Obstacle_Prob() works as intended when given object with otg altitude 1.
*/
TEST(f360_Calculate_Instantaneous_Obstacle_Prob, ObjectWithCenterAltitude_1)
{
    /** \precond
   Object has:
   bbox_center_otg_altitude 1
    bbox_height 1
   */
   obj.bbox_center_otg_altitude = 1.0F;
   obj.bbox_height  = 0.0F;

   /** \action
   Call Calculate_Instantaneous_Obstacle_Prob()
   */
   float32_t result = Calculate_Instantaneous_Obstacle_Prob(obj, 5.0F);

   /** \result
   we expect instantaneous obstacle probability to be 0.4048167
   */
   DOUBLES_EQUAL(0.4048167F, result, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Instantaneous_Obstacle_Prob() works as intended when given object with otg altitude 2 and height 2.
*/
TEST(f360_Calculate_Instantaneous_Obstacle_Prob, ObjectWithCenterAltitude_2_Height_2)
{
   /** \precond
   Object has:
   bbox_center_otg_altitude 2
   bbox_height 2
   */
   obj.bbox_center_otg_altitude = 2.0F;
   obj.bbox_height = 2.0F;

   /** \action
   Call Calculate_Instantaneous_Obstacle_Prob()
   */
   float32_t result = Calculate_Instantaneous_Obstacle_Prob(obj, 7.0F);

   /** \result
   we expect instantaneous obstacle probability to be 0.9622678
   */
   DOUBLES_EQUAL(0.9622678F, result, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Instantaneous_Obstacle_Prob() works as intended when given object with otg altitude 0.4 and height 0.3.
*/
TEST(f360_Calculate_Instantaneous_Obstacle_Prob, ObjectWithlowCenterAltitudeAndLowHeight)
{
   /** \precond
   Object has:
   bbox_center_otg_altitude 0.4
   bbox_height 0.3
   */
   obj.bbox_center_otg_altitude = 0.4F;
   obj.bbox_height = 0.3F;

   /** \action
   Call Calculate_Instantaneous_Obstacle_Prob()
   */
   float32_t result = Calculate_Instantaneous_Obstacle_Prob(obj, 7.0F);

   /** \result
   we expect instantaneous obstacle probability to be 0.9106398
   */
   DOUBLES_EQUAL(0.9106398F, result, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Instantaneous_Obstacle_Prob() works as intended when given stationary object with otg altitude 1.
*/
TEST(f360_Calculate_Instantaneous_Obstacle_Prob, StationaryObjectWithCenterAltitude_1)
{
   /** \precond
   Object has:
   bbox_center_otg_altitude 1
    bbox height is 1
   and is stationary
   */
   obj.bbox_center_otg_altitude = 1.0F;
   obj.movable_prob = 0.0F;

   /** \action
   Call Calculate_Instantaneous_Obstacle_Prob()
   */
   float32_t result = Calculate_Instantaneous_Obstacle_Prob(obj, 5.0F);

   /** \result
   we expect instantaneous obstacle probability to be 0.902373
   */
   DOUBLES_EQUAL(0.902373F, result, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Instantaneous_Obstacle_Prob() works as intended when given stationary object with otg altitude 2 and height 2.
*/
TEST(f360_Calculate_Instantaneous_Obstacle_Prob, StationaryObjectWithCenterAltitude_2_Height_2)
{
   /** \precond
   Object has:
   bbox_center_otg_altitude 2
   bbox_height 2
   and is stationary
   */
   obj.bbox_center_otg_altitude = 2.0F;
   obj.bbox_height = 2.0F;
   obj.movable_prob = 0.0F;

   /** \action
   Call Calculate_Instantaneous_Obstacle_Prob()
   */
   float32_t result = Calculate_Instantaneous_Obstacle_Prob(obj, 7.0F);

   /** \result
   we expect instantaneous obstacle probability to be 0.5879174
   */
   DOUBLES_EQUAL(0.5879174F, result, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_Instantaneous_Obstacle_Prob() works as intended when given stationary object with otg altitude 0.4 and height 0.3.
*/
TEST(f360_Calculate_Instantaneous_Obstacle_Prob, StationaryObjectWithlowCenterAltitudeAndLowHeight)
{
   /** \precond
   Object has:
   bbox_center_otg_altitude 0.4
   bbox_height 0.3
   and is stationary
   */
   obj.bbox_center_otg_altitude = 0.4F;
   obj.bbox_height = 0.3F;
   obj.movable_prob = 0.0F;

   /** \action
   Call Calculate_Instantaneous_Obstacle_Prob()
   */
   float32_t result = Calculate_Instantaneous_Obstacle_Prob(obj, 7.0F);

   /** \result
   we expect instantaneous obstacle probability to be 0.9106398
   */
   DOUBLES_EQUAL(0.9106398F, result, F360_EPSILON);
}

/**
 * @brief
 * Tests for Calculate_And_Filter_Obstacle_Prob (filter coefficient behavior).
 */
TEST_GROUP(f360_Calculate_Filter_Coefficient_For_Obstacle_Prob_test)
{
    /**
     * @setup
     * Prepare object and host.
     * - Object: updated, 10 detections, bbox_center_otg_altitude=2, bbox_height=2
     * - Host: default-initialized
     */
   F360_Object_Track_T obj{};
   F360_Host_T host = {};

   TEST_SETUP()
   {
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.ndets = 10;
      obj.bbox_center_otg_altitude = 2.0F;
      for (uint8_t i = 0; i < obj.ndets; ++i) {
         obj.detids[i] = static_cast<uint32_t>(i + 1U);
      }
      obj.bbox_height = 2.0F;
   }

   TEST_TEARDOWN()
   {
      // e.g., mock().clear();
   }
};

 /** \purpose
This aims to verify that Calculate_And_Filter_Obstacle_Prob() works as intended when given moving object and host accelerating at 1.6 m/s^2.
*/
TEST(f360_Calculate_Filter_Coefficient_For_Obstacle_Prob_test, ObjectMovable_HostAcc_2)
{
   /** \precond
   Object is moving
   Host is accelerating at 1.6 m/s^2
   */
   obj.movable_prob = 1.0F;
   host.acceleration = 1.6F;

   /** \action
   Call Calculate_And_Filter_Obstacle_Prob()
   */
   Calculate_And_Filter_Obstacle_Prob(host , obj );

   /** \result
   we expect that obstacle_prob will be 9.622677e-05
   */
   DOUBLES_EQUAL(9.622677e-05, obj.obstacle_prob, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_And_Filter_Obstacle_Prob() works as intended when given far stationary object and host accelerating at 1.4 m/s^2.
*/
TEST(f360_Calculate_Filter_Coefficient_For_Obstacle_Prob_test, ObjectNonMovableBBox_10x50_HostAcc_1)
{
   /** \precond
   Object is stationary and has box set to 10 40
   Host is accelerating at 1.4 m/s^2
   */
   obj.movable_prob = 0.0F;
   host.acceleration = 1.4F;
   obj.bbox.Set_Center({10.0F, 50.0F});

   /** \action
   Call Calculate_And_Filter_Obstacle_Prob()
   */
   Calculate_And_Filter_Obstacle_Prob(host , obj );

   /** \result
   we expect that obstacle_prob will be 0.2294082
   */
   DOUBLES_EQUAL(0.2294082F, obj.obstacle_prob, F360_EPSILON);
}

 /** \purpose
This aims to verify that Calculate_And_Filter_Obstacle_Prob() works as intended when given far stationary object and host yaw 0.2.
*/
TEST(f360_Calculate_Filter_Coefficient_For_Obstacle_Prob_test, ObjectNonMovableBBox_10x50_HostYawRate_1)
{
   /** \precond
   Object is stationary and has box set to 10 40
   Host yaw is 0.2
   */
   obj.movable_prob = 0.0F;
   host.yaw_rate_rad = 0.2F;
   obj.bbox.Set_Center({10.0F, 50.0F});

   /** \action
   Call Calculate_And_Filter_Obstacle_Prob()
   */
   Calculate_And_Filter_Obstacle_Prob(host , obj );

   /** \result
   we expect that obstacle_prob will be 5.879174e-05
   */
   DOUBLES_EQUAL(5.879174e-05, obj.obstacle_prob, F360_EPSILON);
}
/** @} */
