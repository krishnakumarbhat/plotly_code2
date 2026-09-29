/** \file
   This file contains data types sanity checks on some important interface signals
*/
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cfloat>
#include "f360_tracker.h"
#include "f360_update_relative_timestamps.h"
#include "f360_time_update_tracks.h"
#include "f360_pre_association_track_management.h"
#include "f360_detection_to_track_association.h"
#include "f360_clustering.h"
#include "f360_cluster_grouping.h"
#include "f360_track_grouping.h"
#include "f360_internal_preprocessing.h"
#include "f360_inputs_preprocessing.h"
#include "f360_initialize_tracks.h"
#include "f360_measurement_update_tracks.h"
#include "f360_post_update_track_adjustments.h"
#include "f360_track_classification.h"
#include "f360_track_validity.h"
#include "f360_track_downselection.h"
#include "f360_e2e_protection.h"
#include "f360_static_environment_class.h"
#include "f360_trailer_detector_core.h"
#include "f360_sanity_check.h"
#include "f360_sensor_postprocessing.h"
#include "f360_get_wall_time.h"
#include "f360_occlusion.h"
#include "ocg_occupancy_grid_types.h"
#include "init_tracker_info_T.h"
#include "init_object_tracks_T.h"
#include "init_clusters_T.h"
#include "init_core_T.h"
#include "init_det_hist_T.h"

#include "init_det_props_T.h"
#include "init_globals_T.h"
#include "init_host_props_T.h"
#include "init_host_T.h"
#include "init_raw_detect_T.h"
#include "init_sensor_calib_T.h"
#include "init_sensor_props_T.h"
#include "init_sensors_T.h"
#include "init_timing_info_T.h"
#include "init_static_env_class_T.h"
#include "init_trailer_detector_core.h"
#include "init_occupancy_grid_T.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "Interface_Signals_range.h"

/*F360_Input_Init header file contains input_init() macro  which call input test vector initialization */
#include "F360_Input_Init.h"
using namespace f360_variant_A;

/** \defgroup  f360_tracker_trackclassification_sanity_checks
 *  @{
 */

 /** \brief
 *   This test suits aims to sanity check on data types of the important interface signals
 **/
TEST_GROUP(f360_tracker_trackclassification_sanity_checks)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"

   /** \setup
   * Nothing to setup in this test group
   **/
   TEST_SETUP()
   {
      //input_init
      input_init();

   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
 
};

/**
*\purpose  Describe purpose of test
*\req put in requirement tag if any otherwise set to NA
*/

/**
**********************************************************************************************************
*                                F360_Host_T                                                             *
**********************************************************************************************************
**/

//host.speed

/**
 *\purpose  Purpose of this test is to verify whether input value is verified when host.speed is a nominal value.
 *\req
 * CPR-4779 ,CPR-4266 , CPR-4281, CPR-4268
 **/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_speed_nominal)
{
   /** \precond
    set host.speed to get a nominal  value which is the value from inputs
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.speed <= Max_Host_Speed_M_Per_S, "host.speed is above Max_Host_Speed_M_Per_S in a nominal value test case");
   CHECK_TEXT(host.speed >= Min_Host_Speed_M_Per_S, "host.speed is below Min_Host_Speed_M_Per_S in a nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.speed is a max value.
*\req
* CPR-4779 ,CPR-4266 , CPR-4281, CPR-4268
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_speed_max)
{
   /** \precond
    set host.speed to get a max value
    **/
   host.speed = Max_Host_Speed_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.speed <= Max_Host_Speed_M_Per_S, "host.speed is above Max_Host_Speed_M_Per_S in a max value test case");
   CHECK_TEXT(host.speed >= Min_Host_Speed_M_Per_S, "host.speed is below Min_Host_Speed_M_Per_S in a max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.speed is above max value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4268
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_speed_above_max)
// {
   // /** \precond
   // set host.speed to get above max value
   // **/
   // host.speed = Max_Host_Speed_M_Per_S+ 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.speed <= Max_Host_Speed_M_Per_S, "host.speed is above Max_Host_Speed_M_Per_S in above max value test case");
   // CHECK_TEXT(host.speed >= Min_Host_Speed_M_Per_S, "host.speed is below Min_Host_Speed_M_Per_S in above max value test case" );
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.speed is a min value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4268
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_speed_min)
{
   /** \precond
    set host.speed to get a min value
    **/
   host.speed = Min_Host_Speed_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.speed <= Max_Host_Speed_M_Per_S, "host.speed is above Max_Host_Speed_M_Per_S in a min value test case");
   CHECK_TEXT(host.speed >= Min_Host_Speed_M_Per_S, "host.speed is below Min_Host_Speed_M_Per_S in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.speed is a below min value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4268
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_speed_below_min)
// {
   // /** \precond
   // set host.speed to get a below min value
   // **/
   // host.speed = Min_Host_Speed_M_Per_S - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.speed <= Max_Host_Speed_M_Per_S, "host.speed is above Max_Host_Speed_M_Per_S in a below min value test case");
   // CHECK_TEXT(host.speed >= Min_Host_Speed_M_Per_S, "host.speed is below Min_Host_Speed_M_Per_S in a below min value test case" );
// }

// host.acceleration

/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.acceleration is a nominal value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4270
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_acceleration_nominal)
{
   /** \precond
    set host.acceleration to get a nominal  value which is the value from inputs
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.acceleration <= Max_Host_Acceleration, "host.acceleration is above Max_Host_Acceleration in a nominal value test case");
   CHECK_TEXT(host.acceleration >= Min_Host_Acceleration, "host.acceleration is below Min_Host_Acceleration in a nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.acceleration is a max value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4270
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_acceleration_max)
{
   /** \precond
    set host.acceleration to get a max value
    **/
   host.acceleration = Max_Host_Acceleration;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.acceleration <= Max_Host_Acceleration, "host.acceleration is above Max_Host_Acceleration in a max value test case");
   CHECK_TEXT(host.acceleration >= Min_Host_Acceleration, "host.acceleration is below Min_Host_Acceleration in a max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.acceleration is above max value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4270
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_acceleration_above_max)
// {
   // /** \precond
   // set host.acceleration to get above max value
   // **/
   // host.acceleration = Max_Host_Acceleration+ 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.acceleration <= Max_Host_Acceleration, "host.acceleration is above Max_Host_Acceleration in above max value test case");
   // CHECK_TEXT(host.acceleration >= Min_Host_Acceleration, "host.acceleration is below Min_Host_Acceleration in above max value test case" );
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.acceleration is a min value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4270
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_acceleration_min)
{
   /** \precond
    set host.acceleration to get a min value
    **/
   host.acceleration = Min_Host_Acceleration;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.acceleration <= Max_Host_Acceleration, "host.acceleration is above Max_Host_Acceleration in a min value test case");
   CHECK_TEXT(host.acceleration >= Min_Host_Acceleration, "host.acceleration is below Min_Host_Acceleration in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.acceleration is a below min value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4270
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_acceleration_below_min)
// {
   // /** \precond
   // set host.acceleration to get a below value
   // **/
   // host.acceleration = Min_Host_Acceleration - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.acceleration <= Max_Host_Acceleration, "host.acceleration is above Max_Host_Acceleration in a below min value test case");
   // CHECK_TEXT(host.acceleration >= Min_Host_Acceleration, "host.acceleration is below Min_Host_Acceleration in a below min value test case" );
// }

// host.yaw_rate_rad

