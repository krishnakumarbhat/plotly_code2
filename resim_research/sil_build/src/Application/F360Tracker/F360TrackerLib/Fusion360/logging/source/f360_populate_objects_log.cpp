/*===================================================================================*\
* FILE:  f360_populate_objects_log.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/

#include <cstring>
#include <algorithm>
#include "f360_iterator.h"
#include "f360_populate_objects_log.h"
#include "f360_norm_heading_angle.h"
#include "f360_calculate_priority.h"
#include "f360_reference_point_support_functions.h"

namespace f360_variant_A
{
   void Populate_Objects_Data(F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS], F360_Tracker_Info_T&tracker_info,
      const F360_Object_Log_T(&objects_log)[NUMBER_OF_OBJECT_TRACKS], const F360_Host_T& host)
   {
      // Clear id lists
      std::fill(cmn::begin(tracker_info.active_obj_ids), cmn::end(tracker_info.active_obj_ids), 0);
      std::fill(cmn::begin(tracker_info.inactive_obj_ids), cmn::end(tracker_info.inactive_obj_ids), 0);
      std::fill(cmn::begin(tracker_info.reduced_active_obj_ids), cmn::end(tracker_info.reduced_active_obj_ids), 0);
      std::fill(cmn::begin(tracker_info.reduced_inactive_obj_ids), cmn::end(tracker_info.reduced_inactive_obj_ids), 0);
      std::fill(cmn::begin(tracker_info.reduced_obj_ids), cmn::end(tracker_info.reduced_obj_ids), 0);

      tracker_info.num_active_objs = 0;
      tracker_info.num_unique_objs = 0U;
      tracker_info.reduced_num_active_objs = 0;
      uint32_t unique_objs[NUMBER_OF_OBJECT_TRACKS]{};
      std::fill(cmn::begin(unique_objs), cmn::end(unique_objs), 0U);
      bool reduced_inactive_obj_idx[NUMBER_OF_REDUCED_OBJECT_TRACKS]{};
      std::fill(cmn::begin(reduced_inactive_obj_idx), cmn::end(reduced_inactive_obj_idx), true);
      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         if ((0U < objects_log[i].status) && (0U < objects_log[i].trkID))
         {
            const uint16_t obj_idx = objects_log[i].trkID - 1U;

            /* VCS States */
            objects[obj_idx].vcs_position.x = objects_log[i].vcs_xposn;
            objects[obj_idx].vcs_velocity.longitudinal = objects_log[i].vcs_xvel;
            objects[obj_idx].vcs_accel.longitudinal = objects_log[i].vcs_xaccel;
            objects[obj_idx].vcs_position.y = objects_log[i].vcs_yposn;
            objects[obj_idx].vcs_velocity.lateral = objects_log[i].vcs_yvel;
            objects[obj_idx].vcs_accel.lateral = objects_log[i].vcs_yaccel;
            objects[obj_idx].vcs_heading = Angle{ objects_log[i].vcs_heading };
            objects[obj_idx].Set_Bbox_Orientation(Angle{ objects_log[i].vcs_pointing });
            objects[obj_idx].speed = objects_log[i].speed;
            objects[obj_idx].tang_accel = objects_log[i].tang_accel;
            objects[obj_idx].curvature = objects_log[i].curvature;
            objects[obj_idx].reference_point = static_cast<F360_Reference_Point_T>(objects_log[i].reference_point);
            objects[obj_idx].length_processed = objects_log[i].len1 + objects_log[i].len2;
            objects[obj_idx].width_processed = objects_log[i].wid1 + objects_log[i].wid2;
            

            /* Status & Time */
            objects[obj_idx].id = static_cast<int32_t>(objects_log[i].trkID);
            objects[obj_idx].unique_id = objects_log[i].unique_id;
            objects[obj_idx].status = static_cast<F360_Object_Status_T>(objects_log[i].status);
            objects[obj_idx].reduced_status = static_cast<F360_Object_Status_T>(objects_log[i].reducedStatus);
            objects[obj_idx].init_scheme = static_cast<F360_Track_Init_T>(objects_log[i].init_scheme);
            objects[obj_idx].trk_fltr_type = static_cast<F360_Trk_Fltr_Type_T>(objects_log[i].trk_fltr_type);
            objects[obj_idx].object_class = static_cast<F360_Object_Class_T>(objects_log[i].object_class);
            objects[obj_idx].reduced_id = static_cast<int32_t>(objects_log[i].reducedID);
            objects[obj_idx].confidenceLevel = objects_log[i].confidenceLevel;
            objects[obj_idx].exist_prob = objects_log[i].existence_probability;
            objects[obj_idx].otg_height = objects_log[i].otg_height; // Will be overwritten if internals are available as otg_height depends on additional signals in internals

            objects[obj_idx].probability_pedestrian = objects_log[i].probability_pedestrian;
            objects[obj_idx].probability_car = objects_log[i].probability_car;
            objects[obj_idx].probability_motorcycle = objects_log[i].probability_motorcycle;
            objects[obj_idx].probability_bicycle = objects_log[i].probability_bicycle;
            objects[obj_idx].probability_truck = objects_log[i].probability_truck;
            objects[obj_idx].probability_undet = objects_log[i].probability_undet;
            objects[obj_idx].probability_underdrivable_ocg = objects_log[i].probability_underdrivable_ocg;
            
            objects[obj_idx].time_since_measurement = objects_log[i].time_since_measurement;
            objects[obj_idx].time_since_cluster_created = objects_log[i].time_since_cluster_created;
            objects[obj_idx].time_since_track_updated = objects_log[i].time_since_track_updated;
            objects[obj_idx].time_since_stage_start = objects_log[i].time_since_stage_start;
            objects[obj_idx].time_since_initialization = objects_log[i].time_since_cluster_created; // Proper value is logged in internal_object

            objects[obj_idx].obstacle_prob = static_cast<float32_t>(objects_log[i].obstacle_prob) / 100.0F;
            objects[obj_idx].aeb_confidence = static_cast<F360_AEB_Confidence_T>(objects_log[i].aeb_confidence);
            
            // Mapping SG and OCG drivable status to objects
            objects[obj_idx].drivable_confidence_sg = objects_log[i].drivable_confidence_sg;
            objects[obj_idx].underdrivable_status_ocg = static_cast<ocg::OCG_Underdrivable_Status_T>(objects_log[i].underdrivable_status_ocg);
            objects[obj_idx].drivable_status_sg = static_cast<sg::SG_Drivability_Class_T>(objects_log[i].drivable_status_sg);

            
            // Mapping CCV to CCA
            if (F360_TRACKER_TRKFLTR_CCV == objects_log[i].trk_fltr_type)
            {
               objects[obj_idx].errcov[0][0] = objects_log[i].state_variance[0];
               objects[obj_idx].errcov[1][1] = objects_log[i].state_variance[1];
               objects[obj_idx].errcov[2][2] = 1.0F;
               objects[obj_idx].errcov[3][3] = objects_log[i].state_variance[2];
               objects[obj_idx].errcov[4][4] = objects_log[i].state_variance[3];
               objects[obj_idx].errcov[5][5] = 1.0F;

               objects[obj_idx].errcov[0][3] = objects_log[i].supplemental_state_covariance[0];
               objects[obj_idx].errcov[3][0] = objects_log[i].supplemental_state_covariance[0];
               objects[obj_idx].errcov[1][4] = objects_log[i].supplemental_state_covariance[1];
               objects[obj_idx].errcov[4][1] = objects_log[i].supplemental_state_covariance[1];
               objects[obj_idx].errcov[5][2] = 0.0F;
               objects[obj_idx].errcov[2][5] = 0.0F;

            }
            else
            {
               for (uint8_t j = 0U; j < STATE_DIMENSION; j++)
               {
                  objects[obj_idx].errcov[j][j] = objects_log[i].state_variance[j];
               }
               if (F360_TRACKER_TRKFLTR_CTCA == objects_log[i].trk_fltr_type)
               {
                  objects[obj_idx].errcov[0][1] = objects_log[i].supplemental_state_covariance[0];
                  objects[obj_idx].errcov[1][0] = objects_log[i].supplemental_state_covariance[0];
                  objects[obj_idx].errcov[2][4] = objects_log[i].supplemental_state_covariance[1];
                  objects[obj_idx].errcov[4][2] = objects_log[i].supplemental_state_covariance[1];
                  objects[obj_idx].errcov[3][5] = objects_log[i].supplemental_state_covariance[2];
                  objects[obj_idx].errcov[5][3] = objects_log[i].supplemental_state_covariance[2];
               }
               else if (F360_TRACKER_TRKFLTR_CCA == objects_log[i].trk_fltr_type)
               {
                  objects[obj_idx].errcov[0][3] = objects_log[i].supplemental_state_covariance[0]; // px w py
                  objects[obj_idx].errcov[3][0] = objects_log[i].supplemental_state_covariance[0]; 
                  objects[obj_idx].errcov[1][4] = objects_log[i].supplemental_state_covariance[1]; // vx w vy
                  objects[obj_idx].errcov[4][1] = objects_log[i].supplemental_state_covariance[1];
                  objects[obj_idx].errcov[2][5] = objects_log[i].supplemental_state_covariance[2]; // ax w ay
                  objects[obj_idx].errcov[5][2] = objects_log[i].supplemental_state_covariance[2];
               }
               else
               {
                  //CCV was handled already 
               }
            }


            

            /* Det info */
            objects[obj_idx].ndets = objects_log[i].ndets;
            objects[obj_idx].num_rr_inlier_dets = objects_log[i].num_rr_inlier_dets;
            objects[obj_idx].num_dets_used_in_rr_msmt_update = objects_log[i].num_dets_used_in_rr_msmt_update;

            /* Flags */
            objects[obj_idx].f_moving = (1U == objects_log[i].f_moving);
            objects[obj_idx].f_moveable = (1U == objects_log[i].f_moveable);
            objects[obj_idx].movable_prob = objects_log[i].movable_prob;
            objects[obj_idx].f_oncoming = (1U == objects_log[i].f_oncoming);
            objects[obj_idx].f_vehicular_trk = (1U == objects_log[i].f_vehicular_trk);
            
            objects[obj_idx].hdg_ptng_disagmt = Normalize_Heading_Angle(objects_log[i].vcs_pointing - objects_log[i].vcs_heading, 0.0F); // Will be overwritten if internals are available
            
            /* Calculate the object priority */
            objects[obj_idx].priority = Calculate_Priority(host, objects[obj_idx].movable_prob, objects[obj_idx].confidenceLevel,
               objects[obj_idx].vcs_position.x, objects[obj_idx].vcs_position.y);


            /* Compute object yaw rate from speed and curvature */
            const bool is_ccv_or_stationary_cca_object = ((F360_TRACKER_TRKFLTR_CCV == objects[obj_idx].trk_fltr_type) ||
               ((F360_TRACKER_TRKFLTR_CCA == objects[obj_idx].trk_fltr_type) && (!objects[obj_idx].f_moving)));
            if (is_ccv_or_stationary_cca_object)
            {
               objects[obj_idx].heading_rate = 0.0F;
            }
            else // CTCA or moving CCA object
            {
               objects[obj_idx].heading_rate = objects[obj_idx].speed * objects[obj_idx].curvature;
            }

            tracker_info.active_obj_ids[tracker_info.num_active_objs] = static_cast<int32_t>(obj_idx) + 1;
            tracker_info.num_active_objs++;

            // Check unique_id
            bool f_unique_id_found = false;
            for (uint32_t j = 0U; j < tracker_info.num_unique_objs; j++)
            {
               f_unique_id_found = f_unique_id_found || (objects[obj_idx].unique_id == unique_objs[j]);
            }
            if (!f_unique_id_found)
            {
               unique_objs[tracker_info.num_unique_objs] = objects[obj_idx].unique_id;
               tracker_info.num_unique_objs++;
            }

            // Reduced objects
            if (F360_OBJECT_STATUS_INVALID != objects[obj_idx].reduced_status)
            {
               tracker_info.reduced_active_obj_ids[tracker_info.reduced_num_active_objs] = objects[obj_idx].reduced_id;
               tracker_info.reduced_obj_ids[tracker_info.reduced_num_active_objs] = objects[obj_idx].id;
               tracker_info.reduced_num_active_objs++;
               reduced_inactive_obj_idx[objects[obj_idx].reduced_id - 1] = false;
            }
         }
      }

      int32_t n_inactive_objects = 0;
      for (int32_t i = 0; i < static_cast<int32_t>(NUMBER_OF_OBJECT_TRACKS); i++)
      {
         if (F360_OBJECT_STATUS_INVALID == objects[i].status)
         {
            tracker_info.inactive_obj_ids[n_inactive_objects] = i + 1;
            n_inactive_objects++;
         }
      }

      int32_t n_inactive_reduced_objects = 0;
      for (int32_t i = 0; i < static_cast<int32_t>(NUMBER_OF_REDUCED_OBJECT_TRACKS); i++)
      {
         if (reduced_inactive_obj_idx[i])
         {
            tracker_info.reduced_inactive_obj_ids[n_inactive_reduced_objects] = i + 1;
            n_inactive_reduced_objects++;
         }
      }
   }

   void Populate_Objects_Log_Data(F360_Object_Log_T(&objects_log)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS], const int32_t num_active_objects,
      const int32_t(&active_obj_ids)[NUMBER_OF_OBJECT_TRACKS])
   {
      // Pack the objects in order of increasing ID
      for (int32_t i = 0; i < num_active_objects; i++)
      {
         const int32_t obj_idx = active_obj_ids[i] - 1;

         objects_log[i].unique_id = objects[obj_idx].unique_id;
         objects_log[i].trkID = static_cast<uint16_t>(objects[obj_idx].id);
         objects_log[i].reducedID = static_cast<uint16_t>(objects[obj_idx].reduced_id);
         objects_log[i].trk_fltr_type = static_cast<uint8_t>(objects[obj_idx].trk_fltr_type);
         objects_log[i].vcs_xposn = objects[obj_idx].vcs_position.x;
         objects_log[i].vcs_yposn = objects[obj_idx].vcs_position.y;
         objects_log[i].vcs_xvel = objects[obj_idx].vcs_velocity.longitudinal;
         objects_log[i].vcs_yvel = objects[obj_idx].vcs_velocity.lateral;
         objects_log[i].vcs_xaccel = objects[obj_idx].vcs_accel.longitudinal;
         objects_log[i].vcs_yaccel = objects[obj_idx].vcs_accel.lateral;
         objects_log[i].vcs_heading = objects[obj_idx].vcs_heading.Value();
         objects_log[i].vcs_pointing = objects[obj_idx].bbox.Get_Orientation().Value();
         objects_log[i].speed = objects[obj_idx].speed;
         objects_log[i].curvature = objects[obj_idx].curvature;
         objects_log[i].tang_accel = objects[obj_idx].tang_accel;
         objects_log[i].existence_probability = objects[obj_idx].exist_prob;
         objects_log[i].otg_height = objects[obj_idx].otg_height;
         for (uint8_t j = 0U; j < STATE_DIMENSION; j++)
         {
            objects_log[i].state_variance[j] = objects[obj_idx].errcov[j][j];
         }
         if (F360_TRACKER_TRKFLTR_CTCA == objects[obj_idx].trk_fltr_type)
         {
            objects_log[i].supplemental_state_covariance[0] = objects[obj_idx].errcov[0][1];
            objects_log[i].supplemental_state_covariance[1] = objects[obj_idx].errcov[2][4];
            objects_log[i].supplemental_state_covariance[2] = objects[obj_idx].errcov[3][5];
         }
         else if (F360_TRACKER_TRKFLTR_CCA == objects[obj_idx].trk_fltr_type)
         {
            objects_log[i].supplemental_state_covariance[0] = objects[obj_idx].errcov[0][3]; // px w py
            objects_log[i].supplemental_state_covariance[1] = objects[obj_idx].errcov[1][4]; // vx w vy
            objects_log[i].supplemental_state_covariance[2] = objects[obj_idx].errcov[2][5]; // ax w ay
         }
         else
         {
            objects_log[i].supplemental_state_covariance[0] = objects[obj_idx].errcov[0][2];
            objects_log[i].supplemental_state_covariance[1] = objects[obj_idx].errcov[1][3];
            objects_log[i].supplemental_state_covariance[2] = objects[obj_idx].errcov[4][5];
         }
         objects_log[i].confidenceLevel = objects[obj_idx].confidenceLevel;
         objects_log[i].time_since_measurement = objects[obj_idx].time_since_measurement;
         objects_log[i].time_since_cluster_created = objects[obj_idx].time_since_cluster_created;
         objects_log[i].time_since_track_updated = objects[obj_idx].time_since_track_updated;
         objects_log[i].time_since_stage_start = objects[obj_idx].time_since_stage_start;
         objects_log[i].radar_cross_section = objects[obj_idx].average_rcs;
         float32_t len1;
         float32_t len2;
         float32_t wid1;
         float32_t wid2;
         Get_Distances_From_Ref_Point_To_Obj_Sides(objects[obj_idx], len1, len2, wid1, wid2);
         objects_log[i].len1 = len1;
         objects_log[i].len2 = len2;
         objects_log[i].wid1 = wid1;
         objects_log[i].wid2 = wid2;
         objects_log[i].accuracy_length = 0.0F;
         objects_log[i].accuracy_width = 0.0F;
         objects_log[i].probability_pedestrian = objects[obj_idx].probability_pedestrian;
         objects_log[i].probability_car = objects[obj_idx].probability_car;
         objects_log[i].probability_motorcycle = objects[obj_idx].probability_motorcycle;
         objects_log[i].probability_bicycle = objects[obj_idx].probability_bicycle;
         objects_log[i].probability_truck = objects[obj_idx].probability_truck;
         objects_log[i].probability_undet = objects[obj_idx].probability_undet;
         objects_log[i].probability_underdrivable_ocg = objects[obj_idx].probability_underdrivable_ocg;
         objects_log[i].ndets = static_cast<uint16_t>(objects[obj_idx].ndets);
         objects_log[i].num_rr_inlier_dets = static_cast<uint16_t>(objects[obj_idx].num_rr_inlier_dets);
         objects_log[i].num_dets_used_in_rr_msmt_update = static_cast<uint16_t>(objects[obj_idx].num_dets_used_in_rr_msmt_update);
         objects_log[i].reference_point = static_cast<uint8_t>(objects[obj_idx].reference_point);
         objects_log[i].status = static_cast<uint8_t>(objects[obj_idx].status);
         objects_log[i].reducedStatus = static_cast<uint8_t>(objects[obj_idx].reduced_status);
         objects_log[i].init_scheme = static_cast<int8_t>(objects[obj_idx].init_scheme);
         objects_log[i].object_class = static_cast<uint8_t>(objects[obj_idx].object_class);
         objects_log[i].f_crossing = 0U;
         objects_log[i].f_moving = objects[obj_idx].f_moving ? 1U : 0U;
         objects_log[i].f_moveable = objects[obj_idx].f_moveable ? 1U : 0U;
         objects_log[i].movable_prob = objects[obj_idx].movable_prob;
         objects_log[i].f_oncoming = objects[obj_idx].f_oncoming ? 1U : 0U;
         objects_log[i].f_vehicular_trk = objects[obj_idx].f_vehicular_trk ? 1U : 0U;
         objects_log[i].f_onguardrail = (F360_INVALID_UNSIGNED_ID == objects[obj_idx].on_sep_id) ? 0U : 1U;
         objects_log[i].f_fast_moving = 0U;
         objects_log[i].underdrivable_status_ocg = static_cast<uint8_t>(objects[obj_idx].underdrivable_status_ocg);
         objects_log[i].drivable_status_sg = static_cast<uint8_t>(objects[obj_idx].drivable_status_sg);
         objects_log[i].drivable_confidence_sg = objects[obj_idx].drivable_confidence_sg;
         objects_log[i].current_msmt_type = 0U;
         objects_log[i].obstacle_prob = static_cast<uint8_t>(std::round(100.0F * objects[obj_idx].obstacle_prob));
         objects_log[i].aeb_confidence = static_cast<uint8_t>(objects[obj_idx].aeb_confidence);
      }
   }
}
