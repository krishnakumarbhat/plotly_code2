#include "sg_core_with_dc.h"

#include "dc_contour_fusion.h"

namespace sg
{
SGCoreWithDC::SGCoreWithDC(TimingInfo &timing_info) : SGCore(timing_info)
{
}

void SGCoreWithDC::initialize(const SG_Internals_Dump_T &sg_internals)
{
   SGCore::initialize(sg_internals);
   m_dc_contours.initialize(sg_internals.dc_dump);
}

void SGCoreWithDC::drivability_classification_step(const float elapsed_time,
                                                   const rot::F360_Detection_Log_Output_T &rot_detections,
                                                   const rspp::RSPP_Detection_List_T &rspp_detections, const RSPP_Host_T &host)
{
   const auto start_time = m_timing.elapsed();
   m_dc_interface.step(m_timing, m_dc_contours, m_critical_region, m_contours, rot_detections, rspp_detections, elapsed_time, host,
                       m_host_properties, m_dynamic_calibrations.get().drivability_classification);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::DRIVABILITY_CLASSIFICATION)] = m_timing.elapsed() - start_time;
}

void SGCoreWithDC::sg_dc_fusion_step()
{
   const auto start_time = m_timing.elapsed();
   dc::contour_fusion(m_fused_contours, m_contours, m_dc_contours, m_dynamic_calibrations.get().common.min_segment_length);
   m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::SG_DC_FUSION)] = m_timing.elapsed() - start_time;
}

void SGCoreWithDC::get_internals(SG_Internals_Dump_T &sg_internals) const
{
   SGCore::get_internals(sg_internals);
   dump_internals(sg_internals.dc_dump);
}

void SGCoreWithDC::dump_internals(DC_Dump_T &dc_dump) const
{
   const dc::DC_Contour_T *contours[SG_MAX_NUM_CONTOURS]{};
   for (const auto &dc_contour : m_dc_contours)
   {
      contours[dc_contour.get_id() - 1U] = &dc_contour; // TODO FZD-2474: Incorrect usage of ID as array index
   }

   auto num_DC_contours = 0U;
   for (auto i = 0U; i < SG_MAX_NUM_CONTOURS; ++i)
   {
      if (contours[i])
      {
         const auto num_of_vertices = contours[i]->get_num_of_vertices();
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
         auto num_of_vertices = contours[contour_index]->get_num_of_vertices();
         auto subvertex_idx   = 0U;
         for (auto subsegment = contours[contour_index]->subsegments.begin();
              (num_of_vertices > 0U) && (subvertex_idx < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR) &&
              (output_index < DC_MAX_NUM_OUTPUT_VERTICES) && (subsegment != contours[contour_index]->subsegments.end());
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
            dc_dump.dc_vertices_dump.dc_features_dump.z_scs_abs_recur_mean[output_index] =
               subsegment->features.z_scs_abs_recur_mean;
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

#ifdef SG_DC_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
            dump_assigned_detections(dc_dump, subsegment, output_index);
#endif

            ++output_index;
            --num_of_vertices;
            if ((num_of_vertices == 1U) && (output_index < DC_MAX_NUM_OUTPUT_VERTICES))
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
               dc_dump.dc_vertices_dump.segment_id[output_index]                         = INVALID_SEGMENT_ID;
               dc_dump.dc_vertices_dump.f_critical[output_index]                         = subsegment->end_vertex.f_critical;
               dc_dump.dc_vertices_dump.f_primary[output_index]                          = subsegment->end_vertex.f_primary;
               dc_dump.dc_vertices_dump.position_x[output_index]                         = subsegment->end_vertex.position.x;
               dc_dump.dc_vertices_dump.position_y[output_index]                         = subsegment->end_vertex.position.y;
               ++output_index;
               --num_of_vertices;
               break;
            }
         }
      }
   }
   assert(output_index < DC_MAX_NUM_OUTPUT_VERTICES);
}

#ifdef SG_DC_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
void SGCoreWithDC::dump_assigned_detections(DC_Dump_T &dc_dump, const dc::DC_Contour_T::SubsegmentList::iterator subsegment,
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
      dc_dump.dc_vertices_dump.assigned_detections[output_index].f_bistatic[det_idx]  = assigned_detections.f_bistatic[det_idx];
      dc_dump.dc_vertices_dump.assigned_detections[output_index].num_associated_dets  = assigned_detections.num_associated_dets;
      dc_dump.dc_vertices_dump.assigned_detections[output_index].num_valid_dets       = assigned_detections.num_valid_dets;
   }
}
#endif
} // namespace sg