/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.yaw_rate_rad is a nominal value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4271
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_yaw_rate_rad_nominal)
{
   /** \precond
    set host.yaw_rate_rad to get a nominal  value which is the value from inputs
    **/

   /** \action
   * call TrackClassification module function
   **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.yaw_rate_rad <= Max_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is above Max_Host_Yaw_Rate_Rad_M_Per_S_2 in a nominal value test case");
   CHECK_TEXT(host.yaw_rate_rad >= Min_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is below Min_Host_Yaw_Rate_Rad_M_Per_S_2 in a nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.yaw_rate_rad is a max value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4271
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_yaw_rate_rad_max)
{
   /** \precond
    set host.yaw_rate_rad to get a max value
    **/
   host.yaw_rate_rad = Max_Host_Yaw_Rate_Rad_M_Per_S_2;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.yaw_rate_rad <= Max_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is above Max_Host_Yaw_Rate_Rad_M_Per_S_2 in a max value test case");
   CHECK_TEXT(host.yaw_rate_rad >= Min_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is below Min_Host_Yaw_Rate_Rad_M_Per_S_2 in a max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.yaw_rate_rad is above max value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4271
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_yaw_rate_rad_above_max)
// {
   // /** \precond
   // set host.yaw_rate_rad to get above max value
   // **/
   // // host.yaw_rate_rad = Max_Host_Yaw_Rate_Rad_M_Per_S_2+ 1.0F; 
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.yaw_rate_rad <= Max_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is above Max_Host_Yaw_Rate_Rad_M_Per_S_2 in above max value test case");
   // CHECK_TEXT(host.yaw_rate_rad >= Min_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is below Min_Host_Yaw_Rate_Rad_M_Per_S_2 in above max value test case" );
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.yaw_rate_rad is a min value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4271
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_yaw_rate_rad_min)
{
   /** \precond
    set host.yaw_rate_rad to get a min value
    **/
   host.yaw_rate_rad = Min_Host_Yaw_Rate_Rad_M_Per_S_2;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.yaw_rate_rad <= Max_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is above Max_Host_Yaw_Rate_Rad_M_Per_S_2 in a min value test case");
   CHECK_TEXT(host.yaw_rate_rad >= Min_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is below Min_Host_Yaw_Rate_Rad_M_Per_S_2 in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.yaw_rate_rad is a below min value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4271
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_yaw_rate_rad_below_min)
// {
   // /** \precond
   // set host.yaw_rate_rad to get a below min value
   // **/
   // // host.yaw_rate_rad = Min_Host_Yaw_Rate_Rad_M_Per_S_2 - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.yaw_rate_rad <= Max_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is above Max_Host_Yaw_Rate_Rad_M_Per_S_2 in a below min value test case");
   // CHECK_TEXT(host.yaw_rate_rad >= Min_Host_Yaw_Rate_Rad_M_Per_S_2, "host.yaw_rate_rad is below Min_Host_Yaw_Rate_Rad_M_Per_S_2 in a below min value test case" );
// }

// host.curvature_rear

/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.curvature_rear is a nominal value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4273
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_curvature_rear_nominal)
{
   /** \precond
    set host.curvature_rear to get a nominal  value which is the value from inputs
    **/

   /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.curvature_rear <= Max_Host_curvature_rear_1_Per_M, "host.curvature_rear is above Max_Host_curvature_rear_1_Per_M in a nominal value test case");
   CHECK_TEXT(host.curvature_rear >= Min_Host_curvature_rear_1_Per_M, "host.curvature_rear is below Min_Host_curvature_rear_1_Per_M in a nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.curvature_rear is a max value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4273
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_curvature_rear_max)
{
   /** \precond
    set host.curvature_rear to get a max value
    **/
   host.curvature_rear = Max_Host_curvature_rear_1_Per_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.curvature_rear <= Max_Host_curvature_rear_1_Per_M, "host.curvature_rear is above Max_Host_curvature_rear_1_Per_M in a max value test case");
   CHECK_TEXT(host.curvature_rear >= Min_Host_curvature_rear_1_Per_M, "host.curvature_rear is below Min_Host_curvature_rear_1_Per_M in a max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.curvature_rear is above max value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4273
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_curvature_rear_above_max)
// {
   // /** \precond
   // set host.curvature_rear to get above max value
   // **/
   // // host.curvature_rear = Max_Host_curvature_rear_1_Per_M+ 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.curvature_rear <= Max_Host_curvature_rear_1_Per_M, "host.curvature_rear is above Max_Host_curvature_rear_1_Per_M in above max value test case");
   // CHECK_TEXT(host.curvature_rear >= Min_Host_curvature_rear_1_Per_M, "host.curvature_rear is below Min_Host_curvature_rear_1_Per_M in above max value test case" );
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.curvature_rear is a min value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4273
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_curvature_rear_min)
{
   /** \precond
    set host.curvature_rear to get a min value
    **/
   host.curvature_rear = Min_Host_curvature_rear_1_Per_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(host.curvature_rear <= Max_Host_curvature_rear_1_Per_M, "host.curvature_rear is above Max_Host_curvature_rear_1_Per_M in a min value test case");
   CHECK_TEXT(host.curvature_rear >= Min_Host_curvature_rear_1_Per_M, "host.curvature_rear is below Min_Host_curvature_rear_1_Per_M in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.curvature_rear is a below min value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4273
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_host_curvature_rear_below_min)
// {
   // /** \precond
   // set host.curvature_rear to get a below min value
   // **/
   // // host.curvature_rear = Min_Host_curvature_rear_1_Per_M - 1.0F;  
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(host.curvature_rear <= Max_Host_curvature_rear_1_Per_M, "host.curvature_rear is above Max_Host_curvature_rear_1_Per_M in a below min value test case");
   // CHECK_TEXT(host.curvature_rear >= Min_Host_curvature_rear_1_Per_M, "host.curvature_rear is below Min_Host_curvature_rear_1_Per_M in a below min value test case" );
// }

// host.dist_rear_axle_to_vcs_m
/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.dist_rear_axle_to_vcs_m is a nominal value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281,CPR-4274
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_dist_rear_axle_to_vcs_m_nominal)
{
   /** \precond
    set host.dist_rear_axle_to_vcs_m to get a nominal  value which is the value from inputs
    **/
   
   /** \action
   * call TrackClassification module function
   **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/
   CHECK_TEXT(host.dist_rear_axle_to_vcs_m <= Max_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is above Max_Host_dist_rear_axle_to_vcs_m in a nominal value test case");
   CHECK_TEXT(host.dist_rear_axle_to_vcs_m >= Min_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is below Min_Host_dist_rear_axle_to_vcs_m in a nominal value test case");
}

/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.dist_rear_axle_to_vcs_m is a max value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281 ,CPR-4274
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_dist_rear_axle_to_vcs_m_max)
{
   /** \precond
    set host.dist_rear_axle_to_vcs_m to get a max  value.
    **/
   host.dist_rear_axle_to_vcs_m = Max_Host_dist_rear_axle_to_vcs_m;
   /** \action
   * call TrackClassification module function
   **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/
   CHECK_TEXT(host.dist_rear_axle_to_vcs_m <= Max_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is above Max_Host_dist_rear_axle_to_vcs_m in a max value test case");
   CHECK_TEXT(host.dist_rear_axle_to_vcs_m >= Min_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is below Min_Host_dist_rear_axle_to_vcs_m in a max value test case");
}

// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.dist_rear_axle_to_vcs_m is above max value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281,CPR-4274
// **/
// TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_dist_rear_axle_to_vcs_m_above_max)
// {
   // /** \precond
    // set host.dist_rear_axle_to_vcs_m to get above max  value.
    // **/
      // host.dist_rear_axle_to_vcs_m = Max_Host_dist_rear_axle_to_vcs_m + 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
   // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
     // *- check that the output is within allowed range
    // **/
   // CHECK_TEXT(host.dist_rear_axle_to_vcs_m <= Max_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is above Max_Host_dist_rear_axle_to_vcs_m in above  max value test case");
   // CHECK_TEXT(host.dist_rear_axle_to_vcs_m >= Min_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is below Min_Host_dist_rear_axle_to_vcs_m in above max value test case");
// }

/**
*\purpose  Purpose of this test is to verify whether input value is verified when host.dist_rear_axle_to_vcs_m is a min value.
*\req
*CPR-4779 ,CPR-4266 , CPR-4281, CPR-4274
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_dist_rear_axle_to_vcs_m_min)
{
   /** \precond
    set host.dist_rear_axle_to_vcs_m to get a min  value.
    **/
      host.dist_rear_axle_to_vcs_m = Min_Host_dist_rear_axle_to_vcs_m;
   /** \action
   * call TrackClassification module function
   **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/
   CHECK_TEXT(host.dist_rear_axle_to_vcs_m <= Max_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is above Max_Host_dist_rear_axle_to_vcs_m in a min value test case");
   CHECK_TEXT(host.dist_rear_axle_to_vcs_m >= Min_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is below Min_Host_dist_rear_axle_to_vcs_m in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when host.dist_rear_axle_to_vcs_m is a below min value.
// *\req
// *CPR-4779 ,CPR-4266 , CPR-4281, CPR-4274
// **/
// TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_host_dist_rear_axle_to_vcs_m_below_min)
// {
   // /** \precond
    // set host.dist_rear_axle_to_vcs_m to get a below min  value.
    // **/
      // host.dist_rear_axle_to_vcs_m = Min_Host_dist_rear_axle_to_vcs_m - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
   // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
     // *- check that the output is within allowed range
    // **/
   // CHECK_TEXT(host.dist_rear_axle_to_vcs_m <= Max_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is above Max_Host_dist_rear_axle_to_vcs_m in a below min value test case");
   // CHECK_TEXT(host.dist_rear_axle_to_vcs_m >= Min_Host_dist_rear_axle_to_vcs_m, "host.dist_rear_axle_to_vcs_m is below Min_Host_dist_rear_axle_to_vcs_m in a below min value test case");
// }
/**
**********************************************************************************************************
*                                F360_Radar_Sensor_T                                               *
**********************************************************************************************************
**/

//sensors[0].constant.id

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.id is a nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_id_nominal)
{
   /** \precond
    set sensors[0].constant.id  to get a nominal value  from normal inputs.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.id <= Max_Sensor_Calibs_ID, "sensors[0].constant.id  is above Max_Sensor_Calibs_ID in a nominal value test case");
   CHECK_TEXT(sensors[0].constant.id >= Min_Sensor_Calibs_ID, "sensors[0].constant.id  is below Min_Sensor_Calibs_ID in a nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.id is a max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_id_max)
{
   /** \precond
    set sensors[0].constant.id  to get a max value.
    **/
   sensors[0].constant.id = Max_Sensor_Calibs_ID;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.id <= Max_Sensor_Calibs_ID, "sensors[0].constant.id  is above Max_Sensor_Calibs_ID in a max value test case");
   CHECK_TEXT(sensors[0].constant.id >= Min_Sensor_Calibs_ID, "sensors[0].constant.id  is below Min_Sensor_Calibs_ID in a max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.id is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_id_above_max)
// {
   // /** \precond
   // set sensors[0].constant.id  to get above max value.
   // **/
   // sensors[0].constant.id = Max_Sensor_Calibs_ID + 1;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(sensors[0].constant.id  <= Max_Sensor_Calibs_ID, "sensors[0].constant.id  is above Max_Sensor_Calibs_ID in above max value test case");
   // CHECK_TEXT(sensors[0].constant.id  >= Min_Sensor_Calibs_ID, "sensors[0].constant.id  is below Min_Sensor_Calibs_ID in above max value test case" );
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.id is a min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_id_min)
{
   /** \precond
    set sensors[0].constant.id  to get a min value.
    **/
   sensors[0].constant.id = Min_Sensor_Calibs_ID;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.id <= Max_Sensor_Calibs_ID, "sensors[0].constant.id  is above Max_Sensor_Calibs_ID in a min value test case");
   CHECK_TEXT(sensors[0].constant.id >= Min_Sensor_Calibs_ID, "sensors[0].constant.id  is below Min_Sensor_Calibs_ID in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.id is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_id_below_min)
// {
   // /** \precond
   // set sensors[0].constant.id  to get a  below min value.
   // **/
   // sensors[0].constant.id = Min_Sensor_Calibs_ID - 1;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(sensors[0].constant.id  <= Max_Sensor_Calibs_ID, "sensors[0].constant.id  is above Max_Sensor_Calibs_ID in below min value test case");
   // CHECK_TEXT(sensors[0].constant.id  >= Min_Sensor_Calibs_ID, "sensors[0].constant.id  is below Min_Sensor_Calibs_ID in below min value test case" );
// }

//sensors[0].constant.sensor_type
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.sensor_type is correct value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_sensor_type_correct)
{
   /** \precond
    set sensors[0].constant.sensor_type  to get a  correct value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.sensor_type == F360_SENSOR_TYPE_MRR3_RADAR, "sensors[0].constant.sensor_type  isnt the correct sensor in correct sensor_type test case");

}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.sensor_type is wrong value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_sensor_type_wrong)
// {
   // /** \precond
   // set sensors[0].constant.sensor_type  to get a  wrong value.
   // **/
   // sensors[0].constant.sensor_type = F360_SENSOR_TYPE_UNKNOWN;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

   // CHECK_TEXT(sensors[0].constant.sensor_type  == F360_SENSOR_TYPE_MRR3_RADAR, "sensors[0].constant.sensor_type isnt the correct sensor in sensor_type wrong test case");

// }
//sensors[0].constant.mounting_location
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_location is correct value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_location_correct)
{
   /** \precond
    set sensors[0].constant.mounting_location  to get a  correct value.
    **/
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT((sensors[0].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_FORWARD) ||
      (sensors[0].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_FORWARD) ||
      (sensors[0].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR) ||
      (sensors[0].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR), "sensors[0].constant.mounting_location  isnt the correct value in correct sensor_type test case");

}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_location is wrong value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_location_wrong)
// {
   // /** \precond
   // set sensors[0].constant.mounting_location  to get a  wrong value.
   // **/
   // sensors[0].constant.mounting_location == F360_MOUNTING_LOCATION_UNKNOWN ;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(   (sensors[0].constant.mounting_location  == F360_MOUNTING_LOCATION_LEFT_FORWARD)  ||
            // (sensors[0].constant.mounting_location  == F360_MOUNTING_LOCATION_RIGHT_FORWARD) ||
            // (sensors[0].constant.mounting_location  == F360_MOUNTING_LOCATION_LEFT_REAR)   ||
            // (sensors[0].constant.mounting_location  == F360_MOUNTING_LOCATION_RIGHT_REAR), "sensors[0].constant.mounting_location  isnt the correct value in correct sensor_type test case");

