/** \file
 * This file contains unit tests for content of f360_calc_obj_height.cpp file
 */

#include "f360_calc_obj_height.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_calc_obj_height
 *  @{
 */

/** \brief
 * Test group of Calc_Obj_Height() function. Tests cover conditions for valid detections 
 * filtering and proper extraction of height value from RSPP_Detection_T struct.
 */
TEST_GROUP(f360_calc_obj_height)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T current_track;
   rspp_variant_A::RSPP_Detection_T dets_raw_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Detection_Props_T dets_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Host_T host;

   /** \setup
    * initialize tracker calibrations
    * set current track assigned detections counter to 1
    * set assigned detection id
    * set an arbitrary detection vcs_position_z value to 101.0F
    * detection azimuth confidence level is not low
    */
   TEST_SETUP()
   {
      current_track.ndets = 1;
      current_track.detids[0] = 1;
      current_track.f_moving = 1;
      dets_raw_props[0].processed.vcs_position_z = 101.0F;
      dets_raw_props[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
      dets_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      host.curvature_rear = 0.0F;
   }
};

/** \purpose  
 * This test checks if the otg_height is not calculated for the 
 * stationary objects that is outside the rear zone because of its
 * x position.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_stationary_outside_rear_zone_x_pos)
{
   /** \precond
    * objects f_moving status is set to false.
    * object's x position is set to a value outside the rear zone.
    * object's y position is set to a value inside the rear zone.
    */
   current_track.vcs_position.x = -1000.0F;
   current_track.vcs_position.y = 0.0F;
   current_track.f_moving = 0;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is not calculated for the 
 * stationary objects that is outside the rear zone because of its
 * curvi lat pos.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_stationary_outside_rear_zone_curvi_lat_pos)
{
   /** \precond
    * objects f_moving status is set to false.
    * object's x position is inside the rear zone.
    * object's y position is set to a value outside the rear zone.
    */
   current_track.vcs_position.x = -20.0F;
   current_track.vcs_position.y = -6.0F;
   host.curvature_rear = 0.21F;
   current_track.f_moving = 0;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is calculated for the 
 * stationary objects that is inside the rear zone.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_stationary_inside_rear_zone)
{
   /** \precond
    * objects f_moving status is set to false.
    * object's x position is set to a value inside the rear zone.
    * object's y position is set to a value inside the rear zone.
    */
   current_track.vcs_position.x = -79.0F;
   current_track.vcs_position.y = 0.0F;
   current_track.f_moving = 0;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(101.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is not calculated for the 
 * stationary objects that is outside the front zone because of its
 * x position.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_stationary_outside_front_zone_x_pos)
{
   /** \precond
    * objects f_moving status is set to false.
    * object's x position is set to a value outside the front zone.
    * object's y position is set to a value inside the front zone.
    */
   current_track.vcs_position.x = 1000.0F;
   current_track.vcs_position.y = 0.0F;
   current_track.f_moving = 0;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is not calculated for the 
 * stationary objects that is outside the front zone because of its
 * curvi lat pos.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_stationary_outside_front_zone_curvi_lat_pos)
{
   /** \precond
    * objects f_moving status is set to false.
    * object's x position is inside the front zone.
    * object's y position is set to a value outside the front zone.
    */
   current_track.vcs_position.x = 20.0F;
   current_track.vcs_position.y = -6.0F;
   host.curvature_rear = 0.21F;
   current_track.f_moving = 0;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is calculated for the 
 * stationary objects that is inside the front zone.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_stationary_inside_front_zone)
{
   /** \precond
    * objects f_moving status is set to false.
    * object's x position is set to a value inside the front zone.
    * object's y position is set to a value inside the front zone.
    */
   current_track.vcs_position.x = 79.0F;
   current_track.vcs_position.y = 0.0F;
   current_track.f_moving = 0;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(101.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is not increased when the 
 * azimuth confidence is too low.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_det_is_filtered_motion_status_moving_low_conf)
{
   /** \precond
    * detection azimuth confidence level is low.
    */

   dets_raw_props[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is not increased when the 
 * detection motion status is not classified as moving.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_det_is_filtered_motion_status_ambiguous_midhigh_conf)
{
   /** \precond
    * detection motion status is not classified as moving.
    */
   current_track.speed = 5.1F;
   current_track.vcs_velocity.longitudinal = 5.1F;
   current_track.vcs_velocity.lateral = 0.0F;
   dets_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose  
 * This test checks if the otg_height is not increased when the 
 * detection motion status is not classified as moving.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, check_if_det_is_filtered_motion_status_ambiguous_low_conf)
{
   /** \precond
    * detection motion status is not classified as moving.
    * detection azimuth confidence is low.
    */

   dets_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   dets_raw_props[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   
   /** \action
    * Call Calc_Obj_Height() function
    */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height did not increase
    */
   DOUBLES_EQUAL(0.0F , current_track.otg_height, F360_EPSILON);
}

/** \purpose
 * This test checks if the otg_height properly increases if current track has 
 * assigned detections at given scan index with non-zero vcs_position_z value.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, otg_height_increases_with_non_zero_dets)
{
   /** \precond
    * current_track otg_height equal to 0.0F
    * current_track ndets set to 3
    * current_track historic ndets counter set to 10.0F.
    * current detections heights above the ground
    * current detection confidence level is not low
   */
  const float track_height = 10.0F;
  current_track.f_moving = true;
  current_track.speed = 5.1F;
  current_track.vcs_velocity.longitudinal = 5.1F;
  current_track.vcs_velocity.lateral = 0.0F;
  current_track.otg_height_raw =  track_height;
  current_track.ndets = 3;
  current_track.ud_mov_historic_ndets = 10.0F;
  float vcs_z_pos[] = {9.0F, 10.0F, 11.5F}; // Note: z axis is pointing downwards so negative z is above ground
  for (uint8_t i=0; i<3; i++)
  {
     current_track.detids[i] = i+1;
     dets_raw_props[i].processed.vcs_position_z = vcs_z_pos[i]; 
     dets_raw_props[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
     dets_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
  } 
   /** \action
    * Call Calc_Obj_Height() function
   */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height increased and the value is correct.
    */
   float k_height_threshold = 0.3F;
   DOUBLES_EQUAL(track_height, current_track.otg_height, k_height_threshold)
   CHECK_TRUE(current_track.otg_height > track_height)
}

/** \purpose
 * This test checks if the otg_height properly decreases if current track has 
 * assigned detections at given scan index with non-zero vcs_position_z value which will
 * bring otg_height to lower value than at the previous scan index.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, otg_height_decreases_with_non_zero_dets)
{
   /** \precond
    * current_track otg_height above the ground
    * current_track ndets set to 3
    * current detection heights above the ground
    * current detection confidence level is not low
   */
   const float track_height = 10.0F;
   current_track.f_moving = true;
   current_track.speed = 5.1F;
   current_track.vcs_velocity.longitudinal = 5.1F;
   current_track.vcs_velocity.lateral = 0.0F;
   current_track.otg_height_raw =  track_height;
   current_track.ndets = 3;
   current_track.ud_mov_historic_ndets = 10.0F;
   float vcs_z_pos[] = {8.5F, 10.0F, 11.0F}; // Note: z axis is pointing downwards so negative z is above ground
   for (uint8_t i=0; i<3; i++)
   {
      current_track.detids[i] = i+1;
      dets_raw_props[i].processed.vcs_position_z = vcs_z_pos[i];
      dets_raw_props[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
      dets_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   } 

   /** \action
    * call Calc_Obj_Height() function
   */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height decreased and the value is properly calculated.
    */
   float k_height_threshold = 0.3F;
   DOUBLES_EQUAL(track_height, current_track.otg_height, k_height_threshold)
   CHECK_TRUE(current_track.otg_height < track_height)
}

/** \purpose
 * This test checks if the percentage od underdrivable detections will be correct when
 * we have 5 detections and 2 of them are below the ground (positive z position) and have
 * high elevation confidence.
 * \req
 * NA.
 */
TEST(f360_calc_obj_height, ud_overdrivable_det_pct)
{
   /** \precond
    * current_track ndets set to 5
    * 3 detections below the ground and 2 above
    * last detection below the ground has low elevation confidence
   */
   current_track.ndets = 5;
   current_track.ud_mov_historic_ndets = 0.0F;
   float vcs_z_pos[] = {2.0F, 2.0F, 2.0F, -2.0F, -2.0F}; // Note: z axis is pointing downwards so negative z is above ground
   for (uint8_t i=0U; i<5U; i++)
   {
      current_track.detids[i] = i+1;
      dets_raw_props[i].processed.vcs_position_z = vcs_z_pos[i];
      dets_raw_props[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;
   }
   dets_raw_props[2].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   /** \action
    * call Calc_Obj_Height() function
   */
   Calc_Obj_Height(dets_raw_props, dets_props, host, current_track);

   /** \result
    * Check if otg_height decreased and the value is properly calculated.
    */
   float exp_overdribable_det_pct = 0.5F;
   DOUBLES_EQUAL(exp_overdribable_det_pct, current_track.ud_overdrivable_det_pct, F360_EPSILON)
}
/** @}*/

