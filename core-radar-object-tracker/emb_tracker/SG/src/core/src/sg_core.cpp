#include "sg_core.h"

#include <algorithm>

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
SGCore::SGCore(TimingInfo &timing_info) : m_timing(timing_info)
{
}

void SGCore::initialize(const SG_Internals_Dump_T &sg_internals)
{
   m_timestamp_us    = 0U;
   m_cycle_index     = 0U;
   m_f_state_cleared = false;

   m_detection_storage.initialize(sg_internals.detection_storage);
   m_contours.initialize(sg_internals.contour_storage);
}

void SGCore::reset()
{
   m_f_state_cleared = true; // TODO: FZD-2038 [SG] Implement SG reset logic
}

void SGCore::step(const SG_Input_T &input)
{
   m_f_state_cleared = false;

   const float elapsed_time = calculate_elapsed_time(input.timestamp_us);
   calculate_measurement_timestamp(input.sensors, m_dynamic_calibrations.get().common.elapsed_time_th);

   calculate_host_properties(elapsed_time, input.host, m_host_properties);
   update_calibrations_step(input.host.speed);
   time_update_step(elapsed_time, input.host);
   detection_processing_step(input.rspp_detections, input.rot_detections, input.sensors, input.host);
   detection_clustering_step();
   measurement_association_step();
   measurement_update_step();
   contour_initialization_step(input.host.curvature_rear);
   contour_postprocessing_step(input.host.curvature_rear);
   drivability_classification_step(elapsed_time, input.rot_detections, input.rspp_detections, input.host);
   sg_dc_fusion_step();
   contour_downselection_step(input.host);

   m_cycle_index++;
}

void SGCore::get_internals(SG_Internals_Dump_T &sg_internals) const
{
   sg_internals.execution_timestamp_us = m_timestamp_us;
   clear_internals(sg_internals);
   dump_internals(sg_internals.contour_storage);
   dump_internals(sg_internals.detection_storage);
   // sg_internals.dc_dump is not filled in the SG core (kernel)
}

void SGCore::get_output(SG_Output_T &sg_output) const
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

void SGCore::get_reduced_output(SG_ReducedOutput_T &sg_output) const
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

void SGCore::clear_internals(SG_Internals_Dump_T &internals)
{
   internals.contour_storage   = {};
   internals.detection_storage = {};
   internals.dc_dump           = {};
}

float SGCore::calculate_elapsed_time(const uint64_t timestamp_us)
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
      (void)elapsed_time_th; // MISRA
   }

   m_timestamp_us = timestamp_us;

   return (static_cast<float>(elapsed_time_us) / static_cast<float>(TIME_UNIT_SEC_USEC_CONVERSION_COEF)); // converted to [s]
}

void SGCore::calculate_measurement_timestamp(const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                             const uint64_t timestamp_step)
{
   uint64_t min_timestamp   = std::numeric_limits<uint64_t>::max();
   uint64_t max_timestamp   = std::numeric_limits<uint64_t>::min();
   bool f_any_valid_sensors = false;
   for (const auto &sensor : sensors)
   {
      if (sensor.variable.is_valid && (sensor.variable.look_id != RSPP_DET_LOOK_ID_INVALID) &&
          (sensor.variable.number_of_valid_detections > 0U))
      {
         min_timestamp       = std::min(min_timestamp, sensor.variable.timestamp_us);
         max_timestamp       = std::max(max_timestamp, sensor.variable.timestamp_us);
         f_any_valid_sensors = true;
      }
   }

   const uint64_t new_measurement_timestamp =
      f_any_valid_sensors ? (min_timestamp + (max_timestamp - min_timestamp) / 2U) : (m_measurement_timestamp + timestamp_step);
   assert(new_measurement_timestamp > m_measurement_timestamp);
   m_measurement_timestamp = new_measurement_timestamp;
}

void SGCore::update_calibrations_step(const float host_speed)
{
   const auto start_time = m_timing.elapsed();
   m_dynamic_calibrations.update(host_speed);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::UPDATE_CALIBRATIONS)] = m_timing.elapsed() - start_time;
}

void SGCore::time_update_step(const float elapsed_time, const RSPP_Host_T &host)
{
   const auto start_time = m_timing.elapsed();
   time_update(elapsed_time, host, m_host_properties, m_dynamic_calibrations.get().time_update, m_contours, m_detection_storage);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::TIME_UPDATE)] = m_timing.elapsed() - start_time;
}

void SGCore::detection_processing_step(const rspp::RSPP_Detection_List_T &rspp_detections,
                                       const rot::F360_Detection_Log_Output_T &rot_detections,
                                       const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                       const RSPP_Host_T &host)
{
   const auto start_time = m_timing.elapsed();
   std::bitset<SG_MAX_NUM_INPUT_DETS> nondrivable_detections_mask{};
   std::bitset<SG_MAX_NUM_INPUT_DETS> underdrivable_detections_mask{};
   downselect_input_detections(nondrivable_detections_mask, underdrivable_detections_mask,
                               m_dynamic_calibrations.get().detection_processing.downselect_input_detections,
                               m_dynamic_calibrations.get().common.view_ranges, rot_detections, rspp_detections, host.vcs_speed);
   combine_detections(m_detection_storage, m_dynamic_calibrations.get().detection_processing.importance_calibrations,
                      m_dynamic_calibrations.get().common, host, sensors, rspp_detections, nondrivable_detections_mask,
                      underdrivable_detections_mask);
   m_detection_storage.sort_by_x_pos();
   m_detection_storage.update_bin_info();
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DETECTION_PROCESSING)] = m_timing.elapsed() - start_time;
}

