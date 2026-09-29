/*===========================================================================*\
* FILE: f360_identify_and_flag_internal_reflections.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Identify_And_Flag_Internal_Reflections() and helper functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*============================================================================*/

#include "f360_identify_and_flag_internal_reflections.h"
#include <algorithm>
#include <limits>

namespace f360_variant_A
{

   static inline bool Should_Buffer_Be_Updated_For_Sensor(
      const float32_t sensor_long_vel,
      const float32_t min_sensor_velocity_for_updating_buffer
   );

   /*===========================================================================*\
   * FUNCTION: Identify_And_Flag_Internal_Reflections()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T &detection
   *  const F360_Radar_Sensor_T &sensor
   *  const bool f_mark_internal_reflections_enabled
   *  F360_Radar_Sensor_Props_T &sensor_prop
   *  F360_Detection_Props_T &detection_prop
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Due to geometric integration and design, some vehicle lines tend to have
   * internal reflection paths (typically involving the crash beam or car paint)
   * which lead to detections which appear as if they were reflections from a
   * target in front of the host which is traveling at the same speed as the host.
   * (I.e. range_rate_raw ~ 0, range_rate_comp ~ host_speed)
   * Thus, they are likely to cause the creation of ghost objects.
   *
   * This function is used to identify and flag such detections.
   *
   * This function was ported from the GDSR tracker (written in C) to F360.
   *
   * The functionality is ran per sensor and is currently disabled by default in
   * the tracker core repository. It is enabled through CAF parameters in Customer projects.
   * These flags are part of logs and thus resim is still preserved.
   *
   \*===========================================================================*/
   void Identify_And_Mark_Internal_Reflections(
      const rspp_variant_A::RSPP_Detection_T& detection,
      const F360_Radar_Sensor_T& sensor,
      F360_Radar_Sensor_Props_T& sensor_prop,
      F360_Detection_Props_T& detection_prop)
   {

      const F360_Internal_Reflections_Calib_T& det_int_ref_calib = sensor.constant.internal_reflections;

      if (det_int_ref_calib.f_enable && Is_Detection_Relevant_For_Internal_Reflection_Buffer(detection, det_int_ref_calib))
      {
         const F360_Radar_Sensor_T& det_sensor = sensor;
         Internal_Reflection_Buffer_Slot(&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE] = sensor_prop.internal_reflections_buffer;

         const bool f_perform_buffer_update = Should_Buffer_Be_Updated_For_Sensor(det_sensor.variable.vcs_velocity.longitudinal,
            det_int_ref_calib.min_host_vel);

         const int32_t buffer_idx_matching_det = Find_Buffer_Slot_Matching_Detection(sensor_int_ref_buffer, detection, det_int_ref_calib);
         if (buffer_idx_matching_det > -1)
         {
            if (f_perform_buffer_update)
            {
               Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(det_int_ref_calib, detection, sensor_int_ref_buffer[buffer_idx_matching_det]);
            }

            if (sensor_int_ref_buffer[buffer_idx_matching_det].f_classified_as_internal_reflection)
            {
               detection_prop.f_ok_to_use = false;
            }
         }
         else
         {
            if (f_perform_buffer_update)
            {
               Add_Det_To_Unused_Reflection_Buffer_Slot(detection, sensor_int_ref_buffer);
            }
         }

      }

   }


