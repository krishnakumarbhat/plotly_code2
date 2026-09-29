/** \file
 * This file contains unit tests for content of f360_assign_underdrivability_status_to_tracks_ocg.cpp file
 */

#include "f360_assign_underdrivability_status_to_tracks_ocg.h"
#include "ocg_occupancy_grid_types.h"
#include "f360_calibrations.h"
#include "f360_clear_object_track.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_host_state_update_ocg
 *  @{
 */

/** \brief
 * This test group tests the Host_State_Update_OCG(), which updates the host states according to the constant velocity motion model
 * and time difference between the OCG module and F360 tracker
 */
TEST_GROUP(f360_host_state_update_ocg)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Host_T host = {};
   ocg::OCG_Outputs_T occupancy_grid = {};

   TEST_SETUP()
   {
      // Set Host Properties
      host.dist_rear_axle_to_vcs_m= 3.45000005F;
      host.yaw_rate_rad= -0.00629700907F;
      host.speed = 34.3173103F;

      // Set Occupancy Grid Properties
      occupancy_grid.timestamp = 697.4010; // Arbitrary

      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.num_cells_y = ocg::NUM_CELLS_Y;

      occupancy_grid.underdrivability.grid_curvature = host.curvature_rear;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

      for (uint8_t i = 0U; i < ocg::NUM_CELLS_X; i++)
      {
         for (uint8_t j = 0U; j < ocg::NUM_CELLS_Y; j++)
         {
            occupancy_grid.underdrivability.underdrivability_classification[i][j].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }
   }
};
/** \purpose
 * This test checks if the host motion compensated position in OGCS is as expected, when the yawrate is nonzero
 */
TEST(f360_host_state_update_ocg, Host_Position_Transformation_with_NonZero_YawRate_Test)
{
   /** \precond
    * Set host yaw_rate_rad equal to some non zero value
    * Set the time difference between OCG and F360tracker module
    * The current host position, in OGCS, undergoes a time update according to the constant velocity model
    */
   host.yaw_rate_rad= -0.2F;

   const float32_t f360_ocg_time_diff = 0.01F; // 10ms

   const float32_t exp_host_ogcs_comp_x = -3.106827175F;
   const float32_t exp_host_ogcs_comp_y = -0.0003477301F;
   const float32_t exp_host_ogcs_comp_yaw = -0.002F;

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   /** \action
    * Call Host_State_Update_OCG() to update the host x, y, and yaw in ogcs according to the constant velocity model
    */
   Host_State_Update_OCG(
      host,
      f360_ocg_time_diff,
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x,
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y,
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw,
      host_ogcs_comp_position,
      host_ogcs_comp_yaw);

   /** \result
    * Check the output host motion compensated states
    */
   DOUBLES_EQUAL_TEXT(exp_host_ogcs_comp_x, host_ogcs_comp_position.x, F360_EPSILON, "The host compensated OGCS_x position is not as expected");
   DOUBLES_EQUAL_TEXT(exp_host_ogcs_comp_y, host_ogcs_comp_position.y, F360_EPSILON, "The host compensated OGCS_y position is not as expected");
   DOUBLES_EQUAL_TEXT(exp_host_ogcs_comp_yaw, host_ogcs_comp_yaw.Value(), F360_EPSILON, "The host compensated OGCS_yaw position is not as expected");
}

/** \purpose
 * This test checks if the host motion compensated position in OGCS is as expected, when the yawrate is zero
 */
TEST(f360_host_state_update_ocg, Host_Position_Transformation_with_Zero_YawRate_Test)
{
   /** \precond
    * Set host yaw_rate_rad equal to zero
    * Set the time difference between OCG and F360tracker module
    * The current host position, in OGCS, undergoes a time update according to the constant velocity model
    */

   host.yaw_rate_rad = 0.0F;

   const float32_t f360_ocg_time_diff = 0.001F; // 1ms

   const float32_t exp_host_ogcs_comp_x = -3.415682739F;
   const float32_t exp_host_ogcs_comp_y = 0.0F;
   const float32_t exp_host_ogcs_comp_yaw = 0.0F;

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;
   /** \action
    * Call Host_State_Update_OCG() to update the host x, y, and yaw in ogcs according to the constant velocity model
    */
   Host_State_Update_OCG(
      host,
      f360_ocg_time_diff,
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x,
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y,
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw,
      host_ogcs_comp_position,
      host_ogcs_comp_yaw);

   /** \result
    * Check the output host motion compensated states
    */
   DOUBLES_EQUAL_TEXT(exp_host_ogcs_comp_x, host_ogcs_comp_position.x, F360_EPSILON, "The host compensated OGCS_x position is not as expected");
   DOUBLES_EQUAL_TEXT(exp_host_ogcs_comp_y, host_ogcs_comp_position.y, F360_EPSILON, "The host compensated OGCS_y position is not as expected");
   DOUBLES_EQUAL_TEXT(exp_host_ogcs_comp_yaw, host_ogcs_comp_yaw.Value(), F360_EPSILON, "The host compensated OGCS_yaw position is not as expected");
}

/** \defgroup  f360_convert_vcs_posn_to_ocgcs_posn
 *  @{
 */

/** \brief
 * This test group tests the Convert_VCS_Posn_To_OCGCS_Posn(), which tranfroms position from VCS to OGCS
 */
TEST_GROUP(f360_convert_vcs_posn_to_ocgcs_posn)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Host_T host = {};
   ocg::OCG_Outputs_T occupancy_grid = {};

   TEST_SETUP()
   {
      // Set Object Properties
      for (uint32_t trk_idx = 0U; trk_idx < NUMBER_OF_OBJECT_TRACKS; trk_idx++)
      {
         Clear_Object_Track(object_tracks[trk_idx]);
      }

      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;

      // Set Occupancy Grid Properties
      occupancy_grid.timestamp = 697.4010; // Arbitrary

      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.num_cells_y = ocg::NUM_CELLS_Y;

      occupancy_grid.underdrivability.grid_curvature = host.curvature_rear;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

      for (uint8_t i = 0U; i < ocg::NUM_CELLS_X; i++)
      {
         for (uint8_t j = 0U; j < ocg::NUM_CELLS_Y; j++)
         {
            occupancy_grid.underdrivability.underdrivability_classification[i][j].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }
   }
};

/** \purpose
 * This tests checks if the conversion of object position from VCS to OCGCS is performed correctly
 */
TEST(f360_convert_vcs_posn_to_ocgcs_posn, Object_Position_Transformation_Test)
{
   /** \precond
    * Setup object position in VCS
    * The host postion here is in OGCS, after compensation for host travel distance and host rear axel distance
    */
   object_tracks[0].vcs_position.x = occupancy_grid.grid_definition.cell_length + 0.01F;
   object_tracks[0].vcs_position.y = 3.01F;

   const float32_t host_ogcs_comp_x = 0.1F;
   const float32_t host_ogcs_comp_y = 0.0F;
   const Angle host_ocgcs_comp_yaw = Angle(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   float32_t obj_ogcs_x, obj_ogcs_y;

   const float32_t exp_obj_ogcs_x = 2.110F;
   const float32_t exp_obj_ogcs_y = 3.0100F;
   /** \action
    * Call Convert_VCS_Posn_To_OCGCS_Posn() for transforming object vcs pos to ogcs pos
    */
   Convert_VCS_Posn_To_OCGCS_Posn(
      object_tracks[0].vcs_position.x,
      object_tracks[0].vcs_position.y,
      host_ogcs_comp_x,
      host_ogcs_comp_y,
      host_ocgcs_comp_yaw,
      obj_ogcs_x,
      obj_ogcs_y);

   /** \result
    * Check output object OGCS position
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogcs_x, obj_ogcs_x, F360_EPSILON, "The transformed object OGCS_x position is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogcs_y, obj_ogcs_y, F360_EPSILON, "The transformed object OGCS_y position is not as expected");
}





/** \defgroup  f360_convert_cartesian_to_curvilinear_coordinates
 *  @{
 */

/** \brief
 * This Test Group tests the Convert_Cartesian_To_Curvilinear_Coordinates(), which is used for transforming position from OGCS to OGCCS
 */
TEST_GROUP(f360_convert_cartesian_to_curvilinear_coordinates)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Calibrations_T calib;
   F360_Host_T host = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   bool f_ocg_use_curvilinear_simplification;

   TEST_SETUP()
   {
      // Calibs
      f_ocg_use_curvilinear_simplification = false;

      // Set Caliberation Properties
      Initialize_Tracker_Calibrations(calib);

      // Set Object Properties
      for (uint32_t trk_idx = 0U; trk_idx < NUMBER_OF_OBJECT_TRACKS; trk_idx++)
      {
         Clear_Object_Track(object_tracks[trk_idx]);
      }

      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;

      // Set Occupancy Grid Properties
      occupancy_grid.timestamp = 697.4010; // Arbitrary

      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.num_cells_y = ocg::NUM_CELLS_Y;

      occupancy_grid.underdrivability.grid_curvature = host.curvature_rear;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

      for (uint8_t i = 0U; i < ocg::NUM_CELLS_X; i++)
      {
         for (uint8_t j = 0U; j < ocg::NUM_CELLS_Y; j++)
         {
            occupancy_grid.underdrivability.underdrivability_classification[i][j].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }
   }
};

/** \purpose
 * This test checks if the curvilinear transform is not performed when input curvature is zero
 */
TEST(f360_convert_cartesian_to_curvilinear_coordinates, Object_OGCS_To_OGCCS_Transformation_Zero_Curvature_Test)
{
   /** \precond
    * Setup object position
    * Set grid curvature equal to zero
    */
   const float32_t obj_ogcs_x = 20.0F;
   const float32_t obj_ogcs_y = -5.0F;

   occupancy_grid.underdrivability.grid_curvature = 0.0F;

   f_ocg_use_curvilinear_simplification = true;

   const float32_t exp_obj_ogccs_x = 20.0F;
   const float32_t exp_obj_ogccs_y = -5.0F;

   float32_t obj_ogccs_x, obj_ogccs_y;
   /** \action
    * Call Convert_VCS_Posn_To_OGCS_Posn() to transform obj_ogcs from OGCS to OGCCS (OG Curvilinear Coordinate System)
    */
   Convert_Cartesian_To_Curvilinear_Coordinates(
      obj_ogcs_x,
      obj_ogcs_y,
      occupancy_grid.underdrivability.grid_curvature,
      calib.k_ocg_underdrive_small_curvature_th,
      f_ocg_use_curvilinear_simplification,
      obj_ogccs_x,
      obj_ogccs_y);

   /** \result
    * The output OGCCS position should be equal to the input OGCS position
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_x, obj_ogccs_x, F360_EPSILON, "The curvilinear tranformation of object OGCS_x position for zero curvature is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_y, obj_ogccs_y, F360_EPSILON, "The curvilinear tranformation of object OGCS_y position for zero curvature is not as expected");
}

/** \purpose
 * This test checks if the curvilinear transform is not performed when input curvature is small and curvilinear simplification is used
 */
TEST(f360_convert_cartesian_to_curvilinear_coordinates, Object_OGCS_To_OGCCS_Transformation_Small_Curvature_Test)
{
   /** \precond
    * Setup object position
    * Set grid curvature equal to zero
    */
   const float32_t obj_ogcs_x = 20.0F;
   const float32_t obj_ogcs_y = -5.0F;

   occupancy_grid.underdrivability.grid_curvature = 0.001F;

   const bool f_ocg_use_curvilinear_simplification = true;

   const float32_t exp_obj_ogccs_x = 20.0F;
   const float32_t exp_obj_ogccs_y = -5.19897461F;

   float32_t obj_ogccs_x, obj_ogccs_y;
   /** \action
    * Call Convert_VCS_Posn_To_OGCS_Posn() to transform obj_ogcs from OGCS to OGCCS (OG Curvilinear Coordinate System)
    */
   Convert_Cartesian_To_Curvilinear_Coordinates(
      obj_ogcs_x,
      obj_ogcs_y,
      occupancy_grid.underdrivability.grid_curvature,
      calib.k_ocg_underdrive_small_curvature_th,
      f_ocg_use_curvilinear_simplification,
      obj_ogccs_x,
      obj_ogccs_y);

   /** \result
    * The output OGCCS position should be equal to the input OGCS position
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_x, obj_ogccs_x, F360_EPSILON, "The curvilinear tranformation of object OGCS_x position for zero curvature is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_y, obj_ogccs_y, F360_EPSILON, "The curvilinear tranformation of object OGCS_y position for zero curvature is not as expected");
}

/** \purpose
 * This test checks the curvilinear transformation when the curvature is high and the input object lateral OGCS position is Negative
 */
TEST(f360_convert_cartesian_to_curvilinear_coordinates, Object_OGCS_To_OGCCS_Transform_High_Curvature_Negative_Pos_Test)
{
   /** \precond
    * Setup object OGCS position. The lateral OGCS position is set to be negative
    * The curvature is relatively high
    * Due to high curvature, the curvilinear simplification should not be used
    */
   const float32_t obj_ogcs_x = 20.0F;
   const float32_t obj_ogcs_y = -5.0F;

   const bool f_ocg_use_curvilinear_simplification = false;

   occupancy_grid.underdrivability.grid_curvature = 0.1F;

   const float32_t exp_obj_ogccs_x = 9.272952F;
   const float32_t exp_obj_ogccs_y = -15.0F;

   float32_t obj_ogccs_x, obj_ogccs_y;
   /** \action
    * Call Convert_VCS_Posn_To_OGCS_Posn() to transform obj_ogcs from OGCS to OGCCS (OG Curvilinear Coordinate System)
    */
   Convert_Cartesian_To_Curvilinear_Coordinates(
      obj_ogcs_x,
      obj_ogcs_y,
      occupancy_grid.underdrivability.grid_curvature,
      calib.k_ocg_underdrive_small_curvature_th,
      f_ocg_use_curvilinear_simplification,
      obj_ogccs_x,
      obj_ogccs_y);

   /** \result
    * Check if the curvilinear transformation is correct
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_x, obj_ogccs_x, F360_EPSILON, "The curvilinear tranformation of object OGCS_x position for high curvature is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_y, obj_ogccs_y, F360_EPSILON, "The curvilinear tranformation of object OGCS_y position for high curvature is not as expected");
}

/** \purpose
 * This test checks the curvilinear transformation when the curvature is of high magnitude and the input object lateral OGCS position is Positive
 */
TEST(f360_convert_cartesian_to_curvilinear_coordinates, Object_OGCS_To_OGCCS_Transform_High_Curvature_Positive_Pos_Test)
{
   /** \precond
    * Setup object OGCS position. The lateral OGCS position is set to be positive
    * The curvature is relatively high
    * Due to high curvature, the curvilinear simplification should not be used
    */
   const float32_t obj_ogcs_x = 5.0F;
   const float32_t obj_ogcs_y = 9.0F;

   const bool f_ocg_use_curvilinear_simplification = false;

   occupancy_grid.underdrivability.grid_curvature = 0.1F;

   const float32_t exp_obj_ogccs_x = 13.734007669450F;
   const float32_t exp_obj_ogccs_y = 4.900980486407F;

   float32_t obj_ogccs_x, obj_ogccs_y;
   /** \action
    * Call Convert_VCS_Posn_To_OGCS_Posn() to transform obj_ogcs from OGCS to OGCCS (OG Curvilinear Coordinate System)
    */
   Convert_Cartesian_To_Curvilinear_Coordinates(
      obj_ogcs_x,
      obj_ogcs_y,
      occupancy_grid.underdrivability.grid_curvature,
      calib.k_ocg_underdrive_small_curvature_th,
      f_ocg_use_curvilinear_simplification,
      obj_ogccs_x,
      obj_ogccs_y);

   /** \result
    * Check if the curvilinear transformation is correct
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_x, obj_ogccs_x, F360_EPSILON, "The curvilinear tranformation of object OGCS_x position for high curvature is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_y, obj_ogccs_y, F360_EPSILON, "The curvilinear tranformation of object OGCS_y position for high curvature is not as expected");
}

/** \purpose
 * This test checks the curvilinear transformation when the curvature is Negative and of high magnitude and the input object lateral OGCS position is Positive
 */
TEST(f360_convert_cartesian_to_curvilinear_coordinates, Object_OGCS_To_OGCCS_Transform_High_Negative_Curvature_Test)
{
   /** \precond
    * Setup object OGCS position. The lateral OGCS position is set to be positive
    * The curvature is relatively high
    */
   const float32_t obj_ogcs_x = 5.0F;
   const float32_t obj_ogcs_y = 9.0F;

   const bool f_ocg_use_curvilinear_simplification = true;

   occupancy_grid.underdrivability.grid_curvature = -0.1F;

   const float32_t exp_obj_ogccs_x = 5.0F;
   const float32_t exp_obj_ogccs_y = 9.64688301F;

   float32_t obj_ogccs_x, obj_ogccs_y;
   /** \action
    * Call Convert_VCS_Posn_To_OGCS_Posn() to transform obj_ogcs from OGCS to OGCCS (OG Curvilinear Coordinate System)
    */
   Convert_Cartesian_To_Curvilinear_Coordinates(
      obj_ogcs_x,
      obj_ogcs_y,
      occupancy_grid.underdrivability.grid_curvature,
      calib.k_ocg_underdrive_small_curvature_th,
      f_ocg_use_curvilinear_simplification,
      obj_ogccs_x,
      obj_ogccs_y);

   /** \result
    * Check if the curvilinear transformation is correct
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_x, obj_ogccs_x, F360_EPSILON, "The curvilinear tranformation of object OGCS_x position for high curvature is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_y, obj_ogccs_y, F360_EPSILON, "The curvilinear tranformation of object OGCS_y position for high curvature is not as expected");
}

/** \purpose
 * This test checks the curvilinear transformation when the curvature is Negative and of high magnitude and the input object lateral OGCS position is Positive
 */
TEST(f360_convert_cartesian_to_curvilinear_coordinates, Object_OGCS_To_OGCCS_Transform__Negative_Alpha_Test)
{
   /** \precond
    * Setup object OGCS position. The lateral OGCS position is set to be positive
    * The curvature is relatively high
    */
   const float32_t obj_ogcs_x = -1.0F;
   const float32_t obj_ogcs_y = 5.0F;

   const bool f_ocg_use_curvilinear_simplification = false;

   occupancy_grid.underdrivability.grid_curvature = -0.1F;

   const float32_t exp_obj_ogccs_x = -0.665681362F;
   const float32_t exp_obj_ogccs_y = 5.03329659F;

   float32_t obj_ogccs_x, obj_ogccs_y;
   /** \action
    * Call Convert_VCS_Posn_To_OGCS_Posn() to transform obj_ogcs from OGCS to OGCCS (OG Curvilinear Coordinate System)
    */
   Convert_Cartesian_To_Curvilinear_Coordinates(
      obj_ogcs_x,
      obj_ogcs_y,
      occupancy_grid.underdrivability.grid_curvature,
      calib.k_ocg_underdrive_small_curvature_th,
      f_ocg_use_curvilinear_simplification,
      obj_ogccs_x,
      obj_ogccs_y);

   /** \result
    * Check if the curvilinear transformation is correct
    */
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_x, obj_ogccs_x, F360_EPSILON, "The curvilinear tranformation of object OGCS_x position for high curvature is not as expected");
   DOUBLES_EQUAL_TEXT(exp_obj_ogccs_y, obj_ogccs_y, F360_EPSILON, "The curvilinear tranformation of object OGCS_y position for high curvature is not as expected");
}





/** \defgroup  f360_calc_in_which_zone_track_is_located_ocg
 *  @{
 */

/** \brief
 * This test group tests the Calc_In_Which_Zone_Track_Is_Located_OCG(), which, for an object, finds the occupancy grid zone indices
 * These indices are inidicative of where the object is located in the occupancy grid and its underdrivability status accordingly
 */
TEST_GROUP(f360_calc_in_which_zone_track_is_located_ocg)
{
   ocg::OCG_Outputs_T occupancy_grid = {};
   F360_Host_T host = {};
   float32_t ocg_lower_left_y;

   TEST_SETUP()
   {
      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;

      // Set Occupancy Grid Properties
      occupancy_grid.timestamp = 697.4010; // Arbitrary

      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.num_cells_y = ocg::NUM_CELLS_Y;

      occupancy_grid.underdrivability.grid_curvature = host.curvature_rear;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

      for (uint8_t i = 0U; i < ocg::NUM_CELLS_X; i++)
      {
         for (uint8_t j = 0U; j < ocg::NUM_CELLS_Y; j++)
         {
            occupancy_grid.underdrivability.underdrivability_classification[i][j].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }

      // The lateral position of the occupancy grid lower left corner
      ocg_lower_left_y = -occupancy_grid.grid_definition.cell_width * occupancy_grid.grid_definition.num_cells_y * 0.5F;
   }
};

/** \purpose
 * This test checks if the correct zones indices are returned, when the object is within the occupancy grid boundaries
 */
TEST(f360_calc_in_which_zone_track_is_located_ocg, Calc_In_Which_Zone_Track_Is_Located_OCG__Inside_Longitudinal_Lateral_Boundary_Test)
{
   /** \precond
    * Setup object position in curvilinear coordinate system
    * The object is longitudinally and laterlly inside the occupancy grid
    */
   // Set Object position in OGCCS
   const float32_t obj_ogccs_x = 18.6000F;
   const float32_t obj_ogccs_y = -2.98F;

   const int16_t exp_zone_idx_long = 9;
   const int16_t exp_zone_idx_lat = 0;

   int16_t zone_idx_long, zone_idx_lat;
   /** \action
    * Call Calc_In_Which_Zone_Track_Is_Located_OCG()
    */
   Calc_In_Which_Zone_Track_Is_Located_OCG(
      obj_ogccs_x,
      obj_ogccs_y,
      ocg_lower_left_y,
      occupancy_grid.grid_definition.cell_width,
      occupancy_grid.grid_definition,
      zone_idx_long,
      zone_idx_lat);

   /** \result
    * Check if the correct longitudinal and lateral zone indices are selected
    */
   CHECK_EQUAL_TEXT(exp_zone_idx_long, zone_idx_long, "The occupancy grid longitudinal zone index is not as expected");
   CHECK_EQUAL_TEXT(exp_zone_idx_lat, zone_idx_lat, "The occupancy grid lateral zone index is not as expected");
}

/** \defgroup  f360_calc_secondary_zone_index
 *  @{
 */

/** \brief
 * This test group tests the Calc_Secondary_Zone_Index(), which returns the secondary zone indices for the occupancy grid
 * This function is used as safety mechanism when an object is located too close to the cell boundaries
 */
TEST_GROUP(f360_calc_secondary_zone_index)
{
   float32_t ocg_cell_length, ocg_cell_width;
   uint16_t num_cells_x, num_cells_y;

   TEST_SETUP()
   {
      ocg_cell_length = 2.0F;
      ocg_cell_width = 6.0F;
      num_cells_x = 50U;
      num_cells_y = 1U;
   }
};

/** \purpose
 * This test checks the calc_secondary_zone_index(), when the first index is zero and the position is nearer to the lower boundary
 */
TEST(f360_calc_secondary_zone_index, Calc_Secondary_Zone_Index__For_Zero_Idx1_Closer_Test)
{
   /** \precond
    * Set zone index to 0
    * Set object position to be closer to the lower cell boundary
    */
   // Set Object position in OGCCS
   int16_t zone_idx_long = 0;
   const float32_t obj_ogccs_x = static_cast<float32_t>(zone_idx_long) * ocg_cell_length + 0.01F;

   const int16_t exp_zone_idx_long_2 = 0;

   /** \action
    * Call Calc_Secondary_Zone_Index()
    */
   const int16_t zone_idx_long_2 = Calc_Secondary_Zone_Index(obj_ogccs_x, ocg_cell_length, num_cells_x, zone_idx_long);

   /** \result
    * Check if the zone_index_2 is as expected
    */
   CHECK_EQUAL_TEXT(exp_zone_idx_long_2, zone_idx_long_2, "The occupancy grid longitudinal zone index is not as expected");
}

/** \purpose
 * This test checks the calc_secondary_zone_index(), when the first index is non zero and the position is nearer to the lower boundary
 */
TEST(f360_calc_secondary_zone_index, Calc_Secondary_Longitudinal_Index_For_Non_Zero_Idx1_Closer_Test)
{
   /** \precond
    * Set zone index to a non zero value
    * Set object position to be closer to the lower cell boundary
    */
   // Set Object position in OGCCS
   int16_t zone_idx_long = 10;
   const float32_t obj_ogccs_x = static_cast<float32_t>(zone_idx_long) * ocg_cell_length + 0.01F;

   const int16_t exp_zone_idx_long_2 = 9;

   /** \action
    * Call Calc_Secondary_Zone_Index()
    */
   const int16_t zone_idx_long_2 = Calc_Secondary_Zone_Index(obj_ogccs_x, ocg_cell_length, num_cells_x, zone_idx_long);

   /** \result
    * Check if the zone_index_2 is as expected
    */
   CHECK_EQUAL_TEXT(exp_zone_idx_long_2, zone_idx_long_2, "The occupancy grid longitudinal zone index is not as expected");
}

/** \purpose
 * This test checks the calc_secondary_zone_index(), when the first index is non zero and the position is nearer to the upper boundary
 */
TEST(f360_calc_secondary_zone_index, Calc_Secondary_Longitudinal_Index_For_Further_Test)
{
   /** \precond
    * Set zone index to a non zero value
    * Set object position to be closer to the upper cell boundary
    */
   // Set Object position in OGCCS
   int16_t zone_idx_long = 10;
   const float32_t obj_ogccs_x = static_cast<float32_t>(zone_idx_long) * ocg_cell_length + (ocg_cell_length - 0.01);

   const int16_t exp_zone_idx_long_2 = 11;

   /** \action
    * Call Calc_Secondary_Zone_Index()
    */
   const int16_t zone_idx_long_2 = Calc_Secondary_Zone_Index(obj_ogccs_x, ocg_cell_length, num_cells_x, zone_idx_long);

   /** \result
    * Check if the zone_index_2 is as expected
    */
   CHECK_EQUAL_TEXT(exp_zone_idx_long_2, zone_idx_long_2, "The occupancy grid longitudinal zone index is not as expected");
}

/** \purpose
 * This test checks the calc_secondary_zone_index(), when the first lateral index is zero and the position is nearer to the right boundary
 */
TEST(f360_calc_secondary_zone_index, Calc_Secondary_Lateral_Index_For_Further_Test)
{
   /** \precond
    * Set zone index to zero
    * Set object position to be closer to the right cell boundary
    */
   // Set Object position in OGCCS
   int16_t zone_idx_lat = 0;
   const float32_t total_gird_width = ocg_cell_width * num_cells_y;
   const float32_t ocg_lower_left_y = -total_gird_width * 0.5F;
   const float32_t obj_ogccs_y = static_cast<float32_t>(zone_idx_lat) * ocg_cell_width + (ocg_cell_width * 0.5F - 0.01F);

   const int16_t exp_zone_idx_lat_2 = 0;

   /** \action
    * Call Calc_Secondary_Zone_Index()
    */
   const int16_t zone_idx_lat_2 = Calc_Secondary_Zone_Index(std::abs(obj_ogccs_y - ocg_lower_left_y), ocg_cell_width, num_cells_y, zone_idx_lat);

   /** \result
    * Check if the zone_index_2 is as expected
    */
   CHECK_EQUAL_TEXT(exp_zone_idx_lat_2, zone_idx_lat_2, "The occupancy grid lateral zone index is not as expected");
}

/** \purpose
 * This test checks the calc_secondary_zone_index(), when there are more than one lateral cells
 */
TEST(f360_calc_secondary_zone_index, Calc_Secondary_Lateral_Index_For_Multiple_Lateral_Cells_Test)
{
   /** \precond
    * Set number of lateral cells to be larger than 1
    * Calculate the occupancy gird lower left lateral position
    * Set object to be laterally inside zone 2
    */
   // Set Object position in OGCCS
   num_cells_y = 4U;
   int16_t zone_idx_lat = 1U;
   const float32_t ocg_lower_left_y = -ocg_cell_width * num_cells_y * 0.5F;
   const float32_t obj_ogccs_y = -0.01F;

   const int16_t exp_zone_idx_lat_2 = 2U;

   /** \action
    * Call Calc_Secondary_Zone_Index()
    */
   const int16_t zone_idx_lat_2 = Calc_Secondary_Zone_Index(std::abs(obj_ogccs_y - ocg_lower_left_y), ocg_cell_width, num_cells_y, zone_idx_lat);

   /** \result
    *  Check if the zone_index_2 is as expected
    */
   CHECK_EQUAL_TEXT(exp_zone_idx_lat_2, zone_idx_lat_2, "The occupancy grid lateral zone index is not as expected");
}





/** \defgroup  pick_wanted_underdrivability_status_ocg
 *  @{
 */

/** \brief
 * Test group of Pick_Wanted_Underdrivability_Status function
 * placed in f360_assign_underdrivability_status_to_tracks_helpers.cpp file.
 */
TEST_GROUP(pick_wanted_underdrivability_status_ocg)
{
   ocg::OCG_Cell_Classification first_status;
   ocg::OCG_Cell_Classification second_status;
   ocg::OCG_Cell_Classification new_status;
};

/** \purpose
 * Purpose of this test is to verify whether when first status is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 * and the second status is UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
 * the function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
 * \req
 * NA.
 */
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__First_Status_Can_Pass_Under)
{
   /** \precond
    * Set first status as UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    * Set second status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
    */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;

   /** \action
    * Call tested function.
    */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
    * Check whether function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, new_status.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when second status is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
* and the first status UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
* \req
* NA.
*/
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__Second_Status_Can_Pass_Under)
{
   /** \precond
   * Set first status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   * Set second status as UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;

   /** \action
   * Call tested function.
   */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, new_status.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when first status is UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
* and the second status is UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* the function returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER.
* \req
* NA.
*/
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__First_Status_Is_Likely_To_Pass_Under)
{
   /** \precond
   * Set the first status as UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   * Set the second status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;

   /** \action
   * Call tested function.
   */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, new_status.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when second status is UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
* and the first UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* the function returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER.
* \req
* NA.
*/
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__Second_Status_Is_Likely_To_Pass_Under)
{
   /** \precond
   * Set first status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   * Set second status as UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;

   /** \action
   * Call tested function.
   */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, new_status.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when first status is UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
* second status is UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* the function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
* \req
* NA.
*/
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__First_Status_Can_Not_Pass_Under)
{
   /** \precond
   * Set first status as UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
   * Set second status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;

   /** \action
   * Call tested function.
   */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, new_status.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when second status is UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
* set first status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
* \req
* NA.
*/
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__Second_Status_Can_Not_Pass_Under)
{
   /** \precond
   * Set first status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   * Set second status as UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
   */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

   /** \action
   * Call tested function.
   */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, new_status.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when both status are UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* function returns UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER.
* \req
* NA.
*/
TEST(pick_wanted_underdrivability_status_ocg, Pick_Wanted_Underdrivability_Status__Both_Not_To_Consider)
{
   /** \precond
   * Set both status as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   first_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   second_status.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;

   /** \action
   * Call tested function.
   */
   Pick_Wanted_Underdrivability_Status_OCG(first_status, second_status, new_status);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, new_status.underdrivability_status);
}


/** \defgroup  object_should_not_be_considered_ocg
 *  @{
 */

/** \brief
 * Test group of Object_Should_Not_Be_Considered function
 * placed in f360_assign_underdrivability_status_to_tracks_helpers.cpp file.
 */
TEST_GROUP(object_should_not_be_considered_ocg)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib;
   ocg::OCG_Outputs_T occupancy_grid;
   F360_OCG_INTERNAL_T ocg_internal;
   float32_t underdrive_max_zone_long_posn;
   F360_Host_T host;

   TEST_SETUP()
   {
      /* Set up a scenario where an object has some default properties to be considered for underdrivable checks
         - Set object position to be in front of host, with lateral position within bounds
      */
      Initialize_Tracker_Calibrations(calib);
      occupancy_grid.grid_definition.num_cells_y = 1U;
      ocg_internal.underdrive_max_zone_long_posn = 100.0F;
      ocg_internal.ocg_max_cell_width = 7.2F;
      ocg_internal.ocg_max_grid_width_half = ocg_internal.ocg_max_cell_width * occupancy_grid.grid_definition.num_cells_y * 0.5F;

      object.vcs_position.y = (ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor) - 1.0F;
      object.vcs_position.x = 10.0F;

      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;
   }
};

/** \purpose
* Purpose of this test is to verify that for an object positioned in front of host, not too far away laterally,
* the function returns that it should be considered for underdrivability check. 
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Object_Should_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   */

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns false
   */
   CHECK_FALSE_TEXT(f_not_to_consider, "Object was incorrectly flagged as not to consider");
}

/** \purpose
* Purpose of this test is to verify that for an object positioned too far behind host, not too far away laterally,
* the function returns that it should not be considered for underdrivability check. In this check we use the default
* calibration settings where calib.k_underdrive_min_trk_long_posn < calib.k_underdrive_min_zone_long_posn meaning that
* "too far behind host" is determined by calib.k_underdrive_min_zone_long_posn. With other words the object is outside
* of the OCG grid area.
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Behind_Host_Outside_OCG_Area_Object_Should_Not_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change object's x position to be too far behind host (just below calib.k_underdrive_min_zone_long_posn)
   */
  object.vcs_position.x = calib.k_underdrive_min_zone_long_posn - 0.1F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns true
   */
   CHECK_TRUE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}

/** \purpose
* Purpose of this test is to verify that for an object positioned too far behind host, not too far away laterally,
* the function returns that it should not be considered for underdrivability check. In this check we use a non-default
* calibration settings where calib.k_underdrive_min_zone_long_posn < calib.k_underdrive_min_trk_long_posn meaning that
* "too far behind host" is determined by calib.k_underdrive_min_trk_long_posn. With other words the object is inside
* of the OCG grid area but anyways to far away behind host.
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Behind_Host_Inside_OCG_Area_Object_Should_Not_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change calib.k_underdrive_min_zone_long_posn to be smaller than calib.k_underdrive_min_zone_long_posn. In this test we choose -10m.
   * Change object's x position to be too far behind host (just below calib.k_underdrive_min_zone_long_posn)
   */
  calib.k_underdrive_min_zone_long_posn = -10.0F;
  object.vcs_position.x = calib.k_underdrive_min_trk_long_posn - 0.1F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns true
   */
   CHECK_TRUE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}


/** \purpose
* Purpose of this test is to verify that for an object positioned too far away laterallay towards the right of host,
* but not too far behind longitudinally, the function returns that it should not be considered for underdrivability check.
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Far_Away_Laterally_To_The_Right_Object_Should_Not_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change object's y position to be too far laterally to the right of host (just above (ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor))
   */
  object.vcs_position.y = (ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor) + 0.1F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns true
   */
   CHECK_TRUE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}

/** \purpose
* Purpose of this test is to verify that for an object positioned too far away laterallay towards the left of host,
* but not too far behind longitudinally, the function returns that it should not be considered for underdrivability check.
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Far_Away_Laterally_To_The_Left_Object_Should_Not_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change object's y position to be too far laterally to the left of host (just below -(ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor))
   */
  object.vcs_position.y = -(ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor) - 0.1F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns true
   */
   CHECK_TRUE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}

/** \purpose
* Purpose of this test is to verify that when the grid has some curvature, an object that is positioned within the grid's lateral limits
* and also within the grid's longitudinal limits, then such object should be considered for UD classification.
* The intention of this test is also to ensure that when host travels on a curved path, then an object's eleigibility for UD classification using OCG
* is decided based on its distance to the host trajectory (grid curvature). 
* If that distance is within occupancy grid lateral limit, then, such object should be considered for UD classification
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Object_Is_Laterally_Withtin_Grid_Range_when_Grid_Is_Curved_Should_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change host.curvature_rear to be non_zero
   * Change object's y position to be laterally within grid lateral limits, even when gird is curved with curvature 0.01
   * Object position x = 70 and y = 21, are chosen such that distance from host trajectory (grid curvature) is less 
   * than the lateral imposed limits (ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor)
   * With this object and grid curvature setup, the object is expected to be 3.5m away from host trajectory
   */
   host.curvature_rear = 0.01F;
   object.vcs_position.x = 70.0F;
   object.vcs_position.y = 21.0F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns false
   */
   CHECK_FALSE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}

