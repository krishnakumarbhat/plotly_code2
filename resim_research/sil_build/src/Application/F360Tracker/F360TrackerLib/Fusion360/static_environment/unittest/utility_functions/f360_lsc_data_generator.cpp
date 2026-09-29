/*===========================================================================*\
* FILE: f360_lsc_data_generator.cpp
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


#include "f360_lsc_data_generator.h"
#include "f360_sorted_tracks_mgmt.h"

namespace f360_variant_A
{
   void Create_Four_SEP(
      Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS])
   {
      sep[0].poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;
      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].age = 10U;
      sep[0].confidence = 1.0F;
      sep[0].lower_limit = -30.0F;
      sep[0].upper_limit = 30.0F;
      sep[0].p2 = 0.0F;
      sep[0].p1 = 0.0F;
      sep[0].p0 = -5.0F;

      sep[1].poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;
      sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[1].age = 10U;
      sep[1].confidence = 1.0F;
      sep[1].lower_limit = -30.0F;
      sep[1].upper_limit = 30.0F;
      sep[1].p2 = 0.001F;
      sep[1].p1 = 0.1F;
      sep[1].p0 = -10.0F;

      sep[2].poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;
      sep[2].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[2].age = 10U;
      sep[2].confidence = 1.0F;
      sep[2].lower_limit = -30.0F;
      sep[2].upper_limit = 30.0F;
      sep[2].p2 = -0.001F;
      sep[2].p1 = -0.01F;
      sep[2].p0 = 5.0F;

      sep[3].poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;
      sep[3].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[3].age = 10U;
      sep[3].confidence = 1.0F;
      sep[3].lower_limit = -30.0F;
      sep[3].upper_limit = 30.0F;
      sep[3].p2 = 0.0F;
      sep[3].p1 = 0.0F;
      sep[3].p0 = 10.0F;
   }

   void Create_Four_LSC(
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      // LSC to the left of host, straight line
      longi_stat_curves[0].f_valid = true;
      longi_stat_curves[0].a = 0.0F;
      longi_stat_curves[0].b = 0.0F;
      longi_stat_curves[0].c = -5.0F;
      longi_stat_curves[0].x_min = -30.0F;
      longi_stat_curves[0].x_max = 30.0F;
      longi_stat_curves[0].mean_lat_pos = -5.0F;

      // LSC to the left of first LSC to the left of host, curved line
      longi_stat_curves[1].f_valid = true;
      longi_stat_curves[1].a = 0.001F;
      longi_stat_curves[1].b = 0.1F;
      longi_stat_curves[1].c = -10.0F;
      longi_stat_curves[1].x_min = -30.0F;
      longi_stat_curves[1].x_max = 30.0F;
      longi_stat_curves[1].mean_lat_pos = -10.0F;

      // LSC to the right of host, curved line
      longi_stat_curves[2].f_valid = true;
      longi_stat_curves[2].a = -0.001F;
      longi_stat_curves[2].b = -0.01F;
      longi_stat_curves[2].c = 5.0F;
      longi_stat_curves[2].x_min = -30.0F;
      longi_stat_curves[2].x_max = 30.0F;
      longi_stat_curves[2].mean_lat_pos = 5.0F;

      // LSC to the right of first LSC to the right of host, straight line
      longi_stat_curves[3].f_valid = true;
      longi_stat_curves[3].a = 0.0F;
      longi_stat_curves[3].b = 0.0F;
      longi_stat_curves[3].c = 10.0F;
      longi_stat_curves[3].x_min = -30.0F;
      longi_stat_curves[3].x_max = 30.0F;
      longi_stat_curves[3].mean_lat_pos = 10.0F;
   }

   void Initialize_Tracker_Info(
      F360_Tracker_Info_T& tracker_info)
   {
      tracker_info.num_active_objs = 0;

      tracker_info.vcslong_sorted_start = NULL; // This value must be initialized with NULL when object list is empty
      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = NULL;
         tracker_info.vcslong_sorted_prev_track[i] = NULL;
      }
   }

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_A(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      F360_LSC_Object_Group_Settings_T settings;

      settings.nr_objects = 30U;
      settings.min_long_pos = -30.0F;
      settings.delta_long_pos = 2.0F;
      settings.lat_pos = -5.0F;

      Add_LSC_Group(settings, tracker_info, objects);

      return settings;
   }

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_A_modified(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      F360_LSC_Object_Group_Settings_T settings;

      settings.nr_objects = 30U;
      settings.min_long_pos = -30.0F;
      settings.delta_long_pos = 2.0F;
      settings.lat_pos = 0.0F;

      Add_LSC_Group(settings, tracker_info, objects);

      return settings;
   }

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_B(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      F360_LSC_Object_Group_Settings_T settings;

      settings.nr_objects = 10U;
      settings.min_long_pos = 5.0F;
      settings.delta_long_pos = 2.0F;
      settings.lat_pos = 5.0F;

      Add_LSC_Group(settings, tracker_info, objects);

      return settings;
   }

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_C(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      F360_LSC_Object_Group_Settings_T settings;

      settings.nr_objects = 2U;
      settings.min_long_pos = 10.0F;
      settings.delta_long_pos = 2.0F;
      settings.lat_pos = 10.0F;

      Add_LSC_Group(settings, tracker_info, objects);

      return settings;
   }

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_D(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      F360_LSC_Object_Group_Settings_T settings;

      settings.nr_objects = 1U;
      settings.min_long_pos = 7.5F;
      settings.delta_long_pos = 2.0F;
      settings.lat_pos = 15.0F;

      Add_LSC_Group(settings, tracker_info, objects);

      return settings;
   }

   F360_LSC_Object_Group_Settings_T Add_LSC_Group_E(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      F360_LSC_Object_Group_Settings_T settings;

      settings.nr_objects = 16U;
      settings.min_long_pos = -30.0F;
      settings.delta_long_pos = 1.0F;
      settings.lat_pos = -5.0F;

      Add_LSC_Group(settings, tracker_info, objects);

      return settings;
   }

   void Add_LSC_Group(
      F360_LSC_Object_Group_Settings_T& settings,
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      int32_t num_new_objects = tracker_info.num_active_objs + settings.nr_objects;
      int32_t num_old_objects = tracker_info.num_active_objs;

      for (int32_t i = tracker_info.num_active_objs; i < num_new_objects; i++)
      {
         objects[i].id = i + 1;
         objects[i].vcs_position.x = settings.min_long_pos + static_cast<float32_t>(i - num_old_objects) * settings.delta_long_pos;
         objects[i].vcs_position.y = settings.lat_pos;
         objects[i].reference_point = F360_REFERENCE_POINT_CENTER;
         Point center = objects[i].vcs_position;
         objects[i].bbox.Set_Center(center);

         objects[i].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

         tracker_info.active_obj_ids[i] = objects[i].id;
         tracker_info.num_active_objs++;

         Sorted_Tracks_Insert(tracker_info, &objects[i]);

         // Fill setting struct with relavant object ids
         settings.ids[i - num_old_objects] = objects[i].id;
      }
   }
}
