/** \file
 * This file contains unit tests for content of f360_occlusion.cpp file
 */

#include "f360_occlusion.h"
#include "utilities/f360_occlusion_ut_helpers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_occlusion
 *  @{
 */

/** \brief
 * Test group of Occlusion_T class. Tests verify whether sensor and object information is properly propagated to determine
 * occluded sectors.
 */
TEST_GROUP(f360_occlusion)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   F360_Tracker_Info_T tracker_info{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_TRKR_TIMING_INFO_T timing_info{};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS]{};

   /** \setup
    * Set up 4 corner sensors parameters.
    * Set up two valid objects
    * Initialize occlusion
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Set_Left_Rear_Sensor(sensors[0]);
      Set_Left_Front_Sensor(sensors[1]);
      Set_Right_Rear_Sensor(sensors[2]);
      Set_Right_Front_Sensor(sensors[3]);

      object_tracks[0].id = 1;
      object_tracks[1].id = 2;
      object_tracks[2].id = 3;

      Set_Base_Object_Parameters(object_tracks[0]);
      Point center = {5.0F,-5.0F};
      object_tracks[0].bbox.Set_Center(center);
      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      object_tracks[0].confidenceLevel = 1.0F;
      object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();


      Set_Base_Object_Parameters(object_tracks[1]);
      object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[1].confidenceLevel = 1.0F;
      center = {5.0F,5.0F};
      object_tracks[1].bbox.Set_Center(center);
      object_tracks[1].vcs_position = object_tracks[1].bbox.Get_Corners().Rear_Left();

      Set_Base_Object_Parameters(object_tracks[2]);
      object_tracks[2].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[2].confidenceLevel = 1.0F;
      center = {60.0F,0.0F};
      object_tracks[2].bbox.Set_Center(center);
      object_tracks[2].vcs_position = object_tracks[2].bbox.Get_Corners().Rear_Left();

      tracker_info.num_active_objs = 3;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;

   }
};

/** \purpose  
 * Purpose of this test is to verify whether object that is outside of sensors FOV will have its occlusion status marked as undefined
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Point_Outside_FOV_Is_Marked_As_Undefined)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Create occlusion object
    * Set up point position to be outside of FOV
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   const float32_t lat_pos = 0.9F;
   const float32_t long_pos = -2.5F;
	
   /** \action
    * Determine occlusion status of point outside of FOV
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, long_pos, lat_pos);

   /** \result
    * Check whether occlusion status is UNDEFINED
    */
   CHECK_EQUAL(OCCLUSION_STATUS_UNDEFINED, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether object that is inside of sensors FOV will have its occlusion status determined
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Point_Inside_FOV_Is_Marked_As_Visible_If_Not_Occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Create occlusion object
    * Set up point position to be inside of FOV
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   const float32_t lat_pos = 0.9F;
   const float32_t long_pos = 10.0F;

   /** \action
    * Determine occlusion status of point
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, long_pos, lat_pos);

   /** \result
    * Check whether occlusion status is VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion information is properly propagated
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Information_Is_Properly_Propagated)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Reset FR corner is_valid flag to false
    * Set position of first point to be visible
    * Set position of second point to be occluded
    * Create occlusion object
    */

   sensors[0].variable.is_valid = false;

   const float32_t first_lat_pos = -20.0F;
   const float32_t first_long_pos = 2.5F;

   const float32_t second_lat_pos = -6.0F;
   const float32_t second_long_pos = 6.0F;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
	
   /** \action
    * Determine first point occlusion status
    * Determine second point occlusion status
    */

   const F360_Occlusion_Status_T first_point_occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, first_long_pos, first_lat_pos);
   const F360_Occlusion_Status_T second_point_occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, second_long_pos, second_lat_pos);

   /** \result
    * Check whether first point is visible
    * Check whether second point is occluded
    */
   CHECK_EQUAL(F360_Occlusion_Status_T::OCCLUSION_STATUS_VISIBLE, first_point_occlusion_status);
   CHECK_EQUAL(F360_Occlusion_Status_T::OCCLUSION_STATUS_OCCLUDED, second_point_occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status is properly determined when point is visible
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Status_Is_Properly_Determined_When_Point_Is_Visible)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Set up tested point position to make it visible
    * Create occlusion object
    */
   const float32_t vcs_long_posn = 2.5F;
   const float32_t vcs_lat_posn = -2.5F;
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

    /** \action
     * Determine occlusion status
     */
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, vcs_long_posn, vcs_lat_posn);

   /** \result
    * Check whether returned value is equal to OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status is properly determined when point is on occluded
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Status_Is_Properly_Determined_When_Point_Is_Occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Set up tested point position to make it be occluded
    * Create occlusion object
    */
   const float32_t vcs_long_posn = 10.0F;
   const float32_t vcs_lat_posn = -10.0F;
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Determine occlusion status
    */
      const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, vcs_long_posn, vcs_lat_posn);

   /** \result
    * Check whether returned value is equal to OCCLUSION_STATUS_OCCLUDED
    */
   CHECK_EQUAL(OCCLUSION_STATUS_OCCLUDED, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status is set as visible when point is on edge of
 * object but threshold passed to occlusion object makes it be visible
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Point_On_Edge_Of_Visibility_Marked_As_Visible_By_Adding_Threshold)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Set up tested point position to make it be occluded
    * Set range uncertainty threshold to 0.3F
    * Create occlusion object
    */
   const float32_t vcs_long_posn = 4.0F;
   const float32_t vcs_lat_posn = -4.0F;
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Determine occlusion status
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, vcs_long_posn, vcs_lat_posn);

   /** \result
    * Check whether returned value is equal to OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status are properly sorted
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Status_Is_Sorted)
{
   /** \precond
    * Nothing to set up
    */

   /** \action
    * No action
    */

   /** \result
    * check whether UNDEFINED status is lower than OCCLUDED
    * check whether OCCLUDED status is lower than ON_EDGE
    * check whether ON_EDGE status is lower than VISBILE
    */
   CHECK_TRUE(F360_Occlusion_Status_T::OCCLUSION_STATUS_UNDEFINED < F360_Occlusion_Status_T::OCCLUSION_STATUS_OCCLUDED);
   CHECK_TRUE(F360_Occlusion_Status_T::OCCLUSION_STATUS_OCCLUDED < F360_Occlusion_Status_T::OCCLUSION_STATUS_ON_EDGE);
   CHECK_TRUE(F360_Occlusion_Status_T::OCCLUSION_STATUS_ON_EDGE < F360_Occlusion_Status_T::OCCLUSION_STATUS_VISIBLE);
}