// }

//sensors[0].constant.mounting_position.vcs_position.longitudinal

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.longitudinal is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_long_nominal)
{
   /** \precond
    set sensors[0].constant.mounting_position  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal <= Max_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above Max_Mounting_Position_long_M value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal >= Min_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is below   Min_Mounting_Position_long_Mvalue  in nominal value   test case ")
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.longitudinal is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_long_max)
{
   /** \precond
    set sensors[0].constant.mounting_position  to get a  max value.
    **/
   sensors[0].constant.mounting_position.vcs_position.longitudinal = Max_Mounting_Position_long_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal <= Max_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above Max_Mounting_Position_long_M value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal >= Min_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is below Min_Mounting_Position_long_M value  in max value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.longitudinal is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_long_above_max)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position  to get above  max value.
   // **/
   // sensors[0].constant.mounting_position.vcs_position.longitudinal = Max_Mounting_Position_long_M +1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal <= Max_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above max value  in above max value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal >= Min_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above max value  in above max value   test case ")
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.longitudinal is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_long_min)
{
   /** \precond
    set sensors[0].constant.mounting_position  to get min value.
    **/
   sensors[0].constant.mounting_position.vcs_position.longitudinal = Min_Mounting_Position_long_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal <= Max_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above Max_Mounting_Position_long_M value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal >= Min_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is below Min_Mounting_Position_long_M value  in min value  test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.longitudinal is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_long_below_min)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position  to get below min value.
   // **/
   // sensors[0].constant.mounting_position.vcs_position.longitudinal = Min_Mounting_Position_long_M -1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal <= Max_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above max value  in below min value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.longitudinal >= Min_Mounting_Position_long_M, "sensors[0].constant.mounting_position.vcs_position.longitudinal  is above max value  in below min value   test case ")
