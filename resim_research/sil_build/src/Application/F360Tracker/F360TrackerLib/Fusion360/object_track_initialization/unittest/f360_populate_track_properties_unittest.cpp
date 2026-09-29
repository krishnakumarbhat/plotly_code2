/** \file
 * This file contains unit tests for content of f360_populate_track_properties.cpp file
 */

#include "../source/f360_populate_track_properties.cpp"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_track_properties_Determine_Track_Position
 *  @{
 */

/** \brief
   Group for testing Determine_Track_Position
 */
TEST_GROUP(f360_populate_track_properties_Determine_Track_Position)
{

   F360_Cluster_T cluster{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Object_Track_T obj{};

   // Set host heading, create the cluster with 3 detections and fill those detections positions.
   TEST_SETUP()
   {
      (void)memset(det_props, 0, sizeof(det_props));

      obj.vcs_heading.Value(0.01F);

      cluster.ndets = 3;
      cluster.detids[0] = 1;
      cluster.detids[1] = 2;
      cluster.detids[2] = 4;

      det_props[0].vcs_position.x = 24.1F;
      det_props[0].vcs_position.y = 45.2F;

      det_props[1].vcs_position.x = 24.15F;
      det_props[1].vcs_position.y = 46.0F;

      det_props[3].vcs_position.x = 24.5F;
      det_props[3].vcs_position.y = 44.8F;

   }

};

/** \purpose
    Check whether the position is calculated properly with respect to the front_right reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_front_right)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_FRONT_RIGHT
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */
   CHECK_TRUE((obj.bbox.Get_Center().x - 24.4879665F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 46.003376F < F360_EPSILON));
}

/** \purpose
   Check whether the position is calculated properly with respect to the front_left reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_front_left)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_FRONT_LEFT
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */
   CHECK_TRUE((obj.bbox.Get_Center().x - 24.5F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 44.7999992F < F360_EPSILON));
}

/** \purpose
   Check whether the position is calculated properly with respect to the front reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_front)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_FRONT
    */
   obj.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */
   CHECK_TRUE((obj.bbox.Get_Center().x - 24.4939823F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 45.4016876F < F360_EPSILON));
}

/** \purpose
 * Check whether the position is calculated properly with respect to the right reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_right)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_RIGHT
    */
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */

   CHECK_TRUE((obj.bbox.Get_Center().x - 24.2899857F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 46.0013962F < F360_EPSILON));
 }

/** \purpose
 * Check whether the position is calculated properly with respect to the left reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_left)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_LEFT
    */
   obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */

   CHECK_TRUE((obj.bbox.Get_Center().x - 24.3020191F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 44.7980194F < F360_EPSILON));
 }

/** \purpose
 * Check whether the position is calculated properly with respect to the rear right reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_rear_right)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_REAR_RIGHT
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */

   CHECK_TRUE((obj.bbox.Get_Center().x - 24.0920048F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 45.9994164F < F360_EPSILON));
 }

/** \purpose
 *  Check whether the position is calculated properly with respect to the rear left reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_rear_left)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_REAR_LEFT
    */
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */

   CHECK_TRUE((obj.bbox.Get_Center().x - 24.1040382F < F360_EPSILON)&&(obj.bbox.Get_Center().y - 44.7960396F < F360_EPSILON));
 }

/** \purpose
 *  Check whether the position is calculated properly with respect to the rear reference point.
 */
TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_rear)
{
   /** \precond
    * set object reference point to F360_REFERENCE_POINT_REAR
    */
	obj.reference_point = F360_REFERENCE_POINT_REAR;
   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */

    CHECK_TRUE((obj.bbox.Get_Center().x - 24.2960014F < F360_EPSILON) && (obj.bbox.Get_Center().y - 45.3997078F < F360_EPSILON));
 }

/** \purpose
 * Check whether the position is calculated properly with respect to the default reference point.
 */
 TEST(f360_populate_track_properties_Determine_Track_Position, check_track_position_ref_point_default)
{
   /** \precond
    * Dont set any referece point explicitly
    */

   /** \action
    * Call Determine_Track_Position()
    */
   Determine_Track_Position(cluster, det_props, obj);

   /** \result
    * Check if position of the object matches expected positon.
    */

    CHECK_TRUE((obj.bbox.Get_Center().x - 24.2960014F < F360_EPSILON) && (obj.bbox.Get_Center().y - 45.3997078F < F360_EPSILON));
 }

