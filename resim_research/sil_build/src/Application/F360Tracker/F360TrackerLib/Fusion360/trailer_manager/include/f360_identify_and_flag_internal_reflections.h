/*===========================================================================*\
* FILE: f360_identify_and_flag_internal_reflections.h
*============================================================================
* Copyright (C) 2019-2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Identify_And_Mark_Internal_Reflections() and other helper functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_IDENTIFY_AND_FLAG_INTERNAL_REFLECTIONS_H
#define F360_IDENTIFY_AND_FLAG_INTERNAL_REFLECTIONS_H

#include "f360_constants.h"
#include "f360_radar_sensor.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor_props.h"
#include "f360_internal_reflection_buffer_slot.h"

namespace f360_variant_A
{
   void Identify_And_Mark_Internal_Reflections(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Radar_Sensor_T &sensor,
      F360_Radar_Sensor_Props_T &sensor_prop,
      F360_Detection_Props_T &detection_prop
   );

   bool Is_Bike_Carrier_Attached(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS]
   );

   void Update_Single_Buffer_Age_And_Reset_Implausible_Slots(
      const F360_Internal_Reflections_Calib_T &int_ref_calib,
      Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE]
   );

   bool Is_Detection_Relevant_For_Internal_Reflection_Buffer(
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      const F360_Internal_Reflections_Calib_T &int_ref_calib
   );

   int32_t Find_Buffer_Slot_Matching_Detection(
      const Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE],
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      const F360_Internal_Reflections_Calib_T &int_ref_calib
   );

   bool Does_Det_Match_Buffer_Slot(
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      const Internal_Reflection_Buffer_Slot &buffer_slot,
      const F360_Internal_Reflections_Calib_T &int_ref_calib
   );

   void Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(
      const F360_Internal_Reflections_Calib_T &int_ref_calib,
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      Internal_Reflection_Buffer_Slot &buffer_slot
   );

   bool Classify_Internal_Detection_Buffer_Slot(
      const Internal_Reflection_Buffer_Slot &buffer_slot,
      const F360_Internal_Reflections_Calib_T &int_ref_calib
   );

   void Add_Det_To_Unused_Reflection_Buffer_Slot(
      const rspp_variant_A::RSPP_Detection_T &raw_det,
      Internal_Reflection_Buffer_Slot (&sensor_int_ref_buffer)[INTERNAL_REFLECTIONS_BUFFER_SIZE]
   );

}

#endif
