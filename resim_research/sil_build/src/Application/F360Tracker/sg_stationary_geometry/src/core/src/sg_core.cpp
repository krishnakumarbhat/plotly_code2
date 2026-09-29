#include "sg_core.h"

#include <algorithm>

#include "dc_contour_fusion.h"
#include "sg_calibrations.h"
#include "sg_cluster_detections.h"
#include "sg_combine_detections.h"
#include "sg_constants.h"
#include "sg_contour_downselection.h"
#include "sg_declutter_contours.h"
#include "sg_dissociate_detections.h"
#include "sg_downselect_input_detections.h"
#include "sg_dumping.h"
#include "sg_initialize_contours.h"
#include "sg_measurement_association.h"
#include "sg_measurement_update_contours.h"
#include "sg_merge_contours.h"
#include "sg_remove_vertices.h"
#include "sg_select_and_remove_excess_contours.h"
#include "sg_simplify_contours.h"
#include "sg_squeeze_detections.h"
#include "sg_time_update.h"


namespace sg
{
   SgCore::SgCore(TimingInfo &timing_info) : m_timing(timing_info)
   {
   }

   void SgCore::initialize(const SG_Internals_Dump_T &sg_internals)
   {
      m_timestamp_us    = 0U;
      m_cycle_index     = 0U;
      m_f_state_cleared = false;

      m_detection_storage.initialize(sg_internals.detection_storage);
      m_contours.initialize(sg_internals.contour_storage);
      m_dc_contours.initialize(sg_internals.dc_dump);
   }

   void SgCore::reset()
   {
      m_f_state_cleared = true; // TODO: FZD-2038 [SG] Implement SG reset logic
   }

   void SgCore::step(const SG_Input_T &input)
   {
      m_f_state_cleared = false;

      const float elapsed_time = calculate_elapsed_time(input.timestamp_us);
      calculate_measurement_timestamp(input.sensors, m_dynamic_calibrations.get().elapsed_time_th);

      calculate_host_properties(elapsed_time, input.host, m_host_properties);
      update_calibrations_step(input.host.speed);
      time_update_step(elapsed_time, input.host);
      detection_processing_step(input.rspp_detections, input.sensors, input.host);
      detection_clustering_step();
      measurement_association_step();
      measurement_update_step();
      contour_initialization_step();
      contour_postprocessing_step(input.host.curvature_rear);
      drivability_classification_step(elapsed_time, input.rot_detections, input.rspp_detections, input.host, input.sensors);
      sg_dc_fusion_step();
      contour_downselection_step(input.host);

      m_cycle_index++;
   }

   void SgCore::get_internals(SG_Internals_Dump_T &sg_internals) const
   {
      sg_internals.execution_timestamp_us = m_timestamp_us;
      clear_internals(sg_internals);
      dump_internals(sg_internals.contour_storage);
      dump_internals(sg_internals.detection_storage);
      dump_internals(sg_internals.dc_dump);
   }

   void SgCore::get_output(SG_Output_T &sg_output) const
   {
      sg_output.execution_timestamp_us   = m_timestamp_us;
      sg_output.measurement_timestamp_us = m_measurement_timestamp;
      sg_output.cycle_index              = m_cycle_index;
      dump_output(sg_output);

      if (m_f_state_cleared)
      {
         sg_output.f_valid = false;
      }
   }

   void SgCore::get_reduced_output(SG_ReducedOutput_T &sg_output) const
   {
      sg_output.execution_timestamp_us   = m_timestamp_us;
      sg_output.measurement_timestamp_us = m_measurement_timestamp;
      sg_output.cycle_index              = m_cycle_index;
      dump_reduced_output(sg_output);

      if (m_f_state_cleared)
      {
         sg_output.f_valid = false;
      }
   }

   void SgCore::clear_internals(SG_Internals_Dump_T &internals)
   {
      internals.contour_storage   = {};
      internals.detection_storage = {};
      internals.dc_dump           = {};
   }

   float SgCore::calculate_elapsed_time(const uint64_t timestamp_us)
   {
      uint64_t elapsed_time_us       = 0U;
      const uint64_t elapsed_time_th = 50000U;
      if ((m_timestamp_us == 0U) && (elapsed_time_th < timestamp_us))
      {
         elapsed_time_us = elapsed_time_th;
      }
      else
      {
         elapsed_time_us = timestamp_us - m_timestamp_us;
         (void) elapsed_time_th; // MISRA
      }

      m_timestamp_us = timestamp_us;

      return (static_cast<float>(elapsed_time_us) / static_cast<float>(TIME_UNIT_SEC_USEC_CONVERSION_COEF)); // converted to [s]
   }