/** \defgroup  f360_populate_track_properties_Determine_Refpoint_And_Size
 *  @{
 */

/** \brief
 * Group for testing Determine_Refpoint_And_Size
 */
TEST_GROUP(f360_populate_track_properties_Determine_Refpoint_And_Size)
{
   /** \setup
    * Sets up host, calibration, globals, sensors, clusters and object data required for testing.
    */
   F360_Host_T host{};
   F360_Calibrations_T calib{};
   F360_Globals_T globals{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Object_Track_T obj{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};

   TEST_SETUP()
   {
      (void)memset(sensors, 0, sizeof(sensors));
      (void)memset(det_props, 0, sizeof(det_props));
      (void)memset(&obj, 0, sizeof(obj));

      Initialize_Tracker_Calibrations(calib);
      obj.movable_prob = 0.6F;
      obj.vcs_heading.Value(0.0F);
      host.dist_rear_axle_to_vcs_m = 2.0F;
      globals.f_single_front_center_radar_only = true;
   }
};

/** \purpose
 * Check if non-movable object (movable_prob <= threshold) uses default dimensions.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, non_movable_object)
{
   /** \precond
    * Set movable_prob to 0.3 (below threshold of 0.5)
    */
   obj.movable_prob = 0.3F;

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should return false and use non-movable object dimensions
    * bbox height should be 0.5F
    */
   CHECK_FALSE(result);
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, obj.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, obj.min_projection_reference_point);
   CHECK_EQUAL(0.5F, obj.bbox_height);
   CHECK_EQUAL(calib.k_nonmoveable_target_diameter, obj.bbox.Get_Length());
   CHECK_EQUAL(calib.k_nonmoveable_target_diameter, obj.bbox.Get_Width());
}

/** \purpose
 * Check if slow moving object with all conditions TRUE for length calculation.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_all_conditions_true)
{
   /** \precond
    * Set object as movable (movable_prob > 0.5)
    * Speed < fast_moving_thresh (e.g., 2.9 m/s < 3.0 m/s)
    * Position within range: x < 20.0, y < 25.0
    * ndets > 7
    * Is_Pca_Principal_Dir_Close_To_Heading returns true (aligned detections)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   // Setup 10 detections aligned along heading
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i * 1.5F;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should return true, use slow moving dimensions (1.0 length, 1.0 width)
    * and allow length calculation
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Width());
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, obj.reference_point);
   CHECK_EQUAL(1.0F, obj.bbox_height);
}

/** \purpose
 * Check slow moving object with x position OUT OF RANGE (x >= 20.0).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_x_out_of_range_positive)
{
   /** \precond
    * Speed < fast_moving_thresh
    * x position >= 20.0 (fails x range check)
    * Other conditions would be satisfied
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = 20.0F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default slow moving length (1.0) since condition failed
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Length());
}

/** \purpose
 * Check slow moving object with x position OUT OF RANGE (x <= -20.0).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_x_out_of_range_negative)
{
   /** \precond
    * Speed < fast_moving_thresh
    * x position <= -20.0 (fails x range check)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = -20.0F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = -1.0F - i;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default slow moving length (1.0)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Length());
}

/** \purpose
 * Check slow moving object with y position OUT OF RANGE (y >= 25.0).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_y_out_of_range_positive)
{
   /** \precond
    * Speed < fast_moving_thresh
    * y position >= 25.0 (fails y range check)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = 25.0F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default slow moving length (1.0)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Length());
}

/** \purpose
 * Check slow moving object with y position OUT OF RANGE (y <= -25.0).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_y_out_of_range_negative)
{
   /** \precond
    * Speed < fast_moving_thresh
    * y position <= -25.0 (fails y range check)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = -25.0F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default slow moving length (1.0)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Length());
}

/** \purpose
 * Check slow moving object with insufficient detections (ndets <= 7).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_insufficient_detections)
{
   /** \precond
    * Speed < fast_moving_thresh
    * Position within range
    * ndets = 7 (fails ndets > 7 check)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 7U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 7; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 3);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default slow moving length (1.0)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Length());
}

/** \purpose
 * Check slow moving object with PCA check failing (direction not aligned).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, slow_moving_pca_check_fails)
{
   /** \precond
    * Speed < fast_moving_thresh
    * Position within range
    * ndets > 7
    * But detections not aligned with heading (PCA fails)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 2.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   // Setup detections perpendicular to heading (will fail PCA check)
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 0.05F * (i - 5);
      det_props[i].vcs_position.y = 1.0F + i;
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default slow moving length (1.0) since PCA check failed
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.0F, obj.bbox.Get_Length());
}

/** \purpose
 * Check fast moving object with all conditions TRUE for length calculation.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, fast_moving_all_conditions_true)
{
   /** \precond
    * Speed >= fast_moving_thresh (e.g., 3.0 m/s) and < k_init_fast_moving_upper_threshold (e.g., 5.0 m/s)
    * Position within range
    * ndets > 7
    * Detections aligned with heading
    */
   obj.movable_prob = 0.6F;
   obj.speed = 3.0F;
   obj.vcs_position.x = -19.9F;
   obj.vcs_position.y = -24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i * 1.5F;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should return true, use fast moving dimensions (3.0 length, 1.0 width)
    * and allow length calculation
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(3.0F, obj.bbox.Get_Length());
   CHECK_EQUAL(1.0F, obj.bbox.Get_Width());
   CHECK_EQUAL(1.0F, obj.bbox_height);
}

