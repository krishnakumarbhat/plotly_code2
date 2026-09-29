/*===========================================================================*\
* FILE:f360_static_env_polys_support_functions.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declarations of support function to Static Environment Polynomials
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_STATIC_ENV_POLYS_SUPPORT_FUNCTIONS_H
#define F360_STATIC_ENV_POLYS_SUPPORT_FUNCTIONS_H

#include "f360_static_env_poly_types.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_globals.h"

namespace f360_variant_A
{

   bool Check_If_Point_Is_Behind_SEP(
      const float32_t point_x_start,
      const float32_t point_x_end,
      const float32_t point_y_start,
      const float32_t point_y_end,
      const float32_t k,
      const float32_t m,
      const Static_Env_Poly_T &sep,
      const F360_Calibrations_T &calibs,
      Point &output_intersection
   );

   bool Check_If_Point_Is_On_SEP(
      const float32_t point_x,
      const float32_t point_y,
      const Static_Env_Poly_T &sep,
      const float32_t on_curve_threshold
   );

   bool Calc_Intersection_With_SEP(
      const float32_t k,
      const float32_t m,
      const Static_Env_Poly_T &sep,
      const F360_Calibrations_T &calibs,
      Point &intersection_1,
      Point &intersection_2
   );

   void Flag_Objects_On_And_Behind_SEP(
      const F360_Tracker_Info_T &tracker_info,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Flag_Single_Object_On_And_Behind_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T &obj
   );

   void Flag_Objects_Behind_SEP(
      const F360_Tracker_Info_T &tracker_info,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Flag_Single_Object_Behind_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      const F360_Globals_T& globals,
      F360_Object_Track_T &obj
   );

   void Flag_Objects_On_SEP(
      const F360_Tracker_Info_T &tracker_info,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Flag_Single_Object_On_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Object_Track_T &obj
   );

   void Mark_Detection_Behind_SEP(
      const F360_Radar_Sensor_T &sensor,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T &detection_prop
   );

   void Mark_Detection_On_SEP(
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T &detection_prop
   );
}
#endif