/** \purpose
* Purpose of this test is to verify that when the grid has some curvature, an object that is positioned outside the grid's lateral limits,
* but within the grid's longitudinal limits, then such object should not be considered for UD classification.
* The intention of this test is also to ensure that when host travels on a curved path, then an object's eleigibility for UD classification using OCG
* is decided based on its distance to the host trajectory (grid curvature). 
* If that distance is outside occupancy grid lateral limit, then, such object should not be considered for UD classification
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Object_Is_Not_Laterally_Withtin_Grid_Range_when_Grid_Is_Curved_Should_Not_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change host.curvature_rear to be non_zero
   * Change object's y position to be laterally out within grid lateral limits, even when gird is curved with curvature -0.01
   * Object position x = 70 and y = -3, are chosen such that distance from host trajectory (grid curvature) is greater 
   * than the lateral imposed limits (ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor)
   * With this object and grid curvature setup, the object is expected to be 21.5m away from host trajectory
   */
   host.curvature_rear = -0.01F;
   object.vcs_position.x = 70.0F;
   object.vcs_position.y = -3.0F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns true
   */
   CHECK_TRUE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}

/** \purpose
* Purpose of this test is to verify that for an object positioned too far ahead of host, but not too far away laterally,
* the function returns that it should not be considered for underdrivability check.
* \req
* NA.
*/
TEST(object_should_not_be_considered_ocg, Far_Ahead_Of_Host_Outside_OCG_Area_Object_Should_Not_Be_Considered)
{
   /** \precond
   * Default object has been set up in TEST GROUP
   * Change object's x position to be too far ahead of host (just above underdrive_max_zone_long_posn)
   */
  object.vcs_position.x = ocg_internal.underdrive_max_zone_long_posn + 0.1F;

   /** \action
   * Call tested function.
   */
   bool f_not_to_consider = Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal);

   /** \result
   * Check that function returns true
   */
   CHECK_TRUE_TEXT(f_not_to_consider, "Object was incorrectly flagged to be considered");
}

