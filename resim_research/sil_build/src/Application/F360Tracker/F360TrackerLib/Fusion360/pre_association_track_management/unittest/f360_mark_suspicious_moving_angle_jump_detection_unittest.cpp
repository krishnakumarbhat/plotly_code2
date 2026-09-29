/** \file
 * This file contains unit tests for content of f360_mark_suspicious_moving_angle_jump_detection.cpp file
 */

#include "f360_mark_suspicious_moving_angle_jump_detection.h"
#include <CppUTest/TestHarness.h>
#include "f360_object_track.h"

using namespace f360_variant_A;

/** \defgroup  f360_mark_suspicious_moving_angle_jump_detection
 *  @{
 */

/** \brief
 * The purpose of this test group is to test Mark_Suspicious_Moving_Angle_Jump_Detection().
 * Mark_Suspicious_Moving_Angle_Jump_Detection() is used for flagging angle jumps originating from moving objects
 * to restrict usage of detection in certain applications.
 */
TEST_GROUP(f360_mark_suspicious_moving_angle_jump_detection)
{	
   rspp_variant_A::RSPP_Detection_T det_raw;
   F360_Radar_Sensor_T sensor;
   F360_Host_T host;
   F360_Detection_Props_T det_prop;
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   
   /** \setup
    * Set default setup to step into the function Mark_Suspicious_Moving_Angle_Jump_Detection():
    * Host properties:
    *    - host.speed = 10.0 m/s (satisfies > threshold requirements)
    * Detection properties:
    *    - range_rate_compensated = -2.0 m/s (satisfies abs > 0.7 threshold)
    *    - f_ok_to_use = true
    *    - f_angle_amb = false (initially)
    *    - raw.snr = 10.0 dB (satisfies < 25.0 threshold)
    *    - raw.range = 50.0 m (satisfies > 10.0 threshold)
    *    - raw.azimuth = 1.3346 rad (~76.4 degrees, satisfies > 15 degrees threshold)
    *    - raw.range_rate = -25.0 m/s (relative speed between host and main object)
    *    - motion_status = MOVING
    * Sensor properties:
    *    - sensor_type = SRR7_PLUS_RADAR
    *    - polarity = 1.0 (positive polarity)
    *    - v_wrapping = 28.1 m/s for both look directions
    *    - mounting_location = RIGHT_FORWARD
    *    - mounting_position = (-0.4m, 0.9m) in VCS coordinates
    *    - sensor velocity = (10.0 m/s longitudinal, 0.0 m/s lateral) matching host
    *    - boresight_az_estimated = -0.00057435036 rad for calibration
    *    - min_aliased_range_rate = -23.0 m/s for both look directions
    * Tracker info contains realistic multi-object traffic scenario:
    *    - Multiple oncoming vehicles with varying speeds and positions
    *    - Mix of moving objects (CTCA filters) and stationary objects (CCA filters)
    *    - Objects positioned at different longitudinal distances (30m to 70m range)
    *    - All objects have proper bounding boxes, orientations, and motion characteristics
    * General setup creates conditions for angle jump detection flagging with realistic multi-object scenario.
    */
   TEST_SETUP()
   {
      host.speed = 10.0F;
      det_prop.f_ok_to_use = true;
      det_prop.range_rate_compensated = -2.0F;
      det_prop.f_angle_amb = false;       // Initially not flagged as angle ambiguous
      det_prop.vcs_position.y = 49.6F;
      det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_raw.raw.azimuth = 1.3346F;         // ~76.4 degrees to place detection at (x around 11.1, y around 49.6) with range=50m
      det_raw.raw.range_rate = -20.0F;       // Relative speed: host(10 m/s) + main_object(10 m/s approaching) = -20 m/s
      det_raw.raw.snr = 10.0F;
      det_raw.raw.range = 50.0F;
      det_raw.processed.vcs_az = det_raw.raw.azimuth + sensor.variable.vacs_boresight_az_estimated;
      sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
      sensor.constant.polarity = 1.0F;
      sensor.constant.v_wrapping[0] = 28.1F;
      sensor.constant.v_wrapping[1] = 28.1F;
      sensor.variable.vcs_velocity.lateral = 0.0F;
      sensor.variable.vcs_velocity.longitudinal = 10.0F;
      sensor.variable.vacs_boresight_az_estimated = -0.00057435036F;
      sensor.variable.is_valid = true;
      sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensor.constant.mounting_position.vcs_position.longitudinal = -0.4F;  // x = -0.4m
      sensor.constant.mounting_position.vcs_position.lateral = 0.9F;        // y = 0.9m
      sensor.constant.min_aliaised_range_rate[0] = -23.0F;
      sensor.constant.min_aliaised_range_rate[1] = -23.0F;
      
      // Initialize objects array
      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         objects[i] = {};  // Initialize to zero
         objects[i].id = i + 1U;  // Object IDs start from 1
      }
      
      // Setup test object with specified properties
      objects[0].id = 1U;
      objects[0].vcs_position.x = 50.0F;   // x = 50m
      objects[0].vcs_position.y = -1.5F;   // y = -1.5m (moved slightly to the right)
      objects[0].vcs_velocity.longitudinal = -10.0F;  // vx = -10 m/s (oncoming)
      objects[0].vcs_velocity.lateral = 0.0F;         // vy = 0 m/s
      objects[0].movable_prob = 0.8F;     // High probability of being movable
      objects[0].Update_Bbox_Size(5.0F, 2.0F);       // Set length = 5m, width = 2m using proper function
      objects[0].lat_buffer_zone_wid1 = 3.0F;  // lateral buffer zone
      objects[0].lat_buffer_zone_wid2 = 3.0F;  // lateral buffer zone
      objects[0].long_buffer_zone_len1 = 2.0F; // longitudinal buffer zone
      objects[0].long_buffer_zone_len2 = 2.0F; // longitudinal buffer zone
      objects[0].speed = 10.0F;           // Speed magnitude matching velocity vector sqrt((-10)^2 + 0^2) = 10 m/s
      Angle orientation_angle;
      orientation_angle.Value(F360_PI);  // 180 degrees (oncoming)
      objects[0].Set_Bbox_Orientation(orientation_angle); // Set object orientation to match velocity vector (heading towards host: 180 degrees)
      objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[0].curvature = 0.0F;        // No curvature (straight motion)
      objects[0].reference_point = F360_REFERENCE_POINT_FRONT;  // Set reference point to FRONT
      
      // Setup additional oncoming objects with varying speeds and positions
      // Object 1: In front of main object (x=70m) - fast oncoming
      objects[1].id = 2U;
      objects[1].vcs_position.x = 70.0F;   // x = 70m (in front)
      objects[1].vcs_position.y = -2.5F;   // y = -2.5m (slightly different lateral position)
      objects[1].vcs_velocity.longitudinal = -15.0F;  // vx = -15 m/s (faster oncoming)
      objects[1].vcs_velocity.lateral = 0.0F;         // vy = 0 m/s
      objects[1].movable_prob = 0.9F;     // High probability of being movable
      objects[1].Update_Bbox_Size(4.5F, 1.8F);       // Slightly smaller vehicle
      objects[1].lat_buffer_zone_wid1 = 3.0F;
      objects[1].lat_buffer_zone_wid2 = 3.0F;
      objects[1].long_buffer_zone_len1 = 2.0F;
      objects[1].long_buffer_zone_len2 = 2.0F;
      objects[1].speed = 15.0F;           // Speed magnitude
      Angle orientation_angle1;
      orientation_angle1.Value(F360_PI);
      objects[1].Set_Bbox_Orientation(orientation_angle1);
      objects[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[1].curvature = 0.0F;        // No curvature (straight motion)
      objects[1].reference_point = F360_REFERENCE_POINT_FRONT;  // Set reference point to FRONT
      
      // Object 2: Behind main object (x=55m) - medium speed oncoming
      objects[2].id = 3U;
      objects[2].vcs_position.x = 55.0F;   // x = 55m (behind main object at 50m from host's perspective)
      objects[2].vcs_position.y = -1.8F;   // y = -1.8m (close to main object laterally)
      objects[2].vcs_velocity.longitudinal = -8.0F;   // vx = -8 m/s (medium oncoming)
      objects[2].vcs_velocity.lateral = 0.0F;         // vy = 0 m/s
      objects[2].movable_prob = 0.7F;     // High probability of being movable
      objects[2].Update_Bbox_Size(4.8F, 2.1F);       // Similar sized vehicle
      objects[2].lat_buffer_zone_wid1 = 3.0F;
      objects[2].lat_buffer_zone_wid2 = 3.0F;
      objects[2].long_buffer_zone_len1 = 2.0F;
      objects[2].long_buffer_zone_len2 = 2.0F;
      objects[2].speed = 8.0F;            // Speed magnitude
      Angle orientation_angle2;
      orientation_angle2.Value(F360_PI);
      objects[2].Set_Bbox_Orientation(orientation_angle2);
      objects[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[2].curvature = 0.0F;        // No curvature (straight motion)
      objects[2].reference_point = F360_REFERENCE_POINT_FRONT;  // Set reference point to FRONT
      
      // Object 3: Further behind (x=30m) - slow oncoming
      objects[3].id = 4U;
      objects[3].vcs_position.x = 30.0F;   // x = 30m (further behind)
      objects[3].vcs_position.y = -2.2F;   // y = -2.2m
      objects[3].vcs_velocity.longitudinal = -5.0F;   // vx = -5 m/s (slow oncoming)
      objects[3].vcs_velocity.lateral = 0.0F;         // vy = 0 m/s
      objects[3].movable_prob = 0.6F;     // Moderate probability of being movable
      objects[3].Update_Bbox_Size(5.5F, 2.3F);       // Larger vehicle (truck/SUV)
      objects[3].lat_buffer_zone_wid1 = 3.0F;
      objects[3].lat_buffer_zone_wid2 = 3.0F;
      objects[3].long_buffer_zone_len1 = 2.0F;
      objects[3].long_buffer_zone_len2 = 2.0F;
      objects[3].speed = 5.0F;            // Speed magnitude
      Angle orientation_angle3;
      orientation_angle3.Value(F360_PI);
      objects[3].Set_Bbox_Orientation(orientation_angle3);
      objects[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[3].curvature = 0.0F;        // No curvature (straight motion)
      objects[3].reference_point = F360_REFERENCE_POINT_FRONT;  // Set reference point to FRONT
      
      // Object 4: Stationary object (x=40m) - parked vehicle
      objects[4].id = 5U;
      objects[4].vcs_position.x = 40.0F;   // x = 40m (between objects[2] and objects[0])
      objects[4].vcs_position.y = -3.0F;   // y = -3m (slightly off to the side)
      objects[4].vcs_velocity.longitudinal = 0.0F;   // vx = 0 m/s (stationary)
      objects[4].vcs_velocity.lateral = 0.0F;        // vy = 0 m/s (stationary)
      objects[4].movable_prob = 0.1F;     // Low probability of being movable (parked/stationary)
      objects[4].Update_Bbox_Size(4.2F, 1.9F);       // Standard car size
      objects[4].lat_buffer_zone_wid1 = 3.0F;
      objects[4].lat_buffer_zone_wid2 = 3.0F;
      objects[4].long_buffer_zone_len1 = 2.0F;
      objects[4].long_buffer_zone_len2 = 2.0F;
      objects[4].speed = 0.0F;            // No speed (stationary)
      Angle orientation_angle4;
      orientation_angle4.Value(0.0F);     // 0 degrees (aligned with road direction)
      objects[4].Set_Bbox_Orientation(orientation_angle4);
      objects[4].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;  // CCA filter for stationary object
      objects[4].curvature = 0.0F;        // No curvature (straight motion)
      objects[4].reference_point = F360_REFERENCE_POINT_FRONT;  // Set reference point to FRONT
      
      // Setup tracker_info for longitudinal sorting (closest to furthest order)
      // Order: objects[3] (30m) -> objects[4] (40m) -> objects[0] (50m) -> objects[2] (55m) -> objects[1] (70m)
      tracker_info.vcslong_sorted_first_infront_of_host = &objects[3];  // Start with closest object (30m)
      
      // Setup next track pointers for longitudinal sorting (closest to furthest)
      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = nullptr;
      }
      
      // Link the longitudinal sorting chain: obj3(30m) -> obj4(40m) -> obj0(50m) -> obj2(55m) -> obj1(70m)
      tracker_info.vcslong_sorted_next_track[0] = &objects[2];  // objects[0] -> objects[2]
      tracker_info.vcslong_sorted_next_track[1] = nullptr;      // objects[1] -> end
      tracker_info.vcslong_sorted_next_track[2] = &objects[1];  // objects[2] -> objects[1]
      tracker_info.vcslong_sorted_next_track[3] = &objects[4];  // objects[3] -> objects[4]
      tracker_info.vcslong_sorted_next_track[4] = &objects[0];  // objects[4] -> objects[0]
   }
};

/** \purpose  
 * Purpose of this test is to verify that a detection that fits the angle jump hypothesis with object with ID 1 as the source is correctly flagged.
 * \req
 * NA
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_is_detection_correctly_marked_obj1_source)
{
   /** \precond
    * This uses the test setup detection.
    */

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is correctly marked as f_angle_amb.
    */
   CHECK_TRUE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not attempt to flag detections as angle jump detections when sensor is not relevant for angle jump detection.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_sensor_not_relevant)
{
   /** \precond
    * Set sensor type to unknown to make sensor_angle_amb.f_sensor_relevant false.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since sensor is not relevant.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not perform additional angle jump detection processing when detection is already marked as angle ambiguous.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_already_angle_amb)
{
   /** \precond
    * Set the detection as already marked for angle ambiguity.
    */
   det_prop.f_angle_amb = true;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection remains marked as f_angle_amb (no additional processing).
    */
   CHECK_TRUE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not attempt to flag detections as angle jump detections when detection is marked as not ok to use.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_not_ok_to_use)
{
   /** \precond
    * Set the detection as not ok to use.
    */
   det_prop.f_ok_to_use = false;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since it's not ok to use.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not attempt to flag detections as angle jump detections when host speed is too low.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_low_host_speed)
{
   /** \precond
    * Set host speed below the minimum threshold (20 km/h approximately 5.56 m/s).
    */
   host.speed = 5.0F;  // Below min_host_speed threshold

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb due to low host speed.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not attempt to flag detections as angle jump detections when detection is not marked as moving.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_not_moving_detection)
{
   /** \precond
    * Set the detection motion status to ambiguous.
    */
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since it's not a moving detection.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when range rate compensated is too low.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_low_range_rate_compensated)
{
   /** \precond
    * Set the detection range rate compensated to a value at the threshold (<= 0.7 m/s).
    */
   det_prop.range_rate_compensated = 0.7F;  // At threshold, condition requires > 0.7

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb due to insufficient range rate compensated.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when SNR is too high.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_high_snr)
{
   /** \precond
    * Set the detection SNR to a value at the threshold (>= 25.0 dB).
    */
   det_raw.raw.snr = 25.0F;  // At threshold, condition requires < 25.0

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb due to high SNR.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

// TO DO DFD-3282: technically should probably change the set-up such that the detection azimuth could be flagged as angle jump if below 15 degrees was allowed.
// Now it wouldn't have been flagged anyway, so the tests isn't really doing anything...
/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when VCS azimuth angle is too low.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_low_vcs_azimuth)
{
   /** \precond
    * Set the detection VCS azimuth to a value at the threshold (<= 15 degrees).
    */
   det_raw.raw.azimuth = F360_DEG2RAD(14.0F);  // 14 degrees, below 15 degree threshold
   det_raw.processed.vcs_az = det_raw.raw.azimuth + sensor.variable.vacs_boresight_az_estimated;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb due to low VCS azimuth angle.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

// TO DO DFD-3282: technically should probably change the set-up such that the detection azimuth could be flagged as angle jump if below 10 m was allowed.
// Now it wouldn't have been flagged anyway, so the tests isn't really doing anything...
/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when range is too low.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_low_range)
{
   /** \precond
    * Set the detection range to a value at the threshold (<= 10.0 m).
    */
   det_raw.raw.range = 10.0F;  // At threshold, condition requires > 10.0

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb due to low range.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when the 
 * potential source object is outside the rough zone around the detection.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_detection_outside_rough_zone)
{
   /** \precond
    * Position main object so detection candidate at (49.6, 0.4) after az shift is just outside the rough zone.
    * Rough zone lateral threshold: 5.0m
    * Position object at (49.6, -5.5) so detection candidate at (49.6, 0.4) is just outside lateral rough zone boundary.
    */
   
   // Position main object so detection candidate is just outside rough zone
   objects[0].vcs_position.x = 49.6F;   // Align longitudinally with detection candidate
   objects[0].vcs_position.y = -5.5F;   // Position so lateral distance to detection candidate (0.4 - (-5.5) = 5.9m) exceeds rough zone (5.0m)
   objects[0].Update_Bbox_Center();
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since detection candidate is outside rough zone.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when the detection candidate
 * is inside the rough zone but outside the potential source object's extended bounding box.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_detection_inside_rough_zone_outside_ext_bbox)
{
   /** \precond
    * Position main object so detection candidate at (49.6, 0.4) is inside rough zone but outside extended bbox.
    * Rough zone lateral threshold: 5.0m
    * Extended bbox is smaller than rough zone
    * Position object at (49.6, -3.5) so detection candidate at (49.6, 0.4) is within rough zone but outside extended bbox.
    */
   
   // Position main object so detection candidate is inside rough zone but outside extended bbox
   objects[0].vcs_position.x = 49.6F;   // Align longitudinally with detection candidate
   objects[0].vcs_position.y = -4.1F;   // Position so lateral distance to detection candidate (0.4 - (-4.1) = 4.5m)
   objects[0].Update_Bbox_Center();
  

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since detection candidate is outside extended bbox.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when the potential source object is not moving (movable probability = 0).
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_object_not_moving)
{
   /** \precond
    * Set the potential source object movable probability to 0 (not moving).
    */
   objects[0].movable_prob = 0.0F;  // Set to not moving

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since potential source object is not moving.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when the potential source object is not oncoming (positive x velocity).
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_object_not_oncoming_positive_xvel)
{
   /** \precond
    * Set the potential source object velocity to positive x velocity (receding from host).
    */
   objects[0].vcs_velocity.longitudinal = 10.0F;   // Positive x velocity (receding)
   objects[0].speed = 10.0F;                       // Update speed magnitude

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since potential source object is not oncoming.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when the potential source object is oncoming but too slow (negative x velocity > -3m/s).
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_object_not_oncoming_slow_negative_xvel)
{
   /** \precond
    * Set the potential source object velocity to slow negative x velocity (> -3m/s, not sufficiently oncoming).
    */
   objects[0].vcs_velocity.longitudinal = -2.0F;   // Slow negative x velocity (not sufficiently oncoming)
   objects[0].speed = 2.0F;                        // Update speed magnitude

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since potential source object is not sufficiently oncoming.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function breaks the object search loop early when objects are positioned longitudinally beyond the search distance.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_early_loop_break_objects_too_far_longitudinally)
{
   /** \precond
    * Position objects so that most are close to host (within 40m), but the last two objects have xpos > 60m.
    * This should trigger early loop termination when (obj->vcs_position.x - new_vcs_x) > rough_obj_search_distance_x.
    * Detection candidate is at approximately (49.6, 0.4), so objects with x > (49.6 + 8.0) = 57.6m should cause early break.
    */
   
   // Position first three objects close to host (within 40m)
   objects[0].vcs_position.x = 25.0F;   // Close to host
   objects[0].Update_Bbox_Center();
   objects[1].vcs_position.x = 35.0F;   // Close to host  
   objects[1].Update_Bbox_Center();
   objects[2].vcs_position.x = 38.0F;   // Close to host
   objects[2].Update_Bbox_Center();
   
   // Position last two objects far from detection candidate (> 60m)
   objects[3].vcs_position.x = 65.0F;   // Far from detection candidate (65 - 49.6 = 15.4m > 8.0m search distance)
   objects[3].Update_Bbox_Center();
   objects[4].vcs_position.x = 75.0F;   // Even further from detection candidate
   objects[4].Update_Bbox_Center();
      
   // Update longitudinal sorting to reflect new positions: obj0(25m) -> obj1(35m) -> obj2(38m) -> obj3(65m) -> obj4(75m)
   tracker_info.vcslong_sorted_first_infront_of_host = &objects[0];  // Start with closest object (25m)
   tracker_info.vcslong_sorted_next_track[0] = &objects[1];  // objects[0] -> objects[1]
   tracker_info.vcslong_sorted_next_track[1] = &objects[2];  // objects[1] -> objects[2]  
   tracker_info.vcslong_sorted_next_track[2] = &objects[3];  // objects[2] -> objects[3]
   tracker_info.vcslong_sorted_next_track[3] = &objects[4];  // objects[3] -> objects[4]
   tracker_info.vcslong_sorted_next_track[4] = nullptr;      // objects[4] -> end

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since the loop should break early due to no eligible objects.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function breaks the object search loop correctly when reaching the end of the sorted object list (NULL pointer).
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_loop_break_at_end_of_object_list)
{
   /** \precond
    * Position all objects longitudinally below the azimuth candidate (x < 40m).
    * Detection candidate is at approximately (49.6, 0.4) after azimuth shift.
    * All objects positioned with x < 40m ensures they are all in front of the detection candidate.
    * This should cause the loop to iterate through all objects and then break when obj becomes NULL.
    */
   
   // Position all objects longitudinally below the azimuth candidate (x < 40m)
   objects[0].vcs_position.x = 15.0F;   // Well below detection candidate
   objects[0].Update_Bbox_Center();
   objects[1].vcs_position.x = 20.0F;   // Well below detection candidate
   objects[1].Update_Bbox_Center();
   objects[2].vcs_position.x = 25.0F;   // Well below detection candidate
   objects[2].Update_Bbox_Center();
   objects[3].vcs_position.x = 30.0F;   // Well below detection candidate
   objects[3].Update_Bbox_Center();
   objects[4].vcs_position.x = 35.0F;   // Well below detection candidate
   objects[4].Update_Bbox_Center();
      
   // Update longitudinal sorting to reflect new positions: obj0(15m) -> obj1(20m) -> obj2(25m) -> obj3(30m) -> obj4(35m)
   tracker_info.vcslong_sorted_first_infront_of_host = &objects[0];  // Start with closest object (15m)
   tracker_info.vcslong_sorted_next_track[0] = &objects[1];  // objects[0] -> objects[1]
   tracker_info.vcslong_sorted_next_track[1] = &objects[2];  // objects[1] -> objects[2]  
   tracker_info.vcslong_sorted_next_track[2] = &objects[3];  // objects[2] -> objects[3]
   tracker_info.vcslong_sorted_next_track[3] = &objects[4];  // objects[3] -> objects[4]
   tracker_info.vcslong_sorted_next_track[4] = nullptr;      // objects[4] -> NULL (end of list)

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since all objects are positioned below the detection candidate and loop should terminate at NULL.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when the range rate dealiasing fails.
 * This tests the scenario where an object passes all filtering criteria (moving, oncoming, within zones) but the dealiasing function returns false
 * due to mismatch between the detection's measured range rate and the object's predicted range rate.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_range_rate_dealiasing_failure)
{
   /** \precond
    * Set the potential source object velocity to create a predicted range rate that cannot be successfully dealiased
    * to match the detection's measured range rate. The object will pass all other filtering criteria.
    * 
    * Detection setup:
    * - det_raw.raw.range_rate = -20.0 m/s (from test setup)
    * - Host velocity = 10.0 m/s longitudinal (from test setup)
    * 
    * Object setup modification:
    * - Change object velocity to create a predicted range rate that's far enough from the measured range rate
    *   that dealiasing cannot find a match within the acceptable thresholds.
    * - Set object velocity to -8.8 m/s (well above the -3.0 threshold to pass oncoming filter)
    *   This creates a predicted range rate around -18.8 m/s (host 10 + object 8.8)
    * - The difference between measured (-20.0) and predicted (-18.8) is 1.2 m/s
    *   which should be just above the 1.0 m/s threshold for successful dealiasing.
    */
   
   // Set object velocity to create range rate mismatch while passing movement filters
   objects[0].vcs_velocity.longitudinal = -8.8F;   // Oncoming velocity to create predicted RR of ~-18.8 m/s
   objects[0].vcs_velocity.lateral = 0.0F;         // No lateral velocity
   objects[0].speed = 8.8F;                        // Update speed magnitude to match velocity

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since range rate dealiasing should fail.
    * The object passes all filters but the predicted range rate (~-18.8 m/s) cannot be dealiased 
    * to match the measured range rate (-20.0 m/s) within acceptable thresholds (difference = 1.2 m/s > 1.0 m/s threshold).
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the function does not flag detections as angle jump detections when using FLR7 sensor.
 * FLR7 sensors are disabled in the moving angle jump context, so the function should not flag any detections.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_flr7_sensor_disabled)
{
   /** \precond
    * Set the sensor type to FLR7 to test that moving angle jump detection is disabled for this sensor type.
    * Keep all other setup conditions that would normally trigger angle jump detection.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check if the detection is NOT marked as f_angle_amb since FLR7 sensors are disabled in moving angle jump context.
    * Even though all other conditions would trigger angle jump detection, the FLR7 sensor type should prevent flagging.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that the Process_Moving_Angle_Jump_Detections function correctly processes multiple detections
 * and calls Mark_Suspicious_Moving_Angle_Jump_Detection for each valid detection.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_process_moving_angle_jump_detections_integration)
{
   /** \precond
    * Create arrays for sensors and detections as required by Process_Moving_Angle_Jump_Detections.
    * Use the setup detection and sensor from the test group, plus add a second detection with FLR7 sensor.
    */
   
   // Create sensor array and populate with test sensors
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   sensors[0] = sensor;  // Use the SRR7+ sensor from test setup
   
   // Create second sensor (FLR7) that should not flag detections in moving angle jump context
   sensors[1] = sensor;  // Copy base sensor properties
   sensors[1].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;  // Change to FLR7
   
   // Create detection list and populate with two test detections
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   raw_detect_list.number_of_valid_detections = 2U;
   
   // First detection: SRR7+ sensor (should be flagged as angle jump)
   raw_detect_list.detections[0] = det_raw;  // Use the detection from test setup
   raw_detect_list.detections[0].raw.sensor_id = 1U;  // Set sensor ID to match sensors[0]
   
   // Second detection: FLR7 sensor (should NOT be flagged as angle jump)
   raw_detect_list.detections[1] = det_raw;  // Copy the detection from test setup
   raw_detect_list.detections[1].raw.sensor_id = 2U;  // Set sensor ID to match sensors[1]
 
   // Create detection properties array and populate with test detection props
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   detection_props[0] = det_prop;  // Use the detection props from test setup for first detection
   detection_props[1] = det_prop;  // Copy detection props for second detection

   /** \action
    * Call Process_Moving_Angle_Jump_Detections function.
    */
   Process_Moving_Angle_Jump_Detections(sensors, host, raw_detect_list, tracker_info, detection_props);

   /** \result
    * Check that the first detection (SRR7+) is correctly marked as f_angle_amb after processing.
    * Check that the second detection (FLR7) is NOT marked as f_angle_amb since FLR7 sensors are disabled in moving angle jump context.
    * This verifies that the integration function properly calls the individual detection function and applies sensor-specific logic.
    */
   CHECK_TRUE(detection_props[0].f_angle_amb)   // SRR7+ detection should be flagged
   CHECK_FALSE(detection_props[1].f_angle_amb)  // FLR7 detection should NOT be flagged
}

/** \purpose
 * Purpose of this test is to verify that when the original detection position falls inside a moving
 * object's extended bounding box at short range (range < 15m), the detection is NOT flagged as an
 * angle jump. This prevents false positives: if the detection sits inside a nearby moving object's
 * bbox it may legitimately belong to that object rather than being a ghost angle jump.
 *
 * Geometry (forward sensor, right-forward, range = 12m, azimuth = 1.3346 rad ~76.4 deg):
 *   - Angle shift candidate -62.5 deg maps the detection to approximately (11.2, 3.8) in VCS.
 *   - Object at (10.0, 7.0) with bbox 5x2m + 3m/2m lateral/longitudinal buffers:
 *     extended bbox center ~(12.5, 7.0), half-length 4.5, half-width 4.0 (in object TCS).
 *   - The shifted candidate (11.2, 3.8) falls inside the extended bbox.
 *   - The original detection at (10.0, 10.0) also falls inside the extended bbox.
 *   - With range=12m < 15m, f_apply_original_det_pos_in_obj_check = true,
 *     so f_original_det_pos_in_obj is set and the outer candidate loop exits without flagging.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection, check_not_marked_original_det_inside_obj_bbox_short_range)
{
   /** \precond
    * Reduce range to 12m to enable the original detection position check (range < 15m).
    * Place the original detection position inside the object's extended bbox.
    * Move objects[0] so the angle-shift candidate also falls inside the same extended bbox.
    */
   det_raw.raw.range = 12.0F;
   det_prop.vcs_position.x = 10.0F;  // original det pos - inside object extended bbox
   det_prop.vcs_position.y = 10.0F;  // positive y preserves correct LUT search direction

   objects[0].vcs_position.x = 10.0F;
   objects[0].vcs_position.y = 7.0F;
   objects[0].Update_Bbox_Center();

   // Single-object search chain at the candidate position
   tracker_info.vcslong_sorted_first_infront_of_host = &objects[0];
   tracker_info.vcslong_sorted_next_track[0] = nullptr;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: f_original_det_pos_in_obj = true causes early loop exit.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** @}*/

/** \defgroup f360_mark_suspicious_moving_angle_jump_detection_rear_sensor
 *  @{
 */

/** \brief
 * This test group verifies Mark_Suspicious_Moving_Angle_Jump_Detection() for rear SRR7+ sensors.
 *
 * Key differences from forward sensors tested in this group:
 *   - Range gate: (4, 15) m  instead of (10, inf)
 *   - SNR gate:   bypassed for rear (f_rear_sensor || snr < 25 is always true)
 *   - Object velocity condition: longitudinal > +3 m/s (overtaking) instead of < -3 m/s (oncoming)
 *   - Object list: vcslong_sorted_start (full list) instead of vcslong_sorted_first_infront_of_host
 *   - Range rate threshold: +/-0.4 m/s instead of +/-1.0 m/s
 *
 * Sensor geometry for the positive test:
 *   - RIGHT_REAR sensor at VCS (-4.6, 0.78), boresight 120 deg, polarity = 1
 *   - raw.azimuth = -20 deg -> LUT idx 11, positive angle shifts selected (y > 0)
 *   - range = 8m, snr = 30 dB (above threshold, bypassed for rear)
 *   - Ongoing object at VCS (-12.5, 0.0) with longitudinal velocity +15 m/s
 *   - The 61.1 deg angle shift candidate maps to approximately (-12.16, 3.38) in VCS,
 *     which is inside the extended bbox of the object (center ~(-15.0, 0.0), 9m x 8m after extension).
 *   - Predicted range rate at that position: ~-4.73 m/s.
 *     Measured range_rate = -5.0 m/s: diff = -0.27, within the rear threshold of +/-0.4 m/s.
 */
TEST_GROUP(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor)
{
   rspp_variant_A::RSPP_Detection_T det_raw;
   F360_Radar_Sensor_T sensor;
   F360_Host_T host;
   F360_Detection_Props_T det_prop;
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];

   TEST_SETUP()
   {
      host.speed = 10.0F;  // > F360_KPH2MPS(20.0)

      det_prop.f_ok_to_use = true;
      det_prop.range_rate_compensated = -5.0F;   // abs > 0.7 threshold
      det_prop.f_angle_amb = false;
      det_prop.vcs_position.x = -5.99F;  // ghost detection x (sensor_x + range*cos(vcs_az))
      det_prop.vcs_position.y = 8.65F;   // positive y -> det_side_sign=+1 -> rear: counterclockwise (positive) shifts
      det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

      det_raw.raw.azimuth = F360_DEG2RAD(-20.0F);  // LUT_idx = 11
      det_raw.raw.range_rate = -5.0F;   // within predicted RR +/-0.4 window (~-4.73 m/s)
      det_raw.raw.snr = 30.0F;   // > 25 dB; SNR gate is bypassed for rear sensors
      det_raw.raw.range = 8.0F;    // within (4, 15) m for rear sensor
      det_raw.processed.vcs_az = F360_DEG2RAD(120.0F) + F360_DEG2RAD(-20.0F);  // boresight + raw_az

      sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
      sensor.constant.polarity = 1.0F;
      sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensor.constant.mounting_position.vcs_position.longitudinal = -4.6F;
      sensor.constant.mounting_position.vcs_position.lateral = 0.78F;
      sensor.constant.v_wrapping[0] = 60.1F;
      sensor.constant.v_wrapping[1] = 60.1F;
      sensor.constant.min_aliaised_range_rate[0] = -23.0F;
      sensor.constant.min_aliaised_range_rate[1] = -23.0F;
      sensor.variable.vacs_boresight_az_estimated = F360_DEG2RAD(120.0F);
      sensor.variable.vcs_velocity.longitudinal = 10.0F;
      sensor.variable.vcs_velocity.lateral = 0.0F;
      sensor.variable.is_valid = true;

      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         objects[i]    = {};
         objects[i].id = i + 1U;
      }

      // Ongoing object behind host
      objects[0].id = 1U;
      objects[0].vcs_position.x = -12.5F;
      objects[0].vcs_position.y = 0.0F;
      objects[0].vcs_velocity.longitudinal = 15.0F;  // > 3.0 m/s
      objects[0].vcs_velocity.lateral = 0.0F;
      objects[0].movable_prob = 0.8F;
      objects[0].Update_Bbox_Size(5.0F, 2.0F);
      objects[0].lat_buffer_zone_wid1 = 3.0F;
      objects[0].lat_buffer_zone_wid2 = 3.0F;
      objects[0].long_buffer_zone_len1 = 2.0F;
      objects[0].long_buffer_zone_len2 = 2.0F;
      objects[0].speed = 15.0F;
      Angle orientation_angle;
      orientation_angle.Value(0.0F); 
      objects[0].Set_Bbox_Orientation(orientation_angle);
      objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[0].curvature = 0.0F;
      objects[0].reference_point = F360_REFERENCE_POINT_FRONT;

      // Rear sensor uses vcslong_sorted_start (full sorted list from beginning)
      tracker_info.vcslong_sorted_start = &objects[0];
      tracker_info.vcslong_sorted_first_infront_of_host  = nullptr;
      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = nullptr;
      }
   }
};

