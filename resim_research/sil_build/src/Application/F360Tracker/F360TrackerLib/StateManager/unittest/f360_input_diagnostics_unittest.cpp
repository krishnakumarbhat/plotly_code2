/** \file
   This unit-test file contains UTs for F360 input diagnostics module
*/

#include "f360_input_diagnostics.h"
#include "f360_constants.h"


#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cstring>

using namespace f360_variant_A;

inline uint64_t ms2us(uint64_t in_ms)
{
   return (in_ms * 1000ULL);
}

/** \defgroup  f360_input_diagnostics
 *  @{
 */

 /** \brief
  *  Set up initial values of the signals to be checked by the module.
  *  Keep in mind that class Execute methods needs to be run at least
  *  twice for the module to start detecting errors. In the first cycle
  *  no information about the previous cycle is known, so signals can't be
  *  verified for consistency
  */

TEST_GROUP(f360_input_diagnostics)
{
   /* Define common data for all tests */
   F360_Core_Info_T core_info = {};
   F360_Host_T host = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   Input_Diagnostics input_diagnostics;

   TEST_SETUP()
   {
      /* core_info setup */
      core_info.time_us = ms2us(1100ULL);
      core_info.cnt_loops = 1000U;
      core_info.elapsed_time_s = 0.05F;

      /* host setup */
      host.vehicle_index = 1000U;
      host.speed_qf      = F360_QF_ACCURATE;
      host.yaw_rate_qf   = F360_QF_ACCURATE;
      host.long_accel_qf = F360_QF_ACCURATE;
      host.lat_accel_qf  = F360_QF_ACCURATE;
      host.curvature_rear = 0.0F;
      host.vcs_sideslip   = 0.0F;

      /* raw detection setup */
      raw_detect_list.number_of_valid_detections = 5;
      raw_detect_list.detections[0].raw.range = 0.1F;
      raw_detect_list.detections[1].raw.range_rate = 0.2F;
      raw_detect_list.detections[2].raw.azimuth = 0.3F;
      raw_detect_list.detections[3].raw.elevation = 0.4F;
      raw_detect_list.detections[f360_variant_A::MAX_NUMBER_OF_DETECTIONS-1].raw.range = 0.5F;

      /* sensors setup */
      uint64_t sensor_vs_tracker_timestamp_diff_us = ms2us(50ULL);

      for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
      {
         sensors[idx].variable.timestamp_us = core_info.time_us - sensor_vs_tracker_timestamp_diff_us;
         sensors[idx].variable.look_index = 160U;
      }

      /* sensors_calibs setup */
      for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
      {
         sensors[idx].variable.is_valid = true;
         sensors[idx].constant.polarity = 1;
         sensors[idx].constant.mounting_position.vcs_position.lateral = 0.0F;
         sensors[idx].constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      }
   }

   // Checks if a Core_Info_Faults_T structs contain any faults
   bool Contain_Faults(const Core_Info_Faults_T& core_info)
   {
      bool fault = (core_info.cnt_loops_no_increase          ||
                            core_info.time_us_no_increase            ||
                            core_info.elapsed_time_below_lower_limit ||
                            core_info.elapsed_time_above_upper_limit);
      return fault;
   }

   // Checks if a Host_Info_Faults_T structs contain any faults
   bool Contain_Faults(const Host_Info_Faults_T& host_info)
   {
      bool fault = (host_info.vehicle_index_no_increase           ||
                            host_info.host_speed_invalid                     ||
                            host_info.host_yawrate_invalid                   ||
                            host_info.host_longitudinal_acceleration_invalid ||
                            host_info.host_lateral_acceleration_invalid);
      return fault;
   }

   // Checks if a Raw_Detection_Faults_T structs contain any faults
   bool Contain_Faults(const Raw_Detection_Faults_T& raw_detection)
   {
      bool fault = (raw_detection.range_is_invalid                ||
                            raw_detection.range_rate_is_invalid   ||
                            raw_detection.azimuth_is_invalid      ||
                            raw_detection.elevation_is_invalid);
      return fault;
   }

   // Checks if a Radar_Sensor_Faults_T structs contain any faults
   bool Sensor_Fault_Present(const Radar_Sensor_Faults_T& sensor)
   {
       bool fault = (sensor.look_index_no_increase ||
                             sensor.sensor_vs_tracker_timestamp_divergence);
      return fault;
   }

   // Checks if Radar_Sensor_Faults_T array contain any faults
   bool Sensor_Fault_Present_In_Any_Sensor(const Radar_Sensor_Faults_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      bool fault = false;
      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; ++i)
      {
         fault = (fault || Sensor_Fault_Present(sensors[i]));
      }
      return fault;
   }

   // Checks if a Radar_Sensor_Calib_Faults_T structs contain any faults
   bool Contain_Faults(const Radar_Sensor_Calib_Faults_T& sensor_calib)
   {
      bool fault = (sensor_calib.mounting_pos_is_invalid ||
                            sensor_calib.polarity_is_invalid ||
                            sensor_calib.boresight_angle_is_invalid);
      return fault;
   }

   // Checks if a Input_Faults_T structs contain any faults
   bool Contain_Faults(const Input_Faults_T& input_faults)
   {
      bool fault = (Contain_Faults(input_faults.core_info) || Contain_Faults(input_faults.host_info) || Contain_Faults(input_faults.raw_detection));
      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; ++i)
      {
         fault = (fault || Contain_Faults(input_faults.sensors_calibs[i]) || Sensor_Fault_Present(input_faults.sensors[i]));
      }
      return fault;
   }
};

