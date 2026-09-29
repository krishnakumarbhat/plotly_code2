/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_math_func.h"
#include "f360_is_init_trk_bbox_overlapped_with_trusted_trk.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  is_init_trk_bbox_overlapped_with_trusted_trk
 *  @{
 */

 /** \brief
 *  Add brief description of test group
 **/
TEST_GROUP(is_init_trk_bbox_overlapped_with_trusted_trk)
{
   /** \setup
   * Set up needed variables.
   **/
   F360_Calibrations_T calib;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   int32_t active_obj_idx;
   F360_Tracker_Info_T tracker_info;

   TEST_SETUP()
   {
      // Initialize calibrations 
      Initialize_Tracker_Calibrations(calib);
   }
};

/**
*\purpose Check whether function returns false when there is no active objects.   
*\req     NA
*/
TEST(is_init_trk_bbox_overlapped_with_trusted_trk, Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk_No_Actvie_Obj)
{
   /** \precond
   *   Number of active object is 0.  
   */
   tracker_info.num_active_objs = 0;

   /** \action
   *   Call a function Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk().
   */
   bool f_is_overlapped =  Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(calib, object_tracks, active_obj_idx, tracker_info);

   /** \result
   *   Check whether overlapped object have not be found. 
   */
   CHECK_FALSE(f_is_overlapped);
}

/**
*\purpose Check whether function returns false when every active object doesn't met one condition for being valid object.  
*\req     NA
*/
TEST(is_init_trk_bbox_overlapped_with_trusted_trk, Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk_Lack_Of_Valid_Object)
{
   /** \precond
   *   Define objects properties.
   */
   tracker_info.num_active_objs = 5;
   for (int8_t idx = 0; idx < tracker_info.num_active_objs; idx++)
   {
      tracker_info.active_obj_ids[idx] = idx + 1;
      object_tracks[idx].id = idx + 1;
      object_tracks[idx].f_moving = true;
      object_tracks[idx].status = F360_OBJECT_STATUS_COASTED;
      object_tracks[idx].confidenceLevel = calib.k_puta_min_object_confidence + 0.1F;
      object_tracks[idx].time_since_cluster_created = calib.k_puta_min_object_time + 0.1F;
   }
   
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[2].confidenceLevel = 0.0F;
   object_tracks[3].time_since_cluster_created = 0.0F;
   object_tracks[4].f_moving = false;

   /** \action
   *   Call a function Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk().
   */
   bool f_is_overlapped = Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(calib, object_tracks, active_obj_idx, tracker_info);

   /** \result
   *   Check whether overlapped object have not be found.
   */
   CHECK_FALSE(f_is_overlapped);
}

/**
*\purpose Check whether function returns false when active objects pointing in opposite direction.
*\req     NA
*/
TEST(is_init_trk_bbox_overlapped_with_trusted_trk, Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk_No_Object_Overlaps)
{
   /** \precond
   *   Define objects properties.
   */
   tracker_info.num_active_objs = 2;
   for (int8_t idx = 0; idx < tracker_info.num_active_objs; idx++)
   {
      tracker_info.active_obj_ids[idx] = idx + 1;
      object_tracks[idx].id = idx + 1;
      object_tracks[idx].f_moving = true;
      object_tracks[idx].status = F360_OBJECT_STATUS_COASTED;
      object_tracks[idx].confidenceLevel = calib.k_puta_min_object_confidence + 0.1F;
      object_tracks[idx].time_since_cluster_created = calib.k_puta_min_object_time + 0.1F;
   }
   
   object_tracks[1].Set_Bbox_Orientation(Angle{ F360_PI });

   /** \action
   *   Call a function Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk().
   */
   bool f_is_overlapped = Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(calib, object_tracks, active_obj_idx, tracker_info);

   /** \result
   *   Check whether overlapped object have not be found.
   */
   CHECK_FALSE(f_is_overlapped);
}

/**
*\purpose Check whether function returns false when base active objects do not have overlapping area.
*\req     NA
*/
TEST(is_init_trk_bbox_overlapped_with_trusted_trk, Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk_No_Overlapping_Area)
{
   /** \precond
   *   Define objects properties.
   */
   tracker_info.num_active_objs = 2;
   for (int8_t idx = 0; idx < tracker_info.num_active_objs; idx++)
   {
      tracker_info.active_obj_ids[idx] = idx + 1;
      object_tracks[idx].id = idx + 1;
      object_tracks[idx].f_moving = true;
      object_tracks[idx].status = F360_OBJECT_STATUS_COASTED;
      object_tracks[idx].confidenceLevel = calib.k_puta_min_object_confidence + 0.1F;
      object_tracks[idx].time_since_cluster_created = calib.k_puta_min_object_time + 0.1F;
      object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
   }
   
   object_tracks[1].vcs_position.x = 2.0F;
   object_tracks[1].vcs_position.y = 2.0F;

   /** \action
   *   Call a function Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk().
   */
   bool f_is_overlapped = Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(calib, object_tracks, active_obj_idx, tracker_info);

   /** \result
   *   Check whether overlapped object have not be found.
   */
   CHECK_FALSE(f_is_overlapped);
}

/** @}*/

/** \defgroup  check_if_init_object_should_be_killed
 *  @{
 */

/** \brief
*  Add brief description of test group
**/
TEST_GROUP(check_if_init_object_should_be_killed)
{
   /** \setup
   * Set up needed variables.
   **/
   F360_Calibrations_T  calib{};
   F360_Object_Track_T initialized_object{};
   F360_Object_Track_T object_to_check{};

   TEST_SETUP()
   {
      // Default objects properties
      initialized_object.vcs_position.x = 0.0F;
      initialized_object.vcs_position.y = 0.0F;

      initialized_object.Update_Bbox_Size(4.0F, 2.0F);
      initialized_object.Set_Bbox_Orientation(Angle{ 0.0F });

      object_to_check.Update_Bbox_Size(4.0F, 2.0F);
      object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(-1.0F) });
      
      //Initialize calibrations.
      Initialize_Tracker_Calibrations(calib);
   }
};

