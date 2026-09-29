/*===================================================================================*\
 * FILE: f360_update_global_parameters.cpp
 *====================================================================================
* Copyright (C) 2019-2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * This file contains functions descriptions for
 * Update_Global_Parameters()
 * Calculate_Shrinked_FOV_Normals()
 * Check_Sensor_Configurations()
 *
 * ABBREVIATIONS:
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [06-Sep-2020]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
 \*====================================================================================*/

#include "f360_reuse.h"
#include "f360_update_global_parameters.h"
#include "f360_calc_obj_mov_stat_thresh.h"
#include "f360_constants.h"
#include "f360_math_func.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Global_Parameters()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Calibrations_T &calibrations
   * F360_Globals_T& globals
   * F360_TRKR_TIMING_INFO_T& timing_info
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
   * Function updates global parameters
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Update_Global_Parameters(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibrations,
      F360_Globals_T& globals,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
       Check_Sensor_Configuration(sensors, globals);

       Calculate_Four_Corner_Sensor_Config_Position_Bounds(sensors, globals);

       Calculate_Shrinked_FOV_Normals(sensors, calibrations, globals);

       Calculate_Average_Sensor_Position(sensors, globals);

       globals.obj_mov_stat_spd_thresh = Calc_Obj_Mov_Stat_Thresh(host.vcs_speed, &timing_info);
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Shrinked_FOV_Normals()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Calibrations_T &calibrations
   * F360_Globals_T& globals
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
   * This function calculates the rotated FOV normals
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Calculate_Shrinked_FOV_Normals(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibrations,
      F360_Globals_T& globals)
   {
       for (uint32_t current_sensor_idx = 0U; current_sensor_idx < MAX_NUMBER_OF_SENSORS; current_sensor_idx++)
       {
           const F360_Radar_Sensor_T& current_sensor = sensors[current_sensor_idx];

           if (current_sensor.variable.is_valid)
           {
               // Currently, all sensor normals are rotated by the same angle
               // So, if different rotation angle is needed for another sensors, that condition should be added here

              float32_t left_fov_normal_vector[2]; // index 0 is the x-component, index 1 is the y-component
              float32_t right_fov_normal_vector[2];

              if (F360_DET_RANGE_TYPE_LONG == Get_Range_Type(current_sensor.variable.look_id))
              {
                 left_fov_normal_vector[0] = current_sensor.refined.left_fov_normal[F360_LOOK_ID_0];
                 left_fov_normal_vector[1] = current_sensor.refined.left_fov_normal[F360_LOOK_ID_1];

                 right_fov_normal_vector[0] = current_sensor.refined.right_fov_normal[F360_LOOK_ID_0];
                 right_fov_normal_vector[1] = current_sensor.refined.right_fov_normal[F360_LOOK_ID_1];
              }
              else
              {
                 left_fov_normal_vector[0] = current_sensor.refined.left_fov_normal[F360_LOOK_ID_2];
                 left_fov_normal_vector[1] = current_sensor.refined.left_fov_normal[F360_LOOK_ID_3];

                 right_fov_normal_vector[0] = current_sensor.refined.right_fov_normal[F360_LOOK_ID_2];
                 right_fov_normal_vector[1] = current_sensor.refined.right_fov_normal[F360_LOOK_ID_3];
              }

               F360_Rotate_2D_Vector(
                   left_fov_normal_vector[0],
                   left_fov_normal_vector[1],
                   F360_Cosf(calibrations.k_fov_normal_rotation_angle),
                   F360_Sinf(calibrations.k_fov_normal_rotation_angle),
                   globals.rotated_left_fov_normal[current_sensor_idx][0],
                   globals.rotated_left_fov_normal[current_sensor_idx][1]);

               F360_Rotate_2D_Vector(
                   right_fov_normal_vector[0],
                   right_fov_normal_vector[1],
                   F360_Cosf(-calibrations.k_fov_normal_rotation_angle),
                   F360_Sinf(-calibrations.k_fov_normal_rotation_angle),
                   globals.rotated_right_fov_normal[current_sensor_idx][0],
                   globals.rotated_right_fov_normal[current_sensor_idx][1]);
           }
       }
   }

   /*===========================================================================*\
   * FUNCTION: Check_Sensor_Configuration()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Globals_T& globals
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
   * Updates flags inidicating radar sensor configurations
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Check_Sensor_Configuration(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       F360_Globals_T& globals)
   {
       uint8_t num_active_sensors = 0U;
       F360_Mounting_Location_T sensor_mounting_loc = F360_MOUNTING_LOCATION_UNKNOWN;
       
       // Flags to track corner sensor availability
       bool f_left_front_available = false;
       bool f_right_front_available = false;
       bool f_left_rear_available = false;
       bool f_right_rear_available = false;

       // Flags to check if at least one front sensor is available
       bool f_front_or_front_corner_sensor_available = false;

       // Flag to check if at least one rear sensor is available
       bool f_rear_sensor_available = false;

       for (uint32_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
       {
           const F360_Radar_Sensor_T& current_sensor = sensors[sensor_idx];
           
           if (current_sensor.variable.is_valid)
           {
               sensor_mounting_loc = current_sensor.constant.mounting_location;
               num_active_sensors++;
               
               // Check for sensor mounting locations
               switch (sensor_mounting_loc)
               {
                   case F360_MOUNTING_LOCATION_LEFT_FORWARD:
                       f_left_front_available = true;
                       f_front_or_front_corner_sensor_available = true;
                       break;
                   case F360_MOUNTING_LOCATION_RIGHT_FORWARD:
                       f_right_front_available = true;
                       f_front_or_front_corner_sensor_available = true;
                       break;
                   case F360_MOUNTING_LOCATION_LEFT_REAR:
                       f_left_rear_available = true;
                       f_rear_sensor_available = true;
                       break;
                   case F360_MOUNTING_LOCATION_RIGHT_REAR:
                       f_right_rear_available = true;
                       f_rear_sensor_available = true;
                       break;
                   case F360_MOUNTING_LOCATION_CENTER_FORWARD:
                      f_front_or_front_corner_sensor_available = true;
                      break;
                   case F360_MOUNTING_LOCATION_CENTER_REAR:
                      f_rear_sensor_available = true;
                      break;
                   case F360_MOUNTING_LOCATION_CENTER2_FORWARD:
                      f_front_or_front_corner_sensor_available = true;
                      break;
                   case F360_MOUNTING_LOCATION_CENTER2_REAR:
                      f_rear_sensor_available = true;
                      break;
                   case F360_MOUNTING_LOCATION_CENTER3_FORWARD:
                      f_front_or_front_corner_sensor_available = true;
                      break;
                   case F360_MOUNTING_LOCATION_CENTER3_REAR:
                      f_rear_sensor_available = true;
                      break;
                   default:
                       // Other mounting locations don't affect corner sensor detection
                       break;
               }
           }
       }

       // Flag that indicates if only the front center radar configuration is active
       globals.f_single_front_center_radar_only = ((1U == num_active_sensors)
           && (F360_MOUNTING_LOCATION_CENTER_FORWARD == sensor_mounting_loc));
           
       // Flag that indicates if all 4 corner sensors are available
       globals.f_four_corner_sensors_available = (f_left_front_available 
           && f_right_front_available 
           && f_left_rear_available 
           && f_right_rear_available);

       globals.f_front_or_front_corner_sensor_available = f_front_or_front_corner_sensor_available;
       globals.f_rear_sensor_available = f_rear_sensor_available;

   }

   /*===========================================================================*\
    * FUNCTION: Calculate_Average_Sensor_Position()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Globals_T& globals
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
   * Calculates the average mounting position across all active (valid) sensors
   * and stores the result in globals as scalar fields (x = longitudinal, y = lateral).
   *
   * If no sensors are active, the average position is set to (0.0F, 0.0F).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calculate_Average_Sensor_Position(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       F360_Globals_T& globals)
   {
       float32_t sensor_sum_longi_pos = 0.0F;
       float32_t sensor_sum_lateral_pos = 0.0F;
       uint8_t num_active_sensor = 0U;
       
       for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
       {
           if (sensors[i].variable.is_valid)
           {
               num_active_sensor += 1U;
               sensor_sum_longi_pos += sensors[i].constant.mounting_position.vcs_position.longitudinal;
               sensor_sum_lateral_pos += sensors[i].constant.mounting_position.vcs_position.lateral;
           }
       }
       
       if (num_active_sensor > 0U)
       {
           globals.average_sensor_position_x = sensor_sum_longi_pos / static_cast<float32_t>(num_active_sensor);
           globals.average_sensor_position_y = sensor_sum_lateral_pos / static_cast<float32_t>(num_active_sensor);
       }
       else
       {
           globals.average_sensor_position_x = 0.0F;
           globals.average_sensor_position_y = 0.0F;
       }
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Four_Corner_Sensor_Config_Position_Bounds()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Globals_T& globals
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function calculates the minimum and maximum longitudinal positions
   * of the four corner sensors only and stores them in the globals structure.
   * Only processes sensors if all four corner sensors are available.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calculate_Four_Corner_Sensor_Config_Position_Bounds(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       F360_Globals_T& globals)
   {
       // Only calculate if four corner sensors are available
       if (globals.f_four_corner_sensors_available)
       {
           float32_t min_sensor_x = INFTY;
           float32_t max_sensor_x = -INFTY;
           
           for (uint32_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
           {
               if (sensors[sensor_idx].variable.is_valid)
               {
                   const F360_Mounting_Location_T mounting_loc = sensors[sensor_idx].constant.mounting_location;
                   
                   // Only process corner sensors
                   if ((mounting_loc == F360_MOUNTING_LOCATION_LEFT_FORWARD) ||
                       (mounting_loc == F360_MOUNTING_LOCATION_RIGHT_FORWARD) ||
                       (mounting_loc == F360_MOUNTING_LOCATION_LEFT_REAR) ||
                       (mounting_loc == F360_MOUNTING_LOCATION_RIGHT_REAR))
                   {
                       const float32_t sensor_x = sensors[sensor_idx].constant.mounting_position.vcs_position.longitudinal;
                       if (sensor_x < min_sensor_x)
                       {
                           min_sensor_x = sensor_x;
                       }
                       if (sensor_x > max_sensor_x)
                       {
                           max_sensor_x = sensor_x;
                       }
                   }
               }
           }

           if ((std::abs(min_sensor_x) > 0.0F) && (min_sensor_x > -INFTY) && 
                (std::abs(max_sensor_x) > 0.0F) && (max_sensor_x < INFTY))
           {
               globals.four_corner_sensor_config_min_longitudinal_position = min_sensor_x;
               globals.four_corner_sensor_config_max_longitudinal_position = max_sensor_x;
           }
           else
           {    
               // Values indicating that is something wrong and not leave the variable not initialized
               globals.four_corner_sensor_config_min_longitudinal_position = 0.0F;
               globals.four_corner_sensor_config_max_longitudinal_position = 0.0F;
           }
           

       }
       else
       {
           // Reset to default values when four corner sensors are not available
           globals.four_corner_sensor_config_min_longitudinal_position = 0.0F;
           globals.four_corner_sensor_config_max_longitudinal_position = 0.0F;
       }
   }

}

