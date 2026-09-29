/** \file
   This file contains unit tests for content of f360_update_object_reference_point.cpp file
*/
#include "f360_update_object_reference_point.h"
#include "f360_constants.h"
#include <CppUTest/TestHarness.h>
#include "f360_math.h"
#include "f360_iterator.h"

using namespace f360_variant_A;

/** \defgroup  f360_update_object_reference_point
 *  @{
 */

/** \brief
*  Testing function that determines the object's reference point
**/
TEST_GROUP(f360_update_object_reference_point)
{
   /* Common setup for test.
    Create an arbitrary object. In these specific tests the object position is moved around the host. To verify that the
    correct closest reference point is returned. */
   F360_Object_Track_T obj;
   F360_Calibrations_T calibs;
   F360_Reference_Point_T new_ref_point;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   float32_t host_dist_rear_axle_to_vcs_m;
   bool f_update_obj_pos_only;
   bool f_update_obj_states;
   bool f_current_object_behind_another_object;

   /** \setup
    * Initialize common object to move around host to check returned ref points
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      globals.f_single_front_center_radar_only = false;

      for (F360_Radar_Sensor_T& sensor : sensors)
      {
         sensor.variable.is_valid = false;
      }

      obj.vcs_position.x = -10.0F;
      obj.vcs_position.y = 10.0F;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.Update_Bbox_Size(6.0F, 2.0F);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.movable_prob = 1.0F;
      obj.f_moving = true;
      host_dist_rear_axle_to_vcs_m = 3.0F;
      f_update_obj_pos_only = false;
      f_update_obj_states = false;
      f_current_object_behind_another_object=false;

      globals.rotated_left_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing 45 deg vcs
      globals.rotated_left_fov_normal[0][1] = 0.707106781186548F;
      
      globals.rotated_right_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing -45 deg vcs
      globals.rotated_right_fov_normal[0][1] = -0.707106781186548F;
   }
};

/**
*\purpose  Verify that when the object is non_movable, the refereence point is center
*\req    NA
*/
TEST(f360_update_object_reference_point, check_center_refernce_point_retruned_for_non_moveable_object)
{
   /** \precond
    * Place object in VCS, such that the previous ref point i.e. LEFT is still selected
    **/
   obj.vcs_position.x = 7.7346F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.movable_prob = 0.0F;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect the reference point to be center
   **/
  CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, obj.reference_point);
  CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, obj.min_projection_reference_point);
}

/**
*\purpose  Verify that no size update takes place as new reference point is aligned with the previous one
*\req    NA
*/
TEST(f360_update_object_reference_point, check_no_size_update)
{
   /** \precond
    * Place object in VCS, such that the previous ref point i.e. LEFT is still selected
    **/
   obj.vcs_position.x = 7.7346F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   f_update_obj_states = true;
   f_update_obj_pos_only = false;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect no size update
   **/
   CHECK_EQUAL(6.0F, obj.bbox.Get_Length());
   CHECK_EQUAL(2.0F, obj.bbox.Get_Width());
}

/**
*\purpose  Verify that object position is being updated when object is CCA
*\req    NA
*/
TEST(f360_update_object_reference_point, check_object_kf_state_update_CCA)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.speed = 1.0F;
   f_update_obj_states = true;
   f_update_obj_pos_only = false;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect speed and positional update
   **/
   CHECK_EQUAL(5.2F, obj.vcs_position.x);
   CHECK_EQUAL(9.0F, obj.vcs_position.y);
}

/**
*\purpose  Verify that object position uncertainty is not updated when object is CCA
*\req    NA
*/
TEST(f360_update_object_reference_point, check_object_pos_uncertainty_increase_CCA)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host and set reference point to REAR and set object pointing to 0 
    * (such that reference point is changed from REAR to REAR_LEFT).
    * Set object track filter type to CCA
    * Set object errcov to unit matrix
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   float32_t unit_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      unit_mat[idx][idx] = 1.0F;
   }
   std::copy(cmn::begin(unit_mat), cmn::end(unit_mat), cmn::begin(obj.errcov));

   /** \action 
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Check such that object errcov is unchanged
   **/
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(unit_mat[idx][idx], obj.errcov[idx][idx], 1e-6F);
   }
}