// }

//sensors[0].constant.mounting_position.vcs_position.lateral

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.lateral is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_lat_nominal)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_position.lateral  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral <= Max_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is above Max_Mounting_Position_lat_M value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral >= Min_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is below  Min_Mounting_Position_lat_M value  in nominal value   test case ")
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.lateral is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_lat_max)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_position.lateral  to get a  max value.
    **/
   sensors[0].constant.mounting_position.vcs_position.lateral = Max_Mounting_Position_lat_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral <= Max_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is above max value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral >= Min_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is below min value  in max value  sensors[0].constant.mounting_position.lateral test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.lateral is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_lat_above_max)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_position.lateral  to get above  max value.
   // **/
   // sensors[0].constant.mounting_position.vcs_position.lateral = Max_Mounting_Position_lat_M +1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral <= Max_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is above max value  in above max value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral >= Min_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is below min value  in above max value   test case ")
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.lateral is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_lat_min)
{
   /** \precond
    set sensors[0].constant.mounting_position  to get min value.
    **/
   sensors[0].constant.mounting_position.vcs_position.lateral = Min_Mounting_Position_lat_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral <= Max_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is above min value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral >= Min_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is below min  value  in min value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.lateral is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_lat_below_min)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_position.lateral  to get below min value.
   // **/
   // sensors[0].constant.mounting_position.vcs_position.lateral = Min_Mounting_Position_lat_M -1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral <= Max_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is above max value  in below min value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.lateral >= Min_Mounting_Position_lat_M, "sensors[0].constant.mounting_position.vcs_position.lateral  is below min value  in below min value   test case ")
// }
//sensors[0].constant.mounting_position.vcs_position.height

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.height is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_height_nominal)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_position.height  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height <= Max_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above Max_Mounting_Position_height_M value  in nominal value  test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height >= Min_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is below Min_Mounting_Position_height_M value  in nominal value   test case ")
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.height is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_height_max)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_position.height  to get a  max value.
    **/
   sensors[0].constant.mounting_position.vcs_position.height = Max_Mounting_Position_height_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height <= Max_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above Max_Mounting_Position_height_M value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height >= Min_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is below Min_Mounting_Position_height_M value  in max value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.height is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_height_above_max)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_position.height  to get above  max value.
   // **/
   // sensors[0].constant.mounting_position.vcs_position.height = Max_Mounting_Position_height_M +1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height <= Max_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above max value  in above max value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height >= Min_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above max value  in above max value   test case ")
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.height is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_height_min)
{
   /** \precond
    set sensors[0].constant.mounting_position  to get min value.
    **/
   sensors[0].constant.mounting_position.vcs_position.height = Min_Mounting_Position_height_M;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height <= Max_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above Max_Mounting_Position_height_M value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height >= Min_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is below Min_Mounting_Position_height_M value  in min value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_position.height is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_height_below_min)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_position.height  to get below min value.
   // **/
   // sensors[0].constant.mounting_position.vcs_position.height = Min_Mounting_Position_height_M -1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height <= Max_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above max value  in below min value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_position.height >= Min_Mounting_Position_height_M, "sensors[0].constant.mounting_position.vcs_position.height  is above max value  in below min value   test case ")
// }

//sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle is nominal value.
*\req CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_azimuth_nominal)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle <= Max_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is above Max_Mounting_Position_azimuth value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle >= Min_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is below Min_Mounting_Position_azimuth  value  in nominal value  test case ")
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_azimuth_max)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get a  max value.
    **/
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = Max_Mounting_Position_azimuth;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle <= Max_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle is above Max_Mounting_Position_azimuth value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle >= Min_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is below Min_Mounting_Position_azimuth value  in max value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_azimuth_above_max)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get above  max value.
   // **/
   // sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = Max_Mounting_Position_azimuth +1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle <= Max_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is above max value  in above max value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle >= Min_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is below min value  in above max value   test case ")
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_azimuth_min)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get min value.
    **/
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = Min_Mounting_Position_azimuth;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle <= Max_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is above Max_Mounting_Position_azimuth value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle >= Min_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is below Min_Mounting_Position_azimuth value  in min value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_azimuth_below_min)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get below min value.
   // **/
   // sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = Min_Mounting_Position_azimuth -1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle <= Max_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is above max value  in below min value test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle >= Min_Mounting_Position_azimuth, "sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  is below min value  in below min value test case ")
// }

//sensors[0].constant.mounting_position.vcs_boresight_elevation_angle

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_elevation_angle is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_elevation_nominal)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle <= Max_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is above Max_Mounting_Position_elevation value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle >= Min_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is below Min_Mounting_Position_elevation  value  in nominal value  test case ")
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_elevation_angle is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_elevation_max)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get a  max value.
    **/
   sensors[0].constant.mounting_position.vcs_boresight_elevation_angle = Max_Mounting_Position_elevation;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle <= Max_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle is above max value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle >= Min_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is below min value  in max value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_elevation_angle is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_elevation_above_max)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  to get above  max value.
   // **/
   // sensors[0].constant.mounting_position.vcs_boresight_elevation_angle = Max_Mounting_Position_elevation +1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle <= Max_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is above max value  in above max value   test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle >= Min_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is below min value  in above max value   test case ")
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_elevation_angle is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_mounting_position_elevation_min)
{
   /** \precond
    set sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle  to get min value.
    **/
   sensors[0].constant.mounting_position.vcs_boresight_elevation_angle = Min_Mounting_Position_elevation;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle <= Max_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is above min value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle >= Min_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is below min value  in min value   test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.mounting_position.vcs_boresight_elevation_angle is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_mounting_position_elevation_below_min)
// {
   // /** \precond
   // set sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  to get below min value.
   // **/
   // sensors[0].constant.mounting_position.vcs_boresight_elevation_angle = Min_Mounting_Position_elevation -1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle <= Max_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is above max value  in below min value test case ");
  // CHECK_TEXT(sensors[0].constant.mounting_position.vcs_boresight_elevation_angle >= Min_Mounting_Position_elevation, "sensors[0].constant.mounting_position.vcs_boresight_elevation_angle  is below min value  in below min value test case ")
// }

//sensors[0].constant.polarity

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.polarity is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_polarity_nominal)
{
   /** \precond
    set sensors[0].constant.polarity  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.polarity <= Max_Sensor_Polarity, "sensors[0].constant.polarity  is above Max_Sensor_Polarity value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.polarity >= Min_Sensor_Polarity, "sensors[0].constant.polarity  is below Min_Sensor_Polarity  value  in nominal value  test case ")
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.polarity is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_polarity_max)
{
   /** \precond
    set sensors[0].constant.polarity  to get a  max value.
    **/
   sensors[0].constant.polarity = Max_Sensor_Polarity;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.polarity <= Max_Sensor_Polarity, "sensors[0].constant.polarity  is above Max_Sensor_Polarity value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.polarity >= Min_Sensor_Polarity, "sensors[0].constant.polarity  is below Min_Sensor_Polarity  value  in max value  test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.polarity is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_polarity_above_max)