/** \purpose
 * Purpose of this test is to verify whether object that is outside FOV of single sensor will have its occlusion status marked as undefined
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Point_Outside_FOV_Of_Single_Sensor_Is_Marked_As_Undefined)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Create occlusion object
    * Set up point position to be outside of FOV - in the upper right quarter of VCS
    * Set up teseted sensor idx to rear left
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   const float32_t lat_pos = 5.9F;
   const float32_t long_pos = 20.5F;
   const int32_t tested_sensor_id = 1;

   /** \action
    * Determine occlusion status of point outside of FOV of single given sensor
    */
   
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, long_pos, lat_pos, -1, tested_sensor_id);
   
   /** \result
    * Check whether occlusion status is UNDEFINED
    * Tested sensor is left rear, meanwhile tested point is in upper right quarter of VSC
    */
   CHECK_EQUAL(OCCLUSION_STATUS_UNDEFINED, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether object that is inside of single sensors FOV will have its occlusion status determined
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Point_Inside_Single_Sensors_FOV_Is_Marked_As_Visible_If_Not_Occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Create occlusion object
    * Set up point position to be inside of FOV of sinle given sensor
    * Set up teseted sensor idx to front left
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   const float32_t lat_pos = -5.9F;
   const float32_t long_pos = 15.0F;
   const int32_t tested_sensor_id = 2;

   /** \action
    * Determine occlusion status of point inside of FOV of single given sensor
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, long_pos, lat_pos, -1, tested_sensor_id);

   /** \result
    * Check whether occlusion status is VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status is properly determined when point is occluded for single sensor
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Status_Is_Properly_Determined_When_Point_Is_Occluded_For_Single_Sensor)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Set up tested point position to make it be occluded
    * Create occlusion object
    * Set up tested sensor idx to front left
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   const float32_t vcs_long_posn = 10.0F;
   const float32_t vcs_lat_posn = -10.0F;
   const int32_t tested_sensor_id = 2;

   /** \action
    * Determine occlusion status for single sensor
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, vcs_long_posn, vcs_lat_posn, -1, tested_sensor_id);

   /** \result
    * Check whether returned value is equal to OCCLUSION_STATUS_OCCLUDED
    */
   CHECK_EQUAL(OCCLUSION_STATUS_OCCLUDED, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status is properly marked as partialyl visible when object's
 * f_behind_sep_ambiguous is set to true
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Status_For_Single_Sensor_Is_Properly_Determined_When_Behind_Sep_Ambiguous_Set)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Create occlusion object
    * Set f_behind_sep_ambiguous of tested object to true
    * set behind_sep_id to true
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   object_tracks[0].f_behind_sep_ambiguous = true;
   object_tracks[0].behind_sep_id = 1U;


   /** \action
    * Determine occlusion status
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[0], sensors, occlusion_data);

   /** \result
    * Check whether returned value is equal to OCCLUSION_STATUS_UNDEFINED
    */
   CHECK_EQUAL(OCCLUSION_STATUS_ON_EDGE, occlusion_status);
}

