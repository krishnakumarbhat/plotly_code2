#include "sg_dummy_generator.h"

#include <cassert>
#include <cmath>
#include <random>

#include "sg_calibrations.h"
#include "sg_determine_drivability_class.h"

namespace sg
{
   // TODO (https://jiraprod.aptiv.com/browse/FZD-223) remove unneded functions
   void fill_detections_with_dummy_data(DetectionStorage &sg_det_storage, const Calibrations_T &calibrations)
   {
      sg_det_storage.clear();

      srand(123U); // set constant seed for easier debugging
      std::random_device rd;
      std::mt19937 g(rd());
      const uint32_t num_dets                = 100U;
      const uint32_t num_unclustered_dets    = 20U;
      const uint32_t num_random_cluster_dets = 50U;
      const uint16_t num_clusters            = 6U;
      assert(num_clusters < 10U);
      std::array<uint16_t, 10U> cluster_ids = {1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U};
      std::shuffle(cluster_ids.begin(), cluster_ids.end(), g);
      cluster_ids[0U] = 0U; // for unclustered detections
      std::array<geometry::Point3D_T, num_clusters + 1U> cluster_centers{};
      cluster_centers[0U] = geometry::Point3D_T({0.0F, 0.0F, 0.0F}); // for unclustered detections
      for (uint16_t cluster_idx = 1U; cluster_idx <= num_clusters; cluster_idx++)
      {
         const float x = static_cast<float>(rand() % 50U) + 10.0F;
         const float y = static_cast<float>(rand() % 40U) - 20.0F;
         float z       = static_cast<float>(rand() % 40U) / 10.0F - 2.0F;
         if (cluster_idx % 3 == 0U) // make z within underdrivable region
         {
            z = std::copysign((std::fabs(z) + 8.0F), z);
         }
         cluster_centers[cluster_idx] = geometry::Point3D_T({x, y, z});
      }

      for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx++)
      {
         uint16_t cluster_idx = static_cast<uint16_t>(det_idx) % num_clusters + 1U;
         if (det_idx < num_unclustered_dets)
         {
            cluster_idx = 0U;
         }
         else if (det_idx >= num_dets - num_random_cluster_dets)
         {
            cluster_idx = static_cast<uint16_t>(rand() % num_clusters) + 1U;
         }
         auto sg_det = Detection_T{};
         fill_single_det(sg_det, det_idx, calibrations, cluster_centers[cluster_idx], cluster_ids[cluster_idx]);
         const auto result = sg_det_storage.push_back(sg_det);
         (void) result;
         assert(result != nullptr); // MISRA: result of operation can only be checked in Debug mode
      }
   }

   void fill_single_det(Detection_T &sg_det,
                        const uint32_t det_idx,
                        const Calibrations_T &calibrations,
                        const geometry::Point3D_T &cluster_center,
                        const uint16_t cluster_id)
   {
      if (cluster_id == INVALID_CLUSTER_ID)
      {
         sg_det.position.x = static_cast<float>(rand() % 90U) - 10.0F;
         sg_det.position.y = static_cast<float>(rand() % 80U) - 40.0F;
         sg_det.position.z = static_cast<float>(rand() % 30U) - 15.0F;
      }
      else
      {
         sg_det.position.x = cluster_center.x + static_cast<float>(rand() % 40U) - 20.0F;
         sg_det.position.y = cluster_center.y + static_cast<float>(rand() % 40U) - 20.0F;
         sg_det.position.z = cluster_center.z + static_cast<float>(rand() % 40U) / 40.0F - 1.0F;
      }
      sg_det.position_cov.x      = static_cast<float>(rand() % 2U) - 1.0F;
      sg_det.position_cov.y      = static_cast<float>(rand() % 2U) - 1.0F;
      sg_det.position_cov.xy     = static_cast<float>(rand() % 2U) - 1.0F;
      sg_det.position_squeezed.x = sg_det.position.x * ((static_cast<float>(rand() % 5U) + 5.0F) / 10.0F);
      sg_det.position_squeezed.y = sg_det.position.x * ((static_cast<float>(rand() % 5U) + 5.0F) / 10.0F);
      sg_det.position_squeezed.z = sg_det.position.z;

      sg_det.existence_probability    = static_cast<float>(rand() % 100U) / 100.0F;
      sg_det.probability_of_detection = static_cast<float>(rand() % 100U) / 100.0F;
      sg_det.range_rate_compensated   = static_cast<float>(rand() % 2U) - 1.0F;
      sg_det.importance               = static_cast<float>(rand() % 100U) / 100.0F;
      sg_det.current_num_neighbors    = static_cast<float>(rand() % 5U);
      sg_det.cumulated_num_neighbors  = static_cast<float>(rand() % 5U);

      sg_det.unique_id      = det_idx + 1U;
      sg_det.contour_id     = 1U;
      sg_det.segment_id[0U] = static_cast<uint16_t>(rand() % 20U);
      sg_det.segment_id[1U] = 0U;
      sg_det.cluster_id     = cluster_id;
      sg_det.age            = static_cast<uint16_t>((rand() % 5U) + 1U);

      sg_det.drivability = sg_determine_drivability_class(calibrations, sg_det.position);

      sg_det.f_dbscan_core                = true;
      sg_det.f_dbscan_visited             = true;
      sg_det.f_used_in_measurement_update = true;
      sg_det.f_valid                      = true;
   }

   // TODO (https://jiraprod.aptiv.com/browse/FZD-223) remove unneded functions
   void fill_contours_with_dummy_data(ContourStorage &sg_contours)
   {
      sg_contours.clear();

      auto vertex11 = Vertex_T{{3.0F, 0.3F}};
      auto vertex12 = Vertex_T{{2.0F, 0.2F}};
      auto vertex13 = Vertex_T{{1.0F, 0.1F}};

      vertex11.pos_cov.x            = 1.11F;
      vertex11.pos_cov.y            = 1.12F;
      vertex11.pos_cov.xy           = 1.13F;
      vertex11.pos_cross_cov.x1x2   = 1.14F;
      vertex11.pos_cross_cov.x1y2   = 1.15F;
      vertex11.pos_cross_cov.y1x2   = 1.16F;
      vertex11.pos_cross_cov.y1y2   = 1.17F;
      vertex11.reliability          = 1.18F;
      vertex11.segment_id           = 1U;
      vertex11.age                  = 11U;
      vertex11.num_cycles_no_update = 1U;
      vertex11.f_critical           = true;

      vertex12.pos_cov.x            = 1.21F;
      vertex12.pos_cov.y            = 1.22F;
      vertex12.pos_cov.xy           = 1.23F;
      vertex12.pos_cross_cov.x1x2   = 1.24F;
      vertex12.pos_cross_cov.x1y2   = 1.25F;
      vertex12.pos_cross_cov.y1x2   = 1.26F;
      vertex12.pos_cross_cov.y1y2   = 1.27F;
      vertex12.reliability          = 1.28F;
      vertex12.segment_id           = 2U;
      vertex12.age                  = 12U;
      vertex12.num_cycles_no_update = 2U;
      vertex12.f_critical           = true;

      vertex13.pos_cov.x            = 1.31F;
      vertex13.pos_cov.y            = 1.32F;
      vertex13.pos_cov.xy           = 1.33F;
      vertex13.pos_cross_cov.x1x2   = 1.34F;
      vertex13.pos_cross_cov.x1y2   = 1.35F;
      vertex13.pos_cross_cov.y1x2   = 1.36F;
      vertex13.pos_cross_cov.y1y2   = 1.37F;
      vertex13.reliability          = 1.38F;
      vertex13.segment_id           = 0U;
      vertex13.age                  = 13U;
      vertex13.num_cycles_no_update = 3U;
      vertex13.f_critical           = true;

      auto vertex21 = Vertex_T{{5.0F, 0.1F}};
      auto vertex22 = Vertex_T{{6.0F, 0.1F}};
      auto vertex23 = Vertex_T{{7.0F, 0.1F}};
      auto vertex24 = Vertex_T{{8.0F, 0.1F}};

      vertex21.pos_cov.x            = 2.11F;
      vertex21.pos_cov.y            = 2.12F;
      vertex21.pos_cov.xy           = 2.13F;
      vertex21.pos_cross_cov.x1x2   = 2.14F;
      vertex21.pos_cross_cov.x1y2   = 2.15F;
      vertex21.pos_cross_cov.y1x2   = 2.16F;
      vertex21.pos_cross_cov.y1y2   = 2.17F;
      vertex21.reliability          = 2.18F;
      vertex21.segment_id           = 3U;
      vertex21.age                  = 21U;
      vertex21.num_cycles_no_update = 1U;
      vertex21.f_critical           = true;

      vertex22.pos_cov.x            = 2.21F;
      vertex22.pos_cov.y            = 2.22F;
      vertex22.pos_cov.xy           = 2.23F;
      vertex22.pos_cross_cov.x1x2   = 2.24F;
      vertex22.pos_cross_cov.x1y2   = 2.25F;
      vertex22.pos_cross_cov.y1x2   = 2.26F;
      vertex22.pos_cross_cov.y1y2   = 2.27F;
      vertex22.reliability          = 4.28F;
      vertex22.segment_id           = 4U;
      vertex22.age                  = 22U;
      vertex22.num_cycles_no_update = 2U;
      vertex22.f_critical           = true;

      vertex23.pos_cov.x            = 2.31F;
      vertex23.pos_cov.y            = 2.32F;
      vertex23.pos_cov.xy           = 2.33F;
      vertex23.pos_cross_cov.x1x2   = 2.34F;
      vertex23.pos_cross_cov.x1y2   = 2.35F;
      vertex23.pos_cross_cov.y1x2   = 2.36F;
      vertex23.pos_cross_cov.y1y2   = 2.37F;
      vertex23.reliability          = 2.38F;
      vertex23.segment_id           = 5U;
      vertex23.age                  = 23U;
      vertex23.num_cycles_no_update = 3U;
      vertex23.f_critical           = true;

      vertex24.pos_cov.x            = 2.41F;
      vertex24.pos_cov.y            = 2.42F;
      vertex24.pos_cov.xy           = 2.43F;
      vertex24.pos_cross_cov.x1x2   = 2.44F;
      vertex24.pos_cross_cov.x1y2   = 2.45F;
      vertex24.pos_cross_cov.y1x2   = 2.46F;
      vertex24.pos_cross_cov.y1y2   = 2.47F;
      vertex24.reliability          = 2.48F;
      vertex24.segment_id           = 0U;
      vertex24.age                  = 24U;
      vertex24.num_cycles_no_update = 4U;
      vertex24.f_critical           = true;

      auto vertex31 = Vertex_T{{5.0F, 5.0F}};
      auto vertex32 = Vertex_T{{5.1F, 6.0F}};
      auto vertex33 = Vertex_T{{5.2F, 7.0F}};
      auto vertex34 = Vertex_T{{5.3F, 7.1F}};
      auto vertex35 = Vertex_T{{6.0F, 7.5F}};

      vertex31.pos_cov.x            = 3.11F;
      vertex31.pos_cov.y            = 3.12F;
      vertex31.pos_cov.xy           = 3.13F;
      vertex31.pos_cross_cov.x1x2   = 3.14F;
      vertex31.pos_cross_cov.x1y2   = 3.15F;
      vertex31.pos_cross_cov.y1x2   = 3.16F;
      vertex31.pos_cross_cov.y1y2   = 3.17F;
      vertex31.reliability          = 3.18F;
      vertex31.segment_id           = 6U;
      vertex31.age                  = 31U;
      vertex31.num_cycles_no_update = 1U;
      vertex31.f_critical           = true;

      vertex32.pos_cov.x            = 3.21F;
      vertex32.pos_cov.y            = 3.22F;
      vertex32.pos_cov.xy           = 3.23F;
      vertex32.pos_cross_cov.x1x2   = 3.24F;
      vertex32.pos_cross_cov.x1y2   = 3.25F;
      vertex32.pos_cross_cov.y1x2   = 3.26F;
      vertex32.pos_cross_cov.y1y2   = 3.27F;
      vertex32.reliability          = 3.28F;
      vertex32.segment_id           = 7U;
      vertex32.age                  = 32U;
      vertex32.num_cycles_no_update = 2U;
      vertex32.f_critical           = true;

      vertex33.pos_cov.x            = 3.31F;
      vertex33.pos_cov.y            = 3.32F;
      vertex33.pos_cov.xy           = 3.33F;
      vertex33.pos_cross_cov.x1x2   = 3.34F;
      vertex33.pos_cross_cov.x1y2   = 3.35F;
      vertex33.pos_cross_cov.y1x2   = 3.36F;
      vertex33.pos_cross_cov.y1y2   = 3.37F;
      vertex33.reliability          = 3.38F;
      vertex33.segment_id           = 8U;
      vertex33.age                  = 33U;
      vertex33.num_cycles_no_update = 3U;
      vertex33.f_critical           = true;

      vertex34.pos_cov.x            = 3.41F;
      vertex34.pos_cov.y            = 3.42F;
      vertex34.pos_cov.xy           = 3.43F;
      vertex34.pos_cross_cov.x1x2   = 3.44F;
      vertex34.pos_cross_cov.x1y2   = 3.45F;
      vertex34.pos_cross_cov.y1x2   = 3.46F;
      vertex34.pos_cross_cov.y1y2   = 3.47F;
      vertex34.reliability          = 3.48F;
      vertex34.segment_id           = 9U;
      vertex34.age                  = 34U;
      vertex34.num_cycles_no_update = 4U;
      vertex34.f_critical           = true;

      vertex35.pos_cov.x            = 3.51F;
      vertex35.pos_cov.y            = 3.52F;
      vertex35.pos_cov.xy           = 3.53F;
      vertex35.pos_cross_cov.x1x2   = 3.54F;
      vertex35.pos_cross_cov.x1y2   = 3.55F;
      vertex35.pos_cross_cov.y1x2   = 3.56F;
      vertex35.pos_cross_cov.y1y2   = 3.57F;
      vertex35.reliability          = 3.58F;
      vertex35.segment_id           = 0U;
      vertex35.age                  = 35U;
      vertex35.num_cycles_no_update = 5U;
      vertex35.f_critical           = true;

      auto vertex41 = Vertex_T{{15.0F, 15.0F}};
      auto vertex42 = Vertex_T{{15.1F, 16.0F}};
      auto vertex43 = Vertex_T{{15.2F, 17.0F}};

      vertex41.pos_cov.x            = 3.11F;
      vertex41.pos_cov.y            = 3.12F;
      vertex41.pos_cov.xy           = 3.13F;
      vertex41.pos_cross_cov.x1x2   = 3.14F;
      vertex41.pos_cross_cov.x1y2   = 3.15F;
      vertex41.pos_cross_cov.y1x2   = 3.16F;
      vertex41.pos_cross_cov.y1y2   = 3.17F;
      vertex41.reliability          = 3.18F;
      vertex41.segment_id           = 10U;
      vertex41.age                  = 31U;
      vertex41.num_cycles_no_update = 1U;
      vertex41.f_critical           = true;

      vertex42.pos_cov.x            = 3.21F;
      vertex42.pos_cov.y            = 3.22F;
      vertex42.pos_cov.xy           = 3.23F;
      vertex42.pos_cross_cov.x1x2   = 3.24F;
      vertex42.pos_cross_cov.x1y2   = 3.25F;
      vertex42.pos_cross_cov.y1x2   = 3.26F;
      vertex42.pos_cross_cov.y1y2   = 3.27F;
      vertex42.reliability          = 3.28F;
      vertex42.segment_id           = 11U;
      vertex42.age                  = 32U;
      vertex42.num_cycles_no_update = 2U;
      vertex42.f_critical           = true;

      vertex43.pos_cov.x            = 3.31F;
      vertex43.pos_cov.y            = 3.32F;
      vertex43.pos_cov.xy           = 3.33F;
      vertex43.pos_cross_cov.x1x2   = 3.34F;
      vertex43.pos_cross_cov.x1y2   = 3.35F;
      vertex43.pos_cross_cov.y1x2   = 3.36F;
      vertex43.pos_cross_cov.y1y2   = 3.37F;
      vertex43.reliability          = 3.38F;
      vertex43.segment_id           = 0U;
      vertex43.age                  = 33U;
      vertex43.num_cycles_no_update = 3U;
      vertex43.f_critical           = true;

      auto vertex51 = Vertex_T{{25.0F, -25.0F}};
      auto vertex52 = Vertex_T{{25.1F, -26.0F}};
      auto vertex53 = Vertex_T{{25.2F, -27.0F}};

      vertex51.pos_cov.x            = 3.11F;
      vertex51.pos_cov.y            = 3.12F;
      vertex51.pos_cov.xy           = 3.13F;
      vertex51.pos_cross_cov.x1x2   = 3.14F;
      vertex51.pos_cross_cov.x1y2   = 3.15F;
      vertex51.pos_cross_cov.y1x2   = 3.16F;
      vertex51.pos_cross_cov.y1y2   = 3.17F;
      vertex51.reliability          = 3.18F;
      vertex51.segment_id           = 12U;
      vertex51.age                  = 31U;
      vertex51.num_cycles_no_update = 1U;
      vertex51.f_critical           = true;

      vertex52.pos_cov.x            = 3.21F;
      vertex52.pos_cov.y            = 3.22F;
      vertex52.pos_cov.xy           = 3.23F;
      vertex52.pos_cross_cov.x1x2   = 3.24F;
      vertex52.pos_cross_cov.x1y2   = 3.25F;
      vertex52.pos_cross_cov.y1x2   = 3.26F;
      vertex52.pos_cross_cov.y1y2   = 3.27F;
      vertex52.reliability          = 3.28F;
      vertex52.segment_id           = 13U;
      vertex52.age                  = 32U;
      vertex52.num_cycles_no_update = 2U;
      vertex52.f_critical           = true;

      vertex53.pos_cov.x            = 3.31F;
      vertex53.pos_cov.y            = 3.32F;
      vertex53.pos_cov.xy           = 3.33F;
      vertex53.pos_cross_cov.x1x2   = 3.34F;
      vertex53.pos_cross_cov.x1y2   = 3.35F;
      vertex53.pos_cross_cov.y1x2   = 3.36F;
      vertex53.pos_cross_cov.y1y2   = 3.37F;
      vertex53.reliability          = 3.38F;
      vertex53.segment_id           = 0U;
      vertex53.age                  = 33U;
      vertex53.num_cycles_no_update = 3U;
      vertex53.f_critical           = true;

      // first cluster
      uint16_t cluster1_id = 2U;
      auto contour1        = Contour_T{vertex11, vertex12, vertex13};
      contour1.cluster_id  = cluster1_id;
      contour1.drivability = SG_Drivability_Class_T::NONDRIVABLE;

      auto contour2        = Contour_T{vertex31, vertex32, vertex33, vertex34, vertex35};
      contour2.cluster_id  = cluster1_id;
      contour2.drivability = SG_Drivability_Class_T::NONDRIVABLE;

      // second cluster
      uint16_t cluster2_id = cluster1_id + 1U;
      auto contour3        = Contour_T{vertex21, vertex22, vertex23, vertex24};
      contour3.cluster_id  = cluster2_id;
      contour3.drivability = SG_Drivability_Class_T::UNDERDRIVABLE;

      auto contour4        = Contour_T{vertex41, vertex42, vertex43};
      contour4.cluster_id  = cluster2_id;
      contour4.drivability = SG_Drivability_Class_T::UNDERDRIVABLE;

      // third cluster
      uint16_t cluster3_id = cluster1_id - 1U;
      auto contour5        = Contour_T{vertex51, vertex52, vertex53};
      contour5.cluster_id  = cluster3_id;
      contour5.drivability = SG_Drivability_Class_T::NONDRIVABLE;

      ContourStorage::ContourList::iterator add_contour_result = nullptr;
      add_contour_result                                       = sg_contours.push_back(std::move(contour1));
      assert(add_contour_result != nullptr);
      add_contour_result = sg_contours.push_back(std::move(contour2));
      assert(add_contour_result != nullptr);
      add_contour_result = sg_contours.push_back(std::move(contour3));
      assert(add_contour_result != nullptr);
      add_contour_result = sg_contours.push_back(std::move(contour4));
      assert(add_contour_result != nullptr);
      add_contour_result = sg_contours.push_back(std::move(contour5));
      assert(add_contour_result != nullptr);
   }

   void fill_host_with_dummy_data(RSPP_Host_T &rspp_host)
   {
      rspp_host.vehicle_index             = 1U;
      rspp_host.speed                     = 50.0F;
      rspp_host.vcs_speed                 = 30.0F;
      rspp_host.acceleration              = 5.0F;
      rspp_host.vcs_lat_acceleration      = 0.0F;
      rspp_host.vcs_long_acceleration     = 5.0F;
      rspp_host.yaw_rate_rad              = 0.01F;
      rspp_host.vcs_sideslip              = 0.01F;
      rspp_host.curvature_rear            = 0.001F;
      rspp_host.dist_rear_axle_to_vcs_m   = 3.5F;
      rspp_host.rear_cornering_compliance = 1.0F;
      rspp_host.speed_correction_factor   = 1.0F;
      rspp_host.speed_qf                  = 3U;
      rspp_host.yaw_rate_qf               = 3U;
      rspp_host.lat_accel_qf              = 3U;
      rspp_host.long_accel_qf             = 3U;
   }

   void fill_host_props_with_dummy_data(const float elapsed_time, const RSPP_Host_T &rspp_host, sg::HostProps &host_properties)
   {
      calculate_host_properties(elapsed_time, rspp_host, host_properties);
   }

   void fill_input_detections_with_dummy_data(SG_Input_Detections_T &detection_list)
   {
      srand(123U); // set constant seed for easier debugging

      const std::size_t number_of_input_detections = 100U;
      assert(number_of_input_detections <= rspp::MAX_NUMBER_OF_DETECTIONS);
      detection_list.number_of_valid_detections = number_of_input_detections;

      for (std::size_t idx = 0U; idx < detection_list.number_of_valid_detections; idx++)
      {
         detection_list.detections[idx].processed.vcs_position_x = static_cast<float>(idx + 100U);
         detection_list.detections[idx].processed.vcs_position_y = static_cast<float>(rand() % 80U) - 40.0F;
         detection_list.detections[idx].processed.vcs_position_z = -static_cast<float>(rand() % 4U) - 2.0F;
         // range rate [0.0 .. 3.0] [m/s]
         constexpr float range_rate_lower_bound = 0.0F;
         constexpr float range_rate_upper_bound = 3.0F;
         constexpr float range_rate_divisor     = static_cast<float>(RAND_MAX) / (range_rate_upper_bound - range_rate_lower_bound);
         const float range_rate                 = range_rate_lower_bound + static_cast<float>(rand()) / range_rate_divisor;
         detection_list.detections[idx].processed.range_rate_compensated = range_rate;
         detection_list.detections[idx].processed.motion_status =
            rspp::RSPP_Detection_Motion_Status_T::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
         detection_list.detections[idx].raw.range = static_cast<float>(rand() % 300U);
         // elevation angle range [-1.3 .. 0.09] [rad] => ~[-74 .. 5] [deg]
         constexpr float elevation_lower_bound = -1.3F;
         constexpr float elevation_upper_bound = 0.09F;
         constexpr float elevation_divisor     = static_cast<float>(RAND_MAX) / (elevation_upper_bound - elevation_lower_bound);
         const float elevation_angle           = elevation_lower_bound + static_cast<float>(rand()) / elevation_divisor;
         detection_list.detections[idx].raw.elevation = elevation_angle;
         detection_list.detections[idx].raw.det_id    = static_cast<int32_t>(idx + 1U);
         detection_list.detections[idx].raw.sensor_id = 1;
      }
   }

   void fill_input_sensors_with_dummy_data(sg::rspp::F360_Radar_Sensor_T (&sensors)[sg::rspp::MAX_NUMBER_OF_SENSORS])
   {
      sensors[0].variable.look_id = static_cast<RSPP_Det_Look_ID_T>(0);
   }
}
