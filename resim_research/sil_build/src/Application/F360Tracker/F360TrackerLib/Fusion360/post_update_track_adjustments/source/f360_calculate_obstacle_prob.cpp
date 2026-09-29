/*===================================================================================*\
* FILE: f360_calculate_obstacle_prob.cpp
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions related to obstacle probability calculation
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_calculate_obstacle_prob.h"


namespace f360_variant_A
{
    static float32_t Calculate_Filter_Coefficient_For_Obstacle_Prob(
        const F360_Object_Track_T& obj,
        const F360_Host_T& host,
        const float32_t& range
    );

   /*===========================================================================*\
   * FUNCTION: Calculate_Properties_For_Obstacle_Prob()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Object_Track_T& obj
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
   * This function calculates properties of object's bounding box for obstacle probability calculation.
   * Those properties are:
   * - otg altitude of the center of the bounding box - based on mean of detections' positions
   * - height of a bounding box - based on the spread of detections' positions
   * 
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
    void Calculate_Properties_For_Obstacle_Prob(
        const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
        const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
        const F360_Calibrations_T& calib,
        F360_Object_Track_T& obj)
    {
        if ((obj.status == F360_OBJECT_STATUS_UPDATED) && (obj.ndets > 0U))
        {

            float32_t temp_w = 0.0F;
            float32_t temp_factor = 0.0F;
            float32_t otg_altitude[MAX_DETS_IN_OBJ_TRK] = {};
            float32_t weighted_sum_otg_altitude = 0.0F;
            float32_t total_weight = 0.0F;

            const float32_t object_range = F360_Get_Hypotenuse(obj.bbox.Get_Center().x, obj.bbox.Get_Center().y); //need sqrt for linear interpolation of range later

            const float32_t nr_loops_in_second = 1.0F / 0.05F;
            const int32_t object_age = static_cast<int32_t>(std::round(obj.time_since_initialization * nr_loops_in_second)) + 1;
            
            for (uint8_t i = 0U; i < obj.ndets; i++)
            {
                const uint32_t det_id = obj.detids[i];
                const rspp_variant_A::RSPP_Detection_T det_data = raw_detect_list.detections[det_id - 1U];
                const int32_t sen_idx = det_data.raw.sensor_id - 1;
                const int8_t det_conf_level = det_data.raw.confid_azimuth + det_data.raw.confid_elevation;

                temp_w = std::max(1.0F - 0.1F * (static_cast<float32_t>(object_age) - 1.0F), 0.2F);
                temp_w *= std::max(1.0F - 0.1F * (static_cast<float32_t>(det_conf_level) + 1.0F), 0.1F);

                if (sensors[sen_idx].constant.mounting_location == F360_MOUNTING_LOCATION_CENTER_FORWARD) // FLR only
                {
                    temp_factor = (object_range < 100.0F) ? 2.5F : 1.5F;
                }
                else
                {
                    if (object_range < 50.0F)
                    {
                        temp_factor = 1.25F;
                    }
                    else if (object_range < 100.0F)
                    {
                        temp_factor = 0.75F;
                    }
                    else
                    {
                        temp_factor = 0.5F;
                    }
                }

                temp_w *= temp_factor;
                weighted_sum_otg_altitude -= temp_w * det_data.processed.vcs_position_z; // negative det z is above the ground, positive is below
                otg_altitude[i] = -det_data.processed.vcs_position_z; // negative det z is above the ground, positive is below
                total_weight += temp_w;
            }

            if (total_weight > F360_EPSILON)
            {
                const bool f_stationary = (obj.movable_prob < 0.5F);
                const float32_t height_limit_min = f_stationary ? calib.k_obstacle_prob_min_bbox_height_nonmovable : calib.k_obstacle_prob_min_bbox_height_moving;
                const float32_t height_limit_max = f_stationary ? 2.0F : 3.0F;
                float32_t measured_otg_altitude = (weighted_sum_otg_altitude / total_weight);

                if ((object_age > 1) && (f_stationary))
                {
                    const float32_t filter_coef = 0.4F;
                    measured_otg_altitude = F360_Low_Pass_Filter_First_Order(measured_otg_altitude, obj.bbox_center_otg_altitude, filter_coef);
                }

                if (obj.ndets >= 2U)
                {
                    float32_t measured_height = 0.0F;
                    float32_t altitude_std_value = 0.0F;

                    for (uint8_t i = 0U; i < obj.ndets; i++)
                    {
                        altitude_std_value += (measured_otg_altitude - otg_altitude[i]) * (measured_otg_altitude - otg_altitude[i]);
                    }

                    const uint32_t sample_det_num = (obj.ndets > 4U) ? obj.ndets - 1U : obj.ndets;

                    altitude_std_value = F360_Sqrtf(altitude_std_value / static_cast<float32_t>(sample_det_num));
                    const float32_t double_altitude_std_value = 2.0F * altitude_std_value;
                    float32_t min_z_in_sigma = INFTY;
                    float32_t max_z_in_sigma = -INFTY;
                    uint8_t num_in_sigma = 0U;
                    float32_t min_z_in_double_sigma = INFTY;
                    float32_t max_z_in_double_sigma = -INFTY;
                    uint8_t num_in_double_sigma = 0U;
                    float32_t bbox_height = 0.0F;

                    for (uint8_t i = 0U; i < obj.ndets; i++)
                    {
                        const float32_t deviation = std::abs(measured_otg_altitude - otg_altitude[i]);
                        if (deviation < altitude_std_value)
                        {
                            if (min_z_in_sigma > otg_altitude[i])
                            {
                                min_z_in_sigma = otg_altitude[i];
                            }
                            if (max_z_in_sigma < otg_altitude[i])
                            {
                                max_z_in_sigma = otg_altitude[i];
                            }
                            num_in_sigma++;
                        }

                        if (deviation < double_altitude_std_value)
                        {
                            if (min_z_in_double_sigma > otg_altitude[i])
                            {
                                min_z_in_double_sigma = otg_altitude[i];
                            }
                            if (max_z_in_double_sigma < otg_altitude[i])
                            {
                                max_z_in_double_sigma = otg_altitude[i];
                            }
                            num_in_double_sigma++;
                        }
                    }
                    
                    const float32_t half_ndets = 0.5F * static_cast<float32_t>(obj.ndets);

                    if (static_cast<float32_t>(num_in_sigma) >=  std::max(6.0F, half_ndets))
                    {
                        bbox_height = std::min(double_altitude_std_value, (max_z_in_sigma - min_z_in_sigma));
                    }
                    else if (static_cast<float32_t>(num_in_double_sigma) >= std::max(2.0F, half_ndets))
                    {
                        bbox_height = std::min(4.0F * altitude_std_value, (max_z_in_double_sigma - min_z_in_double_sigma));
                    }
                    else
                    {
                        bbox_height = altitude_std_value;
                    }
                    measured_height = F360_Saturate(bbox_height, height_limit_min, height_limit_max);

                    if (object_range < 70.0F)
                    {
                        const float32_t min_gain_time = f_stationary ? 0.02F : 0.005F;
                        const float32_t min_gain_range = f_stationary ? 0.02F : 0.01F;
                        const float32_t max_gain_time = f_stationary ? 0.1F : 0.05F;
                        const float32_t max_gain_range = f_stationary ? 0.1F : 0.05F;

                        const float32_t gain_time = F360_Linear_Equation_With_Saturation(static_cast<float32_t>(obj.num_updates_since_init), 1.0F, 20.0F, max_gain_time, min_gain_time);
                        const float32_t gain_range = F360_Linear_Equation_With_Saturation(static_cast<float32_t>(F360_Sqrtf(object_range)), 30.0F, 10.0F, min_gain_range, max_gain_range);
                        
                        const float32_t total_gain = gain_time + gain_range;
                        obj.bbox_center_otg_altitude = f_stationary ? measured_otg_altitude : F360_Low_Pass_Filter_First_Order(measured_otg_altitude, obj.bbox_center_otg_altitude, total_gain);
                        obj.bbox_height = F360_Low_Pass_Filter_First_Order(measured_height, obj.bbox_height, total_gain);
                    }
                    else
                    {
                        obj.bbox_center_otg_altitude = measured_otg_altitude;
                        obj.bbox_height = measured_height;
                    }
                }
                else
                {
                    obj.bbox_center_otg_altitude = measured_otg_altitude;
                    if (f_stationary)
                    {
                        obj.bbox_height = F360_Low_Pass_Filter_First_Order(height_limit_min, obj.bbox_height, 0.2F);
                    }
                    else
                    {
                        obj.bbox_height = height_limit_min;
                    }
                }
            }
        }
    }

    /*===========================================================================*\
   * FUNCTION: Calculate_And_Filter_Obstacle_Prob()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * F360_Object_Track_T& obj
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
   * This function calculates and filters obstacle probability of an object based on its bounding box height and z-position.
   * 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
    void Calculate_And_Filter_Obstacle_Prob(
        const F360_Host_T& host,
        F360_Object_Track_T& obj
    )
    {
        if (obj.status >= F360_OBJECT_STATUS_UPDATED) // if an object is new, value is assigned at initialization
        {
            const float32_t range = F360_Get_Hypotenuse(obj.bbox.Get_Center().x, obj.bbox.Get_Center().y); // need sqrt for linear interpolation of range

            const float32_t obstacle_prob = Calculate_Instantaneous_Obstacle_Prob(obj, range);
            const float32_t alpha = Calculate_Filter_Coefficient_For_Obstacle_Prob(obj, host, range);

            obj.obstacle_prob = F360_Low_Pass_Filter_First_Order(obstacle_prob, obj.obstacle_prob, alpha);
        }
        
    }

    /*===========================================================================*\
  * FUNCTION: Calculate_Instantaneous_Obstacle_Prob()
  *===========================================================================
  * RETURN VALUE:
  * None
  *
  * PARAMETERS:
  * const F360_Host_T& host
  * F360_Object_Track_T& obj
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
  * This function calculates obstacle probability of an object based on its bounding box height and center otg altitude in a current scan.
  * Calculation is based on probability mass of normal distribution on an interval occupied by an object in z-axis.
  *
  * PRECONDITIONS:
  * None
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
    float32_t Calculate_Instantaneous_Obstacle_Prob(
        const F360_Object_Track_T& obj, 
        const float32_t& range
    )
    {
        const bool f_stationary = (obj.movable_prob < 0.5F);
        float32_t bottom_z_position = obj.bbox_center_otg_altitude - 0.5F * obj.bbox_height;
        float32_t top_z_position = obj.bbox_center_otg_altitude + 0.5F * obj.bbox_height;
        const float32_t core_zone_lower_limit = F360_Linear_Equation_With_Saturation(range, 10.0F, 150.0F, 0.35F, -0.5F);
        const float32_t core_zone_upper_limit = F360_Linear_Equation_With_Saturation(range, 10.0F, 150.0F, 1.5F, 3.0F);

        const float32_t bottom_z_bound = 0.3F;
        const float32_t upper_z_bound = 1.8F;

        if ((f_stationary) && (obj.bbox_center_otg_altitude > core_zone_lower_limit) && (obj.bbox_center_otg_altitude < core_zone_upper_limit))
        {
            // If height is to small, extend bottom and top edges to 0.3m and 1.8m respectively
            top_z_position = std::max(top_z_position, upper_z_bound);
            bottom_z_position = std::min(bottom_z_position, bottom_z_bound);
        }
        else if (!f_stationary)
        {
            const float32_t lower_limit_saturation = 0.5F;
            const float32_t upper_limit_saturation = 1.5F;

            const bool is_top_z_within_limit = (top_z_position > lower_limit_saturation) && (top_z_position < upper_limit_saturation);
            const bool is_bottom_z_within_limit = (bottom_z_position > lower_limit_saturation) && (bottom_z_position < upper_limit_saturation);
            
            const bool is_top_and_bottom_within_limit = is_top_z_within_limit && is_bottom_z_within_limit;
            const bool is_top_or_bottom_within_limit =  is_top_z_within_limit || is_bottom_z_within_limit;

            if (is_top_and_bottom_within_limit)
            {
                // If height is too small, extend bottom edge to the ground
                bottom_z_position = 0.0F;
            }

            else if (is_top_or_bottom_within_limit)
            {
                // If either of the bounding box edges are between 0.3 and 1.8, saturate to those values, to ensure interval is high enough
                top_z_position = std::max(top_z_position, upper_z_bound);
                bottom_z_position = std::min(bottom_z_position, bottom_z_bound);
            }
            else
            {
                // Do nothing
            }
        }
        else
        {
            // Do nothing
        }

        // We assume that measurement of an obstacle from radar will have mean = 1.1m and sigma = 0.45 below 100m,
        // so that there is 95% probability that the measurement will fall between 0.2m and 2.0m
        const float32_t obstacle_height_mean = 1.1F;
        const float32_t obstacle_height_std = F360_Linear_Equation_With_Saturation(range, 100.0F, 150.0F, 0.45F, 1.0F);
        const float32_t sqrt2 = 1.414213562F;
        const float32_t CDF_value1 = 0.5F * (1.0F + std::erf((top_z_position - obstacle_height_mean) / (obstacle_height_std * sqrt2)));
        const float32_t CDF_value2 = 0.5F * (1.0F + std::erf((bottom_z_position - obstacle_height_mean) / (obstacle_height_std * sqrt2)));
        const float32_t obstacle_prob = F360_Saturate(CDF_value1 - CDF_value2, 0.0F, 1.0F);
        return obstacle_prob;
    }

    /*===========================================================================*\
   * FUNCTION: Calculate_Filter_Coefficient_For_Obstacle_Prob()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * F360_Object_Track_T& obj
   * const float32_t& range
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
   * This function calculates filter coefficient for obstacle probability filter update.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
    static float32_t Calculate_Filter_Coefficient_For_Obstacle_Prob(
        const F360_Object_Track_T& obj, 
        const F360_Host_T& host, 
        const float32_t& range)
    {
        float32_t alpha = 0.1F;

        if ((std::abs(host.acceleration) > 1.5F) || (std::abs(host.yaw_rate_rad) > 0.1F))
        {
            alpha = 0.0001F;
        }
        else if (obj.movable_prob > 0.5F)
        {
            alpha = F360_Linear_Equation_With_Saturation(static_cast<float32_t>(obj.num_updates_since_init), 1.0F, 50.0F, 0.2F, 0.02F);
        } 
        else
        {
            alpha = F360_Linear_Equation_With_Saturation(range, 10.0F, 150.0F, 0.02F, 0.005F);
            if (range > 30.0F) //increase alpha for far away young objects
            {
                alpha *= F360_Linear_Equation_With_Saturation(obj.time_since_initialization, 0.0F, 0.5F, 25.0F, 1.0F); // alpha < 25 * 0.02 = 0.5
            }
        }
        return alpha;
    }
}

