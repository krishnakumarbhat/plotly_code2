/** \file
 * This file contains unit tests for content of f360_check_if_objects_can_overlap_significantly.cpp file
 */

#include "f360_check_if_objects_can_overlap_significantly.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_check_if_objects_can_overlap_significantly
 *  @{
 */

/** \brief
 * Verify the functionality of Check_If_Objects_Can_Overlap_Significantly()
 */
TEST_GROUP(f360_check_if_objects_can_overlap_significantly)
{
   Point obj1_center_position_vcs;
   float32_t obj1_length;
   Point obj2_center_position_vcs;
   float32_t obj2_length;
   /** \setup
    * Set obj1_center_position_vcs to x = -25, y = 25
    * Set obj1_length = 10
    * Set obj2_center_position_vcs to x = -17.9, y = 25
    * Set obj2_length = 4
    */
   TEST_SETUP()
   {
      // Setup 2 objects such that they center points are 7.1m apart such that they cannot overlap (7.1 > (obj1len+obj2len)/2)
      obj1_center_position_vcs = Point(-25.0F, 25.0F); 
      obj1_length = 10.0F;
      obj2_center_position_vcs = Point((-17.9F), 25.0F);
      obj2_length = 4.0F;
   }
};

/** \purpose  
 * Ensure function returns f_can_overlap = false when objects are too far apart
 * \req
 * NA
 */
TEST(f360_check_if_objects_can_overlap_significantly, Check_If_Objects_Can_Overlap_Significantly__Cannot_Overlap)
{
   /** \precond
    * None.
    */

   /** \action
    * Call Check_If_Objects_Can_Overlap_Significantly().
    */
   const bool f_can_overlap = Check_If_Objects_Can_Overlap_Significantly(obj1_center_position_vcs, obj1_length, obj2_center_position_vcs, obj2_length);
   
   /** \result
    * Function should return false as the objects are too far apart
    */
   CHECK_FALSE(f_can_overlap)
}

/** \purpose  
 * Ensure function returns f_can_overlap = true when objects are sufficiently close because obj1 have a larger length
 * \req
 * NA
 */
TEST(f360_check_if_objects_can_overlap_significantly, Check_If_Objects_Can_Overlap_Significantly__Can_Overlap_Obj1_Len)
{
   /** \precond
    * None.
    */
   obj1_length = 10.5F;

   /** \action
    * Call Check_If_Objects_Can_Overlap_Significantly().
    */
   const bool f_can_overlap = Check_If_Objects_Can_Overlap_Significantly(obj1_center_position_vcs, obj1_length, obj2_center_position_vcs, obj2_length);
   
   /** \result
    * The function should return true as the objects may overlap
    */
   CHECK_TRUE(f_can_overlap)
}

/** \purpose
 * Ensure function returns f_can_overlap = true when objects are sufficiently close because obj2 have a larger length
 * \req
 * NA
 */
TEST(f360_check_if_objects_can_overlap_significantly, Check_If_Objects_Can_Overlap_Significantly__Can_Overlap_Obj2_Len)
{
   /** \precond
    * None.
    */
   obj2_length = 4.5F;

   /** \action
    * Call Check_If_Objects_Can_Overlap_Significantly().
    */
   const bool f_can_overlap = Check_If_Objects_Can_Overlap_Significantly(obj1_center_position_vcs, obj1_length, obj2_center_position_vcs, obj2_length);
 
   /** \result
    * The function should return true as the objects may overlap
    */
   CHECK_TRUE(f_can_overlap)
}

/** \purpose
 * Ensure function returns f_can_overlap = true when objects are sufficiently close because obj1 is closer
 * \req
 * NA
 */
TEST(f360_check_if_objects_can_overlap_significantly, Check_If_Objects_Can_Overlap_Significantly__Can_Overlap_Obj1_Moved_Closer)
{
   /** \precond
    * None.
    */
   obj1_center_position_vcs = Point(-24.5F, 25.0F);

   /** \action
    * Call Check_If_Objects_Can_Overlap_Significantly().
    */
   const bool f_can_overlap = Check_If_Objects_Can_Overlap_Significantly(obj1_center_position_vcs, obj1_length, obj2_center_position_vcs, obj2_length);

   /** \result
    * The function should return true as the objects may overlap
    */
   CHECK_TRUE(f_can_overlap)
}

/** \purpose
 * Ensure function returns f_can_overlap = true when objects are sufficiently close because obj2 is closer
 * \req
 * NA
 */
TEST(f360_check_if_objects_can_overlap_significantly, Check_If_Objects_Can_Overlap_Significantly__Can_Overlap_Obj2_Moved_Closer)
{
   /** \precond
    * None.
    */
   obj2_center_position_vcs = Point(-18.4F, 25.0F);

   /** \action
    * Call Check_If_Objects_Can_Overlap_Significantly().
    */
   const bool f_can_overlap = Check_If_Objects_Can_Overlap_Significantly(obj1_center_position_vcs, obj1_length, obj2_center_position_vcs, obj2_length);
 
   /** \result
    * The function should return true as the objects may overlap
    */
   CHECK_TRUE(f_can_overlap)
}
/** @}*/
