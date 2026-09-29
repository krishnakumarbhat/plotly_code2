/** \file
 * This file contains unit tests for content of f360_downselect_longi_stat_clusters.cpp file
 */

#include "f360_downselect_longi_stat_clusters.h"
#include <CppUTest/TestHarness.h>


using namespace f360_variant_A;

/** \defgroup  f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score
 *  @{
 */

/** \brief
 * This test group check Calc_Longi_Stat_Cluster_Score().It creates 7 clusters
 * with various properties to check if the scoring system for them works correctly and if all the branches are covered.
 */
TEST_GROUP(f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score)
{
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS];
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   
   /** \setup
    * Create 7 valid clusters with various properties.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      // Cluster adjacent to host
      float32_t lat_mean_cluster_0 = -5.0F;
      objects[0].vcs_position.x = -15.0F;
      objects[0].vcs_position.y = lat_mean_cluster_0;
      objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = objects[0].vcs_position;
      objects[0].bbox.Set_Center(center);
      objects[1].vcs_position.x = 15.0F;
      objects[1].vcs_position.y = lat_mean_cluster_0;
      objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[1].vcs_position;
      objects[1].bbox.Set_Center(center);
      valid_clusters[0].lat_mean = lat_mean_cluster_0;
      valid_clusters[0].first_object = &objects[0];
      valid_clusters[0].last_object = &objects[1];

      // Cluster behind host left side, should get the same score as cluster 2
      float32_t lat_mean_cluster_1 = -2.0F;
      objects[2].vcs_position.x = -35.0F;
      objects[2].vcs_position.y = lat_mean_cluster_1;
      objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[2].vcs_position;
      objects[2].bbox.Set_Center(center);
      objects[3].vcs_position.x = -20.0F;
      objects[3].vcs_position.y = lat_mean_cluster_1;
      objects[3].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[3].vcs_position;
      objects[3].bbox.Set_Center(center);
      valid_clusters[1].lat_mean = lat_mean_cluster_1;
      valid_clusters[1].first_object = &objects[2];
      valid_clusters[1].last_object = &objects[3];

      // Cluster in front host right side, should get the same score as cluster 1
      float32_t lat_mean_cluster_2 = 2.0F;
      objects[4].vcs_position.x = 20.0F;
      objects[4].vcs_position.y = lat_mean_cluster_2;
      objects[4].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[4].vcs_position;
      objects[4].bbox.Set_Center(center);
      objects[4].bbox.Set_Center(center);
      objects[5].vcs_position.x = 35.0F;
      objects[5].vcs_position.y = lat_mean_cluster_2;
      objects[5].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[5].vcs_position;
      objects[5].bbox.Set_Center(center);
      valid_clusters[2].lat_mean = lat_mean_cluster_2;
      valid_clusters[2].first_object = &objects[4];
      valid_clusters[2].last_object = &objects[5];

      // Cluster with same x_min and x_max. Should in practise never happen but is mimiced for the test
      float32_t lat_mean_cluster_3 = 10.0F;
      objects[6].vcs_position.x = -15.0F;
      objects[6].vcs_position.y = lat_mean_cluster_3;
      objects[6].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[6].vcs_position;
      objects[6].bbox.Set_Center(center);
      objects[7].vcs_position.x = -15.0F;
      objects[7].vcs_position.y = lat_mean_cluster_3;
      objects[7].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[7].vcs_position;
      objects[7].bbox.Set_Center(center);
      valid_clusters[3].lat_mean = lat_mean_cluster_3;
      valid_clusters[3].first_object = &objects[6];
      valid_clusters[3].last_object = &objects[7];

      // Cluster with same lateral position as cluster 0 but shorter
      float32_t lat_mean_cluster_4 = 25.0F;
      objects[8].vcs_position.x = -10.0F;
      objects[8].vcs_position.y = lat_mean_cluster_4;
      objects[8].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[8].vcs_position;
      objects[8].bbox.Set_Center(center);
      objects[9].vcs_position.x = 10.0F;
      objects[9].vcs_position.y = lat_mean_cluster_4;
      objects[9].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[9].vcs_position;
      objects[9].bbox.Set_Center(center);
      valid_clusters[4].lat_mean = lat_mean_cluster_4;
      valid_clusters[4].first_object = &objects[8];
      valid_clusters[4].last_object = &objects[9];

      // Very long cluster that is far away from host
      float32_t lat_mean_cluster_5 = 50.0F;
      objects[10].vcs_position.x = 50.0F;
      objects[10].vcs_position.y = lat_mean_cluster_5;
      objects[10].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[10].vcs_position;
      objects[10].bbox.Set_Center(center);
      objects[11].vcs_position.x = -50.0F;
      objects[11].vcs_position.y = lat_mean_cluster_5;
      objects[11].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[11].vcs_position;
      objects[11].bbox.Set_Center(center);
      valid_clusters[5].lat_mean = lat_mean_cluster_5;
      valid_clusters[5].first_object = &objects[10];
      valid_clusters[5].last_object = &objects[11];

      // Very short cluster that is far away from host
      float32_t lat_mean_cluster_6 = 50.0F;
      objects[12].vcs_position.x = 2.0F;
      objects[12].vcs_position.y = lat_mean_cluster_6;
      objects[12].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[12].vcs_position;
      objects[12].bbox.Set_Center(center);
      objects[13].vcs_position.x = -2.0F;
      objects[13].vcs_position.y = lat_mean_cluster_6;
      objects[13].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[13].vcs_position;
      objects[13].bbox.Set_Center(center);
      valid_clusters[6].lat_mean = lat_mean_cluster_6;
      valid_clusters[6].first_object = &objects[12];
      valid_clusters[6].last_object = &objects[13];
   }

};

/** \purpose  
 * Verifies the score calculation for a cluster adjacent to host.
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score, Calc_Longi_Stat_Cluster_Score_Cluster_Adjacent)
{
   /** \action
    * Call function
    */
   float32_t score = Calc_Longi_Stat_Cluster_Score(valid_clusters[0], calibs);

   /** \result
    * Verify that returned score matches expected data
    */
   DOUBLES_EQUAL(8.33333302F, score, F360_EPSILON);
}