// {
   // /** \precond
   // set sensors[0].constant.polarity  to get above max value.
   // **/
   // sensors[0].constant.polarity = Max_Sensor_Polarity + 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

  // CHECK_TEXT(sensors[0].constant.polarity <= Max_Sensor_Polarity , "sensors[0].constant.polarity  is above max value  in above max value   test case ");
  // CHECK_TEXT(sensors[0].constant.polarity >= Min_Sensor_Polarity, "sensors[0].constant.polarity  is below min  value  in above max value  test case ")
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.polarity is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_polarity_min)
{
   /** \precond
    set sensors[0].constant.polarity  to get a  min value.
    **/
   sensors[0].constant.polarity = Min_Sensor_Polarity;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.polarity <= Max_Sensor_Polarity, "sensors[0].constant.polarity  is above Max_Sensor_Polarity value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.polarity >= Min_Sensor_Polarity, "sensors[0].constant.polarity  is below Min_Sensor_Polarity  value  in min value  test case ")
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.polarity is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_polarity_below_min)
// {
   // /** \precond
   // set sensors[0].constant.polarity  to get a  below min value.
   // **/
   // sensors[0].constant.polarity = Min_Sensor_Polarity - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

  // CHECK_TEXT(sensors[0].constant.polarity <= Max_Sensor_Polarity,  "sensors[0].constant.polarity  is above max value  in below min value   test case ");
  // CHECK_TEXT(sensors[0].constant.polarity >= Min_Sensor_Polarity, "sensors[0].constant.polarity  is below min value  in below min value  test case ")
// }
//sensors[0].constant.fov_min_az_rad

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_min_az_rad is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_fov_min_az_rad_nominal)
{
   /** \precond
    set sensors[0].constant.fov_min_az_rad  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[0]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[1]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[2]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[3]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[0]  is below min value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[1]  is below min value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[2]  is below min value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[3]  is below min value  in nominal value   test case ");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_min_az_rad is max value.
*\req CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_fov_min_az_rad_max)
{
   /** \precond
    set sensors[0].constant.fov_min_az_rad  to get a  max value.
    **/
   sensors[0].constant.fov_min_az_rad[0] = Max_Sensor_Calibs_fov_min_az_rad;
   sensors[0].constant.fov_min_az_rad[1] = Max_Sensor_Calibs_fov_min_az_rad;
   sensors[0].constant.fov_min_az_rad[2] = Max_Sensor_Calibs_fov_min_az_rad;
   sensors[0].constant.fov_min_az_rad[3] = Max_Sensor_Calibs_fov_min_az_rad;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[0]  is above max value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[1]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[2]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[3]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[0]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[1]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[2]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[3]  is below min value  in max value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_min_az_rad is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_fov_min_az_rad_above_max)
// {
   // /** \precond
   // set sensors[0].constant.fov_min_az_rad  to get above max value.
   // **/
   // sensors[0].constant.fov_min_az_rad[0] = Max_Sensor_Calibs_fov_min_az_rad + 1.0F;
   // sensors[0].constant.fov_min_az_rad[1] = Max_Sensor_Calibs_fov_min_az_rad + 1.0F;
   // sensors[0].constant.fov_min_az_rad[2] = Max_Sensor_Calibs_fov_min_az_rad + 1.0F;
   // sensors[0].constant.fov_min_az_rad[3] = Max_Sensor_Calibs_fov_min_az_rad + 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[0]  is above max value  in above max value   test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[1]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[2]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[3]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] >= Min_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[0]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] >= Min_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[1]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] >= Min_Sensor_Calibs_fov_min_az_rad,"sensors[0].constant.fov_min_az_rad[2]  is below min value  in  above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] >= Min_Sensor_Calibs_fov_min_az_rad,"sensors[0].constant.fov_min_az_rad[3]  is below min value  in above max value test case ");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_min_az_rad is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_fov_min_az_rad_min)
{
   /** \precond
    set sensors[0].constant.fov_min_az_rad  to get a  min value.
    **/
   sensors[0].constant.fov_min_az_rad[0] = Min_Sensor_Calibs_fov_min_az_rad;
   sensors[0].constant.fov_min_az_rad[1] = Min_Sensor_Calibs_fov_min_az_rad;
   sensors[0].constant.fov_min_az_rad[2] = Min_Sensor_Calibs_fov_min_az_rad;
   sensors[0].constant.fov_min_az_rad[3] = Min_Sensor_Calibs_fov_min_az_rad;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[0]  is above max value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[1]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[2]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] <= Max_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[3]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[0]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[1]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[2]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] >= Min_Sensor_Calibs_fov_min_az_rad, "sensors[0].constant.fov_min_az_rad[3]  is below min value  in min value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_min_az_rad is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_fov_min_az_rad_below_min)
// {
   // /** \precond
   // set sensors[0].constant.fov_min_az_rad  to get a  below min value.
   // **/
   // sensors[0].constant.fov_min_az_rad[0] = Min_Sensor_Calibs_fov_min_az_rad - 1.0F;
   // sensors[0].constant.fov_min_az_rad[1] = Min_Sensor_Calibs_fov_min_az_rad - 1.0F;
   // sensors[0].constant.fov_min_az_rad[2] = Min_Sensor_Calibs_fov_min_az_rad - 1.0F;
   // sensors[0].constant.fov_min_az_rad[3] = Min_Sensor_Calibs_fov_min_az_rad - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[0]  is above max value  in below min value   test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[1]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[2]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] <= Max_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[3]  is above max value  in below min value test case ");
  // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[0] >= Min_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[0]  is below min value  in  below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[1] >= Min_Sensor_Calibs_fov_min_az_rad ,"sensors[0].constant.fov_min_az_rad[1]  is below min value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[2] >= Min_Sensor_Calibs_fov_min_az_rad,"sensors[0].constant.fov_min_az_rad[2]  is below min value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_min_az_rad[3] >= Min_Sensor_Calibs_fov_min_az_rad,"sensors[0].constant.fov_min_az_rad[3]  is below min value  in below min value test case ");
// }
//sensors[0].constant.fov_max_az_rad

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_max_az_rad is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_fov_max_az_rad_nominal)
{
   /** \precond
    set sensors[0].constant.fov_max_az_rad  to get a  nominal value.
    **/

    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[0]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[1]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[2]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[3]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[0]  is below min value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[1]  is below min value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[2]  is below min value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[3]  is below min value  in nominal value   test case ");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_max_az_rad is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_fov_max_az_rad_max)
{
   /** \precond
    set sensors[0].constant.fov_max_az_rad  to get a  max value.
    **/
   sensors[0].constant.fov_max_az_rad[0] = Max_Sensor_Calibs_fov_max_az_rad;
   sensors[0].constant.fov_max_az_rad[1] = Max_Sensor_Calibs_fov_max_az_rad;
   sensors[0].constant.fov_max_az_rad[2] = Max_Sensor_Calibs_fov_max_az_rad;
   sensors[0].constant.fov_max_az_rad[3] = Max_Sensor_Calibs_fov_max_az_rad;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[0]  is above max value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[1]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[2]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[3]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[0]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[1]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[2]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[3]  is below min value  in max value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_max_az_rad is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_fov_max_az_rad_above_max)
// {
   // /** \precond
   // set sensors[0].constant.fov_max_az_rad  to get above max value.
   // **/
   // sensors[0].constant.fov_max_az_rad[0] = Max_Sensor_Calibs_fov_max_az_rad + 1.0F;
   // sensors[0].constant.fov_max_az_rad[1] = Max_Sensor_Calibs_fov_max_az_rad + 1.0F;
   // sensors[0].constant.fov_max_az_rad[2] = Max_Sensor_Calibs_fov_max_az_rad + 1.0F;
   // sensors[0].constant.fov_max_az_rad[3] = Max_Sensor_Calibs_fov_max_az_rad + 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[0]  is above max value  in above max value   test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[1]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[2]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[3]  is above max value  in above max value test case ");
  // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] >= Min_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[0]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] >= Min_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[1]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] >= Min_Sensor_Calibs_fov_max_az_rad,"sensors[0].constant.fov_max_az_rad[2]  is below min value  in  above max value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] >= Min_Sensor_Calibs_fov_max_az_rad,"sensors[0].constant.fov_max_az_rad[3]  is below min value  in above max value test case ");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_max_az_rad is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_fov_max_az_rad_min)
{
   /** \precond
    set sensors[0].constant.fov_max_az_rad  to get a  min value.
    **/
   sensors[0].constant.fov_max_az_rad[0] = Min_Sensor_Calibs_fov_max_az_rad;
   sensors[0].constant.fov_max_az_rad[1] = Min_Sensor_Calibs_fov_max_az_rad;
   sensors[0].constant.fov_max_az_rad[2] = Min_Sensor_Calibs_fov_max_az_rad;
   sensors[0].constant.fov_max_az_rad[3] = Min_Sensor_Calibs_fov_max_az_rad;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[0]  is above max value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[1]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[2]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] <= Max_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[3]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[0]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[1]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[2]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] >= Min_Sensor_Calibs_fov_max_az_rad, "sensors[0].constant.fov_max_az_rad[3]  is below min value  in min value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.fov_max_az_rad is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_fov_max_az_rad_below_min)