/** \purpose
 * Purpose of this test is to verify whether occlusion status is properly marked as occluded when object is
 * behind a SEP
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Occlusion_Status_For_Single_Sensor_Is_Properly_Determined_When_Behind_Sep)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Create occlusion object
    * Set f_behind_sep_ambiguous of tested object to true
    */
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   object_tracks[0].behind_sep_id = 1U;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   /** \action
    * Determine occlusion status
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[0], sensors, occlusion_data);

   /** \result
    * Check whether returned value is equal to OCCLUSION_STATUS_OCCLUDED
    */
   CHECK_EQUAL(OCCLUSION_STATUS_OCCLUDED, occlusion_status);
}

/** \purpose  
 * Purpose of this test is to verify whether object that is more than 50 meters away and it's reference point is not occluded is marked as visible
 * \req
 * NA.
 */
TEST(f360_occlusion, Verify_Object_Status_object_distance_higher_than_50m_ref_point_not_occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    */
   Point center = {-51.F,5.0F};
   object_tracks[1].vcs_position = center;
   object_tracks[1].bbox.Set_Center(center);
   object_tracks[1].movable_prob = 1.0;
   object_tracks[1].f_behind_sep_ambiguous = false;
   object_tracks[1].behind_sep_id = 0U;
   object_tracks[1].movable_prob = 1U;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   /** \action
    * Determine occlusion status of object +50 meters away from host
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[1], sensors, occlusion_data);

   /** \result
    * Check whether occlusion status is OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}


/** \purpose  
 * Purpose of this test is to verify whether object that is more than 50 meters away and it's reference point is occluded, is marked as occluded
 * \req
 * NA.
 */
TEST(f360_occlusion, Verify_Object_Status_object_distance_higher_than_50m_ref_point_is_occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    */
   Point center = {50.F, 50.F};
   object_tracks[2].vcs_position = center;
   object_tracks[2].bbox.Set_Center(center);
   object_tracks[2].movable_prob = 1.0F;
   object_tracks[2].f_behind_sep_ambiguous = false;
   object_tracks[2].behind_sep_id = 0U;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   /** \action
    * Determine occlusion status of object +50 meters away from host
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[2], sensors, occlusion_data);

   /** \result
    * Check whether occlusion status is OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_OCCLUDED, occlusion_status);
}

/** \purpose  
 * Purpose of this test is to verify whether object that has only one corner visible is marked as parially visible
 * \req
 * NA.
 */
TEST(f360_occlusion, Update__Point_Outside_FOV_Is_Marked_As_Partially_Visible)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Set up object position so that only one corner is visible
    * Create occlusion object
    */
   object_tracks[1].vcs_position = Point(10.0F, 1.0F);
   object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   object_tracks[1].Set_Bbox_Orientation(Angle{F360_DEG2RAD(-45.0F)});
   object_tracks[1].Update_Bbox_Center();

   object_tracks[0].movable_prob = 1.0;
   object_tracks[0].vcs_position = Point(5.0F, -1.0F);
   object_tracks[0].bbox.Set_Width(4.0F);
   object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR;
   object_tracks[0].Update_Bbox_Center();

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Determine occlusion status of point outside of FOV
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[1], sensors, occlusion_data);

   /** \result
    * Check whether occlusion status is OCCLUSION_STATUS_PARTIALLY_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_ON_EDGE, occlusion_status);
}


/** \purpose  
 * Purpose of this test is to verify that nonmovable objects with center reference point cannot occlude obejects
 * \req
 * NA.
 */
TEST(f360_occlusion, Occlusion_of_nonmovable_objects)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    * Set up object position so that it is occlulded
    * Create occlusion object
    */
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].vcs_position = Point(40.0F, 0.0F);
   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].Set_Bbox_Orientation(Angle{F360_DEG2RAD(-45.0F)});
   object_tracks[0].bbox.Set_Length(1.0F);
   object_tracks[0].bbox.Set_Width(1.0F);
   object_tracks[0].Update_Bbox_Center();

   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].vcs_position = Point(43.0F, 0.0F);
   object_tracks[1].reference_point = F360_REFERENCE_POINT_LEFT;
   object_tracks[1].Set_Bbox_Orientation(Angle{F360_DEG2RAD(-90.0F)});
   object_tracks[1].bbox.Set_Length(0.5F);
   object_tracks[1].bbox.Set_Width(0.5F);
   object_tracks[1].Update_Bbox_Center();

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Determine occlusion status of occluded object, then rotate the object and update the reference point and test again
    */
   const F360_Occlusion_Status_T occlusion_status1 = Get_Object_Occlusion_Status(object_tracks[1], sensors, occlusion_data);

   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].vcs_position = Point(43.0F, 0.0F);
   object_tracks[1].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   object_tracks[1].Set_Bbox_Orientation(Angle{F360_DEG2RAD(-135.0F)});
   object_tracks[1].bbox.Set_Length(0.5F);
   object_tracks[1].bbox.Set_Width(0.5F);
   object_tracks[1].Update_Bbox_Center();

   const F360_Occlusion_Status_T occlusion_status2 = Get_Object_Occlusion_Status(object_tracks[1], sensors, occlusion_data);

   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].vcs_position = Point(43.0F, 0.0F);
   object_tracks[1].reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   object_tracks[1].Set_Bbox_Orientation(Angle{F360_DEG2RAD(135.0F)});
   object_tracks[1].bbox.Set_Length(0.5F);
   object_tracks[1].bbox.Set_Width(0.5F);
   object_tracks[1].Update_Bbox_Center();

   const F360_Occlusion_Status_T occlusion_status3 = Get_Object_Occlusion_Status(object_tracks[1], sensors, occlusion_data);

   /** \result
    * Check whether occlusion status is OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status1);
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status2);
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status3);
}

/** \defgroup  f360_occlusion_1_sensor_enabled
 *  @{
 */

