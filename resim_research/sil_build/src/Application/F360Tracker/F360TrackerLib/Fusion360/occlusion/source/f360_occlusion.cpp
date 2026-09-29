/*===================================================================================*\
* FILE:  f360_occlusion.cpp
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*/

#include "f360_occlusion.h"
#include "f360_intersections.h"
#include "f360_norm_heading_angle.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   static void Update_Sensor_Occlusion(
      const F360_Object_Track_T& object,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS]);

   static void Get_Object_VCS_Info(
      const F360_Object_Track_T& object,
      float32_t(&corner_vcs_latpos)[4],
      float32_t(&corner_vcs_longpos)[4]);

   static void Get_Object_Side_Normals(
      const F360_Object_Track_T& object,
      float32_t(&side_normal)[4][2]);

   static void Get_SCS_Data(
      const F360_Radar_Sensor_T& sensor,
      const float32_t(&side_normal)[4][2],
      const float32_t(&corner_vcs_latpos)[4],
      const float32_t(&corner_vcs_longpos)[4],
      bool(&visible_side)[4],
      float32_t& max_az,
      float32_t& min_az,
      float32_t& min_range);

   /*===========================================================================*\
   * FUNCTION: Update_Occlusion_Data()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Estimate free sight accross FoV from the perspective of each sensor.
   \*===========================================================================*/
   void Update_Occlusion_Data(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      // Reset occlusion info
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         if (sensors[i].variable.is_valid)
         {
            int8_t look_id = static_cast<int8_t>(sensors[i].variable.look_id);
            if ((look_id < 0) || (look_id > 3))
            {
               // ensure lookid is always a valid value
               look_id = 0;
            }
            const float32_t k_range_buffer = 50.0F;
            const float32_t max_range = sensors[i].constant.range_limits[look_id] + k_range_buffer;
            const float32_t max_az = sensors[i].constant.fov_max_az_rad[look_id] + sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle;
            const float32_t min_az = sensors[i].constant.fov_min_az_rad[look_id] + sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle;
            occlusion_data[i].min_vcs_az = min_az;
            occlusion_data[i].max_range = max_range;
            occlusion_data[i].sector_size = (max_az - min_az) / static_cast<float32_t>(num_occlusion_sectors);

            for (int16_t j = 0; j < num_occlusion_sectors; j++)
            {
               occlusion_data[i].range[j] = max_range;
               occlusion_data[i].occluding_id[j] = 0;
            }
         }
      }

      // Update Occlusion status
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const float32_t k_min_confidence_level = 0.9F;
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         const F360_Object_Track_T& object = object_tracks[obj_idx];

         if ((object.behind_sep_id == 0U) && (object.on_sep_id == 0U) && (object.confidenceLevel > k_min_confidence_level))
         {
            const float32_t k_max_longpos = 100.0F;
            const float32_t k_min_longpos = -50.0F;
            const float32_t k_max_latpos = 15.0F;
            const float32_t k_min_latpos = -15.0F;

            const float32_t obj_vcs_longpos = object.vcs_position.x;
            const float32_t obj_vcs_latpos = object.vcs_position.y;

            const bool f_relevant_target = (object.movable_prob > 0.5F) &&
               (object.reference_point != F360_REFERENCE_POINT_CENTER) &&
               (obj_vcs_longpos < k_max_longpos) && (obj_vcs_longpos > k_min_longpos) &&
               (obj_vcs_latpos < k_max_latpos) && (obj_vcs_latpos > k_min_latpos);

            if (f_relevant_target)
            {
               Update_Sensor_Occlusion(object, sensors, occlusion_data);
            }
         }
      }

      timing_info.occlusion = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Update_Sensor_Occlusion()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Estimate object extents and update corresponding sectors
   \*===========================================================================*/
   static void Update_Sensor_Occlusion(
      const F360_Object_Track_T& object,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS])
   {
      // Corners: FL, RL, RR, FR
      float32_t corner_vcs_longpos[4];
      float32_t corner_vcs_latpos[4];
      // Sides: Front, Left, Rear, Right
      float32_t side_normal[4][2];

      Get_Object_VCS_Info(object, corner_vcs_latpos, corner_vcs_longpos);
      Get_Object_Side_Normals(object, side_normal);

      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         if (sensors[i].variable.is_valid)
         {
            bool side_visible[4];
            float32_t max_az = -10.0F;
            float32_t min_az = 10.0F;
            float32_t min_range;
            Get_SCS_Data(sensors[i], side_normal, corner_vcs_latpos, corner_vcs_longpos, side_visible, max_az, min_az, min_range);

            const float32_t first_corner_az = min_az;
            const float32_t last_corner_az = max_az;

            const float32_t inv_sector_size = 1.0F / occlusion_data[i].sector_size;
            const float32_t first_sector_val = (first_corner_az - occlusion_data[i].min_vcs_az) * inv_sector_size;
            const float32_t last_sector_val = (last_corner_az - occlusion_data[i].min_vcs_az) * inv_sector_size;

            // casting to int truncates towards zero: 0.9 -> 0, -0.9 -> 0, so negative values need to be reduced by one
            int32_t first_sector = static_cast<int32_t>(first_sector_val);
            int32_t last_sector = static_cast<int32_t>(last_sector_val);
            if (first_sector_val < 0.0F)
            {
               first_sector--;
            }
            if (last_sector_val < 0.0F)
            {
               last_sector--;
            }

            const bool f_overlap = (last_sector >= 0) && (first_sector < num_occlusion_sectors) && (first_sector <= last_sector) && ((max_az - min_az) < F360_PI);
            if (f_overlap)
            {
               first_sector = std::max(0, std::min(num_occlusion_sectors - 1, first_sector));
               last_sector = std::max(0, std::min(num_occlusion_sectors - 1, last_sector));

               const float32_t k_simplified_occlusion_range_threshold = 50.0F;
               if ((min_range > k_simplified_occlusion_range_threshold) || (first_sector == last_sector))
               {
                  // In the simplified case, let all sectors have the same range
                  for (int32_t sector_idx = first_sector; sector_idx <= last_sector; sector_idx++)
                  {
                     if (min_range < occlusion_data[i].range[sector_idx])
                     {
                        occlusion_data[i].range[sector_idx] = min_range;
                        occlusion_data[i].occluding_id[sector_idx] = static_cast<int16_t>(object.id);
                     }
                  }
               }
               else
               {
                  // In the more accurate case, find the intersection of sector midpoints with the object sides to use as range
                  for (int32_t sector_idx = first_sector; sector_idx <= last_sector; sector_idx++)
                  {
                     const float32_t fsector_idx = static_cast<float32_t>(sector_idx);
                     const float32_t sector_mid_az = occlusion_data[i].min_vcs_az + (fsector_idx * occlusion_data[i].sector_size) + (occlusion_data[i].sector_size * 0.5F);

                     const float32_t sector_line_start[2] = { 0.0F , 0.0F };
                     const float32_t sector_line_end[2] = {
                        occlusion_data[i].max_range * F360_Cosf(sector_mid_az) ,
                        occlusion_data[i].max_range * F360_Sinf(sector_mid_az) };

                     for (int8_t side_idx = 0; side_idx < 4; side_idx++)
                     {
                        if (side_visible[side_idx])
                        {
                           const float32_t sensor_longpos = sensors[i].constant.mounting_position.vcs_position.longitudinal;
                           const float32_t sensor_latpos = sensors[i].constant.mounting_position.vcs_position.lateral;

                           // for side i, the connecting corners correspond to i and i-1
                           const int8_t c1 = side_idx;
                           const int8_t c2 = (c1 > 0) ? (c1 - 1) : 3;

                           const float32_t object_side_start[2] = { corner_vcs_longpos[c1] - sensor_longpos, corner_vcs_latpos[c1] - sensor_latpos };
                           const float32_t object_side_end[2] = { corner_vcs_longpos[c2] - sensor_longpos, corner_vcs_latpos[c2] - sensor_latpos };

                           float32_t x_intersect = 0.0F;
                           float32_t y_intersect = 0.0F;
                           const bool f_lines_are_intersecting = Determine_Segments_Intersection_Limited(sector_line_start, sector_line_end,
                              object_side_start, object_side_end, x_intersect, y_intersect);

                           if (f_lines_are_intersecting)
                           {
                              const float32_t intersection_range = hypotf(x_intersect, y_intersect);
                              if (intersection_range < occlusion_data[i].range[sector_idx])
                              {
                                 occlusion_data[i].range[sector_idx] = intersection_range;
                                 occlusion_data[i].occluding_id[sector_idx] = static_cast<int16_t>(object.id);
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Get_Object_Occlusion_Status()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Determine the occlusion status of an object track
   \*===========================================================================*/
   F360_Occlusion_Status_T Get_Object_Occlusion_Status(
      const F360_Object_Track_T& object,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS])
   {
      F360_Occlusion_Status_T new_occl_status_SEP = OCCLUSION_STATUS_VISIBLE;
      if (F360_INVALID_UNSIGNED_ID != object.behind_sep_id)
      {
         if (object.f_behind_sep_ambiguous)
         {
            new_occl_status_SEP = OCCLUSION_STATUS_ON_EDGE;
         }
         else
         {
            new_occl_status_SEP = OCCLUSION_STATUS_OCCLUDED;
         }
      }

      F360_Occlusion_Status_T new_occl_status_objs = OCCLUSION_STATUS_VISIBLE;
      if (new_occl_status_SEP != OCCLUSION_STATUS_OCCLUDED)
      {
         const float32_t dist_to_obj_sq = object.vcs_position.x * object.vcs_position.x + object.vcs_position.y * object.vcs_position.y;
         const F360_Occlusion_Status_T refpoint_occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, object.vcs_position.x, object.vcs_position.y, object.id);

         const float32_t k_min_dist_for_simple_check_sq = 50.0F * 50.0F;
         if ((object.reference_point == F360_REFERENCE_POINT_CENTER) ||
            ((dist_to_obj_sq > k_min_dist_for_simple_check_sq) && (refpoint_occlusion_status == OCCLUSION_STATUS_OCCLUDED)))
         {
            new_occl_status_objs = refpoint_occlusion_status;
         }
         else
         {
            float32_t corner_vcs_longpos[4];
            float32_t corner_vcs_latpos[4];
            int8_t num_corners_visible = 0;
            int8_t num_corners_outside_FOV = 0;
            Get_Object_VCS_Info(object, corner_vcs_latpos, corner_vcs_longpos);

            for (int32_t i = 0; i < 4; i++)
            {
               const F360_Occlusion_Status_T corner_occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, corner_vcs_longpos[i], corner_vcs_latpos[i], object.id);

               if (corner_occlusion_status == OCCLUSION_STATUS_VISIBLE)
               {
                  num_corners_visible++;
                  if (num_corners_visible >= 2)
                  {
                     break;
                  }
               }
               else if (corner_occlusion_status == OCCLUSION_STATUS_UNDEFINED)
               {
                  num_corners_outside_FOV++;
               }
               else
               {
                  // do nothing
               }
            }

            if ((num_corners_visible >= 2) ||
               (num_corners_outside_FOV == 4) ||
               ((num_corners_visible == 1) && (num_corners_outside_FOV == 3)))
            {
               // Set the occlusion status to visible if:
               // - at least two corners are visible
               // - all corners are outside the FOV
               // - only one corner is visible but the other three corners are outside the FOV
               new_occl_status_objs = OCCLUSION_STATUS_VISIBLE;
            }
            else if (num_corners_visible == 1)
            {
               // If only one corner is visible, set status to On Edge
               new_occl_status_objs = OCCLUSION_STATUS_ON_EDGE;
            }
            else
            {
               // No visible corners, and at least one corner in FOV, set status to Occluded
               new_occl_status_objs = OCCLUSION_STATUS_OCCLUDED;
            }
         }
      }

      return std::min(new_occl_status_objs, new_occl_status_SEP);
   }

   /*===========================================================================*\
   * FUNCTION: Get_Point_Occlusion_Status()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Find most optimistic visibility of a point with respect to all sensors.
   * Object ID is an optional input to ensure an object does not occlude itself.
   * Sensor ID is an optional input if a specific sensor should be checked.
   \*===========================================================================*/
   F360_Occlusion_Status_T Get_Point_Occlusion_Status(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      const float32_t vcs_longpos,
      const float32_t vcs_latpos,
      const int32_t obj_id,
      const int32_t sensor_id)
   {
      F360_Occlusion_Status_T result = OCCLUSION_STATUS_UNDEFINED;
      const int8_t num_sensors = static_cast<int8_t>(MAX_NUMBER_OF_SENSORS);

      for (int8_t i = 0; i < num_sensors; i++)
      {
         if (sensors[i].variable.is_valid && ((i == (sensor_id - 1)) || (sensor_id == -1)))
         {
            const float32_t scs_longpos = vcs_longpos - sensors[i].constant.mounting_position.vcs_position.longitudinal;
            const float32_t scs_latpos = vcs_latpos - sensors[i].constant.mounting_position.vcs_position.lateral;
            const float32_t az = F360_Atan2f(scs_latpos, scs_longpos);
            const float32_t normalized_az = Normalize_Heading_Angle(az, sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle);

            const float32_t sector_val = (normalized_az - occlusion_data[i].min_vcs_az) / occlusion_data[i].sector_size;

            // casting to int truncates towards zero: 0.9 -> 0, -0.9 -> 0, so negative values need to be reduced by one
            int32_t sector_idx = static_cast<int32_t>(sector_val);
            if (sector_val < 0.0F)
            {
               sector_idx--;
            }

            if ((sector_idx >= 0) && (sector_idx < num_occlusion_sectors))
            {
               int32_t sector_idx2 = sector_idx + 1;
               sector_idx2 = (sector_idx2 < num_occlusion_sectors) ? sector_idx2 : (num_occlusion_sectors - 1); // checking an extra sector to match original implementation

               const float32_t tested_range = hypotf(scs_latpos, scs_longpos) - 0.5F;
               if ((obj_id == occlusion_data[i].occluding_id[sector_idx]) || 
                  ((tested_range < occlusion_data[i].range[sector_idx]) && (tested_range < occlusion_data[i].range[sector_idx2])))
               {
                  result = OCCLUSION_STATUS_VISIBLE;
                  break;
               }
               else
               {
                  result = OCCLUSION_STATUS_OCCLUDED;
               }
            }
         }
      }
      return result;
   }

   /*===========================================================================*\
   * FUNCTION: Get_SCS_Data()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Calculate object properties relative to a sensor
   \*===========================================================================*/
   static void Get_SCS_Data(
      const F360_Radar_Sensor_T& sensor,
      const float32_t(&side_normal)[4][2],
      const float32_t(&corner_vcs_latpos)[4],
      const float32_t(&corner_vcs_longpos)[4],
      bool(&visible_side)[4],
      float32_t& max_az,
      float32_t& min_az,
      float32_t& min_range)
   {
      bool visible_corner[4] = {};
      float32_t scs_longpos[4];
      float32_t scs_latpos[4];
      for (int8_t i = 0; i < 4; i++)
      {
         scs_longpos[i] = corner_vcs_longpos[i] - sensor.constant.mounting_position.vcs_position.longitudinal;
         scs_latpos[i] = corner_vcs_latpos[i] - sensor.constant.mounting_position.vcs_position.lateral;
         const bool f_visible_side = scs_longpos[i] * side_normal[i][0] + scs_latpos[i] * side_normal[i][1] < 0.0F;

         // for side i, the connecting corners correspond to i and i-1
         const int8_t j = (i > 0) ? (i - 1) : 3;
         visible_corner[i] = visible_corner[i] || f_visible_side;
         visible_corner[j] = visible_corner[j] || f_visible_side;
         visible_side[i] = f_visible_side;
      }

      float32_t min_range_sq = 1.0e8F;
      for (int8_t i = 0; i < 4; i++)
      {
         if (visible_corner[i])
         {
            const float32_t az = F360_Atan2f(scs_latpos[i], scs_longpos[i]);
            const float32_t corner_range_sq = scs_latpos[i] * scs_latpos[i] + scs_longpos[i] * scs_longpos[i];
            const float32_t az_norm = Normalize_Heading_Angle(az, sensor.constant.mounting_position.vcs_boresight_azimuth_angle);
            min_range_sq = fminf(min_range_sq, corner_range_sq);
            min_az = fminf(min_az, az_norm);
            max_az = fmaxf(max_az, az_norm);
         }
      }
      min_range = F360_Sqrtf(min_range_sq);
   }

   /*===========================================================================*\
   * FUNCTION: Get_Object_VCS_Info()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Calculate VCS position of an objects corners.
   \*===========================================================================*/
   static void Get_Object_VCS_Info(
      const F360_Object_Track_T& object,
      float32_t(&corner_vcs_latpos)[4],
      float32_t(&corner_vcs_longpos)[4])
   {
      const float32_t width = object.bbox.Get_Width();
      const float32_t length = object.bbox.Get_Length();

      // Corners: FL, RL, RR, FR
      float32_t vcs_longpos[4];
      float32_t vcs_latpos[4];

      switch (object.reference_point)
      {
         case F360_REFERENCE_POINT_FRONT_LEFT:
         case F360_REFERENCE_POINT_LEFT:
         case F360_REFERENCE_POINT_REAR_LEFT:
         {
            vcs_latpos[0] = 0.0F;
            vcs_latpos[1] = 0.0F;
            vcs_latpos[2] = width;
            vcs_latpos[3] = width;
            break;
         }
         case F360_REFERENCE_POINT_FRONT_RIGHT:
         case F360_REFERENCE_POINT_RIGHT:
         case F360_REFERENCE_POINT_REAR_RIGHT:
         {
            vcs_latpos[0] = -width;
            vcs_latpos[1] = -width;
            vcs_latpos[2] = 0.0F;
            vcs_latpos[3] = 0.0F;
            break;
         }
         case F360_REFERENCE_POINT_FRONT:
         case F360_REFERENCE_POINT_CENTER:
         case F360_REFERENCE_POINT_REAR:
         default:
         {
            const float32_t half_width = width * 0.5F;
            vcs_latpos[0] = -half_width;
            vcs_latpos[1] = -half_width;
            vcs_latpos[2] = half_width;
            vcs_latpos[3] = half_width;
            break;
         }
      }

      switch (object.reference_point)
      {
         case F360_REFERENCE_POINT_FRONT_LEFT:
         case F360_REFERENCE_POINT_FRONT:
         case F360_REFERENCE_POINT_FRONT_RIGHT:
         {
            vcs_longpos[0] = 0.0F;
            vcs_longpos[1] = -length;
            vcs_longpos[2] = -length;
            vcs_longpos[3] = 0.0F;
            break;
         }
         case F360_REFERENCE_POINT_REAR_LEFT:
         case F360_REFERENCE_POINT_REAR:
         case F360_REFERENCE_POINT_REAR_RIGHT:
         {
            vcs_longpos[0] = length;
            vcs_longpos[1] = 0.0F;
            vcs_longpos[2] = 0.0F;
            vcs_longpos[3] = length;
            break;
         }
         case F360_REFERENCE_POINT_LEFT:
         case F360_REFERENCE_POINT_CENTER:
         case F360_REFERENCE_POINT_RIGHT:
         default:
         {
            const float32_t half_length = length * 0.5F;
            vcs_longpos[0] = half_length;
            vcs_longpos[1] = -half_length;
            vcs_longpos[2] = -half_length;
            vcs_longpos[3] = half_length;
            break;
         }
      }

      const float32_t obj_refpt_longpos = object.vcs_position.x;
      const float32_t obj_refpt_latpos = object.vcs_position.y;
      const float32_t cos_ptg = object.bbox.Get_Orientation().Cos();
      const float32_t sin_ptg = object.bbox.Get_Orientation().Sin();

      for (int8_t i = 0; i < 4; i++)
      {
         corner_vcs_longpos[i] = vcs_longpos[i] * cos_ptg + vcs_latpos[i] * -sin_ptg + obj_refpt_longpos;
         corner_vcs_latpos[i] = vcs_longpos[i] * sin_ptg + vcs_latpos[i] * cos_ptg + obj_refpt_latpos;
      }
   }

/*===========================================================================*\
* FUNCTION: Get_Object_VCS_Info()
* --------------------------------------------------------------------------
* ABSTRACT:
* Calculate normal vectors of an object's sides.
\*===========================================================================*/
   static void Get_Object_Side_Normals(
      const F360_Object_Track_T& object,
      float32_t(&side_normal)[4][2])
   {
      const float32_t cos_ptg = object.bbox.Get_Orientation().Cos();
      const float32_t sin_ptg = object.bbox.Get_Orientation().Sin();

      // Front
      side_normal[0][0] = cos_ptg;
      side_normal[0][1] = sin_ptg;

      // Left
      side_normal[1][0] = sin_ptg;
      side_normal[1][1] = -cos_ptg;

      // Rear
      side_normal[2][0] = -cos_ptg;
      side_normal[2][1] = -sin_ptg;

      // Right
      side_normal[3][0] = -sin_ptg;
      side_normal[3][1] = cos_ptg;
   }
}