/** @}*/


/** \defgroup  determine_underdrivable_status_ocg
 *  @{
 */

 /** \brief
  * Test group of Determine_Underdrivable_Status function
  * placed in f360_assign_underdrivability_status_to_tracks_ocg.cpp file.
  */
TEST_GROUP(determine_underdrivable_status_ocg)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Calibrations_T calib;
   F360_Tracker_Info_T tracker_info = {};
   F360_Host_T host = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   uint16_t num_cells_x;
   F360_OCG_INTERNAL_T ocg_internal_t;
   ocg::OCG_Cell_Classification new_object_classification;

   TEST_SETUP()
   {
      // Set Caliberation Properties
      Initialize_Tracker_Calibrations(calib);

      // Set tracker Properties
      Set_Tracker_Variant(tracker_info.variant);

      // Set Object Properties
      for (uint32_t trk_idx = 0U; trk_idx < NUMBER_OF_OBJECT_TRACKS; trk_idx++)
      {
         Clear_Object_Track(object_tracks[trk_idx]);
      }

      tracker_info.num_unique_objs = 2;
      tracker_info.active_obj_ids[0U] = 1;
      tracker_info.active_obj_ids[1U] = 2;

      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;

      // Set Occupancy Grid Properties
      occupancy_grid.timestamp = 697.4010; // Arbitrary
      occupancy_grid.f_valid = true;

      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.num_cells_y = ocg::NUM_CELLS_Y;

      occupancy_grid.underdrivability.grid_curvature = host.curvature_rear;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

      ocg_internal_t.ocg_min_x_mid = static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_x_close) * occupancy_grid.grid_definition.cell_length;
      ocg_internal_t.ocg_max_x_mid = (static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_x_close) + static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_x_mid)) * occupancy_grid.grid_definition.cell_length;
      ocg_internal_t.ocg_min_cell_width = occupancy_grid.grid_definition.cell_width;
      ocg_internal_t.ocg_max_cell_width = occupancy_grid.grid_definition.cell_width * occupancy_grid.grid_definition.cell_width_extension_factor;
      ocg_internal_t.ocg_num_cells_x = occupancy_grid.grid_definition.num_cells_x_close + occupancy_grid.grid_definition.num_cells_x_mid + occupancy_grid.grid_definition.num_cells_x_far;

      for (uint8_t i = 0U; i < ocg::NUM_CELLS_X; i++)
      {
         for (uint8_t j = 0U; j < ocg::NUM_CELLS_Y; j++)
         {
            occupancy_grid.underdrivability.underdrivability_classification[i][j].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }
   }
};

