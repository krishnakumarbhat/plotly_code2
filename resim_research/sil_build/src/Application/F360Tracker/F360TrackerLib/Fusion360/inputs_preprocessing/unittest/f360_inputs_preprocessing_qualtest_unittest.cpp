/** \file
This file contains sw qualifications tests for f360_internal_preprocessing
*/
#include "f360_inputs_preprocessing.h"
#include "f360_clusters_preprocessing.h"
#include "f360_terminate_clusters.h"
#include "f360_clear_cluster.h"
#include "f360_kill_cluster.h"
#include "f360_sorted_clusters_mgmt.h"
#include "f360_set_variant.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <f360_math.h>

using namespace f360_variant_A;


/** \brief
*  Add brief description of test group
**/
TEST_GROUP(inputs_preprocessing_qualtest)
{
   F360_Host_T host{};
   F360_Host_Props_T host_props{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS]{};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Core_Info_T core_info{};
   F360_TRKR_TIMING_INFO_T timing_info{};

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      raw_detection_list.number_of_valid_detections = 1;
      raw_detection_list.detections[0].raw.sensor_id = 1;
   }
};

/**
*\purpose  Test input interface requirements.
*\req   CPR-4690, CPR-2792
*/
TEST(inputs_preprocessing_qualtest, Internal_Preprocessing_Input_Interface_Requirements)
{
   /** \precond
   Initialize structures of the correct type as specified in requirements.
   **/


   /** \action
   * Call function to test that inputs, as specified in requirements, are accepted.
   **/
    Inputs_Preprocessing(core_info, host, host_props,timing_info);

   /** \result
   * Test should compile and run. No functionality is supposed to be tested in this test.
   **/
}