/** \purpose
 * Verifies the score calculation for a cluster behind host left side.
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score, Calc_Longi_Stat_Cluster_Score_Cluster_Behind_Left)
{
   /** \action
    * Call function
    */
   float32_t score = Calc_Longi_Stat_Cluster_Score(valid_clusters[1], calibs);

   /** \result
    * Verify that returned score matches expected data
    */
   DOUBLES_EQUAL(24.28058F, score, F360_EPSILON);
}

/** \purpose
 * Verifies the score calculation for a cluster in front of host right side.
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score, Calc_Longi_Stat_Cluster_Score_Cluster_In_Front_Right)
{
   /** \action
    * Call function
    */
   float32_t score = Calc_Longi_Stat_Cluster_Score(valid_clusters[2], calibs);

   /** \result
    * Verify that returned score matches expected data
    */
   DOUBLES_EQUAL(29.25538F, score, F360_EPSILON);
}

/** \purpose
 * Verifies the score calculation for a cluster with the same x_min and x_max. 
 * This should in practise never happen and we expect the maximum (worst) score possible.
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score, Calc_Longi_Stat_Cluster_Score_Cluster_Invalid_X_Interval)
{
   /** \action
    * Call function
    */
   float32_t score = Calc_Longi_Stat_Cluster_Score(valid_clusters[3], calibs);

   /** \result
    * Verify that returned score matches expected data
    */
   DOUBLES_EQUAL(INFTY, score, F360_EPSILON);
}

/** \purpose
 * Verifies the score calculation for a cluster with the same lateral position as cluster 0.
 * Since this cluster is shorter we expect a higher (worse) score than for cluster 0.
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Calc_Longi_Stat_Cluster_Score, Calc_Longi_Stat_Cluster_Score_Cluster_Short_Curve)
{
   /** \action
    * Call function
    */
   float32_t score = Calc_Longi_Stat_Cluster_Score(valid_clusters[4], calibs);

   /** \result
    * Verify that returned score matches expected data
    */
   DOUBLES_EQUAL(30.0F, score, F360_EPSILON);
}
/** @}*/


/** \brief
 * This test group checks if Find_Special_Longi_Clusters() works properly by 
 * confirming that the score is set to max priority for special clusters.
 */