// {
   // /** \precond
   // set sensors[0].constant.fov_max_az_rad  to get a below  min value.
   // **/
   // sensors[0].constant.fov_max_az_rad[0] = Min_Sensor_Calibs_fov_max_az_rad - 1.0F;
   // sensors[0].constant.fov_max_az_rad[1] = Min_Sensor_Calibs_fov_max_az_rad - 1.0F;
   // sensors[0].constant.fov_max_az_rad[2] = Min_Sensor_Calibs_fov_max_az_rad - 1.0F;
   // sensors[0].constant.fov_max_az_rad[3] = Min_Sensor_Calibs_fov_max_az_rad - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[0]  is above max value  in below min value   test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[1]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[2]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] <= Max_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[3]  is above max value  in below min value test case ");
  // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[0] >= Min_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[0]  is below min value  in  below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[1] >= Min_Sensor_Calibs_fov_max_az_rad ,"sensors[0].constant.fov_max_az_rad[1]  is below min value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[2] >= Min_Sensor_Calibs_fov_max_az_rad,"sensors[0].constant.fov_max_az_rad[2]  is below min value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.fov_max_az_rad[3] >= Min_Sensor_Calibs_fov_max_az_rad,"sensors[0].constant.fov_max_az_rad[3]  is below min value  in below min value test case ");
// }

//sensors[0].constant.min_aliaised_range_rate

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.min_aliaised_range_rate is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_min_aliaised_range_rate_nominal)
{
   /** \precond
    set sensors[0].constant.min_aliaised_range_rate  to get a nominal value from normal input.
    **/
   sensors[0].constant.min_aliaised_range_rate[0] = -45.0F;
   sensors[0].constant.min_aliaised_range_rate[1] = -45.0F;
   sensors[0].constant.min_aliaised_range_rate[2] = -45.0F;
   sensors[0].constant.min_aliaised_range_rate[3] = -45.0F;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[0]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[1]  is above max value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[2]  is above max value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[3]  is above max value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[0]  is below min value  in  nominal value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[1]  is below min value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[2]  is below min value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[3]  is below min value  in nominal value test case ");
}

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.min_aliaised_range_rate is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_min_aliaised_range_rate_max)
{
   /** \precond
    set sensors[0].constant.min_aliaised_range_rate  to get a max value.
    **/
   sensors[0].constant.min_aliaised_range_rate[0] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   sensors[0].constant.min_aliaised_range_rate[1] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   sensors[0].constant.min_aliaised_range_rate[2] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   sensors[0].constant.min_aliaised_range_rate[3] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[0]  is above max value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[1]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[2]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[3]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[0]  is below min value  in  max value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[1]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[2]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[3]  is below min value  in max value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.min_aliaised_range_rate is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_min_aliaised_range_rate_above_max)
// {
   // /** \precond
   // set sensors[0].constant.min_aliaised_range_rate  to get a above max value.
   // **/
   // sensors[0].constant.min_aliaised_range_rate[0] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S + 1.0F;
   // sensors[0].constant.min_aliaised_range_rate[1] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S + 1.0F;
   // sensors[0].constant.min_aliaised_range_rate[2] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S + 1.0F;
   // sensors[0].constant.min_aliaised_range_rate[3] = Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S + 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[0]  is above max value  in above max value   test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[1]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[2]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[3]  is above max value  in above max value test case ");
  // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[0]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[1]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S,"sensors[0].constant.min_aliaised_range_rate[2]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S,"sensors[0].constant.min_aliaised_range_rate[3]  is below min value  in above max value test case ");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.min_aliaised_range_rate is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_min_aliaised_range_rate_min)
{
   /** \precond
    set sensors[0].constant.min_aliaised_range_rate  to get a  min value.
    **/
   sensors[0].constant.min_aliaised_range_rate[0] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   sensors[0].constant.min_aliaised_range_rate[1] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   sensors[0].constant.min_aliaised_range_rate[2] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   sensors[0].constant.min_aliaised_range_rate[3] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[0]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[1]  is above max value  in min value  test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[2]  is above max value  in min value  test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[3]  is above max value  in min value  test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[0]  is below min value  in  min value  test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[1]  is below min value  in min value  test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[2]  is below min value  in min value  test case ");
   CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S, "sensors[0].constant.min_aliaised_range_rate[3]  is below min value  in min value  test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.min_aliaised_range_rate is below min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_min_aliaised_range_rate_below_min)
// {
   // /** \precond
   // set sensors[0].constant.min_aliaised_range_rate  to get a  min value.
   // **/
   // sensors[0].constant.min_aliaised_range_rate[0] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S - 1.0F;
   // sensors[0].constant.min_aliaised_range_rate[1] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S - 1.0F;
   // sensors[0].constant.min_aliaised_range_rate[2] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S - 1.0F;
   // sensors[0].constant.min_aliaised_range_rate[3] = Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[0]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[1]  is above max value  in below min value  test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[2]  is above max value  in below min value  test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] <= Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[3]  is above max value  in below min value  test case ");
  // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[0] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[0]  is below min value  in  below min value  test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[1] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S ,"sensors[0].constant.min_aliaised_range_rate[1]  is below min value  in below min value  test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[2] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S,"sensors[0].constant.min_aliaised_range_rate[2]  is below min value  in below min value  test case ");
 // CHECK_TEXT(sensors[0].constant.min_aliaised_range_rate[3] >= Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S,"sensors[0].constant.min_aliaised_range_rate[3]  is below min value  in below min value  test case ");
// }

//sensors[0].constant.v_wrapping

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.v_wrapping is nominal value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_rng_rate_interval_widths_nominal)
{
   /** \precond
    set sensors[0].constant.v_wrapping  to get a nominal value from normal input.
    **/
   sensors[0].constant.v_wrapping[0] = 69.8359375F;
   sensors[0].constant.v_wrapping[1] = 59.6132813F;
   sensors[0].constant.v_wrapping[2] = 69.8359375F;
   sensors[0].constant.v_wrapping[3] = 59.6132813F;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.v_wrapping[0] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[0]  is above max value  in nominal value   test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[1] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[1]  is above max value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[2] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is above max value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[3] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[3]  is above max value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[0] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[0]  is below min value  in  nominal value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[1] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[1]  is below min value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[2] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is below min value  in nominal value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[3] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[3]  is below min value  in nominal value test case ");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.v_wrapping is max value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_rng_rate_interval_widths_max)
{
   /** \precond
    set sensors[0].constant.v_wrapping  to get a max value.
    **/
   sensors[0].constant.v_wrapping[0] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   sensors[0].constant.v_wrapping[1] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   sensors[0].constant.v_wrapping[2] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   sensors[0].constant.v_wrapping[3] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.v_wrapping[0] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[0]  is above max value  in max value   test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[1] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[1]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[2] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[3] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[3]  is above max value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[0] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[0]  is below min value  in  max value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[1] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[1]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[2] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is below min value  in max value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[3] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[3]  is below min value  in max value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.v_wrapping is above max value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_rng_rate_interval_widths_above_max)
// {
   // /** \precond
   // set sensors[0].constant.v_wrapping  to get above max value.
   // **/
   // sensors[0].constant.v_wrapping[0] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S + 1.0F;
   // sensors[0].constant.v_wrapping[1] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S + 1.0F;
   // sensors[0].constant.v_wrapping[2] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S + 1.0F;
   // sensors[0].constant.v_wrapping[3] = Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S + 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.v_wrapping[0] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[0]  is above max value  in above max value   test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[1] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[1]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[2] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[2]  is above max value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[3] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[3]  is above max value  in above max value test case ");
  // CHECK_TEXT(sensors[0].constant.v_wrapping[0] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[0]  is below min value  in  above max value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[1] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[1]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[2] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S,"sensors[0].constant.v_wrapping[2]  is below min value  in above max value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[3] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S,"sensors[0].constant.v_wrapping[3]  is below min value  in above max value test case ");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.v_wrapping is min value.
*\req
*CPR-4779 ,CPR-4304 
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_rng_rate_interval_widths_min)
{
   /** \precond
    set sensors[0].constant.v_wrapping  to get a min value.
    **/
   sensors[0].constant.v_wrapping[0] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   sensors[0].constant.v_wrapping[1] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   sensors[0].constant.v_wrapping[2] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   sensors[0].constant.v_wrapping[3] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output is within allowed range
    **/

   CHECK_TEXT(sensors[0].constant.v_wrapping[0] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[0]  is above max value  in min value   test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[1] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[1]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[2] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[3] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[3]  is above max value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[0] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[0]  is below min value  in  min value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[1] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[1]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[2] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is below min value  in min value test case ");
   CHECK_TEXT(sensors[0].constant.v_wrapping[3] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S, "sensors[0].constant.v_wrapping[2]  is below min value  in  min value test case ");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].constant.v_wrapping is below  min value.
// *\req
// *CPR-4779 ,CPR-4304 
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_rng_rate_interval_widths_below_min)
// {
   // /** \precond
   // set sensors[0].constant.v_wrapping  to get a below min value.
   // **/
   // sensors[0].constant.v_wrapping[0] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S - 1.0F;
   // sensors[0].constant.v_wrapping[1] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S - 1.0F;
   // sensors[0].constant.v_wrapping[2] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S - 1.0F;
   // sensors[0].constant.v_wrapping[3] = Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S - 1.0F;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // *- check that the output is within allowed range
   // **/

 // CHECK_TEXT(sensors[0].constant.v_wrapping[0] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[0]  is above max value  in below min value   test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[1] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[1]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[2] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[2]  is above max value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[3] <= Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[3]  is above max value  in below min value test case ");
  // CHECK_TEXT(sensors[0].constant.v_wrapping[0] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[0]  is below min value  in  below min value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[1] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S ,"sensors[0].constant.v_wrapping[1]  is below min value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[2] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S,"sensors[0].constant.v_wrapping[2]  is below min value  in below min value test case ");
 // CHECK_TEXT(sensors[0].constant.v_wrapping[3] >= Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S,"sensors[0].constant.v_wrapping[3]  is below min value  in below min value test case ");
