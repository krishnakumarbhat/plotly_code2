#include "sg_downselect_input_detections.h"

#include "sg_downselect_input_detections_helpers.h"

namespace sg
{
void downselect_input_detections(std::bitset<SG_MAX_NUM_INPUT_DETS> &nondrivable_detections_mask,
                                 std::bitset<SG_MAX_NUM_INPUT_DETS> &underdrivable_detections_mask,
                                 const DownselectInputDetectionsCalibs &calibrations, const View_Ranges_T &view_ranges,
                                 const rot::F360_Detection_Log_Output_T &rot_detections,
                                 const SG_Input_Detections_T &input_detections, const float &host_speed)
{
   if (input_detections.number_of_valid_detections > 0U)
   {
      for (std::size_t idx = 0U; idx < input_detections.number_of_valid_detections; idx++)
      {
         assert(idx < SG_MAX_NUM_INPUT_DETS);
         const auto &detection_processed = input_detections.detections[idx].processed;
         const auto &detection_raw       = input_detections.detections[idx].raw;

         nondrivable_detections_mask[idx]   = false;
         underdrivable_detections_mask[idx] = false;
         if ((0U == rot_detections.detection[idx].f_inside_mov_gate) &&
             (0U == rot_detections.detection[idx].f_trailer_related_det) &&
             is_detection_stationary(detection_processed.range_rate_compensated,
                                     static_cast<rspp::RSPP_Detection_Motion_Status_T>(detection_processed.motion_status),
                                     calibrations.max_rr_compens_for_stat_det))
         {
            // TODO: https://jiraprod.aptiv.com/browse/FZD-527 determine value of probability_of_detection
            const float probability_of_detection = 0.75F;
            if (is_probability_accepted(probability_of_detection, calibrations.accepted_probability_level))
            {
               if (is_azimuth_confidence_sufficient(detection_raw.confid_azimuth,
                                                    calibrations.maximum_valid_azimuth_confidence_value) ||
                   is_poor_azimuth_confidence_allowed(host_speed, detection_processed.vcs_position_x,
                                                      calibrations.max_host_speed_for_poor_azim_confid_det,
                                                      calibrations.max_distance_for_poor_azim_confid_det))
               {
                  nondrivable_detections_mask[idx] =
                     is_within_region_of_interest(detection_processed.vcs_position_x, detection_processed.vcs_position_y,
                                                  detection_processed.vcs_position_z, view_ranges.nondrivable);
                  underdrivable_detections_mask[idx] =
                     is_within_region_of_interest(detection_processed.vcs_position_x, detection_processed.vcs_position_y,
                                                  detection_processed.vcs_position_z, view_ranges.underdrivable);
               }
            }
            (void)probability_of_detection; // MISRA
         }
         (void)detection_processed; // MISRA
         (void)detection_raw;       // MISRA
      }
   }
}
} // namespace sg
