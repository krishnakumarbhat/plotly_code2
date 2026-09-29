#include "dc_dummy_generator.h"

#include <cmath>
#include <random>

namespace sg
{
   namespace dc
   {
      float random_float(const float min_value, const float max_value)
      {
         const float range         = max_value - min_value;
         const float random_number = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
         return min_value + random_number * range;
      }

      float random_perturbation()
      {
         const float a = round((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 20.0F - 10.0F) / 100.0F;
         return a;
      }

      void fill_dc_contours_with_dummy_data(DCContourStorage &dc_contours)
      {
         dc_contours.clear();

         srand(123U); // set constant seed for easier debugging
         const uint8_t num_contours{5U};
         const uint8_t num_subsegments{18U};
         const uint32_t segments_id_order[num_subsegments]               = {1U, 2U, 0U, 6U,  7U,  8U, 9U,  0U,  3U,
                                                                            4U, 5U, 0U, 10U, 11U, 0U, 12U, 13U, 0U};
         const float height_under_nondr_bins_proportion[num_subsegments] = {
            0.32F, 0.2F, 0.4F, 0.7F, 0.55F, 0.9F, 1.0F, 0.62F, 0.11F, 0.0F, 0.1F, 0.81F, 0.57F, 0.7F, 0.0F, 0.4F, 0.76F, 0.33F};
         const float height_over_nondr_bins_proportion[num_subsegments] = {
            0.32F, 0.2F, 0.4F, 0.7F, 0.55F, 0.9F, 1.0F, 0.62F, 0.11F, 0.0F, 0.1F, 0.81F, 0.57F, 0.7F, 0.0F, 0.4F, 0.76F, 0.33F};
         const uint8_t num_of_dets_associated_last_scan[num_subsegments] = {4U, 3U, 1U, 2U, 0U, 3U,  4U,  3U, 2U,
                                                                            7U, 5U, 2U, 0U, 9U, 11U, 13U, 1U, 3U};
         const float vertex_x[num_subsegments]            = {2.9F,  2.1F,  1.1F,  5.09F,  5.02F,  5.17F,  5.35F,  6.04F,  4.97F,
                                                             6.04F, 7.08F, 8.07F, 15.01F, 15.12F, 15.23F, 25.01F, 25.12F, 25.23F};
         const float vertex_y[num_subsegments]            = {0.32F, 0.16F, 0.11F, 5.09F,  5.98F,  6.97F,  7.11F,   7.57F,   0.02F,
                                                             0.08F, 0.15F, 0.09F, 15.03F, 16.05F, 17.07F, -25.09F, -26.08F, -27.06F};
         const float contour_z[num_contours]              = {1.2F, 0.1F, 5.2F, 4.5F, 0.5F};
         const bool contour_criticality[num_contours]     = {true, true, true, false, false};
         const uint8_t first_vertex_indices[num_contours] = {0U, 3U, 8U, 12U, 15U};
         const SG_Drivability_Class_T subsegments_drivability[num_subsegments] = {
            SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE,
            SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::OVERDRIVABLE,
            SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::UNDERDRIVABLE,
            SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE,
            SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE,
            SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::UNCLASSIFIED};

         Subsegment_T subsegments_array[num_subsegments];

         uint8_t contour_idx = 0U;
         for (uint8_t idx = 0U; idx < num_subsegments; ++idx)
         {
            if ((segments_id_order[idx]) == 0U && (idx > 0U))
            {
               subsegments_array[idx - 1U].end_vertex.position.x = vertex_x[idx];
               subsegments_array[idx - 1U].end_vertex.position.y = vertex_y[idx];
               ++contour_idx;
               continue;
            }

            subsegments_array[idx] = Subsegment_T{{{vertex_x[idx], vertex_y[idx]}, contour_criticality[contour_idx], true},
                                                  {{0.F, 0.F}, contour_criticality[contour_idx], true}};
            if ((idx != first_vertex_indices[contour_idx]) && (idx > 0U))
            {
               subsegments_array[idx - 1U].end_vertex = subsegments_array[idx].begin_vertex;
            }

            subsegments_array[idx].segment_id                       = segments_id_order[idx];
            subsegments_array[idx].subsegment_id                    = idx;
            subsegments_array[idx].num_of_dets_associated_last_scan = num_of_dets_associated_last_scan[idx];
            subsegments_array[idx].drivability                      = subsegments_drivability[idx];

            Signal_Extremes_T maxes;
            maxes.set_z_scs_abs(contour_z[contour_idx] + random_float(0.5F, 1.0F));
            subsegments_array[idx].past_data.set_maxes(maxes);

            Signals_T means;
            means.set_rcs(random_float(-10.0F, 10.0F));
            means.set_z_scs_abs_ewma025(contour_z[contour_idx] + random_perturbation());
            means.set_z_scs_abs_recur(contour_z[contour_idx] + random_perturbation());
            subsegments_array[idx].past_data.set_means(means);

            subsegments_array[idx].past_data.add_to_detections_sum(num_of_dets_associated_last_scan[idx]);
            subsegments_array[idx].past_data.add_to_detections_sum(static_cast<uint32_t>(std::rand() % 20));

            subsegments_array[idx].features.detections_number =
               static_cast<float>(subsegments_array[idx].past_data.get_detections_sum());
            subsegments_array[idx].features.rcs_recur_mean                     = means.get_rcs();
            subsegments_array[idx].features.z_scs_abs_ewma025_mean             = means.get_z_scs_abs_ewma025();
            subsegments_array[idx].features.z_scs_abs_recur_mean               = means.get_z_scs_abs_recur();
            subsegments_array[idx].features.z_scs_abs_max                      = maxes.get_z_scs_abs();
            subsegments_array[idx].features.height_under_nondr_bins_proportion = height_under_nondr_bins_proportion[idx];
            subsegments_array[idx].features.height_over_nondr_bins_proportion  = height_over_nondr_bins_proportion[idx];
         }

         auto contour1        = DC_Contour_T{subsegments_array[0U], subsegments_array[1U]};
         contour1.drivability = SG_Drivability_Class_T::NONDRIVABLE;

         auto contour2 = DC_Contour_T{subsegments_array[3U], subsegments_array[4U], subsegments_array[5U], subsegments_array[6U]};
         contour2.drivability = SG_Drivability_Class_T::OVERDRIVABLE;

         auto contour3        = DC_Contour_T{subsegments_array[8U], subsegments_array[9U], subsegments_array[10U]};
         contour3.drivability = SG_Drivability_Class_T::UNDERDRIVABLE;

         auto contour4        = DC_Contour_T{subsegments_array[12U], subsegments_array[13U]};
         contour4.drivability = SG_Drivability_Class_T::UNDERDRIVABLE;

         auto contour5        = DC_Contour_T{subsegments_array[15U], subsegments_array[16U]};
         contour5.drivability = SG_Drivability_Class_T::NONDRIVABLE;

         DCContourStorage::ContourList::iterator add_contour_result = nullptr;
         add_contour_result                                         = dc_contours.push_back(std::move(contour1));

         assert(add_contour_result != nullptr);
         add_contour_result = dc_contours.push_back(std::move(contour2));
         assert(add_contour_result != nullptr);
         add_contour_result = dc_contours.push_back(std::move(contour3));
         assert(add_contour_result != nullptr);
         add_contour_result = dc_contours.push_back(std::move(contour4));
         assert(add_contour_result != nullptr);
         add_contour_result = dc_contours.push_back(std::move(contour5));
         assert(add_contour_result != nullptr);
      }

      void dc_fill_input_detections_with_dummy_data(sg::SG_Input_Detections_T &input_detections)
      {
         srand(123U); // set constant seed for easier debugging
         const uint8_t num_of_dets            = 30U;
         const uint8_t num_of_stationary_dets = 20U;
         assert(num_of_dets <= SG_MAX_NUM_INPUT_DETS);
         input_detections.number_of_valid_detections = num_of_dets;
         constexpr uint8_t num_clusters              = 3U;
         const float dets_x[num_of_dets]             = {-1.3F,  2.1F,  3.2F,   23.1F, 75.1F, 65.2F, -1.1F, 2.5F,  2.9F,  82.4F,
                                                        170.3F, 64.9F, -1.2F,  2.4F,  3.1F,  31.3F, 36.2F, 14.4F, -1.6F, 2.3F,
                                                        3.3F,   4.1F,  151.2F, 1.9F,  -1.0F, 2.7F,  5.1F,  99.9F, 81.1F, 57.6F};
         const float dets_y[num_of_dets]             = {0.2F,  -6.1F, 0.1F,  13.1F, -11.1F, 12.1F,  -0.2F, -6.1F,  -0.1F, 5.1F,
                                                        -4.1F, 12.1F, -0.1F, -5.9F, 0.05F,  -12.1F, 5.1F,  4.1F,   0.1F,  -6.2F,
                                                        0.2F,  7.1F,  -5.1F, -3.1F, 4.1F,   9.1F,   13.1F, -12.1F, 33.1F, -11.1F};
         const float dets_z[num_of_stationary_dets]  = {-1.3F,  0.0F,   -5.24F, 6.0F,  -7.16F, -1.55F, -1.22F,
                                                        -0.04F, -5.24F, 8.31F,  5.85F, 9.19F,  -1.13F, -0.02F,
                                                        -5.13F, 3.11F,  -9.28F, 6.98F, -1.26F, -0.18F};
         const float cluster_centers_z[num_clusters] = {-1.2F, -0.1F, -5.2F};
         const float rcs[num_of_stationary_dets]     = {-4.0034F,  19.8914F, -19.0844F, 16.2317F,  -14.9205F, 16.5350F, 16.5026F,
                                                        -5.6722F,  7.0687F,  5.2944F,   -16.0984F, -8.8601F,  -8.3529F, 15.9032F,
                                                        -12.1445F, 1.8753F,  18.3003F,  18.5955F,  -9.4308F,  -8.5287F};

         for (std::size_t idx = 0U; idx < num_of_dets; ++idx)
         {
            uint8_t cluster_idx                                       = idx % num_clusters;
            input_detections.detections[idx].processed.vcs_position_x = dets_x[idx];
            input_detections.detections[idx].processed.vcs_position_y = dets_y[idx];
            if (idx < num_of_stationary_dets)
            {
               input_detections.detections[idx].processed.vcs_position_z = dets_z[idx];
               input_detections.detections[idx].processed.motion_status =
                  rspp::RSPP_Detection_Motion_Status_T::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
               input_detections.detections[idx].raw.rcs = rcs[idx];
            }
            else
            {
               input_detections.detections[idx].processed.vcs_position_z = cluster_centers_z[cluster_idx] + random_perturbation();
               input_detections.detections[idx].processed.motion_status =
                  rspp::RSPP_Detection_Motion_Status_T::RSPP_DETECTION_MOTION_STATUS_MOVING;
               input_detections.detections[idx].raw.rcs = random_float(-20.0F, 20.0F);
            }
            input_detections.detections[idx].raw.confid_azimuth = static_cast<int8_t>(rand() % 4U);
            input_detections.detections[idx].raw.confid_azimuth = static_cast<int8_t>(rand() % 4U);
         }
         const std::array<int16_t, SG_MAX_NUM_INPUT_DETS> sorted_det_indices{
            18, 0, 12, 6, 24, 23, 1, 19, 13, 7, 25, 8, 14, 2, 20, 21, 26, 17, 3, 15, 16, 29, 11, 5, 4, 28, 9, 27, 22, 10};
         input_detections.vcslong_det_idx_min                       = 18;
         input_detections.vcslong_det_idx_max                       = 10;
         input_detections.detections[18U].processed.prev_sorted_idx = -1;
         input_detections.detections[18U].processed.next_sorted_idx = 0;
         input_detections.detections[10U].processed.prev_sorted_idx = 22;
         input_detections.detections[10U].processed.next_sorted_idx = -1;
         for (uint8_t idx{1U}; idx < num_of_dets - 1U; ++idx)
         {
            const int16_t det_idx                                          = sorted_det_indices[idx];
            input_detections.detections[det_idx].processed.prev_sorted_idx = sorted_det_indices[idx - 1U];
            input_detections.detections[det_idx].processed.next_sorted_idx = sorted_det_indices[idx + 1U];
         }
      }

      void fill_fused_contours_with_dummy_data(FusedContourStorage &fused_contours)
      {
         fused_contours.clear();

         srand(123U); // set constant seed for easier debugging
         const uint16_t num_fused_contours{5U};
         const uint16_t num_fused_vertices{18U};
         assert(num_fused_contours < SG_MAX_NUM_FUSED_CONTOURS);

         const float vertex_x[num_fused_vertices] = {2.9F,  2.1F,  1.1F,  5.09F,  5.02F,  5.17F,  5.35F,  6.04F,  4.97F,
                                                     6.04F, 7.08F, 8.07F, 15.01F, 15.12F, 15.23F, 25.01F, 25.12F, 25.23F};
         const float vertex_y[num_fused_vertices] = {0.32F, 0.16F, 0.11F, 5.09F,  5.98F,  6.97F,  7.11F,   7.57F,   0.02F,
                                                     0.08F, 0.15F, 0.09F, 15.03F, 16.05F, 17.07F, -25.09F, -26.08F, -27.06F};

         const uint16_t first_vertex_indices[num_fused_contours]                   = {3U, 8U, 12U, 15U, 18U};
         const SG_Drivability_Class_T fused_vertex_drivability[num_fused_vertices] = {
            SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE,
            SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::OVERDRIVABLE,
            SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::OVERDRIVABLE,  SG_Drivability_Class_T::UNDERDRIVABLE,
            SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE,
            SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE, SG_Drivability_Class_T::UNDERDRIVABLE,
            SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE,   SG_Drivability_Class_T::NONDRIVABLE};

         const float fused_drivability_confidence[num_fused_vertices] = {75.0F, 80.0F, 85.0F, 75.0F, 80.0F, 85.0F,
                                                                         90.0F, 95.0F, 75.0F, 80.0F, 85.0F, 75.0F,
                                                                         80.0F, 85.0F, 90.0F, 95.0F, 75.0F, 80.0F};

         std::array<float, num_fused_vertices> covariance{};

         std::generate(covariance.begin(), covariance.end(), []() { return random_float(0.0F, 3.0F); });

         uint16_t total_num_of_vertices = 0U;

         for (auto idx = 0U; idx < num_fused_contours; ++idx)
         {
            Fused_Contour_T fused_contour;
            fused_contour.set_id(static_cast<uint32_t>(idx + 1U));
            Fused_Contour_T::FusedVertexList fused_vertices;

            uint16_t num_of_vertices = 0U;

            for (; total_num_of_vertices < first_vertex_indices[idx]; total_num_of_vertices++)
            {
               Fused_Vertex_T fused_vertex;

               fused_vertex.position.x         = vertex_x[total_num_of_vertices];
               fused_vertex.position.y         = vertex_y[total_num_of_vertices];
               fused_vertex.pos_cross_cov.x1x2 = covariance[total_num_of_vertices];
               fused_vertex.pos_cross_cov.x1y2 = covariance[total_num_of_vertices] + random_float(0.0F, 0.5F);
               fused_vertex.pos_cross_cov.y1x2 = covariance[total_num_of_vertices] + random_float(0.0F, 0.5F);
               fused_vertex.pos_cross_cov.y1y2 = covariance[total_num_of_vertices] + random_float(0.0F, 0.5F);
               fused_vertex.pos_cov.x          = covariance[total_num_of_vertices] + random_float(0.0F, 0.5F);
               fused_vertex.pos_cov.xy         = covariance[total_num_of_vertices] + random_float(0.0F, 0.5F);
               fused_vertex.pos_cov.y          = covariance[total_num_of_vertices] + random_float(0.0F, 0.5F);

               fused_vertex.drivability            = fused_vertex_drivability[total_num_of_vertices];
               fused_vertex.drivability_confidence = fused_drivability_confidence[total_num_of_vertices];

               (void) fused_contour.vertices.push_back(fused_vertex);

               num_of_vertices++;
            }
            fused_contour.num_of_vertices       = num_of_vertices;
            fused_contour.f_selected_for_output = true;
            fused_contours.push_back(std::move(fused_contour));
         }
      }
   }
}