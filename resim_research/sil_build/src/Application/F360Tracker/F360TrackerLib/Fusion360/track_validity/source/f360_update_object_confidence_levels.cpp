/*===================================================================================*\
* FILE: f360_update_object_confidence_levels.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Update_Object_Confidence_Levels() function
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_math.h"
#include "f360_update_object_confidence_levels.h"
#include "f360_math_func.h"
#include "f360_update_object_confidence_levels_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Object_Confidence_Levels
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info - tracker info structure
   * const F360_Calibrations_T& calib - tracker calib
   * F360_Object_Track_T& object - analysed object
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
   * Function updates track confidenceLevel and raw_confidence_level
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Object_Confidence_Levels(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object)
   {
      if ((F360_OBJECT_STATUS_COASTED == object.status) ||
         (F360_OBJECT_STATUS_UPDATED == object.status))
      {
         Calc_Average_Confidence_Level(tracker_info.elapsed_time_s, object);

         const float32_t filter_coef = Determine_Filter_Coef(object, tracker_info.elapsed_time_s, calib);
         const float32_t raw_confidence_level = Determine_Raw_Confidence_Level(object, calib);

         object.confidenceLevel = (filter_coef * object.confidenceLevel) + ((1.0F - filter_coef) * raw_confidence_level);
         object.raw_confidence_level = raw_confidence_level; // saved with a track for use in EP filter
      }


      Reduce_Confidence_Based_On_Object_Flags(calib.k_conf_overlapping_reduction_factor, object);
   }
}