/** \purpose
 * Check fast moving object with x OUT OF RANGE.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, fast_moving_x_out_of_range)
{
   /** \precond
    * Speed in fast moving range
    * x position >= 20.0
    */
   obj.movable_prob = 0.6F;
   obj.speed = 4.9F;
   obj.vcs_position.x = -20.0F;
   obj.vcs_position.y = -24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default fast moving length (1.75)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.75F, obj.bbox.Get_Length());
}

/** \purpose
 * Check fast moving object with y OUT OF RANGE.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, fast_moving_y_out_of_range)
{
   /** \precond
    * Speed in fast moving range
    * y position >= 25.0
    */
   obj.movable_prob = 0.6F;
   obj.speed = 4.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = -25.0F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 5);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default fast moving length (1.75)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.75F, obj.bbox.Get_Length());
}

/** \purpose
 * Check fast moving object with insufficient detections.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, fast_moving_insufficient_detections)
{
   /** \precond
    * Speed in fast moving range
    * ndets = 6 (fails ndets > 7 check)
    */
   obj.movable_prob = 0.6F;
   obj.speed = 4.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 6U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 6; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 3);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default fast moving length (1.75)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.75F, obj.bbox.Get_Length());
}

/** \purpose
 * Check fast moving object with PCA check failing.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, fast_moving_pca_check_fails)
{
   /** \precond
    * Speed in fast moving range
    * Other conditions satisfied but detections perpendicular to heading
    */
   obj.movable_prob = 0.6F;
   obj.speed = 4.9F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = 24.9F;
   obj.ndets = 10U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 10; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 0.05F * (i - 5);
      det_props[i].vcs_position.y = 1.0F + i;
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use default fast moving length (1.75)
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(1.75F, obj.bbox.Get_Length());
}

/** \purpose
 * Check very fast moving object (speed >= k_init_fast_moving_upper_threshold).
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, very_fast_moving_object)
{
   /** \precond
    * Speed >= k_init_fast_moving_upper_threshold (e.g., 25 m/s)
    * All position conditions satisfied or not, doesn't matter for very fast objects
    */
   obj.movable_prob = 0.6F;
   obj.speed = 5.1F;
   obj.vcs_position.x = 20.0F;
   obj.vcs_position.y = -25.0F;
   obj.ndets = 3;
   obj.vcs_heading.Value(0.0F);

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use very fast moving dimensions (4.0 length, 1.5 width)
    * No length calculation attempt for very fast objects
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(4.0F, obj.bbox.Get_Length());
   CHECK_EQUAL(1.5F, obj.bbox.Get_Width());
   CHECK_EQUAL(1.0F, obj.bbox_height);
}

/** \purpose
 * Check very fast moving object with all conditions would be TRUE but speed prevents calculation.
 */