/** \purpose
* Purpose of this test is to verify whether index of zone in which track is located is calculated properly.
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Zone_Calculated_Properly)
{
   /** \precond
   * Set longitudinal position of track to be placed on edge of selected zone.
   * Assume that there is no time difference between OCG and F360Tracker modules
   * Set the first zone as UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   * Set the second zone as UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg::OCG_Cell_Classification new_object_classification_first;
   ocg::OCG_Cell_Classification new_object_classification_second;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   host_ogcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   const int16_t selected_long_zone_idx_1 = 10;
   const int16_t selected_lat_zone_idx_1 = 0;
   const int16_t selected_long_zone_idx_2 = 5;
   const int16_t selected_lat_zone_idx_2 = 0;

   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(selected_long_zone_idx_1 < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(selected_lat_zone_idx_1 < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");
   CHECK_TRUE_TEXT(selected_long_zone_idx_2 < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(selected_lat_zone_idx_2 < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");

   object_tracks[0].vcs_position.x = static_cast<float32_t>(selected_long_zone_idx_1) * occupancy_grid.grid_definition.cell_length;
   object_tracks[0].vcs_position.y = static_cast<float32_t>(selected_lat_zone_idx_1) * occupancy_grid.grid_definition.cell_width;

   object_tracks[1].vcs_position.x = static_cast<float32_t>(selected_long_zone_idx_2) * occupancy_grid.grid_definition.cell_length;
   object_tracks[1].vcs_position.y = static_cast<float32_t>(selected_lat_zone_idx_2) * occupancy_grid.grid_definition.cell_width;

   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx_1][selected_lat_zone_idx_1].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx_2][selected_lat_zone_idx_2].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;

   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification_first);

   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[1].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification_second);

   /** \result
   * Check whether first status is equal to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   * Check whether second status is equal to UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, new_object_classification_first.underdrivability_status);
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, new_object_classification_second.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether further zone is selected when it is closer to track.
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Closer_To_Further_Zone)
{
   /** \precond
   * Set longitudinal position of track to be placed in selected zone, closer to further zone edge.
   * Assume that there is no time difference between OCG and F360Tracker modules
   * Set the first underdrivability zone as UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   * Set the second underdrivability zone as UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
   * Set the third underdrivability zone as UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */
   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   ocg_internal_t.host_ocgcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   const int16_t selected_long_zone_idx = 10;
   const int16_t selected_lat_zone_idx = 0;

   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(selected_long_zone_idx < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(selected_lat_zone_idx < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");

   object_tracks[0].vcs_position.x = (static_cast<float32_t>(selected_long_zone_idx) + 0.75F) * occupancy_grid.grid_definition.cell_length;
   object_tracks[0].vcs_position.y = static_cast<float32_t>(selected_lat_zone_idx) * occupancy_grid.grid_definition.cell_width;

   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx + 1U][selected_lat_zone_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx][selected_lat_zone_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx - 1U][selected_lat_zone_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;

   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification);

   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, new_object_classification.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify whether when track is placed in first zone, only this zone is analysed.
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Track_In_First_Zone)
{
   /** \precond
   * Set longitudinal position of track to be placed on edge of first zone.
   * Assume that there is no time difference between OCG and F360Tracker modules
   * Set the first zone as UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   * Set the second zone as UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   ocg_internal_t.host_ocgcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   const int16_t selected_long_zone_idx = 0;
   const int16_t selected_lat_zone_idx = 0;

   object_tracks[0].vcs_position.x = static_cast<float32_t>(selected_long_zone_idx) * occupancy_grid.grid_definition.cell_length;

   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx + 1U][selected_lat_zone_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx][selected_lat_zone_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification);


   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, new_object_classification.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify that "Not_To_Consider" UD status is retuned when the object lateral index is out of bounds
* The object here, is longitudinally within occupancy grid bounds, however, it is placed on the right side of the occupancy grid cell
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Track_On_Right_Out_of_Lateral_Bounds)
{
   /** \precond
   * Set x position of track to be within longitudinal bounds of the OCG
   * Set y_position of track to be outside of lateral bounds on the right side
   * Assume that there is no time difference between OCG and F360Tracker modules
   * This test assumes more than one lateral cells
   */

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   ocg_internal_t.host_ocgcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   object_tracks[0].vcs_position.x = 15.07F;
   object_tracks[0].vcs_position.y = 6.5306467F;

   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification);


   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, new_object_classification.underdrivability_status);
}