/**
*\purpose  Verify that only object position but not other states are being updated when object is CTCA
*\req    NA
*/
TEST(f360_update_object_reference_point, check_object_kf_state_update_CTCA)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.speed = 1.0F;
   f_update_obj_states = true;
   f_update_obj_pos_only = false;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect speed and positional update (speed should be unchanged by the function call while position should be updated)
   **/
   DOUBLES_EQUAL_TEXT(1.0F, obj.speed, F360_EPSILON, "Speed is unexpectedly changed"); // Unchanged by the function call
   DOUBLES_EQUAL_TEXT(5.2F, obj.vcs_position.x, F360_EPSILON, "Object vcs x position has unexpected value"); // Should be changed by function call
   DOUBLES_EQUAL_TEXT(9.0F, obj.vcs_position.y, F360_EPSILON, "Object vcs x position has unexpected value"); // SHould be changed by function call
}

/**
*\purpose  Verify that object position uncertainty is updated when object is CCA
*\req    NA
*/
TEST(f360_update_object_reference_point, check_object_pos_uncertainty_increase_CTCA)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host and set reference point to REAR and set object pointing to 0 
    * (such that reference point is changed from REAR to REAR_LEFT).
    * Set object track filter type to CTCA
    * Set object errcov to unit matrix
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   float32_t unit_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      unit_mat[idx][idx] = 1.0F;
   }
   std::copy(cmn::begin(unit_mat), cmn::end(unit_mat), cmn::begin(obj.errcov));

   /** \action 
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Check such that object errcov is unchanged
   **/
  float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
  expected_errcov[0][0] += 0.001217974870088F;
  expected_errcov[1][1] += 0.010000000000000F;
  std::copy(cmn::begin(unit_mat), cmn::end(unit_mat), cmn::begin(expected_errcov));
  for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
  {
     DOUBLES_EQUAL(unit_mat[idx][idx], obj.errcov[idx][idx], 1e-6F);
  }
}

/**
*\purpose  Verify that only object position is being updated
*\req    NA
*/
TEST(f360_update_object_reference_point, check_only_position_update)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.speed = 1.0F;
   f_update_obj_states = true;
   f_update_obj_pos_only = true;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect only positional update
   **/
   CHECK_EQUAL(1.0F, obj.speed);
   CHECK_EQUAL(5.2F, obj.vcs_position.x);
   CHECK_EQUAL(9.0F, obj.vcs_position.y);
}

/**
*\purpose  Verify that object size is not updated with positional update
*\req    NA
*/
TEST(f360_update_object_reference_point, check_object_size_update)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   obj.speed = 1.0F;
   f_update_obj_states = false;
   f_update_obj_pos_only = false;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect positional update and no size update
   **/
   CHECK_EQUAL(6.0F, obj.bbox.Get_Length());
   CHECK_EQUAL(2.0F, obj.bbox.Get_Width());
   CHECK_EQUAL(8.2F, obj.vcs_position.x);
   CHECK_EQUAL(10.0F, obj.vcs_position.y);
}

/**
*\purpose  Verify that there is no change in reference point if no reference point is visible in single front radar
* setup when not called from initialization, i.e. f_update_obj_pos_only is false but that min projection reference point is still selected
*\req    NA
*/
TEST(f360_update_object_reference_point, check_no_change_in_ref_pnt_when_no_reference_points_visible)
{
   /** \precond
    * Place object behind host, pointing vcs 0 deg, outside of sensor FoV
    **/
   f_update_obj_states = true;
   f_update_obj_pos_only = false;

   host_dist_rear_axle_to_vcs_m = 0.0F;

   obj.vcs_position.x = -10.0F;
   obj.vcs_position.y = 0.0F;
   obj.bbox.Set_Length(2.0F);
   obj.bbox.Set_Width(2.0F);
   obj.Set_Bbox_Orientation(Angle{ 0.0F }); // updates bbox center as well

   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
   
   obj.movable_prob = 1.0F;

   globals.f_single_front_center_radar_only = true;

   sensors[0].variable.is_valid = true;

   sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;

   globals.rotated_left_fov_normal[0][0] = 1.0F;
   globals.rotated_left_fov_normal[0][1] = 0.0F;
   globals.rotated_right_fov_normal[0][0] = 1.0F;
   globals.rotated_right_fov_normal[0][1] = 0.0F;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Verify obj.reference_point is set to F360_REFERENCE_POINT_CENTER
   * Verify obj.min_projection_reference_point is set to F360_REFERENCE_POINT_FRONT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, obj.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, obj.min_projection_reference_point);
}

