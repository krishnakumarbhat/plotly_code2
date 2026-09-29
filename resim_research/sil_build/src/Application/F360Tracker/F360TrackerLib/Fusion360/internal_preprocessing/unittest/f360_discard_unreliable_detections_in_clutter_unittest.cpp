/** \file
 * This file contains unit tests for content of f360_discard_unreliable_detections_in_clutter.cpp file
 */

#include "f360_discard_unreliable_detections_in_clutter.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup Discard_Unreliable_Detections_In_Clutter
 *  @{ 
 */

/** \brief
 * Test functionality of Discard_Unreliable_Detections_In_Clutter. Focus on verifying that detections are properly flagged as unreliable
 * and discarded based on their properties when low power clutter logic is enabled.
 */
TEST_GROUP(Discard_Unreliable_Detections_In_Clutter)
{
   F360_Tracker_Info_T tracker_info{};
   F360_Radar_Sensor_T sensor{};
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_Detection_Props_T det_prop{};
   TEST_SETUP()
   {
      // Set all variables so that the detection will be flagged as unreliable in clutter but not discarded by default
      tracker_info.f_low_power_clutter = true; // enable clutter logic branch
      sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_RADAR;
      det_prop.f_ok_to_use = true;
      det_prop.vcs_position.x = 20.1F;
      det_prop.vcs_position.y = 0.0F;

      det_raw.raw.range = 20.1F;
      det_raw.raw.rcs = -10.1F;
      det_raw.raw.snr = 12.9F;
      det_raw.raw.confid_azimuth = 0;
      det_raw.raw.elevation = F360_DEG2RAD(10.1F);
   }
};

/** \purpose
 * Detection should NOT be flagged unreliable or discarded when the clutter logic branch is disabled.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_Tracker_Signal_Not_Set)
{
   /** \precond
    * Disable clutter logic branch.
    */
   tracker_info.f_low_power_clutter = false; // disable clutter logic branch

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection matching clutter hypothesis should be flagged unreliable but NOT discarded if confidence and SNR are sufficient.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Flag_Set)
{
   /** \precond
    * Create single detection inside ROI with properties matching hypothesis but NOT discard criteria.
    */


   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable but NOT discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection matching clutter hypothesis with high azimuth confidence should be flagged unreliable and discarded.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Flag_Set_And_Discarded_Confid_Az)
{
   /** \precond
    * Create single detection inside ROI with high azimuth confidence.
    */
   det_raw.raw.confid_azimuth = 3;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable and discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_FALSE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection matching clutter hypothesis with low SNR should be flagged unreliable and discarded.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Flag_Set_And_Discarded_Snr)
{
   /** \precond
    * Create single detection inside ROI with low SNR.
    */
   det_raw.raw.snr = 9.9F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable and discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_FALSE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection matching clutter hypothesis should be flagged unreliable but NOT discarded if range is very close.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Flag_Set_And_Not_Discarded_Range_Low)
{
   /** \precond
    * Create single detection inside ROI with low SNR, high azimuth confidence, but very close range.
    */
   det_raw.raw.snr = 9.9F;
   det_raw.raw.confid_azimuth = 3;
   det_raw.raw.range = 7.9F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable but NOT discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection outside ROI (X too low) should NOT be flagged unreliable.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Not_In_ROI_X_Low)
{
   /** \precond
    * Create single detection outside ROI (X too low).
    */
   det_prop.vcs_position.x = - 0.1F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection outside ROI (X too high) should NOT be flagged unreliable.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Not_In_ROI_X_High)
{
   /** \precond
    * Create single detection outside ROI (X too high).
    */
   det_prop.vcs_position.x = 75.1F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection outside ROI (Y too high) should NOT be flagged unreliable.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Not_In_ROI_Y)
{
   /** \precond
    * Create single detection outside ROI (Y too high).
    */
   det_prop.vcs_position.y = 30.1F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection with RCS above minimum threshold should still be flagged unreliable if other hypothesis conditions are met.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_RCS)
{
   /** \precond
    * Create single detection inside ROI with RCS above minimum threshold.
    */
   det_raw.raw.rcs = -9.9F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable but NOT discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection with RCS and SNR above thresholds should still be flagged unreliable if elevation is high.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_RCS_SNR)
{
   /** \precond
    * Create single detection inside ROI with RCS and SNR above thresholds.
    */
   det_raw.raw.rcs = -9.9F;
   det_raw.raw.snr = 13.1F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable but NOT discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection with high azimuth confidence should be flagged unreliable and discarded, even if RCS and SNR are good.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_RCS_SNR_Confid)
{
   /** \precond
    * Create single detection inside ROI with RCS and SNR above thresholds, but high azimuth confidence.
    */
   det_raw.raw.rcs = -9.9F;
   det_raw.raw.snr = 13.1F;
   det_raw.raw.confid_azimuth = 3;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection flagged unreliable and discarded.
    */
   CHECK_TRUE(det_prop.f_unreliable_in_clutter);
   CHECK_FALSE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection NOT matching any clutter hypothesis criteria should NOT be flagged unreliable.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_RCS_SNR_Confid_Elevation)
{
   /** \precond
    * Create single detection inside ROI with RCS and SNR above thresholds, and low elevation.
    */
   det_raw.raw.rcs = -9.9F;
   det_raw.raw.snr = 13.1F;
   det_raw.raw.elevation = F360_DEG2RAD(9.9F);

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Detection with RCS above maximum threshold should NOT be flagged unreliable.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Max_RCS)
{
   /** \precond
    * Create single detection inside ROI with RCS above maximum threshold.
    */
   det_raw.raw.rcs = 5.1F;

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Close detections < 20m in gen7 sensor should have stricter snr conditions.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Close_Range)
{
   /** \precond
    * Create single detection inside ROI, range lower than 20m, from gen7 sensor, rcs and elevation above threhold.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   det_raw.raw.range = 19.9F;
   det_raw.raw.rcs = -9.9F;
   det_raw.raw.elevation = F360_DEG2RAD(9.9F);

   /** \action
    * Call Update_Detection_Property
    */
   Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

   /** \result
    * Expect detection NOT flagged unreliable and NOT discarded.
    */
   CHECK_FALSE(det_prop.f_unreliable_in_clutter);
   CHECK_TRUE(det_prop.f_ok_to_use);
}