/** \purpose
 * First run of Execute function should not return any faults as previous signal states are
 * not known and conditions check can't be executed
 * \req
 * NA
 */
TEST(f360_input_diagnostics, No_Faults_Should_Be_Returned_With_First_Cycle)
{
   /** \precond
    * None
    */

    /** \action
     * Execute InputDiagnostics for the first time
     */
   Input_Faults_T actual_faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \result
    * There should be no reported faults
    */
   CHECK_FALSE(Contain_Faults(actual_faults));
}

/** \purpose
 * Execute function after Initialize should not return any faults as previous signal states
 * are not known and conditions check can't be executed
 * \req
 * NA
 */
TEST(f360_input_diagnostics, No_Faults_Should_Be_Returned_With_First_Execution_After_Initialize)
{
   /** \precond
    * Execute input diagnostics class for the first time to fill previous signal states
    */
   input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \action
    * Initialize and Execute Input_Diagnostics
    */
   input_diagnostics.Initialize();
   Input_Faults_T actual_faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \result
    * There should be no reported faults
    */
   CHECK_FALSE(Contain_Faults(actual_faults));
}

/** \purpose
 * Execute function should return no faults when all the signals
 * have been incremented correctly
 * \req
 * NA
 */
TEST(f360_input_diagnostics, No_Faults_Should_Be_Returned_When_Signals_Imcremented_Correct)
{
   /** \precond
    * Execute input diagnostics class for the first time to fill previous signal states
    * New signals values should be incremented compared to the previous ones
    */
   input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);
   core_info.cnt_loops++;
   core_info.time_us += ms2us(50000ULL);
   host.vehicle_index++;
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; ++i)
   {
      sensors[i].variable.look_index++;
      sensors[i].variable.timestamp_us += ms2us(50000ULL);
   }

   /** \action
    * Execute signal check
    */
   Input_Faults_T actual_faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);


   /** \result
    * There should be no reported faults
    */
   CHECK_FALSE(Contain_Faults(actual_faults));
}

/** \purpose
 * Execute function should return true when input raw detection has invalid input
 * \req
 * NA
 */
TEST(f360_input_diagnostics, Raw_Detection_Faults_Should_Be_Returned)
{
   /** \precond
    * Execute input diagnostics class for the first time to fill previous signal states
    * New signals values should be incremented compared to the previous ones
    */
   input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);
   raw_detect_list.detections[1].raw.range_rate = 71.0F;
   raw_detect_list.detections[2].raw.azimuth = F360_DEG2RAD(90.1F);
   raw_detect_list.detections[3].raw.elevation = F360_DEG2RAD(45.1F);
   raw_detect_list.detections[f360_variant_A::MAX_NUMBER_OF_DETECTIONS-1].raw.range = -2.1F;

   /** \action
    * Execute signal check
    */
   Input_Faults_T actual_faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \result
    * There should be raw detection faults reported
    */
   CHECK_TRUE(actual_faults.raw_detection.range_is_invalid);
   CHECK_TRUE(actual_faults.raw_detection.range_rate_is_invalid);
   CHECK_TRUE(actual_faults.raw_detection.azimuth_is_invalid);
   CHECK_TRUE(actual_faults.raw_detection.elevation_is_invalid);
}

/** \purpose
 * Look index fault is not set during normal rollover
 * \req
 * NA
 */
TEST(f360_input_diagnostics, Look_Index_Fault_Should_Not_Be_Set_During_Normal_Rollover)
{
   /** \precond
    * Execute input diagnostics class for the first time to fill previous signal states
    */
   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 65534U;
   }

   Input_Faults_T faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

   /** \action
    * Increment look index, execute input diagnostic and expect no fault
    */
   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 65535U;
   }

   faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 0U;
   }

   faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \result
    * There should be no sensor fault reported
    */
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

}

/** \purpose
 * Look index fault is not set when previous look index is 65533 (min_allowed_rollover_prev_look_index + 1)
 * and current look index is 0
 * \req
 * NA
 */
TEST(f360_input_diagnostics, Look_Index_Fault_Not_Set_During_Rollover_From_Min_Allowed_Rollover_Index_To_Zero)
{
   /** \precond
    * Execute input diagnostics class for the first time to fill previous signal states
    */
   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 65533U;
   }

   Input_Faults_T faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

   /** \action
    * Set look index to 0, execute input diagnostic and expect no fault
    */
   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 0U;
   }

   faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \result
    * There should be no sensor fault reported
    */
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

}

/** \purpose
 * Look index fault is not set when previous look index is 65535 (uint16 max)
 * and current look index is 2 (max_allowed_rollover_look_index - 1)
 * \req
 * NA
 */
TEST(f360_input_diagnostics, Look_Index_Fault_Not_Set_During_Rollover_From_UINT16MAX_To_Max_Allowed_Rollover_Index)
{
   /** \precond
    * Execute input diagnostics class for the first time to fill previous signal states
    */
   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 65535U;
   }

   Input_Faults_T faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

   /** \action
    * Set look index to 2, execute input diagnostic and expect no fault
    */
   for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.look_index = 2U;
   }

   faults = input_diagnostics.Execute(core_info, host, raw_detect_list, sensors);

   /** \result
    * There should be no sensor fault reported
    */
   CHECK_FALSE(Sensor_Fault_Present_In_Any_Sensor(faults.sensors));

}

/** @}*/
