/** \file
 * This file implements shared support functions for safety handler unit tests
 */

#include "sh_unittest_support.h"
#include "sh_boundingbox_helper_functions.h"
#include <cmath>

namespace f360_variant_A
{

   /** \brief
    * Helper function to create a ROT_Object_Output_T with default values for testing.
    * Sets reasonable default values for all fields used in Object_Position_Plausible_Check().
    *
    * \param object Object with default test values
    * \param id The object track ID to assign
    * \param is_moving Flag indicating if the object is moving (affects speed and movement_status)
    */
   static void Fill_Default_Object_Values(ROT_Object_Output_T &object, const int32_t id, const bool is_moving)
   {
      // Fields used in Object_Position_Plausible_Check()
      object.id = id;
      object.object_status = 0U;   // 0 = measured (valid, not coasted)
      object.vcs_x_posn = 10.0f;   // 10 meters ahead
      object.vcs_y_posn = 0.0f;    // centered laterally
      object.vcs_pointing = 0.0f;  // pointing forward (0 radians)
      object.length = 5.0f;        // typical car length in meters
      object.width = 1.8f;         // typical car width in meters
      object.reference_point = 0U; // 0 = CENTER
      object.ndets = 0U;           // number of associated detections (can be overridden)
      object.iso_orientation = 0.0f;

      // Set speed and movement parameters based on is_moving flag
      if (is_moving)
      {
         object.speed = 10.0f;        // 10 m/s (~36 km/h)
         object.movement_status = 3U; // 3 = moving
         object.vcs_x_vel = 10.0f;    // 10 m/s in X direction (forward)
         object.vcs_y_vel = 0.0f;     // 0 m/s in Y direction (no lateral movement)
      }
      else
      {
         object.speed = 0.0f;         // stationary
         object.movement_status = 1U; // 1 = stationary
         object.vcs_x_vel = 0.0f;
         object.vcs_y_vel = 0.0f;
      }

      object.vcs_heading = 0.0f;
      object.existence_probability = 1.0f;
   }

   ROT_Object_Output_T &Add_Default_Object(ROT_Object_List_Info_T &rot_object_list_info, const bool is_moving)
   {
      // Increment the number of objects
      rot_object_list_info.number_of_objects++;

      // Get index of the newly added object (number_of_objects - 1)
      const uint32_t object_index = rot_object_list_info.number_of_objects - 1U;

      // Fill the object with default values, using number_of_objects as the ID
      Fill_Default_Object_Values(rot_object_list_info.rot_object_list[object_index],
                                 static_cast<int32_t>(rot_object_list_info.number_of_objects),
                                 is_moving);

      // Return reference to the newly added object for further customization if needed
      return rot_object_list_info.rot_object_list[object_index];
   }

   void Create_Detection_From_Object(rspp_variant_A::Processed_Detection_T &processed_detection,
                                     const ROT_Object_Output_T &object,
                                     const float32_t offset_scale_x,
                                     const float32_t offset_scale_y)
   {
        // Get object bounding box and center point
        SH_BoundingBox_T object_bbox{};
        const SH_Point_T box_center = Get_Object_Bounding_Box(object, object_bbox);

        // Calculate bounding box offsets using the same logic as Object_Position_Plausible_Check
        typedef struct Ext_Bounding_Box_Calibrations_Tag
        {
          float32_t base_offset;
          float32_t longitudinal_pos_offset;
          float32_t radial_range_coefficient;
          float32_t radial_range_based_saturation;
          float32_t longitudinal_pos_close_to_host_offset;
          float32_t cross_radial_range_based_coefficient;
          float32_t cross_radial_range_based_saturation;
        } Ext_Bounding_Box_Calibrations_T;

        static constexpr Ext_Bounding_Box_Calibrations_T kConfig{
           0.4F,
           2.1F,
           0.1F,
           1.0F,
           4.0F,
           0.0555F,
           3.33F};

        static constexpr float32_t k_max_xpos_to_be_considered_close_to_host = 4.0F;
        const float32_t host_to_target_range = std::sqrt((box_center.x * box_center.x) +
                                             (box_center.y * box_center.y));

        const float32_t range_based_increase = kConfig.base_offset +
                                     std::fminf(kConfig.radial_range_coefficient * host_to_target_range,
                                             kConfig.radial_range_based_saturation) +
                                     std::fminf(kConfig.cross_radial_range_based_coefficient * host_to_target_range,
                                             kConfig.cross_radial_range_based_saturation);

        const float32_t long_pos_gate_tcs_candidates = range_based_increase + kConfig.longitudinal_pos_offset;
        const float32_t long_pos_gate_tcs = (box_center.x < k_max_xpos_to_be_considered_close_to_host)
                                      ? std::fmaxf(kConfig.longitudinal_pos_close_to_host_offset, long_pos_gate_tcs_candidates)
                                      : long_pos_gate_tcs_candidates;
        const float32_t lat_pos_gate_tcs = range_based_increase;

        const float32_t cos_orientation = std::cos(object.iso_orientation);
        const float32_t sin_orientation = std::sin(object.iso_orientation);
        const float32_t long_pos_gate = (long_pos_gate_tcs * std::fabs(cos_orientation)) +
                                (lat_pos_gate_tcs * std::fabs(sin_orientation));
        const float32_t lat_pos_gate = (long_pos_gate_tcs * std::fabs(sin_orientation)) +
                               (lat_pos_gate_tcs * std::fabs(cos_orientation));

        // Get min/max lateral/long points of extended bounding box for object (valid det inside)
        SH_Point_T max_point_out{};
        SH_Point_T min_point_out{};
        Get_Axis_Aligned_Rect_MinMax_Pt(object_bbox, long_pos_gate, lat_pos_gate, max_point_out, min_point_out);

      // Center of the extended bounding box
      const float32_t center_x = (max_point_out.x + min_point_out.x) * 0.5f;
      const float32_t center_y = (max_point_out.y + min_point_out.y) * 0.5f;

      // Compute the detection position based on offset scales
      processed_detection.vcs_position_x = center_x + ((max_point_out.x - min_point_out.x) * 0.5f) * offset_scale_x;
      processed_detection.vcs_position_y = center_y + ((max_point_out.y - min_point_out.y) * 0.5f) * offset_scale_y;

      // Set other required processed detection fields to valid defaults
      processed_detection.vcs_position_z = 0.0f;
      processed_detection.range_rate_compensated = 0.0f;
      processed_detection.vcs_az = atanf(processed_detection.vcs_position_y / processed_detection.vcs_position_x);
      processed_detection.vcs_el = 0.0f;
      processed_detection.cos_vcs_az = cosf(processed_detection.vcs_az);
      processed_detection.sin_vcs_az = sinf(processed_detection.vcs_az);
      processed_detection.next_sorted_idx = -1;
      processed_detection.prev_sorted_idx = -1;
      processed_detection.motion_status = 0; // 0 = stationary
      processed_detection.f_ok_to_use = true;
   }