TEST_GROUP(f360_downselect_longi_stat_clusters_Find_Special_Longi_Clusters)
{
   uint16_t nr_valid_clusters = 6U;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS];
   float32_t cluster_score_array[NR_LONGI_STAT_CLUSTERS] = {};
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   
   /** \setup
   * Create 6 clusters with various properties to make we are able to find special clusters
   * that meet specific requirements. Those clusters are:
   *  - closest to the host on the right (less than 6m) if it is in the same longi position as host
   *  - closest to the host on the left (less than 6m) if it is in the same longi position as host
   *  - longest one that is longer than 30m:
   * Special clusters idx: 3,4,5.
    */
   TEST_SETUP()
   {
      // Cluster to the left of the host that should not be downselected
      float32_t lat_mean_cluster_0 = -5.0F;
      objects[0].vcs_position.x = 5.0F;
      objects[0].vcs_position.y = lat_mean_cluster_0;
      objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = objects[0].vcs_position;
      objects[0].bbox.Set_Center(center);
      objects[1].vcs_position.x = 10.0F;
      objects[1].vcs_position.y = lat_mean_cluster_0;
      objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[1].vcs_position;
      objects[1].bbox.Set_Center(center);
      valid_clusters[0].lat_mean = lat_mean_cluster_0;
      valid_clusters[0].first_object = &objects[0];
      valid_clusters[0].last_object = &objects[1];

      // Cluster to the left of the host that should not be downselected
      float32_t lat_mean_cluster_1 = -4.0F;
      objects[2].vcs_position.x = -20.0F;
      objects[2].vcs_position.y = lat_mean_cluster_1;
      objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[2].vcs_position;
      objects[2].bbox.Set_Center(center);
      objects[3].vcs_position.x = -10.0F;
      objects[3].vcs_position.y = lat_mean_cluster_1;
      objects[3].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[3].vcs_position;
      objects[3].bbox.Set_Center(center);
      valid_clusters[1].lat_mean = lat_mean_cluster_1;
      valid_clusters[1].first_object = &objects[2];
      valid_clusters[1].last_object = &objects[3];
      
      // // Cluster to the right of the host that should not be downselected
      float32_t lat_mean_cluster_2 = 4.0F;
      objects[4].vcs_position.x = -20.0F;
      objects[4].vcs_position.y = lat_mean_cluster_2;
      objects[4].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[4].vcs_position;
      objects[4].bbox.Set_Center(center);
      objects[5].vcs_position.x = -10.0F;
      objects[5].vcs_position.y = lat_mean_cluster_2;
      objects[5].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[5].vcs_position;
      objects[5].bbox.Set_Center(center);
      valid_clusters[2].lat_mean = lat_mean_cluster_2;
      valid_clusters[2].first_object = &objects[4];
      valid_clusters[2].last_object = &objects[5];
      
      // Cluster to the left of the host that should be downselected
      float32_t lat_mean_cluster_3 = -1.0F;
      objects[6].vcs_position.x = -10.0F;
      objects[6].vcs_position.y = lat_mean_cluster_3;
      objects[6].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[6].vcs_position;
      objects[6].bbox.Set_Center(center);
      objects[7].vcs_position.x = 10.0F;
      objects[7].vcs_position.y = lat_mean_cluster_3;
      objects[7].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[7].vcs_position;
      objects[7].bbox.Set_Center(center);
      valid_clusters[3].lat_mean = lat_mean_cluster_3;
      valid_clusters[3].first_object = &objects[6];
      valid_clusters[3].last_object = &objects[7];

      // Cluster to the right of the host that should be downselected
      float32_t lat_mean_cluster_4 = 1.0F;
      objects[8].vcs_position.x = -10.0F;
      objects[8].vcs_position.y = lat_mean_cluster_4;
      objects[8].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[8].vcs_position;
      objects[8].bbox.Set_Center(center);
      objects[9].vcs_position.x = 10.0F;
      objects[9].vcs_position.y = lat_mean_cluster_4;
      objects[9].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[9].vcs_position;
      objects[9].bbox.Set_Center(center);
      valid_clusters[4].lat_mean = lat_mean_cluster_4;
      valid_clusters[4].first_object = &objects[8];
      valid_clusters[4].last_object = &objects[9];

      // Very long cluster that should be downselected
      float32_t lat_mean_cluster_5 = 10.0F;
      objects[10].vcs_position.x = -50.0F;
      objects[10].vcs_position.y = lat_mean_cluster_5;
      objects[10].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[10].vcs_position;
      objects[10].bbox.Set_Center(center);
      objects[11].vcs_position.x = 50.0F;
      objects[11].vcs_position.y = lat_mean_cluster_5;
      objects[11].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[11].vcs_position;
      objects[11].bbox.Set_Center(center);
      valid_clusters[5].lat_mean = lat_mean_cluster_5;
      valid_clusters[5].first_object = &objects[10];
      valid_clusters[5].last_object = &objects[11];
   }
};

