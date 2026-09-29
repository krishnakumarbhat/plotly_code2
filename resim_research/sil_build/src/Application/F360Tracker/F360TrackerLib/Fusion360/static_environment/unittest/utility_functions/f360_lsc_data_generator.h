/*===========================================================================*\
* FILE: f360_lsc_data_generator.h
*============================================================================
* Copyright ? 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential ? Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains utility functions for testing the LSC module
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [30-Mar-2018]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/
#ifndef F360_LSC_DATA_GENERATOR_H
#define F360_LSC_DATA_GENERATOR_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_longi_stat_curve.h"
#include "f360_static_env_poly_types.h"

namespace f360_variant_A
{
   typedef struct F360_LSC_Object_Group_Settings_Tag
   {
      uint32_t nr_objects;
      float32_t min_long_pos;
      float32_t delta_long_pos;
      float32_t lat_pos;

      int32_t ids[NUMBER_OF_OBJECT_TRACKS];
   } F360_LSC_Object_Group_Settings_T;

   void Create_Four_SEP(
      Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   );

   void Create_Four_LSC(
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );

   void Initialize_Tracker_Info(
      F360_Tracker_Info_T& tracker_info
   );

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_A(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_A_modified(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_B(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_C(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_D(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_E(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Add_LSC_Group(
      F360_LSC_Object_Group_Settings_T& settings,
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   );

}
#endif