/** \purpose
 * Gen7 sensors should support extended ROI for flagging unreliable detections.
 */
TEST(Discard_Unreliable_Detections_In_Clutter, Discard_Unreliable_Detections_In_Clutter_Gen7_sensors)
{
   /** \precond
    * Create single detection inside extended ROI for Gen7 sensors.
    */
   constexpr uint8_t sensor_count = 6;
   const F360_Sensor_Type_T sensors[sensor_count] = {
      F360_SENSOR_TYPE_SRR7_PLUS_RADAR,
      F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR,
      F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR,
      F360_SENSOR_TYPE_FLR7_PLT_RADAR,
      F360_SENSOR_TYPE_FLR7_RADAR,
      F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR};
      
   det_prop.vcs_position.x = 99.9F; // beyond SRR6+ valid ROI

   for(uint8_t i = 0; i < sensor_count; i++)
   {
      sensor.constant.sensor_type = sensors[i];
      if(i > 1)
      {
         det_prop.vcs_position.x = -99.9F; // beyond SRR6+ valid ROI
      }
      /** \action
       * Call Update_Detection_Property
       */
      Discard_Unreliable_Detections_In_Clutter(tracker_info, det_raw, sensor, det_prop);

      /** \result
       * Expect detection flagged unreliable but NOT discarded.
       */
      CHECK_TRUE(det_prop.f_unreliable_in_clutter);
      CHECK_TRUE(det_prop.f_ok_to_use);
   }
}

/** @}*/