/**
*\purpose  Test checks if Init_Object_Should_Be_Killed gives expected results for two objects with same direction far from each other. 
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Far_From_Each_Other_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[10, 10]) to be far from initialized object (VCS[0, 0]). Set it pointing (0 deg) to be same as initialized object pointing (0 deg).
   */
   object_to_check.vcs_position.x = 10.0F;
   object_to_check.vcs_position.y = 10.0F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */
   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Object are far from each other, so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with opposite direction far from each other.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Far_From_Each_Other_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[10, 10]) to be far from initialized object (VCS[0, 0]). Set it pointing (180 deg) to be opposide direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 10.0F;
   object_to_check.vcs_position.y = 10.0F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Object are far from each other, so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with same direction and close to each other in a way that 
* side borders of bboxes are touching.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Close_To_Each_Other_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 2]) to be close to initialized object (VCS[0, 0]) - side boundaries of bboxes are touching.
   Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */
   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 2.0F;
   object_to_check.Set_Bbox_Orientation(Angle{0.0F});

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */
   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Object are close each other, but don't overlap so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with opposite direction and close to each other in a way that
* side borders of bboxes are touching.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Close_To_Eachother_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[2, 0]) to be close to initialized object (VCS[0, 0]) - side boundaries of bboxes are touching.
   Set it pointing (180 deg) to be opposide direction as initialized object pointing (0 deg).
   */
   object_to_check.vcs_position.x = 2.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */
   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Object are close each other, but don't overlap so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with opposite direction and their bboxes ovelaps 50%.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Overlaps_50_Percent_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 2]) to overlap initialized object (VCS[0, 0]). Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 2.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Set_Bbox_Orientation(Angle{0.0F});

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Objects overlap, but overlapping area is not above 75 percent so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with opposite direction and their bboxes ovelaps 50%.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Overlaps_50_Percent_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 2]) to overlap initialized object (VCS[0, 0]). Set it pointing (180 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 2.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Objects overlap, but overlapping area is not above 75 percent so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with same direction and their bboxes ovelaps just below 75%.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Overlaps_Below_75_Percent_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0.51]) to ovelap initialized object (VCS[0, 0]) in just below 75%.
   * Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.51F;
   object_to_check.Set_Bbox_Orientation(Angle{0.0F});

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Objects overlap, but overlapping area is not above 75 percent so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with opposite direction and their bboxes ovelaps just below 75%.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Overlaps_Below_75_Percent_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0.51]) to ovelap initialized object (VCS[0, 0]) in just below 75%.
   * Set it pointing (180 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.51F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Objects overlap, but overlapping area is not above 75 percent so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with same direction and their bboxes ovelaps just over 75%.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Overlaps_Over_75_Percent_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0.49]) to ovelap initialized object (VCS[0, 0]) in just over 75%.
   * Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.49F;
   object_to_check.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Objects overlap, and overlapping area is above 75 percent so initialized object should be killed. Return of the function should be TRUE.
   */
   CHECK_TRUE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results for two objects with opposite direction and their bboxes ovelaps just over 75%.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Objects_Overlaps_Over_75_Percent_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0.49]) to overlap initialized object (VCS[0, 0]) in just over 75%.
   * Set it pointing (180 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.49F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Objects overlap over 75%, but has opposite directions so initialized object should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results if checked objects is completly inside initialized object and they have same pointing direction.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Object_To_Check_Inside_Init_Object_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0]) and it dimension to small square 1x1m to be completly inside initialized object bbox.
   * Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Update_Bbox_Size(1.0F, 1.0F);
   object_to_check.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Initialized object completly covers checked object, so it should not be killed. Return of the function should be FALSE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results if checked objects is completly inside initialized object and they have opposite pointing direction.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Object_To_Check_Inside_Init_Object_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0]) and it dimension to small square 1x1m to be completly inside initialized object bbox.
   * Set it pointing (180 deg) to be opposite direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Initialized object completly covers checked object, but it has opposide direction so it should not be killed. Return of the function should be TRUE.
   */
   CHECK_FALSE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results if checked objects is completly inside initialized object and they have same pointing direction.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Init_Object_Inside_Object_To_Check_Same_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0]) and it dimension to be slightly larger to completly cover initialized object bbox.
   * Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Update_Bbox_Size(4.2F, 2.2F);
   object_to_check.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Initialized object is inside checked object, so it should be killed. Return of the function should be TRUE.
   */
   CHECK_TRUE(f_result)
}

/**
*\purpose  Test checks if Check_If_Init_Object_Should_Be_Killed gives expected results if checked objects is completly inside initialized object and they have opposite pointing direction.
*\req   NA
*/
TEST(check_if_init_object_should_be_killed, Init_Object_Inside_Object_To_Check_Opposite_Dir)
{
   /** \precond
   * Set object to check's position (VCS[0, 0]) and it dimension to be slightly larger to completly cover initialized object bbox.
   * Set it pointing (0 deg) to be same direction as initialized object pointing (0 deg).
   */

   object_to_check.vcs_position.x = 0.0F;
   object_to_check.vcs_position.y = 0.0F;
   object_to_check.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });
   object_to_check.Update_Bbox_Size(4.2F, 2.2F);

   /** \action
   * Call a function Check_If_Init_Object_Should_Be_Killed with initialized object and object to check.
   */

   bool f_result = Check_If_Init_Object_Should_Be_Killed(calib, initialized_object, object_to_check);

   /** \result
   * Initialized object is inside checked object, but it has opposide direction so it should not be killed. Return of the function should be TRUE.
   */
   CHECK_FALSE(f_result)
}

/** @}*/