TEST(f360_populate_track_properties_Determine_Refpoint_And_Size, very_fast_moving_no_length_calc)
{
   /** \precond
    * Speed >= k_init_fast_moving_upper_threshold
    * All length calculation conditions would be TRUE if checked
    */
   obj.movable_prob = 0.6F;
   obj.speed = 35.0F;
   obj.vcs_position.x = 19.9F;
   obj.vcs_position.y = -25.0F;
   obj.ndets = 15U;
   obj.vcs_heading.Value(0.0F);
   
   for (int i = 0; i < 15; i++)
   {
      obj.detids[i] = i + 1;
      det_props[i].vcs_position.x = 1.0F + i;
      det_props[i].vcs_position.y = 0.05F * (i - 7);
   }

   /** \action
    * Call Determine_Refpoint_And_Size()
    */
   bool result = Determine_Refpoint_And_Size(host, calib, globals, sensors, det_props, obj);

   /** \result
    * Should use very fast moving dimensions without attempting length calculation
    */
   CHECK_TRUE(result);
   CHECK_EQUAL(4.0F, obj.bbox.Get_Length());
   CHECK_EQUAL(1.5F, obj.bbox.Get_Width());
}


/** @}*/
/** \defgroup  f360_populate_track_properties_Compute_Length_Line
 *  @{
 */

/** \brief
 * Group for testing Compute_Length_Line
 */
TEST_GROUP(f360_populate_track_properties_Compute_Length_Line)
{
   /** \setup
    * Sets up detection properties and object data required for testing.
    */
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Object_Track_T obj{};

   TEST_SETUP()
   {
      (void)memset(det_props, 0, sizeof(det_props));
      (void)memset(&obj, 0, sizeof(obj));
   }
};

/** \purpose
 * Check if length computation returns 0 when no detections are in the track.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, zero_detections_returns_zero)
{
   /** \precond
    * Object has no detections (ndets = 0)
    */
   obj.ndets = 0;
   obj.vcs_heading.Value(0.0F);

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be 0.0F
    */
   CHECK_EQUAL(0.0F, result);
}

/** \purpose
 * Check if length computation returns 0 when only one detection is in the track.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, single_detection_returns_zero)
{
   /** \precond
    * Object has one detection at (5.0, 3.0)
    * Heading is 0.0F (along x-axis)
    */
   obj.ndets = 1;
   obj.detids[0] = 1;
   obj.vcs_heading.Value(0.0F);
   det_props[0].vcs_position.x = 5.0F;
   det_props[0].vcs_position.y = 3.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be 0.0F (min and max projections are equal)
    */
   CHECK_EQUAL(0.0F, result);
}

/** \purpose
 * Check if length computation is correct for collinear detections along heading direction.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, collinear_detections_along_heading)
{
   /** \precond
    * Three detections aligned along heading (0 radians / x-axis)
    * Positions: (2.0, 0.0), (5.0, 0.0), (8.0, 0.0)
    * Expected length: 8.0 - 2.0 = 6.0
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);

   det_props[0].vcs_position.x = 2.0F;
   det_props[0].vcs_position.y = 0.0F;

   det_props[1].vcs_position.x = 5.0F;
   det_props[1].vcs_position.y = 0.0F;

   det_props[2].vcs_position.x = 8.0F;
   det_props[2].vcs_position.y = 0.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   const float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be 6.0F
    */
   DOUBLES_EQUAL(6.0F, result, F360_EPSILON);
}

/** \purpose
 * Check if length computation is correct for detections perpendicular to heading.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, detections_perpendicular_to_heading)
{
   /** \precond
    * Three detections aligned perpendicular to heading (0 radians)
    * Positions: (0.0, 2.0), (0.0, 5.0), (0.0, 8.0)
    * Expected projection along x-axis: all 0.0
    * Expected length: 0.0 - 0.0 = 0.0
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);

   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 2.0F;

   det_props[1].vcs_position.x = 0.0F;
   det_props[1].vcs_position.y = 5.0F;

   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = 8.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, F360_EPSILON);
}

/** \purpose
 * Check if length computation is correct for detections at 45-degree heading.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, detections_at_45_degree_heading)
{
   /** \precond
    * Three detections aligned at 45 degrees
    * Positions: (2.0, 2.0), (5.0, 5.0), (8.0, 8.0)
    * Heading: 45 degrees (pi/4 radians)
    * cos(45 deg) = sin(45 deg) = sqrt(2)/2 ~ 0.7071
    * Projection = x*cos + y*sin = (2*0.7071 + 2*0.7071), (5*0.7071 + 5*0.7071), (8*0.7071 + 8*0.7071)
    * = 2.828, 7.071, 11.314
    * Expected length: 11.314 - 2.828 ~ 8.486
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(F360_PI / 4.0F);

   det_props[0].vcs_position.x = 2.0F;
   det_props[0].vcs_position.y = 2.0F;

   det_props[1].vcs_position.x = 5.0F;
   det_props[1].vcs_position.y = 5.0F;

   det_props[2].vcs_position.x = 8.0F;
   det_props[2].vcs_position.y = 8.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be approximately 8.485 (+/-0.01)
    */
   DOUBLES_EQUAL(8.485F, result, 0.01F);
}

