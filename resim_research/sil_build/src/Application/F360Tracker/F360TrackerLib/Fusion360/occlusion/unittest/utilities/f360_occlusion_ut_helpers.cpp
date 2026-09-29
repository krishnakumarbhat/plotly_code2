/*===================================================================================*\
* FILE: f360_occlusion_ut_helpers.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definitions of additional helper functions for occlusion testing
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_occlusion_ut_helpers.h"

namespace f360_variant_A
{
   void Set_Base_Object_Parameters(
      F360_Object_Track_T& object)
   {
      object.reference_point = F360_REFERENCE_POINT_CENTER;
      object.vcs_position.x = 0.0F;
      object.vcs_position.y = 0.0F;
      object.bbox.Set_Center(object.vcs_position);

      object.vcs_heading.Value(0.0F);
      object.hdg_ptng_disagmt = 0.0F;
      object.bbox.Set_Orientation(object.vcs_heading + object.hdg_ptng_disagmt);

      object.bbox.Set_Length(4.0F);
      object.bbox.Set_Width(2.0F);

      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
   }
   
   void Set_Left_Rear_Sensor(
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensor);
      sensor.constant.mounting_position.vcs_position.lateral = -0.75F;
      sensor.constant.mounting_position.vcs_position.longitudinal = -5.0F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = -2.35F;
   }

   void Set_Left_Front_Sensor(
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensor);
      sensor.constant.mounting_position.vcs_position.lateral = -0.75F;
      sensor.constant.mounting_position.vcs_position.longitudinal = -0.2F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = -0.78F;
   }

   void Set_Right_Rear_Sensor(
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensor);
      sensor.constant.mounting_position.vcs_position.lateral = 0.75F;
      sensor.constant.mounting_position.vcs_position.longitudinal = -5.0F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 2.35F;
   }

   void Set_Right_Front_Sensor(
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensor);
      sensor.constant.mounting_position.vcs_position.lateral = 0.75F;
      sensor.constant.mounting_position.vcs_position.longitudinal = -0.2F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 0.78F;
   }

   void Set_Common_Sensor_Parameters(
      F360_Radar_Sensor_T& sensor)
   {
      sensor.variable.is_valid = true;
      sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
      sensor.variable.look_id = F360_DET_LOOK_ID_0;
      sensor.constant.fov_min_az_rad[sensor.variable.look_id] = -1.5F;
      sensor.constant.fov_max_az_rad[sensor.variable.look_id] = 1.5F;
      sensor.constant.range_limits[sensor.variable.look_id] = 50.0F;
   }
}