/**
*\purpose  Verify that the min projection reference point is always selected even if no reference point is visible in a single front radar
* setup when called from initialization, i.e. f_update_obj_pos_only is true
*\req    NA
*/
TEST(f360_update_object_reference_point, check_min_proj_ref_pnt_is_selected_when_no_reference_points_visible_update_only_position)
{
   /** \precond
    * Place object behind host, pointing vcs 0 deg, outside of sensor FoV
    **/
   f_update_obj_states = true;
   f_update_obj_pos_only = true;

   host_dist_rear_axle_to_vcs_m = 0.0F;

   obj.vcs_position.x = -10.0F;
   obj.vcs_position.y = 0.0F;
   obj.bbox.Set_Length(2.0F);
   obj.bbox.Set_Width(2.0F);
   obj.Set_Bbox_Orientation(Angle{ 0.0F }); // updates bbox center as well

   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
   
   obj.movable_prob = 1.0F;

   globals.f_single_front_center_radar_only = true;

   sensors[0].variable.is_valid = true;

   sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;

   globals.rotated_left_fov_normal[0][0] = 1.0F;
   globals.rotated_left_fov_normal[0][1] = 0.0F;
   globals.rotated_right_fov_normal[0][0] = 1.0F;
   globals.rotated_right_fov_normal[0][1] = 0.0F;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Verify obj.reference_point is set to F360_REFERENCE_POINT_FRONT
   * Verify obj.min_projection_reference_point is set to F360_REFERENCE_POINT_FRONT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, obj.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, obj.min_projection_reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns a new reference point, becuase the hysterisis check is applicable
*\req    NA
*/
TEST(f360_update_object_reference_point, check_hysterisis_select_new_ref_point)
{
   /** \precond
    * Place object in VCS, such that new ref point i.e. REAR_LEFT is selected instead of the previous LEFT
    **/
   obj.vcs_position.x = 10.7347F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect the REAR_LEFT reference point
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, obj.reference_point);
}

/**
*\purpose  This test check if the prev ref point is CENTER, then despite the hysterisis condition being false, a new ref point is returned
* This test also checks if the min_prjection reference point is equal to the obj.reference point, since all points are in the FOV
*\req    NA
*/
TEST(f360_update_object_reference_point, check_hysterisis_center_ref_point_condition)
{
   /** \precond
    * Place object in VCS, such that the hysterisis condition is false and prev_ref_point == CENTER is true
    **/
   obj.vcs_position.x = 7.7346F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_CENTER;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect the REAR_LEFT reference point
   * Expect the min_projection reference point to be equal to the "main" reference point
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, obj.reference_point);
   CHECK_EQUAL(obj.reference_point, obj.min_projection_reference_point);
}

/**
*\purpose  Verify that the reference point algorithm still returns the previous reference point, becuase the hysterisis check fails
*\req NA
*/
TEST(f360_update_object_reference_point, check_hysterisis_select_prev_ref_point)
{
   /** \precond
    * Place object in VCS, such that the previous ref point i.e. LEFT is still selected
    **/
   obj.vcs_position.x = 7.7346F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   calibs.k_normal_obj_ref_pnt_hysteresis_factor = 1.05F;
   
   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect the LEFT reference point
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, obj.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns a new reference point, becuase the hysterisis check is applicable
*          and previous ref point is CENTER
*\req    NA
*/
TEST(f360_update_object_reference_point, check_hysterisis_select_new_ref_point_prev_center)
{
   /** \precond
    * Place object in VCS, such that new ref point i.e. REAR_LEFT is selected instead of the previous CENTER
    **/
   obj.vcs_position.x = 7.7347F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });
   obj.reference_point = F360_REFERENCE_POINT_CENTER;

   /** \action
   * Call function Update_Object_Reference_Point
   **/
   Update_Object_Reference_Point(host_dist_rear_axle_to_vcs_m, f_update_obj_states, f_update_obj_pos_only, f_current_object_behind_another_object, calibs, sensors, globals, obj);

   /** \result
   * Expect the REAR_LEFT reference point
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, obj.reference_point);
}
/** @}*/
