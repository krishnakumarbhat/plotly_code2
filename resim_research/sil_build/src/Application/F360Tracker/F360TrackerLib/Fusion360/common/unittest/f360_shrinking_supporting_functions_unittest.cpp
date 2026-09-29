/** \file
 * This file contains unit tests for content of f360_cipv_supporting_functions.cpp file
 */

#include "f360_shrinking_supporting_functions.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  get_CIPV_long_pos
 *  @{
 */

 /** \brief
  * Test Group of Get_CIPV_Long_Pos() function. Tests verify whether the CIPV
  * object is correctly selected. The most functionality has been tested through above test cases.
  * The remaining branch tests are covered here
  */
TEST_GROUP(get_CIPV_long_pos)
{
   F360_Host_T host{};
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Calibrations_T calib;
   F360_Object_Track_T& object1 = object_tracks[0];
   F360_Object_Track_T& object2 = object_tracks[1];
   F360_Object_Track_T& object3 = object_tracks[2];

   /** \setup
    * Initialize tracker calibrations
    * Set active objects as three
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      tracker_info.num_active_objs = 3;

      object1.movable_prob = 0.0F;
      object1.vcs_position.x = 14.0F;
      object1.vcs_position.y = 0.0F;
      object1.id = 1;

      object2.movable_prob = 0.0F;
      object2.vcs_position.x = 20.0F;
      object2.vcs_position.y = 0.0F;
      object2.id = 2;

      object3.movable_prob = 0.0F;
      object3.vcs_position.x = 20.0F;
      object3.vcs_position.y = 0.0F;
      object3.id = 3;

      tracker_info.vcslong_sorted_first_infront_of_host = &object1;
      tracker_info.vcslong_sorted_next_track[0] = &object2;

      tracker_info.vcslong_sorted_next_track[1] = NULL;

      host.speed = calib.k_object_motion_queue_zone_host_stationary_speed_threshold - 0.01F;
   }
};

/** \purpose
* Purpose of this test is to verify that cipv long pos return 0 due to that there is no moveable CIPV
* through the branch that the next track is NULL.
* \req
* NA.
*/
TEST(get_CIPV_long_pos, get_CIPV_long_pos__next_track_null)
{
   /** \precond
   * Use the default setting that both two valid objects are stationary and the next sorted one is NULL
   */


   /** \action
   * Call tested function
   */
   float32_t cipv_long_pos = Get_CIPV_Long_Pos(tracker_info, host, calib);

   /** \result
   * check if cipv returned longitudinal position is 0.0F
   */
   CHECK_EQUAL(cipv_long_pos, 0.0F);
}

/** \purpose
* Purpose of this test is to verify that cipv long pos return 0 due to that there is no eligible moveable CIPV
* upon reach the maximum number of active objects. 
* \req
* NA.
*/
TEST(get_CIPV_long_pos, get_CIPV_long_pos_max_num_active_objects)
{
   /** \precond
   * Use the default setting
   * Set the object 2 moveable but with a large vcs lateral position
   * Set a "not NULL" object as the next sorted one but beyond the max number of acitve objects
   */

   object2.vcs_position.y = 10.0F;
   object2.movable_prob = 1.0F;

   tracker_info.vcslong_sorted_next_track[1] = &object3;


   /** \action
   * Call tested function
   */
   float32_t cipv_long_pos = Get_CIPV_Long_Pos(tracker_info, host, calib);

   /** \result
   * check if cipv returned longitudinal position is 0.0F
   */
   CHECK_EQUAL(cipv_long_pos, 0.0F);
}

/** \purpose
* Purpose of this test is to verify that cipv long pos return 0 due to that there is no eligible moveable CIPV
* when long pos of the next object exceeds k_object_motion_queue_zone_long_dist
* \req
* NA.
*/
TEST(get_CIPV_long_pos, get_CIPV_long_pos_exceeds_thres)
{
   /** \precond
   * Use the default setting
   * Set the object 2 moveable but with a large vcs lateral position
   * Set a valid object as the next sorted one but the long position exceeds k_object_motion_queue_zone_long_dist
   */

   host.speed = calib.k_object_motion_queue_zone_host_stationary_speed_threshold - 0.01F;

   object2.vcs_position.y = 10.0F;
   object2.movable_prob = 1.0F;

   object3.vcs_position.x = calib.k_object_motion_queue_zone_long_dist + 1.0F;
   tracker_info.vcslong_sorted_next_track[1] = &object3;


   /** \action
   * Call tested function
   */
   float32_t cipv_long_pos = Get_CIPV_Long_Pos(tracker_info, host, calib);

   /** \result
   * check if cipv returned longitudinal position is 0.0F
   */
   CHECK_EQUAL(cipv_long_pos, 0.0F);
}
/** @}*/

/** \defgroup  determine_CIPV_status
 *  @{ 
 */

 /** \brief
  * Test Group of Determine_CIPV_Status() function. Tests verify logic deciding whether an object
  * lies inside the CIPV longitudinal window and is not suspiciously oriented.
  */
TEST_GROUP(determine_CIPV_status)
{
   F360_Calibrations_T calib;
   F360_Object_Track_T object; // object under test

   /** \setup
    * Initialize calibrations and set default neutral object values inside window with non-suspicious orientation.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      object.vcs_position.x = 5.0F;
      object.vcs_position.y = 0.0F;
      object.bbox.Set_Orientation(0.0F); // aligned with host
   }
};

/** \purpose
* Purpose of this test is to verify that if cipv_long_pos is 0.0F no object can be in CIPV zone.
* \req
* NA.
*/
TEST(determine_CIPV_status, determine_CIPV_status_cipv_long_pos_zero)
{
   /** \precond
   * cipv_long_pos = 0.0F; object inside potential window but logic requires cipv_long_pos > 0.
   */

   /** \action
   * Call tested function
   */
   const bool status = Determine_CIPV_Status(object, 0.0F, calib);

   /** \result
   * Expect false because no CIPV reference available (cipv_long_pos == 0).
   */
   CHECK_FALSE(status);
}

/** \purpose
* Purpose of this test is to verify object inside symmetric longitudinal window is accepted when orientation is not suspicious.
* \req
* NA.
*/
TEST(determine_CIPV_status, determine_CIPV_status_inside_window_not_suspicious)
{
   /** \precond
   * cipv_long_pos = 12.0F so window half-range = 14.0F (cipv_long_pos + buffer(2)). object.x = 13.9F inside.
   */
   const float32_t cipv_long_pos = 12.0F;
   object.vcs_position.x = 13.9F;

   /** \action
   * Call tested function
   */
   const bool status = Determine_CIPV_Status(object, cipv_long_pos, calib);

   /** \result
   * Expect true because inside window and orientation 0.0F.
   */
   CHECK_TRUE(status);
}

/** \purpose
* Purpose of this test is to verify object just outside window forward is rejected.
* \req
* NA.
*/
TEST(determine_CIPV_status, determine_CIPV_status_outside_window_forward)
{
   /** \precond
   * cipv_long_pos = 12.0F => limit = 14.0F; object.x = 14.01F outside.
   */
   const float32_t cipv_long_pos = 12.0F;
   object.vcs_position.x = 14.01F;

   /** \action
   * Call tested function
   */
   const bool status = Determine_CIPV_Status(object, cipv_long_pos, calib);

   /** \result
   * Expect false because outside longitudinal window.
   */
   CHECK_FALSE(status);
}

/** \purpose
* Purpose of this test is to verify suspicious orientation causes rejection even if inside window.
* \req
* NA.
*/
TEST(determine_CIPV_status, determine_CIPV_status_inside_window_but_suspicious_orientation)
{
   /** \precond
   * cipv_long_pos = 8.0F => limit = 10.0F; object.x = 1.0F inside. Orientation set to suspicious range.
   */
   const float32_t cipv_long_pos = 8.0F;
   object.vcs_position.x = 1.0F;
   const float32_t th = calib.k_object_motion_max_abs_orient_diff_to_host;
   object.bbox.Set_Orientation(th + 0.1F);

   /** \action
   * Call tested function
   */
   const bool status = Determine_CIPV_Status(object, cipv_long_pos, calib);

   /** \result
   * Expect false because orientation suspicious.
   */
   CHECK_FALSE(status);
}

/** \purpose
* Purpose of this test is to verify object at negative edge just inside window is accepted.
* \req
* NA.
*/
TEST(determine_CIPV_status, determine_CIPV_status_edge_negative_boundary_inside)
{
   /** \precond
   * cipv_long_pos = 5.0F => limit = 7.0F; object.x = -6.999F inside (-7, 7).
   */
   const float32_t cipv_long_pos = 5.0F;
   object.vcs_position.x = -6.999F;
   object.bbox.Set_Orientation(0.0F);

   /** \action
   * Call tested function
   */
   const bool status = Determine_CIPV_Status(object, cipv_long_pos, calib);

   /** \result
   * Expect true because inside window and orientation not suspicious.
   */
   CHECK_TRUE(status);
}

/** \purpose
* Purpose of this test is to verify object just outside window backward is rejected.
* \req
* NA.
*/
TEST(determine_CIPV_status, determine_CIPV_status_outside_window_backward)
{
    /** \precond
   * cipv_long_pos = 6.0F => limit = 8.0F; object.x = -8.01F outside.
   */
   const float32_t cipv_long_pos = 6.0F; 
   object.vcs_position.x = -(cipv_long_pos + 2.1F); 
   object.bbox.Set_Orientation(0.0F);

   /** \action
   * Call tested function
   */
   const bool status = Determine_CIPV_Status(object, cipv_long_pos, calib);

   /** \result
   * Expect false because outside longitudinal window
   */
   CHECK_FALSE(status);
}
/** @}*/

/** \defgroup In_Special_Zone_For_No_Shrinking
 *  @{ 
 */

 /** \brief
   * Test group validating the function In_Special_Zone_For_No_Shrinking().
  * 
  */
TEST_GROUP(In_Special_Zone_For_No_Shrinking)
{
   Point vcs_position{};
   F360_Tracker_Variant_Type_Tag variant_type{};
   float32_t eps;
   // Zone boundaries matching production constants
   float32_t zone1_min_x;
   float32_t zone1_max_x;
   float32_t zone1_max_abs_y;
   float32_t zone2_min_x;
   float32_t zone2_max_x;
   float32_t zone2_max_abs_y;

   TEST_SETUP()
   {
      eps = 1e-6F;
      vcs_position.y = 0.0F; // default lateral within both zones unless changed
      variant_type = F360_VARIANT_TYPE_A; // default non-K
      zone1_min_x = -40.0F;
      zone1_max_x = 25.0F;
      zone1_max_abs_y = 22.0F;
      zone2_min_x = -100.0F;
      zone2_max_x = 0.0F;
      zone2_max_abs_y = 10.0F;
   }
};

/** \purpose
 * variant not K, outside both zones -> expect false.
 */
TEST(In_Special_Zone_For_No_Shrinking, NotK_OutsideZones_False)
{
   /** \precond
    * Place object outside BOTH zones using literal boundaries (zone1 max_x=25, max_abs_y=22).
    */
   vcs_position.x = zone1_max_x + 1.0F;
   vcs_position.y = zone1_max_abs_y + 1.0F;

   /** \action */
   const bool status = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result */
   CHECK_FALSE_TEXT(status, "Non-K variant outside zones incorrectly returned true");
}

/** \purpose
 * variant not K, inside zone1 (should still be false since variant type not K).
 */
TEST(In_Special_Zone_For_No_Shrinking, NotK_InZone1_False)
{
   /** \precond
    * Variant type left as non-K. Place object within zone1 longitudinal and lateral bounds.
    */
   vcs_position.x = (zone1_min_x + zone1_max_x) * 0.5F; // midpoint inside zone1
   vcs_position.y = 0.0F; // within |y| < zone1_max_abs_y

   /** \action
    * Call Determine_Variant_K_In_Zone_Status().
    */
   const bool status = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result
    * Expect false because variant is not K even though inside zone1.
    */
   CHECK_FALSE_TEXT(status, "Non-K variant in zone1 should be false");
}

/** \purpose
 * variant not K, inside zone2 (should still be false).
 */
TEST(In_Special_Zone_For_No_Shrinking, NotK_InZone2_False)
{
   /** \precond
    * Variant not K. Choose x within zone2 longitudinal span (outside zone1 if distinct) and lateral within bounds.
    */
   vcs_position.x = (zone2_min_x + zone2_max_x) * 0.5F; // midpoint inside zone2
   vcs_position.y = 0.0F; // within |y| < zone2_max_abs_y

   /** \action
    * Evaluate status.
    */
   const bool status = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result
    * Expect false because variant is not K.
    */
   CHECK_FALSE_TEXT(status, "Non-K variant in zone2 should be false");
}

/** \purpose
 * variant K outside both zones -> expect false.
 */
TEST(In_Special_Zone_For_No_Shrinking, K_OutsideZones_False)
{
   /** \precond
    * Set variant to K. Position object outside BOTH zones:
    *  - Choose x > zone1_max_x (guarantees outside zone1 and zone2 since zone2_max_x <= 0)
    *  - Choose |y| > zone1_max_abs_y (also outside both lateral bounds)
    */
   variant_type = F360_VARIANT_TYPE_K;
   vcs_position.x = zone1_max_x + 1.0F;
   vcs_position.y = zone1_max_abs_y + 1.0F;

   /** \action
    * Evaluate status.
    */
   const bool status = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result
    * Expect false because object is outside both zones even though variant is K.
    */
   CHECK_FALSE_TEXT(status, "Variant K outside zones should be false");
}

/** \purpose
 * variant K inside zone1 only -> expect true.
 */
TEST(In_Special_Zone_For_No_Shrinking, K_InZone1_True)
{
   /** \precond
    * Variant K. Place object strictly within zone1 longitudinal and lateral bounds (just inside lateral max).
    */
   variant_type = F360_VARIANT_TYPE_K;
   vcs_position.x = (zone1_min_x + zone1_max_x) * 0.5F;
   vcs_position.y = zone1_max_abs_y - 0.01F; // just inside

   /** \action
    * Evaluate status.
    */
   const bool status = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result
    * Expect true due to variant K and inside zone1.
    */
   CHECK_TRUE_TEXT(status, "Variant K in zone1 expected true");
}

/** \purpose
 * variant K inside zone2 only -> expect true.
 */
TEST(In_Special_Zone_For_No_Shrinking, K_InZone2_True)
{
   /** \precond
    * Variant K. Place object inside zone2 bounds (lateral just inside max). If zones overlap, still valid.
    */
   variant_type = F360_VARIANT_TYPE_K;
   vcs_position.x = (zone2_min_x + zone2_max_x) * 0.5F;
   vcs_position.y = zone2_max_abs_y - 0.01F; // just inside

   /** \action
    * Evaluate status.
    */
   const bool status = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result
    * Expect true because variant K and inside zone2.
    */
   CHECK_TRUE_TEXT(status, "Variant K in zone2 expected true");
}

/** \purpose
 * Boundary tests: just inside vs just outside zone1 and zone2 limits for variant K.
 */
TEST(In_Special_Zone_For_No_Shrinking, K_Boundary_Checks)
{
   /** \precond
    * Variant K to enable positive outcomes when inside a zone.
    */
   variant_type = F360_VARIANT_TYPE_K;

   /** \action
    * Sequentially position object at boundary-related points and evaluate status.
    */
   // zone1 just inside
   vcs_position.x = zone1_min_x + 0.0001F; // just above zone1 min
   vcs_position.y = zone1_max_abs_y - 0.0001F;  // just below zone1 lateral max
   const bool z1_inside = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   // zone1 just outside (x below min), |y| above lim
   vcs_position.x = zone1_min_x - 0.0001F;
   vcs_position.y = 10.0F;
   const bool z1_x_below = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   // zone1 just outside (y above abs limit)
   vcs_position.x = zone1_max_x - 0.0001F;
   vcs_position.y = zone1_max_abs_y + 0.0001F;
   const bool z1_y_above = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   // zone2 just inside
   vcs_position.x = zone2_min_x + 0.0001F;
   vcs_position.y = zone2_max_abs_y - 0.0001F;
   const bool z2_inside = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   // zone2 just outside (x above max), but in zone 1
   vcs_position.x = zone2_max_x + 0.0001F; // just above zone2 max; still inside zone1
   vcs_position.y = 0.0F;
   const bool z2_x_above = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   // zone2 just outside (|y| above limit)
   vcs_position.x = zone2_min_x + 0.0001F;
   vcs_position.y = zone2_max_abs_y + 0.0001F;
   const bool z2_y_above = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   // zone2 just outside (z below min)
   vcs_position.x = zone2_min_x - 0.0001F;
   vcs_position.y = zone2_max_abs_y + 0.0001F;
   const bool z2_x_below = In_Special_Zone_For_No_Shrinking(vcs_position, variant_type);

   /** \result
    * Inside cases true; outside cases false.
    */
   CHECK_TRUE_TEXT(z1_inside, "Zone1 just inside should be true");
   CHECK_FALSE_TEXT(z1_x_below, "Zone1 x below min should be false");
   CHECK_FALSE_TEXT(z1_y_above, "Zone1 y above abs limit should be false");
   CHECK_TRUE_TEXT(z2_inside, "Zone2 just inside should be true");
   CHECK_TRUE_TEXT(z2_x_above, "Zone2 x above max (object in zone1) should be true");
   CHECK_FALSE_TEXT(z2_y_above, "Zone2 |y| above limit should be false");
   CHECK_FALSE_TEXT(z2_x_below, "Zone2 x below min should be false");
}

/** @}*/