/** \purpose
 * Check if length computation correctly handles negative coordinates.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, negative_coordinates)
{
   /** \precond
    * Three detections with negative coordinates
    * Positions: (-8.0, 0.0), (-5.0, 0.0), (-2.0, 0.0)
    * Heading: 0.0F (along x-axis)
    * Expected length: -2.0 - (-8.0) = 6.0
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);

   det_props[0].vcs_position.x = -8.0F;
   det_props[0].vcs_position.y = 0.0F;

   det_props[1].vcs_position.x = -5.0F;
   det_props[1].vcs_position.y = 0.0F;

   det_props[2].vcs_position.x = -2.0F;
   det_props[2].vcs_position.y = 0.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be 6.0F
    */
   DOUBLES_EQUAL(6.0F, result, F360_EPSILON);
}

/** \purpose
 * Check if length computation is correct for multiple detections with unordered indices.
 */
TEST(f360_populate_track_properties_Compute_Length_Line, multiple_detections_with_gaps)
{
   /** \precond
    * Four detections with detection IDs that have gaps: 1, 3, 5, 7
    * Positions: (1.0, 0.0), (3.0, 0.0), (5.0, 0.0), (7.0, 0.0)
    * Heading: 0.0F
    * Expected length: 7.0 - 1.0 = 6.0
    */
   obj.ndets = 4;
   obj.detids[0] = 1;
   obj.detids[1] = 3;
   obj.detids[2] = 5;
   obj.detids[3] = 7;
   obj.vcs_heading.Value(0.0F);

   det_props[0].vcs_position.x = 1.0F;
   det_props[0].vcs_position.y = 0.0F;

   det_props[2].vcs_position.x = 3.0F;
   det_props[2].vcs_position.y = 0.0F;

   det_props[4].vcs_position.x = 5.0F;
   det_props[4].vcs_position.y = 0.0F;

   det_props[6].vcs_position.x = 7.0F;
   det_props[6].vcs_position.y = 0.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be 6.0F
    */
   DOUBLES_EQUAL(6.0F, result, F360_EPSILON);
}

/** \purpose
 * Check if length computation is correct when heading is at 180 degrees (negative x direction).
 */
TEST(f360_populate_track_properties_Compute_Length_Line, heading_at_180_degrees)
{
   /** \precond
    * Three detections aligned along negative x-axis
    * Positions: (-8.0, 0.0), (-5.0, 0.0), (-2.0, 0.0)
    * Heading: 180 degrees (pi radians)
    * cos(180 deg) = -1, sin(180 deg) = 0
    * Projections: -8, -5, -2
    * Expected length: -2 - (-8) = 6.0
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(F360_PI);

   det_props[0].vcs_position.x = -8.0F;
   det_props[0].vcs_position.y = 0.0F;

   det_props[1].vcs_position.x = -5.0F;
   det_props[1].vcs_position.y = 0.0F;

   det_props[2].vcs_position.x = -2.0F;
   det_props[2].vcs_position.y = 0.0F;

   /** \action
    * Call Compute_Length_Line()
    */
   float32_t result = Compute_Length_Line(det_props, obj);

   /** \result
    * Result should be approximately 6.0F
    */
   DOUBLES_EQUAL(6.0F, result, F360_EPSILON);
}

/** @}*/

/** \defgroup  f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading
 *  @{
 */

/** \brief
 * Group for testing Is_Pca_Principal_Dir_Close_To_Heading
 */
TEST_GROUP(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading)
{
   /** \setup
    * Sets up detection properties and object data required for testing.
    */
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Object_Track_T obj{};

   TEST_SETUP()
   {
      (void)memset(det_props, 0, sizeof(det_props));
      (void)memset(&obj, 0, sizeof(obj));
   }
};