/** \purpose
* Purpose of this test is to verify that "Not_To_Consider" UD status is retuned when the object lateral index is out of bounds
* The object here, is longitudinally within occupancy grid bounds, however, it is placed on the left side of the occupancy grid cell
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Track_On_Left_Out_of_Lateral_Bounds)
{
   /** \precond
   * Set x position of track to be within longitudinal bounds of the OCG
   * Set y_position of track to be outside of lateral bounds on the left side
   * Assume that there is no time difference between OCG and F360Tracker modules
   * This test assumes more than one lateral cells
   */

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   ocg_internal_t.host_ocgcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   object_tracks[0].vcs_position.x = 15.07F;
   object_tracks[0].vcs_position.y = -6.5306467F;

   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification);


   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, new_object_classification.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify that "Not_To_Consider" UD status is retuned when the object longitudinal index is out of bounds
* The object here, is laterally within occupancy grid bounds, however, it is placed longitudinally above the last occupancy grid cell
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Track_Out_of_Upper_Longitudinal_Bound)
{
   /** \precond
   * Set x position of track to be above the last cell of OCG, such that it is outside OCG's upper longitudinal bound
   * Set y_position of track to be within OCG's lateral bounds
   * Assume that there is no time difference between OCG and F360Tracker modules
   */

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   ocg_internal_t.host_ocgcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   object_tracks[0].vcs_position.x = (occupancy_grid.grid_definition.cell_length * ocg_internal_t.ocg_num_cells_x) + 0.01F;
   object_tracks[0].vcs_position.y = -2.4F;

   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification);


   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, new_object_classification.underdrivability_status);
}

