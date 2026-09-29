/** \file
 * This file contains unit tests for content of f360_object_list_angle_jump_qualifier.cpp file
 */

 #include "f360_object_list_angle_jump_qualifier.h"
 #include <CppUTest/TestHarness.h>
 #include "f360_clear_object_track.h"
 
 //#include "headerfile_needed.h"
 
 // Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines
 
 using namespace f360_variant_A;
 
 /** \defgroup  f360_object_list_angle_jump_qualifier
  *  @{
  */
 
 /** \brief
  * This test group checks that function Object_List_Angle_Jump_Qualifier works correctly
  */
 TEST_GROUP(f360_object_list_angle_jump_qualifier)
 {	
    // Declare common variables used within all tests in this test group.
    F360_Tracker_Info_T tracker_info = {};
    F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
    F360_Calibrations_T calib = {};
    F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
    F360_TRKR_TIMING_INFO_T timing_info = {};
    F360_Host_T host = {};
    rspp_variant_A::RSPP_Detection_List_T dets_raw = {};
    // FLR7 azimuth ambiguity
    const float32_t flr7_angle_shift = F360_DEG2RAD(19.4712F);
 
    /** \setup
     * Initialize tracker calibrations.
     * Set host speed
     * Set sensor props
     * Set object and detection props (see comment in test_setup() for more info)
     */
    TEST_SETUP()
    {
       Initialize_Tracker_Calibrations(calib);
 
       host.speed = 30.0F;
 
       // Setup sensor properties 
       Setup_Front_Center_Sensor(host.speed,sensors[0]);
 
       // Setup tracker info properties
       tracker_info.num_active_objs = 4;
       tracker_info.nr_suspected_stat_angle_jump_dets_filtered = 1.0F;
       tracker_info.f_severe_angle_jump_detected = false;
 
       // Clear object tracks
       for (F360_Object_Track_T& obj : object_tracks)
       {
          Clear_Object_Track(obj);
       }
 
       // Object properties and detection setup
       // The object and detection properties are setup such that they will fullfil all the criteria of being angle jump dets and angle jump crossing ghosts
       // The following setup is done as follows:
       // -> Create a set of points that are on a straight line (same lateral position), just as you would expect detections from a guardrail
       // -> Rotate the previous mentioned set of points by a factor of FL7 azimuth ambiguity shift angle
       // -> The rotated points' position is then used to create objects
       // -> The objects are setup as crossing ghosts, i.e. they have close to  68.75 degrees heading, and speed greater than 10m/s
       // -> For every object 5 associated detections are also setup
       // -> The detections are initially are setup at [-2,-1, 0, 1, 2]m longitudinally from object unrotated position and with same lateral position as object
       // -> The detections' points are then rotated by a factor of FL7 azimuth ambiguity shift angle
       // -> The other dectection properties are setup according to expectations from a angle jump detection
       // -> The object are also added to the tracker info sorted list
 
       const float32_t all_objs_lat_pos = 0.0F; // initial (unrotated) lateral position of all objs and dets
       const float32_t angle_shift = flr7_angle_shift * 2.0F; //factor of FL7 azimuth ambiguity shift angle
       const float32_t dist_between_objs = 5.0F; //Distance/Spacing between setup objects
       uint8_t total_num_dets = 0;
       for (uint8_t obj_i = 0U;  obj_i < tracker_info.num_active_objs; obj_i++)
       {
          const float32_t curr_obj_non_rotated_long_pos = dist_between_objs*(obj_i+1U); //Set initial long pos as a factor of dist_between_objs
          const Point curr_obj_non_rotated_vcs_position = Point{curr_obj_non_rotated_long_pos, all_objs_lat_pos};
          Set_Object_Parameters_To_Be_Valid_Crossing_Angle_Jump_Ghosts(curr_obj_non_rotated_vcs_position, angle_shift, obj_i, object_tracks[obj_i]);
          
          // Create and associate detections to object_tracks[obj_i]
          Set_Associated_Angle_Jump_Detections(
             curr_obj_non_rotated_vcs_position,
             angle_shift,
             host.speed,
             object_tracks[obj_i],
             dets_raw.detections,
             total_num_dets);
          
          // Add created objects to tracker info long pos sorted list 
          if (obj_i == 0U)
          {
             // First obj in long sorted list
             tracker_info.vcslong_sorted_start = &(object_tracks[obj_i]);
             tracker_info.vcslong_sorted_prev_track[obj_i] = NULL;
             tracker_info.vcslong_sorted_next_track[obj_i] = &(object_tracks[obj_i+1U]);
          }
          else if (obj_i == (tracker_info.num_active_objs - 1))
          {
             // last obj in long sorted list
             tracker_info.vcslong_sorted_prev_track[obj_i] = &(object_tracks[obj_i-1U]);
             tracker_info.vcslong_sorted_next_track[obj_i] = NULL;
          }
          else
          {
             tracker_info.vcslong_sorted_next_track[obj_i] = &(object_tracks[obj_i+1U]);
             tracker_info.vcslong_sorted_prev_track[obj_i] = &(object_tracks[obj_i-1U]);
          }
       }
    }
 
    // Helper functions
    void Set_Object_Parameters_To_Be_Valid_Crossing_Angle_Jump_Ghosts(
       const Point& curr_obj_non_rotated_vcs_position,
       const float32_t angle_shift,
       const uint8_t obj_idx,
       F360_Object_Track_T& object)
    {
       constexpr float32_t crossing_absolute_heading_rad = 1.2F;  // 68.75 degrees
       constexpr float32_t min_speed_threshold = 11.0F;
 
       object.id = obj_idx + 1U;
 
       // Set min speed - should be greater than 10m/s
       object.speed = min_speed_threshold;
       
       // Rotate points by a factor of FL7 azimuth ambiguity shift angle
       F360_Rotate_2D_Vector(
          curr_obj_non_rotated_vcs_position.x,
          curr_obj_non_rotated_vcs_position.y,
          F360_Cosf(angle_shift), F360_Sinf(angle_shift),
          object.vcs_position.x,
          object.vcs_position.y);
 
       object.vcs_heading = Angle{crossing_absolute_heading_rad};
       object.bbox.Set_Orientation(object.vcs_heading);
 
       object.bbox.Set_Length(6.0F);
       object.bbox.Set_Width(2.5F);
       object.Update_Bbox_Center();
 
       object.ndets = 0; // to be changed by Set_Associated_Angle_Jump_Detections()
    }
 
    void Set_Associated_Angle_Jump_Detections(
       const Point& curr_obj_non_rotated_vcs_position,
       const float32_t angle_shift,
       const float32_t host_speed,
       F360_Object_Track_T& object,
       rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
       uint8_t& total_num_dets)
    {
       const float32_t all_dets_lat_pos = curr_obj_non_rotated_vcs_position.y; // Should be same as unrotated object lat pos
       const float32_t det_min_long = -2.0F; // The detections are initially are setup at [-2,-1, 0, 1, 2]m longitudinally from object unrotated position
       const uint8_t base_det_idx = total_num_dets;
       for (uint8_t det_i = 0U;  det_i < 5; det_i++)
       {
          const uint8_t det_idx = base_det_idx + det_i;
          
          const float32_t curr_det_non_rotated_long_pos = curr_obj_non_rotated_vcs_position.x + det_min_long + det_i;
          const Point curr_det_non_rotated_vcs_position = Point{curr_det_non_rotated_long_pos, all_dets_lat_pos};
 
          // Rotate points by a factor of FL7 azimuth ambiguity shift angle
          F360_Rotate_2D_Vector(
             curr_det_non_rotated_vcs_position.x,
             curr_det_non_rotated_vcs_position.y,
             F360_Cosf(angle_shift), F360_Sinf(angle_shift),
             detections[det_idx].processed.vcs_position_x,
             detections[det_idx].processed.vcs_position_y);
 
          detections[det_idx].raw.range = F360_Get_Hypotenuse(detections[det_idx].processed.vcs_position_x, detections[det_idx].processed.vcs_position_y);
          detections[det_idx].raw.azimuth = F360_Atan2f(detections[det_idx].processed.vcs_position_y, detections[det_idx].processed.vcs_position_x);
          
          // true expected azimuth of detection
          const float32_t det_true_azimuth = F360_Atan2f(curr_det_non_rotated_vcs_position.y, curr_det_non_rotated_vcs_position.x);
 
          // Det range rate is setup based on true expected azimuth of detection
          detections[det_idx].raw.range_rate = -host_speed * F360_Cosf(det_true_azimuth);
          
          detections[det_idx].raw.sensor_id = 1;
          
          object.detids[det_i] = det_idx + 1U;
          object.ndets++;
          total_num_dets++;
       }
    }   
    
    void Setup_Front_Center_Sensor(
       const float32_t host_speed,
       F360_Radar_Sensor_T& sensor)
    {
       sensor.constant.mounting_position.vcs_position.lateral = 0.0F;
       sensor.constant.mounting_position.vcs_position.longitudinal = 0.0F;
       sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
       sensor.constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
       sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
       sensor.constant.id = 1;
       sensor.constant.polarity = 1;
       sensor.constant.v_wrapping[F360_DET_LOOK_ID_0] = 28.1F;
 
       sensor.variable.is_valid = true;
       sensor.variable.look_id = F360_DET_LOOK_ID_0;
       sensor.variable.vacs_boresight_az_estimated = 0.0F;
       sensor.variable.vcs_velocity.lateral = 0.0F;
       sensor.variable.vcs_velocity.longitudinal = host_speed;
    }
 };
 
 /** \purpose  
  * Check if tracker_info.f_severe_angle_jump_detected is true
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, FunctionToTest_Descriptive_Tag)
 {
    /** \precond
     * Use Default test setup
     */
    
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if function returns true if sensor mounting is center2_fw and sensor type is FLR7_V2_PLT
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Sensor_mounting_position_center2_and_sensor_type_FLR7_V2_PLT)
 {
    /** \precond
     * Set sensor mounting location to center2_fw
     * Set sensor tyoe to FLR7_V2_PLT
     */
    sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER2_FORWARD;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR;
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if function returns true if sensor mounting is center3_fw and sensor type is FLR7_PLT
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Sensor_mounting_position_center3_and_sensor_type_FLR7)
 {
    /** \precond
     * Set sensor mounting location to center3_fw
     * Set sensor tyoe to FLR7_PLT
     */
    sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER3_FORWARD;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if function returns true if sensor mounting is center_fw and sensor type is unknown
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Sensor_mounting_position_unknown_fw_and_sensor_type_FLR7_V2_PLT)
 {
   /* \precond
    * Set sensor mounting location to unknown
    * Set sensor tyoe to unknown
    */
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR;
   /** \action
    *Call Object_List_Angle_Jump_Qualifier()
    */
   Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if function returns false if sensor mounting is center_fw and sensor type is unknown
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Sensor_mounting_position_center_fw_and_sensor_type_unknown)
 {
    /** \precond
     * Set sensor mounting location to unknown
     * Set sensor type to unknown
     */
    sensors[0].constant.mounting_location =F360_MOUNTING_LOCATION_CENTER_FORWARD;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_UNKNOWN;
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(false, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 /** \purpose  
  * Check if function returns false if no crossing object was found, vcslong_sorted_start equal null
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, No_crossing_object_found_because_vcslong_sorted_start_equal_null)
 {
    /** \precond
     * Use Default test setup
     */
    tracker_info.vcslong_sorted_start = NULL;
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(false, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if no crossing object was found, conditions not satisfied by heading
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Conditions_for_crossing_not_satisfiied_by_heading)
 {
    /** \precond
     * Set obj heading higher than threshold
     * Set speed ok
     * Set obj vcs_position.x ok
     */
 
    constexpr float32_t not_crossing_absolute_heading_rad = 1.4F;
    object_tracks[0].vcs_heading = Angle{not_crossing_absolute_heading_rad};
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading);
    object_tracks[0].speed = 11.0F;
    object_tracks[0].vcs_position.x = 10.0F;
 
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if no crossing object was found, conditions not satisfied by speed
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Conditions_for_crossing_not_satisfiied_by_speed)
 {
    /** \precond
     * Set obj heading ok
     * Set speed below threshold
     * Set obj vcs_position.x ok
     */
 
    constexpr float32_t not_crossing_absolute_heading_rad = 1.2F;
    object_tracks[0].vcs_heading = Angle{not_crossing_absolute_heading_rad};
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading);
    object_tracks[0].speed = 0.0F;
    object_tracks[0].vcs_position.x = 10.0F;
 
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if no crossing object was found, conditions not satisfied by speed by x position
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Conditions_for_crossing_not_satisfiied_by_vcs_x_pos)
 {
    /** \precond
     * Set obj heading ok
     * Set speed ok
     * Set obj vcs_position.x below threshold
     */
 
    constexpr float32_t not_crossing_absolute_heading_rad = 1.2F;
    object_tracks[0].vcs_heading = Angle{not_crossing_absolute_heading_rad};
    object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading);
    object_tracks[0].speed = 11.0F;
    object_tracks[0].vcs_position.x = -10.0F;
 
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(true, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 
 /** \purpose  
  * Check if function returns false if crossing object was found but clustering conditions not satisfied
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Crossing_obj_found_but_conditions_for_clustering_not_satisfiied)
 {
    /** \precond
     * Set parameters of objects such that every object would be clustered alone
     */
 
    object_tracks[0].speed = 28.0F;

    object_tracks[1].vcs_position.x = 40.0F;
    object_tracks[1].vcs_position.x = 20.0F;
    object_tracks[1].Update_Bbox_Center();
 
    object_tracks[2].vcs_position.x = object_tracks[1].vcs_position.x + 1.0F;
    object_tracks[2].vcs_position.y = object_tracks[1].vcs_position.y + 20.0F;
    object_tracks[2].Update_Bbox_Center();

    object_tracks[3].vcs_position.x = 50.0F;
    object_tracks[3].vcs_position.y = 30.0F;
    object_tracks[3].Update_Bbox_Center();
    object_tracks[3].vcs_heading = Angle {-1.34F};
 
    /** \action
     * Call Object_List_Angle_Jump_Qualifier()
     */
    Object_List_Angle_Jump_Qualifier(dets_raw, sensors, host, object_tracks, tracker_info);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(false, tracker_info.f_severe_angle_jump_detected, "f_severe_angle_jump_detected is not set correctly");
 }
 /** \purpose  
  * Check if function Count_Suspected_Stationary_Angle_Jumps_In_Objects
  *  computes adequate number of detections even if one detection is not from FLR sensor and one detection does not satisfy stationary hypothesis
  * \req
  * NA
  */
 TEST(f360_object_list_angle_jump_qualifier, Detections_Not_From_FLR7)
 {
    /** \precond
    * Set first detection's range rate to 30.0F
    * Set second detection's sensor's type as unknown
    */
    dets_raw.detections[0].raw.range_rate = 30.0F;
           
    dets_raw.detections[1].raw.sensor_id = 2;

    sensors[1].constant.sensor_type = F360_SENSOR_TYPE_UNKNOWN;

    uint16_t nr_clustered_crossing_objects = 4U;
    uint16_t ids_of_clustered_crossing_objects[NUMBER_OF_OBJECT_TRACKS] = {};
    uint16_t nr_suspected_angle_jumps_in_clustered_objects = 0U;
    ids_of_clustered_crossing_objects[0] = 1;
    ids_of_clustered_crossing_objects[1] = 2;
    ids_of_clustered_crossing_objects[2] = 3;
    ids_of_clustered_crossing_objects[3] = 4;
    
 
    /** \action
     * Call Count_Suspected_Stationary_Angle_Jumps_In_Objects()
     */
    Count_Suspected_Stationary_Angle_Jumps_In_Objects(object_tracks, dets_raw, sensors, ids_of_clustered_crossing_objects, nr_clustered_crossing_objects, nr_suspected_angle_jumps_in_clustered_objects);
 
    /** \result
     * Check that the output match expected data.
     */
    CHECK_EQUAL_TEXT(18, nr_suspected_angle_jumps_in_clustered_objects, "f_severe_angle_jump_detected is not set correctly");
 }
 /** @}*/
 