// }
/**
**********************************************************************************************************
*                                F360_Radar_Sensor_T                                                     *
**********************************************************************************************************
**/

//sensors[0].variable.timestamp_us

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.timestamp_us is a nominal value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4309
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_timestamp_us_nominal)
{
   /** \precond
    set sensors[0].variable.timestamp_us to get a nominal value
    **/
    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   const int64_t timestamp_difference = (static_cast<int64_t>(core_info.time_us) - static_cast<int64_t>(sensors[0].variable.timestamp_us));
   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(timestamp_difference <= max_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is above max_allowed_sensor_vs_tracker_timestamp_diff_us in a nominal value test case");
   CHECK_TEXT(timestamp_difference >= min_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is below min_allowed_sensor_vs_tracker_timestamp_diff_us in a nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.timestamp_us is a max value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4309
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_timestamp_us_max)
{
   /** \precond
    set sensors[0].variable.timestamp_us to get a max value
    **/
   sensors[0].variable.timestamp_us = 980104000LL;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   const int64_t timestamp_difference = (static_cast<int64_t>(core_info.time_us) - static_cast<int64_t>(sensors[0].variable.timestamp_us));
   /** \result
    check that the output is within allowed range
    **/
   CHECK_TEXT(timestamp_difference <= max_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is above max_allowed_sensor_vs_tracker_timestamp_diff_us in a max value test case");
   CHECK_TEXT(timestamp_difference >= min_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is below min_allowed_sensor_vs_tracker_timestamp_diff_us in a max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.timestamp_us is above max value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4309
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_timestamp_us_above_max)
// {
   // /** \precond
   // set sensors[0].variable.timestamp_us to get above max value
   // **/
   // sensors[0].variable.timestamp_us= 980104000LL-1U;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // const int64_t timestamp_difference = (static_cast<int64_t>(core_info.time_us) - static_cast<int64_t>(sensors[0].variable.timestamp_us));
   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(timestamp_difference <= max_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is above max_allowed_sensor_vs_tracker_timestamp_diff_us in above max value test case");
   // CHECK_TEXT(timestamp_difference >= min_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is below min_allowed_sensor_vs_tracker_timestamp_diff_us in above max value test case" );
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.timestamp_us is a min value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4309
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_timestamp_us_min)
{
   /** \precond
    set sensors[0].variable.timestamp_us to get a min value
    **/
   sensors[0].variable.timestamp_us = core_info.time_us;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   const int64_t timestamp_difference = (static_cast<int64_t>(core_info.time_us) - static_cast<int64_t>(sensors[0].variable.timestamp_us));
   /** \result
    check that the output is within allowed range
    **/
   CHECK_TEXT(timestamp_difference <= max_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is above max_allowed_sensor_vs_tracker_timestamp_diff_us in a min value test case");
   CHECK_TEXT(timestamp_difference >= min_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is below min_allowed_sensor_vs_tracker_timestamp_diff_us in a min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.timestamp_us is below min value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4309
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_timestamp_us_below_min)
// {
   // /** \precond
   // set sensors[0].variable.timestamp_us to get a below min value
   // **/
   // sensors[0].variable.timestamp_us= core_info.time_us +1;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // const int64_t timestamp_difference = (static_cast<int64_t>(core_info.time_us) - static_cast<int64_t>(sensors[0].variable.timestamp_us));
   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(timestamp_difference <= max_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is above max_allowed_sensor_vs_tracker_timestamp_diff_us in below min value test case");
   // CHECK_TEXT(timestamp_difference >= min_allowed_sensor_vs_tracker_timestamp_diff_us, "timestamp_difference between core_info.time_us and sensors[0].variable.timestamp_us a  is below min_allowed_sensor_vs_tracker_timestamp_diff_us in below min value test case" );
// }

//sensors[0].variable.number_of_valid_detections
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.number_of_valid_detections is a nominal value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4310
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_number_of_valid_detections_nominal)
{
   /** \precond
    set sensors[0].variable.number_of_valid_detections to get a nominal value
    **/
    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.number_of_valid_detections <= Max_Sensors_Number_Of_Valid_Detections, " sensors[0].variable.number_of_valid_detections a  is above Max_Sensors_Number_Of_Valid_Detections in a nominal value test case");
   
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.number_of_valid_detections is a max value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4310
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_number_of_valid_detections_max)
{
   /** \precond
    set sensors[0].variable.number_of_valid_detections to get a max value
    **/
   sensors[0].variable.number_of_valid_detections = Max_Sensors_Number_Of_Valid_Detections;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.number_of_valid_detections <= Max_Sensors_Number_Of_Valid_Detections, " sensors[0].variable.number_of_valid_detections a  is above Max_Sensors_Number_Of_Valid_Detections in a max value test case");
   
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.number_of_valid_detections is above max value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4310
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_number_of_valid_detections_above_max)
// {
   // /** \precond
   // set sensors[0].variable.number_of_valid_detections to get above max value
   // **/
   // sensors[0].variable.number_of_valid_detections =Max_Sensors_Number_Of_Valid_Detections +1U;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(sensors[0].variable.number_of_valid_detections <= Max_Sensors_Number_Of_Valid_Detections, " sensors[0].variable.number_of_valid_detections a  is above Max_Sensors_Number_Of_Valid_Detections in above max value test case");
   // CHECK_TEXT(sensors[0].variable.number_of_valid_detections >= Min_Sensors_Number_Of_Valid_Detections, " sensors[0].variable.number_of_valid_detections a  is below Min_Sensors_Number_Of_Valid_Detections in above max value test case");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.number_of_valid_detections is a min value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4310
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_number_of_valid_detections_min)
{
   /** \precond
    set sensors[0].variable.number_of_valid_detections to get a min value
    **/
   sensors[0].variable.number_of_valid_detections = Min_Sensors_Number_Of_Valid_Detections;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.number_of_valid_detections <= Max_Sensors_Number_Of_Valid_Detections, " sensors[0].variable.number_of_valid_detections a  is above Max_Sensors_Number_Of_Valid_Detections in a min value test case");
   
}

// cant test below min value of sensors[0].variable.number_of_valid_detections because min =0 and its unsigned int

//sensors[0].variable.look_id

/**
 *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.look_id is valid value.
 *\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4312
 **/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_look_id_value_valid)
{
   /** \precond
    set sensors[0].variable.look_id to get valid value
    **/
   sensors[0].variable.look_id = F360_DET_LOOK_ID_1;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.look_id <= Max_Sensors_Look_ID, " sensors[0].variable.look_id a  is above Max_Sensors_Look_ID in nominal value test case");
   CHECK_TEXT(sensors[0].variable.look_id >= Min_Sensors_Look_ID, " sensors[0].variable.look_id a  is below Min_Sensors_Look_ID in  nominal value test case");
}
// /**
 // *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.look_id is wrong value.
 // *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4312
 // **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_look_id_value_wrong)
