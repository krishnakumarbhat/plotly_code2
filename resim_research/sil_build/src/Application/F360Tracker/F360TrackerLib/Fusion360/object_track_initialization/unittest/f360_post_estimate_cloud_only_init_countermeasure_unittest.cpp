/** \file
* This file contains unit tests for content of f360_post_estimate_cloud_only_init_countermeasure.cpp file
*/

#include <CppUTest/TestHarness.h>
#include "f360_post_estimate_cloud_only_init_countermeasure.h"
#include "f360_set_variant.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup f360_post_estimate_cloud_only_init_countermeasure
* @{
*/

/** \brief
* Test functionality of the Post_Estimate_Cloud_Only_Init_Countermeasure algorithm.
*/
TEST_GROUP(f360_post_estimate_cloud_only_init_countermeasure)
{
    // Declare common variables used within all tests in this test group.
    F360_Host_T host;
    F360_Cluster_T cluster;
    CONF3_T posdiff_confidence;
    float32_t longvel_estimate;
    F360_Track_Init_T init_type;

    /** \setup
    * Initialize inputs after init estimate of cluster
    */
    TEST_SETUP()
    {
    /** \setup
    * Initialize tracker calibrations
    **/
    cluster.vcs_position_x = 50.0F;
    cluster.vcs_position_y = 10.0F;
    host.speed = 30.0F;
    posdiff_confidence = CONF3_NONE;
    longvel_estimate = 25.0F;
    }
};
/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure to inhibit cloud only init if all conditions met
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, all_conditions_met_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init
    */
    init_type = F360_TRACK_INIT_CLOUD;
    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be changed to invalid.
    */
    CHECK_TEXT(F360_TRACK_INIT_INVALID == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should be active but it is not");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not inhibit cloud only init when object init long position threshold is not met
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, outside_long_pos_thresh_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init and set up cluster long position outside threshold
    */
    init_type = F360_TRACK_INIT_CLOUD;
    cluster.vcs_position_x = 39.5F;
    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_CLOUD == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not inhibit cloud only init when object init lat position threshold is not met
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, outside_lat_pos_thresh_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init and set up cluster lat position outside threshold
    */
    init_type = F360_TRACK_INIT_CLOUD;
    cluster.vcs_position_y = 3.4F;
    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_CLOUD == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not inhibit cloud only init when host speed threshold is not met
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, outside_host_speed_thresh_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init and set up host speed outside threshold
    */
    init_type = F360_TRACK_INIT_CLOUD;
    host.speed = 24.9F;
    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_CLOUD == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not inhibit cloud only init when init object speed threshold is not met
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, outside_object_speed_thresh_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init and set up init object speed outside threshold
    */
    init_type = F360_TRACK_INIT_CLOUD;
    longvel_estimate = 19.9F;
    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_CLOUD == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not inhibit cloud only init when init object speed is more than host speed
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, object_faster_than_host_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init and set up init object speed greater than host speed
    */
    init_type = F360_TRACK_INIT_CLOUD;
    host.speed = 26.0F;
    longvel_estimate = 26.5F;
    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_CLOUD == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not inhibit cloud only init when posdiff confidence is not invalid
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, posdiff_confidence_not_invalid_check)
{
    /** \precond
    * Set up object as being calculated as cloud only init and set up posdiff confidence as LOW
    */
    init_type = F360_TRACK_INIT_CLOUD;
    posdiff_confidence = CONF3_LOW;

    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_CLOUD == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}

/** \purpose
* Check Post_Estimate_Cloud_Only_Init_Countermeasure does not set init type to invalid when init type estimate is not cloud only init
* \req
* NA
*/
TEST(f360_post_estimate_cloud_only_init_countermeasure, not_cloud_only_init_check)
{
    /** \precond
    * Set up object as being calculated as weighted init
    */
    init_type = F360_TRACK_INIT_WEIGHTED;

    /** \action
    * Call Post_Estimate_Cloud_Only_Init_Countermeasure().
    */
    Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, posdiff_confidence, longvel_estimate, init_type);

    /** \result
    * Expect init type to be unchanged.
    */
    CHECK_TEXT(F360_TRACK_INIT_WEIGHTED == init_type, "Post_Estimate_Cloud_Only_Init_Countermeasure should not be active but is");
}


/** @}*/