/** \purpose
 * Purpose of this test is to verify that a rear SRR7+ sensor correctly flags a detection
 * matching the angle jump hypothesis from an overtaking vehicle.
 * Key aspects exercised: SNR = 30 dB (bypassed for rear), range = 8m within [4,15),
 * vcslong_sorted_start used for object search, range rate threshold +/-0.4 m/s.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_detection_marked)
{
   /** \action
    * Call tested function with default rear sensor setup.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Check that the detection is correctly flagged as angle ambiguous.
    */
   CHECK_TRUE(det_prop.f_angle_amb)
}

/** \purpose
 * Purpose of this test is to verify that the rear sensor range gate lower bound is enforced.
 * The condition requires range > 4.0m, so range = 4.0m must NOT trigger detection.
 *
 * Geometry: vcs_az = 100 deg, range = 4.0m.
 *   Original detection position: (-5.295, 4.719)
 *   61.1 deg candidate at range 4.0m: (-8.384, 2.076)
 *   Object at (-8.5, 0.0) with FRONT ref, orientation=0: center (-11.0, 0.0),
 *   extended bbox 4.5 x 4.0m. Candidate TCS (2.616, 2.076) is inside.
 *   At range = 4.1m the gate would pass and the detection would be flagged.
 *   At range = 4.0m the condition (range > 4.0) is not satisfied -> not flagged.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_not_marked_range_at_lower_bound)
{
   /** \precond
    * Set range exactly at the minimum rear threshold (4.0m, condition is strictly greater than).
    * Update detection position for range=4.0 geometry and place object so the angle-shift
    * candidate at range ~4.1m would fall inside the extended bbox.
    */
   det_raw.raw.range = 4.0F;
   det_prop.vcs_position.x = -5.295F;  // sensor_x + 4.0 * cos(100 deg)
   det_prop.vcs_position.y = 4.719F;   // sensor_y + 4.0 * sin(100 deg), positive -> same shift direction

   objects[0].vcs_position.x = -8.5F;  // 61.1 deg candidate at range~4.1 lands at ~(-8.48, 2.11)
   objects[0].vcs_position.y = 0.0F;   // center at (-11.0, 0.0), extended half-sizes 4.5 x 4.0
   objects[0].Update_Bbox_Center();

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: 4.0 is not > 4.0 (lower bound not satisfied).
    * At range = 4.1m with this object position the detection would be flagged.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose
 * Purpose of this test is to verify that the rear sensor range gate upper bound is enforced.
 * The condition requires range < 15.0m, so range = 15.0m must NOT trigger detection.
 *
 * Geometry: vcs_az = 100 deg, range = 15.0m.
 *   Original detection position: (-7.205, 15.552)
 *   61.1 deg candidate at range 14.9m: (-18.694, 5.606)
 *   Object at (-18.7, 2.0) with FRONT ref, orientation=0: center (-21.2, 2.0),
 *   extended bbox 4.5 x 4.0m. Candidate TCS (2.506, 3.606) is inside.
 *   At range = 14.9m the gate would pass and the detection would be flagged.
 *   At range = 15.0m the condition (range < 15.0) is not satisfied -> not flagged.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_not_marked_range_at_upper_bound)
{
   /** \precond
    * Set range exactly at the maximum rear threshold (15.0m, condition is strictly less than).
    * Update detection position for range=15.0 geometry and place object so the angle-shift
    * candidate at range ~14.9m would fall inside the extended bbox.
    */
   det_raw.raw.range = 15.0F;
   det_prop.vcs_position.x = -7.205F;   // sensor_x + 15.0 * cos(100 deg)
   det_prop.vcs_position.y = 15.552F;   // sensor_y + 15.0 * sin(100 deg), positive -> same shift direction

   objects[0].vcs_position.x = -18.7F;  // 61.1 deg candidate at range~14.9 lands at ~(-18.69, 5.61)
   objects[0].vcs_position.y = 2.0F;    // center at (-21.2, 2.0), extended half-sizes 4.5 x 4.0
   objects[0].Update_Bbox_Center();

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: 15.0 is not < 15.0 (upper bound not satisfied).
    * At range = 14.9m with this object position the detection would be flagged.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose
 * Purpose of this test is to verify that a rear sensor does not flag a detection when the
 * potential source object does not have sufficient velocity (longitudinal velocity <= 3 m/s).
 *
 * Range rate is set to match the predicted value for an object just above the threshold
 * (v_long = 2.9 m/s): predicted_rr = (2.9 - 10.0) * cos(161.1 deg) ~= +6.5 m/s.
 * range_rate = +6.6 is within the rear +/-0.4 threshold of that prediction, so at
 * v_long = 2.9 the detection WOULD be flagged.  At v_long = 2.9 the velocity condition
 * (longitudinal > 3.0 m/s) is the sole reason flagging is prevented.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_not_marked_object_low_velocity)
{
   /** \precond
    * Set the object longitudinal velocity to 2.9 m/s (does NOT satisfy > 3 m/s).
    * Set range_rate to +6.6 m/s, consistent with a v_long = 2.9 m/s object
    * (predicted_rr ~+6.5 m/s at the 61.1 deg candidate).
    */
   objects[0].vcs_velocity.longitudinal = 2.9F;
   objects[0].speed = 2.9F;
   det_raw.raw.range_rate = 6.6F;
   det_prop.range_rate_compensated = 6.6F;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: object velocity 2.9 m/s does not exceed the 3 m/s threshold.
    * With v_long = 2.9 and the same range_rate the detection would be flagged.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose
 * Purpose of this test is to verify that a rear sensor does not flag a detection when the
 * potential source object is oncoming (negative longitudinal velocity).
 * For a rear sensor the relevant threat is an overtaking vehicle (positive velocity), not an
 * oncoming one.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_not_marked_object_oncoming)
{
   /** \precond
    * Set the object longitudinal velocity to -10 m/s (oncoming).
    */
   objects[0].vcs_velocity.longitudinal = -10.0F;
   objects[0].speed = 10.0F;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: oncoming velocity does not satisfy the rear velocity condition
    * (longitudinal > 3 m/s).
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose
 * Purpose of this test is to verify that the tighter range rate threshold of +/-0.4 m/s used
 * for rear sensors prevents flagging when the measured RR differs from the predicted value by
 * more than 0.4 m/s.
 *
 * For the default rear setup, angle candidates that reach the bbox check are at 61.1 deg
 * (predicted RR ~-4.73 m/s) and 85.2 deg (predicted RR ~-4.98 m/s).
 * range_rate = -5.5 gives differences of ~0.77 and ~0.52 m/s respectively, both exceeding the
 * +/-0.4 rear threshold.  Both differences are below 1.0 m/s, so the forward threshold would pass.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_not_marked_rr_outside_tight_threshold)
{
   /** \precond
    * Set range rate so the difference from the predicted values (~4.73 and ~4.98 m/s) is ~0.52-0.77 m/s,
    * exceeding the rear threshold of 0.4 m/s for all valid candidates.
    */
   det_raw.raw.range_rate = -5.5F;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: range rate differences ~0.52-0.77 m/s exceed the rear threshold of 0.4 m/s.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose
 * Purpose of this test is to verify that the object search loop starts at vcslong_sorted_start
 * (the most-negative-x end of the sorted list) for rear sensors, traverses the first two objects
 * that are within the search window, and then breaks early when the third object is more than
 * 8.0m further ahead (positive x direction) than the angle-shift candidate position.
 *
 * Detection placement first angle jump azimuth candidate:
 *  new_vcs_x ~= -8 m
 *  new_vcs_y ~= 8 m
 *
 * Object placement:
 *   obj0  x = -20.0 m  ->  within window (delta = 7.836 <= 8.0), traversed
 *   obj1  x = -15.0 m  ->  within window (delta = ~7 <= 8.0), traversed
 *   obj2  x =  1.0 m  ->  outside window (delta = 9 > 8.0), triggers break
 *
 * Objects 0 and 1 have longitudinal velocity = 2.0 m/s which fails the rear velocity condition
 * (> 3.0 m/s), so they are traversed but not matched.  Object 2 is never evaluated for a match
 * because the break fires first.  The net result is no detection flagged.
 */
TEST(f360_mark_suspicious_moving_angle_jump_detection_rear_sensor, check_rear_sensor_not_marked_break_due_to_distance)
{
   /** \precond
    * Set up three objects sorted by ascending x (most negative first).
    * Objects 0 and 1 have insufficient overtaking velocity; object 2 triggers the distance break.
    */
   objects[0].vcs_position.x = -20.0F;
   objects[0].vcs_velocity.longitudinal = 2.0F;
   objects[0].speed = 2.0F;
   objects[0].Update_Bbox_Center();

   objects[1].vcs_position.x = -15.0F;
   objects[1].vcs_velocity.longitudinal = 2.0F;
   objects[1].speed = 2.0F;
   objects[1].Update_Bbox_Center();

   objects[2].vcs_position.x = 1.0F;
   objects[2].vcs_velocity.longitudinal = 15.0F;
   objects[2].speed = 15.0F;
   objects[2].Update_Bbox_Center();

   tracker_info.vcslong_sorted_start = &objects[0];
   tracker_info.vcslong_sorted_next_track[0] = &objects[1];
   tracker_info.vcslong_sorted_next_track[1] = &objects[2];
   tracker_info.vcslong_sorted_next_track[2] = nullptr;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Moving_Angle_Jump_Detection(sensor, host, det_raw, tracker_info, det_prop);

   /** \result
    * Detection is NOT flagged: objects 0 and 1 are traversed but fail the velocity condition;
    * object 2 triggers the distance break before any bbox evaluation.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** @}*/