/** \brief
 * Test group of Occlusion_T class. Tests verify whether sensor and object information is properly propagated to determine
 * occluded sectors.
 */

TEST_GROUP(f360_occlusion_1_sensor_enabled)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   F360_Tracker_Info_T tracker_info{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_TRKR_TIMING_INFO_T timing_info{};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS]{};

   /** \setup
    * Set up 1 corner sensor parameters.
    * Set up one valid objects
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Set_Left_Rear_Sensor(sensors[0]);
      
      Set_Base_Object_Parameters(object_tracks[0]);
      Point center = {5.0F,-5.0F};
      object_tracks[0].bbox.Set_Center(center);

      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      object_tracks[0].confidenceLevel = 1.0F;
      object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
   }
};

/** \purpose  
 * Verificatin of occlusion status of object outside sensor FOV. 
 * there is only one corner sensor enabled. Object is outside it's FOV 
 * so it shouldn't be calssified as occluded 
 * \req
 * NA.
 */
TEST(f360_occlusion_1_sensor_enabled, Verify_Single_Object_Status_leaving_FOV)
{
   /** \precond
    * All basic input data was set in TEST_SETUP
    */

   object_tracks[0].movable_prob = 1.0;
   object_tracks[0].f_behind_sep_ambiguous = false;
   object_tracks[0].behind_sep_id = 0U;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   /** \action
    * Determine occlusion status of object outside of FOV
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[0], sensors, occlusion_data);

   /** \result
    * Check whether occlusion status is OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}

/** \purpose
 * Verify that the occlusion status of object that has only one corner in the sensor's FOV is set to visible.
 * \req
 * NA.
 */
TEST(f360_occlusion_1_sensor_enabled, Verify_Single_Object_Status_One_Corner_In_FOV)
{
   /** \precond
    * Set the object position so that only one corner is in the FOV of the sensor.
    */

   Point center = {-1.0F,-3.0F};
   object_tracks[0].bbox.Set_Center(center);

   object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();
   object_tracks[0].movable_prob = 1.0;
   object_tracks[0].f_behind_sep_ambiguous = false;
   object_tracks[0].behind_sep_id = 0U;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Determine occlusion status of object on the edge of FOV
    */
   const F360_Occlusion_Status_T occlusion_status = Get_Object_Occlusion_Status(object_tracks[0], sensors, occlusion_data);

   /** \result
    * Check whether occlusion status is OCCLUSION_STATUS_VISIBLE
    */
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, occlusion_status);
}

/** @}*/