/** \purpose
 * Check if function returns false when track has zero detections.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, zero_detections)
{
   /** \precond
    * Object has no detections (ndets = 0)
    */
   obj.ndets = 0U;
   obj.vcs_heading.Value(0.0F);

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return false (count < 3)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if function returns false when track has only one detection.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, single_detection)
{
   /** \precond
    * Object has one detection
    */
   obj.ndets = 1U;
   obj.detids[0] = 1U;
   obj.vcs_heading.Value(0.0F);
   det_props[0].vcs_position.x = 5.0F;
   det_props[0].vcs_position.y = 3.0F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return false (count < 3)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if function returns false when track has two detections.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, two_detections)
{
   /** \precond
    * Object has two detections
    */
   obj.ndets = 2U;
   obj.detids[0] = 1U;
   obj.detids[1] = 2U;
   obj.vcs_heading.Value(0.0F);
   det_props[0].vcs_position.x = 2.0F;
   det_props[0].vcs_position.y = 1.0F;
   det_props[1].vcs_position.x = 5.0F;
   det_props[1].vcs_position.y = 3.0F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return false (count < 3)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if function returns false when detections have degenerate covariance.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, degenerate_covariance)
{
   /** \precond
    * Three detections at the same location (degenerate covariance)
    * Positions: (5.0, 3.0), (5.0, 3.0), (5.0, 3.0)
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);
   det_props[0].vcs_position.x = 5.0F;
   det_props[0].vcs_position.y = 3.0F;
   det_props[1].vcs_position.x = 5.0F;
   det_props[1].vcs_position.y = 3.0F;
   det_props[2].vcs_position.x = 5.0F;
   det_props[2].vcs_position.y = 3.0F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return false (trace < 1e-4)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if function returns true when principal direction is aligned with heading (0 degrees).
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, aligned_with_heading_0_degrees)
{
   /** \precond
    * Three detections aligned along x-axis (heading 0 degrees)
    * Positions: (1.0, 0.0), (5.0, 0.1), (10.0, -0.1)
    * Heading: 0.0F
    */
   obj.ndets = 3U;
   obj.detids[0] = 1U;
   obj.detids[1] = 2U;
   obj.detids[2] = 3U;
   obj.vcs_heading.Value(0.0F);
   det_props[0].vcs_position.x = 1.0F;
   det_props[0].vcs_position.y = 0.0F;
   det_props[1].vcs_position.x = 5.0F;
   det_props[1].vcs_position.y = 0.1F;
   det_props[2].vcs_position.x = 10.0F;
   det_props[2].vcs_position.y = -0.1F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return true (direction close to heading)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if function returns true when principal direction is aligned with heading (90 degrees).
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, aligned_with_heading_90_degrees)
{
   /** \precond
    * Three detections aligned along y-axis (heading 90 degrees)
    * Positions: (0.0, 1.0), (0.1, 5.0), (-0.1, 10.0)
    * Heading: 90 degrees (pi/2 radians)
    */
   obj.ndets = 3U;
   obj.detids[0] = 1U;
   obj.detids[1] = 2U;
   obj.detids[2] = 3U;
   obj.vcs_heading.Value(F360_PI / 2.0F);
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 1.0F;
   det_props[1].vcs_position.x = 0.1F;
   det_props[1].vcs_position.y = 5.0F;
   det_props[2].vcs_position.x = -0.1F;
   det_props[2].vcs_position.y = 10.0F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return true (direction close to heading)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if function returns false when principal direction differs significantly from heading.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, direction_differs_from_heading)
{
   /** \precond
    * Three detections aligned along y-axis (perpendicular to heading)
    * Positions: (0.0, 1.0), (0.0, 5.0), (0.0, 10.0)
    * Heading: 0.0F (along x-axis, 90 degrees away from principal direction)
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 1.0F;
   det_props[1].vcs_position.x = 0.0F;
   det_props[1].vcs_position.y = 5.0F;
   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = 10.0F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return false (direction differs by 90 degrees from heading)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if function returns true when principal direction is within 25-degree tolerance of heading.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, within_tolerance_20_degrees)
{
   /** \precond
    * Three detections with principal direction 20 degrees off from heading
    * Heading: 0.0F
    * Principal direction approximately: 20 degrees
    * Using detections at positions that create ~20 degree angle
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);
   
   // Create detections that form a ~20 degree angle from x-axis
   float32_t angle = 20.0F * F360_PI / 180.0F;
   float32_t cos_angle = F360_Cosf(angle);
   float32_t sin_angle = F360_Sinf(angle);
   
   det_props[0].vcs_position.x = 1.0F * cos_angle;
   det_props[0].vcs_position.y = 1.0F * sin_angle;
   det_props[1].vcs_position.x = 5.0F * cos_angle;
   det_props[1].vcs_position.y = 5.0F * sin_angle;
   det_props[2].vcs_position.x = 10.0F * cos_angle;
   det_props[2].vcs_position.y = 10.0F * sin_angle;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return true (within 25-degree tolerance)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if function returns false when principal direction is beyond 25-degree tolerance of heading.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, beyond_tolerance_40_degrees)
{
   /** \precond
    * Three detections with principal direction 40 degrees off from heading
    * Heading: 0.0F
    * Principal direction approximately: 40 degrees
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);
   
   // Create detections that form a ~40 degree angle from x-axis
   float32_t angle = 40.0F * F360_PI / 180.0F;
   float32_t cos_angle = F360_Cosf(angle);
   float32_t sin_angle = F360_Sinf(angle);
   
   det_props[0].vcs_position.x = 1.0F * cos_angle;
   det_props[0].vcs_position.y = 1.0F * sin_angle;
   det_props[1].vcs_position.x = 5.0F * cos_angle;
   det_props[1].vcs_position.y = 5.0F * sin_angle;
   det_props[2].vcs_position.x = 10.0F * cos_angle;
   det_props[2].vcs_position.y = 10.0F * sin_angle;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return false (beyond 25-degree tolerance)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if function handles negative angles correctly (wraparound at pi/-pi).
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, wraparound_angle)
{
   /** \precond
    * Three detections with principal direction near -pi (negative direction)
    * Heading: pi - 0.1 (just before wraparound)
    * Principal direction: -pi + 0.1 (just after wraparound)
    * Angle difference should be small due to normalization
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(F360_PI - 0.1F);
   
   // Create detections forming angle at -pi + 0.1
   float32_t angle = -F360_PI + 0.1F;
   float32_t cos_angle = F360_Cosf(angle);
   float32_t sin_angle = F360_Sinf(angle);
   
   det_props[0].vcs_position.x = 1.0F * cos_angle;
   det_props[0].vcs_position.y = 1.0F * sin_angle;
   det_props[1].vcs_position.x = 5.0F * cos_angle;
   det_props[1].vcs_position.y = 5.0F * sin_angle;
   det_props[2].vcs_position.x = 10.0F * cos_angle;
   det_props[2].vcs_position.y = 10.0F * sin_angle;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return true (angles are close when normalized)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if function correctly handles detections with noise while direction is still close.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, noisy_detections_aligned_with_heading)
{
   /** \precond
    * Three detections with small perpendicular noise but still aligned with heading
    * Heading: 0.0F
    * Detections mostly along x-axis with small y variations
    */
   obj.ndets = 3;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.vcs_heading.Value(0.0F);
   
   det_props[0].vcs_position.x = 1.0F;
   det_props[0].vcs_position.y = 0.2F;
   det_props[1].vcs_position.x = 5.0F;
   det_props[1].vcs_position.y = -0.1F;
   det_props[2].vcs_position.x = 10.0F;
   det_props[2].vcs_position.y = 0.15F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return true (principal direction is close despite noise)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if function handles multiple detections with more complex geometry.
 */
TEST(f360_populate_track_properties_Is_Pca_Principal_Dir_Close_To_Heading, many_detections_aligned)
{
   /** \precond
    * Six detections aligned along x-axis direction with minimal noise
    * Heading: 0.0F
    */
   obj.ndets = 6;
   obj.detids[0] = 1;
   obj.detids[1] = 2;
   obj.detids[2] = 3;
   obj.detids[3] = 4;
   obj.detids[4] = 5;
   obj.detids[5] = 6;
   obj.vcs_heading.Value(0.0F);
   
   det_props[0].vcs_position.x = 1.0F;
   det_props[0].vcs_position.y = 0.05F;
   det_props[1].vcs_position.x = 2.5F;
   det_props[1].vcs_position.y = -0.05F;
   det_props[2].vcs_position.x = 4.0F;
   det_props[2].vcs_position.y = 0.03F;
   det_props[3].vcs_position.x = 6.0F;
   det_props[3].vcs_position.y = 0.02F;
   det_props[4].vcs_position.x = 8.0F;
   det_props[4].vcs_position.y = -0.04F;
   det_props[5].vcs_position.x = 10.0F;
   det_props[5].vcs_position.y = 0.01F;

   /** \action
    * Call Is_Pca_Principal_Dir_Close_To_Heading()
    */
   bool result = Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj);

   /** \result
    * Function should return true
    */
   CHECK_TRUE(result);
}

/** @}*/