   void SgCore::calculate_measurement_timestamp(const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                                const uint64_t elapsed_time_th)
   {
      uint64_t min_timestamp   = std::numeric_limits<uint64_t>::max();
      uint64_t max_timestamp   = std::numeric_limits<uint64_t>::min();
      bool f_any_valid_sensors = false;
      for (const auto &sensor : sensors)
      {
         if ((sensor.variable.look_id != RSPP_DET_LOOK_ID_INVALID) && (sensor.variable.number_of_valid_detections > 0U))
         {
            min_timestamp       = std::min(min_timestamp, sensor.variable.timestamp_us);
            max_timestamp       = std::max(min_timestamp, sensor.variable.timestamp_us);
            f_any_valid_sensors = true;
         }
      }

      const uint64_t new_measurement_timestamp = f_any_valid_sensors ? (min_timestamp + (max_timestamp - min_timestamp) / 2U)
                                                                     : (m_measurement_timestamp + elapsed_time_th);
      assert(new_measurement_timestamp > m_measurement_timestamp);
      m_measurement_timestamp = new_measurement_timestamp;
   }

   void SgCore::update_calibrations_step(const float host_speed)
   {
      const auto start_time = m_timing.elapsed();
      m_dynamic_calibrations.update(host_speed);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::UPDATE_CALIBRATIONS)] = m_timing.elapsed() - start_time;
   }

   void SgCore::time_update_step(const float elapsed_time, const RSPP_Host_T &host)
   {
      const auto start_time = m_timing.elapsed();
      time_update(elapsed_time, host, m_host_properties, m_dynamic_calibrations.get().time_update, m_contours, m_detection_storage);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::TIME_UPDATE)] = m_timing.elapsed() - start_time;
   }

   void SgCore::detection_processing_step(const rspp::RSPP_Detection_List_T &rspp_detections,
                                          const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                          const RSPP_Host_T &host)
   {
      const auto start_time = m_timing.elapsed();
      std::bitset<SG_MAX_NUM_INPUT_DETS> nondrivable_detections_mask{};
      std::bitset<SG_MAX_NUM_INPUT_DETS> underdrivable_detections_mask{};
      downselect_input_detections(nondrivable_detections_mask, underdrivable_detections_mask,
                                  m_dynamic_calibrations.get().detection_processing.downselect_input_detections,
                                  m_dynamic_calibrations.get().common.view_ranges, rspp_detections, host.vcs_speed);
      combine_detections(m_detection_storage, m_dynamic_calibrations.get().detection_processing.importance_calibrations,
                         m_dynamic_calibrations.get().common, host, sensors, rspp_detections, nondrivable_detections_mask,
                         underdrivable_detections_mask);
      m_detection_storage.sort_by_x_pos();
      m_detection_storage.update_bin_info();
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DETECTION_PROCESSING)] = m_timing.elapsed() - start_time;
   }

   void SgCore::detection_clustering_step()
   {
      const auto start_time    = m_timing.elapsed();
      const auto &calibrations = m_dynamic_calibrations.get().detection_clustering;
      squeeze_detections(m_detection_storage, calibrations.linear_piecewise_transform_coefficients);
      cluster_detections(m_detection_storage, calibrations.cluster_detections);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DETECTION_CLUSTERING)] = m_timing.elapsed() - start_time;
   }

   void SgCore::measurement_association_step()
   {
      const auto start_time    = m_timing.elapsed();
      const auto &calibrations = m_dynamic_calibrations.get();
      measurement_association(m_contours, m_detection_storage, calibrations.measurement_association, calibrations.common);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::MEASUREMENT_ASSOCIATION)] = m_timing.elapsed() - start_time;
   }

   void SgCore::measurement_update_step()
   {
      const auto start_time    = m_timing.elapsed();
      const auto &calibrations = m_dynamic_calibrations.get();
      measurement_update_contours(m_contours, m_detection_storage, calibrations.measurement_update,
                                  calibrations.common.min_segment_length);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::MEASUREMENT_UPDATE)] = m_timing.elapsed() - start_time;
   }

   void SgCore::contour_initialization_step()
   {
      const auto start_time    = m_timing.elapsed();
      const auto &calibrations = m_dynamic_calibrations.get();
      initialize_contours(calibrations.contour_initialization, calibrations.common, m_detection_storage, m_contours);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::CONTOURS_INITIALIZATION)] = m_timing.elapsed() - start_time;
   }

   void SgCore::contour_postprocessing_step(const float curvature_rear)
   {
      const auto start_time    = m_timing.elapsed();
      const auto &calibrations = m_dynamic_calibrations.get();
      declutter_contours(m_contours, calibrations.contour_postprocessing.declutter_contours, calibrations.common.azimuth_epsilon);
      merge_contours(m_contours, calibrations.contour_postprocessing.merge_contours, curvature_rear);
      simplify_contours(m_contours, calibrations.contour_postprocessing.simplify_contours);
      remove_vertices(m_contours, calibrations.contour_postprocessing.remove_vertices);
      select_and_remove_excess_contours(m_contours, m_host_properties,
                                        calibrations.contour_postprocessing.select_and_remove_excess_contours, calibrations.common);
      dissociate_detections(m_detection_storage, m_contours);
      assert(m_detection_storage.is_sorted_by_x());
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::CONTOURS_POSTPROCESSING)] = m_timing.elapsed() - start_time;
   }

   void SgCore::drivability_classification_step(const float elapsed_time,
                                                const rot::F360_Detection_Log_Output_T &rot_detections,
                                                const rspp::RSPP_Detection_List_T &rspp_detections,
                                                const RSPP_Host_T &host,
                                                const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS])
   {
      (void) sensors; // MISRA
      const auto start_time = m_timing.elapsed();
      m_dc_interface.step(m_timing, m_dc_contours, m_critical_region, m_contours, rot_detections, rspp_detections, elapsed_time,
                          host, m_host_properties, m_dynamic_calibrations.get().drivability_classification);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DRIVABILITY_CLASSIFICATION)] = m_timing.elapsed() - start_time;
   }

   void SgCore::sg_dc_fusion_step()
   {
      const auto start_time = m_timing.elapsed();
      dc::contour_fusion(m_fused_contours, m_contours, m_dc_contours, m_dynamic_calibrations.get().common.min_segment_length);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::SG_DC_FUSION)] = m_timing.elapsed() - start_time;
   }

   void SgCore::contour_downselection_step(const RSPP_Host_T &host)
   {
      const auto start_time = m_timing.elapsed();
      m_contour_selector.run(m_dynamic_calibrations.get().contour_downselection, host);
      m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::CONTOURS_DOWNSELECTION)] = m_timing.elapsed() - start_time;
   }

   void SgCore::dump_output(SG_Output_T &output) const
   {
      std::fill(std::begin(output.contours), std::end(output.contours), SG_Contour_Out_T{});
      std::fill(std::begin(output.vertices), std::end(output.vertices), SG_Vertex_Out_T{});
      output.num_contours = 0U;

      std::size_t contour_idx = 0U;
      std::size_t vertex_idx  = 0U;

      for (const auto &contour : m_fused_contours)
      {
         if (contour_idx >= SG_MAX_NUM_OUTPUT_CONTOURS)
         {
            assert(false);
            break;
         }

         dumping::dump(output.contours[contour_idx], contour);
         output.num_contours++;
         contour_idx++;

         for (const auto &vertex : contour.vertices)
         {
            if (vertex_idx >= SG_MAX_NUM_OUTPUT_VERTICES)
            {
               assert(false);
               break;
            }
            dumping::dump(output.vertices[vertex_idx], vertex);
            vertex_idx++;
         }
      }

      output.f_valid = true;
      (void) contour_idx; // MISRA
      (void) vertex_idx;  // MISRA
   }

   void SgCore::dump_reduced_output(SG_ReducedOutput_T &output) const
   {
      std::fill(std::begin(output.contours), std::end(output.contours), SG_Contour_Out_T{});
      std::fill(std::begin(output.vertices), std::end(output.vertices), SG_Vertex_Out_T{});
      output.num_contours = 0U;

      std::size_t contour_idx = 0U;
      std::size_t vertex_idx  = 0U;

      for (const auto &contour : m_fused_contours)
      {
         if ((contour_idx >= SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS) || (vertex_idx >= SG_MAX_NUM_REDUCED_OUTPUT_VERTICES))
         {
            break;
         }

         if ((vertex_idx + contour.vertices.size()) > SG_MAX_NUM_REDUCED_OUTPUT_VERTICES)
         {
            continue;
         }

         if (contour.f_selected_for_output)
         {
            dumping::dump(output.contours[contour_idx], contour);
            output.num_contours++;
            contour_idx++;

            for (const auto &vertex : contour.vertices)
            {
               if (vertex_idx >= SG_MAX_NUM_REDUCED_OUTPUT_VERTICES)
               {
                  assert(false);
                  break;
               }

               dumping::dump(output.vertices[vertex_idx], vertex);
               vertex_idx++;
            }
         }
      }

      output.f_valid = true;
   }

   void SgCore::dump_internals(SG_Contour_Storage_Dump_T &contour_storage_dump) const
   {
      assert((sizeof(SG_Contour_Storage_Dump_T::contours) / sizeof(SG_Contour_Storage_Dump_T::contours[0]))
             == this->m_contours.capacity());

      std::size_t vertex_idx  = 0U;
      std::size_t contour_idx = 0U;

      for (const auto &contour : m_contours)
      {
         if (contour_idx >= this->m_contours.capacity())
         {
            assert(false);
            break;
         }

         dumping::dump(contour_storage_dump.contours[contour_idx], contour);
         contour_idx++;

         vertex_idx = dumping::dump(contour_storage_dump.vertices, contour.vertices, vertex_idx);
      }

      assert(contour_idx == this->m_contours.size());
      contour_storage_dump.number_of_contours = this->m_contours.size();
   }

   void SgCore::dump_internals(SG_Detection_Storage_Dump_T &detection_storage_dump) const
   {
      assert(sizeof(SG_Detection_Storage_Dump_T::detections) / sizeof(SG_Detection_Storage_Dump_T::detections[0])
             == this->m_detection_storage.capacity());

      if (m_detection_storage.size() > 0U)
      {
         std::size_t detection_idx = 0U;

         for (const auto &detection : m_detection_storage)
         {
            if (detection_idx >= this->m_detection_storage.capacity())
            {
               assert(false);
               break;
            }
            dumping::dump(detection_storage_dump.detections[detection_idx], detection);
            ++detection_idx;
         }
         (void) detection_idx; // MISRA
      }
   }

   void SgCore::dump_internals(DC_Dump_T &dc_dump) const
   {
      const dc::DC_Contour_T *contours[SG_MAX_NUM_CONTOURS] = {nullptr};
      for (const auto &dc_contour : m_dc_contours)
      {
         contours[dc_contour.get_id() - 1U] = &dc_contour;
      }

      auto num_DC_contours = 0U;
      for (auto i = 0U; i < SG_MAX_NUM_CONTOURS; ++i)
      {
         if (contours[i])
         {
            const auto num_of_vertices = contours[i]->subsegments.size() + 1U;
            if (num_of_vertices > 0U)
            {
               dc_dump.dc_contours_dump.contour_id[num_DC_contours]     = contours[i]->get_id();
               dc_dump.dc_contours_dump.num_vertices[num_DC_contours]   = num_of_vertices;
               dc_dump.dc_contours_dump.f_valid[num_DC_contours]        = true;
               dc_dump.dc_contours_dump.sg_drivability[num_DC_contours] = contours[i]->get_drivability();
               ++num_DC_contours;
            }
         }
      }
      dc_dump.num_DC_contours = num_DC_contours;

      for (auto idx = 0U; idx < DC_MAX_COORDINATES_REGION_SIZE; ++idx)
      {
         dc_dump.critical_region_dump.critical_region[idx].x = m_critical_region[idx].y;
         dc_dump.critical_region_dump.critical_region[idx].y = m_critical_region[idx].x;
      }

      auto output_index = 0U;
      for (auto contour_index = 0U; contour_index < SG_MAX_NUM_CONTOURS; ++contour_index)
      {
         if (contours[contour_index])
         {
            auto num_of_vertices = contours[contour_index]->subsegments.size() + 1U;
            auto subvertex_idx   = 0U;
            for (auto subsegment = contours[contour_index]->subsegments.begin();
                 (num_of_vertices > 0U) && (subvertex_idx < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
                 && (output_index < SG_MAX_NUM_OUTPUT_VERTICES) && (subsegment != contours[contour_index]->subsegments.end());
                 ++subvertex_idx, ++subsegment)
            {
               const auto subsegment_id                           = subsegment->subsegment_id;
               dc_dump.dc_vertices_dump.unique_id[output_index]   = subsegment_id;
               dc_dump.dc_vertices_dump.drivability[output_index] = static_cast<SG_Drivability_Class_T>(subsegment->drivability);
               dc_dump.dc_vertices_dump.drivability_confidence[output_index] =
                  static_cast<uint8_t>(std::round(subsegment->drivability_confidence));
               dc_dump.dc_vertices_dump.dc_features_dump.rcs_recur_mean[output_index] = subsegment->features.rcs_recur_mean;
               dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_ewma025_mean[output_index] =
                  subsegment->features.z_scs_abs_ewma025_mean;
               dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_max[output_index] = subsegment->features.z_scs_abs_max;
               dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_recur_mean[output_index] = subsegment->features.z_scs_abs_recur_mean;
               dc_dump.dc_vertices_dump.dc_features_dump.height_under_nondr_bins_proportion[output_index] =
                  subsegment->features.height_under_nondr_bins_proportion;
               dc_dump.dc_vertices_dump.dc_features_dump.height_over_nondr_bins_proportion[output_index] =
                  subsegment->features.height_over_nondr_bins_proportion;
               dc_dump.dc_vertices_dump.dc_features_dump.detections_number[output_index] = subsegment->features.detections_number;
               dc_dump.dc_vertices_dump.dc_features_dump.detection_density[output_index] = subsegment->features.detection_density;
               dc_dump.dc_vertices_dump.segment_id[output_index]                         = subsegment->segment_id;
               dc_dump.dc_vertices_dump.f_critical[output_index]                         = subsegment->begin_vertex.f_critical;
               dc_dump.dc_vertices_dump.f_primary[output_index]                          = subsegment->begin_vertex.f_primary;
               dc_dump.dc_vertices_dump.position_x[output_index]                         = subsegment->begin_vertex.position.x;
               dc_dump.dc_vertices_dump.position_y[output_index]                         = subsegment->begin_vertex.position.y;

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
               dump_assigned_detections(dc_dump, subsegment, output_index);
#endif

               ++output_index;
               --num_of_vertices;
               if ((num_of_vertices == 1U) && (output_index < SG_MAX_NUM_OUTPUT_VERTICES))
               {
                  dc_dump.dc_vertices_dump.unique_id[output_index]                       = INVALID_SEGMENT_ID;
                  dc_dump.dc_vertices_dump.drivability[output_index]                     = SG_Drivability_Class_T::UNCLASSIFIED;
                  dc_dump.dc_vertices_dump.dc_features_dump.rcs_recur_mean[output_index] = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_ewma025_mean[output_index]             = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_max[output_index]                      = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_recur_mean[output_index]               = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.height_under_nondr_bins_proportion[output_index] = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.height_over_nondr_bins_proportion[output_index]  = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.detections_number[output_index]                  = 0.0F;
                  dc_dump.dc_vertices_dump.dc_features_dump.detection_density[output_index] = subsegment->features.detection_density;
                  dc_dump.dc_vertices_dump.segment_id[output_index] = INVALID_SEGMENT_ID;
                  dc_dump.dc_vertices_dump.f_critical[output_index] = subsegment->end_vertex.f_critical;
                  dc_dump.dc_vertices_dump.f_primary[output_index]  = subsegment->end_vertex.f_primary;
                  dc_dump.dc_vertices_dump.position_x[output_index] = subsegment->end_vertex.position.x;
                  dc_dump.dc_vertices_dump.position_y[output_index] = subsegment->end_vertex.position.y;
                  ++output_index;
                  --num_of_vertices;
                  break;
               }
            }
         }
      }
   }

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
   void SgCore::dump_assigned_detections(DC_Dump_T &dc_dump,
                                         const dc::DC_Contour_T::SubsegmentList::iterator subsegment,
                                         uint32_t output_index) const
   {
      auto &assigned_detections = sg::dc::DCContourStorage::assigned_detections[subsegment->subsegment_id];
      for (auto det_idx = 0U; det_idx < assigned_detections.num_valid_dets && det_idx < SG_MAX_NUM_DETS_PER_SUBSEGMENT; det_idx++)
      {
         dc_dump.dc_vertices_dump.assigned_detections[output_index].range[det_idx]     = assigned_detections.range[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].snr[det_idx]       = assigned_detections.snr[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].rcs[det_idx]       = assigned_detections.rcs[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].z_scs[det_idx]     = assigned_detections.z_scs[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].z_scs_abs[det_idx] = assigned_detections.z_scs_abs[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].confid_azimuth[det_idx] =
            assigned_detections.confid_azimuth[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].confid_elevation[det_idx] =
            assigned_detections.confid_elevation[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].f_super_res[det_idx] = assigned_detections.f_super_res[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].f_bistatic[det_idx] = assigned_detections.f_bistatic[det_idx];
         dc_dump.dc_vertices_dump.assigned_detections[output_index].num_associated_dets = assigned_detections.num_associated_dets;
         dc_dump.dc_vertices_dump.assigned_detections[output_index].num_valid_dets      = assigned_detections.num_valid_dets;
      }
   }
#endif

}