   void Create_And_Associate_Detections(rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
                                        F360_Detection_Log_T (&f360_detection_list)[MAX_NUMBER_OF_DETECTIONS],
                                        ROT_Object_Output_T &object,
                                        const uint32_t num_detections,
                                        const bool inside_bbox)
   {
      // Starting index for new detections
      const uint32_t starting_det_idx = raw_detect_list.number_of_valid_detections;

      // Validate inputs
      if ((raw_detect_list.number_of_valid_detections + num_detections) > MAX_NUMBER_OF_DETECTIONS)
      {
         return; // Would exceed array bounds
      }

      // Define offset scale patterns for placing detections
      // These create a distributed pattern of detections within/outside the bounding box
      // For inside_bbox: patterns stay well within [-1.0, 1.0] range
      // For outside_bbox: patterns use coordinates > 1.0 to ensure they're beyond the extended bbox
      const float32_t offset_scales_inside[][2] = {
          {0.0f, 0.0f},   // Center
          {0.5f, 0.0f},   // Front center
          {-0.5f, 0.0f},  // Rear center
          {0.0f, 0.5f},   // Center right
          {0.0f, -0.5f},  // Center left
          {0.5f, 0.5f},   // Front right
          {0.5f, -0.5f},  // Front left
          {-0.5f, 0.5f},  // Rear right
          {-0.5f, -0.5f}, // Rear left
          {1.0f, 0.0f},   // Edge front
          {-1.0f, 0.0f},  // Edge rear
          {0.0f, 1.0f},   // Edge right
          {0.0f, -1.0f},  // Edge left
          {1.0f, 1.0f},   // Edge front right
          {1.0f, -1.0f},  // Edge front left
          {-1.0f, 1.0f},  // Edge rear right
          {-1.0f, -1.0f}  // Edge rear left
      };

      const float32_t offset_scales_outside[][2] = {
          {1.01f, 1.01f},   // Front right - outside
          {1.01f, 0.0f},    // Front center - outside
          {-1.01f, 0.0f},   // Rear center - outside
          {0.0f, 1.01f},    // Center right - outside
          {0.0f, -1.01f},   // Center left - outside
          {1.01f, -1.01f},  // Front left - outside
          {-1.01f, 1.01f},  // Rear right - outside
          {-1.01f, -1.01f}, // Rear left - outside
          {1.5f, 0.0f},     // Far front - outside
          {-1.5f, 0.0f},    // Far rear - outside
          {0.0f, 1.5f},     // Far right - outside
          {0.0f, -1.5f},    // Far left - outside
          {2.5f, 2.5f},     // Far front right - outside
          {2.5f, -3.5f},    // Far front left - outside
          {-1.5f, 2.5f},    // Far rear right - outside
          {-3.0f, -1.5f}    // Far rear left - outside
      };

      const float32_t(*offset_scales)[2] = inside_bbox ? offset_scales_inside : offset_scales_outside;
      const uint32_t num_patterns = inside_bbox ? (sizeof(offset_scales_inside) / sizeof(offset_scales_inside[0])) : (sizeof(offset_scales_outside) / sizeof(offset_scales_outside[0]));

      // Create detections
      for (uint32_t i = 0U; i < num_detections; i++)
      {
         const uint32_t det_idx = starting_det_idx + i;
         const uint32_t pattern_idx = i % num_patterns;

         // Create detection at specified position
         Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed,
                                      object,
                                      offset_scales[pattern_idx][0],
                                      offset_scales[pattern_idx][1]);

         // Associate detection to object
         f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);
      }

      // Update detection list metadata
      raw_detect_list.number_of_valid_detections = starting_det_idx + num_detections;

      // Update object's detection count
      object.ndets += num_detections;
   }

} // namespace f360_variant_A