/** \purpose
 * Verifies that clusters that meets special requirements are downselected.
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Find_Special_Longi_Clusters, Find_Special_Longi_Clusters_Multiple_Clusters)
{
   /** \action
    * Call function
    */
   Find_Special_Longi_Clusters(nr_valid_clusters, valid_clusters, cluster_score_array);

   /** \result
    * Verify that correct special cluster's score has ben set to -INFTY.
    */

   DOUBLES_EQUAL_TEXT(cluster_score_array[3], -INFTY, F360_EPSILON, "Special cluster to the left of the host not downselected");
   DOUBLES_EQUAL_TEXT(cluster_score_array[4], -INFTY, F360_EPSILON, "Special cluster to the right of the host not downselected");
   DOUBLES_EQUAL_TEXT(cluster_score_array[5], -INFTY, F360_EPSILON, "Special long cluster not downselected");
}




/** \brief
 * Check if downselection of longi static clusters is working properly
 */
TEST_GROUP(f360_downselect_longi_stat_clusters_Downselect_Longi_Stat_Clusters)
{
   F360_Calibrations_T calibs;
   uint16_t nr_valid_clusters = 7U;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS];
   uint16_t nr_downselected_clusters;
   F360_Longi_Stat_Cluster_T downselected_clusters[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   
   /** \setup
    * Create 7 clusters with same length but with different distance to host.
    * They are in ascending order meaning first cluster is closest to host and it
    * should have best score so should be downselected first.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      float32_t lat_mean_cluster_0 = -10.0F;
      objects[0].vcs_position.x = -10.0F;
      objects[0].vcs_position.y = lat_mean_cluster_0;
      objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = objects[0].vcs_position;
      objects[0].bbox.Set_Center(center);
      objects[1].vcs_position.x = 10.0F;
      objects[1].vcs_position.y = lat_mean_cluster_0;
      objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[1].vcs_position;
      objects[1].bbox.Set_Center(center);
      valid_clusters[0].lat_mean = lat_mean_cluster_0;
      valid_clusters[0].first_object = &objects[0];
      valid_clusters[0].last_object = &objects[1];

      float32_t lat_mean_cluster_1 = -12.0F;
      objects[2].vcs_position.x = -10.0F;
      objects[2].vcs_position.y = lat_mean_cluster_1;
      objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[2].vcs_position;
      objects[2].bbox.Set_Center(center);
      objects[3].vcs_position.x = 10.0F;
      objects[3].vcs_position.y = lat_mean_cluster_1;
      objects[3].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[3].vcs_position;
      objects[3].bbox.Set_Center(center);
      valid_clusters[1].lat_mean = lat_mean_cluster_1;
      valid_clusters[1].first_object = &objects[2];
      valid_clusters[1].last_object = &objects[3];

      float32_t lat_mean_cluster_2 = -14.0F;
      objects[4].vcs_position.x = -10.0F;
      objects[4].vcs_position.y = lat_mean_cluster_2;
      objects[4].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[4].vcs_position;
      objects[4].bbox.Set_Center(center);
      objects[5].vcs_position.x = 10.0F;
      objects[5].vcs_position.y = lat_mean_cluster_2;
      objects[5].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[5].vcs_position;
      objects[5].bbox.Set_Center(center);
      valid_clusters[2].lat_mean = lat_mean_cluster_2;
      valid_clusters[2].first_object = &objects[4];
      valid_clusters[2].last_object = &objects[5];

      float32_t lat_mean_cluster_3 = -16.0F;
      objects[6].vcs_position.x = -10.0F;
      objects[6].vcs_position.y = lat_mean_cluster_3;
      objects[6].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[6].vcs_position;
      objects[6].bbox.Set_Center(center);
      objects[7].vcs_position.x = 10.0F;
      objects[7].vcs_position.y = lat_mean_cluster_3;
      objects[7].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[7].vcs_position;
      objects[7].bbox.Set_Center(center);
      valid_clusters[3].lat_mean = lat_mean_cluster_3;
      valid_clusters[3].first_object = &objects[6];
      valid_clusters[3].last_object = &objects[7];

      float32_t lat_mean_cluster_4 = 18.0F;
      objects[8].vcs_position.x = -10.0F;
      objects[8].vcs_position.y = lat_mean_cluster_4;
      objects[8].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[8].vcs_position;
      objects[8].bbox.Set_Center(center);
      objects[9].vcs_position.x = 10.0F;
      objects[9].vcs_position.y = lat_mean_cluster_4;
      objects[9].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[9].vcs_position;
      objects[9].bbox.Set_Center(center);
      valid_clusters[4].lat_mean = lat_mean_cluster_4;
      valid_clusters[4].first_object = &objects[8];
      valid_clusters[4].last_object = &objects[9];

      float32_t lat_mean_cluster_5 = 20.0F;
      objects[10].vcs_position.x = -10.0F;
      objects[10].vcs_position.y = lat_mean_cluster_5;
      objects[10].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[10].vcs_position;
      objects[10].bbox.Set_Center(center);
      objects[11].vcs_position.x = 10.0F;
      objects[11].vcs_position.y = lat_mean_cluster_5;
      objects[11].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[11].vcs_position;
      objects[11].bbox.Set_Center(center);
      valid_clusters[5].lat_mean = lat_mean_cluster_5;
      valid_clusters[5].first_object = &objects[10];
      valid_clusters[5].last_object = &objects[11];

      float32_t lat_mean_cluster_6 = 22.0F;
      objects[12].vcs_position.x = -10.0F;
      objects[12].vcs_position.y = lat_mean_cluster_6;
      objects[12].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[12].vcs_position;
      objects[12].bbox.Set_Center(center);
      objects[13].vcs_position.x = 10.0F;
      objects[13].vcs_position.y = lat_mean_cluster_6;
      objects[13].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[13].vcs_position;
      objects[13].bbox.Set_Center(center);
      valid_clusters[6].lat_mean = lat_mean_cluster_6;
      valid_clusters[6].first_object = &objects[12];
      valid_clusters[6].last_object = &objects[13];
   }
};

/** \purpose
 * Verifies what happens if there are more clusters than max nr of clusters to downselect: MAX_NR_OF_LONGITUDINAL_STAT_CURVES
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Downselect_Longi_Stat_Clusters, Downselect_Longi_Stat_Clusters_More_Clusters_Than_Max)
{
   /** \action
    * Call function
    */
   Downselect_Longi_Stat_Clusters(nr_valid_clusters, valid_clusters, calibs, nr_downselected_clusters, downselected_clusters);

   /** \result
    * Verify that right amount of clusters were downselected and those correct clusters have been prioritized.
    * I.e, the are stored in prioritized descending order in the downselected_clusters array. 
    * We verify this by comparing the cluster properties of the downselected clusters with the properties of the clusters we expect to be prioritized.
    */
   CHECK_EQUAL(MAX_NR_OF_LONGITUDINAL_STAT_CURVES, nr_downselected_clusters);

   for(uint16_t i = 0; i < nr_downselected_clusters; i++)
   {
      DOUBLES_EQUAL(valid_clusters[i].lat_mean, downselected_clusters[i].lat_mean, F360_EPSILON);
      CHECK_EQUAL(valid_clusters[i].first_object, downselected_clusters[i].first_object);
      CHECK_EQUAL(valid_clusters[i].last_object, downselected_clusters[i].last_object);
   }
}


