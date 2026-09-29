/** \file
 * This file contains unit tests for content of f360_longi_stat_cluster.cpp file
 */

#include "f360_longi_stat_cluster.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_longi_stat_cluster
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function Get_Cluster_Min_Long_Pos and Get_Cluster_Max_Long_Pos that returns the min and max longitudinal position of a LSC cluster.
 */
TEST_GROUP(f360_longi_stat_cluster)
{	
   const float32_t test_pass_th = 1e-8F;
};

/** \purpose  
 * Verify that function that returns the min longitudinal position of a LSC cluster works as expected.
 * \req
 * NA.
 */
TEST(f360_longi_stat_cluster, Get_Cluster_Min_Long_Pos)
{
   /** \precond
    * Create an object and set its longitudinal vcs position to 5
    * Create a LSC cluster and assign the object as the first object in cluster
    */
   F360_Object_Track_T obj = {};
   obj.vcs_position.x = 5.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);
   F360_Longi_Stat_Cluster_T cluster = {};
   cluster.first_object = &obj;

   /** \action
    * Call Get_Cluster_Min_Long_Pos
    */
   const float32_t min_long_pos = Get_Cluster_Min_Long_Pos(cluster);

   /** \result
    * Check that the min longitudinal position of the cluster is equal to the position of the object
    */
   DOUBLES_EQUAL_TEXT(obj.vcs_position.x, min_long_pos, test_pass_th, "The reported min longitudinal position of the cluster was not correct.");
}

/** \purpose
* Verify that function that returns the max longitudinal position of a LSC cluster works as expected.
 * \req
 * NA
 */
TEST(f360_longi_stat_cluster, Get_Cluster_Max_Long_Pos)
{
   /** \precond
    * Create an object and set its longitudinal vcs position to 10
    * Create a LSC cluster and assign the object as the last object in cluster
    */
   F360_Object_Track_T obj = {};
   obj.vcs_position.x = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);
   F360_Longi_Stat_Cluster_T cluster = {};
   cluster.last_object = &obj;

   /** \action
    * Call Get_Cluster_Max_Long_Pos
    */
   const float32_t max_long_pos = Get_Cluster_Max_Long_Pos(cluster);

   /** \result
    * Check that the max longitudinal position of the cluster is equal to the position of the object
    */
   DOUBLES_EQUAL_TEXT(obj.vcs_position.x, max_long_pos, test_pass_th, "The reported max longitudinal position of the cluster was not correct.");
}
/** @}*/