   /*===========================================================================*\
   * FUNCTION: Update_Buffers_Age_And_Reset_Implausible_Slots()
   * ===========================================================================
   * RETURN VALUE:
   * bool - flag indicating if function should be enabled
   *
   * PARAMETERS:
   *  const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   *  F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS])
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * For a slow host, stationary surroundings would end up in the sensor buffer.
   * Therefore a buffer update can not be performed while the host is in stand still.
   *
   * The buffer is preserved until the host is driving again. This allows the flagging
   * to be accurate without having to fill the buffer again.
   *
   * If the functionality is enabled for at least 1 sensor, f_func_enabled_this_iteration
   * will be set to true.
   *
   \*===========================================================================*/
   bool Is_Bike_Carrier_Attached(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS])
   {
      bool f_func_enabled_this_iteration = false;

      for (uint32_t i_sens = 0U; i_sens < MAX_NUMBER_OF_SENSORS; i_sens++)
      {
         if (sensors[i_sens].constant.internal_reflections.f_enable && sensors[i_sens].variable.is_valid)
         {
            f_func_enabled_this_iteration = true;
            const bool f_perform_buffer_update = Should_Buffer_Be_Updated_For_Sensor(sensors[i_sens].variable.vcs_velocity.longitudinal,
                sensors[i_sens].constant.internal_reflections.min_host_vel);
            if (f_perform_buffer_update)
            {
               Update_Single_Buffer_Age_And_Reset_Implausible_Slots(sensors[i_sens].constant.internal_reflections, sensor_props[i_sens].internal_reflections_buffer);
            }
         }
      }
      return f_func_enabled_this_iteration;
   }

   /*===========================================================================*\
   * FUNCTION: Update_Single_Buffer_Age_And_Reset_Implausible_Slots()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Internal_Reflections_Calib &int_ref_calib,
   *  Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Increments the age of all slots in a single internal reflection buffer.
   * Will reset slots with low occurrence rate of matching detections.
   *
   \*===========================================================================*/
   void Update_Single_Buffer_Age_And_Reset_Implausible_Slots(
      const F360_Internal_Reflections_Calib_T &int_ref_calib,
      Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE])
   {
      for (uint32_t i_buf = 0U; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++)
      {
         if (sensor_int_ref_buffer[i_buf].age > 0U)
         {
            if (sensor_int_ref_buffer[i_buf].age < std::numeric_limits<uint32_t>::max())
            {
               sensor_int_ref_buffer[i_buf].age++;
            }
            else
            {
               /* Age is saturated. Divide both age and occurence count with 2 to
                  keep the same occurence rate while enabling to continue to update
                  the buffer. */
               sensor_int_ref_buffer[i_buf].age /= 2U;
               sensor_int_ref_buffer[i_buf].occurrence_count /= 2U;
            }
            sensor_int_ref_buffer[i_buf].f_updated_this_cycle = false;

            // Reset slots with a low occurance_rate
            if (sensor_int_ref_buffer[i_buf].age >= int_ref_calib.age_threshold)
            {
               const float32_t occurence_rate = static_cast<float32_t>(sensor_int_ref_buffer[i_buf].occurrence_count) / static_cast<float32_t>(sensor_int_ref_buffer[i_buf].age);

               if (occurence_rate < int_ref_calib.occurrence_lowerlimit)
               {
                  sensor_int_ref_buffer[i_buf] = Internal_Reflection_Buffer_Slot{};
               }
            }
         }

      }
   }


   /*===========================================================================*\
   * FUNCTION: Is_Detection_Relevant_For_Internal_Reflection_Buffer()
   * ===========================================================================
   * RETURN VALUE:
   * Boolean f_det_relevant indicating that the detection is relevant for further
   * examination.
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T &raw_det,
   *  const F360_Internal_Reflections_Calib &int_ref_calib
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Checks if a detection meets the required criteria to be further examined
   * for if it should be flagged as an internal reflection.
   *
   \*===========================================================================*/
   bool Is_Detection_Relevant_For_Internal_Reflection_Buffer(
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      const F360_Internal_Reflections_Calib_T &int_ref_calib)
   {
      const bool f_range_rate_ok = std::abs(raw_det.raw.range_rate) <= int_ref_calib.max_abs_range_rate;
      const bool f_range_ok = raw_det.raw.range <= int_ref_calib.range_max;
      const bool f_rcs_ok = raw_det.raw.rcs <= int_ref_calib.rcs_max;

      const bool f_det_relevant = (f_range_rate_ok && f_range_ok && f_rcs_ok);

      return f_det_relevant;
   }


   /*===========================================================================*\
   * FUNCTION: Find_Buffer_Slot_Matching_Detection()
   * ===========================================================================
   * RETURN VALUE:
   * int32_t matching_buffer_idx, index of buffer slot that, according to
   * calibration parameters, is similar enough to the input detection.
   *
   * PARAMETERS:
   *  const Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE],
   *  const rspp_variant_A::RSPP_Detection_T &raw_det,
   *  const F360_Internal_Reflections_Calib &int_ref_calib
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Given a detection, attempts to find a buffer slot that, according to
   * calibration parameters, is similar enough to the input detection.
   *
   \*===========================================================================*/
   int32_t Find_Buffer_Slot_Matching_Detection(
      const Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE],
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      const F360_Internal_Reflections_Calib_T &int_ref_calib)
   {
      int32_t matching_buffer_idx = -1;

      for (uint32_t i_buf = 0U; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++)
      {
         if (Does_Det_Match_Buffer_Slot(raw_det, sensor_int_ref_buffer[i_buf], int_ref_calib))
         {
            matching_buffer_idx = static_cast<int32_t>(i_buf);
            break;
         }
      }
      return matching_buffer_idx;
   }


   /*===========================================================================*\
   * FUNCTION: Does_Det_Match_Buffer_Slot()
   * ===========================================================================
   * RETURN VALUE:
   * Boolean f_det_match_slot indicating that the input detection matches buffer
   * slot according to calibration parameters.
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T &raw_det,
   *  const Internal_Reflection_Buffer_Slot &buffer_slot,
   *  const F360_Internal_Reflections_Calib &int_ref_calib
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Checks if the input detection matches the input buffer slot according to
   * calibration parameters.
   *
   \*===========================================================================*/
   bool Does_Det_Match_Buffer_Slot(
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      const Internal_Reflection_Buffer_Slot &buffer_slot,
      const F360_Internal_Reflections_Calib_T &int_ref_calib)
   {
      const bool f_valid_buffer_slot = buffer_slot.age > 0U;
      const bool f_range_match = std::abs(raw_det.raw.range - buffer_slot.range) < int_ref_calib.range_tolerance;
      const bool f_azimuth_match = std::abs(raw_det.raw.azimuth - buffer_slot.azimuth) < int_ref_calib.azimuth_tolerance;
      const bool f_rcs_match = std::abs(raw_det.raw.rcs - buffer_slot.rcs) < int_ref_calib.rcs_tolerance;

      const bool f_det_match_slot = (f_valid_buffer_slot && f_range_match && f_azimuth_match && f_rcs_match);

      return f_det_match_slot;
   }


   /*===========================================================================*\
   * FUNCTION: Add_Det_to_Existing_Internal_Reflection_Buffer_Slot()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Internal_Reflections_Calib &int_ref_calib,
   *  const rspp_variant_A::RSPP_Detection_T &raw_det,
   *  Internal_Reflection_Buffer_Slot &buffer_slot
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Given a detection, adds it to the given buffer slot.
   *
   \*===========================================================================*/
   void Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(
      const F360_Internal_Reflections_Calib_T &int_ref_calib,
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      Internal_Reflection_Buffer_Slot &buffer_slot)
   {
      buffer_slot.range = (static_cast<float32_t>(buffer_slot.occurrence_count) * buffer_slot.range + raw_det.raw.range) / (static_cast<float32_t>(buffer_slot.occurrence_count) + 1.0F);
      buffer_slot.azimuth = (static_cast<float32_t>(buffer_slot.occurrence_count) * buffer_slot.azimuth + raw_det.raw.azimuth) / (static_cast<float32_t>(buffer_slot.occurrence_count) + 1.0F);
      buffer_slot.rcs = (static_cast<float32_t>(buffer_slot.occurrence_count) * buffer_slot.rcs + raw_det.raw.rcs) / (static_cast<float32_t>(buffer_slot.occurrence_count) + 1.0F);

      if (!buffer_slot.f_updated_this_cycle)
      {
         buffer_slot.f_updated_this_cycle = true;
         if (buffer_slot.occurrence_count < std::numeric_limits<uint32_t>::max())
         {
            buffer_slot.occurrence_count++;
         }
      }
      buffer_slot.f_classified_as_internal_reflection = Classify_Internal_Detection_Buffer_Slot(buffer_slot, int_ref_calib);
   }


   /*===========================================================================*\
   * FUNCTION: Classify_Internal_Detection_Buffer_Slot()
   * ===========================================================================
   * RETURN VALUE:
   * Boolean f_flag_matching_detections indicating that the buffer slot
   * meets the required criteria to flag other, similar detections, as
   * internal reflections.
   *
   * PARAMETERS:
   *  const Internal_Reflection_Buffer_Slot &buffer_slot,
   *  const F360_Internal_Reflections_Calib &int_ref_calib
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Evaluates the given buffer slot to see if it meets the criteria to flag
   * other, similar detections, as internal reflections.
   *
   \*===========================================================================*/
   bool Classify_Internal_Detection_Buffer_Slot(
      const Internal_Reflection_Buffer_Slot &buffer_slot,
      const F360_Internal_Reflections_Calib_T &int_ref_calib)
   {
      bool f_flag_matching_detections = false;
      if (buffer_slot.age >= int_ref_calib.age_threshold)
      {
         const float32_t occurence_rate = static_cast<float32_t>(buffer_slot.occurrence_count) / static_cast<float32_t>(buffer_slot.age);

         if (occurence_rate > int_ref_calib.occurrence_threshold)
         {
            f_flag_matching_detections = true;
         }
      }

      return f_flag_matching_detections;
   }


   /*===========================================================================*\
   * FUNCTION: Add_Det_To_Unused_Reflection_Buffer_Slot()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T &raw_det,
   *  Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Adds a detection to an unused buffer slot. If no unused slot is found, nothing happens.
   *
   \*===========================================================================*/
   void Add_Det_To_Unused_Reflection_Buffer_Slot(
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE])
   {
      for(uint8_t i = 0U; i < INTERNAL_REFLECTIONS_BUFFER_SIZE; i++)
      {
         const bool f_empty_slot_found = sensor_int_ref_buffer[i].occurrence_count == 0U;
         if(f_empty_slot_found)
         {
            sensor_int_ref_buffer[i].range = raw_det.raw.range;
            sensor_int_ref_buffer[i].azimuth = raw_det.raw.azimuth;
            sensor_int_ref_buffer[i].rcs = raw_det.raw.rcs;
            sensor_int_ref_buffer[i].occurrence_count = 1U;
            sensor_int_ref_buffer[i].age = 1U;
            break;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Should_Buffer_Be_Updated_For_Sensor()
   * ===========================================================================
   * RETURN VALUE:
   * Boolean indicating if a buffer slot should be updated or not.
   *
   * PARAMETERS:
   *  const float32_t sensor_long_vel,
   *  const float32_t min_sensor_velocity_for_updating_buffer
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Checks if the sensor is moving fast enough to update its internal
   * reflection buffer with new detections.
   *
   \*===========================================================================*/
   static inline bool Should_Buffer_Be_Updated_For_Sensor(
      const float32_t sensor_long_vel,
      const float32_t min_sensor_velocity_for_updating_buffer)
   {
      return sensor_long_vel > min_sensor_velocity_for_updating_buffer;
   }

}