void SGCore::detection_clustering_step()
{
   const auto start_time    = m_timing.elapsed();
   const auto &calibrations = m_dynamic_calibrations.get().detection_clustering;
   squeeze_detections(m_detection_storage, calibrations.linear_piecewise_transform_coefficients);
   cluster_detections(m_detection_storage, calibrations.cluster_detections);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DETECTION_CLUSTERING)] = m_timing.elapsed() - start_time;
}

void SGCore::measurement_association_step()
{
   const auto start_time    = m_timing.elapsed();
   const auto &calibrations = m_dynamic_calibrations.get();
   measurement_association(m_contours, m_detection_storage, calibrations.measurement_association, calibrations.common);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::MEASUREMENT_ASSOCIATION)] = m_timing.elapsed() - start_time;
}

void SGCore::measurement_update_step()
{
   const auto start_time    = m_timing.elapsed();
   const auto &calibrations = m_dynamic_calibrations.get();
   measurement_update_contours(m_contours, m_detection_storage, calibrations.measurement_update,
                               calibrations.common.min_segment_length);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::MEASUREMENT_UPDATE)] = m_timing.elapsed() - start_time;
}

void SGCore::contour_initialization_step(const float curvature_rear)
{
   const auto start_time    = m_timing.elapsed();
   const auto &calibrations = m_dynamic_calibrations.get();
   initialize_contours(calibrations.contour_initialization, calibrations.common, curvature_rear, m_detection_storage, m_contours);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::CONTOURS_INITIALIZATION)] = m_timing.elapsed() - start_time;
}

void SGCore::contour_postprocessing_step(const float curvature_rear)
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

void SGCore::drivability_classification_step(const float elapsed_time, const rot::F360_Detection_Log_Output_T &rot_detections,
                                             const rspp::RSPP_Detection_List_T &rspp_detections, const RSPP_Host_T &host)
{
   // Do nothing

   (void)elapsed_time;    // MISRA
   (void)rot_detections;  // MISRA
   (void)rspp_detections; // MISRA
   (void)host;            // MISRA
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DRIVABILITY_CLASSIFICATION)] = {};
}

void SGCore::sg_dc_fusion_step()
{
   const auto start_time = m_timing.elapsed();

   m_fused_contours.clear();
   for (const auto &contour : m_contours)
   {
      uint16_t num_of_vertices = 0U;

      Fused_Contour_T::FusedVertexList fused_vertices;
      for (const auto &vertex : contour.vertices)
      {
         Fused_Vertex_T fused_vertex;

         fused_vertex.position               = vertex.position;
         fused_vertex.pos_cross_cov          = vertex.pos_cross_cov;
         fused_vertex.pos_cov                = vertex.pos_cov;
         fused_vertex.drivability_confidence = SG_DEFAULT_DRIVABILITY_CONFIDENCE;

         fused_vertex.sg_age                  = vertex.age;
         fused_vertex.sg_cycles_since_coasted = vertex.num_cycles_no_update;

         fused_vertex.drivability = contour.drivability;

         (void)fused_vertices.push_back(fused_vertex);

         num_of_vertices++;
      }

      auto fused_contour =
         Fused_Contour_T{fused_vertices, contour.unique_id(), num_of_vertices, contour.age, contour.drivability, false};
      (void)m_fused_contours.push_back(std::move(fused_contour));
   }

   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::SG_DC_FUSION)] = m_timing.elapsed() - start_time;
}

void SGCore::contour_downselection_step(const RSPP_Host_T &host)
{
   const auto start_time = m_timing.elapsed();
   m_contour_selector.run(m_dynamic_calibrations.get().contour_downselection, host);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::CONTOURS_DOWNSELECTION)] = m_timing.elapsed() - start_time;
}

void SGCore::dump_output(SG_Output_T &output) const
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
   (void)contour_idx; // MISRA
   (void)vertex_idx;  // MISRA
}

void SGCore::dump_reduced_output(SG_ReducedOutput_T &output) const
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

void SGCore::dump_internals(SG_Contour_Storage_Dump_T &contour_storage_dump) const
{
   assert((sizeof(SG_Contour_Storage_Dump_T::contours) / sizeof(SG_Contour_Storage_Dump_T::contours[0])) ==
          this->m_contours.capacity());

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

void SGCore::dump_internals(SG_Detection_Storage_Dump_T &detection_storage_dump) const
{
   assert(sizeof(SG_Detection_Storage_Dump_T::detections) / sizeof(SG_Detection_Storage_Dump_T::detections[0]) ==
          this->m_detection_storage.capacity());

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
      (void)detection_idx; // MISRA
   }
}
} // namespace sg