// {
   // /** \precond
   // set sensors[0].variable.look_id to get wrong value
   // **/
   // sensors[0].variable.look_id = 5(F360_Det_Look_ID_T);
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(sensors[0].variable.look_id <= Max_Sensors_Look_ID, " sensors[0].variable.look_id a  is above Max_Sensors_Look_ID in nominal value test case");
   // CHECK_TEXT(sensors[0].variable.look_id >= Min_Sensors_Look_ID, " sensors[0].variable.look_id a  is below Min_Sensors_Look_ID in  nominal value test case");
// }

//sensors[0].variable.vcs_velocity.longitudinal

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.longitudinal is nominal value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442 ,CPR-4319, CPR-4320
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_vcs_velocity_longitudinal_nominal)
{
   /** \precond
    set sensors[0].variable.vcs_velocity.longitudinal to get nominal value
    **/
    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal <= Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is above Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S in nominal value test case");
   CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal >= Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is below Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S in  nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.longitudinal is max value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4320
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_vcs_velocity_longitudinal_max)
{
   /** \precond
    set sensors[0].variable.vcs_velocity.longitudinal to get max value
    **/
   sensors[0].variable.vcs_velocity.longitudinal = Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal <= Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is above Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S in max value test case");
   CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal >= Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is below Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S in  max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.longitudinal is above max value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4320
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_vcs_velocity_longitudinal_above_max)
// {
   // /** \precond
   // set sensors[0].variable.vcs_velocity.longitudinal to get above max value
   // **/
   // sensors[0].variable.vcs_velocity.longitudinal=  Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S + 1.0F  ;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal <= Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S ," sensors[0].variable.vcs_velocity.longitudinal a  is above Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S in above max value test case");
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal >= Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is below Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S in above max value test case");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.longitudinal is min value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4320
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_vcs_velocity_longitudinal_min)
{
   /** \precond
    set sensors[0].variable.vcs_velocity.longitudinal to get min value
    **/
   sensors[0].variable.vcs_velocity.longitudinal = Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal <= Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is above Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S in min value test case");
   CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal >= Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is below Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S in  min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.longitudinal is below min value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4320
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_vcs_velocity_longitudinal_below_min)
// {
   // /** \precond
   // set sensors[0].variable.vcs_velocity.longitudinal to get below min value
   // **/
   // sensors[0].variable.vcs_velocity.longitudinal=  Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S - 1.0F ;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal <= Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinala  is above Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S in below min value test case");
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.longitudinal >= Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S, " sensors[0].variable.vcs_velocity.longitudinal a  is below Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S in  below min value test case");
// }
//sensors[0].variable.vcs_velocity.lateral

/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.lateral is nominal value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4321
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_vcs_velocity_lateral_nominal)
{
   /** \precond
    set sensors[0].variable.vcs_velocity.lateral to get nominal value
    **/
    /** \action
     * call TrackClassification module function
     **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral <= Max_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].lateral a  is above Max_Sensors_Vcs_Velocity_lateral_M_Per_S in nominal value test case");
   CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral >= Min_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].lateral a  is below Min_Sensors_Vcs_Velocity_lateral_M_Per_S in  nominal value test case");
}
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.lateral is max value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4321
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_vcs_velocity_lateral_max)
{
   /** \precond
    set sensors[0].variable.vcs_velocity.lateral to get max value
    **/
   sensors[0].variable.vcs_velocity.lateral = Max_Sensors_Vcs_Velocity_lateral_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral <= Max_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is above Max_Sensors_Vcs_Velocity_lateral_M_Per_S in max value test case");
   CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral >= Min_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is below Min_Sensors_Vcs_Velocity_lateral_M_Per_S in  max value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.lateral is above max value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4321
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_vcs_velocity_lateral_above_max)
// {
   // /** \precond
   // set sensors[0].variable.vcs_velocity.lateral to get above max value
   // **/
   // sensors[0].variable.vcs_velocity.lateral=  Max_Sensors_Vcs_Velocity_lateral_M_Per_S + 1.0F  ;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral <= Max_Sensors_Vcs_Velocity_lateral_M_Per_S ," sensors[0].variable.vcs_velocity.lateral a  is above Max_Sensors_Vcs_Velocity_lateral_M_Per_S in above max value test case");
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral >= Min_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is below Min_Sensors_Vcs_Velocity_lateral_M_Per_S in above max value test case");
// }
/**
*\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.lateral is min value.
*\req
*CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4321
**/
TEST(f360_tracker_trackclassification_sanity_checks, TrackClassification_check_verification_of_sensors_vcs_velocity_lateral_min)
{
   /** \precond
    set sensors[0].variable.vcs_velocity.lateral to get min value
    **/
   sensors[0].variable.vcs_velocity.lateral = Min_Sensors_Vcs_Velocity_lateral_M_Per_S;
   /** \action
    * call TrackClassification module function
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
    check that the output is within allowed range
   **/
   CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral <= Max_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is above Max_Sensors_Vcs_Velocity_lateral_M_Per_S in min value test case");
   CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral >= Min_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is below Min_Sensors_Vcs_Velocity_lateral_M_Per_S in  min value test case");
}
// /**
// *\purpose  Purpose of this test is to verify whether input value is verified when sensors[0].variable.vcs_velocity.lateral is below min value.
// *\req
// *CPR-4779 ,CPR-4308 , CPR-4442,CPR-4319, CPR-4321
// **/
// TEST(f360_tracker_trackclassification_sanity_checks,TrackClassification_check_verification_of_sensors_vcs_velocity_lateral_below_min)
// {
   // /** \precond
   // set sensors[0].variable.vcs_velocity.lateral to get below min value
   // **/
   // sensors[0].variable.vcs_velocity.lateral=  Min_Sensors_Vcs_Velocity_lateral_M_Per_S - 1.0F ;
   // /** \action
   // * call TrackClassification module function
   // **/
    // F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   // /** \result
   // check that the output is within allowed range
   // **/
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral <= Max_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is above Max_Sensors_Vcs_Velocity_lateral_M_Per_S in below min value test case");
   // CHECK_TEXT(sensors[0].variable.vcs_velocity.lateral >= Min_Sensors_Vcs_Velocity_lateral_M_Per_S, " sensors[0].variable.vcs_velocity.lateral a  is below Min_Sensors_Vcs_Velocity_lateral_M_Per_S in  below min value test case");
// }
/** @}*/