/** \purpose
* Purpose of this test is to verify that "Not_To_Consider" UD status is retuned when the object longitudinal index is out of bounds
* The object here, is laterally within occupancy grid bounds, however, it is placed longitudinally below the first occupancy grid cell
* \req
* NA.
*/
TEST(determine_underdrivable_status_ocg, Determine_Underdrivable_Status__Track_Out_of_Lower_Longitudinal_Bound)
{
   /** \precond
   * Set x position of track to be below the first cell of OCG, such that it is outside OCG's lower longitudinal bound
   * Set y_position of track to be within OCG's lateral bounds
   * Assume that there is no time difference between OCG and F360Tracker modules
   */

   Point host_ogcs_comp_position;
   Angle host_ogcs_comp_yaw;

   ocg_internal_t.host_ocgcs_comp_position.x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x;
   ocg_internal_t.host_ocgcs_comp_position.y = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y;

   ocg_internal_t.host_ocgcs_comp_yaw.Value(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw);

   object_tracks[0].vcs_position.x = -1.0F;
   object_tracks[0].vcs_position.y = -2.4F;

   /** \action
   * Call tested function.
   */
   Determine_Underdrivable_Status_OCG(
      calib,
      host,
      object_tracks[0].vcs_position,
      occupancy_grid,
      ocg_internal_t,
      new_object_classification);


   /** \result
   * Check whether function returned UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, new_object_classification.underdrivability_status);
}

/** @}*/


/** \defgroup  extract_internal_ocg_grid_data
 *  @{
 */

 /** \brief
  * Test group of Extract_Internal_OCG_Grid_Data() function
  * placed in f360_assign_underdrivability_status_to_tracks_ocg.cpp file.
  */
TEST_GROUP(extract_internal_ocg_grid_data)
{
   ocg::OCG_Outputs_T occupancy_grid = {};
   F360_OCG_INTERNAL_T ocg_internal = {};

   TEST_SETUP()
   {
      // Set some default occupancy grid parameters (exact values are not important)
      occupancy_grid.grid_definition.num_cells_x_close = 10U;
      occupancy_grid.grid_definition.num_cells_x_mid = 30U;
      occupancy_grid.grid_definition.num_cells_x_far = 60U;
      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.6F;

      // Clear ocg_internal data
      ocg_internal.ocg_min_x_mid = 0.0F;
      ocg_internal.ocg_max_x_mid = 0.0F;
      ocg_internal.ocg_min_cell_width = 0.0F;
      ocg_internal.ocg_max_cell_width = 0.0F;
      ocg_internal.ocg_num_cells_x = 0U;
      ocg_internal.underdrive_max_zone_long_posn = 0.0F;
   }
};

/** \purpose
* Purpose of this test is to verify that Extract_Internal_OCG_Grid_Data() are correctly computing and setting fields of ocg_internal
* \req
* NA.
*/
TEST(extract_internal_ocg_grid_data, Check_ocg_internal_Is_Set_Correctly)
{
   /** \precond
   * Default occupancy grid information has been set up in the TEST GROUP
   */

   /** \action
   * Call tested function.
   */
    Extract_Internal_OCG_Grid_Data(occupancy_grid, ocg_internal);


   /** \result
   * Check whether the fields of ocg_interbals has been set correctly. Expected are:
   * ocg_min_x_mid: 20m
   * ocg_max_x_mid: 80m
   * ocg_min_cell_width: 6m
   * ocg_max_cell_width: 9.6m
   * ocg_num_cells_x: 100
   * underdrive_max_zone_long_posn: 200m
   */
   DOUBLES_EQUAL_TEXT(20.0F, ocg_internal.ocg_min_x_mid, F360_EPSILON, "ocg_min_x_mid was incorrectly set");
   DOUBLES_EQUAL_TEXT(80.0F, ocg_internal.ocg_max_x_mid, F360_EPSILON, "ocg_max_x_mid was incorrectly set");
   DOUBLES_EQUAL_TEXT(6.0F, ocg_internal.ocg_min_cell_width, F360_EPSILON, "ocg_min_cell_width was incorrectly set");
   DOUBLES_EQUAL_TEXT(9.6F, ocg_internal.ocg_max_cell_width, F360_EPSILON, "ocg_max_cell_width was incorrectly set");
   CHECK_EQUAL_TEXT(100U, ocg_internal.ocg_num_cells_x, "ocg_num_cells_x was incorrectly set");
   DOUBLES_EQUAL_TEXT(200.0F, ocg_internal.underdrive_max_zone_long_posn, F360_EPSILON, "underdrive_max_zone_long_posn was incorrectly set");
}
/** @}*/

/** \defgroup  preprocess_ocg
 *  @{
 */

 /** \brief
  * Test group of  Preprocess_OCG() function
  * placed in f360_assign_underdrivability_status_to_tracks_ocg.cpp file.
  * Default TEST GROUP default setup is made such that we have a valid occupancy grid and sucg that
  *    - ocg_internal.host_ocgcs_comp_yaw will be 0 if function Host_State_Update_OCG() has not been called but non-zero otherwise
  *    - ocg_internal.ocg_min_cell_width will be 0 if function Extract_Internal_OCG_Grid_Data() has not been called nut non-zero otherwise
  */
TEST_GROUP(preprocess_ocg)
{
      F360_Host_T host;
      ocg::OCG_Outputs_T occupancy_grid;
      float32_t tracker_time_us;
      F360_OCG_INTERNAL_T ocg_internal;

   TEST_SETUP()
   {
      // Clear ocg_internal by setting all fields of ocg_internal to 0
      ocg_internal.ocg_min_cell_width = 0.0F;
      ocg_internal.host_ocgcs_comp_yaw.Value(0.0F);

      // Set up occupancy_grid.f_valid to true such that we enable ocg_internal to be set by Preprocess_OCG()
      occupancy_grid.f_valid = true;

      /* Set up host.dist_rear_axle_to_vcs_m = 3.53 m, host.speed = 34.3173 m/s and host.yaw_rate_rad to 0 and tracker_time_us == occupancy_grid.timestamp such that we know that Host_State_Update_OCG() will set ocg_internal.host_ocgcs_comp_yaw equal to
       occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw. Also set 
       occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw to something non-zero. This will enable the test to check if Host_State_Update_OCG() has
       run or not. If not run then ocg_internal.host_ocgcs_comp_yaw will be 0 (since we clear it to zero here above) but if run then 
       ocg_internal.host_ocgcs_comp_yaw == occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw which is non-zero. */
      host.yaw_rate_rad = 0.0F;
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.speed = 34.3173103F;
      
      tracker_time_us = 10.0F;
      occupancy_grid.timestamp = tracker_time_us;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.5F;
      
      /* Set occupancy_grid.grid_definition.cell_width to something non-zero. This will enable the test to check if Extract_Internal_OCG_Grid_Data() has
       run or not. If not run then ocg_internal.ocg_min_cell_width will be 0 (since we clear it to zero here above) but if run then 
       ocg_internal.ocg_min_cell_width == occupancy_grid.grid_definition.cell_width which is non-zero. */
      occupancy_grid.grid_definition.cell_width = 6.0F;
   }
};

/** \purpose
* Purpose of this test is to verify that Host_State_Update_OCG() and Extract_Internal_OCG_Grid_Data() has been run when there is a valid occupancy grid
* \req
* NA.
*/
TEST(preprocess_ocg, Subfunctions_Run)
{
   /** \precond
   * Default set up from TEST GROUP is used
   */

   /** \action
   * Call Preprocess_OCG().
   */
    Preprocess_OCG(host, &occupancy_grid, tracker_time_us, ocg_internal);


   /** \result
   * Expected result is that ocg_internal.host_ocgcs_comp_yaw and ocg_internal.ocg_min_cell_width are both non-zero (indicating that Host_State_Update_OCG() and Extract_Internal_OCG_Grid_Data() has been run)
   */
   CHECK_TRUE_TEXT(std::abs(ocg_internal.host_ocgcs_comp_yaw.Value()) > F360_EPSILON, "Host_State_Update_OCG() was not run despite the OCG being valid");
   CHECK_TRUE_TEXT(std::abs(ocg_internal.ocg_min_cell_width) > F360_EPSILON, "Extract_Internal_OCG_Grid_Data() was not run despite the OCG being valid");
}

/** \purpose
* Purpose of this test is to verify that Host_State_Update_OCG() and Extract_Internal_OCG_Grid_Data() are not run when there is a non-valid occupancy grid
* \req
* NA.
*/
TEST(preprocess_ocg, Subfunctions_Not_Run_Invalid_OCG)
{
   /** \precond
   * Default set up from TEST GROUP is used except for:
   *    - Change occupancy_grid.f_valid to false
   */
  occupancy_grid.f_valid = false;

   /** \action
   * Call Preprocess_OCG().
   */
    Preprocess_OCG(host, &occupancy_grid, tracker_time_us, ocg_internal);


   /** \result
   * Expected result is that ocg_internal.host_ocgcs_comp_yaw and ocg_internal.ocg_min_cell_width are both zero (indicating that Host_State_Update_OCG() and Extract_Internal_OCG_Grid_Data() has not been run)
   */
   CHECK_TRUE_TEXT(std::abs(ocg_internal.host_ocgcs_comp_yaw.Value()) < F360_EPSILON, "Host_State_Update_OCG() was run despite the OCG being invalid");
   CHECK_TRUE_TEXT(std::abs(ocg_internal.ocg_min_cell_width) < F360_EPSILON, "Extract_Internal_OCG_Grid_Data() was run despite the OCG being invalid");
}

/** \purpose
* Purpose of this test is to verify that Host_State_Update_OCG() and Extract_Internal_OCG_Grid_Data() are not run when there is a non-valid pointer to the occupancy grid
* \req
* NA.
*/
TEST(preprocess_ocg, Subfunctions_Not_Run_Invalid_OCG_Pointer)
{
   /** \precond
   * Default set up from TEST GROUP is used.
   */

   /** \action
   * Call Preprocess_OCG() with NULL as input argument instead of the true pointer to the OCG
   */
    Preprocess_OCG(host, NULL, tracker_time_us, ocg_internal);


   /** \result
   * Expected result is that ocg_internal.host_ocgcs_comp_yaw and ocg_internal.ocg_min_cell_width are both zero (indicating that Host_State_Update_OCG() and Extract_Internal_OCG_Grid_Data() has not been run)
   */
   CHECK_TRUE_TEXT(std::abs(ocg_internal.host_ocgcs_comp_yaw.Value()) < F360_EPSILON, "Host_State_Update_OCG() was run despite the pointer to the OCG being invalid");
   CHECK_TRUE_TEXT(std::abs(ocg_internal.ocg_min_cell_width) < F360_EPSILON, "Extract_Internal_OCG_Grid_Data() was run despite the poionter to the OCG being invalid");
}


/** \purpose
* Purpose of this test is to verify that when there is a non_zero time difference between the tracker and OCG input
* then the Host_State_Update_OCG() should correctly compensate the host position in OGCS coordinates
* This test keeps into account that the input timestamps of OCG and tracker are in microsecond,
* while the Host_State_Update_OCG() uses time difference in seconds
* \req
* NA.
*/
TEST(preprocess_ocg, Check_Host_OCGS_comp_position_when_time_diff_is_non_zero)
{
   /** \precond
   * Set OCG timestamp to be 1ms behind the Tracker timestamp
   * OCG timestamp and Tracker timestamp are in microseconds
   * Set up host position in occupancy grid coordinate system (OCGS)
   * Host and OCG orientation are aligned
   * The expected compensated host x position in OGCS will be original x position minus the straight line distance travelled by host in 1ms
   * The expected compensated host y position should remain unchanged
   * The expected compensated host yaw should remain unchanged
   */
   const float32_t f360_ocg_time_diff_us = 1000.0F;
   occupancy_grid.timestamp = tracker_time_us - f360_ocg_time_diff_us;
   occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
   occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
   occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
   occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

   const float32_t expected_host_ocgcs_comp_position_x = occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x + (host.speed * (f360_ocg_time_diff_us * 1e-6F));
   const float32_t expected_host_ocgcs_comp_position_y =  0.0F;
   const float32_t expected_host_ocgcs_comp_yaw = 0.0F;
   
   /** \action
   * Call Preprocess_OCG().
   */
   Preprocess_OCG(host, &occupancy_grid, tracker_time_us, ocg_internal);


   /** \result
   * Check the output host motion compensated states 
   */
   DOUBLES_EQUAL_TEXT(expected_host_ocgcs_comp_position_x, ocg_internal.host_ocgcs_comp_position.x, F360_EPSILON, "Host_State_Update_OCG() was run incorrectly when time diff is non zero");
   DOUBLES_EQUAL_TEXT(expected_host_ocgcs_comp_position_y, ocg_internal.host_ocgcs_comp_position.y, F360_EPSILON, "Host_State_Update_OCG() was run incorrectly when time diff is non zero");
   DOUBLES_EQUAL_TEXT(expected_host_ocgcs_comp_yaw, ocg_internal.host_ocgcs_comp_yaw.Value(), F360_EPSILON,"Host_State_Update_OCG() was run incorrectly when time diff is non zero");
}
/** @}*/
