/*===========================================================================*\
* FILE: f360_mark_suspicious_angle_jump_detections.cpp
*============================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains definitions of Mark_Suspicious_Stationary_Angle_Jump_Detection.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_mark_suspicious_angle_jump_detections.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_try_to_dealiase_range_rate.h"
#include <algorithm>

namespace f360_variant_A
{
   static inline float32_t Calc_Range_Rate_Comp_diff_thres(const float32_t range_rate_compensated)
   {
      constexpr float32_t range_rate_comp_lower_limit = 0.24F;
      constexpr float32_t range_rate_comp_upper_limit = 0.50F;
      constexpr float32_t range_rate_comp_factor = 0.13F;
      float32_t range_rate_comp_diff_thres = std::max(std::abs(range_rate_compensated*range_rate_comp_factor), range_rate_comp_lower_limit);
      range_rate_comp_diff_thres = std::min(range_rate_comp_diff_thres, range_rate_comp_upper_limit);
      return range_rate_comp_diff_thres;
   }
   /*===========================================================================*\
   * FUNCTION: Mark_Suspicious_Stationary_Angle_Jump_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T& sensor,
   * const rspp_variant_A::RSPP_Detection_T& rspp_det,
   * F360_Detection_Props_T& det_prop)
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function marks out suspicious stationary angle jump detections. Currently
   * it is only applicable to gen 7 radars.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Suspicious_Stationary_Angle_Jump_Detection(
      const F360_Radar_Sensor_T& sensor,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      F360_Detection_Props_T& det_prop)
   {
      Sensor_Stationary_Angle_Ambiguity_T sensor_angle_amb = {};
      const bool f_is_moving_angle_jump_context = false;  // Stationary angle jump detection
      Determine_Precond_For_Angle_Jumps(sensor, rspp_det, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

      constexpr float32_t min_host_speed = F360_KPH2MPS(20.0F);
      if ((sensor_angle_amb.f_sensor_relevant) &&
         (det_prop.f_ok_to_use) &&
         (host.speed > min_host_speed) &&
         (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_prop.motion_status))
      {
         constexpr float32_t abs_range_rate_comp_cond = 0.7F;
         constexpr float32_t det_snr_cond = 25.0F;
         constexpr float32_t det_min_range = 8.0F;
         constexpr float32_t det_min_vcs_az_angle = F360_DEG2RAD(15.0F);
         // to re-examine the snr condition
         if((std::abs(det_prop.range_rate_compensated) > abs_range_rate_comp_cond) && (rspp_det.raw.snr < det_snr_cond)
            && (std::abs(rspp_det.processed.vcs_az) > det_min_vcs_az_angle) && (rspp_det.raw.range > det_min_range))
         {
            //hypothesis is that the object that the angle ambiguous detections come from is stationary
            for (uint8_t az_cand_idx = 0U; az_cand_idx < sensor_angle_amb.nr_of_candidates; az_cand_idx++)
            {
                if (sensor_angle_amb.angle_ambiguity_candidates_rad[az_cand_idx] < INFTY)
                {
                    const float32_t angle_shift = sensor_angle_amb.angle_ambiguity_candidates_rad[az_cand_idx];
                    const float32_t new_vcs_az_candidate = sensor.variable.vacs_boresight_az_estimated + (rspp_det.raw.azimuth + angle_shift) * static_cast<float32_t>(sensor.constant.polarity);
                    const float32_t rr_comp_new = rspp_det.raw.range_rate
                        + sensor.variable.vcs_velocity.longitudinal * F360_Cosf(new_vcs_az_candidate)
                        + sensor.variable.vcs_velocity.lateral * F360_Sinf(new_vcs_az_candidate);

                    const float32_t range_rate_comp_diff_thres = Calc_Range_Rate_Comp_diff_thres(det_prop.range_rate_compensated);

                    float32_t dealiased_rr_comp = 0.0F;
                    float32_t rr_interval = 0.0F;
                    // dealiase against 0 compensated range rate
                    const bool f_suspected_angle_jump = Try_To_Dealiase_Range_Rate(
                        rr_comp_new,
                        0.0F,
                        range_rate_comp_diff_thres,
                        sensor.constant.v_wrapping[sensor.variable.look_id],
                        0.0F,
                        dealiased_rr_comp,
                        rr_interval);

                    if (f_suspected_angle_jump)
                    {
                        det_prop.f_angle_amb = true;
                        break;
                    }
                }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Precond_For_Angle_Jumps()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T& sensor,
   * const rspp_variant_A::RSPP_Detection_T& rspp_det,
   * const F360_Detection_Props_T& det_prop,
   * const bool f_moving_angle_jump_context,
   * Sensor_Stationary_Angle_Ambiguity_T(&sensors_angle_amb)
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determine the preconditions for angle jump counter measures.
   * When f_moving_angle_jump_context is true, only SRR7+ radar is supported
   * (both forward and rear mounting locations).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Determine_Precond_For_Angle_Jumps(
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Detection_Props_T& det_prop,
      const bool f_moving_angle_jump_context,
      Sensor_Stationary_Angle_Ambiguity_T(&sensors_angle_amb))
   {
      if (sensor.variable.is_valid)
      {
         const bool f_srr7_plus_sensor = ((F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR == sensor.constant.sensor_type) ||
            (F360_SENSOR_TYPE_SRR7_PLUS_RADAR == sensor.constant.sensor_type) ||
            (F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR == sensor.constant.sensor_type));

         if (f_moving_angle_jump_context)
         {
            // For moving angle jump context, only SRR7+ radar is supported (forward and rear)
            const bool valid_location =
               (F360_MOUNTING_LOCATION_RIGHT_FORWARD == sensor.constant.mounting_location) ||
               (F360_MOUNTING_LOCATION_LEFT_FORWARD  == sensor.constant.mounting_location) ||
               (F360_MOUNTING_LOCATION_RIGHT_REAR    == sensor.constant.mounting_location) ||
               (F360_MOUNTING_LOCATION_LEFT_REAR     == sensor.constant.mounting_location);
            if ((f_srr7_plus_sensor) && valid_location)
            {
               Get_SRR7p_Amb_Angles(sensor, rspp_det, det_prop, sensors_angle_amb);
            }
            else
            {
               sensors_angle_amb = { false, 0U, {INFTY, INFTY, INFTY, INFTY, INFTY, INFTY, INFTY, INFTY} };
            }
         }
         else
         {
            if ((F360_SENSOR_TYPE_FLR7_RADAR == sensor.constant.sensor_type) ||
               (F360_SENSOR_TYPE_FLR7_PLT_RADAR == sensor.constant.sensor_type) ||
               (F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR == sensor.constant.sensor_type))
            {
               Get_FLR7_Amb_Angles(rspp_det, sensors_angle_amb);
            }
            else if ((f_srr7_plus_sensor) &&
               ((F360_MOUNTING_LOCATION_RIGHT_FORWARD == sensor.constant.mounting_location) || (F360_MOUNTING_LOCATION_LEFT_FORWARD == sensor.constant.mounting_location)))
            {
               Get_SRR7p_Amb_Angles(sensor, rspp_det, det_prop, sensors_angle_amb);
            }
            else
            {
               sensors_angle_amb = { false, 0U, {INFTY, INFTY, INFTY, INFTY, INFTY, INFTY, INFTY, INFTY} };
            }
         }
      }
      else
      {
         sensors_angle_amb = { false, 0U, {INFTY, INFTY, INFTY, INFTY, INFTY, INFTY, INFTY, INFTY} };
      }
   }

/*===========================================================================*\
* FUNCTION: Get_SRR7p_Amb_Angles()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const F360_Radar_Sensor_T& sensor,
* const rspp_variant_A::RSPP_Detection_T& rspp_det,
* const F360_Detection_Props_T& det_prop,
* Sensor_Stationary_Angle_Ambiguity_T(&sensors_angle_amb)
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Determine the angles of possible angle jump from a det in SRR7p radar
* provided the azimuth of the measured det. Since amount of angle jump is
* related to the azimuth of measured det, a Look Up Table (LUT) is used to
* map the different angle jump values possible given a certain azimuth of
* the measured det. The LUT checks in resolution of 5deg of measured det
* azimuth.
* 
* Depending on the measured det properties, only a limited number of angle jumps can
* be chosen to be checked to keep computational costs and risk of False Pos
* low (although in some extreme cases upto 7 angles could be checked). 
* This is done with the following conditions:
* No Angle jumps outside SRR FOV is checked and in front of host only clockwise
* angle jumps are checked for dets to the left of host and anti clockwise angle jumps
* are checked for dets to the right of the host. Behind host only counter clockwise angle 
* jumps are checked for dets to the left of host and for clockwise angle jumps for dets to
* the right of host.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/

   void Get_SRR7p_Amb_Angles(
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Detection_Props_T& det_prop,
      Sensor_Stationary_Angle_Ambiguity_T(&sensors_angle_amb))
   {
      constexpr uint8_t num_LUT_rows = 31U;
      // LUT for raw az -75:5:75, -75 at index 0, 0 at index 15, 75 at index 30

      constexpr float32_t LUT[num_LUT_rows][MAX_NUM_ANGLE_JUMPS] = {
         {0.0F, 29.3F, 47.2F, 62.5F, 77.0F, 91.5F, 107.3F, 126.6F},
         {0.0F, 26.4F, 43.9F, 59.1F, 73.5F, 88.1F, 104.1F, 124.1F},
         {0.0F, 24.0F, 41.0F, 56.0F, 70.4F, 85.1F, 101.4F, 122.5F},
         {0.0F, 22.0F, 38.5F, 53.3F, 67.7F, 82.6F, 99.3F, 122.1F},
         {0.0F, 20.3F, 36.4F, 51.0F, 65.4F, 80.5F, 97.9F, 123.6F},
         {0.0F, 18.9F, 34.6F, 49.1F, 63.5F, 78.9F, 97.2F, 129.7F},
         {-28.2F, 0.0F, 17.8F, 33.0F, 47.5F, 62.0F, 77.9F, 97.5F},
         {-23.2F, 0.0F, 16.9F, 31.8F, 46.2F, 60.9F, 77.4F, 99.0F},
         {-20.4F, 0.0F, 16.1F, 30.8F, 45.2F, 60.2F, 77.6F, 102.9F},
         {-60.0F, -18.6F, 0.0F, 15.5F, 30.0F, 44.5F, 60.0F, 78.6F},
         {-42.3F, -17.3F, 0.0F, 15.1F, 29.4F, 44.1F, 60.3F, 80.8F},
         {-37.4F, -16.3F, 0.0F, 14.7F, 29.1F, 44.1F, 61.1F, 85.2F},
         {-34.4F, -15.6F, 0.0F, 14.5F, 29.0F, 44.4F, 62.8F, 97.4F},
         {-57.5F, -32.3F, -15.1F, 0.0F, 14.4F, 29.0F, 45.2F, 65.7F},
         {-51.8F, -31.0F, -14.7F, 0.0F, 14.4F, 29.4F, 46.5F, 70.9F},
         {-90.0F, -48.6F, -30.0F, -14.5F, 0.0F, 14.5F, 30.0F, 48.6F},
         {-70.9F, -46.5F, -29.4F, -14.4F, 0.0F, 14.7F, 31.0F, 51.8F},
         {-65.7F, -45.2F, -29.0F, -14.4F, 0.0F, 15.1F, 32.3F, 57.5F},
         {-97.4F, -62.8F, -44.4F, -29.0F, -14.5F, 0.0F, 15.6F, 34.4F},
         {-85.2F, -61.1F, -44.1F, -29.1F, -14.7F, 0.0F, 16.3F, 37.4F},
         {-80.8F, -60.3F, -44.1F, -29.4F, -15.1F, 0.0F, 17.3F, 42.3F},
         {-120.0F, -78.6F, -60.0F, -44.5F, -30.0F, -15.5F, 0.0F, 18.6F},
         {-102.9F, -77.6F, -60.2F, -45.2F, -30.8F, -16.1F, 0.0F, 20.4F},
         {-99.0F, -77.4F, -60.9F, -46.2F, -31.8F, -16.9F, 0.0F, 23.2F},
         {-97.5F, -77.9F, -62.0F, -47.5F, -33.0F, -17.8F, 0.0F, 28.2F},
         {-129.7F, -97.2F, -78.9F, -63.5F, -49.1F, -34.6F, -18.9F, 0.0F},
         {-123.6F, -97.9F, -80.5F, -65.4F, -51.0F, -36.4F, -20.3F, 0.0F},
         {-122.1F, -99.3F, -82.6F, -67.7F, -53.3F, -38.5F, -22.0F, 0.0F},
         {-122.5F, -101.4F, -85.1F, -70.4F, -56.0F, -41.0F, -24.0F, 0.0F},
         {-124.1F, -104.1F, -88.1F, -73.5F, -59.1F, -43.9F, -26.4F, 0.0F},
         {-126.6F, -107.3F, -91.5F, -77.0F, -62.5F, -47.2F, -29.3F, 0.0F}
      };

      uint8_t LUT_idx = static_cast<uint8_t>(F360_Roundf((rspp_det.raw.azimuth / F360_DEG2RAD(5.0F)) + static_cast<float32_t>((num_LUT_rows-1U)/2U))); // Mapping Det Az to appropriate row in LUT. Adding (num_LUT_rows-1)/2 to shift the ID so that highest negative az in LUT corresponds to LUT_idx 0.
      LUT_idx = LUT_idx > num_LUT_rows-1U ? num_LUT_rows-1U : LUT_idx; //Saturating with upper value of LUT_idx (no need to saturate lower value to 0 since LUT_idx is uint

      const bool f_rear_sensor = Is_Rear_Corner_Sensor(sensor);
      const int32_t det_side_sign = F360_Sign(det_prop.vcs_position.y) * sensor.constant.polarity;
      // For rear sensor:
      // - if det is on left (negative y), check clockwise angle jump,
      // - if det is on right (positive y), check counterclockwise angle jump.
      // For forward sensor:
      // - if det is on left (positive y), check clockwise angle jump,
      // - if det is on right (negative y), check counterclockwise angle jump.
      const bool f_search_clockwise = f_rear_sensor ? (det_side_sign == -1) : (det_side_sign == 1); 
      const float32_t min_az_check_for_anglejump_deg = f_search_clockwise ? -135.0F : 0.0F;
      const float32_t max_az_check_for_anglejump_deg = f_search_clockwise ? 0.0F : 135.0F;

      sensors_angle_amb = { true, static_cast<uint8_t>(MAX_NUM_ANGLE_JUMPS), {} };

      for (int8_t i = 0; i < MAX_NUM_ANGLE_JUMPS; i++)
      {
         if ((LUT[LUT_idx][i] < max_az_check_for_anglejump_deg) && (LUT[LUT_idx][i] > min_az_check_for_anglejump_deg))
         {
            sensors_angle_amb.angle_ambiguity_candidates_rad[i] = F360_DEG2RAD(LUT[LUT_idx][i]);
         }
         else
         {
            sensors_angle_amb.angle_ambiguity_candidates_rad[i] = INFTY;
         }
      }
   }

   /*===========================================================================*\
* FUNCTION: Get_FLR7_Amb_Angles()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const rspp_variant_A::RSPP_Detection_T& rspp_det,
* Sensor_Stationary_Angle_Ambiguity_T(&sensors_angle_amb),
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Determine the angles of possible angle jump from a det in FLR7 radar
* provided the azimuth of the measured det. Since amount of angle jump is
* related to the azimuth of measured det, a Look Up Table (LUT) is used to
* map the different angle jump values possible given a certain azimuth of
* the measured det. The LUT checks in resolution of 5deg of measured det
* azimuth in between -40 to +40 deg.
*
* To reduce computation, incoming det azimuth is checked with 9 azimuth
* angles and the closest is chosen and its angle jumps are considered.
* Only 2 angle jumps, one clockwise and one counterclockwise are considered 
* since in most other cases the further jumps would be outside of FLR7 FOV.
* Also, this reduces possibility of False marking of angle jumps by reducing
* its scope.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/

   void Get_FLR7_Amb_Angles(
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      Sensor_Stationary_Angle_Ambiguity_T(& sensors_angle_amb))
   {
      const uint8_t num_LUT_rows = 17U;
      // LUT for raw az -40:5:40, -40 at index 0, 0 at index 9, 40 at index 17
      const float32_t LUT_resolution_deg = 5.0F; // Resolution of LUT in degrees

      const uint8_t num_of_angle_jumps_checked_FLR = 2U; // Only 2 angle jumps (one clockwise and one counterclockwise) to be checked for FLR7

      // Look Up Table (LUT) one clockwise and one counterclockwise angle jump from incoming det azimuth where each row is an incoming det az from -40deg to +40deg with a resolution of 5deg
      // Angle Jumps based on the formula AJs = asind((2*pi*3*sind(az)+[-1:1]*2*pi)/6/pi) where "az" is the incoming azimuth in degrees

      const float32_t LUT[num_LUT_rows][num_of_angle_jumps_checked_FLR] = {
       {-37.4537F, 21.9737F},
       {-30.0817F, 21.0991F},
       {-26.4427F, 20.4059F},
       {-24.1085F, 19.8775F},
       {-22.4816F, 19.5023F},
       {-21.3098F, 19.2733F},
       {-20.4630F, 19.1886F},
       {-19.8654F, 19.2514F},
       {-19.4712F, 19.4712F},
       {-19.2514F, 19.8654F},
       {-19.1886F, 20.4630F},
       {-19.2733F, 21.3098F},
       {-19.5023F, 22.4816F},
       {-19.8775F, 24.1085F},
       {-20.4059F, 26.4427F},
       {-21.0991F, 30.0817F},
       {-21.9737F, 37.4537F}
      };

      int8_t LUT_idx_temp = static_cast<int8_t>(F360_Roundf((rspp_det.raw.azimuth / F360_DEG2RAD(LUT_resolution_deg)) + static_cast<float32_t>((num_LUT_rows - 1U) / 2U))); // Mapping Det Az to appropriate row in LUT. Adding (num_LUT_rows-1)/2 to shift the ID so that highest negative az in LUT corresponds to LUT_idx 0.
      LUT_idx_temp = LUT_idx_temp > static_cast<int8_t>(num_LUT_rows - 1U) ? static_cast<int8_t>(num_LUT_rows - 1U) : LUT_idx_temp; //Saturating with upper value of LUT_idx
      LUT_idx_temp = LUT_idx_temp < 0 ? 0 : LUT_idx_temp; //Saturating with lower value of LUT_idx
      const uint8_t LUT_idx = static_cast<uint8_t>(LUT_idx_temp); // Final LUT index after saturation is converted to unsigned integer since it is not supposed to be a negative value

      const float32_t min_az_check_for_anglejump_deg = -75.0F; // FLR7 FOV limits
      const float32_t max_az_check_for_anglejump_deg = 75.0F; // FLR7 FOV limits

      sensors_angle_amb = { true, num_of_angle_jumps_checked_FLR, {} };
      const float32_t det_raw_az_deg = F360_RAD2DEG(rspp_det.raw.azimuth);

      for (uint8_t i = 0U; i < num_of_angle_jumps_checked_FLR; i++)
      {
         if (((det_raw_az_deg + LUT[LUT_idx][i]) < max_az_check_for_anglejump_deg) && ((det_raw_az_deg + LUT[LUT_idx][i]) > min_az_check_for_anglejump_deg)) // Checking if angle jump candidate is within FLR7 FOV
         {
            sensors_angle_amb.angle_ambiguity_candidates_rad[i] = F360_DEG2RAD(LUT[LUT_idx][i]);
         }
         else
         {
            sensors_angle_amb.angle_ambiguity_candidates_rad[i] = INFTY;
         }
      }
   }
}