/** \purpose
 * Verifies what happens if there are less clusters than max nr of clusters to downselect: MAX_NR_OF_LONGITUDINAL_STAT_CURVES
 * \req
 * NA
 */
TEST(f360_downselect_longi_stat_clusters_Downselect_Longi_Stat_Clusters, Downselect_Longi_Stat_Clusters_Less_Clusters_Than_Max)
{
   /** \precond
    * Reduce the number of valid clusters to less than the number of curves we allow.
    */
   nr_valid_clusters = 3U;
   /** \action
    * Call function
    */
   Downselect_Longi_Stat_Clusters(nr_valid_clusters, valid_clusters, calibs, nr_downselected_clusters, downselected_clusters);

   /** \result
    * Verify that right amount of clusters were downselected and those correct clusters have been prioritized.
    * I.e, the are stored in prioritized descending order in the downselected_clusters array. 
    * We verify this by comparing the cluster properties of the downselected clusters with the properties of the clusters we expect to be prioritized.
    */
   CHECK_EQUAL(nr_valid_clusters, nr_downselected_clusters);

   for(uint16_t i = 0; i < nr_downselected_clusters; i++)
   {
      DOUBLES_EQUAL(valid_clusters[i].lat_mean, downselected_clusters[i].lat_mean, F360_EPSILON);
      CHECK_EQUAL(valid_clusters[i].first_object, downselected_clusters[i].first_object);
      CHECK_EQUAL(valid_clusters[i].last_object, downselected_clusters[i].last_object);
   }
}